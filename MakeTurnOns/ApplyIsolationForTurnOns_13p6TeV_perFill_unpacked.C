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
#include <sstream>
#include <TBranchElement.h>
#include <fstream>
#include <TGraphAsymmErrors.h>
#include <stdio.h>
#include <math.h>
#include "../Calibrate/ApplyCalibration_newnTT.C"

using namespace std;

void ApplyIsolationForTurnOns(TString tag, TString run_str, TString fill_str)
{
  TString InputFileName = "/data_CMS/cms/motta/Run3preparation/Muon_Run2022"+tag+"_"+run_str+"_MINIAOD/Muon_Run2022"+tag+"_"+run_str+"_MINIAOD.root";

  TFile f(InputFileName.Data(),"READ");
  TTree* inTree = (TTree*)f.Get("Ntuplizer/TagAndProbe");

  ULong64_t in_EventNumber =  0;
  Int_t     in_RunNumber =  0;
  Int_t     in_lumi =  0;
  Float_t   in_offlineTauPt = 0;
  Float_t   in_offlineTauEta = 0;
  Float_t   in_offlineTauPhi = 0;
  Float_t   in_l1tPt =  0;
  Float_t   in_l1tEta =  0;
  Float_t   in_l1tPhi =  0;
  Int_t     in_l1tQual =  0;
  Int_t     in_l1tIso = 0;
 
  inTree->SetBranchAddress("EventNumber", &in_EventNumber);
  inTree->SetBranchAddress("RunNumber", &in_RunNumber);
  // branches of uncalibrated EphemeralZeroBias NTuples
  inTree->SetBranchAddress("lumi", &in_lumi);
  inTree->SetBranchAddress("tauPt",&in_offlineTauPt);
  inTree->SetBranchAddress("tauEta",&in_offlineTauEta);
  inTree->SetBranchAddress("tauPhi",&in_offlineTauPhi);
  inTree->SetBranchAddress("l1tPt",&in_l1tPt);
  inTree->SetBranchAddress("l1tEta",&in_l1tEta);
  inTree->SetBranchAddress("l1tPhi",&in_l1tPhi);
  inTree->SetBranchAddress("l1tQual",&in_l1tQual);
  inTree->SetBranchAddress("l1tIso",&in_l1tIso);

  Double_t binning[22] = {18,20,22,24,26,28,30,32,35,40,45,50,60,70,90,110,210,350,500,700,1000,2000};
  TH1F* pt = new TH1F("pt","pt",21,binning);
  TH1F* pt_pass_noIso  = new TH1F("pt_pass_noIso"  ,"pt_pass_noIso"  ,21,binning);
  TH1F* pt_pass_Iso    = new TH1F("pt_pass_Iso"    ,"pt_pass_Iso"    ,21,binning);
  TH1F* pt_pass_fixed1 = new TH1F("pt_pass_fixed1" ,"pt_pass_fixed1" ,21,binning);
  TH1F* pt_pass_fixed2 = new TH1F("pt_pass_fixed2" ,"pt_pass_fixed2" ,21,binning);
  TH1F* pt_pass_fixed3 = new TH1F("pt_pass_fixed3" ,"pt_pass_fixed3" ,21,binning);

  Double_t Threshold_NewLayer1_noIso = 0.0;
  Double_t Threshold_NewLayer1_Iso = 0.0;
  
  for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    {
      inTree->GetEntry(i);

      if (fill_str=="8112-8113-8115" && in_RunNumber < 357080) { continue; }
      if (fill_str=="8088-8094-8102" && in_RunNumber >= 357080) { continue; }

      if(fill_str=="8088-8094-8102")
      {
        // Threshold_NewLayer1_noIso = 28.51; // unpacked
        // Threshold_NewLayer1_Iso = 25.51; // unpacked

        Threshold_NewLayer1_noIso = 34.51; // re-emulated
        Threshold_NewLayer1_Iso = 28.51; // re-emulated
      }
      else if(fill_str=="8112-8113-8115")
      {
        // Threshold_NewLayer1_noIso = 30.51; // unpacked
        // Threshold_NewLayer1_Iso = 26.51; // unpacked

        Threshold_NewLayer1_noIso = 35.51; // re-emulated
        Threshold_NewLayer1_Iso = 28.51; // re-emulated
      }

      if (i%10000==0) std::cout << i << std::endl;

      pt->Fill(in_offlineTauPt);

      if(in_l1tPt>=Threshold_NewLayer1_noIso) pt_pass_noIso->Fill(in_offlineTauPt);
      if(in_l1tPt>=Threshold_NewLayer1_Iso && in_l1tIso>0) pt_pass_Iso->Fill(in_offlineTauPt);
      
      if(in_l1tPt>=29.51 && in_l1tIso>0) pt_pass_fixed1->Fill(in_offlineTauPt);
      if(in_l1tPt>=31.51 && in_l1tIso>0) pt_pass_fixed2->Fill(in_offlineTauPt);
      if(in_l1tPt>=33.51 && in_l1tIso>0) pt_pass_fixed3->Fill(in_offlineTauPt);
    }

  TString FileNameOut = "/data_CMS/cms/motta/Run3preparation/Muon_Run2022"+tag+"_"+run_str+"_MINIAOD/Muon_Run2022"+tag+"_Fill"+fill_str+"_TURNONS.root";
  TFile turnOns(FileNameOut.Data(),"RECREATE");

  TGraphAsymmErrors* turnOn_noIso = new TGraphAsymmErrors(pt_pass_noIso,pt,"cp");
  turnOn_noIso->Write();

  TGraphAsymmErrors* turnOn_Iso = new TGraphAsymmErrors(pt_pass_Iso,pt,"cp");
  turnOn_Iso->Write();

  TGraphAsymmErrors* turnOn_fixed1 = new TGraphAsymmErrors(pt_pass_fixed1,pt,"cp");
  turnOn_fixed1->Write();

  TGraphAsymmErrors* turnOn_fixed2 = new TGraphAsymmErrors(pt_pass_fixed2,pt,"cp");
  turnOn_fixed2->Write();

  TGraphAsymmErrors* turnOn_fixed3 = new TGraphAsymmErrors(pt_pass_fixed3,pt,"cp");
  turnOn_fixed3->Write();
}
