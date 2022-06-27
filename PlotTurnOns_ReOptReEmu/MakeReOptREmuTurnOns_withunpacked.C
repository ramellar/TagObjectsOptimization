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
#include "../Calibrate/ApplyCalibration_newnTT.C"

using namespace std;

float linear_interp(TGraphAsymmErrors* turnON, float interp_eff = 0.8) {
  Double_t x1;
  Double_t x2;
  Double_t y1;
  Double_t y2;

  for (int i = 0; i < turnON->GetN(); ++i)
  {
      turnON->GetPoint(i, x1, y1);
      turnON->GetPoint(i+1, x2, y2);
      if (y2 > interp_eff) break;
  }

  float xp = x1 + (x2-x1)/(y2-y1) * (interp_eff-y1);

  return xp;
}

float integral(TH1F* fine_pt, float xp) {
  int binxp = 0;
  for (int i = 1; i <= fine_pt->GetNbinsX(); ++i)
  {
    Double_t low_edge = fine_pt->GetBinLowEdge(i);
    if (low_edge >= xp) 
    {
      binxp = i;
      break;
    }
  }
  if (binxp == 0) binxp = fine_pt->GetNbinsX();

  return fine_pt->Integral(binxp,fine_pt->GetNbinsX());
}

void MakeTurnOns(int targetRate = 14)
{
  TString fixedRate = to_string(targetRate);

  TString InputFileName = "/data_CMS/cms/motta/Run3preparation/2022_04_21_optimizationV11_ReOptReEmu/Run3_MC_VBFHToTauTau_M125_MATCHED_ReOptReEmu"+fixedRate+"kHz_2022_04_21.root";

  TString FileNameOut = "";
  FileNameOut = "/data_CMS/cms/motta/Run3preparation/2022_04_21_optimizationV11_ReOptReEmu/Run3_MC_VBFHToTauTau_M125_TURNONS_ReOptReEmu"+fixedRate+"kHz_2022_04_21.root";
  

  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // MAKE THE RUN3 UNPACKED TURNON

  TFile f(InputFileName.Data(),"READ");
  TTree* inTree = (TTree*)f.Get("outTreeForTurnOns");

  Float_t in_offlineTauPt = 0;
  Float_t in_offlineTauEta = 0;
  Float_t in_l1tEmuPt = 0;
  Float_t in_l1tEmuEta = 0;
  Int_t   in_l1tEmuIso = 0;
  Int_t   in_Nvtx = 0;

  inTree->SetBranchAddress("OfflineTau_pt" , &in_offlineTauPt);
  inTree->SetBranchAddress("OfflineTau_eta", &in_offlineTauEta);
  inTree->SetBranchAddress("L1Tau_pt"      , &in_l1tEmuPt);
  inTree->SetBranchAddress("L1Tau_eta"     , &in_l1tEmuEta);
  inTree->SetBranchAddress("L1Tau_IsoFlag" , &in_l1tEmuIso);
  inTree->SetBranchAddress("Nvtx"          , &in_Nvtx);


  // THESE HOSTOGRAMS ARE USED FOR THE CALCULATION OF THE FIGURE OF MERIT (FM) THAT IS USED TO COMPARE THE GOODNESS OF THE VARIOUS TURNONS
  // THE BIINING IS CHOSEN TO HAVE FINE BINNING BETWEEN 40 AND 60 GeV WHICH IS WHERE THE 80% EFFICIENCY POINT GENERALLY IS, OUTSIDE THIS RANGE THERE IS JUST A SINGLE BIN PER SIDE
  Double_t fine_bins[43] = {10, 40, 40.5, 41. , 41.5, 42. , 42.5, 43. , 43.5, 44. , 44.5, 45. , 45.5, 46. , 46.5, 47. , 47.5, 48. , 48.5, 49. , 49.5, 50. , 50.5, 51. , 51.5, 52. , 52.5, 53. , 53.5, 54. , 54.5, 55. , 55.5, 56. , 56.5, 57. , 57.5, 58. , 58.5, 59. , 59.5, 60, 2000};
  TH1F* fine_pass_Run3unpacked_Iso   = new TH1F("fine_pass_Run3unpacked_Iso"  ,"fine_pass_Run3unpacked_Iso"  ,42,fine_bins);
  TH1F* fine_pass_Run3unpacked_noIso   = new TH1F("fine_pass_Run3unpacked_noIso"  ,"fine_pass_Run3unpacked_noIso"  ,42,fine_bins);

  // THESE HISTOGRAMS ARE USED FOR THE PRODUCTION OF THE TURNON CURVES THEREFORE THEY HAVE A REASONABLE BINNING FOR THE DISPALY
  Double_t binning[22] = {18,20,22,24,26,28,30,32,35,40,45,50,60,70,90,110,210,350,500,700,1000,2000};
  TH1F* Run3unpacked = new TH1F("Run3unpacked","Run3unpacked",21,binning);
  TH1F* Run3unpacked_Iso = new TH1F("Run3unpacked_Iso","Run3unpacked_Iso",21,binning);
  TH1F* Run3unpacked_noIso = new TH1F("Run3unpacked_noIso","Run3unpacked_noIso",21,binning);

  Double_t nVtxBinning[11] = {0,10,15,20,25,30,35,40,45,50,60};
  TH1F* Run3unpacked_nVtx = new TH1F("Run3unpacked_nVtx","Run3unpacked_nVtx",10,nVtxBinning);
  TH1F* Run3unpacked_Iso_nVtx = new TH1F("Run3unpacked_Iso_nVtx","Run3unpacked_Iso_nVtx",10,nVtxBinning);
  TH1F* Run3unpacked_noIso_nVtx = new TH1F("Run3unpacked_noIso_nVtx","Run3unpacked_noIso_nVtx",10,nVtxBinning);

  Double_t etaBinning[19] = {-2.1,-2.0,-1.75,-1.5,-1.25,-1.0,-0.75,-0.5,-0.25,0.0,0.25,0.5,0.75,1.0,1.25,1.5,1.75,2.0,2.1};
  TH1F* Run3unpacked_eta = new TH1F("Run3unpacked_eta","Run3unpacked_eta",18,etaBinning);
  TH1F* Run3unpacked_Iso_eta = new TH1F("Run3unpacked_Iso_eta","Run3unpacked_Iso_eta",18,etaBinning);
  TH1F* Run3unpacked_noIso_eta = new TH1F("Run3unpacked_noIso_eta","Run3unpacked_noIso_eta",18,etaBinning);

  Double_t Run3PtThr = 0.0;
  Double_t Run3PtThr_noIso = 0.0;

  // V9 OPTIMIZATION
  if(targetRate==12) {
    Run3PtThr = 0.0;
    Run3PtThr_noIso = 0.0;
  }
  else if(targetRate==14) {
    Run3PtThr = 34.51;
    Run3PtThr_noIso = 46.51;
  }
  else if(targetRate==16) {
    Run3PtThr = 34.51;
    Run3PtThr_noIso = 44.51;
  }
  else if(targetRate==18) {
    Run3PtThr = 34.51;
    Run3PtThr_noIso = 42.51;
  }
  else if(targetRate==20){
    Run3PtThr = 34.51;
    Run3PtThr_noIso = 41.51;
  }
  else {
    std::cout << "fixed rate value not investigated -> try another one" << std::endl;
    std::cout << "fixing 0.0GeV thresholds" << std::endl;
  }

  for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    {
      inTree->GetEntry(i);

      if(fabs(in_offlineTauEta)>2.1) continue;

      Run3unpacked->Fill(in_offlineTauPt);
      if(in_l1tEmuPt>=Run3PtThr_noIso) Run3unpacked_noIso->Fill(in_offlineTauPt);
      if(in_l1tEmuPt>=Run3PtThr && in_l1tEmuIso>0) Run3unpacked_Iso->Fill(in_offlineTauPt);
      if(in_l1tEmuPt>=Run3PtThr && in_l1tEmuIso>0) fine_pass_Run3unpacked_Iso->Fill(in_offlineTauPt);

      if(in_offlineTauPt>50) Run3unpacked_nVtx->Fill(in_Nvtx);
      if(in_offlineTauPt>50 && in_l1tEmuPt>=Run3PtThr) Run3unpacked_noIso_nVtx->Fill(in_Nvtx);
      if(in_offlineTauPt>50 && in_l1tEmuPt>=Run3PtThr && in_l1tEmuIso>0) Run3unpacked_Iso_nVtx->Fill(in_Nvtx);

      if(in_offlineTauPt>50) Run3unpacked_eta->Fill(in_offlineTauEta);
      if(in_offlineTauPt>50 && in_l1tEmuPt>=Run3PtThr) Run3unpacked_noIso_eta->Fill(in_offlineTauEta);
      if(in_offlineTauPt>50 && in_l1tEmuPt>=Run3PtThr && in_l1tEmuIso>0) Run3unpacked_Iso_eta->Fill(in_offlineTauEta);

    }


  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // MAKE THE RUN2 UNPACKED TURNON

  TString Run2unpacked_in = "/data_CMS/cms/motta/Run3preparation/2022_04_21_optimizationV11_ReOptReEmu/Run2_MC_VBFHToTauTau_M125_MINIAOD102X_2022_04_21.root";
  TFile f_in_Run2unpacked(Run2unpacked_in.Data(),"READ");

  TTree* inTree_Run2upacked = (TTree*)f_in_Run2unpacked.Get("Ntuplizer_noTagAndProbe/TagAndProbe"); // tree of uncalibrated EphemeralZeroBias NTuples

  float Run2PtThr = 31.51; // 2018 thr 31.51

  Float_t in_offlinePt = 0;
  Float_t in_offlineEta = 0;
  Float_t in_l1tPt = 0;
  Float_t in_l1tEta = 0;
  Int_t   in_l1tIso = 0;
  // branches of uncalibrated EphemeralZeroBias NTuples
  inTree_Run2upacked->SetBranchAddress("tauPt",&in_offlinePt);
  inTree_Run2upacked->SetBranchAddress("tauEta",&in_offlineEta);
  inTree_Run2upacked->SetBranchAddress("l1tPt",&in_l1tPt);
  inTree_Run2upacked->SetBranchAddress("l1tEta",&in_l1tEta);
  inTree_Run2upacked->SetBranchAddress("l1tIso",&in_l1tIso);

  TH1F* Run2unpacked = new TH1F("Run2unpacked","Run2unpacked",21,binning);
  TH1F* Run2unpacked_Iso = new TH1F("Run2unpacked_Iso","Run2unpacked_Iso",21,binning);
  TH1F* fine_pass_Run2unpacked_Iso   = new TH1F("fine_pass_Run2unpacked_Iso"  ,"fine_pass_Run2unpacked_Iso"  ,42,fine_bins);

  for(UInt_t i = 0 ; i < inTree_Run2upacked->GetEntries() ; ++i)
  {
    inTree_Run2upacked->GetEntry(i);

    if(fabs(in_offlineEta)>2.1) continue;

    Run2unpacked->Fill(in_offlinePt);
    if(in_l1tPt>=Run2PtThr && in_l1tIso>0) Run2unpacked_Iso->Fill(in_offlinePt);
    if(in_l1tPt>=Run2PtThr && in_l1tIso>0) fine_pass_Run2unpacked_Iso->Fill(in_offlinePt);
  }
  
  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // STORE THINGS

  TFile turnOns(FileNameOut.Data(),"RECREATE");

  TGraphAsymmErrors* efficiency_Run3unpacked_noIso_nVtx = new TGraphAsymmErrors(Run3unpacked_noIso_nVtx,Run3unpacked_nVtx,"cp");
  efficiency_Run3unpacked_noIso_nVtx->Write();

  TGraphAsymmErrors* efficiency_Run3unpacked_Iso_nVtx = new TGraphAsymmErrors(Run3unpacked_Iso_nVtx,Run3unpacked_nVtx,"cp");
  efficiency_Run3unpacked_Iso_nVtx->Write();

  TGraphAsymmErrors* efficiency_Run3unpacked_noIso_eta = new TGraphAsymmErrors(Run3unpacked_noIso_eta,Run3unpacked_eta,"cp");
  efficiency_Run3unpacked_noIso_eta->Write();

  TGraphAsymmErrors* efficiency_Run3unpacked_Iso_eta = new TGraphAsymmErrors(Run3unpacked_Iso_eta,Run3unpacked_eta,"cp");
  efficiency_Run3unpacked_Iso_eta->Write();



  TGraphAsymmErrors* turnOn_Run3unpacked_Iso = new TGraphAsymmErrors(Run3unpacked_Iso,Run3unpacked,"cp");
  turnOn_Run3unpacked_Iso->Write();

  TGraphAsymmErrors* turnOn_Run3unpacked_noIso = new TGraphAsymmErrors(Run3unpacked_noIso,Run3unpacked,"cp");
  turnOn_Run3unpacked_noIso->Write();

  TGraphAsymmErrors* turnOn_Run2unpacked_Iso = new TGraphAsymmErrors(Run2unpacked_Iso,Run2unpacked,"cp");
  turnOn_Run2unpacked_Iso->Write();

  TVectorF thr_Run3unpackedIso(1);
  thr_Run3unpackedIso[0] = Run3PtThr;
  thr_Run3unpackedIso.Write("thr_Run3unpackedIso");

  TVectorF thr_Run3unpacked_noIso(1);
  thr_Run3unpacked_noIso[0] = Run3PtThr_noIso;
  thr_Run3unpacked_noIso.Write("thr_Run3unpacked_noIso");

  TVectorF thr_Run2unpackedIso(1);
  thr_Run2unpackedIso[0] = Run2PtThr;
  thr_Run2unpackedIso.Write("thr_Run2unpackedIso");


  float interp_eff = 0.8;

  TVectorF fm_Run3unpackedIso(2);
  fm_Run3unpackedIso[0] = linear_interp(turnOn_Run3unpacked_Iso, interp_eff);
  fm_Run3unpackedIso[1] = integral(fine_pass_Run3unpacked_Iso, fm_Run3unpackedIso[0]);
  fm_Run3unpackedIso.Write("fm_Run3unpackedIso");
  std::cout << "turnOn_Run3unpacked_Iso -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Run3unpackedIso[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Run3unpackedIso[1] << std::endl;

  TVectorF fm_Run3unpacked_noIso(2);
  fm_Run3unpacked_noIso[0] = linear_interp(turnOn_Run3unpacked_noIso, interp_eff);
  fm_Run3unpacked_noIso[1] = integral(fine_pass_Run3unpacked_noIso, fm_Run3unpacked_noIso[0]);
  fm_Run3unpacked_noIso.Write("fm_Run3unpacked_noIso");
  std::cout << "turnOn_Run3unpacked_noIso -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Run3unpacked_noIso[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Run3unpacked_noIso[1] << std::endl;


  TVectorF fm_Run2unpackedIso(2);
  fm_Run2unpackedIso[0] = linear_interp(turnOn_Run2unpacked_Iso, interp_eff);
  fm_Run2unpackedIso[1] = integral(fine_pass_Run2unpacked_Iso, fm_Run2unpackedIso[0]);
  fm_Run2unpackedIso.Write("fm_Run2unpackedIso");
  std::cout << "turnOn_Run2unpacked_Iso -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Run2unpackedIso[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Run2unpackedIso[1] << std::endl;


}
