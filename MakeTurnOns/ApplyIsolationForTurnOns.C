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
#include "../Calibrate/ApplyCalibration.C"

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

void ApplyIsolationForTurnOns(int run, int targetRate = 0, float calibThr = 1.7, Bool_t nTTRange = kFALSE)
{
  TString run_str = to_string(run); // run=-1 corresponds to rate evaluated with SingleNeutrino MC

  TString fixedRate = to_string(targetRate);

  TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
  TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

  TString InputFileName = "/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_04_optimizationV0p2_calibThr"+intgr+"p"+decim+"/Tau_MC_CALIBRATED_2023_03_04.root";

  TString FileNameOut = "";
  if (run==-1) FileNameOut = "/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_04_optimizationV0p2_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE"+fixedRate+"kHz_SingleNeutrino120X_2023_03_04";
  else         FileNameOut = "/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_04_optimizationV0p2_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE"+fixedRate+"kHz_Run"+run_str+"_2023_03_04";
  

  std::map<TString,TH3F*> histosIsolation;
  TFile f_Isolation("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/Isolate/ROOTs4LUTs/ROOTs4LUTs_2023/LUTrelaxation_Trigger_Stage2_Run3_MC_optimizationV0p2_calibThr"+intgr+"p"+decim+"_linear_old.root");

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

  // THESE HOSTOGRAMS ARE USED FOR THE CALCULATION OF THE FIGURE OF MERIT (FM) THAT IS USED TO COMPARE THE GOODNESS OF THE VARIOUS TURNONS
  // THE BIINING IS CHOSEN TO HAVE FINE BINNING BETWEEN 40 AND 60 GeV WHICH IS WHERE THE 80% EFFICIENCY POINT GENERALLY IS, OUTSIDE THIS RANGE THERE IS JUST A SINGLE BIN PER SIDE
  Double_t fine_bins[43] = {10, 40, 40.5, 41. , 41.5, 42. , 42.5, 43. , 43.5, 44. , 44.5, 45. , 45.5, 46. , 46.5, 47. , 47.5, 48. , 48.5, 49. , 49.5, 50. , 50.5, 51. , 51.5, 52. , 52.5, 53. , 53.5, 54. , 54.5, 55. , 55.5, 56. , 56.5, 57. , 57.5, 58. , 58.5, 59. , 59.5, 60, 2000};
  TH1F* fine_pass_noIso   = new TH1F("fine_pass_noIso"  ,"fine_pass_noIso"  ,42,fine_bins);
  TH1F* fine_pass_Option1   = new TH1F("fine_pass_Option1"  ,"fine_pass_Option1"  ,42,fine_bins);
  TH1F* fine_pass_Option2   = new TH1F("fine_pass_Option2"  ,"fine_pass_Option2"  ,42,fine_bins);
  TH1F* fine_pass_Option3   = new TH1F("fine_pass_Option3"  ,"fine_pass_Option3"  ,42,fine_bins);
  TH1F* fine_pass_Option4   = new TH1F("fine_pass_Option4"  ,"fine_pass_Option4"  ,42,fine_bins);
  TH1F* fine_pass_Option5   = new TH1F("fine_pass_Option5"  ,"fine_pass_Option5"  ,42,fine_bins);
  TH1F* fine_pass_Option6   = new TH1F("fine_pass_Option6"  ,"fine_pass_Option6"  ,42,fine_bins);
  TH1F* fine_pass_Option7   = new TH1F("fine_pass_Option7"  ,"fine_pass_Option7"  ,42,fine_bins);
  TH1F* fine_pass_Option8   = new TH1F("fine_pass_Option8"  ,"fine_pass_Option8"  ,42,fine_bins);
  TH1F* fine_pass_Option9   = new TH1F("fine_pass_Option9"  ,"fine_pass_Option9"  ,42,fine_bins);
  TH1F* fine_pass_Option10  = new TH1F("fine_pass_Option10" ,"fine_pass_Option10" ,42,fine_bins);
  TH1F* fine_pass_Option11  = new TH1F("fine_pass_Option11" ,"fine_pass_Option11" ,42,fine_bins);
  TH1F* fine_pass_Option12  = new TH1F("fine_pass_Option12" ,"fine_pass_Option12" ,42,fine_bins);
  TH1F* fine_pass_Option13  = new TH1F("fine_pass_Option13" ,"fine_pass_Option13" ,42,fine_bins);
  TH1F* fine_pass_Option14  = new TH1F("fine_pass_Option14" ,"fine_pass_Option14" ,42,fine_bins);
  TH1F* fine_pass_Option15  = new TH1F("fine_pass_Option15" ,"fine_pass_Option15" ,42,fine_bins);
  TH1F* fine_pass_Option16  = new TH1F("fine_pass_Option16" ,"fine_pass_Option16" ,42,fine_bins);
  TH1F* fine_pass_Option17  = new TH1F("fine_pass_Option17" ,"fine_pass_Option17" ,42,fine_bins);
  TH1F* fine_pass_Option18  = new TH1F("fine_pass_Option18" ,"fine_pass_Option18" ,42,fine_bins);
  TH1F* fine_pass_Option19  = new TH1F("fine_pass_Option19" ,"fine_pass_Option19" ,42,fine_bins);
  TH1F* fine_pass_Option20  = new TH1F("fine_pass_Option20" ,"fine_pass_Option20" ,42,fine_bins);
  TH1F* fine_pass_Option21  = new TH1F("fine_pass_Option21" ,"fine_pass_Option21" ,42,fine_bins);
  TH1F* fine_pass_Option22  = new TH1F("fine_pass_Option22" ,"fine_pass_Option22" ,42,fine_bins);
  TH1F* fine_pass_Option22_threshold25  = new TH1F("fine_pass_Option22_threshold25" ,"fine_pass_Option22_threshold25" ,42,fine_bins);
  TH1F* fine_pass_Option23  = new TH1F("fine_pass_Option23" ,"fine_pass_Option23" ,42,fine_bins);
  TH1F* fine_pass_Option24  = new TH1F("fine_pass_Option24" ,"fine_pass_Option24" ,42,fine_bins);
  TH1F* fine_pass_Option25  = new TH1F("fine_pass_Option25" ,"fine_pass_Option25" ,42,fine_bins);
  TH1F* fine_pass_Option26  = new TH1F("fine_pass_Option26" ,"fine_pass_Option26" ,42,fine_bins);
  TH1F* fine_pass_Option27  = new TH1F("fine_pass_Option27" ,"fine_pass_Option27" ,42,fine_bins);
  TH1F* fine_pass_Option28  = new TH1F("fine_pass_Option28" ,"fine_pass_Option28" ,42,fine_bins);
  TH1F* fine_pass_Option29  = new TH1F("fine_pass_Option29" ,"fine_pass_Option29" ,42,fine_bins);
  TH1F* fine_pass_Option30  = new TH1F("fine_pass_Option30" ,"fine_pass_Option30" ,42,fine_bins);
  TH1F* fine_pass_Option31  = new TH1F("fine_pass_Option31" ,"fine_pass_Option31" ,42,fine_bins);
  TH1F* fine_pass_Option31_barrel  = new TH1F("fine_pass_Option31_barrel" ,"fine_pass_Option31_barrel" ,42,fine_bins);
  TH1F* fine_pass_Option31_endcaps  = new TH1F("fine_pass_Option31_endcaps" ,"fine_pass_Option31_endcaps" ,42,fine_bins);
  TH1F* fine_pass_Option31_extrap  = new TH1F("fine_pass_Option31_extrap" ,"fine_pass_Option31_extrap" ,42,fine_bins);
  TH1F* fine_pass_Option31_extrap_threshold25  = new TH1F("fine_pass_Option31_extrap_threshold25" ,"fine_pass_Option31_extrap_threshold25" ,42,fine_bins);

  // THESE HISTOGRAMS ARE USED FOR THE PRODUCTION OF THE TURNON CURVES THEREFORE THEY HAVE A REASONABLE BINNING FOR THE DISPALY
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

  Double_t Threshold_NewLayer1_noIso   = 0.0;
  Double_t Threshold_NewLayer1_Option1 = 0.0;
  Double_t Threshold_NewLayer1_Option2 = 0.0;
  Double_t Threshold_NewLayer1_Option3 = 0.0;
  Double_t Threshold_NewLayer1_Option4 = 0.0;
  Double_t Threshold_NewLayer1_Option5 = 0.0;
  Double_t Threshold_NewLayer1_Option6 = 0.0;
  Double_t Threshold_NewLayer1_Option7 = 0.0;
  Double_t Threshold_NewLayer1_Option8 = 0.0;
  Double_t Threshold_NewLayer1_Option9 = 0.0;
  Double_t Threshold_NewLayer1_Option10 = 0.0;
  Double_t Threshold_NewLayer1_Option11 = 0.0;
  Double_t Threshold_NewLayer1_Option12 = 0.0;
  Double_t Threshold_NewLayer1_Option13 = 0.0;
  Double_t Threshold_NewLayer1_Option14 = 0.0;
  Double_t Threshold_NewLayer1_Option15 = 0.0;
  Double_t Threshold_NewLayer1_Option16 = 0.0;
  Double_t Threshold_NewLayer1_Option17 = 0.0;
  Double_t Threshold_NewLayer1_Option18 = 0.0;
  Double_t Threshold_NewLayer1_Option19 = 0.0;
  Double_t Threshold_NewLayer1_Option20 = 0.0;
  Double_t Threshold_NewLayer1_Option21 = 0.0;
  Double_t Threshold_NewLayer1_Option22 = 0.0;
  Double_t Threshold_NewLayer1_Option23 = 0.0;
  Double_t Threshold_NewLayer1_Option24 = 0.0;
  Double_t Threshold_NewLayer1_Option25 = 0.0;
  Double_t Threshold_NewLayer1_Option26 = 0.0;
  Double_t Threshold_NewLayer1_Option27 = 0.0;
  Double_t Threshold_NewLayer1_Option28 = 0.0;
  Double_t Threshold_NewLayer1_Option29 = 0.0;
  Double_t Threshold_NewLayer1_Option30 = 0.0;
  Double_t Threshold_NewLayer1_Option31 = 0.0;
  Double_t Threshold_NewLayer1_Option31_extrap = 0.0;
  Double_t Threshold_NewLayer1_noIso_unpacked = 0.0;
  Double_t Threshold_NewLayer1_Iso_unpacked = 0.0;

  Double_t Threshold_NewLayer1_Option22_threshold25 = 24.51;
  Double_t Threshold_NewLayer1_Option31_extrap_threshold25 = 24.51;

  // V0p2 OPTIMIZATION
  if(targetRate==12) {
    std::cout << "fixed rate value not investigated -> try another one" << std::endl;
    std::cout << "fixing 0.0GeV thresholds" << std::endl;
  }
  else if(targetRate==14) {
    // Threshold_NewLayer1_noIso   = 48.51;
    // Threshold_NewLayer1_noIso_unpacked = 43.51;
    // Threshold_NewLayer1_Iso_unpacked = 35.51;
    // Threshold_NewLayer1_Option1 = 25.51;
    // Threshold_NewLayer1_Option2 = 35.51;
    // Threshold_NewLayer1_Option3 = 37.51;
    // Threshold_NewLayer1_Option4 = 39.51;
    // Threshold_NewLayer1_Option5 = 40.51;
    // Threshold_NewLayer1_Option6 = 48.51;
    // Threshold_NewLayer1_Option7 = 48.51;
    // Threshold_NewLayer1_Option8 = 27.51;
    // Threshold_NewLayer1_Option9 = 48.51;
    // Threshold_NewLayer1_Option10 = 48.51;
    // Threshold_NewLayer1_Option11 = 48.51;
    // Threshold_NewLayer1_Option12 = 48.51;
    // Threshold_NewLayer1_Option13 = 48.51;
    // Threshold_NewLayer1_Option14 = 48.51;
    // Threshold_NewLayer1_Option15 = 42.51;
    // Threshold_NewLayer1_Option16 = 48.51;
    // Threshold_NewLayer1_Option17 = 48.51;
    // Threshold_NewLayer1_Option18 = 48.51;
    // Threshold_NewLayer1_Option19 = 48.51;
    // Threshold_NewLayer1_Option20 = 48.51;
    // Threshold_NewLayer1_Option21 = 43.51;
    // Threshold_NewLayer1_Option22 = 30.51;
    // Threshold_NewLayer1_Option23 = 28.51;
    // Threshold_NewLayer1_Option24 = 26.51;
    // Threshold_NewLayer1_Option25 = 32.51;
    // Threshold_NewLayer1_Option26 = 25.51;
    // Threshold_NewLayer1_Option27 = 39.51;
    // Threshold_NewLayer1_Option28 = 27.51;
    // Threshold_NewLayer1_Option29 = 29.51;
    // Threshold_NewLayer1_Option30 = 35.51;
    // Threshold_NewLayer1_Option31 = 34.51;
    // Threshold_NewLayer1_Option31_extrap = 32.51;

    Threshold_NewLayer1_noIso   = 44.0;
    Threshold_NewLayer1_Option1 = 26.0;
    Threshold_NewLayer1_Option2 = 31.0;
    Threshold_NewLayer1_Option3 = 32.0;
    Threshold_NewLayer1_Option4 = 34.0;
    Threshold_NewLayer1_Option5 = 36.0;
    Threshold_NewLayer1_Option6 = 42.0;
    Threshold_NewLayer1_Option7 = 42.0;
    Threshold_NewLayer1_Option8 = 28.0;
    Threshold_NewLayer1_Option9 = 42.0;
    Threshold_NewLayer1_Option10 = 42.0;
    Threshold_NewLayer1_Option11 = 43.0;
    Threshold_NewLayer1_Option12 = 44.0;
    Threshold_NewLayer1_Option13 = 44.0;
    Threshold_NewLayer1_Option14 = 44.0;
    Threshold_NewLayer1_Option15 = 36.0;
    Threshold_NewLayer1_Option16 = 44.0;
    Threshold_NewLayer1_Option17 = 44.0;
    Threshold_NewLayer1_Option18 = 44.0;
    Threshold_NewLayer1_Option19 = 44.0;
    Threshold_NewLayer1_Option20 = 44.0;
    Threshold_NewLayer1_Option21 = 38.0;
    Threshold_NewLayer1_Option22 = 30.0;
    Threshold_NewLayer1_Option23 = 29.0;
    Threshold_NewLayer1_Option24 = 28.0;
    Threshold_NewLayer1_Option25 = 29.0;
    Threshold_NewLayer1_Option26 = 27.0;
    Threshold_NewLayer1_Option27 = 34.0;
    Threshold_NewLayer1_Option28 = 28.0;
    Threshold_NewLayer1_Option29 = 29.0;
    Threshold_NewLayer1_Option30 = 31.0;
    Threshold_NewLayer1_Option31 = 34.0;
    Threshold_NewLayer1_Option31_extrap = 33.0;
    Threshold_NewLayer1_noIso_unpacked = 45.0;
    Threshold_NewLayer1_Iso_unpacked = 32.0;

  }
  else if(targetRate==16) {
    std::cout << "fixed rate value not investigated -> try another one" << std::endl;
    std::cout << "fixing 0.0GeV thresholds" << std::endl;
  }
  else if(targetRate==18) {
    std::cout << "fixed rate value not investigated -> try another one" << std::endl;
    std::cout << "fixing 0.0GeV thresholds" << std::endl;
  }
  else if(targetRate==20){
    std::cout << "fixed rate value not investigated -> try another one" << std::endl;
    std::cout << "fixing 0.0GeV thresholds" << std::endl;
  }
  else {
    std::cout << "fixed rate value not investigated -> try another one" << std::endl;
    std::cout << "fixing 0.0GeV thresholds" << std::endl;
  }


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
    

      // HERE WE FILL THE HISTOGRAMS USED FOR THE EVALUATION IF THE FIGURE OF MERIT TO COMPARE TURNONS
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_noIso) fine_pass_noIso->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option1 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option1) fine_pass_Option1->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option2 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option2) fine_pass_Option2->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option3 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option3) fine_pass_Option3->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option4 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option4) fine_pass_Option4->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option5 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option5) fine_pass_Option5->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option6 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option6) fine_pass_Option6->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option7 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option7) fine_pass_Option7->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option8 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option8) fine_pass_Option8->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option9 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option9) fine_pass_Option9->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option10 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option10) fine_pass_Option10->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option11 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option11) fine_pass_Option11->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option12 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option12) fine_pass_Option12->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option13 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option13) fine_pass_Option13->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option14 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option14) fine_pass_Option14->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option15 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option15) fine_pass_Option15->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option16 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option16) fine_pass_Option16->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option17 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option17) fine_pass_Option17->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option18 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option18) fine_pass_Option18->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option19 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option19) fine_pass_Option19->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option20 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option20) fine_pass_Option20->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option21 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option21) fine_pass_Option21->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22) fine_pass_Option22->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22_threshold25 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22) fine_pass_Option22_threshold25->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option23 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option23) fine_pass_Option23->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option24 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option24) fine_pass_Option24->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option25 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option25) fine_pass_Option25->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option26 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option26) fine_pass_Option26->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option27 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option27) fine_pass_Option27->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option28 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option28) fine_pass_Option28->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option29 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option29) fine_pass_Option29->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option30 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option30) fine_pass_Option30->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31) fine_pass_Option31->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31 && fabs(in_OfflineTau_eta)<1.5) fine_pass_Option31_barrel->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31 && fabs(in_OfflineTau_eta)>1.5) fine_pass_Option31_endcaps->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31_extrap && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31_extrap) fine_pass_Option31_extrap->Fill(in_OfflineTau_pt);
      if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31_extrap_threshold25 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31_extrap) fine_pass_Option31_extrap_threshold25->Fill(in_OfflineTau_pt);


      // HERE WE FILL TEH HISTOGRAMS USED FOR THE PRODUCTION OF TURNONS
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

  // TString Run2unpacked_in = "/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_04_optimizationV0p2_calibThr"+intgr+"p"+decim+"/Run2_MC_VBFHToTauTau_M125_MINIAOD102X_2023_03_04.root";
  // TFile f_in_Run2unpacked(Run2unpacked_in.Data(),"READ");

  // TTree* inTree_Run2upacked = (TTree*)f_in_Run2unpacked.Get("Ntuplizer_noTagAndProbe/TagAndProbe"); // tree of uncalibrated EphemeralZeroBias NTuples

  // float Run2PtThr = 31.51; // 2018 thr 31.51

  // float in_offlinePt = 0;
  // float in_l1tPt = 0;
  // float in_l1tEta = 0;
  // int   in_l1tIso = 0;
  // // branches of uncalibrated EphemeralZeroBias NTuples
  // inTree_Run2upacked->SetBranchAddress("tauPt",&in_offlinePt);
  // inTree_Run2upacked->SetBranchAddress("l1tPt",&in_l1tPt);
  // inTree_Run2upacked->SetBranchAddress("l1tEta",&in_l1tEta);
  // inTree_Run2upacked->SetBranchAddress("l1tIso",&in_l1tIso);

  // TH1F* Run2unpacked = new TH1F("Run2unpacked","Run2unpacked",21,binning);
  // TH1F* Run2unpacked_Iso = new TH1F("Run2unpacked_Iso","Run2unpacked_Iso",21,binning);
  // TH1F* fine_pass_Run2unpacked_Iso   = new TH1F("fine_pass_Run2unpacked_Iso"  ,"fine_pass_Run2unpacked_Iso"  ,42,fine_bins);

  // for(UInt_t i = 0 ; i < inTree_Run2upacked->GetEntries() ; ++i)
  // {
  //   inTree_Run2upacked->GetEntry(i);

  //   if(fabs(in_l1tEta)>2.1) continue;

  //   Run2unpacked->Fill(in_offlinePt);
  //   if(in_l1tPt>=Run2PtThr && in_l1tIso>0) Run2unpacked_Iso->Fill(in_offlinePt);
  //   if(in_l1tPt>=Run2PtThr && in_l1tIso>0) fine_pass_Run2unpacked_Iso->Fill(in_offlinePt);
  // }
  

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


  // TGraphAsymmErrors* turnOn_Run2unpacked_Iso = new TGraphAsymmErrors(Run2unpacked_Iso,Run2unpacked,"cp");
  // turnOn_Run2unpacked_Iso->Write();

  TVectorF thr_noIso(1);
  thr_noIso[0] = Threshold_NewLayer1_noIso;
  thr_noIso.Write("thr_noIso");

  TVectorF thr_noIso_unpacked(1);
  thr_noIso_unpacked[0] = Threshold_NewLayer1_noIso_unpacked;
  thr_noIso_unpacked.Write("thr_noIso_unpacked");

  TVectorF thr_Iso_unpacked(1);
  thr_Iso_unpacked[0] = Threshold_NewLayer1_Iso_unpacked;
  thr_Iso_unpacked.Write("thr_Iso_unpacked");

  // TVectorF thr_Run2unpackedIso(1);
  // thr_Run2unpackedIso[0] = Run2PtThr;
  // thr_Run2unpackedIso.Write("thr_Run2unpackedIso");

  TVectorF thr_Option1(1);
  thr_Option1[0] = Threshold_NewLayer1_Option1;
  thr_Option1.Write("thr_Option1");

  TVectorF thr_Option2(1);
  thr_Option2[0] = Threshold_NewLayer1_Option2;
  thr_Option2.Write("thr_Option2");

  TVectorF thr_Option3(1);
  thr_Option3[0] = Threshold_NewLayer1_Option3;
  thr_Option3.Write("thr_Option3");

  TVectorF thr_Option4(1);
  thr_Option4[0] = Threshold_NewLayer1_Option4;
  thr_Option4.Write("thr_Option4");

  TVectorF thr_Option5(1);
  thr_Option5[0] = Threshold_NewLayer1_Option5;
  thr_Option5.Write("thr_Option5");

  TVectorF thr_Option6(1);
  thr_Option6[0] = Threshold_NewLayer1_Option6;
  thr_Option6.Write("thr_Option6");

  TVectorF thr_Option7(1);
  thr_Option7[0] = Threshold_NewLayer1_Option7;
  thr_Option7.Write("thr_Option7");

  TVectorF thr_Option8(1);
  thr_Option8[0] = Threshold_NewLayer1_Option8;
  thr_Option8.Write("thr_Option8");

  TVectorF thr_Option9(1);
  thr_Option9[0] = Threshold_NewLayer1_Option9;
  thr_Option9.Write("thr_Option9");

  TVectorF thr_Option10(1);
  thr_Option10[0] = Threshold_NewLayer1_Option10;
  thr_Option10.Write("thr_Option10");

  TVectorF thr_Option11(1);
  thr_Option11[0] = Threshold_NewLayer1_Option11;
  thr_Option11.Write("thr_Option11");

  TVectorF thr_Option12(1);
  thr_Option12[0] = Threshold_NewLayer1_Option12;
  thr_Option12.Write("thr_Option12");

  TVectorF thr_Option13(1);
  thr_Option13[0] = Threshold_NewLayer1_Option13;
  thr_Option13.Write("thr_Option13");

  TVectorF thr_Option14(1);
  thr_Option14[0] = Threshold_NewLayer1_Option14;
  thr_Option14.Write("thr_Option14");

  TVectorF thr_Option15(1);
  thr_Option15[0] = Threshold_NewLayer1_Option15;
  thr_Option15.Write("thr_Option15");

  TVectorF thr_Option16(1);
  thr_Option16[0] = Threshold_NewLayer1_Option16;
  thr_Option16.Write("thr_Option16");

  TVectorF thr_Option17(1);
  thr_Option17[0] = Threshold_NewLayer1_Option17;
  thr_Option17.Write("thr_Option17");

  TVectorF thr_Option18(1);
  thr_Option18[0] = Threshold_NewLayer1_Option18;
  thr_Option18.Write("thr_Option18");

  TVectorF thr_Option19(1);
  thr_Option19[0] = Threshold_NewLayer1_Option19;
  thr_Option19.Write("thr_Option19");

  TVectorF thr_Option20(1);
  thr_Option20[0] = Threshold_NewLayer1_Option20;
  thr_Option20.Write("thr_Option20");

  TVectorF thr_Option21(1);
  thr_Option21[0] = Threshold_NewLayer1_Option21;
  thr_Option21.Write("thr_Option21");

  TVectorF thr_Option22(1);
  thr_Option22[0] = Threshold_NewLayer1_Option22;
  thr_Option22.Write("thr_Option22");

  TVectorF thr_Option23(1);
  thr_Option23[0] = Threshold_NewLayer1_Option23;
  thr_Option23.Write("thr_Option23");

  TVectorF thr_Option24(1);
  thr_Option24[0] = Threshold_NewLayer1_Option24;
  thr_Option24.Write("thr_Option24");

  TVectorF thr_Option25(1);
  thr_Option25[0] = Threshold_NewLayer1_Option25;
  thr_Option25.Write("thr_Option25");

  TVectorF thr_Option26(1);
  thr_Option26[0] = Threshold_NewLayer1_Option26;
  thr_Option26.Write("thr_Option26");

  TVectorF thr_Option27(1);
  thr_Option27[0] = Threshold_NewLayer1_Option27;
  thr_Option27.Write("thr_Option27");

  TVectorF thr_Option28(1);
  thr_Option28[0] = Threshold_NewLayer1_Option28;
  thr_Option28.Write("thr_Option28");

  TVectorF thr_Option29(1);
  thr_Option29[0] = Threshold_NewLayer1_Option29;
  thr_Option29.Write("thr_Option29");

  TVectorF thr_Option30(1);
  thr_Option30[0] = Threshold_NewLayer1_Option30;
  thr_Option30.Write("thr_Option30");

  TVectorF thr_Option31(1);
  thr_Option31[0] = Threshold_NewLayer1_Option31;
  thr_Option31.Write("thr_Option31");

  TVectorF thr_Option31_extrap(1);
  thr_Option31_extrap[0] = Threshold_NewLayer1_Option31_extrap;
  thr_Option31_extrap.Write("thr_Option31_extrap");


  float interp_eff = 0.8;

  TVectorF fm_noIso(2);
  fm_noIso[0] = linear_interp(turnOn_noIso, interp_eff);
  fm_noIso[1] = integral(fine_pass_noIso, fm_noIso[0]);
  fm_noIso.Write("fm_noIso");
  std::cout << "turnOn_noIso -> pt@"+std::to_string(interp_eff)+"eff = " << fm_noIso[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_noIso[1] << std::endl;

  TVectorF fm_Option1(2);
  fm_Option1[0] = linear_interp(turnOn_Option1, interp_eff);
  fm_Option1[1] = integral(fine_pass_Option1, fm_Option1[0]);
  fm_Option1.Write("fm_Option1");
  std::cout << "turnOn_Option1 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option1[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option1[1] << std::endl;

  TVectorF fm_Option2(2);
  fm_Option2[0] = linear_interp(turnOn_Option2, interp_eff);
  fm_Option2[1] = integral(fine_pass_Option2, fm_Option2[0]);
  fm_Option2.Write("fm_Option2");
  std::cout << "turnOn_Option2 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option2[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option2[1] << std::endl;

  TVectorF fm_Option3(2);
  fm_Option3[0] = linear_interp(turnOn_Option3, interp_eff);
  fm_Option3[1] = integral(fine_pass_Option3, fm_Option3[0]);
  fm_Option3.Write("fm_Option3");
  std::cout << "turnOn_Option3 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option3[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option3[1] << std::endl;

  TVectorF fm_Option4(2);
  fm_Option4[0] = linear_interp(turnOn_Option4, interp_eff);
  fm_Option4[1] = integral(fine_pass_Option4, fm_Option4[0]);
  fm_Option4.Write("fm_Option4");
  std::cout << "turnOn_Option4 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option4[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option4[1] << std::endl;

  TVectorF fm_Option5(2);
  fm_Option5[0] = linear_interp(turnOn_Option5, interp_eff);
  fm_Option5[1] = integral(fine_pass_Option5, fm_Option5[0]);
  fm_Option5.Write("fm_Option5");
  std::cout << "turnOn_Option5 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option5[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option5[1] << std::endl;

  TVectorF fm_Option6(2);
  fm_Option6[0] = linear_interp(turnOn_Option6, interp_eff);
  fm_Option6[1] = integral(fine_pass_Option6, fm_Option6[0]);
  fm_Option6.Write("fm_Option6");
  std::cout << "turnOn_Option6 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option6[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option6[1] << std::endl;

  TVectorF fm_Option7(2);
  fm_Option7[0] = linear_interp(turnOn_Option7, interp_eff);
  fm_Option7[1] = integral(fine_pass_Option7, fm_Option7[0]);
  fm_Option7.Write("fm_Option7");
  std::cout << "turnOn_Option7 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option7[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option7[1] << std::endl;

  TVectorF fm_Option8(2);
  fm_Option8[0] = linear_interp(turnOn_Option8, interp_eff);
  fm_Option8[1] = integral(fine_pass_Option8, fm_Option8[0]);
  fm_Option8.Write("fm_Option8");
  std::cout << "turnOn_Option8 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option8[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option8[1] << std::endl;

  TVectorF fm_Option9(2);
  fm_Option9[0] = linear_interp(turnOn_Option9, interp_eff);
  fm_Option9[1] = integral(fine_pass_Option9, fm_Option9[0]);
  fm_Option9.Write("fm_Option9");
  std::cout << "turnOn_Option9 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option9[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option9[1] << std::endl;

  TVectorF fm_Option10(2);
  fm_Option10[0] = linear_interp(turnOn_Option10, interp_eff);
  fm_Option10[1] = integral(fine_pass_Option10, fm_Option10[0]);
  fm_Option10.Write("fm_Option10");
  std::cout << "turnOn_Option10 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option10[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option10[1] << std::endl;

  TVectorF fm_Option11(2);
  fm_Option11[0] = linear_interp(turnOn_Option11, interp_eff);
  fm_Option11[1] = integral(fine_pass_Option11, fm_Option11[0]);
  fm_Option11.Write("fm_Option11");
  std::cout << "turnOn_Option11 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option11[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option11[1] << std::endl;

  TVectorF fm_Option12(2);
  fm_Option12[0] = linear_interp(turnOn_Option12, interp_eff);
  fm_Option12[1] = integral(fine_pass_Option12, fm_Option12[0]);
  fm_Option12.Write("fm_Option12");
  std::cout << "turnOn_Option12 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option12[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option12[1] << std::endl;

  TVectorF fm_Option13(2);
  fm_Option13[0] = linear_interp(turnOn_Option13, interp_eff);
  fm_Option13[1] = integral(fine_pass_Option13, fm_Option13[0]);
  fm_Option13.Write("fm_Option13");
  std::cout << "turnOn_Option13 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option13[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option13[1] << std::endl;

  TVectorF fm_Option14(2);
  fm_Option14[0] = linear_interp(turnOn_Option14, interp_eff);
  fm_Option14[1] = integral(fine_pass_Option14, fm_Option14[0]);
  fm_Option14.Write("fm_Option14");
  std::cout << "turnOn_Option14 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option14[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option14[1] << std::endl;

  TVectorF fm_Option15(2);
  fm_Option15[0] = linear_interp(turnOn_Option15, interp_eff);
  fm_Option15[1] = integral(fine_pass_Option15, fm_Option15[0]);
  fm_Option15.Write("fm_Option15");
  std::cout << "turnOn_Option15 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option15[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option15[1] << std::endl;

  TVectorF fm_Option16(2);
  fm_Option16[0] = linear_interp(turnOn_Option16, interp_eff);
  fm_Option16[1] = integral(fine_pass_Option16, fm_Option16[0]);
  fm_Option16.Write("fm_Option16");
  std::cout << "turnOn_Option16 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option16[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option16[1] << std::endl;

  TVectorF fm_Option17(2);
  fm_Option17[0] = linear_interp(turnOn_Option17, interp_eff);
  fm_Option17[1] = integral(fine_pass_Option17, fm_Option17[0]);
  fm_Option17.Write("fm_Option17");
  std::cout << "turnOn_Option17 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option17[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option17[1] << std::endl;

  TVectorF fm_Option18(2);
  fm_Option18[0] = linear_interp(turnOn_Option18, interp_eff);
  fm_Option18[1] = integral(fine_pass_Option18, fm_Option18[0]);
  fm_Option18.Write("fm_Option18");
  std::cout << "turnOn_Option18 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option18[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option18[1] << std::endl;

  TVectorF fm_Option19(2);
  fm_Option19[0] = linear_interp(turnOn_Option19, interp_eff);
  fm_Option19[1] = integral(fine_pass_Option19, fm_Option19[0]);
  fm_Option19.Write("fm_Option19");
  std::cout << "turnOn_Option19 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option19[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option19[1] << std::endl;

  TVectorF fm_Option20(2);
  fm_Option20[0] = linear_interp(turnOn_Option20, interp_eff);
  fm_Option20[1] = integral(fine_pass_Option20, fm_Option20[0]);
  fm_Option20.Write("fm_Option20");
  std::cout << "turnOn_Option20 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option20[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option20[1] << std::endl;

  TVectorF fm_Option21(2);
  fm_Option21[0] = linear_interp(turnOn_Option21, interp_eff);
  fm_Option21[1] = integral(fine_pass_Option21, fm_Option21[0]);
  fm_Option21.Write("fm_Option21");
  std::cout << "turnOn_Option21 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option21[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option21[1] << std::endl;

  TVectorF fm_Option22(2);
  fm_Option22[0] = linear_interp(turnOn_Option22, interp_eff);
  fm_Option22[1] = integral(fine_pass_Option22, fm_Option22[0]);
  fm_Option22.Write("fm_Option22");
  std::cout << "turnOn_Option22 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option22[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option22[1] << std::endl;

  TVectorF fm_Option22_threshold25(2);
  fm_Option22_threshold25[0] = linear_interp(turnOn_Option22_threshold25, interp_eff);
  fm_Option22_threshold25[1] = integral(fine_pass_Option22_threshold25, fm_Option22_threshold25[0]);
  fm_Option22_threshold25.Write("fm_Option22_threshold25");
  std::cout << "turnOn_Option22_threshold25 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option22_threshold25[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option22_threshold25[1] << std::endl;

  TVectorF fm_Option23(2);
  fm_Option23[0] = linear_interp(turnOn_Option23, interp_eff);
  fm_Option23[1] = integral(fine_pass_Option23, fm_Option23[0]);
  fm_Option23.Write("fm_Option23");
  std::cout << "turnOn_Option23 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option23[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option23[1] << std::endl;

  TVectorF fm_Option24(2);
  fm_Option24[0] = linear_interp(turnOn_Option24, interp_eff);
  fm_Option24[1] = integral(fine_pass_Option24, fm_Option24[0]);
  fm_Option24.Write("fm_Option24");
  std::cout << "turnOn_Option24 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option24[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option24[1] << std::endl;

  TVectorF fm_Option25(2);
  fm_Option25[0] = linear_interp(turnOn_Option25, interp_eff);
  fm_Option25[1] = integral(fine_pass_Option25, fm_Option25[0]);
  fm_Option25.Write("fm_Option25");
  std::cout << "turnOn_Option25 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option25[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option25[1] << std::endl;

  TVectorF fm_Option26(2);
  fm_Option26[0] = linear_interp(turnOn_Option26, interp_eff);
  fm_Option26[1] = integral(fine_pass_Option26, fm_Option26[0]);
  fm_Option26.Write("fm_Option26");
  std::cout << "turnOn_Option26 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option26[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option26[1] << std::endl;

  TVectorF fm_Option27(2);
  fm_Option27[0] = linear_interp(turnOn_Option27, interp_eff);
  fm_Option27[1] = integral(fine_pass_Option27, fm_Option27[0]);
  fm_Option27.Write("fm_Option27");
  std::cout << "turnOn_Option27 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option27[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option27[1] << std::endl;

  TVectorF fm_Option28(2);
  fm_Option28[0] = linear_interp(turnOn_Option28, interp_eff);
  fm_Option28[1] = integral(fine_pass_Option28, fm_Option28[0]);
  fm_Option28.Write("fm_Option28");
  std::cout << "turnOn_Option28 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option28[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option28[1] << std::endl;

  TVectorF fm_Option29(2);
  fm_Option29[0] = linear_interp(turnOn_Option29, interp_eff);
  fm_Option29[1] = integral(fine_pass_Option29, fm_Option29[0]);
  fm_Option29.Write("fm_Option29");
  std::cout << "turnOn_Option29 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option29[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option29[1] << std::endl;

  TVectorF fm_Option30(2);
  fm_Option30[0] = linear_interp(turnOn_Option30, interp_eff);
  fm_Option30[1] = integral(fine_pass_Option30, fm_Option30[0]);
  fm_Option30.Write("fm_Option30");
  std::cout << "turnOn_Option30 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option30[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option30[1] << std::endl;

  TVectorF fm_Option31(2);
  fm_Option31[0] = linear_interp(turnOn_Option31, interp_eff);
  fm_Option31[1] = integral(fine_pass_Option31, fm_Option31[0]);
  fm_Option31.Write("fm_Option31");
  std::cout << "turnOn_Option31 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option31[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option31[1] << std::endl;

  TVectorF fm_Option31_barrel(2);
  fm_Option31_barrel[0] = linear_interp(turnOn_Option31_barrel, interp_eff);
  fm_Option31_barrel[1] = integral(fine_pass_Option31_barrel, fm_Option31_barrel[0]);
  fm_Option31_barrel.Write("fm_Option31_barrel");
  std::cout << "turnOn_Option31_barrel -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option31_barrel[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option31_barrel[1] << std::endl;

  TVectorF fm_Option31_endcaps(2);
  fm_Option31_endcaps[0] = linear_interp(turnOn_Option31_endcaps, interp_eff);
  fm_Option31_endcaps[1] = integral(fine_pass_Option31_endcaps, fm_Option31_endcaps[0]);
  fm_Option31_endcaps.Write("fm_Option31_endcaps");
  std::cout << "turnOn_Option31_endcaps -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option31_endcaps[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option31_endcaps[1] << std::endl;

  TVectorF fm_Option31_extrap(2);
  fm_Option31_extrap[0] = linear_interp(turnOn_Option31_extrap, interp_eff);
  fm_Option31_extrap[1] = integral(fine_pass_Option31_extrap, fm_Option31_extrap[0]);
  fm_Option31_extrap.Write("fm_Option31_extrap");
  std::cout << "turnOn_Option31_extrap -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option31_extrap[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option31_extrap[1] << std::endl;

  TVectorF fm_Option31_extrap_threshold25(2);
  fm_Option31_extrap_threshold25[0] = linear_interp(turnOn_Option31_extrap_threshold25, interp_eff);
  fm_Option31_extrap_threshold25[1] = integral(fine_pass_Option31_extrap_threshold25, fm_Option31_extrap_threshold25[0]);
  fm_Option31_extrap_threshold25.Write("fm_Option31_extrap_threshold25");
  std::cout << "turnOn_Option31_extrap_threshold25 -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Option31_extrap_threshold25[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Option31_extrap_threshold25[1] << std::endl;

  // TVectorF fm_Run2unpackedIso(2);
  // fm_Run2unpackedIso[0] = linear_interp(turnOn_Run2unpacked_Iso, interp_eff);
  // fm_Run2unpackedIso[1] = integral(fine_pass_Run2unpacked_Iso, fm_Run2unpackedIso[0]);
  // fm_Run2unpackedIso.Write("fm_Run2unpackedIso");
  // std::cout << "turnOn_Run2unpacked_Iso -> pt@"+std::to_string(interp_eff)+"eff = " << fm_Run2unpackedIso[0] << " - #(tau pass pt@"+std::to_string(interp_eff)+"eff && pass trg) = " << fm_Run2unpackedIso[1] << std::endl;


}
