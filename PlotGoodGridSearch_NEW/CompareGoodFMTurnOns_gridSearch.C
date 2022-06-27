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

void compare(int targetRate = 14, int fixedThr = 0, TString tag="effMin0p0", TString opt="0", int run=-1, float calibThr = 1.7) {
    TString run_str = to_string(run);

    TString fixedRate = to_string(targetRate);
    TString fixedThreshold = to_string(fixedThr);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    int minThrAllowed = 28;
    int maxThrAllowed = 38;

    gStyle->SetOptStat(000000);

    TString fileName = "/data_CMS/cms/motta/Run3preparation/2022_04_21_optimizationV11_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS";
    if (fixedThr==0) fileName += "_FIXEDRATE"+fixedRate+"kHz";
    else             fileName += "_FIXEDTHR"+fixedThreshold+"GeV";
    if (run==-1)     fileName += "_SingleNeutrino112XpuReweighted";
    else             fileName += "_Run"+run_str;
    fileName += "_gs_"+tag+"_"+opt+"_2022_04_21.root";

    TFile* f;
    f = new TFile(fileName.Data(), "READ");

    TGraphAsymmErrors* turnOn_NewLayer1_noIso_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_noIso_by_pt");

    TGraphAsymmErrors* turnOn_NewLayer1_OptionA_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionA"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionB_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionB"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionC_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionC"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionD_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionD"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionE_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionE"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionF_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionF"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionG_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionG"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionH_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionH"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionI_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionI"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionJ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionJ"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionK_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionK"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionL_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionL"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionM_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionM"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionN_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionN"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionO_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionO"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionP_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionP"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionQ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionQ"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionR_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionR"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionS_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionS"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionT_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionT"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionU_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionU"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionV_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionV"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionW_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionW"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionX_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionX"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionY_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionY"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionZ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionZ"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option1_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option1_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option2_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option2_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option3_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option3_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option4_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option4_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option5_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option5_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option6_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option6_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option7_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option7_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option8_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option8_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option9_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option9_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option10_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option10_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option11_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option11_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option12_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option12_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option13_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option13_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option14_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option14_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option15_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option15_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option16_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option16_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option17_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option17_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option18_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option18_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option19_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option19_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option20_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option20_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option21_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option21_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option22_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option22_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option23_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option23_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option24_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option24_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option25_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option25_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option26_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option26_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option27_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option27_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option28_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option28_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option29_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option29_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option30_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option30_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option31_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option31_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option31_extrap_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option31_extrap_by_pt");

    TString thr_noIso       = std::to_string( int( ((TVectorT<float>*)f->Get("thr_noIso"))[0][0] ) +1 );
    TString thr_OptionA = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionA"))[0][0] ) +1 );
    TString thr_OptionB = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionB"))[0][0] ) +1 );
    TString thr_OptionC = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionC"))[0][0] ) +1 );
    TString thr_OptionD = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionD"))[0][0] ) +1 );
    TString thr_OptionE = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionE"))[0][0] ) +1 );
    TString thr_OptionF = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionF"))[0][0] ) +1 );
    TString thr_OptionG = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionG"))[0][0] ) +1 );
    TString thr_OptionH = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionH"))[0][0] ) +1 );
    TString thr_OptionI = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionI"))[0][0] ) +1 );
    TString thr_OptionJ = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionJ"))[0][0] ) +1 );
    TString thr_OptionK = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionK"))[0][0] ) +1 );
    TString thr_OptionL = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionL"))[0][0] ) +1 );
    TString thr_OptionM = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionM"))[0][0] ) +1 );
    TString thr_OptionN = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionN"))[0][0] ) +1 );
    TString thr_OptionO = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionO"))[0][0] ) +1 );
    TString thr_OptionP = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionP"))[0][0] ) +1 );
    TString thr_OptionQ = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionQ"))[0][0] ) +1 );
    TString thr_OptionR = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionR"))[0][0] ) +1 );
    TString thr_OptionS = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionS"))[0][0] ) +1 );
    TString thr_OptionT = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionT"))[0][0] ) +1 );
    TString thr_OptionU = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionU"))[0][0] ) +1 );
    TString thr_OptionV = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionV"))[0][0] ) +1 );
    TString thr_OptionW = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionW"))[0][0] ) +1 );
    TString thr_OptionX = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionX"))[0][0] ) +1 );
    TString thr_OptionY = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionY"))[0][0] ) +1 );
    TString thr_OptionZ = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionZ"))[0][0] ) +1 );
    TString thr_Option1 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option1"))[0][0] ) +1 );
    TString thr_Option2 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option2"))[0][0] ) +1 );
    TString thr_Option3 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option3"))[0][0] ) +1 );
    TString thr_Option4 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option4"))[0][0] ) +1 );
    TString thr_Option5 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option5"))[0][0] ) +1 );
    TString thr_Option6 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option6"))[0][0] ) +1 );
    TString thr_Option7 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option7"))[0][0] ) +1 );
    TString thr_Option8 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option8"))[0][0] ) +1 );
    TString thr_Option9 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option9"))[0][0] ) +1 );
    TString thr_Option10 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option10"))[0][0] ) +1 );
    TString thr_Option11 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option11"))[0][0] ) +1 );
    TString thr_Option12 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option12"))[0][0] ) +1 );
    TString thr_Option13 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option13"))[0][0] ) +1 );
    TString thr_Option14 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option14"))[0][0] ) +1 );
    TString thr_Option15 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option15"))[0][0] ) +1 );
    TString thr_Option16 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option16"))[0][0] ) +1 );
    TString thr_Option17 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option17"))[0][0] ) +1 );
    TString thr_Option18 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option18"))[0][0] ) +1 );
    TString thr_Option19 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option19"))[0][0] ) +1 );
    TString thr_Option20 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option20"))[0][0] ) +1 );
    TString thr_Option21 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option21"))[0][0] ) +1 );
    TString thr_Option22 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option22"))[0][0] ) +1 );
    TString thr_Option23 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option23"))[0][0] ) +1 );
    TString thr_Option24 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option24"))[0][0] ) +1 );
    TString thr_Option25 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option25"))[0][0] ) +1 );
    TString thr_Option26 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option26"))[0][0] ) +1 );
    TString thr_Option27 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option27"))[0][0] ) +1 );
    TString thr_Option28 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option28"))[0][0] ) +1 );
    TString thr_Option29 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option29"))[0][0] ) +1 );
    TString thr_Option30 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option30"))[0][0] ) +1 );
    TString thr_Option31 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option31"))[0][0] ) +1 );
    TString thr_Option31_extrap = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option31_extrap"))[0][0] ) +1 );

    int thrInt_OptionA = int( ((TVectorT<float>*)f->Get("thr_OptionA"))[0][0] ) +1;
    int thrInt_OptionB = int( ((TVectorT<float>*)f->Get("thr_OptionB"))[0][0] ) +1;
    int thrInt_OptionC = int( ((TVectorT<float>*)f->Get("thr_OptionC"))[0][0] ) +1;
    int thrInt_OptionD = int( ((TVectorT<float>*)f->Get("thr_OptionD"))[0][0] ) +1;
    int thrInt_OptionE = int( ((TVectorT<float>*)f->Get("thr_OptionE"))[0][0] ) +1;
    int thrInt_OptionF = int( ((TVectorT<float>*)f->Get("thr_OptionF"))[0][0] ) +1;
    int thrInt_OptionG = int( ((TVectorT<float>*)f->Get("thr_OptionG"))[0][0] ) +1;
    int thrInt_OptionH = int( ((TVectorT<float>*)f->Get("thr_OptionH"))[0][0] ) +1;
    int thrInt_OptionI = int( ((TVectorT<float>*)f->Get("thr_OptionI"))[0][0] ) +1;
    int thrInt_OptionJ = int( ((TVectorT<float>*)f->Get("thr_OptionJ"))[0][0] ) +1;
    int thrInt_OptionK = int( ((TVectorT<float>*)f->Get("thr_OptionK"))[0][0] ) +1;
    int thrInt_OptionL = int( ((TVectorT<float>*)f->Get("thr_OptionL"))[0][0] ) +1;
    int thrInt_OptionM = int( ((TVectorT<float>*)f->Get("thr_OptionM"))[0][0] ) +1;
    int thrInt_OptionN = int( ((TVectorT<float>*)f->Get("thr_OptionN"))[0][0] ) +1;
    int thrInt_OptionO = int( ((TVectorT<float>*)f->Get("thr_OptionO"))[0][0] ) +1;
    int thrInt_OptionP = int( ((TVectorT<float>*)f->Get("thr_OptionP"))[0][0] ) +1;
    int thrInt_OptionQ = int( ((TVectorT<float>*)f->Get("thr_OptionQ"))[0][0] ) +1;
    int thrInt_OptionR = int( ((TVectorT<float>*)f->Get("thr_OptionR"))[0][0] ) +1;
    int thrInt_OptionS = int( ((TVectorT<float>*)f->Get("thr_OptionS"))[0][0] ) +1;
    int thrInt_OptionT = int( ((TVectorT<float>*)f->Get("thr_OptionT"))[0][0] ) +1;
    int thrInt_OptionU = int( ((TVectorT<float>*)f->Get("thr_OptionU"))[0][0] ) +1;
    int thrInt_OptionV = int( ((TVectorT<float>*)f->Get("thr_OptionV"))[0][0] ) +1;
    int thrInt_OptionW = int( ((TVectorT<float>*)f->Get("thr_OptionW"))[0][0] ) +1;
    int thrInt_OptionX = int( ((TVectorT<float>*)f->Get("thr_OptionX"))[0][0] ) +1;
    int thrInt_OptionY = int( ((TVectorT<float>*)f->Get("thr_OptionY"))[0][0] ) +1;
    int thrInt_OptionZ = int( ((TVectorT<float>*)f->Get("thr_OptionZ"))[0][0] ) +1;
    int thrInt_Option1 = int( ((TVectorT<float>*)f->Get("thr_Option1"))[0][0] ) +1;
    int thrInt_Option2 = int( ((TVectorT<float>*)f->Get("thr_Option2"))[0][0] ) +1;
    int thrInt_Option3 = int( ((TVectorT<float>*)f->Get("thr_Option3"))[0][0] ) +1;
    int thrInt_Option4 = int( ((TVectorT<float>*)f->Get("thr_Option4"))[0][0] ) +1;
    int thrInt_Option5 = int( ((TVectorT<float>*)f->Get("thr_Option5"))[0][0] ) +1;
    int thrInt_Option6 = int( ((TVectorT<float>*)f->Get("thr_Option6"))[0][0] ) +1;
    int thrInt_Option7 = int( ((TVectorT<float>*)f->Get("thr_Option7"))[0][0] ) +1;
    int thrInt_Option8 = int( ((TVectorT<float>*)f->Get("thr_Option8"))[0][0] ) +1;
    int thrInt_Option9 = int( ((TVectorT<float>*)f->Get("thr_Option9"))[0][0] ) +1;
    int thrInt_Option10 = int( ((TVectorT<float>*)f->Get("thr_Option10"))[0][0] ) +1;
    int thrInt_Option11 = int( ((TVectorT<float>*)f->Get("thr_Option11"))[0][0] ) +1;
    int thrInt_Option12 = int( ((TVectorT<float>*)f->Get("thr_Option12"))[0][0] ) +1;
    int thrInt_Option13 = int( ((TVectorT<float>*)f->Get("thr_Option13"))[0][0] ) +1;
    int thrInt_Option14 = int( ((TVectorT<float>*)f->Get("thr_Option14"))[0][0] ) +1;
    int thrInt_Option15 = int( ((TVectorT<float>*)f->Get("thr_Option15"))[0][0] ) +1;
    int thrInt_Option16 = int( ((TVectorT<float>*)f->Get("thr_Option16"))[0][0] ) +1;
    int thrInt_Option17 = int( ((TVectorT<float>*)f->Get("thr_Option17"))[0][0] ) +1;
    int thrInt_Option18 = int( ((TVectorT<float>*)f->Get("thr_Option18"))[0][0] ) +1;
    int thrInt_Option19 = int( ((TVectorT<float>*)f->Get("thr_Option19"))[0][0] ) +1;
    int thrInt_Option20 = int( ((TVectorT<float>*)f->Get("thr_Option20"))[0][0] ) +1;
    int thrInt_Option21 = int( ((TVectorT<float>*)f->Get("thr_Option21"))[0][0] ) +1;
    int thrInt_Option22 = int( ((TVectorT<float>*)f->Get("thr_Option22"))[0][0] ) +1;
    int thrInt_Option23 = int( ((TVectorT<float>*)f->Get("thr_Option23"))[0][0] ) +1;
    int thrInt_Option24 = int( ((TVectorT<float>*)f->Get("thr_Option24"))[0][0] ) +1;
    int thrInt_Option25 = int( ((TVectorT<float>*)f->Get("thr_Option25"))[0][0] ) +1;
    int thrInt_Option26 = int( ((TVectorT<float>*)f->Get("thr_Option26"))[0][0] ) +1;
    int thrInt_Option27 = int( ((TVectorT<float>*)f->Get("thr_Option27"))[0][0] ) +1;
    int thrInt_Option28 = int( ((TVectorT<float>*)f->Get("thr_Option28"))[0][0] ) +1;
    int thrInt_Option29 = int( ((TVectorT<float>*)f->Get("thr_Option29"))[0][0] ) +1;
    int thrInt_Option30 = int( ((TVectorT<float>*)f->Get("thr_Option30"))[0][0] ) +1;
    int thrInt_Option31 = int( ((TVectorT<float>*)f->Get("thr_Option31"))[0][0] ) +1;
    int thrInt_Option31_extrap = int( ((TVectorT<float>*)f->Get("thr_Option31_extrap"))[0][0] ) +1;

    TString CanvasName = "Comparison_TurnOn";
    if (run==-1)     CanvasName += "_SingleNeutrino112XpuReweighted";
    else             CanvasName += "_Run"+run_str;
    CanvasName += "_newnTT_unpacked_optimizationV11gs";
    if (fixedThr==0) CanvasName += "_FIXEDRATE"+fixedRate+"kHz";
    else             CanvasName += "_FIXEDTHR"+fixedThreshold+"GeV";
    CanvasName += "_calibThr"+intgr+"p"+decim+"_"+tag+"_"+opt;
    CanvasName += "_bounds"+to_string(minThrAllowed)+"THR"+to_string(maxThrAllowed);
    TString CanvasNamePdf = CanvasName ;
    CanvasNamePdf += ".pdf";

    bool drewSth = false;

    TCanvas c(CanvasName.Data(),CanvasName.Data(),650,600);
    c.SetLeftMargin(0.15);
    c.SetGrid();
    c.SetLogy();

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0., 1, 1.0);
    //pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
    pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    //pad1->SetLogy();

    turnOn_NewLayer1_noIso_mean->SetLineColor(kBlack);
    turnOn_NewLayer1_noIso_mean->SetLineWidth(1);
    turnOn_NewLayer1_noIso_mean->GetXaxis()->SetLimits(15,120.);
    turnOn_NewLayer1_noIso_mean->GetYaxis()->SetRangeUser(0.0,1.01);
    turnOn_NewLayer1_noIso_mean->SetTitle("");
    turnOn_NewLayer1_noIso_mean->GetYaxis()->SetTitle("Efficiency");
    turnOn_NewLayer1_noIso_mean->GetXaxis()->SetTitle("Offline p_{T}(#tau) [GeV]");
    turnOn_NewLayer1_noIso_mean->Draw();

    turnOn_NewLayer1_OptionA_mean->SetLineColor(kRed);
    turnOn_NewLayer1_OptionA_mean->SetLineWidth(1);
    if(thrInt_OptionA>=minThrAllowed && thrInt_OptionA<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionA_mean) && has100Plateau(turnOn_NewLayer1_OptionA_mean)) { turnOn_NewLayer1_OptionA_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionB_mean->SetLineColor(kRed-4);
    turnOn_NewLayer1_OptionB_mean->SetLineWidth(1);
    if(thrInt_OptionB>=minThrAllowed && thrInt_OptionB<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionB_mean) && has100Plateau(turnOn_NewLayer1_OptionB_mean)) { turnOn_NewLayer1_OptionB_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionC_mean->SetLineColor(kRed-7);
    turnOn_NewLayer1_OptionC_mean->SetLineWidth(1);
    if(thrInt_OptionC>=minThrAllowed && thrInt_OptionC<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionC_mean) && has100Plateau(turnOn_NewLayer1_OptionC_mean)) { turnOn_NewLayer1_OptionC_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionD_mean->SetLineColor(kRed-9);
    turnOn_NewLayer1_OptionD_mean->SetLineWidth(1);
    if(thrInt_OptionD>=minThrAllowed && thrInt_OptionD<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionD_mean) && has100Plateau(turnOn_NewLayer1_OptionD_mean)) { turnOn_NewLayer1_OptionD_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionE_mean->SetLineColor(kRed-10);
    turnOn_NewLayer1_OptionE_mean->SetLineWidth(1);
    if(thrInt_OptionE>=minThrAllowed && thrInt_OptionE<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionE_mean) && has100Plateau(turnOn_NewLayer1_OptionE_mean)) { turnOn_NewLayer1_OptionE_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionF_mean->SetLineColor(kMagenta);
    turnOn_NewLayer1_OptionF_mean->SetLineWidth(1);
    if(thrInt_OptionF>=minThrAllowed && thrInt_OptionF<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionF_mean) && has100Plateau(turnOn_NewLayer1_OptionF_mean)) { turnOn_NewLayer1_OptionF_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionG_mean->SetLineColor(kMagenta-4);
    turnOn_NewLayer1_OptionG_mean->SetLineWidth(1);
    if(thrInt_OptionG>=minThrAllowed && thrInt_OptionG<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionG_mean) && has100Plateau(turnOn_NewLayer1_OptionG_mean)) { turnOn_NewLayer1_OptionG_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionH_mean->SetLineColor(kMagenta-7);
    turnOn_NewLayer1_OptionH_mean->SetLineWidth(1);
    if(thrInt_OptionH>=minThrAllowed && thrInt_OptionH<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionH_mean) && has100Plateau(turnOn_NewLayer1_OptionH_mean)) { turnOn_NewLayer1_OptionH_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionI_mean->SetLineColor(kMagenta-9);
    turnOn_NewLayer1_OptionI_mean->SetLineWidth(1);
    if(thrInt_OptionI>=minThrAllowed && thrInt_OptionI<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionI_mean) && has100Plateau(turnOn_NewLayer1_OptionI_mean)) { turnOn_NewLayer1_OptionI_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionJ_mean->SetLineColor(kMagenta-10);
    turnOn_NewLayer1_OptionJ_mean->SetLineWidth(1);
    if(thrInt_OptionJ>=minThrAllowed && thrInt_OptionJ<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionJ_mean) && has100Plateau(turnOn_NewLayer1_OptionJ_mean)) { turnOn_NewLayer1_OptionJ_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionK_mean->SetLineColor(kBlue);
    turnOn_NewLayer1_OptionK_mean->SetLineWidth(1);
    if(thrInt_OptionK>=minThrAllowed && thrInt_OptionK<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionK_mean) && has100Plateau(turnOn_NewLayer1_OptionK_mean)) { turnOn_NewLayer1_OptionK_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionL_mean->SetLineColor(kBlue);
    turnOn_NewLayer1_OptionL_mean->SetLineWidth(1);
    if(thrInt_OptionL>=minThrAllowed && thrInt_OptionL<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionL_mean) && has100Plateau(turnOn_NewLayer1_OptionL_mean)) { turnOn_NewLayer1_OptionL_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionM_mean->SetLineColor(kBlue-7);
    turnOn_NewLayer1_OptionM_mean->SetLineWidth(1);
    if(thrInt_OptionM>=minThrAllowed && thrInt_OptionM<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionM_mean) && has100Plateau(turnOn_NewLayer1_OptionM_mean)) { turnOn_NewLayer1_OptionM_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionN_mean->SetLineColor(kBlue-9);
    turnOn_NewLayer1_OptionN_mean->SetLineWidth(1);
    if(thrInt_OptionN>=minThrAllowed && thrInt_OptionN<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionN_mean) && has100Plateau(turnOn_NewLayer1_OptionN_mean)) { turnOn_NewLayer1_OptionN_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionO_mean->SetLineColor(kBlue-10);
    turnOn_NewLayer1_OptionO_mean->SetLineWidth(1);
    if(thrInt_OptionO>=minThrAllowed && thrInt_OptionO<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionO_mean) && has100Plateau(turnOn_NewLayer1_OptionO_mean)) { turnOn_NewLayer1_OptionO_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionP_mean->SetLineColor(kCyan);
    turnOn_NewLayer1_OptionP_mean->SetLineWidth(1);
    if(thrInt_OptionP>=minThrAllowed && thrInt_OptionP<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionP_mean) && has100Plateau(turnOn_NewLayer1_OptionP_mean)) { turnOn_NewLayer1_OptionP_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionQ_mean->SetLineColor(kCyan-4);
    turnOn_NewLayer1_OptionQ_mean->SetLineWidth(1);
    if(thrInt_OptionQ>=minThrAllowed && thrInt_OptionQ<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionQ_mean) && has100Plateau(turnOn_NewLayer1_OptionQ_mean)) { turnOn_NewLayer1_OptionQ_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionR_mean->SetLineColor(kCyan-7);
    turnOn_NewLayer1_OptionR_mean->SetLineWidth(1);
    if(thrInt_OptionR>=minThrAllowed && thrInt_OptionR<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionR_mean) && has100Plateau(turnOn_NewLayer1_OptionR_mean)) { turnOn_NewLayer1_OptionR_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionS_mean->SetLineColor(kCyan-9);
    turnOn_NewLayer1_OptionS_mean->SetLineWidth(1);
    if(thrInt_OptionS>=minThrAllowed && thrInt_OptionS<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionS_mean) && has100Plateau(turnOn_NewLayer1_OptionS_mean)) { turnOn_NewLayer1_OptionS_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionT_mean->SetLineColor(kCyan-10);
    turnOn_NewLayer1_OptionT_mean->SetLineWidth(1);
    if(thrInt_OptionT>=minThrAllowed && thrInt_OptionT<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionT_mean) && has100Plateau(turnOn_NewLayer1_OptionT_mean)) { turnOn_NewLayer1_OptionT_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionU_mean->SetLineColor(kGreen);
    turnOn_NewLayer1_OptionU_mean->SetLineWidth(1);
    if(thrInt_OptionU>=minThrAllowed && thrInt_OptionU<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionU_mean) && has100Plateau(turnOn_NewLayer1_OptionU_mean)) { turnOn_NewLayer1_OptionU_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionV_mean->SetLineColor(kGreen-4);
    turnOn_NewLayer1_OptionV_mean->SetLineWidth(1);
    if(thrInt_OptionV>=minThrAllowed && thrInt_OptionV<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionV_mean) && has100Plateau(turnOn_NewLayer1_OptionV_mean)) { turnOn_NewLayer1_OptionV_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionW_mean->SetLineColor(kGreen-7);
    turnOn_NewLayer1_OptionW_mean->SetLineWidth(1);
    if(thrInt_OptionW>=minThrAllowed && thrInt_OptionW<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionW_mean) && has100Plateau(turnOn_NewLayer1_OptionW_mean)) { turnOn_NewLayer1_OptionW_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionX_mean->SetLineColor(kGreen-9);
    turnOn_NewLayer1_OptionX_mean->SetLineWidth(1);
    if(thrInt_OptionX>=minThrAllowed && thrInt_OptionX<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionX_mean) && has100Plateau(turnOn_NewLayer1_OptionX_mean)) { turnOn_NewLayer1_OptionX_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionY_mean->SetLineColor(kGreen-10);
    turnOn_NewLayer1_OptionY_mean->SetLineWidth(1);
    if(thrInt_OptionY>=minThrAllowed && thrInt_OptionY<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionY_mean) && has100Plateau(turnOn_NewLayer1_OptionY_mean)) { turnOn_NewLayer1_OptionY_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_OptionZ_mean->SetLineColor(kYellow);
    turnOn_NewLayer1_OptionZ_mean->SetLineWidth(1);
    if(thrInt_OptionZ>=minThrAllowed && thrInt_OptionZ<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionZ_mean) && has100Plateau(turnOn_NewLayer1_OptionZ_mean)) { turnOn_NewLayer1_OptionZ_mean->Draw("same"); drewSth = true; }

    turnOn_NewLayer1_Option22_mean->SetLineColor(kGray+1);
    turnOn_NewLayer1_Option22_mean->SetLineWidth(1);
    turnOn_NewLayer1_Option22_mean->Draw("same");

    turnOn_NewLayer1_Option31_extrap_mean->SetLineColor(kGray+2);
    turnOn_NewLayer1_Option31_extrap_mean->SetLineWidth(1);
    turnOn_NewLayer1_Option31_extrap_mean->Draw("same");

    TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
    texl->AddText("CMS Internal, #sqrt{s}=13 TeV, Run #"+run_str+" (2018)");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    texl->Draw("same");

    TLegend* leg = new TLegend(0.5,0.15,0.89,0.62);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);
    leg->SetHeader("Linearly scaled to 2.0E34");

    leg->AddEntry(turnOn_NewLayer1_noIso_mean,"Di-#tau no-iso, mean","L");

    if(thrInt_OptionA>=minThrAllowed && thrInt_OptionA<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionA_mean) && has100Plateau(turnOn_NewLayer1_OptionA_mean)) leg->AddEntry(turnOn_NewLayer1_OptionA_mean,"Di-#tau iso (Option A"+opt+") - THR="+thr_OptionA,"L");
    if(thrInt_OptionB>=minThrAllowed && thrInt_OptionB<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionB_mean) && has100Plateau(turnOn_NewLayer1_OptionB_mean)) leg->AddEntry(turnOn_NewLayer1_OptionB_mean,"Di-#tau iso (Option B"+opt+") - THR="+thr_OptionB,"L");
    if(thrInt_OptionC>=minThrAllowed && thrInt_OptionC<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionC_mean) && has100Plateau(turnOn_NewLayer1_OptionC_mean)) leg->AddEntry(turnOn_NewLayer1_OptionC_mean,"Di-#tau iso (Option C"+opt+") - THR="+thr_OptionC,"L");
    if(thrInt_OptionD>=minThrAllowed && thrInt_OptionD<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionD_mean) && has100Plateau(turnOn_NewLayer1_OptionD_mean)) leg->AddEntry(turnOn_NewLayer1_OptionD_mean,"Di-#tau iso (Option D"+opt+") - THR="+thr_OptionD,"L");
    if(thrInt_OptionE>=minThrAllowed && thrInt_OptionE<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionE_mean) && has100Plateau(turnOn_NewLayer1_OptionE_mean)) leg->AddEntry(turnOn_NewLayer1_OptionE_mean,"Di-#tau iso (Option E"+opt+") - THR="+thr_OptionE,"L");
    if(thrInt_OptionF>=minThrAllowed && thrInt_OptionF<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionF_mean) && has100Plateau(turnOn_NewLayer1_OptionF_mean)) leg->AddEntry(turnOn_NewLayer1_OptionF_mean,"Di-#tau iso (Option F"+opt+") - THR="+thr_OptionF,"L");
    if(thrInt_OptionG>=minThrAllowed && thrInt_OptionG<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionG_mean) && has100Plateau(turnOn_NewLayer1_OptionG_mean)) leg->AddEntry(turnOn_NewLayer1_OptionG_mean,"Di-#tau iso (Option G"+opt+") - THR="+thr_OptionG,"L");
    if(thrInt_OptionH>=minThrAllowed && thrInt_OptionH<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionH_mean) && has100Plateau(turnOn_NewLayer1_OptionH_mean)) leg->AddEntry(turnOn_NewLayer1_OptionH_mean,"Di-#tau iso (Option H"+opt+") - THR="+thr_OptionH,"L");
    if(thrInt_OptionI>=minThrAllowed && thrInt_OptionI<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionI_mean) && has100Plateau(turnOn_NewLayer1_OptionI_mean)) leg->AddEntry(turnOn_NewLayer1_OptionI_mean,"Di-#tau iso (Option I"+opt+") - THR="+thr_OptionI,"L");
    if(thrInt_OptionJ>=minThrAllowed && thrInt_OptionJ<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionJ_mean) && has100Plateau(turnOn_NewLayer1_OptionJ_mean)) leg->AddEntry(turnOn_NewLayer1_OptionJ_mean,"Di-#tau iso (Option J"+opt+") - THR="+thr_OptionJ,"L");
    if(thrInt_OptionK>=minThrAllowed && thrInt_OptionK<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionK_mean) && has100Plateau(turnOn_NewLayer1_OptionK_mean)) leg->AddEntry(turnOn_NewLayer1_OptionK_mean,"Di-#tau iso (Option K"+opt+") - THR="+thr_OptionK,"L");
    if(thrInt_OptionL>=minThrAllowed && thrInt_OptionL<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionL_mean) && has100Plateau(turnOn_NewLayer1_OptionL_mean)) leg->AddEntry(turnOn_NewLayer1_OptionL_mean,"Di-#tau iso (Option L"+opt+") - THR="+thr_OptionL,"L");
    if(thrInt_OptionM>=minThrAllowed && thrInt_OptionM<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionM_mean) && has100Plateau(turnOn_NewLayer1_OptionM_mean)) leg->AddEntry(turnOn_NewLayer1_OptionM_mean,"Di-#tau iso (Option M"+opt+") - THR="+thr_OptionM,"L");
    if(thrInt_OptionN>=minThrAllowed && thrInt_OptionN<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionN_mean) && has100Plateau(turnOn_NewLayer1_OptionN_mean)) leg->AddEntry(turnOn_NewLayer1_OptionN_mean,"Di-#tau iso (Option N"+opt+") - THR="+thr_OptionN,"L");
    if(thrInt_OptionO>=minThrAllowed && thrInt_OptionO<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionO_mean) && has100Plateau(turnOn_NewLayer1_OptionO_mean)) leg->AddEntry(turnOn_NewLayer1_OptionO_mean,"Di-#tau iso (Option O"+opt+") - THR="+thr_OptionO,"L");
    if(thrInt_OptionP>=minThrAllowed && thrInt_OptionP<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionP_mean) && has100Plateau(turnOn_NewLayer1_OptionP_mean)) leg->AddEntry(turnOn_NewLayer1_OptionP_mean,"Di-#tau iso (Option P"+opt+") - THR="+thr_OptionP,"L");
    if(thrInt_OptionQ>=minThrAllowed && thrInt_OptionQ<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionQ_mean) && has100Plateau(turnOn_NewLayer1_OptionQ_mean)) leg->AddEntry(turnOn_NewLayer1_OptionQ_mean,"Di-#tau iso (Option Q"+opt+") - THR="+thr_OptionQ,"L");
    if(thrInt_OptionR>=minThrAllowed && thrInt_OptionR<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionR_mean) && has100Plateau(turnOn_NewLayer1_OptionR_mean)) leg->AddEntry(turnOn_NewLayer1_OptionR_mean,"Di-#tau iso (Option R"+opt+") - THR="+thr_OptionR,"L");
    if(thrInt_OptionS>=minThrAllowed && thrInt_OptionS<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionS_mean) && has100Plateau(turnOn_NewLayer1_OptionS_mean)) leg->AddEntry(turnOn_NewLayer1_OptionS_mean,"Di-#tau iso (Option S"+opt+") - THR="+thr_OptionS,"L");
    if(thrInt_OptionT>=minThrAllowed && thrInt_OptionT<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionT_mean) && has100Plateau(turnOn_NewLayer1_OptionT_mean)) leg->AddEntry(turnOn_NewLayer1_OptionT_mean,"Di-#tau iso (Option T"+opt+") - THR="+thr_OptionT,"L");
    if(thrInt_OptionU>=minThrAllowed && thrInt_OptionU<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionU_mean) && has100Plateau(turnOn_NewLayer1_OptionU_mean)) leg->AddEntry(turnOn_NewLayer1_OptionU_mean,"Di-#tau iso (Option U"+opt+") - THR="+thr_OptionU,"L");
    if(thrInt_OptionV>=minThrAllowed && thrInt_OptionV<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionV_mean) && has100Plateau(turnOn_NewLayer1_OptionV_mean)) leg->AddEntry(turnOn_NewLayer1_OptionV_mean,"Di-#tau iso (Option V"+opt+") - THR="+thr_OptionV,"L");
    if(thrInt_OptionW>=minThrAllowed && thrInt_OptionW<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionW_mean) && has100Plateau(turnOn_NewLayer1_OptionW_mean)) leg->AddEntry(turnOn_NewLayer1_OptionW_mean,"Di-#tau iso (Option W"+opt+") - THR="+thr_OptionW,"L");
    if(thrInt_OptionX>=minThrAllowed && thrInt_OptionX<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionX_mean) && has100Plateau(turnOn_NewLayer1_OptionX_mean)) leg->AddEntry(turnOn_NewLayer1_OptionX_mean,"Di-#tau iso (Option X"+opt+") - THR="+thr_OptionX,"L");
    if(thrInt_OptionY>=minThrAllowed && thrInt_OptionY<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionY_mean) && has100Plateau(turnOn_NewLayer1_OptionY_mean)) leg->AddEntry(turnOn_NewLayer1_OptionY_mean,"Di-#tau iso (Option Y"+opt+") - THR="+thr_OptionY,"L");
    if(thrInt_OptionZ>=minThrAllowed && thrInt_OptionZ<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionZ_mean) && has100Plateau(turnOn_NewLayer1_OptionZ_mean)) leg->AddEntry(turnOn_NewLayer1_OptionZ_mean,"Di-#tau iso (Option Z"+opt+") - THR="+thr_OptionZ,"L");

    leg->AddEntry(turnOn_NewLayer1_Option22_mean,"Di-#tau iso (Option 22)","L");
    leg->AddEntry(turnOn_NewLayer1_Option31_extrap_mean,"Di-#tau iso (Option 31 extrap)","L");

    leg->Draw("same");

    TString PDFs = "PDFs/optimizationV11/";
    if(drewSth) c.SaveAs(PDFs+CanvasNamePdf.Data());

    if (tag=="effMin0p0" and opt=="0") {
        drewSth = false; //re-initialize

        TString cName = "Comparison_TurnOn";
        if (run==-1)     cName += "_SingleNeutrino112XpuReweighted";
        else             cName += "_Run"+run_str;
        cName += "_newnTT_unpacked_optimizationV11gs";
        if (fixedThr==0) cName += "_FIXEDRATE"+fixedRate+"kHz";
        else             cName += "_FIXEDTHR"+fixedThreshold+"GeV";
        cName += "_calibThr"+intgr+"p"+decim+"_baseOptions";
        cName += "_bounds"+to_string(minThrAllowed)+"THR"+to_string(maxThrAllowed);
        TString cNamePdf = cName ;
        cNamePdf += ".pdf";

        TCanvas c1(cName.Data(),cName.Data(),650,600);
        c1.SetLeftMargin(0.15);
        c1.SetGrid();
        c1.SetLogy();

        TPad *pad1a = new TPad("pad1a", "pad1a", 0, 0., 1, 1.0);
        //pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
        pad1a->SetLeftMargin(0.15);
        pad1a->SetGridx();         // Vertical grid
        pad1a->SetGridy();         // Vertical grid
        pad1a->Draw();             // Draw the upper pad: pad1
        pad1a->cd();               // pad1 becomes the current pad
        //pad1->SetLogy();

        turnOn_NewLayer1_noIso_mean->SetLineColor(kBlack);
        turnOn_NewLayer1_noIso_mean->SetLineWidth(1);
        turnOn_NewLayer1_noIso_mean->GetXaxis()->SetLimits(15,120.);
        turnOn_NewLayer1_noIso_mean->GetYaxis()->SetRangeUser(0.0,1.01);
        turnOn_NewLayer1_noIso_mean->SetTitle("");
        turnOn_NewLayer1_noIso_mean->GetYaxis()->SetTitle("Efficiency");
        turnOn_NewLayer1_noIso_mean->GetXaxis()->SetTitle("Offline p_{T}(#tau) [GeV]");
        turnOn_NewLayer1_noIso_mean->Draw();

        turnOn_NewLayer1_Option1_mean->SetLineColor(kRed);
        turnOn_NewLayer1_Option1_mean->SetLineWidth(1);
        if(thrInt_Option1>=minThrAllowed && thrInt_Option1<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option1_mean) && has100Plateau(turnOn_NewLayer1_Option1_mean)) { turnOn_NewLayer1_Option1_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option2_mean->SetLineColor(kRed-4);
        turnOn_NewLayer1_Option2_mean->SetLineWidth(1);
        if(thrInt_Option2>=minThrAllowed && thrInt_Option2<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option2_mean) && has100Plateau(turnOn_NewLayer1_Option2_mean)) { turnOn_NewLayer1_Option2_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option3_mean->SetLineColor(kRed-7);
        turnOn_NewLayer1_Option3_mean->SetLineWidth(1);
        if(thrInt_Option3>=minThrAllowed && thrInt_Option3<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option3_mean) && has100Plateau(turnOn_NewLayer1_Option3_mean)) { turnOn_NewLayer1_Option3_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option4_mean->SetLineColor(kRed-9);
        turnOn_NewLayer1_Option4_mean->SetLineWidth(1);
        if(thrInt_Option4>=minThrAllowed && thrInt_Option4<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option4_mean) && has100Plateau(turnOn_NewLayer1_Option4_mean)) { turnOn_NewLayer1_Option4_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option5_mean->SetLineColor(kRed-10);
        turnOn_NewLayer1_Option5_mean->SetLineWidth(1);
        if(thrInt_Option5>=minThrAllowed && thrInt_Option5<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option5_mean) && has100Plateau(turnOn_NewLayer1_Option5_mean)) { turnOn_NewLayer1_Option5_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option6_mean->SetLineColor(kMagenta);
        turnOn_NewLayer1_Option6_mean->SetLineWidth(1);
        if(thrInt_Option6>=minThrAllowed && thrInt_Option6<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option6_mean) && has100Plateau(turnOn_NewLayer1_Option6_mean)) { turnOn_NewLayer1_Option6_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option7_mean->SetLineColor(kMagenta-4);
        turnOn_NewLayer1_Option7_mean->SetLineWidth(1);
        if(thrInt_Option7>=minThrAllowed && thrInt_Option7<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option7_mean) && has100Plateau(turnOn_NewLayer1_Option7_mean)) { turnOn_NewLayer1_Option7_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option8_mean->SetLineColor(kMagenta-7);
        turnOn_NewLayer1_Option8_mean->SetLineWidth(1);
        if(thrInt_Option8>=minThrAllowed && thrInt_Option8<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option8_mean) && has100Plateau(turnOn_NewLayer1_Option8_mean)) { turnOn_NewLayer1_Option8_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option9_mean->SetLineColor(kMagenta-9);
        turnOn_NewLayer1_Option9_mean->SetLineWidth(1);
        if(thrInt_Option9>=minThrAllowed && thrInt_Option9<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option9_mean) && has100Plateau(turnOn_NewLayer1_Option9_mean)) { turnOn_NewLayer1_Option9_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option10_mean->SetLineColor(kMagenta-10);
        turnOn_NewLayer1_Option10_mean->SetLineWidth(1);
        if(thrInt_Option10>=minThrAllowed && thrInt_Option10<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option10_mean) && has100Plateau(turnOn_NewLayer1_Option10_mean)) { turnOn_NewLayer1_Option10_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option11_mean->SetLineColor(kBlue);
        turnOn_NewLayer1_Option11_mean->SetLineWidth(1);
        if(thrInt_Option11>=minThrAllowed && thrInt_Option11<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option11_mean) && has100Plateau(turnOn_NewLayer1_Option11_mean)) { turnOn_NewLayer1_Option11_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_OptionL_mean->SetLineColor(kBlue);
        turnOn_NewLayer1_OptionL_mean->SetLineWidth(1);
        if(thrInt_OptionL>=minThrAllowed && thrInt_OptionL<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionL_mean) && has100Plateau(turnOn_NewLayer1_OptionL_mean)) { turnOn_NewLayer1_Option12_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option13_mean->SetLineColor(kBlue-7);
        turnOn_NewLayer1_Option13_mean->SetLineWidth(1);
        if(thrInt_Option13>=minThrAllowed && thrInt_Option13<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option13_mean) && has100Plateau(turnOn_NewLayer1_Option13_mean)) { turnOn_NewLayer1_Option13_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option14_mean->SetLineColor(kBlue-9);
        turnOn_NewLayer1_Option14_mean->SetLineWidth(1);
        if(thrInt_Option14>=minThrAllowed && thrInt_Option14<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option14_mean) && has100Plateau(turnOn_NewLayer1_Option14_mean)) { turnOn_NewLayer1_Option14_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option15_mean->SetLineColor(kBlue-10);
        turnOn_NewLayer1_Option15_mean->SetLineWidth(1);
        if(thrInt_Option15>=minThrAllowed && thrInt_Option15<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option15_mean) && has100Plateau(turnOn_NewLayer1_Option15_mean)) { turnOn_NewLayer1_Option15_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option16_mean->SetLineColor(kCyan);
        turnOn_NewLayer1_Option16_mean->SetLineWidth(1);
        if(thrInt_Option16>=minThrAllowed && thrInt_Option16<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option16_mean) && has100Plateau(turnOn_NewLayer1_Option16_mean)) { turnOn_NewLayer1_Option16_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option17_mean->SetLineColor(kCyan-4);
        turnOn_NewLayer1_Option17_mean->SetLineWidth(1);
        if(thrInt_Option17>=minThrAllowed && thrInt_Option17<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option17_mean) && has100Plateau(turnOn_NewLayer1_Option17_mean)) { turnOn_NewLayer1_Option17_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option18_mean->SetLineColor(kCyan-7);
        turnOn_NewLayer1_Option18_mean->SetLineWidth(1);
        if(thrInt_Option18>=minThrAllowed && thrInt_Option18<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option18_mean) && has100Plateau(turnOn_NewLayer1_Option18_mean)) { turnOn_NewLayer1_Option18_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option19_mean->SetLineColor(kCyan-9);
        turnOn_NewLayer1_Option19_mean->SetLineWidth(1);
        if(thrInt_Option19>=minThrAllowed && thrInt_Option19<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option19_mean) && has100Plateau(turnOn_NewLayer1_Option19_mean)) { turnOn_NewLayer1_Option19_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option20_mean->SetLineColor(kCyan-10);
        turnOn_NewLayer1_Option20_mean->SetLineWidth(1);
        if(thrInt_Option20>=minThrAllowed && thrInt_Option20<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option20_mean) && has100Plateau(turnOn_NewLayer1_Option20_mean)) { turnOn_NewLayer1_Option20_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option21_mean->SetLineColor(kGreen);
        turnOn_NewLayer1_Option21_mean->SetLineWidth(1);
        if(thrInt_Option21>=minThrAllowed && thrInt_Option21<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option21_mean) && has100Plateau(turnOn_NewLayer1_Option21_mean)) { turnOn_NewLayer1_Option21_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option22_mean->SetLineColor(kGreen-4);
        turnOn_NewLayer1_Option22_mean->SetLineWidth(1);
        if(thrInt_Option22>=minThrAllowed && thrInt_Option22<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option22_mean) && has100Plateau(turnOn_NewLayer1_Option22_mean)) { turnOn_NewLayer1_Option22_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_OptionW_mean->SetLineColor(kGreen-7);
        turnOn_NewLayer1_OptionW_mean->SetLineWidth(1);
        if(thrInt_OptionW>=minThrAllowed && thrInt_OptionW<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_OptionW_mean) && has100Plateau(turnOn_NewLayer1_OptionW_mean)) { turnOn_NewLayer1_Option23_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option24_mean->SetLineColor(kGreen-9);
        turnOn_NewLayer1_Option24_mean->SetLineWidth(1);
        if(thrInt_Option24>=minThrAllowed && thrInt_Option24<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option24_mean) && has100Plateau(turnOn_NewLayer1_Option24_mean)) { turnOn_NewLayer1_Option24_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option25_mean->SetLineColor(kGreen-10);
        turnOn_NewLayer1_Option25_mean->SetLineWidth(1);
        if(thrInt_Option25>=minThrAllowed && thrInt_Option25<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option25_mean) && has100Plateau(turnOn_NewLayer1_Option25_mean)) { turnOn_NewLayer1_Option25_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option26_mean->SetLineColor(kGray);
        turnOn_NewLayer1_Option26_mean->SetLineWidth(1);
        if(thrInt_Option26>=minThrAllowed && thrInt_Option26<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option26_mean) && has100Plateau(turnOn_NewLayer1_Option26_mean)) { turnOn_NewLayer1_Option26_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option27_mean->SetLineColor(kGray-4);
        turnOn_NewLayer1_Option27_mean->SetLineWidth(1);
        if(thrInt_Option27>=minThrAllowed && thrInt_Option27<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option27_mean) && has100Plateau(turnOn_NewLayer1_Option27_mean)) { turnOn_NewLayer1_Option27_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option28_mean->SetLineColor(kGray-7);
        turnOn_NewLayer1_Option28_mean->SetLineWidth(1);
        if(thrInt_Option28>=minThrAllowed && thrInt_Option28<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option28_mean) && has100Plateau(turnOn_NewLayer1_Option28_mean)) { turnOn_NewLayer1_Option28_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option29_mean->SetLineColor(kGray-9);
        turnOn_NewLayer1_Option29_mean->SetLineWidth(1);
        if(thrInt_Option29>=minThrAllowed && thrInt_Option29<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option29_mean) && has100Plateau(turnOn_NewLayer1_Option29_mean)) { turnOn_NewLayer1_Option29_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option30_mean->SetLineColor(kGray-10);
        turnOn_NewLayer1_Option30_mean->SetLineWidth(1);
        if(thrInt_Option30>=minThrAllowed && thrInt_Option30<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option30_mean) && has100Plateau(turnOn_NewLayer1_Option30_mean)) { turnOn_NewLayer1_Option30_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option31_mean->SetLineColor(kYellow);
        turnOn_NewLayer1_Option31_mean->SetLineWidth(1);
        if(thrInt_Option31>=minThrAllowed && thrInt_Option31<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option31_mean) && has100Plateau(turnOn_NewLayer1_Option31_mean)) { turnOn_NewLayer1_Option31_mean->Draw("same"); drewSth = true; }

        turnOn_NewLayer1_Option31_extrap_mean->SetLineColor(kYellow-10);
        turnOn_NewLayer1_Option31_extrap_mean->SetLineWidth(1);
        if(thrInt_Option31_extrap>=minThrAllowed && thrInt_Option31_extrap<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option31_extrap_mean) && has100Plateau(turnOn_NewLayer1_Option31_extrap_mean)) { turnOn_NewLayer1_Option31_extrap_mean->Draw("same"); drewSth = true; }

        TPaveText* texl1 = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
        texl1->AddText("CMS Internal, #sqrt{s}=13 TeV, Run #"+run_str+" (2018)");
        texl1->SetTextSize(0.04);
        texl1->SetFillStyle(0);
        texl1->SetBorderSize(0);
        texl1->Draw("same");

        TLegend* leg1 = new TLegend(0.5,0.15,0.89,0.62);
        leg1->SetBorderSize(0);
        leg1->SetTextSize(0.02);
        leg1->SetHeader("Linearly scaled to 2.0E34");

        leg1->AddEntry(turnOn_NewLayer1_noIso_mean,"Di-#tau no-iso, mean","L");

        if(thrInt_Option1>=minThrAllowed && thrInt_Option1<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option1_mean) && has100Plateau(turnOn_NewLayer1_Option1_mean)) leg1->AddEntry(turnOn_NewLayer1_Option1_mean,"Di-#tau iso (Option 1) - THR"+thr_Option1, "L");
        if(thrInt_Option2>=minThrAllowed && thrInt_Option2<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option2_mean) && has100Plateau(turnOn_NewLayer1_Option2_mean)) leg1->AddEntry(turnOn_NewLayer1_Option2_mean,"Di-#tau iso (Option 2) - THR"+thr_Option2, "L");
        if(thrInt_Option3>=minThrAllowed && thrInt_Option3<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option3_mean) && has100Plateau(turnOn_NewLayer1_Option3_mean)) leg1->AddEntry(turnOn_NewLayer1_Option3_mean,"Di-#tau iso (Option 3) - THR"+thr_Option3, "L");
        if(thrInt_Option4>=minThrAllowed && thrInt_Option4<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option4_mean) && has100Plateau(turnOn_NewLayer1_Option4_mean)) leg1->AddEntry(turnOn_NewLayer1_Option4_mean,"Di-#tau iso (Option 4) - THR"+thr_Option4, "L");
        if(thrInt_Option5>=minThrAllowed && thrInt_Option5<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option5_mean) && has100Plateau(turnOn_NewLayer1_Option5_mean)) leg1->AddEntry(turnOn_NewLayer1_Option5_mean,"Di-#tau iso (Option 5) - THR"+thr_Option5, "L");
        if(thrInt_Option6>=minThrAllowed && thrInt_Option6<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option6_mean) && has100Plateau(turnOn_NewLayer1_Option6_mean)) leg1->AddEntry(turnOn_NewLayer1_Option6_mean,"Di-#tau iso (Option 6) - THR"+thr_Option6, "L");
        if(thrInt_Option7>=minThrAllowed && thrInt_Option7<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option7_mean) && has100Plateau(turnOn_NewLayer1_Option7_mean)) leg1->AddEntry(turnOn_NewLayer1_Option7_mean,"Di-#tau iso (Option 7) - THR"+thr_Option7, "L");
        if(thrInt_Option8>=minThrAllowed && thrInt_Option8<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option8_mean) && has100Plateau(turnOn_NewLayer1_Option8_mean)) leg1->AddEntry(turnOn_NewLayer1_Option8_mean,"Di-#tau iso (Option 8) - THR"+thr_Option8, "L");
        if(thrInt_Option9>=minThrAllowed && thrInt_Option9<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option9_mean) && has100Plateau(turnOn_NewLayer1_Option9_mean)) leg1->AddEntry(turnOn_NewLayer1_Option9_mean,"Di-#tau iso (Option 9) - THR"+thr_Option9, "L");
        if(thrInt_Option10>=minThrAllowed && thrInt_Option10<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option10_mean) && has100Plateau(turnOn_NewLayer1_Option10_mean)) leg1->AddEntry(turnOn_NewLayer1_Option10_mean,"Di-#tau iso (Option 10) - THR"+thr_Option10, "L");
        if(thrInt_Option11>=minThrAllowed && thrInt_Option11<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option11_mean) && has100Plateau(turnOn_NewLayer1_Option11_mean)) leg1->AddEntry(turnOn_NewLayer1_Option11_mean,"Di-#tau iso (Option 11) - THR"+thr_Option11, "L");
        if(thrInt_Option13>=minThrAllowed && thrInt_Option13<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option13_mean) && has100Plateau(turnOn_NewLayer1_Option13_mean)) leg1->AddEntry(turnOn_NewLayer1_Option13_mean,"Di-#tau iso (Option 13) - THR"+thr_Option13, "L");
        if(thrInt_Option14>=minThrAllowed && thrInt_Option14<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option14_mean) && has100Plateau(turnOn_NewLayer1_Option14_mean)) leg1->AddEntry(turnOn_NewLayer1_Option14_mean,"Di-#tau iso (Option 14) - THR"+thr_Option14, "L");
        if(thrInt_Option15>=minThrAllowed && thrInt_Option15<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option15_mean) && has100Plateau(turnOn_NewLayer1_Option15_mean)) leg1->AddEntry(turnOn_NewLayer1_Option15_mean,"Di-#tau iso (Option 15) - THR"+thr_Option15, "L");
        if(thrInt_Option16>=minThrAllowed && thrInt_Option16<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option16_mean) && has100Plateau(turnOn_NewLayer1_Option16_mean)) leg1->AddEntry(turnOn_NewLayer1_Option16_mean,"Di-#tau iso (Option 16) - THR"+thr_Option16, "L");
        if(thrInt_Option17>=minThrAllowed && thrInt_Option17<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option17_mean) && has100Plateau(turnOn_NewLayer1_Option17_mean)) leg1->AddEntry(turnOn_NewLayer1_Option17_mean,"Di-#tau iso (Option 17) - THR"+thr_Option17, "L");
        if(thrInt_Option18>=minThrAllowed && thrInt_Option18<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option18_mean) && has100Plateau(turnOn_NewLayer1_Option18_mean)) leg1->AddEntry(turnOn_NewLayer1_Option18_mean,"Di-#tau iso (Option 18) - THR"+thr_Option18, "L");
        if(thrInt_Option19>=minThrAllowed && thrInt_Option19<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option19_mean) && has100Plateau(turnOn_NewLayer1_Option19_mean)) leg1->AddEntry(turnOn_NewLayer1_Option19_mean,"Di-#tau iso (Option 19) - THR"+thr_Option19, "L");
        if(thrInt_Option20>=minThrAllowed && thrInt_Option20<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option20_mean) && has100Plateau(turnOn_NewLayer1_Option20_mean)) leg1->AddEntry(turnOn_NewLayer1_Option20_mean,"Di-#tau iso (Option 20) - THR"+thr_Option20, "L");
        if(thrInt_Option21>=minThrAllowed && thrInt_Option21<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option21_mean) && has100Plateau(turnOn_NewLayer1_Option21_mean)) leg1->AddEntry(turnOn_NewLayer1_Option21_mean,"Di-#tau iso (Option 21) - THR"+thr_Option21, "L");
        if(thrInt_Option22>=minThrAllowed && thrInt_Option22<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option22_mean) && has100Plateau(turnOn_NewLayer1_Option22_mean)) leg1->AddEntry(turnOn_NewLayer1_Option22_mean,"Di-#tau iso (Option 22) - THR"+thr_Option22, "L");
        if(thrInt_Option24>=minThrAllowed && thrInt_Option24<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option24_mean) && has100Plateau(turnOn_NewLayer1_Option24_mean)) leg1->AddEntry(turnOn_NewLayer1_Option24_mean,"Di-#tau iso (Option 24) - THR"+thr_Option24, "L");
        if(thrInt_Option25>=minThrAllowed && thrInt_Option25<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option25_mean) && has100Plateau(turnOn_NewLayer1_Option25_mean)) leg1->AddEntry(turnOn_NewLayer1_Option25_mean,"Di-#tau iso (Option 25) - THR"+thr_Option25, "L");
        if(thrInt_Option26>=minThrAllowed && thrInt_Option26<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option26_mean) && has100Plateau(turnOn_NewLayer1_Option26_mean)) leg1->AddEntry(turnOn_NewLayer1_Option26_mean,"Di-#tau iso (Option 26) - THR"+thr_Option26, "L");
        if(thrInt_Option27>=minThrAllowed && thrInt_Option27<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option27_mean) && has100Plateau(turnOn_NewLayer1_Option27_mean)) leg1->AddEntry(turnOn_NewLayer1_Option27_mean,"Di-#tau iso (Option 27) - THR"+thr_Option27, "L");
        if(thrInt_Option28>=minThrAllowed && thrInt_Option28<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option28_mean) && has100Plateau(turnOn_NewLayer1_Option28_mean)) leg1->AddEntry(turnOn_NewLayer1_Option28_mean,"Di-#tau iso (Option 28) - THR"+thr_Option28, "L");
        if(thrInt_Option29>=minThrAllowed && thrInt_Option29<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option29_mean) && has100Plateau(turnOn_NewLayer1_Option29_mean)) leg1->AddEntry(turnOn_NewLayer1_Option29_mean,"Di-#tau iso (Option 29) - THR"+thr_Option29, "L");
        if(thrInt_Option30>=minThrAllowed && thrInt_Option30<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option30_mean) && has100Plateau(turnOn_NewLayer1_Option30_mean)) leg1->AddEntry(turnOn_NewLayer1_Option30_mean,"Di-#tau iso (Option 30) - THR"+thr_Option30, "L");
        if(thrInt_Option31>=minThrAllowed && thrInt_Option31<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option31_mean) && has100Plateau(turnOn_NewLayer1_Option31_mean)) leg1->AddEntry(turnOn_NewLayer1_Option31_mean,"Di-#tau iso (Option 31) - THR"+thr_Option31, "L");
        if(thrInt_Option31_extrap>=minThrAllowed && thrInt_Option31_extrap<=maxThrAllowed && has0effAt0pt(turnOn_NewLayer1_Option31_extrap_mean) && has100Plateau(turnOn_NewLayer1_Option31_extrap_mean)) leg1->AddEntry(turnOn_NewLayer1_Option31_extrap_mean,"Di-#tau iso (Option 31_extrap) - THR"+thr_Option31_extrap, "L");

        leg1->Draw("same");

        if(drewSth) c1.SaveAs(PDFs+cNamePdf.Data());

    }

    f->Close();

}
