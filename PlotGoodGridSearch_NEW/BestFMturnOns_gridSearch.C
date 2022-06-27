#include <TROOT.h>
#include <TFile.h>
#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <sstream>
#include <TBranchElement.h>
#include <TGraphAsymmErrors.h>
#include <TVector.h>
#include <fstream>
#include <map>

using namespace std;

float figureOfMerit(TGraphAsymmErrors* turnon, float threshold) {
    int upperLim = 120; // uper limit of the integral of the curve
    int demarcation = threshold;

    Double_t x1, x2, y1, y2;
    float lowerIntegral = 0.0;
    float upperIntegral = 0.0;

    for (int i = 0; i < turnon->GetN(); ++i) {
        turnon->GetPoint(i, x1, y1);
        turnon->GetPoint(i+1, x2, y2);
        if (x2 > upperLim) break; // stop calculating the integrals at the upper limit

        // calculate binned rectangle+triangle approximation of inegral
        if (x1 < demarcation && x2 < demarcation) lowerIntegral += (x2-x1)*y1 + 0.5*(x2-x1)*(y2-y1);
        else if (x1 < demarcation && x2 > demarcation) {
            float y3 = (y2-y1)/(x2-x1) * demarcation + (x1*y2-x2*y1)/(x2-x1);
            lowerIntegral += (demarcation-x1)*y1 + 0.5*(demarcation-x1)*(y3-y1);
            upperIntegral += (x2-demarcation)*y3 + 0.5*(x2-demarcation)*(y2-y3);
        }
        else upperIntegral += (x2-x1)*y1 + 0.5*(x2-x1)*(y2-y1);
    }

    return upperIntegral - lowerIntegral; // - threshold;
}

bool has100Plateau(TGraphAsymmErrors* turnon) {
    Double_t x, y;
    float point95check = 75; // 75GeV
    float point98check = 95; // 95GeV
    bool pass95check = false;
    bool pass98check = false;
    bool has100Plateau = false;

    for (int i = 0; i < turnon->GetN(); ++i) {
        turnon->GetPoint(i, x, y);

        if (x >= point95check && x <= point98check && y >= 0.95) pass95check = true;
        if (x >= point98check) {
            if(y >= 0.98) pass98check = true; 
            break;
        }
    }

    if (pass95check && pass98check) has100Plateau = true;

    return has100Plateau;
    //return true;
}

bool has0effAt0pt(TGraphAsymmErrors* turnon) {
    bool has0effAt0pt = false;

    Double_t x, y;
    turnon->GetPoint(0, x, y);
    if (y<=0.05) has0effAt0pt = true;

    return has0effAt0pt;

}

void isBetterFM(TString FMtype, std::vector< std::pair<TString, std::pair<float,std::pair<float,float>>> > &fm_ordered, float newFM, float thr, float acc,  TString name) {
    for (long unsigned int i = 0; i < fm_ordered.size(); ++i)
    {
        if (FMtype == "FM") {
            if (newFM >= fm_ordered[i].second.second.first) {
                fm_ordered.insert( fm_ordered.begin()+i, std::pair(name, std::pair(thr,std::pair(newFM,acc))) );
                break;
            }
        }
        else if (FMtype == "FMtACC") {
            if (newFM*acc >= fm_ordered[i].second.second.first*fm_ordered[i].second.second.second) {
                fm_ordered.insert( fm_ordered.begin()+i, std::pair(name, std::pair(thr,std::pair(newFM,acc))) );
                break;
            }
        }        
    }
}

