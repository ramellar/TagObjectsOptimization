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

using namespace std;

const Int_t NbinsIEta = 4+1;

const Int_t supercompressedNbinsIEt = 17+1;
const Int_t supercompressedNbinsnTT = 15+1;

const Int_t compressedNbinsIEt = 32+1;
const Int_t compressedNbinsnTT = 32+1;

const Int_t hardcodedIetaBins[NbinsIEta] = {0, 6, 12, 18, 33};
const Float_t hardcodedIetaBinsFloat[NbinsIEta] = {0, 6, 12, 18, 33};
const Double_t hardcodedIetaBinsDouble[NbinsIEta] = {0, 6, 12, 18, 33};

const Int_t hardcodedSupercompressedIetBins[supercompressedNbinsIEt] = {0, 23, 27, 31, 35, 39, 43, 47, 53, 59, 65, 73, 82, 95, 105, 120, 157, 255};
const Float_t hardcodedSupercompressedIetBinsFloat[supercompressedNbinsIEt] = {0, 23, 27, 31, 35, 39, 43, 47, 53, 59, 65, 73, 82, 95, 105, 120, 157, 255};
const Double_t hardcodedSupercompressedIetBinsDouble[supercompressedNbinsIEt] = {0, 23, 27, 31, 35, 39, 43, 47, 53, 59, 65, 73, 82, 95, 105, 120, 157, 255};

const Int_t hardcodedSupercompressednTTBins[supercompressedNbinsnTT] = {0, 26, 31, 36, 41, 46, 51, 56, 61, 66, 71, 76, 81, 91, 106, 255 };
const Float_t hardcodedSupercompressednTTBinsFloat[supercompressedNbinsnTT] = {0, 26, 31, 36, 41, 46, 51, 56, 61, 66, 71, 76, 81, 91, 106, 255 };
const Double_t hardcodedSupercompressednTTBinsDouble[supercompressedNbinsnTT] = {0, 26, 31, 36, 41, 46, 51, 56, 61, 66, 71, 76, 81, 91, 106, 255 };

const Int_t hardcodedCompressedIetBins[compressedNbinsIEt]  = {0, 15, 18, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 50, 53, 56, 59, 62, 65, 69, 73, 77, 82, 88, 95, 105, 120, 157, 255};
const Float_t hardcodedCompressedIetBinsFloat[compressedNbinsIEt] = {0, 15, 18, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 50, 53, 56, 59, 62, 65, 69, 73, 77, 82, 88, 95, 105, 120, 157, 255};
const Double_t hardcodedCompressedIetBinsDouble[compressedNbinsIEt] = {0, 15, 18, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 50, 53, 56, 59, 62, 65, 69, 73, 77, 82, 88, 95, 105, 120, 157, 255};

const Int_t hardcodedCompressednTTBins[compressedNbinsnTT] = {0, 6, 11, 16, 21, 26, 31, 36, 41, 46, 51, 56, 61, 66, 71, 76, 81, 86, 91, 96, 101, 106, 111, 116, 121, 126, 131, 136, 141, 146, 161, 161, 255};
const Float_t hardcodedCompressednTTBinsFloat[compressedNbinsnTT] = {0, 6, 11, 16, 21, 26, 31, 36, 41, 46, 51, 56, 61, 66, 71, 76, 81, 86, 91, 96, 101, 106, 111, 116, 121, 126, 131, 136, 141, 146, 161, 161, 255};
const Double_t hardcodedCompressednTTBinsDouble[compressedNbinsnTT] = {0, 6, 11, 16, 21, 26, 31, 36, 41, 46, 51, 56, 61, 66, 71, 76, 81, 86, 91, 96, 101, 106, 111, 116, 121, 126, 131, 136, 141, 146, 161, 161, 255};


bool CheckBit (short int number, int bitpos)
{
  bool res = number & (1 << bitpos);
  return res;
}

void SetBit (short int& number, int bitpos, bool val)
{
  // int isTrue = (CheckBit(number, bitpos) ? 1 : 0);
    
  // //number ^= (-isTrue ^ number) & (1 << bitpos);
  // //return;

  // number = (number & ~(1<<bitpos)) | (isTrue<<bitpos);

  if (val)
    number |= (1 << bitpos);
  else
    number &= ~(1 << bitpos);
}

