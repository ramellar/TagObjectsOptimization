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

void check(TString era, bool doScaleToLumi)
{
  // TString FileName_in = "/data_CMS/cms/motta/Run3preparation/EphemeralZeroBias_2018D_Run323755__RAW/EphemeralZeroBias_2018D_Run323755__RAW.root";
  TString FileName_in = "/data_CMS/cms/motta/Run3preparation/ZeroBias_Run2022"+era+"__RAW/ZeroBias_Run2022"+era+"__RAW.root";
  TFile f_in(FileName_in.Data(),"READ");
  TTree* inTree = (TTree*)f_in.Get("ZeroBias/ZeroBias");

  ULong64_t       in_EventNumber =  0;
  Int_t           in_RunNumber =  0;
  Int_t           in_lumi =  0;
  std::vector< std::vector<float> >   *in_l1tTauPt = 0;
  std::vector< std::vector<float> >   *in_l1tTauEta = 0;
  std::vector< std::vector<float> >   *in_l1tTauPhi = 0;
  std::vector< std::vector<int> >     *in_l1tTauIso = 0;
  std::vector< std::vector<float> >   *in_l1tEGPt =  0;
  std::vector< std::vector<float> >   *in_l1tEGEta =  0;
  std::vector< std::vector<float> >   *in_l1tEGPhi =  0;
  std::vector< std::vector<int> >     *in_l1tEGIso = 0;
  vector< vector<float> >   *in_l1tMuPt =  0;
  vector< vector<float> >   *in_l1tMuEta =  0;
  vector< vector<float> >   *in_l1tMuPhi =  0;
  vector< vector<int> >     *in_l1tMuQual = 0;

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
  inTree->SetBranchAddress("l1tTauPhi", &in_l1tTauPhi);
  inTree->SetBranchAddress("l1tTauIso", &in_l1tTauIso);
  inTree->SetBranchAddress("l1tEGPt", &in_l1tEGPt);
  inTree->SetBranchAddress("l1tEGEta", &in_l1tEGEta);
  inTree->SetBranchAddress("l1tEGPhi", &in_l1tEGPhi);
  inTree->SetBranchAddress("l1tEGIso", &in_l1tEGIso);
  inTree->SetBranchAddress("l1tMuPt",&in_l1tMuPt);
  inTree->SetBranchAddress("l1tMuEta",&in_l1tMuEta);
  inTree->SetBranchAddress("l1tMuPhi",&in_l1tMuPhi);
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

  float thisLumiRun = 0.;
  float nb = 0.;

  float scaleToLumi = 2.00E34;
  float scale = 0.;

  Int_t Denominator = 0;

  std::vector<int> uGTmissedBit21;  std::vector<int> uGTadditionalBit21;
  std::vector<int> uGTmissedBit168; std::vector<int> uGTadditionalBit168;
  std::vector<int> uGTmissedBit169; std::vector<int> uGTadditionalBit169;
  std::vector<int> uGTmissedBit170; std::vector<int> uGTadditionalBit170;
  std::vector<int> uGTmissedBit171; std::vector<int> uGTadditionalBit171;
  std::vector<int> uGTmissedBit172; std::vector<int> uGTadditionalBit172;
  std::vector<int> uGTmissedBit192; std::vector<int> uGTadditionalBit192;
  std::vector<int> uGTmissedBit194; std::vector<int> uGTadditionalBit194;
  std::vector<int> uGTmissedBit196; std::vector<int> uGTadditionalBit196;
  std::vector<int> uGTmissedBit218; std::vector<int> uGTadditionalBit218;
  std::vector<int> uGTmissedBit219; std::vector<int> uGTadditionalBit219;
  std::vector<int> uGTmissedBit220; std::vector<int> uGTadditionalBit220;
  std::vector<int> uGTmissedBit263; std::vector<int> uGTadditionalBit263;
  std::vector<int> uGTmissedBit264; std::vector<int> uGTadditionalBit264;
  std::vector<int> uGTmissedBit267; std::vector<int> uGTadditionalBit267;
  std::vector<int> uGTmissedBit271; std::vector<int> uGTadditionalBit271;
  std::vector<int> uGTmissedBit272; std::vector<int> uGTadditionalBit272;
  std::vector<int> uGTmissedBit273; std::vector<int> uGTadditionalBit273;
  for(int ibx = 0; ibx < 5; ++ibx)
    { 
      uGTmissedBit21.push_back(0);  uGTadditionalBit21.push_back(0);
      uGTmissedBit168.push_back(0); uGTadditionalBit168.push_back(0);
      uGTmissedBit169.push_back(0); uGTadditionalBit169.push_back(0);
      uGTmissedBit170.push_back(0); uGTadditionalBit170.push_back(0);
      uGTmissedBit171.push_back(0); uGTadditionalBit171.push_back(0);
      uGTmissedBit172.push_back(0); uGTadditionalBit172.push_back(0);
      uGTmissedBit192.push_back(0); uGTadditionalBit192.push_back(0);
      uGTmissedBit194.push_back(0); uGTadditionalBit194.push_back(0);
      uGTmissedBit196.push_back(0); uGTadditionalBit196.push_back(0);
      uGTmissedBit218.push_back(0); uGTadditionalBit218.push_back(0);
      uGTmissedBit219.push_back(0); uGTadditionalBit219.push_back(0);
      uGTmissedBit220.push_back(0); uGTadditionalBit220.push_back(0);
      uGTmissedBit263.push_back(0); uGTadditionalBit263.push_back(0);
      uGTmissedBit264.push_back(0); uGTadditionalBit264.push_back(0);
      uGTmissedBit267.push_back(0); uGTadditionalBit267.push_back(0);
      uGTmissedBit271.push_back(0); uGTadditionalBit271.push_back(0);
      uGTmissedBit272.push_back(0); uGTadditionalBit272.push_back(0);
      uGTmissedBit273.push_back(0); uGTadditionalBit273.push_back(0);
    }

  // histograms for missed/additional events
  TH1F* uGTmissedBit263_BXm2_pt =  new TH1F("uGTmissedBit263_BXm2_pt",  "uGTmissedBit263_BXm2_pt", 600, 0, 600.);
  TH1F* uGTmissedBit263_BXm2_eta = new TH1F("uGTmissedBit263_BXm2_eta", "uGTmissedBit263_BXm2_eta", 50, -2.5, 2.5);
  TH1F* uGTmissedBit263_BXm2_phi = new TH1F("uGTmissedBit263_BXm2_phi", "uGTmissedBit263_BXm2_phi", 64, -3.2, 3.2);
  TH1F* uGTadditionalBit263_BXm2_pt =  new TH1F("uGTadditionalBit263_BXm2_pt",  "uGTadditionalBit263_BXm2_pt", 600, 0, 600.);
  TH1F* uGTadditionalBit263_BXm2_eta = new TH1F("uGTadditionalBit263_BXm2_eta", "uGTadditionalBit263_BXm2_eta", 50, -2.5, 2.5);
  TH1F* uGTadditionalBit263_BXm2_phi = new TH1F("uGTadditionalBit263_BXm2_phi", "uGTadditionalBit263_BXm2_phi", 64, -3.2, 3.2);

  TH1F* uGTmissedBit263_BXm1_pt =  new TH1F("uGTmissedBit263_BXm1_pt",  "uGTmissedBit263_BXm1_pt", 600, 0, 600.);
  TH1F* uGTmissedBit263_BXm1_eta = new TH1F("uGTmissedBit263_BXm1_eta", "uGTmissedBit263_BXm1_eta", 50, -2.5, 2.5);
  TH1F* uGTmissedBit263_BXm1_phi = new TH1F("uGTmissedBit263_BXm1_phi", "uGTmissedBit263_BXm1_phi", 64, -3.2, 3.2);
  TH1F* uGTadditionalBit263_BXm1_pt =  new TH1F("uGTadditionalBit263_BXm1_pt",  "uGTadditionalBit263_BXm1_pt", 600, 0, 600.);
  TH1F* uGTadditionalBit263_BXm1_eta = new TH1F("uGTadditionalBit263_BXm1_eta", "uGTadditionalBit263_BXm1_eta", 50, -2.5, 2.5);
  TH1F* uGTadditionalBit263_BXm1_phi = new TH1F("uGTadditionalBit263_BXm1_phi", "uGTadditionalBit263_BXm1_phi", 64, -3.2, 3.2);

  TH1F* uGTmissedBit263_BX0_pt =  new TH1F("uGTmissedBit263_BX0_pt",  "uGTmissedBit263_BX0_pt", 600, 0, 600.);
  TH1F* uGTmissedBit263_BX0_eta = new TH1F("uGTmissedBit263_BX0_eta", "uGTmissedBit263_BX0_eta", 50, -2.5, 2.5);
  TH1F* uGTmissedBit263_BX0_phi = new TH1F("uGTmissedBit263_BX0_phi", "uGTmissedBit263_BX0_phi", 64, -3.2, 3.2);
  TH1F* uGTadditionalBit263_BX0_pt =  new TH1F("uGTadditionalBit263_BX0_pt",  "uGTadditionalBit263_BX0_pt", 600, 0, 600.);
  TH1F* uGTadditionalBit263_BX0_eta = new TH1F("uGTadditionalBit263_BX0_eta", "uGTadditionalBit263_BX0_eta", 50, -2.5, 2.5);
  TH1F* uGTadditionalBit263_BX0_phi = new TH1F("uGTadditionalBit263_BX0_phi", "uGTadditionalBit263_BX0_phi", 64, -3.2, 3.2);
  
  TH1F* uGTmissedBit263_BXp1_pt =  new TH1F("uGTmissedBit263_BXp1_pt",  "uGTmissedBit263_BXp1_pt", 600, 0, 600.);
  TH1F* uGTmissedBit263_BXp1_eta = new TH1F("uGTmissedBit263_BXp1_eta", "uGTmissedBit263_BXp1_eta", 50, -2.5, 2.5);
  TH1F* uGTmissedBit263_BXp1_phi = new TH1F("uGTmissedBit263_BXp1_phi", "uGTmissedBit263_BXp1_phi", 64, -3.2, 3.2);
  TH1F* uGTadditionalBit263_BXp1_pt =  new TH1F("uGTadditionalBit263_BXp1_pt",  "uGTadditionalBit263_BXp1_pt", 600, 0, 600.);
  TH1F* uGTadditionalBit263_BXp1_eta = new TH1F("uGTadditionalBit263_BXp1_eta", "uGTadditionalBit263_BXp1_eta", 50, -2.5, 2.5);
  TH1F* uGTadditionalBit263_BXp1_phi = new TH1F("uGTadditionalBit263_BXp1_phi", "uGTadditionalBit263_BXp1_phi", 64, -3.2, 3.2);
  
  TH1F* uGTmissedBit263_BXp2_pt =  new TH1F("uGTmissedBit263_BXp2_pt",  "uGTmissedBit263_BXp2_pt", 600, 0, 600.);
  TH1F* uGTmissedBit263_BXp2_eta = new TH1F("uGTmissedBit263_BXp2_eta", "uGTmissedBit263_BXp2_eta", 50, -2.5, 2.5);
  TH1F* uGTmissedBit263_BXp2_phi = new TH1F("uGTmissedBit263_BXp2_phi", "uGTmissedBit263_BXp2_phi", 64, -3.2, 3.2);
  TH1F* uGTadditionalBit263_BXp2_pt =  new TH1F("uGTadditionalBit263_BXp2_pt",  "uGTadditionalBit263_BXp2_pt", 600, 0, 600.);
  TH1F* uGTadditionalBit263_BXp2_eta = new TH1F("uGTadditionalBit263_BXp2_eta", "uGTadditionalBit263_BXp2_eta", 50, -2.5, 2.5);
  TH1F* uGTadditionalBit263_BXp2_phi = new TH1F("uGTadditionalBit263_BXp2_phi", "uGTadditionalBit263_BXp2_phi", 64, -3.2, 3.2);

  // LOOP OVER EVENTS
  std::cout << "begin loop"<< std::endl;
  std::cout << inTree->GetEntries()<< std::endl;
  for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    { 
      inTree->GetEntry(i);
      if(i%100000==0) cout<<"Entry #"<<i<<endl; 

      if (in_RunNumber == 360927)
        { 
          if(in_lumi<99 || in_lumi>719) continue;
          nb = 2450;
          thisLumiRun = 1.85E34;
        }

      if (thisLumiRun == 0. || nb == 0)
        {
          std::cout << "ERROR: something went wrong with the run selection and the lumi/nb initialization" << std::endl;
          return;
        }

      scale = 0.001 * nb * 11245.6;
      if (doScaleToLumi) scale *= scaleToLumi / thisLumiRun;

      ++Denominator;

      // LOOP OVER BXs
      for(int ibx = 0; ibx < 5; ++ibx)
        {
          int BX = ibx - 2;

          std::vector<float> l1tTauPt  = in_l1tTauPt->at(ibx);
          std::vector<float> l1tTauEta = in_l1tTauEta->at(ibx);
          std::vector<float> l1tTauPhi = in_l1tTauPhi->at(ibx);
          std::vector<int>   l1tTauIso = in_l1tTauIso->at(ibx);
          std::vector<float> l1tEGPt   = in_l1tEGPt->at(ibx);
          std::vector<float> l1tEGEta  = in_l1tEGEta->at(ibx);
          std::vector<float> l1tEGPhi  = in_l1tEGPhi->at(ibx);
          std::vector<int>   l1tEGIso  = in_l1tEGIso->at(ibx);
          vector<float> l1tMuPt   = in_l1tMuPt->at(ibx);
          vector<float> l1tMuEta  = in_l1tMuEta->at(ibx);
          vector<float> l1tMuPhi  = in_l1tMuPhi->at(ibx);
          vector<int>   l1tMuQual = in_l1tMuQual->at(ibx);

          int bit21  =  in_bit21->at(ibx);  // L1_SingleMu22
          int bit168 =  in_bit168->at(ibx); // L1_SingleEG36er2p5
          int bit169 =  in_bit169->at(ibx); // L1_SingleEG38er2p5
          int bit170 =  in_bit170->at(ibx); // L1_SingleEG40er2p5
          int bit171 =  in_bit171->at(ibx); // L1_SingleEG42er2p5
          int bit172 =  in_bit172->at(ibx); // L1_SingleEG45er2p5
          int bit178 =  in_bit178->at(ibx); // L1_SingleLooseIsoEG28er2p5
          int bit192 =  in_bit192->at(ibx); // L1_SingleIsoEG30er2p5
          int bit194 =  in_bit194->at(ibx); // L1_SingleIsoEG32er2p5
          int bit196 =  in_bit196->at(ibx); // L1_SingleIsoEG34er2p5
          int bit218 =  in_bit218->at(ibx); // L1_DoubleEG_25_12_er2p5
          int bit219 =  in_bit219->at(ibx); // L1_DoubleEG_25_14_er2p5
          int bit220 =  in_bit220->at(ibx); // L1_DoubleEG_27_14_er2p5
          int bit262 =  in_bit262->at(ibx); // L1_SingleIsoTau32er2p1
          int bit263 =  in_bit263->at(ibx); // L1_SingleTau70er2p1
          int bit264 =  in_bit264->at(ibx); // L1_SingleTau120er2p1
          int bit267 =  in_bit267->at(ibx); // L1_DoubleTau70er2p1
          int bit270 =  in_bit270->at(ibx); // L1_DoubleIsoTau32er2p1
          int bit271 =  in_bit271->at(ibx); // L1_DoubleIsoTau34er2p1
          int bit272 =  in_bit272->at(ibx); // L1_DoubleIsoTau35er2p1
          int bit273 =  in_bit273->at(ibx); // L1_DoubleIsoTau36er2p1
          int bit459 =  in_bit459->at(ibx); // L1_ZeroBias


          // --------------------------------------------------------------------
          // --------------------------------------------------------------------
          // MU TRIGGERS

          //SingleMu trigger
          float highestMuPt = -99.9;
          int highestMuIdx = -99;
          for(UInt_t iL1Mu = 0 ; iL1Mu < l1tMuPt.size() ; ++iL1Mu)
            {
              if (l1tMuQual.at(iL1Mu) < 12) continue;

              //SingleMu trigger
              if(l1tMuPt.at(iL1Mu) >= highestMuPt)
                {
                  highestMuPt = l1tMuPt.at(iL1Mu);
                  highestMuIdx = iL1Mu;
                }
            }

          if(bit21==0)
            {
              if(highestMuPt>22)
                {
                  ++uGTmissedBit21.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 21 L1_SingleMu22 missed : pt " << highestMuPt << " - eta " << l1tMuEta.at(highestMuIdx) << " - phi " << l1tMuPhi.at(highestMuIdx) << std::endl;
                }
            }
          else
            {
              if(highestMuPt<22)
                {
                  ++uGTadditionalBit21.at(ibx);
                  if (highestMuIdx != -99) std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 21 L1_SingleMu22 additional : pt " << highestMuPt << " - eta " << l1tMuEta.at(highestMuIdx) << " - phi " << l1tMuPhi.at(highestMuIdx) << std::endl;
                  else                     std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 21 L1_SingleMu22 additional : no candidate" << std::endl;
                }
            }


          // --------------------------------------------------------------------
          // --------------------------------------------------------------------
          // TAU TRIGGERS

          std::vector<Float_t> pt_Taus;
          pt_Taus.push_back(-99.); pt_Taus.push_back(-99.);
          std::vector<Int_t> idx_Taus;
          idx_Taus.push_back(-99.); idx_Taus.push_back(-99.);
          std::vector<Float_t> pt_Taus_Iso;
          pt_Taus_Iso.push_back(-99.); pt_Taus_Iso.push_back(-99.);
          std::vector<Int_t> idx_Taus_Iso;
          idx_Taus_Iso.push_back(-99.); idx_Taus_Iso.push_back(-99.);

          float highestTauPt = -99.9;
          int highestTauIdx = -99;

          for(UInt_t iL1Tau = 0 ; iL1Tau < l1tTauPt.size() ; ++iL1Tau)
            {
              if(fabs(l1tTauEta.at(iL1Tau))>2.1315) continue;

              // SingleTau Trigger - bits 263, 264
              if(l1tTauPt.at(iL1Tau) >= highestTauPt)
                {
                  highestTauPt = l1tTauPt.at(iL1Tau);
                  highestTauIdx = iL1Tau;
                }

              // DiTau Trigger - bits 267
              if(l1tTauPt.at(iL1Tau)>=pt_Taus.at(0))
              {
                pt_Taus.at(1)=pt_Taus.at(0);
                pt_Taus.at(0)=l1tTauPt.at(iL1Tau);

                idx_Taus.at(1)=idx_Taus.at(0);
                idx_Taus.at(0)=iL1Tau;
              }
            else if(l1tTauPt.at(iL1Tau)>=pt_Taus.at(1))
              {
                pt_Taus.at(1)=l1tTauPt.at(iL1Tau);
                idx_Taus.at(1)=iL1Tau;
              }

              // DiTau Trigger - bits 271, 272, 273
              if(l1tTauPt.at(iL1Tau)>=pt_Taus_Iso.at(0) && l1tTauIso.at(iL1Tau)>0)
              {
                pt_Taus_Iso.at(1)=pt_Taus_Iso.at(0);
                pt_Taus_Iso.at(0)=l1tTauPt.at(iL1Tau);

                idx_Taus_Iso.at(1)=idx_Taus_Iso.at(0);
                idx_Taus_Iso.at(0)=iL1Tau;
              }
            else if(l1tTauPt.at(iL1Tau)>=pt_Taus_Iso.at(1) && l1tTauIso.at(iL1Tau)>0)
              {
                pt_Taus_Iso.at(1)=l1tTauPt.at(iL1Tau);
                idx_Taus_Iso.at(1)=iL1Tau;
              }
            }

          if(bit263==0)
            { 
              if(highestTauPt>70)
                {
                  ++uGTmissedBit263.at(ibx);
                  // std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 263 L1_SingleTau70er2p1 missed : pt " << highestTauPt << " - eta " << l1tTauEta.at(highestTauIdx) << " - phi " << l1tTauPhi.at(highestTauIdx) << std::endl;
                  if(ibx==0)
                    {
                      uGTmissedBit263_BXm2_pt->Fill(highestTauPt);
                      uGTmissedBit263_BXm2_eta->Fill(l1tTauEta.at(highestTauIdx));
                      uGTmissedBit263_BXm2_phi->Fill(l1tTauPhi.at(highestTauIdx));
                    }
                  if(ibx==1)
                    {
                      uGTmissedBit263_BXm1_pt->Fill(highestTauPt);
                      uGTmissedBit263_BXm1_eta->Fill(l1tTauEta.at(highestTauIdx));
                      uGTmissedBit263_BXm1_phi->Fill(l1tTauPhi.at(highestTauIdx));
                    }
                  if(ibx==2)
                    {
                      uGTmissedBit263_BX0_pt->Fill(highestTauPt);
                      uGTmissedBit263_BX0_eta->Fill(l1tTauEta.at(highestTauIdx));
                      uGTmissedBit263_BX0_phi->Fill(l1tTauPhi.at(highestTauIdx));
                    }
                  if(ibx==3)
                    {
                      uGTmissedBit263_BXp1_pt->Fill(highestTauPt);
                      uGTmissedBit263_BXp1_eta->Fill(l1tTauEta.at(highestTauIdx));
                      uGTmissedBit263_BXp1_phi->Fill(l1tTauPhi.at(highestTauIdx));
                    }
                  if(ibx==4)
                    {
                      uGTmissedBit263_BXp2_pt->Fill(highestTauPt);
                      uGTmissedBit263_BXp2_eta->Fill(l1tTauEta.at(highestTauIdx));
                      uGTmissedBit263_BXp2_phi->Fill(l1tTauPhi.at(highestTauIdx));
                    }
                }
            }
          else
            {
              if(highestTauPt==-99.9)
                {
                  ++uGTadditionalBit263.at(ibx);
                  // std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 263 L1_SingleTau70er2p1 additional : pt " << highestTauPt << " - eta " << l1tTauEta.at(highestTauIdx) << " - phi " << l1tTauPhi.at(highestTauIdx) << std::endl;
                  if(ibx==0)
                    {
                      uGTadditionalBit263_BXm2_pt->Fill(highestTauPt);
                      uGTadditionalBit263_BXm2_eta->Fill(l1tTauEta.at(highestTauIdx));
                      uGTadditionalBit263_BXm2_phi->Fill(l1tTauPhi.at(highestTauIdx));
                    }
                  if(ibx==1)
                    {
                      uGTadditionalBit263_BXm1_pt->Fill(highestTauPt);
                      uGTadditionalBit263_BXm1_eta->Fill(l1tTauEta.at(highestTauIdx));
                      uGTadditionalBit263_BXm1_phi->Fill(l1tTauPhi.at(highestTauIdx));
                    }
                  if(ibx==2)
                    {
                      uGTadditionalBit263_BX0_pt->Fill(highestTauPt);
                      uGTadditionalBit263_BX0_eta->Fill(l1tTauEta.at(highestTauIdx));
                      uGTadditionalBit263_BX0_phi->Fill(l1tTauPhi.at(highestTauIdx));
                    }
                  if(ibx==3)
                    {
                      uGTadditionalBit263_BXp1_pt->Fill(highestTauPt);
                      uGTadditionalBit263_BXp1_eta->Fill(l1tTauEta.at(highestTauIdx));
                      uGTadditionalBit263_BXp1_phi->Fill(l1tTauPhi.at(highestTauIdx));
                    }
                  if(ibx==4)
                    {
                      uGTadditionalBit263_BXp2_pt->Fill(highestTauPt);
                      uGTadditionalBit263_BXp2_eta->Fill(l1tTauEta.at(highestTauIdx));
                      uGTadditionalBit263_BXp2_phi->Fill(l1tTauPhi.at(highestTauIdx));
                    }
                }
            }

          if(bit264==0)
            {
              if(highestTauPt>120)
                {
                  ++uGTmissedBit264.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 264 L1_SingleTau120er2p1 missed : pt " << highestTauPt << " - eta " << l1tTauEta.at(highestTauIdx) << " - phi " << l1tTauPhi.at(highestTauIdx) << std::endl;
                }
            }
          else
            {
              if(highestTauPt==-99.9)
                {
                  ++uGTadditionalBit264.at(ibx);
                  if(highestTauIdx != -99) std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 264 L1_SingleTau120er2p1 additional : pt " << highestTauPt << " - eta " << l1tTauEta.at(highestTauIdx) << " - phi " << l1tTauPhi.at(highestTauIdx) << std::endl;
                  else                     std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 264 L1_SingleTau120er2p1 additional : no candidate" << std::endl;
                }
            }

          if(bit267==0)
            { 
              if(pt_Taus.at(0)>70 && pt_Taus.at(1)>70)
                {
                  ++uGTmissedBit267.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 267 L1_DoubleTau70er2p1 missed : pt1 " << pt_Taus.at(0) << " - eta1 " << l1tTauEta.at(idx_Taus.at(0)) << " - phi1 " << l1tTauPhi.at(idx_Taus.at(0)) << std::endl;
                  std::cout << "                                                                                          : pt2 " << pt_Taus.at(1) << " - eta2 " << l1tTauEta.at(idx_Taus.at(1)) << " - phi2 " << l1tTauPhi.at(idx_Taus.at(1)) << std::endl;
                }
            }
          else
            {
              if(pt_Taus.at(0)<70 || pt_Taus.at(1)<70)
                {
                  ++uGTadditionalBit267.at(ibx);
                  if (idx_Taus.at(0)!=-99 && idx_Taus.at(1)!=-99)
                    {
                      std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 267 L1_DoubleTau70er2p1 additional : pt1 " << pt_Taus.at(0) << " - eta1 " << l1tTauEta.at(idx_Taus.at(0)) << " - phi1 " << l1tTauPhi.at(idx_Taus.at(0)) << std::endl;
                      std::cout << "                                                                                          : pt2 " << pt_Taus.at(1) << " - eta2 " << l1tTauEta.at(idx_Taus.at(1)) << " - phi2 " << l1tTauPhi.at(idx_Taus.at(1)) << std::endl;
                    }
                  else
                    {
                      std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 267 L1_DoubleTau70er2p1 additional : no candidate" << std::endl;
                    }
                }
            }

          if(bit271==0)
            {
              if(pt_Taus_Iso.at(0)>34 && pt_Taus_Iso.at(1)>34)
                {
                  ++uGTmissedBit271.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 271 L1_DoubleIsoTau34er2p1 missed : pt1 " << pt_Taus_Iso.at(0) << " - eta1 " << l1tTauEta.at(idx_Taus_Iso.at(0)) << " - phi1 " << l1tTauPhi.at(idx_Taus_Iso.at(0)) << std::endl;
                  std::cout << "                                                                                          : pt2 " << pt_Taus_Iso.at(1) << " - eta2 " << l1tTauEta.at(idx_Taus_Iso.at(1)) << " - phi2 " << l1tTauPhi.at(idx_Taus_Iso.at(1)) << std::endl;
                }
            }

          else
            {
              if(pt_Taus_Iso.at(0)<34 || pt_Taus_Iso.at(1)<34)
                {
                  ++uGTadditionalBit271.at(ibx);
                  if (idx_Taus.at(0)!=-99 && idx_Taus.at(1)!=-99)
                    {
                      std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 271 L1_DoubleIsoTau34er2p1 additional : pt1 " << pt_Taus_Iso.at(0) << " - eta1 " << l1tTauEta.at(idx_Taus_Iso.at(0)) << " - phi1 " << l1tTauPhi.at(idx_Taus_Iso.at(0)) << std::endl;
                      std::cout << "                                                                                          : pt2 " << pt_Taus_Iso.at(1) << " - eta2 " << l1tTauEta.at(idx_Taus_Iso.at(1)) << " - phi2 " << l1tTauPhi.at(idx_Taus_Iso.at(1)) << std::endl;
                    }
                  else
                    {
                      std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 271 L1_DoubleIsoTau34er2p1 additional : no candidate" << std::endl;
                    }
                }
            }

          if(bit272==0)
            {
              if(pt_Taus_Iso.at(0)>35 && pt_Taus_Iso.at(1)>35)
                {
                  ++uGTmissedBit272.at(ibx); 
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 272 L1_DoubleIsoTau35er2p1 missed : pt1 " << pt_Taus_Iso.at(0) << " - eta1 " << l1tTauEta.at(idx_Taus_Iso.at(0)) << " - phi1 " << l1tTauPhi.at(idx_Taus_Iso.at(0)) << std::endl;
                  std::cout << "                                                                                          : pt2 " << pt_Taus_Iso.at(1) << " - eta2 " << l1tTauEta.at(idx_Taus_Iso.at(1)) << " - phi2 " << l1tTauPhi.at(idx_Taus_Iso.at(1)) << std::endl;
                }
            }
          else
            {
              if(pt_Taus_Iso.at(0)<35 || pt_Taus_Iso.at(1)<35)
                {
                  ++uGTadditionalBit272.at(ibx);
                  if (idx_Taus.at(0)!=-99 && idx_Taus.at(1)!=-99)
                    {
                      std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 272 L1_DoubleIsoTau35er2p1 additional : pt1 " << pt_Taus_Iso.at(0) << " - eta1 " << l1tTauEta.at(idx_Taus_Iso.at(0)) << " - phi1 " << l1tTauPhi.at(idx_Taus_Iso.at(0)) << std::endl;
                      std::cout << "                                                                                          : pt2 " << pt_Taus_Iso.at(1) << " - eta2 " << l1tTauEta.at(idx_Taus_Iso.at(1)) << " - phi2 " << l1tTauPhi.at(idx_Taus_Iso.at(1)) << std::endl;
                    }
                  else
                    {
                      std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 272 L1_DoubleIsoTau35er2p1 additional : no candidate" << std::endl;
                    }
                } 
            }

          if(bit273==0)
            {
              if(pt_Taus_Iso.at(0)>36 && pt_Taus_Iso.at(1)>36)
                {
                  ++uGTmissedBit273.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 273 L1_DoubleIsoTau36er2p1 missed : pt1 " << pt_Taus_Iso.at(0) << " - eta1 " << l1tTauEta.at(idx_Taus_Iso.at(0)) << " - phi1 " << l1tTauPhi.at(idx_Taus_Iso.at(0)) << std::endl;
                  std::cout << "                                                                                          : pt2 " << pt_Taus_Iso.at(1) << " - eta2 " << l1tTauEta.at(idx_Taus_Iso.at(1)) << " - phi2 " << l1tTauPhi.at(idx_Taus_Iso.at(1)) << std::endl;
                }
            }
          else
            {
              if(pt_Taus_Iso.at(0)<36 || pt_Taus_Iso.at(1)<36)
                {
                  ++uGTadditionalBit273.at(ibx);
                  if (idx_Taus.at(0)!=-99 && idx_Taus.at(1)!=-99)
                    {
                      std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 273 L1_DoubleIsoTau36er2p1 additional : pt1 " << pt_Taus_Iso.at(0) << " - eta1 " << l1tTauEta.at(idx_Taus_Iso.at(0)) << " - phi1 " << l1tTauPhi.at(idx_Taus_Iso.at(0)) << std::endl;
                      std::cout << "                                                                                          : pt2 " << pt_Taus_Iso.at(1) << " - eta2 " << l1tTauEta.at(idx_Taus_Iso.at(1)) << " - phi2 " << l1tTauPhi.at(idx_Taus_Iso.at(1)) << std::endl;
                    }
                  else
                    {
                      std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 273 L1_DoubleIsoTau36er2p1 additional : no candidate" << std::endl;
                    }
                }
            }


          // --------------------------------------------------------------------
          // --------------------------------------------------------------------
          // EG TRIGGERS

          std::vector<Float_t> pt_EGs;
          pt_EGs.push_back(-99.); pt_EGs.push_back(-99.);
          std::vector<Int_t> idx_EGs;
          idx_EGs.push_back(-99.); idx_EGs.push_back(-99.);

          float highestEGPt = -99.9;
          int highestEGIdx = -99;
          float highestTightEGPt = -99.9;
          int highestTightEGIdx = -99;
          for(UInt_t iL1EG = 0 ; iL1EG < l1tEGPt.size() ; ++iL1EG)
            {
              if(fabs(l1tEGEta.at(iL1EG))>2.5229) continue;

              // SingleEG trigger - bit168
              if(l1tEGPt.at(iL1EG) >= highestEGPt)
                {
                  highestEGPt = l1tEGPt.at(iL1EG);
                  highestEGIdx = iL1EG;
                }

              // SingleEG trigger - bit192
              if(l1tEGPt.at(iL1EG) >= highestTightEGPt && (l1tEGIso.at(iL1EG)==1 || l1tEGIso.at(iL1EG)==3))
                {
                  highestTightEGPt = l1tEGPt.at(iL1EG);
                  highestTightEGIdx = iL1EG;
                }

              // DiEG trigger - bit 218
              if(l1tEGPt.at(iL1EG)>=pt_EGs.at(0))
                {
                  pt_EGs.at(1)=pt_EGs.at(0);
                  pt_EGs.at(0)=l1tEGPt.at(iL1EG);

                  idx_EGs.at(1)=idx_EGs.at(0);
                  idx_EGs.at(0)=iL1EG;
                }
              else if(l1tEGPt.at(iL1EG)>=pt_EGs.at(1))
                {
                  pt_EGs.at(1)=l1tEGPt.at(iL1EG);
                  idx_EGs.at(1)=iL1EG;
                }
            }

          if(bit168==0)
            {
              if(highestEGPt>36)
                {
                  ++uGTmissedBit168.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 168 L1_SingleEG36er2p5 missed : pt " << highestEGPt << " - eta " << l1tEGEta.at(highestEGIdx) << " - phi " << l1tEGPhi.at(highestEGIdx) << std::endl;
                  
                }
            }
          else
            {
              if(highestEGPt<36)
                {
                  ++uGTadditionalBit168.at(ibx);
                  if(highestEGIdx != -99) std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 168 L1_SingleEG36er2p5 additional : pt " << highestEGPt << " - eta " << l1tEGEta.at(highestEGIdx) << " - phi " << l1tEGPhi.at(highestEGIdx) << std::endl;
                  else                    std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 168 L1_SingleEG36er2p5 additional : no candidate" << std::endl;
                }
            }

          if(bit169==0)
            {
              if(highestEGPt>38)
                {
                  ++uGTmissedBit169.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 169 L1_SingleEG38er2p5 missed : pt " << highestEGPt << " - eta " << l1tEGEta.at(highestEGIdx) << " - phi " << l1tEGPhi.at(highestEGIdx) << std::endl;
                }
            }
          else
            {
              if(highestEGPt<38)
                {
                  ++uGTadditionalBit169.at(ibx);
                  if(highestEGIdx != -99) std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 169 L1_SingleEG38er2p5 additional : pt " << highestEGPt << " - eta " << l1tEGEta.at(highestEGIdx) << " - phi " << l1tEGPhi.at(highestEGIdx) << std::endl;
                  else                    std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 169 L1_SingleEG38er2p5 additional : no candidate" << std::endl;
                }
            }

          if(bit170==0)
            {
              if(highestEGPt>40)
                {
                  ++uGTmissedBit170.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 170 L1_SingleEG40er2p5 missed : pt " << highestEGPt << " - eta " << l1tEGEta.at(highestEGIdx) << " - phi " << l1tEGPhi.at(highestEGIdx) << std::endl;
                }
            }
          else
            {
              if(highestEGPt<40)
                {
                  ++uGTadditionalBit170.at(ibx);
                  if(highestEGIdx != -99) std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 170 L1_SingleEG40er2p5 additional : pt " << highestEGPt << " - eta " << l1tEGEta.at(highestEGIdx) << " - phi " << l1tEGPhi.at(highestEGIdx) << std::endl;
                  else                    std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 170 L1_SingleEG40er2p5 additional : no candidate" << std::endl;
                }
            }

          if(bit171==0)
            {
              if(highestEGPt>42)
                {
                  ++uGTmissedBit171.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 171 L1_SingleEG42er2p5 missed : pt " << highestEGPt << " - eta " << l1tEGEta.at(highestEGIdx) << " - phi " << l1tEGPhi.at(highestEGIdx) << std::endl;
                }
            }
          else
            {
              if(highestEGPt<42)
                {
                  ++uGTadditionalBit171.at(ibx);
                  if(highestEGIdx != -99) std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 171 L1_SingleEG42er2p5 additional : pt " << highestEGPt << " - eta " << l1tEGEta.at(highestEGIdx) << " - phi " << l1tEGPhi.at(highestEGIdx) << std::endl;
                  else                    std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 171 L1_SingleEG42er2p5 additional : no candidate" << std::endl;
                }
            }

          if(bit172==0)
            {
              if(highestEGPt>45)
                {
                  ++uGTmissedBit172.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 172 L1_SingleEG45er2p5 missed : pt " << highestEGPt << " - eta " << l1tEGEta.at(highestEGIdx) << " - phi " << l1tEGPhi.at(highestEGIdx) << std::endl;
                }
            }
          else
            {
              if(highestEGPt<45)
                {
                  ++uGTadditionalBit172.at(ibx);
                  if(highestEGIdx != -99) std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 172 L1_SingleEG45er2p5 additional : pt " << highestEGPt << " - eta " << l1tEGEta.at(highestEGIdx) << " - phi " << l1tEGPhi.at(highestEGIdx) << std::endl;
                  else                    std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 172 L1_SingleEG45er2p5 additional : no candidate" << std::endl;
                }
            }

          if(bit192==0)
            {
              if(highestTightEGPt>30)
                {
                  ++uGTmissedBit192.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 192 L1_SingleIsoEG30er2p5 missed : pt " << highestTightEGPt << " - eta " << l1tEGEta.at(highestTightEGIdx) << " - phi " << l1tEGPhi.at(highestTightEGIdx) << std::endl;
                }
            }
          else
            {
              if(highestTightEGPt<30)
                {
                  ++uGTadditionalBit192.at(ibx);
                  if(highestTightEGIdx != -99) std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 192 L1_SingleIsoEG30er2p5 additional : pt " << highestTightEGPt << " - eta " << l1tEGEta.at(highestTightEGIdx) << " - phi " << l1tEGPhi.at(highestTightEGIdx) << std::endl;
                  else                         std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 192 L1_SingleIsoEG30er2p5 additional : no candidate" << std::endl;
                }
            }

          if(bit194==0)
            {
              if(highestTightEGPt>32)
                {
                  ++uGTmissedBit194.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 194 L1_SingleIsoEG32er2p5 missed : pt " << highestTightEGPt << " - eta " << l1tEGEta.at(highestTightEGIdx) << " - phi " << l1tEGPhi.at(highestTightEGIdx) << std::endl;
                }
            }
          else
            {
              if(highestTightEGPt<32)
                {
                  ++uGTadditionalBit194.at(ibx);
                  if(highestTightEGIdx != -99) std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 194 L1_SingleIsoEG32er2p5 additional : pt " << highestTightEGPt << " - eta " << l1tEGEta.at(highestTightEGIdx) << " - phi " << l1tEGPhi.at(highestTightEGIdx) << std::endl;
                  else                         std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 194 L1_SingleIsoEG32er2p5 additional : no candidate" << std::endl;
                }
            }

          if(bit196==0)
            {
              if(highestTightEGPt>34)
                {
                  ++uGTmissedBit196.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 196 L1_SingleIsoEG34er2p5 missed : pt " << highestTightEGPt << " - eta " << l1tEGEta.at(highestTightEGIdx) << " - phi " << l1tEGPhi.at(highestTightEGIdx) << std::endl;
                }
            }
          else
            {
              if(highestTightEGPt<34)
                {
                  ++uGTadditionalBit196.at(ibx);
                  if(highestTightEGIdx != -99) std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 196 L1_SingleIsoEG34er2p5 additional : pt " << highestTightEGPt << " - eta " << l1tEGEta.at(highestTightEGIdx) << " - phi " << l1tEGPhi.at(highestTightEGIdx) << std::endl;
                  else                         std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 196 L1_SingleIsoEG34er2p5 additional : no candidate" << std::endl;
                }
            }

          if(bit218==0)
            {
              if(pt_EGs.at(0)>25 && pt_EGs.at(1)>12)
                {
                  ++uGTmissedBit218.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 218 L1_DoubleEG_25_12_er2p5 missed : pt1 " << pt_EGs.at(0) << " - eta1 " << l1tEGEta.at(idx_EGs.at(0)) << " - phi1 " << l1tEGPhi.at(idx_EGs.at(0)) << std::endl;
                  std::cout << "                                                       : pt2 " << pt_EGs.at(1) << " - eta2 " << l1tEGEta.at(idx_EGs.at(1)) << " - phi2 " << l1tEGPhi.at(idx_EGs.at(1)) << std::endl;
                }
            }
          else
            {
              if(pt_EGs.at(0)<25 || pt_EGs.at(1)<12)
                {
                  ++uGTadditionalBit218.at(ibx);
                  if (idx_EGs.at(0)!=-99 && idx_EGs.at(1)!=-99)
                    {
                      std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 218 L1_DoubleEG_25_12_er2p5 additional : pt1 " << pt_EGs.at(0) << " - eta1 " << l1tEGEta.at(idx_EGs.at(0)) << " - phi1 " << l1tEGPhi.at(idx_EGs.at(0)) << std::endl;
                      std::cout << "                                                                                         : pt2 " << pt_EGs.at(1) << " - eta2 " << l1tEGEta.at(idx_EGs.at(1)) << " - phi2 " << l1tEGPhi.at(idx_EGs.at(1)) << std::endl;
                    }
                  else
                    {
                      std::cout << "  ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 218 L1_DoubleEG_25_12_er2p5 additional : no candidate" << std::endl;
                    }
                }
            }

          if(bit219==0)
            {
              if(pt_EGs.at(0)>25 && pt_EGs.at(1)>14)
                {
                  ++uGTmissedBit219.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 219 L1_DoubleEG_25_14_er2p5 missed : pt1 " << pt_EGs.at(0) << " - eta1 " << l1tEGEta.at(idx_EGs.at(0)) << " - phi1 " << l1tEGPhi.at(idx_EGs.at(0)) << std::endl;
                  std::cout << "                                                                                          : pt2 " << pt_EGs.at(1) << " - eta2 " << l1tEGEta.at(idx_EGs.at(1)) << " - phi2 " << l1tEGPhi.at(idx_EGs.at(1)) << std::endl;
                }
            }
          else
            {
              if(pt_EGs.at(0)<25 || pt_EGs.at(1)<14)
                {
                  ++uGTadditionalBit219.at(ibx);
                  if (idx_EGs.at(0)!=-99 && idx_EGs.at(1)!=-99)
                    {
                      std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 219 L1_DoubleEG_25_14_er2p5 additional : pt1 " << pt_EGs.at(0) << " - eta1 " << l1tEGEta.at(idx_EGs.at(0)) << " - phi1 " << l1tEGPhi.at(idx_EGs.at(0)) << std::endl;
                      std::cout << "                                                                                       : pt2 " << pt_EGs.at(1) << " - eta2 " << l1tEGEta.at(idx_EGs.at(1)) << " - phi2 " << l1tEGPhi.at(idx_EGs.at(1)) << std::endl;
                    }
                  else
                    {
                      std::cout << "  ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 219 L1_DoubleEG_25_14_er2p5 additional : no candidate" << std::endl;
                    }
                }
            }

          if(bit220==0)
            {
              if(pt_EGs.at(0)>27 && pt_EGs.at(1)>14)
                {
                  ++uGTmissedBit220.at(ibx);
                  std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 220 L1_DoubleEG_27_14_er2p5 missed : pt1 " << pt_EGs.at(0) << " - eta1 " << l1tEGEta.at(idx_EGs.at(0)) << " - phi1 " << l1tEGPhi.at(idx_EGs.at(0)) << std::endl;
                  std::cout << "                                                                                   : pt2 " << pt_EGs.at(1) << " - eta2 " << l1tEGEta.at(idx_EGs.at(1)) << " - phi2 " << l1tEGPhi.at(idx_EGs.at(1)) << std::endl;
                }
            }
          else
            {
              if(pt_EGs.at(0)<27 || pt_EGs.at(1)<14)
                {
                  ++uGTadditionalBit220.at(ibx);
                  if (idx_EGs.at(0)!=-99 && idx_EGs.at(1)!=-99)
                    {
                      std::cout << " ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 220 L1_DoubleEG_27_14_er2p5 additional : pt1 " << pt_EGs.at(0) << " - eta1 " << l1tEGEta.at(idx_EGs.at(0)) << " - phi1 " << l1tEGPhi.at(idx_EGs.at(0)) << std::endl;
                      std::cout << "                                                                                        : pt2 " << pt_EGs.at(1) << " - eta2 " << l1tEGEta.at(idx_EGs.at(1)) << " - phi2 " << l1tEGPhi.at(idx_EGs.at(1)) << std::endl;
                    }
                  else
                    {
                      std::cout << "  ** WARNING: EVT "+to_string(in_EventNumber)+" / BX-"+to_string(BX)+" L1 Bit 220 L1_DoubleEG_27_14_er2p5 additional : no candidate" << std::endl;
                    }
                }
            }
        }
    }



    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "Denominator = " << Denominator << " evts" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "21 - uGT has missed bit L1_SingleMu22 BX-2 = "    << uGTmissedBit21.at(0) << " evts (" << uGTmissedBit21.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "21 - uGT has missed bit L1_SingleMu22 BX-1 = "    << uGTmissedBit21.at(1) << " evts (" << uGTmissedBit21.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "21 - uGT has missed bit L1_SingleMu22 BX0  = "    << uGTmissedBit21.at(2) << " evts (" << uGTmissedBit21.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "21 - uGT has missed bit L1_SingleMu22 BX+1 = "    << uGTmissedBit21.at(3) << " evts (" << uGTmissedBit21.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "21 - uGT has missed bit L1_SingleMu22 BX+2 = "    << uGTmissedBit21.at(4) << " evts (" << uGTmissedBit21.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "21 - uGT has additional bit L1_SingleMu22 BX-2 = "    << uGTadditionalBit21.at(0) << " evts (" << uGTadditionalBit21.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "21 - uGT has additional bit L1_SingleMu22 BX-1 = "    << uGTadditionalBit21.at(1) << " evts (" << uGTadditionalBit21.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "21 - uGT has additional bit L1_SingleMu22 BX0  = "    << uGTadditionalBit21.at(2) << " evts (" << uGTadditionalBit21.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "21 - uGT has additional bit L1_SingleMu22 BX+1 = "    << uGTadditionalBit21.at(3) << " evts (" << uGTadditionalBit21.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "21 - uGT has additional bit L1_SingleMu22 BX+2 = "    << uGTadditionalBit21.at(4) << " evts (" << uGTadditionalBit21.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "168 - uGT has missed bit L1_SingleEG36er2p5 BX-2 = "    << uGTmissedBit168.at(0) << " evts (" << uGTmissedBit168.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "168 - uGT has missed bit L1_SingleEG36er2p5 BX-1 = "    << uGTmissedBit168.at(1) << " evts (" << uGTmissedBit168.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "168 - uGT has missed bit L1_SingleEG36er2p5 BX0  = "    << uGTmissedBit168.at(2) << " evts (" << uGTmissedBit168.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "168 - uGT has missed bit L1_SingleEG36er2p5 BX+1 = "    << uGTmissedBit168.at(3) << " evts (" << uGTmissedBit168.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "168 - uGT has missed bit L1_SingleEG36er2p5 BX+2 = "    << uGTmissedBit168.at(4) << " evts (" << uGTmissedBit168.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "168 - uGT has additional bit L1_SingleEG36er2p5 BX-2 = "    << uGTadditionalBit168.at(0) << " evts (" << uGTadditionalBit168.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "168 - uGT has additional bit L1_SingleEG36er2p5 BX-1 = "    << uGTadditionalBit168.at(1) << " evts (" << uGTadditionalBit168.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "168 - uGT has additional bit L1_SingleEG36er2p5 BX0  = "    << uGTadditionalBit168.at(2) << " evts (" << uGTadditionalBit168.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "168 - uGT has additional bit L1_SingleEG36er2p5 BX+1 = "    << uGTadditionalBit168.at(3) << " evts (" << uGTadditionalBit168.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "168 - uGT has additional bit L1_SingleEG36er2p5 BX+2 = "    << uGTadditionalBit168.at(4) << " evts (" << uGTadditionalBit168.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "169 - uGT has missed bit L1_SingleEG38er2p5 BX-2 = "    << uGTmissedBit169.at(0) << " evts (" << uGTmissedBit169.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "169 - uGT has missed bit L1_SingleEG38er2p5 BX-1 = "    << uGTmissedBit169.at(1) << " evts (" << uGTmissedBit169.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "169 - uGT has missed bit L1_SingleEG38er2p5 BX0  = "    << uGTmissedBit169.at(2) << " evts (" << uGTmissedBit169.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "169 - uGT has missed bit L1_SingleEG38er2p5 BX+1 = "    << uGTmissedBit169.at(3) << " evts (" << uGTmissedBit169.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "169 - uGT has missed bit L1_SingleEG38er2p5 BX+2 = "    << uGTmissedBit169.at(4) << " evts (" << uGTmissedBit169.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "169 - uGT has additional bit L1_SingleEG38er2p5 BX-2 = "    << uGTadditionalBit169.at(0) << " evts (" << uGTadditionalBit169.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "169 - uGT has additional bit L1_SingleEG38er2p5 BX-1 = "    << uGTadditionalBit169.at(1) << " evts (" << uGTadditionalBit169.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "169 - uGT has additional bit L1_SingleEG38er2p5 BX0  = "    << uGTadditionalBit169.at(2) << " evts (" << uGTadditionalBit169.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "169 - uGT has additional bit L1_SingleEG38er2p5 BX+1 = "    << uGTadditionalBit169.at(3) << " evts (" << uGTadditionalBit169.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "169 - uGT has additional bit L1_SingleEG38er2p5 BX+2 = "    << uGTadditionalBit169.at(4) << " evts (" << uGTadditionalBit169.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "170 - uGT has missed bit L1_SingleEG40er2p5 BX-2 = "    << uGTmissedBit170.at(0) << " evts (" << uGTmissedBit170.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "170 - uGT has missed bit L1_SingleEG40er2p5 BX-1 = "    << uGTmissedBit170.at(1) << " evts (" << uGTmissedBit170.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "170 - uGT has missed bit L1_SingleEG40er2p5 BX0  = "    << uGTmissedBit170.at(2) << " evts (" << uGTmissedBit170.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "170 - uGT has missed bit L1_SingleEG40er2p5 BX+1 = "    << uGTmissedBit170.at(3) << " evts (" << uGTmissedBit170.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "170 - uGT has missed bit L1_SingleEG40er2p5 BX+2 = "    << uGTmissedBit170.at(4) << " evts (" << uGTmissedBit170.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "170 - uGT has additional bit L1_SingleEG40er2p5 BX-2 = "    << uGTadditionalBit170.at(0) << " evts (" << uGTadditionalBit170.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "170 - uGT has additional bit L1_SingleEG40er2p5 BX-1 = "    << uGTadditionalBit170.at(1) << " evts (" << uGTadditionalBit170.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "170 - uGT has additional bit L1_SingleEG40er2p5 BX0  = "    << uGTadditionalBit170.at(2) << " evts (" << uGTadditionalBit170.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "170 - uGT has additional bit L1_SingleEG40er2p5 BX+1 = "    << uGTadditionalBit170.at(3) << " evts (" << uGTadditionalBit170.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "170 - uGT has additional bit L1_SingleEG40er2p5 BX+2 = "    << uGTadditionalBit170.at(4) << " evts (" << uGTadditionalBit170.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "171 - uGT has missed bit L1_SingleEG42er2p5 BX-2 = "    << uGTmissedBit171.at(0) << " evts (" << uGTmissedBit171.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "171 - uGT has missed bit L1_SingleEG42er2p5 BX-1 = "    << uGTmissedBit171.at(1) << " evts (" << uGTmissedBit171.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "171 - uGT has missed bit L1_SingleEG42er2p5 BX0  = "    << uGTmissedBit171.at(2) << " evts (" << uGTmissedBit171.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "171 - uGT has missed bit L1_SingleEG42er2p5 BX+1 = "    << uGTmissedBit171.at(3) << " evts (" << uGTmissedBit171.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "171 - uGT has missed bit L1_SingleEG42er2p5 BX+2 = "    << uGTmissedBit171.at(4) << " evts (" << uGTmissedBit171.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "171 - uGT has additional bit L1_SingleEG42er2p5 BX-2 = "    << uGTadditionalBit171.at(0) << " evts (" << uGTadditionalBit171.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "171 - uGT has additional bit L1_SingleEG42er2p5 BX-1 = "    << uGTadditionalBit171.at(1) << " evts (" << uGTadditionalBit171.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "171 - uGT has additional bit L1_SingleEG42er2p5 BX0  = "    << uGTadditionalBit171.at(2) << " evts (" << uGTadditionalBit171.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "171 - uGT has additional bit L1_SingleEG42er2p5 BX+1 = "    << uGTadditionalBit171.at(3) << " evts (" << uGTadditionalBit171.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "171 - uGT has additional bit L1_SingleEG42er2p5 BX+2 = "    << uGTadditionalBit171.at(4) << " evts (" << uGTadditionalBit171.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "172 - uGT has missed bit L1_SingleEG45er2p5 BX-2 = "    << uGTmissedBit172.at(0) << " evts (" << uGTmissedBit172.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "172 - uGT has missed bit L1_SingleEG45er2p5 BX-1 = "    << uGTmissedBit172.at(1) << " evts (" << uGTmissedBit172.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "172 - uGT has missed bit L1_SingleEG45er2p5 BX0  = "    << uGTmissedBit172.at(2) << " evts (" << uGTmissedBit172.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "172 - uGT has missed bit L1_SingleEG45er2p5 BX+1 = "    << uGTmissedBit172.at(3) << " evts (" << uGTmissedBit172.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "172 - uGT has missed bit L1_SingleEG45er2p5 BX+2 = "    << uGTmissedBit172.at(4) << " evts (" << uGTmissedBit172.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "172 - uGT has additional bit L1_SingleEG45er2p5 BX-2 = "    << uGTadditionalBit172.at(0) << " evts (" << uGTadditionalBit172.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "172 - uGT has additional bit L1_SingleEG45er2p5 BX-1 = "    << uGTadditionalBit172.at(1) << " evts (" << uGTadditionalBit172.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "172 - uGT has additional bit L1_SingleEG45er2p5 BX0  = "    << uGTadditionalBit172.at(2) << " evts (" << uGTadditionalBit172.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "172 - uGT has additional bit L1_SingleEG45er2p5 BX+1 = "    << uGTadditionalBit172.at(3) << " evts (" << uGTadditionalBit172.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "172 - uGT has additional bit L1_SingleEG45er2p5 BX+2 = "    << uGTadditionalBit172.at(4) << " evts (" << uGTadditionalBit172.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "192 - uGT has missed bit L1_SingleIsoEG30er2p5 BX-2 = "    << uGTmissedBit192.at(0) << " evts (" << uGTmissedBit192.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "192 - uGT has missed bit L1_SingleIsoEG30er2p5 BX-1 = "    << uGTmissedBit192.at(1) << " evts (" << uGTmissedBit192.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "192 - uGT has missed bit L1_SingleIsoEG30er2p5 BX0  = "    << uGTmissedBit192.at(2) << " evts (" << uGTmissedBit192.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "192 - uGT has missed bit L1_SingleIsoEG30er2p5 BX+1 = "    << uGTmissedBit192.at(3) << " evts (" << uGTmissedBit192.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "192 - uGT has missed bit L1_SingleIsoEG30er2p5 BX+2 = "    << uGTmissedBit192.at(4) << " evts (" << uGTmissedBit192.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "192 - uGT has additional bit L1_SingleIsoEG30er2p5 BX-2 = "    << uGTadditionalBit192.at(0) << " evts (" << uGTadditionalBit192.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "192 - uGT has additional bit L1_SingleIsoEG30er2p5 BX-1 = "    << uGTadditionalBit192.at(1) << " evts (" << uGTadditionalBit192.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "192 - uGT has additional bit L1_SingleIsoEG30er2p5 BX0  = "    << uGTadditionalBit192.at(2) << " evts (" << uGTadditionalBit192.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "192 - uGT has additional bit L1_SingleIsoEG30er2p5 BX+1 = "    << uGTadditionalBit192.at(3) << " evts (" << uGTadditionalBit192.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "192 - uGT has additional bit L1_SingleIsoEG30er2p5 BX+2 = "    << uGTadditionalBit192.at(4) << " evts (" << uGTadditionalBit192.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "194 - uGT has missed bit L1_SingleIsoEG32er2p5 BX-2 = "    << uGTmissedBit194.at(0) << " evts (" << uGTmissedBit194.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "194 - uGT has missed bit L1_SingleIsoEG32er2p5 BX-1 = "    << uGTmissedBit194.at(1) << " evts (" << uGTmissedBit194.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "194 - uGT has missed bit L1_SingleIsoEG32er2p5 BX0  = "    << uGTmissedBit194.at(2) << " evts (" << uGTmissedBit194.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "194 - uGT has missed bit L1_SingleIsoEG32er2p5 BX+1 = "    << uGTmissedBit194.at(3) << " evts (" << uGTmissedBit194.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "194 - uGT has missed bit L1_SingleIsoEG32er2p5 BX+2 = "    << uGTmissedBit194.at(4) << " evts (" << uGTmissedBit194.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "194 - uGT has additional bit L1_SingleIsoEG32er2p5 BX-2 = "    << uGTadditionalBit194.at(0) << " evts (" << uGTadditionalBit194.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "194 - uGT has additional bit L1_SingleIsoEG32er2p5 BX-1 = "    << uGTadditionalBit194.at(1) << " evts (" << uGTadditionalBit194.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "194 - uGT has additional bit L1_SingleIsoEG32er2p5 BX0  = "    << uGTadditionalBit194.at(2) << " evts (" << uGTadditionalBit194.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "194 - uGT has additional bit L1_SingleIsoEG32er2p5 BX+1 = "    << uGTadditionalBit194.at(3) << " evts (" << uGTadditionalBit194.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "194 - uGT has additional bit L1_SingleIsoEG32er2p5 BX+2 = "    << uGTadditionalBit194.at(4) << " evts (" << uGTadditionalBit194.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "196 - uGT has missed bit L1_SingleIsoEG34er2p5 BX-2 = "    << uGTmissedBit196.at(0) << " evts (" << uGTmissedBit196.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "196 - uGT has missed bit L1_SingleIsoEG34er2p5 BX-1 = "    << uGTmissedBit196.at(1) << " evts (" << uGTmissedBit196.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "196 - uGT has missed bit L1_SingleIsoEG34er2p5 BX0  = "    << uGTmissedBit196.at(2) << " evts (" << uGTmissedBit196.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "196 - uGT has missed bit L1_SingleIsoEG34er2p5 BX+1 = "    << uGTmissedBit196.at(3) << " evts (" << uGTmissedBit196.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "196 - uGT has missed bit L1_SingleIsoEG34er2p5 BX+2 = "    << uGTmissedBit196.at(4) << " evts (" << uGTmissedBit196.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "196 - uGT has additional bit L1_SingleIsoEG34er2p5 BX-2 = "    << uGTadditionalBit196.at(0) << " evts (" << uGTadditionalBit196.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "196 - uGT has additional bit L1_SingleIsoEG34er2p5 BX-1 = "    << uGTadditionalBit196.at(1) << " evts (" << uGTadditionalBit196.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "196 - uGT has additional bit L1_SingleIsoEG34er2p5 BX0  = "    << uGTadditionalBit196.at(2) << " evts (" << uGTadditionalBit196.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "196 - uGT has additional bit L1_SingleIsoEG34er2p5 BX+1 = "    << uGTadditionalBit196.at(3) << " evts (" << uGTadditionalBit196.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "196 - uGT has additional bit L1_SingleIsoEG34er2p5 BX+2 = "    << uGTadditionalBit196.at(4) << " evts (" << uGTadditionalBit196.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "218 - uGT has missed bit L1_DoubleEG_25_12_er2p5 BX-2 = "    << uGTmissedBit218.at(0) << " evts (" << uGTmissedBit218.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "218 - uGT has missed bit L1_DoubleEG_25_12_er2p5 BX-1 = "    << uGTmissedBit218.at(1) << " evts (" << uGTmissedBit218.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "218 - uGT has missed bit L1_DoubleEG_25_12_er2p5 BX0  = "    << uGTmissedBit218.at(2) << " evts (" << uGTmissedBit218.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "218 - uGT has missed bit L1_DoubleEG_25_12_er2p5 BX+1 = "    << uGTmissedBit218.at(3) << " evts (" << uGTmissedBit218.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "218 - uGT has missed bit L1_DoubleEG_25_12_er2p5 BX+2 = "    << uGTmissedBit218.at(4) << " evts (" << uGTmissedBit218.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "218 - uGT has additional bit L1_DoubleEG_25_12_er2p5 BX-2 = "    << uGTadditionalBit218.at(0) << " evts (" << uGTadditionalBit218.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "218 - uGT has additional bit L1_DoubleEG_25_12_er2p5 BX-1 = "    << uGTadditionalBit218.at(1) << " evts (" << uGTadditionalBit218.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "218 - uGT has additional bit L1_DoubleEG_25_12_er2p5 BX0  = "    << uGTadditionalBit218.at(2) << " evts (" << uGTadditionalBit218.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "218 - uGT has additional bit L1_DoubleEG_25_12_er2p5 BX+1 = "    << uGTadditionalBit218.at(3) << " evts (" << uGTadditionalBit218.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "218 - uGT has additional bit L1_DoubleEG_25_12_er2p5 BX+2 = "    << uGTadditionalBit218.at(4) << " evts (" << uGTadditionalBit218.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "219 - uGT has missed bit L1_DoubleEG_25_14_er2p5 BX-2 = "    << uGTmissedBit219.at(0) << " evts (" << uGTmissedBit219.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "219 - uGT has missed bit L1_DoubleEG_25_14_er2p5 BX-1 = "    << uGTmissedBit219.at(1) << " evts (" << uGTmissedBit219.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "219 - uGT has missed bit L1_DoubleEG_25_14_er2p5 BX0  = "    << uGTmissedBit219.at(2) << " evts (" << uGTmissedBit219.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "219 - uGT has missed bit L1_DoubleEG_25_14_er2p5 BX+1 = "    << uGTmissedBit219.at(3) << " evts (" << uGTmissedBit219.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "219 - uGT has missed bit L1_DoubleEG_25_14_er2p5 BX+2 = "    << uGTmissedBit219.at(4) << " evts (" << uGTmissedBit219.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "219 - uGT has additional bit L1_DoubleEG_25_14_er2p5 BX-2 = "    << uGTadditionalBit219.at(0) << " evts (" << uGTadditionalBit219.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "219 - uGT has additional bit L1_DoubleEG_25_14_er2p5 BX-1 = "    << uGTadditionalBit219.at(1) << " evts (" << uGTadditionalBit219.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "219 - uGT has additional bit L1_DoubleEG_25_14_er2p5 BX0  = "    << uGTadditionalBit219.at(2) << " evts (" << uGTadditionalBit219.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "219 - uGT has additional bit L1_DoubleEG_25_14_er2p5 BX+1 = "    << uGTadditionalBit219.at(3) << " evts (" << uGTadditionalBit219.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "219 - uGT has additional bit L1_DoubleEG_25_14_er2p5 BX+2 = "    << uGTadditionalBit219.at(4) << " evts (" << uGTadditionalBit219.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "220 - uGT has missed bit L1_DoubleEG_27_14_er2p5 BX-2 = "    << uGTmissedBit220.at(0) << " evts (" << uGTmissedBit220.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "220 - uGT has missed bit L1_DoubleEG_27_14_er2p5 BX-1 = "    << uGTmissedBit220.at(1) << " evts (" << uGTmissedBit220.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "220 - uGT has missed bit L1_DoubleEG_27_14_er2p5 BX0  = "    << uGTmissedBit220.at(2) << " evts (" << uGTmissedBit220.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "220 - uGT has missed bit L1_DoubleEG_27_14_er2p5 BX+1 = "    << uGTmissedBit220.at(3) << " evts (" << uGTmissedBit220.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "220 - uGT has missed bit L1_DoubleEG_27_14_er2p5 BX+2 = "    << uGTmissedBit220.at(4) << " evts (" << uGTmissedBit220.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "220 - uGT has additional bit L1_DoubleEG_27_14_er2p5 BX-2 = "    << uGTadditionalBit220.at(0) << " evts (" << uGTadditionalBit220.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "220 - uGT has additional bit L1_DoubleEG_27_14_er2p5 BX-1 = "    << uGTadditionalBit220.at(1) << " evts (" << uGTadditionalBit220.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "220 - uGT has additional bit L1_DoubleEG_27_14_er2p5 BX0  = "    << uGTadditionalBit220.at(2) << " evts (" << uGTadditionalBit220.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "220 - uGT has additional bit L1_DoubleEG_27_14_er2p5 BX+1 = "    << uGTadditionalBit220.at(3) << " evts (" << uGTadditionalBit220.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "220 - uGT has additional bit L1_DoubleEG_27_14_er2p5 BX+2 = "    << uGTadditionalBit220.at(4) << " evts (" << uGTadditionalBit220.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "263 - uGT has missed bit L1_SingleTau70er2p1 BX-2 = "    << uGTmissedBit263.at(0) << " evts (" << uGTmissedBit263.at(0) * scale * 1000 / Denominator / 50 << " Hz @ PS 50)" << std::endl;
    std::cout << "263 - uGT has missed bit L1_SingleTau70er2p1 BX-1 = "    << uGTmissedBit263.at(1) << " evts (" << uGTmissedBit263.at(1) * scale * 1000 / Denominator / 50 << " Hz @ PS 50)" << std::endl;
    std::cout << "263 - uGT has missed bit L1_SingleTau70er2p1 BX0  = "    << uGTmissedBit263.at(2) << " evts (" << uGTmissedBit263.at(2) * scale * 1000 / Denominator / 50 << " Hz @ PS 50)" << std::endl;
    std::cout << "263 - uGT has missed bit L1_SingleTau70er2p1 BX+1 = "    << uGTmissedBit263.at(3) << " evts (" << uGTmissedBit263.at(3) * scale * 1000 / Denominator / 50 << " Hz @ PS 50)" << std::endl;
    std::cout << "263 - uGT has missed bit L1_SingleTau70er2p1 BX+2 = "    << uGTmissedBit263.at(4) << " evts (" << uGTmissedBit263.at(4) * scale * 1000 / Denominator / 50 << " Hz @ PS 50)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "263 - uGT has additional bit L1_SingleTau70er2p1 BX-2 = "    << uGTadditionalBit263.at(0) << " evts (" << uGTadditionalBit263.at(0) * scale * 1000 / Denominator / 50 << " Hz @ PS 50)" << std::endl;
    std::cout << "263 - uGT has additional bit L1_SingleTau70er2p1 BX-1 = "    << uGTadditionalBit263.at(1) << " evts (" << uGTadditionalBit263.at(1) * scale * 1000 / Denominator / 50 << " Hz @ PS 50)" << std::endl;
    std::cout << "263 - uGT has additional bit L1_SingleTau70er2p1 BX0  = "    << uGTadditionalBit263.at(2) << " evts (" << uGTadditionalBit263.at(2) * scale * 1000 / Denominator / 50 << " Hz @ PS 50)" << std::endl;
    std::cout << "263 - uGT has additional bit L1_SingleTau70er2p1 BX+1 = "    << uGTadditionalBit263.at(3) << " evts (" << uGTadditionalBit263.at(3) * scale * 1000 / Denominator / 50 << " Hz @ PS 50)" << std::endl;
    std::cout << "263 - uGT has additional bit L1_SingleTau70er2p1 BX+2 = "    << uGTadditionalBit263.at(4) << " evts (" << uGTadditionalBit263.at(4) * scale * 1000 / Denominator / 50 << " Hz @ PS 50)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "264 - uGT has missed bit L1_SingleTau120er2p1 BX-2 = "    << uGTmissedBit264.at(0) << " evts (" << uGTmissedBit264.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "264 - uGT has missed bit L1_SingleTau120er2p1 BX-1 = "    << uGTmissedBit264.at(1) << " evts (" << uGTmissedBit264.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "264 - uGT has missed bit L1_SingleTau120er2p1 BX0  = "    << uGTmissedBit264.at(2) << " evts (" << uGTmissedBit264.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "264 - uGT has missed bit L1_SingleTau120er2p1 BX+1 = "    << uGTmissedBit264.at(3) << " evts (" << uGTmissedBit264.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "264 - uGT has missed bit L1_SingleTau120er2p1 BX+2 = "    << uGTmissedBit264.at(4) << " evts (" << uGTmissedBit264.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "264 - uGT has additional bit L1_SingleTau120er2p1 BX-2 = "    << uGTadditionalBit264.at(0) << " evts (" << uGTadditionalBit264.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "264 - uGT has additional bit L1_SingleTau120er2p1 BX-1 = "    << uGTadditionalBit264.at(1) << " evts (" << uGTadditionalBit264.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "264 - uGT has additional bit L1_SingleTau120er2p1 BX0  = "    << uGTadditionalBit264.at(2) << " evts (" << uGTadditionalBit264.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "264 - uGT has additional bit L1_SingleTau120er2p1 BX+1 = "    << uGTadditionalBit264.at(3) << " evts (" << uGTadditionalBit264.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "264 - uGT has additional bit L1_SingleTau120er2p1 BX+2 = "    << uGTadditionalBit264.at(4) << " evts (" << uGTadditionalBit264.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "267 - uGT has missed bit L1_DoubleTau70er2p1 BX-2 = "    << uGTmissedBit267.at(0) << " evts (" << uGTmissedBit267.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "267 - uGT has missed bit L1_DoubleTau70er2p1 BX-1 = "    << uGTmissedBit267.at(1) << " evts (" << uGTmissedBit267.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "267 - uGT has missed bit L1_DoubleTau70er2p1 BX0  = "    << uGTmissedBit267.at(2) << " evts (" << uGTmissedBit267.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "267 - uGT has missed bit L1_DoubleTau70er2p1 BX+1 = "    << uGTmissedBit267.at(3) << " evts (" << uGTmissedBit267.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "267 - uGT has missed bit L1_DoubleTau70er2p1 BX+2 = "    << uGTmissedBit267.at(4) << " evts (" << uGTmissedBit267.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "267 - uGT has additional bit L1_DoubleTau70er2p1 BX-2 = "    << uGTadditionalBit267.at(0) << " evts (" << uGTadditionalBit267.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "267 - uGT has additional bit L1_DoubleTau70er2p1 BX-1 = "    << uGTadditionalBit267.at(1) << " evts (" << uGTadditionalBit267.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "267 - uGT has additional bit L1_DoubleTau70er2p1 BX0  = "    << uGTadditionalBit267.at(2) << " evts (" << uGTadditionalBit267.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "267 - uGT has additional bit L1_DoubleTau70er2p1 BX+1 = "    << uGTadditionalBit267.at(3) << " evts (" << uGTadditionalBit267.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "267 - uGT has additional bit L1_DoubleTau70er2p1 BX+2 = "    << uGTadditionalBit267.at(4) << " evts (" << uGTadditionalBit267.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "271 - uGT has missed bit L1_DoubleIsoTau34er2p1 BX-2 = "    << uGTmissedBit271.at(0) << " evts (" << uGTmissedBit271.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "271 - uGT has missed bit L1_DoubleIsoTau34er2p1 BX-1 = "    << uGTmissedBit271.at(1) << " evts (" << uGTmissedBit271.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "271 - uGT has missed bit L1_DoubleIsoTau34er2p1 BX0  = "    << uGTmissedBit271.at(2) << " evts (" << uGTmissedBit271.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "271 - uGT has missed bit L1_DoubleIsoTau34er2p1 BX+1 = "    << uGTmissedBit271.at(3) << " evts (" << uGTmissedBit271.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "271 - uGT has missed bit L1_DoubleIsoTau34er2p1 BX+2 = "    << uGTmissedBit271.at(4) << " evts (" << uGTmissedBit271.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "271 - uGT has additional bit L1_DoubleIsoTau34er2p1 BX-2 = "    << uGTadditionalBit271.at(0) << " evts (" << uGTadditionalBit271.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "271 - uGT has additional bit L1_DoubleIsoTau34er2p1 BX-1 = "    << uGTadditionalBit271.at(1) << " evts (" << uGTadditionalBit271.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "271 - uGT has additional bit L1_DoubleIsoTau34er2p1 BX0  = "    << uGTadditionalBit271.at(2) << " evts (" << uGTadditionalBit271.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "271 - uGT has additional bit L1_DoubleIsoTau34er2p1 BX+1 = "    << uGTadditionalBit271.at(3) << " evts (" << uGTadditionalBit271.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "271 - uGT has additional bit L1_DoubleIsoTau34er2p1 BX+2 = "    << uGTadditionalBit271.at(4) << " evts (" << uGTadditionalBit271.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "272 - uGT has missed bit L1_DoubleIsoTau35er2p1 BX-2 = "    << uGTmissedBit272.at(0) << " evts (" << uGTmissedBit272.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "272 - uGT has missed bit L1_DoubleIsoTau35er2p1 BX-1 = "    << uGTmissedBit272.at(1) << " evts (" << uGTmissedBit272.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "272 - uGT has missed bit L1_DoubleIsoTau35er2p1 BX0  = "    << uGTmissedBit272.at(2) << " evts (" << uGTmissedBit272.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "272 - uGT has missed bit L1_DoubleIsoTau35er2p1 BX+1 = "    << uGTmissedBit272.at(3) << " evts (" << uGTmissedBit272.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "272 - uGT has missed bit L1_DoubleIsoTau35er2p1 BX+2 = "    << uGTmissedBit272.at(4) << " evts (" << uGTmissedBit272.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "272 - uGT has additional bit L1_DoubleIsoTau35er2p1 BX-2 = "    << uGTadditionalBit272.at(0) << " evts (" << uGTadditionalBit272.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "272 - uGT has additional bit L1_DoubleIsoTau35er2p1 BX-1 = "    << uGTadditionalBit272.at(1) << " evts (" << uGTadditionalBit272.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "272 - uGT has additional bit L1_DoubleIsoTau35er2p1 BX0  = "    << uGTadditionalBit272.at(2) << " evts (" << uGTadditionalBit272.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "272 - uGT has additional bit L1_DoubleIsoTau35er2p1 BX+1 = "    << uGTadditionalBit272.at(3) << " evts (" << uGTadditionalBit272.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "272 - uGT has additional bit L1_DoubleIsoTau35er2p1 BX+2 = "    << uGTadditionalBit272.at(4) << " evts (" << uGTadditionalBit272.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "273 - uGT has missed bit L1_DoubleIsoTau36er2p1 BX-2 = "    << uGTmissedBit273.at(0) << " evts (" << uGTmissedBit273.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "273 - uGT has missed bit L1_DoubleIsoTau36er2p1 BX-1 = "    << uGTmissedBit273.at(1) << " evts (" << uGTmissedBit273.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "273 - uGT has missed bit L1_DoubleIsoTau36er2p1 BX0  = "    << uGTmissedBit273.at(2) << " evts (" << uGTmissedBit273.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "273 - uGT has missed bit L1_DoubleIsoTau36er2p1 BX+1 = "    << uGTmissedBit273.at(3) << " evts (" << uGTmissedBit273.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "273 - uGT has missed bit L1_DoubleIsoTau36er2p1 BX+2 = "    << uGTmissedBit273.at(4) << " evts (" << uGTmissedBit273.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "273 - uGT has additional bit L1_DoubleIsoTau36er2p1 BX-2 = "    << uGTadditionalBit273.at(0) << " evts (" << uGTadditionalBit273.at(0) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "273 - uGT has additional bit L1_DoubleIsoTau36er2p1 BX-1 = "    << uGTadditionalBit273.at(1) << " evts (" << uGTadditionalBit273.at(1) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "273 - uGT has additional bit L1_DoubleIsoTau36er2p1 BX0  = "    << uGTadditionalBit273.at(2) << " evts (" << uGTadditionalBit273.at(2) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "273 - uGT has additional bit L1_DoubleIsoTau36er2p1 BX+1 = "    << uGTadditionalBit273.at(3) << " evts (" << uGTadditionalBit273.at(3) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "273 - uGT has additional bit L1_DoubleIsoTau36er2p1 BX+2 = "    << uGTadditionalBit273.at(4) << " evts (" << uGTadditionalBit273.at(4) * scale * 1000 / Denominator << " Hz)" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;



    TFile f("candidates_vs_algobits_Run2022"+era+".root","RECREATE");
    uGTmissedBit263_BXm2_pt->Write();
    uGTmissedBit263_BXm2_eta->Write();
    uGTmissedBit263_BXm2_phi->Write();
    uGTadditionalBit263_BXm2_pt->Write();
    uGTadditionalBit263_BXm2_eta->Write();
    uGTadditionalBit263_BXm2_phi->Write();
    uGTmissedBit263_BXm1_pt->Write();
    uGTmissedBit263_BXm1_eta->Write();
    uGTmissedBit263_BXm1_phi->Write();
    uGTadditionalBit263_BXm1_pt->Write();
    uGTadditionalBit263_BXm1_eta->Write();
    uGTadditionalBit263_BXm1_phi->Write();
    uGTmissedBit263_BX0_pt->Write();
    uGTmissedBit263_BX0_eta->Write();
    uGTmissedBit263_BX0_phi->Write();
    uGTadditionalBit263_BX0_pt->Write();
    uGTadditionalBit263_BX0_eta->Write();
    uGTadditionalBit263_BX0_phi->Write();
    uGTmissedBit263_BXp1_pt->Write();
    uGTmissedBit263_BXp1_eta->Write();
    uGTmissedBit263_BXp1_phi->Write();
    uGTadditionalBit263_BXp1_pt->Write();
    uGTadditionalBit263_BXp1_eta->Write();
    uGTadditionalBit263_BXp1_phi->Write();
    uGTmissedBit263_BXp2_pt->Write();
    uGTmissedBit263_BXp2_eta->Write();
    uGTmissedBit263_BXp2_phi->Write();
    uGTadditionalBit263_BXp2_pt->Write();
    uGTadditionalBit263_BXp2_eta->Write();
    uGTadditionalBit263_BXp2_phi->Write();


    return;
}
