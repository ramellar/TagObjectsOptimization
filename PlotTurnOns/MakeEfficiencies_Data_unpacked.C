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
#include <typeinfo>
#include "../Calibrate/ApplyCalibration_newnTT.C"

using namespace std;

void MakeEfficiencies(TString file, int run_nmbr, TString era = "")
{
  TString run_nmbr_str = to_string(run_nmbr);
  if(era != "" && run_nmbr == -1) { run_nmbr_str = era; }

  TFile f(file,"READ");
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
  Int_t     Nvtx = 0;
  inTree->SetBranchAddress("EventNumber", &in_EventNumber);
  inTree->SetBranchAddress("RunNumber", &in_RunNumber);
  inTree->SetBranchAddress("lumi", &in_lumi);
  inTree->SetBranchAddress("tauPt",&in_offlineTauPt);
  inTree->SetBranchAddress("tauEta",&in_offlineTauEta);
  inTree->SetBranchAddress("tauPhi",&in_offlineTauPhi);
  inTree->SetBranchAddress("l1tPt",&in_l1tPt);
  inTree->SetBranchAddress("l1tEta",&in_l1tEta);
  inTree->SetBranchAddress("l1tPhi",&in_l1tPhi);
  inTree->SetBranchAddress("l1tQual",&in_l1tQual);
  inTree->SetBranchAddress("l1tIso",&in_l1tIso);
  inTree->SetBranchAddress("Nvtx",&Nvtx);

  Double_t binningPt[23] = {18,20,22,24,26,28,30,32,35,40,45,50,60,70,90,110,150,210,350,500,700,1000,2000};
  TH1F* pt = new TH1F("pt","pt",22,binningPt);
  TH1F* barrel_pt = new TH1F("barrel_pt","barrel_pt",22,binningPt);
  TH1F* endcap_pt = new TH1F("endcap_pt","endcap_pt",22,binningPt);
  TH1F* dummy = new TH1F("dummy","dummy",22,binningPt);

  Double_t binningEta[29] = {-2.100, -1.950, -1.800, -1.650, -1.479, -1.305, -1.200, -1.050, -0.9000, -0.7500, -0.6000, -0.4500, -0.3000, -0.1500, 0., 0.1500, 0.3000, 0.4500, 0.6000, 0.7500, 0.9000, 1.050, 1.200,  1.305, 1.479, 1.650, 1.800, 1.950, 2.100};
  TH1F* eta  = new TH1F("eta" ,"eta" ,28, binningEta);

  Double_t binningNvtx[11] = {0., 10., 15., 20., 25., 30., 35., 40., 45., 50., 60.};
  TH1F* nvtx  = new TH1F("nvtx" ,"nvtx" ,10, binningNvtx);
  TH1F* barrel_nvtx  = new TH1F("barrel_nvtx" ,"barrel_nvtx" ,10, binningNvtx);
  TH1F* endcap_nvtx  = new TH1F("endcap_nvtx" ,"endcap_nvtx" ,10, binningNvtx);

  std::vector<int> thrs = {24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45};
  std::vector<TH1F*> ptProgressionFixedThr_noIso = {};
  std::vector<TH1F*> ptProgressionFixedThr_Iso = {};
  std::vector<TH1F*> barrel_ptProgressionFixedThr_noIso = {};
  std::vector<TH1F*> barrel_ptProgressionFixedThr_Iso = {};
  std::vector<TH1F*> endcap_ptProgressionFixedThr_noIso = {};
  std::vector<TH1F*> endcap_ptProgressionFixedThr_Iso = {};
  std::vector<TH1F*> etaProgressionFixedThr_noIso = {};
  std::vector<TH1F*> etaProgressionFixedThr_Iso = {};
  std::vector<TH1F*> nvtxProgressionFixedThr_noIso = {};
  std::vector<TH1F*> nvtxProgressionFixedThr_Iso = {};
  std::vector<TH1F*> barrel_nvtxProgressionFixedThr_noIso = {};
  std::vector<TH1F*> barrel_nvtxProgressionFixedThr_Iso = {};
  std::vector<TH1F*> endcap_nvtxProgressionFixedThr_noIso = {};
  std::vector<TH1F*> endcap_nvtxProgressionFixedThr_Iso = {};

  // ALL THE SAME PLOTS FOR RUN2
  TH1F* R2_pt = new TH1F("R2_pt","R2_pt",22,binningPt);
  TH1F* R2_barrel_pt = new TH1F("R2_barrel_pt","R2_barrel_pt",22,binningPt);
  TH1F* R2_endcap_pt = new TH1F("R2_endcap_pt","R2_endcap_pt",22,binningPt);
  TH1F* R2_eta  = new TH1F("R2_eta" ,"R2_eta" ,28, binningEta);
  TH1F* R2_nvtx  = new TH1F("R2_nvtx" ,"R2_nvtx" ,10, binningNvtx);
  TH1F* R2_barrel_nvtx  = new TH1F("R2_barrel_nvtx" ,"R2_barrel_nvtx" ,10, binningNvtx);
  TH1F* R2_endcap_nvtx  = new TH1F("R2_endcap_nvtx" ,"R2_endcap_nvtx" ,10, binningNvtx);
  std::vector<TH1F*> R2_ptProgressionFixedThr_noIso = {};
  std::vector<TH1F*> R2_ptProgressionFixedThr_Iso = {};
  std::vector<TH1F*> R2_barrel_ptProgressionFixedThr_noIso = {};
  std::vector<TH1F*> R2_barrel_ptProgressionFixedThr_Iso = {};
  std::vector<TH1F*> R2_endcap_ptProgressionFixedThr_noIso = {};
  std::vector<TH1F*> R2_endcap_ptProgressionFixedThr_Iso = {};
  std::vector<TH1F*> R2_etaProgressionFixedThr_noIso = {};
  std::vector<TH1F*> R2_etaProgressionFixedThr_Iso = {};
  std::vector<TH1F*> R2_nvtxProgressionFixedThr_noIso = {};
  std::vector<TH1F*> R2_nvtxProgressionFixedThr_Iso = {};
  std::vector<TH1F*> R2_barrel_nvtxProgressionFixedThr_noIso = {};
  std::vector<TH1F*> R2_barrel_nvtxProgressionFixedThr_Iso = {};
  std::vector<TH1F*> R2_endcap_nvtxProgressionFixedThr_noIso = {};
  std::vector<TH1F*> R2_endcap_nvtxProgressionFixedThr_Iso = {};

  for(long unsigned int i = 0; i < thrs.size(); ++i)
  {
    TString thr;
    thr.Form("%i", thrs[i]);

    ptProgressionFixedThr_noIso.push_back(new TH1F("ptProgressionAt"+thr+"_noIso","ptProgressionAt"+thr+"_noIso",22,binningPt));
    ptProgressionFixedThr_Iso.push_back(new TH1F("ptProgressionAt"+thr+"_Iso","ptProgressionAt"+thr+"_Iso",22,binningPt));

    barrel_ptProgressionFixedThr_noIso.push_back(new TH1F("barrel_ptProgressionAt"+thr+"_noIso","barrel_ptProgressionAt"+thr+"_noIso",22,binningPt));
    barrel_ptProgressionFixedThr_Iso.push_back(new TH1F("barrel_ptProgressionAt"+thr+"_Iso","barrel_ptProgressionAt"+thr+"_Iso",22,binningPt));

    endcap_ptProgressionFixedThr_noIso.push_back(new TH1F("endcap_ptProgressionAt"+thr+"_noIso","endcap_ptProgressionAt"+thr+"_noIso",22,binningPt));
    endcap_ptProgressionFixedThr_Iso.push_back(new TH1F("endcap_ptProgressionAt"+thr+"_Iso","endcap_ptProgressionAt"+thr+"_Iso",22,binningPt));

    etaProgressionFixedThr_noIso.push_back(new TH1F("etaprogressionAt"+thr+"_noIso","etaProgressionAt"+thr+"_noIso",28,binningEta));
    etaProgressionFixedThr_Iso.push_back(new TH1F("etaprogressionAt"+thr+"_Iso","etaProgressionAt"+thr+"_Iso",28,binningEta));

    nvtxProgressionFixedThr_noIso.push_back(new TH1F("nvtxProgressionAt"+thr+"_noIso","nvtxProgressionAt"+thr+"_noIso",10,binningNvtx));
    nvtxProgressionFixedThr_Iso.push_back(new TH1F("nvtxProgressionAt"+thr+"_Iso","nvtxProgressionAt"+thr+"_Iso",10,binningNvtx));

    barrel_nvtxProgressionFixedThr_noIso.push_back(new TH1F("barrel_nvtxProgressionAt"+thr+"_noIso","barrel_nvtxProgressionAt"+thr+"_noIso",10,binningNvtx));
    barrel_nvtxProgressionFixedThr_Iso.push_back(new TH1F("barrel_nvtxProgressionAt"+thr+"_Iso","barrel_nvtxProgressionAt"+thr+"_Iso",10,binningNvtx));

    endcap_nvtxProgressionFixedThr_noIso.push_back(new TH1F("endcap_nvtxProgressionAt"+thr+"_noIso","endcap_nvtxProgressionAt"+thr+"_noIso",10,binningNvtx));
    endcap_nvtxProgressionFixedThr_Iso.push_back(new TH1F("endcap_nvtxProgressionAt"+thr+"_Iso","endcap_nvtxProgressionAt"+thr+"_Iso",10,binningNvtx));

    R2_ptProgressionFixedThr_noIso.push_back(new TH1F("R2_ptProgressionAt"+thr+"_noIso","R2_ptProgressionAt"+thr+"_noIso",22,binningPt));
    R2_ptProgressionFixedThr_Iso.push_back(new TH1F("R2_ptProgressionAt"+thr+"_Iso","R2_ptProgressionAt"+thr+"_Iso",22,binningPt));

    R2_barrel_ptProgressionFixedThr_noIso.push_back(new TH1F("R2_barrel_ptProgressionAt"+thr+"_noIso","R2_barrel_ptProgressionAt"+thr+"_noIso",22,binningPt));
    R2_barrel_ptProgressionFixedThr_Iso.push_back(new TH1F("R2_barrel_ptProgressionAt"+thr+"_Iso","R2_barrel_ptProgressionAt"+thr+"_Iso",22,binningPt));

    R2_endcap_ptProgressionFixedThr_noIso.push_back(new TH1F("R2_endcap_ptProgressionAt"+thr+"_noIso","R2_endcap_ptProgressionAt"+thr+"_noIso",22,binningPt));
    R2_endcap_ptProgressionFixedThr_Iso.push_back(new TH1F("R2_endcap_ptProgressionAt"+thr+"_Iso","R2_endcap_ptProgressionAt"+thr+"_Iso",22,binningPt));

    R2_etaProgressionFixedThr_noIso.push_back(new TH1F("R2_etaprogressionAt"+thr+"_noIso","R2_etaProgressionAt"+thr+"_noIso",28,binningEta));
    R2_etaProgressionFixedThr_Iso.push_back(new TH1F("R2_etaprogressionAt"+thr+"_Iso","R2_etaProgressionAt"+thr+"_Iso",28,binningEta));

    R2_nvtxProgressionFixedThr_noIso.push_back(new TH1F("R2_nvtxProgressionAt"+thr+"_noIso","R2_nvtxProgressionAt"+thr+"_noIso",10,binningNvtx));
    R2_nvtxProgressionFixedThr_Iso.push_back(new TH1F("R2_nvtxProgressionAt"+thr+"_Iso","R2_nvtxProgressionAt"+thr+"_Iso",10,binningNvtx));

    R2_barrel_nvtxProgressionFixedThr_noIso.push_back(new TH1F("R2_barrel_nvtxProgressionAt"+thr+"_noIso","R2_barrel_nvtxProgressionAt"+thr+"_noIso",10,binningNvtx));
    R2_barrel_nvtxProgressionFixedThr_Iso.push_back(new TH1F("R2_barrel_nvtxProgressionAt"+thr+"_Iso","R2_barrel_nvtxProgressionAt"+thr+"_Iso",10,binningNvtx));

    R2_endcap_nvtxProgressionFixedThr_noIso.push_back(new TH1F("R2_endcap_nvtxProgressionAt"+thr+"_noIso","R2_endcap_nvtxProgressionAt"+thr+"_noIso",10,binningNvtx));
    R2_endcap_nvtxProgressionFixedThr_Iso.push_back(new TH1F("R2_endcap_nvtxProgressionAt"+thr+"_Iso","R2_endcap_nvtxProgressionAt"+thr+"_Iso",10,binningNvtx));
  }
  
  for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
  {
    inTree->GetEntry(i);

    // either process the full dataset or just the events with a specific run number
    if (run_nmbr != -1) { if (run_nmbr != in_RunNumber) { continue; } }
    if (i%10000==0) std::cout << i << std::endl;

    if (in_l1tPt<0.) { continue; }

    if (in_offlineTauEta>2.1) { continue; }

    pt->Fill(in_offlineTauPt);
    if (in_offlineTauEta<1.305){ barrel_pt->Fill(in_offlineTauPt); }
    if (in_offlineTauEta>1.479){ endcap_pt->Fill(in_offlineTauPt); }
    if (in_offlineTauPt>40.)
    {
      eta->Fill(in_offlineTauEta);
      nvtx->Fill(Nvtx);
      if (in_offlineTauEta<1.305){ barrel_nvtx->Fill(Nvtx); }
      if (in_offlineTauEta>1.479){ endcap_nvtx->Fill(Nvtx); }
    }

    for (long unsigned int j = 0; j < thrs.size(); j++)
    {
      double thr = thrs[j];

      if(in_l1tPt>=thr)
      { 
        ptProgressionFixedThr_noIso[j]->Fill(in_offlineTauPt);
        if (in_offlineTauEta<1.305) { barrel_ptProgressionFixedThr_noIso[j]->Fill(in_offlineTauPt); }
        if (in_offlineTauEta>1.479) { endcap_ptProgressionFixedThr_noIso[j]->Fill(in_offlineTauPt); }
        if (in_offlineTauPt>40.)
        { 
          etaProgressionFixedThr_noIso[j]->Fill(in_offlineTauEta);
          nvtxProgressionFixedThr_noIso[j]->Fill(Nvtx);
          if (in_offlineTauEta<1.305){ barrel_nvtxProgressionFixedThr_noIso[j]->Fill(Nvtx); }
          if (in_offlineTauEta>1.479){ endcap_nvtxProgressionFixedThr_noIso[j]->Fill(Nvtx); }
        }
      }
      if(in_l1tPt>=thr && in_l1tIso>0)
      { 
        ptProgressionFixedThr_Iso[j]->Fill(in_offlineTauPt);
        if (in_offlineTauEta<1.305) { barrel_ptProgressionFixedThr_Iso[j]->Fill(in_offlineTauPt); }
        if (in_offlineTauEta>1.479) { endcap_ptProgressionFixedThr_Iso[j]->Fill(in_offlineTauPt); }
        if (in_offlineTauPt>40.)
        {
          etaProgressionFixedThr_Iso[j]->Fill(in_offlineTauEta);
          nvtxProgressionFixedThr_Iso[j]->Fill(Nvtx);
          if (in_offlineTauEta<1.305){ barrel_nvtxProgressionFixedThr_Iso[j]->Fill(Nvtx); }
          if (in_offlineTauEta>1.479){ endcap_nvtxProgressionFixedThr_Iso[j]->Fill(Nvtx); }
        }
      }
    }
  }

  TFile R2_f("/data_CMS/cms/motta/Run3preparation/SingleMuon__Run2018D-PromptReco-v2__MINIAOD__GoldenJSON__WMassCutTrue__ZMassCutTrue/SingleMuon__Run2018D-PromptReco-v2__MINIAOD__GoldenJSON__WMassCutTrue__ZMassCutTrue.root","READ");
  TTree* R2_inTree = (TTree*)R2_f.Get("Ntuplizer/TagAndProbe");
  in_EventNumber =  0;
  in_RunNumber =  0;
  in_lumi =  0;
  in_offlineTauPt = 0;
  in_offlineTauEta = 0;
  in_offlineTauPhi = 0;
  in_l1tPt =  0;
  in_l1tEta =  0;
  in_l1tPhi =  0;
  in_l1tQual =  0;
  in_l1tIso = 0;
  Nvtx = 0;
  R2_inTree->SetBranchAddress("EventNumber", &in_EventNumber);
  R2_inTree->SetBranchAddress("RunNumber", &in_RunNumber);
  R2_inTree->SetBranchAddress("lumi", &in_lumi);
  R2_inTree->SetBranchAddress("tauPt",&in_offlineTauPt);
  R2_inTree->SetBranchAddress("tauEta",&in_offlineTauEta);
  R2_inTree->SetBranchAddress("tauPhi",&in_offlineTauPhi);
  R2_inTree->SetBranchAddress("l1tPt",&in_l1tPt);
  R2_inTree->SetBranchAddress("l1tEta",&in_l1tEta);
  R2_inTree->SetBranchAddress("l1tPhi",&in_l1tPhi);
  R2_inTree->SetBranchAddress("l1tQual",&in_l1tQual);
  R2_inTree->SetBranchAddress("l1tIso",&in_l1tIso);
  R2_inTree->SetBranchAddress("Nvtx",&Nvtx);

  for(UInt_t i = 0 ; i < R2_inTree->GetEntries() ; ++i)
  {
    R2_inTree->GetEntry(i);

    if (i%10000==0) std::cout << i << std::endl;

    if (in_l1tPt<0.) { continue; }

    if (in_offlineTauEta>2.1) { continue; }

    R2_pt->Fill(in_offlineTauPt);
    if (in_offlineTauEta<1.305) { R2_barrel_pt->Fill(in_offlineTauPt); }
    if (in_offlineTauEta>1.479) { R2_endcap_pt->Fill(in_offlineTauPt); }
    if (in_offlineTauPt>40.)
    {
      R2_eta->Fill(in_offlineTauEta);
      R2_nvtx->Fill(Nvtx);
      if (in_offlineTauEta<1.305){ R2_barrel_nvtx->Fill(Nvtx); }
      if (in_offlineTauEta>1.479){ R2_endcap_nvtx->Fill(Nvtx); }
    }

    for (long unsigned int j = 0; j < thrs.size(); j++)
    {
      double thr = thrs[j];

      if(in_l1tPt>=thr)
      { 
        R2_ptProgressionFixedThr_noIso[j]->Fill(in_offlineTauPt);
        if (in_offlineTauEta<1.305) { R2_barrel_ptProgressionFixedThr_noIso[j]->Fill(in_offlineTauPt); }
        if (in_offlineTauEta>1.479) { R2_endcap_ptProgressionFixedThr_noIso[j]->Fill(in_offlineTauPt); }
        if (in_offlineTauPt>40.)
        {
          R2_etaProgressionFixedThr_noIso[j]->Fill(in_offlineTauEta);
          R2_nvtxProgressionFixedThr_noIso[j]->Fill(Nvtx);
          if (in_offlineTauEta<1.305){ R2_barrel_nvtxProgressionFixedThr_noIso[j]->Fill(Nvtx); }
          if (in_offlineTauEta>1.479){ R2_endcap_nvtxProgressionFixedThr_noIso[j]->Fill(Nvtx); }
        }
      }
      if(in_l1tPt>=thr && in_l1tIso>0)
      { 
        R2_ptProgressionFixedThr_Iso[j]->Fill(in_offlineTauPt);
        if (in_offlineTauEta<1.305) { R2_barrel_ptProgressionFixedThr_Iso[j]->Fill(in_offlineTauPt); }
        if (in_offlineTauEta>1.479) { R2_endcap_ptProgressionFixedThr_Iso[j]->Fill(in_offlineTauPt); }
        if (in_offlineTauPt>40.)
        {
          R2_etaProgressionFixedThr_Iso[j]->Fill(in_offlineTauEta);
          R2_nvtxProgressionFixedThr_Iso[j]->Fill(Nvtx);
          if (in_offlineTauEta<1.305){ R2_barrel_nvtxProgressionFixedThr_Iso[j]->Fill(Nvtx); }
          if (in_offlineTauEta>1.479){ R2_endcap_nvtxProgressionFixedThr_Iso[j]->Fill(Nvtx); }
        }
      }
    }
  }

  std::vector<TGraphAsymmErrors*> turnOnsFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> turnOnsFixedThr_Iso = {};
  std::vector<TGraphAsymmErrors*> barrel_turnOnsFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> barrel_turnOnsFixedThr_Iso = {};
  std::vector<TGraphAsymmErrors*> endcap_turnOnsFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> endcap_turnOnsFixedThr_Iso = {};
  std::vector<TGraphAsymmErrors*> etaEffFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> etaEffFixedThr_Iso = {};
  std::vector<TGraphAsymmErrors*> nvtxEffFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> nvtxEffFixedThr_Iso = {};
  std::vector<TGraphAsymmErrors*> barrel_nvtxEffFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> barrel_nvtxEffFixedThr_Iso = {};
  std::vector<TGraphAsymmErrors*> endcap_nvtxEffFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> endcap_nvtxEffFixedThr_Iso = {};

  std::vector<TGraphAsymmErrors*> R2_turnOnsFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> R2_turnOnsFixedThr_Iso = {};
  std::vector<TGraphAsymmErrors*> R2_barrel_turnOnsFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> R2_barrel_turnOnsFixedThr_Iso = {};
  std::vector<TGraphAsymmErrors*> R2_endcap_turnOnsFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> R2_endcap_turnOnsFixedThr_Iso = {};
  std::vector<TGraphAsymmErrors*> R2_etaEffFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> R2_etaEffFixedThr_Iso = {};
  std::vector<TGraphAsymmErrors*> R2_nvtxEffFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> R2_nvtxEffFixedThr_Iso = {};
  std::vector<TGraphAsymmErrors*> R2_barrel_nvtxEffFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> R2_barrel_nvtxEffFixedThr_Iso = {};
  std::vector<TGraphAsymmErrors*> R2_endcap_nvtxEffFixedThr_noIso = {};
  std::vector<TGraphAsymmErrors*> R2_endcap_nvtxEffFixedThr_Iso = {};

  for(long unsigned int i = 0; i < thrs.size(); ++i)
  {
    turnOnsFixedThr_noIso.push_back(new TGraphAsymmErrors(ptProgressionFixedThr_noIso[i], pt, "cp"));
    turnOnsFixedThr_Iso.push_back(new TGraphAsymmErrors(ptProgressionFixedThr_Iso[i], pt, "cp"));

    barrel_turnOnsFixedThr_noIso.push_back(new TGraphAsymmErrors(barrel_ptProgressionFixedThr_noIso[i], barrel_pt, "cp"));
    barrel_turnOnsFixedThr_Iso.push_back(new TGraphAsymmErrors(barrel_ptProgressionFixedThr_Iso[i], barrel_pt, "cp"));

    endcap_turnOnsFixedThr_noIso.push_back(new TGraphAsymmErrors(endcap_ptProgressionFixedThr_noIso[i], endcap_pt, "cp"));
    endcap_turnOnsFixedThr_Iso.push_back(new TGraphAsymmErrors(endcap_ptProgressionFixedThr_Iso[i], endcap_pt, "cp"));

    etaEffFixedThr_noIso.push_back(new TGraphAsymmErrors(etaProgressionFixedThr_noIso[i], eta, "cp"));
    etaEffFixedThr_Iso.push_back(new TGraphAsymmErrors(etaProgressionFixedThr_Iso[i], eta, "cp"));

    nvtxEffFixedThr_noIso.push_back(new TGraphAsymmErrors(nvtxProgressionFixedThr_noIso[i], nvtx, "cp"));
    nvtxEffFixedThr_Iso.push_back(new TGraphAsymmErrors(nvtxProgressionFixedThr_Iso[i], nvtx, "cp"));

    barrel_nvtxEffFixedThr_noIso.push_back(new TGraphAsymmErrors(barrel_nvtxProgressionFixedThr_noIso[i], barrel_nvtx, "cp"));
    barrel_nvtxEffFixedThr_Iso.push_back(new TGraphAsymmErrors(barrel_nvtxProgressionFixedThr_Iso[i], barrel_nvtx, "cp"));

    endcap_nvtxEffFixedThr_noIso.push_back(new TGraphAsymmErrors(endcap_nvtxProgressionFixedThr_noIso[i], endcap_nvtx, "cp"));
    endcap_nvtxEffFixedThr_Iso.push_back(new TGraphAsymmErrors(endcap_nvtxProgressionFixedThr_Iso[i], endcap_nvtx, "cp"));

    R2_turnOnsFixedThr_noIso.push_back(new TGraphAsymmErrors(R2_ptProgressionFixedThr_noIso[i], R2_pt, "cp"));
    R2_turnOnsFixedThr_Iso.push_back(new TGraphAsymmErrors(R2_ptProgressionFixedThr_Iso[i], R2_pt, "cp"));

    R2_barrel_turnOnsFixedThr_noIso.push_back(new TGraphAsymmErrors(R2_barrel_ptProgressionFixedThr_noIso[i], R2_barrel_pt, "cp"));
    R2_barrel_turnOnsFixedThr_Iso.push_back(new TGraphAsymmErrors(R2_barrel_ptProgressionFixedThr_Iso[i], R2_barrel_pt, "cp"));

    R2_endcap_turnOnsFixedThr_noIso.push_back(new TGraphAsymmErrors(R2_endcap_ptProgressionFixedThr_noIso[i], R2_endcap_pt, "cp"));
    R2_endcap_turnOnsFixedThr_Iso.push_back(new TGraphAsymmErrors(R2_endcap_ptProgressionFixedThr_Iso[i], R2_endcap_pt, "cp"));

    R2_etaEffFixedThr_noIso.push_back(new TGraphAsymmErrors(R2_etaProgressionFixedThr_noIso[i], R2_eta, "cp"));
    R2_etaEffFixedThr_Iso.push_back(new TGraphAsymmErrors(R2_etaProgressionFixedThr_Iso[i], R2_eta, "cp"));

    R2_nvtxEffFixedThr_noIso.push_back(new TGraphAsymmErrors(R2_nvtxProgressionFixedThr_noIso[i], R2_nvtx, "cp"));
    R2_nvtxEffFixedThr_Iso.push_back(new TGraphAsymmErrors(R2_nvtxProgressionFixedThr_Iso[i], R2_nvtx, "cp"));

    R2_barrel_nvtxEffFixedThr_noIso.push_back(new TGraphAsymmErrors(R2_barrel_nvtxProgressionFixedThr_noIso[i], R2_barrel_nvtx, "cp"));
    R2_barrel_nvtxEffFixedThr_Iso.push_back(new TGraphAsymmErrors(R2_barrel_nvtxProgressionFixedThr_Iso[i], R2_barrel_nvtx, "cp"));

    R2_endcap_nvtxEffFixedThr_noIso.push_back(new TGraphAsymmErrors(R2_endcap_nvtxProgressionFixedThr_noIso[i], R2_endcap_nvtx, "cp"));
    R2_endcap_nvtxEffFixedThr_Iso.push_back(new TGraphAsymmErrors(R2_endcap_nvtxProgressionFixedThr_Iso[i], R2_endcap_nvtx, "cp"));
  }

  // ----------------------------------------------------------------------------    
  // save in root file for future necessity
  TFile* fileout = new TFile("ROOTs/efficiencies_of_Run"+run_nmbr_str+".root","RECREATE");
  for(long unsigned int i = 0; i < thrs.size(); ++i)
  {
    pt->Write();
    barrel_pt->Write();
    endcap_pt->Write();
    eta->Write();
    nvtx->Write();

    ptProgressionFixedThr_noIso[i]->Write();
    ptProgressionFixedThr_Iso[i]->Write();
    turnOnsFixedThr_noIso[i]->Write();
    turnOnsFixedThr_Iso[i]->Write();

    barrel_ptProgressionFixedThr_noIso[i]->Write();
    barrel_ptProgressionFixedThr_Iso[i]->Write();
    barrel_turnOnsFixedThr_noIso[i]->Write();
    barrel_turnOnsFixedThr_Iso[i]->Write();
    
    endcap_ptProgressionFixedThr_noIso[i]->Write();
    endcap_ptProgressionFixedThr_Iso[i]->Write();
    endcap_turnOnsFixedThr_noIso[i]->Write();
    endcap_turnOnsFixedThr_Iso[i]->Write();
    
    etaProgressionFixedThr_noIso[i]->Write();
    etaProgressionFixedThr_Iso[i]->Write();
    etaEffFixedThr_noIso[i]->Write();
    etaEffFixedThr_Iso[i]->Write();

    nvtxProgressionFixedThr_noIso[i]->Write();
    nvtxProgressionFixedThr_Iso[i]->Write();
    nvtxEffFixedThr_noIso[i]->Write();
    nvtxEffFixedThr_Iso[i]->Write();

    barrel_nvtxProgressionFixedThr_noIso[i]->Write();
    barrel_nvtxProgressionFixedThr_Iso[i]->Write();
    barrel_nvtxEffFixedThr_noIso[i]->Write();
    barrel_nvtxEffFixedThr_Iso[i]->Write();

    endcap_nvtxProgressionFixedThr_noIso[i]->Write();
    endcap_nvtxProgressionFixedThr_Iso[i]->Write();
    endcap_nvtxEffFixedThr_noIso[i]->Write();
    endcap_nvtxEffFixedThr_Iso[i]->Write();

    R2_pt->Write();
    R2_barrel_pt->Write();
    R2_endcap_pt->Write();
    R2_eta->Write();
    R2_nvtx->Write();

    R2_ptProgressionFixedThr_noIso[i]->Write();
    R2_ptProgressionFixedThr_Iso[i]->Write();
    R2_turnOnsFixedThr_noIso[i]->Write();
    R2_turnOnsFixedThr_Iso[i]->Write();

    R2_barrel_ptProgressionFixedThr_noIso[i]->Write();
    R2_barrel_ptProgressionFixedThr_Iso[i]->Write();
    R2_barrel_turnOnsFixedThr_noIso[i]->Write();
    R2_barrel_turnOnsFixedThr_Iso[i]->Write();
    
    R2_endcap_ptProgressionFixedThr_noIso[i]->Write();
    R2_endcap_ptProgressionFixedThr_Iso[i]->Write();
    R2_endcap_turnOnsFixedThr_noIso[i]->Write();
    R2_endcap_turnOnsFixedThr_Iso[i]->Write();
    
    R2_etaProgressionFixedThr_noIso[i]->Write();
    R2_etaProgressionFixedThr_Iso[i]->Write();
    R2_etaEffFixedThr_noIso[i]->Write();
    R2_etaEffFixedThr_Iso[i]->Write();

    R2_nvtxProgressionFixedThr_noIso[i]->Write();
    R2_nvtxProgressionFixedThr_Iso[i]->Write();
    R2_nvtxEffFixedThr_noIso[i]->Write();
    R2_nvtxEffFixedThr_Iso[i]->Write();

    R2_barrel_nvtxProgressionFixedThr_noIso[i]->Write();
    R2_barrel_nvtxProgressionFixedThr_Iso[i]->Write();
    R2_barrel_nvtxEffFixedThr_noIso[i]->Write();
    R2_barrel_nvtxEffFixedThr_Iso[i]->Write();

    R2_endcap_nvtxProgressionFixedThr_noIso[i]->Write();
    R2_endcap_nvtxProgressionFixedThr_Iso[i]->Write();
    R2_endcap_nvtxEffFixedThr_noIso[i]->Write();
    R2_endcap_nvtxEffFixedThr_Iso[i]->Write();
  }

  // ----------------------------------------------------------------------------
  // plot performance

  gStyle->SetOptStat(000000);

  for(long unsigned int i = 0; i < thrs.size(); ++i)
  {
    TString thr ;
    thr.Form("%.0f", ceil(thrs[i]));

    // TURNONS
    TCanvas canvas("c","c",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas.SetGrid();
    // canvas.SetLogy();

    turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitle("p_{T}^{Offline #tau} [GeV]");
    turnOnsFixedThr_noIso[i]->SetTitle("");
    turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    turnOnsFixedThr_noIso[i]->SetTitle("");
    turnOnsFixedThr_noIso[i]->GetXaxis()->SetRangeUser(20.,500.);
    turnOnsFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0., 1.05);

    turnOnsFixedThr_noIso[i]->SetLineWidth(2);
    turnOnsFixedThr_noIso[i]->SetLineColor(1);
    turnOnsFixedThr_noIso[i]->SetMarkerStyle(8);
    turnOnsFixedThr_noIso[i]->SetMarkerColor(1);

    turnOnsFixedThr_Iso[i]->SetLineWidth(2);
    turnOnsFixedThr_Iso[i]->SetLineColor(2);
    turnOnsFixedThr_Iso[i]->SetMarkerStyle(8);
    turnOnsFixedThr_Iso[i]->SetMarkerColor(2);

    turnOnsFixedThr_noIso[i]->Draw();
    turnOnsFixedThr_Iso[i]->Draw("same");

    TPaveText* texl1 = new TPaveText(0.15,0.87,0.2,0.99,"NDC");
    texl1->AddText("CMS Internal");
    texl1->SetTextSize(0.03);
    texl1->SetFillStyle(0);
    texl1->SetBorderSize(0);
    texl1->Draw("same");

    TPaveText* texl2 = new TPaveText(0.60,0.87,0.89,0.99,"NDC");
    if (run_nmbr != -1) { texl2->AddText("Run "+run_nmbr_str+" (13.6 TeV)"); }
    else                { texl2->AddText("Runs 2022 (13.6 TeV)"); }
    texl2->SetTextSize(0.03);
    texl2->SetFillStyle(0);
    texl2->SetBorderSize(0);
    texl2->Draw("same");

    TLegend legend(0.55,0.15,0.88,0.28);
    legend.SetBorderSize(0);
    legend.SetHeader("p_{T}^{L1 #tau} > "+thr+" GeV");
    legend.AddEntry(turnOnsFixedThr_noIso[i],"Run-3 No-Iso","LPE");
    legend.AddEntry(turnOnsFixedThr_Iso[i],"Run-3 Iso","LPE");
    legend.Draw("same");

    canvas.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/turnOnsAt"+thr+".pdf");


    // BARREL ONLY TURNONS
    TCanvas canvas1("c1","c1",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas1.SetGrid();
    // canvas.SetLogy();

    barrel_turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitle("p_{T}^{Offline #tau} [GeV]");
    barrel_turnOnsFixedThr_noIso[i]->SetTitle("");
    barrel_turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    barrel_turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    barrel_turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    barrel_turnOnsFixedThr_noIso[i]->SetTitle("");
    barrel_turnOnsFixedThr_noIso[i]->GetXaxis()->SetRangeUser(20.,500.);
    barrel_turnOnsFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0., 1.05);

    barrel_turnOnsFixedThr_noIso[i]->SetLineWidth(2);
    barrel_turnOnsFixedThr_noIso[i]->SetLineColor(1);
    barrel_turnOnsFixedThr_noIso[i]->SetMarkerStyle(8);
    barrel_turnOnsFixedThr_noIso[i]->SetMarkerColor(1);

    barrel_turnOnsFixedThr_Iso[i]->SetLineWidth(2);
    barrel_turnOnsFixedThr_Iso[i]->SetLineColor(2);
    barrel_turnOnsFixedThr_Iso[i]->SetMarkerStyle(8);
    barrel_turnOnsFixedThr_Iso[i]->SetMarkerColor(2);

    barrel_turnOnsFixedThr_noIso[i]->Draw();
    barrel_turnOnsFixedThr_Iso[i]->Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");
    legend.Draw("same");

    canvas1.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/barrel_turnOnsAt"+thr+".pdf");


    // ENDCAP ONLY TURNONS
    TCanvas canvas2("c2","c2",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas2.SetGrid();
    // canvas.SetLogy();

    endcap_turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitle("p_{T}^{Offline #tau} [GeV]");
    endcap_turnOnsFixedThr_noIso[i]->SetTitle("");
    endcap_turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    endcap_turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    endcap_turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    endcap_turnOnsFixedThr_noIso[i]->SetTitle("");
    endcap_turnOnsFixedThr_noIso[i]->GetXaxis()->SetRangeUser(20.,500.);
    endcap_turnOnsFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0., 1.05);

    endcap_turnOnsFixedThr_noIso[i]->SetLineWidth(2);
    endcap_turnOnsFixedThr_noIso[i]->SetLineColor(1);
    endcap_turnOnsFixedThr_noIso[i]->SetMarkerStyle(8);
    endcap_turnOnsFixedThr_noIso[i]->SetMarkerColor(1);

    endcap_turnOnsFixedThr_Iso[i]->SetLineWidth(2);
    endcap_turnOnsFixedThr_Iso[i]->SetLineColor(2);
    endcap_turnOnsFixedThr_Iso[i]->SetMarkerStyle(8);
    endcap_turnOnsFixedThr_Iso[i]->SetMarkerColor(2);

    endcap_turnOnsFixedThr_noIso[i]->Draw();
    endcap_turnOnsFixedThr_Iso[i]->Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");
    legend.Draw("same");

    canvas2.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/endcap_turnOnsAt"+thr+".pdf");


    // ETA EFFICIENCY
    TCanvas canvas3("c3","c3",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas3.SetGrid();
    // canvas.SetLogy();

    etaEffFixedThr_noIso[i]->GetXaxis()->SetTitle("#eta^{Offline #tau}");
    etaEffFixedThr_noIso[i]->SetTitle("");
    etaEffFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    etaEffFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    etaEffFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    etaEffFixedThr_noIso[i]->SetTitle("");
    // etaEffFixedThr_noIso[i]->GetXaxis()->SetRangeUser(20.,500.);
    etaEffFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0., 1.05);

    etaEffFixedThr_noIso[i]->SetLineWidth(2);
    etaEffFixedThr_noIso[i]->SetLineColor(1);
    etaEffFixedThr_noIso[i]->SetMarkerStyle(8);
    etaEffFixedThr_noIso[i]->SetMarkerColor(1);

    etaEffFixedThr_Iso[i]->SetLineWidth(2);
    etaEffFixedThr_Iso[i]->SetLineColor(2);
    etaEffFixedThr_Iso[i]->SetMarkerStyle(8);
    etaEffFixedThr_Iso[i]->SetMarkerColor(2);

    etaEffFixedThr_noIso[i]->Draw();
    etaEffFixedThr_Iso[i]->Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");

    TLegend legendB(0.55,0.15,0.88,0.28);
    legendB.SetBorderSize(0);
    legendB.SetHeader("p_{T}^{L1 #tau} > "+thr+" GeV ; p_{T}^{Offline #tau} > 40 GeV");
    legendB.AddEntry(turnOnsFixedThr_noIso[i],"Run-3 No-Iso","LPE");
    legendB.AddEntry(turnOnsFixedThr_Iso[i],"Run-3 Iso","LPE");
    legendB.Draw("same");

    canvas3.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/etaEffAt"+thr+".pdf");


    TCanvas canvas4("c4","c4",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas4.SetGrid();
    // canvas.SetLogy();

    R2_etaEffFixedThr_noIso[i]->GetXaxis()->SetTitle("#eta^{Offline #tau}");
    R2_etaEffFixedThr_noIso[i]->SetTitle("");
    R2_etaEffFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    R2_etaEffFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    R2_etaEffFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    R2_etaEffFixedThr_noIso[i]->SetTitle("");
    // R2_etaEffFixedThr_noIso[i]->GetXaxis()->SetRangeUser(20.,500.);
    R2_etaEffFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0., 1.05);

    R2_etaEffFixedThr_noIso[i]->SetLineWidth(2);
    R2_etaEffFixedThr_noIso[i]->SetLineColor(13);
    R2_etaEffFixedThr_noIso[i]->SetMarkerStyle(8);
    R2_etaEffFixedThr_noIso[i]->SetMarkerColor(13); 

    R2_etaEffFixedThr_Iso[i]->SetLineWidth(2);
    R2_etaEffFixedThr_Iso[i]->SetLineColor(46);
    R2_etaEffFixedThr_Iso[i]->SetMarkerStyle(8);
    R2_etaEffFixedThr_Iso[i]->SetMarkerColor(46);

    R2_etaEffFixedThr_noIso[i]->Draw();
    R2_etaEffFixedThr_Iso[i]->Draw("same");
    
    legend.AddEntry(R2_etaEffFixedThr_noIso[i],"Run-2 No-Iso","LPE");
    legend.AddEntry(R2_etaEffFixedThr_Iso[i],"Run-2 Iso","LPE");

    texl1->Draw("same");

    TPaveText* texlA = new TPaveText(0.60,0.87,0.89,0.99,"NDC");
    texlA->AddText("Runs 2018 (13 TeV)");
    texlA->SetTextSize(0.03);
    texlA->SetFillStyle(0);
    texlA->SetBorderSize(0);
    texlA->Draw("same");

    TLegend legendA(0.55,0.15,0.88,0.28);
    legendA.SetBorderSize(0);
    legendA.SetHeader("p_{T}^{L1 #tau} > "+thr+" GeV ; p_{T}^{Offline #tau} > 40 GeV");
    legendA.AddEntry(R2_etaEffFixedThr_noIso[i],"Run-2 No-Iso","LPE");
    legendA.AddEntry(R2_etaEffFixedThr_Iso[i],"Run-2 Iso","LPE");
    legendA.Draw("same");

    canvas4.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/etaEffAt"+thr+"_Run2.pdf");















    // NVTX EFFICIENCY
    TCanvas canvas5("c5","c5",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas5.SetGrid();
    // canvas.SetLogy();

    nvtxEffFixedThr_noIso[i]->GetXaxis()->SetTitle("Number of vertices");
    nvtxEffFixedThr_noIso[i]->SetTitle("");
    nvtxEffFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    nvtxEffFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    nvtxEffFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    nvtxEffFixedThr_noIso[i]->SetTitle("");
    nvtxEffFixedThr_noIso[i]->GetXaxis()->SetRangeUser(0.,60.);
    nvtxEffFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0., 1.05);

    nvtxEffFixedThr_noIso[i]->SetLineWidth(2);
    nvtxEffFixedThr_noIso[i]->SetLineColor(1);
    nvtxEffFixedThr_noIso[i]->SetMarkerStyle(8);
    nvtxEffFixedThr_noIso[i]->SetMarkerColor(1);

    nvtxEffFixedThr_Iso[i]->SetLineWidth(2);
    nvtxEffFixedThr_Iso[i]->SetLineColor(2);
    nvtxEffFixedThr_Iso[i]->SetMarkerStyle(8);
    nvtxEffFixedThr_Iso[i]->SetMarkerColor(2);

    nvtxEffFixedThr_noIso[i]->Draw();
    nvtxEffFixedThr_Iso[i]->Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");
    legendB.Draw("same");

    canvas5.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/nvtxEffAt"+thr+".pdf");


    TCanvas canvas6("c6","c6",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas6.SetGrid();
    // canvas.SetLogy();

    R2_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetTitle("Number of vertices");
    R2_nvtxEffFixedThr_noIso[i]->SetTitle("");
    R2_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    R2_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    R2_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    R2_nvtxEffFixedThr_noIso[i]->SetTitle("");
    R2_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetRangeUser(0.,60.);
    R2_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0., 1.05);

    R2_nvtxEffFixedThr_noIso[i]->SetLineWidth(2);
    R2_nvtxEffFixedThr_noIso[i]->SetLineColor(13);
    R2_nvtxEffFixedThr_noIso[i]->SetMarkerStyle(8);
    R2_nvtxEffFixedThr_noIso[i]->SetMarkerColor(13); 

    R2_nvtxEffFixedThr_Iso[i]->SetLineWidth(2);
    R2_nvtxEffFixedThr_Iso[i]->SetLineColor(46);
    R2_nvtxEffFixedThr_Iso[i]->SetMarkerStyle(8);
    R2_nvtxEffFixedThr_Iso[i]->SetMarkerColor(46);

    R2_nvtxEffFixedThr_noIso[i]->Draw();
    R2_nvtxEffFixedThr_Iso[i]->Draw("same");

    texl1->Draw("same");
    texlA->Draw("same");
    legendA.Draw("same");

    canvas6.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/nvtxEffAt"+thr+"_Run2.pdf");

    TCanvas canvas7("c7","c7",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas7.SetGrid();
    // canvas.SetLogy();

    barrel_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetTitle("Number of vertices");
    barrel_nvtxEffFixedThr_noIso[i]->SetTitle("");
    barrel_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    barrel_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    barrel_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    barrel_nvtxEffFixedThr_noIso[i]->SetTitle("");
    barrel_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetRangeUser(0.,60.);
    barrel_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0., 1.05);

    barrel_nvtxEffFixedThr_noIso[i]->SetLineWidth(2);
    barrel_nvtxEffFixedThr_noIso[i]->SetLineColor(1);
    barrel_nvtxEffFixedThr_noIso[i]->SetMarkerStyle(8);
    barrel_nvtxEffFixedThr_noIso[i]->SetMarkerColor(1);

    barrel_nvtxEffFixedThr_Iso[i]->SetLineWidth(2);
    barrel_nvtxEffFixedThr_Iso[i]->SetLineColor(2);
    barrel_nvtxEffFixedThr_Iso[i]->SetMarkerStyle(8);
    barrel_nvtxEffFixedThr_Iso[i]->SetMarkerColor(2);

    barrel_nvtxEffFixedThr_noIso[i]->Draw();
    barrel_nvtxEffFixedThr_Iso[i]->Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");
    legendB.Draw("same");

    canvas7.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/barrel_nvtxEffAt"+thr+".pdf");


    TCanvas canvas8("c8","c8",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas8.SetGrid();
    // canvas.SetLogy();

    R2_barrel_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetTitle("Number of vertices");
    R2_barrel_nvtxEffFixedThr_noIso[i]->SetTitle("");
    R2_barrel_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    R2_barrel_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    R2_barrel_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    R2_barrel_nvtxEffFixedThr_noIso[i]->SetTitle("");
    R2_barrel_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetRangeUser(0.,60.);
    R2_barrel_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0., 1.05);

    R2_barrel_nvtxEffFixedThr_noIso[i]->SetLineWidth(2);
    R2_barrel_nvtxEffFixedThr_noIso[i]->SetLineColor(13);
    R2_barrel_nvtxEffFixedThr_noIso[i]->SetMarkerStyle(8);
    R2_barrel_nvtxEffFixedThr_noIso[i]->SetMarkerColor(13); 

    R2_barrel_nvtxEffFixedThr_Iso[i]->SetLineWidth(2);
    R2_barrel_nvtxEffFixedThr_Iso[i]->SetLineColor(46);
    R2_barrel_nvtxEffFixedThr_Iso[i]->SetMarkerStyle(8);
    R2_barrel_nvtxEffFixedThr_Iso[i]->SetMarkerColor(46);

    R2_barrel_nvtxEffFixedThr_noIso[i]->Draw();
    R2_barrel_nvtxEffFixedThr_Iso[i]->Draw("same");

    texl1->Draw("same");
    texlA->Draw("same");
    legendA.Draw("same");

    canvas8.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/barrel_nvtxEffAt"+thr+"_Run2.pdf");

    TCanvas canvas9("c9","c9",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas9.SetGrid();
    // canvas.SetLogy();

    endcap_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetTitle("Number of vertices");
    endcap_nvtxEffFixedThr_noIso[i]->SetTitle("");
    endcap_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    endcap_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    endcap_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    endcap_nvtxEffFixedThr_noIso[i]->SetTitle("");
    endcap_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetRangeUser(0.,60.);
    endcap_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0., 1.05);

    endcap_nvtxEffFixedThr_noIso[i]->SetLineWidth(2);
    endcap_nvtxEffFixedThr_noIso[i]->SetLineColor(1);
    endcap_nvtxEffFixedThr_noIso[i]->SetMarkerStyle(8);
    endcap_nvtxEffFixedThr_noIso[i]->SetMarkerColor(1);

    endcap_nvtxEffFixedThr_Iso[i]->SetLineWidth(2);
    endcap_nvtxEffFixedThr_Iso[i]->SetLineColor(2);
    endcap_nvtxEffFixedThr_Iso[i]->SetMarkerStyle(8);
    endcap_nvtxEffFixedThr_Iso[i]->SetMarkerColor(2);

    endcap_nvtxEffFixedThr_noIso[i]->Draw();
    endcap_nvtxEffFixedThr_Iso[i]->Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");
    legendB.Draw("same");

    canvas9.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/endcap_nvtxEffAt"+thr+".pdf");


    TCanvas canvas10("c10","c10",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas10.SetGrid();
    // canvas.SetLogy();

    R2_endcap_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetTitle("Number of vertices");
    R2_endcap_nvtxEffFixedThr_noIso[i]->SetTitle("");
    R2_endcap_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    R2_endcap_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    R2_endcap_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    R2_endcap_nvtxEffFixedThr_noIso[i]->SetTitle("");
    R2_endcap_nvtxEffFixedThr_noIso[i]->GetXaxis()->SetRangeUser(0.,60.);
    R2_endcap_nvtxEffFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0., 1.05);

    R2_endcap_nvtxEffFixedThr_noIso[i]->SetLineWidth(2);
    R2_endcap_nvtxEffFixedThr_noIso[i]->SetLineColor(13);
    R2_endcap_nvtxEffFixedThr_noIso[i]->SetMarkerStyle(8);
    R2_endcap_nvtxEffFixedThr_noIso[i]->SetMarkerColor(13); 

    R2_endcap_nvtxEffFixedThr_Iso[i]->SetLineWidth(2);
    R2_endcap_nvtxEffFixedThr_Iso[i]->SetLineColor(46);
    R2_endcap_nvtxEffFixedThr_Iso[i]->SetMarkerStyle(8);
    R2_endcap_nvtxEffFixedThr_Iso[i]->SetMarkerColor(46);

    R2_endcap_nvtxEffFixedThr_noIso[i]->Draw();
    R2_endcap_nvtxEffFixedThr_Iso[i]->Draw("same");

    texl1->Draw("same");
    texlA->Draw("same");
    legendA.Draw("same");

    canvas10.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/endcap_nvtxEffAt"+thr+"_Run2.pdf");






































  }

  for(long unsigned int i = 0; i < thrs.size(); ++i)
  {
    TString thr ;
    thr.Form("%.0f", ceil(thrs[i]));

    // TURNONS
    TCanvas canvas("c","c",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas.SetGrid();
    // canvas.SetLogy();

    turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitle("p_{T}^{Offline #tau} [GeV]");
    turnOnsFixedThr_noIso[i]->SetTitle("");
    turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    turnOnsFixedThr_noIso[i]->SetTitle("");
    turnOnsFixedThr_noIso[i]->GetXaxis()->SetRangeUser(20.,100.);
    turnOnsFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0., 1.05);

    turnOnsFixedThr_noIso[i]->SetLineWidth(2);
    turnOnsFixedThr_noIso[i]->SetLineColor(1);
    turnOnsFixedThr_noIso[i]->SetMarkerStyle(8);
    turnOnsFixedThr_noIso[i]->SetMarkerColor(1);

    turnOnsFixedThr_Iso[i]->SetLineWidth(2);
    turnOnsFixedThr_Iso[i]->SetLineColor(2);
    turnOnsFixedThr_Iso[i]->SetMarkerStyle(8);
    turnOnsFixedThr_Iso[i]->SetMarkerColor(2);

    R2_turnOnsFixedThr_noIso[i]->SetLineWidth(2);
    R2_turnOnsFixedThr_noIso[i]->SetLineColor(13);
    R2_turnOnsFixedThr_noIso[i]->SetMarkerStyle(8);
    R2_turnOnsFixedThr_noIso[i]->SetMarkerColor(13);

    R2_turnOnsFixedThr_Iso[i]->SetLineWidth(2);
    R2_turnOnsFixedThr_Iso[i]->SetLineColor(46);
    R2_turnOnsFixedThr_Iso[i]->SetMarkerStyle(8);
    R2_turnOnsFixedThr_Iso[i]->SetMarkerColor(46);

    turnOnsFixedThr_noIso[i]->Draw();
    turnOnsFixedThr_Iso[i]->Draw("same");
    R2_turnOnsFixedThr_noIso[i]->Draw("same");
    R2_turnOnsFixedThr_Iso[i]->Draw("same");

    TPaveText* texl1 = new TPaveText(0.15,0.87,0.2,0.99,"NDC");
    texl1->AddText("CMS Internal");
    texl1->SetTextSize(0.03);
    texl1->SetFillStyle(0);
    texl1->SetBorderSize(0);
    texl1->Draw("same");

    TPaveText* texl2 = new TPaveText(0.60,0.87,0.89,0.99,"NDC");
    if (run_nmbr != -1) { texl2->AddText("Run "+run_nmbr_str+" (13.6 TeV)"); }
    else                { texl2->AddText("Runs 2022 (13.6 TeV)"); }
    texl2->SetTextSize(0.03);
    texl2->SetFillStyle(0);
    texl2->SetBorderSize(0);
    texl2->Draw("same");

    TLegend legend(0.55,0.15,0.88,0.28);
    legend.SetBorderSize(0);
    legend.SetHeader("p_{T}^{L1 #tau} > "+thr+" GeV");
    legend.AddEntry(turnOnsFixedThr_noIso[i],"Run-3 No-Iso","LPE");
    legend.AddEntry(turnOnsFixedThr_Iso[i],"Run-3 Iso","LPE");
    legend.AddEntry(R2_turnOnsFixedThr_noIso[i],"Run-2 No-Iso","LPE");
    legend.AddEntry(R2_turnOnsFixedThr_Iso[i],"Run-2 Iso","LPE");
    legend.Draw("same");

    canvas.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/zoomed_turnOnsAt"+thr+".pdf");


    // TURNONS
    TCanvas canvas_("c_","c_",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas_.SetGrid();
    // canvas.SetLogy();

    turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitle("p_{T}^{Offline #tau} [GeV]");
    turnOnsFixedThr_noIso[i]->SetTitle("");
    turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    turnOnsFixedThr_noIso[i]->SetTitle("");
    turnOnsFixedThr_noIso[i]->GetXaxis()->SetRangeUser(20.,100.);
    turnOnsFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0., 1.05);

    turnOnsFixedThr_noIso[i]->SetLineWidth(2);
    turnOnsFixedThr_noIso[i]->SetLineColor(1);
    turnOnsFixedThr_noIso[i]->SetMarkerStyle(8);
    turnOnsFixedThr_noIso[i]->SetMarkerColor(1);

    turnOnsFixedThr_Iso[i]->SetLineWidth(2);
    turnOnsFixedThr_Iso[i]->SetLineColor(2);
    turnOnsFixedThr_Iso[i]->SetMarkerStyle(8);
    turnOnsFixedThr_Iso[i]->SetMarkerColor(2);

    turnOnsFixedThr_noIso[i]->Draw();
    turnOnsFixedThr_Iso[i]->Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");

    TLegend legend_(0.55,0.15,0.88,0.28);
    legend_.SetBorderSize(0);
    legend_.SetHeader("p_{T}^{L1 #tau} > "+thr+" GeV");
    legend_.AddEntry(turnOnsFixedThr_noIso[i],"Run-3 No-Iso","LPE");
    legend_.AddEntry(turnOnsFixedThr_Iso[i],"Run-3 Iso","LPE");
    legend_.Draw("same");

    canvas_.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/zoomed_turnOnsAt"+thr+"_Run3Only.pdf");


    // BARREL ONLY TURNONS
    TCanvas canvas1("c1","c1",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas1.SetGrid();
    // canvas.SetLogy();

    barrel_turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitle("p_{T}^{Offline #tau} [GeV]");
    barrel_turnOnsFixedThr_noIso[i]->SetTitle("");
    barrel_turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    barrel_turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    barrel_turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    barrel_turnOnsFixedThr_noIso[i]->SetTitle("");
    barrel_turnOnsFixedThr_noIso[i]->GetXaxis()->SetRangeUser(20.,100.);
    barrel_turnOnsFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0.,1.05);

    barrel_turnOnsFixedThr_noIso[i]->SetLineWidth(2);
    barrel_turnOnsFixedThr_noIso[i]->SetLineColor(1);
    barrel_turnOnsFixedThr_noIso[i]->SetMarkerStyle(8);
    barrel_turnOnsFixedThr_noIso[i]->SetMarkerColor(1);

    barrel_turnOnsFixedThr_Iso[i]->SetLineWidth(2);
    barrel_turnOnsFixedThr_Iso[i]->SetLineColor(2);
    barrel_turnOnsFixedThr_Iso[i]->SetMarkerStyle(8);
    barrel_turnOnsFixedThr_Iso[i]->SetMarkerColor(2);

    R2_barrel_turnOnsFixedThr_noIso[i]->SetLineWidth(2);
    R2_barrel_turnOnsFixedThr_noIso[i]->SetLineColor(13);
    R2_barrel_turnOnsFixedThr_noIso[i]->SetMarkerStyle(8);
    R2_barrel_turnOnsFixedThr_noIso[i]->SetMarkerColor(13);

    R2_barrel_turnOnsFixedThr_Iso[i]->SetLineWidth(2);
    R2_barrel_turnOnsFixedThr_Iso[i]->SetLineColor(46);
    R2_barrel_turnOnsFixedThr_Iso[i]->SetMarkerStyle(8);
    R2_barrel_turnOnsFixedThr_Iso[i]->SetMarkerColor(46);

    barrel_turnOnsFixedThr_noIso[i]->Draw();
    barrel_turnOnsFixedThr_Iso[i]->Draw("same");
    R2_barrel_turnOnsFixedThr_noIso[i]->Draw("same");
    R2_barrel_turnOnsFixedThr_Iso[i]->Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");
    legend.Draw("same");

    canvas1.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/zoomed_barrel_turnOnsAt"+thr+".pdf");


    // ENDCAP ONLY TURNONS
    TCanvas canvas2("c2","c2",800,800);
    // canvas.SetLeftMargin(0.15);
    canvas2.SetGrid();
    // canvas.SetLogy();

    endcap_turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitle("p_{T}^{Offline #tau} [GeV]");
    endcap_turnOnsFixedThr_noIso[i]->SetTitle("");
    endcap_turnOnsFixedThr_noIso[i]->GetXaxis()->SetTitleOffset(1.3);
    endcap_turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitle("Efficiency");
    endcap_turnOnsFixedThr_noIso[i]->GetYaxis()->SetTitleOffset(1.3);
    endcap_turnOnsFixedThr_noIso[i]->SetTitle("");
    endcap_turnOnsFixedThr_noIso[i]->GetXaxis()->SetRangeUser(20.,100.);
    endcap_turnOnsFixedThr_noIso[i]->GetYaxis()->SetRangeUser(0.,1.05);

    endcap_turnOnsFixedThr_noIso[i]->SetLineWidth(2);
    endcap_turnOnsFixedThr_noIso[i]->SetLineColor(1);
    endcap_turnOnsFixedThr_noIso[i]->SetMarkerStyle(8);
    endcap_turnOnsFixedThr_noIso[i]->SetMarkerColor(1);

    endcap_turnOnsFixedThr_Iso[i]->SetLineWidth(2);
    endcap_turnOnsFixedThr_Iso[i]->SetLineColor(2);
    endcap_turnOnsFixedThr_Iso[i]->SetMarkerStyle(8);
    endcap_turnOnsFixedThr_Iso[i]->SetMarkerColor(2);

    R2_endcap_turnOnsFixedThr_noIso[i]->SetLineWidth(2);
    R2_endcap_turnOnsFixedThr_noIso[i]->SetLineColor(13);
    R2_endcap_turnOnsFixedThr_noIso[i]->SetMarkerStyle(8);
    R2_endcap_turnOnsFixedThr_noIso[i]->SetMarkerColor(13);

    R2_endcap_turnOnsFixedThr_Iso[i]->SetLineWidth(2);
    R2_endcap_turnOnsFixedThr_Iso[i]->SetLineColor(46);
    R2_endcap_turnOnsFixedThr_Iso[i]->SetMarkerStyle(8);
    R2_endcap_turnOnsFixedThr_Iso[i]->SetMarkerColor(46);

    endcap_turnOnsFixedThr_noIso[i]->Draw();
    endcap_turnOnsFixedThr_Iso[i]->Draw("same");
    R2_endcap_turnOnsFixedThr_noIso[i]->Draw("same");
    R2_endcap_turnOnsFixedThr_Iso[i]->Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");
    legend.Draw("same");

    canvas2.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/zoomed_endcap_turnOnsAt"+thr+".pdf");
  }





  // TURNONS NO-ISO PROGRESSION
  TCanvas canvas("c","c",800,800);
  // canvas.SetLeftMargin(0.15);
  canvas.SetGrid();
  // canvas.SetLogy();
  canvas.cd();

  TLegend legend(0.75,0.11,0.89,0.76);
  legend.SetBorderSize(0);
  legend.SetHeader("Tau No-Iso");

  dummy->GetXaxis()->SetTitle("p_{T}^{Offline #tau} [GeV]");
  dummy->SetTitle("");
  dummy->GetXaxis()->SetTitleOffset(1.3);
  dummy->GetYaxis()->SetTitle("Efficiency");
  dummy->GetYaxis()->SetTitleOffset(1.3);
  dummy->SetTitle("");
  dummy->GetXaxis()->SetRangeUser(20.,100.);
  dummy->GetYaxis()->SetRangeUser(0.,1.05);
  dummy->Draw();

  std::vector<int> colors = {1,2,4,5,6,7,8,9,11,12,13,14,15,16,17,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49};

  for(long unsigned int i = 0; i < thrs.size(); ++i)
  {
    TString thr ;
    thr.Form("%.0f", ceil(thrs[i]));

    turnOnsFixedThr_noIso[i]->SetLineWidth(2);
    turnOnsFixedThr_noIso[i]->SetLineColor(colors[i]);
    turnOnsFixedThr_noIso[i]->SetMarkerStyle(8);
    turnOnsFixedThr_noIso[i]->SetMarkerColor(colors[i]);
    turnOnsFixedThr_noIso[i]->Draw("same");
    legend.AddEntry(turnOnsFixedThr_noIso[i],"p_{T}^{L1 #tau} > "+thr+" GeV","LPE");
  }

  TPaveText* texl1 = new TPaveText(0.15,0.87,0.2,0.99,"NDC");
  texl1->AddText("CMS Internal");
  texl1->SetTextSize(0.03);
  texl1->SetFillStyle(0);
  texl1->SetBorderSize(0);
  texl1->Draw("same");

  TPaveText* texl2 = new TPaveText(0.60,0.87,0.89,0.99,"NDC");
  if (run_nmbr != -1) { texl2->AddText("Run "+run_nmbr_str+" (13.6 TeV)"); }
  else                { texl2->AddText("Runs 2022 (13.6 TeV)"); }
  texl2->SetTextSize(0.03);
  texl2->SetFillStyle(0);
  texl2->SetBorderSize(0);
  texl2->Draw("same");

  legend.Draw("same");

  canvas.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/turnOnsProgression_noIso.pdf");


  // TURNONS ISO PROGRESSION
  TCanvas canvas1("c1","c1",800,800);
  // canvas.SetLeftMargin(0.15);
  canvas1.SetGrid();
  // canvas.SetLogy();
  canvas1.cd();

  TLegend legendX(0.75,0.11,0.89,0.76);
  legendX.SetBorderSize(0);
  legendX.SetHeader("Tau Iso");

  dummy->Draw();

  for(long unsigned int i = 0; i < thrs.size(); ++i)
  {
    TString thr ;
    thr.Form("%.0f", ceil(thrs[i]));

    turnOnsFixedThr_Iso[i]->SetLineWidth(2);
    turnOnsFixedThr_Iso[i]->SetLineColor(colors[i]);
    turnOnsFixedThr_Iso[i]->SetMarkerStyle(8);
    turnOnsFixedThr_Iso[i]->SetMarkerColor(colors[i]);
    turnOnsFixedThr_Iso[i]->Draw("same");
    legendX.AddEntry(turnOnsFixedThr_noIso[i],"p_{T}^{L1 #tau} > "+thr+" GeV","LPE");
  }

  texl1->Draw("same");
  texl2->Draw("same");
  legendX.Draw("same");

  canvas1.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/turnOnsProgression_Iso.pdf");







   // TURNONS ISO PROGRESSION
  TCanvas canvas1_("c1_","c1_",800,800);
  // canvas.SetLeftMargin(0.15);
  canvas1_.SetGrid();
  // canvas.SetLogy();
  canvas1_.cd();

  TLegend legendX_(0.65,0.11,0.89,0.41);
  legendX_.SetBorderSize(0);

  dummy->Draw();

  turnOnsFixedThr_Iso[7]->SetLineWidth(2);
  turnOnsFixedThr_Iso[7]->SetLineColor(4);
  turnOnsFixedThr_Iso[7]->SetMarkerStyle(8);
  turnOnsFixedThr_Iso[7]->SetMarkerColor(4);
  turnOnsFixedThr_Iso[7]->Draw("same");
  legendX_.AddEntry(turnOnsFixedThr_Iso[7],"p_{T}^{L1 #tau} > 32 GeV, Iso","LPE");

  turnOnsFixedThr_Iso[9]->SetLineWidth(2);
  turnOnsFixedThr_Iso[9]->SetLineColor(2);
  turnOnsFixedThr_Iso[9]->SetMarkerStyle(8);
  turnOnsFixedThr_Iso[9]->SetMarkerColor(2);
  turnOnsFixedThr_Iso[9]->Draw("same");
  legendX_.AddEntry(turnOnsFixedThr_Iso[9],"p_{T}^{L1 #tau} > 34 GeV, Iso","LPE");

  turnOnsFixedThr_noIso[7]->SetLineWidth(2);
  turnOnsFixedThr_noIso[7]->SetLineColor(1);
  turnOnsFixedThr_noIso[7]->SetMarkerStyle(8);
  turnOnsFixedThr_noIso[7]->SetMarkerColor(1);
  turnOnsFixedThr_noIso[7]->Draw("same");
  legendX_.AddEntry(turnOnsFixedThr_noIso[7],"p_{T}^{L1 #tau} > 32 GeV, NoIso","LPE");

  turnOnsFixedThr_noIso[9]->SetLineWidth(2);
  turnOnsFixedThr_noIso[9]->SetLineColor(3);
  turnOnsFixedThr_noIso[9]->SetMarkerStyle(8);
  turnOnsFixedThr_noIso[9]->SetMarkerColor(3);
  turnOnsFixedThr_noIso[9]->Draw("same");
  legendX_.AddEntry(turnOnsFixedThr_noIso[9],"p_{T}^{L1 #tau} > 34 GeV, NoIso","LPE");

  texl1->Draw("same");
  texl2->Draw("same");
  legendX_.Draw("same");

  canvas1_.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/turnOnsForCri_Iso_2.pdf");





  
  TCanvas canvas2_("c2_","c2_",800,800);
  // canvas.SetLeftMargin(0.15);
  canvas2_.SetGrid();
  // canvas.SetLogy();

  turnOnsFixedThr_Iso[6]->GetXaxis()->SetTitle("p_{T}^{Offline #tau} [GeV]");
  turnOnsFixedThr_Iso[6]->SetTitle("");
  turnOnsFixedThr_Iso[6]->GetXaxis()->SetTitleOffset(1.3);
  turnOnsFixedThr_Iso[6]->GetYaxis()->SetTitle("Efficiency");
  turnOnsFixedThr_Iso[6]->GetYaxis()->SetTitleOffset(1.3);
  turnOnsFixedThr_Iso[6]->SetTitle("");
  turnOnsFixedThr_Iso[6]->GetXaxis()->SetRangeUser(20.,100.);
  turnOnsFixedThr_Iso[6]->GetYaxis()->SetRangeUser(0.,1.05);

  turnOnsFixedThr_Iso[6]->SetLineWidth(2);
  turnOnsFixedThr_Iso[6]->SetLineColor(2);
  turnOnsFixedThr_Iso[6]->SetMarkerStyle(8);
  turnOnsFixedThr_Iso[6]->SetMarkerColor(2);
  
  R2_turnOnsFixedThr_Iso[9]->SetLineWidth(2);
  R2_turnOnsFixedThr_Iso[9]->SetLineColor(46);
  R2_turnOnsFixedThr_Iso[9]->SetMarkerStyle(8);
  R2_turnOnsFixedThr_Iso[9]->SetMarkerColor(46);

  turnOnsFixedThr_Iso[6]->Draw();
  R2_turnOnsFixedThr_Iso[9]->Draw("same");

  texl1->Draw("same");
  texl2->Draw("same");
  
  TLegend legend_(0.55,0.15,0.88,0.28);
  legend_.SetBorderSize(0);
  legend_.AddEntry(turnOnsFixedThr_Iso[6],"Run-3, p_{T}^{L1 #tau} > 29 GeV","LPE");
  legend_.AddEntry(R2_turnOnsFixedThr_Iso[9],"Run-2, p_{T}^{L1 #tau} > 32 GeV","LPE");
  legend_.Draw("same");

  canvas2_.SaveAs("PDFs/Run3_13p6TeV_Run"+run_nmbr_str+"/aaaa_plot.pdf");





}
