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

  TString inTree_name = "l1UpgradeTree/L1UpgradeTree";
  TChain inTree(inTree_name.Data());
  inTree.Add(FileName_in.Data());
  inTree.SetMakeClass(1);

  TString inTree1_name = "l1EventTree/L1EventTree";
  TChain inTree1(inTree1_name.Data());
  inTree1.Add(FileName_in.Data());
  inTree1.SetMakeClass(1);

  TBranch *b_Event_run;
  TBranch *b_Event_event;
  TBranch *b_Event_lumi;
  TBranch *b_L1Upgrade_tauEt;
  TBranch *b_L1Upgrade_tauEta;
  TBranch *b_L1Upgrade_tauIso;
  TBranch *b_L1Upgrade_tauBx;
  TBranch *b_L1Upgrade_egEt;
  TBranch *b_L1Upgrade_egEta;
  TBranch *b_L1Upgrade_egIso;
  TBranch *b_L1Upgrade_egBx;

  ULong64_t         in_EventNumber;
  UInt_t            in_RunNumber;
  UInt_t            in_lumi;
  vector<float>     in_l1tTauPt;
  vector<float>     in_l1tTauEta;
  vector<short int> in_l1tTauIso;
  vector<short int> in_l1tTauBx;
  vector<float>     in_l1tEGPt;
  vector<float>     in_l1tEGEta;
  vector<short int> in_l1tEGIso;
  vector<short int> in_l1tEGBx;
  // vector<float>   *in_l1tMuPt =  0;
  // vector<float>   *in_l1tMuEta =  0;
  // vector<int>     *in_l1tMuQual = 0;

  inTree1.SetBranchAddress("event", &in_EventNumber, &b_Event_run);
  inTree1.SetBranchAddress("run", &in_RunNumber, &b_Event_event);
  inTree1.SetBranchAddress("lumi", &in_lumi, &b_Event_lumi);
  inTree.SetBranchAddress("tauEt", &in_l1tTauPt, &b_L1Upgrade_tauEt);
  inTree.SetBranchAddress("tauEta", &in_l1tTauEta, &b_L1Upgrade_tauEta);
  inTree.SetBranchAddress("tauIso", &in_l1tTauIso, &b_L1Upgrade_tauIso);
  inTree.SetBranchAddress("tauBx", &in_l1tTauBx, &b_L1Upgrade_tauBx);
  inTree.SetBranchAddress("egEt", &in_l1tEGPt, &b_L1Upgrade_egEt);
  inTree.SetBranchAddress("egEta", &in_l1tEGEta, &b_L1Upgrade_egEta);
  inTree.SetBranchAddress("egIso", &in_l1tEGIso, &b_L1Upgrade_egIso);
  inTree.SetBranchAddress("egBx", &in_l1tEGBx, &b_L1Upgrade_egBx);
  // inTree->SetBranchAddress("l1tMuPt",&in_l1tMuPt);
  // inTree->SetBranchAddress("l1tMuEta",&in_l1tMuEta);
  // inTree->SetBranchAddress("l1tMuQual",&in_l1tMuQual);

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

  Int_t Denominator = 0;

  float thisLumiRun = 0.;
  float nb = 0.;

  cout<<"begin loop"<<endl;
  cout<<inTree.GetEntries()<<endl;
 
  for(UInt_t i = 0 ; i < inTree.GetEntries() ; ++i)
    {
      inTree1.GetEntry(i);
      inTree.GetEntry(i);

      if(i%20000==0) cout<<"Entry #"<<i<<endl; 
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
      else
        {
          continue;
        }
      if (thisLumiRun == 0. || nb == 0)
        {
          std::cout << "ERROR: something went wrong with the run selection and the lumi/nb initialization" << std::endl;
          return;
        }

      float scaleToLumi = 2.00E34;
      float scale = 0.001 * nb * 11245.6;
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
      for(UInt_t iL1Tau = 0 ; iL1Tau < in_l1tTauPt.size() ; ++iL1Tau)
        {
          if(in_l1tTauBx.at(iL1Tau)!=0) continue;
          if(fabs(in_l1tTauEta.at(iL1Tau))>2.1315) continue;

          //SingleTau trigger
          if(in_l1tTauPt.at(iL1Tau) >= highestTauPt)
            {
              highestTauPt = in_l1tTauPt.at(iL1Tau);
              highestTauIdx = iL1Tau;
            }

          if(in_l1tTauPt.at(iL1Tau) >= highestTauIsoPt && in_l1tTauIso.at(iL1Tau)>0)
            {
              highestTauIsoPt = in_l1tTauPt.at(iL1Tau);
              highestTauIsoIdx = iL1Tau;
            }

          //DiTau trigger
          if(in_l1tTauPt.at(iL1Tau)>=pt_Taus.at(0))
            {
              Index_Taus.at(1)=Index_Taus.at(0);
              pt_Taus.at(1)=pt_Taus.at(0);
              Index_Taus.at(0)=iL1Tau;
              pt_Taus.at(0)=in_l1tTauPt.at(iL1Tau);
            }
          else if(in_l1tTauPt.at(iL1Tau)>=pt_Taus.at(1))
            {
              Index_Taus.at(1)=iL1Tau;
              pt_Taus.at(1)=in_l1tTauPt.at(iL1Tau);
            }

          if(in_l1tTauPt.at(iL1Tau)>=pt_Taus_Iso.at(0) && in_l1tTauIso.at(iL1Tau)>0)
            {
              Index_Taus_Iso.at(1)=Index_Taus_Iso.at(0);
              pt_Taus_Iso.at(1)=pt_Taus_Iso.at(0);
              Index_Taus_Iso.at(0)=iL1Tau;
              pt_Taus_Iso.at(0)=in_l1tTauPt.at(iL1Tau);
            }
          else if(in_l1tTauPt.at(iL1Tau)>=pt_Taus_Iso.at(1) && in_l1tTauIso.at(iL1Tau)>0)
            {
              Index_Taus_Iso.at(1)=iL1Tau;
              pt_Taus_Iso.at(1)=in_l1tTauPt.at(iL1Tau);
            }

        }

      //SingleTau trigger
      if(highestTauIdx != -99)
        {
          pt_SingleTau->Fill(in_l1tTauPt.at(highestTauIdx),scale);
        }

      if(highestTauIsoIdx != -99)
        {
          pt_Iso_SingleTau->Fill(in_l1tTauPt.at(highestTauIsoIdx),scale);
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
      for(UInt_t iL1EG = 0 ; iL1EG < in_l1tEGPt.size() ; ++iL1EG)
        {
          if(in_l1tEGBx.at(iL1EG)!=0) continue;
          // if(fabs(in_l1tEGEta.at(iL1EG))>2.1315) continue;
          if(fabs(in_l1tEGEta.at(iL1EG))>2.5) continue;

          //SingleEG trigger
          if(in_l1tEGPt.at(iL1EG) >= highestEGPt)
            {
              highestEGPt = in_l1tEGPt.at(iL1EG);
              highestEGIdx = iL1EG;
            }

          if(in_l1tEGPt.at(iL1EG) >= highestLooseEGPt && (in_l1tEGIso.at(iL1EG)==2 || in_l1tEGIso.at(iL1EG)==3))
            {
              highestLooseEGPt = in_l1tEGPt.at(iL1EG);
              highestLooseEGIdx = iL1EG;
            }

          if(in_l1tEGPt.at(iL1EG) >= highestTightEGPt && (in_l1tEGIso.at(iL1EG)==1 || in_l1tEGIso.at(iL1EG)==3))
            {
              highestTightEGPt = in_l1tEGPt.at(iL1EG);
              highestTightEGIdx = iL1EG;
            }

          //DiEG trigger
          if(in_l1tEGPt.at(iL1EG)>=pt_EGs.at(0))
            {
              Index_EGs.at(1)=Index_EGs.at(0);
              pt_EGs.at(1)=pt_EGs.at(0);
              Index_EGs.at(0)=iL1EG;
              pt_EGs.at(0)=in_l1tEGPt.at(iL1EG);
            }
          else if(in_l1tEGPt.at(iL1EG)>=pt_EGs.at(1))
            {
              Index_EGs.at(1)=iL1EG;
              pt_EGs.at(1)=in_l1tEGPt.at(iL1EG);
            }
          
          if(in_l1tEGPt.at(iL1EG)>=pt_EGs_LooseIso.at(0) && (in_l1tEGIso.at(iL1EG)==2 || in_l1tEGIso.at(iL1EG)==3))
            {
              Index_EGs_LooseIso.at(1)=Index_EGs_LooseIso.at(0);
              pt_EGs_LooseIso.at(1)=pt_EGs_LooseIso.at(0);
              Index_EGs_LooseIso.at(0)=iL1EG;
              pt_EGs_LooseIso.at(0)=in_l1tEGPt.at(iL1EG);
            }
          else if(in_l1tEGPt.at(iL1EG)>=pt_EGs_LooseIso.at(1) && (in_l1tEGIso.at(iL1EG)==2 || in_l1tEGIso.at(iL1EG)==3))
            {
              Index_EGs_LooseIso.at(1)=iL1EG;
              pt_EGs_LooseIso.at(1)=in_l1tEGPt.at(iL1EG);
            }

          if(in_l1tEGPt.at(iL1EG)>=pt_EGs_TightIso.at(0) && (in_l1tEGIso.at(iL1EG)==1 || in_l1tEGIso.at(iL1EG)==3))
            {
              Index_EGs_TightIso.at(1)=Index_EGs_TightIso.at(0);
              pt_EGs_TightIso.at(1)=pt_EGs_TightIso.at(0);
              Index_EGs_TightIso.at(0)=iL1EG;
              pt_EGs_TightIso.at(0)=in_l1tEGPt.at(iL1EG);
            }
          else if(in_l1tEGPt.at(iL1EG)>=pt_EGs_TightIso.at(1) && (in_l1tEGIso.at(iL1EG)==1 || in_l1tEGIso.at(iL1EG)==3))
            {
              Index_EGs_TightIso.at(1)=iL1EG;
              pt_EGs_TightIso.at(1)=in_l1tEGPt.at(iL1EG);
            }

        }

      //SingleEG trigger
      if(highestEGIdx != -99)
        {
          pt_SingleEG->Fill(in_l1tEGPt.at(highestEGIdx),scale);
        }

      if(highestLooseEGIdx != -99)
        {
          pt_LooseIso_SingleEG->Fill(in_l1tEGPt.at(highestLooseEGIdx),scale);
        }

      if(highestTightEGIdx != -99)
        {
          pt_TightIso_SingleEG->Fill(in_l1tEGPt.at(highestTightEGIdx),scale);
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

  std::cout << "178 - L1_SingleLooseIsoEG28er2p5 = " << 1000 * rate_LooseIso_SingleEG->GetBinContent(29) << " Hz" << std::endl;
  std::cout << "192 - L1_SingleIsoEG30er2p5 = " << 1000 * rate_TightIso_SingleEG->GetBinContent(31) << " Hz" << std::endl;
  std::cout << "218 - L1_DoubleEG_25_12_er2p5 = " << 1000 * pt_DiEG->Integral(25+1,241,12+1,241)/Denominator << " Hz" << std::endl;
  // std::cout << " - L1_DoubleEG_LooseIso22_12_er2p5 = " << 1000 * pt_LooseIso_DiEG->Integral(22+1,241,12+1,241)/Denominator << " Hz" << std::endl;

  std::cout << "262 - L1_SingleIsoTau32er2p1 = " << 1000 * rate_Iso_SingleTau->GetBinContent(33)/800 << " Hz" << std::endl;
  std::cout << "263 - L1_SingleTau70er2p1 = " << 1000 * rate_SingleTau->GetBinContent(71)/50 << " Hz" << std::endl;
  std::cout << "270 - L1_DoubleIsoTau32er2p1 = " << 1000 * rate_Iso_DiTau->GetBinContent(33) << " Hz" << std::endl;
  std::cout << "271 - L1_DoubleIsoTau34er2p1 = " << 1000 * rate_Iso_DiTau->GetBinContent(35) << " Hz" << std::endl;
  

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