void compare(TString FMtype, int targetRate = 14, int fixedThr = 0, TString parametrisation = "linear", Double_t Kfact = 0.0, int run=-1, float calibThr = 1.7) {
    TString run_str = to_string(run);

    TString fixedRate = to_string(targetRate);
    TString fixedThreshold = to_string(fixedThr);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    TString Kintgr = to_string(Kfact).substr(0, to_string(Kfact).find("."));
    TString Kdecim = to_string(Kfact).substr(2, to_string(Kfact).find("."));
    
    gStyle->SetOptStat(000000);

    int minThrAllowed = 28;
    int maxThrAllowed = 38;

    std::vector<TString> tags = {"effMin0p0", "effMin0p1", "effMin0p2", "effMin0p3", "effMin0p4", "effMin0p5", "effMin0p5", "effMin0p6", "effMin0p7", "effMin0p8", "effMin0p9"};
    std::vector<TString> opts = {"0", "1", "2", "3", "4", "5", "6", "7", "8"};

    std::vector< std::pair<TString, std::pair<float,std::pair<float,float>>> > FM_ordered = { std::pair("init", std::pair(0,std::pair(0,0))) };

    TString fileName0 = "/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS";
    if (fixedThr==0) fileName0 += "_FIXEDRATE"+fixedRate+"kHz";
    else             fileName0 += "_FIXEDTHR"+fixedThreshold+"GeV";
    if (run==-1)     fileName0 += "_SingleNeutrino122X";
    else             fileName0 += "_Run"+run_str;
    if(parametrisation=="linear") fileName0 += "_gs_linear_effMin0p0_0_2022_06_13.root";
    else                          fileName0 += "_gs_"+parametrisation+Kintgr+"p"+Kdecim+"_effMin0p0_0_2022_06_13.root";

    TFile* f0;
    f0 = new TFile(fileName0.Data(), "READ");

    TGraphAsymmErrors* turnon_NewLayer1_Option1_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option1_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option2_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option2_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option3_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option3_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option4_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option4_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option5_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option5_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option6_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option6_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option7_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option7_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option8_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option8_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option9_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option9_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option10_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option10_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option11_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option11_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option12_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option12_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option13_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option13_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option14_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option14_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option15_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option15_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option16_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option16_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option17_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option17_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option18_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option18_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option19_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option19_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option20_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option20_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option21_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option21_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option22_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option22_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option23_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option23_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option24_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option24_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option25_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option25_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option26_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option26_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option27_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option27_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option28_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option28_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option29_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option29_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option30_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option30_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option31_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option31_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option31_extrap_mean = (TGraphAsymmErrors*)f0->Get("divide_pt_pass_Option31_extrap_by_pt");
    
    int thrInt_Option1  = int( ( (TVectorT<float>*)f0->Get("thr_Option1") )[0][0]) +1;
    int thrInt_Option2  = int( ( (TVectorT<float>*)f0->Get("thr_Option2") )[0][0]) +1;
    int thrInt_Option3  = int( ( (TVectorT<float>*)f0->Get("thr_Option3") )[0][0]) +1;
    int thrInt_Option4  = int( ( (TVectorT<float>*)f0->Get("thr_Option4") )[0][0]) +1;
    int thrInt_Option5  = int( ( (TVectorT<float>*)f0->Get("thr_Option5") )[0][0]) +1;
    int thrInt_Option6  = int( ( (TVectorT<float>*)f0->Get("thr_Option6") )[0][0]) +1;
    int thrInt_Option7  = int( ( (TVectorT<float>*)f0->Get("thr_Option7") )[0][0]) +1;
    int thrInt_Option8  = int( ( (TVectorT<float>*)f0->Get("thr_Option8") )[0][0]) +1;
    int thrInt_Option9  = int( ( (TVectorT<float>*)f0->Get("thr_Option9") )[0][0]) +1;
    int thrInt_Option10 = int( ( (TVectorT<float>*)f0->Get("thr_Option10") )[0][0]) +1;
    int thrInt_Option11 = int( ( (TVectorT<float>*)f0->Get("thr_Option11") )[0][0]) +1;
    int thrInt_Option12 = int( ( (TVectorT<float>*)f0->Get("thr_Option12") )[0][0]) +1;
    int thrInt_Option13 = int( ( (TVectorT<float>*)f0->Get("thr_Option13") )[0][0]) +1;
    int thrInt_Option14 = int( ( (TVectorT<float>*)f0->Get("thr_Option14") )[0][0]) +1;
    int thrInt_Option15 = int( ( (TVectorT<float>*)f0->Get("thr_Option15") )[0][0]) +1;
    int thrInt_Option16 = int( ( (TVectorT<float>*)f0->Get("thr_Option16") )[0][0]) +1;
    int thrInt_Option17 = int( ( (TVectorT<float>*)f0->Get("thr_Option17") )[0][0]) +1;
    int thrInt_Option18 = int( ( (TVectorT<float>*)f0->Get("thr_Option18") )[0][0]) +1;
    int thrInt_Option19 = int( ( (TVectorT<float>*)f0->Get("thr_Option19") )[0][0]) +1;
    int thrInt_Option20 = int( ( (TVectorT<float>*)f0->Get("thr_Option20") )[0][0]) +1;
    int thrInt_Option21 = int( ( (TVectorT<float>*)f0->Get("thr_Option21") )[0][0]) +1;
    int thrInt_Option22 = int( ( (TVectorT<float>*)f0->Get("thr_Option22") )[0][0]) +1;
    int thrInt_Option23 = int( ( (TVectorT<float>*)f0->Get("thr_Option23") )[0][0]) +1;
    int thrInt_Option24 = int( ( (TVectorT<float>*)f0->Get("thr_Option24") )[0][0]) +1;
    int thrInt_Option25 = int( ( (TVectorT<float>*)f0->Get("thr_Option25") )[0][0]) +1;
    int thrInt_Option26 = int( ( (TVectorT<float>*)f0->Get("thr_Option26") )[0][0]) +1;
    int thrInt_Option27 = int( ( (TVectorT<float>*)f0->Get("thr_Option27") )[0][0]) +1;
    int thrInt_Option28 = int( ( (TVectorT<float>*)f0->Get("thr_Option28") )[0][0]) +1;
    int thrInt_Option29 = int( ( (TVectorT<float>*)f0->Get("thr_Option29") )[0][0]) +1;
    int thrInt_Option30 = int( ( (TVectorT<float>*)f0->Get("thr_Option30") )[0][0]) +1;
    int thrInt_Option31 = int( ( (TVectorT<float>*)f0->Get("thr_Option31") )[0][0]) +1;
    int thrInt_Option31_extrap  = int( ( (TVectorT<float>*)f0->Get("thr_Option31_extrap") )[0][0]) +1;

    float acc_Option1  = -1.;
    float acc_Option2  = -1.;
    float acc_Option3  = -1.;
    float acc_Option4  = -1.;
    float acc_Option5  = -1.;
    float acc_Option6  = -1.;
    float acc_Option7  = -1.;
    float acc_Option8  = -1.;
    float acc_Option9  = -1.;
    float acc_Option10 = -1.;
    float acc_Option11 = -1.;
    float acc_Option12 = -1.;
    float acc_Option13 = -1.;
    float acc_Option14 = -1.;
    float acc_Option15 = -1.;
    float acc_Option16 = -1.;
    float acc_Option17 = -1.;
    float acc_Option18 = -1.;
    float acc_Option19 = -1.;
    float acc_Option20 = -1.;
    float acc_Option21 = -1.;
    float acc_Option22 = ( (TVectorT<float>*)f0->Get("acc_Option22") )[0][0];
    float acc_Option23 = -1.;
    float acc_Option24 = -1.;
    float acc_Option25 = -1.;
    float acc_Option26 = -1.;
    float acc_Option27 = -1.;
    float acc_Option28 = -1.;
    float acc_Option29 = -1.;
    float acc_Option30 = -1.;
    float acc_Option31 = -1.;
    float acc_Option31_extrap  = ( (TVectorT<float>*)f0->Get("acc_Option31_extrap") )[0][0];

    float fm_Option1  = figureOfMerit(turnon_NewLayer1_Option1_mean, thrInt_Option1);
    float fm_Option2  = figureOfMerit(turnon_NewLayer1_Option2_mean, thrInt_Option2);
    float fm_Option3  = figureOfMerit(turnon_NewLayer1_Option3_mean, thrInt_Option3);
    float fm_Option4  = figureOfMerit(turnon_NewLayer1_Option4_mean, thrInt_Option4);
    float fm_Option5  = figureOfMerit(turnon_NewLayer1_Option5_mean, thrInt_Option5);
    float fm_Option6  = figureOfMerit(turnon_NewLayer1_Option6_mean, thrInt_Option6);
    float fm_Option7  = figureOfMerit(turnon_NewLayer1_Option7_mean, thrInt_Option7);
    float fm_Option8  = figureOfMerit(turnon_NewLayer1_Option8_mean, thrInt_Option8);
    float fm_Option9  = figureOfMerit(turnon_NewLayer1_Option9_mean, thrInt_Option9);
    float fm_Option10 = figureOfMerit(turnon_NewLayer1_Option10_mean, thrInt_Option10);
    float fm_Option11 = figureOfMerit(turnon_NewLayer1_Option11_mean, thrInt_Option11);
    float fm_Option12 = figureOfMerit(turnon_NewLayer1_Option12_mean, thrInt_Option12);
    float fm_Option13 = figureOfMerit(turnon_NewLayer1_Option13_mean, thrInt_Option13);
    float fm_Option14 = figureOfMerit(turnon_NewLayer1_Option14_mean, thrInt_Option14);
    float fm_Option15 = figureOfMerit(turnon_NewLayer1_Option15_mean, thrInt_Option15);
    float fm_Option16 = figureOfMerit(turnon_NewLayer1_Option16_mean, thrInt_Option16);
    float fm_Option17 = figureOfMerit(turnon_NewLayer1_Option17_mean, thrInt_Option17);
    float fm_Option18 = figureOfMerit(turnon_NewLayer1_Option18_mean, thrInt_Option18);
    float fm_Option19 = figureOfMerit(turnon_NewLayer1_Option19_mean, thrInt_Option19);
    float fm_Option20 = figureOfMerit(turnon_NewLayer1_Option20_mean, thrInt_Option20);
    float fm_Option21 = figureOfMerit(turnon_NewLayer1_Option21_mean, thrInt_Option21);
    float fm_Option22 = figureOfMerit(turnon_NewLayer1_Option22_mean, thrInt_Option22);
    float fm_Option23 = figureOfMerit(turnon_NewLayer1_Option23_mean, thrInt_Option23);
    float fm_Option24 = figureOfMerit(turnon_NewLayer1_Option24_mean, thrInt_Option24);
    float fm_Option25 = figureOfMerit(turnon_NewLayer1_Option25_mean, thrInt_Option25);
    float fm_Option26 = figureOfMerit(turnon_NewLayer1_Option26_mean, thrInt_Option26);
    float fm_Option27 = figureOfMerit(turnon_NewLayer1_Option27_mean, thrInt_Option27);
    float fm_Option28 = figureOfMerit(turnon_NewLayer1_Option28_mean, thrInt_Option28);
    float fm_Option29 = figureOfMerit(turnon_NewLayer1_Option29_mean, thrInt_Option29);
    float fm_Option30 = figureOfMerit(turnon_NewLayer1_Option30_mean, thrInt_Option30);
    float fm_Option31 = figureOfMerit(turnon_NewLayer1_Option31_mean, thrInt_Option31);
    float fm_Option31_extrap = figureOfMerit(turnon_NewLayer1_Option31_extrap_mean, thrInt_Option31_extrap);

    if(thrInt_Option1>=minThrAllowed && thrInt_Option1<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option1_mean) && has100Plateau(turnon_NewLayer1_Option1_mean)) isBetterFM(FMtype, FM_ordered, fm_Option1, thrInt_Option1, acc_Option1, "Option1");
    if(thrInt_Option2>=minThrAllowed && thrInt_Option2<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option2_mean) && has100Plateau(turnon_NewLayer1_Option2_mean)) isBetterFM(FMtype, FM_ordered, fm_Option2, thrInt_Option2, acc_Option2, "Option2");
    if(thrInt_Option3>=minThrAllowed && thrInt_Option3<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option3_mean) && has100Plateau(turnon_NewLayer1_Option3_mean)) isBetterFM(FMtype, FM_ordered, fm_Option3, thrInt_Option3, acc_Option3, "Option3");
    if(thrInt_Option4>=minThrAllowed && thrInt_Option4<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option4_mean) && has100Plateau(turnon_NewLayer1_Option4_mean)) isBetterFM(FMtype, FM_ordered, fm_Option4, thrInt_Option4, acc_Option4, "Option4");
    if(thrInt_Option5>=minThrAllowed && thrInt_Option5<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option5_mean) && has100Plateau(turnon_NewLayer1_Option5_mean)) isBetterFM(FMtype, FM_ordered, fm_Option5, thrInt_Option5, acc_Option5, "Option5");
    if(thrInt_Option6>=minThrAllowed && thrInt_Option6<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option6_mean) && has100Plateau(turnon_NewLayer1_Option6_mean)) isBetterFM(FMtype, FM_ordered, fm_Option6, thrInt_Option6, acc_Option6, "Option6");
    if(thrInt_Option7>=minThrAllowed && thrInt_Option7<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option7_mean) && has100Plateau(turnon_NewLayer1_Option7_mean)) isBetterFM(FMtype, FM_ordered, fm_Option7, thrInt_Option7, acc_Option7, "Option7");
    if(thrInt_Option8>=minThrAllowed && thrInt_Option8<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option8_mean) && has100Plateau(turnon_NewLayer1_Option8_mean)) isBetterFM(FMtype, FM_ordered, fm_Option8, thrInt_Option8, acc_Option8, "Option8");
    if(thrInt_Option9>=minThrAllowed && thrInt_Option9<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option9_mean) && has100Plateau(turnon_NewLayer1_Option9_mean)) isBetterFM(FMtype, FM_ordered, fm_Option9, thrInt_Option9, acc_Option9, "Option9");
    if(thrInt_Option10>=minThrAllowed && thrInt_Option10<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option10_mean) && has100Plateau(turnon_NewLayer1_Option10_mean)) isBetterFM(FMtype, FM_ordered, fm_Option10, thrInt_Option10, acc_Option10, "Option10");
    if(thrInt_Option11>=minThrAllowed && thrInt_Option11<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option11_mean) && has100Plateau(turnon_NewLayer1_Option11_mean)) isBetterFM(FMtype, FM_ordered, fm_Option11, thrInt_Option11, acc_Option11, "Option11");
    if(thrInt_Option12>=minThrAllowed && thrInt_Option12<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option12_mean) && has100Plateau(turnon_NewLayer1_Option12_mean)) isBetterFM(FMtype, FM_ordered, fm_Option12, thrInt_Option12, acc_Option12, "Option12");
    if(thrInt_Option13>=minThrAllowed && thrInt_Option13<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option13_mean) && has100Plateau(turnon_NewLayer1_Option13_mean)) isBetterFM(FMtype, FM_ordered, fm_Option13, thrInt_Option13, acc_Option13, "Option13");
    if(thrInt_Option14>=minThrAllowed && thrInt_Option14<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option14_mean) && has100Plateau(turnon_NewLayer1_Option14_mean)) isBetterFM(FMtype, FM_ordered, fm_Option14, thrInt_Option14, acc_Option14, "Option14");
    if(thrInt_Option15>=minThrAllowed && thrInt_Option15<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option15_mean) && has100Plateau(turnon_NewLayer1_Option15_mean)) isBetterFM(FMtype, FM_ordered, fm_Option15, thrInt_Option15, acc_Option15, "Option15");
    if(thrInt_Option16>=minThrAllowed && thrInt_Option16<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option16_mean) && has100Plateau(turnon_NewLayer1_Option16_mean)) isBetterFM(FMtype, FM_ordered, fm_Option16, thrInt_Option16, acc_Option16, "Option16");
    if(thrInt_Option17>=minThrAllowed && thrInt_Option17<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option17_mean) && has100Plateau(turnon_NewLayer1_Option17_mean)) isBetterFM(FMtype, FM_ordered, fm_Option17, thrInt_Option17, acc_Option17, "Option17");
    if(thrInt_Option18>=minThrAllowed && thrInt_Option18<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option18_mean) && has100Plateau(turnon_NewLayer1_Option18_mean)) isBetterFM(FMtype, FM_ordered, fm_Option18, thrInt_Option18, acc_Option18, "Option18");
    if(thrInt_Option19>=minThrAllowed && thrInt_Option19<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option19_mean) && has100Plateau(turnon_NewLayer1_Option19_mean)) isBetterFM(FMtype, FM_ordered, fm_Option19, thrInt_Option19, acc_Option19, "Option19");
    if(thrInt_Option20>=minThrAllowed && thrInt_Option20<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option20_mean) && has100Plateau(turnon_NewLayer1_Option20_mean)) isBetterFM(FMtype, FM_ordered, fm_Option20, thrInt_Option20, acc_Option20, "Option20");
    if(thrInt_Option21>=minThrAllowed && thrInt_Option21<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option21_mean) && has100Plateau(turnon_NewLayer1_Option21_mean)) isBetterFM(FMtype, FM_ordered, fm_Option21, thrInt_Option21, acc_Option21, "Option21");
    if(thrInt_Option22>=minThrAllowed && thrInt_Option22<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option22_mean) && has100Plateau(turnon_NewLayer1_Option22_mean)) isBetterFM(FMtype, FM_ordered, fm_Option22, thrInt_Option22, acc_Option22, "Option22");
    if(thrInt_Option23>=minThrAllowed && thrInt_Option23<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option23_mean) && has100Plateau(turnon_NewLayer1_Option23_mean)) isBetterFM(FMtype, FM_ordered, fm_Option23, thrInt_Option23, acc_Option23, "Option23");
    if(thrInt_Option24>=minThrAllowed && thrInt_Option24<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option24_mean) && has100Plateau(turnon_NewLayer1_Option24_mean)) isBetterFM(FMtype, FM_ordered, fm_Option24, thrInt_Option24, acc_Option24, "Option24");
    if(thrInt_Option25>=minThrAllowed && thrInt_Option25<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option25_mean) && has100Plateau(turnon_NewLayer1_Option25_mean)) isBetterFM(FMtype, FM_ordered, fm_Option25, thrInt_Option25, acc_Option25, "Option25");
    if(thrInt_Option26>=minThrAllowed && thrInt_Option26<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option26_mean) && has100Plateau(turnon_NewLayer1_Option26_mean)) isBetterFM(FMtype, FM_ordered, fm_Option26, thrInt_Option26, acc_Option26, "Option26");
    if(thrInt_Option27>=minThrAllowed && thrInt_Option27<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option27_mean) && has100Plateau(turnon_NewLayer1_Option27_mean)) isBetterFM(FMtype, FM_ordered, fm_Option27, thrInt_Option27, acc_Option27, "Option27");
    if(thrInt_Option28>=minThrAllowed && thrInt_Option28<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option28_mean) && has100Plateau(turnon_NewLayer1_Option28_mean)) isBetterFM(FMtype, FM_ordered, fm_Option28, thrInt_Option28, acc_Option28, "Option28");
    if(thrInt_Option29>=minThrAllowed && thrInt_Option29<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option29_mean) && has100Plateau(turnon_NewLayer1_Option29_mean)) isBetterFM(FMtype, FM_ordered, fm_Option29, thrInt_Option29, acc_Option29, "Option29");
    if(thrInt_Option30>=minThrAllowed && thrInt_Option30<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option30_mean) && has100Plateau(turnon_NewLayer1_Option30_mean)) isBetterFM(FMtype, FM_ordered, fm_Option30, thrInt_Option30, acc_Option30, "Option30");
    if(thrInt_Option31>=minThrAllowed && thrInt_Option31<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option31_mean) && has100Plateau(turnon_NewLayer1_Option31_mean)) isBetterFM(FMtype, FM_ordered, fm_Option31, thrInt_Option31, acc_Option31, "Option31");
    if(thrInt_Option31_extrap>=minThrAllowed && thrInt_Option31_extrap<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_Option31_extrap_mean) && has100Plateau(turnon_NewLayer1_Option31_extrap_mean)) isBetterFM(FMtype, FM_ordered, fm_Option31_extrap, thrInt_Option31_extrap, acc_Option31_extrap, "Option31_extrap");

    f0->Close();

    for (long unsigned int i = 0; i < tags.size(); ++i)
    {
        TString tag = tags[i];

        for (long unsigned int j = 0; j < opts.size(); ++j)
        {
            TString opt = opts[j];

            TString fileName = "/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS";
            if (fixedThr==0) fileName += "_FIXEDRATE"+fixedRate+"kHz";
            else             fileName += "_FIXEDTHR"+fixedThreshold+"GeV";
            if (run==-1)     fileName += "_SingleNeutrino122X";
            else             fileName += "_Run"+run_str;
            if(parametrisation=="linear") fileName += "_gs_linear_"+tag+"_"+opt+"_2022_06_13";
            else                          fileName += "_gs_"+parametrisation+Kintgr+"p"+Kdecim+"_"+tag+"_"+opt+"_2022_06_13";
            fileName += ".root";

            TFile* f;
            f = new TFile(fileName.Data(), "READ");
            
            TGraphAsymmErrors* turnon_NewLayer1_OptionA_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionA"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionB_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionB"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionC_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionC"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionD_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionD"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionE_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionE"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionF_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionF"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionG_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionG"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionH_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionH"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionI_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionI"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionJ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionJ"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionK_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionK"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionL_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionL"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionM_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionM"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionN_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionN"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionO_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionO"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionP_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionP"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionQ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionQ"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionR_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionR"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionS_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionS"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionT_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionT"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionU_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionU"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionV_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionV"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionW_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionW"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionX_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionX"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionY_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionY"+opt+"_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_OptionZ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionZ"+opt+"_by_pt");
            
            int thrInt_OptionA = int( ( (TVectorT<float>*)f->Get("thr_OptionA") )[0][0]) +1;
            int thrInt_OptionB = int( ( (TVectorT<float>*)f->Get("thr_OptionB") )[0][0]) +1;
            int thrInt_OptionC = int( ( (TVectorT<float>*)f->Get("thr_OptionC") )[0][0]) +1;
            int thrInt_OptionD = int( ( (TVectorT<float>*)f->Get("thr_OptionD") )[0][0]) +1;
            int thrInt_OptionE = int( ( (TVectorT<float>*)f->Get("thr_OptionE") )[0][0]) +1;
            int thrInt_OptionF = int( ( (TVectorT<float>*)f->Get("thr_OptionF") )[0][0]) +1;
            int thrInt_OptionG = int( ( (TVectorT<float>*)f->Get("thr_OptionG") )[0][0]) +1;
            int thrInt_OptionH = int( ( (TVectorT<float>*)f->Get("thr_OptionH") )[0][0]) +1;
            int thrInt_OptionI = int( ( (TVectorT<float>*)f->Get("thr_OptionI") )[0][0]) +1;
            int thrInt_OptionJ = int( ( (TVectorT<float>*)f->Get("thr_OptionJ") )[0][0]) +1;
            int thrInt_OptionK = int( ( (TVectorT<float>*)f->Get("thr_OptionK") )[0][0]) +1;
            int thrInt_OptionL = int( ( (TVectorT<float>*)f->Get("thr_OptionL") )[0][0]) +1;
            int thrInt_OptionM = int( ( (TVectorT<float>*)f->Get("thr_OptionM") )[0][0]) +1;
            int thrInt_OptionN = int( ( (TVectorT<float>*)f->Get("thr_OptionN") )[0][0]) +1;
            int thrInt_OptionO = int( ( (TVectorT<float>*)f->Get("thr_OptionO") )[0][0]) +1;
            int thrInt_OptionP = int( ( (TVectorT<float>*)f->Get("thr_OptionP") )[0][0]) +1;
            int thrInt_OptionQ = int( ( (TVectorT<float>*)f->Get("thr_OptionQ") )[0][0]) +1;
            int thrInt_OptionR = int( ( (TVectorT<float>*)f->Get("thr_OptionR") )[0][0]) +1;
            int thrInt_OptionS = int( ( (TVectorT<float>*)f->Get("thr_OptionS") )[0][0]) +1;
            int thrInt_OptionT = int( ( (TVectorT<float>*)f->Get("thr_OptionT") )[0][0]) +1;
            int thrInt_OptionU = int( ( (TVectorT<float>*)f->Get("thr_OptionU") )[0][0]) +1;
            int thrInt_OptionV = int( ( (TVectorT<float>*)f->Get("thr_OptionV") )[0][0]) +1;
            int thrInt_OptionW = int( ( (TVectorT<float>*)f->Get("thr_OptionW") )[0][0]) +1;
            int thrInt_OptionX = int( ( (TVectorT<float>*)f->Get("thr_OptionX") )[0][0]) +1;
            int thrInt_OptionY = int( ( (TVectorT<float>*)f->Get("thr_OptionY") )[0][0]) +1;
            int thrInt_OptionZ = int( ( (TVectorT<float>*)f->Get("thr_OptionZ") )[0][0]) +1;

            float acc_OptionA = ( (TVectorT<float>*)f->Get("acc_OptionA") )[0][0];
            float acc_OptionB = ( (TVectorT<float>*)f->Get("acc_OptionB") )[0][0];
            float acc_OptionC = ( (TVectorT<float>*)f->Get("acc_OptionC") )[0][0];
            float acc_OptionD = ( (TVectorT<float>*)f->Get("acc_OptionD") )[0][0];
            float acc_OptionE = ( (TVectorT<float>*)f->Get("acc_OptionE") )[0][0];
            float acc_OptionF = ( (TVectorT<float>*)f->Get("acc_OptionF") )[0][0];
            float acc_OptionG = ( (TVectorT<float>*)f->Get("acc_OptionG") )[0][0];
            float acc_OptionH = ( (TVectorT<float>*)f->Get("acc_OptionH") )[0][0];
            float acc_OptionI = ( (TVectorT<float>*)f->Get("acc_OptionI") )[0][0];
            float acc_OptionJ = ( (TVectorT<float>*)f->Get("acc_OptionJ") )[0][0];
            float acc_OptionK = ( (TVectorT<float>*)f->Get("acc_OptionK") )[0][0];
            float acc_OptionL = ( (TVectorT<float>*)f->Get("acc_OptionL") )[0][0];
            float acc_OptionM = ( (TVectorT<float>*)f->Get("acc_OptionM") )[0][0];
            float acc_OptionN = ( (TVectorT<float>*)f->Get("acc_OptionN") )[0][0];
            float acc_OptionO = ( (TVectorT<float>*)f->Get("acc_OptionO") )[0][0];
            float acc_OptionP = ( (TVectorT<float>*)f->Get("acc_OptionP") )[0][0];
            float acc_OptionQ = ( (TVectorT<float>*)f->Get("acc_OptionQ") )[0][0];
            float acc_OptionR = ( (TVectorT<float>*)f->Get("acc_OptionR") )[0][0];
            float acc_OptionS = ( (TVectorT<float>*)f->Get("acc_OptionS") )[0][0];
            float acc_OptionT = ( (TVectorT<float>*)f->Get("acc_OptionT") )[0][0];
            float acc_OptionU = ( (TVectorT<float>*)f->Get("acc_OptionU") )[0][0];
            float acc_OptionV = ( (TVectorT<float>*)f->Get("acc_OptionV") )[0][0];
            float acc_OptionW = ( (TVectorT<float>*)f->Get("acc_OptionW") )[0][0];
            float acc_OptionX = ( (TVectorT<float>*)f->Get("acc_OptionX") )[0][0];
            float acc_OptionY = ( (TVectorT<float>*)f->Get("acc_OptionY") )[0][0];
            float acc_OptionZ = ( (TVectorT<float>*)f->Get("acc_OptionZ") )[0][0];

            float fm_OptionA = figureOfMerit(turnon_NewLayer1_OptionA_mean, thrInt_OptionA);
            float fm_OptionB = figureOfMerit(turnon_NewLayer1_OptionB_mean, thrInt_OptionB);
            float fm_OptionC = figureOfMerit(turnon_NewLayer1_OptionC_mean, thrInt_OptionC);
            float fm_OptionD = figureOfMerit(turnon_NewLayer1_OptionD_mean, thrInt_OptionD);
            float fm_OptionE = figureOfMerit(turnon_NewLayer1_OptionE_mean, thrInt_OptionE);
            float fm_OptionF = figureOfMerit(turnon_NewLayer1_OptionF_mean, thrInt_OptionF);
            float fm_OptionG = figureOfMerit(turnon_NewLayer1_OptionG_mean, thrInt_OptionG);
            float fm_OptionH = figureOfMerit(turnon_NewLayer1_OptionH_mean, thrInt_OptionH);
            float fm_OptionI = figureOfMerit(turnon_NewLayer1_OptionI_mean, thrInt_OptionI);
            float fm_OptionJ = figureOfMerit(turnon_NewLayer1_OptionJ_mean, thrInt_OptionJ);
            float fm_OptionK = figureOfMerit(turnon_NewLayer1_OptionK_mean, thrInt_OptionK);
            float fm_OptionL = figureOfMerit(turnon_NewLayer1_OptionL_mean, thrInt_OptionL);
            float fm_OptionM = figureOfMerit(turnon_NewLayer1_OptionM_mean, thrInt_OptionM);
            float fm_OptionN = figureOfMerit(turnon_NewLayer1_OptionN_mean, thrInt_OptionN);
            float fm_OptionO = figureOfMerit(turnon_NewLayer1_OptionO_mean, thrInt_OptionO);
            float fm_OptionP = figureOfMerit(turnon_NewLayer1_OptionP_mean, thrInt_OptionP);
            float fm_OptionQ = figureOfMerit(turnon_NewLayer1_OptionQ_mean, thrInt_OptionQ);
            float fm_OptionR = figureOfMerit(turnon_NewLayer1_OptionR_mean, thrInt_OptionR);
            float fm_OptionS = figureOfMerit(turnon_NewLayer1_OptionS_mean, thrInt_OptionS);
            float fm_OptionT = figureOfMerit(turnon_NewLayer1_OptionT_mean, thrInt_OptionT);
            float fm_OptionU = figureOfMerit(turnon_NewLayer1_OptionU_mean, thrInt_OptionU);
            float fm_OptionV = figureOfMerit(turnon_NewLayer1_OptionV_mean, thrInt_OptionV);
            float fm_OptionW = figureOfMerit(turnon_NewLayer1_OptionW_mean, thrInt_OptionW);
            float fm_OptionX = figureOfMerit(turnon_NewLayer1_OptionX_mean, thrInt_OptionX);
            float fm_OptionY = figureOfMerit(turnon_NewLayer1_OptionY_mean, thrInt_OptionY);
            float fm_OptionZ = figureOfMerit(turnon_NewLayer1_OptionZ_mean, thrInt_OptionZ);

            if (thrInt_OptionA>=minThrAllowed && thrInt_OptionA<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionA_mean) && has100Plateau(turnon_NewLayer1_OptionA_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionA, thrInt_OptionA, acc_OptionA, tag+" OptionA"+opt);
            if (thrInt_OptionB>=minThrAllowed && thrInt_OptionB<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionB_mean) && has100Plateau(turnon_NewLayer1_OptionB_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionB, thrInt_OptionB, acc_OptionB, tag+" OptionB"+opt);
            if (thrInt_OptionC>=minThrAllowed && thrInt_OptionC<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionC_mean) && has100Plateau(turnon_NewLayer1_OptionC_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionC, thrInt_OptionC, acc_OptionC, tag+" OptionC"+opt);
            if (thrInt_OptionD>=minThrAllowed && thrInt_OptionD<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionD_mean) && has100Plateau(turnon_NewLayer1_OptionD_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionD, thrInt_OptionD, acc_OptionD, tag+" OptionD"+opt);
            if (thrInt_OptionE>=minThrAllowed && thrInt_OptionE<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionE_mean) && has100Plateau(turnon_NewLayer1_OptionE_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionE, thrInt_OptionE, acc_OptionE, tag+" OptionE"+opt);
            if (thrInt_OptionF>=minThrAllowed && thrInt_OptionF<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionF_mean) && has100Plateau(turnon_NewLayer1_OptionF_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionF, thrInt_OptionF, acc_OptionF, tag+" OptionF"+opt);
            if (thrInt_OptionG>=minThrAllowed && thrInt_OptionG<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionG_mean) && has100Plateau(turnon_NewLayer1_OptionG_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionG, thrInt_OptionG, acc_OptionG, tag+" OptionG"+opt);
            if (thrInt_OptionH>=minThrAllowed && thrInt_OptionH<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionH_mean) && has100Plateau(turnon_NewLayer1_OptionH_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionH, thrInt_OptionH, acc_OptionH, tag+" OptionH"+opt);
            if (thrInt_OptionI>=minThrAllowed && thrInt_OptionI<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionI_mean) && has100Plateau(turnon_NewLayer1_OptionI_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionI, thrInt_OptionI, acc_OptionI, tag+" OptionI"+opt);
            if (thrInt_OptionJ>=minThrAllowed && thrInt_OptionJ<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionJ_mean) && has100Plateau(turnon_NewLayer1_OptionJ_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionJ, thrInt_OptionJ, acc_OptionJ, tag+" OptionJ"+opt);
            if (thrInt_OptionK>=minThrAllowed && thrInt_OptionK<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionK_mean) && has100Plateau(turnon_NewLayer1_OptionK_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionK, thrInt_OptionK, acc_OptionK, tag+" OptionK"+opt);
            if (thrInt_OptionL>=minThrAllowed && thrInt_OptionL<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionL_mean) && has100Plateau(turnon_NewLayer1_OptionL_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionL, thrInt_OptionL, acc_OptionL, tag+" OptionL"+opt);
            if (thrInt_OptionM>=minThrAllowed && thrInt_OptionM<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionM_mean) && has100Plateau(turnon_NewLayer1_OptionM_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionM, thrInt_OptionM, acc_OptionM, tag+" OptionM"+opt);
            if (thrInt_OptionN>=minThrAllowed && thrInt_OptionN<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionN_mean) && has100Plateau(turnon_NewLayer1_OptionN_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionN, thrInt_OptionN, acc_OptionN, tag+" OptionN"+opt);
            if (thrInt_OptionO>=minThrAllowed && thrInt_OptionO<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionO_mean) && has100Plateau(turnon_NewLayer1_OptionO_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionO, thrInt_OptionO, acc_OptionO, tag+" OptionO"+opt);
            if (thrInt_OptionP>=minThrAllowed && thrInt_OptionP<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionP_mean) && has100Plateau(turnon_NewLayer1_OptionP_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionP, thrInt_OptionP, acc_OptionP, tag+" OptionP"+opt);
            if (thrInt_OptionQ>=minThrAllowed && thrInt_OptionQ<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionQ_mean) && has100Plateau(turnon_NewLayer1_OptionQ_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionQ, thrInt_OptionQ, acc_OptionQ, tag+" OptionQ"+opt);
            if (thrInt_OptionR>=minThrAllowed && thrInt_OptionR<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionR_mean) && has100Plateau(turnon_NewLayer1_OptionR_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionR, thrInt_OptionR, acc_OptionR, tag+" OptionR"+opt);
            if (thrInt_OptionS>=minThrAllowed && thrInt_OptionS<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionS_mean) && has100Plateau(turnon_NewLayer1_OptionS_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionS, thrInt_OptionS, acc_OptionS, tag+" OptionS"+opt);
            if (thrInt_OptionT>=minThrAllowed && thrInt_OptionT<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionT_mean) && has100Plateau(turnon_NewLayer1_OptionT_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionT, thrInt_OptionT, acc_OptionT, tag+" OptionT"+opt);
            if (thrInt_OptionU>=minThrAllowed && thrInt_OptionU<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionU_mean) && has100Plateau(turnon_NewLayer1_OptionU_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionU, thrInt_OptionU, acc_OptionU, tag+" OptionU"+opt);
            if (thrInt_OptionV>=minThrAllowed && thrInt_OptionV<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionV_mean) && has100Plateau(turnon_NewLayer1_OptionV_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionV, thrInt_OptionV, acc_OptionV, tag+" OptionV"+opt);
            if (thrInt_OptionW>=minThrAllowed && thrInt_OptionW<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionW_mean) && has100Plateau(turnon_NewLayer1_OptionW_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionW, thrInt_OptionW, acc_OptionW, tag+" OptionW"+opt);
            if (thrInt_OptionX>=minThrAllowed && thrInt_OptionX<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionX_mean) && has100Plateau(turnon_NewLayer1_OptionX_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionX, thrInt_OptionX, acc_OptionX, tag+" OptionX"+opt);
            if (thrInt_OptionY>=minThrAllowed && thrInt_OptionY<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionY_mean) && has100Plateau(turnon_NewLayer1_OptionY_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionY, thrInt_OptionY, acc_OptionY, tag+" OptionY"+opt);
            if (thrInt_OptionZ>=minThrAllowed && thrInt_OptionZ<=maxThrAllowed && has0effAt0pt(turnon_NewLayer1_OptionZ_mean) && has100Plateau(turnon_NewLayer1_OptionZ_mean)) isBetterFM(FMtype, FM_ordered, fm_OptionZ, thrInt_OptionZ, acc_OptionZ, tag+" OptionZ"+opt);
            
            f->Close();
        }
    }

    FM_ordered.pop_back(); // remove init entry

    TString Filename = "FMs/optimizationV13/"; 
    if (FMtype=="FM") Filename += "FM_orderd_turnons";
    else if (FMtype=="FMtACC") Filename += "FMtACC_orderd_turnons";
    if (targetRate!=0) Filename += "_FIXEDRATE"+fixedRate+"kHz";
    if (fixedThr!=0)   Filename += "_FIXEDTHR"+fixedThreshold+"kHz";
    if(parametrisation=="linear") Filename += "_linear.txt";
    else                          Filename += "_"+parametrisation+Kintgr+"p"+Kdecim+".txt";
    ofstream file;
    file.open(Filename, std::ofstream::trunc);
    for (long unsigned int i = 0; i < FM_ordered.size(); ++i)
    {
        file << FM_ordered[i].first << " : FM=" << FM_ordered[i].second.second.first << " - THR=" << FM_ordered[i].second.first << " - ACC=" << FM_ordered[i].second.second.second << " - FMtACC=" << FM_ordered[i].second.second.first*FM_ordered[i].second.second.second << std::endl;
    }
    file.close();

