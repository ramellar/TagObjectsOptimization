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

void Rate(int run, bool doScaleToLumi = false, float calibThr = 1.7)
{
  TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
  TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

  TString run_str = to_string(run);

  TString FileName_in = "/data_CMS/cms/mchiusi/Run3preparation/Run3preparation_2023/2023_07_27_olivier/EphemeralZeroBias0__Run2022G_Run362616__RAW_curr_calo/EphemeralZeroBias_362616_olivier_current.root";
  // TString FileName_in = "/data_CMS/cms/mchiusi/Run3preparation/Run3preparation_2023/DP_note/rates_2023C_376883/rates_367883_RAW.root";
  TFile f_in(FileName_in.Data(),"READ");
  
  // TTree* inTree = (TTree*)f_in.Get("Ntuplizer_noTagAndProbe/TagAndProbe");
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

  TH2F* DiTauPtPassDistribution_noIso = new TH2F("DiTauPtPassDistribution_noIso","DiTauPtPassDistribution_noIso",240,0.,240.,240,0.,240.);
  TH2F* DiTauPtPassDistribution_Iso = new TH2F("DiTauPtPassDistribution_Iso","DiTauPtPassDistribution_Iso",240,0.,240.,240,0.,240.);

  Int_t Denominator = 0;

  cout<<"begin loop"<<endl;
  cout<<inTree->GetEntries()<<endl;
 
  for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    { 
      inTree->GetEntry(i);
      if(i%100000==0) cout<<"Entry #"<<i<<endl; 
      // SET RUN INFO
      if (run == 355414) { if(in_lumi<0) continue; }
      if (run == 355417) { if(in_lumi>40) continue; }
      if (run == 355418) { if((in_lumi>38 && in_lumi<60) || in_lumi>98) continue; }
      if (run == 355769) { if((in_lumi>76 && in_lumi<99) || (in_lumi>162 && in_lumi<198) || (in_lumi>245 && in_lumi<269) || (in_lumi>333 && in_lumi<357) || (in_lumi>436 && in_lumi<460) || in_lumi>539) continue; }
      if (run == 355865) { if(in_lumi>286 || in_lumi<26) continue; }
      if (run == 355872) { if(in_lumi>1217 || in_lumi<997) continue; }
      if (run == 355913) { if(in_lumi>103) continue; }
      if (run == 356375) { if(in_lumi>1001) continue; }
      if (run == 356378) { if(in_lumi<-1) continue; }
      if (run == 356381) { if(in_lumi<35) continue; }
      if (in_RunNumber == 362616) { if(in_lumi<0) continue; }
      if (in_RunNumber == 362617) { if(in_lumi<0) continue; }
      if (run == 366727) { if(in_lumi<198 || in_lumi>242) continue; }
      if (run == 366821) { if(in_lumi<144 || in_lumi>920) continue; }
      if (run == 366833) { if(in_lumi<240 || in_lumi>620) continue; }
      if (run == 366895) { if(in_lumi<370 || in_lumi>1242) continue; }
      if (run == 367079) { if(in_lumi<196 || in_lumi>570) continue; }
      if (run == 366874) { if(in_lumi<59 || in_lumi>564) continue; }
      if (run == 366801) { if(in_lumi<115 || in_lumi>240) continue; }
      if (run == 366820) { if(in_lumi<240 || in_lumi>410) continue; }
      if (run == 366729) { if(in_lumi<28 || in_lumi>410) continue; }
      if (run == 367619) { if(in_lumi>576) continue; }

      Float_t weight = 1.;

      ++Denominator;

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
          DiTauPtPassDistribution_noIso->Fill(pt_Taus_IsoInf.at(0),pt_Taus_IsoInf.at(1),weight);
        }

      if(Index_Taus_Iso.at(0)>=0 && Index_Taus_Iso.at(1)>=0)
        {
          DiTauPtPassDistribution_Iso->Fill(pt_Taus_Iso.at(0),pt_Taus_Iso.at(1),weight);
        }
    } //new

  // SET RUN INFO
  float nb = 0.;
  if (run == 355414 or run == 355417 or run == 355418) { nb = 62.; }
  if (run == 355769) { nb = 302.; }
  if (run == 355865 or run == 355872 or run == 355913) { nb = 590.; }
  if (run == 356375 or run == 356378 or run == 356381) { nb = 974.; }
  if (run == 362616 or run == 362617) { nb = 2450; }
  if (run == 366727) { nb = 398; } // colliding bunches
  if (run == 366821) { nb = 398; } // colliding bunches  
  if (run == 366833) { nb = 386; } // colliding bunches  
  if (run == 366895) { nb = 986; } // colliding bunches  
  if (run == 367079) { nb = 986; } // colliding bunches  
  if (run == 366874) { nb = 898; } // colliding bunches
  if (run == 366801) { nb = 398; } // colliding bunches
  if (run == 366820) { nb = 398; } // colliding bunches
  if (run == 366729) { nb = 398; } // colliding bunches
  if (run == 367619) { nb = 2361; } // colliding bunches
  if (run == 367883) { nb = 2345; } // colliding bunches
  if (nb == 0.)
  {
    std::cout << "ERROR: something went wrong with the run selection and the nb initialization" << std::endl;
    return;
  }
  float thisLumiRun = 0.;
  if (run == 355414) thisLumiRun = 0.265E33;
  if (run == 355417) thisLumiRun = 0.256E33;
  if (run == 355418) thisLumiRun = 0.249E33;
  if (run == 355769) thisLumiRun = 1.540E33;
  if (run == 355865) thisLumiRun = 2.567E33;
  if (run == 355872) thisLumiRun = 2.667E33;
  if (run == 355913) thisLumiRun = 2.537E33;
  if (run == 356375) thisLumiRun = 5.700E33;
  if (run == 356378) thisLumiRun = 6.350E33;
  if (run == 356381) thisLumiRun = 5.000E33;
  if (run == 362616) thisLumiRun = 2.05E34;
  if (run == 362617) thisLumiRun = 2.50E34;
  if (run == 366727) thisLumiRun = 3.6E33; // end lumi
  if (run == 366820) thisLumiRun = 3.5E33; // end lumi
  if (run == 366821) thisLumiRun = 2.7E33; // end lumi
  if (run == 366833) thisLumiRun = 2.8E33; // end lumi
  if (run == 366895) thisLumiRun = 6.3E33; // end lumi
  if (run == 367079) thisLumiRun = 8.4E33; // end lumi
  if (run == 366874) thisLumiRun = 6.9E33; // end lumi
  if (run == 366801) thisLumiRun = 2.7E33; // end lumi
  if (run == 366729) thisLumiRun = 3.2E33; // end lumi
  if (run == 367619) thisLumiRun = 1.3E34; // end lumi
  if (run == 367883) thisLumiRun = 2.0E34; // end lumi
  if (thisLumiRun == 0. and doScaleToLumi)
  {
    std::cout << "ERROR: something went wrong with the run selection and the lumi initialization" << std::endl;
    return;
  }

  float scaleToLumi = 2.00E34;
  float scale = 0.001 * nb * 11245.6;
  if (doScaleToLumi) scale = scale * scaleToLumi/thisLumiRun;

  cout<<"Denominator = "<<Denominator<<endl;

  TH1F* DiTauRate_noIso = new TH1F("DiTauRate_noIso","DiTauRate_noIso",240,0.,240.);
  TH1F* DiTauRate_Iso = new TH1F("DiTauRate_Iso","DiTauRate_Iso",240,0.,240.);

  for(UInt_t i = 0 ; i < 241 ; ++i)
    {
      DiTauRate_noIso->SetBinContent(i+1,DiTauPtPassDistribution_noIso->Integral(i+1,241,i+1,241)/Denominator*scale);
      DiTauRate_Iso->SetBinContent(i+1,DiTauPtPassDistribution_Iso->Integral(i+1,241,i+1,241)/Denominator*scale);
    }

  std::cout << "DoubleIsoTau32 = " << DiTauRate_Iso->GetBinContent(33) << " kHz" << std::endl;
  std::cout << "DoubleIsoTau34 = " << DiTauRate_Iso->GetBinContent(35) << " kHz" << std::endl;
  std::cout << "DoubleIsoTau36 = " << DiTauRate_Iso->GetBinContent(37) << " kHz" << std::endl;

  TString scaledToLumi = "";
  if (doScaleToLumi) scaledToLumi = "_scaledTo2e34Lumi";
  TFile f_out("histos_2023/olivier/histos_rate_ZeroBias_Run"+run_str+"_unpacked"+scaledToLumi+".root","RECREATE");

  DiTauPtPassDistribution_noIso->Write();
  DiTauPtPassDistribution_Iso->Write();

  DiTauRate_noIso->Write();
  DiTauRate_Iso->Write();

  f_out.Close();

  return;
}

