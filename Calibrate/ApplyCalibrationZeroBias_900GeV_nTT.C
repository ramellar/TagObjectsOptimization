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
#include <TGraphAsymmErrors.h>
#include <stdio.h>
#include <math.h>
#include "ApplyCalibration_newnTT.C"

using namespace std;

void ApplyCalibrationZeroBias(TString tag, float calibThr = 1.7)
// void ApplyCalibrationZeroBias(TString InputFileName = "Ntuple_ZeroBias_With2017Layer1_ShapeVeto_20_02_17.root")
{

  TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
  TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));
  TString InputFileName  = "/data_CMS/cms/motta/Run3preparation/ZeroBias1_Run2022A_"+tag+"_reEmuTPs/ZeroBias1_Run2022A_"+tag+"_reEmuTPs.root";
  TString OutputFileName = "/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr"+intgr+"p"+decim+"/ZeroBias1_Run2022A_"+tag+"_reEmuTPs_CALIBRATED.root";

  TH2F* isolation_vs_pt = new TH2F("isolation_vs_pt","isolation_vs_pt",100,0,100,NbinsIEt2-1,hardcodedIetBins2double);
  isolation_vs_pt->Clear();

  const UInt_t nIsolation = 101;
  const Double_t isolationBins[nIsolation] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100};

  TFile f_histos("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/Calibrate/corrections/corrections_Trigger_Stage2_Run3_MC_VBFHToTauTau_M125_compressedieta_compressediet_hasEM_isMerged_optimizationV13.root","READ");
  TH3F* h_LUT_isMerged0 = (TH3F*)f_histos.Get("LUT_isMerged0_GBRFullLikelihood_Trigger_Stage2_Run3_MC_VBFHToTauTau_M125_compressedieta_compressediet_hasEM_isMerged_optimizationV13");
  TH3F* h_LUT_isMerged1 = (TH3F*)f_histos.Get("LUT_isMerged1_GBRFullLikelihood_Trigger_Stage2_Run3_MC_VBFHToTauTau_M125_compressedieta_compressediet_hasEM_isMerged_optimizationV13"); 
 
  TH1F* histo_shape = new TH1F("histo_shape","histo_shape",256,0,256);
  TH1F* histo_symmShape = new TH1F("histo_symmShape","histo_symmShape",256,0,256);

  TFile f_in(InputFileName.Data(),"READ");
  TTree* inTree = (TTree*)f_in.Get("ZeroBias/ZeroBias");

  // Declaration of leaf types
  ULong64_t       EventNumber =  0;
  Int_t           RunNumber =  0;
  Int_t           lumi =  0;
  vector<float>   *l1tPt =  0;
  vector<float>   *l1tEta =  0;
  vector<float>   *l1tPhi =  0;
  vector<int>     *l1tQual =  0;
  vector<int>     *l1tIso =  0;
  vector<float>   *l1tEmuPt =  0;
  vector<float>   *l1tEmuEta =  0;
  vector<float>   *l1tEmuPhi =  0;
  vector<int>     *l1tEmuQual =  0;
  vector<int>     *l1tEmuIso =  0;
  vector<int>     *l1tEmuNTT =  0;
  vector<int>     *l1tEmuHasEM =  0;
  vector<int>     *l1tEmuIsMerged =  0;
  vector<int>     *l1tEmuTowerIEta =  0;
  vector<int>     *l1tEmuTowerIPhi =  0;
  vector<int>     *l1tEmuRawEt =  0;
  vector<int>     *l1tEmuIsoEt =  0;
   
  TBranch        *b_EventNumber;   //!
  TBranch        *b_RunNumber;   //!
  TBranch        *b_lumi;   //!
  TBranch        *b_l1tPt;   //!
  TBranch        *b_l1tEta;   //!
  TBranch        *b_l1tPhi;   //!
  TBranch        *b_l1tQual;   //!
  TBranch        *b_l1tIso;   //!
  TBranch        *b_l1tEmuPt;   //!
  TBranch        *b_l1tEmuEta;   //!
  TBranch        *b_l1tEmuPhi;   //!
  TBranch        *b_l1tEmuQual;   //!
  TBranch        *b_l1tEmuIso;   //!
  TBranch        *b_l1tEmuNTT;   //!
  TBranch        *b_l1tEmuHasEM;   //!
  TBranch        *b_l1tEmuIsMerged;   //!
  TBranch        *b_l1tEmuTowerIEta;   //!
  TBranch        *b_l1tEmuTowerIPhi;   //!
  TBranch        *b_l1tEmuRawEt;   //!
  TBranch        *b_l1tEmuIsoEt;   //!
      
  inTree->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
  inTree->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
  inTree->SetBranchAddress("lumi", &lumi, &b_lumi);
  inTree->SetBranchAddress("l1tEmuPt", &l1tEmuPt, &b_l1tEmuPt);
  inTree->SetBranchAddress("l1tEmuEta", &l1tEmuEta, &b_l1tEmuEta);
  inTree->SetBranchAddress("l1tEmuPhi", &l1tEmuPhi, &b_l1tEmuPhi);
  inTree->SetBranchAddress("l1tEmuQual", &l1tEmuQual, &b_l1tEmuQual);
  inTree->SetBranchAddress("l1tEmuIso", &l1tEmuIso, &b_l1tEmuIso);
  inTree->SetBranchAddress("l1tEmuNTT", &l1tEmuNTT, &b_l1tEmuNTT);
  inTree->SetBranchAddress("l1tEmuHasEM", &l1tEmuHasEM, &b_l1tEmuHasEM);
  inTree->SetBranchAddress("l1tEmuIsMerged", &l1tEmuIsMerged, &b_l1tEmuIsMerged);
  inTree->SetBranchAddress("l1tEmuTowerIEta", &l1tEmuTowerIEta, &b_l1tEmuTowerIEta);
  inTree->SetBranchAddress("l1tEmuTowerIPhi", &l1tEmuTowerIPhi, &b_l1tEmuTowerIPhi);
  inTree->SetBranchAddress("l1tEmuRawEt", &l1tEmuRawEt, &b_l1tEmuRawEt);
  inTree->SetBranchAddress("l1tEmuIsoEt", &l1tEmuIsoEt, &b_l1tEmuIsoEt);
  
  TFile f(OutputFileName.Data(),"RECREATE");
  TTree outTree("outTreeCalibrated","outTreeCalibrated");  

  ULong64_t       out_EventNumber =  0;
  Int_t           out_RunNumber =  0;
  Int_t           out_lumi =  0;
  vector<float>   *out_l1tEmuPt =  0;
  vector<float>   *out_l1tEmuEta =  0;
  vector<float>   *out_l1tEmuPhi =  0;
  vector<int>     *out_l1tEmuQual =  0;
  vector<int>     *out_l1tEmuNTT =  0;
  vector<int>     *out_l1tEmuHasEM =  0;
  vector<int>     *out_l1tEmuIsMerged =  0;
  vector<int>     *out_l1tEmuTowerIEta =  0;
  vector<int>     *out_l1tEmuTowerIPhi =  0;
  vector<int>     *out_l1tEmuRawEt =  0;
  vector<int>     *out_l1tEmuIsoEt =  0;
  vector<int>     *out_compressedieta =  0;
  vector<int>     *out_compressedE =  0;
  vector<int>     *out_compressednTT =  0;
  vector<int>     *out_supercompressedE =  0;
  vector<int>     *out_supercompressednTT =  0;
  vector<float>   *out_CalibPt =  0;
  vector<Bool_t>  *out_L1Tau_PassShapeVeto = 0;
  
  outTree.Branch("EventNumber", &out_EventNumber, "EventNumber/I");
  outTree.Branch("RunNumber", &out_RunNumber, "RunNumber/I");
  outTree.Branch("lumi", &out_lumi, "lumi/I");
  outTree.Branch("L1Tau_pt",&out_l1tEmuPt);
  outTree.Branch("L1Tau_eta",&out_l1tEmuEta);
  outTree.Branch("L1Tau_phi",&out_l1tEmuPhi);
  outTree.Branch("L1Tau_Qual",&out_l1tEmuQual);
  outTree.Branch("L1Tau_nTT",&out_l1tEmuNTT);
  outTree.Branch("L1Tau_hasEM",&out_l1tEmuHasEM);
  outTree.Branch("L1Tau_isMerged",&out_l1tEmuIsMerged);
  outTree.Branch("L1Tau_IEt",&out_l1tEmuRawEt);
  outTree.Branch("L1Tau_Iso",&out_l1tEmuIsoEt);
  outTree.Branch("L1Tau_IEta",&out_l1tEmuTowerIEta);
  outTree.Branch("L1Tau_IPhi",&out_l1tEmuTowerIPhi);
  outTree.Branch("compressedieta",&out_compressedieta);
  outTree.Branch("compressedE",&out_compressedE);
  outTree.Branch("compressednTT",&out_compressednTT);
  outTree.Branch("supercompressedE",&out_supercompressedE);
  outTree.Branch("supercompressednTT",&out_supercompressednTT);
  outTree.Branch("L1Tau_CalibPt",&out_CalibPt);
  outTree.Branch("L1Tau_PassShapeVeto",&out_L1Tau_PassShapeVeto);

  map<int, int> remap;
  remap[0] = 6 ;
  remap[1] = 5 ;
  remap[2] = 1 ;
  remap[3] = 0 ;
  remap[4] = 4 ;
  remap[5] = 3 ;
  remap[6] = 2 ;

  // pt in 20-30
  vector<int> veto_set_20 = {0,4,99,8,20};
  // pt in 30-40
  vector<int> veto_set_30 = {127, 119, 99, 111, 103, 23, 55, 20, 31, 27, 63, 107, 19, 51, 28};
  // pt in 40-50
  vector<int> veto_set_40 = {127, 119, 99, 103, 20, 111, 23, 19, 27, 4, 55};


  UInt_t Entries = inTree->GetEntries();
  

  for(UInt_t i = 0 ; i < Entries ; ++i)
  //for(UInt_t i = 0 ; i < 100000 ; ++i)
    {
      inTree->GetEntry(i);
      if(i%10000==0) cout<<"Entry #"<<i<<"/"<<Entries<<endl;

      out_EventNumber = -999;
      out_RunNumber =  -999;
      out_lumi = -999; 
      out_l1tEmuPt->clear();
      out_l1tEmuEta->clear();
      out_l1tEmuPhi->clear();
      out_l1tEmuQual->clear();
      out_l1tEmuNTT->clear();
      out_l1tEmuHasEM->clear();
      out_l1tEmuIsMerged->clear();
      out_l1tEmuTowerIEta->clear();
      out_l1tEmuTowerIPhi->clear();
      out_l1tEmuRawEt->clear();
      out_l1tEmuIsoEt->clear();
      out_compressedieta->clear();
      out_compressedE->clear();
      out_compressednTT->clear();
      out_supercompressedE->clear();
      out_supercompressednTT->clear();    
      out_CalibPt->clear();
      out_L1Tau_PassShapeVeto->clear();

      //loop on L1 taus
      for(UInt_t iL1Tau = 0 ; iL1Tau < l1tEmuPt->size() ; ++iL1Tau)
        {

          out_EventNumber = EventNumber;
          out_RunNumber =  RunNumber;
          out_lumi = lumi; 
          out_l1tEmuPt->push_back(l1tEmuPt->at(iL1Tau));
          out_l1tEmuEta->push_back(l1tEmuEta->at(iL1Tau));
          out_l1tEmuPhi->push_back(l1tEmuPhi->at(iL1Tau));
          out_l1tEmuQual->push_back(l1tEmuQual->at(iL1Tau));
          out_l1tEmuNTT->push_back(l1tEmuNTT->at(iL1Tau));
          out_l1tEmuHasEM->push_back(l1tEmuHasEM->at(iL1Tau));
          out_l1tEmuIsMerged->push_back(l1tEmuIsMerged->at(iL1Tau));
          out_l1tEmuTowerIEta->push_back(l1tEmuTowerIEta->at(iL1Tau));
          out_l1tEmuTowerIPhi->push_back(l1tEmuTowerIPhi->at(iL1Tau));
          out_l1tEmuRawEt->push_back(l1tEmuRawEt->at(iL1Tau));
          out_l1tEmuIsoEt->push_back(l1tEmuIsoEt->at(iL1Tau));

          for(Int_t ieta = 0 ; ieta < NbinsIEta ; ++ieta)
            {
              if(abs(l1tEmuTowerIEta->at(iL1Tau))>=33)
                {
                  out_compressedieta->push_back(NbinsIEta-2);
                  break;
                }
              else if(abs(l1tEmuTowerIEta->at(iL1Tau))>= hardcodedIetaBins[ieta] && abs(l1tEmuTowerIEta->at(iL1Tau)) < hardcodedIetaBins[ieta+1])
                {
                  out_compressedieta->push_back(ieta);
                  break;
                }
            }

          for(Int_t ie = 0 ; ie < NbinsIEt2 ; ++ie)
            {
              if(l1tEmuRawEt->at(iL1Tau)>=255)
                {
                  out_compressedE->push_back(NbinsIEt2-2);
                  break;
                }
              else if(l1tEmuRawEt->at(iL1Tau)>= hardcodedIetBins2[ie] && l1tEmuRawEt->at(iL1Tau) < hardcodedIetBins2[ie+1])
                {
                  out_compressedE->push_back(ie);
                  break;
                }
            }

          for(Int_t ie = 0 ; ie < NbinsIEt ; ++ie)
            {
              if(l1tEmuRawEt->at(iL1Tau)>=255)
                {
                  out_supercompressedE->push_back(NbinsIEt-2);
                  break;
                }
              else if(l1tEmuRawEt->at(iL1Tau)>= hardcodedIetBins[ie] && l1tEmuRawEt->at(iL1Tau) < hardcodedIetBins[ie+1])
                {
                  out_supercompressedE->push_back(ie);
                  break;
                }
            }
          
          for(Int_t inTT = 0 ; inTT < NbinsnTT2 ; ++inTT)
            {
              if(l1tEmuNTT->at(iL1Tau)>=255)
                {
                  out_compressednTT->push_back(NbinsnTT-2);
                  break;
                }
              else if(l1tEmuNTT->at(iL1Tau)>= hardcodednTTBins2[inTT] && l1tEmuNTT->at(iL1Tau) < hardcodednTTBins2[inTT+1])
                {
                  out_compressednTT->push_back(inTT);
                  break;
                }
            }
          
          for(Int_t inTT = 0 ; inTT < NbinsnTT ; ++inTT)
            {
              if(l1tEmuNTT->at(iL1Tau)>=255)
                {
                  out_supercompressednTT->push_back(NbinsnTT-2);
                  break;
                }
              else if(l1tEmuNTT->at(iL1Tau)>= hardcodednTTBins[inTT] && l1tEmuNTT->at(iL1Tau) < hardcodednTTBins[inTT+1])
                {
                  out_supercompressednTT->push_back(inTT);
                  break;
                }
            }

          if(!out_l1tEmuIsMerged->at(iL1Tau))
            {
              Float_t CalibConstant = h_LUT_isMerged0->GetBinContent(abs(out_compressedieta->at(iL1Tau))+1,out_compressedE->at(iL1Tau)+1,out_l1tEmuHasEM->at(iL1Tau)+1);
              if(CalibConstant>calibThr) CalibConstant = calibThr;
              int thr = int(round(CalibConstant/4.*1024))*2;
              int calibPtInt = ((out_l1tEmuRawEt->at(iL1Tau)*thr)>>9);
              // cout<<"int = "<<calibPtInt/2.<<endl;
              // cout<<"float = "<<CalibConstant*out_l1tEmuRawEt->at(iL1Tau)/2.<<endl;
              // if(CalibConstant>1.3) CalibConstant = 1.3;
              out_CalibPt->push_back(calibPtInt/2.);
              // out_CalibPt->push_back(CalibConstant*out_l1tEmuRawEt->at(iL1Tau)/2.);
            }
          else
            {
              Float_t CalibConstant = h_LUT_isMerged1->GetBinContent(abs(out_compressedieta->at(iL1Tau))+1,out_compressedE->at(iL1Tau)+1,out_l1tEmuHasEM->at(iL1Tau)+1);
              if(CalibConstant>calibThr) CalibConstant = calibThr;
              int thr = int(round(CalibConstant/4.*1024))*2;
              int calibPtInt = ((out_l1tEmuRawEt->at(iL1Tau)*thr)>>9);
              // if(CalibConstant>1.3) CalibConstant = 1.3;
              out_CalibPt->push_back(calibPtInt/2.);
              // out_CalibPt->push_back(CalibConstant*out_l1tEmuRawEt->at(iL1Tau)/2.);
            }

          short int symmShape = getSymmShape (out_l1tEmuQual->at(iL1Tau), remap);     

          histo_symmShape->Fill(symmShape);

          if (out_CalibPt->at(iL1Tau) >= 20 && out_CalibPt->at(iL1Tau) < 30)
            {
              out_L1Tau_PassShapeVeto->push_back((find(veto_set_20.begin(), veto_set_20.end(), symmShape) == veto_set_20.end() ? true : false));
            }
          else if (out_CalibPt->at(iL1Tau) >= 30 && out_CalibPt->at(iL1Tau) < 40)
            {
              out_L1Tau_PassShapeVeto->push_back((find(veto_set_30.begin(), veto_set_30.end(), symmShape) == veto_set_30.end() ? true : false));
            }
          else if (out_CalibPt->at(iL1Tau) >= 40 && out_CalibPt->at(iL1Tau) < 50)
            {
              out_L1Tau_PassShapeVeto->push_back((find(veto_set_40.begin(), veto_set_40.end(), symmShape) == veto_set_40.end() ? true : false));
            }
          else out_L1Tau_PassShapeVeto->push_back(true);      

        }

 
      outTree.Fill();
    }

  outTree.Write();

  histo_shape->Write();
  histo_symmShape->Write();
  
}

