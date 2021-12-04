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

void ApplyIsolationForTurnOns(TString InputFileName = "/data_CMS/cms/motta/Run3preparation/2021_11_22_optimizationV1/Run3_MC_VBFHToTauTau_M125_CALIBRATED_2021_11_22.root", Bool_t nTTRange = kFALSE)
{
  std::map<TString,TH3F*> histosIsolation;
  TFile f_Isolation("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/Isolate/LUTs/LUTrelaxation_Trigger_Stage2_Run3_MC_VBFHToTauTau_M125_optimizationV1.root");

  for(UInt_t i = 0 ; i < 101 ; ++i)
    {
      TString CurrentNameHisto = "LUT_WP";
      ostringstream convert;
      convert << i;
      CurrentNameHisto += convert.str();
      TH3F* current_Histo = (TH3F*)f_Isolation.Get(CurrentNameHisto.Data());
      histosIsolation.insert(make_pair(CurrentNameHisto,current_Histo));
    }  

  for(UInt_t i = 1 ; i < 32 ; ++i)
    {
      TString CurrentNameHisto = "LUT_Progression_";
      ostringstream convert;
      convert << i;
      CurrentNameHisto += convert.str();
      TH3F* current_Histo = (TH3F*)f_Isolation.Get(CurrentNameHisto.Data());
      histosIsolation.insert(make_pair(CurrentNameHisto,current_Histo));
    }  

  TH3F* current_Histo_extrap = (TH3F*)f_Isolation.Get("LUT_Progression_31_extrap");
  histosIsolation.insert(make_pair("LUT_Progression_31_extrap",current_Histo_extrap));

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
  //inTree->SetBranchAddress("L1Tau_PassShapeVeto_0p6_10_50",&in_L1Tau_PassShapeVeto_0p6_10_50);

  Double_t binning[22] = {18,20,22,24,26,28,30,32,35,40,45,50,60,70,90,110,210,350,500,700,1000,2000};
  TH1F* pt = new TH1F("pt","pt",21,binning);
  TH1F* pt_barrel = new TH1F("pt_barrel","pt_barrel",21,binning);
  TH1F* pt_endcaps = new TH1F("pt_endcaps","pt_endcaps",21,binning);
  TH1F* pt_pass_noIso   = new TH1F("pt_pass_noIso"  ,"pt_pass_noIso"  ,21,binning);
  TH1F* pt_pass_Option1   = new TH1F("pt_pass_Option1"  ,"pt_pass_Option1"  ,21,binning);
  TH1F* pt_pass_Option2   = new TH1F("pt_pass_Option2"  ,"pt_pass_Option2"  ,21,binning);
  TH1F* pt_pass_Option3   = new TH1F("pt_pass_Option3"  ,"pt_pass_Option3"  ,21,binning);
  TH1F* pt_pass_Option4   = new TH1F("pt_pass_Option4"  ,"pt_pass_Option4"  ,21,binning);
  TH1F* pt_pass_Option5   = new TH1F("pt_pass_Option5"  ,"pt_pass_Option5"  ,21,binning);
  TH1F* pt_pass_Option6   = new TH1F("pt_pass_Option6"  ,"pt_pass_Option6"  ,21,binning);
  TH1F* pt_pass_Option7   = new TH1F("pt_pass_Option7"  ,"pt_pass_Option7"  ,21,binning);
  TH1F* pt_pass_Option8   = new TH1F("pt_pass_Option8"  ,"pt_pass_Option8"  ,21,binning);
  TH1F* pt_pass_Option9   = new TH1F("pt_pass_Option9"  ,"pt_pass_Option9"  ,21,binning);
  TH1F* pt_pass_Option10  = new TH1F("pt_pass_Option10" ,"pt_pass_Option10" ,21,binning);
  TH1F* pt_pass_Option11  = new TH1F("pt_pass_Option11" ,"pt_pass_Option11" ,21,binning);
  TH1F* pt_pass_Option12  = new TH1F("pt_pass_Option12" ,"pt_pass_Option12" ,21,binning);
  TH1F* pt_pass_Option13  = new TH1F("pt_pass_Option13" ,"pt_pass_Option13" ,21,binning);
  TH1F* pt_pass_Option14  = new TH1F("pt_pass_Option14" ,"pt_pass_Option14" ,21,binning);
  TH1F* pt_pass_Option15  = new TH1F("pt_pass_Option15" ,"pt_pass_Option15" ,21,binning);
  TH1F* pt_pass_Option16  = new TH1F("pt_pass_Option16" ,"pt_pass_Option16" ,21,binning);
  TH1F* pt_pass_Option17  = new TH1F("pt_pass_Option17" ,"pt_pass_Option17" ,21,binning);
  TH1F* pt_pass_Option18  = new TH1F("pt_pass_Option18" ,"pt_pass_Option18" ,21,binning);
  TH1F* pt_pass_Option19  = new TH1F("pt_pass_Option19" ,"pt_pass_Option19" ,21,binning);
  TH1F* pt_pass_Option20  = new TH1F("pt_pass_Option20" ,"pt_pass_Option20" ,21,binning);
  TH1F* pt_pass_Option21  = new TH1F("pt_pass_Option21" ,"pt_pass_Option21" ,21,binning);
  TH1F* pt_pass_Option22  = new TH1F("pt_pass_Option22" ,"pt_pass_Option22" ,21,binning);
  TH1F* pt_pass_Option22_threshold25  = new TH1F("pt_pass_Option22_threshold25" ,"pt_pass_Option22_threshold25" ,21,binning);
  TH1F* pt_pass_Option23  = new TH1F("pt_pass_Option23" ,"pt_pass_Option23" ,21,binning);
  TH1F* pt_pass_Option24  = new TH1F("pt_pass_Option24" ,"pt_pass_Option24" ,21,binning);
  TH1F* pt_pass_Option25  = new TH1F("pt_pass_Option25" ,"pt_pass_Option25" ,21,binning);
  TH1F* pt_pass_Option26  = new TH1F("pt_pass_Option26" ,"pt_pass_Option26" ,21,binning);
  TH1F* pt_pass_Option27  = new TH1F("pt_pass_Option27" ,"pt_pass_Option27" ,21,binning);
  TH1F* pt_pass_Option28  = new TH1F("pt_pass_Option28" ,"pt_pass_Option28" ,21,binning);
  TH1F* pt_pass_Option29  = new TH1F("pt_pass_Option29" ,"pt_pass_Option29" ,21,binning);
  TH1F* pt_pass_Option30  = new TH1F("pt_pass_Option30" ,"pt_pass_Option30" ,21,binning);
  TH1F* pt_pass_Option31  = new TH1F("pt_pass_Option31" ,"pt_pass_Option31" ,21,binning);
  TH1F* pt_pass_Option31_barrel  = new TH1F("pt_pass_Option31_barrel" ,"pt_pass_Option31_barrel" ,21,binning);
  TH1F* pt_pass_Option31_endcaps  = new TH1F("pt_pass_Option31_endcaps" ,"pt_pass_Option31_endcaps" ,21,binning);
  TH1F* pt_pass_Option31_extrap  = new TH1F("pt_pass_Option31_extrap" ,"pt_pass_Option31_extrap" ,21,binning);
  TH1F* pt_pass_Option31_extrap_threshold25  = new TH1F("pt_pass_Option31_extrap_threshold25" ,"pt_pass_Option31_extrap_threshold25" ,21,binning);

  TH1F* nVtx_pass_Option22 = new TH1F("nVtx_pass_Option22" ,"nVtx_pass_Option22" ,50,0.,100.);
  TH1F* nVtx_pass_Option31 = new TH1F("nVtx_pass_Option31" ,"nVtx_pass_Option31" ,50,0.,100.);
  TH1F* nVtx_pass_Option31_extrap = new TH1F("nVtx_pass_Option31_extrap" ,"nVtx_pass_Option31_extrap" ,50,0.,100.);
  TH1F* nVtx  = new TH1F("nVtx" ,"nVtx" ,50,0.,100.);

  TH1F* eta_pass_Option22 = new TH1F("eta_pass_Option22" ,"eta_pass_Option22" ,43,-2.1,2.1);
  TH1F* eta_pass_Option31 = new TH1F("eta_pass_Option31" ,"eta_pass_Option31" ,43,-2.1,2.1);
  TH1F* eta_pass_Option31_extrap = new TH1F("eta_pass_Option31_extrap" ,"eta_pass_Option31_extrap" ,43,-2.1,2.1);
  TH1F* eta  = new TH1F("eta" ,"eta" ,43,-2.1,2.1);

  //TH1F* pt_pass_Option22_ShapeVeto  = new TH1F("pt_pass_Option22_ShapeVeto" ,"pt_pass_Option22_ShapeVeto" ,21,binning);
  //TH1F* pt_pass_Option22_ShapeVeto_0p6_10_50  = new TH1F("pt_pass_Option22_ShapeVeto_0p6_10_50" ,"pt_pass_Option22_ShapeVeto_0p6_10_50" ,21,binning);
  //TH1F* pt_pass_Option22_ShapeVeto_0p5_20_45  = new TH1F("pt_pass_Option22_ShapeVeto_0p5_20_45" ,"pt_pass_Option22_ShapeVeto_0p5_20_45" ,21,binning);
  //TH1F* pt_pass_Option22_ShapeVeto_0p2_25_60  = new TH1F("pt_pass_Option22_ShapeVeto_0p2_25_60" ,"pt_pass_Option22_ShapeVeto_0p2_25_60" ,21,binning);
  //TH1F* pt_pass_Option22_ShapeVeto_0p2_20_100  = new TH1F("pt_pass_Option22_ShapeVeto_0p2_20_100" ,"pt_pass_Option22_ShapeVeto_0p2_20_100" ,21,binning);

  
 
  map<int, int> remap;
  remap[0] = 6 ;
  remap[1] = 5 ;
  remap[2] = 1 ;
  remap[3] = 0 ;
  remap[4] = 4 ;
  remap[5] = 3 ;
  remap[6] = 2 ;
  
  for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    {
      inTree->GetEntry(i);
      if(in_L1Tau_nTT<60 && nTTRange) continue;
      //if(in_Nvtx<40 && in_Nvtx>50) continue;      

      if(fabs(in_OfflineTau_eta)>2.1) continue;

      pt->Fill(in_OfflineTau_pt);
      if(fabs(in_OfflineTau_eta)<1.5) pt_barrel->Fill(in_OfflineTau_pt);
      else pt_endcaps->Fill(in_OfflineTau_pt);
      Int_t Cut_L1Tau_Iso_Option1   = histosIsolation["LUT_Progression_1"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option2   = histosIsolation["LUT_Progression_2"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option3   = histosIsolation["LUT_Progression_3"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option4   = histosIsolation["LUT_Progression_4"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option5   = histosIsolation["LUT_Progression_5"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option6   = histosIsolation["LUT_Progression_6"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option7   = histosIsolation["LUT_Progression_7"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option8   = histosIsolation["LUT_Progression_8"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option9   = histosIsolation["LUT_Progression_9"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option10  = histosIsolation["LUT_Progression_10"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option11  = histosIsolation["LUT_Progression_11"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option12  = histosIsolation["LUT_Progression_12"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option13  = histosIsolation["LUT_Progression_13"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option14  = histosIsolation["LUT_Progression_14"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option15  = histosIsolation["LUT_Progression_15"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option16  = histosIsolation["LUT_Progression_16"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option17  = histosIsolation["LUT_Progression_17"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option18  = histosIsolation["LUT_Progression_18"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option19  = histosIsolation["LUT_Progression_19"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option20  = histosIsolation["LUT_Progression_20"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option21  = histosIsolation["LUT_Progression_21"] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option22  = histosIsolation["LUT_Progression_22"]->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option23  = histosIsolation["LUT_Progression_23"]->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option24  = histosIsolation["LUT_Progression_24"]->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option25  = histosIsolation["LUT_Progression_25"]->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option26  = histosIsolation["LUT_Progression_26"]->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option27  = histosIsolation["LUT_Progression_27"]->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option28  = histosIsolation["LUT_Progression_28"]->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option29  = histosIsolation["LUT_Progression_29"]->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option30  = histosIsolation["LUT_Progression_30"]->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option31  = histosIsolation["LUT_Progression_31"]->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
      Int_t Cut_L1Tau_Iso_Option31_extrap  = histosIsolation["LUT_Progression_31_extrap"]->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);

      Double_t Threshold_NewLayer1_noIso   = 38.51;
      Double_t Threshold_NewLayer1_Option1 = 24.51;
      Double_t Threshold_NewLayer1_Option2 = 29.51;
      Double_t Threshold_NewLayer1_Option3 = 31.51;
      Double_t Threshold_NewLayer1_Option4 = 32.51;
      Double_t Threshold_NewLayer1_Option5 = 33.51;
      Double_t Threshold_NewLayer1_Option6 = 36.51;
      Double_t Threshold_NewLayer1_Option7 = 36.51;
      Double_t Threshold_NewLayer1_Option8 = 27.51;
      Double_t Threshold_NewLayer1_Option9 = 36.51;
      Double_t Threshold_NewLayer1_Option10 = 36.51;
      Double_t Threshold_NewLayer1_Option11 = 36.51;
      Double_t Threshold_NewLayer1_Option12 = 38.51;
      Double_t Threshold_NewLayer1_Option13 = 38.51;
      Double_t Threshold_NewLayer1_Option14 = 38.51;
      Double_t Threshold_NewLayer1_Option15 = 34.51;
      Double_t Threshold_NewLayer1_Option16 = 38.51;
      Double_t Threshold_NewLayer1_Option17 = 38.51;
      Double_t Threshold_NewLayer1_Option18 = 38.51;
      Double_t Threshold_NewLayer1_Option19 = 38.51;
      Double_t Threshold_NewLayer1_Option20 = 38.51;
      Double_t Threshold_NewLayer1_Option21 = 35.51;
      Double_t Threshold_NewLayer1_Option22 = 33.51;
      Double_t Threshold_NewLayer1_Option22_threshold25 = 24.51;
      // Double_t Threshold_NewLayer1_Option22 = 29.51;
      Double_t Threshold_NewLayer1_Option23 = 28.51;
      Double_t Threshold_NewLayer1_Option24 = 27.51;
      Double_t Threshold_NewLayer1_Option25 = 29.51;
      Double_t Threshold_NewLayer1_Option26 = 26.51;
      Double_t Threshold_NewLayer1_Option27 = 32.51;
      Double_t Threshold_NewLayer1_Option28 = 27.51;
      Double_t Threshold_NewLayer1_Option29 = 28.51;
      Double_t Threshold_NewLayer1_Option30 = 29.51;
      Double_t Threshold_NewLayer1_Option31 = 33.51;
      Double_t Threshold_NewLayer1_Option31_extrap = 33.51;
      Double_t Threshold_NewLayer1_Option31_extrap_threshold25 = 24.51;

      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_noIso) pt_pass_noIso->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option1 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option1) pt_pass_Option1->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option2 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option2) pt_pass_Option2->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option3 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option3) pt_pass_Option3->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option4 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option4) pt_pass_Option4->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option5 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option5) pt_pass_Option5->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option6 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option6) pt_pass_Option6->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option7 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option7) pt_pass_Option7->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option8 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option8) pt_pass_Option8->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option9 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option9) pt_pass_Option9->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option10 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option10) pt_pass_Option10->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option11 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option11) pt_pass_Option11->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option12 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option12) pt_pass_Option12->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option13 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option13) pt_pass_Option13->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option14 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option14) pt_pass_Option14->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option15 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option15) pt_pass_Option15->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option16 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option16) pt_pass_Option16->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option17 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option17) pt_pass_Option17->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option18 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option18) pt_pass_Option18->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option19 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option19) pt_pass_Option19->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option20 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option20) pt_pass_Option20->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option21 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option21) pt_pass_Option21->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22) pt_pass_Option22->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22_threshold25 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22) pt_pass_Option22_threshold25->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option23 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option23) pt_pass_Option23->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option24 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option24) pt_pass_Option24->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option25 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option25) pt_pass_Option25->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option26 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option26) pt_pass_Option26->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option27 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option27) pt_pass_Option27->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option28 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option28) pt_pass_Option28->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option29 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option29) pt_pass_Option29->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option30 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option30) pt_pass_Option30->Fill(in_OfflineTau_pt);

      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31) pt_pass_Option31->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31 && fabs(in_OfflineTau_eta)<1.5) pt_pass_Option31_barrel->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31 && fabs(in_OfflineTau_eta)>1.5) pt_pass_Option31_endcaps->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22 && in_OfflineTau_pt>44.) nVtx_pass_Option22->Fill(in_Nvtx);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31 && in_OfflineTau_pt>44.) nVtx_pass_Option31->Fill(in_Nvtx);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31 && in_OfflineTau_pt>44.) eta_pass_Option31->Fill(in_OfflineTau_eta);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22 && in_OfflineTau_pt>44.) eta_pass_Option22->Fill(in_OfflineTau_eta);

      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31_extrap && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31_extrap) pt_pass_Option31_extrap->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31_extrap_threshold25 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31_extrap) pt_pass_Option31_extrap_threshold25->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31_extrap && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31_extrap && in_OfflineTau_pt>44.) nVtx_pass_Option31_extrap->Fill(in_Nvtx);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31_extrap && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31_extrap && in_OfflineTau_pt>44.) eta_pass_Option31_extrap->Fill(in_OfflineTau_eta);

      if(in_OfflineTau_pt>44.) nVtx->Fill(in_Nvtx);
      if(in_OfflineTau_pt>44.) eta->Fill(in_OfflineTau_eta);
     
	// if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22 && in_L1Tau_PassShapeVeto==kTRUE) pt_pass_Option22_ShapeVeto->Fill(in_OfflineTau_pt);

      short int symmShape = getSymmShape (in_L1Tau_Qual, remap);

     // if(TwoDShapeVetos_0p6_10_50->GetBinContent(symmShape,in_compressedE)==1) L1Tau_PassShapeVeto_0p6_10_50 = true;
     // else L1Tau_PassShapeVeto_0p6_10_50 = false;
     // if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22 && L1Tau_PassShapeVeto_0p6_10_50) pt_pass_Option22_ShapeVeto_0p6_10_50->Fill(in_OfflineTau_pt);

      //if(TwoDShapeVetos_0p5_20_45->GetBinContent(symmShape,in_compressedE)==1) L1Tau_PassShapeVeto_0p5_20_45 = true;
      //else L1Tau_PassShapeVeto_0p5_20_45 = false;
     // if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22 && L1Tau_PassShapeVeto_0p5_20_45) pt_pass_Option22_ShapeVeto_0p5_20_45->Fill(in_OfflineTau_pt);

      //if(TwoDShapeVetos_0p2_25_60->GetBinContent(symmShape,in_compressedE)==1) L1Tau_PassShapeVeto_0p2_25_60 = true;
      //else L1Tau_PassShapeVeto_0p2_25_60 = false;
      //if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22 && L1Tau_PassShapeVeto_0p2_25_60) pt_pass_Option22_ShapeVeto_0p2_25_60->Fill(in_OfflineTau_pt);

      //if(TwoDShapeVetos_0p2_20_100->GetBinContent(symmShape,in_compressedE)==1) L1Tau_PassShapeVeto_0p2_20_100 = true;
      //else L1Tau_PassShapeVeto_0p2_20_100 = false;
      //if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22 && L1Tau_PassShapeVeto_0p2_20_100) pt_pass_Option22_ShapeVeto_0p2_20_100->Fill(in_OfflineTau_pt);


      
      
    }

  TString FileNameOut = "/data_CMS/cms/motta/Run3preparation/2021_11_22_optimizationV1/Run3_MC_VBFHToTauTau_M125_TURNONS_2021_11_22";
  // TString FileNameOut = "turnOns_2017Layer1Calibration";
  if(nTTRange) FileNameOut += "_nTTRange";
  FileNameOut += ".root";

  TFile turnOns(FileNameOut.Data(),"RECREATE");

  TGraphAsymmErrors* turnOn_noIso = new TGraphAsymmErrors(pt_pass_noIso,pt,"cp");
  turnOn_noIso->Write();

  TGraphAsymmErrors* turnOn_Option1 = new TGraphAsymmErrors(pt_pass_Option1,pt,"cp");
  turnOn_Option1->Write();

  TGraphAsymmErrors* turnOn_Option2 = new TGraphAsymmErrors(pt_pass_Option2,pt,"cp");
  turnOn_Option2->Write();

  TGraphAsymmErrors* turnOn_Option3 = new TGraphAsymmErrors(pt_pass_Option3,pt,"cp");
  turnOn_Option3->Write();

  TGraphAsymmErrors* turnOn_Option4 = new TGraphAsymmErrors(pt_pass_Option4,pt,"cp");
  turnOn_Option4->Write();

  TGraphAsymmErrors* turnOn_Option5 = new TGraphAsymmErrors(pt_pass_Option5,pt,"cp");
  turnOn_Option5->Write();

  TGraphAsymmErrors* turnOn_Option6 = new TGraphAsymmErrors(pt_pass_Option6,pt,"cp");
  turnOn_Option6->Write();

  TGraphAsymmErrors* turnOn_Option7 = new TGraphAsymmErrors(pt_pass_Option7,pt,"cp");
  turnOn_Option7->Write();

  TGraphAsymmErrors* turnOn_Option8 = new TGraphAsymmErrors(pt_pass_Option8,pt,"cp");
  turnOn_Option8->Write();

  TGraphAsymmErrors* turnOn_Option9 = new TGraphAsymmErrors(pt_pass_Option9,pt,"cp");
  turnOn_Option9->Write();

  TGraphAsymmErrors* turnOn_Option10 = new TGraphAsymmErrors(pt_pass_Option10,pt,"cp");
  turnOn_Option10->Write();

  TGraphAsymmErrors* turnOn_Option11 = new TGraphAsymmErrors(pt_pass_Option11,pt,"cp");
  turnOn_Option11->Write();

  TGraphAsymmErrors* turnOn_Option12 = new TGraphAsymmErrors(pt_pass_Option12,pt,"cp");
  turnOn_Option12->Write();

  TGraphAsymmErrors* turnOn_Option13 = new TGraphAsymmErrors(pt_pass_Option13,pt,"cp");
  turnOn_Option13->Write();

  TGraphAsymmErrors* turnOn_Option14 = new TGraphAsymmErrors(pt_pass_Option14,pt,"cp");
  turnOn_Option14->Write();

  TGraphAsymmErrors* turnOn_Option15 = new TGraphAsymmErrors(pt_pass_Option15,pt,"cp");
  turnOn_Option15->Write();

  TGraphAsymmErrors* turnOn_Option16 = new TGraphAsymmErrors(pt_pass_Option16,pt,"cp");
  turnOn_Option16->Write();

  TGraphAsymmErrors* turnOn_Option17 = new TGraphAsymmErrors(pt_pass_Option17,pt,"cp");
  turnOn_Option17->Write();

  TGraphAsymmErrors* turnOn_Option18 = new TGraphAsymmErrors(pt_pass_Option18,pt,"cp");
  turnOn_Option18->Write();

  TGraphAsymmErrors* turnOn_Option19 = new TGraphAsymmErrors(pt_pass_Option19,pt,"cp");
  turnOn_Option19->Write();

  TGraphAsymmErrors* turnOn_Option20 = new TGraphAsymmErrors(pt_pass_Option20,pt,"cp");
  turnOn_Option20->Write();

  TGraphAsymmErrors* turnOn_Option21 = new TGraphAsymmErrors(pt_pass_Option21,pt,"cp");
  turnOn_Option21->Write();

  TGraphAsymmErrors* turnOn_Option22 = new TGraphAsymmErrors(pt_pass_Option22,pt,"cp");
  turnOn_Option22->Write();

  TGraphAsymmErrors* turnOn_Option22_threshold25 = new TGraphAsymmErrors(pt_pass_Option22_threshold25,pt,"cp");
  turnOn_Option22_threshold25->Write();

  TGraphAsymmErrors* turnOn_Option23 = new TGraphAsymmErrors(pt_pass_Option23,pt,"cp");
  turnOn_Option23->Write();

  TGraphAsymmErrors* turnOn_Option24 = new TGraphAsymmErrors(pt_pass_Option24,pt,"cp");
  turnOn_Option24->Write();

  TGraphAsymmErrors* turnOn_Option25 = new TGraphAsymmErrors(pt_pass_Option25,pt,"cp");
  turnOn_Option25->Write();

  TGraphAsymmErrors* turnOn_Option26 = new TGraphAsymmErrors(pt_pass_Option26,pt,"cp");
  turnOn_Option26->Write();

  TGraphAsymmErrors* turnOn_Option27 = new TGraphAsymmErrors(pt_pass_Option27,pt,"cp");
  turnOn_Option27->Write();

  TGraphAsymmErrors* turnOn_Option28 = new TGraphAsymmErrors(pt_pass_Option28,pt,"cp");
  turnOn_Option28->Write();

  TGraphAsymmErrors* turnOn_Option29 = new TGraphAsymmErrors(pt_pass_Option29,pt,"cp");
  turnOn_Option29->Write();

  TGraphAsymmErrors* turnOn_Option30 = new TGraphAsymmErrors(pt_pass_Option30,pt,"cp");
  turnOn_Option30->Write();

  TGraphAsymmErrors* turnOn_Option31 = new TGraphAsymmErrors(pt_pass_Option31,pt,"cp");
  turnOn_Option31->Write();

  TGraphAsymmErrors* turnOn_Option31_barrel = new TGraphAsymmErrors(pt_pass_Option31_barrel,pt_barrel,"cp");
  turnOn_Option31_barrel->Write();

  TGraphAsymmErrors* turnOn_Option31_endcaps = new TGraphAsymmErrors(pt_pass_Option31_endcaps,pt_endcaps,"cp");
  turnOn_Option31_endcaps->Write();

  TGraphAsymmErrors* nVtx_Option22 = new TGraphAsymmErrors(nVtx_pass_Option22,nVtx,"cp");
  nVtx_Option22->Write();

  TGraphAsymmErrors* nVtx_Option31 = new TGraphAsymmErrors(nVtx_pass_Option31,nVtx,"cp");
  nVtx_Option31->Write();

  TGraphAsymmErrors* eta_Option22 = new TGraphAsymmErrors(eta_pass_Option22,eta,"cp");
  eta_Option22->Write();

  TGraphAsymmErrors* eta_Option31 = new TGraphAsymmErrors(eta_pass_Option31,eta,"cp");
  eta_Option31->Write();

  TGraphAsymmErrors* turnOn_Option31_extrap = new TGraphAsymmErrors(pt_pass_Option31_extrap,pt,"cp");
  turnOn_Option31_extrap->Write();

  TGraphAsymmErrors* turnOn_Option31_extrap_threshold25 = new TGraphAsymmErrors(pt_pass_Option31_extrap_threshold25,pt,"cp");
  turnOn_Option31_extrap_threshold25->Write();

  TGraphAsymmErrors* nVtx_Option31_extrap = new TGraphAsymmErrors(nVtx_pass_Option31_extrap,nVtx,"cp");
  nVtx_Option31_extrap->Write();

  TGraphAsymmErrors* eta_Option31_extrap = new TGraphAsymmErrors(eta_pass_Option31_extrap,eta,"cp");
  eta_Option31_extrap->Write();

  //TGraphAsymmErrors* turnOn_Option22_ShapeVeto = new TGraphAsymmErrors(pt_pass_Option22_ShapeVeto,pt,"cp");
  //turnOn_Option22_ShapeVeto->Write();

  //TGraphAsymmErrors* turnOn_Option22_ShapeVeto_0p6_10_50 = new TGraphAsymmErrors(pt_pass_Option22_ShapeVeto_0p6_10_50,pt,"cp");
  //turnOn_Option22_ShapeVeto_0p6_10_50->Write();

  //TGraphAsymmErrors* turnOn_Option22_ShapeVeto_0p5_20_45 = new TGraphAsymmErrors(pt_pass_Option22_ShapeVeto_0p5_20_45,pt,"cp");
  //turnOn_Option22_ShapeVeto_0p5_20_45->Write();

  //TGraphAsymmErrors* turnOn_Option22_ShapeVeto_0p2_25_60 = new TGraphAsymmErrors(pt_pass_Option22_ShapeVeto_0p2_25_60,pt,"cp");
  //turnOn_Option22_ShapeVeto_0p2_25_60->Write();

  //TGraphAsymmErrors* turnOn_Option22_ShapeVeto_0p2_20_100 = new TGraphAsymmErrors(pt_pass_Option22_ShapeVeto_0p2_20_100,pt,"cp");
  //turnOn_Option22_ShapeVeto_0p2_20_100->Write();

}