/*
    TCanvas c("c","c",650,600);
    c.SetLeftMargin(0.15);
    c.SetGrid();
    c.SetLogy();

    TLegend* leg = new TLegend(0.5,0.10,0.89,0.70);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.009);
    leg->SetHeader("Linearly scaled to 2.0E34");

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0., 1, 1.0);
    pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
    pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    //pad1->SetLogy();
    //pad1->SetLogx();

    TFile* f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_02_02_optimizationV7_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_Run"+run_str+"_gs_effMin0p0_0_2022_02_02.root","READ");
    TGraphAsymmErrors* turnon_NewLayer1_Option22_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option22_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option31_extrap_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option31_extrap_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_noIso_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_noIso_by_pt");
    
    TString fm = "FM";
    turnon_NewLayer1_noIso_mean->SetTitle("");
    turnon_NewLayer1_noIso_mean->SetLineColor(kBlack);
    turnon_NewLayer1_noIso_mean->SetLineWidth(1);
    turnon_NewLayer1_noIso_mean->GetXaxis()->SetLimits(15,120.);
    if (zoom) 
    {
        turnon_NewLayer1_noIso_mean->GetXaxis()->SetLimits(40,120.);
        turnon_NewLayer1_noIso_mean->GetYaxis()->SetRangeUser(0.8,1.01);
    }
    turnon_NewLayer1_noIso_mean->GetYaxis()->SetTitle("Efficiency");
    turnon_NewLayer1_noIso_mean->GetXaxis()->SetTitle("Offline p_{T}(#tau) [GeV]");
    turnon_NewLayer1_noIso_mean->Draw();
    leg->AddEntry(turnon_NewLayer1_noIso_mean,"Di-#tau no-iso, mean - "+fm+"="+fm_noIso,"L");

    turnon_NewLayer1_Option22_mean->SetLineColor(kGray+1);
    turnon_NewLayer1_Option22_mean->SetLineWidth(1);
    turnon_NewLayer1_Option22_mean->Draw("same");
    leg->AddEntry(turnon_NewLayer1_Option22_mean,"Di-#tau iso (Option 22) - "+fm+"="+fm_Option22,"L");

    turnon_NewLayer1_Option31_extrap_mean->SetLineColor(kGray+2);
    turnon_NewLayer1_Option31_extrap_mean->SetLineWidth(1);
    turnon_NewLayer1_Option31_extrap_mean->Draw("same");
    leg->AddEntry(turnon_NewLayer1_Option31_extrap_mean,"Di-#tau iso (Option 31 extrap) - "+fm+"="+fm_Option31_extrap,"L");

    int j = 0;
    std::vector<int> colors = {kBlue, kOrange, kCyan, kMagenta, kGreen, kYellow, kRed, kBlue-1, kOrange-1, kCyan-1, kMagenta-1, kGreen-1, kYellow-1, kRed-1, kBlue-2, kOrange-2, kCyan-2, kMagenta-1, kGreen-2, kYellow-2, kRed-2 , kBlue-3, kOrange-3, kCyan-3, kMagenta-3, kGreen-3, kYellow-3, kRed-3, kBlue-4, kOrange-4, kCyan-4, kMagenta-4, kGreen-4, kYellow-4, kRed-4, kBlue-5, kOrange-5, kCyan-5, kMagenta-5, kGreen-5, kYellow-5, kRed-5, kBlue-6, kOrange-6, kCyan-6, kMagenta-6, kGreen-6, kYellow-6, kRed-7, kBlue-7, kOrange-7, kCyan-7, kMagenta-7, kGreen-7, kYellow-7, kRed-7, kBlue-8, kOrange-8, kCyan-8, kMagenta-8, kGreen-8, kYellow-8, kRed-8, kBlue-9, kOrange-9, kCyan-9, kMagenta-9, kGreen-9, kYellow-9, kRed-9, kBlue-10, kOrange-10, kCyan-10, kMagenta-10, kGreen-10, kYellow-10, kRed-10, kBlue+4, kOrange+4, kCyan+4, kMagenta+4, kGreen+4, kYellow+4, kRed+4, kBlue+3, kOrange+3, kCyan+3, kMagenta+3, kGreen+3, kYellow+3, kRed+3, kBlue+2, kOrange+2, kCyan+2, kMagenta+2, kGreen+2, kYellow+2, kRed+2, kBlue+1, kOrange+1, kCyan+1, kMagenta+1, kGreen+1, kYellow+1, kRed+1, kBlue, kOrange, kCyan, kMagenta, kGreen, kYellow, kRed, kBlue-1, kOrange-1, kCyan-1, kMagenta-1, kGreen-1, kYellow-1, kRed-1};

    for (long unsigned int i = 0; i < FM_ordered.size(); ++i)
    {
        std::string gridPoint = FM_ordered[i].first.Data();
        TString effMin = gridPoint.substr(0, gridPoint.find(" "));
        TString point = gridPoint.substr(gridPoint.length() - 2);
        TString option = gridPoint.back();

        float FM = FM_ordered[i].second.first;
        float TH = FM_ordered[i].second.second;

        if (baseline=="22")
        {
            if (FM >= fm_Option22) // TH == thrInt_Option22 and
            {
                TFile* f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_02_02_optimizationV7_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_Run"+run_str+"_gs_"+effMin+"_"+option+"_2022_02_02.root","READ");

                TGraphAsymmErrors* turnon = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option"+point+"_by_pt");
                turnon->SetLineColor(colors[j]); ++j;
                turnon->SetLineWidth(1);
                turnon->Draw("same");
                leg->AddEntry(turnon, "Di-#tau iso ("+effMin+" Option "+point+") - FM="+FM, "L");

                f->Close();
            }
        }
        else
        {
            if (FM >= fm_Option31_extrap) //TH == thrInt_Option31_extrap and
            {
                TFile* f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_02_02_optimizationV7_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_Run"+run_str+"_gs_"+effMin+"_"+option+"_2022_02_02.root","READ");

                TGraphAsymmErrors* turnon = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option"+point+"_by_pt");
                turnon->SetLineColor(colors[j]); ++j;
                turnon->SetLineWidth(1);
                turnon->Draw("same");
                leg->AddEntry(turnon, "Di-#tau iso ("+effMin+" Option "+point+") - FM="+FM, "L");

                f->Close();
            }
        }

    }

    leg->Draw("same");

    TString tag = "";
    if (zoom) tag = "_zoom";
    c.SaveAs("plotsBestFMturnons_Run"+run_str+"/Comparison_TurnOn_Rate_Run"+run_str+"_newnTT_unpacked_optimizationV7gs_calibThr"+intgr+"p"+decim+"_betterThan"+baseline+"_thrLeq"+thrLim+tag+".pdf");
*/

}





































