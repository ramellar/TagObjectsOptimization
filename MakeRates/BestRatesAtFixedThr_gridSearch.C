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

void isLowerRate(std::vector< std::pair<TString, float> > &orderedHz, float newHz, TString name) {
    for (long unsigned int i = 0; i < orderedHz.size(); ++i)
    {
        if (newHz >= orderedHz[i].second && newHz >= 13 && newHz <= 20) {
            orderedHz.insert( orderedHz.begin()+i, std::pair(name, newHz) );
            break;
        }
    }
}

void compare(int run, float calibThr = 1.7) {
    TString run_str = to_string(run);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));
    
    gStyle->SetOptStat(000000);

    std::vector<TString> tags = {"effMin0p0", "effMin0p1", "effMin0p2", "effMin0p3", "effMin0p4", "effMin0p5", "effMin0p5", "effMin0p6", "effMin0p7", "effMin0p8", "effMin0p9"};
    std::vector<TString> opts = {"1", "2", "3", "4", "5", "6", "7", "8"};

    std::vector< std::pair<TString, float> > orderedHzAt32 = { std::pair("init", 0) };
    std::vector< std::pair<TString, float> > orderedHzAt33 = { std::pair("init", 0) };

    TVectorF* Hz_noIso;
    TVectorF* Hz_Option31_extrap;
    TVectorF* Hz_Option22;

    for (long unsigned int i = 0; i < tags.size(); ++i)
    {
        TString tag = tags[i];

        TFile* f = new TFile("histos/histos_rate_ZeroBias_Run"+run_str+"_optimizationV3gs_calibThr"+intgr+"p"+decim+"_"+tag+".root","READ");

        for (long unsigned int j = 0; j < opts.size(); ++j)
        {
            TString opt = opts[j];        

            Hz_noIso       = ( (TVectorT<float>*)f->Get("Hz_noIso") );
            Hz_Option31_extrap = ( (TVectorT<float>*)f->Get("Hz_Option31_extrap") );
            Hz_Option22 = ( (TVectorT<float>*)f->Get("Hz_Option22") );
            TVectorF* Hz_OptionA = (TVectorF*)f->Get("Hz_OptionA"+opt);
            TVectorF* Hz_OptionB = (TVectorF*)f->Get("Hz_OptionB"+opt);
            TVectorF* Hz_OptionC = (TVectorF*)f->Get("Hz_OptionC"+opt);
            TVectorF* Hz_OptionD = (TVectorF*)f->Get("Hz_OptionD"+opt);
            TVectorF* Hz_OptionE = (TVectorF*)f->Get("Hz_OptionE"+opt);
            TVectorF* Hz_OptionF = (TVectorF*)f->Get("Hz_OptionF"+opt);
            TVectorF* Hz_OptionG = (TVectorF*)f->Get("Hz_OptionG"+opt);
            TVectorF* Hz_OptionH = (TVectorF*)f->Get("Hz_OptionH"+opt);
            TVectorF* Hz_OptionI = (TVectorF*)f->Get("Hz_OptionI"+opt);
            TVectorF* Hz_OptionJ = (TVectorF*)f->Get("Hz_OptionJ"+opt);
            TVectorF* Hz_OptionK = (TVectorF*)f->Get("Hz_OptionK"+opt);
            TVectorF* Hz_OptionL = (TVectorF*)f->Get("Hz_OptionL"+opt);
            TVectorF* Hz_OptionM = (TVectorF*)f->Get("Hz_OptionM"+opt);
            TVectorF* Hz_OptionN = (TVectorF*)f->Get("Hz_OptionN"+opt);
            TVectorF* Hz_OptionO = (TVectorF*)f->Get("Hz_OptionO"+opt);
            TVectorF* Hz_OptionP = (TVectorF*)f->Get("Hz_OptionP"+opt);
            TVectorF* Hz_OptionQ = (TVectorF*)f->Get("Hz_OptionQ"+opt);
            TVectorF* Hz_OptionR = (TVectorF*)f->Get("Hz_OptionR"+opt);
            TVectorF* Hz_OptionS = (TVectorF*)f->Get("Hz_OptionS"+opt);
            TVectorF* Hz_OptionT = (TVectorF*)f->Get("Hz_OptionT"+opt);
            TVectorF* Hz_OptionU = (TVectorF*)f->Get("Hz_OptionU"+opt);
            TVectorF* Hz_OptionV = (TVectorF*)f->Get("Hz_OptionV"+opt);
            TVectorF* Hz_OptionW = (TVectorF*)f->Get("Hz_OptionW"+opt);
            TVectorF* Hz_OptionX = (TVectorF*)f->Get("Hz_OptionX"+opt);
            TVectorF* Hz_OptionY = (TVectorF*)f->Get("Hz_OptionY"+opt);
            TVectorF* Hz_OptionZ = (TVectorF*)f->Get("Hz_OptionZ"+opt);

            // HERE WE CAN USE MAX AND MIN TO GET THE RATES BECAUSE THE RATE IS A MONOTONICALY DECRESCENT
            // FUNCTION OF THE THRESHOLD, SO THE RATE AT 32GeV WILL ALWASY BE HIGHER THAN THE RATE AT 33GeV

            isLowerRate(orderedHzAt32, Hz_OptionA->Max(), tag+" OptionA"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionB->Max(), tag+" OptionB"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionC->Max(), tag+" OptionC"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionD->Max(), tag+" OptionD"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionE->Max(), tag+" OptionE"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionF->Max(), tag+" OptionF"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionG->Max(), tag+" OptionG"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionH->Max(), tag+" OptionH"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionI->Max(), tag+" OptionI"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionJ->Max(), tag+" OptionJ"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionK->Max(), tag+" OptionK"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionL->Max(), tag+" OptionL"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionM->Max(), tag+" OptionM"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionN->Max(), tag+" OptionN"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionO->Max(), tag+" OptionO"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionP->Max(), tag+" OptionP"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionQ->Max(), tag+" OptionQ"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionR->Max(), tag+" OptionR"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionS->Max(), tag+" OptionS"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionT->Max(), tag+" OptionT"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionU->Max(), tag+" OptionU"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionV->Max(), tag+" OptionV"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionW->Max(), tag+" OptionW"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionX->Max(), tag+" OptionX"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionY->Max(), tag+" OptionY"+opt);
            isLowerRate(orderedHzAt32, Hz_OptionZ->Max(), tag+" OptionZ"+opt);

            isLowerRate(orderedHzAt33, Hz_OptionA->Min(), tag+" OptionA"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionB->Min(), tag+" OptionB"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionC->Min(), tag+" OptionC"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionD->Min(), tag+" OptionD"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionE->Min(), tag+" OptionE"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionF->Min(), tag+" OptionF"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionG->Min(), tag+" OptionG"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionH->Min(), tag+" OptionH"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionI->Min(), tag+" OptionI"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionJ->Min(), tag+" OptionJ"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionK->Min(), tag+" OptionK"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionL->Min(), tag+" OptionL"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionM->Min(), tag+" OptionM"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionN->Min(), tag+" OptionN"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionO->Min(), tag+" OptionO"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionP->Min(), tag+" OptionP"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionQ->Min(), tag+" OptionQ"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionR->Min(), tag+" OptionR"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionS->Min(), tag+" OptionS"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionT->Min(), tag+" OptionT"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionU->Min(), tag+" OptionU"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionV->Min(), tag+" OptionV"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionW->Min(), tag+" OptionW"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionX->Min(), tag+" OptionX"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionY->Min(), tag+" OptionY"+opt);
            isLowerRate(orderedHzAt33, Hz_OptionZ->Min(), tag+" OptionZ"+opt);
            
        }

        f->Close();
    }

    orderedHzAt32.pop_back(); // remove init entry
    orderedHzAt33.pop_back(); // "

    TString Filename32 = "BestRatesAt32Thr.txt";
    ofstream file32;
    file32.open(Filename32, std::ofstream::trunc);
    file32 << "noIso :  kHz=" << Hz_noIso->Max() << std::endl;
    file32 << "Option22 :  kHz=" << Hz_Option22->Max() << std::endl;
    file32 << "Option31_extrap :  kHz=" << Hz_Option31_extrap->Max() << std::endl;
    file32 << std::endl << std::endl;
    for (long unsigned int i = 0; i < orderedHzAt32.size(); ++i)
    {
        file32 << orderedHzAt32[i].first << " : kHz=" << orderedHzAt32[i].second << std::endl;
    }
    file32.close();

    TString Filename33 = "BestRatesAt33Thr.txt";
    ofstream file33;
    file33.open(Filename33, std::ofstream::trunc);
    file33 << "noIso :  kHz=" << Hz_noIso->Min() << std::endl;
    file33 << "Option22 :  kHz=" << Hz_Option22->Min() << std::endl;
    file33 << "Option31_extrap :  kHz=" << Hz_Option31_extrap->Min() << std::endl;
    file33 << std::endl << std::endl;
    for (long unsigned int i = 0; i < orderedHzAt33.size(); ++i)
    {
        file33 << orderedHzAt33[i].first << " : kHz=" << orderedHzAt33[i].second << std::endl;
    }
    file33.close();

}





































