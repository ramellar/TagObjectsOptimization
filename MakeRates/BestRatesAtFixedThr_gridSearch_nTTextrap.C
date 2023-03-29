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
#include "../Isolate/Fill_Isolation_TH3_MC_newnTT_gridSearch_nTTextrap.C"

using namespace std;

void isLowerRate(std::vector< std::pair<TString, float> > &orderedHz, float newHz, TString name) {
    for (long unsigned int i = 0; i < orderedHz.size(); ++i)
    {
        if (newHz >= orderedHz[i].second) { //} && newHz >= 13 && newHz <= 20) {
            orderedHz.insert( orderedHz.begin()+i, std::pair(name, newHz) );
            break;
        }
    }
}

void compare(int run, TString version, TString tag, int fixedThr, bool doScaleToLumi=false, float calibThr = 1.7) {
    TString run_str = to_string(run);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    std::vector< std::pair<TString, float> > orderedHzAtThr = { std::pair("init", 0) };

    std::map<int, int> thresholdRemap;
    thresholdRemap.insert(make_pair(30,1));
    thresholdRemap.insert(make_pair(31,2));
    thresholdRemap.insert(make_pair(32,3));
    thresholdRemap.insert(make_pair(33,4));
    thresholdRemap.insert(make_pair(34,5));
    thresholdRemap.insert(make_pair(35,6));
    thresholdRemap.insert(make_pair(36,7));
    int thresholdBin = thresholdRemap[fixedThr];

    TFile* f_rate = new TFile("histos_2023/histos_rate_ZeroBias_Run"+run_str+"_optimization"+version+"gs_calibThr"+intgr+"p"+decim+"_"+tag+".root","READ");

    // START OF GRID SEARCH
    for (UInt_t iEff = 0; iEff < NEffsMin; ++iEff)
    {
        for (UInt_t iEmin = 0; iEmin < NEmins; ++iEmin)
        {
            for (UInt_t iEmax = 0; iEmax < NEmaxs_sum; ++iEmax)
            {
                Float_t effMin = EffsMin[iEff];
                TString effMin_intgr = to_string(effMin).substr(0, to_string(effMin).find("."));
                TString effMin_decim = to_string(effMin).substr(2, to_string(effMin).find("."));
                Int_t Emin = Emins[iEmin];
                Int_t Emax = Emins[iEmin] + Emaxs_sum[iEmax];
                TString FixedThresholdDiTauRateName = "FixedThresholdDiTauRate_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                TH1F* FixedThresholdDiTauRate = (TH1F*)f_rate->Get(FixedThresholdDiTauRateName);
                Float_t currentRate = FixedThresholdDiTauRate->GetBinContent(thresholdBin);

                isLowerRate(orderedHzAtThr, currentRate, FixedThresholdDiTauRateName);
            }
        }
    }
    // END OF GRID SEARCH

    f_rate->Close();

    orderedHzAtThr.pop_back(); // remove init entry

    TString FilenameAtThr = "BestRateAtFixedThr/2023/BestRatesAt"+to_string(fixedThr)+"Thr_Run"+run_str+"_optimization"+version+".txt";
    ofstream file;
    file.open(FilenameAtThr, std::ofstream::trunc);
    file << std::endl << std::endl;
    for (long unsigned int i = 0; i < orderedHzAtThr.size(); ++i)
    {
        file << orderedHzAtThr[i].first << " : kHz=" << orderedHzAtThr[i].second << std::endl;
    }
    file.close();

}

