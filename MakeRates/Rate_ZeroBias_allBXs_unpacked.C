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

using namespace std;

void Rate(TString era, bool ephemeral, bool doScaleToLumi=false)
{
  // TString FileName_in = "/data_CMS/cms/motta/Run3preparation/EphemeralZeroBias_2018D_Run323755_allBXbits__RAW/EphemeralZeroBias_2018D_Run323755_allBXbits__RAW.root";
  TString FileName_in = "/data_CMS/cms/motta/Run3preparation/ZeroBias__Run2022"+era+"__RAW/ZeroBias__Run2022"+era+"__RAW.root";
  if (ephemeral) { FileName_in = "/data_CMS/cms/motta/Run3preparation/EphemeralZeroBias0__Run2022"+era+"__RAW/EphemeralZeroBias0__Run2022"+era+"__RAW.root"; }
  TFile f_in(FileName_in.Data(),"READ");
  TTree* inTree = (TTree*)f_in.Get("ZeroBias/ZeroBias");

  ULong64_t       in_EventNumber =  0;
  Int_t           in_RunNumber =  0;
  Int_t           in_lumi =  0;
  std::vector< std::vector<float> >   *in_l1tTauPt =  0;
  std::vector< std::vector<float> >   *in_l1tTauEta =  0;
  std::vector< std::vector<int> >     *in_l1tTauIso = 0;
  std::vector< std::vector<float> >   *in_l1tEGPt =  0;
  std::vector< std::vector<float> >   *in_l1tEGEta =  0;
  std::vector< std::vector<int> >     *in_l1tEGIso = 0;
  std::vector< std::vector<float> >   *in_l1tMuPt =  0;
  std::vector< std::vector<float> >   *in_l1tMuEta =  0;
  std::vector< std::vector<int> >     *in_l1tMuQual = 0;

  std::vector<int> *in_bit21 = 0;
  std::vector<int> *in_bit168 = 0;
  std::vector<int> *in_bit169 = 0;
  std::vector<int> *in_bit170 = 0;
  std::vector<int> *in_bit171 = 0;
  std::vector<int> *in_bit172 = 0;
  std::vector<int> *in_bit178 = 0;
  std::vector<int> *in_bit192 = 0;
  std::vector<int> *in_bit194 = 0;
  std::vector<int> *in_bit196 = 0;
  std::vector<int> *in_bit218 = 0;
  std::vector<int> *in_bit219 = 0;
  std::vector<int> *in_bit220 = 0;
  std::vector<int> *in_bit262 = 0;
  std::vector<int> *in_bit263 = 0;
  std::vector<int> *in_bit264 = 0;
  std::vector<int> *in_bit267 = 0;
  std::vector<int> *in_bit270 = 0;
  std::vector<int> *in_bit271 = 0;
  std::vector<int> *in_bit272 = 0;
  std::vector<int> *in_bit273 = 0;
  std::vector<int> *in_bit459 = 0;

  inTree->SetBranchAddress("EventNumber", &in_EventNumber);
  inTree->SetBranchAddress("RunNumber", &in_RunNumber);
  inTree->SetBranchAddress("lumi", &in_lumi);
  inTree->SetBranchAddress("l1tTauPt", &in_l1tTauPt);
  inTree->SetBranchAddress("l1tTauEta", &in_l1tTauEta);
  inTree->SetBranchAddress("l1tTauIso", &in_l1tTauIso);
  inTree->SetBranchAddress("l1tEGPt", &in_l1tEGPt);
  inTree->SetBranchAddress("l1tEGEta", &in_l1tEGEta);
  inTree->SetBranchAddress("l1tEGIso", &in_l1tEGIso);
  inTree->SetBranchAddress("l1tMuPt",&in_l1tMuPt);
  inTree->SetBranchAddress("l1tMuEta",&in_l1tMuEta);
  inTree->SetBranchAddress("l1tMuQual",&in_l1tMuQual);

  inTree->SetBranchAddress("bit21",  &in_bit21);  // L1_SingleMu22
  inTree->SetBranchAddress("bit168", &in_bit168); // L1_SingleEG36er2p5
  inTree->SetBranchAddress("bit169", &in_bit169); // L1_SingleEG38er2p5
  inTree->SetBranchAddress("bit170", &in_bit170); // L1_SingleEG40er2p5
  inTree->SetBranchAddress("bit171", &in_bit171); // L1_SingleEG42er2p5
  inTree->SetBranchAddress("bit172", &in_bit172); // L1_SingleEG45er2p5
  inTree->SetBranchAddress("bit178", &in_bit178); // L1_SingleLooseIsoEG28er2p5
  inTree->SetBranchAddress("bit192", &in_bit192); // L1_SingleIsoEG30er2p5
  inTree->SetBranchAddress("bit194", &in_bit194); // L1_SingleIsoEG32er2p5
  inTree->SetBranchAddress("bit196", &in_bit196); // L1_SingleIsoEG34er2p5
  inTree->SetBranchAddress("bit218", &in_bit218); // L1_DoubleEG_25_12_er2p5
  inTree->SetBranchAddress("bit219", &in_bit219); // L1_DoubleEG_25_12_er2p5
  inTree->SetBranchAddress("bit220", &in_bit220); // L1_DoubleEG_25_12_er2p5
  inTree->SetBranchAddress("bit262", &in_bit262); // L1_SingleIsoTau32er2p1
  inTree->SetBranchAddress("bit263", &in_bit263); // L1_SingleTau70er2p1
  inTree->SetBranchAddress("bit264", &in_bit264); // L1_SingleTau120er2p1
  inTree->SetBranchAddress("bit267", &in_bit267); // L1_DoubleTau70er2p1
  inTree->SetBranchAddress("bit270", &in_bit270); // L1_DoubleIsoTau32er2p1
  inTree->SetBranchAddress("bit271", &in_bit271); // L1_DoubleIsoTau34er2p1
  inTree->SetBranchAddress("bit272", &in_bit272); // L1_DoubleIsoTau35er2p1
  inTree->SetBranchAddress("bit273", &in_bit273); // L1_DoubleIsoTau36er2p1
  inTree->SetBranchAddress("bit459", &in_bit459); // L1_ZeroBias

  TH1F* pt_SingleMu = new TH1F("pt_SingleMu","pt_SingleMu",600,0.,600.);

  TH1F* pt_SingleTau = new TH1F("pt_SingleTau","pt_SingleTau",600,0.,600.);
  TH1F* pt_Iso_SingleTau = new TH1F("pt_Iso_SingleTau","pt_Iso_SingleTau",600,0.,600.);

  TH2F* pt_DiTau = new TH2F("pt_DiTau","pt_DiTau",600,0.,600.,600,0.,600.);
  TH2F* pt_Iso_DiTau = new TH2F("pt_Iso_DiTau","pt_Iso_DiTau",600,0.,600.,600,0.,600.);

  TH1F* pt_SingleEG = new TH1F("pt_SingleEG","pt_SingleEG",600,0.,600.);
  TH2F* pt_DiEG = new TH2F("pt_DiEG","pt_DiEG",600,0.,600.,600,0.,600.);

  TH1F* pt_LooseIso_SingleEG = new TH1F("pt_LooseIso_SingleEG","pt_LooseIso_SingleEG",600,0.,600.);
  TH2F* pt_LooseIso_DiEG = new TH2F("pt_LooseIso_DiEG","pt_LooseIso_DiEG",600,0.,600.,600,0.,600.);

  TH1F* pt_TightIso_SingleEG = new TH1F("pt_TightIso_SingleEG","pt_TightIso_SingleEG",600,0.,600.);
  TH2F* pt_TightIso_DiEG = new TH2F("pt_TightIso_DiEG","pt_TightIso_DiEG",600,0.,600.,600,0.,600.);

  TH1F* bit21_vs_BX = new TH1F("SingleMu22_vs_BX", "SingleMu22_vs_BX", 5, -2.5, 2.5);
  TH1F* bit178_vs_BX = new TH1F("SingleLooseIsoEG28er2p5_vs_BX", "SingleLooseIsoEG28er2p5_vs_BX", 5, -2.5, 2.5);
  TH1F* bit192_vs_BX = new TH1F("SingleIsoEG30er2p5_vs_BX", "SingleIsoEG30er2p5_vs_BX", 5, -2.5, 2.5);
  TH1F* bit218_vs_BX = new TH1F("DoubleEG_25_12_er2p5_vs_BX", "DoubleEG_25_12_er2p5_vs_BX", 5, -2.5, 2.5);
  TH1F* bit262_vs_BX = new TH1F("SingleIsoTau32er2p1_vs_BX", "SingleIsoTau32er2p1_vs_BX", 5, -2.5, 2.5);
  TH1F* bit263_vs_BX = new TH1F("SingleTau70er2p1_vs_BX", "SingleTau70er2p1_vs_BX", 5, -2.5, 2.5);
  TH1F* bit270_vs_BX = new TH1F("DoubleIsoTau32er2p1_vs_BX", "DoubleIsoTau32er2p1_vs_BX", 5, -2.5, 2.5);
  TH1F* bit271_vs_BX = new TH1F("DoubleIsoTau34er2p1_vs_BX", "DoubleIsoTau34er2p1_vs_BX", 5, -2.5, 2.5);
  TH1F* bit459_vs_BX = new TH1F("ZeroBias_vs_BX", "ZeroBias_vs_BX", 5, -2.5, 2.5);


  Int_t Denominator = 0;

  float thisLumiRun = 0.;
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
      if (in_RunNumber == 323755)
        {
          if(in_lumi<44 || in_lumi>544) continue;
          nb = 2554;
          thisLumiRun = 1.6225E34;
        }
      /*if (in_RunNumber == 357438)
        { 
          if(in_lumi<135 || in_lumi>229) continue;
          nb = 2400;
          thisLumiRun = 1.64E34;
        }*/
      else if (in_RunNumber == 357440)
        { 
          if(in_lumi<0 || in_lumi>293) continue;
          nb = 2400;
          thisLumiRun = 1.47E34;
        }
      /*if (in_RunNumber == 357442)
        { 
          if(in_lumi<0 || in_lumi>922) continue;
          nb = 2400;
          if(in_lumi>0 && in_lumi<=382) thisLumiRun = 1.17E34;
          if(in_lumi>382 && in_lumi<=922) thisLumiRun = 0.99E34;
        }*/
      else if (in_RunNumber == 357696)
        { 
          if(in_lumi<173 || in_lumi>403) continue;
          nb = 2400;
          thisLumiRun = 1.67E34;
        }
      /*if (in_RunNumber == 357700)
        { 
          if(in_lumi<0 || in_lumi>545) continue;
          nb = 2400;
          if(in_lumi>0 && in_lumi<=357) thisLumiRun = 1.31E34;
          if(in_lumi>357 && in_lumi<=545) thisLumiRun = 1.17E34;
        }*/
      else if (in_RunNumber == 357802)
        { 
          if(in_lumi<95 || in_lumi>205) continue;
          nb = 2448;
          thisLumiRun = 1.57E34;
        }
      else if (in_RunNumber == 359998)
        { 
          if(in_lumi<244 || in_lumi>417) continue;
          nb = 2448;
          thisLumiRun = 1.82E34;
        }
      else if (in_RunNumber == 360075)
        { 
          // if(in_lumi<74 || (in_lumi>429 && in_lumi<462) || in_lumi>808) continue;
          if(in_lumi<76 || in_lumi>487) continue;
          thisLumiRun = 1.84E34;
          nb = 2448;
          if(in_lumi>73 && in_lumi<=429) thisLumiRun = 1.84E34;
          // if(in_lumi>461 && in_lumi<=681) thisLumiRun = 1.71E34;
          // if(in_lumi>681 && in_lumi<=808) thisLumiRun = 1.55E34;
        }
      /*if (in_RunNumber == 360090)
        {
          if(in_lumi<0 || in_lumi>752) continue;
          nb = 2448;
          if(in_lumi>0 && in_lumi<=408) thisLumiRun = 1.17E34;
          if(in_lumi>408 && in_lumi<752) thisLumiRun = 1.02E34;
        }
      if (in_RunNumber == 360458)
        { 
          if(in_lumi<125 || in_lumi>248) continue;
          nb = 2450;
          thisLumiRun = 1.71E34;
        }*/
      else if (in_RunNumber == 360459)
        { 
          // if(in_lumi<0 || in_lumi>1163) continue;
          if(in_lumi<0 || in_lumi>516) continue;
          nb = 2450;
          if(in_lumi>0 && in_lumi<=516) thisLumiRun = 1.69E34;
          // if(in_lumi>516 && in_lumi<=808) thisLumiRun = 1.49E34;
          // if(in_lumi>808 && in_lumi<=1163) thisLumiRun = 1.29E34;
        }
      /*if (in_RunNumber == 360460)
        { 
          if(in_lumi<0 || in_lumi>1000) continue;
          nb = 2450;
          if(in_lumi>0 && in_lumi<=561) thisLumiRun = 1.01E34;
          if(in_lumi>561 && in_lumi<=1000) thisLumiRun = 0.84E34;
        }*/
      else if (in_RunNumber == 360820)
        { 
          if(in_lumi<254 || in_lumi>657) continue;
          nb = 2352;
          thisLumiRun = 1.85E34;
        }
      else if (in_RunNumber == 360927)
        { 
          if(in_lumi<99 || in_lumi>719) continue;
          nb = 2450;
          thisLumiRun = 1.85E34;
        }
      else if (in_RunNumber == 361197)
        { 
          if(in_lumi<10 || in_lumi>389) continue;
          nb = 2450;
          thisLumiRun = 1.85E34;
        }
      else if (in_RunNumber == 361303)
        { 
          if(in_lumi<136 || in_lumi>621) continue;
          nb = 2450;
          thisLumiRun = 1.83E34;
        }
      else if (in_RunNumber == 361443)
        { 
          if(in_lumi<73 || in_lumi>932) continue;
          nb = 2450;
          thisLumiRun = 1.85E34;
        }
      else if (in_RunNumber == 361512)
        { 
          if(in_lumi<105 || in_lumi>843) continue;
          nb = 2450;
          thisLumiRun = 1.87E34;
        }
      else if (in_RunNumber == 362616)
        { 
          if(in_lumi<0) continue;
          nb = 2450;
          thisLumiRun = 2.05E34;
        }
      else if (in_RunNumber == 362617)
        { 
          if(in_lumi<0) continue;
          nb = 2450;
          thisLumiRun = 2.50E34;
        }
      else
        {
          continue;
        }

      if (thisLumiRun == 0. || nb == 0)
        {
          std::cout << "ERROR: something went wrong with the run selection and the lumi/nb initialization" << std::endl;
          return;
        }

      scale = 0.001 * nb * 11245.6;
      if (doScaleToLumi) scale *= scaleToLumi / thisLumiRun;

      ++Denominator;

      int BX = 2; // in BXVector entry 2 is BX0

      //SingleMu trigger
      float highestMuPt = -99.9;
      int highestMuIdx = -99;
      for(UInt_t iL1Mu = 0 ; iL1Mu < in_l1tMuPt->at(BX).size() ; ++iL1Mu)
        {
          if (in_l1tMuQual->at(BX).at(iL1Mu) < 12) continue;

          //SingleMu trigger
          if(in_l1tMuPt->at(BX).at(iL1Mu) >= highestMuPt)
            {
              highestMuPt = in_l1tMuPt->at(BX).at(iL1Mu);
              highestMuIdx = iL1Mu;
            }
        }

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
      for(UInt_t iL1Tau = 0 ; iL1Tau < in_l1tTauPt->at(BX).size() ; ++iL1Tau)
        {
          if(fabs(in_l1tTauEta->at(BX).at(iL1Tau))>2.1315) continue;

          //SingleTau trigger
          if(in_l1tTauPt->at(BX).at(iL1Tau) >= highestTauPt)
            {
              highestTauPt = in_l1tTauPt->at(BX).at(iL1Tau);
              highestTauIdx = iL1Tau;
            }

          if(in_l1tTauPt->at(BX).at(iL1Tau) >= highestTauIsoPt && in_l1tTauIso->at(BX).at(iL1Tau)>0)
            {
              highestTauIsoPt = in_l1tTauPt->at(BX).at(iL1Tau);
              highestTauIsoIdx = iL1Tau;
            }

          //DiTau trigger
          if(in_l1tTauPt->at(BX).at(iL1Tau)>=pt_Taus.at(0))
            {
              Index_Taus.at(1)=Index_Taus.at(0);
              pt_Taus.at(1)=pt_Taus.at(0);
              Index_Taus.at(0)=iL1Tau;
              pt_Taus.at(0)=in_l1tTauPt->at(BX).at(iL1Tau);
            }
          else if(in_l1tTauPt->at(BX).at(iL1Tau)>=pt_Taus.at(1))
            {
              Index_Taus.at(1)=iL1Tau;
              pt_Taus.at(1)=in_l1tTauPt->at(BX).at(iL1Tau);
            }

          if(in_l1tTauPt->at(BX).at(iL1Tau)>=pt_Taus_Iso.at(0) && in_l1tTauIso->at(BX).at(iL1Tau)>0)
            {
              Index_Taus_Iso.at(1)=Index_Taus_Iso.at(0);
              pt_Taus_Iso.at(1)=pt_Taus_Iso.at(0);
              Index_Taus_Iso.at(0)=iL1Tau;
              pt_Taus_Iso.at(0)=in_l1tTauPt->at(BX).at(iL1Tau);
            }
          else if(in_l1tTauPt->at(BX).at(iL1Tau)>=pt_Taus_Iso.at(1) && in_l1tTauIso->at(BX).at(iL1Tau)>0)
            {
              Index_Taus_Iso.at(1)=iL1Tau;
              pt_Taus_Iso.at(1)=in_l1tTauPt->at(BX).at(iL1Tau);
            }

        }

      //SingleMu trigger
      if(highestMuIdx != -99)
        {
          pt_SingleMu->Fill(in_l1tMuPt->at(BX).at(highestMuIdx),scale);
        }

      //SingleTau trigger
      if(highestTauIdx != -99)
        {
          pt_SingleTau->Fill(in_l1tTauPt->at(BX).at(highestTauIdx),scale);
        }

      if(highestTauIsoIdx != -99)
        {
          pt_Iso_SingleTau->Fill(in_l1tTauPt->at(BX).at(highestTauIsoIdx),scale);
        }

      //DiTau trigger
      if(Index_Taus.at(0)>=0 && Index_Taus.at(1)>=0)
        {
          pt_DiTau->Fill(pt_Taus.at(0),pt_Taus.at(1),scale);
        }

      if(Index_Taus_Iso.at(0)>=0 && Index_Taus_Iso.at(1)>=0)
        {
          pt_Iso_DiTau->Fill(pt_Taus_Iso.at(0),pt_Taus_Iso.at(1),scale);
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
      for(UInt_t iL1EG = 0 ; iL1EG < in_l1tEGPt->at(BX).size() ; ++iL1EG)
        {
          // if(fabs(in_l1tEGEta->at(BX).at(iL1EG))>2.1315) continue;
          if(fabs(in_l1tEGEta->at(BX).at(iL1EG))>2.5229) continue;

          //SingleEG trigger
          if(in_l1tEGPt->at(BX).at(iL1EG) >= highestEGPt)
            {
              highestEGPt = in_l1tEGPt->at(BX).at(iL1EG);
              highestEGIdx = iL1EG;
            }

          if(in_l1tEGPt->at(BX).at(iL1EG) >= highestLooseEGPt && (in_l1tEGIso->at(BX).at(iL1EG)==2 || in_l1tEGIso->at(BX).at(iL1EG)==3))
            {
              highestLooseEGPt = in_l1tEGPt->at(BX).at(iL1EG);
              highestLooseEGIdx = iL1EG;
            }

          if(in_l1tEGPt->at(BX).at(iL1EG) >= highestTightEGPt && (in_l1tEGIso->at(BX).at(iL1EG)==1 || in_l1tEGIso->at(BX).at(iL1EG)==3))
            {
              highestTightEGPt = in_l1tEGPt->at(BX).at(iL1EG);
              highestTightEGIdx = iL1EG;
            }

          //DiEG trigger
          if(in_l1tEGPt->at(BX).at(iL1EG)>=pt_EGs.at(0))
            {
              Index_EGs.at(1)=Index_EGs.at(0);
              pt_EGs.at(1)=pt_EGs.at(0);
              Index_EGs.at(0)=iL1EG;
              pt_EGs.at(0)=in_l1tEGPt->at(BX).at(iL1EG);
            }
          else if(in_l1tEGPt->at(BX).at(iL1EG)>=pt_EGs.at(1))
            {
              Index_EGs.at(1)=iL1EG;
              pt_EGs.at(1)=in_l1tEGPt->at(BX).at(iL1EG);
            }
          
          if(in_l1tEGPt->at(BX).at(iL1EG)>=pt_EGs_LooseIso.at(0) && (in_l1tEGIso->at(BX).at(iL1EG)==2 || in_l1tEGIso->at(BX).at(iL1EG)==3))
            {
              Index_EGs_LooseIso.at(1)=Index_EGs_LooseIso.at(0);
              pt_EGs_LooseIso.at(1)=pt_EGs_LooseIso.at(0);
              Index_EGs_LooseIso.at(0)=iL1EG;
              pt_EGs_LooseIso.at(0)=in_l1tEGPt->at(BX).at(iL1EG);
            }
          else if(in_l1tEGPt->at(BX).at(iL1EG)>=pt_EGs_LooseIso.at(1) && (in_l1tEGIso->at(BX).at(iL1EG)==2 || in_l1tEGIso->at(BX).at(iL1EG)==3))
            {
              Index_EGs_LooseIso.at(1)=iL1EG;
              pt_EGs_LooseIso.at(1)=in_l1tEGPt->at(BX).at(iL1EG);
            }

          if(in_l1tEGPt->at(BX).at(iL1EG)>=pt_EGs_TightIso.at(0) && (in_l1tEGIso->at(BX).at(iL1EG)==1 || in_l1tEGIso->at(BX).at(iL1EG)==3))
            {
              Index_EGs_TightIso.at(1)=Index_EGs_TightIso.at(0);
              pt_EGs_TightIso.at(1)=pt_EGs_TightIso.at(0);
              Index_EGs_TightIso.at(0)=iL1EG;
              pt_EGs_TightIso.at(0)=in_l1tEGPt->at(BX).at(iL1EG);
            }
          else if(in_l1tEGPt->at(BX).at(iL1EG)>=pt_EGs_TightIso.at(1) && (in_l1tEGIso->at(BX).at(iL1EG)==1 || in_l1tEGIso->at(BX).at(iL1EG)==3))
            {
              Index_EGs_TightIso.at(1)=iL1EG;
              pt_EGs_TightIso.at(1)=in_l1tEGPt->at(BX).at(iL1EG);
            }

        }

      // 178 - L1_SingleLooseIsoEG28er2p5
      // 192 - L1_SingleIsoEG30er2p5
      // 218 - L1_DoubleEG_25_12_er2p5

      //SingleEG trigger
      if(highestEGIdx != -99)
        {
          pt_SingleEG->Fill(in_l1tEGPt->at(BX).at(highestEGIdx),scale);
        }

      if(highestLooseEGIdx != -99)
        {
          pt_LooseIso_SingleEG->Fill(in_l1tEGPt->at(BX).at(highestLooseEGIdx),scale);  
        }

      if(highestTightEGIdx != -99)
        {
          pt_TightIso_SingleEG->Fill(in_l1tEGPt->at(BX).at(highestTightEGIdx),scale);
        }

      //DiEG trigger
      if(Index_EGs.at(0)>=0 && Index_EGs.at(1)>=0)
        {
          pt_DiEG->Fill(pt_EGs.at(0),pt_EGs.at(1),scale);
        }

      if(Index_EGs_LooseIso.at(0)>=0 && Index_EGs_LooseIso.at(1)>=0)
        {
          pt_LooseIso_DiEG->Fill(pt_EGs_LooseIso.at(0),pt_EGs_LooseIso.at(1),scale);
        }

      if(Index_EGs_TightIso.at(0)>=0 && Index_EGs_TightIso.at(1)>=0)
        {
          pt_TightIso_DiEG->Fill(pt_EGs_TightIso.at(0),pt_EGs_TightIso.at(1),scale);
        }


      for (int i = 0; i < 5; i++)
        {
          int ibx = -1;
          if(i == 0) ibx = -2;  // 0 <-> BX-2
          if(i == 1) ibx = -1;  // 1 <-> BX-1
          if(i == 2) ibx = 0;   // 2 <-> BX0
          if(i == 3) ibx = 1;   // 3 <-> BX+1
          if(i == 4) ibx = 2;   // 4 <-> BX+2

          if (in_bit21->at(2) && in_bit21->at(i))  bit21_vs_BX->Fill(ibx);
          if (in_bit178->at(2) && in_bit178->at(i)) bit178_vs_BX->Fill(ibx);
          if (in_bit192->at(2) && in_bit192->at(i)) bit192_vs_BX->Fill(ibx);
          if (in_bit218->at(2) && in_bit218->at(i)) bit218_vs_BX->Fill(ibx);
          if (in_bit262->at(2) && in_bit262->at(i)) bit262_vs_BX->Fill(ibx);
          if (in_bit263->at(2) && in_bit263->at(i)) bit263_vs_BX->Fill(ibx);
          if (in_bit270->at(2) && in_bit270->at(i)) bit270_vs_BX->Fill(ibx);
          if (in_bit271->at(2) && in_bit271->at(i)) bit271_vs_BX->Fill(ibx);
          if (in_bit459->at(2) && in_bit459->at(i)) bit459_vs_BX->Fill(ibx);
        }

    } // end of for loop on events

  cout << "Denominator = " << Denominator << endl;

  TH1F* rate_SingleMu = new TH1F("rate_SingleMu","rate_SingleMu",600,0.,600.);

  TH1F* rate_SingleTau = new TH1F("rate_SingleTau","rate_SingleTau",600,0.,600.);
  TH1F* rate_Iso_SingleTau = new TH1F("rate_Iso_SingleTau","rate_Iso_SingleTau",600,0.,600.);

  TH1F* rate_DiTau = new TH1F("rate_DiTau","rate_DiTau",600,0.,600.);
  TH1F* rate_Iso_DiTau = new TH1F("rate_Iso_DiTau","rate_Iso_DiTau",600,0.,600.);

  TH1F* rate_SingleEG = new TH1F("rate_SingleEG","rate_SingleEG",600,0.,600.);
  TH1F* rate_LooseIso_SingleEG = new TH1F("rate_LooseIso_SingleEG","rate_LooseIso_SingleEG",600,0.,600.);
  TH1F* rate_TightIso_SingleEG = new TH1F("rate_TightIso_SingleEG","rate_TightIso_SingleEG",600,0.,600.);

  for(UInt_t i = 0 ; i < 601 ; ++i)
    {
      rate_SingleMu->SetBinContent(i+1,pt_SingleMu->Integral(i+1,601)/Denominator);

      rate_DiTau->SetBinContent(i+1,pt_DiTau->Integral(i+1,601,i+1,601)/Denominator);
      rate_Iso_DiTau->SetBinContent(i+1,pt_Iso_DiTau->Integral(i+1,601,i+1,601)/Denominator);

      rate_SingleTau->SetBinContent(i+1,pt_SingleTau->Integral(i+1,601)/Denominator);
      rate_Iso_SingleTau->SetBinContent(i+1,pt_Iso_SingleTau->Integral(i+1,601)/Denominator);

      rate_SingleEG->SetBinContent(i+1,pt_SingleEG->Integral(i+1,601)/Denominator);
      rate_LooseIso_SingleEG->SetBinContent(i+1,pt_LooseIso_SingleEG->Integral(i+1,601)/Denominator);
      rate_TightIso_SingleEG->SetBinContent(i+1,pt_TightIso_SingleEG->Integral(i+1,601)/Denominator);
    }


  std::cout << "-----------------------------------------------------------------------------------" << std::endl;
  std::cout << "scale = " << scale << " ; " << "denominator = " << Denominator << std::endl;
  std::cout << "-----------------------------------------------------------------------------------" << std::endl;
  std::cout << "21  - L1_SingleMu22 = " << 1000 * rate_SingleMu->GetBinContent(23) << " Hz" << std::endl;
  std::cout << "168 - L1_SingleEG36er2p5 = " << 1000 * rate_SingleEG->GetBinContent(37) << " Hz" << std::endl;
  std::cout << "169 - L1_SingleEG38er2p5 = " << 1000 * rate_SingleEG->GetBinContent(39) << " Hz" << std::endl;
  std::cout << "170 - L1_SingleEG40er2p5 = " << 1000 * rate_SingleEG->GetBinContent(41) << " Hz" << std::endl;
  std::cout << "171 - L1_SingleEG42er2p5 = " << 1000 * rate_SingleEG->GetBinContent(43) << " Hz" << std::endl;
  std::cout << "172 - L1_SingleEG45er2p5 = " << 1000 * rate_SingleEG->GetBinContent(46) << " Hz" << std::endl;
  // std::cout << "178 - L1_SingleLooseIsoEG28er2p5 = " << 1000 * rate_LooseIso_SingleEG->GetBinContent(29) << " Hz" << std::endl;
  std::cout << "192 - L1_SingleIsoEG30er2p5 = " << 1000 * rate_TightIso_SingleEG->GetBinContent(31) << " Hz" << std::endl;
  std::cout << "194 - L1_SingleIsoEG32er2p5 = " << 1000 * rate_TightIso_SingleEG->GetBinContent(33) << " Hz" << std::endl;
  std::cout << "196 - L1_SingleIsoEG34er2p5 = " << 1000 * rate_TightIso_SingleEG->GetBinContent(35) << " Hz" << std::endl;
  std::cout << "218 - L1_DoubleEG_25_12_er2p5 = " << 1000 * pt_DiEG->Integral(26,601,13,601)/Denominator << " Hz" << std::endl;
  std::cout << "219 - L1_DoubleEG_25_14_er2p5 = " << 1000 * pt_DiEG->Integral(26,601,15,601)/Denominator << " Hz" << std::endl;
  std::cout << "220 - L1_DoubleEG_27_14_er2p5 = " << 1000 * pt_DiEG->Integral(28,601,15,601)/Denominator << " Hz" << std::endl;
  // std::cout << " - L1_DoubleEG_LooseIso22_12_er2p5 = " << 1000 * pt_LooseIso_DiEG->Integral(22+1,601,12+1,601)/Denominator << " Hz" << std::endl;
  // std::cout << "262 - L1_SingleIsoTau32er2p1 = " << 1000 * rate_Iso_SingleTau->GetBinContent(33)/800 << " Hz" << std::endl;
  std::cout << "263 - L1_SingleTau70er2p1 = " << 1000 * rate_SingleTau->GetBinContent(71)/50 << " Hz" << std::endl;
  std::cout << "264 - L1_SingleTau120er2p1 = " << 1000 * rate_SingleTau->GetBinContent(121) << " Hz" << std::endl;
  std::cout << "267 - L1_DoubleTau70er2p1 = " << 1000 * rate_DiTau->GetBinContent(71) << " Hz" << std::endl;
  std::cout << "270 - L1_DoubleIsoTau32er2p1 = " << 1000 * rate_Iso_DiTau->GetBinContent(33) << " Hz" << std::endl;
  std::cout << "271 - L1_DoubleIsoTau34er2p1 = " << 1000 * rate_Iso_DiTau->GetBinContent(35) << " Hz" << std::endl;
  std::cout << "272 - L1_DoubleIsoTau35er2p1 = " << 1000 * rate_Iso_DiTau->GetBinContent(36) << " Hz" << std::endl;
  std::cout << "273 - L1_DoubleIsoTau36er2p1 = " << 1000 * rate_Iso_DiTau->GetBinContent(37) << " Hz" << std::endl;
  std::cout << "-----------------------------------------------------------------------------------" << std::endl;



  TString scaledToLumi = "";
  if (doScaleToLumi) { scaledToLumi = "_scaledTo2e34Lumi"; }
  // TFile f("histos/histos_rate_EphemeralZeroBias_2018D_Run323755_allBXbits_unpacked"+scaledToLumi+".root","RECREATE");
  TString FileName_out = "histos/histos_rate_ZeroBias_Run2022"+era+"_unpacked"+scaledToLumi+".root";
  if (ephemeral) { FileName_out = "histos/histos_rate_EphemeralZeroBias0_Run2022"+era+"_unpacked"+scaledToLumi+".root"; }
  TFile f(FileName_out, "RECREATE");

  pt_SingleMu->Write();

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

  rate_SingleMu->Write();

  rate_SingleTau->Write();
  rate_Iso_SingleTau->Write();

  rate_DiTau->Write();
  rate_Iso_DiTau->Write();

  rate_SingleEG->Write();
  rate_LooseIso_SingleEG->Write();
  rate_TightIso_SingleEG->Write();

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

