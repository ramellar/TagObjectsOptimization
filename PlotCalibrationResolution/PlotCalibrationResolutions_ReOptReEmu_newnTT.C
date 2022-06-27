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
#include <TF1.h>
#include <stdio.h>
#include <math.h>
#include "../Calibrate/ApplyCalibration_newnTT.C"

using namespace std;

TF1 fitResponseHisto(TString option, TH1F* hist2fit, float min_range, float max_range, float norm, float mean, float sigma, float alpha=0., float beta=0.) {
    TF1* f;
    if (option=="gaussian")
    {
        f  = new TF1("f","gaus",min_range,max_range);
        f->SetParameters(norm, mean, sigma);
        hist2fit->Fit(f);
    }
    else 
    {
        f  = new TF1("f","crystalball",min_range,max_range);
        f->SetParameters(norm, mean, sigma, alpha, beta);
        hist2fit->Fit(f);
    }
    
    return *f;
}


void plotResolutions(TString date, int vers, int targetRate = 14, TString fitOption="crystalball")
{
    TString version = to_string(vers);
    TString fixedRate = to_string(targetRate);

    // PT RESPONSE - PT BINS HISTOGRAMS
    TH1F* pt_barrel_resp_ptInclusive = new TH1F("pt_barrel_resp_ptInclusive","pt_barrel_resp_ptInclusive",60,0,3);
    TH1F* pt_barrel_resp_ptBin20to25 = new TH1F("pt_barrel_resp_ptBin20to25","pt_barrel_resp_ptBin20to25",60,0,3);
    TH1F* pt_barrel_resp_ptBin25to30 = new TH1F("pt_barrel_resp_ptBin25to30","pt_barrel_resp_ptBin25to30",60,0,3);
    TH1F* pt_barrel_resp_ptBin30to35 = new TH1F("pt_barrel_resp_ptBin30to35","pt_barrel_resp_ptBin30to35",60,0,3);
    TH1F* pt_barrel_resp_ptBin35to40 = new TH1F("pt_barrel_resp_ptBin35to40","pt_barrel_resp_ptBin35to40",60,0,3);
    TH1F* pt_barrel_resp_ptBin40to45 = new TH1F("pt_barrel_resp_ptBin40to45","pt_barrel_resp_ptBin40to45",60,0,3);
    TH1F* pt_barrel_resp_ptBin45to50 = new TH1F("pt_barrel_resp_ptBin45to50","pt_barrel_resp_ptBin45to50",60,0,3);
    TH1F* pt_barrel_resp_ptBin50to60 = new TH1F("pt_barrel_resp_ptBin50to60","pt_barrel_resp_ptBin50to60",60,0,3);
    TH1F* pt_barrel_resp_ptBin60to70 = new TH1F("pt_barrel_resp_ptBin60to70","pt_barrel_resp_ptBin60to70",60,0,3);
    TH1F* pt_barrel_resp_ptBin70to90 = new TH1F("pt_barrel_resp_ptBin70to90","pt_barrel_resp_ptBin70to90",60,0,3);
    TH1F* pt_barrel_resp_ptBin90to110 = new TH1F("pt_barrel_resp_ptBin90to110","pt_barrel_resp_ptBin90to110",60,0,3);

    TH1F* pt_endcap_resp_ptInclusive = new TH1F("pt_endcap_resp_ptInclusive","pt_endcap_resp_ptInclusive",60,0,3);
    TH1F* pt_endcap_resp_ptBin20to25 = new TH1F("pt_endcap_resp_ptBin20to25","pt_endcap_resp_ptBin20to25",60,0,3);
    TH1F* pt_endcap_resp_ptBin25to30 = new TH1F("pt_endcap_resp_ptBin25to30","pt_endcap_resp_ptBin25to30",60,0,3);
    TH1F* pt_endcap_resp_ptBin30to35 = new TH1F("pt_endcap_resp_ptBin30to35","pt_endcap_resp_ptBin30to35",60,0,3);
    TH1F* pt_endcap_resp_ptBin35to40 = new TH1F("pt_endcap_resp_ptBin35to40","pt_endcap_resp_ptBin35to40",60,0,3);
    TH1F* pt_endcap_resp_ptBin40to45 = new TH1F("pt_endcap_resp_ptBin40to45","pt_endcap_resp_ptBin40to45",60,0,3);
    TH1F* pt_endcap_resp_ptBin45to50 = new TH1F("pt_endcap_resp_ptBin45to50","pt_endcap_resp_ptBin45to50",60,0,3);
    TH1F* pt_endcap_resp_ptBin50to60 = new TH1F("pt_endcap_resp_ptBin50to60","pt_endcap_resp_ptBin50to60",60,0,3);
    TH1F* pt_endcap_resp_ptBin60to70 = new TH1F("pt_endcap_resp_ptBin60to70","pt_endcap_resp_ptBin60to70",60,0,3);
    TH1F* pt_endcap_resp_ptBin70to90 = new TH1F("pt_endcap_resp_ptBin70to90","pt_endcap_resp_ptBin70to90",60,0,3);
    TH1F* pt_endcap_resp_ptBin90to110 = new TH1F("pt_endcap_resp_ptBin90to110","pt_endcap_resp_ptBin90to110",60,0,3);

    // PT RESPONSE - FULL INCLUSIVE
    TH1F* pt_response_ptInclusive_mc = new TH1F("pt_response_ptInclusive_mc","pt_response_ptInclusive_mc",60,0,3);
    TH1F* pt_response_ptInclusive_data = new TH1F("pt_response_ptInclusive_data","pt_response_ptInclusive_data",60,0,3);

    TH1F* pt_response_uncalib_ptInclusive_mc = new TH1F("pt_response_uncalib_ptInclusive_mc","pt_response_uncalib_ptInclusive_mc",60,0,3);
    TH1F* pt_response_calib_ptInclusive_mc = new TH1F("pt_response_calib_ptInclusive_mc","pt_response_calib_ptInclusive_mc",60,0,3);

    // eta/phi RESPONSE - FULL INCLUSIVE
    TH1F* eta_response_barrel = new TH1F("eta_response_barrel","eta_response_barrel",30,-0.15,0.25);
    TH1F* eta_response_endcap = new TH1F("eta_response_endcap","eta_response_endcap",30,-0.15,0.25);
    TH1F* phi_response_barrel = new TH1F("phi_response_barrel","phi_response_barrel",30,-0.15,0.25);
    TH1F* phi_response_endcap = new TH1F("phi_response_endcap","phi_response_endcap",30,-0.15,0.25);

    // PT RESPONSE -  ETA BINS HISTIGRAMS
    TH1F* pt_resp_AbsEtaBin0to0p5 = new TH1F("pt_resp_AbsEtaBin0to0p5","pt_resp_AbsEtaBin0to0p5",60,0,3);
    TH1F* pt_resp_AbsEtaBin0p5to1 = new TH1F("pt_resp_AbsEtaBin0p5to1","pt_resp_AbsEtaBin0p5to1",60,0,3);
    TH1F* pt_resp_AbsEtaBin1to1p5 = new TH1F("pt_resp_AbsEtaBin1to1p5","pt_resp_AbsEtaBin1to1p5",60,0,3);
    TH1F* pt_resp_AbsEtaBin1p5to2p1 = new TH1F("pt_resp_AbsEtaBin1p5to2p1","pt_resp_AbsEtaBin1p5to2p1",60,0,3);

    TH1F* pt_resp_MinusEtaBin0to0p5 = new TH1F("pt_resp_MinusEtaBin0to0p5","pt_resp_MinusEtaBin0to0p5",60,0,3);
    TH1F* pt_resp_MinusEtaBin0p5to1 = new TH1F("pt_resp_MinusEtaBin0p5to1","pt_resp_MinusEtaBin0p5to1",60,0,3);
    TH1F* pt_resp_MinusEtaBin1to1p5 = new TH1F("pt_resp_MinusEtaBin1to1p5","pt_resp_MinusEtaBin1to1p5",60,0,3);
    TH1F* pt_resp_MinusEtaBin1p5to2p1 = new TH1F("pt_resp_MinusEtaBin1p5to2p1","pt_resp_MinusEtaBin1p5to2p1",60,0,3);

    TH1F* pt_resp_PlusEtaBin0to0p5 = new TH1F("pt_resp_PlusEtaBin0to0p5","pt_resp_PlusEtaBin0to0p5",60,0,3);
    TH1F* pt_resp_PlusEtaBin0p5to1 = new TH1F("pt_resp_PlusEtaBin0p5to1","pt_resp_PlusEtaBin0p5to1",60,0,3);
    TH1F* pt_resp_PlusEtaBin1to1p5 = new TH1F("pt_resp_PlusEtaBin1to1p5","pt_resp_PlusEtaBin1to1p5",60,0,3);
    TH1F* pt_resp_PlusEtaBin1p5to2p1 = new TH1F("pt_resp_PlusEtaBin1p5to2p1","pt_resp_PlusEtaBin1p5to2p1",60,0,3);

    // PT RESOLUTION HISTGRAMS
    Double_t pt_bins[11] = {20,25,30,35,40,45,50,60,70,90,110};
    Double_t absEta_bins[5] = {0.,0.5,1.,1.5,2.1};
    Double_t eta_bins[9] = {-2.1,-1.5,-1.,-0.5,0.,0.5,1.,1.5,2.1};
    TH1F* pt_resol_barrel_fctPt = new TH1F("pt_resol_barrel_fctPt","pt_resol_barrel_fctPt",10, pt_bins);
    TH1F* pt_resol_endcap_fctPt = new TH1F("pt_resol_endcap_fctPt","pt_resol_endcap_fctPt",10, pt_bins);
    TH1F* pt_resol_fctAbsEta = new TH1F("pt_resol_fctAbsEta","pt_resol_fctAbsEta",4, absEta_bins);
    TH1F* pt_resol_fctEta = new TH1F("pt_resol_fctEta","pt_resol_fctEta",8, eta_bins);

    // // ETA RESOLUTION - ETA BINS HISTIGRAMS
    // TH1F* eta_resol_etaBin0to0p5 = new TH1F("eta_resol_etaBin0to0p5","eta_resol_etaBin0to0p5",60,0,3);
    // TH1F* eta_resol_etaBin0p5to1 = new TH1F("eta_resol_etaBin0p5to1","eta_resol_etaBin0p5to1",60,0,3);
    // TH1F* eta_resol_etaBin1to1p5 = new TH1F("eta_resol_etaBin1to1p5","eta_resol_etaBin1to1p5",60,0,3);
    // TH1F* eta_resol_etaBin1p5to2p1 = new TH1F("eta_resol_etaBin1p5to2p1","eta_resol_etaBin1p5to2p1",60,0,3);

    // PT RESOLUTION - PT_ETA BINS HISTOGRAM
    Double_t pt_bins_2d[11] = {20,25,30,35,40,45,50,60,70,90,110};
    Double_t eta_bins_2d[17] = {-2.1,-1.75,-1.5,-1.25,-1.,-0.75,-0.5,-0.25,0.,0.25,0.5,0.75,1.,1.25,1.5,1.75,2.1};
    TH2F* PTvsETA_response = new TH2F("PTvsETA_response","PTvsETA_response",10,pt_bins_2d,16,eta_bins_2d);
    TH2F* PTvsETA_events = new TH2F("PTvsETA_events","PTvsETA_events",10,pt_bins_2d,16,eta_bins_2d);

    TString InputFileName0 = "/data_CMS/cms/motta/Run3preparation/"+date+"_optimizationV"+version+"_calibThr1p7/Run3_MC_VBFHToTauTau_M125_CALIBRATED_"+date+".root";
    TFile f0(InputFileName0.Data(),"READ");
    TTree* inTree0 = (TTree*)f0.Get("outTreeCalibrated");

    Float_t in_OfflineTau_pt0 = 0;
    Float_t in_OfflineTau_eta0 = 0;
    Float_t in_L1Tau_Pt0 = 0;
    Float_t in_L1Tau_CalibPt0 = 0;
    //Float_t in_l1tEmuEta = 0;
    //Int_t   in_l1tEmuIso = 0;
        
    inTree0->SetBranchAddress("OfflineTau_pt", &in_OfflineTau_pt0);
    inTree0->SetBranchAddress("OfflineTau_eta", &in_OfflineTau_eta0);
    inTree0->SetBranchAddress("L1Tau_pt"     , &in_L1Tau_Pt0);
    inTree0->SetBranchAddress("L1Tau_CalibPt", &in_L1Tau_CalibPt0);

    for(UInt_t i = 0 ; i < inTree0->GetEntries() ; ++i)
    {
        inTree0->GetEntry(i);
        if(fabs(in_OfflineTau_eta0)>2.1) continue;

        pt_response_uncalib_ptInclusive_mc->Fill(in_L1Tau_Pt0/in_OfflineTau_pt0);
        pt_response_calib_ptInclusive_mc->Fill(in_L1Tau_CalibPt0/in_OfflineTau_pt0);
    }
    f0.Close();

    TString InputFileName = "/data_CMS/cms/motta/Run3preparation/"+date+"_optimizationV"+version+"_ReOptReEmu/Run3_MC_VBFHToTauTau_M125_MATCHED_ReOptReEmu"+fixedRate+"kHz_"+date+".root";
    TFile f(InputFileName.Data(),"READ");
    TTree* inTree = (TTree*)f.Get("outTreeForTurnOns");

    Float_t in_OfflineTau_pt = 0;
    Float_t in_OfflineTau_eta = 0;
    Float_t in_OfflineTau_phi = 0;
    Float_t in_L1Tau_CalibPt = 0;
    Float_t in_l1tEmuEta = 0;
    Float_t in_l1tEmuPhi = 0;
    //Int_t   in_l1tEmuIso = 0;
        
    inTree->SetBranchAddress("OfflineTau_pt", &in_OfflineTau_pt);
    inTree->SetBranchAddress("OfflineTau_eta", &in_OfflineTau_eta);
    inTree->SetBranchAddress("OfflineTau_phi", &in_OfflineTau_phi);
    inTree->SetBranchAddress("L1Tau_pt"     , &in_L1Tau_CalibPt);
    inTree->SetBranchAddress("L1Tau_eta"    , &in_l1tEmuEta);
    inTree->SetBranchAddress("L1Tau_phi"    , &in_l1tEmuPhi);
    //inTree->SetBranchAddress("L1Tau_IsoFlag", &in_l1tEmuIso);


    for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    {
        inTree->GetEntry(i);
        if(fabs(in_OfflineTau_eta)>2.1) continue;

        // FILL PT RESPONSE - PT BINS HISTOGRAMS
        if (fabs(in_OfflineTau_eta)<1.5)
        {
            pt_barrel_resp_ptInclusive->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 20 and in_OfflineTau_pt < 25) pt_barrel_resp_ptBin20to25->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 25 and in_OfflineTau_pt < 30) pt_barrel_resp_ptBin25to30->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 30 and in_OfflineTau_pt < 35) pt_barrel_resp_ptBin30to35->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 35 and in_OfflineTau_pt < 40) pt_barrel_resp_ptBin35to40->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 40 and in_OfflineTau_pt < 45) pt_barrel_resp_ptBin40to45->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 45 and in_OfflineTau_pt < 50) pt_barrel_resp_ptBin45to50->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 50 and in_OfflineTau_pt < 60) pt_barrel_resp_ptBin50to60->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 60 and in_OfflineTau_pt < 70) pt_barrel_resp_ptBin60to70->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 70 and in_OfflineTau_pt < 90) pt_barrel_resp_ptBin70to90->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 90 and in_OfflineTau_pt <= 110) pt_barrel_resp_ptBin90to110->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        }
        else
        {
            pt_endcap_resp_ptInclusive->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 20 and in_OfflineTau_pt < 25) pt_endcap_resp_ptBin20to25->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 25 and in_OfflineTau_pt < 30) pt_endcap_resp_ptBin25to30->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 30 and in_OfflineTau_pt < 35) pt_endcap_resp_ptBin30to35->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 35 and in_OfflineTau_pt < 40) pt_endcap_resp_ptBin35to40->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 40 and in_OfflineTau_pt < 45) pt_endcap_resp_ptBin40to45->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 45 and in_OfflineTau_pt < 50) pt_endcap_resp_ptBin45to50->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 50 and in_OfflineTau_pt < 60) pt_endcap_resp_ptBin50to60->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 60 and in_OfflineTau_pt < 70) pt_endcap_resp_ptBin60to70->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 70 and in_OfflineTau_pt < 90) pt_endcap_resp_ptBin70to90->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if (in_OfflineTau_pt >= 90 and in_OfflineTau_pt <= 110) pt_endcap_resp_ptBin90to110->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        }

        // FILL PT RESPONSE - FULL INCLUSIVE
        pt_response_ptInclusive_mc->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt);

        if (in_OfflineTau_pt >= 30)
        {
            if (fabs(in_OfflineTau_eta)<1.305){
                eta_response_barrel->Fill(in_l1tEmuEta-in_OfflineTau_eta);
                phi_response_barrel->Fill(in_l1tEmuPhi-in_OfflineTau_phi);
            }
            else if (fabs(in_OfflineTau_eta)<1.479){
                eta_response_endcap->Fill(in_l1tEmuEta-in_OfflineTau_eta);
                phi_response_endcap->Fill(in_l1tEmuPhi-in_OfflineTau_phi);
            }
        }

        // FILL PT RESPONSE - ETA BINS HISTOGRAMS
        if (in_OfflineTau_pt>=0 and abs(in_OfflineTau_eta) >= 0.0 and abs(in_OfflineTau_eta) < 0.5) pt_resp_AbsEtaBin0to0p5->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_pt>=0 and abs(in_OfflineTau_eta) >= 0.5 and abs(in_OfflineTau_eta) < 1.0) pt_resp_AbsEtaBin0p5to1->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_pt>=0 and abs(in_OfflineTau_eta) >= 1.0 and abs(in_OfflineTau_eta) < 1.5) pt_resp_AbsEtaBin1to1p5->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_pt>=0 and abs(in_OfflineTau_eta) >= 1.5 and abs(in_OfflineTau_eta) <= 2.1) pt_resp_AbsEtaBin1p5to2p1->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;

        if (in_OfflineTau_pt>=0 and in_OfflineTau_eta >= 0.0 and in_OfflineTau_eta < 0.5) pt_resp_PlusEtaBin0to0p5->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_pt>=0 and in_OfflineTau_eta >= 0.5 and in_OfflineTau_eta < 1.0) pt_resp_PlusEtaBin0p5to1->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_pt>=0 and in_OfflineTau_eta >= 1.0 and in_OfflineTau_eta < 1.5) pt_resp_PlusEtaBin1to1p5->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_pt>=0 and in_OfflineTau_eta >= 1.5 and in_OfflineTau_eta <= 2.1) pt_resp_PlusEtaBin1p5to2p1->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;

        if (in_OfflineTau_pt>=0 and in_OfflineTau_eta <=  0.0 and in_OfflineTau_eta > -0.5) pt_resp_MinusEtaBin0to0p5->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_pt>=0 and in_OfflineTau_eta <= -0.5 and in_OfflineTau_eta > -1.0) pt_resp_MinusEtaBin0p5to1->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_pt>=0 and in_OfflineTau_eta <= -1.0 and in_OfflineTau_eta > -1.5) pt_resp_MinusEtaBin1to1p5->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_pt>=0 and in_OfflineTau_eta <= -1.5 and in_OfflineTau_eta >= -2.1) pt_resp_MinusEtaBin1p5to2p1->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
    
        // FILL PT RESPONSE - PT_ETA BINS HISTOGRAMS
        PTvsETA_response->Fill(in_OfflineTau_pt, in_OfflineTau_eta, in_L1Tau_CalibPt/in_OfflineTau_pt);
        PTvsETA_events->Fill(in_OfflineTau_pt, in_OfflineTau_eta);
    }
    f.Close();

    // NORMALIZE THE HISTOGRAMS
    pt_barrel_resp_ptInclusive->Scale(1.0/pt_barrel_resp_ptInclusive->Integral());
    pt_barrel_resp_ptBin20to25->Scale(1.0/pt_barrel_resp_ptBin20to25->Integral());
    pt_barrel_resp_ptBin25to30->Scale(1.0/pt_barrel_resp_ptBin25to30->Integral());
    pt_barrel_resp_ptBin30to35->Scale(1.0/pt_barrel_resp_ptBin30to35->Integral());
    pt_barrel_resp_ptBin35to40->Scale(1.0/pt_barrel_resp_ptBin35to40->Integral());
    pt_barrel_resp_ptBin40to45->Scale(1.0/pt_barrel_resp_ptBin40to45->Integral());
    pt_barrel_resp_ptBin45to50->Scale(1.0/pt_barrel_resp_ptBin20to25->Integral());
    pt_barrel_resp_ptBin50to60->Scale(1.0/pt_barrel_resp_ptBin25to30->Integral());
    pt_barrel_resp_ptBin60to70->Scale(1.0/pt_barrel_resp_ptBin30to35->Integral());
    pt_barrel_resp_ptBin70to90->Scale(1.0/pt_barrel_resp_ptBin35to40->Integral());
    pt_barrel_resp_ptBin90to110->Scale(1.0/pt_barrel_resp_ptBin40to45->Integral());

    pt_endcap_resp_ptInclusive->Scale(1.0/pt_endcap_resp_ptInclusive->Integral());
    pt_endcap_resp_ptBin20to25->Scale(1.0/pt_endcap_resp_ptBin20to25->Integral());
    pt_endcap_resp_ptBin25to30->Scale(1.0/pt_endcap_resp_ptBin25to30->Integral());
    pt_endcap_resp_ptBin30to35->Scale(1.0/pt_endcap_resp_ptBin30to35->Integral());
    pt_endcap_resp_ptBin35to40->Scale(1.0/pt_endcap_resp_ptBin35to40->Integral());
    pt_endcap_resp_ptBin40to45->Scale(1.0/pt_endcap_resp_ptBin40to45->Integral());
    pt_endcap_resp_ptBin45to50->Scale(1.0/pt_endcap_resp_ptBin20to25->Integral());
    pt_endcap_resp_ptBin50to60->Scale(1.0/pt_endcap_resp_ptBin25to30->Integral());
    pt_endcap_resp_ptBin60to70->Scale(1.0/pt_endcap_resp_ptBin30to35->Integral());
    pt_endcap_resp_ptBin70to90->Scale(1.0/pt_endcap_resp_ptBin35to40->Integral());
    pt_endcap_resp_ptBin90to110->Scale(1.0/pt_endcap_resp_ptBin40to45->Integral());

    pt_resp_AbsEtaBin0to0p5->Scale(1.0/pt_resp_AbsEtaBin0to0p5->Integral());
    pt_resp_AbsEtaBin0p5to1->Scale(1.0/pt_resp_AbsEtaBin0p5to1->Integral());
    pt_resp_AbsEtaBin1to1p5->Scale(1.0/pt_resp_AbsEtaBin1to1p5->Integral());
    pt_resp_AbsEtaBin1p5to2p1->Scale(1.0/pt_resp_AbsEtaBin1p5to2p1->Integral());

    pt_resp_PlusEtaBin0to0p5->Scale(1.0/pt_resp_PlusEtaBin0to0p5->Integral());
    pt_resp_PlusEtaBin0p5to1->Scale(1.0/pt_resp_PlusEtaBin0p5to1->Integral());
    pt_resp_PlusEtaBin1to1p5->Scale(1.0/pt_resp_PlusEtaBin1to1p5->Integral());
    pt_resp_PlusEtaBin1p5to2p1->Scale(1.0/pt_resp_PlusEtaBin1p5to2p1->Integral());

    pt_resp_MinusEtaBin0to0p5->Scale(1.0/pt_resp_MinusEtaBin0to0p5->Integral());
    pt_resp_MinusEtaBin0p5to1->Scale(1.0/pt_resp_MinusEtaBin0p5to1->Integral());
    pt_resp_MinusEtaBin1to1p5->Scale(1.0/pt_resp_MinusEtaBin1to1p5->Integral());
    pt_resp_MinusEtaBin1p5to2p1->Scale(1.0/pt_resp_MinusEtaBin1p5to2p1->Integral());

    //pt_response_ptInclusive_data->Scale(1.0/pt_response_ptInclusive_data->Integral());
    pt_response_ptInclusive_mc->Scale(1.0/pt_response_ptInclusive_mc->Integral());

    eta_response_barrel->Scale(1.0/eta_response_barrel->Integral());
    phi_response_barrel->Scale(1.0/phi_response_barrel->Integral());
    eta_response_endcap->Scale(1.0/eta_response_endcap->Integral());
    phi_response_endcap->Scale(1.0/phi_response_endcap->Integral());

    pt_response_uncalib_ptInclusive_mc->Scale(1.0/pt_response_uncalib_ptInclusive_mc->Integral());
    pt_response_calib_ptInclusive_mc->Scale(1.0/pt_response_calib_ptInclusive_mc->Integral());

    PTvsETA_response->Divide(PTvsETA_events);

    // FIT CRYSTALBALL FUNCTIONS
    TF1 fit_barrel_Inclusive; // = fitResponseHisto(fitOption, pt_barrel_resp_ptInclusive, 0, 3, pt_barrel_resp_ptInclusive->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin20to25; // = fitResponseHisto(fitOption, pt_barrel_resp_ptBin20to25, 0, 3, pt_barrel_resp_ptBin20to25->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin25to30; // = fitResponseHisto(fitOption, pt_barrel_resp_ptBin25to30, 0, 3, pt_barrel_resp_ptBin25to30->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin30to35; // = fitResponseHisto(fitOption, pt_barrel_resp_ptBin30to35, 0, 3, pt_barrel_resp_ptBin30to35->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin35to40; // = fitResponseHisto(fitOption, pt_barrel_resp_ptBin35to40, 0, 3, pt_barrel_resp_ptBin35to40->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin40to45; // = fitResponseHisto(fitOption, pt_barrel_resp_ptBin40to45, 0, 3, pt_barrel_resp_ptBin40to45->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin45to50; // = fitResponseHisto(fitOption, pt_barrel_resp_ptBin45to50, 0, 3, pt_barrel_resp_ptBin45to50->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin50to60; // = fitResponseHisto(fitOption, pt_barrel_resp_ptBin50to60, 0, 3, pt_barrel_resp_ptBin50to60->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin60to70; // = fitResponseHisto(fitOption, pt_barrel_resp_ptBin60to70, 0, 3, pt_barrel_resp_ptBin60to70->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin70to90; // = fitResponseHisto(fitOption, pt_barrel_resp_ptBin70to90, 0, 3, pt_barrel_resp_ptBin70to90->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin90to110; // = fitResponseHisto(fitOption, pt_barrel_resp_ptBin90to110, 0, 3, pt_barrel_resp_ptBin90to110->Integral(), 1, 0.2, -1, 1);

    TF1 fit_endcap_Inclusive; // = fitResponseHisto(fitOption, pt_endcap_resp_ptInclusive, 0, 3, pt_endcap_resp_ptInclusive->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin20to25; // = fitResponseHisto(fitOption, pt_endcap_resp_ptBin20to25, 0, 3, pt_endcap_resp_ptBin20to25->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin25to30; // = fitResponseHisto(fitOption, pt_endcap_resp_ptBin25to30, 0, 3, pt_endcap_resp_ptBin25to30->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin30to35; // = fitResponseHisto(fitOption, pt_endcap_resp_ptBin30to35, 0, 3, pt_endcap_resp_ptBin30to35->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin35to40; // = fitResponseHisto(fitOption, pt_endcap_resp_ptBin35to40, 0, 3, pt_endcap_resp_ptBin35to40->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin40to45; // = fitResponseHisto(fitOption, pt_endcap_resp_ptBin40to45, 0, 3, pt_endcap_resp_ptBin40to45->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin45to50; // = fitResponseHisto(fitOption, pt_endcap_resp_ptBin45to50, 0, 3, pt_endcap_resp_ptBin45to50->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin50to60; // = fitResponseHisto(fitOption, pt_endcap_resp_ptBin50to60, 0, 3, pt_endcap_resp_ptBin50to60->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin60to70; // = fitResponseHisto(fitOption, pt_endcap_resp_ptBin60to70, 0, 3, pt_endcap_resp_ptBin60to70->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin70to90; // = fitResponseHisto(fitOption, pt_endcap_resp_ptBin70to90, 0, 3, pt_endcap_resp_ptBin70to90->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin90to110; // = fitResponseHisto(fitOption, pt_endcap_resp_ptBin90to110, 0, 3, pt_endcap_resp_ptBin90to110->Integral(), 1, 0.2, -1, 1);

    TF1 fit_AbsEtaBin0to0p5; // = fitResponseHisto(fitOption, pt_resp_AbsEtaBin0to0p5, 0, 3, pt_resp_AbsEtaBin0to0p5->Integral(), 1, 0.2, -1, 1);
    TF1 fit_AbsEtaBin0p5to1; // = fitResponseHisto(fitOption, pt_resp_AbsEtaBin0p5to1, 0, 3, pt_resp_AbsEtaBin0p5to1->Integral(), 1, 0.2, -1, 1);
    TF1 fit_AbsEtaBin1to1p5; // = fitResponseHisto(fitOption, pt_resp_AbsEtaBin1to1p5, 0, 3, pt_resp_AbsEtaBin1to1p5->Integral(), 1, 0.2, -1, 1);
    TF1 fit_AbsEtaBin1p5to2p1; // = fitResponseHisto(fitOption, pt_resp_AbsEtaBin1p5to2p1, 0, 3, pt_resp_AbsEtaBin1p5to2p1->Integral(), 1, 0.2, -1, 1);

    TF1 fit_PlusEtaBin0to0p5; // = fitResponseHisto(fitOption, pt_resp_PlusEtaBin0to0p5, 0, 3, pt_resp_PlusEtaBin0to0p5->Integral(), 1, 0.2, -1, 1);
    TF1 fit_PlusEtaBin0p5to1; // = fitResponseHisto(fitOption, pt_resp_PlusEtaBin0p5to1, 0, 3, pt_resp_PlusEtaBin0p5to1->Integral(), 1, 0.2, -1, 1);
    TF1 fit_PlusEtaBin1to1p5; // = fitResponseHisto(fitOption, pt_resp_PlusEtaBin1to1p5, 0, 3, pt_resp_PlusEtaBin1to1p5->Integral(), 1, 0.2, -1, 1);
    TF1 fit_PlusEtaBin1p5to2p1; // = fitResponseHisto(fitOption, pt_resp_PlusEtaBin1p5to2p1, 0, 3, pt_resp_PlusEtaBin1p5to2p1->Integral(), 1, 0.2, -1, 1);

    TF1 fit_MinusEtaBin0to0p5; // = fitResponseHisto(fitOption, pt_resp_MinusEtaBin0to0p5, 0, 3, pt_resp_MinusEtaBin0to0p5->Integral(), 1, 0.2, -1, 1);
    TF1 fit_MinusEtaBin0p5to1; // = fitResponseHisto(fitOption, pt_resp_MinusEtaBin0p5to1, 0, 3, pt_resp_MinusEtaBin0p5to1->Integral(), 1, 0.2, -1, 1);
    TF1 fit_MinusEtaBin1to1p5; // = fitResponseHisto(fitOption, pt_resp_MinusEtaBin1to1p5, 0, 3, pt_resp_MinusEtaBin1to1p5->Integral(), 1, 0.2, -1, 1);
    TF1 fit_MinusEtaBin1p5to2p1; // = fitResponseHisto(fitOption, pt_resp_MinusEtaBin1p5to2p1, 0, 3, pt_resp_MinusEtaBin1p5to2p1->Integral(), 1, 0.2, -1, 1);    

    // FILL RESOLUTION HISTOGRAMS AS FCT OF PT/ETA
    // pt_resol_barrel_fctPt->SetBinContent(1, fit_barrel_Bin20to25.GetParameter("Sigma"));
    // pt_resol_barrel_fctPt->SetBinError(1, fit_barrel_Bin20to25.GetParError(3));
    // pt_resol_barrel_fctPt->SetBinContent(2, fit_barrel_Bin25to30.GetParameter("Sigma"));
    // pt_resol_barrel_fctPt->SetBinError(2, fit_barrel_Bin25to30.GetParError(3));
    // pt_resol_barrel_fctPt->SetBinContent(3, fit_barrel_Bin30to35.GetParameter("Sigma"));
    // pt_resol_barrel_fctPt->SetBinError(3, fit_barrel_Bin30to35.GetParError(3));
    // pt_resol_barrel_fctPt->SetBinContent(4, fit_barrel_Bin35to40.GetParameter("Sigma"));
    // pt_resol_barrel_fctPt->SetBinError(4, fit_barrel_Bin35to40.GetParError(3));
    // pt_resol_barrel_fctPt->SetBinContent(5, fit_barrel_Bin40to45.GetParameter("Sigma"));
    // pt_resol_barrel_fctPt->SetBinError(5, fit_barrel_Bin40to45.GetParError(3));
    // pt_resol_barrel_fctPt->SetBinContent(6, fit_barrel_Bin45to50.GetParameter("Sigma"));
    // pt_resol_barrel_fctPt->SetBinError(6, fit_barrel_Bin45to50.GetParError(3));
    // pt_resol_barrel_fctPt->SetBinContent(7, fit_barrel_Bin50to60.GetParameter("Sigma"));
    // pt_resol_barrel_fctPt->SetBinError(7, fit_barrel_Bin50to60.GetParError(3));
    // pt_resol_barrel_fctPt->SetBinContent(8, fit_barrel_Bin60to70.GetParameter("Sigma"));
    // pt_resol_barrel_fctPt->SetBinError(8, fit_barrel_Bin60to70.GetParError(3));
    // pt_resol_barrel_fctPt->SetBinContent(9, fit_barrel_Bin70to90.GetParameter("Sigma"));
    // pt_resol_barrel_fctPt->SetBinError(9, fit_barrel_Bin70to90.GetParError(3));
    // pt_resol_barrel_fctPt->SetBinContent(10, fit_barrel_Bin90to110.GetParameter("Sigma"));
    // pt_resol_barrel_fctPt->SetBinError(10, fit_barrel_Bin90to110.GetParError(3));

    // pt_resol_endcap_fctPt->SetBinContent(1, fit_endcap_Bin20to25.GetParameter("Sigma"));
    // pt_resol_endcap_fctPt->SetBinError(1, fit_endcap_Bin20to25.GetParError(3));
    // pt_resol_endcap_fctPt->SetBinContent(2, fit_endcap_Bin25to30.GetParameter("Sigma"));
    // pt_resol_endcap_fctPt->SetBinError(2, fit_endcap_Bin25to30.GetParError(3));
    // pt_resol_endcap_fctPt->SetBinContent(3, fit_endcap_Bin30to35.GetParameter("Sigma"));
    // pt_resol_endcap_fctPt->SetBinError(3, fit_endcap_Bin30to35.GetParError(3));
    // pt_resol_endcap_fctPt->SetBinContent(4, fit_endcap_Bin35to40.GetParameter("Sigma"));
    // pt_resol_endcap_fctPt->SetBinError(4, fit_endcap_Bin35to40.GetParError(3));
    // pt_resol_endcap_fctPt->SetBinContent(5, fit_endcap_Bin40to45.GetParameter("Sigma"));
    // pt_resol_endcap_fctPt->SetBinError(5, fit_endcap_Bin40to45.GetParError(3));
    // pt_resol_endcap_fctPt->SetBinContent(6, fit_endcap_Bin45to50.GetParameter("Sigma"));
    // pt_resol_endcap_fctPt->SetBinError(6, fit_endcap_Bin45to50.GetParError(3));
    // pt_resol_endcap_fctPt->SetBinContent(7, fit_endcap_Bin50to60.GetParameter("Sigma"));
    // pt_resol_endcap_fctPt->SetBinError(7, fit_endcap_Bin50to60.GetParError(3));
    // pt_resol_endcap_fctPt->SetBinContent(8, fit_endcap_Bin60to70.GetParameter("Sigma"));
    // pt_resol_endcap_fctPt->SetBinError(8, fit_endcap_Bin60to70.GetParError(3));
    // pt_resol_endcap_fctPt->SetBinContent(9, fit_endcap_Bin70to90.GetParameter("Sigma"));
    // pt_resol_endcap_fctPt->SetBinError(9, fit_endcap_Bin70to90.GetParError(3));
    // pt_resol_endcap_fctPt->SetBinContent(10, fit_endcap_Bin90to110.GetParameter("Sigma"));
    // pt_resol_endcap_fctPt->SetBinError(10, fit_endcap_Bin90to110.GetParError(3));

    // pt_resol_fctAbsEta->SetBinContent(1, fit_AbsEtaBin0to0p5.GetParameter("Sigma"));
    // pt_resol_fctAbsEta->SetBinError(1, fit_AbsEtaBin0to0p5.GetParError(3));
    // pt_resol_fctAbsEta->SetBinContent(2, fit_AbsEtaBin0p5to1.GetParameter("Sigma"));
    // pt_resol_fctAbsEta->SetBinError(2, fit_AbsEtaBin0p5to1.GetParError(3));
    // pt_resol_fctAbsEta->SetBinContent(3, fit_AbsEtaBin1to1p5.GetParameter("Sigma"));
    // pt_resol_fctAbsEta->SetBinError(3, fit_AbsEtaBin1to1p5.GetParError(3));
    // pt_resol_fctAbsEta->SetBinContent(4, fit_AbsEtaBin1p5to2p1.GetParameter("Sigma"));
    // pt_resol_fctAbsEta->SetBinError(4, fit_AbsEtaBin1p5to2p1.GetParError(3));
    
    // pt_resol_fctEta->SetBinContent(1, fit_MinusEtaBin1p5to2p1.GetParameter("Sigma"));
    // pt_resol_fctEta->SetBinError(1, fit_MinusEtaBin1p5to2p1.GetParError(3));
    // pt_resol_fctEta->SetBinContent(2, fit_MinusEtaBin1to1p5.GetParameter("Sigma"));
    // pt_resol_fctEta->SetBinError(2, fit_MinusEtaBin1to1p5.GetParError(3));
    // pt_resol_fctEta->SetBinContent(3, fit_MinusEtaBin0p5to1.GetParameter("Sigma"));
    // pt_resol_fctEta->SetBinError(3, fit_MinusEtaBin0p5to1.GetParError(3));
    // pt_resol_fctEta->SetBinContent(4, fit_MinusEtaBin0to0p5.GetParameter("Sigma"));
    // pt_resol_fctEta->SetBinError(4, fit_MinusEtaBin0to0p5.GetParError(3));
    // pt_resol_fctEta->SetBinContent(5, fit_PlusEtaBin0to0p5.GetParameter("Sigma"));
    // pt_resol_fctEta->SetBinError(5, fit_PlusEtaBin0to0p5.GetParError(3));
    // pt_resol_fctEta->SetBinContent(6, fit_PlusEtaBin0p5to1.GetParameter("Sigma"));
    // pt_resol_fctEta->SetBinError(6, fit_PlusEtaBin0p5to1.GetParError(3));
    // pt_resol_fctEta->SetBinContent(7, fit_PlusEtaBin1to1p5.GetParameter("Sigma"));
    // pt_resol_fctEta->SetBinError(7, fit_PlusEtaBin1to1p5.GetParError(3));
    // pt_resol_fctEta->SetBinContent(8, fit_PlusEtaBin1p5to2p1.GetParameter("Sigma"));
    // pt_resol_fctEta->SetBinError(8, fit_PlusEtaBin1p5to2p1.GetParError(3));


    pt_resol_barrel_fctPt->SetBinContent(1, pt_barrel_resp_ptBin20to25->GetRMS());
    pt_resol_barrel_fctPt->SetBinError(1, pt_barrel_resp_ptBin20to25->GetRMSError());
    pt_resol_barrel_fctPt->SetBinContent(2, pt_barrel_resp_ptBin25to30->GetRMS());
    pt_resol_barrel_fctPt->SetBinError(2, pt_barrel_resp_ptBin25to30->GetRMSError());
    pt_resol_barrel_fctPt->SetBinContent(3, pt_barrel_resp_ptBin30to35->GetRMS());
    pt_resol_barrel_fctPt->SetBinError(3, pt_barrel_resp_ptBin30to35->GetRMSError());
    pt_resol_barrel_fctPt->SetBinContent(4, pt_barrel_resp_ptBin35to40->GetRMS());
    pt_resol_barrel_fctPt->SetBinError(4, pt_barrel_resp_ptBin35to40->GetRMSError());
    pt_resol_barrel_fctPt->SetBinContent(5, pt_barrel_resp_ptBin40to45->GetRMS());
    pt_resol_barrel_fctPt->SetBinError(5, pt_barrel_resp_ptBin40to45->GetRMSError());
    pt_resol_barrel_fctPt->SetBinContent(6, pt_barrel_resp_ptBin45to50->GetRMS());
    pt_resol_barrel_fctPt->SetBinError(6, pt_barrel_resp_ptBin45to50->GetRMSError());
    pt_resol_barrel_fctPt->SetBinContent(7, pt_barrel_resp_ptBin50to60->GetRMS());
    pt_resol_barrel_fctPt->SetBinError(7, pt_barrel_resp_ptBin50to60->GetRMSError());
    pt_resol_barrel_fctPt->SetBinContent(8, pt_barrel_resp_ptBin60to70->GetRMS());
    pt_resol_barrel_fctPt->SetBinError(8, pt_barrel_resp_ptBin60to70->GetRMSError());
    pt_resol_barrel_fctPt->SetBinContent(9, pt_barrel_resp_ptBin70to90->GetRMS());
    pt_resol_barrel_fctPt->SetBinError(9, pt_barrel_resp_ptBin70to90->GetRMSError());
    pt_resol_barrel_fctPt->SetBinContent(10, pt_barrel_resp_ptBin90to110->GetRMS());
    pt_resol_barrel_fctPt->SetBinError(10, pt_barrel_resp_ptBin90to110->GetRMSError());

    pt_resol_endcap_fctPt->SetBinContent(1, pt_endcap_resp_ptBin20to25->GetRMS());
    pt_resol_endcap_fctPt->SetBinError(1, pt_endcap_resp_ptBin20to25->GetRMSError());
    pt_resol_endcap_fctPt->SetBinContent(2, pt_endcap_resp_ptBin25to30->GetRMS());
    pt_resol_endcap_fctPt->SetBinError(2, pt_endcap_resp_ptBin25to30->GetRMSError());
    pt_resol_endcap_fctPt->SetBinContent(3, pt_endcap_resp_ptBin30to35->GetRMS());
    pt_resol_endcap_fctPt->SetBinError(3, pt_endcap_resp_ptBin30to35->GetRMSError());
    pt_resol_endcap_fctPt->SetBinContent(4, pt_endcap_resp_ptBin35to40->GetRMS());
    pt_resol_endcap_fctPt->SetBinError(4, pt_endcap_resp_ptBin35to40->GetRMSError());
    pt_resol_endcap_fctPt->SetBinContent(5, pt_endcap_resp_ptBin40to45->GetRMS());
    pt_resol_endcap_fctPt->SetBinError(5, pt_endcap_resp_ptBin40to45->GetRMSError());
    pt_resol_endcap_fctPt->SetBinContent(6, pt_endcap_resp_ptBin45to50->GetRMS());
    pt_resol_endcap_fctPt->SetBinError(6, pt_endcap_resp_ptBin45to50->GetRMSError());
    pt_resol_endcap_fctPt->SetBinContent(7, pt_endcap_resp_ptBin50to60->GetRMS());
    pt_resol_endcap_fctPt->SetBinError(7, pt_endcap_resp_ptBin50to60->GetRMSError());
    pt_resol_endcap_fctPt->SetBinContent(8, pt_endcap_resp_ptBin60to70->GetRMS());
    pt_resol_endcap_fctPt->SetBinError(8, pt_endcap_resp_ptBin60to70->GetRMSError());
    pt_resol_endcap_fctPt->SetBinContent(9, pt_endcap_resp_ptBin70to90->GetRMS());
    pt_resol_endcap_fctPt->SetBinError(9, pt_endcap_resp_ptBin70to90->GetRMSError());
    pt_resol_endcap_fctPt->SetBinContent(10, pt_endcap_resp_ptBin90to110->GetRMS());
    pt_resol_endcap_fctPt->SetBinError(10, pt_endcap_resp_ptBin90to110->GetRMSError());

    pt_resol_fctAbsEta->SetBinContent(1, pt_resp_AbsEtaBin0to0p5->GetRMS());
    pt_resol_fctAbsEta->SetBinError(1, pt_resp_AbsEtaBin0to0p5->GetRMSError());
    pt_resol_fctAbsEta->SetBinContent(2, pt_resp_AbsEtaBin0p5to1->GetRMS());
    pt_resol_fctAbsEta->SetBinError(2, pt_resp_AbsEtaBin0p5to1->GetRMSError());
    pt_resol_fctAbsEta->SetBinContent(3, pt_resp_AbsEtaBin1to1p5->GetRMS());
    pt_resol_fctAbsEta->SetBinError(3, pt_resp_AbsEtaBin1to1p5->GetRMSError());
    pt_resol_fctAbsEta->SetBinContent(4, pt_resp_AbsEtaBin1p5to2p1->GetRMS());
    pt_resol_fctAbsEta->SetBinError(4, pt_resp_AbsEtaBin1p5to2p1->GetRMSError());
    
    pt_resol_fctEta->SetBinContent(1, pt_resp_MinusEtaBin1p5to2p1->GetRMS());
    pt_resol_fctEta->SetBinError(1, pt_resp_MinusEtaBin1p5to2p1->GetRMSError());
    pt_resol_fctEta->SetBinContent(2, pt_resp_MinusEtaBin1to1p5->GetRMS());
    pt_resol_fctEta->SetBinError(2, pt_resp_MinusEtaBin1to1p5->GetRMSError());
    pt_resol_fctEta->SetBinContent(3, pt_resp_MinusEtaBin0p5to1->GetRMS());
    pt_resol_fctEta->SetBinError(3, pt_resp_MinusEtaBin0p5to1->GetRMSError());
    pt_resol_fctEta->SetBinContent(4, pt_resp_MinusEtaBin0to0p5->GetRMS());
    pt_resol_fctEta->SetBinError(4, pt_resp_MinusEtaBin0to0p5->GetRMSError());
    pt_resol_fctEta->SetBinContent(5, pt_resp_PlusEtaBin0to0p5->GetRMS());
    pt_resol_fctEta->SetBinError(5, pt_resp_PlusEtaBin0to0p5->GetRMSError());
    pt_resol_fctEta->SetBinContent(6, pt_resp_PlusEtaBin0p5to1->GetRMS());
    pt_resol_fctEta->SetBinError(6, pt_resp_PlusEtaBin0p5to1->GetRMSError());
    pt_resol_fctEta->SetBinContent(7, pt_resp_PlusEtaBin1to1p5->GetRMS());
    pt_resol_fctEta->SetBinError(7, pt_resp_PlusEtaBin1to1p5->GetRMSError());
    pt_resol_fctEta->SetBinContent(8, pt_resp_PlusEtaBin1p5to2p1->GetRMS());
    pt_resol_fctEta->SetBinError(8, pt_resp_PlusEtaBin1p5to2p1->GetRMSError());

    // SET HIST COLORS
    pt_barrel_resp_ptInclusive->SetLineColor(kRed);
    pt_barrel_resp_ptBin20to25->SetLineColor(kRed);
    pt_barrel_resp_ptBin25to30->SetLineColor(kRed);
    pt_barrel_resp_ptBin30to35->SetLineColor(kRed);
    pt_barrel_resp_ptBin35to40->SetLineColor(kRed);
    pt_barrel_resp_ptBin40to45->SetLineColor(kRed);
    pt_barrel_resp_ptBin45to50->SetLineColor(kRed);
    pt_barrel_resp_ptBin50to60->SetLineColor(kRed);
    pt_barrel_resp_ptBin60to70->SetLineColor(kRed);
    pt_barrel_resp_ptBin70to90->SetLineColor(kRed);
    pt_barrel_resp_ptBin90to110->SetLineColor(kRed);


    pt_endcap_resp_ptInclusive->SetLineColor(kBlack);
    pt_endcap_resp_ptBin20to25->SetLineColor(kBlack);
    pt_endcap_resp_ptBin25to30->SetLineColor(kBlack);
    pt_endcap_resp_ptBin30to35->SetLineColor(kBlack);
    pt_endcap_resp_ptBin35to40->SetLineColor(kBlack);
    pt_endcap_resp_ptBin40to45->SetLineColor(kBlack);
    pt_endcap_resp_ptBin45to50->SetLineColor(kBlack);
    pt_endcap_resp_ptBin50to60->SetLineColor(kBlack);
    pt_endcap_resp_ptBin60to70->SetLineColor(kBlack);
    pt_endcap_resp_ptBin70to90->SetLineColor(kBlack);
    pt_endcap_resp_ptBin90to110->SetLineColor(kBlack);

    // SET HIST MARKERS
    pt_barrel_resp_ptInclusive->SetMarkerStyle(8);
    pt_barrel_resp_ptBin20to25->SetMarkerStyle(8);
    pt_barrel_resp_ptBin25to30->SetMarkerStyle(8);
    pt_barrel_resp_ptBin30to35->SetMarkerStyle(8);
    pt_barrel_resp_ptBin35to40->SetMarkerStyle(8);
    pt_barrel_resp_ptBin40to45->SetMarkerStyle(8);
    pt_barrel_resp_ptBin45to50->SetMarkerStyle(8);
    pt_barrel_resp_ptBin50to60->SetMarkerStyle(8);
    pt_barrel_resp_ptBin60to70->SetMarkerStyle(8);
    pt_barrel_resp_ptBin70to90->SetMarkerStyle(8);
    pt_barrel_resp_ptBin90to110->SetMarkerStyle(8);

    pt_endcap_resp_ptInclusive->SetMarkerStyle(8);
    pt_endcap_resp_ptBin20to25->SetMarkerStyle(8);
    pt_endcap_resp_ptBin25to30->SetMarkerStyle(8);
    pt_endcap_resp_ptBin30to35->SetMarkerStyle(8);
    pt_endcap_resp_ptBin35to40->SetMarkerStyle(8);
    pt_endcap_resp_ptBin40to45->SetMarkerStyle(8);
    pt_endcap_resp_ptBin45to50->SetMarkerStyle(8);
    pt_endcap_resp_ptBin50to60->SetMarkerStyle(8);
    pt_endcap_resp_ptBin60to70->SetMarkerStyle(8);
    pt_endcap_resp_ptBin70to90->SetMarkerStyle(8);
    pt_endcap_resp_ptBin90to110->SetMarkerStyle(8);

    pt_resp_AbsEtaBin0to0p5->SetMarkerStyle(8);
    pt_resp_AbsEtaBin0p5to1->SetMarkerStyle(8);
    pt_resp_AbsEtaBin1to1p5->SetMarkerStyle(8);
    pt_resp_AbsEtaBin1p5to2p1->SetMarkerStyle(8);

    pt_resp_PlusEtaBin0to0p5->SetMarkerStyle(8);
    pt_resp_PlusEtaBin0p5to1->SetMarkerStyle(8);
    pt_resp_PlusEtaBin1to1p5->SetMarkerStyle(8);
    pt_resp_PlusEtaBin1p5to2p1->SetMarkerStyle(8);

    pt_resp_MinusEtaBin0to0p5->SetMarkerStyle(8);
    pt_resp_MinusEtaBin0p5to1->SetMarkerStyle(8);
    pt_resp_MinusEtaBin1to1p5->SetMarkerStyle(8);
    pt_resp_MinusEtaBin1p5to2p1->SetMarkerStyle(8);

    eta_response_barrel->SetMarkerStyle(8);
    eta_response_endcap->SetMarkerStyle(8);
    phi_response_barrel->SetMarkerStyle(8);
    phi_response_endcap->SetMarkerStyle(8);

    // SET HIST MARKERS' COLORS
    pt_barrel_resp_ptInclusive->SetMarkerColor(kRed);
    pt_barrel_resp_ptBin20to25->SetMarkerColor(kRed);
    pt_barrel_resp_ptBin25to30->SetMarkerColor(kRed);
    pt_barrel_resp_ptBin30to35->SetMarkerColor(kRed);
    pt_barrel_resp_ptBin35to40->SetMarkerColor(kRed);
    pt_barrel_resp_ptBin40to45->SetMarkerColor(kRed);
    pt_barrel_resp_ptBin45to50->SetMarkerColor(kRed);
    pt_barrel_resp_ptBin50to60->SetMarkerColor(kRed);
    pt_barrel_resp_ptBin60to70->SetMarkerColor(kRed);
    pt_barrel_resp_ptBin70to90->SetMarkerColor(kRed);
    pt_barrel_resp_ptBin90to110->SetMarkerColor(kRed);

    pt_endcap_resp_ptInclusive->SetMarkerColor(kBlack);
    pt_endcap_resp_ptBin20to25->SetMarkerColor(kBlack);
    pt_endcap_resp_ptBin25to30->SetMarkerColor(kBlack);
    pt_endcap_resp_ptBin30to35->SetMarkerColor(kBlack);
    pt_endcap_resp_ptBin35to40->SetMarkerColor(kBlack);
    pt_endcap_resp_ptBin40to45->SetMarkerColor(kBlack);
    pt_endcap_resp_ptBin45to50->SetMarkerColor(kBlack);
    pt_endcap_resp_ptBin50to60->SetMarkerColor(kBlack);
    pt_endcap_resp_ptBin60to70->SetMarkerColor(kBlack);
    pt_endcap_resp_ptBin70to90->SetMarkerColor(kBlack);
    pt_endcap_resp_ptBin90to110->SetMarkerColor(kBlack);

    pt_resp_AbsEtaBin0to0p5->SetMarkerColor(kBlack);
    pt_resp_AbsEtaBin0p5to1->SetMarkerColor(kBlack);
    pt_resp_AbsEtaBin1to1p5->SetMarkerColor(kBlack);
    pt_resp_AbsEtaBin1p5to2p1->SetMarkerColor(kBlack);

    pt_resp_PlusEtaBin0to0p5->SetMarkerColor(kBlack);
    pt_resp_PlusEtaBin0p5to1->SetMarkerColor(kBlack);
    pt_resp_PlusEtaBin1to1p5->SetMarkerColor(kBlack);
    pt_resp_PlusEtaBin1p5to2p1->SetMarkerColor(kBlack);

    pt_resp_MinusEtaBin0to0p5->SetMarkerColor(kBlack);
    pt_resp_MinusEtaBin0p5to1->SetMarkerColor(kBlack);
    pt_resp_MinusEtaBin1to1p5->SetMarkerColor(kBlack);
    pt_resp_MinusEtaBin1p5to2p1->SetMarkerColor(kBlack);

    eta_response_barrel->SetMarkerColor(kBlack);
    eta_response_endcap->SetMarkerColor(kRed);
    phi_response_barrel->SetMarkerColor(kBlack);
    phi_response_endcap->SetMarkerColor(kRed);

    // SET FIT LINE COLORS
    fit_barrel_Inclusive.SetLineColor(kRed);
    fit_barrel_Bin20to25.SetLineColor(kRed);
    fit_barrel_Bin25to30.SetLineColor(kRed);
    fit_barrel_Bin30to35.SetLineColor(kRed);
    fit_barrel_Bin35to40.SetLineColor(kRed);
    fit_barrel_Bin40to45.SetLineColor(kRed);
    fit_barrel_Bin45to50.SetLineColor(kRed);
    fit_barrel_Bin50to60.SetLineColor(kRed);
    fit_barrel_Bin60to70.SetLineColor(kRed);
    fit_barrel_Bin70to90.SetLineColor(kRed);
    fit_barrel_Bin90to110.SetLineColor(kRed);

    fit_endcap_Inclusive.SetLineColor(kBlack);
    fit_endcap_Bin20to25.SetLineColor(kBlack);
    fit_endcap_Bin25to30.SetLineColor(kBlack);
    fit_endcap_Bin30to35.SetLineColor(kBlack);
    fit_endcap_Bin35to40.SetLineColor(kBlack);
    fit_endcap_Bin40to45.SetLineColor(kBlack);
    fit_endcap_Bin45to50.SetLineColor(kBlack);
    fit_endcap_Bin50to60.SetLineColor(kBlack);
    fit_endcap_Bin60to70.SetLineColor(kBlack);
    fit_endcap_Bin70to90.SetLineColor(kBlack);
    fit_endcap_Bin90to110.SetLineColor(kBlack);
    
    fit_AbsEtaBin0to0p5.SetLineColor(kRed);
    fit_AbsEtaBin0p5to1.SetLineColor(kRed);
    fit_AbsEtaBin1to1p5.SetLineColor(kRed);
    fit_AbsEtaBin1p5to2p1.SetLineColor(kRed);

    fit_PlusEtaBin0to0p5.SetLineColor(kRed);
    fit_PlusEtaBin0p5to1.SetLineColor(kRed);
    fit_PlusEtaBin1to1p5.SetLineColor(kRed);
    fit_PlusEtaBin1p5to2p1.SetLineColor(kRed);

    fit_MinusEtaBin0to0p5.SetLineColor(kRed);
    fit_MinusEtaBin0p5to1.SetLineColor(kRed);
    fit_MinusEtaBin1to1p5.SetLineColor(kRed);
    fit_MinusEtaBin1p5to2p1.SetLineColor(kRed);

    //REMOVE TITLES
    pt_barrel_resp_ptInclusive->SetTitle("");
    pt_barrel_resp_ptBin20to25->SetTitle("");
    pt_barrel_resp_ptBin25to30->SetTitle("");
    pt_barrel_resp_ptBin30to35->SetTitle("");
    pt_barrel_resp_ptBin35to40->SetTitle("");
    pt_barrel_resp_ptBin40to45->SetTitle("");
    pt_barrel_resp_ptBin45to50->SetTitle("");
    pt_barrel_resp_ptBin50to60->SetTitle("");
    pt_barrel_resp_ptBin60to70->SetTitle("");
    pt_barrel_resp_ptBin70to90->SetTitle("");
    pt_barrel_resp_ptBin90to110->SetTitle("");

    pt_endcap_resp_ptInclusive->SetTitle("");
    pt_endcap_resp_ptBin20to25->SetTitle("");
    pt_endcap_resp_ptBin25to30->SetTitle("");
    pt_endcap_resp_ptBin30to35->SetTitle("");
    pt_endcap_resp_ptBin35to40->SetTitle("");
    pt_endcap_resp_ptBin40to45->SetTitle("");
    pt_endcap_resp_ptBin45to50->SetTitle("");
    pt_endcap_resp_ptBin50to60->SetTitle("");
    pt_endcap_resp_ptBin60to70->SetTitle("");
    pt_endcap_resp_ptBin70to90->SetTitle("");
    pt_endcap_resp_ptBin90to110->SetTitle("");

    pt_resp_AbsEtaBin0to0p5->SetTitle("");
    pt_resp_AbsEtaBin0p5to1->SetTitle("");
    pt_resp_AbsEtaBin1to1p5->SetTitle("");
    pt_resp_AbsEtaBin1p5to2p1->SetTitle("");

    pt_resp_PlusEtaBin0to0p5->SetTitle("");
    pt_resp_PlusEtaBin0p5to1->SetTitle("");
    pt_resp_PlusEtaBin1to1p5->SetTitle("");
    pt_resp_PlusEtaBin1p5to2p1->SetTitle("");

    pt_resp_MinusEtaBin0to0p5->SetTitle("");
    pt_resp_MinusEtaBin0p5to1->SetTitle("");
    pt_resp_MinusEtaBin1to1p5->SetTitle("");
    pt_resp_MinusEtaBin1p5to2p1->SetTitle("");

    PTvsETA_response->SetTitle("");


    // REMOVE STATS PANEL
    gStyle->SetOptStat(000000);

    TPaveText* texl = new TPaveText(0.2,0.87,0.3,0.99,"NDC");
    texl->AddText("CMS Internal");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    
    TLegend* leg = new TLegend(0.45,0.7,0.89,0.88);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);

    //##########
    // response in pt bins

    TCanvas c("c","c",2400,1000);
    c.SetLeftMargin(0.15);
    c.SetGrid();

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.5, 0.33, 1.0);
    pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad

    leg->AddEntry((TObject*)0, "pT inclusive", "");
    leg->AddEntry(pt_barrel_resp_ptInclusive,"Barrel |#eta|<1.5","L");
    leg->AddEntry(pt_endcap_resp_ptInclusive,"Endcap 1.5<|#eta|<2.1","L");

    pt_barrel_resp_ptInclusive->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_barrel_resp_ptInclusive->GetYaxis()->SetTitle("a.u");

    pt_barrel_resp_ptInclusive->Draw("E0");
    pt_endcap_resp_ptInclusive->Draw("E0 same");
    //fit_barrel_Inclusive.Draw("same");
    //fit_endcap_Inclusive.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad1->Update();
    c.Update();

    c.cd();
    TPad *pad2 = new TPad("pad2", "pad2", 0.33, 0.5, 0.66, 1.0);
    pad2->SetLeftMargin(0.15);
    pad2->SetGridx();         // Vertical grid
    pad2->SetGridy();         // Vertical grid
    pad2->Draw();             // Draw the upper pad: pad2
    pad2->cd();               // pad2 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "20 < p_{T} < 25", "");
    leg->AddEntry(pt_barrel_resp_ptBin20to25,"Barrel |#eta|<1.5","L");
    leg->AddEntry(pt_endcap_resp_ptBin20to25,"Endcap 1.5<|#eta|<2.1","L");

    pt_barrel_resp_ptBin20to25->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_barrel_resp_ptBin20to25->GetYaxis()->SetTitle("a.u");

    pt_barrel_resp_ptBin20to25->Draw("E0");
    pt_endcap_resp_ptBin20to25->Draw("E0 same");
    //fit_barrel_Bin20to25.Draw("same");
    //fit_endcap_Bin20to25.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad2->Update();
    c.Update();

    c.cd();
    TPad *pad3 = new TPad("pad3", "pad3", 0.66, 0.5, 1., 1.);
    pad3->SetLeftMargin(0.15);
    pad3->SetGridx();         // Vertical grid
    pad3->SetGridy();         // Vertical grid
    pad3->Draw();             // Draw the upper pad: pad3
    pad3->cd();               // pad3 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "25 < p_{T} < 30", "");
    leg->AddEntry(pt_barrel_resp_ptBin25to30,"Barrel |#eta|<1.5","L");
    leg->AddEntry(pt_endcap_resp_ptBin25to30,"Endcap 1.5<|#eta|<2.1","L");

    pt_barrel_resp_ptBin25to30->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_barrel_resp_ptBin25to30->GetYaxis()->SetTitle("a.u");

    pt_barrel_resp_ptBin25to30->Draw("E0");
    pt_endcap_resp_ptBin25to30->Draw("E0 same");
    //fit_barrel_Bin25to30.Draw("same");
    //fit_endcap_Bin25to30.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad3->Update();
    c.Update();

    c.cd();
    TPad *pad4 = new TPad("pad4", "pad4", 0., 0., 0.33, 0.5);
    pad4->SetLeftMargin(0.15);
    pad4->SetGridx();         // Vertical grid
    pad4->SetGridy();         // Vertical grid
    pad4->Draw();             // Draw the upper pad: pad4
    pad4->cd();               // pad4 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "30 < p_{T} < 35", "");
    leg->AddEntry(pt_barrel_resp_ptBin30to35,"Barrel |#eta|<1.5","L");
    leg->AddEntry(pt_endcap_resp_ptBin30to35,"Endcap 1.5<|#eta|<2.1","L");

    pt_barrel_resp_ptBin30to35->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_barrel_resp_ptBin30to35->GetYaxis()->SetTitle("a.u");

    pt_barrel_resp_ptBin30to35->Draw("E0");
    pt_endcap_resp_ptBin30to35->Draw("E0 same");
    //fit_barrel_Bin30to35.Draw("same");
    //fit_endcap_Bin30to35.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad4->Update();
    c.Update();

    c.cd();
    TPad *pad5 = new TPad("pad5", "pad5", 0.33, 0., 0.66, 0.5);
    pad5->SetLeftMargin(0.15);
    pad5->SetGridx();         // Vertical grid
    pad5->SetGridy();         // Vertical grid
    pad5->Draw();             // Draw the upper pad: pad5
    pad5->cd();               // pad5 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "35 < p_{T} < 40", "");
    leg->AddEntry(pt_barrel_resp_ptBin35to40,"Barrel |#eta|<1.5","L");
    leg->AddEntry(pt_endcap_resp_ptBin35to40,"Endcap 1.5<|#eta|<2.1","L");

    pt_barrel_resp_ptBin35to40->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_barrel_resp_ptBin35to40->GetYaxis()->SetTitle("a.u");

    pt_barrel_resp_ptBin35to40->Draw("E0");
    pt_endcap_resp_ptBin35to40->Draw("E0 same");
    //fit_barrel_Bin35to40.Draw("same");
    //fit_endcap_Bin35to40.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad5->Update();
    c.Update();

    c.cd();
    TPad *pad6 = new TPad("pad6", "pad6", 0.66, 0., 1., 0.5);
    pad6->SetLeftMargin(0.15);
    pad6->SetGridx();         // Vertical grid
    pad6->SetGridy();         // Vertical grid
    pad6->Draw();             // Draw the upper pad: pad6
    pad6->cd();               // pad6 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "40 < p_{T} < 45", "");
    leg->AddEntry(pt_barrel_resp_ptBin40to45,"Barrel |#eta|<1.5","L");
    leg->AddEntry(pt_endcap_resp_ptBin40to45,"Endcap 1.5<|#eta|<2.1","L");

    pt_barrel_resp_ptBin40to45->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_barrel_resp_ptBin40to45->GetYaxis()->SetTitle("a.u");

    pt_barrel_resp_ptBin40to45->Draw("E0");
    pt_endcap_resp_ptBin40to45->Draw("E0 same");
    //fit_barrel_Bin40to45.Draw("same");
    //fit_endcap_Bin40to45.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad6->Update();
    c.Update();

    c.SaveAs("plots/optimizationV"+version+"_ReOptReEmu/split/response_ptBins_optimizationV"+version+"_ReOptReEmu"+fixedRate+"kHz.png");

    //##########
    // plot response in absolute eta bins

    TCanvas c2("c2","c2",1200,800);
    c2.SetLeftMargin(0.15);
    c2.SetGrid();
    c2.cd();

    TPad *pad7 = new TPad("pad7", "pad7", 0., 0.5, 0.5, 1.);
    pad7->SetLeftMargin(0.15);
    pad7->SetGridx();         // Vertical grid
    pad7->SetGridy();         // Vertical grid
    pad7->Draw();             // Draw the upper pad: pad1
    pad7->cd();               // pad1 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "0.0 < #eta < 0.5", "");
    leg->AddEntry(pt_resp_AbsEtaBin0to0p5,"Response","L");
    //leg->AddEntry(&fit_AbsEtaBin0to0p5,fitOption+" fit","L");

    pt_resp_AbsEtaBin0to0p5->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_AbsEtaBin0to0p5->GetYaxis()->SetTitle("a.u");

    pt_resp_AbsEtaBin0to0p5->Draw("E0");
    //fit_AbsEtaBin0to0p5.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad7->Update();
    c2.Update();

    c2.cd();
    TPad *pad8 = new TPad("pad8", "pad8", 0.5, 0.5, 1., 1.);
    pad8->SetLeftMargin(0.15);
    pad8->SetGridx();         // Vertical grid
    pad8->SetGridy();         // Vertical grid
    pad8->Draw();             // Draw the upper pad: pad2
    pad8->cd();               // pad2 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "0.5 < #eta < 1.0", "");
    leg->AddEntry(pt_resp_AbsEtaBin0p5to1,"Response","L");
    //leg->AddEntry(&fit_AbsEtaBin0p5to1,fitOption+" fit","L");

    pt_resp_AbsEtaBin0p5to1->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_AbsEtaBin0p5to1->GetYaxis()->SetTitle("a.u");

    pt_resp_AbsEtaBin0p5to1->Draw("E0");
    //fit_AbsEtaBin0p5to1.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad8->Update();
    c2.Update();

    c2.cd();
    TPad *pad9 = new TPad("pad9", "pad9", 0., 0., 0.5, 0.5);
    pad9->SetLeftMargin(0.15);
    pad9->SetGridx();         // Vertical grid
    pad9->SetGridy();         // Vertical grid
    pad9->Draw();             // Draw the upper pad: pad3
    pad9->cd();               // pad3 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "1.0 < #eta < 1.5", "");
    leg->AddEntry(pt_resp_AbsEtaBin1to1p5,"Response","L");
    //leg->AddEntry(&fit_AbsEtaBin1to1p5,fitOption+" fit","L");

    pt_resp_AbsEtaBin1to1p5->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_AbsEtaBin1to1p5->GetYaxis()->SetTitle("a.u");

    pt_resp_AbsEtaBin1to1p5->Draw("E0");
    //fit_AbsEtaBin1to1p5.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad9->Update();
    c2.Update();

    c2.cd();
    TPad *pad10 = new TPad("pad10", "pad10", 0.5, 0., 1., 0.5);
    pad10->SetLeftMargin(0.15);
    pad10->SetGridx();         // Vertical grid
    pad10->SetGridy();         // Vertical grid
    pad10->Draw();             // Draw the upper pad: pad4
    pad10->cd();               // pad4 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "1.5 < #eta < 2.1", "");
    leg->AddEntry(pt_resp_AbsEtaBin1p5to2p1,"Response","L");
    //leg->AddEntry(&fit_AbsEtaBin1p5to2p1,fitOption+" fit","L");

    pt_resp_AbsEtaBin1p5to2p1->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_AbsEtaBin1p5to2p1->GetYaxis()->SetTitle("a.u");

    pt_resp_AbsEtaBin1p5to2p1->Draw("E0");
    //fit_AbsEtaBin1p5to2p1.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad10->Update();
    c2.Update();

    c2.SaveAs("plots/optimizationV"+version+"_ReOptReEmu/split/response_absEtaBins_optimizationV"+version+"_ReOptReEmu"+fixedRate+"kHz.png");

    //##########
    // plot response in plus eta bins

    TCanvas c4("c4","c4",1200,800);
    c4.SetLeftMargin(0.15);
    c4.SetGrid();
    c4.cd();

    TPad *pad14 = new TPad("pad14", "pad14", 0., 0.5, 0.5, 1.);
    pad14->SetLeftMargin(0.15);
    pad14->SetGridx();         // Vertical grid
    pad14->SetGridy();         // Vertical grid
    pad14->Draw();             // Draw the upper pad: pad1
    pad14->cd();               // pad1 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "0.0 < #eta < 0.5", "");
    leg->AddEntry(pt_resp_PlusEtaBin0to0p5,"Response","L");
    //leg->AddEntry(&fit_PlusEtaBin0to0p5,fitOption+" fit","L");

    pt_resp_PlusEtaBin0to0p5->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_PlusEtaBin0to0p5->GetYaxis()->SetTitle("a.u");

    pt_resp_PlusEtaBin0to0p5->Draw("E0");
    //fit_PlusEtaBin0to0p5.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad14->Update();
    c4.Update();

    c4.cd();
    TPad *pad15 = new TPad("pad15", "pad15", 0.5, 0.5, 1., 1.);
    pad15->SetLeftMargin(0.15);
    pad15->SetGridx();         // Vertical grid
    pad15->SetGridy();         // Vertical grid
    pad15->Draw();             // Draw the upper pad: pad2
    pad15->cd();               // pad2 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "0.5 < #eta < 1.0", "");
    leg->AddEntry(pt_resp_PlusEtaBin0p5to1,"Response","L");
    //leg->AddEntry(&fit_PlusEtaBin0p5to1,fitOption+" fit","L");

    pt_resp_PlusEtaBin0p5to1->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_PlusEtaBin0p5to1->GetYaxis()->SetTitle("a.u");

    pt_resp_PlusEtaBin0p5to1->Draw("E0");
    //fit_PlusEtaBin0p5to1.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad15->Update();
    c4.Update();

    c4.cd();
    TPad *pad16 = new TPad("pad16", "pad16", 0., 0., 0.5, 0.5);
    pad16->SetLeftMargin(0.15);
    pad16->SetGridx();         // Vertical grid
    pad16->SetGridy();         // Vertical grid
    pad16->Draw();             // Draw the upper pad: pad3
    pad16->cd();               // pad3 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "1.0 < #eta < 1.5", "");
    leg->AddEntry(pt_resp_PlusEtaBin1to1p5,"Response","L");
    //leg->AddEntry(&fit_PlusEtaBin1to1p5,fitOption+" fit","L");

    pt_resp_PlusEtaBin1to1p5->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_PlusEtaBin1to1p5->GetYaxis()->SetTitle("a.u");

    pt_resp_PlusEtaBin1to1p5->Draw("E0");
    //fit_PlusEtaBin1to1p5.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad16->Update();
    c4.Update();

    c4.cd();
    TPad *pad17 = new TPad("pad17", "pad17", 0.5, 0., 1., 0.5);
    pad17->SetLeftMargin(0.15);
    pad17->SetGridx();         // Vertical grid
    pad17->SetGridy();         // Vertical grid
    pad17->Draw();             // Draw the upper pad: pad4
    pad17->cd();               // pad4 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "1.5 < #eta < 2.1", "");
    leg->AddEntry(pt_resp_PlusEtaBin1p5to2p1,"Response","L");
    //leg->AddEntry(&fit_PlusEtaBin1p5to2p1,fitOption+" fit","L");

    pt_resp_PlusEtaBin1p5to2p1->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_PlusEtaBin1p5to2p1->GetYaxis()->SetTitle("a.u");

    pt_resp_PlusEtaBin1p5to2p1->Draw("E0");
    //fit_PlusEtaBin1p5to2p1.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad17->Update();
    c4.Update();

    c4.SaveAs("plots/optimizationV"+version+"_ReOptReEmu/split/response_plusEtaBins_optimizationV"+version+"_ReOptReEmu"+fixedRate+"kHz.png");

    //##########
    // plot response in minus eta bins

    TCanvas c5("c5","c5",1200,800);
    c5.SetLeftMargin(0.15);
    c5.SetGrid();
    c5.cd();

    TPad *pad18 = new TPad("pad18", "pad18", 0., 0.5, 0.5, 1.);
    pad18->SetLeftMargin(0.15);
    pad18->SetGridx();         // Vertical grid
    pad18->SetGridy();         // Vertical grid
    pad18->Draw();             // Draw the upper pad: pad1
    pad18->cd();               // pad1 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "0.0 < #eta < 0.5", "");
    leg->AddEntry(pt_resp_MinusEtaBin0to0p5,"Response","L");
    //leg->AddEntry(&fit_MinusEtaBin0to0p5,fitOption+" fit","L");

    pt_resp_MinusEtaBin0to0p5->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_MinusEtaBin0to0p5->GetYaxis()->SetTitle("a.u");

    pt_resp_MinusEtaBin0to0p5->Draw("E0");
    //fit_MinusEtaBin0to0p5.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad18->Update();
    c5.Update();

    c5.cd();
    TPad *pad19 = new TPad("pad19", "pad19", 0.5, 0.5, 1., 1.);
    pad19->SetLeftMargin(0.15);
    pad19->SetGridx();         // Vertical grid
    pad19->SetGridy();         // Vertical grid
    pad19->Draw();             // Draw the upper pad: pad2
    pad19->cd();               // pad2 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "0.5 < #eta < 1.0", "");
    leg->AddEntry(pt_resp_MinusEtaBin0p5to1,"Response","L");
    //leg->AddEntry(&fit_MinusEtaBin0p5to1,fitOption+" fit","L");

    pt_resp_MinusEtaBin0p5to1->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_MinusEtaBin0p5to1->GetYaxis()->SetTitle("a.u");

    pt_resp_MinusEtaBin0p5to1->Draw("E0");
    //fit_MinusEtaBin0p5to1.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad19->Update();
    c5.Update();

    c5.cd();
    TPad *pad20 = new TPad("pad20", "pad20", 0., 0., 0.5, 0.5);
    pad20->SetLeftMargin(0.15);
    pad20->SetGridx();         // Vertical grid
    pad20->SetGridy();         // Vertical grid
    pad20->Draw();             // Draw the upper pad: pad3
    pad20->cd();               // pad3 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "1.0 < #eta < 1.5", "");
    leg->AddEntry(pt_resp_MinusEtaBin1to1p5,"Response","L");
    //leg->AddEntry(&fit_MinusEtaBin1to1p5,fitOption+" fit","L");

    pt_resp_MinusEtaBin1to1p5->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_MinusEtaBin1to1p5->GetYaxis()->SetTitle("a.u");

    pt_resp_MinusEtaBin1to1p5->Draw("E0");
    //fit_MinusEtaBin1to1p5.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad20->Update();
    c5.Update();

    c5.cd();
    TPad *pad21 = new TPad("pad21", "pad21", 0.5, 0., 1., 0.5);
    pad21->SetLeftMargin(0.15);
    pad21->SetGridx();         // Vertical grid
    pad21->SetGridy();         // Vertical grid
    pad21->Draw();             // Draw the upper pad: pad4
    pad21->cd();               // pad4 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "1.5 < #eta < 2.1", "");
    leg->AddEntry(pt_resp_MinusEtaBin1p5to2p1,"Response","L");
    //leg->AddEntry(&fit_MinusEtaBin1p5to2p1,fitOption+" fit","L");

    pt_resp_MinusEtaBin1p5to2p1->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_MinusEtaBin1p5to2p1->GetYaxis()->SetTitle("a.u");

    pt_resp_MinusEtaBin1p5to2p1->Draw("E0");
    //fit_MinusEtaBin1p5to2p1.Draw("same");
    texl->Draw("same");
    //leg->Draw("same");
    
    pad21->Update();
    c5.Update();

    c5.SaveAs("plots/optimizationV"+version+"_ReOptReEmu/split/response_minusEtaBins_optimizationV"+version+"_ReOptReEmu"+fixedRate+"kHz.png");

    //##########
    // plot resolution in pt and eta bins

    TCanvas c3("c3","c3",650,600);
    c3.SetLeftMargin(0.15);
    c3.SetGrid();
    c3.cd();

    TPad *pad11 = new TPad("pad11", "pad11", 0., 0., 1., 1.);
    pad11->SetLeftMargin(0.15);
    pad11->SetGridx();         // Vertical grid
    pad11->SetGridy();         // Vertical grid
    pad11->Draw();             // Draw the upper pad: pad1
    pad11->cd();               // pad1 becomes the current pad

    pt_resol_endcap_fctPt->SetLineColor(kBlack);
    pt_resol_endcap_fctPt->SetMarkerStyle(8);
    pt_resol_endcap_fctPt->SetMarkerColor(kBlack);
    pt_resol_endcap_fctPt->SetTitle("");
    pt_resol_endcap_fctPt->GetXaxis()->SetTitle("Offline p_{T}(#tau) [GeV]");
    pt_resol_endcap_fctPt->GetYaxis()->SetTitle("p^{L1}_{T}(#tau) resolution");
    //pt_resol_endcap_fctPt->GetYaxis()->SetRangeUser(0.,0.3);
    pt_resol_endcap_fctPt->Draw("E0");
    pt_resol_barrel_fctPt->SetLineColor(kRed);
    pt_resol_barrel_fctPt->SetMarkerStyle(8);
    pt_resol_barrel_fctPt->SetMarkerColor(kRed);
    pt_resol_barrel_fctPt->SetTitle("");
    pt_resol_barrel_fctPt->Draw("E0 same");
    leg->Clear();
    leg->AddEntry(pt_resol_barrel_fctPt,"Barrel |#eta|<1.5","L");
    leg->AddEntry(pt_resol_endcap_fctPt,"Endcap 1.5<|#eta|<2.1","L");
    leg->Draw("same");
    texl->Draw("same");

    c3.SaveAs("plots/optimizationV"+version+"_ReOptReEmu/pt_resolution_optimizationV"+version+"_ReOptReEmu"+fixedRate+"kHz_fctPt.png");

    TCanvas c6("c6","c6",2000,800);
    c6.SetLeftMargin(0.15);
    c6.SetGrid();
    c6.cd();

    TPad *pad12 = new TPad("pad12", "pad12", 0., 0., 0.5, 1.);
    pad12->SetLeftMargin(0.15);
    pad12->SetGridx();         // Vertical grid
    pad12->SetGridy();         // Vertical grid
    pad12->Draw();             // Draw the upper pad: pad2
    pad12->cd();               // pad2 becomes the current pad

    pt_resol_fctAbsEta->SetLineColor(kRed);
    pt_resol_fctAbsEta->SetMarkerStyle(8);
    pt_resol_fctAbsEta->SetMarkerColor(kRed);
    pt_resol_fctAbsEta->SetTitle("");
    pt_resol_fctAbsEta->GetXaxis()->SetTitle("Offline |#eta(#tau)|");
    pt_resol_fctAbsEta->GetYaxis()->SetTitle("p^{L1}_{T}(#tau) response width");
    pt_resol_fctAbsEta->Draw("E0");
    texl->Draw("same");

    c6.cd();
    TPad *pad13 = new TPad("pad13", "pad13", 0.5, 0., 1., 1.);
    pad13->SetLeftMargin(0.15);
    pad13->SetGridx();         // Vertical grid
    pad13->SetGridy();         // Vertical grid
    pad13->Draw();             // Draw the upper pad: pad2
    pad13->cd();               // pad2 becomes the current pad

    pt_resol_fctEta->SetLineColor(kRed);
    pt_resol_fctEta->SetMarkerStyle(8);
    pt_resol_fctEta->SetMarkerColor(kRed);
    pt_resol_fctEta->SetTitle("");
    pt_resol_fctEta->GetXaxis()->SetTitle("Offline #eta(#tau)");
    pt_resol_fctEta->GetYaxis()->SetTitle("p^{L1}_{T}(#tau) response width");
    pt_resol_fctEta->Draw("E0");
    texl->Draw("same");

    c6.SaveAs("plots/optimizationV"+version+"_ReOptReEmu/pt_resolution_optimizationV"+version+"_ReOptReEmu"+fixedRate+"kHz_fctEta.png");

    //##########
    // plot resolution in pt vs. eta

    TCanvas c8("c8","c8",650,600);
    //c8.SetLeftMargin(0.15);
    c8.SetGrid();
    c8.cd();

    TPad *pad23 = new TPad("pad23", "pad23", 0., 0., 1., 1.);
    pad23->SetLeftMargin(0.15);
    pad23->SetGridx();         // Vertical grid
    pad23->SetGridy();         // Vertical grid
    pad23->Draw();             // Draw the upper pad: pad1
    pad23->cd();               // pad1 becomes the current pad

    
    PTvsETA_response->SetTitle("");
    PTvsETA_response->GetXaxis()->SetTitle("Offline p_{T}(#tau) [GeV]");
    PTvsETA_response->GetYaxis()->SetTitle("Offline #eta(#tau) [GeV]");
    PTvsETA_response->GetZaxis()->SetTitle("Average resolution");
    PTvsETA_response->Draw("colz");
    texl->Draw("same");

    c8.SaveAs("plots/optimizationV"+version+"_ReOptReEmu/ptVSeta_resolution_optimizationV"+version+"_ReOptReEmu"+fixedRate+"kHz.png");
    
    TString mean = "Mean=";
    TPaveText* texl1 = new TPaveText(0.45,0.5,0.89,0.7,"NDC");
    texl1->AddText(mean+to_string(pt_response_uncalib_ptInclusive_mc->GetMean())+" ; RMS="+to_string(pt_response_uncalib_ptInclusive_mc->GetRMS()));
    texl1->AddText(mean+to_string(pt_response_calib_ptInclusive_mc->GetMean())+" ; RMS="+to_string(pt_response_calib_ptInclusive_mc->GetRMS()));
    texl1->AddText(mean+to_string(pt_response_ptInclusive_mc->GetMean())+" ; RMS="+to_string(pt_response_ptInclusive_mc->GetRMS()));
    texl1->SetTextSize(0.04);
    texl1->SetFillStyle(0);
    texl1->SetBorderSize(0);
    texl1->SetTextColor(kBlue);
    ((TText*)texl1->GetListOfLines()->First())->SetTextColor(kRed);
    ((TText*)texl1->GetListOfLines()->Last())->SetTextColor(kGreen+2);

    TCanvas c9("c9","c9",800,600);
    c9.SetLeftMargin(0.15);
    c9.SetGrid();

    TPad *pad24 = new TPad("pad24", "pad24", 0, 0., 1., 1.);
    pad24->SetLeftMargin(0.15);
    pad24->SetGridx();         // Vertical grid
    pad24->SetGridy();         // Vertical grid
    pad24->Draw();             // Draw the upper pad: pad1
    pad24->cd();               // pad1 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "pT inclusive", "");
    leg->AddEntry(pt_response_uncalib_ptInclusive_mc,"Run3 MC uncalibrated","L");
    leg->AddEntry(pt_response_calib_ptInclusive_mc,"Run3 MC calibrated","L");
    leg->AddEntry(pt_response_ptInclusive_mc,"Run3 MC re-emulated","L");

    pt_response_uncalib_ptInclusive_mc->SetTitle("");
    pt_response_uncalib_ptInclusive_mc->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_response_uncalib_ptInclusive_mc->GetYaxis()->SetTitle("a.u");

    pt_response_uncalib_ptInclusive_mc->SetLineColor(kRed);
    pt_response_calib_ptInclusive_mc->SetLineColor(kBlue);
    pt_response_ptInclusive_mc->SetLineColor(kGreen+2);

    pt_response_uncalib_ptInclusive_mc->SetMarkerStyle(8);
    pt_response_calib_ptInclusive_mc->SetMarkerStyle(8);
    pt_response_ptInclusive_mc->SetMarkerStyle(8);

    pt_response_uncalib_ptInclusive_mc->SetMarkerColor(kRed);
    pt_response_calib_ptInclusive_mc->SetMarkerColor(kBlue);
    pt_response_ptInclusive_mc->SetMarkerColor(kGreen+2);
    
    pt_response_uncalib_ptInclusive_mc->Draw("E0");
    pt_response_calib_ptInclusive_mc->Draw("E0 same");
    pt_response_ptInclusive_mc->Draw("E0 same");
    texl->Draw("same");
    texl1->Draw("same");
    leg->Draw("same");
    
    pad24->Update();
    c9.SaveAs("plots/optimizationV"+version+"_ReOptReEmu/ptInclusive_resolution_optimizationV"+version+"_ReOptReEmu.png");


    TCanvas c("c","c",650,600);
    c.SetLeftMargin(0.15);
    c.SetGrid();

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0., 1., 1.);
    //pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "p_{T}^{offline}(#tau)>30GeV", "");
    leg->AddEntry(eta_response_barrel,"Barrel |#eta|<1.305","L");
    leg->AddEntry(eta_response_endcap,"Endcap 1.479<|#eta|<2.1","L");

    eta_response_barrel->SetTitle("");
    eta_response_barrel->GetXaxis()->SetTitle("#eta^{L1}(#tau) - #eta^{offline}(#tau)");
    eta_response_barrel->GetYaxis()->SetTitle("a.u");
    eta_response_barrel->GetYaxis()->SetRangeUser(0,0.2);

    eta_response_barrel->SetLineColor(kBlack);
    eta_response_endcap->SetLineColor(kRed);

    eta_response_barrel->Draw("L E0");
    eta_response_endcap->Draw("L E0 same");
    //fit_barrel_Inclusive.Draw("same");
    //fit_endcap_Inclusive.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad1->Update();
    c.SaveAs("plots/optimizationV"+version+"_ReOptReEmu/eta_resolution_ptInclusive_optimizationV"+version+"_ReOptReEmu.png");


    TCanvas cx("cx","cx",650,600);
    cx.SetLeftMargin(0.15);
    cx.SetGrid();

    TPad *pad1x = new TPad("pad1x", "pad1x", 0, 0., 1., 1.);
    //pad1->SetLeftMargin(0.15);
    pad1x->SetGridx();         // Vertical grid
    pad1x->SetGridy();         // Vertical grid
    pad1x->Draw();             // Draw the upper pad: pad1
    pad1x->cd();               // pad1 becomes the current pad

    leg->Clear();
    leg->AddEntry((TObject*)0, "p_{T}^{offline}(#tau)>30GeV", "");
    leg->AddEntry(phi_response_barrel,"Barrel |#eta|<1.305","L");
    leg->AddEntry(phi_response_endcap,"Endcap 1.479<|#eta|<2.1","L");

    phi_response_barrel->SetTitle("");
    phi_response_barrel->GetXaxis()->SetTitle("#phi^{L1}(#tau) - #phi^{offline}(#tau)");
    phi_response_barrel->GetYaxis()->SetTitle("a.u");
    phi_response_barrel->GetYaxis()->SetRangeUser(0,0.2);

    phi_response_barrel->SetLineColor(kBlack);
    phi_response_endcap->SetLineColor(kRed);

    phi_response_barrel->Draw("L E0");
    phi_response_endcap->Draw("L E0 same");
    //fit_barrel_Inclusive.Draw("same");
    //fit_endcap_Inclusive.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad1x->Update();
    cx.SaveAs("plots/optimizationV"+version+"_ReOptReEmu/phi_resolution_ptInclusive_optimizationV"+version+"_ReOptReEmu.png");
}
