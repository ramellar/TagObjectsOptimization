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

bool isGoodTurnON(TGraphAsymmErrors* baseline, TGraphAsymmErrors* newOpt, float tunronThr, float limitTurnonThr) {
    Int_t point1 = 8;
    Int_t point2 = 14;
    Double_t y;
    Double_t th;
    Double_t _;
    bool good = true;

    for (int i = point1; i < point2; ++i)
    {
        baseline->GetPoint(i, _, th);
        newOpt->GetPoint(i, _, y);
        if (y < th) 
        {
            good = false;
            break;
        }
    }

    if (tunronThr > limitTurnonThr) good = false;

    return good;
}

void isBetterFM(std::vector< std::pair<TString, std::pair<float,float>> > &orderedfm, std::vector< std::vector<float> > &acc_ordered, float newFM, float thr, std::vector<float> acceptances,  TString name) {
    for (long unsigned int i = 0; i < orderedfm.size(); ++i)
    {
        if (newFM >= orderedfm[i].second.first) {
            orderedfm.insert( orderedfm.begin()+i, std::pair(name, std::pair(newFM,thr)) );
            acc_ordered.insert( acc_ordered.begin()+i, acceptances );
            break;
        }
    }
}

void compare(int run, TString baseline="22", float limitTurnonThr=99, bool zoom=false, float calibThr = 1.7) {
    TString run_str = to_string(run);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));
    
    gStyle->SetOptStat(000000);

    std::vector<TString> tags = {"effMin0p0", "effMin0p1", "effMin0p2", "effMin0p3", "effMin0p4", "effMin0p5", "effMin0p5", "effMin0p6", "effMin0p7", "effMin0p8", "effMin0p9"};
    std::vector<TString> opts = {"1", "2", "3", "4", "5", "6", "7", "8"};

    std::vector< std::pair<TString, std::pair<float,float>> > orderedFM = { std::pair("init", std::pair(0,0)) };
    std::vector<float> dummy = {0, 0, 0, 0};
    std::vector< std::vector<float> > acc_orderedFM = { dummy };

    float fm_noIso           = 0.0;
    float fm_Option31_extrap = 0.0;
    float fm_Option22        = 0.0;

    float thr_noIso           = 0.0;
    float thr_Option31_extrap = 0.0;
    float thr_Option22        = 0.0;

    TVectorT<float>* acc_noIso;
    TVectorT<float>* acc_Option31_extrap;
    TVectorT<float>* acc_Option22;

    std::vector<float> acceptance_noIso;
    std::vector<float> acceptance_Option31_extrap;
    std::vector<float> acceptance_Option22;

    for (long unsigned int i = 0; i < tags.size(); ++i)
    {
        TString tag = tags[i];

        for (long unsigned int j = 0; j < opts.size(); ++j)
        {
            TString opt = opts[j];

            TFile* f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_01_28_optimizationV6_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_Run"+run_str+"_gs_"+tag+"_"+opt+"_2022_01_28.root","READ");

            TGraphAsymmErrors* turnon_NewLayer1_Option22_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option22_by_pt");
            TGraphAsymmErrors* turnon_NewLayer1_Option31_extrap_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option31_extrap_by_pt");
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

            fm_noIso       = ( (TVectorT<float>*)f->Get("fm_noIso") )[0][1];
            fm_Option31_extrap = ( (TVectorT<float>*)f->Get("fm_Option31_extrap") )[0][1];
            fm_Option22 = ( (TVectorT<float>*)f->Get("fm_Option22") )[0][1];
            float fm_OptionA = ( (TVectorT<float>*)f->Get("fm_OptionA") )[0][1];
            float fm_OptionB = ( (TVectorT<float>*)f->Get("fm_OptionB") )[0][1];
            float fm_OptionC = ( (TVectorT<float>*)f->Get("fm_OptionC") )[0][1];
            float fm_OptionD = ( (TVectorT<float>*)f->Get("fm_OptionD") )[0][1];
            float fm_OptionE = ( (TVectorT<float>*)f->Get("fm_OptionE") )[0][1];
            float fm_OptionF = ( (TVectorT<float>*)f->Get("fm_OptionF") )[0][1];
            float fm_OptionG = ( (TVectorT<float>*)f->Get("fm_OptionG") )[0][1];
            float fm_OptionH = ( (TVectorT<float>*)f->Get("fm_OptionH") )[0][1];
            float fm_OptionI = ( (TVectorT<float>*)f->Get("fm_OptionI") )[0][1];
            float fm_OptionJ = ( (TVectorT<float>*)f->Get("fm_OptionJ") )[0][1];
            float fm_OptionK = ( (TVectorT<float>*)f->Get("fm_OptionK") )[0][1];
            float fm_OptionL = ( (TVectorT<float>*)f->Get("fm_OptionL") )[0][1];
            float fm_OptionM = ( (TVectorT<float>*)f->Get("fm_OptionM") )[0][1];
            float fm_OptionN = ( (TVectorT<float>*)f->Get("fm_OptionN") )[0][1];
            float fm_OptionO = ( (TVectorT<float>*)f->Get("fm_OptionO") )[0][1];
            float fm_OptionP = ( (TVectorT<float>*)f->Get("fm_OptionP") )[0][1];
            float fm_OptionQ = ( (TVectorT<float>*)f->Get("fm_OptionQ") )[0][1];
            float fm_OptionR = ( (TVectorT<float>*)f->Get("fm_OptionR") )[0][1];
            float fm_OptionS = ( (TVectorT<float>*)f->Get("fm_OptionS") )[0][1];
            float fm_OptionT = ( (TVectorT<float>*)f->Get("fm_OptionT") )[0][1];
            float fm_OptionU = ( (TVectorT<float>*)f->Get("fm_OptionU") )[0][1];
            float fm_OptionV = ( (TVectorT<float>*)f->Get("fm_OptionV") )[0][1];
            float fm_OptionW = ( (TVectorT<float>*)f->Get("fm_OptionW") )[0][1];
            float fm_OptionX = ( (TVectorT<float>*)f->Get("fm_OptionX") )[0][1];
            float fm_OptionY = ( (TVectorT<float>*)f->Get("fm_OptionY") )[0][1];
            float fm_OptionZ = ( (TVectorT<float>*)f->Get("fm_OptionZ") )[0][1];

            thr_noIso       = ( (TVectorT<float>*)f->Get("thr_noIso") )[0][0];
            thr_Option31_extrap = ( (TVectorT<float>*)f->Get("thr_Option31_extrap") )[0][0];
            thr_Option22 = ( (TVectorT<float>*)f->Get("thr_Option22") )[0][0];
            float thr_OptionA = ( (TVectorT<float>*)f->Get("thr_OptionA") )[0][0];
            float thr_OptionB = ( (TVectorT<float>*)f->Get("thr_OptionB") )[0][0];
            float thr_OptionC = ( (TVectorT<float>*)f->Get("thr_OptionC") )[0][0];
            float thr_OptionD = ( (TVectorT<float>*)f->Get("thr_OptionD") )[0][0];
            float thr_OptionE = ( (TVectorT<float>*)f->Get("thr_OptionE") )[0][0];
            float thr_OptionF = ( (TVectorT<float>*)f->Get("thr_OptionF") )[0][0];
            float thr_OptionG = ( (TVectorT<float>*)f->Get("thr_OptionG") )[0][0];
            float thr_OptionH = ( (TVectorT<float>*)f->Get("thr_OptionH") )[0][0];
            float thr_OptionI = ( (TVectorT<float>*)f->Get("thr_OptionI") )[0][0];
            float thr_OptionJ = ( (TVectorT<float>*)f->Get("thr_OptionJ") )[0][0];
            float thr_OptionK = ( (TVectorT<float>*)f->Get("thr_OptionK") )[0][0];
            float thr_OptionL = ( (TVectorT<float>*)f->Get("thr_OptionL") )[0][0];
            float thr_OptionM = ( (TVectorT<float>*)f->Get("thr_OptionM") )[0][0];
            float thr_OptionN = ( (TVectorT<float>*)f->Get("thr_OptionN") )[0][0];
            float thr_OptionO = ( (TVectorT<float>*)f->Get("thr_OptionO") )[0][0];
            float thr_OptionP = ( (TVectorT<float>*)f->Get("thr_OptionP") )[0][0];
            float thr_OptionQ = ( (TVectorT<float>*)f->Get("thr_OptionQ") )[0][0];
            float thr_OptionR = ( (TVectorT<float>*)f->Get("thr_OptionR") )[0][0];
            float thr_OptionS = ( (TVectorT<float>*)f->Get("thr_OptionS") )[0][0];
            float thr_OptionT = ( (TVectorT<float>*)f->Get("thr_OptionT") )[0][0];
            float thr_OptionU = ( (TVectorT<float>*)f->Get("thr_OptionU") )[0][0];
            float thr_OptionV = ( (TVectorT<float>*)f->Get("thr_OptionV") )[0][0];
            float thr_OptionW = ( (TVectorT<float>*)f->Get("thr_OptionW") )[0][0];
            float thr_OptionX = ( (TVectorT<float>*)f->Get("thr_OptionX") )[0][0];
            float thr_OptionY = ( (TVectorT<float>*)f->Get("thr_OptionY") )[0][0];
            float thr_OptionZ = ( (TVectorT<float>*)f->Get("thr_OptionZ") )[0][0];

            acc_noIso       = ( (TVectorT<float>*)f->Get("acc_noIso") );
            acc_Option31_extrap = ( (TVectorT<float>*)f->Get("acc_Option31_extrap") );
            acc_Option22 = ( (TVectorT<float>*)f->Get("acc_Option22") );
            TVectorT<float>* acc_OptionA = ( (TVectorT<float>*)f->Get("acc_OptionA") );
            TVectorT<float>* acc_OptionB = ( (TVectorT<float>*)f->Get("acc_OptionB") );
            TVectorT<float>* acc_OptionC = ( (TVectorT<float>*)f->Get("acc_OptionC") );
            TVectorT<float>* acc_OptionD = ( (TVectorT<float>*)f->Get("acc_OptionD") );
            TVectorT<float>* acc_OptionE = ( (TVectorT<float>*)f->Get("acc_OptionE") );
            TVectorT<float>* acc_OptionF = ( (TVectorT<float>*)f->Get("acc_OptionF") );
            TVectorT<float>* acc_OptionG = ( (TVectorT<float>*)f->Get("acc_OptionG") );
            TVectorT<float>* acc_OptionH = ( (TVectorT<float>*)f->Get("acc_OptionH") );
            TVectorT<float>* acc_OptionI = ( (TVectorT<float>*)f->Get("acc_OptionI") );
            TVectorT<float>* acc_OptionJ = ( (TVectorT<float>*)f->Get("acc_OptionJ") );
            TVectorT<float>* acc_OptionK = ( (TVectorT<float>*)f->Get("acc_OptionK") );
            TVectorT<float>* acc_OptionL = ( (TVectorT<float>*)f->Get("acc_OptionL") );
            TVectorT<float>* acc_OptionM = ( (TVectorT<float>*)f->Get("acc_OptionM") );
            TVectorT<float>* acc_OptionN = ( (TVectorT<float>*)f->Get("acc_OptionN") );
            TVectorT<float>* acc_OptionO = ( (TVectorT<float>*)f->Get("acc_OptionO") );
            TVectorT<float>* acc_OptionP = ( (TVectorT<float>*)f->Get("acc_OptionP") );
            TVectorT<float>* acc_OptionQ = ( (TVectorT<float>*)f->Get("acc_OptionQ") );
            TVectorT<float>* acc_OptionR = ( (TVectorT<float>*)f->Get("acc_OptionR") );
            TVectorT<float>* acc_OptionS = ( (TVectorT<float>*)f->Get("acc_OptionS") );
            TVectorT<float>* acc_OptionT = ( (TVectorT<float>*)f->Get("acc_OptionT") );
            TVectorT<float>* acc_OptionU = ( (TVectorT<float>*)f->Get("acc_OptionU") );
            TVectorT<float>* acc_OptionV = ( (TVectorT<float>*)f->Get("acc_OptionV") );
            TVectorT<float>* acc_OptionW = ( (TVectorT<float>*)f->Get("acc_OptionW") );
            TVectorT<float>* acc_OptionX = ( (TVectorT<float>*)f->Get("acc_OptionX") );
            TVectorT<float>* acc_OptionY = ( (TVectorT<float>*)f->Get("acc_OptionY") );
            TVectorT<float>* acc_OptionZ = ( (TVectorT<float>*)f->Get("acc_OptionZ") );

            acceptance_noIso = {acc_noIso[0][0], acc_noIso[0][1], acc_noIso[0][2], acc_noIso[0][3]};
            acceptance_Option31_extrap = {acc_Option31_extrap[0][0], acc_Option31_extrap[0][1], acc_Option31_extrap[0][2], acc_Option31_extrap[0][3]};
            acceptance_Option22 = {acc_Option22[0][0], acc_Option22[0][1], acc_Option22[0][2], acc_Option22[0][3]};
            std::vector<float> acceptance_OptionA = {acc_OptionA[0][0], acc_OptionA[0][1], acc_OptionA[0][2], acc_OptionA[0][3]};
            std::vector<float> acceptance_OptionB = {acc_OptionB[0][0], acc_OptionB[0][1], acc_OptionB[0][2], acc_OptionB[0][3]};
            std::vector<float> acceptance_OptionC = {acc_OptionC[0][0], acc_OptionC[0][1], acc_OptionC[0][2], acc_OptionC[0][3]};
            std::vector<float> acceptance_OptionD = {acc_OptionD[0][0], acc_OptionD[0][1], acc_OptionD[0][2], acc_OptionD[0][3]};
            std::vector<float> acceptance_OptionE = {acc_OptionE[0][0], acc_OptionE[0][1], acc_OptionE[0][2], acc_OptionE[0][3]};
            std::vector<float> acceptance_OptionF = {acc_OptionF[0][0], acc_OptionF[0][1], acc_OptionF[0][2], acc_OptionF[0][3]};
            std::vector<float> acceptance_OptionG = {acc_OptionG[0][0], acc_OptionG[0][1], acc_OptionG[0][2], acc_OptionG[0][3]};
            std::vector<float> acceptance_OptionH = {acc_OptionH[0][0], acc_OptionH[0][1], acc_OptionH[0][2], acc_OptionH[0][3]};
            std::vector<float> acceptance_OptionI = {acc_OptionI[0][0], acc_OptionI[0][1], acc_OptionI[0][2], acc_OptionI[0][3]};
            std::vector<float> acceptance_OptionJ = {acc_OptionJ[0][0], acc_OptionJ[0][1], acc_OptionJ[0][2], acc_OptionJ[0][3]};
            std::vector<float> acceptance_OptionK = {acc_OptionK[0][0], acc_OptionK[0][1], acc_OptionK[0][2], acc_OptionK[0][3]};
            std::vector<float> acceptance_OptionL = {acc_OptionL[0][0], acc_OptionL[0][1], acc_OptionL[0][2], acc_OptionL[0][3]};
            std::vector<float> acceptance_OptionM = {acc_OptionM[0][0], acc_OptionM[0][1], acc_OptionM[0][2], acc_OptionM[0][3]};
            std::vector<float> acceptance_OptionN = {acc_OptionN[0][0], acc_OptionN[0][1], acc_OptionN[0][2], acc_OptionN[0][3]};
            std::vector<float> acceptance_OptionO = {acc_OptionO[0][0], acc_OptionO[0][1], acc_OptionO[0][2], acc_OptionO[0][3]};
            std::vector<float> acceptance_OptionP = {acc_OptionP[0][0], acc_OptionP[0][1], acc_OptionP[0][2], acc_OptionP[0][3]};
            std::vector<float> acceptance_OptionQ = {acc_OptionQ[0][0], acc_OptionQ[0][1], acc_OptionQ[0][2], acc_OptionQ[0][3]};
            std::vector<float> acceptance_OptionR = {acc_OptionR[0][0], acc_OptionR[0][1], acc_OptionR[0][2], acc_OptionR[0][3]};
            std::vector<float> acceptance_OptionS = {acc_OptionS[0][0], acc_OptionS[0][1], acc_OptionS[0][2], acc_OptionS[0][3]};
            std::vector<float> acceptance_OptionT = {acc_OptionT[0][0], acc_OptionT[0][1], acc_OptionT[0][2], acc_OptionT[0][3]};
            std::vector<float> acceptance_OptionU = {acc_OptionU[0][0], acc_OptionU[0][1], acc_OptionU[0][2], acc_OptionU[0][3]};
            std::vector<float> acceptance_OptionV = {acc_OptionV[0][0], acc_OptionV[0][1], acc_OptionV[0][2], acc_OptionV[0][3]};
            std::vector<float> acceptance_OptionW = {acc_OptionW[0][0], acc_OptionW[0][1], acc_OptionW[0][2], acc_OptionW[0][3]};
            std::vector<float> acceptance_OptionX = {acc_OptionX[0][0], acc_OptionX[0][1], acc_OptionX[0][2], acc_OptionX[0][3]};
            std::vector<float> acceptance_OptionY = {acc_OptionY[0][0], acc_OptionY[0][1], acc_OptionY[0][2], acc_OptionY[0][3]};
            std::vector<float> acceptance_OptionZ = {acc_OptionZ[0][0], acc_OptionZ[0][1], acc_OptionZ[0][2], acc_OptionZ[0][3]};

            TGraphAsymmErrors* betterThan;
            if (baseline == "22") betterThan = turnon_NewLayer1_Option22_mean;
            else betterThan = turnon_NewLayer1_Option31_extrap_mean;

            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionA_mean, thr_OptionA, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionA, thr_OptionA, acceptance_OptionA, tag+" OptionA"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionB_mean, thr_OptionB, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionB, thr_OptionB, acceptance_OptionB, tag+" OptionB"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionC_mean, thr_OptionC, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionC, thr_OptionC, acceptance_OptionC, tag+" OptionC"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionD_mean, thr_OptionD, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionD, thr_OptionD, acceptance_OptionD, tag+" OptionD"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionE_mean, thr_OptionE, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionE, thr_OptionE, acceptance_OptionE, tag+" OptionE"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionF_mean, thr_OptionF, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionF, thr_OptionF, acceptance_OptionF, tag+" OptionF"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionG_mean, thr_OptionG, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionG, thr_OptionG, acceptance_OptionG, tag+" OptionG"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionH_mean, thr_OptionH, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionH, thr_OptionH, acceptance_OptionH, tag+" OptionH"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionI_mean, thr_OptionI, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionI, thr_OptionI, acceptance_OptionI, tag+" OptionI"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionJ_mean, thr_OptionJ, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionJ, thr_OptionJ, acceptance_OptionJ, tag+" OptionJ"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionK_mean, thr_OptionK, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionK, thr_OptionK, acceptance_OptionK, tag+" OptionK"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionL_mean, thr_OptionL, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionL, thr_OptionL, acceptance_OptionL, tag+" OptionL"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionM_mean, thr_OptionM, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionM, thr_OptionM, acceptance_OptionM, tag+" OptionM"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionN_mean, thr_OptionN, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionN, thr_OptionN, acceptance_OptionN, tag+" OptionN"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionO_mean, thr_OptionO, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionO, thr_OptionO, acceptance_OptionO, tag+" OptionO"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionP_mean, thr_OptionP, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionP, thr_OptionP, acceptance_OptionP, tag+" OptionP"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionQ_mean, thr_OptionQ, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionQ, thr_OptionQ, acceptance_OptionQ, tag+" OptionQ"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionR_mean, thr_OptionR, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionR, thr_OptionR, acceptance_OptionR, tag+" OptionR"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionS_mean, thr_OptionS, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionS, thr_OptionS, acceptance_OptionS, tag+" OptionS"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionT_mean, thr_OptionT, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionT, thr_OptionT, acceptance_OptionT, tag+" OptionT"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionU_mean, thr_OptionU, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionU, thr_OptionU, acceptance_OptionU, tag+" OptionU"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionV_mean, thr_OptionV, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionV, thr_OptionV, acceptance_OptionV, tag+" OptionV"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionW_mean, thr_OptionW, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionW, thr_OptionW, acceptance_OptionW, tag+" OptionW"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionX_mean, thr_OptionX, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionX, thr_OptionX, acceptance_OptionX, tag+" OptionX"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionY_mean, thr_OptionY, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionY, thr_OptionY, acceptance_OptionY, tag+" OptionY"+opt);
            if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionZ_mean, thr_OptionZ, limitTurnonThr) ) isBetterFM(orderedFM, acc_orderedFM, fm_OptionZ, thr_OptionZ, acceptance_OptionZ, tag+" OptionZ"+opt);
            
            f->Close();
        }
    }

    orderedFM.pop_back(); // remove init entry

    TString thrLim = to_string(int(round(limitTurnonThr)));
    if (limitTurnonThr==99) thrLim = "None";
    TString Filename = "plotsBestFMturnons_Run"+run_str+"/Comparison_TurnOn_Rate_Run"+run_str+"_newnTT_unpacked_optimizationV6gs_calibThr"+intgr+"p"+decim+"_betterThan"+baseline+"_thrLeq"+thrLim+".txt";
    ofstream file;
    file.open(Filename, std::ofstream::trunc);
    file << "noIso :  FM=" << fm_noIso << " - THR=" << thr_noIso << " - ACC@0GeV = " << acceptance_noIso[0] << " - ACC@20GeV = " << acceptance_noIso[1] << " - ACC@40GeV = " << acceptance_noIso[2] << " - ACC@60GeV = " << acceptance_noIso[3] << std::endl;
    file << "Option22 :  FM=" << fm_Option22 << " - THR=" << thr_Option22 << " - ACC@0GeV = " << acceptance_Option22[0] << " - ACC@20GeV = " << acceptance_Option22[1] << " - ACC@40GeV = " << acceptance_Option22[2] << " - ACC@60GeV = " << acceptance_Option22[3] << std::endl;
    file << "Option31_extrap :  FM=" << fm_Option31_extrap << " - THR=" << thr_Option31_extrap << " - ACC@0GeV = " << acceptance_Option31_extrap[0] << " - ACC@20GeV = " << acceptance_Option31_extrap[1] << " - ACC@40GeV = " << acceptance_Option31_extrap[2] << " - ACC@60GeV = " << acceptance_Option31_extrap[3] << std::endl;
    file << std::endl << std::endl;
    for (long unsigned int i = 0; i < orderedFM.size(); ++i)
    {
        file << orderedFM[i].first << " : FM=" << orderedFM[i].second.first << " - THR=" << orderedFM[i].second.second << " - ACC@0GeV = " << acc_orderedFM[i][0] << " - ACC@20GeV = " << acc_orderedFM[i][1] << " - ACC@40GeV = " << acc_orderedFM[i][2] << " - ACC@60GeV = " << acc_orderedFM[i][3] << std::endl;
    }
    file.close();


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

    TFile* f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_01_28_optimizationV6_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_Run"+run_str+"_gs_effMin0p0_0_2022_01_28.root","READ");
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

    for (long unsigned int i = 0; i < orderedFM.size(); ++i)
    {
        std::string gridPoint = orderedFM[i].first.Data();
        TString effMin = gridPoint.substr(0, gridPoint.find(" "));
        TString point = gridPoint.substr(gridPoint.length() - 2);
        TString option = gridPoint.back();

        float FM = orderedFM[i].second.first;
        float TH = orderedFM[i].second.second;

        if (baseline=="22")
        {
            if (FM >= fm_Option22) // TH == thr_Option22 and
            {
                TFile* f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_01_28_optimizationV6_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_Run"+run_str+"_gs_"+effMin+"_"+option+"_2022_01_28.root","READ");

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
            if (FM >= fm_Option31_extrap) //TH == thr_Option31_extrap and
            {
                TFile* f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_01_28_optimizationV6_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_Run"+run_str+"_gs_"+effMin+"_"+option+"_2022_01_28.root","READ");

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
    c.SaveAs("plotsBestFMturnons_Run"+run_str+"/Comparison_TurnOn_Rate_Run"+run_str+"_newnTT_unpacked_optimizationV6gs_calibThr"+intgr+"p"+decim+"_betterThan"+baseline+"_thrLeq"+thrLim+tag+".pdf");


}





































