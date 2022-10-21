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

using namespace std;

void Rate(TString era, bool doScaleToLumi)
{
  TString FileName_in = "/data_CMS/cms/motta/Run3preparation/ZeroBias_Run2022"+era+"__RAW/ZeroBias_Run2022"+era+"__RAW.root";
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

  UInt_t in_bit178 = 0;
  UInt_t in_bit192 = 0;
  UInt_t in_bit218 = 0;
  UInt_t in_bit262 = 0;
  UInt_t in_bit263 = 0;
  UInt_t in_bit270 = 0;
  UInt_t in_bit271 = 0;

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

  inTree->SetBranchAddress("bit178", &in_bit178);
  inTree->SetBranchAddress("bit192", &in_bit192);
  inTree->SetBranchAddress("bit218", &in_bit218);
  inTree->SetBranchAddress("bit262", &in_bit262);
  inTree->SetBranchAddress("bit263", &in_bit263);
  inTree->SetBranchAddress("bit270", &in_bit270);
  inTree->SetBranchAddress("bit271", &in_bit271);

  TH1F* pt_SingleTau = new TH1F("pt_SingleTau","pt_SingleTau",240,0.,240.);
  TH1F* pt_Iso_SingleTau = new TH1F("pt_Iso_SingleTau","pt_Iso_SingleTau",240,0.,240.);

  TH2F* pt_DiTau = new TH2F("pt_DiTau","pt_DiTau",240,0.,240.,240,0.,240.);
  TH2F* pt_Iso_DiTau = new TH2F("pt_Iso_DiTau","pt_Iso_DiTau",240,0.,240.,240,0.,240.);

  TH1F* pt_SingleEG = new TH1F("pt_SingleEG","pt_SingleEG",240,0.,240.);
  TH2F* pt_DiEG = new TH2F("pt_DiEG","pt_DiEG",240,0.,240.,240,0.,240.);

  TH1F* pt_LooseIso_SingleEG = new TH1F("pt_LooseIso_SingleEG","pt_LooseIso_SingleEG",240,0.,240.);
  TH2F* pt_LooseIso_DiEG = new TH2F("pt_LooseIso_DiEG","pt_LooseIso_DiEG",240,0.,240.,240,0.,240.);

  TH1F* pt_TightIso_SingleEG = new TH1F("pt_TightIso_SingleEG","pt_TightIso_SingleEG",240,0.,240.);
  TH2F* pt_TightIso_DiEG = new TH2F("pt_TightIso_DiEG","pt_TightIso_DiEG",240,0.,240.,240,0.,240.);

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
      /*if (in_RunNumber == 357438)
        { 
          if(in_lumi<135 || in_lumi>229) continue;
          nb = 2400;
          thisLumiRun = 1.64E34;
        }*/
      if (in_RunNumber == 357440)
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

          if(in_l1tTauPt->at(highestTauIdx)>=70)
            {
              if(!in_bit263) missedBit263 += 1;
            }
          else
            {
              if(in_bit263) additionalBit263 += 1;
            }
        }
      else
        {
          if(in_bit263) additionalBit263 += 1;
        }

      if(highestTauIsoIdx != -99)
        {
          pt_Iso_SingleTau->Fill(in_l1tTauPt->at(highestTauIsoIdx),scale);

          if(in_l1tTauPt->at(highestTauIsoIdx)>=32)
            {
              if(!in_bit262) missedBit262 += 1;
            }
          else
            {
              if(in_bit262) additionalBit262 += 1;
            }
        }
      else
        {
          if(in_bit262) additionalBit262 += 1;
        }

      //DiTau trigger
      if(Index_Taus.at(0)>=0 && Index_Taus.at(1)>=0)
        {
          pt_DiTau->Fill(pt_Taus.at(0),pt_Taus.at(1),scale);
        }

      if(Index_Taus_Iso.at(0)>=0 && Index_Taus_Iso.at(1)>=0)
        {
          pt_Iso_DiTau->Fill(pt_Taus_Iso.at(0),pt_Taus_Iso.at(1),scale);

          if(pt_Taus_Iso.at(0)>=32 && pt_Taus_Iso.at(1)>=32)
            {
              if(!in_bit270) missedBit270 += 1;
            }
          else
            {
              if(in_bit270) additionalBit270 += 1;
            }

          if(pt_Taus_Iso.at(0)>=34 && pt_Taus_Iso.at(1)>=34)
            {
              if(!in_bit271) missedBit271 += 1;
            }
          else
            {
              if(in_bit271) additionalBit271 += 1;
            }
        }
      else
        {
          if(in_bit270) additionalBit270 += 1;
          if(in_bit271) additionalBit271 += 1;
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
        }

      if(highestLooseEGIdx != -99)
        {
          pt_LooseIso_SingleEG->Fill(in_l1tEGPt->at(highestLooseEGIdx),scale);
          
          if(in_l1tEGPt->at(highestLooseEGIdx)>=28)
            {
              if(!in_bit178) missedBit178 += 1;
            }
          else
            {
              if(in_bit178) additionalBit178 += 1;
            }
        }
      else
        {
          if(in_bit178) additionalBit178 += 1;
        }

      if(highestTightEGIdx != -99)
        {
          pt_TightIso_SingleEG->Fill(in_l1tEGPt->at(highestTightEGIdx),scale);

          if(in_l1tEGPt->at(highestTightEGIdx)>=30)
            {
              if(!in_bit192) missedBit192 += 1;
            }
          else
            {
              if(in_bit192) additionalBit192 += 1;
            }
        }
      else
        {
          if(in_bit192) additionalBit192 += 1;
        }

      //DiEG trigger
      if(Index_EGs.at(0)>=0 && Index_EGs.at(1)>=0)
        {
          pt_DiEG->Fill(pt_EGs.at(0),pt_EGs.at(1),scale);

          if(pt_EGs.at(0)>=25 && pt_EGs.at(1)>=12)
            {
              if(!in_bit218) missedBit218 += 1;
            }
          else
            {
              if(in_bit218) additionalBit218 += 1;
            }
        }
      else
        {
          if(in_bit218) additionalBit218 += 1;
        }

      if(Index_EGs_LooseIso.at(0)>=0 && Index_EGs_LooseIso.at(1)>=0)
        {
          pt_LooseIso_DiEG->Fill(pt_EGs_LooseIso.at(0),pt_EGs_LooseIso.at(1),scale);
        }

      if(Index_EGs_TightIso.at(0)>=0 && Index_EGs_TightIso.at(1)>=0)
        {
          pt_TightIso_DiEG->Fill(pt_EGs_TightIso.at(0),pt_EGs_TightIso.at(1),scale);
        }

    } // end of for loop on events

  cout << "Denominator = " << Denominator << endl;

  TH1F* rate_SingleTau = new TH1F("rate_SingleTau","rate_SingleTau",240,0.,240.);
  TH1F* rate_Iso_SingleTau = new TH1F("rate_Iso_SingleTau","rate_Iso_SingleTau",240,0.,240.);

  TH1F* rate_DiTau = new TH1F("rate_DiTau","rate_DiTau",240,0.,240.);
  TH1F* rate_Iso_DiTau = new TH1F("rate_Iso_DiTau","rate_Iso_DiTau",240,0.,240.);

  TH1F* rate_LooseIso_SingleEG = new TH1F("rate_LooseIso_SingleEG","rate_LooseIso_SingleEG",240,0.,240.);
  TH1F* rate_TightIso_SingleEG = new TH1F("rate_TightIso_SingleEG","rate_TightIso_SingleEG",240,0.,240.);

  for(UInt_t i = 0 ; i < 241 ; ++i)
    {
      rate_DiTau->SetBinContent(i+1,pt_DiTau->Integral(i+1,241,i+1,241)/Denominator);
      rate_Iso_DiTau->SetBinContent(i+1,pt_Iso_DiTau->Integral(i+1,241,i+1,241)/Denominator);

      rate_SingleTau->SetBinContent(i+1,pt_SingleTau->Integral(i+1,241)/Denominator);
      rate_Iso_SingleTau->SetBinContent(i+1,pt_Iso_SingleTau->Integral(i+1,241)/Denominator);

      rate_LooseIso_SingleEG->SetBinContent(i+1,pt_LooseIso_SingleEG->Integral(i+1,241)/Denominator);
      rate_TightIso_SingleEG->SetBinContent(i+1,pt_TightIso_SingleEG->Integral(i+1,241)/Denominator);
    }


  std::cout << "-----------------------------------------------------------------------------------" << std::endl;
  std::cout << "scale = " << scale << " ; " << "denominator = " << Denominator << std::endl;
  std::cout << "-----------------------------------------------------------------------------------" << std::endl;
  std::cout << "178 - L1_SingleLooseIsoEG28er2p5 = " << 1000 * rate_LooseIso_SingleEG->GetBinContent(29) << " Hz" << std::endl;
  std::cout << "192 - L1_SingleIsoEG30er2p5 = " << 1000 * rate_TightIso_SingleEG->GetBinContent(31) << " Hz" << std::endl;
  std::cout << "218 - L1_DoubleEG_25_12_er2p5 = " << 1000 * pt_DiEG->Integral(25+1,241,12+1,241)/Denominator << " Hz" << std::endl;
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


  TString scaledToLumi = "";
  if (doScaleToLumi) scaledToLumi = "_scaledTo2e34Lumi";
  TFile f("histos/histos_rate_ZeroBias_Run2022"+era+"_unpacked"+scaledToLumi+".root","RECREATE");

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

  return;
}