// vertically flip a shape
short int reflectShape (short int shape)
{
  short int newshape = 0;
  map<int, int> remap;
  remap[0] = 3;
  remap[1] = 2;
  remap[2] = 1;
  remap[3] = 0;
  remap[4] = 6;
  remap[5] = 5;
  remap[6] = 4;


  for (int i = 0; i < 7; i++)
    {
      bool val = CheckBit(shape, i);
      SetBit (newshape, remap[i], val) ;
    }
  return newshape;
}

short int symmShape(short int shape)
{
  // truncate to first 7 bits
  shape &= 127; // first 7 bits

  int N_n = 0;
  int N_s = 0;
  int N_n_c = 0;
  int N_s_c = 0;
  int N_n_dx = 0;
  int N_s_dx = 0;

  if (CheckBit(shape, 0))
    {
      N_n++;
      N_n_c++;
    }

  if (CheckBit(shape, 1))
    {
      N_n++;
      N_n_c++;
    }

  if (CheckBit(shape, 2))
    {
      N_s++;
      N_s_c++;
    }

  if (CheckBit(shape, 3))
    {
      N_s++;
      N_s_c++;
    }

  if (CheckBit(shape, 4))
    {
      N_n++;
      N_n_dx++;
    }

  if (CheckBit(shape, 6))
    {
      N_s++;
      N_s_dx++;
    }

  ////////////

  if (N_n > N_s) return shape;
  if (N_n < N_s) return reflectShape(shape);
  if (N_n == N_s)
    {
      if (N_n_c > N_s_c) return shape;
      if (N_n_c < N_s_c) return reflectShape(shape);
      if (N_n_c == N_s_c)
  {
    if (N_n_dx >= N_s_dx) return shape;
    else return reflectShape(shape);
  }
    }

  cout << "I should not arrive here!!!" << endl;
  return shape;
}

// will encode the shape in a new short int mapped as:
// ## this is in the format:
// # D
// #BC
// #AX
// #bc
// # d
// so that vertical reflectionn == shape vertical flip
short int encodeSymm (short int shape, map<int, int>& remap)
{
  short int symm = 0;
  for (int i = 0; i < 7; i++)
    {
      if (CheckBit(shape, i))
  SetBit (symm, remap[i], true);
    }
  return symm;
}

// vertically flip the shape
short int reflect (short int shape)
{
  short int shRev = 0;
  int numbits = 7;
  for (int i = 0; i < numbits; i++)
    {
      if ((shape >> i) & 1)
  {
    shRev += (1 << (numbits-1-i));
  }
    }
  // shRev = sum(1<<(numbits-1-i) for i in range(numbits) if sh1>>i&1)
  return shRev;
}

// this is ensured to give only symm shapes
short int getSymmShape (short int shape, map<int, int>& remap)
{
  shape &= 127; // first 7 bits
  short int symmForm = encodeSymm (shape, remap);
  short int reflected = reflect (symmForm);

  // return the one having the lowest value!
  return (symmForm < reflected ? symmForm : reflected);
}


Int_t FindBinIEt(Int_t IEt)
{
  for(UInt_t i = 0 ; i < compressedNbinsIEt-1 ; ++i)
    {
      if(IEt>=157) return compressedNbinsIEt-2;
      if(IEt>= hardcodedCompressedIetBins[i] && IEt < hardcodedCompressedIetBins[i+1]) return i;
    }
  return -1;
}

Int_t FindBinnTT(Int_t nTT)
{
  for(UInt_t i = 0 ; i < compressedNbinsnTT-1 ; ++i)
    {
      if(nTT>=161) return compressedNbinsnTT-2;
      if(nTT>= hardcodedCompressednTTBins[i] && nTT < hardcodedCompressednTTBins[i+1]) return i;
    }
  return -1;
}

Int_t FindBinCorrespondenceIEt(Int_t IEt_fine)
{
  for(UInt_t i = 0 ; i < supercompressedNbinsIEt-1 ; ++i)
    {
      if(IEt_fine>=157) return supercompressedNbinsIEt-2;
      // if(IEt_fine>=120) return supercompressedNbinsIEt-2;
      if(IEt_fine>= hardcodedSupercompressedIetBins[i] && IEt_fine < hardcodedSupercompressedIetBins[i+1]) return i;
    }
  return -1;
}

