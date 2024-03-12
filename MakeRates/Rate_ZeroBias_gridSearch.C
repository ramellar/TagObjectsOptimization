#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <iostream>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TVector.h>
#include <sstream>
#include <TBranchElement.h>
#include <fstream>
#include <map>
#include <typeinfo>
#include "../Isolate/Fill_RelaxedIsolation_gridsearch.C"

using namespace std;

void Rate(TString FileName_in, TString FileName_out, TString isolation, int run, bool doScaleToLumi=false, float calibThr = 1.7)
{
    TString run_str = to_string(run);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    TFile f_Isolation(isolation, "READ");
    TFile f_in(FileName_in.Data(),"READ");
    TTree* inTree = (TTree*)f_in.Get("outTreeCalibrated");
    // "/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/Isolate/ROOTs4LUTs_2024/LUTrelaxation_Run3Summer23_caloParams_2023_v0_4.root"
    // "/data_CMS/cms/mchiusi/Run3preparation/Run3_2024/2023S-MC_caloParams_2023_v0_4/EphemeralZeroBias_2023D_RAW_369978_optimization23_v4_HCAL_corr_CALIBRATED.root";

    UInt_t Nevents = inTree->GetEntries();

    Int_t           in_RunNumber       =  0;
    Int_t           in_lumi            =  0;
    vector<float>   *in_l1tEmuEta      =  0;
    vector<int>     *in_l1tEmuIsoEt    =  0;
    vector<int>     *in_compressedieta =  0;
    vector<int>     *in_compressedE    =  0;
    vector<int>     *in_compressednTT  =  0;
    vector<float>   *in_CalibPt        =  0;

    inTree->SetBranchAddress("RunNumber",      &in_RunNumber);
    inTree->SetBranchAddress("lumi",           &in_lumi);
    inTree->SetBranchAddress("L1Tau_eta",      &in_l1tEmuEta);
    inTree->SetBranchAddress("L1Tau_Iso",      &in_l1tEmuIsoEt);
    inTree->SetBranchAddress("compressedieta", &in_compressedieta);
    inTree->SetBranchAddress("compressedE",    &in_compressedE);
    inTree->SetBranchAddress("compressednTT",  &in_compressednTT);
    inTree->SetBranchAddress("L1Tau_CalibPt",  &in_CalibPt);

    // SET RUN INFO
    float thisLumiRun = 0.;
    float nb = 0.;
    float scaleToLumi = 2.00E34;
    float scale = 0.;
    inTree->GetEntry(0);
    if (in_RunNumber == 362616) { nb = 2450; thisLumiRun = 2.05E34; }
    if (in_RunNumber == 362617) { nb = 2450; thisLumiRun = 2.50E34; }
    if (in_RunNumber == 369978) { nb = 1165; thisLumiRun = 1.0E34; }
    if (thisLumiRun == 0. || nb == 0) { std::cout << "ERROR: something went wrong with the run selection and the lumi/nb initialization" << std::endl; return; }
    scale = 0.001 * nb * 11245.6;
    if (doScaleToLumi) scale *= scaleToLumi / thisLumiRun;

    // CREATE OUTPUT FILE
    TFile f_out(FileName_out, "RECREATE");
    // "histos_2024/histos_rate_ZeroBias_Run"+run_str+"_optimization_Run3Summer23_caloParams_2023_v0_4.root"

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
                TString ProgressionName = "LUT_progression_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                TH3F* currentProgression = (TH3F*)f_Isolation.Get(ProgressionName);
                std::cout << " Running " << ProgressionName << std::endl;

                // create histograms for single and double tau pt distributions of passing candidates
                TString TauPtPassDistributionName = "TauPtPassDistribution_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                TString DiTauPtPassDistributionName = "DiTauPtPassDistribution_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                TH1F* TauPtPassDistribution = new TH1F(TauPtPassDistributionName, TauPtPassDistributionName, 240, 0., 240.);
                TH2F* DiTauPtPassDistribution = new TH2F(DiTauPtPassDistributionName, DiTauPtPassDistributionName, 240, 0., 240., 240, 0., 240.);

                Int_t Denominator = 0;

                // loop over events
                // for(UInt_t ievt = 0 ; ievt < Nevents ; ++ievt)
                for(UInt_t ievt = 0 ; ievt < 1000000 ; ++ievt)
                {
                    inTree->GetEntry(ievt);
                    if(ievt%100000==0) cout<<"Entry #"<<ievt<<endl; 
                    // SET RUN INFO
                    if (in_RunNumber == 362616) { if(in_lumi<0) continue; }
                    if (in_RunNumber == 362617) { if(in_lumi<0) continue; }

                    ++Denominator;

                    // double tau filling pairs
                    std::vector<Int_t> IndexTausProgression;
                    IndexTausProgression.push_back(-1); IndexTausProgression.push_back(-1);
                    std::vector<Float_t> PtTausProgression;
                    PtTausProgression.push_back(-99.); PtTausProgression.push_back(-99.);

                    // loop over l1 tau candidates to get the single and double highest pt candidates
                    Float_t highestPt = -99.;
                    for(UInt_t iL1Tau = 0 ; iL1Tau < in_CalibPt->size() ; ++iL1Tau)
                    {
                        if(fabs(in_l1tEmuEta->at(iL1Tau))>2.1315) { continue; }

                        // get isolation cut form the current progression
                        Int_t IsoCut = currentProgression->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                        
                        // get single tau highest pt
                        if(in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut && in_CalibPt->at(iL1Tau)>highestPt) { highestPt = in_CalibPt->at(iL1Tau); }

                        // get double tau highest pts
                        if(in_CalibPt->at(iL1Tau)>=PtTausProgression.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut)
                        {
                            IndexTausProgression.at(1)=IndexTausProgression.at(0);
                            PtTausProgression.at(1)=PtTausProgression.at(0);
                            IndexTausProgression.at(0)=iL1Tau;
                            PtTausProgression.at(0)=in_CalibPt->at(iL1Tau);
                        }
                        else if(in_CalibPt->at(iL1Tau)>=PtTausProgression.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut)
                        {
                            IndexTausProgression.at(1)=iL1Tau;
                            PtTausProgression.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    }

                    // fill single tau pt distribution
                    TauPtPassDistribution->Fill(highestPt);
                    
                    // fill double tau pt distribution
                    if(IndexTausProgression.at(0)>=0 && IndexTausProgression.at(1)>=0) { DiTauPtPassDistribution->Fill(PtTausProgression.at(0), PtTausProgression.at(1)); }
                }

                // store pt pass distributions
                TauPtPassDistribution->Write();
                DiTauPtPassDistribution->Write();

                // create, fill, and write rate histograms
                TString TauRateName = "TauRate_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                TString DiTauRateName = "DiTauRate_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                TH1F* TauRate = new TH1F(TauRateName, TauRateName, 240, 0., 240.);
                TH1F* DiTauRate = new TH1F(DiTauRateName, DiTauRateName, 240, 0., 240.);

                for(Int_t ibin = 1 ; ibin <= DiTauRate->GetNbinsX() ; ++ibin)
                {
                    TauRate->SetBinContent(ibin,TauPtPassDistribution->Integral(ibin,241)/Denominator*scale);
                    DiTauRate->SetBinContent(ibin,DiTauPtPassDistribution->Integral(ibin,241,ibin,241)/Denominator*scale);
                }

                TauRate->Write();
                DiTauRate->Write();

                // store fixed threshold rates
                TVectorD CurrentRelaxationRate(7);
                CurrentRelaxationRate[0] = -999.9; CurrentRelaxationRate[1] = -999.9; CurrentRelaxationRate[2] = -999.9; CurrentRelaxationRate[3] = -999.9; CurrentRelaxationRate[4] = -999.9; CurrentRelaxationRate[5] = -999.9; CurrentRelaxationRate[6] = -999.9;
                CurrentRelaxationRate[0] = DiTauRate->GetBinContent(31); // 30Gev
                CurrentRelaxationRate[1] = DiTauRate->GetBinContent(32); // 31Gev
                CurrentRelaxationRate[2] = DiTauRate->GetBinContent(33); // 32Gev
                CurrentRelaxationRate[3] = DiTauRate->GetBinContent(34); // 33Gev
                CurrentRelaxationRate[4] = DiTauRate->GetBinContent(35); // 34Gev
                CurrentRelaxationRate[5] = DiTauRate->GetBinContent(36); // 35Gev
                CurrentRelaxationRate[6] = DiTauRate->GetBinContent(37); // 36Gev
                TString FixedThresholdDiTauRateName = "FixedThresholdDiTauRates_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                CurrentRelaxationRate.Write(FixedThresholdDiTauRateName);
            }
        }
    }
    // END OF GRID SEARCH

    // DO NO-ISO CASE
    TH1F* TauPtPassDistribution_noIso = new TH1F("TauPtPassDistribution_noIso", "TauPtPassDistribution_noIso", 240, 0., 240.);
    TH2F* DiTauPtPassDistribution_noIso = new TH2F("DiTauPtPassDistribution_noIso", "DiTauPtPassDistribution_noIso", 240, 0., 240., 240, 0., 240.);

    Int_t Denominator = 0;

    std::cout << " Running No-Iso" << std::endl;

    // loop over events
    for(UInt_t ievt = 0 ; ievt < Nevents ; ++ievt)
    {
        inTree->GetEntry(ievt);
        if(ievt%100000==0) cout<<"Entry #"<<ievt<<endl; 
        // SET RUN INFO
        if (in_RunNumber == 362616) { if(in_lumi<0) continue; }
        if (in_RunNumber == 362617) { if(in_lumi<0) continue; }

        ++Denominator;

        // double tau filling pairs
        std::vector<Int_t> IndexTausProgression;
        IndexTausProgression.push_back(-1); IndexTausProgression.push_back(-1);
        std::vector<Float_t> PtTausProgression;
        PtTausProgression.push_back(-99.); PtTausProgression.push_back(-99.);

        // loop over l1 tau candidates to get the single and double highest pt candidates
        Float_t highestPt = -99.;
        for(UInt_t iL1Tau = 0 ; iL1Tau < in_CalibPt->size() ; ++iL1Tau)
        {
            if(fabs(in_l1tEmuEta->at(iL1Tau))>2.1315) { continue; }
            
            // get single tau highest pt
            if(in_CalibPt->at(iL1Tau)>highestPt) { highestPt = in_CalibPt->at(iL1Tau); }

            // get double tau highest pts
            if(in_CalibPt->at(iL1Tau)>=PtTausProgression.at(0))
            {
                IndexTausProgression.at(1)=IndexTausProgression.at(0);
                PtTausProgression.at(1)=PtTausProgression.at(0);
                IndexTausProgression.at(0)=iL1Tau;
                PtTausProgression.at(0)=in_CalibPt->at(iL1Tau);
            }
            else if(in_CalibPt->at(iL1Tau)>=PtTausProgression.at(1))
            {
                IndexTausProgression.at(1)=iL1Tau;
                PtTausProgression.at(1)=in_CalibPt->at(iL1Tau);
            }
        }

        // fill single tau pt distribution
        TauPtPassDistribution_noIso->Fill(highestPt);
        
        // fill double tau pt distribution
        if(IndexTausProgression.at(0)>=0 && IndexTausProgression.at(1)>=0) { DiTauPtPassDistribution_noIso->Fill(PtTausProgression.at(0), PtTausProgression.at(1)); }
    }

    
    // create, fill, and write rate histograms for no-iso cases
    TH1F* TauRate_noIso = new TH1F("TauRate_noIso", "TauRate_noIso", 240, 0., 240.);
    TH1F* DiTauRate_noIso = new TH1F("DiTauRate_noIso", "DiTauRate_noIso", 240, 0., 240.);

    for(Int_t ibin =  1 ; ibin < DiTauRate_noIso->GetNbinsX() ; ++ibin)
    {
        TauRate_noIso->SetBinContent(ibin,TauPtPassDistribution_noIso->Integral(ibin,241)/Denominator*scale);
        DiTauRate_noIso->SetBinContent(ibin,DiTauPtPassDistribution_noIso->Integral(ibin,241,ibin,241)/Denominator*scale);
    }

    TauRate_noIso->Write();
    DiTauRate_noIso->Write();
    
    // store fixed threshold rates
    TVectorD FixedThresholdDiTauRate_noIso(7);
    FixedThresholdDiTauRate_noIso[0] = -999.9; FixedThresholdDiTauRate_noIso[1] = -999.9; FixedThresholdDiTauRate_noIso[2] = -999.9; FixedThresholdDiTauRate_noIso[3] = -999.9; FixedThresholdDiTauRate_noIso[4] = -999.9; FixedThresholdDiTauRate_noIso[5] = -999.9; FixedThresholdDiTauRate_noIso[6] = -999.9;
    FixedThresholdDiTauRate_noIso[0] = DiTauRate_noIso->GetBinContent(31); // 30 Gev
    FixedThresholdDiTauRate_noIso[1] = DiTauRate_noIso->GetBinContent(32); // 31 Gev
    FixedThresholdDiTauRate_noIso[2] = DiTauRate_noIso->GetBinContent(33); // 32 Gev
    FixedThresholdDiTauRate_noIso[3] = DiTauRate_noIso->GetBinContent(34); // 33 Gev
    FixedThresholdDiTauRate_noIso[4] = DiTauRate_noIso->GetBinContent(35); // 34 Gev
    FixedThresholdDiTauRate_noIso[5] = DiTauRate_noIso->GetBinContent(36); // 35 Gev
    FixedThresholdDiTauRate_noIso[6] = DiTauRate_noIso->GetBinContent(37); // 36 Gev
    FixedThresholdDiTauRate_noIso.Write("FixedThresholdDiTauRate_noIso");


    f_out.Close();

    return;
}
