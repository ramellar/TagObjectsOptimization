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
#include "../Calibrate/ApplyCalibration.C"
#include <map>
#include <typeinfo>

using namespace std;

void Rate(int run, TString parametrisation = "linear", Double_t Kfact = 0.0, float calibThr = 1.7)
{
  // TFile f_2DShapeVetos("/home/llr/cms/mperez/TauTrigger/CMSSW_9_2_10/src/TauTagAndProbe/TauTagAndProbe/test/ShapeVeto/TwoDShapeVetos.root","READ");
  // TH2I* TwoDShapeVetos_0p6_10_50 = (TH2I*)f_2DShapeVetos.Get("TwoDShapeVetos_0p6_10_50");
  // TH2I* TwoDShapeVetos_0p5_20_45 = (TH2I*)f_2DShapeVetos.Get("TwoDShapeVetos_0p5_20_45");
  // TH2I* TwoDShapeVetos_0p2_25_60 = (TH2I*)f_2DShapeVetos.Get("TwoDShapeVetos_0p2_25_60");
  // TH2I* TwoDShapeVetos_0p2_20_100 = (TH2I*)f_2DShapeVetos.Get("TwoDShapeVetos_0p2_20_100");
  // TH2I* TwoDShapeVetos_0p2_20_100_dummy = (TH2I*)f_2DShapeVetos.Get("TwoDShapeVetos_0p2_20_100_dummy");
  // TH2I* TwoDShapeVetos_0p5_10_70 = (TH2I*)f_2DShapeVetos.Get("TwoDShapeVetos_0p5_10_70");
  // TH2I* TwoDShapeVetos_0p4_10_70 = (TH2I*)f_2DShapeVetos.Get("TwoDShapeVetos_0p4_10_70");
  // TH2I* TwoDShapeVetos_0p3_10_70 = (TH2I*)f_2DShapeVetos.Get("TwoDShapeVetos_0p3_10_70");
  // TH2I* TwoDShapeVetos_0p3_10_100 = (TH2I*)f_2DShapeVetos.Get("TwoDShapeVetos_0p3_10_100");
  // TH2I* TwoDShapeVetos_0p4_10_35 = (TH2I*)f_2DShapeVetos.Get("TwoDShapeVetos_0p4_10_35");
  // TH2I* TwoDShapeVetos_0p2_10_35 = (TH2I*)f_2DShapeVetos.Get("TwoDShapeVetos_0p2_10_35");

  /*
  ifstream PUFile("PU_per_LS.txt");

  std::map<Int_t,Float_t> PU_per_LS;
  std::string str; 
  while (std::getline(PUFile, str))
    {
      TString temp(str);
      temp.ReplaceAll("5412,283171,","");
      int pos_coma = temp.First(",");
      TString LS_str(temp,pos_coma);
      //cout<<LS_str<<endl;
      TString Replacing = LS_str ;
      Replacing += ",";
      temp.ReplaceAll(Replacing.Data(),"");
      TString PU_str = temp;
      //cout<<PU_str<<endl;
      std::istringstream ss_LS(LS_str.Data());
      Int_t LS ;
      ss_LS >> LS;
      std::istringstream ss_PU(PU_str.Data());
      Float_t PU ;
      ss_PU >> PU;     
      PU_per_LS.insert(std::pair<Int_t,Float_t>(LS , PU ));

    }
  // cout<<"LS = 522, PU = "<<PU_per_LS[522]<<endl;
  // Float_t weight = PU_per_LS[48]/PU_per_LS[522];
  // cout<<"weight = "<<weight<<endl;

  */

  TString run_str = to_string(run);

  TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
  TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

  TString Kintgr = to_string(Kfact).substr(0, to_string(Kfact).find("."));
  TString Kdecim = to_string(Kfact).substr(2, to_string(Kfact).find("."));

  std::map<TString,TH3F*> histosIsolation;
  
  TString fIsoName = "";
  if(parametrisation=="linear") fIsoName = "/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/Isolate/ROOTs4LUTs/ROOTs4LUTs_2023/LUTrelaxation_Trigger_Stage2_Run3_MC_optimizationV0p2_calibThr"+intgr+"p"+decim+"_linear_old.root";
  else                          fIsoName = "/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/Isolate/ROOTs4LUTs/ROOTs4LUTs_2023/LUTrelaxation_Trigger_Stage2_Run3_MC_optimizationV0p2_calibThr"+intgr+"p"+decim+"_"+parametrisation+Kintgr+"p"+Kdecim+".root";
  TFile f_Isolation(fIsoName, "READ");

  for(UInt_t i = 0 ; i < 101 ; ++i)
    {
      TString CurrentNameHisto = "LUT_WP";
      ostringstream convert;
      convert << i;
      CurrentNameHisto += convert.str();
      TH3F* current_Histo = (TH3F*)f_Isolation.Get(CurrentNameHisto.Data());
      histosIsolation.insert(make_pair(CurrentNameHisto,current_Histo));
    }

  // DEBUG
  // for (map<TString,TH3F*>::iterator it = histosIsolation.begin(); it != histosIsolation.end(); it++)
  //   {
  //     cout << it->first << endl;
  //   }
  // return;

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

  // DEBUG
  // cout << typeid(histosIsolation["LUT_WP50"]).name() << endl;
  // cout << typeid(histosIsolation["LUT_WP50"]->GetBinContent(1,1,1)).name() << endl;
  // cout << histosIsolation["LUT_WP50"]->GetBinContent(1,1,1) << endl;
  // return;

  TString FileName_in = "/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_04_optimizationV0p2_calibThr"+intgr+"p"+decim+"/EphemeralZeroBias0__Run2022G_Run"+run_str+"__CALIBRATED.root";
  TFile f_in(FileName_in.Data(),"READ");
  TTree* inTree = (TTree*)f_in.Get("outTreeCalibrated");

  Int_t           in_EventNumber =  0;
  Int_t           in_RunNumber =  0;
  Int_t           in_lumi =  0;
  vector<float>   *in_l1tEmuPt =  0;
  vector<float>   *in_l1tEmuEta =  0;
  vector<float>   *in_l1tEmuPhi =  0;
  vector<int>     *in_l1tEmuQual =  0;
  vector<int>     *in_l1tEmuNTT =  0;
  vector<int>     *in_l1tEmuHasEM =  0;
  vector<int>     *in_l1tEmuIsMerged =  0;
  vector<int>     *in_l1tEmuTowerIEta =  0;
  vector<int>     *in_l1tEmuTowerIPhi =  0;
  vector<int>     *in_l1tEmuRawEt =  0;
  vector<int>     *in_l1tEmuIsoEt =  0;
  vector<int>     *in_compressedieta =  0;
  vector<int>     *in_compressedE =  0;
  vector<int>     *in_compressednTT =  0;
  vector<int>     *in_supercompressedE =  0;
  vector<int>     *in_supercompressednTT =  0;
  vector<float>   *in_CalibPt =  0;
  // vector<Bool_t>  *in_L1Tau_PassShapeVeto = 0;

  inTree->SetBranchAddress("EventNumber", &in_EventNumber);
  inTree->SetBranchAddress("RunNumber", &in_RunNumber);
  inTree->SetBranchAddress("lumi", &in_lumi);
  inTree->SetBranchAddress("L1Tau_pt",&in_l1tEmuPt);
  inTree->SetBranchAddress("L1Tau_eta",&in_l1tEmuEta);
  inTree->SetBranchAddress("L1Tau_phi",&in_l1tEmuPhi);
  inTree->SetBranchAddress("L1Tau_Qual",&in_l1tEmuQual);
  inTree->SetBranchAddress("L1Tau_nTT",&in_l1tEmuNTT);
  inTree->SetBranchAddress("L1Tau_hasEM",&in_l1tEmuHasEM);
  inTree->SetBranchAddress("L1Tau_isMerged",&in_l1tEmuIsMerged);
  inTree->SetBranchAddress("L1Tau_IEt",&in_l1tEmuRawEt);
  inTree->SetBranchAddress("L1Tau_Iso",&in_l1tEmuIsoEt);
  inTree->SetBranchAddress("L1Tau_IEta",&in_l1tEmuTowerIEta);
  inTree->SetBranchAddress("L1Tau_IPhi",&in_l1tEmuTowerIPhi);
  inTree->SetBranchAddress("compressedieta",&in_compressedieta);
  inTree->SetBranchAddress("compressedE",&in_compressedE);
  inTree->SetBranchAddress("compressednTT",&in_compressednTT);
  inTree->SetBranchAddress("supercompressedE",&in_supercompressedE);
  inTree->SetBranchAddress("supercompressednTT",&in_supercompressednTT);
  inTree->SetBranchAddress("L1Tau_CalibPt",&in_CalibPt);
  // inTree->SetBranchAddress("L1Tau_PassShapeVeto",&in_L1Tau_PassShapeVeto);

  TH1F* h_pt_uncalibrated = new TH1F("h_pt_uncalibrated","h_pt_uncalibrated",240,0.,240.);
  TH1F* h_pt_calibrated = new TH1F("h_pt_calibrated","h_pt_calibrated",240,0.,240.);
  TH1F* pt_IsoInf = new TH1F("pt_IsoInf","pt_IsoInf",240,0.,240.);
  // TH1F* pt_IsoInf_ShapeVeto = new TH1F("pt_IsoInf_ShapeVeto","pt_IsoInf_ShapeVeto",240,0.,240.);
  // TH1F* pt_IsoInf_ShapeVeto_0p2_20_100 = new TH1F("pt_IsoInf_ShapeVeto_0p2_20_100","pt_IsoInf_ShapeVeto_0p2_20_100",240,0.,240.);
  // TH1F* pt_IsoInf_ShapeVeto_0p2_20_100_dummy = new TH1F("pt_IsoInf_ShapeVeto_0p2_20_100_dummy","pt_IsoInf_ShapeVeto_0p2_20_100_dummy",240,0.,240.);
  // TH1F* pt_IsoInf_ShapeVeto_0p6_10_50 = new TH1F("pt_IsoInf_ShapeVeto_0p6_10_50","pt_IsoInf_ShapeVeto_0p6_10_50",240,0.,240.);
  // TH1F* pt_IsoInf_ShapeVeto_0p4_10_35 = new TH1F("pt_IsoInf_ShapeVeto_0p4_10_35","pt_IsoInf_ShapeVeto_0p4_10_35",240,0.,240.);
  // TH1F* pt_IsoInf_ShapeVeto_0p5_10_70 = new TH1F("pt_IsoInf_ShapeVeto_0p5_10_70","pt_IsoInf_ShapeVeto_0p5_10_70",240,0.,240.);
  // TH1F* pt_IsoInf_ShapeVeto_0p4_10_70 = new TH1F("pt_IsoInf_ShapeVeto_0p4_10_70","pt_IsoInf_ShapeVeto_0p4_10_70",240,0.,240.);
  // TH1F* pt_IsoInf_ShapeVeto_0p3_10_70 = new TH1F("pt_IsoInf_ShapeVeto_0p3_10_70","pt_IsoInf_ShapeVeto_0p3_10_70",240,0.,240.);
  // TH1F* pt_IsoInf_ShapeVeto_0p3_10_100 = new TH1F("pt_IsoInf_ShapeVeto_0p3_10_100","pt_IsoInf_ShapeVeto_0p3_10_100",240,0.,240.);
  TH1F* pt_Iso_50 = new TH1F("pt_Iso_50","pt_Iso_50",240,0.,240.);
  TH1F* pt_Iso_60 = new TH1F("pt_Iso_60","pt_Iso_60",240,0.,240.);
  TH1F* pt_Iso_70 = new TH1F("pt_Iso_70","pt_Iso_70",240,0.,240.);
  TH1F* pt_Iso_80 = new TH1F("pt_Iso_80","pt_Iso_80",240,0.,240.);
  TH1F* pt_Iso_90 = new TH1F("pt_Iso_90","pt_Iso_90",240,0.,240.);

  TH1F* pt_Progression_1 = new TH1F("pt_Progression_1","pt_Progression_1",240,0.,240.);
  TH1F* pt_Progression_2 = new TH1F("pt_Progression_2","pt_Progression_2",240,0.,240.);
  TH1F* pt_Progression_3 = new TH1F("pt_Progression_3","pt_Progression_3",240,0.,240.);
  TH1F* pt_Progression_4 = new TH1F("pt_Progression_4","pt_Progression_4",240,0.,240.);
  TH1F* pt_Progression_5 = new TH1F("pt_Progression_5","pt_Progression_5",240,0.,240.);
  TH1F* pt_Progression_6 = new TH1F("pt_Progression_6","pt_Progression_6",240,0.,240.);
  TH1F* pt_Progression_7 = new TH1F("pt_Progression_7","pt_Progression_7",240,0.,240.);
  TH1F* pt_Progression_8 = new TH1F("pt_Progression_8","pt_Progression_8",240,0.,240.);
  TH1F* pt_Progression_9 = new TH1F("pt_Progression_9","pt_Progression_9",240,0.,240.);
  TH1F* pt_Progression_10 = new TH1F("pt_Progression_10","pt_Progression_10",240,0.,240.);
  TH1F* pt_Progression_11 = new TH1F("pt_Progression_11","pt_Progression_11",240,0.,240.);
  TH1F* pt_Progression_12 = new TH1F("pt_Progression_12","pt_Progression_12",240,0.,240.);
  TH1F* pt_Progression_13 = new TH1F("pt_Progression_13","pt_Progression_13",240,0.,240.);
  TH1F* pt_Progression_14 = new TH1F("pt_Progression_14","pt_Progression_14",240,0.,240.);
  TH1F* pt_Progression_15 = new TH1F("pt_Progression_15","pt_Progression_15",240,0.,240.);
  TH1F* pt_Progression_16 = new TH1F("pt_Progression_16","pt_Progression_16",240,0.,240.);
  TH1F* pt_Progression_17 = new TH1F("pt_Progression_17","pt_Progression_17",240,0.,240.);
  TH1F* pt_Progression_18 = new TH1F("pt_Progression_18","pt_Progression_18",240,0.,240.);
  TH1F* pt_Progression_19 = new TH1F("pt_Progression_19","pt_Progression_19",240,0.,240.);
  TH1F* pt_Progression_20 = new TH1F("pt_Progression_20","pt_Progression_20",240,0.,240.);
  TH1F* pt_Progression_21 = new TH1F("pt_Progression_21","pt_Progression_21",240,0.,240.);
  TH1F* pt_Progression_22 = new TH1F("pt_Progression_22","pt_Progression_22",240,0.,240.);

  TH2F* pt_IsoInf_DiTau = new TH2F("pt_IsoInf_DiTau","pt_IsoInf_DiTau",240,0.,240.,240,0.,240.);
  // TH2F* pt_IsoInf_DiTau_ShapeVeto = new TH2F("pt_IsoInf_DiTau_ShapeVeto","pt_IsoInf_DiTau_ShapeVeto",240,0.,240.,240,0.,240.);
  // TH2F* pt_IsoInf_DiTau_ShapeVeto_0p2_20_100 = new TH2F("pt_IsoInf_DiTau_ShapeVeto_0p2_20_100","pt_IsoInf_DiTau_ShapeVeto_0p2_20_100",240,0.,240.,240,0.,240.);
  // TH2F* pt_IsoInf_DiTau_ShapeVeto_0p2_20_100_dummy = new TH2F("pt_IsoInf_DiTau_ShapeVeto_0p2_20_100_dummy","pt_IsoInf_DiTau_ShapeVeto_0p2_20_100_dummy",240,0.,240.,240,0.,240.);
  // TH2F* pt_IsoInf_DiTau_ShapeVeto_0p6_10_50 = new TH2F("pt_IsoInf_DiTau_ShapeVeto_0p6_10_50","pt_IsoInf_DiTau_ShapeVeto_0p6_10_50",240,0.,240.,240,0.,240.);
  // TH2F* pt_IsoInf_DiTau_ShapeVeto_0p5_10_70 = new TH2F("pt_IsoInf_DiTau_ShapeVeto_0p5_10_70","pt_IsoInf_DiTau_ShapeVeto_0p5_10_70",240,0.,240.,240,0.,240.);
  // TH2F* pt_IsoInf_DiTau_ShapeVeto_0p4_10_70 = new TH2F("pt_IsoInf_DiTau_ShapeVeto_0p4_10_70","pt_IsoInf_DiTau_ShapeVeto_0p4_10_70",240,0.,240.,240,0.,240.);
  // TH2F* pt_IsoInf_DiTau_ShapeVeto_0p3_10_70 = new TH2F("pt_IsoInf_DiTau_ShapeVeto_0p3_10_70","pt_IsoInf_DiTau_ShapeVeto_0p3_10_70",240,0.,240.,240,0.,240.);
  // TH2F* pt_IsoInf_DiTau_ShapeVeto_0p3_10_100 = new TH2F("pt_IsoInf_DiTau_ShapeVeto_0p3_10_100","pt_IsoInf_DiTau_ShapeVeto_0p3_10_100",240,0.,240.,240,0.,240.);
  // TH2F* pt_IsoInf_DiTau_ShapeVeto_0p4_10_35 = new TH2F("pt_IsoInf_DiTau_ShapeVeto_0p4_10_35","pt_IsoInf_DiTau_ShapeVeto_0p4_10_35",240,0.,240.,240,0.,240.);

  TH2F* pt_DiTau_90pc = new TH2F("pt_DiTau_90pc","pt_DiTau_90pc",240,0.,240.,240,0.,240.);
  TH2F* pt_DiTau_80pc = new TH2F("pt_DiTau_80pc","pt_DiTau_80pc",240,0.,240.,240,0.,240.);
  TH2F* pt_DiTau_70pc = new TH2F("pt_DiTau_70pc","pt_DiTau_70pc",240,0.,240.,240,0.,240.);
  TH2F* pt_DiTau_60pc = new TH2F("pt_DiTau_60pc","pt_DiTau_60pc",240,0.,240.,240,0.,240.);
  TH2F* pt_DiTau_50pc = new TH2F("pt_DiTau_50pc","pt_DiTau_50pc",240,0.,240.,240,0.,240.);

  TH2F* pt_DiTau_Progression_1 = new TH2F("pt_DiTau_Progression_1","pt_DiTau_Progression_1",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_2 = new TH2F("pt_DiTau_Progression_2","pt_DiTau_Progression_2",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_3 = new TH2F("pt_DiTau_Progression_3","pt_DiTau_Progression_3",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_4 = new TH2F("pt_DiTau_Progression_4","pt_DiTau_Progression_4",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_5 = new TH2F("pt_DiTau_Progression_5","pt_DiTau_Progression_5",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_6 = new TH2F("pt_DiTau_Progression_6","pt_DiTau_Progression_6",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_7 = new TH2F("pt_DiTau_Progression_7","pt_DiTau_Progression_7",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_8 = new TH2F("pt_DiTau_Progression_8","pt_DiTau_Progression_8",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_9 = new TH2F("pt_DiTau_Progression_9","pt_DiTau_Progression_9",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_10 = new TH2F("pt_DiTau_Progression_10","pt_DiTau_Progression_10",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_11 = new TH2F("pt_DiTau_Progression_11","pt_DiTau_Progression_11",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_12 = new TH2F("pt_DiTau_Progression_12","pt_DiTau_Progression_12",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_13 = new TH2F("pt_DiTau_Progression_13","pt_DiTau_Progression_13",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_14 = new TH2F("pt_DiTau_Progression_14","pt_DiTau_Progression_14",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_15 = new TH2F("pt_DiTau_Progression_15","pt_DiTau_Progression_15",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_16 = new TH2F("pt_DiTau_Progression_16","pt_DiTau_Progression_16",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_17 = new TH2F("pt_DiTau_Progression_17","pt_DiTau_Progression_17",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_18 = new TH2F("pt_DiTau_Progression_18","pt_DiTau_Progression_18",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_19 = new TH2F("pt_DiTau_Progression_19","pt_DiTau_Progression_19",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_20 = new TH2F("pt_DiTau_Progression_20","pt_DiTau_Progression_20",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_21 = new TH2F("pt_DiTau_Progression_21","pt_DiTau_Progression_21",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_21_barrel = new TH2F("pt_DiTau_Progression_21_barrel","pt_DiTau_Progression_21_barrel",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_21_endcaps = new TH2F("pt_DiTau_Progression_21_endcaps","pt_DiTau_Progression_21_endcaps",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_22 = new TH2F("pt_DiTau_Progression_22","pt_DiTau_Progression_22",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_22_barrel = new TH2F("pt_DiTau_Progression_22_barrel","pt_DiTau_Progression_22_barrel",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_22_barrelendcap = new TH2F("pt_DiTau_Progression_22_barrelendcap","pt_DiTau_Progression_22_barrelendcap",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_22_endcaps = new TH2F("pt_DiTau_Progression_22_endcaps","pt_DiTau_Progression_22_endcaps",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_22_BothAreMerged = new TH2F("pt_DiTau_Progression_22_BothAreMerged","pt_DiTau_Progression_22_BothAreMerged",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_22_OneIsMerged = new TH2F("pt_DiTau_Progression_22_OneIsMerged","pt_DiTau_Progression_22_OneIsMerged",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_22_BothAreNotMerged = new TH2F("pt_DiTau_Progression_22_BothAreNotMerged","pt_DiTau_Progression_22_BothAreNotMerged",240,0.,240.,240,0.,240.);//

  // TH2F* pt_DiTau_Progression_22_ShapeVeto = new TH2F("pt_DiTau_Progression_22_ShapeVeto","pt_DiTau_Progression_22_ShapeVeto",240,0.,240.,240,0.,240.);//
  // TH2F* pt_DiTau_Progression_22_ShapeVeto_0p6_10_50 = new TH2F("pt_DiTau_Progression_22_ShapeVeto_0p6_10_50","pt_DiTau_Progression_22_ShapeVeto_0p6_10_50",240,0.,240.,240,0.,240.);//
  // TH2F* pt_DiTau_Progression_22_ShapeVeto_0p4_10_35 = new TH2F("pt_DiTau_Progression_22_ShapeVeto_0p4_10_35","pt_DiTau_Progression_22_ShapeVeto_0p4_10_35",240,0.,240.,240,0.,240.);//
  // TH2F* pt_DiTau_Progression_22_ShapeVeto_0p2_10_35 = new TH2F("pt_DiTau_Progression_22_ShapeVeto_0p2_10_35","pt_DiTau_Progression_22_ShapeVeto_0p2_10_35",240,0.,240.,240,0.,240.);//
  // TH2F* pt_DiTau_Progression_22_ShapeVeto_0p5_20_45 = new TH2F("pt_DiTau_Progression_22_ShapeVeto_0p5_20_45","pt_DiTau_Progression_22_ShapeVeto_0p5_20_45",240,0.,240.,240,0.,240.);//
  // TH2F* pt_DiTau_Progression_22_ShapeVeto_0p2_25_60 = new TH2F("pt_DiTau_Progression_22_ShapeVeto_0p2_25_60","pt_DiTau_Progression_22_ShapeVeto_0p2_25_60",240,0.,240.,240,0.,240.);//
  // TH2F* pt_DiTau_Progression_22_ShapeVeto_0p2_20_100 = new TH2F("pt_DiTau_Progression_22_ShapeVeto_0p2_20_100","pt_DiTau_Progression_22_ShapeVeto_0p2_20_100",240,0.,240.,240,0.,240.);//
  // TH2F* pt_DiTau_Progression_22_ShapeVeto_0p2_20_100_dummy = new TH2F("pt_DiTau_Progression_22_ShapeVeto_0p2_20_100_dummy","pt_DiTau_Progression_22_ShapeVeto_0p2_20_100_dummy",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_23 = new TH2F("pt_DiTau_Progression_23","pt_DiTau_Progression_23",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_24 = new TH2F("pt_DiTau_Progression_24","pt_DiTau_Progression_24",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_25 = new TH2F("pt_DiTau_Progression_25","pt_DiTau_Progression_25",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_26 = new TH2F("pt_DiTau_Progression_26","pt_DiTau_Progression_26",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_27 = new TH2F("pt_DiTau_Progression_27","pt_DiTau_Progression_27",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_28 = new TH2F("pt_DiTau_Progression_28","pt_DiTau_Progression_28",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_29 = new TH2F("pt_DiTau_Progression_29","pt_DiTau_Progression_29",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_30 = new TH2F("pt_DiTau_Progression_30","pt_DiTau_Progression_30",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_31 = new TH2F("pt_DiTau_Progression_31","pt_DiTau_Progression_31",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_31_barrel = new TH2F("pt_DiTau_Progression_31_barrel","pt_DiTau_Progression_31_barrel",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_31_barrelendcap = new TH2F("pt_DiTau_Progression_31_barrelendcap","pt_DiTau_Progression_31_barrelendcap",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_31_endcaps = new TH2F("pt_DiTau_Progression_31_endcaps","pt_DiTau_Progression_31_endcaps",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_31_extrap = new TH2F("pt_DiTau_Progression_31_extrap","pt_DiTau_Progression_31_extrap",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_31_extrap_barrel = new TH2F("pt_DiTau_Progression_31_extrap_barrel","pt_DiTau_Progression_31_extrap_barrel",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_31_extrap_barrelendcap = new TH2F("pt_DiTau_Progression_31_extrap_barrelendcap","pt_DiTau_Progression_31_extrap_barrelendcap",240,0.,240.,240,0.,240.);//
  TH2F* pt_DiTau_Progression_31_extrap_endcaps = new TH2F("pt_DiTau_Progression_31_extrap_endcaps","pt_DiTau_Progression_31_extrap_endcaps",240,0.,240.,240,0.,240.);//

  TH1F* pt_Stage1 = new TH1F("pt_Stage1","pt_Stage1",240,0.,240.);

  TH1F* Iso_MinBias = new TH1F("Iso_MinBias","Iso_MinBias",20,0.,20.);
  TH1F* Correction_Factor = new TH1F("Correction_Factor","Correction_Factor",100,0.,3.);
  TH1F* Correction_Factor_IEt_30 = new TH1F("Correction_Factor_IEt_30","Correction_Factor_IEt_30",100,0.,3.);

  /*
  //output tree
  TFile f2("Calibrated_Tree_ForRate_WithWP_Flags_ggH_GoodThresholds_NewCompression_NewLayer1_Option5_277069_OlivierFlatWP_SK1616.root","RECREATE");
  TTree outTree("tree","tree");  

  int   Out_L1_IEt      = -99;
  int   Out_L1_compressed_IEt      = -99;
  int   Out_L1_IEta     = -99;
  int   Out_L1_compressed_IEta     = -99;
  int   Out_L1_hasEM    = -99;
  int   Out_L1_isMerged = -99;
  int   Out_L1_nTT      = -99;
  int   Out_L1_Iso      = -99;
  float Out_L1_Calibrated_pt = -99.;
  float Out_L1_UnCalibrated_pt = -99.;
  float Out_L1_CalibrationConstant = -99.;
  float Out_L1_Stage1_Function_Value = 0.;

  outTree.Branch("Out_L1_IEta",&Out_L1_IEta,"Out_L1_IEta/I");  
  outTree.Branch("Out_L1_compressed_IEta",&Out_L1_compressed_IEta,"Out_L1_compressed_IEta/I");  
  outTree.Branch("Out_L1_IEt",&Out_L1_IEt,"Out_L1_IEt/I");  
  outTree.Branch("Out_L1_compressed_IEt",&Out_L1_compressed_IEt,"Out_L1_compressed_IEt/I");  
  outTree.Branch("Out_L1_hasEM",&Out_L1_hasEM,"Out_L1_hasEM/I");  
  outTree.Branch("Out_L1_isMerged",&Out_L1_isMerged,"Out_L1_isMerged/I");  
  outTree.Branch("Out_L1_nTT",&Out_L1_nTT,"Out_L1_nTT/I");  
  outTree.Branch("Out_L1_Iso",&Out_L1_Iso,"Out_L1_Iso/I");  
  outTree.Branch("Out_L1_Calibrated_pt",&Out_L1_Calibrated_pt,"Out_L1_Calibrated_pt/F");  
  outTree.Branch("Out_L1_UnCalibrated_pt",&Out_L1_UnCalibrated_pt,"Out_L1_UnCalibrated_pt/F");  
  outTree.Branch("Out_L1_CalibrationConstant",&Out_L1_CalibrationConstant,"Out_L1_CalibrationConstant/F");  
  outTree.Branch("Out_L1_Stage1_Function_Value",&Out_L1_Stage1_Function_Value,"Out_L1_Stage1_Function_Value/F");  
  */ 

  Int_t Denominator = 0;

  cout<<"begin loop"<<endl;


  map<int, int> remap;
  remap[0] = 6 ;
  remap[1] = 5 ;
  remap[2] = 1 ;
  remap[3] = 0 ;
  remap[4] = 4 ;
  remap[5] = 3 ;
  remap[6] = 2 ;


  //for(UInt_t i = 0 ; i < 2000000 ; ++i)

  int CounterPass = 0;
  int CounterFail = 0;
  int CounterAll = 0;

  int DiTauCounterPass = 0;
  int DiTauCounterFail = 0;
  int DiTauCounterAll = 0;

  // for(UInt_t i = 0 ; i < 3000000 ; ++i)
  for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    {
      inTree->GetEntry(i);
      if(i%10000==0) cout<<"Entry #"<<i<<endl; 
      // SET RUN INFO
      if (run == 323755) { if(in_lumi<44 || in_lumi>544) continue; } // Run323755
      if (run == 323775) { if(in_lumi<53 || in_lumi>171 || in_lumi==82 || in_lumi==83) continue; } // Run323775 --> very smal numer of lumisections!!
      if (run == 362616) { if(in_lumi<0) continue; }
      if (run == 362617) { if(in_lumi<0) continue; }


      // if(PU_per_LS.find(in_lumi)==PU_per_LS.end()) continue;
      // Float_t weight = PU_per_LS[48]/PU_per_LS[in_lumi];
      // if(weight<0.5) cout<<"lumi = "<<in_lumi<<", weight = "<<weight<<endl;
      // if(weight>2) cout<<"lumi = "<<in_lumi<<", weight = "<<weight<<endl;
      Float_t weight = 1.;

      //if(lumi<81||lumi>300) continue;
      ++Denominator;
            
      bool Filled_IsoInf = kFALSE;
      // bool Filled_IsoInf_ShapeVeto = kFALSE;
      // bool Filled_IsoInf_ShapeVeto_0p2_20_100 = kFALSE;
      // bool Filled_IsoInf_ShapeVeto_0p2_20_100_dummy = kFALSE;
      // bool Filled_IsoInf_ShapeVeto_0p6_10_50 = kFALSE;
      // bool Filled_IsoInf_ShapeVeto_0p4_10_35 = kFALSE;
      // bool Filled_IsoInf_ShapeVeto_0p5_10_70 = kFALSE;
      // bool Filled_IsoInf_ShapeVeto_0p4_10_70 = kFALSE;
      // bool Filled_IsoInf_ShapeVeto_0p3_10_70 = kFALSE;
      // bool Filled_IsoInf_ShapeVeto_0p3_10_100 = kFALSE;
      bool Filled_Iso_50 = kFALSE;
      bool Filled_Iso_60 = kFALSE;
      bool Filled_Iso_70 = kFALSE;
      bool Filled_Iso_80 = kFALSE;
      bool Filled_Iso_90 = kFALSE;

      bool Filled_Progression_1 = kFALSE;
      bool Filled_Progression_2 = kFALSE;
      bool Filled_Progression_3 = kFALSE;
      bool Filled_Progression_4 = kFALSE;
      bool Filled_Progression_5 = kFALSE;
      bool Filled_Progression_6 = kFALSE;
      bool Filled_Progression_7 = kFALSE;
      bool Filled_Progression_8 = kFALSE;
      bool Filled_Progression_9 = kFALSE;
      bool Filled_Progression_10 = kFALSE;
      bool Filled_Progression_11 = kFALSE;
      bool Filled_Progression_12 = kFALSE;
      bool Filled_Progression_13 = kFALSE;
      bool Filled_Progression_14 = kFALSE;
      bool Filled_Progression_15 = kFALSE;
      bool Filled_Progression_16 = kFALSE;
      bool Filled_Progression_17 = kFALSE;
      bool Filled_Progression_18 = kFALSE;
      bool Filled_Progression_19 = kFALSE;
      bool Filled_Progression_20 = kFALSE;
      bool Filled_Progression_21 = kFALSE;
      bool Filled_Progression_22 = kFALSE;
      // bool Filled_Progression_22_ShapeVeto = kFALSE;
      // bool Filled_Progression_22_ShapeVeto_0p6_10_50 = kFALSE;
      // bool Filled_Progression_22_ShapeVeto_0p4_10_35 = kFALSE;
      // bool Filled_Progression_22_ShapeVeto_0p2_10_35 = kFALSE;
      // bool Filled_Progression_22_ShapeVeto_0p5_20_45 = kFALSE;
      // bool Filled_Progression_22_ShapeVeto_0p2_25_60 = kFALSE;
      // bool Filled_Progression_22_ShapeVeto_0p2_20_100 = kFALSE;
      // bool Filled_Progression_22_ShapeVeto_0p2_20_100_dummy = kFALSE;

      std::vector<Int_t> Index_Taus_IsoInf;
      Index_Taus_IsoInf.push_back(-1); Index_Taus_IsoInf.push_back(-1);
      std::vector<Float_t> pt_Taus_IsoInf;
      pt_Taus_IsoInf.push_back(-99.); pt_Taus_IsoInf.push_back(-99.);

      // std::vector<Int_t> Index_Taus_IsoInf_ShapeVeto;
      // Index_Taus_IsoInf_ShapeVeto.push_back(-1); Index_Taus_IsoInf_ShapeVeto.push_back(-1);
      // std::vector<Float_t> pt_Taus_IsoInf_ShapeVeto;
      // pt_Taus_IsoInf_ShapeVeto.push_back(-99.); pt_Taus_IsoInf_ShapeVeto.push_back(-99.);

      // std::vector<Int_t> Index_Taus_IsoInf_ShapeVeto_0p2_20_100;
      // Index_Taus_IsoInf_ShapeVeto_0p2_20_100.push_back(-1); Index_Taus_IsoInf_ShapeVeto_0p2_20_100.push_back(-1);
      // std::vector<Float_t> pt_Taus_IsoInf_ShapeVeto_0p2_20_100;
      // pt_Taus_IsoInf_ShapeVeto_0p2_20_100.push_back(-99.); pt_Taus_IsoInf_ShapeVeto_0p2_20_100.push_back(-99.);

      // std::vector<Int_t> Index_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy;
      // Index_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.push_back(-1); Index_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.push_back(-1);
      // std::vector<Float_t> pt_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy;
      // pt_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.push_back(-99.); pt_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.push_back(-99.);

      // std::vector<Int_t> Index_Taus_IsoInf_ShapeVeto_0p6_10_50;
      // Index_Taus_IsoInf_ShapeVeto_0p6_10_50.push_back(-1); Index_Taus_IsoInf_ShapeVeto_0p6_10_50.push_back(-1);
      // std::vector<Float_t> pt_Taus_IsoInf_ShapeVeto_0p6_10_50;
      // pt_Taus_IsoInf_ShapeVeto_0p6_10_50.push_back(-99.); pt_Taus_IsoInf_ShapeVeto_0p6_10_50.push_back(-99.);

      // std::vector<Int_t> Index_Taus_IsoInf_ShapeVeto_0p4_10_35;
      // Index_Taus_IsoInf_ShapeVeto_0p4_10_35.push_back(-1); Index_Taus_IsoInf_ShapeVeto_0p4_10_35.push_back(-1);
      // std::vector<Float_t> pt_Taus_IsoInf_ShapeVeto_0p4_10_35;
      // pt_Taus_IsoInf_ShapeVeto_0p4_10_35.push_back(-99.); pt_Taus_IsoInf_ShapeVeto_0p4_10_35.push_back(-99.);

      // std::vector<Int_t> Index_Taus_IsoInf_ShapeVeto_0p5_10_70;
      // Index_Taus_IsoInf_ShapeVeto_0p5_10_70.push_back(-1); Index_Taus_IsoInf_ShapeVeto_0p5_10_70.push_back(-1);
      // std::vector<Float_t> pt_Taus_IsoInf_ShapeVeto_0p5_10_70;
      // pt_Taus_IsoInf_ShapeVeto_0p5_10_70.push_back(-99.); pt_Taus_IsoInf_ShapeVeto_0p5_10_70.push_back(-99.);

      // std::vector<Int_t> Index_Taus_IsoInf_ShapeVeto_0p4_10_70;
      // Index_Taus_IsoInf_ShapeVeto_0p4_10_70.push_back(-1); Index_Taus_IsoInf_ShapeVeto_0p4_10_70.push_back(-1);
      // std::vector<Float_t> pt_Taus_IsoInf_ShapeVeto_0p4_10_70;
      // pt_Taus_IsoInf_ShapeVeto_0p4_10_70.push_back(-99.); pt_Taus_IsoInf_ShapeVeto_0p4_10_70.push_back(-99.);

      // std::vector<Int_t> Index_Taus_IsoInf_ShapeVeto_0p3_10_70;
      // Index_Taus_IsoInf_ShapeVeto_0p3_10_70.push_back(-1); Index_Taus_IsoInf_ShapeVeto_0p3_10_70.push_back(-1);
      // std::vector<Float_t> pt_Taus_IsoInf_ShapeVeto_0p3_10_70;
      // pt_Taus_IsoInf_ShapeVeto_0p3_10_70.push_back(-99.); pt_Taus_IsoInf_ShapeVeto_0p3_10_70.push_back(-99.);

      // std::vector<Int_t> Index_Taus_IsoInf_ShapeVeto_0p3_10_100;
      // Index_Taus_IsoInf_ShapeVeto_0p3_10_100.push_back(-1); Index_Taus_IsoInf_ShapeVeto_0p3_10_100.push_back(-1);
      // std::vector<Float_t> pt_Taus_IsoInf_ShapeVeto_0p3_10_100;
      // pt_Taus_IsoInf_ShapeVeto_0p3_10_100.push_back(-99.); pt_Taus_IsoInf_ShapeVeto_0p3_10_100.push_back(-99.);

      std::vector<Int_t> Index_Taus_90;
      Index_Taus_90.push_back(-1); Index_Taus_90.push_back(-1);
      std::vector<Float_t> pt_Taus_90;
      pt_Taus_90.push_back(-99.); pt_Taus_90.push_back(-99.);

      std::vector<Int_t> Index_Taus_80;
      Index_Taus_80.push_back(-1); Index_Taus_80.push_back(-1);
      std::vector<Float_t> pt_Taus_80;
      pt_Taus_80.push_back(-99.); pt_Taus_80.push_back(-99.);

      std::vector<Int_t> Index_Taus_70;
      Index_Taus_70.push_back(-1); Index_Taus_70.push_back(-1);
      std::vector<Float_t> pt_Taus_70;
      pt_Taus_70.push_back(-99.); pt_Taus_70.push_back(-99.);

      std::vector<Int_t> Index_Taus_60;
      Index_Taus_60.push_back(-1); Index_Taus_60.push_back(-1);
      std::vector<Float_t> pt_Taus_60;
      pt_Taus_60.push_back(-99.); pt_Taus_60.push_back(-99.);

      std::vector<Int_t> Index_Taus_50;
      Index_Taus_50.push_back(-1); Index_Taus_50.push_back(-1);
      std::vector<Float_t> pt_Taus_50;
      pt_Taus_50.push_back(-99.); pt_Taus_50.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_1;
      Index_Taus_Progression_1.push_back(-1); Index_Taus_Progression_1.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_1;
      pt_Taus_Progression_1.push_back(-99.); pt_Taus_Progression_1.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_2;
      Index_Taus_Progression_2.push_back(-1); Index_Taus_Progression_2.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_2;
      pt_Taus_Progression_2.push_back(-99.); pt_Taus_Progression_2.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_3;
      Index_Taus_Progression_3.push_back(-1); Index_Taus_Progression_3.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_3;
      pt_Taus_Progression_3.push_back(-99.); pt_Taus_Progression_3.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_4;
      Index_Taus_Progression_4.push_back(-1); Index_Taus_Progression_4.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_4;
      pt_Taus_Progression_4.push_back(-99.); pt_Taus_Progression_4.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_5;
      Index_Taus_Progression_5.push_back(-1); Index_Taus_Progression_5.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_5;
      pt_Taus_Progression_5.push_back(-99.); pt_Taus_Progression_5.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_6;
      Index_Taus_Progression_6.push_back(-1); Index_Taus_Progression_6.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_6;
      pt_Taus_Progression_6.push_back(-99.); pt_Taus_Progression_6.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_7;
      Index_Taus_Progression_7.push_back(-1); Index_Taus_Progression_7.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_7;
      pt_Taus_Progression_7.push_back(-99.); pt_Taus_Progression_7.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_8;
      Index_Taus_Progression_8.push_back(-1); Index_Taus_Progression_8.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_8;
      pt_Taus_Progression_8.push_back(-99.); pt_Taus_Progression_8.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_9;
      Index_Taus_Progression_9.push_back(-1); Index_Taus_Progression_9.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_9;
      pt_Taus_Progression_9.push_back(-99.); pt_Taus_Progression_9.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_10;
      Index_Taus_Progression_10.push_back(-1); Index_Taus_Progression_10.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_10;
      pt_Taus_Progression_10.push_back(-99.); pt_Taus_Progression_10.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_11;
      Index_Taus_Progression_11.push_back(-1); Index_Taus_Progression_11.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_11;
      pt_Taus_Progression_11.push_back(-99.); pt_Taus_Progression_11.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_12;
      Index_Taus_Progression_12.push_back(-1); Index_Taus_Progression_12.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_12;
      pt_Taus_Progression_12.push_back(-99.); pt_Taus_Progression_12.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_13;
      Index_Taus_Progression_13.push_back(-1); Index_Taus_Progression_13.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_13;
      pt_Taus_Progression_13.push_back(-99.); pt_Taus_Progression_13.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_14;
      Index_Taus_Progression_14.push_back(-1); Index_Taus_Progression_14.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_14;
      pt_Taus_Progression_14.push_back(-99.); pt_Taus_Progression_14.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_15;
      Index_Taus_Progression_15.push_back(-1); Index_Taus_Progression_15.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_15;
      pt_Taus_Progression_15.push_back(-99.); pt_Taus_Progression_15.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_16;
      Index_Taus_Progression_16.push_back(-1); Index_Taus_Progression_16.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_16;
      pt_Taus_Progression_16.push_back(-99.); pt_Taus_Progression_16.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_17;
      Index_Taus_Progression_17.push_back(-1); Index_Taus_Progression_17.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_17;
      pt_Taus_Progression_17.push_back(-99.); pt_Taus_Progression_17.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_18;
      Index_Taus_Progression_18.push_back(-1); Index_Taus_Progression_18.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_18;
      pt_Taus_Progression_18.push_back(-99.); pt_Taus_Progression_18.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_19;
      Index_Taus_Progression_19.push_back(-1); Index_Taus_Progression_19.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_19;
      pt_Taus_Progression_19.push_back(-99.); pt_Taus_Progression_19.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_20;
      Index_Taus_Progression_20.push_back(-1); Index_Taus_Progression_20.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_20;
      pt_Taus_Progression_20.push_back(-99.); pt_Taus_Progression_20.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_21;
      Index_Taus_Progression_21.push_back(-1); Index_Taus_Progression_21.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_21;
      pt_Taus_Progression_21.push_back(-99.); pt_Taus_Progression_21.push_back(-99.);
      std::vector<Float_t> eta_Taus_Progression_21;
      eta_Taus_Progression_21.push_back(-99.); eta_Taus_Progression_21.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_22;
      Index_Taus_Progression_22.push_back(-1); Index_Taus_Progression_22.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_22;
      pt_Taus_Progression_22.push_back(-99.); pt_Taus_Progression_22.push_back(-99.);
      std::vector<Float_t> eta_Taus_Progression_22;
      eta_Taus_Progression_22.push_back(-99.); eta_Taus_Progression_22.push_back(-99.);
      std::vector<Int_t> isMerged_Taus_Progression_22;
      isMerged_Taus_Progression_22.push_back(-99.); isMerged_Taus_Progression_22.push_back(-99.);

      // std::vector<Int_t> Index_Taus_Progression_22_ShapeVeto;
      // Index_Taus_Progression_22_ShapeVeto.push_back(-1); Index_Taus_Progression_22_ShapeVeto.push_back(-1);
      // std::vector<Float_t> pt_Taus_Progression_22_ShapeVeto;
      // pt_Taus_Progression_22_ShapeVeto.push_back(-99.); pt_Taus_Progression_22_ShapeVeto.push_back(-99.);
      // std::vector<Float_t> eta_Taus_Progression_22_ShapeVeto;
      // eta_Taus_Progression_22_ShapeVeto.push_back(-99.); eta_Taus_Progression_22_ShapeVeto.push_back(-99.);

      // std::vector<Int_t> Index_Taus_Progression_22_ShapeVeto_0p6_10_50;
      // Index_Taus_Progression_22_ShapeVeto_0p6_10_50.push_back(-1); Index_Taus_Progression_22_ShapeVeto_0p6_10_50.push_back(-1);
      // std::vector<Float_t> pt_Taus_Progression_22_ShapeVeto_0p6_10_50;
      // pt_Taus_Progression_22_ShapeVeto_0p6_10_50.push_back(-99.); pt_Taus_Progression_22_ShapeVeto_0p6_10_50.push_back(-99.);
      // std::vector<Float_t> eta_Taus_Progression_22_ShapeVeto_0p6_10_50;
      // eta_Taus_Progression_22_ShapeVeto_0p6_10_50.push_back(-99.); eta_Taus_Progression_22_ShapeVeto_0p6_10_50.push_back(-99.);

      // std::vector<Int_t> Index_Taus_Progression_22_ShapeVeto_0p4_10_35;
      // Index_Taus_Progression_22_ShapeVeto_0p4_10_35.push_back(-1); Index_Taus_Progression_22_ShapeVeto_0p4_10_35.push_back(-1);
      // std::vector<Float_t> pt_Taus_Progression_22_ShapeVeto_0p4_10_35;
      // pt_Taus_Progression_22_ShapeVeto_0p4_10_35.push_back(-99.); pt_Taus_Progression_22_ShapeVeto_0p4_10_35.push_back(-99.);
      // std::vector<Float_t> eta_Taus_Progression_22_ShapeVeto_0p4_10_35;
      // eta_Taus_Progression_22_ShapeVeto_0p4_10_35.push_back(-99.); eta_Taus_Progression_22_ShapeVeto_0p4_10_35.push_back(-99.);

      // std::vector<Int_t> Index_Taus_Progression_22_ShapeVeto_0p2_10_35;
      // Index_Taus_Progression_22_ShapeVeto_0p2_10_35.push_back(-1); Index_Taus_Progression_22_ShapeVeto_0p2_10_35.push_back(-1);
      // std::vector<Float_t> pt_Taus_Progression_22_ShapeVeto_0p2_10_35;
      // pt_Taus_Progression_22_ShapeVeto_0p2_10_35.push_back(-99.); pt_Taus_Progression_22_ShapeVeto_0p2_10_35.push_back(-99.);
      // std::vector<Float_t> eta_Taus_Progression_22_ShapeVeto_0p2_10_35;
      // eta_Taus_Progression_22_ShapeVeto_0p2_10_35.push_back(-99.); eta_Taus_Progression_22_ShapeVeto_0p2_10_35.push_back(-99.);

      // std::vector<Int_t> Index_Taus_Progression_22_ShapeVeto_0p5_20_45;
      // Index_Taus_Progression_22_ShapeVeto_0p5_20_45.push_back(-1); Index_Taus_Progression_22_ShapeVeto_0p5_20_45.push_back(-1);
      // std::vector<Float_t> pt_Taus_Progression_22_ShapeVeto_0p5_20_45;
      // pt_Taus_Progression_22_ShapeVeto_0p5_20_45.push_back(-99.); pt_Taus_Progression_22_ShapeVeto_0p5_20_45.push_back(-99.);
      // std::vector<Float_t> eta_Taus_Progression_22_ShapeVeto_0p5_20_45;
      // eta_Taus_Progression_22_ShapeVeto_0p5_20_45.push_back(-99.); eta_Taus_Progression_22_ShapeVeto_0p5_20_45.push_back(-99.);

      // std::vector<Int_t> Index_Taus_Progression_22_ShapeVeto_0p2_25_60;
      // Index_Taus_Progression_22_ShapeVeto_0p2_25_60.push_back(-1); Index_Taus_Progression_22_ShapeVeto_0p2_25_60.push_back(-1);
      // std::vector<Float_t> pt_Taus_Progression_22_ShapeVeto_0p2_25_60;
      // pt_Taus_Progression_22_ShapeVeto_0p2_25_60.push_back(-99.); pt_Taus_Progression_22_ShapeVeto_0p2_25_60.push_back(-99.);
      // std::vector<Float_t> eta_Taus_Progression_22_ShapeVeto_0p2_25_60;
      // eta_Taus_Progression_22_ShapeVeto_0p2_25_60.push_back(-99.); eta_Taus_Progression_22_ShapeVeto_0p2_25_60.push_back(-99.);

      // std::vector<Int_t> Index_Taus_Progression_22_ShapeVeto_0p2_20_100;
      // Index_Taus_Progression_22_ShapeVeto_0p2_20_100.push_back(-1); Index_Taus_Progression_22_ShapeVeto_0p2_20_100.push_back(-1);
      // std::vector<Float_t> pt_Taus_Progression_22_ShapeVeto_0p2_20_100;
      // pt_Taus_Progression_22_ShapeVeto_0p2_20_100.push_back(-99.); pt_Taus_Progression_22_ShapeVeto_0p2_20_100.push_back(-99.);
      // std::vector<Float_t> eta_Taus_Progression_22_ShapeVeto_0p2_20_100;
      // eta_Taus_Progression_22_ShapeVeto_0p2_20_100.push_back(-99.); eta_Taus_Progression_22_ShapeVeto_0p2_20_100.push_back(-99.);

      // std::vector<Int_t> Index_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy;
      // Index_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.push_back(-1); Index_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.push_back(-1);
      // std::vector<Float_t> pt_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy;
      // pt_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.push_back(-99.); pt_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.push_back(-99.);
      // std::vector<Float_t> eta_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy;
      // eta_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.push_back(-99.); eta_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_23;
      Index_Taus_Progression_23.push_back(-1); Index_Taus_Progression_23.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_23;
      pt_Taus_Progression_23.push_back(-99.); pt_Taus_Progression_23.push_back(-99.);
      std::vector<Float_t> eta_Taus_Progression_23;
      eta_Taus_Progression_23.push_back(-99.); eta_Taus_Progression_23.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_24;
      Index_Taus_Progression_24.push_back(-1); Index_Taus_Progression_24.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_24;
      pt_Taus_Progression_24.push_back(-99.); pt_Taus_Progression_24.push_back(-99.);
      std::vector<Float_t> eta_Taus_Progression_24;
      eta_Taus_Progression_24.push_back(-99.); eta_Taus_Progression_24.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_25;
      Index_Taus_Progression_25.push_back(-1); Index_Taus_Progression_25.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_25;
      pt_Taus_Progression_25.push_back(-99.); pt_Taus_Progression_25.push_back(-99.);
      std::vector<Float_t> eta_Taus_Progression_25;
      eta_Taus_Progression_25.push_back(-99.); eta_Taus_Progression_25.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_26;
      Index_Taus_Progression_26.push_back(-1); Index_Taus_Progression_26.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_26;
      pt_Taus_Progression_26.push_back(-99.); pt_Taus_Progression_26.push_back(-99.);
      std::vector<Float_t> eta_Taus_Progression_26;
      eta_Taus_Progression_26.push_back(-99.); eta_Taus_Progression_26.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_27;
      Index_Taus_Progression_27.push_back(-1); Index_Taus_Progression_27.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_27;
      pt_Taus_Progression_27.push_back(-99.); pt_Taus_Progression_27.push_back(-99.);
      std::vector<Float_t> eta_Taus_Progression_27;
      eta_Taus_Progression_27.push_back(-99.); eta_Taus_Progression_27.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_28;
      Index_Taus_Progression_28.push_back(-1); Index_Taus_Progression_28.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_28;
      pt_Taus_Progression_28.push_back(-99.); pt_Taus_Progression_28.push_back(-99.);
      std::vector<Float_t> eta_Taus_Progression_28;
      eta_Taus_Progression_28.push_back(-99.); eta_Taus_Progression_28.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_29;
      Index_Taus_Progression_29.push_back(-1); Index_Taus_Progression_29.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_29;
      pt_Taus_Progression_29.push_back(-99.); pt_Taus_Progression_29.push_back(-99.);
      std::vector<Float_t> eta_Taus_Progression_29;
      eta_Taus_Progression_29.push_back(-99.); eta_Taus_Progression_29.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_30;
      Index_Taus_Progression_30.push_back(-1); Index_Taus_Progression_30.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_30;
      pt_Taus_Progression_30.push_back(-99.); pt_Taus_Progression_30.push_back(-99.);
      std::vector<Float_t> eta_Taus_Progression_30;
      eta_Taus_Progression_30.push_back(-99.); eta_Taus_Progression_30.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_31;
      Index_Taus_Progression_31.push_back(-1); Index_Taus_Progression_31.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_31;
      pt_Taus_Progression_31.push_back(-99.); pt_Taus_Progression_31.push_back(-99.);
      std::vector<Float_t> eta_Taus_Progression_31;
      eta_Taus_Progression_31.push_back(-99.); eta_Taus_Progression_31.push_back(-99.);

      std::vector<Int_t> Index_Taus_Progression_31_extrap;
      Index_Taus_Progression_31_extrap.push_back(-1); Index_Taus_Progression_31_extrap.push_back(-1);
      std::vector<Float_t> pt_Taus_Progression_31_extrap;
      pt_Taus_Progression_31_extrap.push_back(-99.); pt_Taus_Progression_31_extrap.push_back(-99.);
      std::vector<Float_t> eta_Taus_Progression_31_extrap;
      eta_Taus_Progression_31_extrap.push_back(-99.); eta_Taus_Progression_31_extrap.push_back(-99.);


      for(UInt_t iL1Tau = 0 ; iL1Tau < in_CalibPt->size() ; ++iL1Tau)
        {
          if(fabs(in_l1tEmuEta->at(iL1Tau))>2.1315) continue;

          Int_t IsoCut_50pc = histosIsolation["LUT_WP50"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          Int_t IsoCut_60pc = histosIsolation["LUT_WP60"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          Int_t IsoCut_70pc = histosIsolation["LUT_WP70"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          Int_t IsoCut_80pc = histosIsolation["LUT_WP80"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          Int_t IsoCut_90pc = histosIsolation["LUT_WP90"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

          TString Result_Progression_1 = "LUT_Progression_1";
          Int_t IsoCut_Progression_1 = histosIsolation[Result_Progression_1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_1)
            {
              pt_Progression_1->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_1 = kTRUE;
            }

          TString Result_Progression_2 = "LUT_Progression_2";
          Int_t IsoCut_Progression_2 = histosIsolation[Result_Progression_2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_2)
            {
              pt_Progression_2->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_2 = kTRUE;
            }

          TString Result_Progression_3 = "LUT_Progression_3";
          Int_t IsoCut_Progression_3 = histosIsolation[Result_Progression_3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_3)
            {
              pt_Progression_3->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_3 = kTRUE;
            }

          TString Result_Progression_4 = "LUT_Progression_4";
          Int_t IsoCut_Progression_4 = histosIsolation[Result_Progression_4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_4)
            {
              pt_Progression_4->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_4 = kTRUE;
            }

          TString Result_Progression_5 = "LUT_Progression_5";
          Int_t IsoCut_Progression_5 = histosIsolation[Result_Progression_5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_5)
            {
              pt_Progression_5->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_5 = kTRUE;
            }

          TString Result_Progression_6 = "LUT_Progression_6";
          Int_t IsoCut_Progression_6 = histosIsolation[Result_Progression_6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_6)
            {
              pt_Progression_6->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_6 = kTRUE;
            }

          TString Result_Progression_7 = "LUT_Progression_7";
          Int_t IsoCut_Progression_7 = histosIsolation[Result_Progression_7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_7)
            {
              pt_Progression_7->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_7 = kTRUE;
            }

          TString Result_Progression_8 = "LUT_Progression_8";
          Int_t IsoCut_Progression_8 = histosIsolation[Result_Progression_8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_8)
            {
              pt_Progression_8->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_8 = kTRUE;
            }

          TString Result_Progression_9 = "LUT_Progression_9";
          Int_t IsoCut_Progression_9 = histosIsolation[Result_Progression_9]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_9 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_9)
            {
              pt_Progression_9->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_9 = kTRUE;
            }

          TString Result_Progression_10 = "LUT_Progression_10";
          Int_t IsoCut_Progression_10 = histosIsolation[Result_Progression_10]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_10 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_10)
            {
              pt_Progression_10->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_10 = kTRUE;
            }

          TString Result_Progression_11 = "LUT_Progression_11";
          Int_t IsoCut_Progression_11 = histosIsolation[Result_Progression_11]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_11 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_11)
            {
              pt_Progression_11->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_11 = kTRUE;
            }

          TString Result_Progression_12 = "LUT_Progression_12";
          Int_t IsoCut_Progression_12 = histosIsolation[Result_Progression_12]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_12 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_12)
            {
              pt_Progression_12->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_12 = kTRUE;
            }

          TString Result_Progression_13 = "LUT_Progression_13";
          Int_t IsoCut_Progression_13 = histosIsolation[Result_Progression_13]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_13 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_13)
            {
              pt_Progression_13->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_13 = kTRUE;
            }

          TString Result_Progression_14 = "LUT_Progression_14";
          Int_t IsoCut_Progression_14 = histosIsolation[Result_Progression_14]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_14 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_14)
            {
              pt_Progression_14->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_14 = kTRUE;
            }

          TString Result_Progression_15 = "LUT_Progression_15";
          Int_t IsoCut_Progression_15 = histosIsolation[Result_Progression_15]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_15 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_15)
            {
              pt_Progression_15->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_15 = kTRUE;
            }

          TString Result_Progression_16 = "LUT_Progression_16";
          Int_t IsoCut_Progression_16 = histosIsolation[Result_Progression_16]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_16 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_16)
            {
              pt_Progression_16->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_16 = kTRUE;
            }

          TString Result_Progression_17 = "LUT_Progression_17";
          Int_t IsoCut_Progression_17 = histosIsolation[Result_Progression_17]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_17 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_17)
            {
              pt_Progression_17->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_17 = kTRUE;
            }

          TString Result_Progression_18 = "LUT_Progression_18";
          Int_t IsoCut_Progression_18 = histosIsolation[Result_Progression_18]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_18 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_18)
            {
              pt_Progression_18->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_18 = kTRUE;
            }

          TString Result_Progression_19 = "LUT_Progression_19";
          Int_t IsoCut_Progression_19 = histosIsolation[Result_Progression_19]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_19 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_19)
            {
              pt_Progression_19->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_19 = kTRUE;
            }

          TString Result_Progression_20 = "LUT_Progression_20";
          Int_t IsoCut_Progression_20 = histosIsolation[Result_Progression_20]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_20 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_20)
            {
              pt_Progression_20->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_20 = kTRUE;
            }

          TString Result_Progression_21 = "LUT_Progression_21";
          Int_t IsoCut_Progression_21 = histosIsolation[Result_Progression_21]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_21 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_21)
            {
              pt_Progression_21->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_21 = kTRUE;
            }

          TString Result_Progression_22 = "LUT_Progression_22";
          Int_t IsoCut_Progression_22 = histosIsolation[Result_Progression_22]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
          if(!Filled_Progression_22 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22)
            {
              pt_Progression_22->Fill(in_CalibPt->at(iL1Tau));
              Filled_Progression_22 = kTRUE;
            }

          TString Result_Progression_23 = "LUT_Progression_23";
          Int_t IsoCut_Progression_23 = histosIsolation[Result_Progression_23]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

          TString Result_Progression_24 = "LUT_Progression_24";
          Int_t IsoCut_Progression_24 = histosIsolation[Result_Progression_24]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

          TString Result_Progression_25 = "LUT_Progression_25";
          Int_t IsoCut_Progression_25 = histosIsolation[Result_Progression_25]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

          TString Result_Progression_26 = "LUT_Progression_26";
          Int_t IsoCut_Progression_26 = histosIsolation[Result_Progression_26]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

          TString Result_Progression_27 = "LUT_Progression_27";
          Int_t IsoCut_Progression_27 = histosIsolation[Result_Progression_27]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

          TString Result_Progression_28 = "LUT_Progression_28";
          Int_t IsoCut_Progression_28 = histosIsolation[Result_Progression_28]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

          TString Result_Progression_29 = "LUT_Progression_29";
          Int_t IsoCut_Progression_29 = histosIsolation[Result_Progression_29]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

          TString Result_Progression_30 = "LUT_Progression_30";
          Int_t IsoCut_Progression_30 = histosIsolation[Result_Progression_30]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

          TString Result_Progression_31 = "LUT_Progression_31";
          Int_t IsoCut_Progression_31 = histosIsolation[Result_Progression_31]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

          TString Result_Progression_31_extrap = "LUT_Progression_31_extrap";
          Int_t IsoCut_Progression_31_extrap = histosIsolation[Result_Progression_31_extrap]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

          //DiTau trigger
          if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf.at(0))
            {
              Index_Taus_IsoInf.at(1)=Index_Taus_IsoInf.at(0);
              pt_Taus_IsoInf.at(1)=pt_Taus_IsoInf.at(0);
              Index_Taus_IsoInf.at(0)=iL1Tau;
              pt_Taus_IsoInf.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf.at(1))
            {
              Index_Taus_IsoInf.at(1)=iL1Tau;
              pt_Taus_IsoInf.at(1)=in_CalibPt->at(iL1Tau);
            }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto.at(0) && in_L1Tau_PassShapeVeto->at(iL1Tau))
          //   {
          //     Index_Taus_IsoInf_ShapeVeto.at(1)=Index_Taus_IsoInf_ShapeVeto.at(0);
          //     pt_Taus_IsoInf_ShapeVeto.at(1)=pt_Taus_IsoInf_ShapeVeto.at(0);
          //     Index_Taus_IsoInf_ShapeVeto.at(0)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto.at(1) && in_L1Tau_PassShapeVeto->at(iL1Tau))
          //   {
          //     Index_Taus_IsoInf_ShapeVeto.at(1)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_90.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP90"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
            {
              Index_Taus_90.at(1)=Index_Taus_90.at(0);
              pt_Taus_90.at(1)=pt_Taus_90.at(0);
              Index_Taus_90.at(0)=iL1Tau;
              pt_Taus_90.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_90.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP90"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
            {
              Index_Taus_90.at(1)=iL1Tau;
              pt_Taus_90.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_80.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP80"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
            {
              Index_Taus_80.at(1)=Index_Taus_80.at(0);
              pt_Taus_80.at(1)=pt_Taus_80.at(0);
              Index_Taus_80.at(0)=iL1Tau;
              pt_Taus_80.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_80.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP80"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
            {
              Index_Taus_80.at(1)=iL1Tau;
              pt_Taus_80.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_70.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP70"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
            {
              Index_Taus_70.at(1)=Index_Taus_70.at(0);
              pt_Taus_70.at(1)=pt_Taus_70.at(0);
              Index_Taus_70.at(0)=iL1Tau;
              pt_Taus_70.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_70.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP70"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
            {
              Index_Taus_70.at(1)=iL1Tau;
              pt_Taus_70.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_60.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP60"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
            {
              Index_Taus_60.at(1)=Index_Taus_60.at(0);
              pt_Taus_60.at(1)=pt_Taus_60.at(0);
              Index_Taus_60.at(0)=iL1Tau;
              pt_Taus_60.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_60.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP60"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
            {
              Index_Taus_60.at(1)=iL1Tau;
              pt_Taus_60.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_50.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP50"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
            {
              Index_Taus_50.at(1)=Index_Taus_50.at(0);
              pt_Taus_50.at(1)=pt_Taus_50.at(0);
              Index_Taus_50.at(0)=iL1Tau;
              pt_Taus_50.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_50.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP50"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
            {
              Index_Taus_50.at(1)=iL1Tau;
              pt_Taus_50.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_1)
            {
              Index_Taus_Progression_1.at(1)=Index_Taus_Progression_1.at(0);
              pt_Taus_Progression_1.at(1)=pt_Taus_Progression_1.at(0);
              Index_Taus_Progression_1.at(0)=iL1Tau;
              pt_Taus_Progression_1.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_1)
            {
              Index_Taus_Progression_1.at(1)=iL1Tau;
              pt_Taus_Progression_1.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_2)
            {
              Index_Taus_Progression_2.at(1)=Index_Taus_Progression_2.at(0);
              pt_Taus_Progression_2.at(1)=pt_Taus_Progression_2.at(0);
              Index_Taus_Progression_2.at(0)=iL1Tau;
              pt_Taus_Progression_2.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_2)
            {
              Index_Taus_Progression_2.at(1)=iL1Tau;
              pt_Taus_Progression_2.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_3)
            {
              Index_Taus_Progression_3.at(1)=Index_Taus_Progression_3.at(0);
              pt_Taus_Progression_3.at(1)=pt_Taus_Progression_3.at(0);
              Index_Taus_Progression_3.at(0)=iL1Tau;
              pt_Taus_Progression_3.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_3)
            {
              Index_Taus_Progression_3.at(1)=iL1Tau;
              pt_Taus_Progression_3.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_4)
            {
              Index_Taus_Progression_4.at(1)=Index_Taus_Progression_4.at(0);
              pt_Taus_Progression_4.at(1)=pt_Taus_Progression_4.at(0);
              Index_Taus_Progression_4.at(0)=iL1Tau;
              pt_Taus_Progression_4.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_4)
            {
              Index_Taus_Progression_4.at(1)=iL1Tau;
              pt_Taus_Progression_4.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_5)
            {
              Index_Taus_Progression_5.at(1)=Index_Taus_Progression_5.at(0);
              pt_Taus_Progression_5.at(1)=pt_Taus_Progression_5.at(0);
              Index_Taus_Progression_5.at(0)=iL1Tau;
              pt_Taus_Progression_5.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_5)
            {
              Index_Taus_Progression_5.at(1)=iL1Tau;
              pt_Taus_Progression_5.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_6)
            {
              Index_Taus_Progression_6.at(1)=Index_Taus_Progression_6.at(0);
              pt_Taus_Progression_6.at(1)=pt_Taus_Progression_6.at(0);
              Index_Taus_Progression_6.at(0)=iL1Tau;
              pt_Taus_Progression_6.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_6)
            {
              Index_Taus_Progression_6.at(1)=iL1Tau;
              pt_Taus_Progression_6.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_7)
            {
              Index_Taus_Progression_7.at(1)=Index_Taus_Progression_7.at(0);
              pt_Taus_Progression_7.at(1)=pt_Taus_Progression_7.at(0);
              Index_Taus_Progression_7.at(0)=iL1Tau;
              pt_Taus_Progression_7.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_7)
            {
              Index_Taus_Progression_7.at(1)=iL1Tau;
              pt_Taus_Progression_7.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_8)
            {
              Index_Taus_Progression_8.at(1)=Index_Taus_Progression_8.at(0);
              pt_Taus_Progression_8.at(1)=pt_Taus_Progression_8.at(0);
              Index_Taus_Progression_8.at(0)=iL1Tau;
              pt_Taus_Progression_8.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_8)
            {
              Index_Taus_Progression_8.at(1)=iL1Tau;
              pt_Taus_Progression_8.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_9.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_9)
            {
              Index_Taus_Progression_9.at(1)=Index_Taus_Progression_9.at(0);
              pt_Taus_Progression_9.at(1)=pt_Taus_Progression_9.at(0);
              Index_Taus_Progression_9.at(0)=iL1Tau;
              pt_Taus_Progression_9.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_9.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_9)
            {
              Index_Taus_Progression_9.at(1)=iL1Tau;
              pt_Taus_Progression_9.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_10.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_10)
            {
              Index_Taus_Progression_10.at(1)=Index_Taus_Progression_10.at(0);
              pt_Taus_Progression_10.at(1)=pt_Taus_Progression_10.at(0);
              Index_Taus_Progression_10.at(0)=iL1Tau;
              pt_Taus_Progression_10.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_10.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_10)
            {
              Index_Taus_Progression_10.at(1)=iL1Tau;
              pt_Taus_Progression_10.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_11.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_11)
            {
              Index_Taus_Progression_11.at(1)=Index_Taus_Progression_11.at(0);
              pt_Taus_Progression_11.at(1)=pt_Taus_Progression_11.at(0);
              Index_Taus_Progression_11.at(0)=iL1Tau;
              pt_Taus_Progression_11.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_11.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_11)
            {
              Index_Taus_Progression_11.at(1)=iL1Tau;
              pt_Taus_Progression_11.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_12.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_12)
            {
              Index_Taus_Progression_12.at(1)=Index_Taus_Progression_12.at(0);
              pt_Taus_Progression_12.at(1)=pt_Taus_Progression_12.at(0);
              Index_Taus_Progression_12.at(0)=iL1Tau;
              pt_Taus_Progression_12.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_12.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_12)
            {
              Index_Taus_Progression_12.at(1)=iL1Tau;
              pt_Taus_Progression_12.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_13.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_13)
            {
              Index_Taus_Progression_13.at(1)=Index_Taus_Progression_13.at(0);
              pt_Taus_Progression_13.at(1)=pt_Taus_Progression_13.at(0);
              Index_Taus_Progression_13.at(0)=iL1Tau;
              pt_Taus_Progression_13.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_13.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_13)
            {
              Index_Taus_Progression_13.at(1)=iL1Tau;
              pt_Taus_Progression_13.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_14.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_14)
            {
              Index_Taus_Progression_14.at(1)=Index_Taus_Progression_14.at(0);
              pt_Taus_Progression_14.at(1)=pt_Taus_Progression_14.at(0);
              Index_Taus_Progression_14.at(0)=iL1Tau;
              pt_Taus_Progression_14.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_14.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_14)
            {
              Index_Taus_Progression_14.at(1)=iL1Tau;
              pt_Taus_Progression_14.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_15.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_15)
            {
              Index_Taus_Progression_15.at(1)=Index_Taus_Progression_15.at(0);
              pt_Taus_Progression_15.at(1)=pt_Taus_Progression_15.at(0);
              Index_Taus_Progression_15.at(0)=iL1Tau;
              pt_Taus_Progression_15.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_15.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_15)
            {
              Index_Taus_Progression_15.at(1)=iL1Tau;
              pt_Taus_Progression_15.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_16.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_16)
            {
              Index_Taus_Progression_16.at(1)=Index_Taus_Progression_16.at(0);
              pt_Taus_Progression_16.at(1)=pt_Taus_Progression_16.at(0);
              Index_Taus_Progression_16.at(0)=iL1Tau;
              pt_Taus_Progression_16.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_16.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_16)
            {
              Index_Taus_Progression_16.at(1)=iL1Tau;
              pt_Taus_Progression_16.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_17.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_17)
            {
              Index_Taus_Progression_17.at(1)=Index_Taus_Progression_17.at(0);
              pt_Taus_Progression_17.at(1)=pt_Taus_Progression_17.at(0);
              Index_Taus_Progression_17.at(0)=iL1Tau;
              pt_Taus_Progression_17.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_17.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_17)
            {
              Index_Taus_Progression_17.at(1)=iL1Tau;
              pt_Taus_Progression_17.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_18.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_18)
            {
              Index_Taus_Progression_18.at(1)=Index_Taus_Progression_18.at(0);
              pt_Taus_Progression_18.at(1)=pt_Taus_Progression_18.at(0);
              Index_Taus_Progression_18.at(0)=iL1Tau;
              pt_Taus_Progression_18.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_18.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_18)
            {
              Index_Taus_Progression_18.at(1)=iL1Tau;
              pt_Taus_Progression_18.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_19.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_19)
            {
              Index_Taus_Progression_19.at(1)=Index_Taus_Progression_19.at(0);
              pt_Taus_Progression_19.at(1)=pt_Taus_Progression_19.at(0);
              Index_Taus_Progression_19.at(0)=iL1Tau;
              pt_Taus_Progression_19.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_19.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_19)
            {
              Index_Taus_Progression_19.at(1)=iL1Tau;
              pt_Taus_Progression_19.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_20.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_20)
            {
              Index_Taus_Progression_20.at(1)=Index_Taus_Progression_20.at(0);
              pt_Taus_Progression_20.at(1)=pt_Taus_Progression_20.at(0);
              Index_Taus_Progression_20.at(0)=iL1Tau;
              pt_Taus_Progression_20.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_20.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_20)
            {
              Index_Taus_Progression_20.at(1)=iL1Tau;
              pt_Taus_Progression_20.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_21.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_21)
            {
              Index_Taus_Progression_21.at(1)=Index_Taus_Progression_21.at(0);
              pt_Taus_Progression_21.at(1)=pt_Taus_Progression_21.at(0);
              Index_Taus_Progression_21.at(0)=iL1Tau;
              pt_Taus_Progression_21.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_21.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_21)
            {
              Index_Taus_Progression_21.at(1)=iL1Tau;
              pt_Taus_Progression_21.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22)
            {
              Index_Taus_Progression_22.at(1)=Index_Taus_Progression_22.at(0);
              pt_Taus_Progression_22.at(1)=pt_Taus_Progression_22.at(0);
              eta_Taus_Progression_22.at(1)=eta_Taus_Progression_22.at(0);
              isMerged_Taus_Progression_22.at(1)=isMerged_Taus_Progression_22.at(0);
              Index_Taus_Progression_22.at(0)=iL1Tau;
              pt_Taus_Progression_22.at(0)=in_CalibPt->at(iL1Tau);
              eta_Taus_Progression_22.at(0)=in_l1tEmuEta->at(iL1Tau);
              isMerged_Taus_Progression_22.at(0)=in_l1tEmuIsMerged->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22)
            {
              Index_Taus_Progression_22.at(1)=iL1Tau;
              pt_Taus_Progression_22.at(1)=in_CalibPt->at(iL1Tau);
              eta_Taus_Progression_22.at(1)=in_l1tEmuEta->at(iL1Tau);
              isMerged_Taus_Progression_22.at(1)=in_l1tEmuIsMerged->at(iL1Tau);
            }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && in_L1Tau_PassShapeVeto->at(iL1Tau))
          //   {
          //     Index_Taus_Progression_22_ShapeVeto.at(1)=Index_Taus_Progression_22_ShapeVeto.at(0);
          //     pt_Taus_Progression_22_ShapeVeto.at(1)=pt_Taus_Progression_22_ShapeVeto.at(0);
          //     Index_Taus_Progression_22_ShapeVeto.at(0)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && in_L1Tau_PassShapeVeto->at(iL1Tau))
          //   {
          //     Index_Taus_Progression_22_ShapeVeto.at(1)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto.at(1)=in_CalibPt->at(iL1Tau);
          //   }



          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_23.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_23)
            {
              Index_Taus_Progression_23.at(1)=Index_Taus_Progression_23.at(0);
              pt_Taus_Progression_23.at(1)=pt_Taus_Progression_23.at(0);
              Index_Taus_Progression_23.at(0)=iL1Tau;
              pt_Taus_Progression_23.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_23.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_23)
            {
              Index_Taus_Progression_23.at(1)=iL1Tau;
              pt_Taus_Progression_23.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_24.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_24)
            {
              Index_Taus_Progression_24.at(1)=Index_Taus_Progression_24.at(0);
              pt_Taus_Progression_24.at(1)=pt_Taus_Progression_24.at(0);
              Index_Taus_Progression_24.at(0)=iL1Tau;
              pt_Taus_Progression_24.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_24.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_24)
            {
              Index_Taus_Progression_24.at(1)=iL1Tau;
              pt_Taus_Progression_24.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_25.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_25)
            {
              Index_Taus_Progression_25.at(1)=Index_Taus_Progression_25.at(0);
              pt_Taus_Progression_25.at(1)=pt_Taus_Progression_25.at(0);
              Index_Taus_Progression_25.at(0)=iL1Tau;
              pt_Taus_Progression_25.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_25.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_25)
            {
              Index_Taus_Progression_25.at(1)=iL1Tau;
              pt_Taus_Progression_25.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_26.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_26)
            {
              Index_Taus_Progression_26.at(1)=Index_Taus_Progression_26.at(0);
              pt_Taus_Progression_26.at(1)=pt_Taus_Progression_26.at(0);
              Index_Taus_Progression_26.at(0)=iL1Tau;
              pt_Taus_Progression_26.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_26.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_26)
            {
              Index_Taus_Progression_26.at(1)=iL1Tau;
              pt_Taus_Progression_26.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_27.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_27)
            {
              Index_Taus_Progression_27.at(1)=Index_Taus_Progression_27.at(0);
              pt_Taus_Progression_27.at(1)=pt_Taus_Progression_27.at(0);
              Index_Taus_Progression_27.at(0)=iL1Tau;
              pt_Taus_Progression_27.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_27.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_27)
            {
              Index_Taus_Progression_27.at(1)=iL1Tau;
              pt_Taus_Progression_27.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_28.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_28)
            {
              Index_Taus_Progression_28.at(1)=Index_Taus_Progression_28.at(0);
              pt_Taus_Progression_28.at(1)=pt_Taus_Progression_28.at(0);
              Index_Taus_Progression_28.at(0)=iL1Tau;
              pt_Taus_Progression_28.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_28.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_28)
            {
              Index_Taus_Progression_28.at(1)=iL1Tau;
              pt_Taus_Progression_28.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_29.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_29)
            {
              Index_Taus_Progression_29.at(1)=Index_Taus_Progression_29.at(0);
              pt_Taus_Progression_29.at(1)=pt_Taus_Progression_29.at(0);
              Index_Taus_Progression_29.at(0)=iL1Tau;
              pt_Taus_Progression_29.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_29.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_29)
            {
              Index_Taus_Progression_29.at(1)=iL1Tau;
              pt_Taus_Progression_29.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_30.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_30)
            {
              Index_Taus_Progression_30.at(1)=Index_Taus_Progression_30.at(0);
              pt_Taus_Progression_30.at(1)=pt_Taus_Progression_30.at(0);
              Index_Taus_Progression_30.at(0)=iL1Tau;
              pt_Taus_Progression_30.at(0)=in_CalibPt->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_30.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_30)
            {
              Index_Taus_Progression_30.at(1)=iL1Tau;
              pt_Taus_Progression_30.at(1)=in_CalibPt->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_31.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_31)
            {
              Index_Taus_Progression_31.at(1)=Index_Taus_Progression_31.at(0);
              pt_Taus_Progression_31.at(1)=pt_Taus_Progression_31.at(0);
              eta_Taus_Progression_31.at(1)=eta_Taus_Progression_31.at(0);
              Index_Taus_Progression_31.at(0)=iL1Tau;
              pt_Taus_Progression_31.at(0)=in_CalibPt->at(iL1Tau);
              eta_Taus_Progression_31.at(0)=in_l1tEmuEta->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_31.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_31)
            {
              Index_Taus_Progression_31.at(1)=iL1Tau;
              pt_Taus_Progression_31.at(1)=in_CalibPt->at(iL1Tau);
              eta_Taus_Progression_31.at(1)=in_l1tEmuEta->at(iL1Tau);
            }

          if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_31_extrap.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_31_extrap)
            {
              Index_Taus_Progression_31_extrap.at(1)=Index_Taus_Progression_31_extrap.at(0);
              pt_Taus_Progression_31_extrap.at(1)=pt_Taus_Progression_31_extrap.at(0);
              eta_Taus_Progression_31_extrap.at(1)=eta_Taus_Progression_31_extrap.at(0);
              Index_Taus_Progression_31_extrap.at(0)=iL1Tau;
              pt_Taus_Progression_31_extrap.at(0)=in_CalibPt->at(iL1Tau);
              eta_Taus_Progression_31_extrap.at(0)=in_l1tEmuEta->at(iL1Tau);
            }
          else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_31_extrap.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_31_extrap)
            {
              Index_Taus_Progression_31_extrap.at(1)=iL1Tau;
              pt_Taus_Progression_31_extrap.at(1)=in_CalibPt->at(iL1Tau);
              eta_Taus_Progression_31_extrap.at(1)=in_l1tEmuEta->at(iL1Tau);
            }

          // short int symmShape0 = getSymmShape (in_l1tEmuQual->at(iL1Tau), remap);
          // Bool_t out_L1Tau_PassShapeVeto_0p6_10_50 = false;
          // Bool_t out_L1Tau_PassShapeVeto_0p4_10_35 = false;
          // Bool_t out_L1Tau_PassShapeVeto_0p2_10_35 = false;
          // Bool_t out_L1Tau_PassShapeVeto_0p5_20_45 = false;
          // Bool_t out_L1Tau_PassShapeVeto_0p2_25_60 = false;
          // Bool_t out_L1Tau_PassShapeVeto_0p2_20_100 = false;
          // Bool_t out_L1Tau_PassShapeVeto_0p2_20_100_dummy = false;
          // Bool_t out_L1Tau_PassShapeVeto_0p5_10_70 = false;
          // Bool_t out_L1Tau_PassShapeVeto_0p4_10_70 = false;
          // Bool_t out_L1Tau_PassShapeVeto_0p3_10_70 = false;
          // Bool_t out_L1Tau_PassShapeVeto_0p3_10_100 = false;

          // if(TwoDShapeVetos_0p6_10_50->GetBinContent(symmShape0+1,in_compressedE->at(iL1Tau)+1)==1) out_L1Tau_PassShapeVeto_0p6_10_50 = true;
          // else out_L1Tau_PassShapeVeto_0p6_10_50 = false;

          // if(TwoDShapeVetos_0p4_10_35->GetBinContent(symmShape0+1,in_compressedE->at(iL1Tau)+1)==1) out_L1Tau_PassShapeVeto_0p4_10_35 = true;
          // else out_L1Tau_PassShapeVeto_0p4_10_35 = false;

          // if(TwoDShapeVetos_0p2_10_35->GetBinContent(symmShape0+1,in_compressedE->at(iL1Tau)+1)==1) out_L1Tau_PassShapeVeto_0p2_10_35 = true;
          // else out_L1Tau_PassShapeVeto_0p2_10_35 = false;

          // if(TwoDShapeVetos_0p5_20_45->GetBinContent(symmShape0+1,in_compressedE->at(iL1Tau)+1)==1) out_L1Tau_PassShapeVeto_0p5_20_45 = true;
          // else out_L1Tau_PassShapeVeto_0p5_20_45 = false;

          // if(TwoDShapeVetos_0p2_25_60->GetBinContent(symmShape0+1,in_compressedE->at(iL1Tau)+1)==1) out_L1Tau_PassShapeVeto_0p2_25_60 = true;
          // else out_L1Tau_PassShapeVeto_0p2_25_60 = false;

          // if(TwoDShapeVetos_0p2_20_100->GetBinContent(symmShape0+1,in_compressedE->at(iL1Tau)+1)==1) out_L1Tau_PassShapeVeto_0p2_20_100 = true;
          // else out_L1Tau_PassShapeVeto_0p2_20_100 = false;

          // if(TwoDShapeVetos_0p2_20_100_dummy->GetBinContent(symmShape0,in_compressedE->at(iL1Tau)+1)==1) out_L1Tau_PassShapeVeto_0p2_20_100_dummy = true;
          // else out_L1Tau_PassShapeVeto_0p2_20_100_dummy = false;

          // if(TwoDShapeVetos_0p5_10_70->GetBinContent(symmShape0+1,in_compressedE->at(iL1Tau)+1)==1) out_L1Tau_PassShapeVeto_0p5_10_70 = true;
          // else out_L1Tau_PassShapeVeto_0p5_10_70 = false;

          // if(TwoDShapeVetos_0p4_10_70->GetBinContent(symmShape0+1,in_compressedE->at(iL1Tau)+1)==1) out_L1Tau_PassShapeVeto_0p4_10_70 = true;
          // else out_L1Tau_PassShapeVeto_0p4_10_70 = false;

          // if(TwoDShapeVetos_0p3_10_70->GetBinContent(symmShape0+1,in_compressedE->at(iL1Tau)+1)==1) out_L1Tau_PassShapeVeto_0p3_10_70 = true;
          // else out_L1Tau_PassShapeVeto_0p3_10_70 = false;

          // if(TwoDShapeVetos_0p3_10_100->GetBinContent(symmShape0+1,in_compressedE->at(iL1Tau)+1)==1) out_L1Tau_PassShapeVeto_0p3_10_100 = true;
          // else out_L1Tau_PassShapeVeto_0p3_10_100 = false;

          // if(in_compressedE->at(iL1Tau)>27) CounterAll++;
          // if(in_compressedE->at(iL1Tau)>27 && TwoDShapeVetos_0p3_10_100->GetBinContent(symmShape0+1,in_compressedE->at(iL1Tau)+1)!=1)
          //   {
          //     CounterFail++;
          //     // cout<<"in_CalibPt->at(iL1Tau) = "<<in_CalibPt->at(iL1Tau)<<endl;
          //   }
          // else if(in_compressedE->at(iL1Tau)>27 && TwoDShapeVetos_0p3_10_100->GetBinContent(symmShape0+1,in_compressedE->at(iL1Tau)+1)==1)
          //   {
          //     CounterPass++;
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p6_10_50.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p6_10_50)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p6_10_50.at(1)=Index_Taus_Progression_22_ShapeVeto_0p6_10_50.at(0);
          //     pt_Taus_Progression_22_ShapeVeto_0p6_10_50.at(1)=pt_Taus_Progression_22_ShapeVeto_0p6_10_50.at(0);
          //     Index_Taus_Progression_22_ShapeVeto_0p6_10_50.at(0)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p6_10_50.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p6_10_50.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p6_10_50)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p6_10_50.at(1)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p6_10_50.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p4_10_35.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p4_10_35)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p4_10_35.at(1)=Index_Taus_Progression_22_ShapeVeto_0p4_10_35.at(0);
          //     pt_Taus_Progression_22_ShapeVeto_0p4_10_35.at(1)=pt_Taus_Progression_22_ShapeVeto_0p4_10_35.at(0);
          //     Index_Taus_Progression_22_ShapeVeto_0p4_10_35.at(0)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p4_10_35.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p4_10_35.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p4_10_35)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p4_10_35.at(1)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p4_10_35.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p2_10_35.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p2_10_35)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p2_10_35.at(1)=Index_Taus_Progression_22_ShapeVeto_0p2_10_35.at(0);
          //     pt_Taus_Progression_22_ShapeVeto_0p2_10_35.at(1)=pt_Taus_Progression_22_ShapeVeto_0p2_10_35.at(0);
          //     Index_Taus_Progression_22_ShapeVeto_0p2_10_35.at(0)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p2_10_35.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p2_10_35.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p2_10_35)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p2_10_35.at(1)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p2_10_35.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p5_20_45.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p5_20_45)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p5_20_45.at(1)=Index_Taus_Progression_22_ShapeVeto_0p5_20_45.at(0);
          //     pt_Taus_Progression_22_ShapeVeto_0p5_20_45.at(1)=pt_Taus_Progression_22_ShapeVeto_0p5_20_45.at(0);
          //     Index_Taus_Progression_22_ShapeVeto_0p5_20_45.at(0)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p5_20_45.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p5_20_45.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p5_20_45)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p5_20_45.at(1)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p5_20_45.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p2_25_60.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p2_25_60)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p2_25_60.at(1)=Index_Taus_Progression_22_ShapeVeto_0p2_25_60.at(0);
          //     pt_Taus_Progression_22_ShapeVeto_0p2_25_60.at(1)=pt_Taus_Progression_22_ShapeVeto_0p2_25_60.at(0);
          //     Index_Taus_Progression_22_ShapeVeto_0p2_25_60.at(0)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p2_25_60.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p2_25_60.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p2_25_60)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p2_25_60.at(1)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p2_25_60.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p2_20_100.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p2_20_100)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p2_20_100.at(1)=Index_Taus_Progression_22_ShapeVeto_0p2_20_100.at(0);
          //     pt_Taus_Progression_22_ShapeVeto_0p2_20_100.at(1)=pt_Taus_Progression_22_ShapeVeto_0p2_20_100.at(0);
          //     Index_Taus_Progression_22_ShapeVeto_0p2_20_100.at(0)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p2_20_100.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p2_20_100.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p2_20_100)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p2_20_100.at(1)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p2_20_100.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p2_20_100_dummy)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(1)=Index_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(0);
          //     pt_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(1)=pt_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(0);
          //     Index_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(0)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22 && out_L1Tau_PassShapeVeto_0p2_20_100_dummy)
          //   {
          //     Index_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(1)=iL1Tau;
          //     pt_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p6_10_50.at(0) && out_L1Tau_PassShapeVeto_0p6_10_50)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p6_10_50.at(1)=Index_Taus_IsoInf_ShapeVeto_0p6_10_50.at(0);
          //     pt_Taus_IsoInf_ShapeVeto_0p6_10_50.at(1)=pt_Taus_IsoInf_ShapeVeto_0p6_10_50.at(0);
          //     Index_Taus_IsoInf_ShapeVeto_0p6_10_50.at(0)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p6_10_50.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p6_10_50.at(1) && out_L1Tau_PassShapeVeto_0p6_10_50)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p6_10_50.at(1)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p6_10_50.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p4_10_35.at(0) && out_L1Tau_PassShapeVeto_0p4_10_35)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p4_10_35.at(1)=Index_Taus_IsoInf_ShapeVeto_0p4_10_35.at(0);
          //     pt_Taus_IsoInf_ShapeVeto_0p4_10_35.at(1)=pt_Taus_IsoInf_ShapeVeto_0p4_10_35.at(0);
          //     Index_Taus_IsoInf_ShapeVeto_0p4_10_35.at(0)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p4_10_35.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p4_10_35.at(1) && out_L1Tau_PassShapeVeto_0p4_10_35)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p4_10_35.at(1)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p4_10_35.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p2_20_100.at(0) && out_L1Tau_PassShapeVeto_0p2_20_100)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p2_20_100.at(1)=Index_Taus_IsoInf_ShapeVeto_0p2_20_100.at(0);
          //     pt_Taus_IsoInf_ShapeVeto_0p2_20_100.at(1)=pt_Taus_IsoInf_ShapeVeto_0p2_20_100.at(0);
          //     Index_Taus_IsoInf_ShapeVeto_0p2_20_100.at(0)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p2_20_100.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p2_20_100.at(1) && out_L1Tau_PassShapeVeto_0p2_20_100)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p2_20_100.at(1)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p2_20_100.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(0) && out_L1Tau_PassShapeVeto_0p2_20_100_dummy)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(1)=Index_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(0);
          //     pt_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(1)=pt_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(0);
          //     Index_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(0)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(1) && out_L1Tau_PassShapeVeto_0p2_20_100_dummy)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(1)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p5_10_70.at(0) && out_L1Tau_PassShapeVeto_0p5_10_70)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p5_10_70.at(1)=Index_Taus_IsoInf_ShapeVeto_0p5_10_70.at(0);
          //     pt_Taus_IsoInf_ShapeVeto_0p5_10_70.at(1)=pt_Taus_IsoInf_ShapeVeto_0p5_10_70.at(0);
          //     Index_Taus_IsoInf_ShapeVeto_0p5_10_70.at(0)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p5_10_70.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p5_10_70.at(1) && out_L1Tau_PassShapeVeto_0p5_10_70)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p5_10_70.at(1)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p5_10_70.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p4_10_70.at(0) && out_L1Tau_PassShapeVeto_0p4_10_70)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p4_10_70.at(1)=Index_Taus_IsoInf_ShapeVeto_0p4_10_70.at(0);
          //     pt_Taus_IsoInf_ShapeVeto_0p4_10_70.at(1)=pt_Taus_IsoInf_ShapeVeto_0p4_10_70.at(0);
          //     Index_Taus_IsoInf_ShapeVeto_0p4_10_70.at(0)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p4_10_70.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p4_10_70.at(1) && out_L1Tau_PassShapeVeto_0p4_10_70)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p4_10_70.at(1)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p4_10_70.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p3_10_70.at(0) && out_L1Tau_PassShapeVeto_0p3_10_70)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p3_10_70.at(1)=Index_Taus_IsoInf_ShapeVeto_0p3_10_70.at(0);
          //     pt_Taus_IsoInf_ShapeVeto_0p3_10_70.at(1)=pt_Taus_IsoInf_ShapeVeto_0p3_10_70.at(0);
          //     Index_Taus_IsoInf_ShapeVeto_0p3_10_70.at(0)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p3_10_70.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p3_10_70.at(1) && out_L1Tau_PassShapeVeto_0p3_10_70)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p3_10_70.at(1)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p3_10_70.at(1)=in_CalibPt->at(iL1Tau);
          //   }

          // if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p3_10_100.at(0) && out_L1Tau_PassShapeVeto_0p3_10_100)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(1)=Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(0);
          //     pt_Taus_IsoInf_ShapeVeto_0p3_10_100.at(1)=pt_Taus_IsoInf_ShapeVeto_0p3_10_100.at(0);
          //     Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(0)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p3_10_100.at(0)=in_CalibPt->at(iL1Tau);
          //   }
          // else if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf_ShapeVeto_0p3_10_100.at(1) && out_L1Tau_PassShapeVeto_0p3_10_100)
          //   {
          //     Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(1)=iL1Tau;
          //     pt_Taus_IsoInf_ShapeVeto_0p3_10_100.at(1)=in_CalibPt->at(iL1Tau);
          //   }
        }
      
      Bool_t Flag1 = false;
      Bool_t Flag2 = false;

      if(Index_Taus_IsoInf.at(0)>=0 && Index_Taus_IsoInf.at(1)>=0)
        {
          pt_IsoInf_DiTau->Fill(pt_Taus_IsoInf.at(0),pt_Taus_IsoInf.at(1),weight);
          if(pt_Taus_IsoInf.at(0)>80. && pt_Taus_IsoInf.at(1)>80.)
            {
              Flag1 = true;
              DiTauCounterAll++;
              // cout<<"event passing no iso"<<endl;
            }
        }
      // if(Index_Taus_IsoInf_ShapeVeto.at(0)>=0 && Index_Taus_IsoInf_ShapeVeto.at(1)>=0)
      //   {
      //     pt_IsoInf_DiTau_ShapeVeto->Fill(pt_Taus_IsoInf_ShapeVeto.at(0),pt_Taus_IsoInf_ShapeVeto.at(1),weight);
      //   }
      // if(Index_Taus_IsoInf_ShapeVeto_0p2_20_100.at(0)>=0 && Index_Taus_IsoInf_ShapeVeto_0p2_20_100.at(1)>=0)
      //   {
      //     pt_IsoInf_DiTau_ShapeVeto_0p2_20_100->Fill(pt_Taus_IsoInf_ShapeVeto_0p2_20_100.at(0),pt_Taus_IsoInf_ShapeVeto_0p2_20_100.at(1),weight);
      //   }
      // if(Index_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(0)>=0 && Index_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(1)>=0)
      //   {
      //     pt_IsoInf_DiTau_ShapeVeto_0p2_20_100_dummy->Fill(pt_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(0),pt_Taus_IsoInf_ShapeVeto_0p2_20_100_dummy.at(1),weight);
      //   }
      // if(Index_Taus_IsoInf_ShapeVeto_0p6_10_50.at(0)>=0 && Index_Taus_IsoInf_ShapeVeto_0p6_10_50.at(1)>=0)
      //   {
      //     pt_IsoInf_DiTau_ShapeVeto_0p6_10_50->Fill(pt_Taus_IsoInf_ShapeVeto_0p6_10_50.at(0),pt_Taus_IsoInf_ShapeVeto_0p6_10_50.at(1),weight);
      //   }
      // if(Index_Taus_IsoInf_ShapeVeto_0p4_10_35.at(0)>=0 && Index_Taus_IsoInf_ShapeVeto_0p4_10_35.at(1)>=0)
      //   {
      //     pt_IsoInf_DiTau_ShapeVeto_0p4_10_35->Fill(pt_Taus_IsoInf_ShapeVeto_0p4_10_35.at(0),pt_Taus_IsoInf_ShapeVeto_0p4_10_35.at(1),weight);
      //   }
      // if(Index_Taus_IsoInf_ShapeVeto_0p5_10_70.at(0)>=0 && Index_Taus_IsoInf_ShapeVeto_0p5_10_70.at(1)>=0)
      //   {
      //     pt_IsoInf_DiTau_ShapeVeto_0p5_10_70->Fill(pt_Taus_IsoInf_ShapeVeto_0p5_10_70.at(0),pt_Taus_IsoInf_ShapeVeto_0p5_10_70.at(1),weight);
      //   }
      // if(Index_Taus_IsoInf_ShapeVeto_0p4_10_70.at(0)>=0 && Index_Taus_IsoInf_ShapeVeto_0p4_10_70.at(1)>=0)
      //   {
      //     pt_IsoInf_DiTau_ShapeVeto_0p4_10_70->Fill(pt_Taus_IsoInf_ShapeVeto_0p4_10_70.at(0),pt_Taus_IsoInf_ShapeVeto_0p4_10_70.at(1),weight);
      //   }
      // if(Index_Taus_IsoInf_ShapeVeto_0p3_10_70.at(0)>=0 && Index_Taus_IsoInf_ShapeVeto_0p3_10_70.at(1)>=0)
      //   {
      //     pt_IsoInf_DiTau_ShapeVeto_0p3_10_70->Fill(pt_Taus_IsoInf_ShapeVeto_0p3_10_70.at(0),pt_Taus_IsoInf_ShapeVeto_0p3_10_70.at(1),weight);
      //   }
      // if(Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(0)>=0 && Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(1)>=0)
      //   {
      //     pt_IsoInf_DiTau_ShapeVeto_0p3_10_100->Fill(pt_Taus_IsoInf_ShapeVeto_0p3_10_100.at(0),pt_Taus_IsoInf_ShapeVeto_0p3_10_100.at(1),weight);
      //     if(pt_Taus_IsoInf_ShapeVeto_0p3_10_100.at(0)>80. && pt_Taus_IsoInf_ShapeVeto_0p3_10_100.at(1)>80.)
      //       {
      //         short int symmShape1 = getSymmShape (in_l1tEmuQual->at(Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(0)), remap);
      //         short int symmShape2 = getSymmShape (in_l1tEmuQual->at(Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(1)), remap);


      //         cout<<"Leading shape = "<<symmShape1<<endl;
      //         cout<<"in_l1tEmuRawEt = "<<in_l1tEmuRawEt->at(Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(0))<<endl;
      //         cout<<"Leading Et bin = "<<in_compressedE->at(Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(0))<<endl;
      //         cout<<"Leading pass shape veto = "<<TwoDShapeVetos_0p3_10_100->GetBinContent(symmShape1+1,in_compressedE->at(Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(0))+1)<<endl;
      //         cout<<"SubLeading shape = "<<symmShape2<<endl;
      //         cout<<"in_l1tEmuRawEt = "<<in_l1tEmuRawEt->at(Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(1))<<endl;
      //         cout<<"SubLeading Et bin = "<<in_compressedE->at(Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(1))<<endl;
      //         cout<<"SubLeading pass shape veto = "<<TwoDShapeVetos_0p3_10_100->GetBinContent(symmShape2+1,in_compressedE->at(Index_Taus_IsoInf_ShapeVeto_0p3_10_100.at(1))+1)<<endl;

      //         DiTauCounterPass++;
      //         Flag2 = true;
      //       }
      //   }

      // if(Flag1!=Flag2) cout<<"!!! found it !!!"<<endl;
      if(Index_Taus_90.at(0)>=0 && Index_Taus_90.at(1)>=0)
        {
          pt_DiTau_90pc->Fill(pt_Taus_90.at(0),pt_Taus_90.at(1),weight);
        }
      if(Index_Taus_80.at(0)>=0 && Index_Taus_80.at(1)>=0)
        {
          pt_DiTau_80pc->Fill(pt_Taus_80.at(0),pt_Taus_80.at(1),weight);
        }
      if(Index_Taus_70.at(0)>=0 && Index_Taus_70.at(1)>=0)
        {
          pt_DiTau_70pc->Fill(pt_Taus_70.at(0),pt_Taus_70.at(1),weight);
        }
      if(Index_Taus_60.at(0)>=0 && Index_Taus_60.at(1)>=0)
        {
          pt_DiTau_60pc->Fill(pt_Taus_60.at(0),pt_Taus_60.at(1),weight);
        }
      if(Index_Taus_50.at(0)>=0 && Index_Taus_50.at(1)>=0)
        {
          pt_DiTau_50pc->Fill(pt_Taus_50.at(0),pt_Taus_50.at(1),weight);
        }
      if(Index_Taus_Progression_1.at(0)>=0 && Index_Taus_Progression_1.at(1)>=0)
        {
          pt_DiTau_Progression_1->Fill(pt_Taus_Progression_1.at(0),pt_Taus_Progression_1.at(1),weight);
        }
      if(Index_Taus_Progression_2.at(0)>=0 && Index_Taus_Progression_2.at(1)>=0)
        {
          pt_DiTau_Progression_2->Fill(pt_Taus_Progression_2.at(0),pt_Taus_Progression_2.at(1),weight);
        }
      if(Index_Taus_Progression_3.at(0)>=0 && Index_Taus_Progression_3.at(1)>=0)
        {
          pt_DiTau_Progression_3->Fill(pt_Taus_Progression_3.at(0),pt_Taus_Progression_3.at(1),weight);
        }
      if(Index_Taus_Progression_4.at(0)>=0 && Index_Taus_Progression_4.at(1)>=0)
        {
          pt_DiTau_Progression_4->Fill(pt_Taus_Progression_4.at(0),pt_Taus_Progression_4.at(1),weight);
        }
      if(Index_Taus_Progression_5.at(0)>=0 && Index_Taus_Progression_5.at(1)>=0)
        {
          pt_DiTau_Progression_5->Fill(pt_Taus_Progression_5.at(0),pt_Taus_Progression_5.at(1),weight);
        }
      if(Index_Taus_Progression_6.at(0)>=0 && Index_Taus_Progression_6.at(1)>=0)
        {
          pt_DiTau_Progression_6->Fill(pt_Taus_Progression_6.at(0),pt_Taus_Progression_6.at(1),weight);
        }
      if(Index_Taus_Progression_7.at(0)>=0 && Index_Taus_Progression_7.at(1)>=0)
        {
          pt_DiTau_Progression_7->Fill(pt_Taus_Progression_7.at(0),pt_Taus_Progression_7.at(1),weight);
        }
      if(Index_Taus_Progression_8.at(0)>=0 && Index_Taus_Progression_8.at(1)>=0)
        {
          pt_DiTau_Progression_8->Fill(pt_Taus_Progression_8.at(0),pt_Taus_Progression_8.at(1),weight);
        }
      if(Index_Taus_Progression_9.at(0)>=0 && Index_Taus_Progression_9.at(1)>=0)
        {
          pt_DiTau_Progression_9->Fill(pt_Taus_Progression_9.at(0),pt_Taus_Progression_9.at(1),weight);
        }
      if(Index_Taus_Progression_10.at(0)>=0 && Index_Taus_Progression_10.at(1)>=0)
        {
          pt_DiTau_Progression_10->Fill(pt_Taus_Progression_10.at(0),pt_Taus_Progression_10.at(1),weight);
        }
      if(Index_Taus_Progression_11.at(0)>=0 && Index_Taus_Progression_11.at(1)>=0)
        {
          pt_DiTau_Progression_11->Fill(pt_Taus_Progression_11.at(0),pt_Taus_Progression_11.at(1),weight);
        }
      if(Index_Taus_Progression_12.at(0)>=0 && Index_Taus_Progression_12.at(1)>=0)
        {
          pt_DiTau_Progression_12->Fill(pt_Taus_Progression_12.at(0),pt_Taus_Progression_12.at(1),weight);
        }
      if(Index_Taus_Progression_13.at(0)>=0 && Index_Taus_Progression_13.at(1)>=0)
        {
          pt_DiTau_Progression_13->Fill(pt_Taus_Progression_13.at(0),pt_Taus_Progression_13.at(1),weight);
        }
      if(Index_Taus_Progression_14.at(0)>=0 && Index_Taus_Progression_14.at(1)>=0)
        {
          pt_DiTau_Progression_14->Fill(pt_Taus_Progression_14.at(0),pt_Taus_Progression_14.at(1),weight);
        }
      if(Index_Taus_Progression_15.at(0)>=0 && Index_Taus_Progression_15.at(1)>=0)
        {
          pt_DiTau_Progression_15->Fill(pt_Taus_Progression_15.at(0),pt_Taus_Progression_15.at(1),weight);
        }
      if(Index_Taus_Progression_16.at(0)>=0 && Index_Taus_Progression_16.at(1)>=0)
        {
          pt_DiTau_Progression_16->Fill(pt_Taus_Progression_16.at(0),pt_Taus_Progression_16.at(1),weight);
        }
      if(Index_Taus_Progression_17.at(0)>=0 && Index_Taus_Progression_17.at(1)>=0)
        {
          pt_DiTau_Progression_17->Fill(pt_Taus_Progression_17.at(0),pt_Taus_Progression_17.at(1),weight);
        }
      if(Index_Taus_Progression_18.at(0)>=0 && Index_Taus_Progression_18.at(1)>=0)
        {
          pt_DiTau_Progression_18->Fill(pt_Taus_Progression_18.at(0),pt_Taus_Progression_18.at(1),weight);
        }
      if(Index_Taus_Progression_19.at(0)>=0 && Index_Taus_Progression_19.at(1)>=0)
        {
          pt_DiTau_Progression_19->Fill(pt_Taus_Progression_19.at(0),pt_Taus_Progression_19.at(1),weight);
        }
      if(Index_Taus_Progression_20.at(0)>=0 && Index_Taus_Progression_20.at(1)>=0)
        {
          pt_DiTau_Progression_20->Fill(pt_Taus_Progression_20.at(0),pt_Taus_Progression_20.at(1),weight);
        }
      if(Index_Taus_Progression_21.at(0)>=0 && Index_Taus_Progression_21.at(1)>=0)
        {
          pt_DiTau_Progression_21->Fill(pt_Taus_Progression_21.at(0),pt_Taus_Progression_21.at(1),weight);
        }
      if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0)
        {
          // cout<<"weight = "<<weight<<endl;
          pt_DiTau_Progression_22->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
        }
      if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0 && abs(eta_Taus_Progression_22.at(0))<1.4 && abs(eta_Taus_Progression_22.at(1))<1.4)
        {
          pt_DiTau_Progression_22_barrel->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
        }
      if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0 && ((abs(eta_Taus_Progression_22.at(0))<1.4 && abs(eta_Taus_Progression_22.at(1))>1.4) || (abs(eta_Taus_Progression_22.at(0))>1.4 && abs(eta_Taus_Progression_22.at(1))<1.4)))
        {
          pt_DiTau_Progression_22_barrelendcap->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
        }
      if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0 && abs(eta_Taus_Progression_22.at(0))>1.4 && abs(eta_Taus_Progression_22.at(1))>1.4)
        {
          pt_DiTau_Progression_22_endcaps->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
        }

      if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0 && isMerged_Taus_Progression_22.at(0) && isMerged_Taus_Progression_22.at(1))
        {
          pt_DiTau_Progression_22_BothAreMerged->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
        }
      if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0 && ((!isMerged_Taus_Progression_22.at(0) && isMerged_Taus_Progression_22.at(1)) || (isMerged_Taus_Progression_22.at(0) && !isMerged_Taus_Progression_22.at(1))))
        {
          pt_DiTau_Progression_22_OneIsMerged->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
        }
      if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0 && !isMerged_Taus_Progression_22.at(0) && !isMerged_Taus_Progression_22.at(1))
        {
          pt_DiTau_Progression_22_BothAreNotMerged->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
        }

      // if(Index_Taus_Progression_22_ShapeVeto.at(0)>=0 && Index_Taus_Progression_22_ShapeVeto.at(1)>=0)
      //   {
      //     // cout<<"weight = "<<weight<<endl;
      //     pt_DiTau_Progression_22_ShapeVeto->Fill(pt_Taus_Progression_22_ShapeVeto.at(0),pt_Taus_Progression_22_ShapeVeto.at(1),weight);
      //   }
      // if(Index_Taus_Progression_22_ShapeVeto_0p6_10_50.at(0)>=0 && Index_Taus_Progression_22_ShapeVeto_0p6_10_50.at(1)>=0)
      //   {
      //     // cout<<"weight = "<<weight<<endl;
      //     pt_DiTau_Progression_22_ShapeVeto_0p6_10_50->Fill(pt_Taus_Progression_22_ShapeVeto_0p6_10_50.at(0),pt_Taus_Progression_22_ShapeVeto_0p6_10_50.at(1),weight);
      //   }
      // if(Index_Taus_Progression_22_ShapeVeto_0p4_10_35.at(0)>=0 && Index_Taus_Progression_22_ShapeVeto_0p4_10_35.at(1)>=0)
      //   {
      //     // cout<<"weight = "<<weight<<endl;
      //     pt_DiTau_Progression_22_ShapeVeto_0p4_10_35->Fill(pt_Taus_Progression_22_ShapeVeto_0p4_10_35.at(0),pt_Taus_Progression_22_ShapeVeto_0p4_10_35.at(1),weight);
      //   }
      // if(Index_Taus_Progression_22_ShapeVeto_0p2_10_35.at(0)>=0 && Index_Taus_Progression_22_ShapeVeto_0p2_10_35.at(1)>=0)
      //   {
      //     // cout<<"weight = "<<weight<<endl;
      //     pt_DiTau_Progression_22_ShapeVeto_0p2_10_35->Fill(pt_Taus_Progression_22_ShapeVeto_0p2_10_35.at(0),pt_Taus_Progression_22_ShapeVeto_0p2_10_35.at(1),weight);
      //   }
      // if(Index_Taus_Progression_22_ShapeVeto_0p5_20_45.at(0)>=0 && Index_Taus_Progression_22_ShapeVeto_0p5_20_45.at(1)>=0)
      //   {
      //     pt_DiTau_Progression_22_ShapeVeto_0p5_20_45->Fill(pt_Taus_Progression_22_ShapeVeto_0p5_20_45.at(0),pt_Taus_Progression_22_ShapeVeto_0p5_20_45.at(1),weight);
      //   }
      // if(Index_Taus_Progression_22_ShapeVeto_0p2_25_60.at(0)>=0 && Index_Taus_Progression_22_ShapeVeto_0p2_25_60.at(1)>=0)
      //   {
      //     pt_DiTau_Progression_22_ShapeVeto_0p2_25_60->Fill(pt_Taus_Progression_22_ShapeVeto_0p2_25_60.at(0),pt_Taus_Progression_22_ShapeVeto_0p2_25_60.at(1),weight);
      //   }
      // if(Index_Taus_Progression_22_ShapeVeto_0p2_20_100.at(0)>=0 && Index_Taus_Progression_22_ShapeVeto_0p2_20_100.at(1)>=0)
      //   {
      //     pt_DiTau_Progression_22_ShapeVeto_0p2_20_100->Fill(pt_Taus_Progression_22_ShapeVeto_0p2_20_100.at(0),pt_Taus_Progression_22_ShapeVeto_0p2_20_100.at(1),weight);
      //   }
      // if(Index_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(0)>=0 && Index_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(1)>=0)
      //   {
      //     pt_DiTau_Progression_22_ShapeVeto_0p2_20_100_dummy->Fill(pt_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(0),pt_Taus_Progression_22_ShapeVeto_0p2_20_100_dummy.at(1),weight);
      //   }
      if(Index_Taus_Progression_23.at(0)>=0 && Index_Taus_Progression_23.at(1)>=0)
        {
          pt_DiTau_Progression_23->Fill(pt_Taus_Progression_23.at(0),pt_Taus_Progression_23.at(1),weight);
        }
      if(Index_Taus_Progression_24.at(0)>=0 && Index_Taus_Progression_24.at(1)>=0)
        {
          pt_DiTau_Progression_24->Fill(pt_Taus_Progression_24.at(0),pt_Taus_Progression_24.at(1),weight);
        }
      if(Index_Taus_Progression_25.at(0)>=0 && Index_Taus_Progression_25.at(1)>=0)
        {
          pt_DiTau_Progression_25->Fill(pt_Taus_Progression_25.at(0),pt_Taus_Progression_25.at(1),weight);
        }
      if(Index_Taus_Progression_26.at(0)>=0 && Index_Taus_Progression_26.at(1)>=0)
        {
          pt_DiTau_Progression_26->Fill(pt_Taus_Progression_26.at(0),pt_Taus_Progression_26.at(1),weight);
        }
      if(Index_Taus_Progression_27.at(0)>=0 && Index_Taus_Progression_27.at(1)>=0)
        {
          pt_DiTau_Progression_27->Fill(pt_Taus_Progression_27.at(0),pt_Taus_Progression_27.at(1),weight);
        }
      if(Index_Taus_Progression_28.at(0)>=0 && Index_Taus_Progression_28.at(1)>=0)
        {
          pt_DiTau_Progression_28->Fill(pt_Taus_Progression_28.at(0),pt_Taus_Progression_28.at(1),weight);
        }
      if(Index_Taus_Progression_29.at(0)>=0 && Index_Taus_Progression_29.at(1)>=0)
        {
          pt_DiTau_Progression_29->Fill(pt_Taus_Progression_29.at(0),pt_Taus_Progression_29.at(1),weight);
        }
      if(Index_Taus_Progression_30.at(0)>=0 && Index_Taus_Progression_30.at(1)>=0)
        {
          pt_DiTau_Progression_30->Fill(pt_Taus_Progression_30.at(0),pt_Taus_Progression_30.at(1),weight);
        }


      if(Index_Taus_Progression_31.at(0)>=0 && Index_Taus_Progression_31.at(1)>=0)
        {
          pt_DiTau_Progression_31->Fill(pt_Taus_Progression_31.at(0),pt_Taus_Progression_31.at(1),weight);
        }
      if(Index_Taus_Progression_31.at(0)>=0 && Index_Taus_Progression_31.at(1)>=0 && abs(eta_Taus_Progression_31.at(0))<1.4 && abs(eta_Taus_Progression_31.at(1))<1.4)
        {
          pt_DiTau_Progression_31_barrel->Fill(pt_Taus_Progression_31.at(0),pt_Taus_Progression_31.at(1),weight);
        }
      if(Index_Taus_Progression_31.at(0)>=0 && Index_Taus_Progression_31.at(1)>=0 && ((abs(eta_Taus_Progression_31.at(0))<1.4 && abs(eta_Taus_Progression_31.at(1))>1.4) || (abs(eta_Taus_Progression_31.at(0))>1.4 && abs(eta_Taus_Progression_31.at(1))<1.4)))
        {
          pt_DiTau_Progression_31_barrelendcap->Fill(pt_Taus_Progression_31.at(0),pt_Taus_Progression_31.at(1),weight);
        }
      if(Index_Taus_Progression_31.at(0)>=0 && Index_Taus_Progression_31.at(1)>=0 && abs(eta_Taus_Progression_31.at(0))>1.4 && abs(eta_Taus_Progression_31.at(1))>1.4)
        {
          pt_DiTau_Progression_31_endcaps->Fill(pt_Taus_Progression_31.at(0),pt_Taus_Progression_31.at(1),weight);
        }


      if(Index_Taus_Progression_31_extrap.at(0)>=0 && Index_Taus_Progression_31_extrap.at(1)>=0)
        {
          pt_DiTau_Progression_31_extrap->Fill(pt_Taus_Progression_31_extrap.at(0),pt_Taus_Progression_31_extrap.at(1),weight);
        }
      if(Index_Taus_Progression_31_extrap.at(0)>=0 && Index_Taus_Progression_31_extrap.at(1)>=0 && abs(eta_Taus_Progression_31_extrap.at(0))<1.4 && abs(eta_Taus_Progression_31_extrap.at(1))<1.4)
        {
          pt_DiTau_Progression_31_extrap_barrel->Fill(pt_Taus_Progression_31_extrap.at(0),pt_Taus_Progression_31_extrap.at(1),weight);
        }
      if(Index_Taus_Progression_31_extrap.at(0)>=0 && Index_Taus_Progression_31_extrap.at(1)>=0 && ((abs(eta_Taus_Progression_31_extrap.at(0))<1.4 && abs(eta_Taus_Progression_31_extrap.at(1))>1.4) || (abs(eta_Taus_Progression_31_extrap.at(0))>1.4 && abs(eta_Taus_Progression_31_extrap.at(1))<1.4)))
        {
          pt_DiTau_Progression_31_extrap_barrelendcap->Fill(pt_Taus_Progression_31_extrap.at(0),pt_Taus_Progression_31_extrap.at(1),weight);
        }
      if(Index_Taus_Progression_31_extrap.at(0)>=0 && Index_Taus_Progression_31_extrap.at(1)>=0 && abs(eta_Taus_Progression_31_extrap.at(0))>1.4 && abs(eta_Taus_Progression_31_extrap.at(1))>1.4)
        {
          pt_DiTau_Progression_31_extrap_endcaps->Fill(pt_Taus_Progression_31_extrap.at(0),pt_Taus_Progression_31_extrap.at(1),weight);
        }
    }

  cout<<"CounterAll = "<<CounterAll<<endl;
  cout<<"CounterPass = "<<CounterPass<<endl;
  cout<<"CounterFail = "<<CounterFail<<endl;

  cout<<"DiTauCounterAll = "<<DiTauCounterAll<<endl;
  cout<<"DiTauCounterPass = "<<DiTauCounterPass<<endl;

  // SET RUN INFO
  float nb = 2544.; // Run323755 and Run323775
  float thisLumiRun = 0.;
  if (run == 323755) thisLumiRun = 1.6225E34; // Run323755
  if (run == 323775) thisLumiRun = 1.736E34; // Run323775 --> very smal numer of lumisections!!
  if (run == 362616) { nb = 2450; thisLumiRun = 2.05E34; }
  if (run == 362617) { nb = 2450; thisLumiRun = 2.50E34; }
  if (thisLumiRun == 0.)
  {
    std::cout << "ERROR: something went wrong with the run selection and the lumi initialization" << std::endl;
    return;
  }
  float scaleToLumi = 2.00E34;
  float scale = 0.001*(nb*11245.6);//*scaleToLumi/thisLumiRun;

  // float nb = 2064;
  // float thisLumiRun = 12118.9E30;
  // float scaleToLumi = 1.15E34;
  // float scale = 0.001*(nb*11245.6)*scaleToLumi/thisLumiRun;

  // float scale = 0.001*(517.*11245.6);
  cout<<"Denominator = "<<Denominator<<endl;
  // float scale = 0.001*(2232.*11245.6);
  // rate_IsoInf->Scale(scale/dataL1Upgrade.GetEntries());
  // rate_Iso_50->Scale(scale/dataL1Upgrade.GetEntries());
  // rate_Iso_60->Scale(scale/dataL1Upgrade.GetEntries());
  // rate_Iso_70->Scale(scale/dataL1Upgrade.GetEntries());

  TH1F* rate_noCut = new TH1F("rate_noCut","rate_noCut",240,0.,240.);
  // TH1F* rate_noCut_ShapeVeto = new TH1F("rate_noCut_ShapeVeto","rate_noCut_ShapeVeto",240,0.,240.);
  // TH1F* rate_noCut_ShapeVeto_0p2_20_100 = new TH1F("rate_noCut_ShapeVeto_0p2_20_100","rate_noCut_ShapeVeto_0p2_20_100",240,0.,240.);
  // TH1F* rate_noCut_ShapeVeto_0p2_20_100_dummy = new TH1F("rate_noCut_ShapeVeto_0p2_20_100_dummy","rate_noCut_ShapeVeto_0p2_20_100_dummy",240,0.,240.);
  // TH1F* rate_noCut_ShapeVeto_0p6_10_50 = new TH1F("rate_noCut_ShapeVeto_0p6_10_50","rate_noCut_ShapeVeto_0p6_10_50",240,0.,240.);
  // TH1F* rate_noCut_ShapeVeto_0p4_10_35 = new TH1F("rate_noCut_ShapeVeto_0p4_10_35","rate_noCut_ShapeVeto_0p4_10_35",240,0.,240.);
  // TH1F* rate_noCut_ShapeVeto_0p5_10_70 = new TH1F("rate_noCut_ShapeVeto_0p5_10_70","rate_noCut_ShapeVeto_0p5_10_70",240,0.,240.);
  // TH1F* rate_noCut_ShapeVeto_0p4_10_70 = new TH1F("rate_noCut_ShapeVeto_0p4_10_70","rate_noCut_ShapeVeto_0p4_10_70",240,0.,240.);
  // TH1F* rate_noCut_ShapeVeto_0p3_10_70 = new TH1F("rate_noCut_ShapeVeto_0p3_10_70","rate_noCut_ShapeVeto_0p3_10_70",240,0.,240.);
  // TH1F* rate_noCut_ShapeVeto_0p3_10_100 = new TH1F("rate_noCut_ShapeVeto_0p3_10_100","rate_noCut_ShapeVeto_0p3_10_100",240,0.,240.);
  TH1F* rate_IsoCut_50 = new TH1F("rate_IsoCut_50","rate_IsoCut_50",240,0.,240.);
  TH1F* rate_IsoCut_60 = new TH1F("rate_IsoCut_60","rate_IsoCut_60",240,0.,240.);
  TH1F* rate_IsoCut_70 = new TH1F("rate_IsoCut_70","rate_IsoCut_70",240,0.,240.);
  TH1F* rate_IsoCut_80 = new TH1F("rate_IsoCut_80","rate_IsoCut_80",240,0.,240.);
  TH1F* rate_IsoCut_90 = new TH1F("rate_IsoCut_90","rate_IsoCut_90",240,0.,240.);
  TH1F* rate_Stage1 = new TH1F("rate_Stage1","rate_Stage1",240,0.,240.);
  TH1F* rate_Progression_1 = new TH1F("rate_Progression_1","rate_Progression_1",240,0.,240.);
  TH1F* rate_Progression_2 = new TH1F("rate_Progression_2","rate_Progression_2",240,0.,240.);
  TH1F* rate_Progression_3 = new TH1F("rate_Progression_3","rate_Progression_3",240,0.,240.);
  TH1F* rate_Progression_4 = new TH1F("rate_Progression_4","rate_Progression_4",240,0.,240.);
  TH1F* rate_Progression_5 = new TH1F("rate_Progression_5","rate_Progression_5",240,0.,240.);
  TH1F* rate_Progression_6 = new TH1F("rate_Progression_6","rate_Progression_6",240,0.,240.);
  TH1F* rate_Progression_7 = new TH1F("rate_Progression_7","rate_Progression_7",240,0.,240.);
  TH1F* rate_Progression_8 = new TH1F("rate_Progression_8","rate_Progression_8",240,0.,240.);
  TH1F* rate_Progression_9 = new TH1F("rate_Progression_9","rate_Progression_9",240,0.,240.);
  TH1F* rate_Progression_10 = new TH1F("rate_Progression_10","rate_Progression_10",240,0.,240.);
  TH1F* rate_Progression_11 = new TH1F("rate_Progression_11","rate_Progression_11",240,0.,240.);
  TH1F* rate_Progression_12 = new TH1F("rate_Progression_12","rate_Progression_12",240,0.,240.);
  TH1F* rate_Progression_13 = new TH1F("rate_Progression_13","rate_Progression_13",240,0.,240.);
  TH1F* rate_Progression_14 = new TH1F("rate_Progression_14","rate_Progression_14",240,0.,240.);
  TH1F* rate_Progression_15 = new TH1F("rate_Progression_15","rate_Progression_15",240,0.,240.);
  TH1F* rate_Progression_16 = new TH1F("rate_Progression_16","rate_Progression_16",240,0.,240.);
  TH1F* rate_Progression_17 = new TH1F("rate_Progression_17","rate_Progression_17",240,0.,240.);
  TH1F* rate_Progression_18 = new TH1F("rate_Progression_18","rate_Progression_18",240,0.,240.);
  TH1F* rate_Progression_19 = new TH1F("rate_Progression_19","rate_Progression_19",240,0.,240.);
  TH1F* rate_Progression_20 = new TH1F("rate_Progression_20","rate_Progression_20",240,0.,240.);
  TH1F* rate_Progression_21 = new TH1F("rate_Progression_21","rate_Progression_21",240,0.,240.);
  TH1F* rate_Progression_22 = new TH1F("rate_Progression_22","rate_Progression_22",240,0.,240.);

  TH1F* rate_noCut_DiTau = new TH1F("rate_noCut_DiTau","rate_noCut_DiTau",240,0.,240.);
  // TH1F* rate_noCut_DiTau_ShapeVeto = new TH1F("rate_noCut_DiTau_ShapeVeto","rate_noCut_DiTau_ShapeVeto",240,0.,240.);
  // TH1F* rate_noCut_DiTau_ShapeVeto_0p2_20_100 = new TH1F("rate_noCut_DiTau_ShapeVeto_0p2_20_100","rate_noCut_DiTau_ShapeVeto_0p2_20_100",240,0.,240.);
  // TH1F* rate_noCut_DiTau_ShapeVeto_0p2_20_100_dummy = new TH1F("rate_noCut_DiTau_ShapeVeto_0p2_20_100_dummy","rate_noCut_DiTau_ShapeVeto_0p2_20_100_dummy",240,0.,240.);
  // TH1F* rate_noCut_DiTau_ShapeVeto_0p6_10_50 = new TH1F("rate_noCut_DiTau_ShapeVeto_0p6_10_50","rate_noCut_DiTau_ShapeVeto_0p6_10_50",240,0.,240.);
  // TH1F* rate_noCut_DiTau_ShapeVeto_0p4_10_35 = new TH1F("rate_noCut_DiTau_ShapeVeto_0p4_10_35","rate_noCut_DiTau_ShapeVeto_0p4_10_35",240,0.,240.);
  // TH1F* rate_noCut_DiTau_ShapeVeto_0p5_10_70 = new TH1F("rate_noCut_DiTau_ShapeVeto_0p5_10_70","rate_noCut_DiTau_ShapeVeto_0p5_10_70",240,0.,240.);
  // TH1F* rate_noCut_DiTau_ShapeVeto_0p4_10_70 = new TH1F("rate_noCut_DiTau_ShapeVeto_0p4_10_70","rate_noCut_DiTau_ShapeVeto_0p4_10_70",240,0.,240.);
  // TH1F* rate_noCut_DiTau_ShapeVeto_0p3_10_70 = new TH1F("rate_noCut_DiTau_ShapeVeto_0p3_10_70","rate_noCut_DiTau_ShapeVeto_0p3_10_70",240,0.,240.);
  // TH1F* rate_noCut_DiTau_ShapeVeto_0p3_10_100 = new TH1F("rate_noCut_DiTau_ShapeVeto_0p3_10_100","rate_noCut_DiTau_ShapeVeto_0p3_10_100",240,0.,240.);
  TH1F* rate_DiTau_90 = new TH1F("rate_DiTau_90","rate_DiTau_90",240,0.,240.);
  TH1F* rate_DiTau_80 = new TH1F("rate_DiTau_80","rate_DiTau_80",240,0.,240.);
  TH1F* rate_DiTau_70 = new TH1F("rate_DiTau_70","rate_DiTau_70",240,0.,240.);
  TH1F* rate_DiTau_60 = new TH1F("rate_DiTau_60","rate_DiTau_60",240,0.,240.);
  TH1F* rate_DiTau_50 = new TH1F("rate_DiTau_50","rate_DiTau_50",240,0.,240.);
  TH1F* rate_DiTau_Progression_1 = new TH1F("rate_DiTau_Progression_1","rate_DiTau_Progression_1",240,0.,240.);
  TH1F* rate_DiTau_Progression_2 = new TH1F("rate_DiTau_Progression_2","rate_DiTau_Progression_2",240,0.,240.);
  TH1F* rate_DiTau_Progression_3 = new TH1F("rate_DiTau_Progression_3","rate_DiTau_Progression_3",240,0.,240.);
  TH1F* rate_DiTau_Progression_4 = new TH1F("rate_DiTau_Progression_4","rate_DiTau_Progression_4",240,0.,240.);
  TH1F* rate_DiTau_Progression_5 = new TH1F("rate_DiTau_Progression_5","rate_DiTau_Progression_5",240,0.,240.);
  TH1F* rate_DiTau_Progression_6 = new TH1F("rate_DiTau_Progression_6","rate_DiTau_Progression_6",240,0.,240.);
  TH1F* rate_DiTau_Progression_7 = new TH1F("rate_DiTau_Progression_7","rate_DiTau_Progression_7",240,0.,240.);
  TH1F* rate_DiTau_Progression_8 = new TH1F("rate_DiTau_Progression_8","rate_DiTau_Progression_8",240,0.,240.);
  TH1F* rate_DiTau_Progression_9 = new TH1F("rate_DiTau_Progression_9","rate_DiTau_Progression_9",240,0.,240.);
  TH1F* rate_DiTau_Progression_10 = new TH1F("rate_DiTau_Progression_10","rate_DiTau_Progression_10",240,0.,240.);
  TH1F* rate_DiTau_Progression_11 = new TH1F("rate_DiTau_Progression_11","rate_DiTau_Progression_11",240,0.,240.);
  TH1F* rate_DiTau_Progression_12 = new TH1F("rate_DiTau_Progression_12","rate_DiTau_Progression_12",240,0.,240.);
  TH1F* rate_DiTau_Progression_13 = new TH1F("rate_DiTau_Progression_13","rate_DiTau_Progression_13",240,0.,240.);
  TH1F* rate_DiTau_Progression_14 = new TH1F("rate_DiTau_Progression_14","rate_DiTau_Progression_14",240,0.,240.);
  TH1F* rate_DiTau_Progression_15 = new TH1F("rate_DiTau_Progression_15","rate_DiTau_Progression_15",240,0.,240.);
  TH1F* rate_DiTau_Progression_16 = new TH1F("rate_DiTau_Progression_16","rate_DiTau_Progression_16",240,0.,240.);
  TH1F* rate_DiTau_Progression_17 = new TH1F("rate_DiTau_Progression_17","rate_DiTau_Progression_17",240,0.,240.);
  TH1F* rate_DiTau_Progression_18 = new TH1F("rate_DiTau_Progression_18","rate_DiTau_Progression_18",240,0.,240.);
  TH1F* rate_DiTau_Progression_19 = new TH1F("rate_DiTau_Progression_19","rate_DiTau_Progression_19",240,0.,240.);
  TH1F* rate_DiTau_Progression_20 = new TH1F("rate_DiTau_Progression_20","rate_DiTau_Progression_20",240,0.,240.);
  TH1F* rate_DiTau_Progression_21 = new TH1F("rate_DiTau_Progression_21","rate_DiTau_Progression_21",240,0.,240.);
  TH1F* rate_DiTau_Progression_21_barrel = new TH1F("rate_DiTau_Progression_21_barrel","rate_DiTau_Progression_21_barrel",240,0.,240.);
  TH1F* rate_DiTau_Progression_21_endcaps = new TH1F("rate_DiTau_Progression_21_endcaps","rate_DiTau_Progression_21_endcaps",240,0.,240.);
  TH1F* rate_DiTau_Progression_22 = new TH1F("rate_DiTau_Progression_22","rate_DiTau_Progression_22",240,0.,240.);
  TH1F* rate_DiTau_Progression_22_barrel = new TH1F("rate_DiTau_Progression_22_barrel","rate_DiTau_Progression_22_barrel",240,0.,240.);
  TH1F* rate_DiTau_Progression_22_barrelendcap = new TH1F("rate_DiTau_Progression_22_barrelendcap","rate_DiTau_Progression_22_barrelendcap",240,0.,240.);
  TH1F* rate_DiTau_Progression_22_endcaps = new TH1F("rate_DiTau_Progression_22_endcaps","rate_DiTau_Progression_22_endcaps",240,0.,240.);
  TH1F* rate_DiTau_Progression_22_BothAreMerged = new TH1F("rate_DiTau_Progression_22_BothAreMerged","rate_DiTau_Progression_22_BothAreMerged",240,0.,240.);
  TH1F* rate_DiTau_Progression_22_OneIsMerged = new TH1F("rate_DiTau_Progression_22_OneIsMerged","rate_DiTau_Progression_22_OneIsMerged",240,0.,240.);
  TH1F* rate_DiTau_Progression_22_BothAreNotMerged = new TH1F("rate_DiTau_Progression_22_BothAreNotMerged","rate_DiTau_Progression_22_BothAreNotMerged",240,0.,240.);
  // TH1F* rate_DiTau_Progression_22_ShapeVeto = new TH1F("rate_DiTau_Progression_22_ShapeVeto","rate_DiTau_Progression_22_ShapeVeto",240,0.,240.);
  // TH1F* rate_DiTau_Progression_22_ShapeVeto_0p6_10_50 = new TH1F("rate_DiTau_Progression_22_ShapeVeto_0p6_10_50","rate_DiTau_Progression_22_ShapeVeto_0p6_10_50",240,0.,240.);
  // TH1F* rate_DiTau_Progression_22_ShapeVeto_0p4_10_35 = new TH1F("rate_DiTau_Progression_22_ShapeVeto_0p4_10_35","rate_DiTau_Progression_22_ShapeVeto_0p4_10_35",240,0.,240.);
  // TH1F* rate_DiTau_Progression_22_ShapeVeto_0p2_10_35 = new TH1F("rate_DiTau_Progression_22_ShapeVeto_0p2_10_35","rate_DiTau_Progression_22_ShapeVeto_0p2_10_35",240,0.,240.);
  // TH1F* rate_DiTau_Progression_22_ShapeVeto_0p5_20_45 = new TH1F("rate_DiTau_Progression_22_ShapeVeto_0p5_20_45","rate_DiTau_Progression_22_ShapeVeto_0p5_20_45",240,0.,240.);
  // TH1F* rate_DiTau_Progression_22_ShapeVeto_0p2_25_60 = new TH1F("rate_DiTau_Progression_22_ShapeVeto_0p2_25_60","rate_DiTau_Progression_22_ShapeVeto_0p2_25_60",240,0.,240.);
  // TH1F* rate_DiTau_Progression_22_ShapeVeto_0p2_20_100 = new TH1F("rate_DiTau_Progression_22_ShapeVeto_0p2_20_100","rate_DiTau_Progression_22_ShapeVeto_0p2_20_100",240,0.,240.);
  // TH1F* rate_DiTau_Progression_22_ShapeVeto_0p2_20_100_dummy = new TH1F("rate_DiTau_Progression_22_ShapeVeto_0p2_20_100_dummy","rate_DiTau_Progression_22_ShapeVeto_0p2_20_100_dummy",240,0.,240.);

  TH1F* rate_DiTau_Progression_23 = new TH1F("rate_DiTau_Progression_23","rate_DiTau_Progression_23",240,0.,240.);
  TH1F* rate_DiTau_Progression_24 = new TH1F("rate_DiTau_Progression_24","rate_DiTau_Progression_24",240,0.,240.);
  TH1F* rate_DiTau_Progression_25 = new TH1F("rate_DiTau_Progression_25","rate_DiTau_Progression_25",240,0.,240.);
  TH1F* rate_DiTau_Progression_26 = new TH1F("rate_DiTau_Progression_26","rate_DiTau_Progression_26",240,0.,240.);
  TH1F* rate_DiTau_Progression_27 = new TH1F("rate_DiTau_Progression_27","rate_DiTau_Progression_27",240,0.,240.);
  TH1F* rate_DiTau_Progression_28 = new TH1F("rate_DiTau_Progression_28","rate_DiTau_Progression_28",240,0.,240.);
  TH1F* rate_DiTau_Progression_29 = new TH1F("rate_DiTau_Progression_29","rate_DiTau_Progression_29",240,0.,240.);
  TH1F* rate_DiTau_Progression_30 = new TH1F("rate_DiTau_Progression_30","rate_DiTau_Progression_30",240,0.,240.);
  TH1F* rate_DiTau_Progression_31 = new TH1F("rate_DiTau_Progression_31","rate_DiTau_Progression_31",240,0.,240.);
  TH1F* rate_DiTau_Progression_31_barrel = new TH1F("rate_DiTau_Progression_31_barrel","rate_DiTau_Progression_31_barrel",240,0.,240.);
  TH1F* rate_DiTau_Progression_31_barrelendcap = new TH1F("rate_DiTau_Progression_31_barrelendcap","rate_DiTau_Progression_31_barrelendcap",240,0.,240.);
  TH1F* rate_DiTau_Progression_31_endcaps = new TH1F("rate_DiTau_Progression_31_endcaps","rate_DiTau_Progression_31_endcaps",240,0.,240.);
  TH1F* rate_DiTau_Progression_31_extrap = new TH1F("rate_DiTau_Progression_31_extrap","rate_DiTau_Progression_31_extrap",240,0.,240.);
  TH1F* rate_DiTau_Progression_31_extrap_barrel = new TH1F("rate_DiTau_Progression_31_extrap_barrel","rate_DiTau_Progression_31_extrap_barrel",240,0.,240.);
  TH1F* rate_DiTau_Progression_31_extrap_barrelendcap = new TH1F("rate_DiTau_Progression_31_extrap_barrelendcap","rate_DiTau_Progression_31_extrap_barrelendcap",240,0.,240.);
  TH1F* rate_DiTau_Progression_31_extrap_endcaps = new TH1F("rate_DiTau_Progression_31_extrap_endcaps","rate_DiTau_Progression_31_extrap_endcaps",240,0.,240.);

  for(UInt_t i = 0 ; i < 241 ; ++i)
    {
      rate_noCut->SetBinContent(i+1,pt_IsoInf->Integral(i+1,241)/Denominator*scale);
      // rate_noCut_ShapeVeto->SetBinContent(i+1,pt_IsoInf_ShapeVeto->Integral(i+1,241)/Denominator*scale);
      // rate_noCut_ShapeVeto_0p2_20_100->SetBinContent(i+1,pt_IsoInf_ShapeVeto_0p2_20_100->Integral(i+1,241)/Denominator*scale);
      // rate_noCut_ShapeVeto_0p2_20_100_dummy->SetBinContent(i+1,pt_IsoInf_ShapeVeto_0p2_20_100_dummy->Integral(i+1,241)/Denominator*scale);
      // rate_noCut_ShapeVeto_0p6_10_50->SetBinContent(i+1,pt_IsoInf_ShapeVeto_0p6_10_50->Integral(i+1,241)/Denominator*scale);
      // rate_noCut_ShapeVeto_0p4_10_35->SetBinContent(i+1,pt_IsoInf_ShapeVeto_0p4_10_35->Integral(i+1,241)/Denominator*scale);
      // rate_noCut_ShapeVeto_0p5_10_70->SetBinContent(i+1,pt_IsoInf_ShapeVeto_0p5_10_70->Integral(i+1,241)/Denominator*scale);
      // rate_noCut_ShapeVeto_0p4_10_70->SetBinContent(i+1,pt_IsoInf_ShapeVeto_0p4_10_70->Integral(i+1,241)/Denominator*scale);
      // rate_noCut_ShapeVeto_0p3_10_70->SetBinContent(i+1,pt_IsoInf_ShapeVeto_0p3_10_70->Integral(i+1,241)/Denominator*scale);
      // rate_noCut_ShapeVeto_0p3_10_100->SetBinContent(i+1,pt_IsoInf_ShapeVeto_0p3_10_100->Integral(i+1,241)/Denominator*scale);
      rate_IsoCut_50->SetBinContent(i+1,pt_Iso_50->Integral(i+1,241)/Denominator*scale);
      rate_IsoCut_60->SetBinContent(i+1,pt_Iso_60->Integral(i+1,241)/Denominator*scale);
      rate_IsoCut_70->SetBinContent(i+1,pt_Iso_70->Integral(i+1,241)/Denominator*scale);
      rate_IsoCut_80->SetBinContent(i+1,pt_Iso_80->Integral(i+1,241)/Denominator*scale);
      rate_IsoCut_90->SetBinContent(i+1,pt_Iso_90->Integral(i+1,241)/Denominator*scale);
      rate_Progression_1->SetBinContent(i+1,pt_Progression_1->Integral(i+1,241)/Denominator*scale);
      rate_Progression_2->SetBinContent(i+1,pt_Progression_2->Integral(i+1,241)/Denominator*scale);
      rate_Progression_3->SetBinContent(i+1,pt_Progression_3->Integral(i+1,241)/Denominator*scale);
      rate_Progression_4->SetBinContent(i+1,pt_Progression_4->Integral(i+1,241)/Denominator*scale);
      rate_Progression_5->SetBinContent(i+1,pt_Progression_5->Integral(i+1,241)/Denominator*scale);
      rate_Progression_6->SetBinContent(i+1,pt_Progression_6->Integral(i+1,241)/Denominator*scale);
      rate_Progression_7->SetBinContent(i+1,pt_Progression_7->Integral(i+1,241)/Denominator*scale);
      rate_Progression_8->SetBinContent(i+1,pt_Progression_8->Integral(i+1,241)/Denominator*scale);
      rate_Progression_9->SetBinContent(i+1,pt_Progression_9->Integral(i+1,241)/Denominator*scale);
      rate_Progression_10->SetBinContent(i+1,pt_Progression_10->Integral(i+1,241)/Denominator*scale);
      rate_Progression_11->SetBinContent(i+1,pt_Progression_11->Integral(i+1,241)/Denominator*scale);
      rate_Progression_12->SetBinContent(i+1,pt_Progression_12->Integral(i+1,241)/Denominator*scale);
      rate_Progression_13->SetBinContent(i+1,pt_Progression_13->Integral(i+1,241)/Denominator*scale);
      rate_Progression_14->SetBinContent(i+1,pt_Progression_14->Integral(i+1,241)/Denominator*scale);
      rate_Progression_15->SetBinContent(i+1,pt_Progression_15->Integral(i+1,241)/Denominator*scale);
      rate_Progression_16->SetBinContent(i+1,pt_Progression_16->Integral(i+1,241)/Denominator*scale);
      rate_Progression_17->SetBinContent(i+1,pt_Progression_17->Integral(i+1,241)/Denominator*scale);
      rate_Progression_18->SetBinContent(i+1,pt_Progression_18->Integral(i+1,241)/Denominator*scale);
      rate_Progression_19->SetBinContent(i+1,pt_Progression_19->Integral(i+1,241)/Denominator*scale);
      rate_Progression_20->SetBinContent(i+1,pt_Progression_20->Integral(i+1,241)/Denominator*scale);
      rate_Progression_21->SetBinContent(i+1,pt_Progression_21->Integral(i+1,241)/Denominator*scale);
      rate_Progression_22->SetBinContent(i+1,pt_Progression_22->Integral(i+1,241)/Denominator*scale);

      rate_noCut_DiTau->SetBinContent(i+1,pt_IsoInf_DiTau->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_noCut_DiTau_ShapeVeto->SetBinContent(i+1,pt_IsoInf_DiTau_ShapeVeto->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_noCut_DiTau_ShapeVeto_0p2_20_100->SetBinContent(i+1,pt_IsoInf_DiTau_ShapeVeto_0p2_20_100->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_noCut_DiTau_ShapeVeto_0p2_20_100_dummy->SetBinContent(i+1,pt_IsoInf_DiTau_ShapeVeto_0p2_20_100_dummy->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_noCut_DiTau_ShapeVeto_0p6_10_50->SetBinContent(i+1,pt_IsoInf_DiTau_ShapeVeto_0p6_10_50->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_noCut_DiTau_ShapeVeto_0p4_10_35->SetBinContent(i+1,pt_IsoInf_DiTau_ShapeVeto_0p4_10_35->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_noCut_DiTau_ShapeVeto_0p5_10_70->SetBinContent(i+1,pt_IsoInf_DiTau_ShapeVeto_0p5_10_70->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_noCut_DiTau_ShapeVeto_0p4_10_70->SetBinContent(i+1,pt_IsoInf_DiTau_ShapeVeto_0p4_10_70->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_noCut_DiTau_ShapeVeto_0p3_10_70->SetBinContent(i+1,pt_IsoInf_DiTau_ShapeVeto_0p3_10_70->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_noCut_DiTau_ShapeVeto_0p3_10_100->SetBinContent(i+1,pt_IsoInf_DiTau_ShapeVeto_0p3_10_100->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_90->SetBinContent(i+1,pt_DiTau_90pc->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_80->SetBinContent(i+1,pt_DiTau_80pc->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_70->SetBinContent(i+1,pt_DiTau_70pc->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_60->SetBinContent(i+1,pt_DiTau_60pc->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_50->SetBinContent(i+1,pt_DiTau_50pc->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_1->SetBinContent(i+1,pt_DiTau_Progression_1->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_2->SetBinContent(i+1,pt_DiTau_Progression_2->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_3->SetBinContent(i+1,pt_DiTau_Progression_3->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_4->SetBinContent(i+1,pt_DiTau_Progression_4->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_5->SetBinContent(i+1,pt_DiTau_Progression_5->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_6->SetBinContent(i+1,pt_DiTau_Progression_6->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_7->SetBinContent(i+1,pt_DiTau_Progression_7->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_8->SetBinContent(i+1,pt_DiTau_Progression_8->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_9->SetBinContent(i+1,pt_DiTau_Progression_9->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_10->SetBinContent(i+1,pt_DiTau_Progression_10->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_11->SetBinContent(i+1,pt_DiTau_Progression_11->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_12->SetBinContent(i+1,pt_DiTau_Progression_12->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_13->SetBinContent(i+1,pt_DiTau_Progression_13->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_14->SetBinContent(i+1,pt_DiTau_Progression_14->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_15->SetBinContent(i+1,pt_DiTau_Progression_15->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_16->SetBinContent(i+1,pt_DiTau_Progression_16->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_17->SetBinContent(i+1,pt_DiTau_Progression_17->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_18->SetBinContent(i+1,pt_DiTau_Progression_18->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_19->SetBinContent(i+1,pt_DiTau_Progression_19->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_20->SetBinContent(i+1,pt_DiTau_Progression_20->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_21->SetBinContent(i+1,pt_DiTau_Progression_21->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_21_barrel->SetBinContent(i+1,pt_DiTau_Progression_21_barrel->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_21_endcaps->SetBinContent(i+1,pt_DiTau_Progression_21_endcaps->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_22->SetBinContent(i+1,pt_DiTau_Progression_22->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_22_barrel->SetBinContent(i+1,pt_DiTau_Progression_22_barrel->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_22_barrelendcap->SetBinContent(i+1,pt_DiTau_Progression_22_barrelendcap->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_22_endcaps->SetBinContent(i+1,pt_DiTau_Progression_22_endcaps->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_22_BothAreMerged->SetBinContent(i+1,pt_DiTau_Progression_22_BothAreMerged->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_22_OneIsMerged->SetBinContent(i+1,pt_DiTau_Progression_22_OneIsMerged->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_22_BothAreNotMerged->SetBinContent(i+1,pt_DiTau_Progression_22_BothAreNotMerged->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_DiTau_Progression_22_ShapeVeto->SetBinContent(i+1,pt_DiTau_Progression_22_ShapeVeto->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_DiTau_Progression_22_ShapeVeto_0p6_10_50->SetBinContent(i+1,pt_DiTau_Progression_22_ShapeVeto_0p6_10_50->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_DiTau_Progression_22_ShapeVeto_0p4_10_35->SetBinContent(i+1,pt_DiTau_Progression_22_ShapeVeto_0p4_10_35->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_DiTau_Progression_22_ShapeVeto_0p2_10_35->SetBinContent(i+1,pt_DiTau_Progression_22_ShapeVeto_0p2_10_35->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_DiTau_Progression_22_ShapeVeto_0p5_20_45->SetBinContent(i+1,pt_DiTau_Progression_22_ShapeVeto_0p5_20_45->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_DiTau_Progression_22_ShapeVeto_0p2_25_60->SetBinContent(i+1,pt_DiTau_Progression_22_ShapeVeto_0p2_25_60->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_DiTau_Progression_22_ShapeVeto_0p2_20_100->SetBinContent(i+1,pt_DiTau_Progression_22_ShapeVeto_0p2_20_100->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_DiTau_Progression_22_ShapeVeto_0p2_20_100_dummy->SetBinContent(i+1,pt_DiTau_Progression_22_ShapeVeto_0p2_20_100_dummy->Integral(i+1,241,i+1,241)/Denominator*scale);

      rate_DiTau_Progression_23->SetBinContent(i+1,pt_DiTau_Progression_23->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_24->SetBinContent(i+1,pt_DiTau_Progression_24->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_25->SetBinContent(i+1,pt_DiTau_Progression_25->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_26->SetBinContent(i+1,pt_DiTau_Progression_26->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_27->SetBinContent(i+1,pt_DiTau_Progression_27->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_28->SetBinContent(i+1,pt_DiTau_Progression_28->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_29->SetBinContent(i+1,pt_DiTau_Progression_29->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_30->SetBinContent(i+1,pt_DiTau_Progression_30->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_31->SetBinContent(i+1,pt_DiTau_Progression_31->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_31_barrel->SetBinContent(i+1,pt_DiTau_Progression_31_barrel->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_31_barrelendcap->SetBinContent(i+1,pt_DiTau_Progression_31_barrelendcap->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_31_endcaps->SetBinContent(i+1,pt_DiTau_Progression_31_endcaps->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_31_extrap->SetBinContent(i+1,pt_DiTau_Progression_31_extrap->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_31_extrap_barrel->SetBinContent(i+1,pt_DiTau_Progression_31_extrap_barrel->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_31_extrap_barrelendcap->SetBinContent(i+1,pt_DiTau_Progression_31_extrap_barrelendcap->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_DiTau_Progression_31_extrap_endcaps->SetBinContent(i+1,pt_DiTau_Progression_31_extrap_endcaps->Integral(i+1,241,i+1,241)/Denominator*scale);
      // rate_IsoCut_Linear->SetBinContent(i+1,pt_IsoLinear->Integral(i+1,1201)/Denominator*scale);
      // rate_IsoCut_Linear_DiTau->SetBinContent(i+1,pt_IsoLinear_DiTau->Integral(i+1,1201)/Denominator*scale);
      //rate_Stage1->SetBinContent(i+1,pt_Stage1->Integral(i+1,1201)/dataStage1.GetEntries()*scale);
    }


  // TH1F* rate_noCut = new TH1F("rate_noCut","rate_noCut",1200,0.,240.);
  // TH1F* rate_IsoCut_50 = new TH1F("rate_IsoCut_50","rate_IsoCut_50",1200,0.,240.);
  // TH1F* rate_IsoCut_60 = new TH1F("rate_IsoCut_60","rate_IsoCut_60",1200,0.,240.);
  // TH1F* rate_IsoCut_70 = new TH1F("rate_IsoCut_70","rate_IsoCut_70",1200,0.,240.);
  // TH1F* rate_IsoCut_80 = new TH1F("rate_IsoCut_80","rate_IsoCut_80",1200,0.,240.);
  // TH1F* rate_IsoCut_90 = new TH1F("rate_IsoCut_90","rate_IsoCut_90",1200,0.,240.);
  // TH1F* rate_Stage1 = new TH1F("rate_Stage1","rate_Stage1",1200,0.,240.);
  // TH1F* rate_Progression_1 = new TH1F("rate_Progression_1","rate_Progression_1",1200,0.,240.);
  // TH1F* rate_Progression_2 = new TH1F("rate_Progression_2","rate_Progression_2",1200,0.,240.);
  // TH1F* rate_Progression_3 = new TH1F("rate_Progression_3","rate_Progression_3",1200,0.,240.);
  // TH1F* rate_Progression_4 = new TH1F("rate_Progression_4","rate_Progression_4",1200,0.,240.);
  // TH1F* rate_Progression_5 = new TH1F("rate_Progression_5","rate_Progression_5",1200,0.,240.);
  // TH1F* rate_Progression_6 = new TH1F("rate_Progression_6","rate_Progression_6",1200,0.,240.);
  // TH1F* rate_Progression_7 = new TH1F("rate_Progression_7","rate_Progression_7",1200,0.,240.);
  // TH1F* rate_Progression_8 = new TH1F("rate_Progression_8","rate_Progression_8",1200,0.,240.);
  // TH1F* rate_Progression_9 = new TH1F("rate_Progression_9","rate_Progression_9",1200,0.,240.);
  // TH1F* rate_Progression_10 = new TH1F("rate_Progression_10","rate_Progression_10",1200,0.,240.);
  // TH1F* rate_Progression_11 = new TH1F("rate_Progression_11","rate_Progression_11",1200,0.,240.);
  // TH1F* rate_Progression_12 = new TH1F("rate_Progression_12","rate_Progression_12",1200,0.,240.);
  // TH1F* rate_Progression_13 = new TH1F("rate_Progression_13","rate_Progression_13",1200,0.,240.);
  // TH1F* rate_Progression_14 = new TH1F("rate_Progression_14","rate_Progression_14",1200,0.,240.);
  // TH1F* rate_Progression_15 = new TH1F("rate_Progression_15","rate_Progression_15",1200,0.,240.);
  // TH1F* rate_Progression_16 = new TH1F("rate_Progression_16","rate_Progression_16",1200,0.,240.);
  // TH1F* rate_Progression_17 = new TH1F("rate_Progression_17","rate_Progression_17",1200,0.,240.);
  // TH1F* rate_Progression_18 = new TH1F("rate_Progression_18","rate_Progression_18",1200,0.,240.);
  // TH1F* rate_Progression_19 = new TH1F("rate_Progression_19","rate_Progression_19",1200,0.,240.);
  // TH1F* rate_Progression_20 = new TH1F("rate_Progression_20","rate_Progression_20",1200,0.,240.);
  // TH1F* rate_Progression_21 = new TH1F("rate_Progression_21","rate_Progression_21",1200,0.,240.);

  // TH1F* rate_noCut_DiTau = new TH1F("rate_noCut_DiTau","rate_noCut_DiTau",1200,0.,240.);
  // TH1F* rate_DiTau_90 = new TH1F("rate_DiTau_90","rate_DiTau_90",1200,0.,240.);
  // TH1F* rate_DiTau_80 = new TH1F("rate_DiTau_80","rate_DiTau_80",1200,0.,240.);
  // TH1F* rate_DiTau_70 = new TH1F("rate_DiTau_70","rate_DiTau_70",1200,0.,240.);
  // TH1F* rate_DiTau_60 = new TH1F("rate_DiTau_60","rate_DiTau_60",1200,0.,240.);
  // TH1F* rate_DiTau_50 = new TH1F("rate_DiTau_50","rate_DiTau_50",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_1 = new TH1F("rate_DiTau_Progression_1","rate_DiTau_Progression_1",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_2 = new TH1F("rate_DiTau_Progression_2","rate_DiTau_Progression_2",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_3 = new TH1F("rate_DiTau_Progression_3","rate_DiTau_Progression_3",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_4 = new TH1F("rate_DiTau_Progression_4","rate_DiTau_Progression_4",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_5 = new TH1F("rate_DiTau_Progression_5","rate_DiTau_Progression_5",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_6 = new TH1F("rate_DiTau_Progression_6","rate_DiTau_Progression_6",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_7 = new TH1F("rate_DiTau_Progression_7","rate_DiTau_Progression_7",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_8 = new TH1F("rate_DiTau_Progression_8","rate_DiTau_Progression_8",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_9 = new TH1F("rate_DiTau_Progression_9","rate_DiTau_Progression_9",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_10 = new TH1F("rate_DiTau_Progression_10","rate_DiTau_Progression_10",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_11 = new TH1F("rate_DiTau_Progression_11","rate_DiTau_Progression_11",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_12 = new TH1F("rate_DiTau_Progression_12","rate_DiTau_Progression_12",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_13 = new TH1F("rate_DiTau_Progression_13","rate_DiTau_Progression_13",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_14 = new TH1F("rate_DiTau_Progression_14","rate_DiTau_Progression_14",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_15 = new TH1F("rate_DiTau_Progression_15","rate_DiTau_Progression_15",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_16 = new TH1F("rate_DiTau_Progression_16","rate_DiTau_Progression_16",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_17 = new TH1F("rate_DiTau_Progression_17","rate_DiTau_Progression_17",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_18 = new TH1F("rate_DiTau_Progression_18","rate_DiTau_Progression_18",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_19 = new TH1F("rate_DiTau_Progression_19","rate_DiTau_Progression_19",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_20 = new TH1F("rate_DiTau_Progression_20","rate_DiTau_Progression_20",1200,0.,240.);
  // TH1F* rate_DiTau_Progression_21 = new TH1F("rate_DiTau_Progression_21","rate_DiTau_Progression_21",1200,0.,240.);

  // for(UInt_t i = 0 ; i < 1201 ; ++i)
  //   {
  //     rate_noCut->SetBinContent(i+1,pt_IsoInf->Integral(i+1,1201)/Denominator*scale);
  //     rate_IsoCut_50->SetBinContent(i+1,pt_Iso_50->Integral(i+1,1201)/Denominator*scale);
  //     rate_IsoCut_60->SetBinContent(i+1,pt_Iso_60->Integral(i+1,1201)/Denominator*scale);
  //     rate_IsoCut_70->SetBinContent(i+1,pt_Iso_70->Integral(i+1,1201)/Denominator*scale);
  //     rate_IsoCut_80->SetBinContent(i+1,pt_Iso_80->Integral(i+1,1201)/Denominator*scale);
  //     rate_IsoCut_90->SetBinContent(i+1,pt_Iso_90->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_1->SetBinContent(i+1,pt_Progression_1->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_2->SetBinContent(i+1,pt_Progression_2->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_3->SetBinContent(i+1,pt_Progression_3->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_4->SetBinContent(i+1,pt_Progression_4->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_5->SetBinContent(i+1,pt_Progression_5->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_6->SetBinContent(i+1,pt_Progression_6->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_7->SetBinContent(i+1,pt_Progression_7->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_8->SetBinContent(i+1,pt_Progression_8->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_9->SetBinContent(i+1,pt_Progression_9->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_10->SetBinContent(i+1,pt_Progression_10->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_11->SetBinContent(i+1,pt_Progression_11->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_12->SetBinContent(i+1,pt_Progression_12->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_13->SetBinContent(i+1,pt_Progression_13->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_14->SetBinContent(i+1,pt_Progression_14->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_15->SetBinContent(i+1,pt_Progression_15->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_16->SetBinContent(i+1,pt_Progression_16->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_17->SetBinContent(i+1,pt_Progression_17->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_18->SetBinContent(i+1,pt_Progression_18->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_19->SetBinContent(i+1,pt_Progression_19->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_20->SetBinContent(i+1,pt_Progression_20->Integral(i+1,1201)/Denominator*scale);
  //     rate_Progression_21->SetBinContent(i+1,pt_Progression_21->Integral(i+1,1201)/Denominator*scale);

  //     rate_noCut_DiTau->SetBinContent(i+1,pt_IsoInf_DiTau->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_90->SetBinContent(i+1,pt_DiTau_90pc->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_80->SetBinContent(i+1,pt_DiTau_80pc->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_70->SetBinContent(i+1,pt_DiTau_70pc->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_60->SetBinContent(i+1,pt_DiTau_60pc->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_50->SetBinContent(i+1,pt_DiTau_50pc->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_1->SetBinContent(i+1,pt_DiTau_Progression_1->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_2->SetBinContent(i+1,pt_DiTau_Progression_2->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_3->SetBinContent(i+1,pt_DiTau_Progression_3->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_4->SetBinContent(i+1,pt_DiTau_Progression_4->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_5->SetBinContent(i+1,pt_DiTau_Progression_5->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_6->SetBinContent(i+1,pt_DiTau_Progression_6->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_7->SetBinContent(i+1,pt_DiTau_Progression_7->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_8->SetBinContent(i+1,pt_DiTau_Progression_8->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_9->SetBinContent(i+1,pt_DiTau_Progression_9->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_10->SetBinContent(i+1,pt_DiTau_Progression_10->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_11->SetBinContent(i+1,pt_DiTau_Progression_11->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_12->SetBinContent(i+1,pt_DiTau_Progression_12->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_13->SetBinContent(i+1,pt_DiTau_Progression_13->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_14->SetBinContent(i+1,pt_DiTau_Progression_14->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_15->SetBinContent(i+1,pt_DiTau_Progression_15->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_16->SetBinContent(i+1,pt_DiTau_Progression_16->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_17->SetBinContent(i+1,pt_DiTau_Progression_17->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_18->SetBinContent(i+1,pt_DiTau_Progression_18->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_19->SetBinContent(i+1,pt_DiTau_Progression_19->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_20->SetBinContent(i+1,pt_DiTau_Progression_20->Integral(i+1,1201,i+1,1201)/Denominator*scale);
  //     rate_DiTau_Progression_21->SetBinContent(i+1,pt_DiTau_Progression_21->Integral(i+1,1201,i+1,1201)/Denominator*scale);

  //     // rate_IsoCut_Linear->SetBinContent(i+1,pt_IsoLinear->Integral(i+1,1201)/Denominator*scale);
  //     // rate_IsoCut_Linear_DiTau->SetBinContent(i+1,pt_IsoLinear_DiTau->Integral(i+1,1201)/Denominator*scale);
  //     //rate_Stage1->SetBinContent(i+1,pt_Stage1->Integral(i+1,1201)/dataStage1.GetEntries()*scale);
  //   }
  
  TString TFileName = "";  
  if(parametrisation=="linear") TFileName = "histos_2023/histos_rate_ZeroBias_Run"+run_str+"_optimizationV0p2_calibThr"+intgr+"p"+decim+"_linear_old.root";
  else                          TFileName = "histos_2023/histos_rate_ZeroBias_Run"+run_str+"_optimizationV0p2_calibThr"+intgr+"p"+decim+"_"+parametrisation+Kintgr+"p"+Kdecim+".root";
  TFile f(TFileName,"RECREATE");

  Iso_MinBias->Write();
  Correction_Factor->Write();
  Correction_Factor_IEt_30->Write();


  h_pt_calibrated->Write();
  h_pt_uncalibrated->Write();

  pt_IsoInf->Write();
  // pt_IsoInf_ShapeVeto->Write();
  // pt_IsoInf_ShapeVeto_0p2_20_100->Write();
  // pt_IsoInf_ShapeVeto_0p2_20_100_dummy->Write();
  // pt_IsoInf_ShapeVeto_0p6_10_50->Write();
  // pt_IsoInf_ShapeVeto_0p4_10_35->Write();
  // pt_IsoInf_ShapeVeto_0p5_10_70->Write();
  // pt_IsoInf_ShapeVeto_0p4_10_70->Write();
  // pt_IsoInf_ShapeVeto_0p3_10_70->Write();
  // pt_IsoInf_ShapeVeto_0p3_10_100->Write();
  pt_Iso_50->Write();
  pt_Iso_60->Write();
  pt_Iso_70->Write();
  pt_Iso_80->Write();
  pt_Iso_90->Write();
  pt_Progression_1->Write();
  pt_Progression_2->Write();
  pt_Progression_3->Write();
  pt_Progression_4->Write();
  pt_Progression_5->Write();
  pt_Progression_6->Write();
  pt_Progression_7->Write();
  pt_Progression_8->Write();
  pt_Progression_9->Write();
  pt_Progression_10->Write();
  pt_Progression_11->Write();
  pt_Progression_12->Write();
  pt_Progression_13->Write();
  pt_Progression_14->Write();
  pt_Progression_15->Write();
  pt_Progression_16->Write();
  pt_Progression_17->Write();
  pt_Progression_18->Write();
  pt_Progression_19->Write();
  pt_Progression_20->Write();
  pt_Progression_21->Write();
  pt_Progression_22->Write();

  pt_IsoInf_DiTau->Write();
  // pt_IsoInf_DiTau_ShapeVeto->Write();
  // pt_IsoInf_DiTau_ShapeVeto_0p2_20_100->Write();
  // pt_IsoInf_DiTau_ShapeVeto_0p2_20_100_dummy->Write();
  // pt_IsoInf_DiTau_ShapeVeto_0p6_10_50->Write();
  // pt_IsoInf_DiTau_ShapeVeto_0p4_10_35->Write();
  // pt_IsoInf_DiTau_ShapeVeto_0p5_10_70->Write();
  // pt_IsoInf_DiTau_ShapeVeto_0p4_10_70->Write();
  // pt_IsoInf_DiTau_ShapeVeto_0p3_10_70->Write();
  // pt_IsoInf_DiTau_ShapeVeto_0p3_10_100->Write();
  pt_DiTau_90pc->Write();
  pt_DiTau_80pc->Write();
  pt_DiTau_70pc->Write();
  pt_DiTau_60pc->Write();
  pt_DiTau_50pc->Write();
  pt_DiTau_Progression_1->Write();
  pt_DiTau_Progression_2->Write();
  pt_DiTau_Progression_4->Write();
  pt_DiTau_Progression_5->Write();
  pt_DiTau_Progression_6->Write();
  pt_DiTau_Progression_7->Write();
  pt_DiTau_Progression_8->Write();
  pt_DiTau_Progression_9->Write();
  pt_DiTau_Progression_10->Write();
  pt_DiTau_Progression_11->Write();
  pt_DiTau_Progression_12->Write();
  pt_DiTau_Progression_13->Write();
  pt_DiTau_Progression_14->Write();
  pt_DiTau_Progression_15->Write();
  pt_DiTau_Progression_16->Write();
  pt_DiTau_Progression_17->Write();
  pt_DiTau_Progression_18->Write();
  pt_DiTau_Progression_19->Write();
  pt_DiTau_Progression_20->Write();
  pt_DiTau_Progression_21->Write();
  pt_DiTau_Progression_22->Write();
  // pt_DiTau_Progression_22_ShapeVeto->Write();
  // pt_DiTau_Progression_22_ShapeVeto_0p6_10_50->Write();
  // pt_DiTau_Progression_22_ShapeVeto_0p4_10_35->Write();
  // pt_DiTau_Progression_22_ShapeVeto_0p2_10_35->Write();
  // pt_DiTau_Progression_22_ShapeVeto_0p5_20_45->Write();
  // pt_DiTau_Progression_22_ShapeVeto_0p2_25_60->Write();
  // pt_DiTau_Progression_22_ShapeVeto_0p2_20_100->Write();
  // pt_DiTau_Progression_22_ShapeVeto_0p2_20_100_dummy->Write();

  pt_DiTau_Progression_21->Write();
  pt_DiTau_Progression_22->Write();
  pt_DiTau_Progression_23->Write();
  pt_DiTau_Progression_24->Write();
  pt_DiTau_Progression_25->Write();
  pt_DiTau_Progression_26->Write();
  pt_DiTau_Progression_27->Write();
  pt_DiTau_Progression_28->Write();
  pt_DiTau_Progression_29->Write();
  pt_DiTau_Progression_30->Write();
  pt_DiTau_Progression_31->Write();
  pt_DiTau_Progression_31_barrel->Write();
  pt_DiTau_Progression_31_barrelendcap->Write();
  pt_DiTau_Progression_31_endcaps->Write();
  pt_DiTau_Progression_31_extrap->Write();
  pt_DiTau_Progression_31_extrap_barrel->Write();
  pt_DiTau_Progression_31_extrap_barrelendcap->Write();
  pt_DiTau_Progression_31_extrap_endcaps->Write();
  // pt_IsoLinear->Write();
  // pt_IsoLinear_DiTau->Write();

  rate_noCut->Write();
  // rate_noCut_ShapeVeto->Write();
  rate_IsoCut_50->Write();
  rate_IsoCut_60->Write();
  rate_IsoCut_70->Write();  
  rate_IsoCut_80->Write();  
  rate_IsoCut_90->Write();  
  rate_Progression_1->Write();  
  rate_Progression_2->Write();  
  rate_Progression_3->Write();  
  rate_Progression_4->Write();  
  rate_Progression_5->Write();  
  rate_Progression_6->Write();  
  rate_Progression_7->Write();  
  rate_Progression_8->Write();  
  rate_Progression_9->Write();  
  rate_Progression_10->Write();  
  rate_Progression_11->Write();  
  rate_Progression_12->Write();  
  rate_Progression_13->Write();  
  rate_Progression_14->Write();  
  rate_Progression_15->Write();  
  rate_Progression_16->Write();  
  rate_Progression_17->Write();  
  rate_Progression_18->Write();  
  rate_Progression_19->Write();  
  rate_Progression_20->Write();  
  rate_Progression_21->Write();  
  rate_Progression_22->Write();  

  rate_noCut_DiTau->Write();
  // rate_noCut_DiTau_ShapeVeto->Write();
  // rate_noCut_DiTau_ShapeVeto_0p2_20_100->Write();
  // rate_noCut_DiTau_ShapeVeto_0p2_20_100_dummy->Write();
  // rate_noCut_DiTau_ShapeVeto_0p6_10_50->Write();
  // rate_noCut_DiTau_ShapeVeto_0p4_10_35->Write();
  // rate_noCut_DiTau_ShapeVeto_0p5_10_70->Write();
  // rate_noCut_DiTau_ShapeVeto_0p4_10_70->Write();
  // rate_noCut_DiTau_ShapeVeto_0p3_10_70->Write();
  // rate_noCut_DiTau_ShapeVeto_0p3_10_100->Write();
  rate_DiTau_90->Write();
  rate_DiTau_80->Write();
  rate_DiTau_70->Write();
  rate_DiTau_60->Write();
  rate_DiTau_50->Write();
  rate_DiTau_Progression_1->Write();
  rate_DiTau_Progression_2->Write();
  rate_DiTau_Progression_3->Write();
  rate_DiTau_Progression_4->Write();
  rate_DiTau_Progression_5->Write();
  rate_DiTau_Progression_6->Write();
  rate_DiTau_Progression_7->Write();
  rate_DiTau_Progression_8->Write();
  rate_DiTau_Progression_9->Write();
  rate_DiTau_Progression_10->Write();
  rate_DiTau_Progression_11->Write();
  rate_DiTau_Progression_12->Write();
  rate_DiTau_Progression_13->Write();
  rate_DiTau_Progression_14->Write();
  rate_DiTau_Progression_15->Write();
  rate_DiTau_Progression_16->Write();
  rate_DiTau_Progression_17->Write();
  rate_DiTau_Progression_18->Write();
  rate_DiTau_Progression_19->Write();
  rate_DiTau_Progression_20->Write();
  rate_DiTau_Progression_21->Write();
  rate_DiTau_Progression_21_endcaps->Write();
  rate_DiTau_Progression_21_barrel->Write();
  rate_DiTau_Progression_22->Write();
  rate_DiTau_Progression_22_barrel->Write();
  rate_DiTau_Progression_22_barrelendcap->Write();
  rate_DiTau_Progression_22_endcaps->Write();
  rate_DiTau_Progression_22_BothAreMerged->Write();
  rate_DiTau_Progression_22_OneIsMerged->Write();
  rate_DiTau_Progression_22_BothAreNotMerged->Write();
  // rate_DiTau_Progression_22_ShapeVeto->Write();
  // rate_DiTau_Progression_22_ShapeVeto_0p6_10_50->Write();
  // rate_DiTau_Progression_22_ShapeVeto_0p4_10_35->Write();
  // rate_DiTau_Progression_22_ShapeVeto_0p2_10_35->Write();
  // rate_DiTau_Progression_22_ShapeVeto_0p5_20_45->Write();
  // rate_DiTau_Progression_22_ShapeVeto_0p2_25_60->Write();
  // rate_DiTau_Progression_22_ShapeVeto_0p2_20_100->Write();
  // rate_DiTau_Progression_22_ShapeVeto_0p2_20_100_dummy->Write();

  rate_DiTau_Progression_22->Write();
  rate_DiTau_Progression_23->Write();
  rate_DiTau_Progression_24->Write();
  rate_DiTau_Progression_25->Write();
  rate_DiTau_Progression_26->Write();
  rate_DiTau_Progression_27->Write();
  rate_DiTau_Progression_28->Write();
  rate_DiTau_Progression_29->Write();
  rate_DiTau_Progression_30->Write();
  rate_DiTau_Progression_31->Write();
  rate_DiTau_Progression_31_barrel->Write();
  rate_DiTau_Progression_31_barrelendcap->Write();
  rate_DiTau_Progression_31_endcaps->Write();
  rate_DiTau_Progression_31_extrap->Write();
  rate_DiTau_Progression_31_extrap_barrel->Write();
  rate_DiTau_Progression_31_extrap_barrelendcap->Write();
  rate_DiTau_Progression_31_extrap_endcaps->Write();
  rate_Stage1->Write();  

  TCanvas c("c","c",800.,800.);
  rate_noCut->Draw();

  rate_noCut->SetLineColor(kBlack);
  rate_IsoCut_50->SetLineColor(kRed);
  rate_IsoCut_60->SetLineColor(kBlue);
  rate_IsoCut_70->SetLineColor(kMagenta);
  rate_IsoCut_80->SetLineColor(kOrange);
  rate_IsoCut_90->SetLineColor(kViolet);
  // rate_IsoCut_Linear->SetLineColor(kOrange);

  rate_IsoCut_50->Draw("same");
  rate_IsoCut_60->Draw("same");
  rate_IsoCut_70->Draw("same");
  rate_IsoCut_80->Draw("same");
  rate_IsoCut_90->Draw("same");
  // rate_IsoCut_Linear->Draw("same");

  c.SetLogy();

  c.Write();
  //  c.SaveAs("/home/llr/cms/mperez/TauTrigger/CMSSW_9_2_10/src/TauTagAndProbe/TauTagAndProbe/test/Plots_meanparam/PlotRate/canvas_rate_MC_Run305310_mean_newnTT.pdf");
  //  c.SaveAs("/home/llr/cms/mperez/TauTrigger/CMSSW_9_2_10/src/TauTagAndProbe/TauTagAndProbe/test/Trees_meanparam/canvas_rate_MC_Run305310_mean_newnTT.root");

  // c.SaveAs("canvas_rate_new_MC_NewCompression_extendedRange_Layer1_2016_Conditions_Thomas_nTT_NewLayer1_Option5_277069_OlivierFlatWP_SK1616.pdf");
  // c.SaveAs("canvas_rate_new_MC_NewCompression_extendedRange_Layer1_2016_Conditions_Thomas_nTT_NewLayer1_Option5_277069_OlivierFlatWP_SK1616.root");

  return;
}
