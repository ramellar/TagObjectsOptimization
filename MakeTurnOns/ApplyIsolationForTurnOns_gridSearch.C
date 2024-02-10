#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <iostream>
#include <TLorentzVector.h>
#include <TVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <TROOT.h>
#include <sstream>
#include <TBranchElement.h>
#include <fstream>
#include <TGraphAsymmErrors.h>
#include <stdio.h>
#include <math.h>
#include "../Isolate/Fill_RelaxedIsolation_gridsearch_nTTextrap.C"

using namespace std;

double acceptacePercentage(TH1F* pass, TH1F* tot, float pt) {
    int binxp = 0;
    for (int i = 1; i <= pass->GetNbinsX(); ++i)
    {
        if (pass->GetBinLowEdge(i) >= pt) 
        {
            binxp = i;
            break;
        }
    }
    if (binxp == 0) binxp = pass->GetNbinsX();

    return pass->Integral(binxp,pass->GetNbinsX()+1) / tot->Integral(binxp,tot->GetNbinsX()+1);
}

// void ApplyIsolationForTurnOns(TString date, TString version, int run, int targetRate = 0, int fixedThr = 34, float calibThr = 1.7)
void ApplyIsolationForTurnOns(TString version, int run, int targetRate = 14, int fixedThr = 0, float calibThr = 1.7)
{
    TString run_str = to_string(run);

    TString fixedRate = to_string(targetRate);
    TString fixedThreshold = to_string(fixedThr);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    // target remap
    std::map<int, int> targetRemap;
    targetRemap.insert(make_pair(12,0));
    targetRemap.insert(make_pair(13,1));
    targetRemap.insert(make_pair(14,2));
    targetRemap.insert(make_pair(15,3));
    targetRemap.insert(make_pair(16,4));

    int targetIdx = targetRemap[targetRate];

    TFile f_Isolation("/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/Isolate/ROOTs4LUTs_2023/LUTrelaxation_2023_07_27_Run3_MC_optimization"+version+"_olivier_linear_current.root","READ");
    TFile f_Thresholds("/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/thresholds_fixedrate_ZeroBias_Run"+run_str+"_optimization"+version+"_olivier_2023_07_27_current.root","READ");
    TFile f_MCunpacked("/data_CMS/cms/mchiusi/Run3preparation/Run3preparation_2023/2023_07_27_olivier/current_calo_params/MINIAOD_124X_current.root", "READ");

    TString InputFileName = "/data_CMS/cms/mchiusi/Run3preparation/Run3preparation_2023/2023_07_27_olivier/current_calo_params/RAW_124X_CALIBRATED_current.root";
    TFile f(InputFileName.Data(),"READ");
    TTree* inTree = (TTree*)f.Get("outTreeCalibrated");

    Int_t           in_Nvtx;
    Float_t         in_L1Tau_CalibPt;
    Float_t         in_OfflineTau_pt;
    Float_t         in_OfflineTau_eta;
    Int_t           in_compressedieta;
    Int_t           in_compressedE;
    Int_t           in_compressednTT;
    Int_t           in_L1Tau_Iso;

    inTree->SetBranchAddress("Nvtx", &in_Nvtx);
    inTree->SetBranchAddress("L1Tau_CalibPt", &in_L1Tau_CalibPt);
    inTree->SetBranchAddress("OfflineTau_pt", &in_OfflineTau_pt);
    inTree->SetBranchAddress("OfflineTau_eta", &in_OfflineTau_eta);
    inTree->SetBranchAddress("compressedieta", &in_compressedieta);
    inTree->SetBranchAddress("compressedE", &in_compressedE);
    inTree->SetBranchAddress("compressednTT", &in_compressednTT);
    inTree->SetBranchAddress("L1Tau_Iso", &in_L1Tau_Iso);

    // binning for turnons compuattions and display
    Double_t binning[22] = {18,20,22,24,26,28,30,32,35,40,45,50,60,70,90,110,210,350,500,700,1000,2000};

    TString FileNameOut = "/data_CMS/cms/mchiusi/Run3preparation/Run3preparation_2023/2023_07_27_olivier/current_calo_params/Tau_MC_TURNONS";
    if (fixedThr==0) FileNameOut += "_FIXEDRATE"+fixedRate+"kHz";
    else             FileNameOut += "_FIXEDTHR"+fixedThreshold+"GeV";
    FileNameOut += "_Run"+run_str;
    FileNameOut += "_"+version+".root";
    TFile f_turnons(FileNameOut.Data(),"RECREATE");

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
                TString ProgressionName = "LUT_progression_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                TH3F* currentProgression = (TH3F*)f_Isolation.Get(ProgressionName);
                std::cout << " Running " << ProgressionName << std::endl;

                TString TotName = "tot_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                TH1F* tot = new TH1F(TotName, TotName, 21, binning);

                TString PassName = "passIso_progression_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                TH1F* pass_Iso  = new TH1F(PassName, PassName, 21, binning);

                Float_t Threshold_Iso;
                if (fixedThr != 0) { Threshold_Iso = fixedThr; }
                else
                {
                    TString ThresholdsName = "Thresholds_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                    TVectorD* ThresholdsVect_Iso  = (TVectorD*)f_Thresholds.Get(ThresholdsName);
                    Threshold_Iso = ThresholdsVect_Iso[0][targetIdx];
                }

                for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
                {
                    inTree->GetEntry(i);
                    //if(in_Nvtx<40 && in_Nvtx>50) continue;
                    if(fabs(in_OfflineTau_eta)>2.1) continue;

                    tot->Fill(in_OfflineTau_pt);

                    Int_t IsoCut  = currentProgression->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
                    if(in_L1Tau_CalibPt>=Threshold_Iso && in_L1Tau_Iso<=IsoCut) { pass_Iso->Fill(in_OfflineTau_pt); }
                }

                TString TurnonName = "TurnOn_progression_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                TGraphAsymmErrors* turnOn = new TGraphAsymmErrors(pass_Iso, tot, "cp");
                turnOn->Write(TurnonName);

                TVectorD acceptance(4);
                acceptance[0] = 0.0; acceptance[1] = 0.0; acceptance[2] = 0.0; acceptance[3] = 0.0;
                acceptance[0] = acceptacePercentage(pass_Iso, tot, 0);
                acceptance[1] = acceptacePercentage(pass_Iso, tot, 20);
                acceptance[2] = acceptacePercentage(pass_Iso, tot, 40);
                acceptance[3] = acceptacePercentage(pass_Iso, tot, 60);
                TString AcceptanceName = "Acceptance_progression_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                acceptance.Write(AcceptanceName);

            }
        }
    }
    // END OF GRID SEARCH

    // DO NO-ISO CASE
    std::cout << " Running No-Iso" << std::endl;
    TH1F* tot = new TH1F("tot","tot",21,binning);
    TH1F* pass_noIso   = new TH1F("pass_noIso", "pass_noIso", 21, binning);
    Float_t Threshold_noIso;
    if (fixedThr != 0) { Threshold_noIso = fixedThr; }
    else
    {
        TVectorD* ThresholdsVect_noIso = (TVectorD*)f_Thresholds.Get("Thresholds_noIso");
        Threshold_noIso = ThresholdsVect_noIso[0][targetIdx];
    }
    for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    {
        inTree->GetEntry(i);
        //if(in_Nvtx<40 && in_Nvtx>50) continue;
        if(fabs(in_OfflineTau_eta)>2.1) continue;

        tot->Fill(in_OfflineTau_pt);

        if(in_L1Tau_CalibPt>=Threshold_noIso) { pass_noIso->Fill(in_OfflineTau_pt); }
    }

    TGraphAsymmErrors* turnOn_noIso = new TGraphAsymmErrors(pass_noIso,tot,"cp");
    turnOn_noIso->Write("TurnOn_noIso");

    TVectorD acceptance_noIso(4);
    acceptance_noIso[0] = 0.0; acceptance_noIso[1] = 0.0; acceptance_noIso[2] = 0.0; acceptance_noIso[3] = 0.0;
    acceptance_noIso[0] = acceptacePercentage(pass_noIso, tot, 0);
    acceptance_noIso[1] = acceptacePercentage(pass_noIso, tot, 20);
    acceptance_noIso[2] = acceptacePercentage(pass_noIso, tot, 40);
    acceptance_noIso[3] = acceptacePercentage(pass_noIso, tot, 60);
    acceptance_noIso.Write("Acceptance_noIso");

    // DO UNPACKED CASE
    std::cout << " Running unpacked" << std::endl;

    TTree* inTree_unpacked = (TTree*)f_MCunpacked.Get("Ntuplizer_noTagAndProbe/TagAndProbe");
    
    float PtThr_2022 = 34.;
    float in_offlinePt = 0;
    float in_l1tPt = 0;
    float in_l1tEta = 0;
    int   in_l1tIso = 0;
    // branches of uncalibrated EphemeralZeroBias NTuples
    inTree_unpacked->SetBranchAddress("tauPt",&in_offlinePt);
    inTree_unpacked->SetBranchAddress("l1tPt",&in_l1tPt);
    inTree_unpacked->SetBranchAddress("l1tEta",&in_l1tEta);
    inTree_unpacked->SetBranchAddress("l1tIso",&in_l1tIso);

    TH1F* MCunpacked2022_tot = new TH1F("MCunpacked2022_tot","MCunpacked2022_tot",21,binning);
    TH1F* MCunpacked2022_Iso = new TH1F("MCunpacked2022_Iso","MCunpacked2022_Iso",21,binning);
    TH1F* MCunpacked2022_noIso = new TH1F("MCunpacked2022_noIso","MCunpacked2022_noIso",21,binning);

    for(UInt_t i = 0 ; i < inTree_unpacked->GetEntries() ; ++i)
    {
        inTree_unpacked->GetEntry(i);

        if(fabs(in_l1tEta)>2.1) continue;

        MCunpacked2022_tot->Fill(in_offlinePt);
        if(in_l1tPt>=PtThr_2022 && in_l1tIso>0) { MCunpacked2022_Iso->Fill(in_offlinePt); }
        if(in_l1tPt>=PtThr_2022)                { MCunpacked2022_noIso->Fill(in_offlinePt); }
    }

    TGraphAsymmErrors* turnOn_MCunpacked2022_Iso = new TGraphAsymmErrors(MCunpacked2022_Iso,MCunpacked2022_tot,"cp");
    turnOn_MCunpacked2022_Iso->Write("TurnOn_unpacked_Iso");
    TVectorD acceptance_unpacked_Iso(4);
    acceptance_unpacked_Iso[0] = 0.0; acceptance_unpacked_Iso[1] = 0.0; acceptance_unpacked_Iso[2] = 0.0; acceptance_unpacked_Iso[3] = 0.0;
    acceptance_unpacked_Iso[0] = acceptacePercentage(MCunpacked2022_Iso, MCunpacked2022_tot, 0);
    acceptance_unpacked_Iso[1] = acceptacePercentage(MCunpacked2022_Iso, MCunpacked2022_tot, 20);
    acceptance_unpacked_Iso[2] = acceptacePercentage(MCunpacked2022_Iso, MCunpacked2022_tot, 40);
    acceptance_unpacked_Iso[3] = acceptacePercentage(MCunpacked2022_Iso, MCunpacked2022_tot, 60);
    acceptance_unpacked_Iso.Write("Acceptance_Iso");

    TGraphAsymmErrors* turnOn_MCunpacked2022_noIso = new TGraphAsymmErrors(MCunpacked2022_noIso,MCunpacked2022_tot,"cp");
    turnOn_MCunpacked2022_noIso->Write("TurnOn_unpacked_noIso");

    TVectorD acceptance_unpacked_noIso(4);
    acceptance_unpacked_noIso[0] = 0.0; acceptance_unpacked_noIso[1] = 0.0; acceptance_unpacked_noIso[2] = 0.0; acceptance_unpacked_noIso[3] = 0.0;
    acceptance_unpacked_noIso[0] = acceptacePercentage(MCunpacked2022_noIso, MCunpacked2022_tot, 0);
    acceptance_unpacked_noIso[1] = acceptacePercentage(MCunpacked2022_noIso, MCunpacked2022_tot, 20);
    acceptance_unpacked_noIso[2] = acceptacePercentage(MCunpacked2022_noIso, MCunpacked2022_tot, 40);
    acceptance_unpacked_noIso[3] = acceptacePercentage(MCunpacked2022_noIso, MCunpacked2022_tot, 60);
    acceptance_unpacked_noIso.Write("Acceptance_noIso");

    f_turnons.Close();

    return;
}
