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


void plotResolutions(int run, int vers = 3, float calibThr = 1.7, TString fitOption="crystalball", Bool_t nTTRange = kFALSE)
{
    TString version = to_string(vers);
    TString run_str = to_string(run);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

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


    TString InputFileName = "/data_CMS/cms/motta/Run3preparation/2022_01_06_optimizationV"+version+"_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_CALIBRATED_2022_01_06.root";
    TFile f(InputFileName.Data(),"READ");
    TTree* inTree = (TTree*)f.Get("outTreeCalibrated");

    Int_t           in_L1Tau_IEta;
    Int_t           in_compressedsortedshape;
    Int_t           in_L1Tau_hasEM;
    Float_t         in_Target;
    Int_t           in_L1Tau_IEt;
    Int_t           in_L1Tau_RawIEt;
    Int_t           in_EventNumber;
    Int_t           in_RunNumber;
    Int_t           in_Nvtx;
    Int_t           in_L1Tau_nTT;
    Float_t         in_L1Tau_pt;
    Float_t         in_L1Tau_CalibPt;
    Float_t         in_OfflineTau_pt;
    Float_t         in_OfflineTau_eta;
    Int_t           in_compressedieta;
    Int_t           in_compressedE;
    Int_t           in_L1Tau_Iso;
    Int_t           in_compressednTT;
    Int_t           in_OfflineTau_isMatched;
    Int_t           in_L1Tau_isMerged;
    Int_t           in_compressedshape;
    Int_t           in_supercompressedE;
    Int_t           in_supercompressednTT;
    Int_t           in_L1Tau_Qual;
    Bool_t          in_L1Tau_PassShapeVeto;
    Bool_t          L1Tau_PassShapeVeto_0p6_10_50;
    Bool_t          L1Tau_PassShapeVeto_0p5_20_45;
    Bool_t          L1Tau_PassShapeVeto_0p2_25_60;
    Bool_t          L1Tau_PassShapeVeto_0p2_20_100;

    inTree->SetBranchAddress("L1Tau_IEta", &in_L1Tau_IEta);
    inTree->SetBranchAddress("compressedsortedshape", &in_compressedsortedshape);
    inTree->SetBranchAddress("L1Tau_hasEM", &in_L1Tau_hasEM);
    inTree->SetBranchAddress("Target", &in_Target);
    inTree->SetBranchAddress("L1Tau_IEt", &in_L1Tau_IEt);
    inTree->SetBranchAddress("L1Tau_RawIEt", &in_L1Tau_RawIEt);
    inTree->SetBranchAddress("EventNumber", &in_EventNumber);
    inTree->SetBranchAddress("RunNumber", &in_RunNumber);
    inTree->SetBranchAddress("Nvtx", &in_Nvtx);
    inTree->SetBranchAddress("L1Tau_nTT", &in_L1Tau_nTT);
    inTree->SetBranchAddress("L1Tau_pt", &in_L1Tau_pt);
    inTree->SetBranchAddress("L1Tau_CalibPt", &in_L1Tau_CalibPt);
    inTree->SetBranchAddress("OfflineTau_pt", &in_OfflineTau_pt);
    inTree->SetBranchAddress("OfflineTau_eta", &in_OfflineTau_eta);
    inTree->SetBranchAddress("compressedieta", &in_compressedieta);
    inTree->SetBranchAddress("compressedE", &in_compressedE);
    inTree->SetBranchAddress("L1Tau_Iso", &in_L1Tau_Iso);
    inTree->SetBranchAddress("compressednTT", &in_compressednTT);
    inTree->SetBranchAddress("OfflineTau_isMatched", &in_OfflineTau_isMatched);
    inTree->SetBranchAddress("L1Tau_isMerged", &in_L1Tau_isMerged);
    inTree->SetBranchAddress("compressedshape", &in_compressedshape);
    inTree->SetBranchAddress("supercompressedE", &in_supercompressedE);
    inTree->SetBranchAddress("supercompressednTT", &in_supercompressednTT);
    inTree->SetBranchAddress("L1Tau_PassShapeVeto", &in_L1Tau_PassShapeVeto);
    inTree->SetBranchAddress("L1Tau_Qual",&in_L1Tau_Qual);

    for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    {
        inTree->GetEntry(i);
        if(in_L1Tau_nTT<60 && nTTRange) continue;
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

        // FILL PT RESPONSE - ETA BINS HISTOGRAMS
        if (abs(in_OfflineTau_eta) >= 0.0 and abs(in_OfflineTau_eta) < 0.5) pt_resp_AbsEtaBin0to0p5->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (abs(in_OfflineTau_eta) >= 0.5 and abs(in_OfflineTau_eta) < 1.0) pt_resp_AbsEtaBin0p5to1->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (abs(in_OfflineTau_eta) >= 1.0 and abs(in_OfflineTau_eta) < 1.5) pt_resp_AbsEtaBin1to1p5->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (abs(in_OfflineTau_eta) >= 1.5 and abs(in_OfflineTau_eta) <= 2.1) pt_resp_AbsEtaBin1p5to2p1->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;

        if (in_OfflineTau_eta >= 0.0 and in_OfflineTau_eta < 0.5) pt_resp_PlusEtaBin0to0p5->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_eta >= 0.5 and in_OfflineTau_eta < 1.0) pt_resp_PlusEtaBin0p5to1->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_eta >= 1.0 and in_OfflineTau_eta < 1.5) pt_resp_PlusEtaBin1to1p5->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_eta >= 1.5 and in_OfflineTau_eta <= 2.1) pt_resp_PlusEtaBin1p5to2p1->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;

        if (in_OfflineTau_eta <=  0.0 and in_OfflineTau_eta > -0.5) pt_resp_MinusEtaBin0to0p5->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_eta <= -0.5 and in_OfflineTau_eta > -1.0) pt_resp_MinusEtaBin0p5to1->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_eta <= -1.0 and in_OfflineTau_eta > -1.5) pt_resp_MinusEtaBin1to1p5->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        if (in_OfflineTau_eta <= -1.5 and in_OfflineTau_eta >= -2.1) pt_resp_MinusEtaBin1p5to2p1->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
    }
    f.Close();


    TString InputFileName_data = "/data_CMS/cms/motta/Run3preparation/2022_01_06_optimizationV"+version+"_calibThr"+intgr+"p"+decim+"/EphemeralZeroBias_2018D_Run"+run_str+"_CALIBRATED.root";
    TFile f_data(InputFileName_data.Data(),"READ");
    TTree* inTree_data = (TTree*)f_data.Get("outTreeCalibrated");

    std::vector<int>           *in_L1Tau_nTT_data = 0;
    std::vector<float>         *in_L1Tau_pt_data = 0;
    std::vector<float>         *in_L1Tau_eta_data = 0;
    std::vector<float>         *in_L1Tau_CalibPt_data = 0;

    inTree_data->SetBranchAddress("L1Tau_nTT", &in_L1Tau_nTT_data);
    inTree_data->SetBranchAddress("L1Tau_pt", &in_L1Tau_pt_data);
    inTree_data->SetBranchAddress("L1Tau_eta", &in_L1Tau_eta_data);
    inTree_data->SetBranchAddress("L1Tau_CalibPt", &in_L1Tau_CalibPt_data);

    for(UInt_t i = 0 ; i < inTree_data->GetEntries() ; ++i)
    {
        inTree_data->GetEntry(i);
        
        for(UInt_t iTau = 0 ; iTau < in_L1Tau_CalibPt_data->size() ; ++iTau)
        {
            if(in_L1Tau_nTT_data->at(iTau)<60 && nTTRange) continue;
            if(fabs(in_L1Tau_eta_data->at(iTau))>2.1) continue;
            
            // FILL PT RESPONSE - FULL INCLUSIVE
            pt_response_ptInclusive_data->Fill( in_L1Tau_CalibPt_data->at(iTau) / in_L1Tau_pt_data->at(iTau) );
        }
    }
    f_data.Close();

    std:cout << "test";

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

    pt_response_ptInclusive_data->Scale(1.0/pt_response_ptInclusive_data->Integral());
    pt_response_ptInclusive_mc->Scale(1.0/pt_response_ptInclusive_mc->Integral());

    // FIT CRYSTALBALL FUNCTIONS
    TF1 fit_barrel_Inclusive = fitResponseHisto(fitOption, pt_barrel_resp_ptInclusive, 0, 3, pt_barrel_resp_ptInclusive->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin20to25 = fitResponseHisto(fitOption, pt_barrel_resp_ptBin20to25, 0, 3, pt_barrel_resp_ptBin20to25->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin25to30 = fitResponseHisto(fitOption, pt_barrel_resp_ptBin25to30, 0, 3, pt_barrel_resp_ptBin25to30->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin30to35 = fitResponseHisto(fitOption, pt_barrel_resp_ptBin30to35, 0, 3, pt_barrel_resp_ptBin30to35->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin35to40 = fitResponseHisto(fitOption, pt_barrel_resp_ptBin35to40, 0, 3, pt_barrel_resp_ptBin35to40->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin40to45 = fitResponseHisto(fitOption, pt_barrel_resp_ptBin40to45, 0, 3, pt_barrel_resp_ptBin40to45->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin45to50 = fitResponseHisto(fitOption, pt_barrel_resp_ptBin45to50, 0, 3, pt_barrel_resp_ptBin45to50->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin50to60 = fitResponseHisto(fitOption, pt_barrel_resp_ptBin50to60, 0, 3, pt_barrel_resp_ptBin50to60->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin60to70 = fitResponseHisto(fitOption, pt_barrel_resp_ptBin60to70, 0, 3, pt_barrel_resp_ptBin60to70->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin70to90 = fitResponseHisto(fitOption, pt_barrel_resp_ptBin70to90, 0, 3, pt_barrel_resp_ptBin70to90->Integral(), 1, 0.2, -1, 1);
    TF1 fit_barrel_Bin90to110 = fitResponseHisto(fitOption, pt_barrel_resp_ptBin90to110, 0, 3, pt_barrel_resp_ptBin90to110->Integral(), 1, 0.2, -1, 1);

    TF1 fit_endcap_Inclusive = fitResponseHisto(fitOption, pt_endcap_resp_ptInclusive, 0, 3, pt_endcap_resp_ptInclusive->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin20to25 = fitResponseHisto(fitOption, pt_endcap_resp_ptBin20to25, 0, 3, pt_endcap_resp_ptBin20to25->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin25to30 = fitResponseHisto(fitOption, pt_endcap_resp_ptBin25to30, 0, 3, pt_endcap_resp_ptBin25to30->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin30to35 = fitResponseHisto(fitOption, pt_endcap_resp_ptBin30to35, 0, 3, pt_endcap_resp_ptBin30to35->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin35to40 = fitResponseHisto(fitOption, pt_endcap_resp_ptBin35to40, 0, 3, pt_endcap_resp_ptBin35to40->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin40to45 = fitResponseHisto(fitOption, pt_endcap_resp_ptBin40to45, 0, 3, pt_endcap_resp_ptBin40to45->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin45to50 = fitResponseHisto(fitOption, pt_endcap_resp_ptBin45to50, 0, 3, pt_endcap_resp_ptBin45to50->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin50to60 = fitResponseHisto(fitOption, pt_endcap_resp_ptBin50to60, 0, 3, pt_endcap_resp_ptBin50to60->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin60to70 = fitResponseHisto(fitOption, pt_endcap_resp_ptBin60to70, 0, 3, pt_endcap_resp_ptBin60to70->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin70to90 = fitResponseHisto(fitOption, pt_endcap_resp_ptBin70to90, 0, 3, pt_endcap_resp_ptBin70to90->Integral(), 1, 0.2, -1, 1);
    TF1 fit_endcap_Bin90to110 = fitResponseHisto(fitOption, pt_endcap_resp_ptBin90to110, 0, 3, pt_endcap_resp_ptBin90to110->Integral(), 1, 0.2, -1, 1);

    TF1 fit_AbsEtaBin0to0p5 = fitResponseHisto(fitOption, pt_resp_AbsEtaBin0to0p5, 0, 3, pt_resp_AbsEtaBin0to0p5->Integral(), 1, 0.2, -1, 1);
    TF1 fit_AbsEtaBin0p5to1 = fitResponseHisto(fitOption, pt_resp_AbsEtaBin0p5to1, 0, 3, pt_resp_AbsEtaBin0p5to1->Integral(), 1, 0.2, -1, 1);
    TF1 fit_AbsEtaBin1to1p5 = fitResponseHisto(fitOption, pt_resp_AbsEtaBin1to1p5, 0, 3, pt_resp_AbsEtaBin1to1p5->Integral(), 1, 0.2, -1, 1);
    TF1 fit_AbsEtaBin1p5to2p1 = fitResponseHisto(fitOption, pt_resp_AbsEtaBin1p5to2p1, 0, 3, pt_resp_AbsEtaBin1p5to2p1->Integral(), 1, 0.2, -1, 1);

    TF1 fit_PlusEtaBin0to0p5 = fitResponseHisto(fitOption, pt_resp_PlusEtaBin0to0p5, 0, 3, pt_resp_PlusEtaBin0to0p5->Integral(), 1, 0.2, -1, 1);
    TF1 fit_PlusEtaBin0p5to1 = fitResponseHisto(fitOption, pt_resp_PlusEtaBin0p5to1, 0, 3, pt_resp_PlusEtaBin0p5to1->Integral(), 1, 0.2, -1, 1);
    TF1 fit_PlusEtaBin1to1p5 = fitResponseHisto(fitOption, pt_resp_PlusEtaBin1to1p5, 0, 3, pt_resp_PlusEtaBin1to1p5->Integral(), 1, 0.2, -1, 1);
    TF1 fit_PlusEtaBin1p5to2p1 = fitResponseHisto(fitOption, pt_resp_PlusEtaBin1p5to2p1, 0, 3, pt_resp_PlusEtaBin1p5to2p1->Integral(), 1, 0.2, -1, 1);

    TF1 fit_MinusEtaBin0to0p5 = fitResponseHisto(fitOption, pt_resp_MinusEtaBin0to0p5, 0, 3, pt_resp_MinusEtaBin0to0p5->Integral(), 1, 0.2, -1, 1);
    TF1 fit_MinusEtaBin0p5to1 = fitResponseHisto(fitOption, pt_resp_MinusEtaBin0p5to1, 0, 3, pt_resp_MinusEtaBin0p5to1->Integral(), 1, 0.2, -1, 1);
    TF1 fit_MinusEtaBin1to1p5 = fitResponseHisto(fitOption, pt_resp_MinusEtaBin1to1p5, 0, 3, pt_resp_MinusEtaBin1to1p5->Integral(), 1, 0.2, -1, 1);
    TF1 fit_MinusEtaBin1p5to2p1 = fitResponseHisto(fitOption, pt_resp_MinusEtaBin1p5to2p1, 0, 3, pt_resp_MinusEtaBin1p5to2p1->Integral(), 1, 0.2, -1, 1);    

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


    // REMOVE STATS PANEL
    gStyle->SetOptStat(000000);

    TPaveText* texl = new TPaveText(0.2,0.87,0.3,0.99,"NDC");
    texl->AddText("CMS Internal");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    
    TLegend* leg = new TLegend(0.5,0.7,0.8,0.88);
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
    fit_barrel_Inclusive.Draw("same");
    fit_endcap_Inclusive.Draw("same");
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
    fit_barrel_Bin20to25.Draw("same");
    fit_endcap_Bin20to25.Draw("same");
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
    fit_barrel_Bin25to30.Draw("same");
    fit_endcap_Bin25to30.Draw("same");
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
    fit_barrel_Bin30to35.Draw("same");
    fit_endcap_Bin30to35.Draw("same");
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
    fit_barrel_Bin35to40.Draw("same");
    fit_endcap_Bin35to40.Draw("same");
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
    fit_barrel_Bin40to45.Draw("same");
    fit_endcap_Bin40to45.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad6->Update();
    c.Update();

    c.SaveAs("plots/split/response_ptBins_optimizationV"+version+"_calibThr"+intgr+"p"+decim+".png");

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
    leg->AddEntry(&fit_AbsEtaBin0to0p5,fitOption+" fit","L");

    pt_resp_AbsEtaBin0to0p5->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_AbsEtaBin0to0p5->GetYaxis()->SetTitle("a.u");

    pt_resp_AbsEtaBin0to0p5->Draw("E0");
    fit_AbsEtaBin0to0p5.Draw("same");
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
    leg->AddEntry(&fit_AbsEtaBin0p5to1,fitOption+" fit","L");

    pt_resp_AbsEtaBin0p5to1->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_AbsEtaBin0p5to1->GetYaxis()->SetTitle("a.u");

    pt_resp_AbsEtaBin0p5to1->Draw("E0");
    fit_AbsEtaBin0p5to1.Draw("same");
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
    leg->AddEntry(&fit_AbsEtaBin1to1p5,fitOption+" fit","L");

    pt_resp_AbsEtaBin1to1p5->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_AbsEtaBin1to1p5->GetYaxis()->SetTitle("a.u");

    pt_resp_AbsEtaBin1to1p5->Draw("E0");
    fit_AbsEtaBin1to1p5.Draw("same");
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
    leg->AddEntry(&fit_AbsEtaBin1p5to2p1,fitOption+" fit","L");

    pt_resp_AbsEtaBin1p5to2p1->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_AbsEtaBin1p5to2p1->GetYaxis()->SetTitle("a.u");

    pt_resp_AbsEtaBin1p5to2p1->Draw("E0");
    fit_AbsEtaBin1p5to2p1.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad10->Update();
    c2.Update();

    c2.SaveAs("plots/split/response_absEtaBins_optimizationV"+version+"_calibThr"+intgr+"p"+decim+".png");

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
    leg->AddEntry(&fit_PlusEtaBin0to0p5,fitOption+" fit","L");

    pt_resp_PlusEtaBin0to0p5->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_PlusEtaBin0to0p5->GetYaxis()->SetTitle("a.u");

    pt_resp_PlusEtaBin0to0p5->Draw("E0");
    fit_PlusEtaBin0to0p5.Draw("same");
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
    leg->AddEntry(&fit_PlusEtaBin0p5to1,fitOption+" fit","L");

    pt_resp_PlusEtaBin0p5to1->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_PlusEtaBin0p5to1->GetYaxis()->SetTitle("a.u");

    pt_resp_PlusEtaBin0p5to1->Draw("E0");
    fit_PlusEtaBin0p5to1.Draw("same");
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
    leg->AddEntry(&fit_PlusEtaBin1to1p5,fitOption+" fit","L");

    pt_resp_PlusEtaBin1to1p5->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_PlusEtaBin1to1p5->GetYaxis()->SetTitle("a.u");

    pt_resp_PlusEtaBin1to1p5->Draw("E0");
    fit_PlusEtaBin1to1p5.Draw("same");
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
    leg->AddEntry(&fit_PlusEtaBin1p5to2p1,fitOption+" fit","L");

    pt_resp_PlusEtaBin1p5to2p1->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_PlusEtaBin1p5to2p1->GetYaxis()->SetTitle("a.u");

    pt_resp_PlusEtaBin1p5to2p1->Draw("E0");
    fit_PlusEtaBin1p5to2p1.Draw("same");
    texl->Draw("same");
    leg->Draw("same");
    
    pad17->Update();
    c4.Update();

    c4.SaveAs("plots/split/response_plusEtaBins_optimizationV"+version+"_calibThr"+intgr+"p"+decim+".png");

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
    leg->AddEntry(&fit_MinusEtaBin0to0p5,fitOption+" fit","L");

    pt_resp_MinusEtaBin0to0p5->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_MinusEtaBin0to0p5->GetYaxis()->SetTitle("a.u");

    pt_resp_MinusEtaBin0to0p5->Draw("E0");
    fit_MinusEtaBin0to0p5.Draw("same");
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
    leg->AddEntry(&fit_MinusEtaBin0p5to1,fitOption+" fit","L");

    pt_resp_MinusEtaBin0p5to1->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_MinusEtaBin0p5to1->GetYaxis()->SetTitle("a.u");

    pt_resp_MinusEtaBin0p5to1->Draw("E0");
    fit_MinusEtaBin0p5to1.Draw("same");
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
    leg->AddEntry(&fit_MinusEtaBin1to1p5,fitOption+" fit","L");

    pt_resp_MinusEtaBin1to1p5->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_MinusEtaBin1to1p5->GetYaxis()->SetTitle("a.u");

    pt_resp_MinusEtaBin1to1p5->Draw("E0");
    fit_MinusEtaBin1to1p5.Draw("same");
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
    leg->AddEntry(&fit_MinusEtaBin1p5to2p1,fitOption+" fit","L");

    pt_resp_MinusEtaBin1p5to2p1->GetXaxis()->SetTitle("E_{T}^{L1}(#tau)/p_{T}^{offline}(#tau)");
    pt_resp_MinusEtaBin1p5to2p1->GetYaxis()->SetTitle("a.u");

    pt_resp_MinusEtaBin1p5to2p1->Draw("E0");
    fit_MinusEtaBin1p5to2p1.Draw("same");
    texl->Draw("same");
    //leg->Draw("same");
    
    pad21->Update();
    c5.Update();

    c5.SaveAs("plots/split/response_minusEtaBins_optimizationV"+version+"_calibThr"+intgr+"p"+decim+".png");

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

    c3.SaveAs("plots/pt_resolution_optimizationV"+version+"_calibThr"+intgr+"p"+decim+"_fctPt.png");

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

    c6.SaveAs("plots/pt_resolution_optimizationV"+version+"_calibThr"+intgr+"p"+decim+"_fctEta.png");

    //##########
    // plot resolution in pt inclusive

    TCanvas c7("c7","c7",650,600);
    c7.SetLeftMargin(0.15);
    c7.SetGrid();
    c7.cd();

    TPad *pad22 = new TPad("pad22", "pad22", 0., 0., 1., 1.);
    pad22->SetLeftMargin(0.15);
    pad22->SetGridx();         // Vertical grid
    pad22->SetGridy();         // Vertical grid
    pad22->Draw();             // Draw the upper pad: pad1
    pad22->cd();               // pad1 becomes the current pad

    pt_response_ptInclusive_data->SetLineColor(kBlack);
    pt_response_ptInclusive_data->SetMarkerStyle(8);
    pt_response_ptInclusive_data->SetMarkerColor(kBlack);
    pt_response_ptInclusive_data->SetTitle("");
    pt_response_ptInclusive_data->GetXaxis()->SetTitle("Offline p_{T}(#tau) [GeV]");
    pt_response_ptInclusive_data->GetYaxis()->SetTitle("p^{L1}_{T}(#tau) resolution");
    //pt_response_ptInclusive_data->GetYaxis()->SetRangeUser(0.,0.3);
    pt_response_ptInclusive_data->Draw("E0");
    pt_response_ptInclusive_mc->SetLineColor(kRed);
    pt_response_ptInclusive_mc->SetMarkerStyle(8);
    pt_response_ptInclusive_mc->SetMarkerColor(kRed);
    pt_response_ptInclusive_mc->SetTitle("");
    pt_response_ptInclusive_mc->Draw("E0 same");
    leg->Clear();
    leg->AddEntry(pt_response_ptInclusive_mc,"Run3 MC VBFHTauTau M125","L");
    leg->AddEntry(pt_response_ptInclusive_data,"Data 2018D - Run"+run_str,"L");
    leg->Draw("same");
    texl->Draw("same");

    c7.SaveAs("plots/data/Run"+run_str+"comparison_pt_resolution_optimizationV"+version+"_calibThr"+intgr+"p"+decim+"_inclusivePtEta.png");
}
