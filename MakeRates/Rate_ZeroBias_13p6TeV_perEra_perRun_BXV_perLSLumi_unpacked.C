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
#include "/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/Calibrate/ApplyCalibration_newnTT.C"
#include <map>
#include <string>
#include <vector>
#include <iterator>
#include <regex>

const std::regex comma(",");

using namespace std;

void Rate(TString era, int run, bool doScaleToLumi)
{
  TString run_str = to_string(run);

  std::ifstream LumiInfoFile("LumiByLSFiles/Run"+run_str+"LumiByLS.csv");
  std::vector<float> LumiByLS;
  std::string line{};
  while (LumiInfoFile && getline(LumiInfoFile, line)) {
    if (line[0] == '#') continue;
    // Tokenize the line and store result in vector. Use range constructor of std::vector
    std::vector<std::string> row{ std::sregex_token_iterator(line.begin(),line.end(),comma,-1), std::sregex_token_iterator() };
    LumiByLS.push_back(stof(row[5])*1E30); // unit is 1e30/cm2s
  }

  // TString FileName_in = "/data_CMS/cms/motta/Run3preparation/EphemeralZeroBias_2018D_Run323755__RAW/EphemeralZeroBias_2018D_Run323755__RAW.root";
  TString FileName_in = "/data_CMS/cms/motta/Run3preparation/ZeroBias_Run2022"+era+"_Run"+run_str+"__RAW/ZeroBias_Run2022"+era+"_Run"+run_str+"__RAW.root";
  TFile f_in(FileName_in.Data(),"READ");
  TTree* inTree = (TTree*)f_in.Get("ZeroBias/ZeroBias");

  ULong64_t       in_EventNumber =  0;
  Int_t           in_RunNumber =  0;
  Int_t           in_lumi =  0;
  vector<float>   *in_l1tTauPt =  0;
  vector<float>   *in_l1tTauEta =  0;
  vector<int>     *in_l1tTauIso = 0;
  vector<float>   *in_l1tEGPt =  0;
  vector<float>   *in_l1tEGEta =  0;
  vector<int>     *in_l1tEGIso = 0;
  // vector<float>   *in_l1tMuPt =  0;
  // vector<float>   *in_l1tMuEta =  0;
  // vector<int>     *in_l1tMuQual = 0;

  vector<int> *in_bit21 = 0;
  vector<int> *in_bit178 = 0;
  vector<int> *in_bit192 = 0;
  vector<int> *in_bit218 = 0;
  vector<int> *in_bit262 = 0;
  vector<int> *in_bit263 = 0;
  vector<int> *in_bit270 = 0;
  vector<int> *in_bit271 = 0;
  vector<int> *in_bit459 = 0;

  inTree->SetBranchAddress("EventNumber", &in_EventNumber);
  inTree->SetBranchAddress("RunNumber", &in_RunNumber);
  inTree->SetBranchAddress("lumi", &in_lumi);
  inTree->SetBranchAddress("l1tPt", &in_l1tTauPt);
  inTree->SetBranchAddress("l1tEta", &in_l1tTauEta);
  inTree->SetBranchAddress("l1tIso", &in_l1tTauIso);
  inTree->SetBranchAddress("l1tEGPt", &in_l1tEGPt);
  inTree->SetBranchAddress("l1tEGEta", &in_l1tEGEta);
  inTree->SetBranchAddress("l1tEGIso", &in_l1tEGIso);
  // inTree->SetBranchAddress("l1tMuPt",&in_l1tMuPt);
  // inTree->SetBranchAddress("l1tMuEta",&in_l1tMuEta);
  // inTree->SetBranchAddress("l1tMuQual",&in_l1tMuQual);

  inTree->SetBranchAddress("bit21",  &in_bit21);
  inTree->SetBranchAddress("bit178", &in_bit178);
  inTree->SetBranchAddress("bit192", &in_bit192);
  inTree->SetBranchAddress("bit218", &in_bit218);
  inTree->SetBranchAddress("bit262", &in_bit262);
  inTree->SetBranchAddress("bit263", &in_bit263);
  inTree->SetBranchAddress("bit270", &in_bit270);
  inTree->SetBranchAddress("bit271", &in_bit271);
  // inTree->SetBranchAddress("bit459", &in_bit459);

  TH1F* pt_SingleTau = new TH1F("pt_SingleTau","pt_SingleTau",240,0.,240.);
  TH1F* pt_Iso_SingleTau = new TH1F("pt_Iso_SingleTau","pt_Iso_SingleTau",240,0.,240.);

  TH2F* pt_DiTau = new TH2F("pt_DiTau","pt_DiTau",240,0.,240.,240,0.,240.);
  TH2F* pt_Iso_DiTau = new TH2F("pt_Iso_DiTau","pt_Iso_DiTau",240,0.,240.,240,0.,240.);

  TH1F* pt_SingleEG = new TH1F("pt_SingleEG","pt_SingleEG",240,0.,240.);
  TH1F* pt_LooseIso_SingleEG = new TH1F("pt_LooseIso_SingleEG","pt_LooseIso_SingleEG",240,0.,240.);
  TH1F* pt_TightIso_SingleEG = new TH1F("pt_TightIso_SingleEG","pt_TightIso_SingleEG",240,0.,240.);

  TH2F* pt_DiEG = new TH2F("pt_DiEG","pt_DiEG",240,0.,240.,240,0.,240.);
  TH2F* pt_LooseIso_DiEG = new TH2F("pt_LooseIso_DiEG","pt_LooseIso_DiEG",240,0.,240.,240,0.,240.);
  TH2F* pt_TightIso_DiEG = new TH2F("pt_TightIso_DiEG","pt_TightIso_DiEG",240,0.,240.,240,0.,240.);



  TH2F* LS_pt_SingleTau = new TH2F("LS_pt_SingleTau","LS_pt_SingleTau",3000,1.,3001., 240,0.,240.);
  TH2F* LS_pt_Iso_SingleTau = new TH2F("LS_pt_Iso_SingleTau","LS_pt_Iso_SingleTau",3000,1.,3001., 240,0.,240.);

  TH3F* LS_pt_DiTau = new TH3F("LS_pt_DiTau","LS_pt_DiTau",3000,1.,3001.,240,0.,240.,240,0.,240.);
  TH3F* LS_pt_Iso_DiTau = new TH3F("LS_pt_Iso_DiTau","LS_pt_Iso_DiTau",3000,1.,3001.,240,0.,240.,240,0.,240.);

  TH2F* LS_pt_SingleEG = new TH2F("LS_pt_SingleEG","LS_pt_SingleEG",3000,1.,3001.,240,0.,240.);
  TH2F* LS_pt_LooseIso_SingleEG = new TH2F("LS_pt_LooseIso_SingleEG","LS_pt_LooseIso_SingleEG",3000,1.,3001.,240,0.,240.);
  TH2F* LS_pt_TightIso_SingleEG = new TH2F("LS_pt_TightIso_SingleEG","LS_pt_TightIso_SingleEG",3000,1.,3001.,240,0.,240.);

  TH3F* LS_pt_DiEG = new TH3F("LS_pt_DiEG","LS_pt_DiEG",3000,1.,3001.,240,0.,240.,240,0.,240.);
  TH3F* LS_pt_LooseIso_DiEG = new TH3F("LS_pt_LooseIso_DiEG","LS_pt_LooseIso_DiEG",3000,1.,3001.,240,0.,240.,240,0.,240.);
  TH3F* LS_pt_TightIso_DiEG = new TH3F("LS_pt_TightIso_DiEG","LS_pt_TightIso_DiEG",3000,1.,3001.,240,0.,240.,240,0.,240.);

  TH1F* LS_Denominator = new TH1F("LS_Denominator","LS_Denominator",3000,1.,3001.);



  TH1F* bit21_vs_BX = new TH1F("SingleMu22_vs_BX", "SingleMu22_vs_BX", 5, -2.5, 2.5);
  TH1F* bit178_vs_BX = new TH1F("SingleLooseIsoEG28er2p5_vs_BX", "SingleLooseIsoEG28er2p5_vs_BX", 5, -2.5, 2.5);
  TH1F* bit192_vs_BX = new TH1F("SingleIsoEG30er2p5_vs_BX", "SingleIsoEG30er2p5_vs_BX", 5, -2.5, 2.5);
  TH1F* bit218_vs_BX = new TH1F("DoubleEG_25_12_er2p5_vs_BX", "DoubleEG_25_12_er2p5_vs_BX", 5, -2.5, 2.5);
  TH1F* bit262_vs_BX = new TH1F("SingleIsoTau32er2p1_vs_BX", "SingleIsoTau32er2p1_vs_BX", 5, -2.5, 2.5);
  TH1F* bit263_vs_BX = new TH1F("SingleTau70er2p1_vs_BX", "SingleTau70er2p1_vs_BX", 5, -2.5, 2.5);
  TH1F* bit270_vs_BX = new TH1F("DoubleIsoTau32er2p1_vs_BX", "DoubleIsoTau32er2p1_vs_BX", 5, -2.5, 2.5);
  TH1F* bit271_vs_BX = new TH1F("DoubleIsoTau34er2p1_vs_BX", "DoubleIsoTau34er2p1_vs_BX", 5, -2.5, 2.5);
  TH1F* bit459_vs_BX = new TH1F("ZeroBias_vs_BX", "ZeroBias_vs_BX", 5, -2.5, 2.5);


  int uGT_rateBit21 = 0;
  int uGT_rateBit178 = 0;
  int uGT_rateBit192 = 0;
  int uGT_rateBit218 = 0;
  int uGT_rateBit262 = 0;
  int uGT_rateBit263 = 0;
  int uGT_rateBit270 = 0;
  int uGT_rateBit271 = 0;

  TH1F* LS_uGT_SingleMu22 = new TH1F("LS_uGT_SingleMu22","LS_uGT_SingleMu22",3000,1.,3001.);
  TH1F* LS_uGT_SingleLooseIsoEG28er2p5 = new TH1F("LS_uGT_SingleLooseIsoEG28er2p5","LS_uGT_SingleLooseIsoEG28er2p5",3000,1.,3001.);
  TH1F* LS_uGT_SingleIsoEG30er2p5 = new TH1F("LS_uGT_SingleIsoEG30er2p5","LS_uGT_SingleIsoEG30er2p5",3000,1.,3001.);
  TH1F* LS_uGT_DoubleEG_25_12_er2p5 = new TH1F("LS_uGT_DoubleEG_25_12_er2p5","LS_uGT_DoubleEG_25_12_er2p5",3000,1.,3001.);
  TH1F* LS_uGT_SingleIsoTau32er2p1 = new TH1F("LS_uGT_SingleIsoTau32er2p1","LS_uGT_SingleIsoTau32er2p1",3000,1.,3001.);
  TH1F* LS_uGT_SingleTau70er2p1 = new TH1F("LS_uGT_SingleTau70er2p1","LS_uGT_SingleTau70er2p1",3000,1.,3001.);
  TH1F* LS_uGT_DoubleIsoTau32er2p1 = new TH1F("LS_uGT_DoubleIsoTau32er2p1","LS_uGT_DoubleIsoTau32er2p1",3000,1.,3001.);
  TH1F* LS_uGT_DoubleIsoTau34er2p1 = new TH1F("LS_uGT_DoubleIsoTau34er2p1","LS_uGT_DoubleIsoTau34er2p1",3000,1.,3001.);


  // counters for missed trigger bits
  int missedBit178 = 0;
  int missedBit192 = 0;
  int missedBit218 = 0;
  int missedBit262 = 0;
  int missedBit263 = 0;
  int missedBit270 = 0;
  int missedBit271 = 0;
  int additionalBit178 = 0;
  int additionalBit192 = 0;
  int additionalBit218 = 0;
  int additionalBit262 = 0;
  int additionalBit263 = 0;
  int additionalBit270 = 0;
  int additionalBit271 = 0;

  Int_t Denominator = 0;

  float thisLSLumi = 0.;
  float nb = 0.;

  float scaleToLumi = 2.00E34;
  float scale = 0.;

  cout<<"begin loop"<<endl;
  cout<<inTree->GetEntries()<<endl;
 
  for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    { 
      inTree->GetEntry(i);
      if(i%100000==0) cout<<"Entry #"<<i<<endl; 
      // SET RUN INFO
      if (in_RunNumber == 323755 && in_RunNumber == run)
        {
          if(in_lumi<44 || in_lumi>544) continue;
          nb = 2554;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 357438 && in_RunNumber == run)
        { 
          if(in_lumi<135 || in_lumi>229) continue;
          nb = 2400;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 357440 && in_RunNumber == run)
        { 
          if(in_lumi<0 || in_lumi>293) continue;
          nb = 2400;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 357442 && in_RunNumber == run)
        { 
          if(in_lumi<0 || in_lumi>922) continue;
          nb = 2400;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 357696 && in_RunNumber == run)
        { 
          if(in_lumi<173 || in_lumi>403) continue;
          nb = 2400;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 357700 && in_RunNumber == run)
        { 
          if(in_lumi<0 || in_lumi>545) continue;
          nb = 2400;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 357802 && in_RunNumber == run)
        { 
          if(in_lumi<95 || in_lumi>205) continue;
          nb = 2448;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 360075 && in_RunNumber == run)
        { 
          // if(in_lumi<74 || (in_lumi>429 && in_lumi<462) || in_lumi>808) continue;
          if(in_lumi<76 || in_lumi>487) continue;
          nb = 2448;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 360090 && in_RunNumber == run)
        {
          if(in_lumi<0 || in_lumi>752) continue;
          nb = 2448;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 360458 && in_RunNumber == run)
        { 
          if(in_lumi<125 || in_lumi>248) continue;
          nb = 2450;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 360459 && in_RunNumber == run)
        { 
          // if(in_lumi<0 || in_lumi>1163) continue;
          if(in_lumi<0 || in_lumi>516) continue;
          nb = 2450;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 360460 && in_RunNumber == run)
        { 
          if(in_lumi<0 || in_lumi>1000) continue;
          nb = 2450;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 360927 && in_RunNumber == run)
        { 
          // if(in_lumi<37 || in_lumi==332 || in_lumi==582 || in_lumi>1930) continue;
          if(in_lumi<99 || in_lumi>719) continue;
          // if(in_lumi!=978) continue;
          // if(in_lumi!=980) continue;
          nb = 2450;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 361197 && in_RunNumber == run)
        { 
          if(in_lumi<10 || in_lumi>389) continue;
          nb = 2450;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 361303 && in_RunNumber == run)
        { 
          if(in_lumi<136 || in_lumi>621) continue;
          nb = 2450;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 361443 && in_RunNumber == run)
        { 
          if(in_lumi<73 || in_lumi>932) continue;
          nb = 2450;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else if (in_RunNumber == 361512 && in_RunNumber == run)
        { 
          if(in_lumi<105 || in_lumi>843) continue;
          nb = 2450;
          thisLSLumi = LumiByLS[in_lumi-1];
        }
      else
        {
          continue;
        }

      if (thisLSLumi == 0. || nb == 0)
        {
          std::cout << "ERROR: something went wrong with the run selection and the lumi/nb initialization" << std::endl;
          return;
        }

      scale = 0.001 * nb * 11245.6;
      if (doScaleToLumi) scale *= scaleToLumi / thisLSLumi;

      ++Denominator;
      LS_Denominator->Fill(in_lumi);

      int BX = 2; // in BXVector entry 2 is BX0
      if(in_bit21->at(BX))  { uGT_rateBit21 += 1;  LS_uGT_SingleMu22->Fill(in_lumi, scale); }
      if(in_bit178->at(BX)) { uGT_rateBit178 += 1; LS_uGT_SingleLooseIsoEG28er2p5->Fill(in_lumi, scale); }
      if(in_bit192->at(BX)) { uGT_rateBit192 += 1; LS_uGT_SingleIsoEG30er2p5->Fill(in_lumi, scale); }
      if(in_bit218->at(BX)) { uGT_rateBit218 += 1; LS_uGT_DoubleEG_25_12_er2p5->Fill(in_lumi, scale); }
      if(in_bit262->at(BX)) { uGT_rateBit262 += 1; LS_uGT_SingleIsoTau32er2p1->Fill(in_lumi, scale); }
      if(in_bit263->at(BX)) { uGT_rateBit263 += 1; LS_uGT_SingleTau70er2p1->Fill(in_lumi, scale); }
      if(in_bit270->at(BX)) { uGT_rateBit270 += 1; LS_uGT_DoubleIsoTau32er2p1->Fill(in_lumi, scale); }
      if(in_bit271->at(BX)) { uGT_rateBit271 += 1; LS_uGT_DoubleIsoTau34er2p1->Fill(in_lumi, scale); }

      std::vector<Int_t> Index_Taus;
      Index_Taus.push_back(-1); Index_Taus.push_back(-1);
      std::vector<Float_t> pt_Taus;
      pt_Taus.push_back(-99.); pt_Taus.push_back(-99.);

      std::vector<Int_t> Index_Taus_Iso;
      Index_Taus_Iso.push_back(-1); Index_Taus_Iso.push_back(-1);
      std::vector<Float_t> pt_Taus_Iso;
      pt_Taus_Iso.push_back(-99.); pt_Taus_Iso.push_back(-99.);

      float highestTauPt = -99.9;
      int highestTauIdx = -99;
      float highestTauIsoPt = -99.9;
      int highestTauIsoIdx = -99;
      for(UInt_t iL1Tau = 0 ; iL1Tau < in_l1tTauPt->size() ; ++iL1Tau)
        {
          if(fabs(in_l1tTauEta->at(iL1Tau))>2.1315) continue;

          //SingleTau trigger
          if(in_l1tTauPt->at(iL1Tau) >= highestTauPt)
            {
              highestTauPt = in_l1tTauPt->at(iL1Tau);
              highestTauIdx = iL1Tau;
            }

          if(in_l1tTauPt->at(iL1Tau) >= highestTauIsoPt && in_l1tTauIso->at(iL1Tau)>0)
            {
              highestTauIsoPt = in_l1tTauPt->at(iL1Tau);
              highestTauIsoIdx = iL1Tau;
            }

          //DiTau trigger
          if(in_l1tTauPt->at(iL1Tau)>=pt_Taus.at(0))
            {
              Index_Taus.at(1)=Index_Taus.at(0);
              pt_Taus.at(1)=pt_Taus.at(0);
              Index_Taus.at(0)=iL1Tau;
              pt_Taus.at(0)=in_l1tTauPt->at(iL1Tau);
            }
          else if(in_l1tTauPt->at(iL1Tau)>=pt_Taus.at(1))
            {
              Index_Taus.at(1)=iL1Tau;
              pt_Taus.at(1)=in_l1tTauPt->at(iL1Tau);
            }

          if(in_l1tTauPt->at(iL1Tau)>=pt_Taus_Iso.at(0) && in_l1tTauIso->at(iL1Tau)>0)
            {
              Index_Taus_Iso.at(1)=Index_Taus_Iso.at(0);
              pt_Taus_Iso.at(1)=pt_Taus_Iso.at(0);
              Index_Taus_Iso.at(0)=iL1Tau;
              pt_Taus_Iso.at(0)=in_l1tTauPt->at(iL1Tau);
            }
          else if(in_l1tTauPt->at(iL1Tau)>=pt_Taus_Iso.at(1) && in_l1tTauIso->at(iL1Tau)>0)
            {
              Index_Taus_Iso.at(1)=iL1Tau;
              pt_Taus_Iso.at(1)=in_l1tTauPt->at(iL1Tau);
            }

        }

      // 262 - L1_SingleIsoTau32er2p1
      // 263 - L1_SingleTau70er2p1
      // 270 - L1_DoubleIsoTau32er2p1
      // 271 - L1_DoubleIsoTau34er2p1


      //SingleTau trigger
      if(highestTauIdx != -99)
        {
          pt_SingleTau->Fill(in_l1tTauPt->at(highestTauIdx),scale);
          LS_pt_SingleTau->Fill(in_lumi,in_l1tTauPt->at(highestTauIdx),scale);

          if(in_l1tTauPt->at(highestTauIdx)>=70)
            {
              if(!in_bit263->at(BX)) missedBit263 += 1;
            }
          else
            {
              if(in_bit263->at(BX)) additionalBit263 += 1;
            }
        }
      else
        {
          if(in_bit263->at(BX)) additionalBit263 += 1;
        }

      if(highestTauIsoIdx != -99)
        {
          pt_Iso_SingleTau->Fill(in_l1tTauPt->at(highestTauIsoIdx),scale);
          LS_pt_Iso_SingleTau->Fill(in_lumi,in_l1tTauPt->at(highestTauIsoIdx),scale);

          if(in_l1tTauPt->at(highestTauIsoIdx)>=32)
            {
              if(!in_bit262->at(BX)) missedBit262 += 1;
            }
          else
            {
              if(in_bit262->at(BX)) additionalBit262 += 1;
            }
        }
      else
        {
          if(in_bit262->at(BX)) additionalBit262 += 1;
        }

      //DiTau trigger
      if(Index_Taus.at(0)>=0 && Index_Taus.at(1)>=0)
        {
          pt_DiTau->Fill(pt_Taus.at(0),pt_Taus.at(1),scale);
          LS_pt_DiTau->Fill(in_lumi,pt_Taus.at(0),pt_Taus.at(1),scale);
        }

      if(Index_Taus_Iso.at(0)>=0 && Index_Taus_Iso.at(1)>=0)
        {
          pt_Iso_DiTau->Fill(pt_Taus_Iso.at(0),pt_Taus_Iso.at(1),scale);
          LS_pt_Iso_DiTau->Fill(in_lumi,pt_Taus_Iso.at(0),pt_Taus_Iso.at(1),scale);

          if(pt_Taus_Iso.at(0)>=32 && pt_Taus_Iso.at(1)>=32)
            {
              if(!in_bit270->at(BX)) missedBit270 += 1;
            }
          else
            {
              if(in_bit270->at(BX)) additionalBit270 += 1;
            }

          if(pt_Taus_Iso.at(0)>=34 && pt_Taus_Iso.at(1)>=34)
            {
              if(!in_bit271->at(BX)) missedBit271 += 1;
            }
          else
            {
              if(in_bit271->at(BX)) additionalBit271 += 1;
            }
        }
      else
        {
          if(in_bit270->at(BX)) additionalBit270 += 1;
          if(in_bit271->at(BX)) additionalBit271 += 1;
        }


      std::vector<Int_t> Index_EGs;
      Index_EGs.push_back(-1); Index_EGs.push_back(-1);
      std::vector<Float_t> pt_EGs;
      pt_EGs.push_back(-99.); pt_EGs.push_back(-99.);

      std::vector<Int_t> Index_EGs_LooseIso;
      Index_EGs_LooseIso.push_back(-1); Index_EGs_LooseIso.push_back(-1);
      std::vector<Float_t> pt_EGs_LooseIso;
      pt_EGs_LooseIso.push_back(-99.); pt_EGs_LooseIso.push_back(-99.);

      std::vector<Int_t> Index_EGs_TightIso;
      Index_EGs_TightIso.push_back(-1); Index_EGs_TightIso.push_back(-1);
      std::vector<Float_t> pt_EGs_TightIso;
      pt_EGs_TightIso.push_back(-99.); pt_EGs_TightIso.push_back(-99.);

      float highestEGPt = -99.9;
      int highestEGIdx = -99;
      float highestLooseEGPt = -99.9;
      int highestLooseEGIdx = -99;
      float highestTightEGPt = -99.9;
      int highestTightEGIdx = -99;
      for(UInt_t iL1EG = 0 ; iL1EG < in_l1tEGPt->size() ; ++iL1EG)
        {
          // if(fabs(in_l1tEGEta->at(iL1EG))>2.1315) continue;
          if(fabs(in_l1tEGEta->at(iL1EG))>2.5229) continue;

          //SingleEG trigger
          if(in_l1tEGPt->at(iL1EG) >= highestEGPt)
            {
              highestEGPt = in_l1tEGPt->at(iL1EG);
              highestEGIdx = iL1EG;
            }

          if(in_l1tEGPt->at(iL1EG) >= highestLooseEGPt && (in_l1tEGIso->at(iL1EG)==2 || in_l1tEGIso->at(iL1EG)==3))
            {
              highestLooseEGPt = in_l1tEGPt->at(iL1EG);
              highestLooseEGIdx = iL1EG;
            }

          if(in_l1tEGPt->at(iL1EG) >= highestTightEGPt && (in_l1tEGIso->at(iL1EG)==1 || in_l1tEGIso->at(iL1EG)==3))
            {
              highestTightEGPt = in_l1tEGPt->at(iL1EG);
              highestTightEGIdx = iL1EG;
            }

          //DiEG trigger
          if(in_l1tEGPt->at(iL1EG)>=pt_EGs.at(0))
            {
              Index_EGs.at(1)=Index_EGs.at(0);
              pt_EGs.at(1)=pt_EGs.at(0);
              Index_EGs.at(0)=iL1EG;
              pt_EGs.at(0)=in_l1tEGPt->at(iL1EG);
            }
          else if(in_l1tEGPt->at(iL1EG)>=pt_EGs.at(1))
            {
              Index_EGs.at(1)=iL1EG;
              pt_EGs.at(1)=in_l1tEGPt->at(iL1EG);
            }
          
          if(in_l1tEGPt->at(iL1EG)>=pt_EGs_LooseIso.at(0) && (in_l1tEGIso->at(iL1EG)==2 || in_l1tEGIso->at(iL1EG)==3))
            {
              Index_EGs_LooseIso.at(1)=Index_EGs_LooseIso.at(0);
              pt_EGs_LooseIso.at(1)=pt_EGs_LooseIso.at(0);
              Index_EGs_LooseIso.at(0)=iL1EG;
              pt_EGs_LooseIso.at(0)=in_l1tEGPt->at(iL1EG);
            }
          else if(in_l1tEGPt->at(iL1EG)>=pt_EGs_LooseIso.at(1) && (in_l1tEGIso->at(iL1EG)==2 || in_l1tEGIso->at(iL1EG)==3))
            {
              Index_EGs_LooseIso.at(1)=iL1EG;
              pt_EGs_LooseIso.at(1)=in_l1tEGPt->at(iL1EG);
            }

          if(in_l1tEGPt->at(iL1EG)>=pt_EGs_TightIso.at(0) && (in_l1tEGIso->at(iL1EG)==1 || in_l1tEGIso->at(iL1EG)==3))
            {
              Index_EGs_TightIso.at(1)=Index_EGs_TightIso.at(0);
              pt_EGs_TightIso.at(1)=pt_EGs_TightIso.at(0);
              Index_EGs_TightIso.at(0)=iL1EG;
              pt_EGs_TightIso.at(0)=in_l1tEGPt->at(iL1EG);
            }
          else if(in_l1tEGPt->at(iL1EG)>=pt_EGs_TightIso.at(1) && (in_l1tEGIso->at(iL1EG)==1 || in_l1tEGIso->at(iL1EG)==3))
            {
              Index_EGs_TightIso.at(1)=iL1EG;
              pt_EGs_TightIso.at(1)=in_l1tEGPt->at(iL1EG);
            }

        }

      // 178 - L1_SingleLooseIsoEG28er2p5
      // 192 - L1_SingleIsoEG30er2p5
      // 218 - L1_DoubleEG_25_12_er2p5

      //SingleEG trigger
      if(highestEGIdx != -99)
        {
          pt_SingleEG->Fill(in_l1tEGPt->at(highestEGIdx),scale);
          LS_pt_SingleEG->Fill(in_lumi,in_l1tEGPt->at(highestEGIdx),scale);
        }

      if(highestLooseEGIdx != -99)
        {
          pt_LooseIso_SingleEG->Fill(in_l1tEGPt->at(highestLooseEGIdx),scale);
          LS_pt_LooseIso_SingleEG->Fill(in_lumi,in_l1tEGPt->at(highestLooseEGIdx),scale);
          
          if(in_l1tEGPt->at(highestLooseEGIdx)>=28)
            {
              if(!in_bit178->at(BX)) missedBit178 += 1;
            }
          else
            {
              if(in_bit178->at(BX)) additionalBit178 += 1;
            }
        }
      else
        {
          if(in_bit178->at(BX)) additionalBit178 += 1;
        }

      if(highestTightEGIdx != -99)
        {
          pt_TightIso_SingleEG->Fill(in_l1tEGPt->at(highestTightEGIdx),scale);
          LS_pt_TightIso_SingleEG->Fill(in_lumi,in_l1tEGPt->at(highestTightEGIdx),scale);

          if(in_l1tEGPt->at(highestTightEGIdx)>=30)
            {
              if(!in_bit192->at(BX)) missedBit192 += 1;
            }
          else
            {
              if(in_bit192->at(BX)) additionalBit192 += 1;
            }
        }
      else
        {
          if(in_bit192->at(BX)) additionalBit192 += 1;
        }

      //DiEG trigger
      if(Index_EGs.at(0)>=0 && Index_EGs.at(1)>=0)
        {
          pt_DiEG->Fill(pt_EGs.at(0),pt_EGs.at(1),scale);
          LS_pt_DiEG->Fill(in_lumi,pt_EGs.at(0),pt_EGs.at(1),scale);

          if(pt_EGs.at(0)>=25 && pt_EGs.at(1)>=12)
            {
              if(!in_bit218->at(BX)) missedBit218 += 1;
            }
          else
            {
              if(in_bit218->at(BX)) additionalBit218 += 1;
            }
        }
      else
        {
          if(in_bit218->at(BX)) additionalBit218 += 1;
        }

      if(Index_EGs_LooseIso.at(0)>=0 && Index_EGs_LooseIso.at(1)>=0)
        {
          pt_LooseIso_DiEG->Fill(pt_EGs_LooseIso.at(0),pt_EGs_LooseIso.at(1),scale);
          LS_pt_LooseIso_DiEG->Fill(in_lumi,pt_EGs_LooseIso.at(0),pt_EGs_LooseIso.at(1),scale);
        }

      if(Index_EGs_TightIso.at(0)>=0 && Index_EGs_TightIso.at(1)>=0)
        {
          pt_TightIso_DiEG->Fill(pt_EGs_TightIso.at(0),pt_EGs_TightIso.at(1),scale);
          LS_pt_TightIso_DiEG->Fill(in_lumi,pt_EGs_TightIso.at(0),pt_EGs_TightIso.at(1),scale);
        }


      for (int i = 0; i < 5; i++)
        {
          int ibx = -1;
          if(i == 0) ibx = -2;  // 0 <-> BX-2
          if(i == 1) ibx = -1;  // 1 <-> BX-1
          if(i == 2) ibx = 0;   // 2 <-> BX0
          if(i == 3) ibx = 1;   // 3 <-> BX+1
          if(i == 4) ibx = 2;   // 4 <-> BX+2

          if (in_bit21->at(i))  bit21_vs_BX->Fill(ibx);
          if (in_bit178->at(i)) bit178_vs_BX->Fill(ibx);
          if (in_bit192->at(i)) bit192_vs_BX->Fill(ibx);
          if (in_bit218->at(i)) bit218_vs_BX->Fill(ibx);
          if (in_bit262->at(i)) bit262_vs_BX->Fill(ibx);
          if (in_bit263->at(i)) bit263_vs_BX->Fill(ibx);
          if (in_bit270->at(i)) bit270_vs_BX->Fill(ibx);
          if (in_bit271->at(i)) bit271_vs_BX->Fill(ibx);
          // if (in_bit459->at(i)) bit459_vs_BX->Fill(ibx);
        }

    } // end of for loop on events

  cout << "Denominator = " << Denominator << endl;

  TH1F* rate_SingleTau = new TH1F("rate_SingleTau","rate_SingleTau",240,0.,240.);
  TH1F* rate_Iso_SingleTau = new TH1F("rate_Iso_SingleTau","rate_Iso_SingleTau",240,0.,240.);

  TH1F* rate_DiTau = new TH1F("rate_DiTau","rate_DiTau",240,0.,240.);
  TH1F* rate_Iso_DiTau = new TH1F("rate_Iso_DiTau","rate_Iso_DiTau",240,0.,240.);

  TH1F* rate_LooseIso_SingleEG = new TH1F("rate_LooseIso_SingleEG","rate_LooseIso_SingleEG",240,0.,240.);
  TH1F* rate_TightIso_SingleEG = new TH1F("rate_TightIso_SingleEG","rate_TightIso_SingleEG",240,0.,240.);


  TH1F* LS_SingleLooseIsoEG28er2p5 = new TH1F("LS_SingleLooseIsoEG28er2p5","LS_SingleLooseIsoEG28er2p5",3000,1.,3001.);
  TH1F* LS_SingleIsoEG30er2p5 = new TH1F("LS_SingleIsoEG30er2p5","LS_SingleIsoEG30er2p5",3000,1.,3001.);
  TH1F* LS_DoubleEG_25_12_er2p5 = new TH1F("LS_DoubleEG_25_12_er2p5","LS_DoubleEG_25_12_er2p5",3000,1.,3001.);
  TH1F* LS_SingleIsoTau32er2p1 = new TH1F("LS_SingleIsoTau32er2p1","LS_SingleIsoTau32er2p1",3000,1.,3001.);
  TH1F* LS_SingleTau70er2p1 = new TH1F("LS_SingleTau70er2p1","LS_SingleTau70er2p1",3000,1.,3001.);
  TH1F* LS_DoubleIsoTau32er2p1 = new TH1F("LS_DoubleIsoTau32er2p1","LS_DoubleIsoTau32er2p1",3000,1.,3001.);
  TH1F* LS_DoubleIsoTau34er2p1 = new TH1F("LS_DoubleIsoTau34er2p1","LS_DoubleIsoTau34er2p1",3000,1.,3001.);


  for(UInt_t i = 0 ; i < 241 ; ++i)
    {
      rate_DiTau->SetBinContent(i+1,pt_DiTau->Integral(i+1,241,i+1,241)/Denominator);
      rate_Iso_DiTau->SetBinContent(i+1,pt_Iso_DiTau->Integral(i+1,241,i+1,241)/Denominator);

      rate_SingleTau->SetBinContent(i+1,pt_SingleTau->Integral(i+1,241)/Denominator);
      rate_Iso_SingleTau->SetBinContent(i+1,pt_Iso_SingleTau->Integral(i+1,241)/Denominator);

      rate_LooseIso_SingleEG->SetBinContent(i+1,pt_LooseIso_SingleEG->Integral(i+1,241)/Denominator);
      rate_TightIso_SingleEG->SetBinContent(i+1,pt_TightIso_SingleEG->Integral(i+1,241)/Denominator);
    }

  for(int i = 1; i < 3001; ++i)
    {
      int norm = LS_Denominator->GetBinContent(i);

      if (norm>0.)
        {
          LS_SingleLooseIsoEG28er2p5->SetBinContent(i, 1000 * LS_pt_LooseIso_SingleEG->Integral(i,i,28,241) / norm);
          LS_SingleIsoEG30er2p5->SetBinContent(i, 1000 * LS_pt_TightIso_SingleEG->Integral(i,i,30,241) / norm);
          LS_DoubleEG_25_12_er2p5->SetBinContent(i, 1000 * LS_pt_DiEG->Integral(i,i,25+1,241,12+1,241) / norm);
          LS_SingleIsoTau32er2p1->SetBinContent(i, 1000 * LS_pt_Iso_SingleTau->Integral(i,i,32,241) / norm / 800);
          LS_SingleTau70er2p1->SetBinContent(i, 1000 * LS_pt_SingleTau->Integral(i,i,70,241) / norm / 50);
          LS_DoubleIsoTau32er2p1->SetBinContent(i, 1000 * LS_pt_Iso_DiTau->Integral(i,i,32,241,32,241) / norm);
          LS_DoubleIsoTau34er2p1->SetBinContent(i, 1000 * LS_pt_Iso_DiTau->Integral(i,i,34,241,34,241) / norm);

          LS_uGT_SingleMu22->SetBinContent(i, 1000 * LS_uGT_SingleMu22->GetBinContent(i));// / norm);
          LS_uGT_SingleLooseIsoEG28er2p5->SetBinContent(i, 1000 * LS_uGT_SingleLooseIsoEG28er2p5->GetBinContent(i));// / norm);
          LS_uGT_SingleIsoEG30er2p5->SetBinContent(i, 1000 * LS_uGT_SingleIsoEG30er2p5->GetBinContent(i));// / norm);
          LS_uGT_DoubleEG_25_12_er2p5->SetBinContent(i, 1000 * LS_uGT_DoubleEG_25_12_er2p5->GetBinContent(i));// / norm);
          LS_uGT_SingleIsoTau32er2p1->SetBinContent(i, 1000 * LS_uGT_SingleIsoTau32er2p1->GetBinContent(i));// / norm);
          LS_uGT_SingleTau70er2p1->SetBinContent(i, 1000 * LS_uGT_SingleTau70er2p1->GetBinContent(i));// / norm);
          LS_uGT_DoubleIsoTau32er2p1->SetBinContent(i, 1000 * LS_uGT_DoubleIsoTau32er2p1->GetBinContent(i));// / norm);
          LS_uGT_DoubleIsoTau34er2p1->SetBinContent(i, 1000 * LS_uGT_DoubleIsoTau34er2p1->GetBinContent(i));// / norm);
        }
    }


  std::cout << "-----------------------------------------------------------------------------------" << std::endl;
  std::cout << "scale = " << scale << " ; " << "denominator = " << Denominator << std::endl;
  std::cout << "-----------------------------------------------------------------------------------" << std::endl;
  std::cout << "178 - L1_SingleLooseIsoEG28er2p5 = " << 1000 * rate_LooseIso_SingleEG->GetBinContent(29) << " Hz" << std::endl;
  std::cout << "192 - L1_SingleIsoEG30er2p5 = " << 1000 * rate_TightIso_SingleEG->GetBinContent(31) << " Hz" << std::endl;
  std::cout << "218 - L1_DoubleEG_25_12_er2p5 = " << 1000 * pt_DiEG->Integral(26,241,12,241)/Denominator << " Hz" << std::endl;
  // std::cout << " - L1_DoubleEG_LooseIso22_12_er2p5 = " << 1000 * pt_LooseIso_DiEG->Integral(22+1,241,12+1,241)/Denominator << " Hz" << std::endl;
  std::cout << "262 - L1_SingleIsoTau32er2p1 = " << 1000 * rate_Iso_SingleTau->GetBinContent(33)/800 << " Hz" << std::endl;
  std::cout << "263 - L1_SingleTau70er2p1 = " << 1000 * rate_SingleTau->GetBinContent(71)/50 << " Hz" << std::endl;
  std::cout << "270 - L1_DoubleIsoTau32er2p1 = " << 1000 * rate_Iso_DiTau->GetBinContent(33) << " Hz" << std::endl;
  std::cout << "271 - L1_DoubleIsoTau34er2p1 = " << 1000 * rate_Iso_DiTau->GetBinContent(35) << " Hz" << std::endl;
  std::cout << "-----------------------------------------------------------------------------------" << std::endl;
  std::cout << "178 - Missed L1_SingleLooseIsoEG28er2p5 = " << missedBit178 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout << "192 - Missed L1_SingleIsoEG30er2p5 = " << missedBit192 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout << "218 - Missed L1_DoubleEG_25_12_er2p5 = " << missedBit218 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout << "262 - Missed L1_SingleIsoTau32er2p1 = " << missedBit262 * scale * 1000 / Denominator / 800 << " Hz" << std::endl;
  std::cout << "263 - Missed L1_SingleTau70er2p1 = "    << missedBit263 * scale * 1000 / Denominator / 50 << " Hz" << std::endl;
  std::cout << "270 - Missed L1_DoubleIsoTau32er2p1 = " << missedBit270 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout << "271 - Missed L1_DoubleIsoTau34er2p1 = " << missedBit271 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout << "-----------------------------------------------------------------------------------" << std::endl;
  std::cout << "178 - Additional L1_SingleLooseIsoEG28er2p5 = " << additionalBit178 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout << "192 - Additional L1_SingleIsoEG30er2p5 = " << additionalBit192 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout << "218 - Additional L1_DoubleEG_25_12_er2p5 = " << additionalBit218 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout << "262 - Additional L1_SingleIsoTau32er2p1 = " << additionalBit262 * scale * 1000 / Denominator / 800 << " Hz" << std::endl;
  std::cout << "263 - Additional L1_SingleTau70er2p1 = "    << additionalBit263 * scale * 1000 / Denominator / 50 << " Hz" << std::endl;
  std::cout << "270 - Additional L1_DoubleIsoTau32er2p1 = " << additionalBit270 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout << "271 - Additional L1_DoubleIsoTau34er2p1 = " << additionalBit271 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout << "-----------------------------------------------------------------------------------" << std::endl;
  // std::cout<< "21  - uGT rate L1_SingleMu22 = " << uGT_rateBit21 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout<< "178 - uGT rate L1_SingleLooseIsoEG28er2p5 = " << uGT_rateBit178 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout<< "192 - uGT rate L1_SingleIsoEG30er2p5 = " << uGT_rateBit192 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout<< "218 - uGT rate L1_DoubleEG_25_12_er2p5 = " << uGT_rateBit218 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout<< "262 - uGT rate L1_SingleIsoTau32er2p1 = " << uGT_rateBit262 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout<< "263 - uGT rate L1_SingleTau70er2p1 = "    << uGT_rateBit263 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout<< "270 - uGT rate L1_DoubleIsoTau32er2p1 = " << uGT_rateBit270 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout<< "271 - uGT rate L1_DoubleIsoTau34er2p1 = " << uGT_rateBit271 * scale * 1000 / Denominator << " Hz" << std::endl;
  std::cout << "-----------------------------------------------------------------------------------" << std::endl;

  TString scaledToLumi = "";
  if (doScaleToLumi) scaledToLumi = "_scaledTo2e34Lumi";
  // TFile f("histos/histos_rate_EphemeralZeroBias_2018D_Run323755_unpacked"+scaledToLumi+".root","RECREATE");
  TFile f("histos/histos_rate_ZeroBias_Run2022"+era+"_Run"+run_str+"_unpacked_LumiByLS"+scaledToLumi+".root","RECREATE");

  pt_SingleTau->Write();
  pt_Iso_SingleTau->Write();

  pt_DiTau->Write();
  pt_Iso_DiTau->Write();

  pt_SingleEG->Write();
  pt_DiEG->Write();

  pt_LooseIso_SingleEG->Write();
  pt_LooseIso_DiEG->Write();

  pt_TightIso_SingleEG->Write();
  pt_TightIso_DiEG->Write();

  rate_SingleTau->Write();
  rate_Iso_SingleTau->Write();

  rate_DiTau->Write();
  rate_Iso_DiTau->Write();

  rate_LooseIso_SingleEG->Write();
  rate_TightIso_SingleEG->Write();

  LS_SingleLooseIsoEG28er2p5->Write();
  LS_SingleIsoEG30er2p5->Write();
  LS_DoubleEG_25_12_er2p5->Write();
  LS_SingleIsoTau32er2p1->Write();
  LS_SingleTau70er2p1->Write();
  LS_DoubleIsoTau32er2p1->Write();
  LS_DoubleIsoTau34er2p1->Write();

  LS_Denominator->Write();

  LS_uGT_SingleMu22->Write();
  LS_uGT_SingleLooseIsoEG28er2p5->Write();
  LS_uGT_SingleIsoEG30er2p5->Write();
  LS_uGT_DoubleEG_25_12_er2p5->Write();
  LS_uGT_SingleIsoTau32er2p1->Write();
  LS_uGT_SingleTau70er2p1->Write();
  LS_uGT_DoubleIsoTau32er2p1->Write();
  LS_uGT_DoubleIsoTau34er2p1->Write();

  bit21_vs_BX->Write();
  bit178_vs_BX->Write();
  bit192_vs_BX->Write();
  bit218_vs_BX->Write();
  bit262_vs_BX->Write();
  bit263_vs_BX->Write();
  bit270_vs_BX->Write();
  bit271_vs_BX->Write();
  bit459_vs_BX->Write();

  return;
}

