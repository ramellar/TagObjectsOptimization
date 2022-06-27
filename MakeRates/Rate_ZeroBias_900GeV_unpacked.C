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

void Rate(TString tag, bool doScaleToLumi)
{

  TString FileName_in = "/data_CMS/cms/motta/Run3preparation/ZeroBias1_Run2022A_"+tag+"_reEmuTPs/ZeroBias1_Run2022A_"+tag+"_reEmuTPs.root";
  TFile f_in(FileName_in.Data(),"READ");
  TTree* inTree = (TTree*)f_in.Get("ZeroBias/ZeroBias"); // tree of uncalibrated EphemeralZeroBias NTuples

  //Replace by unpacked stuff from input file that you produced with ApplyCalibrationUnpacked.C
  ULong64_t       in_EventNumber =  0;
  Int_t           in_RunNumber =  0;
  Int_t           in_lumi =  0;
  vector<float>   *in_l1tPt =  0;
  vector<float>   *in_l1tEta =  0;
  vector<float>   *in_l1tPhi =  0;
  vector<int>     *in_l1tQual =  0;
  vector<int>     *in_l1tIso = 0;
 
  inTree->SetBranchAddress("EventNumber", &in_EventNumber);
  inTree->SetBranchAddress("RunNumber", &in_RunNumber);
  // branches of uncalibrated EphemeralZeroBias NTuples
  inTree->SetBranchAddress("lumi", &in_lumi);
  inTree->SetBranchAddress("l1tPt",&in_l1tPt);
  inTree->SetBranchAddress("l1tEta",&in_l1tEta);
  inTree->SetBranchAddress("l1tPhi",&in_l1tPhi);
  inTree->SetBranchAddress("l1tQual",&in_l1tQual);
  inTree->SetBranchAddress("l1tIso",&in_l1tIso);
  // branches of calibrated EphemeralZeroBias NTuples
  //inTree->SetBranchAddress("L1Tau_pt",&in_l1tPt);
  //inTree->SetBranchAddress("L1Tau_eta",&in_l1tEta);
  //inTree->SetBranchAddress("L1Tau_phi",&in_l1tPhi);
  //inTree->SetBranchAddress("L1Tau_Qual",&in_l1tQual);
  //inTree->SetBranchAddress("L1Tau_Iso",&in_l1tIso);

  TH1F* pt_IsoInf = new TH1F("pt_IsoInf","pt_IsoInf",240,0.,240.);
  TH1F* pt_Iso = new TH1F("pt_Iso","pt_Iso",240,0.,240.);

  TH2F* pt_IsoInf_DiTau = new TH2F("pt_IsoInf_DiTau","pt_IsoInf_DiTau",240,0.,240.,240,0.,240.);
  TH2F* pt_Iso_DiTau = new TH2F("pt_Iso_DiTau","pt_Iso_DiTau",240,0.,240.,240,0.,240.);

  Int_t Denominator = 0;

  cout<<"begin loop"<<endl;
  cout<<inTree->GetEntries()<<endl;
 
  for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    { 
      inTree->GetEntry(i);
      if(i%10000==0) cout<<"Entry #"<<i<<endl; 
      if (in_RunNumber == 352900) { if(in_lumi<3) continue; }
      if (in_RunNumber == 353060) { if(in_lumi<15 || in_lumi>300) continue; }
      if (in_RunNumber == 353087) { if(in_lumi<3 || in_lumi>283) continue; }
      if (in_RunNumber == 353689) { if(in_lumi<9) continue; }
      if (in_RunNumber == 353015) { if(in_lumi>309) continue; }
      if (in_RunNumber == 352425) { if(in_lumi>434) continue; }
      if (in_RunNumber == 352931) { if(in_lumi==8 || in_lumi>334) continue; }
      if (in_RunNumber == 353709) { if(in_lumi>160) continue; }

      Float_t weight = 1.;

      ++Denominator;
            
      bool Filled_IsoInf = kFALSE;
      bool Filled_Iso = kFALSE;

      std::vector<Int_t> Index_Taus_IsoInf;
      Index_Taus_IsoInf.push_back(-1); Index_Taus_IsoInf.push_back(-1);
      std::vector<Float_t> pt_Taus_IsoInf;
      pt_Taus_IsoInf.push_back(-99.); pt_Taus_IsoInf.push_back(-99.);

      std::vector<Int_t> Index_Taus_Iso;
      Index_Taus_Iso.push_back(-1); Index_Taus_Iso.push_back(-1);
      std::vector<Float_t> pt_Taus_Iso;
      pt_Taus_Iso.push_back(-99.); pt_Taus_Iso.push_back(-99.);

      //Replace by l1tPt
      for(UInt_t iL1Tau = 0 ; iL1Tau < in_l1tPt->size() ; ++iL1Tau)
	{
	  if(fabs(in_l1tEta->at(iL1Tau))>2.1315) continue;

	  //DiTau trigger
	  if(in_l1tPt->at(iL1Tau)>=pt_Taus_IsoInf.at(0))
	    {
	      Index_Taus_IsoInf.at(1)=Index_Taus_IsoInf.at(0);
	      pt_Taus_IsoInf.at(1)=pt_Taus_IsoInf.at(0);
	      Index_Taus_IsoInf.at(0)=iL1Tau;
	      pt_Taus_IsoInf.at(0)=in_l1tPt->at(iL1Tau);
	    }
	  else if(in_l1tPt->at(iL1Tau)>=pt_Taus_IsoInf.at(1))
	    {
	      Index_Taus_IsoInf.at(1)=iL1Tau;
	      pt_Taus_IsoInf.at(1)=in_l1tPt->at(iL1Tau);
	    }

	  if(in_l1tPt->at(iL1Tau)>=pt_Taus_Iso.at(0) && in_l1tIso->at(iL1Tau)>0)
	    {
	      Index_Taus_Iso.at(1)=Index_Taus_Iso.at(0);
	      pt_Taus_Iso.at(1)=pt_Taus_Iso.at(0);
	      Index_Taus_Iso.at(0)=iL1Tau;
	      pt_Taus_Iso.at(0)=in_l1tPt->at(iL1Tau);
	    }
	  else if(in_l1tPt->at(iL1Tau)>=pt_Taus_Iso.at(1) && in_l1tIso->at(iL1Tau)>0)
	    {
	      Index_Taus_Iso.at(1)=iL1Tau;
	      pt_Taus_Iso.at(1)=in_l1tPt->at(iL1Tau);
	    }

	}

      if(Index_Taus_IsoInf.at(0)>=0 && Index_Taus_IsoInf.at(1)>=0)
	{
	  pt_IsoInf_DiTau->Fill(pt_Taus_IsoInf.at(0),pt_Taus_IsoInf.at(1),weight);
	}

      if(Index_Taus_Iso.at(0)>=0 && Index_Taus_Iso.at(1)>=0)
	{
	  pt_Iso_DiTau->Fill(pt_Taus_Iso.at(0),pt_Taus_Iso.at(1),weight);
	}
    } //new

  // SET RUN INFO
  float nb = 0.;
  if (tag == "Runs2bunches0p03lumi" || tag == "Runs2bunches0p015lumi") nb=2.;
  if (tag == "Runs3bunches0p04lumi") nb=3.;
  if (nb == 0.) {
    std::cout << "** ERROR: number of bunches not correctly initialized - EXITING!" << std::endl;
    return;
  }

  float thisLumiRun = 0.;
  if (tag == "Runs2bunches0p03lumi")  thisLumiRun=0.03E30;
  if (tag == "Runs2bunches0p015lumi") thisLumiRun=0.015E30;
  if (tag == "Runs3bunches0p04lumi")  thisLumiRun=0.04E30;
  if (thisLumiRun == 0.)
  {
    std::cout << "** ERROR: luminosity not correctly initialized - EXITING!" << std::endl;
    return;
  }

  float scaleToLumi = 2.00E34;
  float scale = 0.001 * nb * 11245.6;
  if (doScaleToLumi) scale = scale * scaleToLumi/thisLumiRun;

  cout<<"Denominator = "<<Denominator<<endl;

  TH1F* rate_noCut_DiTau = new TH1F("rate_noCut_DiTau","rate_noCut_DiTau",240,0.,240.);
  TH1F* rate_Iso_DiTau = new TH1F("rate_Iso_DiTau","rate_Iso_DiTau",240,0.,240.);

  for(UInt_t i = 0 ; i < 241 ; ++i)
    {
      rate_noCut_DiTau->SetBinContent(i+1,pt_IsoInf_DiTau->Integral(i+1,241,i+1,241)/Denominator*scale);
      rate_Iso_DiTau->SetBinContent(i+1,pt_Iso_DiTau->Integral(i+1,241,i+1,241)/Denominator*scale);
    }

  TString scaledToLumi = "";
  if (doScaleToLumi) scaledToLumi = "_scaledTo2e34Lumi";
  TFile f("histos/histos_rate_ZeroBias1_Run2022A_"+tag+"_optimizationV11gs_effMin0p9_G3_calibThr1p7_V11_unpacked"+scaledToLumi+".root","RECREATE");

  pt_IsoInf_DiTau->Write();
  pt_Iso_DiTau->Write();

  rate_noCut_DiTau->Write();
  rate_Iso_DiTau->Write();

  return;
}

