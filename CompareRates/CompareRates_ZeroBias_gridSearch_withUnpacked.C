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
#include <fstream>
#include <map>
#include <TVector.h>
#include "../Isolate/Fill_RelaxedIsolation_gridsearch_nTTextrap.C"

using namespace std;

void compare(int run = 0,  bool doScaleToLumi = true, float calibThr = 1.7) {
    // TString run_str = to_string(run);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    TFile* f_gridseacrh = new TFile("/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2024/histos_rate_ZeroBias_Run369978_optimization24_v0_gridsearch.root","READ");

    TString scaledToLumi = "";
    if (doScaleToLumi) scaledToLumi = "_scaledTo2e34Lumi";
    TFile* f_unpacked = new TFile("/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2024/histos_rate_ZeroBias_Run369978_unpacked_optimization24_v0.root","READ");

    // CREATE OUTPUT FILE
    TFile f_out("/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2024/thresholds_fixedrate_ZeroBias_Run369978_unpacked_optimization24_v0.root","RECREATE");

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
                Int_t Emin = int(Emins[iEmin]);
                Int_t Emax = int(Emins[iEmin] + Emaxs_sum[iEmax]);
                TString DiTauRateName = "DiTauRate_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                TH1F* DiTauRate = (TH1F*)f_gridseacrh->Get(DiTauRateName);
                std::cout << " Running " << DiTauRateName << std::endl;

                TVectorD CurrentRelaxationThreshold(5);
                CurrentRelaxationThreshold[0] = 999.9; CurrentRelaxationThreshold[1] = 999.9; CurrentRelaxationThreshold[2] = 999.9; CurrentRelaxationThreshold[3] = 999.9; CurrentRelaxationThreshold[4] = 999.9;
                bool Filled16 = false; bool Filled15 = false; bool Filled14 = false; bool Filled13 = false; bool Filled12 = false;
                for(Int_t i = 1 ; i <= DiTauRate->GetNbinsX() ; ++i)
                {
                    if(!Filled16 && DiTauRate->GetBinContent(i)<=double(16.)) { CurrentRelaxationThreshold[4] = DiTauRate->GetBinLowEdge(i); Filled16 = true; }
                    if(!Filled15 && DiTauRate->GetBinContent(i)<=double(15.)) { CurrentRelaxationThreshold[3] = DiTauRate->GetBinLowEdge(i); Filled15 = true; }
                    if(!Filled14 && DiTauRate->GetBinContent(i)<=double(14.)) { CurrentRelaxationThreshold[2] = DiTauRate->GetBinLowEdge(i); Filled14 = true; }
                    if(!Filled13 && DiTauRate->GetBinContent(i)<=double(13.)) { CurrentRelaxationThreshold[1] = DiTauRate->GetBinLowEdge(i); Filled13 = true; }
                    if(!Filled12 && DiTauRate->GetBinContent(i)<=double(12.))
                    {
                        CurrentRelaxationThreshold[0] = DiTauRate->GetBinLowEdge(i);
                        Filled12 = true;
                        break; // rate is strictly descendent so when the lowest target is found break!
                    } 
                }

                TString ThresholdsName = "Thresholds_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                CurrentRelaxationThreshold.Write(ThresholdsName);

            }
        }
    }
    // END OF GRID SEARCH

    // DO NO-ISO CASE
    std::cout << " Running No-Iso" << std::endl;
    TH1F* DiTauRate_noIso = (TH1F*)f_gridseacrh->Get("DiTauRate_noIso");

    TVectorD Threshold_noIso(5);
    Threshold_noIso[0] = 999.9; Threshold_noIso[1] = 999.9; Threshold_noIso[2] = 999.9; Threshold_noIso[3] = 999.9; Threshold_noIso[4] = 999.9;
    bool Filled16 = false; bool Filled15 = false; bool Filled14 = false; bool Filled13 = false; bool Filled12 = false;
    for(Int_t i = 1 ; i <= DiTauRate_noIso->GetNbinsX() ; ++i)
    {
        if(!Filled16 && DiTauRate_noIso->GetBinContent(i)<=double(16.)) { Threshold_noIso[4] = DiTauRate_noIso->GetBinLowEdge(i); Filled16=true; }
        if(!Filled15 && DiTauRate_noIso->GetBinContent(i)<=double(15.)) { Threshold_noIso[3] = DiTauRate_noIso->GetBinLowEdge(i); Filled15=true; }
        if(!Filled14 && DiTauRate_noIso->GetBinContent(i)<=double(14.)) { Threshold_noIso[2] = DiTauRate_noIso->GetBinLowEdge(i); Filled14=true; }
        if(!Filled13 && DiTauRate_noIso->GetBinContent(i)<=double(13.)) { Threshold_noIso[1] = DiTauRate_noIso->GetBinLowEdge(i); Filled13=true; }
        if(!Filled12 && DiTauRate_noIso->GetBinContent(i)<=double(12.)) 
        {
            Threshold_noIso[0] = DiTauRate_noIso->GetBinLowEdge(i);
            Filled12 = true;
            break; // rate is strictly descendent so when the lowest target is found break!
        } 
    }
    Threshold_noIso.Write("Thresholds_noIso");

    // DO UNPACKED CASE
    std::cout << " Running unpacked" << std::endl;
    TH1F* DiTauRate_noIso_unpacked   = (TH1F*)f_unpacked->Get("DiTauRate_noIso");
    TH1F* DiTauRate_Iso_unpacked     = (TH1F*)f_unpacked->Get("DiTauRate_Iso");

    TVectorD Threshold_noIso_unpacked(5);
    Threshold_noIso_unpacked[0] = 999.9; Threshold_noIso_unpacked[1] = 999.9; Threshold_noIso_unpacked[2] = 999.9; Threshold_noIso_unpacked[3] = 999.9; Threshold_noIso_unpacked[4] = 999.9;
    Filled16 = false; Filled15 = false; Filled14 = false; Filled13 = false; Filled12 = false;
    for(Int_t i = 1 ; i <= DiTauRate_noIso_unpacked->GetNbinsX() ; ++i)
    {
        if(!Filled16 && DiTauRate_noIso_unpacked->GetBinContent(i)<=double(16.)) { Threshold_noIso_unpacked[4] = DiTauRate_noIso_unpacked->GetBinLowEdge(i); Filled16=true; }
        if(!Filled15 && DiTauRate_noIso_unpacked->GetBinContent(i)<=double(15.)) { Threshold_noIso_unpacked[3] = DiTauRate_noIso_unpacked->GetBinLowEdge(i); Filled15=true; }
        if(!Filled14 && DiTauRate_noIso_unpacked->GetBinContent(i)<=double(14.)) { Threshold_noIso_unpacked[2] = DiTauRate_noIso_unpacked->GetBinLowEdge(i); Filled14=true; }
        if(!Filled13 && DiTauRate_noIso_unpacked->GetBinContent(i)<=double(13.)) { Threshold_noIso_unpacked[1] = DiTauRate_noIso_unpacked->GetBinLowEdge(i); Filled13=true; }
        if(!Filled12 && DiTauRate_noIso_unpacked->GetBinContent(i)<=double(12.)) 
        {
            Threshold_noIso_unpacked[0] = DiTauRate_noIso_unpacked->GetBinLowEdge(i);
            Filled12 = true;
            break; // rate is strictly descendent so when the lowest target is found break!
        } 
    }
    Threshold_noIso_unpacked.Write("Thresholds_noIso_unpacked");

    TVectorD Threshold_Iso_unpacked(5);
    Threshold_Iso_unpacked[0] = 999.9; Threshold_Iso_unpacked[1] = 999.9; Threshold_Iso_unpacked[2] = 999.9; Threshold_Iso_unpacked[3] = 999.9; Threshold_Iso_unpacked[4] = 999.9;
    Filled16 = false; Filled15 = false; Filled14 = false; Filled13 = false; Filled12 = false;
    for(Int_t i = 1 ; i <= DiTauRate_Iso_unpacked->GetNbinsX() ; ++i)
    {
        if(!Filled16 && DiTauRate_Iso_unpacked->GetBinContent(i)<=double(16.)) { Threshold_Iso_unpacked[4] = DiTauRate_Iso_unpacked->GetBinLowEdge(i); Filled16=true; }
        if(!Filled15 && DiTauRate_Iso_unpacked->GetBinContent(i)<=double(15.)) { Threshold_Iso_unpacked[3] = DiTauRate_Iso_unpacked->GetBinLowEdge(i); Filled15=true; }
        if(!Filled14 && DiTauRate_Iso_unpacked->GetBinContent(i)<=double(14.)) { Threshold_Iso_unpacked[2] = DiTauRate_Iso_unpacked->GetBinLowEdge(i); Filled14=true; }
        if(!Filled13 && DiTauRate_Iso_unpacked->GetBinContent(i)<=double(13.)) { Threshold_Iso_unpacked[1] = DiTauRate_Iso_unpacked->GetBinLowEdge(i); Filled13=true; }
        if(!Filled12 && DiTauRate_Iso_unpacked->GetBinContent(i)<=double(12.)) 
        {
            Threshold_Iso_unpacked[0] = DiTauRate_Iso_unpacked->GetBinLowEdge(i);
            Filled12 = true;
            break; // rate is strictly descendent so when the lowest target is found break!
        } 
    }
    Threshold_Iso_unpacked.Write("Thresholds_Iso_unpacked");

    f_gridseacrh->Close();
    f_unpacked->Close();
    f_out.Close();

}