Int_t FindBinCorrespondencenTT(Int_t nTT_fine)
{
  for(UInt_t i = 0 ; i < supercompressedNbinsnTT-1 ; ++i)
    {
      if(nTT_fine>=161) return supercompressedNbinsnTT-2;
      // if(nTT_fine>=201) return supercompressedNbinsnTT-2;
      //if(nTT_fine>=76) return supercompressedNbinsnTT-2;
      // if(nTT_fine>=106) return supercompressedNbinsnTT-2;
      // if(nTT_fine>=56) return supercompressedNbinsnTT-2;
      if(nTT_fine>= hardcodedSupercompressednTTBins[i] && nTT_fine < hardcodedSupercompressednTTBins[i+1]) return i;
    }
  return -1;
}

void ApplyCalibration(float calibThr = 1.7)
{
  // SHAPE COMMENT OUT
  // TFile f_shapes("/home/llr/cms/cadamuro/Tau_ShapeVeto/CMSSW_8_0_10/src/L1Trigger/L1TNtuples/ShapeVeto/shapes_signal.root");
  // // TH2F* shapes_vs_pt = (TH2F*)f_shapes.Get("hShapes_vs_pt");
  // // shapes_vs_pt->Clear();
  // TH2F* shapes_vs_eta = (TH2F*)f_shapes.Get("hShapes_vs_eta");
  // shapes_vs_eta->Clear();
  // TH1F* shapes_1D = (TH1F*) f_shapes.Get("hShapes_gt30_merg");
  // shapes_1D->Clear();
  //
  // TH2F* shapes_vs_pt = new TH2F("shapes_vs_pt","shapes_vs_pt",128,0,128,compressedNbinsIEt-1,hardcodedCompressedIetBinsDouble);
  // shapes_vs_pt->Clear();  

  TH2F* isolation_vs_pt = new TH2F("isolation_vs_pt","isolation_vs_pt",100,0,100,compressedNbinsIEt-1,hardcodedCompressedIetBinsDouble);
  // TH2F* isolation_vs_pt = new TH2F("isolation_vs_pt","isolation_vs_pt",100,0.,100.,200,0.,200.);
  isolation_vs_pt->Clear();

  const UInt_t nIsolation = 101;
  const Double_t isolationBins[nIsolation] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100};
 
  // SHAPE COMMENT OUT
  // const UInt_t nShapes = 129;
  // const Double_t shapesBins[nShapes] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128};
  //
  // TH3F* shapes_vs_isolation_vs_pt = new TH3F("shapes_vs_isolation_vs_pt","shapes_vs_isolation_vs_pt",nShapes-1,shapesBins,nIsolation-1,isolationBins,compressedNbinsIEt-1,hardcodedCompressedIetBinsDouble);
  // shapes_vs_isolation_vs_pt->Clear();

  TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
  TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));
  TString InputFileName  = "/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_04_optimizationV0_calibThr"+intgr+"p"+decim+"/Tau_MC_COMPRESSED_2023_03_04.root";
  TString OutputFileName = "/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_04_optimizationV0_calibThr"+intgr+"p"+decim+"/Tau_MC_CALIBRATED_2023_03_04.root";

  TFile f_histos("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/Calibrate/corrections_2023/corrections_Trigger_Stage2_Run3_MC_compressedieta_compressediet_hasEM_isMerged_optimizationV0.root","READ");
  TH3F* h_LUT_isMerged0 = (TH3F*)f_histos.Get("LUT_isMerged0_GBRFullLikelihood_Trigger_Stage2_Run3_MC_compressedieta_compressediet_hasEM_isMerged_optimizationV0");
  TH3F* h_LUT_isMerged1 = (TH3F*)f_histos.Get("LUT_isMerged1_GBRFullLikelihood_Trigger_Stage2_Run3_MC_compressedieta_compressediet_hasEM_isMerged_optimizationV0");

  TFile f_in(InputFileName.Data(),"READ");
  TTree* inTree = (TTree*)f_in.Get("outTreeForCalibration");

  Int_t           L1Tau_IEta;
  Int_t           L1Tau_hasEM;
  Float_t         Target;
  Int_t           L1Tau_IEt;
  Int_t           L1Tau_RawIEt;
  Int_t           EventNumber;
  Int_t           RunNumber;
  Int_t           Nvtx;
  Int_t           L1Tau_nTT;
  Float_t         L1Tau_pt;
  Float_t         OfflineTau_pt;
  Float_t         OfflineTau_eta;
  Int_t           L1Tau_Iso;
  Int_t           OfflineTau_isMatched;
  Int_t           L1Tau_isMerged;
  Int_t           compressedshape;
  Int_t           compressedsortedshape;
  Int_t           compressedieta;
  Int_t           compressedE;
  Int_t           compressednTT;
  Int_t           supercompressedE;
  Int_t           supercompressednTT;
  Int_t           L1Tau_Qual;

    
  inTree->SetBranchAddress("L1Tau_IEta", &L1Tau_IEta);
  inTree->SetBranchAddress("compressedsortedshape", &compressedsortedshape);
  inTree->SetBranchAddress("L1Tau_hasEM", &L1Tau_hasEM);
  inTree->SetBranchAddress("Target", &Target);
  inTree->SetBranchAddress("L1Tau_IEt", &L1Tau_IEt);
  inTree->SetBranchAddress("L1Tau_RawIEt", &L1Tau_RawIEt);
  inTree->SetBranchAddress("EventNumber", &EventNumber);
  inTree->SetBranchAddress("RunNumber", &RunNumber);
  inTree->SetBranchAddress("Nvtx", &Nvtx);
  inTree->SetBranchAddress("L1Tau_nTT", &L1Tau_nTT);
  inTree->SetBranchAddress("L1Tau_pt", &L1Tau_pt);
  inTree->SetBranchAddress("OfflineTau_pt", &OfflineTau_pt);
  inTree->SetBranchAddress("OfflineTau_eta", &OfflineTau_eta);
  inTree->SetBranchAddress("compressedieta", &compressedieta);
  inTree->SetBranchAddress("compressedE", &compressedE);
  inTree->SetBranchAddress("L1Tau_Iso", &L1Tau_Iso);
  inTree->SetBranchAddress("compressednTT", &compressednTT);
  inTree->SetBranchAddress("OfflineTau_isMatched", &OfflineTau_isMatched);
  inTree->SetBranchAddress("L1Tau_isMerged", &L1Tau_isMerged);
  inTree->SetBranchAddress("compressedshape", &compressedshape);
  inTree->SetBranchAddress("L1Tau_Qual",&L1Tau_Qual);
  // inTree->SetBranchAddress("supercompressedE", &supercompressedE);
  // inTree->SetBranchAddress("supercompressednTT", &supercompressednTT);

  TFile f(OutputFileName.Data(),"RECREATE");
  TTree outTree("outTreeCalibrated","outTreeCalibrated");

  Int_t           out_L1Tau_IEta;
  Int_t           out_compressedsortedshape;
  Int_t           out_L1Tau_hasEM;
  Float_t         out_Target;
  Int_t           out_L1Tau_IEt;
  Int_t           out_L1Tau_RawIEt;
  Int_t           out_EventNumber;
  Int_t           out_RunNumber;
  Int_t           out_Nvtx;
  Int_t           out_L1Tau_nTT;
  Float_t         out_L1Tau_pt;
  Float_t         out_L1Tau_CalibPt;
  Float_t         out_OfflineTau_pt;
  Float_t         out_OfflineTau_eta;
  Int_t           out_compressedieta;
  Int_t           out_compressedE;
  Int_t           out_L1Tau_Iso;
  Int_t           out_compressednTT;
  Int_t           out_OfflineTau_isMatched;
  Int_t           out_L1Tau_isMerged;
  Int_t           out_compressedshape;
  Int_t           out_supercompressedE;
  Int_t           out_supercompressednTT;
  Int_t           out_L1Tau_Qual;
  Bool_t          out_L1Tau_PassShapeVeto;
  Float_t         out_L1Tau_CalibConstant;

  outTree.Branch("L1Tau_IEta", &out_L1Tau_IEta,"L1Tau_IEta/I");
  outTree.Branch("compressedsortedshape", &out_compressedsortedshape,"compressedsortedshape/I");
  outTree.Branch("L1Tau_hasEM", &L1Tau_hasEM, "L1Tau_hasEM/I");
  outTree.Branch("Target", &out_Target, "Target/F");
  outTree.Branch("L1Tau_IEt", &out_L1Tau_IEt, "L1Tau_IEt/I");
  outTree.Branch("L1Tau_RawIEt", &out_L1Tau_RawIEt, "L1Tau_RawIEt/I");
  outTree.Branch("EventNumber", &out_EventNumber, "EventNumber/I");
  outTree.Branch("RunNumber", &out_RunNumber, "RunNumber/I");
  outTree.Branch("Nvtx", &out_Nvtx, "Nvtx/I");
  outTree.Branch("L1Tau_nTT", &out_L1Tau_nTT, "L1Tau/I");
  outTree.Branch("L1Tau_pt", &out_L1Tau_pt, "L1Tau_pt/F");
  outTree.Branch("L1Tau_CalibPt", &out_L1Tau_CalibPt, "L1Tau_CalibPt/F");
  outTree.Branch("L1Tau_CalibConstant", &out_L1Tau_CalibConstant, "L1Tau_CalibConstant/F");
  outTree.Branch("OfflineTau_pt", &out_OfflineTau_pt, "OfflineTau_pt/F");
  outTree.Branch("OfflineTau_eta", &out_OfflineTau_eta, "OfflineTau_eta/F");
  outTree.Branch("compressedieta", &out_compressedieta, "compressedieta/I");
  outTree.Branch("compressedE", &out_compressedE, "compressedE/I");
  outTree.Branch("L1Tau_Iso", &out_L1Tau_Iso, "L1Tau_Iso/I");
  outTree.Branch("compressednTT", &out_compressednTT, "compressednTT/I");
  outTree.Branch("OfflineTau_isMatched", &out_OfflineTau_isMatched, "OfflineTau_isMatched/I");
  outTree.Branch("L1Tau_isMerged", &out_L1Tau_isMerged, "L1Tau_isMerged/I");
  outTree.Branch("compressedshape", &out_compressedshape, "compressedshape/I");
  outTree.Branch("supercompressedE", &out_supercompressedE, "supercompressedE/I");
  outTree.Branch("supercompressednTT", &out_supercompressednTT, "supercompressednTT/I");
  outTree.Branch("L1Tau_PassShapeVeto",&out_L1Tau_PassShapeVeto, "out_L1Tau_PassShapeVeto/O");
  outTree.Branch("L1Tau_Qual",&out_L1Tau_Qual, "out_L1Tau_Qual/I");

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

  for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    {
      inTree->GetEntry(i);
      if(i%10000==0) cout<<"Entry #"<<i<<endl;

      out_L1Tau_IEta = abs(L1Tau_IEta);
      out_compressedsortedshape = compressedsortedshape;
      out_L1Tau_hasEM = L1Tau_hasEM;
      out_Target = Target;
      out_L1Tau_IEt = L1Tau_IEt;
      out_L1Tau_RawIEt = L1Tau_RawIEt;
      out_EventNumber = EventNumber;
      out_RunNumber = RunNumber;
      out_Nvtx = Nvtx;
      out_L1Tau_nTT = L1Tau_nTT;
      out_L1Tau_pt = L1Tau_pt;
      out_OfflineTau_pt = OfflineTau_pt;
      out_OfflineTau_eta = OfflineTau_eta;
      out_compressedieta = abs(compressedieta);
      out_compressedE = compressedE;
      out_L1Tau_Iso = L1Tau_Iso;
      out_compressednTT = compressednTT;
      out_OfflineTau_isMatched = OfflineTau_isMatched;
      out_L1Tau_isMerged = L1Tau_isMerged;
      out_compressedshape = compressedshape;

      // SHAPE COMMENT OUT
      // out_L1Tau_PassShapeVeto = true;
      // if (out_L1Tau_IEt > 20 && out_L1Tau_IEt < 30)
      //   {
      //     out_L1Tau_PassShapeVeto = (find(veto_set_20.begin(), veto_set_20.end(), symmShape) == veto_set_20.end() ? true : false);
      //   }
      // else if (out_L1Tau_IEt > 30 && out_L1Tau_IEt < 40)
      //   {
      //     out_L1Tau_PassShapeVeto = (find(veto_set_30.begin(), veto_set_30.end(), symmShape) == veto_set_30.end() ? true : false);
      //   }
      // else if (out_L1Tau_IEt > 40 && out_L1Tau_IEt < 50)
      //   {
      //     out_L1Tau_PassShapeVeto = (find(veto_set_40.begin(), veto_set_40.end(), symmShape) == veto_set_40.end() ? true : false);
      //   }
      // else out_L1Tau_PassShapeVeto = true;

      for(Int_t ie = 0 ; ie < supercompressedNbinsIEt ; ++ie)
      {
        if(L1Tau_IEt>=255)
          {
            out_supercompressedE = supercompressedNbinsIEt-2;
            break;
          }
        else if(L1Tau_IEt >= hardcodedSupercompressedIetBins[ie] && L1Tau_IEt < hardcodedSupercompressedIetBins[ie+1])
          {
            out_supercompressedE = ie;
            break;
          }
      }

      for(Int_t inTT = 0 ; inTT < supercompressedNbinsnTT ; ++inTT)
      {
        if(L1Tau_nTT>=255)
          {
            out_supercompressednTT = supercompressedNbinsnTT-2;
            break;
          }
        else if(L1Tau_nTT >= hardcodedSupercompressednTTBins[inTT] && L1Tau_nTT < hardcodedSupercompressednTTBins[inTT+1])
          {
            out_supercompressednTT = inTT;
            break;
          }
      }

      float calibThr = 1.7;

      if(!out_L1Tau_isMerged)
        {
          out_L1Tau_CalibConstant = h_LUT_isMerged0->GetBinContent(abs(compressedieta)+1,compressedE+1,L1Tau_hasEM+1);
          // if(out_L1Tau_CalibConstant>1.3) out_L1Tau_CalibConstant = 1.3;
          if(out_L1Tau_CalibConstant>calibThr) out_L1Tau_CalibConstant = calibThr;
          out_L1Tau_CalibPt = out_L1Tau_CalibConstant*L1Tau_IEt/2.;
        }
      else
        {
          out_L1Tau_CalibConstant = h_LUT_isMerged1->GetBinContent(abs(compressedieta)+1,compressedE+1,L1Tau_hasEM+1);
          // if(out_L1Tau_CalibConstant>1.3) out_L1Tau_CalibConstant = 1.3;
          if(out_L1Tau_CalibConstant>calibThr) out_L1Tau_CalibConstant = calibThr;
          out_L1Tau_CalibPt = out_L1Tau_CalibConstant*L1Tau_IEt/2.;
        }


      out_L1Tau_Qual = L1Tau_Qual;
      
      // SHAPE COMMENT OUT
      // short int symmShape = getSymmShape (L1Tau_Qual, remap);
      // out_L1Tau_PassShapeVeto = true;

      // SHAPE COMMENT OUT
      // shapes_vs_pt->Fill(symmShape,out_L1Tau_IEt);
      // // shapes_vs_pt->Fill(symmShape,out_L1Tau_IEt/2.);
      // shapes_vs_eta->Fill(symmShape,out_L1Tau_IEta);
      // isolation_vs_pt->Fill(L1Tau_Iso,out_L1Tau_IEt);
      // shapes_vs_isolation_vs_pt->Fill(symmShape,L1Tau_Iso,out_L1Tau_IEt);
      // // isolation_vs_pt->Fill(L1Tau_Iso,out_L1Tau_IEt/2.);
      // if(out_L1Tau_IEt/2.>=30.) shapes_1D->Fill(symmShape);

      // SHAPE COMMENT OUT
      // if (out_L1Tau_CalibPt >= 20 && out_L1Tau_CalibPt < 30)
      //   {
      //     out_L1Tau_PassShapeVeto = (find(veto_set_20.begin(), veto_set_20.end(), symmShape) == veto_set_20.end() ? true : false);
      //   }
      // else if (out_L1Tau_CalibPt >= 30 && out_L1Tau_CalibPt < 40)
      //   {
      //     out_L1Tau_PassShapeVeto = (find(veto_set_30.begin(), veto_set_30.end(), symmShape) == veto_set_30.end() ? true : false);
      //   }
      // else if (out_L1Tau_CalibPt >= 40 && out_L1Tau_CalibPt < 50)
      //   {
      //     out_L1Tau_PassShapeVeto = (find(veto_set_40.begin(), veto_set_40.end(), symmShape) == veto_set_40.end() ? true : false);
      //   }
      // else out_L1Tau_PassShapeVeto = true;      

      outTree.Fill();

    }

  outTree.Write();

  // SHAPE COMMENT OUT
  // TFile f_out_shapes("signal_shapes_forTraining_fine_binning_dummy_19_10_17.root","RECREATE");
  // // TFile f_out_shapes("signal_shapes_forTraining.root","RECREATE");
  // shapes_vs_pt->Write("shapes_vs_pt");
  // isolation_vs_pt->Write("isolation_vs_pt");
  // shapes_vs_isolation_vs_pt->Write("shapes_vs_isolation_vs_pt");
  // shapes_vs_eta->Write("shapes_vs_eta");
  // shapes_1D->Write("hShapes_gt30_merg");

}
