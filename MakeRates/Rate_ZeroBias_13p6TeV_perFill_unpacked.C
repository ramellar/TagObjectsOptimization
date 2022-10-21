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

void Rate(TString tag, TString fill_str, bool doScaleToLumi, float calibThr = 1.7)
{
  TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
  TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

  TString FileName_in = "/data_CMS/cms/motta/Run3preparation/ZeroBias_Run2022"+tag+"_Fill"+fill_str+"_RAW_reEmuTPs/ZeroBias_Run2022"+tag+"_Fill"+fill_str+"_RAW_reEmuTPs.root";
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
  inTree->SetBranchAddress("lumi", &in_lumi);
  inTree->SetBranchAddress("l1tPt",&in_l1tPt);
  inTree->SetBranchAddress("l1tEta",&in_l1tEta);
  inTree->SetBranchAddress("l1tPhi",&in_l1tPhi);
  inTree->SetBranchAddress("l1tQual",&in_l1tQual);
  inTree->SetBranchAddress("l1tIso",&in_l1tIso);

  TH1F* pt_IsoInf = new TH1F("pt_IsoInf","pt_IsoInf",240,0.,240.);
  TH1F* pt_Iso = new TH1F("pt_Iso","pt_Iso",240,0.,240.);

  TH2F* pt_IsoInf_DiTau = new TH2F("pt_IsoInf_DiTau","pt_IsoInf_DiTau",240,0.,240.,240,0.,240.);
  TH2F* pt_Iso_DiTau = new TH2F("pt_Iso_DiTau","pt_Iso_DiTau",240,0.,240.,240,0.,240.);

  Int_t Denominator = 0;

  // SET RUN INFO
  float nb = 0.;
  if (fill_str == "8088-8094-8102") { nb = 1922; }
  if (fill_str == "8112-8113-8115") { nb = 2160; }
  if (nb == 0.)
  {
    std::cout << "ERROR: something went wrong with the run selection and the nb initialization" << std::endl;
    return;
  }
  float thisLumiRun = 0.;

  cout<<"begin loop"<<endl;
  cout<<inTree->GetEntries()<<endl;
 
  for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    { 
      inTree->GetEntry(i);
      if(i%10000==0) cout<<"Entry #"<<i<<endl; 
      // SET RUN INFO
      if (in_RunNumber == 356722) { if(in_lumi<569) continue; }
      if (in_RunNumber == 356719) { if(in_lumi<83 || in_lumi > 917) continue; }
      if (in_RunNumber == 356815) { if(in_lumi>219) continue; }
      if (in_RunNumber == 356814) { if((in_lumi>127 && in_lumi<169) || in_lumi>488) continue; }
      // if (in_RunNumber == 356812) { if(in_lumi) continue; }
      // if (in_RunNumber == 356811) { if(in_lumi) continue; }
      if (in_RunNumber == 356810) { if(in_lumi<47) continue; }
      if (in_RunNumber == 356824) { if(in_lumi<26) continue; }
      if (in_RunNumber == 356955) { if(in_lumi>282 && in_lumi<295) continue; }
      if (in_RunNumber == 356954) { if(in_lumi>364) continue; }
      // if (in_RunNumber == 356951) { if(in_lumi) continue; }
      // if (in_RunNumber == 356947) { if(in_lumi) continue; }
      // if (in_RunNumber == 356946) { if(in_lumi) continue; }
      if (in_RunNumber == 356943) { if(in_lumi<40) continue; }
      if (in_RunNumber == 357081) { if(in_lumi>757) continue; }
      if (in_RunNumber == 357080) { if(in_lumi<18 || in_lumi>432) continue; }
      if (in_RunNumber == 357078) { if(in_lumi==102 || (in_lumi>14 && in_lumi<18)) continue; }
      if (in_RunNumber == 357112) { if(in_lumi>519) continue; }
      if (in_RunNumber == 357101) { if(in_lumi<54 || in_lumi>105) continue; }
      // if (in_RunNumber == 357102) { if(in_lumi) continue; }
      if (in_RunNumber == 357721) { if(in_lumi<172 || (in_lumi>304 && in_lumi<325) || in_lumi>634) continue; }

      if (in_RunNumber == 356722) thisLumiRun = 8.7E33;
      if (in_RunNumber == 356719) thisLumiRun = 10.3E33;
      if (in_RunNumber == 356815) thisLumiRun = 9.5E33;
      if (in_RunNumber == 356814) thisLumiRun = 10.5E33;
      if (in_RunNumber == 356812) thisLumiRun = 12.1E33;
      if (in_RunNumber == 356811) thisLumiRun = 12.8E33;
      if (in_RunNumber == 356810) thisLumiRun = 10E33;
      if (in_RunNumber == 356824) thisLumiRun = 8E33;
      if (in_RunNumber == 356955) thisLumiRun = 7E33;
      if (in_RunNumber == 356954) thisLumiRun = 8E33;
      if (in_RunNumber == 356951) thisLumiRun = 9E33;
      if (in_RunNumber == 356947) thisLumiRun = 11.5E33;
      if (in_RunNumber == 356946) thisLumiRun = 12.7E33;
      if (in_RunNumber == 356943) thisLumiRun = 12E33;
      if (in_RunNumber == 357081) thisLumiRun = 10E33;
      if (in_RunNumber == 357080) thisLumiRun = 14.5E33;
      if (in_RunNumber == 357078) thisLumiRun = 15.5E33;
      if (in_RunNumber == 357112) thisLumiRun = 11.4E33;
      if (in_RunNumber == 357101) thisLumiRun = 15E33;
      if (in_RunNumber == 357102) thisLumiRun = 14.6E33;
      if (in_RunNumber == 357721) thisLumiRun = 14E33;
      if (thisLumiRun == 0. and doScaleToLumi)
      {
        std::cout << "ERROR: something went wrong with the run selection and the lumi initialization" << std::endl;
        return;
      }

      float scaleToLumi = 2.00E34;
      float scale = 0.001 * nb * 11245.6;
      // cout << "    --> Run "+to_string(in_RunNumber)+" - scaleToLumi " << scaleToLumi << "  - scale " << scale << " - thisLumiRun " << thisLumiRun << endl;
      // cout << "        scaleToLumi / thisLumiRun " << scaleToLumi / thisLumiRun << endl;
      if (doScaleToLumi) scale *= scaleToLumi / thisLumiRun;
      // cout << "        scale " << scale << endl;

      Float_t weight = scale;

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
    } // end of for loop

  cout << "Denominator = " << Denominator << endl;

  TH1F* rate_noCut_DiTau = new TH1F("rate_noCut_DiTau","rate_noCut_DiTau",240,0.,240.);
  TH1F* rate_Iso_DiTau = new TH1F("rate_Iso_DiTau","rate_Iso_DiTau",240,0.,240.);

  for(UInt_t i = 0 ; i < 241 ; ++i)
    {
      rate_noCut_DiTau->SetBinContent(i+1,pt_IsoInf_DiTau->Integral(i+1,241,i+1,241)/Denominator);
      rate_Iso_DiTau->SetBinContent(i+1,pt_Iso_DiTau->Integral(i+1,241,i+1,241)/Denominator);
    }

  TString scaledToLumi = "";
  if (doScaleToLumi) scaledToLumi = "_scaledTo2e34Lumi";
  TFile f("histos/histos_rate_ZeroBias_Run2022"+tag+"_Fill"+fill_str+"_unpacked"+scaledToLumi+".root","RECREATE");

  pt_IsoInf_DiTau->Write();
  pt_Iso_DiTau->Write();

  rate_noCut_DiTau->Write();
  rate_Iso_DiTau->Write();

  return;
}

