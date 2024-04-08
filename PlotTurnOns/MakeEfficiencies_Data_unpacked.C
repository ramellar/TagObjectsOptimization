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

using namespace std;

void MakeEfficiencies(TString file, TString tree, int run_nmbr, TString era = "", int DecayMode = -1)
{
  TString run_nmbr_str = to_string(run_nmbr);
  if(era != "" && run_nmbr == -1) { run_nmbr_str = era; }

  TFile f(file,"READ");
  TTree* inTree = (TTree*)f.Get(tree); // "Ntuplizer/TagAndProbe" "Ntuplizer_noTagAndProbe/TagAndProbe"
  ULong64_t in_EventNumber =  0;
  Int_t     in_RunNumber =  0;
  Int_t     in_lumi =  0;
  Float_t   in_offlineTauPt = 0;
  Float_t   in_offlineTauEta = 0;
  Float_t   in_offlineTauPhi = 0;
  Int_t     in_offlineTauDecayMode = -1;
  Float_t   in_l1tPt =  0;
  Float_t   in_l1tEta =  0;
  Float_t   in_l1tPhi =  0;
  Int_t     in_l1tQual =  0;
  Int_t     in_l1tIso = 0;
  Int_t     Nvtx = 0;

  Int_t     in_l1tEmuHasEM = 0;
  Int_t     in_l1tEmuIsMerged = 0;

  inTree->SetBranchAddress("EventNumber", &in_EventNumber);
  inTree->SetBranchAddress("RunNumber", &in_RunNumber);
  inTree->SetBranchAddress("lumi", &in_lumi);
  inTree->SetBranchAddress("tauPt",&in_offlineTauPt);
  inTree->SetBranchAddress("tauEta",&in_offlineTauEta);
  inTree->SetBranchAddress("tauPhi",&in_offlineTauPhi);
  inTree->SetBranchAddress("tauDM",&in_offlineTauDecayMode);
  inTree->SetBranchAddress("l1tPt",&in_l1tPt);
  inTree->SetBranchAddress("l1tEta",&in_l1tEta);
  inTree->SetBranchAddress("l1tPhi",&in_l1tPhi);
  inTree->SetBranchAddress("l1tQual",&in_l1tQual);
  inTree->SetBranchAddress("l1tIso",&in_l1tIso);
  inTree->SetBranchAddress("Nvtx",&Nvtx);

  inTree->SetBranchAddress("l1tEmuHasEM",&in_l1tEmuHasEM);
  inTree->SetBranchAddress("l1tEmuIsMerged",&in_l1tEmuIsMerged);

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


  Double_t binningPt_red[16] = {18,20,22,24,26,28,30,32,35,40,45,50,60,70,90,110};
  TH1F* L1pt_noIso = new TH1F("","", 15, binningPt_red);
  TH1F* L1pt_included_noIso = new TH1F("","", 15, binningPt_red);
  TH1F* L1eta_noIso = new TH1F("","", 15, -3, 3);
  TH1F* L1eta_included_noIso = new TH1F("","", 15, -3, 3);
  TH1F* L1phi_noIso = new TH1F("","", 15, -5, 5);
  TH1F* L1phi_included_noIso = new TH1F("","", 15, -5, 5);
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
  }
  
  for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
  {
    inTree->GetEntry(i);

    // either process the full dataset or just the events with a specific run number
    if (run_nmbr != -1) { if (run_nmbr != in_RunNumber) { continue; } }
    if (i%10000==0) std::cout << i << std::endl;

    if (in_l1tPt<0.) { continue; }
    if (DecayMode != -1) {
      if (in_offlineTauDecayMode != DecayMode) { continue; }
    } 
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
  }

  // ----------------------------------------------------------------------------    
  // save in root file for future necessity
  TFile* fileout = new TFile("ROOTs/ROOTs_2024/efficiencies_of_"+run_nmbr_str+"_unpacked.root","RECREATE");
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
  }
}
