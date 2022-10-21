#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <iostream>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TProfile.h>
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
//#include "myIncludes.h"

using namespace std;

void CompareIsoEnergies(TString FileName1 = "", TString FileName2 = "", TString FileName3 = "", TString FileName4 = "")
{

  // File1 is set to be a Matched file with an outTreeForCalibration inside
  // File2 is set to be a ZeroBias file with an ZeroBias/ZeroBias inside
  // File3 is set to be a ZeroBias file with an ZeroBias/ZeroBias inside
  // File4 is set to be a Matched file with an outTreeForCalibration inside


  TFile File1(FileName1.Data(),"READ");
  TTree * Tree1 = (TTree*)File1.Get("outTreeForCalibration");
  
  // int MC_run;
  // int MC_event;
  // int MC_lumi;
  // float MC_tauPt;
  // float MC_tauEta;
  // float MC_tauPhi;
  // bool MC_hasTriggerMuonType;
  // bool MC_hasTriggerTauType;
  // bool MC_isMatched;
  // bool MC_isOS;
  // int MC_foundJet;
  // float MC_muonPt;
  // float MC_muonEta;
  // float MC_muonPhi;
  // int MC_Nvtx;
  // int MC_l1tEmuQual= 0;
  float MC_l1tEmuPt= 0;
  float MC_l1tEmuEta= 0;
  // float MC_l1tEmuPhi= 0;
  // int MC_l1tEmuIso= 0;
  // int MC_l1tEmuNTT= 0;
  // int MC_l1tEmuHasEM= 0;
  int MC_l1tEmuIsMerged= 0;
  // int MC_l1tEmuTowerIEta= 0;
  // int MC_l1tEmuTowerIPhi= 0;
  // int MC_l1tEmuRawEt= 0;
  int MC_l1tEmuIsoEt= 0; 

  // Tree1->SetBranchAddress("RunNumber",&MC_run);
  // Tree1->SetBranchAddress("EventNumber",&MC_event);
  // Tree1->SetBranchAddress("lumi",&MC_lumi);
  // Tree1->SetBranchAddress("tauPt",&MC_tauPt);
  // Tree1->SetBranchAddress("tauEta",&MC_tauEta);
  // Tree1->SetBranchAddress("tauPhi",&MC_tauPhi);
  // Tree1->SetBranchAddress("hasTriggerMuonType",&MC_hasTriggerMuonType);
  // Tree1->SetBranchAddress("hasTriggerTauType",&MC_hasTriggerTauType);
  // Tree1->SetBranchAddress("isMatched",&MC_isMatched);
  // Tree1->SetBranchAddress("isOS",&MC_isOS);
  // Tree1->SetBranchAddress("foundJet",&MC_foundJet);
  // Tree1->SetBranchAddress("muonPt",&MC_muonPt);
  // Tree1->SetBranchAddress("muonEta",&MC_muonEta);
  // Tree1->SetBranchAddress("muonPhi",&MC_muonPhi);
  // Tree1->SetBranchAddress("Nvtx",&MC_Nvtx);
  // Tree1->SetBranchAddress("l1tEmuQual",&MC_l1tEmuQual);
  Tree1->SetBranchAddress("L1Tau_pt",&MC_l1tEmuPt);
  Tree1->SetBranchAddress("L1Tau_eta",&MC_l1tEmuEta);
  // Tree1->SetBranchAddress("l1tEmuPhi",&MC_l1tEmuPhi);
  // Tree1->SetBranchAddress("l1tEmuIso",&MC_l1tEmuIso);
  // Tree1->SetBranchAddress("l1tEmuNTT",&MC_l1tEmuNTT);
  // Tree1->SetBranchAddress("l1tEmuHasEM",&MC_l1tEmuHasEM);
  Tree1->SetBranchAddress("L1Tau_isMerged",&MC_l1tEmuIsMerged);
  // Tree1->SetBranchAddress("l1tEmuTowerIEta",&MC_l1tEmuTowerIEta);
  // Tree1->SetBranchAddress("l1tEmuTowerIPhi",&MC_l1tEmuTowerIPhi);
  // Tree1->SetBranchAddress("l1tEmuRawEt",&MC_l1tEmuRawEt);
  Tree1->SetBranchAddress("L1Tau_Iso",&MC_l1tEmuIsoEt);

  TH1F* pt1 = new TH1F("pt1","pt1",40,20.,100.);
  TH1F* Iso1 = new TH1F("Iso1","Iso1",30,0.,15.);
  TH1F* Iso1_NotMerged = new TH1F("Iso1_NotMerged","Iso1_NotMerged",30,0.,15.);
  TH1F* Iso1_Merged = new TH1F("Iso1_Merged","Iso1_Merged",30,0.,15.);
  
  TH1F* isMerged1 = new TH1F("isMerged1","isMerged1",2,0.,2.);
  TH1F* isMerged1_20_25 = new TH1F("isMerged1_20_25","isMerged1_20_25",2,0.,2.);
  TH1F* isMerged1_25_30 = new TH1F("isMerged1_25_30","isMerged1_25_30",2,0.,2.);
  TH1F* isMerged1_30_35 = new TH1F("isMerged1_30_35","isMerged1_30_35",2,0.,2.);
  TH1F* isMerged1_35_40 = new TH1F("isMerged1_35_40","isMerged1_35_40",2,0.,2.);
  TH1F* isMerged1_40_45 = new TH1F("isMerged1_40_45","isMerged1_40_45",2,0.,2.);
  TH1F* Merged1_vs_pt = new TH1F("Merged1_vs_pt","Merged1_vs_pt",20,20.,100.);
  TH1F* FractionOfMerged1_vs_pt = new TH1F("FractionOfMerged1_vs_pt","FractionOfMerged1_vs_pt",20,20.,100.);
  TH2F* Iso1_vs_pt = new TH2F("Iso1_vs_pt","Iso1_vs_pt",20,20.,100.,30,0.,15.);

  for(UInt_t i = 0 ; i < Tree1->GetEntries() ; ++i)
    {
      Tree1->GetEntry(i);
      if(MC_l1tEmuPt>=20. && MC_l1tEmuPt<30. && fabs(MC_l1tEmuEta)<2.1)
	{
	  Iso1->Fill(MC_l1tEmuIsoEt/2.);
	  if(MC_l1tEmuIsMerged) Iso1_NotMerged->Fill(MC_l1tEmuIsoEt/2.);
	  else Iso1_Merged->Fill(MC_l1tEmuIsoEt/2.);
	}
      if(MC_l1tEmuPt>=20. && fabs(MC_l1tEmuEta)<2.1)
	{
	  pt1->Fill(MC_l1tEmuPt);
	  isMerged1->Fill(MC_l1tEmuIsMerged);
	  if(MC_l1tEmuPt>=20. && MC_l1tEmuPt<25.) isMerged1_20_25->Fill(MC_l1tEmuIsMerged);
	  if(MC_l1tEmuPt>=25. && MC_l1tEmuPt<30.) isMerged1_25_30->Fill(MC_l1tEmuIsMerged);
	  if(MC_l1tEmuPt>=30. && MC_l1tEmuPt<35.) isMerged1_30_35->Fill(MC_l1tEmuIsMerged);
	  if(MC_l1tEmuPt>=35. && MC_l1tEmuPt<40.) isMerged1_35_40->Fill(MC_l1tEmuIsMerged);
	  if(MC_l1tEmuPt>=40. && MC_l1tEmuPt<45.) isMerged1_40_45->Fill(MC_l1tEmuIsMerged);

	  FractionOfMerged1_vs_pt->Fill(MC_l1tEmuPt);
	  if(MC_l1tEmuIsMerged==1) Merged1_vs_pt->Fill(MC_l1tEmuPt);

	  Iso1_vs_pt->Fill(MC_l1tEmuPt,MC_l1tEmuIsoEt/2.);
	}
      
    }

  // ##############################################################################################
  // ##############################################################################################

  TFile File4(FileName4.Data(),"READ");
  TTree * Tree4 = (TTree*)File4.Get("outTreeForCalibration");
  
  MC_l1tEmuPt= 0;
  MC_l1tEmuEta= 0;
  MC_l1tEmuIsMerged= 0;
  MC_l1tEmuIsoEt= 0;

  Tree4->SetBranchAddress("L1Tau_pt",&MC_l1tEmuPt);
  Tree4->SetBranchAddress("L1Tau_eta",&MC_l1tEmuEta);
  Tree4->SetBranchAddress("L1Tau_isMerged",&MC_l1tEmuIsMerged);
  Tree4->SetBranchAddress("L1Tau_Iso",&MC_l1tEmuIsoEt);

  TH1F* pt4 = new TH1F("pt4","pt4",40,20.,100.);
  TH1F* Iso4 = new TH1F("Iso4","Iso4",30,0.,15.);
  TH1F* Iso4_NotMerged = new TH1F("Iso4_NotMerged","Iso4_NotMerged",30,0.,15.);
  TH1F* Iso4_Merged = new TH1F("Iso4_Merged","Iso4_Merged",30,0.,15.);
  
  TH1F* isMerged4 = new TH1F("isMerged4","isMerged4",2,0.,2.);
  TH1F* isMerged4_20_25 = new TH1F("isMerged4_20_25","isMerged4_20_25",2,0.,2.);
  TH1F* isMerged4_25_30 = new TH1F("isMerged4_25_30","isMerged4_25_30",2,0.,2.);
  TH1F* isMerged4_30_35 = new TH1F("isMerged4_30_35","isMerged4_30_35",2,0.,2.);
  TH1F* isMerged4_35_40 = new TH1F("isMerged4_35_40","isMerged4_35_40",2,0.,2.);
  TH1F* isMerged4_40_45 = new TH1F("isMerged4_40_45","isMerged4_40_45",2,0.,2.);
  TH1F* Merged4_vs_pt = new TH1F("Merged4_vs_pt","Merged4_vs_pt",20,20.,100.);
  TH1F* FractionOfMerged4_vs_pt = new TH1F("FractionOfMerged4_vs_pt","FractionOfMerged4_vs_pt",20,20.,100.);
  TH2F* Iso4_vs_pt = new TH2F("Iso4_vs_pt","Iso4_vs_pt",20,20.,100.,30,0.,15.);

  for(UInt_t i = 0 ; i < Tree4->GetEntries() ; ++i)
    {
      Tree4->GetEntry(i);
      if(MC_l1tEmuPt>=20. && MC_l1tEmuPt<30. && fabs(MC_l1tEmuEta)<2.1)
  {
    Iso4->Fill(MC_l1tEmuIsoEt/2.);
    if(MC_l1tEmuIsMerged) Iso4_NotMerged->Fill(MC_l1tEmuIsoEt/2.);
    else Iso4_Merged->Fill(MC_l1tEmuIsoEt/2.);
  }
      if(MC_l1tEmuPt>=20. && fabs(MC_l1tEmuEta)<2.1)
  {
    pt4->Fill(MC_l1tEmuPt);
    isMerged4->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=20. && MC_l1tEmuPt<25.) isMerged4_20_25->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=25. && MC_l1tEmuPt<30.) isMerged4_25_30->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=30. && MC_l1tEmuPt<35.) isMerged4_30_35->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=35. && MC_l1tEmuPt<40.) isMerged4_35_40->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=40. && MC_l1tEmuPt<45.) isMerged4_40_45->Fill(MC_l1tEmuIsMerged);

    FractionOfMerged4_vs_pt->Fill(MC_l1tEmuPt);
    if(MC_l1tEmuIsMerged==1) Merged4_vs_pt->Fill(MC_l1tEmuPt);

    Iso4_vs_pt->Fill(MC_l1tEmuPt,MC_l1tEmuIsoEt/2.);
  }
      
    }

  // ##############################################################################################
  // ##############################################################################################

  TFile File2(FileName2.Data(),"READ");
  TTree * Tree2 = (TTree*)File2.Get("outTreeForCalibration");
  
  MC_l1tEmuPt= 0;
  MC_l1tEmuEta= 0;
  MC_l1tEmuIsMerged= 0;
  MC_l1tEmuIsoEt= 0;

  Tree2->SetBranchAddress("L1Tau_pt",&MC_l1tEmuPt);
  Tree2->SetBranchAddress("L1Tau_eta",&MC_l1tEmuEta);
  Tree2->SetBranchAddress("L1Tau_isMerged",&MC_l1tEmuIsMerged);
  Tree2->SetBranchAddress("L1Tau_Iso",&MC_l1tEmuIsoEt);

  TH1F* pt2 = new TH1F("pt2","pt2",40,20.,100.);
  TH1F* Iso2 = new TH1F("Iso2","Iso2",30,0.,15.);
  TH1F* Iso2_NotMerged = new TH1F("Iso2_NotMerged","Iso2_NotMerged",30,0.,15.);
  TH1F* Iso2_Merged = new TH1F("Iso2_Merged","Iso2_Merged",30,0.,15.);
  
  TH1F* isMerged2 = new TH1F("isMerged2","isMerged2",2,0.,2.);
  TH1F* isMerged2_20_25 = new TH1F("isMerged2_20_25","isMerged2_20_25",2,0.,2.);
  TH1F* isMerged2_25_30 = new TH1F("isMerged2_25_30","isMerged2_25_30",2,0.,2.);
  TH1F* isMerged2_30_35 = new TH1F("isMerged2_30_35","isMerged2_30_35",2,0.,2.);
  TH1F* isMerged2_35_40 = new TH1F("isMerged2_35_40","isMerged2_35_40",2,0.,2.);
  TH1F* isMerged2_40_45 = new TH1F("isMerged2_40_45","isMerged2_40_45",2,0.,2.);
  TH1F* Merged2_vs_pt = new TH1F("Merged2_vs_pt","Merged2_vs_pt",20,20.,100.);
  TH1F* FractionOfMerged2_vs_pt = new TH1F("FractionOfMerged2_vs_pt","FractionOfMerged2_vs_pt",20,20.,100.);
  TH2F* Iso2_vs_pt = new TH2F("Iso2_vs_pt","Iso2_vs_pt",20,20.,100.,30,0.,15.);

  for(UInt_t i = 0 ; i < Tree2->GetEntries() ; ++i)
    {
      Tree2->GetEntry(i);
      if(MC_l1tEmuPt>=20. && MC_l1tEmuPt<30. && fabs(MC_l1tEmuEta)<2.1)
  {
    Iso2->Fill(MC_l1tEmuIsoEt/2.);
    if(MC_l1tEmuIsMerged) Iso2_NotMerged->Fill(MC_l1tEmuIsoEt/2.);
    else Iso2_Merged->Fill(MC_l1tEmuIsoEt/2.);
  }
      if(MC_l1tEmuPt>=20. && fabs(MC_l1tEmuEta)<2.1)
  {
    pt2->Fill(MC_l1tEmuPt);
    isMerged2->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=20. && MC_l1tEmuPt<25.) isMerged2_20_25->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=25. && MC_l1tEmuPt<30.) isMerged2_25_30->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=30. && MC_l1tEmuPt<35.) isMerged2_30_35->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=35. && MC_l1tEmuPt<40.) isMerged2_35_40->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=40. && MC_l1tEmuPt<45.) isMerged2_40_45->Fill(MC_l1tEmuIsMerged);

    FractionOfMerged2_vs_pt->Fill(MC_l1tEmuPt);
    if(MC_l1tEmuIsMerged==1) Merged2_vs_pt->Fill(MC_l1tEmuPt);

    Iso2_vs_pt->Fill(MC_l1tEmuPt,MC_l1tEmuIsoEt/2.);
  }
      
    }

  // ##############################################################################################
  // ##############################################################################################

  TFile File3(FileName3.Data(),"READ");
  TTree * Tree3 = (TTree*)File3.Get("outTreeForCalibration");
  
  MC_l1tEmuPt= 0;
  MC_l1tEmuEta= 0;
  MC_l1tEmuIsMerged= 0;
  MC_l1tEmuIsoEt= 0;

  Tree3->SetBranchAddress("L1Tau_pt",&MC_l1tEmuPt);
  Tree3->SetBranchAddress("L1Tau_eta",&MC_l1tEmuEta);
  Tree3->SetBranchAddress("L1Tau_isMerged",&MC_l1tEmuIsMerged);
  Tree3->SetBranchAddress("L1Tau_Iso",&MC_l1tEmuIsoEt);

  TH1F* pt3 = new TH1F("pt3","pt3",40,20.,100.);
  TH1F* Iso3 = new TH1F("Iso3","Iso3",30,0.,15.);
  TH1F* Iso3_NotMerged = new TH1F("Iso3_NotMerged","Iso3_NotMerged",30,0.,15.);
  TH1F* Iso3_Merged = new TH1F("Iso3_Merged","Iso3_Merged",30,0.,15.);
  
  TH1F* isMerged3 = new TH1F("isMerged3","isMerged3",2,0.,2.);
  TH1F* isMerged3_20_25 = new TH1F("isMerged3_20_25","isMerged3_20_25",2,0.,2.);
  TH1F* isMerged3_25_30 = new TH1F("isMerged3_25_30","isMerged3_25_30",2,0.,2.);
  TH1F* isMerged3_30_35 = new TH1F("isMerged3_30_35","isMerged3_30_35",2,0.,2.);
  TH1F* isMerged3_35_40 = new TH1F("isMerged3_35_30","isMerged3_35_30",2,0.,2.);
  TH1F* isMerged3_40_45 = new TH1F("isMerged3_40_45","isMerged3_40_45",2,0.,2.);
  TH1F* Merged3_vs_pt = new TH1F("Merged3_vs_pt","Merged3_vs_pt",20,20.,100.);
  TH1F* FractionOfMerged3_vs_pt = new TH1F("FractionOfMerged3_vs_pt","FractionOfMerged3_vs_pt",20,20.,100.);
  TH2F* Iso3_vs_pt = new TH2F("Iso3_vs_pt","Iso3_vs_pt",20,20.,100.,30,0.,15.);

  for(UInt_t i = 0 ; i < Tree3->GetEntries() ; ++i)
    {
      Tree3->GetEntry(i);
      if(MC_l1tEmuPt>=20. && MC_l1tEmuPt<30. && fabs(MC_l1tEmuEta)<2.1)
  {
    Iso3->Fill(MC_l1tEmuIsoEt/2.);
    if(MC_l1tEmuIsMerged) Iso3_NotMerged->Fill(MC_l1tEmuIsoEt/2.);
    else Iso3_Merged->Fill(MC_l1tEmuIsoEt/2.);
  }
      if(MC_l1tEmuPt>=20. && fabs(MC_l1tEmuEta)<2.1)
  {
    pt3->Fill(MC_l1tEmuPt);
    isMerged3->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=20. && MC_l1tEmuPt<25.) isMerged3_20_25->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=25. && MC_l1tEmuPt<30.) isMerged3_25_30->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=30. && MC_l1tEmuPt<35.) isMerged3_30_35->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=35. && MC_l1tEmuPt<40.) isMerged3_35_40->Fill(MC_l1tEmuIsMerged);
    if(MC_l1tEmuPt>=40. && MC_l1tEmuPt<45.) isMerged3_40_45->Fill(MC_l1tEmuIsMerged);

    FractionOfMerged3_vs_pt->Fill(MC_l1tEmuPt);
    if(MC_l1tEmuIsMerged==1) Merged3_vs_pt->Fill(MC_l1tEmuPt);

    Iso3_vs_pt->Fill(MC_l1tEmuPt,MC_l1tEmuIsoEt/2.);
  }
      
    }

 //  // ##############################################################################################
 //  // ##############################################################################################

 //  TFile File2(FileName2.Data(),"READ");
 //  TTree * Tree2 = (TTree*)File2.Get("ZeroBias/ZeroBias");

 //  int ZeroBias_run;
 //  unsigned long long int ZeroBias_event;
 //  int ZeroBias_lumi;
 //  std::vector<int>* ZeroBias_l1tEmuQual= 0;
 //  std::vector<float>* ZeroBias_l1tEmuPt= 0;
 //  std::vector<float>* ZeroBias_l1tEmuEta= 0;
 //  std::vector<float>* ZeroBias_l1tEmuPhi= 0;
 //  std::vector<int>* ZeroBias_l1tEmuIso= 0;
 //  std::vector<int>* ZeroBias_l1tEmuNTT= 0;
 //  std::vector<int>* ZeroBias_l1tEmuHasEM= 0;
 //  std::vector<int>* ZeroBias_l1tEmuIsMerged= 0;
 //  std::vector<int>* ZeroBias_l1tEmuTowerIEta= 0;
 //  std::vector<int>* ZeroBias_l1tEmuTowerIPhi= 0;
 //  std::vector<int>* ZeroBias_l1tEmuRawEt= 0;
 //  std::vector<int>* ZeroBias_l1tEmuIsoEt= 0; 

 //  Tree2->SetBranchAddress("RunNumber",&ZeroBias_run);
 //  Tree2->SetBranchAddress("EventNumber",&ZeroBias_event);
 //  Tree2->SetBranchAddress("lumi",&ZeroBias_lumi);
 //  Tree2->SetBranchAddress("l1tEmuQual",&ZeroBias_l1tEmuQual);
 //  Tree2->SetBranchAddress("l1tEmuPt",&ZeroBias_l1tEmuPt);
 //  Tree2->SetBranchAddress("l1tEmuEta",&ZeroBias_l1tEmuEta);
 //  Tree2->SetBranchAddress("l1tEmuPhi",&ZeroBias_l1tEmuPhi);
 //  Tree2->SetBranchAddress("l1tEmuIso",&ZeroBias_l1tEmuIso);
 //  Tree2->SetBranchAddress("l1tEmuNTT",&ZeroBias_l1tEmuNTT);
 //  // Tree2->SetBranchAddress("l1tEmuHasEM",&ZeroBias_l1tEmuHasEM);
 //  Tree2->SetBranchAddress("l1tEmuIsMerged",&ZeroBias_l1tEmuIsMerged);
 //  Tree2->SetBranchAddress("l1tEmuTowerIEta",&ZeroBias_l1tEmuTowerIEta);
 //  Tree2->SetBranchAddress("l1tEmuTowerIPhi",&ZeroBias_l1tEmuTowerIPhi);
 //  Tree2->SetBranchAddress("l1tEmuRawEt",&ZeroBias_l1tEmuRawEt);
 //  Tree2->SetBranchAddress("l1tEmuIsoEt",&ZeroBias_l1tEmuIsoEt);

 //  TH1F* pt2 = new TH1F("pt2","pt2",40,20.,100.);
 //  TH1F* Iso2 = new TH1F("Iso2","Iso2",30,0.,15.);
 //  TH1F* Iso2_NotMerged = new TH1F("Iso2_NotMerged","Iso2_NotMerged",30,0.,15.);
 //  TH1F* Iso2_Merged = new TH1F("Iso2_Merged","Iso2_Merged",30,0.,15.);

 //  TH1F* isMerged2 = new TH1F("isMerged2","isMerged2",2,0.,2.);
 //  TH1F* isMerged2_20_25 = new TH1F("isMerged2_20_25","isMerged2_20_25",2,0.,2.);
 //  TH1F* isMerged2_25_30 = new TH1F("isMerged2_25_30","isMerged2_25_30",2,0.,2.);
 //  TH1F* isMerged2_30_35 = new TH1F("isMerged2_30_35","isMerged2_30_35",2,0.,2.);
 //  TH1F* isMerged2_35_40 = new TH1F("isMerged2_35_40","isMerged2_35_40",2,0.,2.);
 //  TH1F* isMerged2_40_45 = new TH1F("isMerged2_40_45","isMerged2_40_45",2,0.,2.);
 //  TH1F* Merged2_vs_pt = new TH1F("Merged2_vs_pt","Merged2_vs_pt",20,20.,100.);
 //  TH1F* FractionOfMerged2_vs_pt = new TH1F("FractionOfMerged2_vs_pt","FractionOfMerged2_vs_pt",20,20.,100.);
 //  TH2F* Iso2_vs_pt = new TH2F("Iso2_vs_pt","Iso2_vs_pt",20,20.,100.,30,0.,15.);

 //  for(UInt_t i = 0 ; i < Tree2->GetEntries() ; ++i)
 //    {
 //      Tree2->GetEntry(i);
 //      for(UInt_t iL1Tau = 0 ; iL1Tau < ZeroBias_l1tEmuIsoEt->size() ; ++iL1Tau)
	// {
	//   if(ZeroBias_l1tEmuPt->at(iL1Tau)>=20. && ZeroBias_l1tEmuPt->at(iL1Tau)<30. && fabs(ZeroBias_l1tEmuEta->at(iL1Tau))<2.1)
	//     {
	//       Iso2->Fill(ZeroBias_l1tEmuIsoEt->at(iL1Tau)/2.);
	//       if(ZeroBias_l1tEmuIsMerged->at(iL1Tau)) Iso2_NotMerged->Fill(ZeroBias_l1tEmuIsoEt->at(iL1Tau)/2.);
	//       else Iso2_Merged->Fill(ZeroBias_l1tEmuIsoEt->at(iL1Tau)/2.);
	//     }
	//   if(ZeroBias_l1tEmuPt->at(iL1Tau)>=20. && fabs(ZeroBias_l1tEmuEta->at(iL1Tau))<2.1)
	//     {
	//       pt2->Fill(ZeroBias_l1tEmuPt->at(iL1Tau));
	//       isMerged2->Fill(ZeroBias_l1tEmuIsMerged->at(iL1Tau));
	//       if(ZeroBias_l1tEmuPt->at(iL1Tau)>=20. && ZeroBias_l1tEmuPt->at(iL1Tau)<25.) isMerged2_20_25->Fill(ZeroBias_l1tEmuIsMerged->at(iL1Tau));
	//       if(ZeroBias_l1tEmuPt->at(iL1Tau)>=25. && ZeroBias_l1tEmuPt->at(iL1Tau)<30.) isMerged2_25_30->Fill(ZeroBias_l1tEmuIsMerged->at(iL1Tau));
	//       if(ZeroBias_l1tEmuPt->at(iL1Tau)>=30. && ZeroBias_l1tEmuPt->at(iL1Tau)<35.) isMerged2_30_35->Fill(ZeroBias_l1tEmuIsMerged->at(iL1Tau));
	//       if(ZeroBias_l1tEmuPt->at(iL1Tau)>=35. && ZeroBias_l1tEmuPt->at(iL1Tau)<40.) isMerged2_35_40->Fill(ZeroBias_l1tEmuIsMerged->at(iL1Tau));
	//       if(ZeroBias_l1tEmuPt->at(iL1Tau)>=40. && ZeroBias_l1tEmuPt->at(iL1Tau)<45.) isMerged2_40_45->Fill(ZeroBias_l1tEmuIsMerged->at(iL1Tau));

	//       FractionOfMerged2_vs_pt->Fill(ZeroBias_l1tEmuPt->at(iL1Tau));
	//       if(ZeroBias_l1tEmuIsMerged->at(iL1Tau)==1) Merged2_vs_pt->Fill(ZeroBias_l1tEmuPt->at(iL1Tau));

	//       Iso2_vs_pt->Fill(ZeroBias_l1tEmuPt->at(iL1Tau),ZeroBias_l1tEmuIsoEt->at(iL1Tau)/2.);

	//     }
	// }
 //    }


 //  // ##############################################################################################
 //  // ##############################################################################################


 //  TFile File3(FileName3.Data(),"READ");
 //  TTree * Tree3 = (TTree*)File3.Get("ZeroBias/ZeroBias");

 //  ZeroBias_run=0;
 //  ZeroBias_event=0;
 //  ZeroBias_lumi=0;

 //  ZeroBias_l1tEmuQual = 0;
 //  ZeroBias_l1tEmuPt = 0;
 //  ZeroBias_l1tEmuEta = 0;
 //  ZeroBias_l1tEmuPhi = 0;
 //  ZeroBias_l1tEmuIso = 0;
 //  ZeroBias_l1tEmuNTT = 0;
 //  ZeroBias_l1tEmuHasEM = 0;
 //  ZeroBias_l1tEmuIsMerged = 0;
 //  ZeroBias_l1tEmuTowerIEta = 0;
 //  ZeroBias_l1tEmuTowerIPhi = 0;
 //  ZeroBias_l1tEmuRawEt = 0;
 //  ZeroBias_l1tEmuIsoEt = 0;

 //  Tree3->SetBranchAddress("RunNumber",&ZeroBias_run);
 //  Tree3->SetBranchAddress("EventNumber",&ZeroBias_event);
 //  Tree3->SetBranchAddress("lumi",&ZeroBias_lumi);
 //  Tree3->SetBranchAddress("l1tEmuQual",&ZeroBias_l1tEmuQual);
 //  Tree3->SetBranchAddress("l1tEmuPt",&ZeroBias_l1tEmuPt);
 //  Tree3->SetBranchAddress("l1tEmuEta",&ZeroBias_l1tEmuEta);
 //  Tree3->SetBranchAddress("l1tEmuPhi",&ZeroBias_l1tEmuPhi);
 //  Tree3->SetBranchAddress("l1tEmuIso",&ZeroBias_l1tEmuIso);
 //  Tree3->SetBranchAddress("l1tEmuNTT",&ZeroBias_l1tEmuNTT);
 //  // Tree3->SetBranchAddress("l1tEmuHasEM",&ZeroBias_l1tEmuHasEM);
 //  Tree3->SetBranchAddress("l1tEmuIsMerged",&ZeroBias_l1tEmuIsMerged);
 //  Tree3->SetBranchAddress("l1tEmuTowerIEta",&ZeroBias_l1tEmuTowerIEta);
 //  Tree3->SetBranchAddress("l1tEmuTowerIPhi",&ZeroBias_l1tEmuTowerIPhi);
 //  Tree3->SetBranchAddress("l1tEmuRawEt",&ZeroBias_l1tEmuRawEt);
 //  Tree3->SetBranchAddress("l1tEmuIsoEt",&ZeroBias_l1tEmuIsoEt);

 //  TH1F* pt3 = new TH1F("pt3","pt3",40,20.,100.);
 //  TH1F* Iso3 = new TH1F("Iso3","Iso3",30,0.,15.);
 //  TH1F* Iso3_NotMerged = new TH1F("Iso3_NotMerged","Iso3_NotMerged",30,0.,15.);
 //  TH1F* Iso3_Merged = new TH1F("Iso3_Merged","Iso3_Merged",30,0.,15.);

 //  TH1F* isMerged3 = new TH1F("isMerged3","isMerged3",2,0.,2.);
 //  TH1F* isMerged3_20_25 = new TH1F("isMerged3_20_25","isMerged3_20_25",2,0.,2.);
 //  TH1F* isMerged3_25_30 = new TH1F("isMerged3_25_30","isMerged3_25_30",2,0.,2.);
 //  TH1F* isMerged3_30_35 = new TH1F("isMerged3_30_35","isMerged3_30_35",2,0.,2.);
 //  TH1F* isMerged3_35_40 = new TH1F("isMerged3_35_40","isMerged3_35_40",2,0.,2.);
 //  TH1F* isMerged3_40_45 = new TH1F("isMerged3_40_45","isMerged3_40_45",2,0.,2.);
 //  TH1F* Merged3_vs_pt = new TH1F("Merged3_vs_pt","Merged3_vs_pt",20,20.,100.);
 //  TH1F* FractionOfMerged3_vs_pt = new TH1F("FractionOfMerged3_vs_pt","FractionOfMerged3_vs_pt",20,20.,100.);
 //  TH2F* Iso3_vs_pt = new TH2F("Iso3_vs_pt","Iso3_vs_pt",20,20.,100.,30,0.,15.);

 //  for(UInt_t i = 0 ; i < Tree3->GetEntries() ; ++i)
 //    {
 //      Tree3->GetEntry(i);
 //      for(UInt_t iL1Tau = 0 ; iL1Tau < ZeroBias_l1tEmuIsoEt->size() ; ++iL1Tau)
 //  {
 //    if(ZeroBias_l1tEmuPt->at(iL1Tau)>=20. && ZeroBias_l1tEmuPt->at(iL1Tau)<30. && fabs(ZeroBias_l1tEmuEta->at(iL1Tau))<2.1)
 //      {
 //        Iso3->Fill(ZeroBias_l1tEmuIsoEt->at(iL1Tau)/2.);
 //        if(ZeroBias_l1tEmuIsMerged->at(iL1Tau)) Iso3_NotMerged->Fill(ZeroBias_l1tEmuIsoEt->at(iL1Tau)/2.);
 //        else Iso3_Merged->Fill(ZeroBias_l1tEmuIsoEt->at(iL1Tau)/2.);
 //      }
 //    if(ZeroBias_l1tEmuPt->at(iL1Tau)>=20. && fabs(ZeroBias_l1tEmuEta->at(iL1Tau))<2.1)
 //      {
 //        pt3->Fill(ZeroBias_l1tEmuPt->at(iL1Tau));
 //        isMerged3->Fill(ZeroBias_l1tEmuIsMerged->at(iL1Tau));
 //        if(ZeroBias_l1tEmuPt->at(iL1Tau)>=20. && ZeroBias_l1tEmuPt->at(iL1Tau)<25.) isMerged3_20_25->Fill(ZeroBias_l1tEmuIsMerged->at(iL1Tau));
 //        if(ZeroBias_l1tEmuPt->at(iL1Tau)>=25. && ZeroBias_l1tEmuPt->at(iL1Tau)<30.) isMerged3_25_30->Fill(ZeroBias_l1tEmuIsMerged->at(iL1Tau));
 //        if(ZeroBias_l1tEmuPt->at(iL1Tau)>=30. && ZeroBias_l1tEmuPt->at(iL1Tau)<35.) isMerged3_30_35->Fill(ZeroBias_l1tEmuIsMerged->at(iL1Tau));
 //        if(ZeroBias_l1tEmuPt->at(iL1Tau)>=35. && ZeroBias_l1tEmuPt->at(iL1Tau)<40.) isMerged3_35_40->Fill(ZeroBias_l1tEmuIsMerged->at(iL1Tau));
 //        if(ZeroBias_l1tEmuPt->at(iL1Tau)>=40. && ZeroBias_l1tEmuPt->at(iL1Tau)<45.) isMerged3_40_45->Fill(ZeroBias_l1tEmuIsMerged->at(iL1Tau));

 //        FractionOfMerged3_vs_pt->Fill(ZeroBias_l1tEmuPt->at(iL1Tau));
 //        if(ZeroBias_l1tEmuIsMerged->at(iL1Tau)==1) Merged3_vs_pt->Fill(ZeroBias_l1tEmuPt->at(iL1Tau));

 //        Iso3_vs_pt->Fill(ZeroBias_l1tEmuPt->at(iL1Tau),ZeroBias_l1tEmuIsoEt->at(iL1Tau)/2.);

 //      }
 //  }
 //    }

 //  // ##############################################################################################
 //  // ##############################################################################################

  TString HistogramFileName = "histograms_MC_ZeroBias.root";
 
  TFile FileHistograms(HistogramFileName.Data(),"RECREATE");
  Iso1->Write();
  Iso1_NotMerged->Write();
  Iso1_Merged->Write();
  isMerged1->Write();
  isMerged1_20_25->Write();
  isMerged1_25_30->Write();
  isMerged1_30_35->Write();
  isMerged1_35_40->Write();
  isMerged1_40_45->Write();
  Merged1_vs_pt->Divide(FractionOfMerged1_vs_pt);
  Merged1_vs_pt->Write();
  pt1->Write(),
  Iso1_vs_pt->Write();
  TProfile *AverageIso1_vs_pt = Iso1_vs_pt->ProfileX("AverageIso1_vs_pt");
  AverageIso1_vs_pt->Write();

  Iso2->Write();
  Iso2_NotMerged->Write();
  Iso2_Merged->Write();
  isMerged2->Write();
  isMerged2_20_25->Write();
  isMerged2_25_30->Write();
  isMerged2_30_35->Write();
  isMerged2_35_40->Write();
  isMerged2_40_45->Write();
  Merged2_vs_pt->Divide(FractionOfMerged2_vs_pt);
  Merged2_vs_pt->Write();
  pt2->Write();
  Iso2_vs_pt->Write();
  TProfile *AverageIso2_vs_pt = Iso2_vs_pt->ProfileX("AverageIso2_vs_pt");
  AverageIso2_vs_pt->Write();

  Iso3->Write();
  Iso3_NotMerged->Write();
  Iso3_Merged->Write();
  isMerged3->Write();
  isMerged3_20_25->Write();
  isMerged3_25_30->Write();
  isMerged3_30_35->Write();
  isMerged3_35_40->Write();
  isMerged3_40_45->Write();
  Merged3_vs_pt->Divide(FractionOfMerged3_vs_pt);
  Merged3_vs_pt->Write();
  pt3->Write();
  Iso3_vs_pt->Write();
  TProfile *AverageIso3_vs_pt = Iso3_vs_pt->ProfileX("AverageIso3_vs_pt");
  AverageIso3_vs_pt->Write();

  Iso4->Write();
  Iso4_NotMerged->Write();
  Iso4_Merged->Write();
  isMerged4->Write();
  isMerged4_20_25->Write();
  isMerged4_25_30->Write();
  isMerged4_30_35->Write();
  isMerged4_35_40->Write();
  isMerged4_40_45->Write();
  Merged4_vs_pt->Divide(FractionOfMerged4_vs_pt);
  Merged4_vs_pt->Write();
  pt4->Write();
  Iso4_vs_pt->Write();
  TProfile *AverageIso4_vs_pt = Iso4_vs_pt->ProfileX("AverageIso4_vs_pt");
  AverageIso4_vs_pt->Write();


  return;
}
