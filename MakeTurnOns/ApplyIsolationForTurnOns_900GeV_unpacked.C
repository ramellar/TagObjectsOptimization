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

void ApplyIsolationForTurnOns()
{
  TString InputFileName = "/data_CMS/cms/motta/Run3preparation/2022_04_21_optimizationV11_ReOptReEmu/ZeroBias1_Run2022A_reEmuTPs.root";
  TFile f(InputFileName.Data(),"READ");
  TTree* inTree = (TTree*)f.Get("ZeroBias/ZeroBias");

  ULong64_t       in_EventNumber =  0;
  Int_t           in_RunNumber =  0;
  Int_t           in_lumi =  0;
  vector<float>   *in_l1tPt =  0;
  vector<float>   *in_l1tEta =  0;
  vector<float>   *in_l1tPhi =  0;
  vector<int>     *in_l1tQual =  0;
  vector<int>     *in_l1tIso = 0;
 
  inTree->SetBranchAddress("EventNumber", &in_EventNumber);
  inTree->SetBranchAddress("RunNumber", &in_RunNumber);
  // branches of uncalibrated EphemeralZeroBias NTuples
  inTree->SetBranchAddress("lumi", &in_lumi);
  inTree->SetBranchAddress("l1tPt",&in_l1tPt);
  inTree->SetBranchAddress("l1tEta",&in_l1tEta);
  inTree->SetBranchAddress("l1tPhi",&in_l1tPhi);
  inTree->SetBranchAddress("l1tQual",&in_l1tQual);
  inTree->SetBranchAddress("l1tIso",&in_l1tIso);

  Double_t binning[22] = {18,20,22,24,26,28,30,32,35,40,45,50,60,70,90,110,210,350,500,700,1000,2000};
  TH1F* pt = new TH1F("pt","pt",21,binning);
  TH1F* pt_pass_noIso   = new TH1F("pt_pass_noIso"  ,"pt_pass_noIso"  ,21,binning);
  TH1F* pt_pass_OptionG3  = new TH1F("pt_pass_OptionG3" ,"pt_pass_OptionG3" ,21,binning);
  TH1F* pt_pass_fixed15  = new TH1F("pt_pass_fixed15" ,"pt_pass_fixed15" ,21,binning);
  TH1F* pt_pass_fixed20  = new TH1F("pt_pass_fixed20" ,"pt_pass_fixed20" ,21,binning);
  TH1F* pt_pass_fixed25  = new TH1F("pt_pass_fixed25" ,"pt_pass_fixed25" ,21,binning);

  Double_t Threshold_NewLayer1_noIso = 46.51;
  Double_t Threshold_NewLayer1_OptionG3 = 34.51;
  
  for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    {
      inTree->GetEntry(i);

      if (i%10000==0) std::cout << i << std::endl;

      for(UInt_t iL1Tau = 0 ; iL1Tau < in_l1tPt->size() ; ++iL1Tau)
        {
          pt->Fill(in_l1tPt->at(iL1Tau));
	
          if(in_l1tPt->at(iL1Tau)>=Threshold_NewLayer1_noIso) pt_pass_noIso->Fill(in_l1tPt->at(iL1Tau));
          if(in_l1tPt->at(iL1Tau)>=Threshold_NewLayer1_OptionG3 && in_l1tIso->at(iL1Tau)>0) pt_pass_OptionG3->Fill(in_l1tPt->at(iL1Tau));
          
          if(in_l1tPt->at(iL1Tau)>=15. && in_l1tIso->at(iL1Tau)>0) pt_pass_fixed15->Fill(in_l1tPt->at(iL1Tau));
          if(in_l1tPt->at(iL1Tau)>=20. && in_l1tIso->at(iL1Tau)>0) pt_pass_fixed20->Fill(in_l1tPt->at(iL1Tau));
          if(in_l1tPt->at(iL1Tau)>=25. && in_l1tIso->at(iL1Tau)>0) pt_pass_fixed25->Fill(in_l1tPt->at(iL1Tau));
        }  
    }

  TString FileNameOut = "/data_CMS/cms/motta/Run3preparation/2022_04_21_optimizationV11_ReOptReEmu/ZeroBias1_Run2022A_reEmuTPs_TURNONS.root";
  TFile turnOns(FileNameOut.Data(),"RECREATE");

  TGraphAsymmErrors* turnOn_noIso = new TGraphAsymmErrors(pt_pass_noIso,pt,"cp");
  turnOn_noIso->Write();

  TGraphAsymmErrors* turnOn_OptionG3 = new TGraphAsymmErrors(pt_pass_OptionG3,pt,"cp");
  turnOn_OptionG3->Write();

  TGraphAsymmErrors* turnOn_fixed15 = new TGraphAsymmErrors(pt_pass_fixed15,pt,"cp");
  turnOn_fixed15->Write();

  TGraphAsymmErrors* turnOn_fixed20 = new TGraphAsymmErrors(pt_pass_fixed20,pt,"cp");
  turnOn_fixed20->Write();

  TGraphAsymmErrors* turnOn_fixed25 = new TGraphAsymmErrors(pt_pass_fixed25,pt,"cp");
  turnOn_fixed25->Write();
}
