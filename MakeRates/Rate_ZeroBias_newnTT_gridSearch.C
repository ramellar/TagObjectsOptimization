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
#include <TVector.h>
#include <sstream>
#include <TBranchElement.h>
#include <fstream>
#include "/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/Calibrate/ApplyCalibration_newnTT.C"
#include <map>
#include <typeinfo>

using namespace std;

float rateAtFixedThreshold(float treshold, TH1F* rateHist) {
    float rate = 0.0;

    for(int i = 1 ; i <= rateHist->GetNbinsX() ; ++i) {
        if(rateHist->GetBinLowEdge(i)>=treshold) {
            rate = rateHist->GetBinContent(i);
            break;
        }
    }

    return rate;
}

void Rate(int run, TString tag, float calibThr = 1.7)
{
    TString run_str = to_string(run);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    std::map<TString,TH3F*> histosIsolation;
    TFile f_Isolation("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/Isolate/LUTs/LUTrelaxation_Trigger_Stage2_Run3_MC_VBFHToTauTau_M125_optimizationV3gs_calibThr"+intgr+"p"+decim+"_"+tag+".root","READ");

    for(UInt_t i = 0 ; i < 101 ; ++i)
        {
            TString CurrentNameHisto = "LUT_WP";
            ostringstream convert;
            convert << i;
            CurrentNameHisto += convert.str();
            TH3F* current_Histo = (TH3F*)f_Isolation.Get(CurrentNameHisto.Data());
            histosIsolation.insert(make_pair(CurrentNameHisto,current_Histo));
        }

    // DEBUG
    // for (map<TString,TH3F*>::iterator it = histosIsolation.begin(); it != histosIsolation.end(); it++)
    //   {
    //     cout << it->first << endl;
    //   }
    // return;

    for(UInt_t i = 1 ; i < 32 ; ++i)
        {
            TString CurrentNameHisto = "LUT_Progression_";
            ostringstream convert;
            convert << i;
            CurrentNameHisto += convert.str();
            TH3F* current_Histo = (TH3F*)f_Isolation.Get(CurrentNameHisto.Data());
            histosIsolation.insert(make_pair(CurrentNameHisto,current_Histo));
        }  

    TH3F* current_Histo_extrap = (TH3F*)f_Isolation.Get("LUT_Progression_31_extrap");
    histosIsolation.insert(make_pair("LUT_Progression_31_extrap",current_Histo_extrap));

    std::vector<TString> gridSearch = {"A0", "B0", "C0", "D0", "E0", "F0", "G0", "H0", "I0", "J0", "K0", "L0", "M0", "N0", "O0", "P0", "Q0", "R0", "S0", "T0", "U0", "V0", "W0", "X0", "Y0", "Z0", "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1", "I1", "J1", "K1", "L1", "M1", "N1", "O1", "P1", "Q1", "R1", "S1", "T1", "U1", "V1", "W1", "X1", "Y1", "Z1", "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2", "I2", "J2", "K2", "L2", "M2", "N2", "O2", "P2", "Q2", "R2", "S2", "T2", "U2", "V2", "W2", "X2", "Y2", "Z2", "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3", "I3", "J3", "K3", "L3", "M3", "N3", "O3", "P3", "Q3", "R3", "S3", "T3", "U3", "V3", "W3", "X3", "Y3", "Z3", "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4", "I4", "J4", "K4", "L4", "M4", "N4", "O4", "P4", "Q4", "R4", "S4", "T4", "U4", "V4", "W4", "X4", "Y4", "Z4", "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5", "I5", "J5", "K5", "L5", "M5", "N5", "O5", "P5", "Q5", "R5", "S5", "T5", "U5", "V5", "W5", "X5", "Y5", "Z5", "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6", "I6", "J6", "K6", "L6", "M6", "N6", "O6", "P6", "Q6", "R6", "S6", "T6", "U6", "V6", "W6", "X6", "Y6", "Z6", "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7", "I7", "J7", "K7", "L7", "M7", "N7", "O7", "P7", "Q7", "R7", "S7", "T7", "U7", "V7", "W7", "X7", "Y7", "Z7", "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8", "I8", "J8", "K8", "L8", "M8", "N8", "O8", "P8", "Q8", "R8", "S8", "T8", "U8", "V8", "W8", "X8", "Y8", "Z8"};
    for(UInt_t i = 0 ; i < gridSearch.size() ; ++i)
        {
            TString CurrentNameHisto = "LUT_Progression_"+gridSearch[i];
            TH3F* current_Histo = (TH3F*)f_Isolation.Get(CurrentNameHisto.Data());
            histosIsolation.insert(make_pair(CurrentNameHisto,current_Histo));
        }

    // DEBUG
    // cout << typeid(histosIsolation["LUT_WP50"]).name() << endl;
    // cout << typeid(histosIsolation["LUT_WP50"]->GetBinContent(1,1,1)).name() << endl;
    // cout << histosIsolation["LUT_WP50"]->GetBinContent(1,1,1) << endl;
    // return;

    TString FileName_in = "/data_CMS/cms/motta/Run3preparation/2022_01_15_optimizationV3_calibThr"+intgr+"p"+decim+"/EphemeralZeroBias_2018D_Run"+run_str+"_CALIBRATED.root";
    TFile f_in(FileName_in.Data(),"READ");
    TTree* inTree = (TTree*)f_in.Get("outTreeCalibrated");

    Int_t       in_EventNumber =  0;
    Int_t           in_RunNumber =  0;
    Int_t           in_lumi =  0;
    vector<float>   *in_l1tEmuPt =  0;
    vector<float>   *in_l1tEmuEta =  0;
    vector<float>   *in_l1tEmuPhi =  0;
    vector<int>     *in_l1tEmuQual =  0;
    vector<int>     *in_l1tEmuNTT =  0;
    vector<int>     *in_l1tEmuHasEM =  0;
    vector<int>     *in_l1tEmuIsMerged =  0;
    vector<int>     *in_l1tEmuTowerIEta =  0;
    vector<int>     *in_l1tEmuTowerIPhi =  0;
    vector<int>     *in_l1tEmuRawEt =  0;
    vector<int>     *in_l1tEmuIsoEt =  0;
    vector<int>     *in_compressedieta =  0;
    vector<int>     *in_compressedE =  0;
    vector<int>     *in_compressednTT =  0;
    vector<int>     *in_supercompressedE =  0;
    vector<int>     *in_supercompressednTT =  0;
    vector<float>   *in_CalibPt =  0;

    inTree->SetBranchAddress("EventNumber", &in_EventNumber);
    inTree->SetBranchAddress("RunNumber", &in_RunNumber);
    inTree->SetBranchAddress("lumi", &in_lumi);
    inTree->SetBranchAddress("L1Tau_pt",&in_l1tEmuPt);
    inTree->SetBranchAddress("L1Tau_eta",&in_l1tEmuEta);
    inTree->SetBranchAddress("L1Tau_phi",&in_l1tEmuPhi);
    inTree->SetBranchAddress("L1Tau_Qual",&in_l1tEmuQual);
    inTree->SetBranchAddress("L1Tau_nTT",&in_l1tEmuNTT);
    inTree->SetBranchAddress("L1Tau_hasEM",&in_l1tEmuHasEM);
    inTree->SetBranchAddress("L1Tau_isMerged",&in_l1tEmuIsMerged);
    inTree->SetBranchAddress("L1Tau_IEt",&in_l1tEmuRawEt);
    inTree->SetBranchAddress("L1Tau_Iso",&in_l1tEmuIsoEt);
    inTree->SetBranchAddress("L1Tau_IEta",&in_l1tEmuTowerIEta);
    inTree->SetBranchAddress("L1Tau_IPhi",&in_l1tEmuTowerIPhi);
    inTree->SetBranchAddress("compressedieta",&in_compressedieta);
    inTree->SetBranchAddress("compressedE",&in_compressedE);
    inTree->SetBranchAddress("compressednTT",&in_compressednTT);
    inTree->SetBranchAddress("supercompressedE",&in_supercompressedE);
    inTree->SetBranchAddress("supercompressednTT",&in_supercompressednTT);
    inTree->SetBranchAddress("L1Tau_CalibPt",&in_CalibPt);

    TH1F* h_pt_uncalibrated = new TH1F("h_pt_uncalibrated","h_pt_uncalibrated",240,0.,240.);
    TH1F* h_pt_calibrated = new TH1F("h_pt_calibrated","h_pt_calibrated",240,0.,240.);
    TH1F* pt_IsoInf = new TH1F("pt_IsoInf","pt_IsoInf",240,0.,240.);
    TH1F* pt_Iso_50 = new TH1F("pt_Iso_50","pt_Iso_50",240,0.,240.);
    TH1F* pt_Iso_60 = new TH1F("pt_Iso_60","pt_Iso_60",240,0.,240.);
    TH1F* pt_Iso_70 = new TH1F("pt_Iso_70","pt_Iso_70",240,0.,240.);
    TH1F* pt_Iso_80 = new TH1F("pt_Iso_80","pt_Iso_80",240,0.,240.);
    TH1F* pt_Iso_90 = new TH1F("pt_Iso_90","pt_Iso_90",240,0.,240.);

    TH1F* pt_Progression_1 = new TH1F("pt_Progression_1","pt_Progression_1",240,0.,240.);
    TH1F* pt_Progression_2 = new TH1F("pt_Progression_2","pt_Progression_2",240,0.,240.);
    TH1F* pt_Progression_3 = new TH1F("pt_Progression_3","pt_Progression_3",240,0.,240.);
    TH1F* pt_Progression_4 = new TH1F("pt_Progression_4","pt_Progression_4",240,0.,240.);
    TH1F* pt_Progression_5 = new TH1F("pt_Progression_5","pt_Progression_5",240,0.,240.);
    TH1F* pt_Progression_6 = new TH1F("pt_Progression_6","pt_Progression_6",240,0.,240.);
    TH1F* pt_Progression_7 = new TH1F("pt_Progression_7","pt_Progression_7",240,0.,240.);
    TH1F* pt_Progression_8 = new TH1F("pt_Progression_8","pt_Progression_8",240,0.,240.);
    TH1F* pt_Progression_9 = new TH1F("pt_Progression_9","pt_Progression_9",240,0.,240.);
    TH1F* pt_Progression_10 = new TH1F("pt_Progression_10","pt_Progression_10",240,0.,240.);
    TH1F* pt_Progression_11 = new TH1F("pt_Progression_11","pt_Progression_11",240,0.,240.);
    TH1F* pt_Progression_12 = new TH1F("pt_Progression_12","pt_Progression_12",240,0.,240.);
    TH1F* pt_Progression_13 = new TH1F("pt_Progression_13","pt_Progression_13",240,0.,240.);
    TH1F* pt_Progression_14 = new TH1F("pt_Progression_14","pt_Progression_14",240,0.,240.);
    TH1F* pt_Progression_15 = new TH1F("pt_Progression_15","pt_Progression_15",240,0.,240.);
    TH1F* pt_Progression_16 = new TH1F("pt_Progression_16","pt_Progression_16",240,0.,240.);
    TH1F* pt_Progression_17 = new TH1F("pt_Progression_17","pt_Progression_17",240,0.,240.);
    TH1F* pt_Progression_18 = new TH1F("pt_Progression_18","pt_Progression_18",240,0.,240.);
    TH1F* pt_Progression_19 = new TH1F("pt_Progression_19","pt_Progression_19",240,0.,240.);
    TH1F* pt_Progression_20 = new TH1F("pt_Progression_20","pt_Progression_20",240,0.,240.);
    TH1F* pt_Progression_21 = new TH1F("pt_Progression_21","pt_Progression_21",240,0.,240.);
    TH1F* pt_Progression_22 = new TH1F("pt_Progression_22","pt_Progression_22",240,0.,240.);

    TH2F* pt_IsoInf_DiTau = new TH2F("pt_IsoInf_DiTau","pt_IsoInf_DiTau",240,0.,240.,240,0.,240.);

    TH2F* pt_DiTau_90pc = new TH2F("pt_DiTau_90pc","pt_DiTau_90pc",240,0.,240.,240,0.,240.);
    TH2F* pt_DiTau_80pc = new TH2F("pt_DiTau_80pc","pt_DiTau_80pc",240,0.,240.,240,0.,240.);
    TH2F* pt_DiTau_70pc = new TH2F("pt_DiTau_70pc","pt_DiTau_70pc",240,0.,240.,240,0.,240.);
    TH2F* pt_DiTau_60pc = new TH2F("pt_DiTau_60pc","pt_DiTau_60pc",240,0.,240.,240,0.,240.);
    TH2F* pt_DiTau_50pc = new TH2F("pt_DiTau_50pc","pt_DiTau_50pc",240,0.,240.,240,0.,240.);

    // START OF FULL BLOWN GRID SEARCH
    TH1F* pt_Progression_A0 = new TH1F("pt_Progression_A0","pt_Progression_A0",240,0.,240.);
    TH1F* pt_Progression_B0 = new TH1F("pt_Progression_B0","pt_Progression_B0",240,0.,240.);
    TH1F* pt_Progression_C0 = new TH1F("pt_Progression_C0","pt_Progression_C0",240,0.,240.);
    TH1F* pt_Progression_D0 = new TH1F("pt_Progression_D0","pt_Progression_D0",240,0.,240.);
    TH1F* pt_Progression_E0 = new TH1F("pt_Progression_E0","pt_Progression_E0",240,0.,240.);
    TH1F* pt_Progression_F0 = new TH1F("pt_Progression_F0","pt_Progression_F0",240,0.,240.);
    TH1F* pt_Progression_G0 = new TH1F("pt_Progression_G0","pt_Progression_G0",240,0.,240.);
    TH1F* pt_Progression_H0 = new TH1F("pt_Progression_H0","pt_Progression_H0",240,0.,240.);
    TH1F* pt_Progression_I0 = new TH1F("pt_Progression_I0","pt_Progression_I0",240,0.,240.);
    TH1F* pt_Progression_J0 = new TH1F("pt_Progression_J0","pt_Progression_J0",240,0.,240.);
    TH1F* pt_Progression_K0 = new TH1F("pt_Progression_K0","pt_Progression_K0",240,0.,240.);
    TH1F* pt_Progression_L0 = new TH1F("pt_Progression_L0","pt_Progression_L0",240,0.,240.);
    TH1F* pt_Progression_M0 = new TH1F("pt_Progression_M0","pt_Progression_M0",240,0.,240.);
    TH1F* pt_Progression_N0 = new TH1F("pt_Progression_N0","pt_Progression_N0",240,0.,240.);
    TH1F* pt_Progression_O0 = new TH1F("pt_Progression_O0","pt_Progression_O0",240,0.,240.);
    TH1F* pt_Progression_P0 = new TH1F("pt_Progression_P0","pt_Progression_P0",240,0.,240.);
    TH1F* pt_Progression_Q0 = new TH1F("pt_Progression_Q0","pt_Progression_Q0",240,0.,240.);
    TH1F* pt_Progression_R0 = new TH1F("pt_Progression_R0","pt_Progression_R0",240,0.,240.);
    TH1F* pt_Progression_S0 = new TH1F("pt_Progression_S0","pt_Progression_S0",240,0.,240.);
    TH1F* pt_Progression_T0 = new TH1F("pt_Progression_T0","pt_Progression_T0",240,0.,240.);
    TH1F* pt_Progression_U0 = new TH1F("pt_Progression_U0","pt_Progression_U0",240,0.,240.);
    TH1F* pt_Progression_V0 = new TH1F("pt_Progression_V0","pt_Progression_V0",240,0.,240.);
    TH1F* pt_Progression_W0 = new TH1F("pt_Progression_W0","pt_Progression_W0",240,0.,240.);
    TH1F* pt_Progression_X0 = new TH1F("pt_Progression_X0","pt_Progression_X0",240,0.,240.);
    TH1F* pt_Progression_Y0 = new TH1F("pt_Progression_Y0","pt_Progression_Y0",240,0.,240.);
    TH1F* pt_Progression_Z0 = new TH1F("pt_Progression_Z0","pt_Progression_Z0",240,0.,240.);
    TH1F* pt_Progression_A1 = new TH1F("pt_Progression_A1","pt_Progression_A1",240,0.,240.);
    TH1F* pt_Progression_B1 = new TH1F("pt_Progression_B1","pt_Progression_B1",240,0.,240.);
    TH1F* pt_Progression_C1 = new TH1F("pt_Progression_C1","pt_Progression_C1",240,0.,240.);
    TH1F* pt_Progression_D1 = new TH1F("pt_Progression_D1","pt_Progression_D1",240,0.,240.);
    TH1F* pt_Progression_E1 = new TH1F("pt_Progression_E1","pt_Progression_E1",240,0.,240.);
    TH1F* pt_Progression_F1 = new TH1F("pt_Progression_F1","pt_Progression_F1",240,0.,240.);
    TH1F* pt_Progression_G1 = new TH1F("pt_Progression_G1","pt_Progression_G1",240,0.,240.);
    TH1F* pt_Progression_H1 = new TH1F("pt_Progression_H1","pt_Progression_H1",240,0.,240.);
    TH1F* pt_Progression_I1 = new TH1F("pt_Progression_I1","pt_Progression_I1",240,0.,240.);
    TH1F* pt_Progression_J1 = new TH1F("pt_Progression_J1","pt_Progression_J1",240,0.,240.);
    TH1F* pt_Progression_K1 = new TH1F("pt_Progression_K1","pt_Progression_K1",240,0.,240.);
    TH1F* pt_Progression_L1 = new TH1F("pt_Progression_L1","pt_Progression_L1",240,0.,240.);
    TH1F* pt_Progression_M1 = new TH1F("pt_Progression_M1","pt_Progression_M1",240,0.,240.);
    TH1F* pt_Progression_N1 = new TH1F("pt_Progression_N1","pt_Progression_N1",240,0.,240.);
    TH1F* pt_Progression_O1 = new TH1F("pt_Progression_O1","pt_Progression_O1",240,0.,240.);
    TH1F* pt_Progression_P1 = new TH1F("pt_Progression_P1","pt_Progression_P1",240,0.,240.);
    TH1F* pt_Progression_Q1 = new TH1F("pt_Progression_Q1","pt_Progression_Q1",240,0.,240.);
    TH1F* pt_Progression_R1 = new TH1F("pt_Progression_R1","pt_Progression_R1",240,0.,240.);
    TH1F* pt_Progression_S1 = new TH1F("pt_Progression_S1","pt_Progression_S1",240,0.,240.);
    TH1F* pt_Progression_T1 = new TH1F("pt_Progression_T1","pt_Progression_T1",240,0.,240.);
    TH1F* pt_Progression_U1 = new TH1F("pt_Progression_U1","pt_Progression_U1",240,0.,240.);
    TH1F* pt_Progression_V1 = new TH1F("pt_Progression_V1","pt_Progression_V1",240,0.,240.);
    TH1F* pt_Progression_W1 = new TH1F("pt_Progression_W1","pt_Progression_W1",240,0.,240.);
    TH1F* pt_Progression_X1 = new TH1F("pt_Progression_X1","pt_Progression_X1",240,0.,240.);
    TH1F* pt_Progression_Y1 = new TH1F("pt_Progression_Y1","pt_Progression_Y1",240,0.,240.);
    TH1F* pt_Progression_Z1 = new TH1F("pt_Progression_Z1","pt_Progression_Z1",240,0.,240.);
    TH1F* pt_Progression_A2 = new TH1F("pt_Progression_A2","pt_Progression_A2",240,0.,240.);
    TH1F* pt_Progression_B2 = new TH1F("pt_Progression_B2","pt_Progression_B2",240,0.,240.);
    TH1F* pt_Progression_C2 = new TH1F("pt_Progression_C2","pt_Progression_C2",240,0.,240.);
    TH1F* pt_Progression_D2 = new TH1F("pt_Progression_D2","pt_Progression_D2",240,0.,240.);
    TH1F* pt_Progression_E2 = new TH1F("pt_Progression_E2","pt_Progression_E2",240,0.,240.);
    TH1F* pt_Progression_F2 = new TH1F("pt_Progression_F2","pt_Progression_F2",240,0.,240.);
    TH1F* pt_Progression_G2 = new TH1F("pt_Progression_G2","pt_Progression_G2",240,0.,240.);
    TH1F* pt_Progression_H2 = new TH1F("pt_Progression_H2","pt_Progression_H2",240,0.,240.);
    TH1F* pt_Progression_I2 = new TH1F("pt_Progression_I2","pt_Progression_I2",240,0.,240.);
    TH1F* pt_Progression_J2 = new TH1F("pt_Progression_J2","pt_Progression_J2",240,0.,240.);
    TH1F* pt_Progression_K2 = new TH1F("pt_Progression_K2","pt_Progression_K2",240,0.,240.);
    TH1F* pt_Progression_L2 = new TH1F("pt_Progression_L2","pt_Progression_L2",240,0.,240.);
    TH1F* pt_Progression_M2 = new TH1F("pt_Progression_M2","pt_Progression_M2",240,0.,240.);
    TH1F* pt_Progression_N2 = new TH1F("pt_Progression_N2","pt_Progression_N2",240,0.,240.);
    TH1F* pt_Progression_O2 = new TH1F("pt_Progression_O2","pt_Progression_O2",240,0.,240.);
    TH1F* pt_Progression_P2 = new TH1F("pt_Progression_P2","pt_Progression_P2",240,0.,240.);
    TH1F* pt_Progression_Q2 = new TH1F("pt_Progression_Q2","pt_Progression_Q2",240,0.,240.);
    TH1F* pt_Progression_R2 = new TH1F("pt_Progression_R2","pt_Progression_R2",240,0.,240.);
    TH1F* pt_Progression_S2 = new TH1F("pt_Progression_S2","pt_Progression_S2",240,0.,240.);
    TH1F* pt_Progression_T2 = new TH1F("pt_Progression_T2","pt_Progression_T2",240,0.,240.);
    TH1F* pt_Progression_U2 = new TH1F("pt_Progression_U2","pt_Progression_U2",240,0.,240.);
    TH1F* pt_Progression_V2 = new TH1F("pt_Progression_V2","pt_Progression_V2",240,0.,240.);
    TH1F* pt_Progression_W2 = new TH1F("pt_Progression_W2","pt_Progression_W2",240,0.,240.);
    TH1F* pt_Progression_X2 = new TH1F("pt_Progression_X2","pt_Progression_X2",240,0.,240.);
    TH1F* pt_Progression_Y2 = new TH1F("pt_Progression_Y2","pt_Progression_Y2",240,0.,240.);
    TH1F* pt_Progression_Z2 = new TH1F("pt_Progression_Z2","pt_Progression_Z2",240,0.,240.);
    TH1F* pt_Progression_A3 = new TH1F("pt_Progression_A3","pt_Progression_A3",240,0.,240.);
    TH1F* pt_Progression_B3 = new TH1F("pt_Progression_B3","pt_Progression_B3",240,0.,240.);
    TH1F* pt_Progression_C3 = new TH1F("pt_Progression_C3","pt_Progression_C3",240,0.,240.);
    TH1F* pt_Progression_D3 = new TH1F("pt_Progression_D3","pt_Progression_D3",240,0.,240.);
    TH1F* pt_Progression_E3 = new TH1F("pt_Progression_E3","pt_Progression_E3",240,0.,240.);
    TH1F* pt_Progression_F3 = new TH1F("pt_Progression_F3","pt_Progression_F3",240,0.,240.);
    TH1F* pt_Progression_G3 = new TH1F("pt_Progression_G3","pt_Progression_G3",240,0.,240.);
    TH1F* pt_Progression_H3 = new TH1F("pt_Progression_H3","pt_Progression_H3",240,0.,240.);
    TH1F* pt_Progression_I3 = new TH1F("pt_Progression_I3","pt_Progression_I3",240,0.,240.);
    TH1F* pt_Progression_J3 = new TH1F("pt_Progression_J3","pt_Progression_J3",240,0.,240.);
    TH1F* pt_Progression_K3 = new TH1F("pt_Progression_K3","pt_Progression_K3",240,0.,240.);
    TH1F* pt_Progression_L3 = new TH1F("pt_Progression_L3","pt_Progression_L3",240,0.,240.);
    TH1F* pt_Progression_M3 = new TH1F("pt_Progression_M3","pt_Progression_M3",240,0.,240.);
    TH1F* pt_Progression_N3 = new TH1F("pt_Progression_N3","pt_Progression_N3",240,0.,240.);
    TH1F* pt_Progression_O3 = new TH1F("pt_Progression_O3","pt_Progression_O3",240,0.,240.);
    TH1F* pt_Progression_P3 = new TH1F("pt_Progression_P3","pt_Progression_P3",240,0.,240.);
    TH1F* pt_Progression_Q3 = new TH1F("pt_Progression_Q3","pt_Progression_Q3",240,0.,240.);
    TH1F* pt_Progression_R3 = new TH1F("pt_Progression_R3","pt_Progression_R3",240,0.,240.);
    TH1F* pt_Progression_S3 = new TH1F("pt_Progression_S3","pt_Progression_S3",240,0.,240.);
    TH1F* pt_Progression_T3 = new TH1F("pt_Progression_T3","pt_Progression_T3",240,0.,240.);
    TH1F* pt_Progression_U3 = new TH1F("pt_Progression_U3","pt_Progression_U3",240,0.,240.);
    TH1F* pt_Progression_V3 = new TH1F("pt_Progression_V3","pt_Progression_V3",240,0.,240.);
    TH1F* pt_Progression_W3 = new TH1F("pt_Progression_W3","pt_Progression_W3",240,0.,240.);
    TH1F* pt_Progression_X3 = new TH1F("pt_Progression_X3","pt_Progression_X3",240,0.,240.);
    TH1F* pt_Progression_Y3 = new TH1F("pt_Progression_Y3","pt_Progression_Y3",240,0.,240.);
    TH1F* pt_Progression_Z3 = new TH1F("pt_Progression_Z3","pt_Progression_Z3",240,0.,240.);
    TH1F* pt_Progression_A4 = new TH1F("pt_Progression_A4","pt_Progression_A4",240,0.,240.);
    TH1F* pt_Progression_B4 = new TH1F("pt_Progression_B4","pt_Progression_B4",240,0.,240.);
    TH1F* pt_Progression_C4 = new TH1F("pt_Progression_C4","pt_Progression_C4",240,0.,240.);
    TH1F* pt_Progression_D4 = new TH1F("pt_Progression_D4","pt_Progression_D4",240,0.,240.);
    TH1F* pt_Progression_E4 = new TH1F("pt_Progression_E4","pt_Progression_E4",240,0.,240.);
    TH1F* pt_Progression_F4 = new TH1F("pt_Progression_F4","pt_Progression_F4",240,0.,240.);
    TH1F* pt_Progression_G4 = new TH1F("pt_Progression_G4","pt_Progression_G4",240,0.,240.);
    TH1F* pt_Progression_H4 = new TH1F("pt_Progression_H4","pt_Progression_H4",240,0.,240.);
    TH1F* pt_Progression_I4 = new TH1F("pt_Progression_I4","pt_Progression_I4",240,0.,240.);
    TH1F* pt_Progression_J4 = new TH1F("pt_Progression_J4","pt_Progression_J4",240,0.,240.);
    TH1F* pt_Progression_K4 = new TH1F("pt_Progression_K4","pt_Progression_K4",240,0.,240.);
    TH1F* pt_Progression_L4 = new TH1F("pt_Progression_L4","pt_Progression_L4",240,0.,240.);
    TH1F* pt_Progression_M4 = new TH1F("pt_Progression_M4","pt_Progression_M4",240,0.,240.);
    TH1F* pt_Progression_N4 = new TH1F("pt_Progression_N4","pt_Progression_N4",240,0.,240.);
    TH1F* pt_Progression_O4 = new TH1F("pt_Progression_O4","pt_Progression_O4",240,0.,240.);
    TH1F* pt_Progression_P4 = new TH1F("pt_Progression_P4","pt_Progression_P4",240,0.,240.);
    TH1F* pt_Progression_Q4 = new TH1F("pt_Progression_Q4","pt_Progression_Q4",240,0.,240.);
    TH1F* pt_Progression_R4 = new TH1F("pt_Progression_R4","pt_Progression_R4",240,0.,240.);
    TH1F* pt_Progression_S4 = new TH1F("pt_Progression_S4","pt_Progression_S4",240,0.,240.);
    TH1F* pt_Progression_T4 = new TH1F("pt_Progression_T4","pt_Progression_T4",240,0.,240.);
    TH1F* pt_Progression_U4 = new TH1F("pt_Progression_U4","pt_Progression_U4",240,0.,240.);
    TH1F* pt_Progression_V4 = new TH1F("pt_Progression_V4","pt_Progression_V4",240,0.,240.);
    TH1F* pt_Progression_W4 = new TH1F("pt_Progression_W4","pt_Progression_W4",240,0.,240.);
    TH1F* pt_Progression_X4 = new TH1F("pt_Progression_X4","pt_Progression_X4",240,0.,240.);
    TH1F* pt_Progression_Y4 = new TH1F("pt_Progression_Y4","pt_Progression_Y4",240,0.,240.);
    TH1F* pt_Progression_Z4 = new TH1F("pt_Progression_Z4","pt_Progression_Z4",240,0.,240.);
    TH1F* pt_Progression_A5 = new TH1F("pt_Progression_A5","pt_Progression_A5",240,0.,240.);
    TH1F* pt_Progression_B5 = new TH1F("pt_Progression_B5","pt_Progression_B5",240,0.,240.);
    TH1F* pt_Progression_C5 = new TH1F("pt_Progression_C5","pt_Progression_C5",240,0.,240.);
    TH1F* pt_Progression_D5 = new TH1F("pt_Progression_D5","pt_Progression_D5",240,0.,240.);
    TH1F* pt_Progression_E5 = new TH1F("pt_Progression_E5","pt_Progression_E5",240,0.,240.);
    TH1F* pt_Progression_F5 = new TH1F("pt_Progression_F5","pt_Progression_F5",240,0.,240.);
    TH1F* pt_Progression_G5 = new TH1F("pt_Progression_G5","pt_Progression_G5",240,0.,240.);
    TH1F* pt_Progression_H5 = new TH1F("pt_Progression_H5","pt_Progression_H5",240,0.,240.);
    TH1F* pt_Progression_I5 = new TH1F("pt_Progression_I5","pt_Progression_I5",240,0.,240.);
    TH1F* pt_Progression_J5 = new TH1F("pt_Progression_J5","pt_Progression_J5",240,0.,240.);
    TH1F* pt_Progression_K5 = new TH1F("pt_Progression_K5","pt_Progression_K5",240,0.,240.);
    TH1F* pt_Progression_L5 = new TH1F("pt_Progression_L5","pt_Progression_L5",240,0.,240.);
    TH1F* pt_Progression_M5 = new TH1F("pt_Progression_M5","pt_Progression_M5",240,0.,240.);
    TH1F* pt_Progression_N5 = new TH1F("pt_Progression_N5","pt_Progression_N5",240,0.,240.);
    TH1F* pt_Progression_O5 = new TH1F("pt_Progression_O5","pt_Progression_O5",240,0.,240.);
    TH1F* pt_Progression_P5 = new TH1F("pt_Progression_P5","pt_Progression_P5",240,0.,240.);
    TH1F* pt_Progression_Q5 = new TH1F("pt_Progression_Q5","pt_Progression_Q5",240,0.,240.);
    TH1F* pt_Progression_R5 = new TH1F("pt_Progression_R5","pt_Progression_R5",240,0.,240.);
    TH1F* pt_Progression_S5 = new TH1F("pt_Progression_S5","pt_Progression_S5",240,0.,240.);
    TH1F* pt_Progression_T5 = new TH1F("pt_Progression_T5","pt_Progression_T5",240,0.,240.);
    TH1F* pt_Progression_U5 = new TH1F("pt_Progression_U5","pt_Progression_U5",240,0.,240.);
    TH1F* pt_Progression_V5 = new TH1F("pt_Progression_V5","pt_Progression_V5",240,0.,240.);
    TH1F* pt_Progression_W5 = new TH1F("pt_Progression_W5","pt_Progression_W5",240,0.,240.);
    TH1F* pt_Progression_X5 = new TH1F("pt_Progression_X5","pt_Progression_X5",240,0.,240.);
    TH1F* pt_Progression_Y5 = new TH1F("pt_Progression_Y5","pt_Progression_Y5",240,0.,240.);
    TH1F* pt_Progression_Z5 = new TH1F("pt_Progression_Z5","pt_Progression_Z5",240,0.,240.);
    TH1F* pt_Progression_A6 = new TH1F("pt_Progression_A6","pt_Progression_A6",240,0.,240.);
    TH1F* pt_Progression_B6 = new TH1F("pt_Progression_B6","pt_Progression_B6",240,0.,240.);
    TH1F* pt_Progression_C6 = new TH1F("pt_Progression_C6","pt_Progression_C6",240,0.,240.);
    TH1F* pt_Progression_D6 = new TH1F("pt_Progression_D6","pt_Progression_D6",240,0.,240.);
    TH1F* pt_Progression_E6 = new TH1F("pt_Progression_E6","pt_Progression_E6",240,0.,240.);
    TH1F* pt_Progression_F6 = new TH1F("pt_Progression_F6","pt_Progression_F6",240,0.,240.);
    TH1F* pt_Progression_G6 = new TH1F("pt_Progression_G6","pt_Progression_G6",240,0.,240.);
    TH1F* pt_Progression_H6 = new TH1F("pt_Progression_H6","pt_Progression_H6",240,0.,240.);
    TH1F* pt_Progression_I6 = new TH1F("pt_Progression_I6","pt_Progression_I6",240,0.,240.);
    TH1F* pt_Progression_J6 = new TH1F("pt_Progression_J6","pt_Progression_J6",240,0.,240.);
    TH1F* pt_Progression_K6 = new TH1F("pt_Progression_K6","pt_Progression_K6",240,0.,240.);
    TH1F* pt_Progression_L6 = new TH1F("pt_Progression_L6","pt_Progression_L6",240,0.,240.);
    TH1F* pt_Progression_M6 = new TH1F("pt_Progression_M6","pt_Progression_M6",240,0.,240.);
    TH1F* pt_Progression_N6 = new TH1F("pt_Progression_N6","pt_Progression_N6",240,0.,240.);
    TH1F* pt_Progression_O6 = new TH1F("pt_Progression_O6","pt_Progression_O6",240,0.,240.);
    TH1F* pt_Progression_P6 = new TH1F("pt_Progression_P6","pt_Progression_P6",240,0.,240.);
    TH1F* pt_Progression_Q6 = new TH1F("pt_Progression_Q6","pt_Progression_Q6",240,0.,240.);
    TH1F* pt_Progression_R6 = new TH1F("pt_Progression_R6","pt_Progression_R6",240,0.,240.);
    TH1F* pt_Progression_S6 = new TH1F("pt_Progression_S6","pt_Progression_S6",240,0.,240.);
    TH1F* pt_Progression_T6 = new TH1F("pt_Progression_T6","pt_Progression_T6",240,0.,240.);
    TH1F* pt_Progression_U6 = new TH1F("pt_Progression_U6","pt_Progression_U6",240,0.,240.);
    TH1F* pt_Progression_V6 = new TH1F("pt_Progression_V6","pt_Progression_V6",240,0.,240.);
    TH1F* pt_Progression_W6 = new TH1F("pt_Progression_W6","pt_Progression_W6",240,0.,240.);
    TH1F* pt_Progression_X6 = new TH1F("pt_Progression_X6","pt_Progression_X6",240,0.,240.);
    TH1F* pt_Progression_Y6 = new TH1F("pt_Progression_Y6","pt_Progression_Y6",240,0.,240.);
    TH1F* pt_Progression_Z6 = new TH1F("pt_Progression_Z6","pt_Progression_Z6",240,0.,240.);
    TH1F* pt_Progression_A7 = new TH1F("pt_Progression_A7","pt_Progression_A7",240,0.,240.);
    TH1F* pt_Progression_B7 = new TH1F("pt_Progression_B7","pt_Progression_B7",240,0.,240.);
    TH1F* pt_Progression_C7 = new TH1F("pt_Progression_C7","pt_Progression_C7",240,0.,240.);
    TH1F* pt_Progression_D7 = new TH1F("pt_Progression_D7","pt_Progression_D7",240,0.,240.);
    TH1F* pt_Progression_E7 = new TH1F("pt_Progression_E7","pt_Progression_E7",240,0.,240.);
    TH1F* pt_Progression_F7 = new TH1F("pt_Progression_F7","pt_Progression_F7",240,0.,240.);
    TH1F* pt_Progression_G7 = new TH1F("pt_Progression_G7","pt_Progression_G7",240,0.,240.);
    TH1F* pt_Progression_H7 = new TH1F("pt_Progression_H7","pt_Progression_H7",240,0.,240.);
    TH1F* pt_Progression_I7 = new TH1F("pt_Progression_I7","pt_Progression_I7",240,0.,240.);
    TH1F* pt_Progression_J7 = new TH1F("pt_Progression_J7","pt_Progression_J7",240,0.,240.);
    TH1F* pt_Progression_K7 = new TH1F("pt_Progression_K7","pt_Progression_K7",240,0.,240.);
    TH1F* pt_Progression_L7 = new TH1F("pt_Progression_L7","pt_Progression_L7",240,0.,240.);
    TH1F* pt_Progression_M7 = new TH1F("pt_Progression_M7","pt_Progression_M7",240,0.,240.);
    TH1F* pt_Progression_N7 = new TH1F("pt_Progression_N7","pt_Progression_N7",240,0.,240.);
    TH1F* pt_Progression_O7 = new TH1F("pt_Progression_O7","pt_Progression_O7",240,0.,240.);
    TH1F* pt_Progression_P7 = new TH1F("pt_Progression_P7","pt_Progression_P7",240,0.,240.);
    TH1F* pt_Progression_Q7 = new TH1F("pt_Progression_Q7","pt_Progression_Q7",240,0.,240.);
    TH1F* pt_Progression_R7 = new TH1F("pt_Progression_R7","pt_Progression_R7",240,0.,240.);
    TH1F* pt_Progression_S7 = new TH1F("pt_Progression_S7","pt_Progression_S7",240,0.,240.);
    TH1F* pt_Progression_T7 = new TH1F("pt_Progression_T7","pt_Progression_T7",240,0.,240.);
    TH1F* pt_Progression_U7 = new TH1F("pt_Progression_U7","pt_Progression_U7",240,0.,240.);
    TH1F* pt_Progression_V7 = new TH1F("pt_Progression_V7","pt_Progression_V7",240,0.,240.);
    TH1F* pt_Progression_W7 = new TH1F("pt_Progression_W7","pt_Progression_W7",240,0.,240.);
    TH1F* pt_Progression_X7 = new TH1F("pt_Progression_X7","pt_Progression_X7",240,0.,240.);
    TH1F* pt_Progression_Y7 = new TH1F("pt_Progression_Y7","pt_Progression_Y7",240,0.,240.);
    TH1F* pt_Progression_Z7 = new TH1F("pt_Progression_Z7","pt_Progression_Z7",240,0.,240.);
    TH1F* pt_Progression_A8 = new TH1F("pt_Progression_A8","pt_Progression_A8",240,0.,240.);
    TH1F* pt_Progression_B8 = new TH1F("pt_Progression_B8","pt_Progression_B8",240,0.,240.);
    TH1F* pt_Progression_C8 = new TH1F("pt_Progression_C8","pt_Progression_C8",240,0.,240.);
    TH1F* pt_Progression_D8 = new TH1F("pt_Progression_D8","pt_Progression_D8",240,0.,240.);
    TH1F* pt_Progression_E8 = new TH1F("pt_Progression_E8","pt_Progression_E8",240,0.,240.);
    TH1F* pt_Progression_F8 = new TH1F("pt_Progression_F8","pt_Progression_F8",240,0.,240.);
    TH1F* pt_Progression_G8 = new TH1F("pt_Progression_G8","pt_Progression_G8",240,0.,240.);
    TH1F* pt_Progression_H8 = new TH1F("pt_Progression_H8","pt_Progression_H8",240,0.,240.);
    TH1F* pt_Progression_I8 = new TH1F("pt_Progression_I8","pt_Progression_I8",240,0.,240.);
    TH1F* pt_Progression_J8 = new TH1F("pt_Progression_J8","pt_Progression_J8",240,0.,240.);
    TH1F* pt_Progression_K8 = new TH1F("pt_Progression_K8","pt_Progression_K8",240,0.,240.);
    TH1F* pt_Progression_L8 = new TH1F("pt_Progression_L8","pt_Progression_L8",240,0.,240.);
    TH1F* pt_Progression_M8 = new TH1F("pt_Progression_M8","pt_Progression_M8",240,0.,240.);
    TH1F* pt_Progression_N8 = new TH1F("pt_Progression_N8","pt_Progression_N8",240,0.,240.);
    TH1F* pt_Progression_O8 = new TH1F("pt_Progression_O8","pt_Progression_O8",240,0.,240.);
    TH1F* pt_Progression_P8 = new TH1F("pt_Progression_P8","pt_Progression_P8",240,0.,240.);
    TH1F* pt_Progression_Q8 = new TH1F("pt_Progression_Q8","pt_Progression_Q8",240,0.,240.);
    TH1F* pt_Progression_R8 = new TH1F("pt_Progression_R8","pt_Progression_R8",240,0.,240.);
    TH1F* pt_Progression_S8 = new TH1F("pt_Progression_S8","pt_Progression_S8",240,0.,240.);
    TH1F* pt_Progression_T8 = new TH1F("pt_Progression_T8","pt_Progression_T8",240,0.,240.);
    TH1F* pt_Progression_U8 = new TH1F("pt_Progression_U8","pt_Progression_U8",240,0.,240.);
    TH1F* pt_Progression_V8 = new TH1F("pt_Progression_V8","pt_Progression_V8",240,0.,240.);
    TH1F* pt_Progression_W8 = new TH1F("pt_Progression_W8","pt_Progression_W8",240,0.,240.);
    TH1F* pt_Progression_X8 = new TH1F("pt_Progression_X8","pt_Progression_X8",240,0.,240.);
    TH1F* pt_Progression_Y8 = new TH1F("pt_Progression_Y8","pt_Progression_Y8",240,0.,240.);
    TH1F* pt_Progression_Z8 = new TH1F("pt_Progression_Z8","pt_Progression_Z8",240,0.,240.);
    // END OF FULL BLOWN GRID SEARCH

    TH2F* pt_DiTau_Progression_1 = new TH2F("pt_DiTau_Progression_1","pt_DiTau_Progression_1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_2 = new TH2F("pt_DiTau_Progression_2","pt_DiTau_Progression_2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_3 = new TH2F("pt_DiTau_Progression_3","pt_DiTau_Progression_3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_4 = new TH2F("pt_DiTau_Progression_4","pt_DiTau_Progression_4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_5 = new TH2F("pt_DiTau_Progression_5","pt_DiTau_Progression_5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_6 = new TH2F("pt_DiTau_Progression_6","pt_DiTau_Progression_6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_7 = new TH2F("pt_DiTau_Progression_7","pt_DiTau_Progression_7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_8 = new TH2F("pt_DiTau_Progression_8","pt_DiTau_Progression_8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_9 = new TH2F("pt_DiTau_Progression_9","pt_DiTau_Progression_9",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_10 = new TH2F("pt_DiTau_Progression_10","pt_DiTau_Progression_10",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_11 = new TH2F("pt_DiTau_Progression_11","pt_DiTau_Progression_11",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_12 = new TH2F("pt_DiTau_Progression_12","pt_DiTau_Progression_12",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_13 = new TH2F("pt_DiTau_Progression_13","pt_DiTau_Progression_13",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_14 = new TH2F("pt_DiTau_Progression_14","pt_DiTau_Progression_14",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_15 = new TH2F("pt_DiTau_Progression_15","pt_DiTau_Progression_15",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_16 = new TH2F("pt_DiTau_Progression_16","pt_DiTau_Progression_16",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_17 = new TH2F("pt_DiTau_Progression_17","pt_DiTau_Progression_17",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_18 = new TH2F("pt_DiTau_Progression_18","pt_DiTau_Progression_18",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_19 = new TH2F("pt_DiTau_Progression_19","pt_DiTau_Progression_19",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_20 = new TH2F("pt_DiTau_Progression_20","pt_DiTau_Progression_20",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_21 = new TH2F("pt_DiTau_Progression_21","pt_DiTau_Progression_21",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_21_barrel = new TH2F("pt_DiTau_Progression_21_barrel","pt_DiTau_Progression_21_barrel",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_21_endcaps = new TH2F("pt_DiTau_Progression_21_endcaps","pt_DiTau_Progression_21_endcaps",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_22 = new TH2F("pt_DiTau_Progression_22","pt_DiTau_Progression_22",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_22_barrel = new TH2F("pt_DiTau_Progression_22_barrel","pt_DiTau_Progression_22_barrel",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_22_barrelendcap = new TH2F("pt_DiTau_Progression_22_barrelendcap","pt_DiTau_Progression_22_barrelendcap",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_22_endcaps = new TH2F("pt_DiTau_Progression_22_endcaps","pt_DiTau_Progression_22_endcaps",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_22_BothAreMerged = new TH2F("pt_DiTau_Progression_22_BothAreMerged","pt_DiTau_Progression_22_BothAreMerged",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_22_OneIsMerged = new TH2F("pt_DiTau_Progression_22_OneIsMerged","pt_DiTau_Progression_22_OneIsMerged",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_22_BothAreNotMerged = new TH2F("pt_DiTau_Progression_22_BothAreNotMerged","pt_DiTau_Progression_22_BothAreNotMerged",240,0.,240.,240,0.,240.);//

    TH2F* pt_DiTau_Progression_23 = new TH2F("pt_DiTau_Progression_23","pt_DiTau_Progression_23",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_24 = new TH2F("pt_DiTau_Progression_24","pt_DiTau_Progression_24",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_25 = new TH2F("pt_DiTau_Progression_25","pt_DiTau_Progression_25",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_26 = new TH2F("pt_DiTau_Progression_26","pt_DiTau_Progression_26",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_27 = new TH2F("pt_DiTau_Progression_27","pt_DiTau_Progression_27",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_28 = new TH2F("pt_DiTau_Progression_28","pt_DiTau_Progression_28",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_29 = new TH2F("pt_DiTau_Progression_29","pt_DiTau_Progression_29",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_30 = new TH2F("pt_DiTau_Progression_30","pt_DiTau_Progression_30",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_31 = new TH2F("pt_DiTau_Progression_31","pt_DiTau_Progression_31",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_31_barrel = new TH2F("pt_DiTau_Progression_31_barrel","pt_DiTau_Progression_31_barrel",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_31_barrelendcap = new TH2F("pt_DiTau_Progression_31_barrelendcap","pt_DiTau_Progression_31_barrelendcap",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_31_endcaps = new TH2F("pt_DiTau_Progression_31_endcaps","pt_DiTau_Progression_31_endcaps",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_31_extrap = new TH2F("pt_DiTau_Progression_31_extrap","pt_DiTau_Progression_31_extrap",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_31_extrap_barrel = new TH2F("pt_DiTau_Progression_31_extrap_barrel","pt_DiTau_Progression_31_extrap_barrel",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_31_extrap_barrelendcap = new TH2F("pt_DiTau_Progression_31_extrap_barrelendcap","pt_DiTau_Progression_31_extrap_barrelendcap",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_31_extrap_endcaps = new TH2F("pt_DiTau_Progression_31_extrap_endcaps","pt_DiTau_Progression_31_extrap_endcaps",240,0.,240.,240,0.,240.);//

    // START OF FULL BLOWN GRID SEARCH
    TH2F* pt_DiTau_Progression_A0 = new TH2F("pt_DiTau_Progression_A0","pt_DiTau_Progression_A0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_B0 = new TH2F("pt_DiTau_Progression_B0","pt_DiTau_Progression_B0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_C0 = new TH2F("pt_DiTau_Progression_C0","pt_DiTau_Progression_C0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_D0 = new TH2F("pt_DiTau_Progression_D0","pt_DiTau_Progression_D0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_E0 = new TH2F("pt_DiTau_Progression_E0","pt_DiTau_Progression_E0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_F0 = new TH2F("pt_DiTau_Progression_F0","pt_DiTau_Progression_F0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_G0 = new TH2F("pt_DiTau_Progression_G0","pt_DiTau_Progression_G0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_H0 = new TH2F("pt_DiTau_Progression_H0","pt_DiTau_Progression_H0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_I0 = new TH2F("pt_DiTau_Progression_I0","pt_DiTau_Progression_I0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_J0 = new TH2F("pt_DiTau_Progression_J0","pt_DiTau_Progression_J0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_K0 = new TH2F("pt_DiTau_Progression_K0","pt_DiTau_Progression_K0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_L0 = new TH2F("pt_DiTau_Progression_L0","pt_DiTau_Progression_L0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_M0 = new TH2F("pt_DiTau_Progression_M0","pt_DiTau_Progression_M0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_N0 = new TH2F("pt_DiTau_Progression_N0","pt_DiTau_Progression_N0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_O0 = new TH2F("pt_DiTau_Progression_O0","pt_DiTau_Progression_O0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_P0 = new TH2F("pt_DiTau_Progression_P0","pt_DiTau_Progression_P0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Q0 = new TH2F("pt_DiTau_Progression_Q0","pt_DiTau_Progression_Q0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_R0 = new TH2F("pt_DiTau_Progression_R0","pt_DiTau_Progression_R0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_S0 = new TH2F("pt_DiTau_Progression_S0","pt_DiTau_Progression_S0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_T0 = new TH2F("pt_DiTau_Progression_T0","pt_DiTau_Progression_T0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_U0 = new TH2F("pt_DiTau_Progression_U0","pt_DiTau_Progression_U0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_V0 = new TH2F("pt_DiTau_Progression_V0","pt_DiTau_Progression_V0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_W0 = new TH2F("pt_DiTau_Progression_W0","pt_DiTau_Progression_W0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_X0 = new TH2F("pt_DiTau_Progression_X0","pt_DiTau_Progression_X0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Y0 = new TH2F("pt_DiTau_Progression_Y0","pt_DiTau_Progression_Y0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Z0 = new TH2F("pt_DiTau_Progression_Z0","pt_DiTau_Progression_Z0",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_A1 = new TH2F("pt_DiTau_Progression_A1","pt_DiTau_Progression_A1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_B1 = new TH2F("pt_DiTau_Progression_B1","pt_DiTau_Progression_B1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_C1 = new TH2F("pt_DiTau_Progression_C1","pt_DiTau_Progression_C1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_D1 = new TH2F("pt_DiTau_Progression_D1","pt_DiTau_Progression_D1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_E1 = new TH2F("pt_DiTau_Progression_E1","pt_DiTau_Progression_E1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_F1 = new TH2F("pt_DiTau_Progression_F1","pt_DiTau_Progression_F1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_G1 = new TH2F("pt_DiTau_Progression_G1","pt_DiTau_Progression_G1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_H1 = new TH2F("pt_DiTau_Progression_H1","pt_DiTau_Progression_H1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_I1 = new TH2F("pt_DiTau_Progression_I1","pt_DiTau_Progression_I1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_J1 = new TH2F("pt_DiTau_Progression_J1","pt_DiTau_Progression_J1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_K1 = new TH2F("pt_DiTau_Progression_K1","pt_DiTau_Progression_K1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_L1 = new TH2F("pt_DiTau_Progression_L1","pt_DiTau_Progression_L1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_M1 = new TH2F("pt_DiTau_Progression_M1","pt_DiTau_Progression_M1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_N1 = new TH2F("pt_DiTau_Progression_N1","pt_DiTau_Progression_N1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_O1 = new TH2F("pt_DiTau_Progression_O1","pt_DiTau_Progression_O1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_P1 = new TH2F("pt_DiTau_Progression_P1","pt_DiTau_Progression_P1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Q1 = new TH2F("pt_DiTau_Progression_Q1","pt_DiTau_Progression_Q1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_R1 = new TH2F("pt_DiTau_Progression_R1","pt_DiTau_Progression_R1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_S1 = new TH2F("pt_DiTau_Progression_S1","pt_DiTau_Progression_S1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_T1 = new TH2F("pt_DiTau_Progression_T1","pt_DiTau_Progression_T1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_U1 = new TH2F("pt_DiTau_Progression_U1","pt_DiTau_Progression_U1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_V1 = new TH2F("pt_DiTau_Progression_V1","pt_DiTau_Progression_V1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_W1 = new TH2F("pt_DiTau_Progression_W1","pt_DiTau_Progression_W1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_X1 = new TH2F("pt_DiTau_Progression_X1","pt_DiTau_Progression_X1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Y1 = new TH2F("pt_DiTau_Progression_Y1","pt_DiTau_Progression_Y1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Z1 = new TH2F("pt_DiTau_Progression_Z1","pt_DiTau_Progression_Z1",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_A2 = new TH2F("pt_DiTau_Progression_A2","pt_DiTau_Progression_A2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_B2 = new TH2F("pt_DiTau_Progression_B2","pt_DiTau_Progression_B2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_C2 = new TH2F("pt_DiTau_Progression_C2","pt_DiTau_Progression_C2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_D2 = new TH2F("pt_DiTau_Progression_D2","pt_DiTau_Progression_D2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_E2 = new TH2F("pt_DiTau_Progression_E2","pt_DiTau_Progression_E2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_F2 = new TH2F("pt_DiTau_Progression_F2","pt_DiTau_Progression_F2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_G2 = new TH2F("pt_DiTau_Progression_G2","pt_DiTau_Progression_G2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_H2 = new TH2F("pt_DiTau_Progression_H2","pt_DiTau_Progression_H2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_I2 = new TH2F("pt_DiTau_Progression_I2","pt_DiTau_Progression_I2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_J2 = new TH2F("pt_DiTau_Progression_J2","pt_DiTau_Progression_J2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_K2 = new TH2F("pt_DiTau_Progression_K2","pt_DiTau_Progression_K2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_L2 = new TH2F("pt_DiTau_Progression_L2","pt_DiTau_Progression_L2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_M2 = new TH2F("pt_DiTau_Progression_M2","pt_DiTau_Progression_M2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_N2 = new TH2F("pt_DiTau_Progression_N2","pt_DiTau_Progression_N2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_O2 = new TH2F("pt_DiTau_Progression_O2","pt_DiTau_Progression_O2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_P2 = new TH2F("pt_DiTau_Progression_P2","pt_DiTau_Progression_P2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Q2 = new TH2F("pt_DiTau_Progression_Q2","pt_DiTau_Progression_Q2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_R2 = new TH2F("pt_DiTau_Progression_R2","pt_DiTau_Progression_R2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_S2 = new TH2F("pt_DiTau_Progression_S2","pt_DiTau_Progression_S2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_T2 = new TH2F("pt_DiTau_Progression_T2","pt_DiTau_Progression_T2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_U2 = new TH2F("pt_DiTau_Progression_U2","pt_DiTau_Progression_U2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_V2 = new TH2F("pt_DiTau_Progression_V2","pt_DiTau_Progression_V2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_W2 = new TH2F("pt_DiTau_Progression_W2","pt_DiTau_Progression_W2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_X2 = new TH2F("pt_DiTau_Progression_X2","pt_DiTau_Progression_X2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Y2 = new TH2F("pt_DiTau_Progression_Y2","pt_DiTau_Progression_Y2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Z2 = new TH2F("pt_DiTau_Progression_Z2","pt_DiTau_Progression_Z2",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_A3 = new TH2F("pt_DiTau_Progression_A3","pt_DiTau_Progression_A3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_B3 = new TH2F("pt_DiTau_Progression_B3","pt_DiTau_Progression_B3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_C3 = new TH2F("pt_DiTau_Progression_C3","pt_DiTau_Progression_C3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_D3 = new TH2F("pt_DiTau_Progression_D3","pt_DiTau_Progression_D3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_E3 = new TH2F("pt_DiTau_Progression_E3","pt_DiTau_Progression_E3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_F3 = new TH2F("pt_DiTau_Progression_F3","pt_DiTau_Progression_F3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_G3 = new TH2F("pt_DiTau_Progression_G3","pt_DiTau_Progression_G3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_H3 = new TH2F("pt_DiTau_Progression_H3","pt_DiTau_Progression_H3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_I3 = new TH2F("pt_DiTau_Progression_I3","pt_DiTau_Progression_I3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_J3 = new TH2F("pt_DiTau_Progression_J3","pt_DiTau_Progression_J3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_K3 = new TH2F("pt_DiTau_Progression_K3","pt_DiTau_Progression_K3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_L3 = new TH2F("pt_DiTau_Progression_L3","pt_DiTau_Progression_L3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_M3 = new TH2F("pt_DiTau_Progression_M3","pt_DiTau_Progression_M3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_N3 = new TH2F("pt_DiTau_Progression_N3","pt_DiTau_Progression_N3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_O3 = new TH2F("pt_DiTau_Progression_O3","pt_DiTau_Progression_O3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_P3 = new TH2F("pt_DiTau_Progression_P3","pt_DiTau_Progression_P3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Q3 = new TH2F("pt_DiTau_Progression_Q3","pt_DiTau_Progression_Q3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_R3 = new TH2F("pt_DiTau_Progression_R3","pt_DiTau_Progression_R3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_S3 = new TH2F("pt_DiTau_Progression_S3","pt_DiTau_Progression_S3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_T3 = new TH2F("pt_DiTau_Progression_T3","pt_DiTau_Progression_T3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_U3 = new TH2F("pt_DiTau_Progression_U3","pt_DiTau_Progression_U3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_V3 = new TH2F("pt_DiTau_Progression_V3","pt_DiTau_Progression_V3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_W3 = new TH2F("pt_DiTau_Progression_W3","pt_DiTau_Progression_W3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_X3 = new TH2F("pt_DiTau_Progression_X3","pt_DiTau_Progression_X3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Y3 = new TH2F("pt_DiTau_Progression_Y3","pt_DiTau_Progression_Y3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Z3 = new TH2F("pt_DiTau_Progression_Z3","pt_DiTau_Progression_Z3",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_A4 = new TH2F("pt_DiTau_Progression_A4","pt_DiTau_Progression_A4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_B4 = new TH2F("pt_DiTau_Progression_B4","pt_DiTau_Progression_B4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_C4 = new TH2F("pt_DiTau_Progression_C4","pt_DiTau_Progression_C4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_D4 = new TH2F("pt_DiTau_Progression_D4","pt_DiTau_Progression_D4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_E4 = new TH2F("pt_DiTau_Progression_E4","pt_DiTau_Progression_E4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_F4 = new TH2F("pt_DiTau_Progression_F4","pt_DiTau_Progression_F4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_G4 = new TH2F("pt_DiTau_Progression_G4","pt_DiTau_Progression_G4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_H4 = new TH2F("pt_DiTau_Progression_H4","pt_DiTau_Progression_H4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_I4 = new TH2F("pt_DiTau_Progression_I4","pt_DiTau_Progression_I4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_J4 = new TH2F("pt_DiTau_Progression_J4","pt_DiTau_Progression_J4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_K4 = new TH2F("pt_DiTau_Progression_K4","pt_DiTau_Progression_K4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_L4 = new TH2F("pt_DiTau_Progression_L4","pt_DiTau_Progression_L4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_M4 = new TH2F("pt_DiTau_Progression_M4","pt_DiTau_Progression_M4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_N4 = new TH2F("pt_DiTau_Progression_N4","pt_DiTau_Progression_N4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_O4 = new TH2F("pt_DiTau_Progression_O4","pt_DiTau_Progression_O4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_P4 = new TH2F("pt_DiTau_Progression_P4","pt_DiTau_Progression_P4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Q4 = new TH2F("pt_DiTau_Progression_Q4","pt_DiTau_Progression_Q4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_R4 = new TH2F("pt_DiTau_Progression_R4","pt_DiTau_Progression_R4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_S4 = new TH2F("pt_DiTau_Progression_S4","pt_DiTau_Progression_S4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_T4 = new TH2F("pt_DiTau_Progression_T4","pt_DiTau_Progression_T4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_U4 = new TH2F("pt_DiTau_Progression_U4","pt_DiTau_Progression_U4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_V4 = new TH2F("pt_DiTau_Progression_V4","pt_DiTau_Progression_V4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_W4 = new TH2F("pt_DiTau_Progression_W4","pt_DiTau_Progression_W4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_X4 = new TH2F("pt_DiTau_Progression_X4","pt_DiTau_Progression_X4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Y4 = new TH2F("pt_DiTau_Progression_Y4","pt_DiTau_Progression_Y4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Z4 = new TH2F("pt_DiTau_Progression_Z4","pt_DiTau_Progression_Z4",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_A5 = new TH2F("pt_DiTau_Progression_A5","pt_DiTau_Progression_A5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_B5 = new TH2F("pt_DiTau_Progression_B5","pt_DiTau_Progression_B5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_C5 = new TH2F("pt_DiTau_Progression_C5","pt_DiTau_Progression_C5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_D5 = new TH2F("pt_DiTau_Progression_D5","pt_DiTau_Progression_D5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_E5 = new TH2F("pt_DiTau_Progression_E5","pt_DiTau_Progression_E5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_F5 = new TH2F("pt_DiTau_Progression_F5","pt_DiTau_Progression_F5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_G5 = new TH2F("pt_DiTau_Progression_G5","pt_DiTau_Progression_G5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_H5 = new TH2F("pt_DiTau_Progression_H5","pt_DiTau_Progression_H5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_I5 = new TH2F("pt_DiTau_Progression_I5","pt_DiTau_Progression_I5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_J5 = new TH2F("pt_DiTau_Progression_J5","pt_DiTau_Progression_J5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_K5 = new TH2F("pt_DiTau_Progression_K5","pt_DiTau_Progression_K5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_L5 = new TH2F("pt_DiTau_Progression_L5","pt_DiTau_Progression_L5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_M5 = new TH2F("pt_DiTau_Progression_M5","pt_DiTau_Progression_M5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_N5 = new TH2F("pt_DiTau_Progression_N5","pt_DiTau_Progression_N5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_O5 = new TH2F("pt_DiTau_Progression_O5","pt_DiTau_Progression_O5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_P5 = new TH2F("pt_DiTau_Progression_P5","pt_DiTau_Progression_P5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Q5 = new TH2F("pt_DiTau_Progression_Q5","pt_DiTau_Progression_Q5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_R5 = new TH2F("pt_DiTau_Progression_R5","pt_DiTau_Progression_R5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_S5 = new TH2F("pt_DiTau_Progression_S5","pt_DiTau_Progression_S5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_T5 = new TH2F("pt_DiTau_Progression_T5","pt_DiTau_Progression_T5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_U5 = new TH2F("pt_DiTau_Progression_U5","pt_DiTau_Progression_U5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_V5 = new TH2F("pt_DiTau_Progression_V5","pt_DiTau_Progression_V5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_W5 = new TH2F("pt_DiTau_Progression_W5","pt_DiTau_Progression_W5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_X5 = new TH2F("pt_DiTau_Progression_X5","pt_DiTau_Progression_X5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Y5 = new TH2F("pt_DiTau_Progression_Y5","pt_DiTau_Progression_Y5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Z5 = new TH2F("pt_DiTau_Progression_Z5","pt_DiTau_Progression_Z5",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_A6 = new TH2F("pt_DiTau_Progression_A6","pt_DiTau_Progression_A6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_B6 = new TH2F("pt_DiTau_Progression_B6","pt_DiTau_Progression_B6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_C6 = new TH2F("pt_DiTau_Progression_C6","pt_DiTau_Progression_C6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_D6 = new TH2F("pt_DiTau_Progression_D6","pt_DiTau_Progression_D6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_E6 = new TH2F("pt_DiTau_Progression_E6","pt_DiTau_Progression_E6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_F6 = new TH2F("pt_DiTau_Progression_F6","pt_DiTau_Progression_F6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_G6 = new TH2F("pt_DiTau_Progression_G6","pt_DiTau_Progression_G6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_H6 = new TH2F("pt_DiTau_Progression_H6","pt_DiTau_Progression_H6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_I6 = new TH2F("pt_DiTau_Progression_I6","pt_DiTau_Progression_I6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_J6 = new TH2F("pt_DiTau_Progression_J6","pt_DiTau_Progression_J6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_K6 = new TH2F("pt_DiTau_Progression_K6","pt_DiTau_Progression_K6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_L6 = new TH2F("pt_DiTau_Progression_L6","pt_DiTau_Progression_L6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_M6 = new TH2F("pt_DiTau_Progression_M6","pt_DiTau_Progression_M6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_N6 = new TH2F("pt_DiTau_Progression_N6","pt_DiTau_Progression_N6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_O6 = new TH2F("pt_DiTau_Progression_O6","pt_DiTau_Progression_O6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_P6 = new TH2F("pt_DiTau_Progression_P6","pt_DiTau_Progression_P6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Q6 = new TH2F("pt_DiTau_Progression_Q6","pt_DiTau_Progression_Q6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_R6 = new TH2F("pt_DiTau_Progression_R6","pt_DiTau_Progression_R6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_S6 = new TH2F("pt_DiTau_Progression_S6","pt_DiTau_Progression_S6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_T6 = new TH2F("pt_DiTau_Progression_T6","pt_DiTau_Progression_T6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_U6 = new TH2F("pt_DiTau_Progression_U6","pt_DiTau_Progression_U6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_V6 = new TH2F("pt_DiTau_Progression_V6","pt_DiTau_Progression_V6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_W6 = new TH2F("pt_DiTau_Progression_W6","pt_DiTau_Progression_W6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_X6 = new TH2F("pt_DiTau_Progression_X6","pt_DiTau_Progression_X6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Y6 = new TH2F("pt_DiTau_Progression_Y6","pt_DiTau_Progression_Y6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Z6 = new TH2F("pt_DiTau_Progression_Z6","pt_DiTau_Progression_Z6",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_A7 = new TH2F("pt_DiTau_Progression_A7","pt_DiTau_Progression_A7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_B7 = new TH2F("pt_DiTau_Progression_B7","pt_DiTau_Progression_B7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_C7 = new TH2F("pt_DiTau_Progression_C7","pt_DiTau_Progression_C7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_D7 = new TH2F("pt_DiTau_Progression_D7","pt_DiTau_Progression_D7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_E7 = new TH2F("pt_DiTau_Progression_E7","pt_DiTau_Progression_E7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_F7 = new TH2F("pt_DiTau_Progression_F7","pt_DiTau_Progression_F7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_G7 = new TH2F("pt_DiTau_Progression_G7","pt_DiTau_Progression_G7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_H7 = new TH2F("pt_DiTau_Progression_H7","pt_DiTau_Progression_H7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_I7 = new TH2F("pt_DiTau_Progression_I7","pt_DiTau_Progression_I7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_J7 = new TH2F("pt_DiTau_Progression_J7","pt_DiTau_Progression_J7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_K7 = new TH2F("pt_DiTau_Progression_K7","pt_DiTau_Progression_K7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_L7 = new TH2F("pt_DiTau_Progression_L7","pt_DiTau_Progression_L7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_M7 = new TH2F("pt_DiTau_Progression_M7","pt_DiTau_Progression_M7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_N7 = new TH2F("pt_DiTau_Progression_N7","pt_DiTau_Progression_N7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_O7 = new TH2F("pt_DiTau_Progression_O7","pt_DiTau_Progression_O7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_P7 = new TH2F("pt_DiTau_Progression_P7","pt_DiTau_Progression_P7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Q7 = new TH2F("pt_DiTau_Progression_Q7","pt_DiTau_Progression_Q7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_R7 = new TH2F("pt_DiTau_Progression_R7","pt_DiTau_Progression_R7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_S7 = new TH2F("pt_DiTau_Progression_S7","pt_DiTau_Progression_S7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_T7 = new TH2F("pt_DiTau_Progression_T7","pt_DiTau_Progression_T7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_U7 = new TH2F("pt_DiTau_Progression_U7","pt_DiTau_Progression_U7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_V7 = new TH2F("pt_DiTau_Progression_V7","pt_DiTau_Progression_V7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_W7 = new TH2F("pt_DiTau_Progression_W7","pt_DiTau_Progression_W7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_X7 = new TH2F("pt_DiTau_Progression_X7","pt_DiTau_Progression_X7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Y7 = new TH2F("pt_DiTau_Progression_Y7","pt_DiTau_Progression_Y7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Z7 = new TH2F("pt_DiTau_Progression_Z7","pt_DiTau_Progression_Z7",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_A8 = new TH2F("pt_DiTau_Progression_A8","pt_DiTau_Progression_A8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_B8 = new TH2F("pt_DiTau_Progression_B8","pt_DiTau_Progression_B8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_C8 = new TH2F("pt_DiTau_Progression_C8","pt_DiTau_Progression_C8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_D8 = new TH2F("pt_DiTau_Progression_D8","pt_DiTau_Progression_D8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_E8 = new TH2F("pt_DiTau_Progression_E8","pt_DiTau_Progression_E8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_F8 = new TH2F("pt_DiTau_Progression_F8","pt_DiTau_Progression_F8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_G8 = new TH2F("pt_DiTau_Progression_G8","pt_DiTau_Progression_G8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_H8 = new TH2F("pt_DiTau_Progression_H8","pt_DiTau_Progression_H8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_I8 = new TH2F("pt_DiTau_Progression_I8","pt_DiTau_Progression_I8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_J8 = new TH2F("pt_DiTau_Progression_J8","pt_DiTau_Progression_J8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_K8 = new TH2F("pt_DiTau_Progression_K8","pt_DiTau_Progression_K8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_L8 = new TH2F("pt_DiTau_Progression_L8","pt_DiTau_Progression_L8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_M8 = new TH2F("pt_DiTau_Progression_M8","pt_DiTau_Progression_M8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_N8 = new TH2F("pt_DiTau_Progression_N8","pt_DiTau_Progression_N8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_O8 = new TH2F("pt_DiTau_Progression_O8","pt_DiTau_Progression_O8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_P8 = new TH2F("pt_DiTau_Progression_P8","pt_DiTau_Progression_P8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Q8 = new TH2F("pt_DiTau_Progression_Q8","pt_DiTau_Progression_Q8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_R8 = new TH2F("pt_DiTau_Progression_R8","pt_DiTau_Progression_R8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_S8 = new TH2F("pt_DiTau_Progression_S8","pt_DiTau_Progression_S8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_T8 = new TH2F("pt_DiTau_Progression_T8","pt_DiTau_Progression_T8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_U8 = new TH2F("pt_DiTau_Progression_U8","pt_DiTau_Progression_U8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_V8 = new TH2F("pt_DiTau_Progression_V8","pt_DiTau_Progression_V8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_W8 = new TH2F("pt_DiTau_Progression_W8","pt_DiTau_Progression_W8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_X8 = new TH2F("pt_DiTau_Progression_X8","pt_DiTau_Progression_X8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Y8 = new TH2F("pt_DiTau_Progression_Y8","pt_DiTau_Progression_Y8",240,0.,240.,240,0.,240.);//
    TH2F* pt_DiTau_Progression_Z8 = new TH2F("pt_DiTau_Progression_Z8","pt_DiTau_Progression_Z8",240,0.,240.,240,0.,240.);//
    // END OF FULL BLOWN GRID SEARCH


    TH1F* pt_Stage1 = new TH1F("pt_Stage1","pt_Stage1",240,0.,240.);

    TH1F* Iso_MinBias = new TH1F("Iso_MinBias","Iso_MinBias",20,0.,20.);
    TH1F* Correction_Factor = new TH1F("Correction_Factor","Correction_Factor",100,0.,3.);
    TH1F* Correction_Factor_IEt_30 = new TH1F("Correction_Factor_IEt_30","Correction_Factor_IEt_30",100,0.,3.);

    Int_t Denominator = 0;

    cout<<"begin loop"<<endl;


    map<int, int> remap;
    remap[0] = 6 ;
    remap[1] = 5 ;
    remap[2] = 1 ;
    remap[3] = 0 ;
    remap[4] = 4 ;
    remap[5] = 3 ;
    remap[6] = 2 ;


    //for(UInt_t i = 0 ; i < 2000000 ; ++i)

    int CounterPass = 0;
    int CounterFail = 0;
    int CounterAll = 0;

    int DiTauCounterPass = 0;
    int DiTauCounterFail = 0;
    int DiTauCounterAll = 0;

    // for(UInt_t i = 0 ; i < 3000000 ; ++i)
    for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
        {
            inTree->GetEntry(i);
            if(i%10000==0) cout<<"Entry #"<<i<<endl; 
            // SET RUN INFO
            if (run == 323755) { if(in_lumi<44 || in_lumi>544) continue; } // Run323755
            if (run == 323775) { if(in_lumi<53 || in_lumi>171 || in_lumi==82 || in_lumi==83) continue; } // Run323775 --> very smal numer of lumisections!!

            // if(in_lumi<60 || in_lumi>455) continue;

            //if(in_lumi<48 || in_lumi>221) continue;
            // if(in_lumi<48 || in_lumi>530) continue;

            // if(PU_per_LS.find(in_lumi)==PU_per_LS.end()) continue;
            // Float_t weight = PU_per_LS[48]/PU_per_LS[in_lumi];
            // if(weight<0.5) cout<<"lumi = "<<in_lumi<<", weight = "<<weight<<endl;
            // if(weight>2) cout<<"lumi = "<<in_lumi<<", weight = "<<weight<<endl;
            Float_t weight = 1.;

            //if(lumi<81||lumi>300) continue;
            ++Denominator;
                        
            bool Filled_IsoInf = kFALSE;
            bool Filled_Iso_50 = kFALSE;
            bool Filled_Iso_60 = kFALSE;
            bool Filled_Iso_70 = kFALSE;
            bool Filled_Iso_80 = kFALSE;
            bool Filled_Iso_90 = kFALSE;

            bool Filled_Progression_1 = kFALSE;
            bool Filled_Progression_2 = kFALSE;
            bool Filled_Progression_3 = kFALSE;
            bool Filled_Progression_4 = kFALSE;
            bool Filled_Progression_5 = kFALSE;
            bool Filled_Progression_6 = kFALSE;
            bool Filled_Progression_7 = kFALSE;
            bool Filled_Progression_8 = kFALSE;
            bool Filled_Progression_9 = kFALSE;
            bool Filled_Progression_10 = kFALSE;
            bool Filled_Progression_11 = kFALSE;
            bool Filled_Progression_12 = kFALSE;
            bool Filled_Progression_13 = kFALSE;
            bool Filled_Progression_14 = kFALSE;
            bool Filled_Progression_15 = kFALSE;
            bool Filled_Progression_16 = kFALSE;
            bool Filled_Progression_17 = kFALSE;
            bool Filled_Progression_18 = kFALSE;
            bool Filled_Progression_19 = kFALSE;
            bool Filled_Progression_20 = kFALSE;
            bool Filled_Progression_21 = kFALSE;
            bool Filled_Progression_22 = kFALSE;

            // START OF FULL BLOWN GRID SEARCH
            bool Filled_Progression_A0 = kFALSE;
            bool Filled_Progression_B0 = kFALSE;
            bool Filled_Progression_C0 = kFALSE;
            bool Filled_Progression_D0 = kFALSE;
            bool Filled_Progression_E0 = kFALSE;
            bool Filled_Progression_F0 = kFALSE;
            bool Filled_Progression_G0 = kFALSE;
            bool Filled_Progression_H0 = kFALSE;
            bool Filled_Progression_I0 = kFALSE;
            bool Filled_Progression_J0 = kFALSE;
            bool Filled_Progression_K0 = kFALSE;
            bool Filled_Progression_L0 = kFALSE;
            bool Filled_Progression_M0 = kFALSE;
            bool Filled_Progression_N0 = kFALSE;
            bool Filled_Progression_O0 = kFALSE;
            bool Filled_Progression_P0 = kFALSE;
            bool Filled_Progression_Q0 = kFALSE;
            bool Filled_Progression_R0 = kFALSE;
            bool Filled_Progression_S0 = kFALSE;
            bool Filled_Progression_T0 = kFALSE;
            bool Filled_Progression_U0 = kFALSE;
            bool Filled_Progression_V0 = kFALSE;
            bool Filled_Progression_W0 = kFALSE;
            bool Filled_Progression_X0 = kFALSE;
            bool Filled_Progression_Y0 = kFALSE;
            bool Filled_Progression_Z0 = kFALSE;
            bool Filled_Progression_A1 = kFALSE;
            bool Filled_Progression_B1 = kFALSE;
            bool Filled_Progression_C1 = kFALSE;
            bool Filled_Progression_D1 = kFALSE;
            bool Filled_Progression_E1 = kFALSE;
            bool Filled_Progression_F1 = kFALSE;
            bool Filled_Progression_G1 = kFALSE;
            bool Filled_Progression_H1 = kFALSE;
            bool Filled_Progression_I1 = kFALSE;
            bool Filled_Progression_J1 = kFALSE;
            bool Filled_Progression_K1 = kFALSE;
            bool Filled_Progression_L1 = kFALSE;
            bool Filled_Progression_M1 = kFALSE;
            bool Filled_Progression_N1 = kFALSE;
            bool Filled_Progression_O1 = kFALSE;
            bool Filled_Progression_P1 = kFALSE;
            bool Filled_Progression_Q1 = kFALSE;
            bool Filled_Progression_R1 = kFALSE;
            bool Filled_Progression_S1 = kFALSE;
            bool Filled_Progression_T1 = kFALSE;
            bool Filled_Progression_U1 = kFALSE;
            bool Filled_Progression_V1 = kFALSE;
            bool Filled_Progression_W1 = kFALSE;
            bool Filled_Progression_X1 = kFALSE;
            bool Filled_Progression_Y1 = kFALSE;
            bool Filled_Progression_Z1 = kFALSE;
            bool Filled_Progression_A2 = kFALSE;
            bool Filled_Progression_B2 = kFALSE;
            bool Filled_Progression_C2 = kFALSE;
            bool Filled_Progression_D2 = kFALSE;
            bool Filled_Progression_E2 = kFALSE;
            bool Filled_Progression_F2 = kFALSE;
            bool Filled_Progression_G2 = kFALSE;
            bool Filled_Progression_H2 = kFALSE;
            bool Filled_Progression_I2 = kFALSE;
            bool Filled_Progression_J2 = kFALSE;
            bool Filled_Progression_K2 = kFALSE;
            bool Filled_Progression_L2 = kFALSE;
            bool Filled_Progression_M2 = kFALSE;
            bool Filled_Progression_N2 = kFALSE;
            bool Filled_Progression_O2 = kFALSE;
            bool Filled_Progression_P2 = kFALSE;
            bool Filled_Progression_Q2 = kFALSE;
            bool Filled_Progression_R2 = kFALSE;
            bool Filled_Progression_S2 = kFALSE;
            bool Filled_Progression_T2 = kFALSE;
            bool Filled_Progression_U2 = kFALSE;
            bool Filled_Progression_V2 = kFALSE;
            bool Filled_Progression_W2 = kFALSE;
            bool Filled_Progression_X2 = kFALSE;
            bool Filled_Progression_Y2 = kFALSE;
            bool Filled_Progression_Z2 = kFALSE;
            bool Filled_Progression_A3 = kFALSE;
            bool Filled_Progression_B3 = kFALSE;
            bool Filled_Progression_C3 = kFALSE;
            bool Filled_Progression_D3 = kFALSE;
            bool Filled_Progression_E3 = kFALSE;
            bool Filled_Progression_F3 = kFALSE;
            bool Filled_Progression_G3 = kFALSE;
            bool Filled_Progression_H3 = kFALSE;
            bool Filled_Progression_I3 = kFALSE;
            bool Filled_Progression_J3 = kFALSE;
            bool Filled_Progression_K3 = kFALSE;
            bool Filled_Progression_L3 = kFALSE;
            bool Filled_Progression_M3 = kFALSE;
            bool Filled_Progression_N3 = kFALSE;
            bool Filled_Progression_O3 = kFALSE;
            bool Filled_Progression_P3 = kFALSE;
            bool Filled_Progression_Q3 = kFALSE;
            bool Filled_Progression_R3 = kFALSE;
            bool Filled_Progression_S3 = kFALSE;
            bool Filled_Progression_T3 = kFALSE;
            bool Filled_Progression_U3 = kFALSE;
            bool Filled_Progression_V3 = kFALSE;
            bool Filled_Progression_W3 = kFALSE;
            bool Filled_Progression_X3 = kFALSE;
            bool Filled_Progression_Y3 = kFALSE;
            bool Filled_Progression_Z3 = kFALSE;
            bool Filled_Progression_A4 = kFALSE;
            bool Filled_Progression_B4 = kFALSE;
            bool Filled_Progression_C4 = kFALSE;
            bool Filled_Progression_D4 = kFALSE;
            bool Filled_Progression_E4 = kFALSE;
            bool Filled_Progression_F4 = kFALSE;
            bool Filled_Progression_G4 = kFALSE;
            bool Filled_Progression_H4 = kFALSE;
            bool Filled_Progression_I4 = kFALSE;
            bool Filled_Progression_J4 = kFALSE;
            bool Filled_Progression_K4 = kFALSE;
            bool Filled_Progression_L4 = kFALSE;
            bool Filled_Progression_M4 = kFALSE;
            bool Filled_Progression_N4 = kFALSE;
            bool Filled_Progression_O4 = kFALSE;
            bool Filled_Progression_P4 = kFALSE;
            bool Filled_Progression_Q4 = kFALSE;
            bool Filled_Progression_R4 = kFALSE;
            bool Filled_Progression_S4 = kFALSE;
            bool Filled_Progression_T4 = kFALSE;
            bool Filled_Progression_U4 = kFALSE;
            bool Filled_Progression_V4 = kFALSE;
            bool Filled_Progression_W4 = kFALSE;
            bool Filled_Progression_X4 = kFALSE;
            bool Filled_Progression_Y4 = kFALSE;
            bool Filled_Progression_Z4 = kFALSE;
            bool Filled_Progression_A5 = kFALSE;
            bool Filled_Progression_B5 = kFALSE;
            bool Filled_Progression_C5 = kFALSE;
            bool Filled_Progression_D5 = kFALSE;
            bool Filled_Progression_E5 = kFALSE;
            bool Filled_Progression_F5 = kFALSE;
            bool Filled_Progression_G5 = kFALSE;
            bool Filled_Progression_H5 = kFALSE;
            bool Filled_Progression_I5 = kFALSE;
            bool Filled_Progression_J5 = kFALSE;
            bool Filled_Progression_K5 = kFALSE;
            bool Filled_Progression_L5 = kFALSE;
            bool Filled_Progression_M5 = kFALSE;
            bool Filled_Progression_N5 = kFALSE;
            bool Filled_Progression_O5 = kFALSE;
            bool Filled_Progression_P5 = kFALSE;
            bool Filled_Progression_Q5 = kFALSE;
            bool Filled_Progression_R5 = kFALSE;
            bool Filled_Progression_S5 = kFALSE;
            bool Filled_Progression_T5 = kFALSE;
            bool Filled_Progression_U5 = kFALSE;
            bool Filled_Progression_V5 = kFALSE;
            bool Filled_Progression_W5 = kFALSE;
            bool Filled_Progression_X5 = kFALSE;
            bool Filled_Progression_Y5 = kFALSE;
            bool Filled_Progression_Z5 = kFALSE;
            bool Filled_Progression_A6 = kFALSE;
            bool Filled_Progression_B6 = kFALSE;
            bool Filled_Progression_C6 = kFALSE;
            bool Filled_Progression_D6 = kFALSE;
            bool Filled_Progression_E6 = kFALSE;
            bool Filled_Progression_F6 = kFALSE;
            bool Filled_Progression_G6 = kFALSE;
            bool Filled_Progression_H6 = kFALSE;
            bool Filled_Progression_I6 = kFALSE;
            bool Filled_Progression_J6 = kFALSE;
            bool Filled_Progression_K6 = kFALSE;
            bool Filled_Progression_L6 = kFALSE;
            bool Filled_Progression_M6 = kFALSE;
            bool Filled_Progression_N6 = kFALSE;
            bool Filled_Progression_O6 = kFALSE;
            bool Filled_Progression_P6 = kFALSE;
            bool Filled_Progression_Q6 = kFALSE;
            bool Filled_Progression_R6 = kFALSE;
            bool Filled_Progression_S6 = kFALSE;
            bool Filled_Progression_T6 = kFALSE;
            bool Filled_Progression_U6 = kFALSE;
            bool Filled_Progression_V6 = kFALSE;
            bool Filled_Progression_W6 = kFALSE;
            bool Filled_Progression_X6 = kFALSE;
            bool Filled_Progression_Y6 = kFALSE;
            bool Filled_Progression_Z6 = kFALSE;
            bool Filled_Progression_A7 = kFALSE;
            bool Filled_Progression_B7 = kFALSE;
            bool Filled_Progression_C7 = kFALSE;
            bool Filled_Progression_D7 = kFALSE;
            bool Filled_Progression_E7 = kFALSE;
            bool Filled_Progression_F7 = kFALSE;
            bool Filled_Progression_G7 = kFALSE;
            bool Filled_Progression_H7 = kFALSE;
            bool Filled_Progression_I7 = kFALSE;
            bool Filled_Progression_J7 = kFALSE;
            bool Filled_Progression_K7 = kFALSE;
            bool Filled_Progression_L7 = kFALSE;
            bool Filled_Progression_M7 = kFALSE;
            bool Filled_Progression_N7 = kFALSE;
            bool Filled_Progression_O7 = kFALSE;
            bool Filled_Progression_P7 = kFALSE;
            bool Filled_Progression_Q7 = kFALSE;
            bool Filled_Progression_R7 = kFALSE;
            bool Filled_Progression_S7 = kFALSE;
            bool Filled_Progression_T7 = kFALSE;
            bool Filled_Progression_U7 = kFALSE;
            bool Filled_Progression_V7 = kFALSE;
            bool Filled_Progression_W7 = kFALSE;
            bool Filled_Progression_X7 = kFALSE;
            bool Filled_Progression_Y7 = kFALSE;
            bool Filled_Progression_Z7 = kFALSE;
            bool Filled_Progression_A8 = kFALSE;
            bool Filled_Progression_B8 = kFALSE;
            bool Filled_Progression_C8 = kFALSE;
            bool Filled_Progression_D8 = kFALSE;
            bool Filled_Progression_E8 = kFALSE;
            bool Filled_Progression_F8 = kFALSE;
            bool Filled_Progression_G8 = kFALSE;
            bool Filled_Progression_H8 = kFALSE;
            bool Filled_Progression_I8 = kFALSE;
            bool Filled_Progression_J8 = kFALSE;
            bool Filled_Progression_K8 = kFALSE;
            bool Filled_Progression_L8 = kFALSE;
            bool Filled_Progression_M8 = kFALSE;
            bool Filled_Progression_N8 = kFALSE;
            bool Filled_Progression_O8 = kFALSE;
            bool Filled_Progression_P8 = kFALSE;
            bool Filled_Progression_Q8 = kFALSE;
            bool Filled_Progression_R8 = kFALSE;
            bool Filled_Progression_S8 = kFALSE;
            bool Filled_Progression_T8 = kFALSE;
            bool Filled_Progression_U8 = kFALSE;
            bool Filled_Progression_V8 = kFALSE;
            bool Filled_Progression_W8 = kFALSE;
            bool Filled_Progression_X8 = kFALSE;
            bool Filled_Progression_Y8 = kFALSE;
            bool Filled_Progression_Z8 = kFALSE;
            // END OF FULL BLOWN GRID SEARCH

            std::vector<Int_t> Index_Taus_IsoInf;
            Index_Taus_IsoInf.push_back(-1); Index_Taus_IsoInf.push_back(-1);
            std::vector<Float_t> pt_Taus_IsoInf;
            pt_Taus_IsoInf.push_back(-99.); pt_Taus_IsoInf.push_back(-99.);

            std::vector<Int_t> Index_Taus_90;
            Index_Taus_90.push_back(-1); Index_Taus_90.push_back(-1);
            std::vector<Float_t> pt_Taus_90;
            pt_Taus_90.push_back(-99.); pt_Taus_90.push_back(-99.);

            std::vector<Int_t> Index_Taus_80;
            Index_Taus_80.push_back(-1); Index_Taus_80.push_back(-1);
            std::vector<Float_t> pt_Taus_80;
            pt_Taus_80.push_back(-99.); pt_Taus_80.push_back(-99.);

            std::vector<Int_t> Index_Taus_70;
            Index_Taus_70.push_back(-1); Index_Taus_70.push_back(-1);
            std::vector<Float_t> pt_Taus_70;
            pt_Taus_70.push_back(-99.); pt_Taus_70.push_back(-99.);

            std::vector<Int_t> Index_Taus_60;
            Index_Taus_60.push_back(-1); Index_Taus_60.push_back(-1);
            std::vector<Float_t> pt_Taus_60;
            pt_Taus_60.push_back(-99.); pt_Taus_60.push_back(-99.);

            std::vector<Int_t> Index_Taus_50;
            Index_Taus_50.push_back(-1); Index_Taus_50.push_back(-1);
            std::vector<Float_t> pt_Taus_50;
            pt_Taus_50.push_back(-99.); pt_Taus_50.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_1;
            Index_Taus_Progression_1.push_back(-1); Index_Taus_Progression_1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_1;
            pt_Taus_Progression_1.push_back(-99.); pt_Taus_Progression_1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_2;
            Index_Taus_Progression_2.push_back(-1); Index_Taus_Progression_2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_2;
            pt_Taus_Progression_2.push_back(-99.); pt_Taus_Progression_2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_3;
            Index_Taus_Progression_3.push_back(-1); Index_Taus_Progression_3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_3;
            pt_Taus_Progression_3.push_back(-99.); pt_Taus_Progression_3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_4;
            Index_Taus_Progression_4.push_back(-1); Index_Taus_Progression_4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_4;
            pt_Taus_Progression_4.push_back(-99.); pt_Taus_Progression_4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_5;
            Index_Taus_Progression_5.push_back(-1); Index_Taus_Progression_5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_5;
            pt_Taus_Progression_5.push_back(-99.); pt_Taus_Progression_5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_6;
            Index_Taus_Progression_6.push_back(-1); Index_Taus_Progression_6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_6;
            pt_Taus_Progression_6.push_back(-99.); pt_Taus_Progression_6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_7;
            Index_Taus_Progression_7.push_back(-1); Index_Taus_Progression_7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_7;
            pt_Taus_Progression_7.push_back(-99.); pt_Taus_Progression_7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_8;
            Index_Taus_Progression_8.push_back(-1); Index_Taus_Progression_8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_8;
            pt_Taus_Progression_8.push_back(-99.); pt_Taus_Progression_8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_9;
            Index_Taus_Progression_9.push_back(-1); Index_Taus_Progression_9.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_9;
            pt_Taus_Progression_9.push_back(-99.); pt_Taus_Progression_9.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_10;
            Index_Taus_Progression_10.push_back(-1); Index_Taus_Progression_10.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_10;
            pt_Taus_Progression_10.push_back(-99.); pt_Taus_Progression_10.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_11;
            Index_Taus_Progression_11.push_back(-1); Index_Taus_Progression_11.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_11;
            pt_Taus_Progression_11.push_back(-99.); pt_Taus_Progression_11.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_12;
            Index_Taus_Progression_12.push_back(-1); Index_Taus_Progression_12.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_12;
            pt_Taus_Progression_12.push_back(-99.); pt_Taus_Progression_12.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_13;
            Index_Taus_Progression_13.push_back(-1); Index_Taus_Progression_13.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_13;
            pt_Taus_Progression_13.push_back(-99.); pt_Taus_Progression_13.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_14;
            Index_Taus_Progression_14.push_back(-1); Index_Taus_Progression_14.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_14;
            pt_Taus_Progression_14.push_back(-99.); pt_Taus_Progression_14.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_15;
            Index_Taus_Progression_15.push_back(-1); Index_Taus_Progression_15.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_15;
            pt_Taus_Progression_15.push_back(-99.); pt_Taus_Progression_15.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_16;
            Index_Taus_Progression_16.push_back(-1); Index_Taus_Progression_16.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_16;
            pt_Taus_Progression_16.push_back(-99.); pt_Taus_Progression_16.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_17;
            Index_Taus_Progression_17.push_back(-1); Index_Taus_Progression_17.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_17;
            pt_Taus_Progression_17.push_back(-99.); pt_Taus_Progression_17.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_18;
            Index_Taus_Progression_18.push_back(-1); Index_Taus_Progression_18.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_18;
            pt_Taus_Progression_18.push_back(-99.); pt_Taus_Progression_18.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_19;
            Index_Taus_Progression_19.push_back(-1); Index_Taus_Progression_19.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_19;
            pt_Taus_Progression_19.push_back(-99.); pt_Taus_Progression_19.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_20;
            Index_Taus_Progression_20.push_back(-1); Index_Taus_Progression_20.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_20;
            pt_Taus_Progression_20.push_back(-99.); pt_Taus_Progression_20.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_21;
            Index_Taus_Progression_21.push_back(-1); Index_Taus_Progression_21.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_21;
            pt_Taus_Progression_21.push_back(-99.); pt_Taus_Progression_21.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_21;
            eta_Taus_Progression_21.push_back(-99.); eta_Taus_Progression_21.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_22;
            Index_Taus_Progression_22.push_back(-1); Index_Taus_Progression_22.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_22;
            pt_Taus_Progression_22.push_back(-99.); pt_Taus_Progression_22.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_22;
            eta_Taus_Progression_22.push_back(-99.); eta_Taus_Progression_22.push_back(-99.);
            std::vector<Int_t> isMerged_Taus_Progression_22;
            isMerged_Taus_Progression_22.push_back(-99.); isMerged_Taus_Progression_22.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_23;
            Index_Taus_Progression_23.push_back(-1); Index_Taus_Progression_23.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_23;
            pt_Taus_Progression_23.push_back(-99.); pt_Taus_Progression_23.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_23;
            eta_Taus_Progression_23.push_back(-99.); eta_Taus_Progression_23.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_24;
            Index_Taus_Progression_24.push_back(-1); Index_Taus_Progression_24.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_24;
            pt_Taus_Progression_24.push_back(-99.); pt_Taus_Progression_24.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_24;
            eta_Taus_Progression_24.push_back(-99.); eta_Taus_Progression_24.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_25;
            Index_Taus_Progression_25.push_back(-1); Index_Taus_Progression_25.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_25;
            pt_Taus_Progression_25.push_back(-99.); pt_Taus_Progression_25.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_25;
            eta_Taus_Progression_25.push_back(-99.); eta_Taus_Progression_25.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_26;
            Index_Taus_Progression_26.push_back(-1); Index_Taus_Progression_26.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_26;
            pt_Taus_Progression_26.push_back(-99.); pt_Taus_Progression_26.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_26;
            eta_Taus_Progression_26.push_back(-99.); eta_Taus_Progression_26.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_27;
            Index_Taus_Progression_27.push_back(-1); Index_Taus_Progression_27.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_27;
            pt_Taus_Progression_27.push_back(-99.); pt_Taus_Progression_27.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_27;
            eta_Taus_Progression_27.push_back(-99.); eta_Taus_Progression_27.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_28;
            Index_Taus_Progression_28.push_back(-1); Index_Taus_Progression_28.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_28;
            pt_Taus_Progression_28.push_back(-99.); pt_Taus_Progression_28.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_28;
            eta_Taus_Progression_28.push_back(-99.); eta_Taus_Progression_28.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_29;
            Index_Taus_Progression_29.push_back(-1); Index_Taus_Progression_29.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_29;
            pt_Taus_Progression_29.push_back(-99.); pt_Taus_Progression_29.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_29;
            eta_Taus_Progression_29.push_back(-99.); eta_Taus_Progression_29.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_30;
            Index_Taus_Progression_30.push_back(-1); Index_Taus_Progression_30.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_30;
            pt_Taus_Progression_30.push_back(-99.); pt_Taus_Progression_30.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_30;
            eta_Taus_Progression_30.push_back(-99.); eta_Taus_Progression_30.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_31;
            Index_Taus_Progression_31.push_back(-1); Index_Taus_Progression_31.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_31;
            pt_Taus_Progression_31.push_back(-99.); pt_Taus_Progression_31.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_31;
            eta_Taus_Progression_31.push_back(-99.); eta_Taus_Progression_31.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_31_extrap;
            Index_Taus_Progression_31_extrap.push_back(-1); Index_Taus_Progression_31_extrap.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_31_extrap;
            pt_Taus_Progression_31_extrap.push_back(-99.); pt_Taus_Progression_31_extrap.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_31_extrap;
            eta_Taus_Progression_31_extrap.push_back(-99.); eta_Taus_Progression_31_extrap.push_back(-99.);

            // START OF FULL BLOWN GRID SEARCH
            std::vector<Int_t> Index_Taus_Progression_A0;
            Index_Taus_Progression_A0.push_back(-1); Index_Taus_Progression_A0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_A0;
            pt_Taus_Progression_A0.push_back(-99.); pt_Taus_Progression_A0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_A0;
            eta_Taus_Progression_A0.push_back(-99.); eta_Taus_Progression_A0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_B0;
            Index_Taus_Progression_B0.push_back(-1); Index_Taus_Progression_B0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_B0;
            pt_Taus_Progression_B0.push_back(-99.); pt_Taus_Progression_B0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_B0;
            eta_Taus_Progression_B0.push_back(-99.); eta_Taus_Progression_B0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_C0;
            Index_Taus_Progression_C0.push_back(-1); Index_Taus_Progression_C0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_C0;
            pt_Taus_Progression_C0.push_back(-99.); pt_Taus_Progression_C0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_C0;
            eta_Taus_Progression_C0.push_back(-99.); eta_Taus_Progression_C0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_D0;
            Index_Taus_Progression_D0.push_back(-1); Index_Taus_Progression_D0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_D0;
            pt_Taus_Progression_D0.push_back(-99.); pt_Taus_Progression_D0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_D0;
            eta_Taus_Progression_D0.push_back(-99.); eta_Taus_Progression_D0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_E0;
            Index_Taus_Progression_E0.push_back(-1); Index_Taus_Progression_E0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_E0;
            pt_Taus_Progression_E0.push_back(-99.); pt_Taus_Progression_E0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_E0;
            eta_Taus_Progression_E0.push_back(-99.); eta_Taus_Progression_E0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_F0;
            Index_Taus_Progression_F0.push_back(-1); Index_Taus_Progression_F0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_F0;
            pt_Taus_Progression_F0.push_back(-99.); pt_Taus_Progression_F0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_F0;
            eta_Taus_Progression_F0.push_back(-99.); eta_Taus_Progression_F0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_G0;
            Index_Taus_Progression_G0.push_back(-1); Index_Taus_Progression_G0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_G0;
            pt_Taus_Progression_G0.push_back(-99.); pt_Taus_Progression_G0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_G0;
            eta_Taus_Progression_G0.push_back(-99.); eta_Taus_Progression_G0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_H0;
            Index_Taus_Progression_H0.push_back(-1); Index_Taus_Progression_H0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_H0;
            pt_Taus_Progression_H0.push_back(-99.); pt_Taus_Progression_H0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_H0;
            eta_Taus_Progression_H0.push_back(-99.); eta_Taus_Progression_H0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_I0;
            Index_Taus_Progression_I0.push_back(-1); Index_Taus_Progression_I0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_I0;
            pt_Taus_Progression_I0.push_back(-99.); pt_Taus_Progression_I0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_I0;
            eta_Taus_Progression_I0.push_back(-99.); eta_Taus_Progression_I0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_J0;
            Index_Taus_Progression_J0.push_back(-1); Index_Taus_Progression_J0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_J0;
            pt_Taus_Progression_J0.push_back(-99.); pt_Taus_Progression_J0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_J0;
            eta_Taus_Progression_J0.push_back(-99.); eta_Taus_Progression_J0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_K0;
            Index_Taus_Progression_K0.push_back(-1); Index_Taus_Progression_K0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_K0;
            pt_Taus_Progression_K0.push_back(-99.); pt_Taus_Progression_K0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_K0;
            eta_Taus_Progression_K0.push_back(-99.); eta_Taus_Progression_K0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_L0;
            Index_Taus_Progression_L0.push_back(-1); Index_Taus_Progression_L0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_L0;
            pt_Taus_Progression_L0.push_back(-99.); pt_Taus_Progression_L0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_L0;
            eta_Taus_Progression_L0.push_back(-99.); eta_Taus_Progression_L0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_M0;
            Index_Taus_Progression_M0.push_back(-1); Index_Taus_Progression_M0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_M0;
            pt_Taus_Progression_M0.push_back(-99.); pt_Taus_Progression_M0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_M0;
            eta_Taus_Progression_M0.push_back(-99.); eta_Taus_Progression_M0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_N0;
            Index_Taus_Progression_N0.push_back(-1); Index_Taus_Progression_N0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_N0;
            pt_Taus_Progression_N0.push_back(-99.); pt_Taus_Progression_N0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_N0;
            eta_Taus_Progression_N0.push_back(-99.); eta_Taus_Progression_N0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_O0;
            Index_Taus_Progression_O0.push_back(-1); Index_Taus_Progression_O0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_O0;
            pt_Taus_Progression_O0.push_back(-99.); pt_Taus_Progression_O0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_O0;
            eta_Taus_Progression_O0.push_back(-99.); eta_Taus_Progression_O0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_P0;
            Index_Taus_Progression_P0.push_back(-1); Index_Taus_Progression_P0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_P0;
            pt_Taus_Progression_P0.push_back(-99.); pt_Taus_Progression_P0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_P0;
            eta_Taus_Progression_P0.push_back(-99.); eta_Taus_Progression_P0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Q0;
            Index_Taus_Progression_Q0.push_back(-1); Index_Taus_Progression_Q0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Q0;
            pt_Taus_Progression_Q0.push_back(-99.); pt_Taus_Progression_Q0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Q0;
            eta_Taus_Progression_Q0.push_back(-99.); eta_Taus_Progression_Q0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_R0;
            Index_Taus_Progression_R0.push_back(-1); Index_Taus_Progression_R0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_R0;
            pt_Taus_Progression_R0.push_back(-99.); pt_Taus_Progression_R0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_R0;
            eta_Taus_Progression_R0.push_back(-99.); eta_Taus_Progression_R0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_S0;
            Index_Taus_Progression_S0.push_back(-1); Index_Taus_Progression_S0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_S0;
            pt_Taus_Progression_S0.push_back(-99.); pt_Taus_Progression_S0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_S0;
            eta_Taus_Progression_S0.push_back(-99.); eta_Taus_Progression_S0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_T0;
            Index_Taus_Progression_T0.push_back(-1); Index_Taus_Progression_T0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_T0;
            pt_Taus_Progression_T0.push_back(-99.); pt_Taus_Progression_T0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_T0;
            eta_Taus_Progression_T0.push_back(-99.); eta_Taus_Progression_T0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_U0;
            Index_Taus_Progression_U0.push_back(-1); Index_Taus_Progression_U0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_U0;
            pt_Taus_Progression_U0.push_back(-99.); pt_Taus_Progression_U0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_U0;
            eta_Taus_Progression_U0.push_back(-99.); eta_Taus_Progression_U0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_V0;
            Index_Taus_Progression_V0.push_back(-1); Index_Taus_Progression_V0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_V0;
            pt_Taus_Progression_V0.push_back(-99.); pt_Taus_Progression_V0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_V0;
            eta_Taus_Progression_V0.push_back(-99.); eta_Taus_Progression_V0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_W0;
            Index_Taus_Progression_W0.push_back(-1); Index_Taus_Progression_W0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_W0;
            pt_Taus_Progression_W0.push_back(-99.); pt_Taus_Progression_W0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_W0;
            eta_Taus_Progression_W0.push_back(-99.); eta_Taus_Progression_W0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_X0;
            Index_Taus_Progression_X0.push_back(-1); Index_Taus_Progression_X0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_X0;
            pt_Taus_Progression_X0.push_back(-99.); pt_Taus_Progression_X0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_X0;
            eta_Taus_Progression_X0.push_back(-99.); eta_Taus_Progression_X0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Y0;
            Index_Taus_Progression_Y0.push_back(-1); Index_Taus_Progression_Y0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Y0;
            pt_Taus_Progression_Y0.push_back(-99.); pt_Taus_Progression_Y0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Y0;
            eta_Taus_Progression_Y0.push_back(-99.); eta_Taus_Progression_Y0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Z0;
            Index_Taus_Progression_Z0.push_back(-1); Index_Taus_Progression_Z0.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Z0;
            pt_Taus_Progression_Z0.push_back(-99.); pt_Taus_Progression_Z0.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Z0;
            eta_Taus_Progression_Z0.push_back(-99.); eta_Taus_Progression_Z0.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_A1;
            Index_Taus_Progression_A1.push_back(-1); Index_Taus_Progression_A1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_A1;
            pt_Taus_Progression_A1.push_back(-99.); pt_Taus_Progression_A1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_A1;
            eta_Taus_Progression_A1.push_back(-99.); eta_Taus_Progression_A1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_B1;
            Index_Taus_Progression_B1.push_back(-1); Index_Taus_Progression_B1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_B1;
            pt_Taus_Progression_B1.push_back(-99.); pt_Taus_Progression_B1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_B1;
            eta_Taus_Progression_B1.push_back(-99.); eta_Taus_Progression_B1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_C1;
            Index_Taus_Progression_C1.push_back(-1); Index_Taus_Progression_C1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_C1;
            pt_Taus_Progression_C1.push_back(-99.); pt_Taus_Progression_C1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_C1;
            eta_Taus_Progression_C1.push_back(-99.); eta_Taus_Progression_C1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_D1;
            Index_Taus_Progression_D1.push_back(-1); Index_Taus_Progression_D1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_D1;
            pt_Taus_Progression_D1.push_back(-99.); pt_Taus_Progression_D1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_D1;
            eta_Taus_Progression_D1.push_back(-99.); eta_Taus_Progression_D1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_E1;
            Index_Taus_Progression_E1.push_back(-1); Index_Taus_Progression_E1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_E1;
            pt_Taus_Progression_E1.push_back(-99.); pt_Taus_Progression_E1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_E1;
            eta_Taus_Progression_E1.push_back(-99.); eta_Taus_Progression_E1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_F1;
            Index_Taus_Progression_F1.push_back(-1); Index_Taus_Progression_F1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_F1;
            pt_Taus_Progression_F1.push_back(-99.); pt_Taus_Progression_F1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_F1;
            eta_Taus_Progression_F1.push_back(-99.); eta_Taus_Progression_F1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_G1;
            Index_Taus_Progression_G1.push_back(-1); Index_Taus_Progression_G1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_G1;
            pt_Taus_Progression_G1.push_back(-99.); pt_Taus_Progression_G1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_G1;
            eta_Taus_Progression_G1.push_back(-99.); eta_Taus_Progression_G1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_H1;
            Index_Taus_Progression_H1.push_back(-1); Index_Taus_Progression_H1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_H1;
            pt_Taus_Progression_H1.push_back(-99.); pt_Taus_Progression_H1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_H1;
            eta_Taus_Progression_H1.push_back(-99.); eta_Taus_Progression_H1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_I1;
            Index_Taus_Progression_I1.push_back(-1); Index_Taus_Progression_I1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_I1;
            pt_Taus_Progression_I1.push_back(-99.); pt_Taus_Progression_I1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_I1;
            eta_Taus_Progression_I1.push_back(-99.); eta_Taus_Progression_I1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_J1;
            Index_Taus_Progression_J1.push_back(-1); Index_Taus_Progression_J1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_J1;
            pt_Taus_Progression_J1.push_back(-99.); pt_Taus_Progression_J1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_J1;
            eta_Taus_Progression_J1.push_back(-99.); eta_Taus_Progression_J1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_K1;
            Index_Taus_Progression_K1.push_back(-1); Index_Taus_Progression_K1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_K1;
            pt_Taus_Progression_K1.push_back(-99.); pt_Taus_Progression_K1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_K1;
            eta_Taus_Progression_K1.push_back(-99.); eta_Taus_Progression_K1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_L1;
            Index_Taus_Progression_L1.push_back(-1); Index_Taus_Progression_L1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_L1;
            pt_Taus_Progression_L1.push_back(-99.); pt_Taus_Progression_L1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_L1;
            eta_Taus_Progression_L1.push_back(-99.); eta_Taus_Progression_L1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_M1;
            Index_Taus_Progression_M1.push_back(-1); Index_Taus_Progression_M1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_M1;
            pt_Taus_Progression_M1.push_back(-99.); pt_Taus_Progression_M1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_M1;
            eta_Taus_Progression_M1.push_back(-99.); eta_Taus_Progression_M1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_N1;
            Index_Taus_Progression_N1.push_back(-1); Index_Taus_Progression_N1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_N1;
            pt_Taus_Progression_N1.push_back(-99.); pt_Taus_Progression_N1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_N1;
            eta_Taus_Progression_N1.push_back(-99.); eta_Taus_Progression_N1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_O1;
            Index_Taus_Progression_O1.push_back(-1); Index_Taus_Progression_O1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_O1;
            pt_Taus_Progression_O1.push_back(-99.); pt_Taus_Progression_O1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_O1;
            eta_Taus_Progression_O1.push_back(-99.); eta_Taus_Progression_O1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_P1;
            Index_Taus_Progression_P1.push_back(-1); Index_Taus_Progression_P1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_P1;
            pt_Taus_Progression_P1.push_back(-99.); pt_Taus_Progression_P1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_P1;
            eta_Taus_Progression_P1.push_back(-99.); eta_Taus_Progression_P1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Q1;
            Index_Taus_Progression_Q1.push_back(-1); Index_Taus_Progression_Q1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Q1;
            pt_Taus_Progression_Q1.push_back(-99.); pt_Taus_Progression_Q1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Q1;
            eta_Taus_Progression_Q1.push_back(-99.); eta_Taus_Progression_Q1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_R1;
            Index_Taus_Progression_R1.push_back(-1); Index_Taus_Progression_R1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_R1;
            pt_Taus_Progression_R1.push_back(-99.); pt_Taus_Progression_R1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_R1;
            eta_Taus_Progression_R1.push_back(-99.); eta_Taus_Progression_R1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_S1;
            Index_Taus_Progression_S1.push_back(-1); Index_Taus_Progression_S1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_S1;
            pt_Taus_Progression_S1.push_back(-99.); pt_Taus_Progression_S1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_S1;
            eta_Taus_Progression_S1.push_back(-99.); eta_Taus_Progression_S1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_T1;
            Index_Taus_Progression_T1.push_back(-1); Index_Taus_Progression_T1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_T1;
            pt_Taus_Progression_T1.push_back(-99.); pt_Taus_Progression_T1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_T1;
            eta_Taus_Progression_T1.push_back(-99.); eta_Taus_Progression_T1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_U1;
            Index_Taus_Progression_U1.push_back(-1); Index_Taus_Progression_U1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_U1;
            pt_Taus_Progression_U1.push_back(-99.); pt_Taus_Progression_U1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_U1;
            eta_Taus_Progression_U1.push_back(-99.); eta_Taus_Progression_U1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_V1;
            Index_Taus_Progression_V1.push_back(-1); Index_Taus_Progression_V1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_V1;
            pt_Taus_Progression_V1.push_back(-99.); pt_Taus_Progression_V1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_V1;
            eta_Taus_Progression_V1.push_back(-99.); eta_Taus_Progression_V1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_W1;
            Index_Taus_Progression_W1.push_back(-1); Index_Taus_Progression_W1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_W1;
            pt_Taus_Progression_W1.push_back(-99.); pt_Taus_Progression_W1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_W1;
            eta_Taus_Progression_W1.push_back(-99.); eta_Taus_Progression_W1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_X1;
            Index_Taus_Progression_X1.push_back(-1); Index_Taus_Progression_X1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_X1;
            pt_Taus_Progression_X1.push_back(-99.); pt_Taus_Progression_X1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_X1;
            eta_Taus_Progression_X1.push_back(-99.); eta_Taus_Progression_X1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Y1;
            Index_Taus_Progression_Y1.push_back(-1); Index_Taus_Progression_Y1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Y1;
            pt_Taus_Progression_Y1.push_back(-99.); pt_Taus_Progression_Y1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Y1;
            eta_Taus_Progression_Y1.push_back(-99.); eta_Taus_Progression_Y1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Z1;
            Index_Taus_Progression_Z1.push_back(-1); Index_Taus_Progression_Z1.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Z1;
            pt_Taus_Progression_Z1.push_back(-99.); pt_Taus_Progression_Z1.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Z1;
            eta_Taus_Progression_Z1.push_back(-99.); eta_Taus_Progression_Z1.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_A2;
            Index_Taus_Progression_A2.push_back(-1); Index_Taus_Progression_A2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_A2;
            pt_Taus_Progression_A2.push_back(-99.); pt_Taus_Progression_A2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_A2;
            eta_Taus_Progression_A2.push_back(-99.); eta_Taus_Progression_A2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_B2;
            Index_Taus_Progression_B2.push_back(-1); Index_Taus_Progression_B2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_B2;
            pt_Taus_Progression_B2.push_back(-99.); pt_Taus_Progression_B2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_B2;
            eta_Taus_Progression_B2.push_back(-99.); eta_Taus_Progression_B2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_C2;
            Index_Taus_Progression_C2.push_back(-1); Index_Taus_Progression_C2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_C2;
            pt_Taus_Progression_C2.push_back(-99.); pt_Taus_Progression_C2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_C2;
            eta_Taus_Progression_C2.push_back(-99.); eta_Taus_Progression_C2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_D2;
            Index_Taus_Progression_D2.push_back(-1); Index_Taus_Progression_D2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_D2;
            pt_Taus_Progression_D2.push_back(-99.); pt_Taus_Progression_D2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_D2;
            eta_Taus_Progression_D2.push_back(-99.); eta_Taus_Progression_D2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_E2;
            Index_Taus_Progression_E2.push_back(-1); Index_Taus_Progression_E2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_E2;
            pt_Taus_Progression_E2.push_back(-99.); pt_Taus_Progression_E2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_E2;
            eta_Taus_Progression_E2.push_back(-99.); eta_Taus_Progression_E2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_F2;
            Index_Taus_Progression_F2.push_back(-1); Index_Taus_Progression_F2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_F2;
            pt_Taus_Progression_F2.push_back(-99.); pt_Taus_Progression_F2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_F2;
            eta_Taus_Progression_F2.push_back(-99.); eta_Taus_Progression_F2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_G2;
            Index_Taus_Progression_G2.push_back(-1); Index_Taus_Progression_G2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_G2;
            pt_Taus_Progression_G2.push_back(-99.); pt_Taus_Progression_G2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_G2;
            eta_Taus_Progression_G2.push_back(-99.); eta_Taus_Progression_G2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_H2;
            Index_Taus_Progression_H2.push_back(-1); Index_Taus_Progression_H2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_H2;
            pt_Taus_Progression_H2.push_back(-99.); pt_Taus_Progression_H2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_H2;
            eta_Taus_Progression_H2.push_back(-99.); eta_Taus_Progression_H2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_I2;
            Index_Taus_Progression_I2.push_back(-1); Index_Taus_Progression_I2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_I2;
            pt_Taus_Progression_I2.push_back(-99.); pt_Taus_Progression_I2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_I2;
            eta_Taus_Progression_I2.push_back(-99.); eta_Taus_Progression_I2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_J2;
            Index_Taus_Progression_J2.push_back(-1); Index_Taus_Progression_J2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_J2;
            pt_Taus_Progression_J2.push_back(-99.); pt_Taus_Progression_J2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_J2;
            eta_Taus_Progression_J2.push_back(-99.); eta_Taus_Progression_J2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_K2;
            Index_Taus_Progression_K2.push_back(-1); Index_Taus_Progression_K2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_K2;
            pt_Taus_Progression_K2.push_back(-99.); pt_Taus_Progression_K2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_K2;
            eta_Taus_Progression_K2.push_back(-99.); eta_Taus_Progression_K2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_L2;
            Index_Taus_Progression_L2.push_back(-1); Index_Taus_Progression_L2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_L2;
            pt_Taus_Progression_L2.push_back(-99.); pt_Taus_Progression_L2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_L2;
            eta_Taus_Progression_L2.push_back(-99.); eta_Taus_Progression_L2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_M2;
            Index_Taus_Progression_M2.push_back(-1); Index_Taus_Progression_M2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_M2;
            pt_Taus_Progression_M2.push_back(-99.); pt_Taus_Progression_M2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_M2;
            eta_Taus_Progression_M2.push_back(-99.); eta_Taus_Progression_M2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_N2;
            Index_Taus_Progression_N2.push_back(-1); Index_Taus_Progression_N2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_N2;
            pt_Taus_Progression_N2.push_back(-99.); pt_Taus_Progression_N2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_N2;
            eta_Taus_Progression_N2.push_back(-99.); eta_Taus_Progression_N2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_O2;
            Index_Taus_Progression_O2.push_back(-1); Index_Taus_Progression_O2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_O2;
            pt_Taus_Progression_O2.push_back(-99.); pt_Taus_Progression_O2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_O2;
            eta_Taus_Progression_O2.push_back(-99.); eta_Taus_Progression_O2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_P2;
            Index_Taus_Progression_P2.push_back(-1); Index_Taus_Progression_P2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_P2;
            pt_Taus_Progression_P2.push_back(-99.); pt_Taus_Progression_P2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_P2;
            eta_Taus_Progression_P2.push_back(-99.); eta_Taus_Progression_P2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Q2;
            Index_Taus_Progression_Q2.push_back(-1); Index_Taus_Progression_Q2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Q2;
            pt_Taus_Progression_Q2.push_back(-99.); pt_Taus_Progression_Q2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Q2;
            eta_Taus_Progression_Q2.push_back(-99.); eta_Taus_Progression_Q2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_R2;
            Index_Taus_Progression_R2.push_back(-1); Index_Taus_Progression_R2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_R2;
            pt_Taus_Progression_R2.push_back(-99.); pt_Taus_Progression_R2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_R2;
            eta_Taus_Progression_R2.push_back(-99.); eta_Taus_Progression_R2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_S2;
            Index_Taus_Progression_S2.push_back(-1); Index_Taus_Progression_S2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_S2;
            pt_Taus_Progression_S2.push_back(-99.); pt_Taus_Progression_S2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_S2;
            eta_Taus_Progression_S2.push_back(-99.); eta_Taus_Progression_S2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_T2;
            Index_Taus_Progression_T2.push_back(-1); Index_Taus_Progression_T2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_T2;
            pt_Taus_Progression_T2.push_back(-99.); pt_Taus_Progression_T2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_T2;
            eta_Taus_Progression_T2.push_back(-99.); eta_Taus_Progression_T2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_U2;
            Index_Taus_Progression_U2.push_back(-1); Index_Taus_Progression_U2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_U2;
            pt_Taus_Progression_U2.push_back(-99.); pt_Taus_Progression_U2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_U2;
            eta_Taus_Progression_U2.push_back(-99.); eta_Taus_Progression_U2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_V2;
            Index_Taus_Progression_V2.push_back(-1); Index_Taus_Progression_V2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_V2;
            pt_Taus_Progression_V2.push_back(-99.); pt_Taus_Progression_V2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_V2;
            eta_Taus_Progression_V2.push_back(-99.); eta_Taus_Progression_V2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_W2;
            Index_Taus_Progression_W2.push_back(-1); Index_Taus_Progression_W2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_W2;
            pt_Taus_Progression_W2.push_back(-99.); pt_Taus_Progression_W2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_W2;
            eta_Taus_Progression_W2.push_back(-99.); eta_Taus_Progression_W2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_X2;
            Index_Taus_Progression_X2.push_back(-1); Index_Taus_Progression_X2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_X2;
            pt_Taus_Progression_X2.push_back(-99.); pt_Taus_Progression_X2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_X2;
            eta_Taus_Progression_X2.push_back(-99.); eta_Taus_Progression_X2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Y2;
            Index_Taus_Progression_Y2.push_back(-1); Index_Taus_Progression_Y2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Y2;
            pt_Taus_Progression_Y2.push_back(-99.); pt_Taus_Progression_Y2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Y2;
            eta_Taus_Progression_Y2.push_back(-99.); eta_Taus_Progression_Y2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Z2;
            Index_Taus_Progression_Z2.push_back(-1); Index_Taus_Progression_Z2.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Z2;
            pt_Taus_Progression_Z2.push_back(-99.); pt_Taus_Progression_Z2.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Z2;
            eta_Taus_Progression_Z2.push_back(-99.); eta_Taus_Progression_Z2.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_A3;
            Index_Taus_Progression_A3.push_back(-1); Index_Taus_Progression_A3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_A3;
            pt_Taus_Progression_A3.push_back(-99.); pt_Taus_Progression_A3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_A3;
            eta_Taus_Progression_A3.push_back(-99.); eta_Taus_Progression_A3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_B3;
            Index_Taus_Progression_B3.push_back(-1); Index_Taus_Progression_B3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_B3;
            pt_Taus_Progression_B3.push_back(-99.); pt_Taus_Progression_B3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_B3;
            eta_Taus_Progression_B3.push_back(-99.); eta_Taus_Progression_B3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_C3;
            Index_Taus_Progression_C3.push_back(-1); Index_Taus_Progression_C3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_C3;
            pt_Taus_Progression_C3.push_back(-99.); pt_Taus_Progression_C3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_C3;
            eta_Taus_Progression_C3.push_back(-99.); eta_Taus_Progression_C3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_D3;
            Index_Taus_Progression_D3.push_back(-1); Index_Taus_Progression_D3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_D3;
            pt_Taus_Progression_D3.push_back(-99.); pt_Taus_Progression_D3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_D3;
            eta_Taus_Progression_D3.push_back(-99.); eta_Taus_Progression_D3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_E3;
            Index_Taus_Progression_E3.push_back(-1); Index_Taus_Progression_E3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_E3;
            pt_Taus_Progression_E3.push_back(-99.); pt_Taus_Progression_E3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_E3;
            eta_Taus_Progression_E3.push_back(-99.); eta_Taus_Progression_E3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_F3;
            Index_Taus_Progression_F3.push_back(-1); Index_Taus_Progression_F3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_F3;
            pt_Taus_Progression_F3.push_back(-99.); pt_Taus_Progression_F3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_F3;
            eta_Taus_Progression_F3.push_back(-99.); eta_Taus_Progression_F3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_G3;
            Index_Taus_Progression_G3.push_back(-1); Index_Taus_Progression_G3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_G3;
            pt_Taus_Progression_G3.push_back(-99.); pt_Taus_Progression_G3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_G3;
            eta_Taus_Progression_G3.push_back(-99.); eta_Taus_Progression_G3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_H3;
            Index_Taus_Progression_H3.push_back(-1); Index_Taus_Progression_H3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_H3;
            pt_Taus_Progression_H3.push_back(-99.); pt_Taus_Progression_H3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_H3;
            eta_Taus_Progression_H3.push_back(-99.); eta_Taus_Progression_H3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_I3;
            Index_Taus_Progression_I3.push_back(-1); Index_Taus_Progression_I3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_I3;
            pt_Taus_Progression_I3.push_back(-99.); pt_Taus_Progression_I3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_I3;
            eta_Taus_Progression_I3.push_back(-99.); eta_Taus_Progression_I3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_J3;
            Index_Taus_Progression_J3.push_back(-1); Index_Taus_Progression_J3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_J3;
            pt_Taus_Progression_J3.push_back(-99.); pt_Taus_Progression_J3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_J3;
            eta_Taus_Progression_J3.push_back(-99.); eta_Taus_Progression_J3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_K3;
            Index_Taus_Progression_K3.push_back(-1); Index_Taus_Progression_K3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_K3;
            pt_Taus_Progression_K3.push_back(-99.); pt_Taus_Progression_K3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_K3;
            eta_Taus_Progression_K3.push_back(-99.); eta_Taus_Progression_K3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_L3;
            Index_Taus_Progression_L3.push_back(-1); Index_Taus_Progression_L3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_L3;
            pt_Taus_Progression_L3.push_back(-99.); pt_Taus_Progression_L3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_L3;
            eta_Taus_Progression_L3.push_back(-99.); eta_Taus_Progression_L3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_M3;
            Index_Taus_Progression_M3.push_back(-1); Index_Taus_Progression_M3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_M3;
            pt_Taus_Progression_M3.push_back(-99.); pt_Taus_Progression_M3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_M3;
            eta_Taus_Progression_M3.push_back(-99.); eta_Taus_Progression_M3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_N3;
            Index_Taus_Progression_N3.push_back(-1); Index_Taus_Progression_N3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_N3;
            pt_Taus_Progression_N3.push_back(-99.); pt_Taus_Progression_N3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_N3;
            eta_Taus_Progression_N3.push_back(-99.); eta_Taus_Progression_N3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_O3;
            Index_Taus_Progression_O3.push_back(-1); Index_Taus_Progression_O3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_O3;
            pt_Taus_Progression_O3.push_back(-99.); pt_Taus_Progression_O3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_O3;
            eta_Taus_Progression_O3.push_back(-99.); eta_Taus_Progression_O3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_P3;
            Index_Taus_Progression_P3.push_back(-1); Index_Taus_Progression_P3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_P3;
            pt_Taus_Progression_P3.push_back(-99.); pt_Taus_Progression_P3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_P3;
            eta_Taus_Progression_P3.push_back(-99.); eta_Taus_Progression_P3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Q3;
            Index_Taus_Progression_Q3.push_back(-1); Index_Taus_Progression_Q3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Q3;
            pt_Taus_Progression_Q3.push_back(-99.); pt_Taus_Progression_Q3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Q3;
            eta_Taus_Progression_Q3.push_back(-99.); eta_Taus_Progression_Q3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_R3;
            Index_Taus_Progression_R3.push_back(-1); Index_Taus_Progression_R3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_R3;
            pt_Taus_Progression_R3.push_back(-99.); pt_Taus_Progression_R3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_R3;
            eta_Taus_Progression_R3.push_back(-99.); eta_Taus_Progression_R3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_S3;
            Index_Taus_Progression_S3.push_back(-1); Index_Taus_Progression_S3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_S3;
            pt_Taus_Progression_S3.push_back(-99.); pt_Taus_Progression_S3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_S3;
            eta_Taus_Progression_S3.push_back(-99.); eta_Taus_Progression_S3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_T3;
            Index_Taus_Progression_T3.push_back(-1); Index_Taus_Progression_T3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_T3;
            pt_Taus_Progression_T3.push_back(-99.); pt_Taus_Progression_T3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_T3;
            eta_Taus_Progression_T3.push_back(-99.); eta_Taus_Progression_T3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_U3;
            Index_Taus_Progression_U3.push_back(-1); Index_Taus_Progression_U3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_U3;
            pt_Taus_Progression_U3.push_back(-99.); pt_Taus_Progression_U3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_U3;
            eta_Taus_Progression_U3.push_back(-99.); eta_Taus_Progression_U3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_V3;
            Index_Taus_Progression_V3.push_back(-1); Index_Taus_Progression_V3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_V3;
            pt_Taus_Progression_V3.push_back(-99.); pt_Taus_Progression_V3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_V3;
            eta_Taus_Progression_V3.push_back(-99.); eta_Taus_Progression_V3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_W3;
            Index_Taus_Progression_W3.push_back(-1); Index_Taus_Progression_W3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_W3;
            pt_Taus_Progression_W3.push_back(-99.); pt_Taus_Progression_W3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_W3;
            eta_Taus_Progression_W3.push_back(-99.); eta_Taus_Progression_W3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_X3;
            Index_Taus_Progression_X3.push_back(-1); Index_Taus_Progression_X3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_X3;
            pt_Taus_Progression_X3.push_back(-99.); pt_Taus_Progression_X3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_X3;
            eta_Taus_Progression_X3.push_back(-99.); eta_Taus_Progression_X3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Y3;
            Index_Taus_Progression_Y3.push_back(-1); Index_Taus_Progression_Y3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Y3;
            pt_Taus_Progression_Y3.push_back(-99.); pt_Taus_Progression_Y3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Y3;
            eta_Taus_Progression_Y3.push_back(-99.); eta_Taus_Progression_Y3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Z3;
            Index_Taus_Progression_Z3.push_back(-1); Index_Taus_Progression_Z3.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Z3;
            pt_Taus_Progression_Z3.push_back(-99.); pt_Taus_Progression_Z3.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Z3;
            eta_Taus_Progression_Z3.push_back(-99.); eta_Taus_Progression_Z3.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_A4;
            Index_Taus_Progression_A4.push_back(-1); Index_Taus_Progression_A4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_A4;
            pt_Taus_Progression_A4.push_back(-99.); pt_Taus_Progression_A4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_A4;
            eta_Taus_Progression_A4.push_back(-99.); eta_Taus_Progression_A4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_B4;
            Index_Taus_Progression_B4.push_back(-1); Index_Taus_Progression_B4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_B4;
            pt_Taus_Progression_B4.push_back(-99.); pt_Taus_Progression_B4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_B4;
            eta_Taus_Progression_B4.push_back(-99.); eta_Taus_Progression_B4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_C4;
            Index_Taus_Progression_C4.push_back(-1); Index_Taus_Progression_C4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_C4;
            pt_Taus_Progression_C4.push_back(-99.); pt_Taus_Progression_C4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_C4;
            eta_Taus_Progression_C4.push_back(-99.); eta_Taus_Progression_C4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_D4;
            Index_Taus_Progression_D4.push_back(-1); Index_Taus_Progression_D4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_D4;
            pt_Taus_Progression_D4.push_back(-99.); pt_Taus_Progression_D4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_D4;
            eta_Taus_Progression_D4.push_back(-99.); eta_Taus_Progression_D4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_E4;
            Index_Taus_Progression_E4.push_back(-1); Index_Taus_Progression_E4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_E4;
            pt_Taus_Progression_E4.push_back(-99.); pt_Taus_Progression_E4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_E4;
            eta_Taus_Progression_E4.push_back(-99.); eta_Taus_Progression_E4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_F4;
            Index_Taus_Progression_F4.push_back(-1); Index_Taus_Progression_F4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_F4;
            pt_Taus_Progression_F4.push_back(-99.); pt_Taus_Progression_F4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_F4;
            eta_Taus_Progression_F4.push_back(-99.); eta_Taus_Progression_F4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_G4;
            Index_Taus_Progression_G4.push_back(-1); Index_Taus_Progression_G4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_G4;
            pt_Taus_Progression_G4.push_back(-99.); pt_Taus_Progression_G4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_G4;
            eta_Taus_Progression_G4.push_back(-99.); eta_Taus_Progression_G4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_H4;
            Index_Taus_Progression_H4.push_back(-1); Index_Taus_Progression_H4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_H4;
            pt_Taus_Progression_H4.push_back(-99.); pt_Taus_Progression_H4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_H4;
            eta_Taus_Progression_H4.push_back(-99.); eta_Taus_Progression_H4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_I4;
            Index_Taus_Progression_I4.push_back(-1); Index_Taus_Progression_I4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_I4;
            pt_Taus_Progression_I4.push_back(-99.); pt_Taus_Progression_I4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_I4;
            eta_Taus_Progression_I4.push_back(-99.); eta_Taus_Progression_I4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_J4;
            Index_Taus_Progression_J4.push_back(-1); Index_Taus_Progression_J4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_J4;
            pt_Taus_Progression_J4.push_back(-99.); pt_Taus_Progression_J4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_J4;
            eta_Taus_Progression_J4.push_back(-99.); eta_Taus_Progression_J4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_K4;
            Index_Taus_Progression_K4.push_back(-1); Index_Taus_Progression_K4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_K4;
            pt_Taus_Progression_K4.push_back(-99.); pt_Taus_Progression_K4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_K4;
            eta_Taus_Progression_K4.push_back(-99.); eta_Taus_Progression_K4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_L4;
            Index_Taus_Progression_L4.push_back(-1); Index_Taus_Progression_L4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_L4;
            pt_Taus_Progression_L4.push_back(-99.); pt_Taus_Progression_L4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_L4;
            eta_Taus_Progression_L4.push_back(-99.); eta_Taus_Progression_L4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_M4;
            Index_Taus_Progression_M4.push_back(-1); Index_Taus_Progression_M4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_M4;
            pt_Taus_Progression_M4.push_back(-99.); pt_Taus_Progression_M4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_M4;
            eta_Taus_Progression_M4.push_back(-99.); eta_Taus_Progression_M4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_N4;
            Index_Taus_Progression_N4.push_back(-1); Index_Taus_Progression_N4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_N4;
            pt_Taus_Progression_N4.push_back(-99.); pt_Taus_Progression_N4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_N4;
            eta_Taus_Progression_N4.push_back(-99.); eta_Taus_Progression_N4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_O4;
            Index_Taus_Progression_O4.push_back(-1); Index_Taus_Progression_O4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_O4;
            pt_Taus_Progression_O4.push_back(-99.); pt_Taus_Progression_O4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_O4;
            eta_Taus_Progression_O4.push_back(-99.); eta_Taus_Progression_O4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_P4;
            Index_Taus_Progression_P4.push_back(-1); Index_Taus_Progression_P4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_P4;
            pt_Taus_Progression_P4.push_back(-99.); pt_Taus_Progression_P4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_P4;
            eta_Taus_Progression_P4.push_back(-99.); eta_Taus_Progression_P4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Q4;
            Index_Taus_Progression_Q4.push_back(-1); Index_Taus_Progression_Q4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Q4;
            pt_Taus_Progression_Q4.push_back(-99.); pt_Taus_Progression_Q4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Q4;
            eta_Taus_Progression_Q4.push_back(-99.); eta_Taus_Progression_Q4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_R4;
            Index_Taus_Progression_R4.push_back(-1); Index_Taus_Progression_R4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_R4;
            pt_Taus_Progression_R4.push_back(-99.); pt_Taus_Progression_R4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_R4;
            eta_Taus_Progression_R4.push_back(-99.); eta_Taus_Progression_R4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_S4;
            Index_Taus_Progression_S4.push_back(-1); Index_Taus_Progression_S4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_S4;
            pt_Taus_Progression_S4.push_back(-99.); pt_Taus_Progression_S4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_S4;
            eta_Taus_Progression_S4.push_back(-99.); eta_Taus_Progression_S4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_T4;
            Index_Taus_Progression_T4.push_back(-1); Index_Taus_Progression_T4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_T4;
            pt_Taus_Progression_T4.push_back(-99.); pt_Taus_Progression_T4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_T4;
            eta_Taus_Progression_T4.push_back(-99.); eta_Taus_Progression_T4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_U4;
            Index_Taus_Progression_U4.push_back(-1); Index_Taus_Progression_U4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_U4;
            pt_Taus_Progression_U4.push_back(-99.); pt_Taus_Progression_U4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_U4;
            eta_Taus_Progression_U4.push_back(-99.); eta_Taus_Progression_U4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_V4;
            Index_Taus_Progression_V4.push_back(-1); Index_Taus_Progression_V4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_V4;
            pt_Taus_Progression_V4.push_back(-99.); pt_Taus_Progression_V4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_V4;
            eta_Taus_Progression_V4.push_back(-99.); eta_Taus_Progression_V4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_W4;
            Index_Taus_Progression_W4.push_back(-1); Index_Taus_Progression_W4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_W4;
            pt_Taus_Progression_W4.push_back(-99.); pt_Taus_Progression_W4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_W4;
            eta_Taus_Progression_W4.push_back(-99.); eta_Taus_Progression_W4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_X4;
            Index_Taus_Progression_X4.push_back(-1); Index_Taus_Progression_X4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_X4;
            pt_Taus_Progression_X4.push_back(-99.); pt_Taus_Progression_X4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_X4;
            eta_Taus_Progression_X4.push_back(-99.); eta_Taus_Progression_X4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Y4;
            Index_Taus_Progression_Y4.push_back(-1); Index_Taus_Progression_Y4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Y4;
            pt_Taus_Progression_Y4.push_back(-99.); pt_Taus_Progression_Y4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Y4;
            eta_Taus_Progression_Y4.push_back(-99.); eta_Taus_Progression_Y4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Z4;
            Index_Taus_Progression_Z4.push_back(-1); Index_Taus_Progression_Z4.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Z4;
            pt_Taus_Progression_Z4.push_back(-99.); pt_Taus_Progression_Z4.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Z4;
            eta_Taus_Progression_Z4.push_back(-99.); eta_Taus_Progression_Z4.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_A5;
            Index_Taus_Progression_A5.push_back(-1); Index_Taus_Progression_A5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_A5;
            pt_Taus_Progression_A5.push_back(-99.); pt_Taus_Progression_A5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_A5;
            eta_Taus_Progression_A5.push_back(-99.); eta_Taus_Progression_A5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_B5;
            Index_Taus_Progression_B5.push_back(-1); Index_Taus_Progression_B5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_B5;
            pt_Taus_Progression_B5.push_back(-99.); pt_Taus_Progression_B5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_B5;
            eta_Taus_Progression_B5.push_back(-99.); eta_Taus_Progression_B5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_C5;
            Index_Taus_Progression_C5.push_back(-1); Index_Taus_Progression_C5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_C5;
            pt_Taus_Progression_C5.push_back(-99.); pt_Taus_Progression_C5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_C5;
            eta_Taus_Progression_C5.push_back(-99.); eta_Taus_Progression_C5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_D5;
            Index_Taus_Progression_D5.push_back(-1); Index_Taus_Progression_D5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_D5;
            pt_Taus_Progression_D5.push_back(-99.); pt_Taus_Progression_D5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_D5;
            eta_Taus_Progression_D5.push_back(-99.); eta_Taus_Progression_D5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_E5;
            Index_Taus_Progression_E5.push_back(-1); Index_Taus_Progression_E5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_E5;
            pt_Taus_Progression_E5.push_back(-99.); pt_Taus_Progression_E5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_E5;
            eta_Taus_Progression_E5.push_back(-99.); eta_Taus_Progression_E5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_F5;
            Index_Taus_Progression_F5.push_back(-1); Index_Taus_Progression_F5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_F5;
            pt_Taus_Progression_F5.push_back(-99.); pt_Taus_Progression_F5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_F5;
            eta_Taus_Progression_F5.push_back(-99.); eta_Taus_Progression_F5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_G5;
            Index_Taus_Progression_G5.push_back(-1); Index_Taus_Progression_G5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_G5;
            pt_Taus_Progression_G5.push_back(-99.); pt_Taus_Progression_G5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_G5;
            eta_Taus_Progression_G5.push_back(-99.); eta_Taus_Progression_G5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_H5;
            Index_Taus_Progression_H5.push_back(-1); Index_Taus_Progression_H5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_H5;
            pt_Taus_Progression_H5.push_back(-99.); pt_Taus_Progression_H5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_H5;
            eta_Taus_Progression_H5.push_back(-99.); eta_Taus_Progression_H5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_I5;
            Index_Taus_Progression_I5.push_back(-1); Index_Taus_Progression_I5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_I5;
            pt_Taus_Progression_I5.push_back(-99.); pt_Taus_Progression_I5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_I5;
            eta_Taus_Progression_I5.push_back(-99.); eta_Taus_Progression_I5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_J5;
            Index_Taus_Progression_J5.push_back(-1); Index_Taus_Progression_J5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_J5;
            pt_Taus_Progression_J5.push_back(-99.); pt_Taus_Progression_J5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_J5;
            eta_Taus_Progression_J5.push_back(-99.); eta_Taus_Progression_J5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_K5;
            Index_Taus_Progression_K5.push_back(-1); Index_Taus_Progression_K5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_K5;
            pt_Taus_Progression_K5.push_back(-99.); pt_Taus_Progression_K5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_K5;
            eta_Taus_Progression_K5.push_back(-99.); eta_Taus_Progression_K5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_L5;
            Index_Taus_Progression_L5.push_back(-1); Index_Taus_Progression_L5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_L5;
            pt_Taus_Progression_L5.push_back(-99.); pt_Taus_Progression_L5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_L5;
            eta_Taus_Progression_L5.push_back(-99.); eta_Taus_Progression_L5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_M5;
            Index_Taus_Progression_M5.push_back(-1); Index_Taus_Progression_M5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_M5;
            pt_Taus_Progression_M5.push_back(-99.); pt_Taus_Progression_M5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_M5;
            eta_Taus_Progression_M5.push_back(-99.); eta_Taus_Progression_M5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_N5;
            Index_Taus_Progression_N5.push_back(-1); Index_Taus_Progression_N5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_N5;
            pt_Taus_Progression_N5.push_back(-99.); pt_Taus_Progression_N5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_N5;
            eta_Taus_Progression_N5.push_back(-99.); eta_Taus_Progression_N5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_O5;
            Index_Taus_Progression_O5.push_back(-1); Index_Taus_Progression_O5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_O5;
            pt_Taus_Progression_O5.push_back(-99.); pt_Taus_Progression_O5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_O5;
            eta_Taus_Progression_O5.push_back(-99.); eta_Taus_Progression_O5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_P5;
            Index_Taus_Progression_P5.push_back(-1); Index_Taus_Progression_P5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_P5;
            pt_Taus_Progression_P5.push_back(-99.); pt_Taus_Progression_P5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_P5;
            eta_Taus_Progression_P5.push_back(-99.); eta_Taus_Progression_P5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Q5;
            Index_Taus_Progression_Q5.push_back(-1); Index_Taus_Progression_Q5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Q5;
            pt_Taus_Progression_Q5.push_back(-99.); pt_Taus_Progression_Q5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Q5;
            eta_Taus_Progression_Q5.push_back(-99.); eta_Taus_Progression_Q5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_R5;
            Index_Taus_Progression_R5.push_back(-1); Index_Taus_Progression_R5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_R5;
            pt_Taus_Progression_R5.push_back(-99.); pt_Taus_Progression_R5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_R5;
            eta_Taus_Progression_R5.push_back(-99.); eta_Taus_Progression_R5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_S5;
            Index_Taus_Progression_S5.push_back(-1); Index_Taus_Progression_S5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_S5;
            pt_Taus_Progression_S5.push_back(-99.); pt_Taus_Progression_S5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_S5;
            eta_Taus_Progression_S5.push_back(-99.); eta_Taus_Progression_S5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_T5;
            Index_Taus_Progression_T5.push_back(-1); Index_Taus_Progression_T5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_T5;
            pt_Taus_Progression_T5.push_back(-99.); pt_Taus_Progression_T5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_T5;
            eta_Taus_Progression_T5.push_back(-99.); eta_Taus_Progression_T5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_U5;
            Index_Taus_Progression_U5.push_back(-1); Index_Taus_Progression_U5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_U5;
            pt_Taus_Progression_U5.push_back(-99.); pt_Taus_Progression_U5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_U5;
            eta_Taus_Progression_U5.push_back(-99.); eta_Taus_Progression_U5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_V5;
            Index_Taus_Progression_V5.push_back(-1); Index_Taus_Progression_V5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_V5;
            pt_Taus_Progression_V5.push_back(-99.); pt_Taus_Progression_V5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_V5;
            eta_Taus_Progression_V5.push_back(-99.); eta_Taus_Progression_V5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_W5;
            Index_Taus_Progression_W5.push_back(-1); Index_Taus_Progression_W5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_W5;
            pt_Taus_Progression_W5.push_back(-99.); pt_Taus_Progression_W5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_W5;
            eta_Taus_Progression_W5.push_back(-99.); eta_Taus_Progression_W5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_X5;
            Index_Taus_Progression_X5.push_back(-1); Index_Taus_Progression_X5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_X5;
            pt_Taus_Progression_X5.push_back(-99.); pt_Taus_Progression_X5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_X5;
            eta_Taus_Progression_X5.push_back(-99.); eta_Taus_Progression_X5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Y5;
            Index_Taus_Progression_Y5.push_back(-1); Index_Taus_Progression_Y5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Y5;
            pt_Taus_Progression_Y5.push_back(-99.); pt_Taus_Progression_Y5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Y5;
            eta_Taus_Progression_Y5.push_back(-99.); eta_Taus_Progression_Y5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Z5;
            Index_Taus_Progression_Z5.push_back(-1); Index_Taus_Progression_Z5.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Z5;
            pt_Taus_Progression_Z5.push_back(-99.); pt_Taus_Progression_Z5.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Z5;
            eta_Taus_Progression_Z5.push_back(-99.); eta_Taus_Progression_Z5.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_A6;
            Index_Taus_Progression_A6.push_back(-1); Index_Taus_Progression_A6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_A6;
            pt_Taus_Progression_A6.push_back(-99.); pt_Taus_Progression_A6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_A6;
            eta_Taus_Progression_A6.push_back(-99.); eta_Taus_Progression_A6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_B6;
            Index_Taus_Progression_B6.push_back(-1); Index_Taus_Progression_B6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_B6;
            pt_Taus_Progression_B6.push_back(-99.); pt_Taus_Progression_B6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_B6;
            eta_Taus_Progression_B6.push_back(-99.); eta_Taus_Progression_B6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_C6;
            Index_Taus_Progression_C6.push_back(-1); Index_Taus_Progression_C6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_C6;
            pt_Taus_Progression_C6.push_back(-99.); pt_Taus_Progression_C6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_C6;
            eta_Taus_Progression_C6.push_back(-99.); eta_Taus_Progression_C6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_D6;
            Index_Taus_Progression_D6.push_back(-1); Index_Taus_Progression_D6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_D6;
            pt_Taus_Progression_D6.push_back(-99.); pt_Taus_Progression_D6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_D6;
            eta_Taus_Progression_D6.push_back(-99.); eta_Taus_Progression_D6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_E6;
            Index_Taus_Progression_E6.push_back(-1); Index_Taus_Progression_E6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_E6;
            pt_Taus_Progression_E6.push_back(-99.); pt_Taus_Progression_E6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_E6;
            eta_Taus_Progression_E6.push_back(-99.); eta_Taus_Progression_E6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_F6;
            Index_Taus_Progression_F6.push_back(-1); Index_Taus_Progression_F6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_F6;
            pt_Taus_Progression_F6.push_back(-99.); pt_Taus_Progression_F6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_F6;
            eta_Taus_Progression_F6.push_back(-99.); eta_Taus_Progression_F6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_G6;
            Index_Taus_Progression_G6.push_back(-1); Index_Taus_Progression_G6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_G6;
            pt_Taus_Progression_G6.push_back(-99.); pt_Taus_Progression_G6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_G6;
            eta_Taus_Progression_G6.push_back(-99.); eta_Taus_Progression_G6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_H6;
            Index_Taus_Progression_H6.push_back(-1); Index_Taus_Progression_H6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_H6;
            pt_Taus_Progression_H6.push_back(-99.); pt_Taus_Progression_H6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_H6;
            eta_Taus_Progression_H6.push_back(-99.); eta_Taus_Progression_H6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_I6;
            Index_Taus_Progression_I6.push_back(-1); Index_Taus_Progression_I6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_I6;
            pt_Taus_Progression_I6.push_back(-99.); pt_Taus_Progression_I6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_I6;
            eta_Taus_Progression_I6.push_back(-99.); eta_Taus_Progression_I6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_J6;
            Index_Taus_Progression_J6.push_back(-1); Index_Taus_Progression_J6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_J6;
            pt_Taus_Progression_J6.push_back(-99.); pt_Taus_Progression_J6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_J6;
            eta_Taus_Progression_J6.push_back(-99.); eta_Taus_Progression_J6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_K6;
            Index_Taus_Progression_K6.push_back(-1); Index_Taus_Progression_K6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_K6;
            pt_Taus_Progression_K6.push_back(-99.); pt_Taus_Progression_K6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_K6;
            eta_Taus_Progression_K6.push_back(-99.); eta_Taus_Progression_K6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_L6;
            Index_Taus_Progression_L6.push_back(-1); Index_Taus_Progression_L6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_L6;
            pt_Taus_Progression_L6.push_back(-99.); pt_Taus_Progression_L6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_L6;
            eta_Taus_Progression_L6.push_back(-99.); eta_Taus_Progression_L6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_M6;
            Index_Taus_Progression_M6.push_back(-1); Index_Taus_Progression_M6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_M6;
            pt_Taus_Progression_M6.push_back(-99.); pt_Taus_Progression_M6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_M6;
            eta_Taus_Progression_M6.push_back(-99.); eta_Taus_Progression_M6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_N6;
            Index_Taus_Progression_N6.push_back(-1); Index_Taus_Progression_N6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_N6;
            pt_Taus_Progression_N6.push_back(-99.); pt_Taus_Progression_N6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_N6;
            eta_Taus_Progression_N6.push_back(-99.); eta_Taus_Progression_N6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_O6;
            Index_Taus_Progression_O6.push_back(-1); Index_Taus_Progression_O6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_O6;
            pt_Taus_Progression_O6.push_back(-99.); pt_Taus_Progression_O6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_O6;
            eta_Taus_Progression_O6.push_back(-99.); eta_Taus_Progression_O6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_P6;
            Index_Taus_Progression_P6.push_back(-1); Index_Taus_Progression_P6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_P6;
            pt_Taus_Progression_P6.push_back(-99.); pt_Taus_Progression_P6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_P6;
            eta_Taus_Progression_P6.push_back(-99.); eta_Taus_Progression_P6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Q6;
            Index_Taus_Progression_Q6.push_back(-1); Index_Taus_Progression_Q6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Q6;
            pt_Taus_Progression_Q6.push_back(-99.); pt_Taus_Progression_Q6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Q6;
            eta_Taus_Progression_Q6.push_back(-99.); eta_Taus_Progression_Q6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_R6;
            Index_Taus_Progression_R6.push_back(-1); Index_Taus_Progression_R6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_R6;
            pt_Taus_Progression_R6.push_back(-99.); pt_Taus_Progression_R6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_R6;
            eta_Taus_Progression_R6.push_back(-99.); eta_Taus_Progression_R6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_S6;
            Index_Taus_Progression_S6.push_back(-1); Index_Taus_Progression_S6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_S6;
            pt_Taus_Progression_S6.push_back(-99.); pt_Taus_Progression_S6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_S6;
            eta_Taus_Progression_S6.push_back(-99.); eta_Taus_Progression_S6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_T6;
            Index_Taus_Progression_T6.push_back(-1); Index_Taus_Progression_T6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_T6;
            pt_Taus_Progression_T6.push_back(-99.); pt_Taus_Progression_T6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_T6;
            eta_Taus_Progression_T6.push_back(-99.); eta_Taus_Progression_T6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_U6;
            Index_Taus_Progression_U6.push_back(-1); Index_Taus_Progression_U6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_U6;
            pt_Taus_Progression_U6.push_back(-99.); pt_Taus_Progression_U6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_U6;
            eta_Taus_Progression_U6.push_back(-99.); eta_Taus_Progression_U6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_V6;
            Index_Taus_Progression_V6.push_back(-1); Index_Taus_Progression_V6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_V6;
            pt_Taus_Progression_V6.push_back(-99.); pt_Taus_Progression_V6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_V6;
            eta_Taus_Progression_V6.push_back(-99.); eta_Taus_Progression_V6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_W6;
            Index_Taus_Progression_W6.push_back(-1); Index_Taus_Progression_W6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_W6;
            pt_Taus_Progression_W6.push_back(-99.); pt_Taus_Progression_W6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_W6;
            eta_Taus_Progression_W6.push_back(-99.); eta_Taus_Progression_W6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_X6;
            Index_Taus_Progression_X6.push_back(-1); Index_Taus_Progression_X6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_X6;
            pt_Taus_Progression_X6.push_back(-99.); pt_Taus_Progression_X6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_X6;
            eta_Taus_Progression_X6.push_back(-99.); eta_Taus_Progression_X6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Y6;
            Index_Taus_Progression_Y6.push_back(-1); Index_Taus_Progression_Y6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Y6;
            pt_Taus_Progression_Y6.push_back(-99.); pt_Taus_Progression_Y6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Y6;
            eta_Taus_Progression_Y6.push_back(-99.); eta_Taus_Progression_Y6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Z6;
            Index_Taus_Progression_Z6.push_back(-1); Index_Taus_Progression_Z6.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Z6;
            pt_Taus_Progression_Z6.push_back(-99.); pt_Taus_Progression_Z6.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Z6;
            eta_Taus_Progression_Z6.push_back(-99.); eta_Taus_Progression_Z6.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_A7;
            Index_Taus_Progression_A7.push_back(-1); Index_Taus_Progression_A7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_A7;
            pt_Taus_Progression_A7.push_back(-99.); pt_Taus_Progression_A7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_A7;
            eta_Taus_Progression_A7.push_back(-99.); eta_Taus_Progression_A7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_B7;
            Index_Taus_Progression_B7.push_back(-1); Index_Taus_Progression_B7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_B7;
            pt_Taus_Progression_B7.push_back(-99.); pt_Taus_Progression_B7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_B7;
            eta_Taus_Progression_B7.push_back(-99.); eta_Taus_Progression_B7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_C7;
            Index_Taus_Progression_C7.push_back(-1); Index_Taus_Progression_C7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_C7;
            pt_Taus_Progression_C7.push_back(-99.); pt_Taus_Progression_C7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_C7;
            eta_Taus_Progression_C7.push_back(-99.); eta_Taus_Progression_C7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_D7;
            Index_Taus_Progression_D7.push_back(-1); Index_Taus_Progression_D7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_D7;
            pt_Taus_Progression_D7.push_back(-99.); pt_Taus_Progression_D7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_D7;
            eta_Taus_Progression_D7.push_back(-99.); eta_Taus_Progression_D7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_E7;
            Index_Taus_Progression_E7.push_back(-1); Index_Taus_Progression_E7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_E7;
            pt_Taus_Progression_E7.push_back(-99.); pt_Taus_Progression_E7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_E7;
            eta_Taus_Progression_E7.push_back(-99.); eta_Taus_Progression_E7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_F7;
            Index_Taus_Progression_F7.push_back(-1); Index_Taus_Progression_F7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_F7;
            pt_Taus_Progression_F7.push_back(-99.); pt_Taus_Progression_F7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_F7;
            eta_Taus_Progression_F7.push_back(-99.); eta_Taus_Progression_F7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_G7;
            Index_Taus_Progression_G7.push_back(-1); Index_Taus_Progression_G7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_G7;
            pt_Taus_Progression_G7.push_back(-99.); pt_Taus_Progression_G7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_G7;
            eta_Taus_Progression_G7.push_back(-99.); eta_Taus_Progression_G7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_H7;
            Index_Taus_Progression_H7.push_back(-1); Index_Taus_Progression_H7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_H7;
            pt_Taus_Progression_H7.push_back(-99.); pt_Taus_Progression_H7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_H7;
            eta_Taus_Progression_H7.push_back(-99.); eta_Taus_Progression_H7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_I7;
            Index_Taus_Progression_I7.push_back(-1); Index_Taus_Progression_I7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_I7;
            pt_Taus_Progression_I7.push_back(-99.); pt_Taus_Progression_I7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_I7;
            eta_Taus_Progression_I7.push_back(-99.); eta_Taus_Progression_I7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_J7;
            Index_Taus_Progression_J7.push_back(-1); Index_Taus_Progression_J7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_J7;
            pt_Taus_Progression_J7.push_back(-99.); pt_Taus_Progression_J7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_J7;
            eta_Taus_Progression_J7.push_back(-99.); eta_Taus_Progression_J7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_K7;
            Index_Taus_Progression_K7.push_back(-1); Index_Taus_Progression_K7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_K7;
            pt_Taus_Progression_K7.push_back(-99.); pt_Taus_Progression_K7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_K7;
            eta_Taus_Progression_K7.push_back(-99.); eta_Taus_Progression_K7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_L7;
            Index_Taus_Progression_L7.push_back(-1); Index_Taus_Progression_L7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_L7;
            pt_Taus_Progression_L7.push_back(-99.); pt_Taus_Progression_L7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_L7;
            eta_Taus_Progression_L7.push_back(-99.); eta_Taus_Progression_L7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_M7;
            Index_Taus_Progression_M7.push_back(-1); Index_Taus_Progression_M7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_M7;
            pt_Taus_Progression_M7.push_back(-99.); pt_Taus_Progression_M7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_M7;
            eta_Taus_Progression_M7.push_back(-99.); eta_Taus_Progression_M7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_N7;
            Index_Taus_Progression_N7.push_back(-1); Index_Taus_Progression_N7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_N7;
            pt_Taus_Progression_N7.push_back(-99.); pt_Taus_Progression_N7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_N7;
            eta_Taus_Progression_N7.push_back(-99.); eta_Taus_Progression_N7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_O7;
            Index_Taus_Progression_O7.push_back(-1); Index_Taus_Progression_O7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_O7;
            pt_Taus_Progression_O7.push_back(-99.); pt_Taus_Progression_O7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_O7;
            eta_Taus_Progression_O7.push_back(-99.); eta_Taus_Progression_O7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_P7;
            Index_Taus_Progression_P7.push_back(-1); Index_Taus_Progression_P7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_P7;
            pt_Taus_Progression_P7.push_back(-99.); pt_Taus_Progression_P7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_P7;
            eta_Taus_Progression_P7.push_back(-99.); eta_Taus_Progression_P7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Q7;
            Index_Taus_Progression_Q7.push_back(-1); Index_Taus_Progression_Q7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Q7;
            pt_Taus_Progression_Q7.push_back(-99.); pt_Taus_Progression_Q7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Q7;
            eta_Taus_Progression_Q7.push_back(-99.); eta_Taus_Progression_Q7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_R7;
            Index_Taus_Progression_R7.push_back(-1); Index_Taus_Progression_R7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_R7;
            pt_Taus_Progression_R7.push_back(-99.); pt_Taus_Progression_R7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_R7;
            eta_Taus_Progression_R7.push_back(-99.); eta_Taus_Progression_R7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_S7;
            Index_Taus_Progression_S7.push_back(-1); Index_Taus_Progression_S7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_S7;
            pt_Taus_Progression_S7.push_back(-99.); pt_Taus_Progression_S7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_S7;
            eta_Taus_Progression_S7.push_back(-99.); eta_Taus_Progression_S7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_T7;
            Index_Taus_Progression_T7.push_back(-1); Index_Taus_Progression_T7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_T7;
            pt_Taus_Progression_T7.push_back(-99.); pt_Taus_Progression_T7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_T7;
            eta_Taus_Progression_T7.push_back(-99.); eta_Taus_Progression_T7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_U7;
            Index_Taus_Progression_U7.push_back(-1); Index_Taus_Progression_U7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_U7;
            pt_Taus_Progression_U7.push_back(-99.); pt_Taus_Progression_U7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_U7;
            eta_Taus_Progression_U7.push_back(-99.); eta_Taus_Progression_U7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_V7;
            Index_Taus_Progression_V7.push_back(-1); Index_Taus_Progression_V7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_V7;
            pt_Taus_Progression_V7.push_back(-99.); pt_Taus_Progression_V7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_V7;
            eta_Taus_Progression_V7.push_back(-99.); eta_Taus_Progression_V7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_W7;
            Index_Taus_Progression_W7.push_back(-1); Index_Taus_Progression_W7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_W7;
            pt_Taus_Progression_W7.push_back(-99.); pt_Taus_Progression_W7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_W7;
            eta_Taus_Progression_W7.push_back(-99.); eta_Taus_Progression_W7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_X7;
            Index_Taus_Progression_X7.push_back(-1); Index_Taus_Progression_X7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_X7;
            pt_Taus_Progression_X7.push_back(-99.); pt_Taus_Progression_X7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_X7;
            eta_Taus_Progression_X7.push_back(-99.); eta_Taus_Progression_X7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Y7;
            Index_Taus_Progression_Y7.push_back(-1); Index_Taus_Progression_Y7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Y7;
            pt_Taus_Progression_Y7.push_back(-99.); pt_Taus_Progression_Y7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Y7;
            eta_Taus_Progression_Y7.push_back(-99.); eta_Taus_Progression_Y7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Z7;
            Index_Taus_Progression_Z7.push_back(-1); Index_Taus_Progression_Z7.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Z7;
            pt_Taus_Progression_Z7.push_back(-99.); pt_Taus_Progression_Z7.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Z7;
            eta_Taus_Progression_Z7.push_back(-99.); eta_Taus_Progression_Z7.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_A8;
            Index_Taus_Progression_A8.push_back(-1); Index_Taus_Progression_A8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_A8;
            pt_Taus_Progression_A8.push_back(-99.); pt_Taus_Progression_A8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_A8;
            eta_Taus_Progression_A8.push_back(-99.); eta_Taus_Progression_A8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_B8;
            Index_Taus_Progression_B8.push_back(-1); Index_Taus_Progression_B8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_B8;
            pt_Taus_Progression_B8.push_back(-99.); pt_Taus_Progression_B8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_B8;
            eta_Taus_Progression_B8.push_back(-99.); eta_Taus_Progression_B8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_C8;
            Index_Taus_Progression_C8.push_back(-1); Index_Taus_Progression_C8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_C8;
            pt_Taus_Progression_C8.push_back(-99.); pt_Taus_Progression_C8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_C8;
            eta_Taus_Progression_C8.push_back(-99.); eta_Taus_Progression_C8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_D8;
            Index_Taus_Progression_D8.push_back(-1); Index_Taus_Progression_D8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_D8;
            pt_Taus_Progression_D8.push_back(-99.); pt_Taus_Progression_D8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_D8;
            eta_Taus_Progression_D8.push_back(-99.); eta_Taus_Progression_D8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_E8;
            Index_Taus_Progression_E8.push_back(-1); Index_Taus_Progression_E8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_E8;
            pt_Taus_Progression_E8.push_back(-99.); pt_Taus_Progression_E8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_E8;
            eta_Taus_Progression_E8.push_back(-99.); eta_Taus_Progression_E8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_F8;
            Index_Taus_Progression_F8.push_back(-1); Index_Taus_Progression_F8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_F8;
            pt_Taus_Progression_F8.push_back(-99.); pt_Taus_Progression_F8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_F8;
            eta_Taus_Progression_F8.push_back(-99.); eta_Taus_Progression_F8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_G8;
            Index_Taus_Progression_G8.push_back(-1); Index_Taus_Progression_G8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_G8;
            pt_Taus_Progression_G8.push_back(-99.); pt_Taus_Progression_G8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_G8;
            eta_Taus_Progression_G8.push_back(-99.); eta_Taus_Progression_G8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_H8;
            Index_Taus_Progression_H8.push_back(-1); Index_Taus_Progression_H8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_H8;
            pt_Taus_Progression_H8.push_back(-99.); pt_Taus_Progression_H8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_H8;
            eta_Taus_Progression_H8.push_back(-99.); eta_Taus_Progression_H8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_I8;
            Index_Taus_Progression_I8.push_back(-1); Index_Taus_Progression_I8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_I8;
            pt_Taus_Progression_I8.push_back(-99.); pt_Taus_Progression_I8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_I8;
            eta_Taus_Progression_I8.push_back(-99.); eta_Taus_Progression_I8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_J8;
            Index_Taus_Progression_J8.push_back(-1); Index_Taus_Progression_J8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_J8;
            pt_Taus_Progression_J8.push_back(-99.); pt_Taus_Progression_J8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_J8;
            eta_Taus_Progression_J8.push_back(-99.); eta_Taus_Progression_J8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_K8;
            Index_Taus_Progression_K8.push_back(-1); Index_Taus_Progression_K8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_K8;
            pt_Taus_Progression_K8.push_back(-99.); pt_Taus_Progression_K8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_K8;
            eta_Taus_Progression_K8.push_back(-99.); eta_Taus_Progression_K8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_L8;
            Index_Taus_Progression_L8.push_back(-1); Index_Taus_Progression_L8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_L8;
            pt_Taus_Progression_L8.push_back(-99.); pt_Taus_Progression_L8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_L8;
            eta_Taus_Progression_L8.push_back(-99.); eta_Taus_Progression_L8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_M8;
            Index_Taus_Progression_M8.push_back(-1); Index_Taus_Progression_M8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_M8;
            pt_Taus_Progression_M8.push_back(-99.); pt_Taus_Progression_M8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_M8;
            eta_Taus_Progression_M8.push_back(-99.); eta_Taus_Progression_M8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_N8;
            Index_Taus_Progression_N8.push_back(-1); Index_Taus_Progression_N8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_N8;
            pt_Taus_Progression_N8.push_back(-99.); pt_Taus_Progression_N8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_N8;
            eta_Taus_Progression_N8.push_back(-99.); eta_Taus_Progression_N8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_O8;
            Index_Taus_Progression_O8.push_back(-1); Index_Taus_Progression_O8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_O8;
            pt_Taus_Progression_O8.push_back(-99.); pt_Taus_Progression_O8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_O8;
            eta_Taus_Progression_O8.push_back(-99.); eta_Taus_Progression_O8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_P8;
            Index_Taus_Progression_P8.push_back(-1); Index_Taus_Progression_P8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_P8;
            pt_Taus_Progression_P8.push_back(-99.); pt_Taus_Progression_P8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_P8;
            eta_Taus_Progression_P8.push_back(-99.); eta_Taus_Progression_P8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Q8;
            Index_Taus_Progression_Q8.push_back(-1); Index_Taus_Progression_Q8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Q8;
            pt_Taus_Progression_Q8.push_back(-99.); pt_Taus_Progression_Q8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Q8;
            eta_Taus_Progression_Q8.push_back(-99.); eta_Taus_Progression_Q8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_R8;
            Index_Taus_Progression_R8.push_back(-1); Index_Taus_Progression_R8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_R8;
            pt_Taus_Progression_R8.push_back(-99.); pt_Taus_Progression_R8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_R8;
            eta_Taus_Progression_R8.push_back(-99.); eta_Taus_Progression_R8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_S8;
            Index_Taus_Progression_S8.push_back(-1); Index_Taus_Progression_S8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_S8;
            pt_Taus_Progression_S8.push_back(-99.); pt_Taus_Progression_S8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_S8;
            eta_Taus_Progression_S8.push_back(-99.); eta_Taus_Progression_S8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_T8;
            Index_Taus_Progression_T8.push_back(-1); Index_Taus_Progression_T8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_T8;
            pt_Taus_Progression_T8.push_back(-99.); pt_Taus_Progression_T8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_T8;
            eta_Taus_Progression_T8.push_back(-99.); eta_Taus_Progression_T8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_U8;
            Index_Taus_Progression_U8.push_back(-1); Index_Taus_Progression_U8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_U8;
            pt_Taus_Progression_U8.push_back(-99.); pt_Taus_Progression_U8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_U8;
            eta_Taus_Progression_U8.push_back(-99.); eta_Taus_Progression_U8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_V8;
            Index_Taus_Progression_V8.push_back(-1); Index_Taus_Progression_V8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_V8;
            pt_Taus_Progression_V8.push_back(-99.); pt_Taus_Progression_V8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_V8;
            eta_Taus_Progression_V8.push_back(-99.); eta_Taus_Progression_V8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_W8;
            Index_Taus_Progression_W8.push_back(-1); Index_Taus_Progression_W8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_W8;
            pt_Taus_Progression_W8.push_back(-99.); pt_Taus_Progression_W8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_W8;
            eta_Taus_Progression_W8.push_back(-99.); eta_Taus_Progression_W8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_X8;
            Index_Taus_Progression_X8.push_back(-1); Index_Taus_Progression_X8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_X8;
            pt_Taus_Progression_X8.push_back(-99.); pt_Taus_Progression_X8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_X8;
            eta_Taus_Progression_X8.push_back(-99.); eta_Taus_Progression_X8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Y8;
            Index_Taus_Progression_Y8.push_back(-1); Index_Taus_Progression_Y8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Y8;
            pt_Taus_Progression_Y8.push_back(-99.); pt_Taus_Progression_Y8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Y8;
            eta_Taus_Progression_Y8.push_back(-99.); eta_Taus_Progression_Y8.push_back(-99.);

            std::vector<Int_t> Index_Taus_Progression_Z8;
            Index_Taus_Progression_Z8.push_back(-1); Index_Taus_Progression_Z8.push_back(-1);
            std::vector<Float_t> pt_Taus_Progression_Z8;
            pt_Taus_Progression_Z8.push_back(-99.); pt_Taus_Progression_Z8.push_back(-99.);
            std::vector<Float_t> eta_Taus_Progression_Z8;
            eta_Taus_Progression_Z8.push_back(-99.); eta_Taus_Progression_Z8.push_back(-99.);
            // END OF FULL BLOWN GRID SEARCH

            for(UInt_t iL1Tau = 0 ; iL1Tau < in_CalibPt->size() ; ++iL1Tau)
                {
                    if(fabs(in_l1tEmuEta->at(iL1Tau))>2.1315) continue;

                    Int_t IsoCut_50pc = histosIsolation["LUT_WP50"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    Int_t IsoCut_60pc = histosIsolation["LUT_WP60"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    Int_t IsoCut_70pc = histosIsolation["LUT_WP70"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    Int_t IsoCut_80pc = histosIsolation["LUT_WP80"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    Int_t IsoCut_90pc = histosIsolation["LUT_WP90"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

                    TString Result_Progression_1 = "LUT_Progression_1";
                    Int_t IsoCut_Progression_1 = histosIsolation[Result_Progression_1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_1)
                        {
                            pt_Progression_1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_1 = kTRUE;
                        }

                    TString Result_Progression_2 = "LUT_Progression_2";
                    Int_t IsoCut_Progression_2 = histosIsolation[Result_Progression_2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_2)
                        {
                            pt_Progression_2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_2 = kTRUE;
                        }

                    TString Result_Progression_3 = "LUT_Progression_3";
                    Int_t IsoCut_Progression_3 = histosIsolation[Result_Progression_3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_3)
                        {
                            pt_Progression_3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_3 = kTRUE;
                        }

                    TString Result_Progression_4 = "LUT_Progression_4";
                    Int_t IsoCut_Progression_4 = histosIsolation[Result_Progression_4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_4)
                        {
                            pt_Progression_4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_4 = kTRUE;
                        }

                    TString Result_Progression_5 = "LUT_Progression_5";
                    Int_t IsoCut_Progression_5 = histosIsolation[Result_Progression_5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_5)
                        {
                            pt_Progression_5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_5 = kTRUE;
                        }

                    TString Result_Progression_6 = "LUT_Progression_6";
                    Int_t IsoCut_Progression_6 = histosIsolation[Result_Progression_6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_6)
                        {
                            pt_Progression_6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_6 = kTRUE;
                        }

                    TString Result_Progression_7 = "LUT_Progression_7";
                    Int_t IsoCut_Progression_7 = histosIsolation[Result_Progression_7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_7)
                        {
                            pt_Progression_7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_7 = kTRUE;
                        }

                    TString Result_Progression_8 = "LUT_Progression_8";
                    Int_t IsoCut_Progression_8 = histosIsolation[Result_Progression_8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_8)
                        {
                            pt_Progression_8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_8 = kTRUE;
                        }

                    TString Result_Progression_9 = "LUT_Progression_9";
                    Int_t IsoCut_Progression_9 = histosIsolation[Result_Progression_9]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_9 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_9)
                        {
                            pt_Progression_9->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_9 = kTRUE;
                        }

                    TString Result_Progression_10 = "LUT_Progression_10";
                    Int_t IsoCut_Progression_10 = histosIsolation[Result_Progression_10]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_10 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_10)
                        {
                            pt_Progression_10->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_10 = kTRUE;
                        }

                    TString Result_Progression_11 = "LUT_Progression_11";
                    Int_t IsoCut_Progression_11 = histosIsolation[Result_Progression_11]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_11 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_11)
                        {
                            pt_Progression_11->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_11 = kTRUE;
                        }

                    TString Result_Progression_12 = "LUT_Progression_12";
                    Int_t IsoCut_Progression_12 = histosIsolation[Result_Progression_12]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_12 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_12)
                        {
                            pt_Progression_12->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_12 = kTRUE;
                        }

                    TString Result_Progression_13 = "LUT_Progression_13";
                    Int_t IsoCut_Progression_13 = histosIsolation[Result_Progression_13]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_13 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_13)
                        {
                            pt_Progression_13->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_13 = kTRUE;
                        }

                    TString Result_Progression_14 = "LUT_Progression_14";
                    Int_t IsoCut_Progression_14 = histosIsolation[Result_Progression_14]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_14 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_14)
                        {
                            pt_Progression_14->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_14 = kTRUE;
                        }

                    TString Result_Progression_15 = "LUT_Progression_15";
                    Int_t IsoCut_Progression_15 = histosIsolation[Result_Progression_15]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_15 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_15)
                        {
                            pt_Progression_15->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_15 = kTRUE;
                        }

                    TString Result_Progression_16 = "LUT_Progression_16";
                    Int_t IsoCut_Progression_16 = histosIsolation[Result_Progression_16]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_16 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_16)
                        {
                            pt_Progression_16->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_16 = kTRUE;
                        }

                    TString Result_Progression_17 = "LUT_Progression_17";
                    Int_t IsoCut_Progression_17 = histosIsolation[Result_Progression_17]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_17 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_17)
                        {
                            pt_Progression_17->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_17 = kTRUE;
                        }

                    TString Result_Progression_18 = "LUT_Progression_18";
                    Int_t IsoCut_Progression_18 = histosIsolation[Result_Progression_18]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_18 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_18)
                        {
                            pt_Progression_18->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_18 = kTRUE;
                        }

                    TString Result_Progression_19 = "LUT_Progression_19";
                    Int_t IsoCut_Progression_19 = histosIsolation[Result_Progression_19]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_19 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_19)
                        {
                            pt_Progression_19->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_19 = kTRUE;
                        }

                    TString Result_Progression_20 = "LUT_Progression_20";
                    Int_t IsoCut_Progression_20 = histosIsolation[Result_Progression_20]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_20 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_20)
                        {
                            pt_Progression_20->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_20 = kTRUE;
                        }

                    TString Result_Progression_21 = "LUT_Progression_21";
                    Int_t IsoCut_Progression_21 = histosIsolation[Result_Progression_21]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_21 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_21)
                        {
                            pt_Progression_21->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_21 = kTRUE;
                        }

                    TString Result_Progression_22 = "LUT_Progression_22";
                    Int_t IsoCut_Progression_22 = histosIsolation[Result_Progression_22]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_22 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22)
                        {
                            pt_Progression_22->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_22 = kTRUE;
                        }

                    TString Result_Progression_23 = "LUT_Progression_23";
                    Int_t IsoCut_Progression_23 = histosIsolation[Result_Progression_23]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

                    TString Result_Progression_24 = "LUT_Progression_24";
                    Int_t IsoCut_Progression_24 = histosIsolation[Result_Progression_24]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

                    TString Result_Progression_25 = "LUT_Progression_25";
                    Int_t IsoCut_Progression_25 = histosIsolation[Result_Progression_25]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

                    TString Result_Progression_26 = "LUT_Progression_26";
                    Int_t IsoCut_Progression_26 = histosIsolation[Result_Progression_26]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

                    TString Result_Progression_27 = "LUT_Progression_27";
                    Int_t IsoCut_Progression_27 = histosIsolation[Result_Progression_27]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

                    TString Result_Progression_28 = "LUT_Progression_28";
                    Int_t IsoCut_Progression_28 = histosIsolation[Result_Progression_28]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

                    TString Result_Progression_29 = "LUT_Progression_29";
                    Int_t IsoCut_Progression_29 = histosIsolation[Result_Progression_29]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

                    TString Result_Progression_30 = "LUT_Progression_30";
                    Int_t IsoCut_Progression_30 = histosIsolation[Result_Progression_30]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

                    TString Result_Progression_31 = "LUT_Progression_31";
                    Int_t IsoCut_Progression_31 = histosIsolation[Result_Progression_31]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

                    TString Result_Progression_31_extrap = "LUT_Progression_31_extrap";
                    Int_t IsoCut_Progression_31_extrap = histosIsolation[Result_Progression_31_extrap]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);

                    // START OF FULL BLOWN GRID SEARCH
                    TString Result_Progression_A0 = "LUT_Progression_A0";
                    Int_t IsoCut_Progression_A0 = histosIsolation[Result_Progression_A0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_A0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A0)
                        {
                            pt_Progression_A0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_A0 = kTRUE;
                        }

                    TString Result_Progression_B0 = "LUT_Progression_B0";
                    Int_t IsoCut_Progression_B0 = histosIsolation[Result_Progression_B0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_B0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B0)
                        {
                            pt_Progression_B0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_B0 = kTRUE;
                        }

                    TString Result_Progression_C0 = "LUT_Progression_C0";
                    Int_t IsoCut_Progression_C0 = histosIsolation[Result_Progression_C0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_C0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C0)
                        {
                            pt_Progression_C0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_C0 = kTRUE;
                        }

                    TString Result_Progression_D0 = "LUT_Progression_D0";
                    Int_t IsoCut_Progression_D0 = histosIsolation[Result_Progression_D0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_D0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D0)
                        {
                            pt_Progression_D0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_D0 = kTRUE;
                        }

                    TString Result_Progression_E0 = "LUT_Progression_E0";
                    Int_t IsoCut_Progression_E0 = histosIsolation[Result_Progression_E0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_E0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E0)
                        {
                            pt_Progression_E0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_E0 = kTRUE;
                        }

                    TString Result_Progression_F0 = "LUT_Progression_F0";
                    Int_t IsoCut_Progression_F0 = histosIsolation[Result_Progression_F0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_F0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F0)
                        {
                            pt_Progression_F0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_F0 = kTRUE;
                        }

                    TString Result_Progression_G0 = "LUT_Progression_G0";
                    Int_t IsoCut_Progression_G0 = histosIsolation[Result_Progression_G0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_G0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G0)
                        {
                            pt_Progression_G0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_G0 = kTRUE;
                        }

                    TString Result_Progression_H0 = "LUT_Progression_H0";
                    Int_t IsoCut_Progression_H0 = histosIsolation[Result_Progression_H0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_H0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H0)
                        {
                            pt_Progression_H0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_H0 = kTRUE;
                        }

                    TString Result_Progression_I0 = "LUT_Progression_I0";
                    Int_t IsoCut_Progression_I0 = histosIsolation[Result_Progression_I0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_I0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I0)
                        {
                            pt_Progression_I0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_I0 = kTRUE;
                        }

                    TString Result_Progression_J0 = "LUT_Progression_J0";
                    Int_t IsoCut_Progression_J0 = histosIsolation[Result_Progression_J0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_J0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J0)
                        {
                            pt_Progression_J0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_J0 = kTRUE;
                        }

                    TString Result_Progression_K0 = "LUT_Progression_K0";
                    Int_t IsoCut_Progression_K0 = histosIsolation[Result_Progression_K0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_K0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K0)
                        {
                            pt_Progression_K0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_K0 = kTRUE;
                        }

                    TString Result_Progression_L0 = "LUT_Progression_L0";
                    Int_t IsoCut_Progression_L0 = histosIsolation[Result_Progression_L0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_L0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L0)
                        {
                            pt_Progression_L0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_L0 = kTRUE;
                        }

                    TString Result_Progression_M0 = "LUT_Progression_M0";
                    Int_t IsoCut_Progression_M0 = histosIsolation[Result_Progression_M0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_M0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M0)
                        {
                            pt_Progression_M0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_M0 = kTRUE;
                        }

                    TString Result_Progression_N0 = "LUT_Progression_N0";
                    Int_t IsoCut_Progression_N0 = histosIsolation[Result_Progression_N0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_N0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N0)
                        {
                            pt_Progression_N0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_N0 = kTRUE;
                        }

                    TString Result_Progression_O0 = "LUT_Progression_O0";
                    Int_t IsoCut_Progression_O0 = histosIsolation[Result_Progression_O0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_O0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O0)
                        {
                            pt_Progression_O0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_O0 = kTRUE;
                        }

                    TString Result_Progression_P0 = "LUT_Progression_P0";
                    Int_t IsoCut_Progression_P0 = histosIsolation[Result_Progression_P0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_P0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P0)
                        {
                            pt_Progression_P0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_P0 = kTRUE;
                        }

                    TString Result_Progression_Q0 = "LUT_Progression_Q0";
                    Int_t IsoCut_Progression_Q0 = histosIsolation[Result_Progression_Q0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Q0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q0)
                        {
                            pt_Progression_Q0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Q0 = kTRUE;
                        }

                    TString Result_Progression_R0 = "LUT_Progression_R0";
                    Int_t IsoCut_Progression_R0 = histosIsolation[Result_Progression_R0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_R0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R0)
                        {
                            pt_Progression_R0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_R0 = kTRUE;
                        }

                    TString Result_Progression_S0 = "LUT_Progression_S0";
                    Int_t IsoCut_Progression_S0 = histosIsolation[Result_Progression_S0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_S0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S0)
                        {
                            pt_Progression_S0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_S0 = kTRUE;
                        }

                    TString Result_Progression_T0 = "LUT_Progression_T0";
                    Int_t IsoCut_Progression_T0 = histosIsolation[Result_Progression_T0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_T0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T0)
                        {
                            pt_Progression_T0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_T0 = kTRUE;
                        }

                    TString Result_Progression_U0 = "LUT_Progression_U0";
                    Int_t IsoCut_Progression_U0 = histosIsolation[Result_Progression_U0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_U0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U0)
                        {
                            pt_Progression_U0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_U0 = kTRUE;
                        }

                    TString Result_Progression_V0 = "LUT_Progression_V0";
                    Int_t IsoCut_Progression_V0 = histosIsolation[Result_Progression_V0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_V0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V0)
                        {
                            pt_Progression_V0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_V0 = kTRUE;
                        }

                    TString Result_Progression_W0 = "LUT_Progression_W0";
                    Int_t IsoCut_Progression_W0 = histosIsolation[Result_Progression_W0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_W0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W0)
                        {
                            pt_Progression_W0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_W0 = kTRUE;
                        }

                    TString Result_Progression_X0 = "LUT_Progression_X0";
                    Int_t IsoCut_Progression_X0 = histosIsolation[Result_Progression_X0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_X0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X0)
                        {
                            pt_Progression_X0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_X0 = kTRUE;
                        }

                    TString Result_Progression_Y0 = "LUT_Progression_Y0";
                    Int_t IsoCut_Progression_Y0 = histosIsolation[Result_Progression_Y0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Y0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y0)
                        {
                            pt_Progression_Y0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Y0 = kTRUE;
                        }

                    TString Result_Progression_Z0 = "LUT_Progression_Z0";
                    Int_t IsoCut_Progression_Z0 = histosIsolation[Result_Progression_Z0]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Z0 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z0)
                        {
                            pt_Progression_Z0->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Z0 = kTRUE;
                        }

                    TString Result_Progression_A1 = "LUT_Progression_A1";
                    Int_t IsoCut_Progression_A1 = histosIsolation[Result_Progression_A1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_A1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A1)
                        {
                            pt_Progression_A1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_A1 = kTRUE;
                        }

                    TString Result_Progression_B1 = "LUT_Progression_B1";
                    Int_t IsoCut_Progression_B1 = histosIsolation[Result_Progression_B1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_B1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B1)
                        {
                            pt_Progression_B1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_B1 = kTRUE;
                        }

                    TString Result_Progression_C1 = "LUT_Progression_C1";
                    Int_t IsoCut_Progression_C1 = histosIsolation[Result_Progression_C1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_C1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C1)
                        {
                            pt_Progression_C1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_C1 = kTRUE;
                        }

                    TString Result_Progression_D1 = "LUT_Progression_D1";
                    Int_t IsoCut_Progression_D1 = histosIsolation[Result_Progression_D1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_D1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D1)
                        {
                            pt_Progression_D1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_D1 = kTRUE;
                        }

                    TString Result_Progression_E1 = "LUT_Progression_E1";
                    Int_t IsoCut_Progression_E1 = histosIsolation[Result_Progression_E1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_E1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E1)
                        {
                            pt_Progression_E1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_E1 = kTRUE;
                        }

                    TString Result_Progression_F1 = "LUT_Progression_F1";
                    Int_t IsoCut_Progression_F1 = histosIsolation[Result_Progression_F1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_F1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F1)
                        {
                            pt_Progression_F1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_F1 = kTRUE;
                        }

                    TString Result_Progression_G1 = "LUT_Progression_G1";
                    Int_t IsoCut_Progression_G1 = histosIsolation[Result_Progression_G1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_G1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G1)
                        {
                            pt_Progression_G1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_G1 = kTRUE;
                        }

                    TString Result_Progression_H1 = "LUT_Progression_H1";
                    Int_t IsoCut_Progression_H1 = histosIsolation[Result_Progression_H1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_H1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H1)
                        {
                            pt_Progression_H1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_H1 = kTRUE;
                        }

                    TString Result_Progression_I1 = "LUT_Progression_I1";
                    Int_t IsoCut_Progression_I1 = histosIsolation[Result_Progression_I1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_I1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I1)
                        {
                            pt_Progression_I1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_I1 = kTRUE;
                        }

                    TString Result_Progression_J1 = "LUT_Progression_J1";
                    Int_t IsoCut_Progression_J1 = histosIsolation[Result_Progression_J1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_J1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J1)
                        {
                            pt_Progression_J1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_J1 = kTRUE;
                        }

                    TString Result_Progression_K1 = "LUT_Progression_K1";
                    Int_t IsoCut_Progression_K1 = histosIsolation[Result_Progression_K1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_K1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K1)
                        {
                            pt_Progression_K1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_K1 = kTRUE;
                        }

                    TString Result_Progression_L1 = "LUT_Progression_L1";
                    Int_t IsoCut_Progression_L1 = histosIsolation[Result_Progression_L1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_L1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L1)
                        {
                            pt_Progression_L1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_L1 = kTRUE;
                        }

                    TString Result_Progression_M1 = "LUT_Progression_M1";
                    Int_t IsoCut_Progression_M1 = histosIsolation[Result_Progression_M1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_M1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M1)
                        {
                            pt_Progression_M1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_M1 = kTRUE;
                        }

                    TString Result_Progression_N1 = "LUT_Progression_N1";
                    Int_t IsoCut_Progression_N1 = histosIsolation[Result_Progression_N1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_N1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N1)
                        {
                            pt_Progression_N1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_N1 = kTRUE;
                        }

                    TString Result_Progression_O1 = "LUT_Progression_O1";
                    Int_t IsoCut_Progression_O1 = histosIsolation[Result_Progression_O1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_O1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O1)
                        {
                            pt_Progression_O1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_O1 = kTRUE;
                        }

                    TString Result_Progression_P1 = "LUT_Progression_P1";
                    Int_t IsoCut_Progression_P1 = histosIsolation[Result_Progression_P1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_P1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P1)
                        {
                            pt_Progression_P1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_P1 = kTRUE;
                        }

                    TString Result_Progression_Q1 = "LUT_Progression_Q1";
                    Int_t IsoCut_Progression_Q1 = histosIsolation[Result_Progression_Q1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Q1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q1)
                        {
                            pt_Progression_Q1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Q1 = kTRUE;
                        }

                    TString Result_Progression_R1 = "LUT_Progression_R1";
                    Int_t IsoCut_Progression_R1 = histosIsolation[Result_Progression_R1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_R1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R1)
                        {
                            pt_Progression_R1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_R1 = kTRUE;
                        }

                    TString Result_Progression_S1 = "LUT_Progression_S1";
                    Int_t IsoCut_Progression_S1 = histosIsolation[Result_Progression_S1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_S1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S1)
                        {
                            pt_Progression_S1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_S1 = kTRUE;
                        }

                    TString Result_Progression_T1 = "LUT_Progression_T1";
                    Int_t IsoCut_Progression_T1 = histosIsolation[Result_Progression_T1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_T1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T1)
                        {
                            pt_Progression_T1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_T1 = kTRUE;
                        }

                    TString Result_Progression_U1 = "LUT_Progression_U1";
                    Int_t IsoCut_Progression_U1 = histosIsolation[Result_Progression_U1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_U1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U1)
                        {
                            pt_Progression_U1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_U1 = kTRUE;
                        }

                    TString Result_Progression_V1 = "LUT_Progression_V1";
                    Int_t IsoCut_Progression_V1 = histosIsolation[Result_Progression_V1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_V1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V1)
                        {
                            pt_Progression_V1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_V1 = kTRUE;
                        }

                    TString Result_Progression_W1 = "LUT_Progression_W1";
                    Int_t IsoCut_Progression_W1 = histosIsolation[Result_Progression_W1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_W1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W1)
                        {
                            pt_Progression_W1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_W1 = kTRUE;
                        }

                    TString Result_Progression_X1 = "LUT_Progression_X1";
                    Int_t IsoCut_Progression_X1 = histosIsolation[Result_Progression_X1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_X1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X1)
                        {
                            pt_Progression_X1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_X1 = kTRUE;
                        }

                    TString Result_Progression_Y1 = "LUT_Progression_Y1";
                    Int_t IsoCut_Progression_Y1 = histosIsolation[Result_Progression_Y1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Y1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y1)
                        {
                            pt_Progression_Y1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Y1 = kTRUE;
                        }

                    TString Result_Progression_Z1 = "LUT_Progression_Z1";
                    Int_t IsoCut_Progression_Z1 = histosIsolation[Result_Progression_Z1]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Z1 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z1)
                        {
                            pt_Progression_Z1->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Z1 = kTRUE;
                        }

                    TString Result_Progression_A2 = "LUT_Progression_A2";
                    Int_t IsoCut_Progression_A2 = histosIsolation[Result_Progression_A2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_A2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A2)
                        {
                            pt_Progression_A2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_A2 = kTRUE;
                        }

                    TString Result_Progression_B2 = "LUT_Progression_B2";
                    Int_t IsoCut_Progression_B2 = histosIsolation[Result_Progression_B2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_B2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B2)
                        {
                            pt_Progression_B2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_B2 = kTRUE;
                        }

                    TString Result_Progression_C2 = "LUT_Progression_C2";
                    Int_t IsoCut_Progression_C2 = histosIsolation[Result_Progression_C2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_C2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C2)
                        {
                            pt_Progression_C2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_C2 = kTRUE;
                        }

                    TString Result_Progression_D2 = "LUT_Progression_D2";
                    Int_t IsoCut_Progression_D2 = histosIsolation[Result_Progression_D2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_D2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D2)
                        {
                            pt_Progression_D2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_D2 = kTRUE;
                        }

                    TString Result_Progression_E2 = "LUT_Progression_E2";
                    Int_t IsoCut_Progression_E2 = histosIsolation[Result_Progression_E2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_E2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E2)
                        {
                            pt_Progression_E2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_E2 = kTRUE;
                        }

                    TString Result_Progression_F2 = "LUT_Progression_F2";
                    Int_t IsoCut_Progression_F2 = histosIsolation[Result_Progression_F2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_F2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F2)
                        {
                            pt_Progression_F2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_F2 = kTRUE;
                        }

                    TString Result_Progression_G2 = "LUT_Progression_G2";
                    Int_t IsoCut_Progression_G2 = histosIsolation[Result_Progression_G2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_G2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G2)
                        {
                            pt_Progression_G2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_G2 = kTRUE;
                        }

                    TString Result_Progression_H2 = "LUT_Progression_H2";
                    Int_t IsoCut_Progression_H2 = histosIsolation[Result_Progression_H2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_H2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H2)
                        {
                            pt_Progression_H2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_H2 = kTRUE;
                        }

                    TString Result_Progression_I2 = "LUT_Progression_I2";
                    Int_t IsoCut_Progression_I2 = histosIsolation[Result_Progression_I2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_I2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I2)
                        {
                            pt_Progression_I2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_I2 = kTRUE;
                        }

                    TString Result_Progression_J2 = "LUT_Progression_J2";
                    Int_t IsoCut_Progression_J2 = histosIsolation[Result_Progression_J2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_J2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J2)
                        {
                            pt_Progression_J2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_J2 = kTRUE;
                        }

                    TString Result_Progression_K2 = "LUT_Progression_K2";
                    Int_t IsoCut_Progression_K2 = histosIsolation[Result_Progression_K2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_K2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K2)
                        {
                            pt_Progression_K2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_K2 = kTRUE;
                        }

                    TString Result_Progression_L2 = "LUT_Progression_L2";
                    Int_t IsoCut_Progression_L2 = histosIsolation[Result_Progression_L2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_L2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L2)
                        {
                            pt_Progression_L2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_L2 = kTRUE;
                        }

                    TString Result_Progression_M2 = "LUT_Progression_M2";
                    Int_t IsoCut_Progression_M2 = histosIsolation[Result_Progression_M2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_M2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M2)
                        {
                            pt_Progression_M2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_M2 = kTRUE;
                        }

                    TString Result_Progression_N2 = "LUT_Progression_N2";
                    Int_t IsoCut_Progression_N2 = histosIsolation[Result_Progression_N2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_N2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N2)
                        {
                            pt_Progression_N2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_N2 = kTRUE;
                        }

                    TString Result_Progression_O2 = "LUT_Progression_O2";
                    Int_t IsoCut_Progression_O2 = histosIsolation[Result_Progression_O2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_O2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O2)
                        {
                            pt_Progression_O2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_O2 = kTRUE;
                        }

                    TString Result_Progression_P2 = "LUT_Progression_P2";
                    Int_t IsoCut_Progression_P2 = histosIsolation[Result_Progression_P2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_P2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P2)
                        {
                            pt_Progression_P2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_P2 = kTRUE;
                        }

                    TString Result_Progression_Q2 = "LUT_Progression_Q2";
                    Int_t IsoCut_Progression_Q2 = histosIsolation[Result_Progression_Q2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Q2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q2)
                        {
                            pt_Progression_Q2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Q2 = kTRUE;
                        }

                    TString Result_Progression_R2 = "LUT_Progression_R2";
                    Int_t IsoCut_Progression_R2 = histosIsolation[Result_Progression_R2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_R2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R2)
                        {
                            pt_Progression_R2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_R2 = kTRUE;
                        }

                    TString Result_Progression_S2 = "LUT_Progression_S2";
                    Int_t IsoCut_Progression_S2 = histosIsolation[Result_Progression_S2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_S2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S2)
                        {
                            pt_Progression_S2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_S2 = kTRUE;
                        }

                    TString Result_Progression_T2 = "LUT_Progression_T2";
                    Int_t IsoCut_Progression_T2 = histosIsolation[Result_Progression_T2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_T2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T2)
                        {
                            pt_Progression_T2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_T2 = kTRUE;
                        }

                    TString Result_Progression_U2 = "LUT_Progression_U2";
                    Int_t IsoCut_Progression_U2 = histosIsolation[Result_Progression_U2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_U2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U2)
                        {
                            pt_Progression_U2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_U2 = kTRUE;
                        }

                    TString Result_Progression_V2 = "LUT_Progression_V2";
                    Int_t IsoCut_Progression_V2 = histosIsolation[Result_Progression_V2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_V2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V2)
                        {
                            pt_Progression_V2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_V2 = kTRUE;
                        }

                    TString Result_Progression_W2 = "LUT_Progression_W2";
                    Int_t IsoCut_Progression_W2 = histosIsolation[Result_Progression_W2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_W2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W2)
                        {
                            pt_Progression_W2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_W2 = kTRUE;
                        }

                    TString Result_Progression_X2 = "LUT_Progression_X2";
                    Int_t IsoCut_Progression_X2 = histosIsolation[Result_Progression_X2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_X2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X2)
                        {
                            pt_Progression_X2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_X2 = kTRUE;
                        }

                    TString Result_Progression_Y2 = "LUT_Progression_Y2";
                    Int_t IsoCut_Progression_Y2 = histosIsolation[Result_Progression_Y2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Y2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y2)
                        {
                            pt_Progression_Y2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Y2 = kTRUE;
                        }

                    TString Result_Progression_Z2 = "LUT_Progression_Z2";
                    Int_t IsoCut_Progression_Z2 = histosIsolation[Result_Progression_Z2]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Z2 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z2)
                        {
                            pt_Progression_Z2->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Z2 = kTRUE;
                        }

                    TString Result_Progression_A3 = "LUT_Progression_A3";
                    Int_t IsoCut_Progression_A3 = histosIsolation[Result_Progression_A3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_A3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A3)
                        {
                            pt_Progression_A3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_A3 = kTRUE;
                        }

                    TString Result_Progression_B3 = "LUT_Progression_B3";
                    Int_t IsoCut_Progression_B3 = histosIsolation[Result_Progression_B3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_B3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B3)
                        {
                            pt_Progression_B3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_B3 = kTRUE;
                        }

                    TString Result_Progression_C3 = "LUT_Progression_C3";
                    Int_t IsoCut_Progression_C3 = histosIsolation[Result_Progression_C3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_C3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C3)
                        {
                            pt_Progression_C3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_C3 = kTRUE;
                        }

                    TString Result_Progression_D3 = "LUT_Progression_D3";
                    Int_t IsoCut_Progression_D3 = histosIsolation[Result_Progression_D3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_D3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D3)
                        {
                            pt_Progression_D3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_D3 = kTRUE;
                        }

                    TString Result_Progression_E3 = "LUT_Progression_E3";
                    Int_t IsoCut_Progression_E3 = histosIsolation[Result_Progression_E3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_E3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E3)
                        {
                            pt_Progression_E3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_E3 = kTRUE;
                        }

                    TString Result_Progression_F3 = "LUT_Progression_F3";
                    Int_t IsoCut_Progression_F3 = histosIsolation[Result_Progression_F3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_F3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F3)
                        {
                            pt_Progression_F3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_F3 = kTRUE;
                        }

                    TString Result_Progression_G3 = "LUT_Progression_G3";
                    Int_t IsoCut_Progression_G3 = histosIsolation[Result_Progression_G3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_G3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G3)
                        {
                            pt_Progression_G3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_G3 = kTRUE;
                        }

                    TString Result_Progression_H3 = "LUT_Progression_H3";
                    Int_t IsoCut_Progression_H3 = histosIsolation[Result_Progression_H3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_H3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H3)
                        {
                            pt_Progression_H3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_H3 = kTRUE;
                        }

                    TString Result_Progression_I3 = "LUT_Progression_I3";
                    Int_t IsoCut_Progression_I3 = histosIsolation[Result_Progression_I3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_I3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I3)
                        {
                            pt_Progression_I3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_I3 = kTRUE;
                        }

                    TString Result_Progression_J3 = "LUT_Progression_J3";
                    Int_t IsoCut_Progression_J3 = histosIsolation[Result_Progression_J3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_J3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J3)
                        {
                            pt_Progression_J3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_J3 = kTRUE;
                        }

                    TString Result_Progression_K3 = "LUT_Progression_K3";
                    Int_t IsoCut_Progression_K3 = histosIsolation[Result_Progression_K3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_K3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K3)
                        {
                            pt_Progression_K3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_K3 = kTRUE;
                        }

                    TString Result_Progression_L3 = "LUT_Progression_L3";
                    Int_t IsoCut_Progression_L3 = histosIsolation[Result_Progression_L3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_L3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L3)
                        {
                            pt_Progression_L3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_L3 = kTRUE;
                        }

                    TString Result_Progression_M3 = "LUT_Progression_M3";
                    Int_t IsoCut_Progression_M3 = histosIsolation[Result_Progression_M3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_M3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M3)
                        {
                            pt_Progression_M3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_M3 = kTRUE;
                        }

                    TString Result_Progression_N3 = "LUT_Progression_N3";
                    Int_t IsoCut_Progression_N3 = histosIsolation[Result_Progression_N3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_N3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N3)
                        {
                            pt_Progression_N3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_N3 = kTRUE;
                        }

                    TString Result_Progression_O3 = "LUT_Progression_O3";
                    Int_t IsoCut_Progression_O3 = histosIsolation[Result_Progression_O3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_O3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O3)
                        {
                            pt_Progression_O3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_O3 = kTRUE;
                        }

                    TString Result_Progression_P3 = "LUT_Progression_P3";
                    Int_t IsoCut_Progression_P3 = histosIsolation[Result_Progression_P3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_P3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P3)
                        {
                            pt_Progression_P3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_P3 = kTRUE;
                        }

                    TString Result_Progression_Q3 = "LUT_Progression_Q3";
                    Int_t IsoCut_Progression_Q3 = histosIsolation[Result_Progression_Q3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Q3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q3)
                        {
                            pt_Progression_Q3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Q3 = kTRUE;
                        }

                    TString Result_Progression_R3 = "LUT_Progression_R3";
                    Int_t IsoCut_Progression_R3 = histosIsolation[Result_Progression_R3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_R3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R3)
                        {
                            pt_Progression_R3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_R3 = kTRUE;
                        }

                    TString Result_Progression_S3 = "LUT_Progression_S3";
                    Int_t IsoCut_Progression_S3 = histosIsolation[Result_Progression_S3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_S3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S3)
                        {
                            pt_Progression_S3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_S3 = kTRUE;
                        }

                    TString Result_Progression_T3 = "LUT_Progression_T3";
                    Int_t IsoCut_Progression_T3 = histosIsolation[Result_Progression_T3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_T3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T3)
                        {
                            pt_Progression_T3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_T3 = kTRUE;
                        }

                    TString Result_Progression_U3 = "LUT_Progression_U3";
                    Int_t IsoCut_Progression_U3 = histosIsolation[Result_Progression_U3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_U3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U3)
                        {
                            pt_Progression_U3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_U3 = kTRUE;
                        }

                    TString Result_Progression_V3 = "LUT_Progression_V3";
                    Int_t IsoCut_Progression_V3 = histosIsolation[Result_Progression_V3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_V3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V3)
                        {
                            pt_Progression_V3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_V3 = kTRUE;
                        }

                    TString Result_Progression_W3 = "LUT_Progression_W3";
                    Int_t IsoCut_Progression_W3 = histosIsolation[Result_Progression_W3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_W3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W3)
                        {
                            pt_Progression_W3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_W3 = kTRUE;
                        }

                    TString Result_Progression_X3 = "LUT_Progression_X3";
                    Int_t IsoCut_Progression_X3 = histosIsolation[Result_Progression_X3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_X3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X3)
                        {
                            pt_Progression_X3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_X3 = kTRUE;
                        }

                    TString Result_Progression_Y3 = "LUT_Progression_Y3";
                    Int_t IsoCut_Progression_Y3 = histosIsolation[Result_Progression_Y3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Y3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y3)
                        {
                            pt_Progression_Y3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Y3 = kTRUE;
                        }

                    TString Result_Progression_Z3 = "LUT_Progression_Z3";
                    Int_t IsoCut_Progression_Z3 = histosIsolation[Result_Progression_Z3]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Z3 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z3)
                        {
                            pt_Progression_Z3->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Z3 = kTRUE;
                        }

                    TString Result_Progression_A4 = "LUT_Progression_A4";
                    Int_t IsoCut_Progression_A4 = histosIsolation[Result_Progression_A4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_A4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A4)
                        {
                            pt_Progression_A4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_A4 = kTRUE;
                        }

                    TString Result_Progression_B4 = "LUT_Progression_B4";
                    Int_t IsoCut_Progression_B4 = histosIsolation[Result_Progression_B4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_B4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B4)
                        {
                            pt_Progression_B4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_B4 = kTRUE;
                        }

                    TString Result_Progression_C4 = "LUT_Progression_C4";
                    Int_t IsoCut_Progression_C4 = histosIsolation[Result_Progression_C4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_C4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C4)
                        {
                            pt_Progression_C4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_C4 = kTRUE;
                        }

                    TString Result_Progression_D4 = "LUT_Progression_D4";
                    Int_t IsoCut_Progression_D4 = histosIsolation[Result_Progression_D4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_D4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D4)
                        {
                            pt_Progression_D4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_D4 = kTRUE;
                        }

                    TString Result_Progression_E4 = "LUT_Progression_E4";
                    Int_t IsoCut_Progression_E4 = histosIsolation[Result_Progression_E4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_E4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E4)
                        {
                            pt_Progression_E4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_E4 = kTRUE;
                        }

                    TString Result_Progression_F4 = "LUT_Progression_F4";
                    Int_t IsoCut_Progression_F4 = histosIsolation[Result_Progression_F4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_F4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F4)
                        {
                            pt_Progression_F4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_F4 = kTRUE;
                        }

                    TString Result_Progression_G4 = "LUT_Progression_G4";
                    Int_t IsoCut_Progression_G4 = histosIsolation[Result_Progression_G4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_G4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G4)
                        {
                            pt_Progression_G4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_G4 = kTRUE;
                        }

                    TString Result_Progression_H4 = "LUT_Progression_H4";
                    Int_t IsoCut_Progression_H4 = histosIsolation[Result_Progression_H4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_H4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H4)
                        {
                            pt_Progression_H4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_H4 = kTRUE;
                        }

                    TString Result_Progression_I4 = "LUT_Progression_I4";
                    Int_t IsoCut_Progression_I4 = histosIsolation[Result_Progression_I4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_I4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I4)
                        {
                            pt_Progression_I4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_I4 = kTRUE;
                        }

                    TString Result_Progression_J4 = "LUT_Progression_J4";
                    Int_t IsoCut_Progression_J4 = histosIsolation[Result_Progression_J4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_J4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J4)
                        {
                            pt_Progression_J4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_J4 = kTRUE;
                        }

                    TString Result_Progression_K4 = "LUT_Progression_K4";
                    Int_t IsoCut_Progression_K4 = histosIsolation[Result_Progression_K4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_K4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K4)
                        {
                            pt_Progression_K4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_K4 = kTRUE;
                        }

                    TString Result_Progression_L4 = "LUT_Progression_L4";
                    Int_t IsoCut_Progression_L4 = histosIsolation[Result_Progression_L4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_L4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L4)
                        {
                            pt_Progression_L4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_L4 = kTRUE;
                        }

                    TString Result_Progression_M4 = "LUT_Progression_M4";
                    Int_t IsoCut_Progression_M4 = histosIsolation[Result_Progression_M4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_M4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M4)
                        {
                            pt_Progression_M4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_M4 = kTRUE;
                        }

                    TString Result_Progression_N4 = "LUT_Progression_N4";
                    Int_t IsoCut_Progression_N4 = histosIsolation[Result_Progression_N4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_N4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N4)
                        {
                            pt_Progression_N4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_N4 = kTRUE;
                        }

                    TString Result_Progression_O4 = "LUT_Progression_O4";
                    Int_t IsoCut_Progression_O4 = histosIsolation[Result_Progression_O4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_O4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O4)
                        {
                            pt_Progression_O4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_O4 = kTRUE;
                        }

                    TString Result_Progression_P4 = "LUT_Progression_P4";
                    Int_t IsoCut_Progression_P4 = histosIsolation[Result_Progression_P4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_P4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P4)
                        {
                            pt_Progression_P4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_P4 = kTRUE;
                        }

                    TString Result_Progression_Q4 = "LUT_Progression_Q4";
                    Int_t IsoCut_Progression_Q4 = histosIsolation[Result_Progression_Q4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Q4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q4)
                        {
                            pt_Progression_Q4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Q4 = kTRUE;
                        }

                    TString Result_Progression_R4 = "LUT_Progression_R4";
                    Int_t IsoCut_Progression_R4 = histosIsolation[Result_Progression_R4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_R4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R4)
                        {
                            pt_Progression_R4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_R4 = kTRUE;
                        }

                    TString Result_Progression_S4 = "LUT_Progression_S4";
                    Int_t IsoCut_Progression_S4 = histosIsolation[Result_Progression_S4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_S4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S4)
                        {
                            pt_Progression_S4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_S4 = kTRUE;
                        }

                    TString Result_Progression_T4 = "LUT_Progression_T4";
                    Int_t IsoCut_Progression_T4 = histosIsolation[Result_Progression_T4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_T4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T4)
                        {
                            pt_Progression_T4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_T4 = kTRUE;
                        }

                    TString Result_Progression_U4 = "LUT_Progression_U4";
                    Int_t IsoCut_Progression_U4 = histosIsolation[Result_Progression_U4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_U4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U4)
                        {
                            pt_Progression_U4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_U4 = kTRUE;
                        }

                    TString Result_Progression_V4 = "LUT_Progression_V4";
                    Int_t IsoCut_Progression_V4 = histosIsolation[Result_Progression_V4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_V4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V4)
                        {
                            pt_Progression_V4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_V4 = kTRUE;
                        }

                    TString Result_Progression_W4 = "LUT_Progression_W4";
                    Int_t IsoCut_Progression_W4 = histosIsolation[Result_Progression_W4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_W4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W4)
                        {
                            pt_Progression_W4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_W4 = kTRUE;
                        }

                    TString Result_Progression_X4 = "LUT_Progression_X4";
                    Int_t IsoCut_Progression_X4 = histosIsolation[Result_Progression_X4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_X4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X4)
                        {
                            pt_Progression_X4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_X4 = kTRUE;
                        }

                    TString Result_Progression_Y4 = "LUT_Progression_Y4";
                    Int_t IsoCut_Progression_Y4 = histosIsolation[Result_Progression_Y4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Y4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y4)
                        {
                            pt_Progression_Y4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Y4 = kTRUE;
                        }

                    TString Result_Progression_Z4 = "LUT_Progression_Z4";
                    Int_t IsoCut_Progression_Z4 = histosIsolation[Result_Progression_Z4]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Z4 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z4)
                        {
                            pt_Progression_Z4->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Z4 = kTRUE;
                        }

                    TString Result_Progression_A5 = "LUT_Progression_A5";
                    Int_t IsoCut_Progression_A5 = histosIsolation[Result_Progression_A5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_A5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A5)
                        {
                            pt_Progression_A5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_A5 = kTRUE;
                        }

                    TString Result_Progression_B5 = "LUT_Progression_B5";
                    Int_t IsoCut_Progression_B5 = histosIsolation[Result_Progression_B5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_B5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B5)
                        {
                            pt_Progression_B5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_B5 = kTRUE;
                        }

                    TString Result_Progression_C5 = "LUT_Progression_C5";
                    Int_t IsoCut_Progression_C5 = histosIsolation[Result_Progression_C5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_C5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C5)
                        {
                            pt_Progression_C5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_C5 = kTRUE;
                        }

                    TString Result_Progression_D5 = "LUT_Progression_D5";
                    Int_t IsoCut_Progression_D5 = histosIsolation[Result_Progression_D5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_D5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D5)
                        {
                            pt_Progression_D5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_D5 = kTRUE;
                        }

                    TString Result_Progression_E5 = "LUT_Progression_E5";
                    Int_t IsoCut_Progression_E5 = histosIsolation[Result_Progression_E5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_E5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E5)
                        {
                            pt_Progression_E5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_E5 = kTRUE;
                        }

                    TString Result_Progression_F5 = "LUT_Progression_F5";
                    Int_t IsoCut_Progression_F5 = histosIsolation[Result_Progression_F5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_F5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F5)
                        {
                            pt_Progression_F5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_F5 = kTRUE;
                        }

                    TString Result_Progression_G5 = "LUT_Progression_G5";
                    Int_t IsoCut_Progression_G5 = histosIsolation[Result_Progression_G5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_G5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G5)
                        {
                            pt_Progression_G5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_G5 = kTRUE;
                        }

                    TString Result_Progression_H5 = "LUT_Progression_H5";
                    Int_t IsoCut_Progression_H5 = histosIsolation[Result_Progression_H5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_H5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H5)
                        {
                            pt_Progression_H5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_H5 = kTRUE;
                        }

                    TString Result_Progression_I5 = "LUT_Progression_I5";
                    Int_t IsoCut_Progression_I5 = histosIsolation[Result_Progression_I5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_I5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I5)
                        {
                            pt_Progression_I5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_I5 = kTRUE;
                        }

                    TString Result_Progression_J5 = "LUT_Progression_J5";
                    Int_t IsoCut_Progression_J5 = histosIsolation[Result_Progression_J5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_J5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J5)
                        {
                            pt_Progression_J5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_J5 = kTRUE;
                        }

                    TString Result_Progression_K5 = "LUT_Progression_K5";
                    Int_t IsoCut_Progression_K5 = histosIsolation[Result_Progression_K5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_K5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K5)
                        {
                            pt_Progression_K5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_K5 = kTRUE;
                        }

                    TString Result_Progression_L5 = "LUT_Progression_L5";
                    Int_t IsoCut_Progression_L5 = histosIsolation[Result_Progression_L5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_L5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L5)
                        {
                            pt_Progression_L5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_L5 = kTRUE;
                        }

                    TString Result_Progression_M5 = "LUT_Progression_M5";
                    Int_t IsoCut_Progression_M5 = histosIsolation[Result_Progression_M5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_M5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M5)
                        {
                            pt_Progression_M5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_M5 = kTRUE;
                        }

                    TString Result_Progression_N5 = "LUT_Progression_N5";
                    Int_t IsoCut_Progression_N5 = histosIsolation[Result_Progression_N5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_N5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N5)
                        {
                            pt_Progression_N5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_N5 = kTRUE;
                        }

                    TString Result_Progression_O5 = "LUT_Progression_O5";
                    Int_t IsoCut_Progression_O5 = histosIsolation[Result_Progression_O5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_O5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O5)
                        {
                            pt_Progression_O5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_O5 = kTRUE;
                        }

                    TString Result_Progression_P5 = "LUT_Progression_P5";
                    Int_t IsoCut_Progression_P5 = histosIsolation[Result_Progression_P5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_P5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P5)
                        {
                            pt_Progression_P5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_P5 = kTRUE;
                        }

                    TString Result_Progression_Q5 = "LUT_Progression_Q5";
                    Int_t IsoCut_Progression_Q5 = histosIsolation[Result_Progression_Q5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Q5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q5)
                        {
                            pt_Progression_Q5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Q5 = kTRUE;
                        }

                    TString Result_Progression_R5 = "LUT_Progression_R5";
                    Int_t IsoCut_Progression_R5 = histosIsolation[Result_Progression_R5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_R5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R5)
                        {
                            pt_Progression_R5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_R5 = kTRUE;
                        }

                    TString Result_Progression_S5 = "LUT_Progression_S5";
                    Int_t IsoCut_Progression_S5 = histosIsolation[Result_Progression_S5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_S5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S5)
                        {
                            pt_Progression_S5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_S5 = kTRUE;
                        }

                    TString Result_Progression_T5 = "LUT_Progression_T5";
                    Int_t IsoCut_Progression_T5 = histosIsolation[Result_Progression_T5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_T5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T5)
                        {
                            pt_Progression_T5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_T5 = kTRUE;
                        }

                    TString Result_Progression_U5 = "LUT_Progression_U5";
                    Int_t IsoCut_Progression_U5 = histosIsolation[Result_Progression_U5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_U5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U5)
                        {
                            pt_Progression_U5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_U5 = kTRUE;
                        }

                    TString Result_Progression_V5 = "LUT_Progression_V5";
                    Int_t IsoCut_Progression_V5 = histosIsolation[Result_Progression_V5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_V5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V5)
                        {
                            pt_Progression_V5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_V5 = kTRUE;
                        }

                    TString Result_Progression_W5 = "LUT_Progression_W5";
                    Int_t IsoCut_Progression_W5 = histosIsolation[Result_Progression_W5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_W5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W5)
                        {
                            pt_Progression_W5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_W5 = kTRUE;
                        }

                    TString Result_Progression_X5 = "LUT_Progression_X5";
                    Int_t IsoCut_Progression_X5 = histosIsolation[Result_Progression_X5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_X5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X5)
                        {
                            pt_Progression_X5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_X5 = kTRUE;
                        }

                    TString Result_Progression_Y5 = "LUT_Progression_Y5";
                    Int_t IsoCut_Progression_Y5 = histosIsolation[Result_Progression_Y5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Y5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y5)
                        {
                            pt_Progression_Y5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Y5 = kTRUE;
                        }

                    TString Result_Progression_Z5 = "LUT_Progression_Z5";
                    Int_t IsoCut_Progression_Z5 = histosIsolation[Result_Progression_Z5]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Z5 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z5)
                        {
                            pt_Progression_Z5->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Z5 = kTRUE;
                        }

                    TString Result_Progression_A6 = "LUT_Progression_A6";
                    Int_t IsoCut_Progression_A6 = histosIsolation[Result_Progression_A6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_A6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A6)
                        {
                            pt_Progression_A6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_A6 = kTRUE;
                        }

                    TString Result_Progression_B6 = "LUT_Progression_B6";
                    Int_t IsoCut_Progression_B6 = histosIsolation[Result_Progression_B6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_B6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B6)
                        {
                            pt_Progression_B6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_B6 = kTRUE;
                        }

                    TString Result_Progression_C6 = "LUT_Progression_C6";
                    Int_t IsoCut_Progression_C6 = histosIsolation[Result_Progression_C6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_C6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C6)
                        {
                            pt_Progression_C6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_C6 = kTRUE;
                        }

                    TString Result_Progression_D6 = "LUT_Progression_D6";
                    Int_t IsoCut_Progression_D6 = histosIsolation[Result_Progression_D6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_D6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D6)
                        {
                            pt_Progression_D6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_D6 = kTRUE;
                        }

                    TString Result_Progression_E6 = "LUT_Progression_E6";
                    Int_t IsoCut_Progression_E6 = histosIsolation[Result_Progression_E6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_E6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E6)
                        {
                            pt_Progression_E6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_E6 = kTRUE;
                        }

                    TString Result_Progression_F6 = "LUT_Progression_F6";
                    Int_t IsoCut_Progression_F6 = histosIsolation[Result_Progression_F6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_F6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F6)
                        {
                            pt_Progression_F6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_F6 = kTRUE;
                        }

                    TString Result_Progression_G6 = "LUT_Progression_G6";
                    Int_t IsoCut_Progression_G6 = histosIsolation[Result_Progression_G6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_G6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G6)
                        {
                            pt_Progression_G6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_G6 = kTRUE;
                        }

                    TString Result_Progression_H6 = "LUT_Progression_H6";
                    Int_t IsoCut_Progression_H6 = histosIsolation[Result_Progression_H6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_H6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H6)
                        {
                            pt_Progression_H6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_H6 = kTRUE;
                        }

                    TString Result_Progression_I6 = "LUT_Progression_I6";
                    Int_t IsoCut_Progression_I6 = histosIsolation[Result_Progression_I6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_I6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I6)
                        {
                            pt_Progression_I6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_I6 = kTRUE;
                        }

                    TString Result_Progression_J6 = "LUT_Progression_J6";
                    Int_t IsoCut_Progression_J6 = histosIsolation[Result_Progression_J6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_J6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J6)
                        {
                            pt_Progression_J6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_J6 = kTRUE;
                        }

                    TString Result_Progression_K6 = "LUT_Progression_K6";
                    Int_t IsoCut_Progression_K6 = histosIsolation[Result_Progression_K6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_K6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K6)
                        {
                            pt_Progression_K6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_K6 = kTRUE;
                        }

                    TString Result_Progression_L6 = "LUT_Progression_L6";
                    Int_t IsoCut_Progression_L6 = histosIsolation[Result_Progression_L6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_L6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L6)
                        {
                            pt_Progression_L6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_L6 = kTRUE;
                        }

                    TString Result_Progression_M6 = "LUT_Progression_M6";
                    Int_t IsoCut_Progression_M6 = histosIsolation[Result_Progression_M6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_M6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M6)
                        {
                            pt_Progression_M6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_M6 = kTRUE;
                        }

                    TString Result_Progression_N6 = "LUT_Progression_N6";
                    Int_t IsoCut_Progression_N6 = histosIsolation[Result_Progression_N6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_N6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N6)
                        {
                            pt_Progression_N6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_N6 = kTRUE;
                        }

                    TString Result_Progression_O6 = "LUT_Progression_O6";
                    Int_t IsoCut_Progression_O6 = histosIsolation[Result_Progression_O6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_O6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O6)
                        {
                            pt_Progression_O6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_O6 = kTRUE;
                        }

                    TString Result_Progression_P6 = "LUT_Progression_P6";
                    Int_t IsoCut_Progression_P6 = histosIsolation[Result_Progression_P6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_P6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P6)
                        {
                            pt_Progression_P6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_P6 = kTRUE;
                        }

                    TString Result_Progression_Q6 = "LUT_Progression_Q6";
                    Int_t IsoCut_Progression_Q6 = histosIsolation[Result_Progression_Q6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Q6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q6)
                        {
                            pt_Progression_Q6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Q6 = kTRUE;
                        }

                    TString Result_Progression_R6 = "LUT_Progression_R6";
                    Int_t IsoCut_Progression_R6 = histosIsolation[Result_Progression_R6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_R6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R6)
                        {
                            pt_Progression_R6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_R6 = kTRUE;
                        }

                    TString Result_Progression_S6 = "LUT_Progression_S6";
                    Int_t IsoCut_Progression_S6 = histosIsolation[Result_Progression_S6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_S6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S6)
                        {
                            pt_Progression_S6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_S6 = kTRUE;
                        }

                    TString Result_Progression_T6 = "LUT_Progression_T6";
                    Int_t IsoCut_Progression_T6 = histosIsolation[Result_Progression_T6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_T6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T6)
                        {
                            pt_Progression_T6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_T6 = kTRUE;
                        }

                    TString Result_Progression_U6 = "LUT_Progression_U6";
                    Int_t IsoCut_Progression_U6 = histosIsolation[Result_Progression_U6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_U6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U6)
                        {
                            pt_Progression_U6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_U6 = kTRUE;
                        }

                    TString Result_Progression_V6 = "LUT_Progression_V6";
                    Int_t IsoCut_Progression_V6 = histosIsolation[Result_Progression_V6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_V6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V6)
                        {
                            pt_Progression_V6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_V6 = kTRUE;
                        }

                    TString Result_Progression_W6 = "LUT_Progression_W6";
                    Int_t IsoCut_Progression_W6 = histosIsolation[Result_Progression_W6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_W6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W6)
                        {
                            pt_Progression_W6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_W6 = kTRUE;
                        }

                    TString Result_Progression_X6 = "LUT_Progression_X6";
                    Int_t IsoCut_Progression_X6 = histosIsolation[Result_Progression_X6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_X6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X6)
                        {
                            pt_Progression_X6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_X6 = kTRUE;
                        }

                    TString Result_Progression_Y6 = "LUT_Progression_Y6";
                    Int_t IsoCut_Progression_Y6 = histosIsolation[Result_Progression_Y6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Y6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y6)
                        {
                            pt_Progression_Y6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Y6 = kTRUE;
                        }

                    TString Result_Progression_Z6 = "LUT_Progression_Z6";
                    Int_t IsoCut_Progression_Z6 = histosIsolation[Result_Progression_Z6]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Z6 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z6)
                        {
                            pt_Progression_Z6->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Z6 = kTRUE;
                        }

                    TString Result_Progression_A7 = "LUT_Progression_A7";
                    Int_t IsoCut_Progression_A7 = histosIsolation[Result_Progression_A7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_A7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A7)
                        {
                            pt_Progression_A7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_A7 = kTRUE;
                        }

                    TString Result_Progression_B7 = "LUT_Progression_B7";
                    Int_t IsoCut_Progression_B7 = histosIsolation[Result_Progression_B7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_B7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B7)
                        {
                            pt_Progression_B7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_B7 = kTRUE;
                        }

                    TString Result_Progression_C7 = "LUT_Progression_C7";
                    Int_t IsoCut_Progression_C7 = histosIsolation[Result_Progression_C7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_C7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C7)
                        {
                            pt_Progression_C7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_C7 = kTRUE;
                        }

                    TString Result_Progression_D7 = "LUT_Progression_D7";
                    Int_t IsoCut_Progression_D7 = histosIsolation[Result_Progression_D7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_D7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D7)
                        {
                            pt_Progression_D7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_D7 = kTRUE;
                        }

                    TString Result_Progression_E7 = "LUT_Progression_E7";
                    Int_t IsoCut_Progression_E7 = histosIsolation[Result_Progression_E7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_E7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E7)
                        {
                            pt_Progression_E7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_E7 = kTRUE;
                        }

                    TString Result_Progression_F7 = "LUT_Progression_F7";
                    Int_t IsoCut_Progression_F7 = histosIsolation[Result_Progression_F7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_F7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F7)
                        {
                            pt_Progression_F7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_F7 = kTRUE;
                        }

                    TString Result_Progression_G7 = "LUT_Progression_G7";
                    Int_t IsoCut_Progression_G7 = histosIsolation[Result_Progression_G7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_G7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G7)
                        {
                            pt_Progression_G7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_G7 = kTRUE;
                        }

                    TString Result_Progression_H7 = "LUT_Progression_H7";
                    Int_t IsoCut_Progression_H7 = histosIsolation[Result_Progression_H7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_H7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H7)
                        {
                            pt_Progression_H7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_H7 = kTRUE;
                        }

                    TString Result_Progression_I7 = "LUT_Progression_I7";
                    Int_t IsoCut_Progression_I7 = histosIsolation[Result_Progression_I7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_I7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I7)
                        {
                            pt_Progression_I7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_I7 = kTRUE;
                        }

                    TString Result_Progression_J7 = "LUT_Progression_J7";
                    Int_t IsoCut_Progression_J7 = histosIsolation[Result_Progression_J7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_J7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J7)
                        {
                            pt_Progression_J7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_J7 = kTRUE;
                        }

                    TString Result_Progression_K7 = "LUT_Progression_K7";
                    Int_t IsoCut_Progression_K7 = histosIsolation[Result_Progression_K7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_K7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K7)
                        {
                            pt_Progression_K7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_K7 = kTRUE;
                        }

                    TString Result_Progression_L7 = "LUT_Progression_L7";
                    Int_t IsoCut_Progression_L7 = histosIsolation[Result_Progression_L7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_L7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L7)
                        {
                            pt_Progression_L7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_L7 = kTRUE;
                        }

                    TString Result_Progression_M7 = "LUT_Progression_M7";
                    Int_t IsoCut_Progression_M7 = histosIsolation[Result_Progression_M7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_M7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M7)
                        {
                            pt_Progression_M7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_M7 = kTRUE;
                        }

                    TString Result_Progression_N7 = "LUT_Progression_N7";
                    Int_t IsoCut_Progression_N7 = histosIsolation[Result_Progression_N7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_N7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N7)
                        {
                            pt_Progression_N7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_N7 = kTRUE;
                        }

                    TString Result_Progression_O7 = "LUT_Progression_O7";
                    Int_t IsoCut_Progression_O7 = histosIsolation[Result_Progression_O7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_O7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O7)
                        {
                            pt_Progression_O7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_O7 = kTRUE;
                        }

                    TString Result_Progression_P7 = "LUT_Progression_P7";
                    Int_t IsoCut_Progression_P7 = histosIsolation[Result_Progression_P7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_P7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P7)
                        {
                            pt_Progression_P7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_P7 = kTRUE;
                        }

                    TString Result_Progression_Q7 = "LUT_Progression_Q7";
                    Int_t IsoCut_Progression_Q7 = histosIsolation[Result_Progression_Q7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Q7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q7)
                        {
                            pt_Progression_Q7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Q7 = kTRUE;
                        }

                    TString Result_Progression_R7 = "LUT_Progression_R7";
                    Int_t IsoCut_Progression_R7 = histosIsolation[Result_Progression_R7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_R7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R7)
                        {
                            pt_Progression_R7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_R7 = kTRUE;
                        }

                    TString Result_Progression_S7 = "LUT_Progression_S7";
                    Int_t IsoCut_Progression_S7 = histosIsolation[Result_Progression_S7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_S7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S7)
                        {
                            pt_Progression_S7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_S7 = kTRUE;
                        }

                    TString Result_Progression_T7 = "LUT_Progression_T7";
                    Int_t IsoCut_Progression_T7 = histosIsolation[Result_Progression_T7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_T7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T7)
                        {
                            pt_Progression_T7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_T7 = kTRUE;
                        }

                    TString Result_Progression_U7 = "LUT_Progression_U7";
                    Int_t IsoCut_Progression_U7 = histosIsolation[Result_Progression_U7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_U7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U7)
                        {
                            pt_Progression_U7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_U7 = kTRUE;
                        }

                    TString Result_Progression_V7 = "LUT_Progression_V7";
                    Int_t IsoCut_Progression_V7 = histosIsolation[Result_Progression_V7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_V7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V7)
                        {
                            pt_Progression_V7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_V7 = kTRUE;
                        }

                    TString Result_Progression_W7 = "LUT_Progression_W7";
                    Int_t IsoCut_Progression_W7 = histosIsolation[Result_Progression_W7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_W7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W7)
                        {
                            pt_Progression_W7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_W7 = kTRUE;
                        }

                    TString Result_Progression_X7 = "LUT_Progression_X7";
                    Int_t IsoCut_Progression_X7 = histosIsolation[Result_Progression_X7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_X7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X7)
                        {
                            pt_Progression_X7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_X7 = kTRUE;
                        }

                    TString Result_Progression_Y7 = "LUT_Progression_Y7";
                    Int_t IsoCut_Progression_Y7 = histosIsolation[Result_Progression_Y7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Y7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y7)
                        {
                            pt_Progression_Y7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Y7 = kTRUE;
                        }

                    TString Result_Progression_Z7 = "LUT_Progression_Z7";
                    Int_t IsoCut_Progression_Z7 = histosIsolation[Result_Progression_Z7]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Z7 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z7)
                        {
                            pt_Progression_Z7->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Z7 = kTRUE;
                        }

                    TString Result_Progression_A8 = "LUT_Progression_A8";
                    Int_t IsoCut_Progression_A8 = histosIsolation[Result_Progression_A8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_A8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A8)
                        {
                            pt_Progression_A8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_A8 = kTRUE;
                        }

                    TString Result_Progression_B8 = "LUT_Progression_B8";
                    Int_t IsoCut_Progression_B8 = histosIsolation[Result_Progression_B8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_B8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B8)
                        {
                            pt_Progression_B8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_B8 = kTRUE;
                        }

                    TString Result_Progression_C8 = "LUT_Progression_C8";
                    Int_t IsoCut_Progression_C8 = histosIsolation[Result_Progression_C8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_C8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C8)
                        {
                            pt_Progression_C8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_C8 = kTRUE;
                        }

                    TString Result_Progression_D8 = "LUT_Progression_D8";
                    Int_t IsoCut_Progression_D8 = histosIsolation[Result_Progression_D8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_D8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D8)
                        {
                            pt_Progression_D8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_D8 = kTRUE;
                        }

                    TString Result_Progression_E8 = "LUT_Progression_E8";
                    Int_t IsoCut_Progression_E8 = histosIsolation[Result_Progression_E8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_E8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E8)
                        {
                            pt_Progression_E8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_E8 = kTRUE;
                        }

                    TString Result_Progression_F8 = "LUT_Progression_F8";
                    Int_t IsoCut_Progression_F8 = histosIsolation[Result_Progression_F8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_F8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F8)
                        {
                            pt_Progression_F8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_F8 = kTRUE;
                        }

                    TString Result_Progression_G8 = "LUT_Progression_G8";
                    Int_t IsoCut_Progression_G8 = histosIsolation[Result_Progression_G8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_G8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G8)
                        {
                            pt_Progression_G8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_G8 = kTRUE;
                        }

                    TString Result_Progression_H8 = "LUT_Progression_H8";
                    Int_t IsoCut_Progression_H8 = histosIsolation[Result_Progression_H8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_H8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H8)
                        {
                            pt_Progression_H8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_H8 = kTRUE;
                        }

                    TString Result_Progression_I8 = "LUT_Progression_I8";
                    Int_t IsoCut_Progression_I8 = histosIsolation[Result_Progression_I8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_I8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I8)
                        {
                            pt_Progression_I8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_I8 = kTRUE;
                        }

                    TString Result_Progression_J8 = "LUT_Progression_J8";
                    Int_t IsoCut_Progression_J8 = histosIsolation[Result_Progression_J8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_J8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J8)
                        {
                            pt_Progression_J8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_J8 = kTRUE;
                        }

                    TString Result_Progression_K8 = "LUT_Progression_K8";
                    Int_t IsoCut_Progression_K8 = histosIsolation[Result_Progression_K8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_K8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K8)
                        {
                            pt_Progression_K8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_K8 = kTRUE;
                        }

                    TString Result_Progression_L8 = "LUT_Progression_L8";
                    Int_t IsoCut_Progression_L8 = histosIsolation[Result_Progression_L8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_L8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L8)
                        {
                            pt_Progression_L8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_L8 = kTRUE;
                        }

                    TString Result_Progression_M8 = "LUT_Progression_M8";
                    Int_t IsoCut_Progression_M8 = histosIsolation[Result_Progression_M8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_M8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M8)
                        {
                            pt_Progression_M8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_M8 = kTRUE;
                        }

                    TString Result_Progression_N8 = "LUT_Progression_N8";
                    Int_t IsoCut_Progression_N8 = histosIsolation[Result_Progression_N8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_N8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N8)
                        {
                            pt_Progression_N8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_N8 = kTRUE;
                        }

                    TString Result_Progression_O8 = "LUT_Progression_O8";
                    Int_t IsoCut_Progression_O8 = histosIsolation[Result_Progression_O8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_O8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O8)
                        {
                            pt_Progression_O8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_O8 = kTRUE;
                        }

                    TString Result_Progression_P8 = "LUT_Progression_P8";
                    Int_t IsoCut_Progression_P8 = histosIsolation[Result_Progression_P8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_P8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P8)
                        {
                            pt_Progression_P8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_P8 = kTRUE;
                        }

                    TString Result_Progression_Q8 = "LUT_Progression_Q8";
                    Int_t IsoCut_Progression_Q8 = histosIsolation[Result_Progression_Q8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Q8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q8)
                        {
                            pt_Progression_Q8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Q8 = kTRUE;
                        }

                    TString Result_Progression_R8 = "LUT_Progression_R8";
                    Int_t IsoCut_Progression_R8 = histosIsolation[Result_Progression_R8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_R8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R8)
                        {
                            pt_Progression_R8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_R8 = kTRUE;
                        }

                    TString Result_Progression_S8 = "LUT_Progression_S8";
                    Int_t IsoCut_Progression_S8 = histosIsolation[Result_Progression_S8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_S8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S8)
                        {
                            pt_Progression_S8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_S8 = kTRUE;
                        }

                    TString Result_Progression_T8 = "LUT_Progression_T8";
                    Int_t IsoCut_Progression_T8 = histosIsolation[Result_Progression_T8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_T8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T8)
                        {
                            pt_Progression_T8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_T8 = kTRUE;
                        }

                    TString Result_Progression_U8 = "LUT_Progression_U8";
                    Int_t IsoCut_Progression_U8 = histosIsolation[Result_Progression_U8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_U8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U8)
                        {
                            pt_Progression_U8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_U8 = kTRUE;
                        }

                    TString Result_Progression_V8 = "LUT_Progression_V8";
                    Int_t IsoCut_Progression_V8 = histosIsolation[Result_Progression_V8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_V8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V8)
                        {
                            pt_Progression_V8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_V8 = kTRUE;
                        }

                    TString Result_Progression_W8 = "LUT_Progression_W8";
                    Int_t IsoCut_Progression_W8 = histosIsolation[Result_Progression_W8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_W8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W8)
                        {
                            pt_Progression_W8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_W8 = kTRUE;
                        }

                    TString Result_Progression_X8 = "LUT_Progression_X8";
                    Int_t IsoCut_Progression_X8 = histosIsolation[Result_Progression_X8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_X8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X8)
                        {
                            pt_Progression_X8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_X8 = kTRUE;
                        }

                    TString Result_Progression_Y8 = "LUT_Progression_Y8";
                    Int_t IsoCut_Progression_Y8 = histosIsolation[Result_Progression_Y8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Y8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y8)
                        {
                            pt_Progression_Y8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Y8 = kTRUE;
                        }

                    TString Result_Progression_Z8 = "LUT_Progression_Z8";
                    Int_t IsoCut_Progression_Z8 = histosIsolation[Result_Progression_Z8]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1);
                    if(!Filled_Progression_Z8 && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z8)
                        {
                            pt_Progression_Z8->Fill(in_CalibPt->at(iL1Tau));
                            Filled_Progression_Z8 = kTRUE;
                        }
                    // END OF FULL BLOWN GRID SEARCH

                    //DiTau trigger
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf.at(0))
                        {
                            Index_Taus_IsoInf.at(1)=Index_Taus_IsoInf.at(0);
                            pt_Taus_IsoInf.at(1)=pt_Taus_IsoInf.at(0);
                            Index_Taus_IsoInf.at(0)=iL1Tau;
                            pt_Taus_IsoInf.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_IsoInf.at(1))
                        {
                            Index_Taus_IsoInf.at(1)=iL1Tau;
                            pt_Taus_IsoInf.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_90.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP90"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
                        {
                            Index_Taus_90.at(1)=Index_Taus_90.at(0);
                            pt_Taus_90.at(1)=pt_Taus_90.at(0);
                            Index_Taus_90.at(0)=iL1Tau;
                            pt_Taus_90.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_90.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP90"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
                        {
                            Index_Taus_90.at(1)=iL1Tau;
                            pt_Taus_90.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_80.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP80"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
                        {
                            Index_Taus_80.at(1)=Index_Taus_80.at(0);
                            pt_Taus_80.at(1)=pt_Taus_80.at(0);
                            Index_Taus_80.at(0)=iL1Tau;
                            pt_Taus_80.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_80.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP80"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
                        {
                            Index_Taus_80.at(1)=iL1Tau;
                            pt_Taus_80.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_70.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP70"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
                        {
                            Index_Taus_70.at(1)=Index_Taus_70.at(0);
                            pt_Taus_70.at(1)=pt_Taus_70.at(0);
                            Index_Taus_70.at(0)=iL1Tau;
                            pt_Taus_70.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_70.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP70"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
                        {
                            Index_Taus_70.at(1)=iL1Tau;
                            pt_Taus_70.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_60.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP60"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
                        {
                            Index_Taus_60.at(1)=Index_Taus_60.at(0);
                            pt_Taus_60.at(1)=pt_Taus_60.at(0);
                            Index_Taus_60.at(0)=iL1Tau;
                            pt_Taus_60.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_60.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP60"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
                        {
                            Index_Taus_60.at(1)=iL1Tau;
                            pt_Taus_60.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_50.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP50"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
                        {
                            Index_Taus_50.at(1)=Index_Taus_50.at(0);
                            pt_Taus_50.at(1)=pt_Taus_50.at(0);
                            Index_Taus_50.at(0)=iL1Tau;
                            pt_Taus_50.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_50.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=histosIsolation["LUT_WP50"]->GetBinContent(in_compressedieta->at(iL1Tau)+1,in_compressedE->at(iL1Tau)+1,in_compressednTT->at(iL1Tau)+1))
                        {
                            Index_Taus_50.at(1)=iL1Tau;
                            pt_Taus_50.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_1)
                        {
                            Index_Taus_Progression_1.at(1)=Index_Taus_Progression_1.at(0);
                            pt_Taus_Progression_1.at(1)=pt_Taus_Progression_1.at(0);
                            Index_Taus_Progression_1.at(0)=iL1Tau;
                            pt_Taus_Progression_1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_1)
                        {
                            Index_Taus_Progression_1.at(1)=iL1Tau;
                            pt_Taus_Progression_1.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_2)
                        {
                            Index_Taus_Progression_2.at(1)=Index_Taus_Progression_2.at(0);
                            pt_Taus_Progression_2.at(1)=pt_Taus_Progression_2.at(0);
                            Index_Taus_Progression_2.at(0)=iL1Tau;
                            pt_Taus_Progression_2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_2)
                        {
                            Index_Taus_Progression_2.at(1)=iL1Tau;
                            pt_Taus_Progression_2.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_3)
                        {
                            Index_Taus_Progression_3.at(1)=Index_Taus_Progression_3.at(0);
                            pt_Taus_Progression_3.at(1)=pt_Taus_Progression_3.at(0);
                            Index_Taus_Progression_3.at(0)=iL1Tau;
                            pt_Taus_Progression_3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_3)
                        {
                            Index_Taus_Progression_3.at(1)=iL1Tau;
                            pt_Taus_Progression_3.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_4)
                        {
                            Index_Taus_Progression_4.at(1)=Index_Taus_Progression_4.at(0);
                            pt_Taus_Progression_4.at(1)=pt_Taus_Progression_4.at(0);
                            Index_Taus_Progression_4.at(0)=iL1Tau;
                            pt_Taus_Progression_4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_4)
                        {
                            Index_Taus_Progression_4.at(1)=iL1Tau;
                            pt_Taus_Progression_4.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_5)
                        {
                            Index_Taus_Progression_5.at(1)=Index_Taus_Progression_5.at(0);
                            pt_Taus_Progression_5.at(1)=pt_Taus_Progression_5.at(0);
                            Index_Taus_Progression_5.at(0)=iL1Tau;
                            pt_Taus_Progression_5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_5)
                        {
                            Index_Taus_Progression_5.at(1)=iL1Tau;
                            pt_Taus_Progression_5.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_6)
                        {
                            Index_Taus_Progression_6.at(1)=Index_Taus_Progression_6.at(0);
                            pt_Taus_Progression_6.at(1)=pt_Taus_Progression_6.at(0);
                            Index_Taus_Progression_6.at(0)=iL1Tau;
                            pt_Taus_Progression_6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_6)
                        {
                            Index_Taus_Progression_6.at(1)=iL1Tau;
                            pt_Taus_Progression_6.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_7)
                        {
                            Index_Taus_Progression_7.at(1)=Index_Taus_Progression_7.at(0);
                            pt_Taus_Progression_7.at(1)=pt_Taus_Progression_7.at(0);
                            Index_Taus_Progression_7.at(0)=iL1Tau;
                            pt_Taus_Progression_7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_7)
                        {
                            Index_Taus_Progression_7.at(1)=iL1Tau;
                            pt_Taus_Progression_7.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_8)
                        {
                            Index_Taus_Progression_8.at(1)=Index_Taus_Progression_8.at(0);
                            pt_Taus_Progression_8.at(1)=pt_Taus_Progression_8.at(0);
                            Index_Taus_Progression_8.at(0)=iL1Tau;
                            pt_Taus_Progression_8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_8)
                        {
                            Index_Taus_Progression_8.at(1)=iL1Tau;
                            pt_Taus_Progression_8.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_9.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_9)
                        {
                            Index_Taus_Progression_9.at(1)=Index_Taus_Progression_9.at(0);
                            pt_Taus_Progression_9.at(1)=pt_Taus_Progression_9.at(0);
                            Index_Taus_Progression_9.at(0)=iL1Tau;
                            pt_Taus_Progression_9.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_9.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_9)
                        {
                            Index_Taus_Progression_9.at(1)=iL1Tau;
                            pt_Taus_Progression_9.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_10.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_10)
                        {
                            Index_Taus_Progression_10.at(1)=Index_Taus_Progression_10.at(0);
                            pt_Taus_Progression_10.at(1)=pt_Taus_Progression_10.at(0);
                            Index_Taus_Progression_10.at(0)=iL1Tau;
                            pt_Taus_Progression_10.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_10.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_10)
                        {
                            Index_Taus_Progression_10.at(1)=iL1Tau;
                            pt_Taus_Progression_10.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_11.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_11)
                        {
                            Index_Taus_Progression_11.at(1)=Index_Taus_Progression_11.at(0);
                            pt_Taus_Progression_11.at(1)=pt_Taus_Progression_11.at(0);
                            Index_Taus_Progression_11.at(0)=iL1Tau;
                            pt_Taus_Progression_11.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_11.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_11)
                        {
                            Index_Taus_Progression_11.at(1)=iL1Tau;
                            pt_Taus_Progression_11.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_12.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_12)
                        {
                            Index_Taus_Progression_12.at(1)=Index_Taus_Progression_12.at(0);
                            pt_Taus_Progression_12.at(1)=pt_Taus_Progression_12.at(0);
                            Index_Taus_Progression_12.at(0)=iL1Tau;
                            pt_Taus_Progression_12.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_12.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_12)
                        {
                            Index_Taus_Progression_12.at(1)=iL1Tau;
                            pt_Taus_Progression_12.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_13.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_13)
                        {
                            Index_Taus_Progression_13.at(1)=Index_Taus_Progression_13.at(0);
                            pt_Taus_Progression_13.at(1)=pt_Taus_Progression_13.at(0);
                            Index_Taus_Progression_13.at(0)=iL1Tau;
                            pt_Taus_Progression_13.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_13.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_13)
                        {
                            Index_Taus_Progression_13.at(1)=iL1Tau;
                            pt_Taus_Progression_13.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_14.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_14)
                        {
                            Index_Taus_Progression_14.at(1)=Index_Taus_Progression_14.at(0);
                            pt_Taus_Progression_14.at(1)=pt_Taus_Progression_14.at(0);
                            Index_Taus_Progression_14.at(0)=iL1Tau;
                            pt_Taus_Progression_14.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_14.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_14)
                        {
                            Index_Taus_Progression_14.at(1)=iL1Tau;
                            pt_Taus_Progression_14.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_15.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_15)
                        {
                            Index_Taus_Progression_15.at(1)=Index_Taus_Progression_15.at(0);
                            pt_Taus_Progression_15.at(1)=pt_Taus_Progression_15.at(0);
                            Index_Taus_Progression_15.at(0)=iL1Tau;
                            pt_Taus_Progression_15.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_15.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_15)
                        {
                            Index_Taus_Progression_15.at(1)=iL1Tau;
                            pt_Taus_Progression_15.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_16.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_16)
                        {
                            Index_Taus_Progression_16.at(1)=Index_Taus_Progression_16.at(0);
                            pt_Taus_Progression_16.at(1)=pt_Taus_Progression_16.at(0);
                            Index_Taus_Progression_16.at(0)=iL1Tau;
                            pt_Taus_Progression_16.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_16.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_16)
                        {
                            Index_Taus_Progression_16.at(1)=iL1Tau;
                            pt_Taus_Progression_16.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_17.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_17)
                        {
                            Index_Taus_Progression_17.at(1)=Index_Taus_Progression_17.at(0);
                            pt_Taus_Progression_17.at(1)=pt_Taus_Progression_17.at(0);
                            Index_Taus_Progression_17.at(0)=iL1Tau;
                            pt_Taus_Progression_17.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_17.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_17)
                        {
                            Index_Taus_Progression_17.at(1)=iL1Tau;
                            pt_Taus_Progression_17.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_18.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_18)
                        {
                            Index_Taus_Progression_18.at(1)=Index_Taus_Progression_18.at(0);
                            pt_Taus_Progression_18.at(1)=pt_Taus_Progression_18.at(0);
                            Index_Taus_Progression_18.at(0)=iL1Tau;
                            pt_Taus_Progression_18.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_18.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_18)
                        {
                            Index_Taus_Progression_18.at(1)=iL1Tau;
                            pt_Taus_Progression_18.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_19.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_19)
                        {
                            Index_Taus_Progression_19.at(1)=Index_Taus_Progression_19.at(0);
                            pt_Taus_Progression_19.at(1)=pt_Taus_Progression_19.at(0);
                            Index_Taus_Progression_19.at(0)=iL1Tau;
                            pt_Taus_Progression_19.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_19.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_19)
                        {
                            Index_Taus_Progression_19.at(1)=iL1Tau;
                            pt_Taus_Progression_19.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_20.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_20)
                        {
                            Index_Taus_Progression_20.at(1)=Index_Taus_Progression_20.at(0);
                            pt_Taus_Progression_20.at(1)=pt_Taus_Progression_20.at(0);
                            Index_Taus_Progression_20.at(0)=iL1Tau;
                            pt_Taus_Progression_20.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_20.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_20)
                        {
                            Index_Taus_Progression_20.at(1)=iL1Tau;
                            pt_Taus_Progression_20.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_21.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_21)
                        {
                            Index_Taus_Progression_21.at(1)=Index_Taus_Progression_21.at(0);
                            pt_Taus_Progression_21.at(1)=pt_Taus_Progression_21.at(0);
                            Index_Taus_Progression_21.at(0)=iL1Tau;
                            pt_Taus_Progression_21.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_21.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_21)
                        {
                            Index_Taus_Progression_21.at(1)=iL1Tau;
                            pt_Taus_Progression_21.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22)
                        {
                            Index_Taus_Progression_22.at(1)=Index_Taus_Progression_22.at(0);
                            pt_Taus_Progression_22.at(1)=pt_Taus_Progression_22.at(0);
                            eta_Taus_Progression_22.at(1)=eta_Taus_Progression_22.at(0);
                            isMerged_Taus_Progression_22.at(1)=isMerged_Taus_Progression_22.at(0);
                            Index_Taus_Progression_22.at(0)=iL1Tau;
                            pt_Taus_Progression_22.at(0)=in_CalibPt->at(iL1Tau);
                            eta_Taus_Progression_22.at(0)=in_l1tEmuEta->at(iL1Tau);
                            isMerged_Taus_Progression_22.at(0)=in_l1tEmuIsMerged->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_22.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_22)
                        {
                            Index_Taus_Progression_22.at(1)=iL1Tau;
                            pt_Taus_Progression_22.at(1)=in_CalibPt->at(iL1Tau);
                            eta_Taus_Progression_22.at(1)=in_l1tEmuEta->at(iL1Tau);
                            isMerged_Taus_Progression_22.at(1)=in_l1tEmuIsMerged->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_23.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_23)
                        {
                            Index_Taus_Progression_23.at(1)=Index_Taus_Progression_23.at(0);
                            pt_Taus_Progression_23.at(1)=pt_Taus_Progression_23.at(0);
                            Index_Taus_Progression_23.at(0)=iL1Tau;
                            pt_Taus_Progression_23.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_23.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_23)
                        {
                            Index_Taus_Progression_23.at(1)=iL1Tau;
                            pt_Taus_Progression_23.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_24.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_24)
                        {
                            Index_Taus_Progression_24.at(1)=Index_Taus_Progression_24.at(0);
                            pt_Taus_Progression_24.at(1)=pt_Taus_Progression_24.at(0);
                            Index_Taus_Progression_24.at(0)=iL1Tau;
                            pt_Taus_Progression_24.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_24.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_24)
                        {
                            Index_Taus_Progression_24.at(1)=iL1Tau;
                            pt_Taus_Progression_24.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_25.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_25)
                        {
                            Index_Taus_Progression_25.at(1)=Index_Taus_Progression_25.at(0);
                            pt_Taus_Progression_25.at(1)=pt_Taus_Progression_25.at(0);
                            Index_Taus_Progression_25.at(0)=iL1Tau;
                            pt_Taus_Progression_25.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_25.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_25)
                        {
                            Index_Taus_Progression_25.at(1)=iL1Tau;
                            pt_Taus_Progression_25.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_26.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_26)
                        {
                            Index_Taus_Progression_26.at(1)=Index_Taus_Progression_26.at(0);
                            pt_Taus_Progression_26.at(1)=pt_Taus_Progression_26.at(0);
                            Index_Taus_Progression_26.at(0)=iL1Tau;
                            pt_Taus_Progression_26.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_26.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_26)
                        {
                            Index_Taus_Progression_26.at(1)=iL1Tau;
                            pt_Taus_Progression_26.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_27.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_27)
                        {
                            Index_Taus_Progression_27.at(1)=Index_Taus_Progression_27.at(0);
                            pt_Taus_Progression_27.at(1)=pt_Taus_Progression_27.at(0);
                            Index_Taus_Progression_27.at(0)=iL1Tau;
                            pt_Taus_Progression_27.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_27.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_27)
                        {
                            Index_Taus_Progression_27.at(1)=iL1Tau;
                            pt_Taus_Progression_27.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_28.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_28)
                        {
                            Index_Taus_Progression_28.at(1)=Index_Taus_Progression_28.at(0);
                            pt_Taus_Progression_28.at(1)=pt_Taus_Progression_28.at(0);
                            Index_Taus_Progression_28.at(0)=iL1Tau;
                            pt_Taus_Progression_28.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_28.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_28)
                        {
                            Index_Taus_Progression_28.at(1)=iL1Tau;
                            pt_Taus_Progression_28.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_29.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_29)
                        {
                            Index_Taus_Progression_29.at(1)=Index_Taus_Progression_29.at(0);
                            pt_Taus_Progression_29.at(1)=pt_Taus_Progression_29.at(0);
                            Index_Taus_Progression_29.at(0)=iL1Tau;
                            pt_Taus_Progression_29.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_29.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_29)
                        {
                            Index_Taus_Progression_29.at(1)=iL1Tau;
                            pt_Taus_Progression_29.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_30.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_30)
                        {
                            Index_Taus_Progression_30.at(1)=Index_Taus_Progression_30.at(0);
                            pt_Taus_Progression_30.at(1)=pt_Taus_Progression_30.at(0);
                            Index_Taus_Progression_30.at(0)=iL1Tau;
                            pt_Taus_Progression_30.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_30.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_30)
                        {
                            Index_Taus_Progression_30.at(1)=iL1Tau;
                            pt_Taus_Progression_30.at(1)=in_CalibPt->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_31.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_31)
                        {
                            Index_Taus_Progression_31.at(1)=Index_Taus_Progression_31.at(0);
                            pt_Taus_Progression_31.at(1)=pt_Taus_Progression_31.at(0);
                            eta_Taus_Progression_31.at(1)=eta_Taus_Progression_31.at(0);
                            Index_Taus_Progression_31.at(0)=iL1Tau;
                            pt_Taus_Progression_31.at(0)=in_CalibPt->at(iL1Tau);
                            eta_Taus_Progression_31.at(0)=in_l1tEmuEta->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_31.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_31)
                        {
                            Index_Taus_Progression_31.at(1)=iL1Tau;
                            pt_Taus_Progression_31.at(1)=in_CalibPt->at(iL1Tau);
                            eta_Taus_Progression_31.at(1)=in_l1tEmuEta->at(iL1Tau);
                        }

                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_31_extrap.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_31_extrap)
                        {
                            Index_Taus_Progression_31_extrap.at(1)=Index_Taus_Progression_31_extrap.at(0);
                            pt_Taus_Progression_31_extrap.at(1)=pt_Taus_Progression_31_extrap.at(0);
                            eta_Taus_Progression_31_extrap.at(1)=eta_Taus_Progression_31_extrap.at(0);
                            Index_Taus_Progression_31_extrap.at(0)=iL1Tau;
                            pt_Taus_Progression_31_extrap.at(0)=in_CalibPt->at(iL1Tau);
                            eta_Taus_Progression_31_extrap.at(0)=in_l1tEmuEta->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_31_extrap.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_31_extrap)
                        {
                            Index_Taus_Progression_31_extrap.at(1)=iL1Tau;
                            pt_Taus_Progression_31_extrap.at(1)=in_CalibPt->at(iL1Tau);
                            eta_Taus_Progression_31_extrap.at(1)=in_l1tEmuEta->at(iL1Tau);
                        }

                    // START OF FULL BLOWN GRID SEARCH 
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A0)
                        {
                            Index_Taus_Progression_A0.at(1)=Index_Taus_Progression_A0.at(0);
                            pt_Taus_Progression_A0.at(1)=pt_Taus_Progression_A0.at(0);
                            Index_Taus_Progression_A0.at(0)=iL1Tau;
                            pt_Taus_Progression_A0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A0)
                        {
                            Index_Taus_Progression_A0.at(1)=iL1Tau;
                            pt_Taus_Progression_A0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B0)
                        {
                            Index_Taus_Progression_B0.at(1)=Index_Taus_Progression_B0.at(0);
                            pt_Taus_Progression_B0.at(1)=pt_Taus_Progression_B0.at(0);
                            Index_Taus_Progression_B0.at(0)=iL1Tau;
                            pt_Taus_Progression_B0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B0)
                        {
                            Index_Taus_Progression_B0.at(1)=iL1Tau;
                            pt_Taus_Progression_B0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C0)
                        {
                            Index_Taus_Progression_C0.at(1)=Index_Taus_Progression_C0.at(0);
                            pt_Taus_Progression_C0.at(1)=pt_Taus_Progression_C0.at(0);
                            Index_Taus_Progression_C0.at(0)=iL1Tau;
                            pt_Taus_Progression_C0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C0)
                        {
                            Index_Taus_Progression_C0.at(1)=iL1Tau;
                            pt_Taus_Progression_C0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D0)
                        {
                            Index_Taus_Progression_D0.at(1)=Index_Taus_Progression_D0.at(0);
                            pt_Taus_Progression_D0.at(1)=pt_Taus_Progression_D0.at(0);
                            Index_Taus_Progression_D0.at(0)=iL1Tau;
                            pt_Taus_Progression_D0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D0)
                        {
                            Index_Taus_Progression_D0.at(1)=iL1Tau;
                            pt_Taus_Progression_D0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E0)
                        {
                            Index_Taus_Progression_E0.at(1)=Index_Taus_Progression_E0.at(0);
                            pt_Taus_Progression_E0.at(1)=pt_Taus_Progression_E0.at(0);
                            Index_Taus_Progression_E0.at(0)=iL1Tau;
                            pt_Taus_Progression_E0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E0)
                        {
                            Index_Taus_Progression_E0.at(1)=iL1Tau;
                            pt_Taus_Progression_E0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F0)
                        {
                            Index_Taus_Progression_F0.at(1)=Index_Taus_Progression_F0.at(0);
                            pt_Taus_Progression_F0.at(1)=pt_Taus_Progression_F0.at(0);
                            Index_Taus_Progression_F0.at(0)=iL1Tau;
                            pt_Taus_Progression_F0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F0)
                        {
                            Index_Taus_Progression_F0.at(1)=iL1Tau;
                            pt_Taus_Progression_F0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G0)
                        {
                            Index_Taus_Progression_G0.at(1)=Index_Taus_Progression_G0.at(0);
                            pt_Taus_Progression_G0.at(1)=pt_Taus_Progression_G0.at(0);
                            Index_Taus_Progression_G0.at(0)=iL1Tau;
                            pt_Taus_Progression_G0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G0)
                        {
                            Index_Taus_Progression_G0.at(1)=iL1Tau;
                            pt_Taus_Progression_G0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H0)
                        {
                            Index_Taus_Progression_H0.at(1)=Index_Taus_Progression_H0.at(0);
                            pt_Taus_Progression_H0.at(1)=pt_Taus_Progression_H0.at(0);
                            Index_Taus_Progression_H0.at(0)=iL1Tau;
                            pt_Taus_Progression_H0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H0)
                        {
                            Index_Taus_Progression_H0.at(1)=iL1Tau;
                            pt_Taus_Progression_H0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I0)
                        {
                            Index_Taus_Progression_I0.at(1)=Index_Taus_Progression_I0.at(0);
                            pt_Taus_Progression_I0.at(1)=pt_Taus_Progression_I0.at(0);
                            Index_Taus_Progression_I0.at(0)=iL1Tau;
                            pt_Taus_Progression_I0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I0)
                        {
                            Index_Taus_Progression_I0.at(1)=iL1Tau;
                            pt_Taus_Progression_I0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J0)
                        {
                            Index_Taus_Progression_J0.at(1)=Index_Taus_Progression_J0.at(0);
                            pt_Taus_Progression_J0.at(1)=pt_Taus_Progression_J0.at(0);
                            Index_Taus_Progression_J0.at(0)=iL1Tau;
                            pt_Taus_Progression_J0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J0)
                        {
                            Index_Taus_Progression_J0.at(1)=iL1Tau;
                            pt_Taus_Progression_J0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K0)
                        {
                            Index_Taus_Progression_K0.at(1)=Index_Taus_Progression_K0.at(0);
                            pt_Taus_Progression_K0.at(1)=pt_Taus_Progression_K0.at(0);
                            Index_Taus_Progression_K0.at(0)=iL1Tau;
                            pt_Taus_Progression_K0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K0)
                        {
                            Index_Taus_Progression_K0.at(1)=iL1Tau;
                            pt_Taus_Progression_K0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L0)
                        {
                            Index_Taus_Progression_L0.at(1)=Index_Taus_Progression_L0.at(0);
                            pt_Taus_Progression_L0.at(1)=pt_Taus_Progression_L0.at(0);
                            Index_Taus_Progression_L0.at(0)=iL1Tau;
                            pt_Taus_Progression_L0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L0)
                        {
                            Index_Taus_Progression_L0.at(1)=iL1Tau;
                            pt_Taus_Progression_L0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M0)
                        {
                            Index_Taus_Progression_M0.at(1)=Index_Taus_Progression_M0.at(0);
                            pt_Taus_Progression_M0.at(1)=pt_Taus_Progression_M0.at(0);
                            Index_Taus_Progression_M0.at(0)=iL1Tau;
                            pt_Taus_Progression_M0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M0)
                        {
                            Index_Taus_Progression_M0.at(1)=iL1Tau;
                            pt_Taus_Progression_M0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N0)
                        {
                            Index_Taus_Progression_N0.at(1)=Index_Taus_Progression_N0.at(0);
                            pt_Taus_Progression_N0.at(1)=pt_Taus_Progression_N0.at(0);
                            Index_Taus_Progression_N0.at(0)=iL1Tau;
                            pt_Taus_Progression_N0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N0)
                        {
                            Index_Taus_Progression_N0.at(1)=iL1Tau;
                            pt_Taus_Progression_N0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O0)
                        {
                            Index_Taus_Progression_O0.at(1)=Index_Taus_Progression_O0.at(0);
                            pt_Taus_Progression_O0.at(1)=pt_Taus_Progression_O0.at(0);
                            Index_Taus_Progression_O0.at(0)=iL1Tau;
                            pt_Taus_Progression_O0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O0)
                        {
                            Index_Taus_Progression_O0.at(1)=iL1Tau;
                            pt_Taus_Progression_O0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P0)
                        {
                            Index_Taus_Progression_P0.at(1)=Index_Taus_Progression_P0.at(0);
                            pt_Taus_Progression_P0.at(1)=pt_Taus_Progression_P0.at(0);
                            Index_Taus_Progression_P0.at(0)=iL1Tau;
                            pt_Taus_Progression_P0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P0)
                        {
                            Index_Taus_Progression_P0.at(1)=iL1Tau;
                            pt_Taus_Progression_P0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q0)
                        {
                            Index_Taus_Progression_Q0.at(1)=Index_Taus_Progression_Q0.at(0);
                            pt_Taus_Progression_Q0.at(1)=pt_Taus_Progression_Q0.at(0);
                            Index_Taus_Progression_Q0.at(0)=iL1Tau;
                            pt_Taus_Progression_Q0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q0)
                        {
                            Index_Taus_Progression_Q0.at(1)=iL1Tau;
                            pt_Taus_Progression_Q0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R0)
                        {
                            Index_Taus_Progression_R0.at(1)=Index_Taus_Progression_R0.at(0);
                            pt_Taus_Progression_R0.at(1)=pt_Taus_Progression_R0.at(0);
                            Index_Taus_Progression_R0.at(0)=iL1Tau;
                            pt_Taus_Progression_R0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R0)
                        {
                            Index_Taus_Progression_R0.at(1)=iL1Tau;
                            pt_Taus_Progression_R0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S0)
                        {
                            Index_Taus_Progression_S0.at(1)=Index_Taus_Progression_S0.at(0);
                            pt_Taus_Progression_S0.at(1)=pt_Taus_Progression_S0.at(0);
                            Index_Taus_Progression_S0.at(0)=iL1Tau;
                            pt_Taus_Progression_S0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S0)
                        {
                            Index_Taus_Progression_S0.at(1)=iL1Tau;
                            pt_Taus_Progression_S0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T0)
                        {
                            Index_Taus_Progression_T0.at(1)=Index_Taus_Progression_T0.at(0);
                            pt_Taus_Progression_T0.at(1)=pt_Taus_Progression_T0.at(0);
                            Index_Taus_Progression_T0.at(0)=iL1Tau;
                            pt_Taus_Progression_T0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T0)
                        {
                            Index_Taus_Progression_T0.at(1)=iL1Tau;
                            pt_Taus_Progression_T0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U0)
                        {
                            Index_Taus_Progression_U0.at(1)=Index_Taus_Progression_U0.at(0);
                            pt_Taus_Progression_U0.at(1)=pt_Taus_Progression_U0.at(0);
                            Index_Taus_Progression_U0.at(0)=iL1Tau;
                            pt_Taus_Progression_U0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U0)
                        {
                            Index_Taus_Progression_U0.at(1)=iL1Tau;
                            pt_Taus_Progression_U0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V0)
                        {
                            Index_Taus_Progression_V0.at(1)=Index_Taus_Progression_V0.at(0);
                            pt_Taus_Progression_V0.at(1)=pt_Taus_Progression_V0.at(0);
                            Index_Taus_Progression_V0.at(0)=iL1Tau;
                            pt_Taus_Progression_V0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V0)
                        {
                            Index_Taus_Progression_V0.at(1)=iL1Tau;
                            pt_Taus_Progression_V0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W0)
                        {
                            Index_Taus_Progression_W0.at(1)=Index_Taus_Progression_W0.at(0);
                            pt_Taus_Progression_W0.at(1)=pt_Taus_Progression_W0.at(0);
                            Index_Taus_Progression_W0.at(0)=iL1Tau;
                            pt_Taus_Progression_W0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W0)
                        {
                            Index_Taus_Progression_W0.at(1)=iL1Tau;
                            pt_Taus_Progression_W0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X0)
                        {
                            Index_Taus_Progression_X0.at(1)=Index_Taus_Progression_X0.at(0);
                            pt_Taus_Progression_X0.at(1)=pt_Taus_Progression_X0.at(0);
                            Index_Taus_Progression_X0.at(0)=iL1Tau;
                            pt_Taus_Progression_X0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X0)
                        {
                            Index_Taus_Progression_X0.at(1)=iL1Tau;
                            pt_Taus_Progression_X0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y0)
                        {
                            Index_Taus_Progression_Y0.at(1)=Index_Taus_Progression_Y0.at(0);
                            pt_Taus_Progression_Y0.at(1)=pt_Taus_Progression_Y0.at(0);
                            Index_Taus_Progression_Y0.at(0)=iL1Tau;
                            pt_Taus_Progression_Y0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y0)
                        {
                            Index_Taus_Progression_Y0.at(1)=iL1Tau;
                            pt_Taus_Progression_Y0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z0.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z0)
                        {
                            Index_Taus_Progression_Z0.at(1)=Index_Taus_Progression_Z0.at(0);
                            pt_Taus_Progression_Z0.at(1)=pt_Taus_Progression_Z0.at(0);
                            Index_Taus_Progression_Z0.at(0)=iL1Tau;
                            pt_Taus_Progression_Z0.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z0.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z0)
                        {
                            Index_Taus_Progression_Z0.at(1)=iL1Tau;
                            pt_Taus_Progression_Z0.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A1)
                        {
                            Index_Taus_Progression_A1.at(1)=Index_Taus_Progression_A1.at(0);
                            pt_Taus_Progression_A1.at(1)=pt_Taus_Progression_A1.at(0);
                            Index_Taus_Progression_A1.at(0)=iL1Tau;
                            pt_Taus_Progression_A1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A1)
                        {
                            Index_Taus_Progression_A1.at(1)=iL1Tau;
                            pt_Taus_Progression_A1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B1)
                        {
                            Index_Taus_Progression_B1.at(1)=Index_Taus_Progression_B1.at(0);
                            pt_Taus_Progression_B1.at(1)=pt_Taus_Progression_B1.at(0);
                            Index_Taus_Progression_B1.at(0)=iL1Tau;
                            pt_Taus_Progression_B1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B1)
                        {
                            Index_Taus_Progression_B1.at(1)=iL1Tau;
                            pt_Taus_Progression_B1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C1)
                        {
                            Index_Taus_Progression_C1.at(1)=Index_Taus_Progression_C1.at(0);
                            pt_Taus_Progression_C1.at(1)=pt_Taus_Progression_C1.at(0);
                            Index_Taus_Progression_C1.at(0)=iL1Tau;
                            pt_Taus_Progression_C1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C1)
                        {
                            Index_Taus_Progression_C1.at(1)=iL1Tau;
                            pt_Taus_Progression_C1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D1)
                        {
                            Index_Taus_Progression_D1.at(1)=Index_Taus_Progression_D1.at(0);
                            pt_Taus_Progression_D1.at(1)=pt_Taus_Progression_D1.at(0);
                            Index_Taus_Progression_D1.at(0)=iL1Tau;
                            pt_Taus_Progression_D1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D1)
                        {
                            Index_Taus_Progression_D1.at(1)=iL1Tau;
                            pt_Taus_Progression_D1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E1)
                        {
                            Index_Taus_Progression_E1.at(1)=Index_Taus_Progression_E1.at(0);
                            pt_Taus_Progression_E1.at(1)=pt_Taus_Progression_E1.at(0);
                            Index_Taus_Progression_E1.at(0)=iL1Tau;
                            pt_Taus_Progression_E1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E1)
                        {
                            Index_Taus_Progression_E1.at(1)=iL1Tau;
                            pt_Taus_Progression_E1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F1)
                        {
                            Index_Taus_Progression_F1.at(1)=Index_Taus_Progression_F1.at(0);
                            pt_Taus_Progression_F1.at(1)=pt_Taus_Progression_F1.at(0);
                            Index_Taus_Progression_F1.at(0)=iL1Tau;
                            pt_Taus_Progression_F1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F1)
                        {
                            Index_Taus_Progression_F1.at(1)=iL1Tau;
                            pt_Taus_Progression_F1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G1)
                        {
                            Index_Taus_Progression_G1.at(1)=Index_Taus_Progression_G1.at(0);
                            pt_Taus_Progression_G1.at(1)=pt_Taus_Progression_G1.at(0);
                            Index_Taus_Progression_G1.at(0)=iL1Tau;
                            pt_Taus_Progression_G1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G1)
                        {
                            Index_Taus_Progression_G1.at(1)=iL1Tau;
                            pt_Taus_Progression_G1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H1)
                        {
                            Index_Taus_Progression_H1.at(1)=Index_Taus_Progression_H1.at(0);
                            pt_Taus_Progression_H1.at(1)=pt_Taus_Progression_H1.at(0);
                            Index_Taus_Progression_H1.at(0)=iL1Tau;
                            pt_Taus_Progression_H1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H1)
                        {
                            Index_Taus_Progression_H1.at(1)=iL1Tau;
                            pt_Taus_Progression_H1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I1)
                        {
                            Index_Taus_Progression_I1.at(1)=Index_Taus_Progression_I1.at(0);
                            pt_Taus_Progression_I1.at(1)=pt_Taus_Progression_I1.at(0);
                            Index_Taus_Progression_I1.at(0)=iL1Tau;
                            pt_Taus_Progression_I1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I1)
                        {
                            Index_Taus_Progression_I1.at(1)=iL1Tau;
                            pt_Taus_Progression_I1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J1)
                        {
                            Index_Taus_Progression_J1.at(1)=Index_Taus_Progression_J1.at(0);
                            pt_Taus_Progression_J1.at(1)=pt_Taus_Progression_J1.at(0);
                            Index_Taus_Progression_J1.at(0)=iL1Tau;
                            pt_Taus_Progression_J1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J1)
                        {
                            Index_Taus_Progression_J1.at(1)=iL1Tau;
                            pt_Taus_Progression_J1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K1)
                        {
                            Index_Taus_Progression_K1.at(1)=Index_Taus_Progression_K1.at(0);
                            pt_Taus_Progression_K1.at(1)=pt_Taus_Progression_K1.at(0);
                            Index_Taus_Progression_K1.at(0)=iL1Tau;
                            pt_Taus_Progression_K1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K1)
                        {
                            Index_Taus_Progression_K1.at(1)=iL1Tau;
                            pt_Taus_Progression_K1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L1)
                        {
                            Index_Taus_Progression_L1.at(1)=Index_Taus_Progression_L1.at(0);
                            pt_Taus_Progression_L1.at(1)=pt_Taus_Progression_L1.at(0);
                            Index_Taus_Progression_L1.at(0)=iL1Tau;
                            pt_Taus_Progression_L1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L1)
                        {
                            Index_Taus_Progression_L1.at(1)=iL1Tau;
                            pt_Taus_Progression_L1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M1)
                        {
                            Index_Taus_Progression_M1.at(1)=Index_Taus_Progression_M1.at(0);
                            pt_Taus_Progression_M1.at(1)=pt_Taus_Progression_M1.at(0);
                            Index_Taus_Progression_M1.at(0)=iL1Tau;
                            pt_Taus_Progression_M1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M1)
                        {
                            Index_Taus_Progression_M1.at(1)=iL1Tau;
                            pt_Taus_Progression_M1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N1)
                        {
                            Index_Taus_Progression_N1.at(1)=Index_Taus_Progression_N1.at(0);
                            pt_Taus_Progression_N1.at(1)=pt_Taus_Progression_N1.at(0);
                            Index_Taus_Progression_N1.at(0)=iL1Tau;
                            pt_Taus_Progression_N1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N1)
                        {
                            Index_Taus_Progression_N1.at(1)=iL1Tau;
                            pt_Taus_Progression_N1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O1)
                        {
                            Index_Taus_Progression_O1.at(1)=Index_Taus_Progression_O1.at(0);
                            pt_Taus_Progression_O1.at(1)=pt_Taus_Progression_O1.at(0);
                            Index_Taus_Progression_O1.at(0)=iL1Tau;
                            pt_Taus_Progression_O1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O1)
                        {
                            Index_Taus_Progression_O1.at(1)=iL1Tau;
                            pt_Taus_Progression_O1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P1)
                        {
                            Index_Taus_Progression_P1.at(1)=Index_Taus_Progression_P1.at(0);
                            pt_Taus_Progression_P1.at(1)=pt_Taus_Progression_P1.at(0);
                            Index_Taus_Progression_P1.at(0)=iL1Tau;
                            pt_Taus_Progression_P1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P1)
                        {
                            Index_Taus_Progression_P1.at(1)=iL1Tau;
                            pt_Taus_Progression_P1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q1)
                        {
                            Index_Taus_Progression_Q1.at(1)=Index_Taus_Progression_Q1.at(0);
                            pt_Taus_Progression_Q1.at(1)=pt_Taus_Progression_Q1.at(0);
                            Index_Taus_Progression_Q1.at(0)=iL1Tau;
                            pt_Taus_Progression_Q1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q1)
                        {
                            Index_Taus_Progression_Q1.at(1)=iL1Tau;
                            pt_Taus_Progression_Q1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R1)
                        {
                            Index_Taus_Progression_R1.at(1)=Index_Taus_Progression_R1.at(0);
                            pt_Taus_Progression_R1.at(1)=pt_Taus_Progression_R1.at(0);
                            Index_Taus_Progression_R1.at(0)=iL1Tau;
                            pt_Taus_Progression_R1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R1)
                        {
                            Index_Taus_Progression_R1.at(1)=iL1Tau;
                            pt_Taus_Progression_R1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S1)
                        {
                            Index_Taus_Progression_S1.at(1)=Index_Taus_Progression_S1.at(0);
                            pt_Taus_Progression_S1.at(1)=pt_Taus_Progression_S1.at(0);
                            Index_Taus_Progression_S1.at(0)=iL1Tau;
                            pt_Taus_Progression_S1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S1)
                        {
                            Index_Taus_Progression_S1.at(1)=iL1Tau;
                            pt_Taus_Progression_S1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T1)
                        {
                            Index_Taus_Progression_T1.at(1)=Index_Taus_Progression_T1.at(0);
                            pt_Taus_Progression_T1.at(1)=pt_Taus_Progression_T1.at(0);
                            Index_Taus_Progression_T1.at(0)=iL1Tau;
                            pt_Taus_Progression_T1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T1)
                        {
                            Index_Taus_Progression_T1.at(1)=iL1Tau;
                            pt_Taus_Progression_T1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U1)
                        {
                            Index_Taus_Progression_U1.at(1)=Index_Taus_Progression_U1.at(0);
                            pt_Taus_Progression_U1.at(1)=pt_Taus_Progression_U1.at(0);
                            Index_Taus_Progression_U1.at(0)=iL1Tau;
                            pt_Taus_Progression_U1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U1)
                        {
                            Index_Taus_Progression_U1.at(1)=iL1Tau;
                            pt_Taus_Progression_U1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V1)
                        {
                            Index_Taus_Progression_V1.at(1)=Index_Taus_Progression_V1.at(0);
                            pt_Taus_Progression_V1.at(1)=pt_Taus_Progression_V1.at(0);
                            Index_Taus_Progression_V1.at(0)=iL1Tau;
                            pt_Taus_Progression_V1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V1)
                        {
                            Index_Taus_Progression_V1.at(1)=iL1Tau;
                            pt_Taus_Progression_V1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W1)
                        {
                            Index_Taus_Progression_W1.at(1)=Index_Taus_Progression_W1.at(0);
                            pt_Taus_Progression_W1.at(1)=pt_Taus_Progression_W1.at(0);
                            Index_Taus_Progression_W1.at(0)=iL1Tau;
                            pt_Taus_Progression_W1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W1)
                        {
                            Index_Taus_Progression_W1.at(1)=iL1Tau;
                            pt_Taus_Progression_W1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X1)
                        {
                            Index_Taus_Progression_X1.at(1)=Index_Taus_Progression_X1.at(0);
                            pt_Taus_Progression_X1.at(1)=pt_Taus_Progression_X1.at(0);
                            Index_Taus_Progression_X1.at(0)=iL1Tau;
                            pt_Taus_Progression_X1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X1)
                        {
                            Index_Taus_Progression_X1.at(1)=iL1Tau;
                            pt_Taus_Progression_X1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y1)
                        {
                            Index_Taus_Progression_Y1.at(1)=Index_Taus_Progression_Y1.at(0);
                            pt_Taus_Progression_Y1.at(1)=pt_Taus_Progression_Y1.at(0);
                            Index_Taus_Progression_Y1.at(0)=iL1Tau;
                            pt_Taus_Progression_Y1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y1)
                        {
                            Index_Taus_Progression_Y1.at(1)=iL1Tau;
                            pt_Taus_Progression_Y1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z1.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z1)
                        {
                            Index_Taus_Progression_Z1.at(1)=Index_Taus_Progression_Z1.at(0);
                            pt_Taus_Progression_Z1.at(1)=pt_Taus_Progression_Z1.at(0);
                            Index_Taus_Progression_Z1.at(0)=iL1Tau;
                            pt_Taus_Progression_Z1.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z1.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z1)
                        {
                            Index_Taus_Progression_Z1.at(1)=iL1Tau;
                            pt_Taus_Progression_Z1.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A2)
                        {
                            Index_Taus_Progression_A2.at(1)=Index_Taus_Progression_A2.at(0);
                            pt_Taus_Progression_A2.at(1)=pt_Taus_Progression_A2.at(0);
                            Index_Taus_Progression_A2.at(0)=iL1Tau;
                            pt_Taus_Progression_A2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A2)
                        {
                            Index_Taus_Progression_A2.at(1)=iL1Tau;
                            pt_Taus_Progression_A2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B2)
                        {
                            Index_Taus_Progression_B2.at(1)=Index_Taus_Progression_B2.at(0);
                            pt_Taus_Progression_B2.at(1)=pt_Taus_Progression_B2.at(0);
                            Index_Taus_Progression_B2.at(0)=iL1Tau;
                            pt_Taus_Progression_B2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B2)
                        {
                            Index_Taus_Progression_B2.at(1)=iL1Tau;
                            pt_Taus_Progression_B2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C2)
                        {
                            Index_Taus_Progression_C2.at(1)=Index_Taus_Progression_C2.at(0);
                            pt_Taus_Progression_C2.at(1)=pt_Taus_Progression_C2.at(0);
                            Index_Taus_Progression_C2.at(0)=iL1Tau;
                            pt_Taus_Progression_C2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C2)
                        {
                            Index_Taus_Progression_C2.at(1)=iL1Tau;
                            pt_Taus_Progression_C2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D2)
                        {
                            Index_Taus_Progression_D2.at(1)=Index_Taus_Progression_D2.at(0);
                            pt_Taus_Progression_D2.at(1)=pt_Taus_Progression_D2.at(0);
                            Index_Taus_Progression_D2.at(0)=iL1Tau;
                            pt_Taus_Progression_D2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D2)
                        {
                            Index_Taus_Progression_D2.at(1)=iL1Tau;
                            pt_Taus_Progression_D2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E2)
                        {
                            Index_Taus_Progression_E2.at(1)=Index_Taus_Progression_E2.at(0);
                            pt_Taus_Progression_E2.at(1)=pt_Taus_Progression_E2.at(0);
                            Index_Taus_Progression_E2.at(0)=iL1Tau;
                            pt_Taus_Progression_E2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E2)
                        {
                            Index_Taus_Progression_E2.at(1)=iL1Tau;
                            pt_Taus_Progression_E2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F2)
                        {
                            Index_Taus_Progression_F2.at(1)=Index_Taus_Progression_F2.at(0);
                            pt_Taus_Progression_F2.at(1)=pt_Taus_Progression_F2.at(0);
                            Index_Taus_Progression_F2.at(0)=iL1Tau;
                            pt_Taus_Progression_F2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F2)
                        {
                            Index_Taus_Progression_F2.at(1)=iL1Tau;
                            pt_Taus_Progression_F2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G2)
                        {
                            Index_Taus_Progression_G2.at(1)=Index_Taus_Progression_G2.at(0);
                            pt_Taus_Progression_G2.at(1)=pt_Taus_Progression_G2.at(0);
                            Index_Taus_Progression_G2.at(0)=iL1Tau;
                            pt_Taus_Progression_G2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G2)
                        {
                            Index_Taus_Progression_G2.at(1)=iL1Tau;
                            pt_Taus_Progression_G2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H2)
                        {
                            Index_Taus_Progression_H2.at(1)=Index_Taus_Progression_H2.at(0);
                            pt_Taus_Progression_H2.at(1)=pt_Taus_Progression_H2.at(0);
                            Index_Taus_Progression_H2.at(0)=iL1Tau;
                            pt_Taus_Progression_H2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H2)
                        {
                            Index_Taus_Progression_H2.at(1)=iL1Tau;
                            pt_Taus_Progression_H2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I2)
                        {
                            Index_Taus_Progression_I2.at(1)=Index_Taus_Progression_I2.at(0);
                            pt_Taus_Progression_I2.at(1)=pt_Taus_Progression_I2.at(0);
                            Index_Taus_Progression_I2.at(0)=iL1Tau;
                            pt_Taus_Progression_I2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I2)
                        {
                            Index_Taus_Progression_I2.at(1)=iL1Tau;
                            pt_Taus_Progression_I2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J2)
                        {
                            Index_Taus_Progression_J2.at(1)=Index_Taus_Progression_J2.at(0);
                            pt_Taus_Progression_J2.at(1)=pt_Taus_Progression_J2.at(0);
                            Index_Taus_Progression_J2.at(0)=iL1Tau;
                            pt_Taus_Progression_J2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J2)
                        {
                            Index_Taus_Progression_J2.at(1)=iL1Tau;
                            pt_Taus_Progression_J2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K2)
                        {
                            Index_Taus_Progression_K2.at(1)=Index_Taus_Progression_K2.at(0);
                            pt_Taus_Progression_K2.at(1)=pt_Taus_Progression_K2.at(0);
                            Index_Taus_Progression_K2.at(0)=iL1Tau;
                            pt_Taus_Progression_K2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K2)
                        {
                            Index_Taus_Progression_K2.at(1)=iL1Tau;
                            pt_Taus_Progression_K2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L2)
                        {
                            Index_Taus_Progression_L2.at(1)=Index_Taus_Progression_L2.at(0);
                            pt_Taus_Progression_L2.at(1)=pt_Taus_Progression_L2.at(0);
                            Index_Taus_Progression_L2.at(0)=iL1Tau;
                            pt_Taus_Progression_L2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L2)
                        {
                            Index_Taus_Progression_L2.at(1)=iL1Tau;
                            pt_Taus_Progression_L2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M2)
                        {
                            Index_Taus_Progression_M2.at(1)=Index_Taus_Progression_M2.at(0);
                            pt_Taus_Progression_M2.at(1)=pt_Taus_Progression_M2.at(0);
                            Index_Taus_Progression_M2.at(0)=iL1Tau;
                            pt_Taus_Progression_M2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M2)
                        {
                            Index_Taus_Progression_M2.at(1)=iL1Tau;
                            pt_Taus_Progression_M2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N2)
                        {
                            Index_Taus_Progression_N2.at(1)=Index_Taus_Progression_N2.at(0);
                            pt_Taus_Progression_N2.at(1)=pt_Taus_Progression_N2.at(0);
                            Index_Taus_Progression_N2.at(0)=iL1Tau;
                            pt_Taus_Progression_N2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N2)
                        {
                            Index_Taus_Progression_N2.at(1)=iL1Tau;
                            pt_Taus_Progression_N2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O2)
                        {
                            Index_Taus_Progression_O2.at(1)=Index_Taus_Progression_O2.at(0);
                            pt_Taus_Progression_O2.at(1)=pt_Taus_Progression_O2.at(0);
                            Index_Taus_Progression_O2.at(0)=iL1Tau;
                            pt_Taus_Progression_O2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O2)
                        {
                            Index_Taus_Progression_O2.at(1)=iL1Tau;
                            pt_Taus_Progression_O2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P2)
                        {
                            Index_Taus_Progression_P2.at(1)=Index_Taus_Progression_P2.at(0);
                            pt_Taus_Progression_P2.at(1)=pt_Taus_Progression_P2.at(0);
                            Index_Taus_Progression_P2.at(0)=iL1Tau;
                            pt_Taus_Progression_P2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P2)
                        {
                            Index_Taus_Progression_P2.at(1)=iL1Tau;
                            pt_Taus_Progression_P2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q2)
                        {
                            Index_Taus_Progression_Q2.at(1)=Index_Taus_Progression_Q2.at(0);
                            pt_Taus_Progression_Q2.at(1)=pt_Taus_Progression_Q2.at(0);
                            Index_Taus_Progression_Q2.at(0)=iL1Tau;
                            pt_Taus_Progression_Q2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q2)
                        {
                            Index_Taus_Progression_Q2.at(1)=iL1Tau;
                            pt_Taus_Progression_Q2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R2)
                        {
                            Index_Taus_Progression_R2.at(1)=Index_Taus_Progression_R2.at(0);
                            pt_Taus_Progression_R2.at(1)=pt_Taus_Progression_R2.at(0);
                            Index_Taus_Progression_R2.at(0)=iL1Tau;
                            pt_Taus_Progression_R2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R2)
                        {
                            Index_Taus_Progression_R2.at(1)=iL1Tau;
                            pt_Taus_Progression_R2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S2)
                        {
                            Index_Taus_Progression_S2.at(1)=Index_Taus_Progression_S2.at(0);
                            pt_Taus_Progression_S2.at(1)=pt_Taus_Progression_S2.at(0);
                            Index_Taus_Progression_S2.at(0)=iL1Tau;
                            pt_Taus_Progression_S2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S2)
                        {
                            Index_Taus_Progression_S2.at(1)=iL1Tau;
                            pt_Taus_Progression_S2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T2)
                        {
                            Index_Taus_Progression_T2.at(1)=Index_Taus_Progression_T2.at(0);
                            pt_Taus_Progression_T2.at(1)=pt_Taus_Progression_T2.at(0);
                            Index_Taus_Progression_T2.at(0)=iL1Tau;
                            pt_Taus_Progression_T2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T2)
                        {
                            Index_Taus_Progression_T2.at(1)=iL1Tau;
                            pt_Taus_Progression_T2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U2)
                        {
                            Index_Taus_Progression_U2.at(1)=Index_Taus_Progression_U2.at(0);
                            pt_Taus_Progression_U2.at(1)=pt_Taus_Progression_U2.at(0);
                            Index_Taus_Progression_U2.at(0)=iL1Tau;
                            pt_Taus_Progression_U2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U2)
                        {
                            Index_Taus_Progression_U2.at(1)=iL1Tau;
                            pt_Taus_Progression_U2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V2)
                        {
                            Index_Taus_Progression_V2.at(1)=Index_Taus_Progression_V2.at(0);
                            pt_Taus_Progression_V2.at(1)=pt_Taus_Progression_V2.at(0);
                            Index_Taus_Progression_V2.at(0)=iL1Tau;
                            pt_Taus_Progression_V2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V2)
                        {
                            Index_Taus_Progression_V2.at(1)=iL1Tau;
                            pt_Taus_Progression_V2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W2)
                        {
                            Index_Taus_Progression_W2.at(1)=Index_Taus_Progression_W2.at(0);
                            pt_Taus_Progression_W2.at(1)=pt_Taus_Progression_W2.at(0);
                            Index_Taus_Progression_W2.at(0)=iL1Tau;
                            pt_Taus_Progression_W2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W2)
                        {
                            Index_Taus_Progression_W2.at(1)=iL1Tau;
                            pt_Taus_Progression_W2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X2)
                        {
                            Index_Taus_Progression_X2.at(1)=Index_Taus_Progression_X2.at(0);
                            pt_Taus_Progression_X2.at(1)=pt_Taus_Progression_X2.at(0);
                            Index_Taus_Progression_X2.at(0)=iL1Tau;
                            pt_Taus_Progression_X2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X2)
                        {
                            Index_Taus_Progression_X2.at(1)=iL1Tau;
                            pt_Taus_Progression_X2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y2)
                        {
                            Index_Taus_Progression_Y2.at(1)=Index_Taus_Progression_Y2.at(0);
                            pt_Taus_Progression_Y2.at(1)=pt_Taus_Progression_Y2.at(0);
                            Index_Taus_Progression_Y2.at(0)=iL1Tau;
                            pt_Taus_Progression_Y2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y2)
                        {
                            Index_Taus_Progression_Y2.at(1)=iL1Tau;
                            pt_Taus_Progression_Y2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z2.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z2)
                        {
                            Index_Taus_Progression_Z2.at(1)=Index_Taus_Progression_Z2.at(0);
                            pt_Taus_Progression_Z2.at(1)=pt_Taus_Progression_Z2.at(0);
                            Index_Taus_Progression_Z2.at(0)=iL1Tau;
                            pt_Taus_Progression_Z2.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z2.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z2)
                        {
                            Index_Taus_Progression_Z2.at(1)=iL1Tau;
                            pt_Taus_Progression_Z2.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A3)
                        {
                            Index_Taus_Progression_A3.at(1)=Index_Taus_Progression_A3.at(0);
                            pt_Taus_Progression_A3.at(1)=pt_Taus_Progression_A3.at(0);
                            Index_Taus_Progression_A3.at(0)=iL1Tau;
                            pt_Taus_Progression_A3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A3)
                        {
                            Index_Taus_Progression_A3.at(1)=iL1Tau;
                            pt_Taus_Progression_A3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B3)
                        {
                            Index_Taus_Progression_B3.at(1)=Index_Taus_Progression_B3.at(0);
                            pt_Taus_Progression_B3.at(1)=pt_Taus_Progression_B3.at(0);
                            Index_Taus_Progression_B3.at(0)=iL1Tau;
                            pt_Taus_Progression_B3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B3)
                        {
                            Index_Taus_Progression_B3.at(1)=iL1Tau;
                            pt_Taus_Progression_B3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C3)
                        {
                            Index_Taus_Progression_C3.at(1)=Index_Taus_Progression_C3.at(0);
                            pt_Taus_Progression_C3.at(1)=pt_Taus_Progression_C3.at(0);
                            Index_Taus_Progression_C3.at(0)=iL1Tau;
                            pt_Taus_Progression_C3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C3)
                        {
                            Index_Taus_Progression_C3.at(1)=iL1Tau;
                            pt_Taus_Progression_C3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D3)
                        {
                            Index_Taus_Progression_D3.at(1)=Index_Taus_Progression_D3.at(0);
                            pt_Taus_Progression_D3.at(1)=pt_Taus_Progression_D3.at(0);
                            Index_Taus_Progression_D3.at(0)=iL1Tau;
                            pt_Taus_Progression_D3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D3)
                        {
                            Index_Taus_Progression_D3.at(1)=iL1Tau;
                            pt_Taus_Progression_D3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E3)
                        {
                            Index_Taus_Progression_E3.at(1)=Index_Taus_Progression_E3.at(0);
                            pt_Taus_Progression_E3.at(1)=pt_Taus_Progression_E3.at(0);
                            Index_Taus_Progression_E3.at(0)=iL1Tau;
                            pt_Taus_Progression_E3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E3)
                        {
                            Index_Taus_Progression_E3.at(1)=iL1Tau;
                            pt_Taus_Progression_E3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F3)
                        {
                            Index_Taus_Progression_F3.at(1)=Index_Taus_Progression_F3.at(0);
                            pt_Taus_Progression_F3.at(1)=pt_Taus_Progression_F3.at(0);
                            Index_Taus_Progression_F3.at(0)=iL1Tau;
                            pt_Taus_Progression_F3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F3)
                        {
                            Index_Taus_Progression_F3.at(1)=iL1Tau;
                            pt_Taus_Progression_F3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G3)
                        {
                            Index_Taus_Progression_G3.at(1)=Index_Taus_Progression_G3.at(0);
                            pt_Taus_Progression_G3.at(1)=pt_Taus_Progression_G3.at(0);
                            Index_Taus_Progression_G3.at(0)=iL1Tau;
                            pt_Taus_Progression_G3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G3)
                        {
                            Index_Taus_Progression_G3.at(1)=iL1Tau;
                            pt_Taus_Progression_G3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H3)
                        {
                            Index_Taus_Progression_H3.at(1)=Index_Taus_Progression_H3.at(0);
                            pt_Taus_Progression_H3.at(1)=pt_Taus_Progression_H3.at(0);
                            Index_Taus_Progression_H3.at(0)=iL1Tau;
                            pt_Taus_Progression_H3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H3)
                        {
                            Index_Taus_Progression_H3.at(1)=iL1Tau;
                            pt_Taus_Progression_H3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I3)
                        {
                            Index_Taus_Progression_I3.at(1)=Index_Taus_Progression_I3.at(0);
                            pt_Taus_Progression_I3.at(1)=pt_Taus_Progression_I3.at(0);
                            Index_Taus_Progression_I3.at(0)=iL1Tau;
                            pt_Taus_Progression_I3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I3)
                        {
                            Index_Taus_Progression_I3.at(1)=iL1Tau;
                            pt_Taus_Progression_I3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J3)
                        {
                            Index_Taus_Progression_J3.at(1)=Index_Taus_Progression_J3.at(0);
                            pt_Taus_Progression_J3.at(1)=pt_Taus_Progression_J3.at(0);
                            Index_Taus_Progression_J3.at(0)=iL1Tau;
                            pt_Taus_Progression_J3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J3)
                        {
                            Index_Taus_Progression_J3.at(1)=iL1Tau;
                            pt_Taus_Progression_J3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K3)
                        {
                            Index_Taus_Progression_K3.at(1)=Index_Taus_Progression_K3.at(0);
                            pt_Taus_Progression_K3.at(1)=pt_Taus_Progression_K3.at(0);
                            Index_Taus_Progression_K3.at(0)=iL1Tau;
                            pt_Taus_Progression_K3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K3)
                        {
                            Index_Taus_Progression_K3.at(1)=iL1Tau;
                            pt_Taus_Progression_K3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L3)
                        {
                            Index_Taus_Progression_L3.at(1)=Index_Taus_Progression_L3.at(0);
                            pt_Taus_Progression_L3.at(1)=pt_Taus_Progression_L3.at(0);
                            Index_Taus_Progression_L3.at(0)=iL1Tau;
                            pt_Taus_Progression_L3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L3)
                        {
                            Index_Taus_Progression_L3.at(1)=iL1Tau;
                            pt_Taus_Progression_L3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M3)
                        {
                            Index_Taus_Progression_M3.at(1)=Index_Taus_Progression_M3.at(0);
                            pt_Taus_Progression_M3.at(1)=pt_Taus_Progression_M3.at(0);
                            Index_Taus_Progression_M3.at(0)=iL1Tau;
                            pt_Taus_Progression_M3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M3)
                        {
                            Index_Taus_Progression_M3.at(1)=iL1Tau;
                            pt_Taus_Progression_M3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N3)
                        {
                            Index_Taus_Progression_N3.at(1)=Index_Taus_Progression_N3.at(0);
                            pt_Taus_Progression_N3.at(1)=pt_Taus_Progression_N3.at(0);
                            Index_Taus_Progression_N3.at(0)=iL1Tau;
                            pt_Taus_Progression_N3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N3)
                        {
                            Index_Taus_Progression_N3.at(1)=iL1Tau;
                            pt_Taus_Progression_N3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O3)
                        {
                            Index_Taus_Progression_O3.at(1)=Index_Taus_Progression_O3.at(0);
                            pt_Taus_Progression_O3.at(1)=pt_Taus_Progression_O3.at(0);
                            Index_Taus_Progression_O3.at(0)=iL1Tau;
                            pt_Taus_Progression_O3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O3)
                        {
                            Index_Taus_Progression_O3.at(1)=iL1Tau;
                            pt_Taus_Progression_O3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P3)
                        {
                            Index_Taus_Progression_P3.at(1)=Index_Taus_Progression_P3.at(0);
                            pt_Taus_Progression_P3.at(1)=pt_Taus_Progression_P3.at(0);
                            Index_Taus_Progression_P3.at(0)=iL1Tau;
                            pt_Taus_Progression_P3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P3)
                        {
                            Index_Taus_Progression_P3.at(1)=iL1Tau;
                            pt_Taus_Progression_P3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q3)
                        {
                            Index_Taus_Progression_Q3.at(1)=Index_Taus_Progression_Q3.at(0);
                            pt_Taus_Progression_Q3.at(1)=pt_Taus_Progression_Q3.at(0);
                            Index_Taus_Progression_Q3.at(0)=iL1Tau;
                            pt_Taus_Progression_Q3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q3)
                        {
                            Index_Taus_Progression_Q3.at(1)=iL1Tau;
                            pt_Taus_Progression_Q3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R3)
                        {
                            Index_Taus_Progression_R3.at(1)=Index_Taus_Progression_R3.at(0);
                            pt_Taus_Progression_R3.at(1)=pt_Taus_Progression_R3.at(0);
                            Index_Taus_Progression_R3.at(0)=iL1Tau;
                            pt_Taus_Progression_R3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R3)
                        {
                            Index_Taus_Progression_R3.at(1)=iL1Tau;
                            pt_Taus_Progression_R3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S3)
                        {
                            Index_Taus_Progression_S3.at(1)=Index_Taus_Progression_S3.at(0);
                            pt_Taus_Progression_S3.at(1)=pt_Taus_Progression_S3.at(0);
                            Index_Taus_Progression_S3.at(0)=iL1Tau;
                            pt_Taus_Progression_S3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S3)
                        {
                            Index_Taus_Progression_S3.at(1)=iL1Tau;
                            pt_Taus_Progression_S3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T3)
                        {
                            Index_Taus_Progression_T3.at(1)=Index_Taus_Progression_T3.at(0);
                            pt_Taus_Progression_T3.at(1)=pt_Taus_Progression_T3.at(0);
                            Index_Taus_Progression_T3.at(0)=iL1Tau;
                            pt_Taus_Progression_T3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T3)
                        {
                            Index_Taus_Progression_T3.at(1)=iL1Tau;
                            pt_Taus_Progression_T3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U3)
                        {
                            Index_Taus_Progression_U3.at(1)=Index_Taus_Progression_U3.at(0);
                            pt_Taus_Progression_U3.at(1)=pt_Taus_Progression_U3.at(0);
                            Index_Taus_Progression_U3.at(0)=iL1Tau;
                            pt_Taus_Progression_U3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U3)
                        {
                            Index_Taus_Progression_U3.at(1)=iL1Tau;
                            pt_Taus_Progression_U3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V3)
                        {
                            Index_Taus_Progression_V3.at(1)=Index_Taus_Progression_V3.at(0);
                            pt_Taus_Progression_V3.at(1)=pt_Taus_Progression_V3.at(0);
                            Index_Taus_Progression_V3.at(0)=iL1Tau;
                            pt_Taus_Progression_V3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V3)
                        {
                            Index_Taus_Progression_V3.at(1)=iL1Tau;
                            pt_Taus_Progression_V3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W3)
                        {
                            Index_Taus_Progression_W3.at(1)=Index_Taus_Progression_W3.at(0);
                            pt_Taus_Progression_W3.at(1)=pt_Taus_Progression_W3.at(0);
                            Index_Taus_Progression_W3.at(0)=iL1Tau;
                            pt_Taus_Progression_W3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W3)
                        {
                            Index_Taus_Progression_W3.at(1)=iL1Tau;
                            pt_Taus_Progression_W3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X3)
                        {
                            Index_Taus_Progression_X3.at(1)=Index_Taus_Progression_X3.at(0);
                            pt_Taus_Progression_X3.at(1)=pt_Taus_Progression_X3.at(0);
                            Index_Taus_Progression_X3.at(0)=iL1Tau;
                            pt_Taus_Progression_X3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X3)
                        {
                            Index_Taus_Progression_X3.at(1)=iL1Tau;
                            pt_Taus_Progression_X3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y3)
                        {
                            Index_Taus_Progression_Y3.at(1)=Index_Taus_Progression_Y3.at(0);
                            pt_Taus_Progression_Y3.at(1)=pt_Taus_Progression_Y3.at(0);
                            Index_Taus_Progression_Y3.at(0)=iL1Tau;
                            pt_Taus_Progression_Y3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y3)
                        {
                            Index_Taus_Progression_Y3.at(1)=iL1Tau;
                            pt_Taus_Progression_Y3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z3.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z3)
                        {
                            Index_Taus_Progression_Z3.at(1)=Index_Taus_Progression_Z3.at(0);
                            pt_Taus_Progression_Z3.at(1)=pt_Taus_Progression_Z3.at(0);
                            Index_Taus_Progression_Z3.at(0)=iL1Tau;
                            pt_Taus_Progression_Z3.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z3.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z3)
                        {
                            Index_Taus_Progression_Z3.at(1)=iL1Tau;
                            pt_Taus_Progression_Z3.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A4)
                        {
                            Index_Taus_Progression_A4.at(1)=Index_Taus_Progression_A4.at(0);
                            pt_Taus_Progression_A4.at(1)=pt_Taus_Progression_A4.at(0);
                            Index_Taus_Progression_A4.at(0)=iL1Tau;
                            pt_Taus_Progression_A4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A4)
                        {
                            Index_Taus_Progression_A4.at(1)=iL1Tau;
                            pt_Taus_Progression_A4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B4)
                        {
                            Index_Taus_Progression_B4.at(1)=Index_Taus_Progression_B4.at(0);
                            pt_Taus_Progression_B4.at(1)=pt_Taus_Progression_B4.at(0);
                            Index_Taus_Progression_B4.at(0)=iL1Tau;
                            pt_Taus_Progression_B4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B4)
                        {
                            Index_Taus_Progression_B4.at(1)=iL1Tau;
                            pt_Taus_Progression_B4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C4)
                        {
                            Index_Taus_Progression_C4.at(1)=Index_Taus_Progression_C4.at(0);
                            pt_Taus_Progression_C4.at(1)=pt_Taus_Progression_C4.at(0);
                            Index_Taus_Progression_C4.at(0)=iL1Tau;
                            pt_Taus_Progression_C4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C4)
                        {
                            Index_Taus_Progression_C4.at(1)=iL1Tau;
                            pt_Taus_Progression_C4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D4)
                        {
                            Index_Taus_Progression_D4.at(1)=Index_Taus_Progression_D4.at(0);
                            pt_Taus_Progression_D4.at(1)=pt_Taus_Progression_D4.at(0);
                            Index_Taus_Progression_D4.at(0)=iL1Tau;
                            pt_Taus_Progression_D4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D4)
                        {
                            Index_Taus_Progression_D4.at(1)=iL1Tau;
                            pt_Taus_Progression_D4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E4)
                        {
                            Index_Taus_Progression_E4.at(1)=Index_Taus_Progression_E4.at(0);
                            pt_Taus_Progression_E4.at(1)=pt_Taus_Progression_E4.at(0);
                            Index_Taus_Progression_E4.at(0)=iL1Tau;
                            pt_Taus_Progression_E4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E4)
                        {
                            Index_Taus_Progression_E4.at(1)=iL1Tau;
                            pt_Taus_Progression_E4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F4)
                        {
                            Index_Taus_Progression_F4.at(1)=Index_Taus_Progression_F4.at(0);
                            pt_Taus_Progression_F4.at(1)=pt_Taus_Progression_F4.at(0);
                            Index_Taus_Progression_F4.at(0)=iL1Tau;
                            pt_Taus_Progression_F4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F4)
                        {
                            Index_Taus_Progression_F4.at(1)=iL1Tau;
                            pt_Taus_Progression_F4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G4)
                        {
                            Index_Taus_Progression_G4.at(1)=Index_Taus_Progression_G4.at(0);
                            pt_Taus_Progression_G4.at(1)=pt_Taus_Progression_G4.at(0);
                            Index_Taus_Progression_G4.at(0)=iL1Tau;
                            pt_Taus_Progression_G4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G4)
                        {
                            Index_Taus_Progression_G4.at(1)=iL1Tau;
                            pt_Taus_Progression_G4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H4)
                        {
                            Index_Taus_Progression_H4.at(1)=Index_Taus_Progression_H4.at(0);
                            pt_Taus_Progression_H4.at(1)=pt_Taus_Progression_H4.at(0);
                            Index_Taus_Progression_H4.at(0)=iL1Tau;
                            pt_Taus_Progression_H4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H4)
                        {
                            Index_Taus_Progression_H4.at(1)=iL1Tau;
                            pt_Taus_Progression_H4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I4)
                        {
                            Index_Taus_Progression_I4.at(1)=Index_Taus_Progression_I4.at(0);
                            pt_Taus_Progression_I4.at(1)=pt_Taus_Progression_I4.at(0);
                            Index_Taus_Progression_I4.at(0)=iL1Tau;
                            pt_Taus_Progression_I4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I4)
                        {
                            Index_Taus_Progression_I4.at(1)=iL1Tau;
                            pt_Taus_Progression_I4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J4)
                        {
                            Index_Taus_Progression_J4.at(1)=Index_Taus_Progression_J4.at(0);
                            pt_Taus_Progression_J4.at(1)=pt_Taus_Progression_J4.at(0);
                            Index_Taus_Progression_J4.at(0)=iL1Tau;
                            pt_Taus_Progression_J4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J4)
                        {
                            Index_Taus_Progression_J4.at(1)=iL1Tau;
                            pt_Taus_Progression_J4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K4)
                        {
                            Index_Taus_Progression_K4.at(1)=Index_Taus_Progression_K4.at(0);
                            pt_Taus_Progression_K4.at(1)=pt_Taus_Progression_K4.at(0);
                            Index_Taus_Progression_K4.at(0)=iL1Tau;
                            pt_Taus_Progression_K4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K4)
                        {
                            Index_Taus_Progression_K4.at(1)=iL1Tau;
                            pt_Taus_Progression_K4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L4)
                        {
                            Index_Taus_Progression_L4.at(1)=Index_Taus_Progression_L4.at(0);
                            pt_Taus_Progression_L4.at(1)=pt_Taus_Progression_L4.at(0);
                            Index_Taus_Progression_L4.at(0)=iL1Tau;
                            pt_Taus_Progression_L4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L4)
                        {
                            Index_Taus_Progression_L4.at(1)=iL1Tau;
                            pt_Taus_Progression_L4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M4)
                        {
                            Index_Taus_Progression_M4.at(1)=Index_Taus_Progression_M4.at(0);
                            pt_Taus_Progression_M4.at(1)=pt_Taus_Progression_M4.at(0);
                            Index_Taus_Progression_M4.at(0)=iL1Tau;
                            pt_Taus_Progression_M4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M4)
                        {
                            Index_Taus_Progression_M4.at(1)=iL1Tau;
                            pt_Taus_Progression_M4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N4)
                        {
                            Index_Taus_Progression_N4.at(1)=Index_Taus_Progression_N4.at(0);
                            pt_Taus_Progression_N4.at(1)=pt_Taus_Progression_N4.at(0);
                            Index_Taus_Progression_N4.at(0)=iL1Tau;
                            pt_Taus_Progression_N4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N4)
                        {
                            Index_Taus_Progression_N4.at(1)=iL1Tau;
                            pt_Taus_Progression_N4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O4)
                        {
                            Index_Taus_Progression_O4.at(1)=Index_Taus_Progression_O4.at(0);
                            pt_Taus_Progression_O4.at(1)=pt_Taus_Progression_O4.at(0);
                            Index_Taus_Progression_O4.at(0)=iL1Tau;
                            pt_Taus_Progression_O4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O4)
                        {
                            Index_Taus_Progression_O4.at(1)=iL1Tau;
                            pt_Taus_Progression_O4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P4)
                        {
                            Index_Taus_Progression_P4.at(1)=Index_Taus_Progression_P4.at(0);
                            pt_Taus_Progression_P4.at(1)=pt_Taus_Progression_P4.at(0);
                            Index_Taus_Progression_P4.at(0)=iL1Tau;
                            pt_Taus_Progression_P4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P4)
                        {
                            Index_Taus_Progression_P4.at(1)=iL1Tau;
                            pt_Taus_Progression_P4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q4)
                        {
                            Index_Taus_Progression_Q4.at(1)=Index_Taus_Progression_Q4.at(0);
                            pt_Taus_Progression_Q4.at(1)=pt_Taus_Progression_Q4.at(0);
                            Index_Taus_Progression_Q4.at(0)=iL1Tau;
                            pt_Taus_Progression_Q4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q4)
                        {
                            Index_Taus_Progression_Q4.at(1)=iL1Tau;
                            pt_Taus_Progression_Q4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R4)
                        {
                            Index_Taus_Progression_R4.at(1)=Index_Taus_Progression_R4.at(0);
                            pt_Taus_Progression_R4.at(1)=pt_Taus_Progression_R4.at(0);
                            Index_Taus_Progression_R4.at(0)=iL1Tau;
                            pt_Taus_Progression_R4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R4)
                        {
                            Index_Taus_Progression_R4.at(1)=iL1Tau;
                            pt_Taus_Progression_R4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S4)
                        {
                            Index_Taus_Progression_S4.at(1)=Index_Taus_Progression_S4.at(0);
                            pt_Taus_Progression_S4.at(1)=pt_Taus_Progression_S4.at(0);
                            Index_Taus_Progression_S4.at(0)=iL1Tau;
                            pt_Taus_Progression_S4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S4)
                        {
                            Index_Taus_Progression_S4.at(1)=iL1Tau;
                            pt_Taus_Progression_S4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T4)
                        {
                            Index_Taus_Progression_T4.at(1)=Index_Taus_Progression_T4.at(0);
                            pt_Taus_Progression_T4.at(1)=pt_Taus_Progression_T4.at(0);
                            Index_Taus_Progression_T4.at(0)=iL1Tau;
                            pt_Taus_Progression_T4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T4)
                        {
                            Index_Taus_Progression_T4.at(1)=iL1Tau;
                            pt_Taus_Progression_T4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U4)
                        {
                            Index_Taus_Progression_U4.at(1)=Index_Taus_Progression_U4.at(0);
                            pt_Taus_Progression_U4.at(1)=pt_Taus_Progression_U4.at(0);
                            Index_Taus_Progression_U4.at(0)=iL1Tau;
                            pt_Taus_Progression_U4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U4)
                        {
                            Index_Taus_Progression_U4.at(1)=iL1Tau;
                            pt_Taus_Progression_U4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V4)
                        {
                            Index_Taus_Progression_V4.at(1)=Index_Taus_Progression_V4.at(0);
                            pt_Taus_Progression_V4.at(1)=pt_Taus_Progression_V4.at(0);
                            Index_Taus_Progression_V4.at(0)=iL1Tau;
                            pt_Taus_Progression_V4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V4)
                        {
                            Index_Taus_Progression_V4.at(1)=iL1Tau;
                            pt_Taus_Progression_V4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W4)
                        {
                            Index_Taus_Progression_W4.at(1)=Index_Taus_Progression_W4.at(0);
                            pt_Taus_Progression_W4.at(1)=pt_Taus_Progression_W4.at(0);
                            Index_Taus_Progression_W4.at(0)=iL1Tau;
                            pt_Taus_Progression_W4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W4)
                        {
                            Index_Taus_Progression_W4.at(1)=iL1Tau;
                            pt_Taus_Progression_W4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X4)
                        {
                            Index_Taus_Progression_X4.at(1)=Index_Taus_Progression_X4.at(0);
                            pt_Taus_Progression_X4.at(1)=pt_Taus_Progression_X4.at(0);
                            Index_Taus_Progression_X4.at(0)=iL1Tau;
                            pt_Taus_Progression_X4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X4)
                        {
                            Index_Taus_Progression_X4.at(1)=iL1Tau;
                            pt_Taus_Progression_X4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y4)
                        {
                            Index_Taus_Progression_Y4.at(1)=Index_Taus_Progression_Y4.at(0);
                            pt_Taus_Progression_Y4.at(1)=pt_Taus_Progression_Y4.at(0);
                            Index_Taus_Progression_Y4.at(0)=iL1Tau;
                            pt_Taus_Progression_Y4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y4)
                        {
                            Index_Taus_Progression_Y4.at(1)=iL1Tau;
                            pt_Taus_Progression_Y4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z4.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z4)
                        {
                            Index_Taus_Progression_Z4.at(1)=Index_Taus_Progression_Z4.at(0);
                            pt_Taus_Progression_Z4.at(1)=pt_Taus_Progression_Z4.at(0);
                            Index_Taus_Progression_Z4.at(0)=iL1Tau;
                            pt_Taus_Progression_Z4.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z4.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z4)
                        {
                            Index_Taus_Progression_Z4.at(1)=iL1Tau;
                            pt_Taus_Progression_Z4.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A5)
                        {
                            Index_Taus_Progression_A5.at(1)=Index_Taus_Progression_A5.at(0);
                            pt_Taus_Progression_A5.at(1)=pt_Taus_Progression_A5.at(0);
                            Index_Taus_Progression_A5.at(0)=iL1Tau;
                            pt_Taus_Progression_A5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A5)
                        {
                            Index_Taus_Progression_A5.at(1)=iL1Tau;
                            pt_Taus_Progression_A5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B5)
                        {
                            Index_Taus_Progression_B5.at(1)=Index_Taus_Progression_B5.at(0);
                            pt_Taus_Progression_B5.at(1)=pt_Taus_Progression_B5.at(0);
                            Index_Taus_Progression_B5.at(0)=iL1Tau;
                            pt_Taus_Progression_B5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B5)
                        {
                            Index_Taus_Progression_B5.at(1)=iL1Tau;
                            pt_Taus_Progression_B5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C5)
                        {
                            Index_Taus_Progression_C5.at(1)=Index_Taus_Progression_C5.at(0);
                            pt_Taus_Progression_C5.at(1)=pt_Taus_Progression_C5.at(0);
                            Index_Taus_Progression_C5.at(0)=iL1Tau;
                            pt_Taus_Progression_C5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C5)
                        {
                            Index_Taus_Progression_C5.at(1)=iL1Tau;
                            pt_Taus_Progression_C5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D5)
                        {
                            Index_Taus_Progression_D5.at(1)=Index_Taus_Progression_D5.at(0);
                            pt_Taus_Progression_D5.at(1)=pt_Taus_Progression_D5.at(0);
                            Index_Taus_Progression_D5.at(0)=iL1Tau;
                            pt_Taus_Progression_D5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D5)
                        {
                            Index_Taus_Progression_D5.at(1)=iL1Tau;
                            pt_Taus_Progression_D5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E5)
                        {
                            Index_Taus_Progression_E5.at(1)=Index_Taus_Progression_E5.at(0);
                            pt_Taus_Progression_E5.at(1)=pt_Taus_Progression_E5.at(0);
                            Index_Taus_Progression_E5.at(0)=iL1Tau;
                            pt_Taus_Progression_E5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E5)
                        {
                            Index_Taus_Progression_E5.at(1)=iL1Tau;
                            pt_Taus_Progression_E5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F5)
                        {
                            Index_Taus_Progression_F5.at(1)=Index_Taus_Progression_F5.at(0);
                            pt_Taus_Progression_F5.at(1)=pt_Taus_Progression_F5.at(0);
                            Index_Taus_Progression_F5.at(0)=iL1Tau;
                            pt_Taus_Progression_F5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F5)
                        {
                            Index_Taus_Progression_F5.at(1)=iL1Tau;
                            pt_Taus_Progression_F5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G5)
                        {
                            Index_Taus_Progression_G5.at(1)=Index_Taus_Progression_G5.at(0);
                            pt_Taus_Progression_G5.at(1)=pt_Taus_Progression_G5.at(0);
                            Index_Taus_Progression_G5.at(0)=iL1Tau;
                            pt_Taus_Progression_G5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G5)
                        {
                            Index_Taus_Progression_G5.at(1)=iL1Tau;
                            pt_Taus_Progression_G5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H5)
                        {
                            Index_Taus_Progression_H5.at(1)=Index_Taus_Progression_H5.at(0);
                            pt_Taus_Progression_H5.at(1)=pt_Taus_Progression_H5.at(0);
                            Index_Taus_Progression_H5.at(0)=iL1Tau;
                            pt_Taus_Progression_H5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H5)
                        {
                            Index_Taus_Progression_H5.at(1)=iL1Tau;
                            pt_Taus_Progression_H5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I5)
                        {
                            Index_Taus_Progression_I5.at(1)=Index_Taus_Progression_I5.at(0);
                            pt_Taus_Progression_I5.at(1)=pt_Taus_Progression_I5.at(0);
                            Index_Taus_Progression_I5.at(0)=iL1Tau;
                            pt_Taus_Progression_I5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I5)
                        {
                            Index_Taus_Progression_I5.at(1)=iL1Tau;
                            pt_Taus_Progression_I5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J5)
                        {
                            Index_Taus_Progression_J5.at(1)=Index_Taus_Progression_J5.at(0);
                            pt_Taus_Progression_J5.at(1)=pt_Taus_Progression_J5.at(0);
                            Index_Taus_Progression_J5.at(0)=iL1Tau;
                            pt_Taus_Progression_J5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J5)
                        {
                            Index_Taus_Progression_J5.at(1)=iL1Tau;
                            pt_Taus_Progression_J5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K5)
                        {
                            Index_Taus_Progression_K5.at(1)=Index_Taus_Progression_K5.at(0);
                            pt_Taus_Progression_K5.at(1)=pt_Taus_Progression_K5.at(0);
                            Index_Taus_Progression_K5.at(0)=iL1Tau;
                            pt_Taus_Progression_K5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K5)
                        {
                            Index_Taus_Progression_K5.at(1)=iL1Tau;
                            pt_Taus_Progression_K5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L5)
                        {
                            Index_Taus_Progression_L5.at(1)=Index_Taus_Progression_L5.at(0);
                            pt_Taus_Progression_L5.at(1)=pt_Taus_Progression_L5.at(0);
                            Index_Taus_Progression_L5.at(0)=iL1Tau;
                            pt_Taus_Progression_L5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L5)
                        {
                            Index_Taus_Progression_L5.at(1)=iL1Tau;
                            pt_Taus_Progression_L5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M5)
                        {
                            Index_Taus_Progression_M5.at(1)=Index_Taus_Progression_M5.at(0);
                            pt_Taus_Progression_M5.at(1)=pt_Taus_Progression_M5.at(0);
                            Index_Taus_Progression_M5.at(0)=iL1Tau;
                            pt_Taus_Progression_M5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M5)
                        {
                            Index_Taus_Progression_M5.at(1)=iL1Tau;
                            pt_Taus_Progression_M5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N5)
                        {
                            Index_Taus_Progression_N5.at(1)=Index_Taus_Progression_N5.at(0);
                            pt_Taus_Progression_N5.at(1)=pt_Taus_Progression_N5.at(0);
                            Index_Taus_Progression_N5.at(0)=iL1Tau;
                            pt_Taus_Progression_N5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N5)
                        {
                            Index_Taus_Progression_N5.at(1)=iL1Tau;
                            pt_Taus_Progression_N5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O5)
                        {
                            Index_Taus_Progression_O5.at(1)=Index_Taus_Progression_O5.at(0);
                            pt_Taus_Progression_O5.at(1)=pt_Taus_Progression_O5.at(0);
                            Index_Taus_Progression_O5.at(0)=iL1Tau;
                            pt_Taus_Progression_O5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O5)
                        {
                            Index_Taus_Progression_O5.at(1)=iL1Tau;
                            pt_Taus_Progression_O5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P5)
                        {
                            Index_Taus_Progression_P5.at(1)=Index_Taus_Progression_P5.at(0);
                            pt_Taus_Progression_P5.at(1)=pt_Taus_Progression_P5.at(0);
                            Index_Taus_Progression_P5.at(0)=iL1Tau;
                            pt_Taus_Progression_P5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P5)
                        {
                            Index_Taus_Progression_P5.at(1)=iL1Tau;
                            pt_Taus_Progression_P5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q5)
                        {
                            Index_Taus_Progression_Q5.at(1)=Index_Taus_Progression_Q5.at(0);
                            pt_Taus_Progression_Q5.at(1)=pt_Taus_Progression_Q5.at(0);
                            Index_Taus_Progression_Q5.at(0)=iL1Tau;
                            pt_Taus_Progression_Q5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q5)
                        {
                            Index_Taus_Progression_Q5.at(1)=iL1Tau;
                            pt_Taus_Progression_Q5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R5)
                        {
                            Index_Taus_Progression_R5.at(1)=Index_Taus_Progression_R5.at(0);
                            pt_Taus_Progression_R5.at(1)=pt_Taus_Progression_R5.at(0);
                            Index_Taus_Progression_R5.at(0)=iL1Tau;
                            pt_Taus_Progression_R5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R5)
                        {
                            Index_Taus_Progression_R5.at(1)=iL1Tau;
                            pt_Taus_Progression_R5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S5)
                        {
                            Index_Taus_Progression_S5.at(1)=Index_Taus_Progression_S5.at(0);
                            pt_Taus_Progression_S5.at(1)=pt_Taus_Progression_S5.at(0);
                            Index_Taus_Progression_S5.at(0)=iL1Tau;
                            pt_Taus_Progression_S5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S5)
                        {
                            Index_Taus_Progression_S5.at(1)=iL1Tau;
                            pt_Taus_Progression_S5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T5)
                        {
                            Index_Taus_Progression_T5.at(1)=Index_Taus_Progression_T5.at(0);
                            pt_Taus_Progression_T5.at(1)=pt_Taus_Progression_T5.at(0);
                            Index_Taus_Progression_T5.at(0)=iL1Tau;
                            pt_Taus_Progression_T5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T5)
                        {
                            Index_Taus_Progression_T5.at(1)=iL1Tau;
                            pt_Taus_Progression_T5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U5)
                        {
                            Index_Taus_Progression_U5.at(1)=Index_Taus_Progression_U5.at(0);
                            pt_Taus_Progression_U5.at(1)=pt_Taus_Progression_U5.at(0);
                            Index_Taus_Progression_U5.at(0)=iL1Tau;
                            pt_Taus_Progression_U5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U5)
                        {
                            Index_Taus_Progression_U5.at(1)=iL1Tau;
                            pt_Taus_Progression_U5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V5)
                        {
                            Index_Taus_Progression_V5.at(1)=Index_Taus_Progression_V5.at(0);
                            pt_Taus_Progression_V5.at(1)=pt_Taus_Progression_V5.at(0);
                            Index_Taus_Progression_V5.at(0)=iL1Tau;
                            pt_Taus_Progression_V5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V5)
                        {
                            Index_Taus_Progression_V5.at(1)=iL1Tau;
                            pt_Taus_Progression_V5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W5)
                        {
                            Index_Taus_Progression_W5.at(1)=Index_Taus_Progression_W5.at(0);
                            pt_Taus_Progression_W5.at(1)=pt_Taus_Progression_W5.at(0);
                            Index_Taus_Progression_W5.at(0)=iL1Tau;
                            pt_Taus_Progression_W5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W5)
                        {
                            Index_Taus_Progression_W5.at(1)=iL1Tau;
                            pt_Taus_Progression_W5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X5)
                        {
                            Index_Taus_Progression_X5.at(1)=Index_Taus_Progression_X5.at(0);
                            pt_Taus_Progression_X5.at(1)=pt_Taus_Progression_X5.at(0);
                            Index_Taus_Progression_X5.at(0)=iL1Tau;
                            pt_Taus_Progression_X5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X5)
                        {
                            Index_Taus_Progression_X5.at(1)=iL1Tau;
                            pt_Taus_Progression_X5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y5)
                        {
                            Index_Taus_Progression_Y5.at(1)=Index_Taus_Progression_Y5.at(0);
                            pt_Taus_Progression_Y5.at(1)=pt_Taus_Progression_Y5.at(0);
                            Index_Taus_Progression_Y5.at(0)=iL1Tau;
                            pt_Taus_Progression_Y5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y5)
                        {
                            Index_Taus_Progression_Y5.at(1)=iL1Tau;
                            pt_Taus_Progression_Y5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z5.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z5)
                        {
                            Index_Taus_Progression_Z5.at(1)=Index_Taus_Progression_Z5.at(0);
                            pt_Taus_Progression_Z5.at(1)=pt_Taus_Progression_Z5.at(0);
                            Index_Taus_Progression_Z5.at(0)=iL1Tau;
                            pt_Taus_Progression_Z5.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z5.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z5)
                        {
                            Index_Taus_Progression_Z5.at(1)=iL1Tau;
                            pt_Taus_Progression_Z5.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A6)
                        {
                            Index_Taus_Progression_A6.at(1)=Index_Taus_Progression_A6.at(0);
                            pt_Taus_Progression_A6.at(1)=pt_Taus_Progression_A6.at(0);
                            Index_Taus_Progression_A6.at(0)=iL1Tau;
                            pt_Taus_Progression_A6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A6)
                        {
                            Index_Taus_Progression_A6.at(1)=iL1Tau;
                            pt_Taus_Progression_A6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B6)
                        {
                            Index_Taus_Progression_B6.at(1)=Index_Taus_Progression_B6.at(0);
                            pt_Taus_Progression_B6.at(1)=pt_Taus_Progression_B6.at(0);
                            Index_Taus_Progression_B6.at(0)=iL1Tau;
                            pt_Taus_Progression_B6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B6)
                        {
                            Index_Taus_Progression_B6.at(1)=iL1Tau;
                            pt_Taus_Progression_B6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C6)
                        {
                            Index_Taus_Progression_C6.at(1)=Index_Taus_Progression_C6.at(0);
                            pt_Taus_Progression_C6.at(1)=pt_Taus_Progression_C6.at(0);
                            Index_Taus_Progression_C6.at(0)=iL1Tau;
                            pt_Taus_Progression_C6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C6)
                        {
                            Index_Taus_Progression_C6.at(1)=iL1Tau;
                            pt_Taus_Progression_C6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D6)
                        {
                            Index_Taus_Progression_D6.at(1)=Index_Taus_Progression_D6.at(0);
                            pt_Taus_Progression_D6.at(1)=pt_Taus_Progression_D6.at(0);
                            Index_Taus_Progression_D6.at(0)=iL1Tau;
                            pt_Taus_Progression_D6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D6)
                        {
                            Index_Taus_Progression_D6.at(1)=iL1Tau;
                            pt_Taus_Progression_D6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E6)
                        {
                            Index_Taus_Progression_E6.at(1)=Index_Taus_Progression_E6.at(0);
                            pt_Taus_Progression_E6.at(1)=pt_Taus_Progression_E6.at(0);
                            Index_Taus_Progression_E6.at(0)=iL1Tau;
                            pt_Taus_Progression_E6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E6)
                        {
                            Index_Taus_Progression_E6.at(1)=iL1Tau;
                            pt_Taus_Progression_E6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F6)
                        {
                            Index_Taus_Progression_F6.at(1)=Index_Taus_Progression_F6.at(0);
                            pt_Taus_Progression_F6.at(1)=pt_Taus_Progression_F6.at(0);
                            Index_Taus_Progression_F6.at(0)=iL1Tau;
                            pt_Taus_Progression_F6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F6)
                        {
                            Index_Taus_Progression_F6.at(1)=iL1Tau;
                            pt_Taus_Progression_F6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G6)
                        {
                            Index_Taus_Progression_G6.at(1)=Index_Taus_Progression_G6.at(0);
                            pt_Taus_Progression_G6.at(1)=pt_Taus_Progression_G6.at(0);
                            Index_Taus_Progression_G6.at(0)=iL1Tau;
                            pt_Taus_Progression_G6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G6)
                        {
                            Index_Taus_Progression_G6.at(1)=iL1Tau;
                            pt_Taus_Progression_G6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H6)
                        {
                            Index_Taus_Progression_H6.at(1)=Index_Taus_Progression_H6.at(0);
                            pt_Taus_Progression_H6.at(1)=pt_Taus_Progression_H6.at(0);
                            Index_Taus_Progression_H6.at(0)=iL1Tau;
                            pt_Taus_Progression_H6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H6)
                        {
                            Index_Taus_Progression_H6.at(1)=iL1Tau;
                            pt_Taus_Progression_H6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I6)
                        {
                            Index_Taus_Progression_I6.at(1)=Index_Taus_Progression_I6.at(0);
                            pt_Taus_Progression_I6.at(1)=pt_Taus_Progression_I6.at(0);
                            Index_Taus_Progression_I6.at(0)=iL1Tau;
                            pt_Taus_Progression_I6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I6)
                        {
                            Index_Taus_Progression_I6.at(1)=iL1Tau;
                            pt_Taus_Progression_I6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J6)
                        {
                            Index_Taus_Progression_J6.at(1)=Index_Taus_Progression_J6.at(0);
                            pt_Taus_Progression_J6.at(1)=pt_Taus_Progression_J6.at(0);
                            Index_Taus_Progression_J6.at(0)=iL1Tau;
                            pt_Taus_Progression_J6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J6)
                        {
                            Index_Taus_Progression_J6.at(1)=iL1Tau;
                            pt_Taus_Progression_J6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K6)
                        {
                            Index_Taus_Progression_K6.at(1)=Index_Taus_Progression_K6.at(0);
                            pt_Taus_Progression_K6.at(1)=pt_Taus_Progression_K6.at(0);
                            Index_Taus_Progression_K6.at(0)=iL1Tau;
                            pt_Taus_Progression_K6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K6)
                        {
                            Index_Taus_Progression_K6.at(1)=iL1Tau;
                            pt_Taus_Progression_K6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L6)
                        {
                            Index_Taus_Progression_L6.at(1)=Index_Taus_Progression_L6.at(0);
                            pt_Taus_Progression_L6.at(1)=pt_Taus_Progression_L6.at(0);
                            Index_Taus_Progression_L6.at(0)=iL1Tau;
                            pt_Taus_Progression_L6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L6)
                        {
                            Index_Taus_Progression_L6.at(1)=iL1Tau;
                            pt_Taus_Progression_L6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M6)
                        {
                            Index_Taus_Progression_M6.at(1)=Index_Taus_Progression_M6.at(0);
                            pt_Taus_Progression_M6.at(1)=pt_Taus_Progression_M6.at(0);
                            Index_Taus_Progression_M6.at(0)=iL1Tau;
                            pt_Taus_Progression_M6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M6)
                        {
                            Index_Taus_Progression_M6.at(1)=iL1Tau;
                            pt_Taus_Progression_M6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N6)
                        {
                            Index_Taus_Progression_N6.at(1)=Index_Taus_Progression_N6.at(0);
                            pt_Taus_Progression_N6.at(1)=pt_Taus_Progression_N6.at(0);
                            Index_Taus_Progression_N6.at(0)=iL1Tau;
                            pt_Taus_Progression_N6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N6)
                        {
                            Index_Taus_Progression_N6.at(1)=iL1Tau;
                            pt_Taus_Progression_N6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O6)
                        {
                            Index_Taus_Progression_O6.at(1)=Index_Taus_Progression_O6.at(0);
                            pt_Taus_Progression_O6.at(1)=pt_Taus_Progression_O6.at(0);
                            Index_Taus_Progression_O6.at(0)=iL1Tau;
                            pt_Taus_Progression_O6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O6)
                        {
                            Index_Taus_Progression_O6.at(1)=iL1Tau;
                            pt_Taus_Progression_O6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P6)
                        {
                            Index_Taus_Progression_P6.at(1)=Index_Taus_Progression_P6.at(0);
                            pt_Taus_Progression_P6.at(1)=pt_Taus_Progression_P6.at(0);
                            Index_Taus_Progression_P6.at(0)=iL1Tau;
                            pt_Taus_Progression_P6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P6)
                        {
                            Index_Taus_Progression_P6.at(1)=iL1Tau;
                            pt_Taus_Progression_P6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q6)
                        {
                            Index_Taus_Progression_Q6.at(1)=Index_Taus_Progression_Q6.at(0);
                            pt_Taus_Progression_Q6.at(1)=pt_Taus_Progression_Q6.at(0);
                            Index_Taus_Progression_Q6.at(0)=iL1Tau;
                            pt_Taus_Progression_Q6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q6)
                        {
                            Index_Taus_Progression_Q6.at(1)=iL1Tau;
                            pt_Taus_Progression_Q6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R6)
                        {
                            Index_Taus_Progression_R6.at(1)=Index_Taus_Progression_R6.at(0);
                            pt_Taus_Progression_R6.at(1)=pt_Taus_Progression_R6.at(0);
                            Index_Taus_Progression_R6.at(0)=iL1Tau;
                            pt_Taus_Progression_R6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R6)
                        {
                            Index_Taus_Progression_R6.at(1)=iL1Tau;
                            pt_Taus_Progression_R6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S6)
                        {
                            Index_Taus_Progression_S6.at(1)=Index_Taus_Progression_S6.at(0);
                            pt_Taus_Progression_S6.at(1)=pt_Taus_Progression_S6.at(0);
                            Index_Taus_Progression_S6.at(0)=iL1Tau;
                            pt_Taus_Progression_S6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S6)
                        {
                            Index_Taus_Progression_S6.at(1)=iL1Tau;
                            pt_Taus_Progression_S6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T6)
                        {
                            Index_Taus_Progression_T6.at(1)=Index_Taus_Progression_T6.at(0);
                            pt_Taus_Progression_T6.at(1)=pt_Taus_Progression_T6.at(0);
                            Index_Taus_Progression_T6.at(0)=iL1Tau;
                            pt_Taus_Progression_T6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T6)
                        {
                            Index_Taus_Progression_T6.at(1)=iL1Tau;
                            pt_Taus_Progression_T6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U6)
                        {
                            Index_Taus_Progression_U6.at(1)=Index_Taus_Progression_U6.at(0);
                            pt_Taus_Progression_U6.at(1)=pt_Taus_Progression_U6.at(0);
                            Index_Taus_Progression_U6.at(0)=iL1Tau;
                            pt_Taus_Progression_U6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U6)
                        {
                            Index_Taus_Progression_U6.at(1)=iL1Tau;
                            pt_Taus_Progression_U6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V6)
                        {
                            Index_Taus_Progression_V6.at(1)=Index_Taus_Progression_V6.at(0);
                            pt_Taus_Progression_V6.at(1)=pt_Taus_Progression_V6.at(0);
                            Index_Taus_Progression_V6.at(0)=iL1Tau;
                            pt_Taus_Progression_V6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V6)
                        {
                            Index_Taus_Progression_V6.at(1)=iL1Tau;
                            pt_Taus_Progression_V6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W6)
                        {
                            Index_Taus_Progression_W6.at(1)=Index_Taus_Progression_W6.at(0);
                            pt_Taus_Progression_W6.at(1)=pt_Taus_Progression_W6.at(0);
                            Index_Taus_Progression_W6.at(0)=iL1Tau;
                            pt_Taus_Progression_W6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W6)
                        {
                            Index_Taus_Progression_W6.at(1)=iL1Tau;
                            pt_Taus_Progression_W6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X6)
                        {
                            Index_Taus_Progression_X6.at(1)=Index_Taus_Progression_X6.at(0);
                            pt_Taus_Progression_X6.at(1)=pt_Taus_Progression_X6.at(0);
                            Index_Taus_Progression_X6.at(0)=iL1Tau;
                            pt_Taus_Progression_X6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X6)
                        {
                            Index_Taus_Progression_X6.at(1)=iL1Tau;
                            pt_Taus_Progression_X6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y6)
                        {
                            Index_Taus_Progression_Y6.at(1)=Index_Taus_Progression_Y6.at(0);
                            pt_Taus_Progression_Y6.at(1)=pt_Taus_Progression_Y6.at(0);
                            Index_Taus_Progression_Y6.at(0)=iL1Tau;
                            pt_Taus_Progression_Y6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y6)
                        {
                            Index_Taus_Progression_Y6.at(1)=iL1Tau;
                            pt_Taus_Progression_Y6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z6.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z6)
                        {
                            Index_Taus_Progression_Z6.at(1)=Index_Taus_Progression_Z6.at(0);
                            pt_Taus_Progression_Z6.at(1)=pt_Taus_Progression_Z6.at(0);
                            Index_Taus_Progression_Z6.at(0)=iL1Tau;
                            pt_Taus_Progression_Z6.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z6.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z6)
                        {
                            Index_Taus_Progression_Z6.at(1)=iL1Tau;
                            pt_Taus_Progression_Z6.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A7)
                        {
                            Index_Taus_Progression_A7.at(1)=Index_Taus_Progression_A7.at(0);
                            pt_Taus_Progression_A7.at(1)=pt_Taus_Progression_A7.at(0);
                            Index_Taus_Progression_A7.at(0)=iL1Tau;
                            pt_Taus_Progression_A7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A7)
                        {
                            Index_Taus_Progression_A7.at(1)=iL1Tau;
                            pt_Taus_Progression_A7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B7)
                        {
                            Index_Taus_Progression_B7.at(1)=Index_Taus_Progression_B7.at(0);
                            pt_Taus_Progression_B7.at(1)=pt_Taus_Progression_B7.at(0);
                            Index_Taus_Progression_B7.at(0)=iL1Tau;
                            pt_Taus_Progression_B7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B7)
                        {
                            Index_Taus_Progression_B7.at(1)=iL1Tau;
                            pt_Taus_Progression_B7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C7)
                        {
                            Index_Taus_Progression_C7.at(1)=Index_Taus_Progression_C7.at(0);
                            pt_Taus_Progression_C7.at(1)=pt_Taus_Progression_C7.at(0);
                            Index_Taus_Progression_C7.at(0)=iL1Tau;
                            pt_Taus_Progression_C7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C7)
                        {
                            Index_Taus_Progression_C7.at(1)=iL1Tau;
                            pt_Taus_Progression_C7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D7)
                        {
                            Index_Taus_Progression_D7.at(1)=Index_Taus_Progression_D7.at(0);
                            pt_Taus_Progression_D7.at(1)=pt_Taus_Progression_D7.at(0);
                            Index_Taus_Progression_D7.at(0)=iL1Tau;
                            pt_Taus_Progression_D7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D7)
                        {
                            Index_Taus_Progression_D7.at(1)=iL1Tau;
                            pt_Taus_Progression_D7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E7)
                        {
                            Index_Taus_Progression_E7.at(1)=Index_Taus_Progression_E7.at(0);
                            pt_Taus_Progression_E7.at(1)=pt_Taus_Progression_E7.at(0);
                            Index_Taus_Progression_E7.at(0)=iL1Tau;
                            pt_Taus_Progression_E7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E7)
                        {
                            Index_Taus_Progression_E7.at(1)=iL1Tau;
                            pt_Taus_Progression_E7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F7)
                        {
                            Index_Taus_Progression_F7.at(1)=Index_Taus_Progression_F7.at(0);
                            pt_Taus_Progression_F7.at(1)=pt_Taus_Progression_F7.at(0);
                            Index_Taus_Progression_F7.at(0)=iL1Tau;
                            pt_Taus_Progression_F7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F7)
                        {
                            Index_Taus_Progression_F7.at(1)=iL1Tau;
                            pt_Taus_Progression_F7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G7)
                        {
                            Index_Taus_Progression_G7.at(1)=Index_Taus_Progression_G7.at(0);
                            pt_Taus_Progression_G7.at(1)=pt_Taus_Progression_G7.at(0);
                            Index_Taus_Progression_G7.at(0)=iL1Tau;
                            pt_Taus_Progression_G7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G7)
                        {
                            Index_Taus_Progression_G7.at(1)=iL1Tau;
                            pt_Taus_Progression_G7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H7)
                        {
                            Index_Taus_Progression_H7.at(1)=Index_Taus_Progression_H7.at(0);
                            pt_Taus_Progression_H7.at(1)=pt_Taus_Progression_H7.at(0);
                            Index_Taus_Progression_H7.at(0)=iL1Tau;
                            pt_Taus_Progression_H7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H7)
                        {
                            Index_Taus_Progression_H7.at(1)=iL1Tau;
                            pt_Taus_Progression_H7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I7)
                        {
                            Index_Taus_Progression_I7.at(1)=Index_Taus_Progression_I7.at(0);
                            pt_Taus_Progression_I7.at(1)=pt_Taus_Progression_I7.at(0);
                            Index_Taus_Progression_I7.at(0)=iL1Tau;
                            pt_Taus_Progression_I7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I7)
                        {
                            Index_Taus_Progression_I7.at(1)=iL1Tau;
                            pt_Taus_Progression_I7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J7)
                        {
                            Index_Taus_Progression_J7.at(1)=Index_Taus_Progression_J7.at(0);
                            pt_Taus_Progression_J7.at(1)=pt_Taus_Progression_J7.at(0);
                            Index_Taus_Progression_J7.at(0)=iL1Tau;
                            pt_Taus_Progression_J7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J7)
                        {
                            Index_Taus_Progression_J7.at(1)=iL1Tau;
                            pt_Taus_Progression_J7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K7)
                        {
                            Index_Taus_Progression_K7.at(1)=Index_Taus_Progression_K7.at(0);
                            pt_Taus_Progression_K7.at(1)=pt_Taus_Progression_K7.at(0);
                            Index_Taus_Progression_K7.at(0)=iL1Tau;
                            pt_Taus_Progression_K7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K7)
                        {
                            Index_Taus_Progression_K7.at(1)=iL1Tau;
                            pt_Taus_Progression_K7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L7)
                        {
                            Index_Taus_Progression_L7.at(1)=Index_Taus_Progression_L7.at(0);
                            pt_Taus_Progression_L7.at(1)=pt_Taus_Progression_L7.at(0);
                            Index_Taus_Progression_L7.at(0)=iL1Tau;
                            pt_Taus_Progression_L7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L7)
                        {
                            Index_Taus_Progression_L7.at(1)=iL1Tau;
                            pt_Taus_Progression_L7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M7)
                        {
                            Index_Taus_Progression_M7.at(1)=Index_Taus_Progression_M7.at(0);
                            pt_Taus_Progression_M7.at(1)=pt_Taus_Progression_M7.at(0);
                            Index_Taus_Progression_M7.at(0)=iL1Tau;
                            pt_Taus_Progression_M7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M7)
                        {
                            Index_Taus_Progression_M7.at(1)=iL1Tau;
                            pt_Taus_Progression_M7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N7)
                        {
                            Index_Taus_Progression_N7.at(1)=Index_Taus_Progression_N7.at(0);
                            pt_Taus_Progression_N7.at(1)=pt_Taus_Progression_N7.at(0);
                            Index_Taus_Progression_N7.at(0)=iL1Tau;
                            pt_Taus_Progression_N7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N7)
                        {
                            Index_Taus_Progression_N7.at(1)=iL1Tau;
                            pt_Taus_Progression_N7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O7)
                        {
                            Index_Taus_Progression_O7.at(1)=Index_Taus_Progression_O7.at(0);
                            pt_Taus_Progression_O7.at(1)=pt_Taus_Progression_O7.at(0);
                            Index_Taus_Progression_O7.at(0)=iL1Tau;
                            pt_Taus_Progression_O7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O7)
                        {
                            Index_Taus_Progression_O7.at(1)=iL1Tau;
                            pt_Taus_Progression_O7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P7)
                        {
                            Index_Taus_Progression_P7.at(1)=Index_Taus_Progression_P7.at(0);
                            pt_Taus_Progression_P7.at(1)=pt_Taus_Progression_P7.at(0);
                            Index_Taus_Progression_P7.at(0)=iL1Tau;
                            pt_Taus_Progression_P7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P7)
                        {
                            Index_Taus_Progression_P7.at(1)=iL1Tau;
                            pt_Taus_Progression_P7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q7)
                        {
                            Index_Taus_Progression_Q7.at(1)=Index_Taus_Progression_Q7.at(0);
                            pt_Taus_Progression_Q7.at(1)=pt_Taus_Progression_Q7.at(0);
                            Index_Taus_Progression_Q7.at(0)=iL1Tau;
                            pt_Taus_Progression_Q7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q7)
                        {
                            Index_Taus_Progression_Q7.at(1)=iL1Tau;
                            pt_Taus_Progression_Q7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R7)
                        {
                            Index_Taus_Progression_R7.at(1)=Index_Taus_Progression_R7.at(0);
                            pt_Taus_Progression_R7.at(1)=pt_Taus_Progression_R7.at(0);
                            Index_Taus_Progression_R7.at(0)=iL1Tau;
                            pt_Taus_Progression_R7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R7)
                        {
                            Index_Taus_Progression_R7.at(1)=iL1Tau;
                            pt_Taus_Progression_R7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S7)
                        {
                            Index_Taus_Progression_S7.at(1)=Index_Taus_Progression_S7.at(0);
                            pt_Taus_Progression_S7.at(1)=pt_Taus_Progression_S7.at(0);
                            Index_Taus_Progression_S7.at(0)=iL1Tau;
                            pt_Taus_Progression_S7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S7)
                        {
                            Index_Taus_Progression_S7.at(1)=iL1Tau;
                            pt_Taus_Progression_S7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T7)
                        {
                            Index_Taus_Progression_T7.at(1)=Index_Taus_Progression_T7.at(0);
                            pt_Taus_Progression_T7.at(1)=pt_Taus_Progression_T7.at(0);
                            Index_Taus_Progression_T7.at(0)=iL1Tau;
                            pt_Taus_Progression_T7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T7)
                        {
                            Index_Taus_Progression_T7.at(1)=iL1Tau;
                            pt_Taus_Progression_T7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U7)
                        {
                            Index_Taus_Progression_U7.at(1)=Index_Taus_Progression_U7.at(0);
                            pt_Taus_Progression_U7.at(1)=pt_Taus_Progression_U7.at(0);
                            Index_Taus_Progression_U7.at(0)=iL1Tau;
                            pt_Taus_Progression_U7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U7)
                        {
                            Index_Taus_Progression_U7.at(1)=iL1Tau;
                            pt_Taus_Progression_U7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V7)
                        {
                            Index_Taus_Progression_V7.at(1)=Index_Taus_Progression_V7.at(0);
                            pt_Taus_Progression_V7.at(1)=pt_Taus_Progression_V7.at(0);
                            Index_Taus_Progression_V7.at(0)=iL1Tau;
                            pt_Taus_Progression_V7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V7)
                        {
                            Index_Taus_Progression_V7.at(1)=iL1Tau;
                            pt_Taus_Progression_V7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W7)
                        {
                            Index_Taus_Progression_W7.at(1)=Index_Taus_Progression_W7.at(0);
                            pt_Taus_Progression_W7.at(1)=pt_Taus_Progression_W7.at(0);
                            Index_Taus_Progression_W7.at(0)=iL1Tau;
                            pt_Taus_Progression_W7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W7)
                        {
                            Index_Taus_Progression_W7.at(1)=iL1Tau;
                            pt_Taus_Progression_W7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X7)
                        {
                            Index_Taus_Progression_X7.at(1)=Index_Taus_Progression_X7.at(0);
                            pt_Taus_Progression_X7.at(1)=pt_Taus_Progression_X7.at(0);
                            Index_Taus_Progression_X7.at(0)=iL1Tau;
                            pt_Taus_Progression_X7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X7)
                        {
                            Index_Taus_Progression_X7.at(1)=iL1Tau;
                            pt_Taus_Progression_X7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y7)
                        {
                            Index_Taus_Progression_Y7.at(1)=Index_Taus_Progression_Y7.at(0);
                            pt_Taus_Progression_Y7.at(1)=pt_Taus_Progression_Y7.at(0);
                            Index_Taus_Progression_Y7.at(0)=iL1Tau;
                            pt_Taus_Progression_Y7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y7)
                        {
                            Index_Taus_Progression_Y7.at(1)=iL1Tau;
                            pt_Taus_Progression_Y7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z7.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z7)
                        {
                            Index_Taus_Progression_Z7.at(1)=Index_Taus_Progression_Z7.at(0);
                            pt_Taus_Progression_Z7.at(1)=pt_Taus_Progression_Z7.at(0);
                            Index_Taus_Progression_Z7.at(0)=iL1Tau;
                            pt_Taus_Progression_Z7.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z7.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z7)
                        {
                            Index_Taus_Progression_Z7.at(1)=iL1Tau;
                            pt_Taus_Progression_Z7.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A8)
                        {
                            Index_Taus_Progression_A8.at(1)=Index_Taus_Progression_A8.at(0);
                            pt_Taus_Progression_A8.at(1)=pt_Taus_Progression_A8.at(0);
                            Index_Taus_Progression_A8.at(0)=iL1Tau;
                            pt_Taus_Progression_A8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_A8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_A8)
                        {
                            Index_Taus_Progression_A8.at(1)=iL1Tau;
                            pt_Taus_Progression_A8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B8)
                        {
                            Index_Taus_Progression_B8.at(1)=Index_Taus_Progression_B8.at(0);
                            pt_Taus_Progression_B8.at(1)=pt_Taus_Progression_B8.at(0);
                            Index_Taus_Progression_B8.at(0)=iL1Tau;
                            pt_Taus_Progression_B8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_B8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_B8)
                        {
                            Index_Taus_Progression_B8.at(1)=iL1Tau;
                            pt_Taus_Progression_B8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C8)
                        {
                            Index_Taus_Progression_C8.at(1)=Index_Taus_Progression_C8.at(0);
                            pt_Taus_Progression_C8.at(1)=pt_Taus_Progression_C8.at(0);
                            Index_Taus_Progression_C8.at(0)=iL1Tau;
                            pt_Taus_Progression_C8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_C8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_C8)
                        {
                            Index_Taus_Progression_C8.at(1)=iL1Tau;
                            pt_Taus_Progression_C8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D8)
                        {
                            Index_Taus_Progression_D8.at(1)=Index_Taus_Progression_D8.at(0);
                            pt_Taus_Progression_D8.at(1)=pt_Taus_Progression_D8.at(0);
                            Index_Taus_Progression_D8.at(0)=iL1Tau;
                            pt_Taus_Progression_D8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_D8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_D8)
                        {
                            Index_Taus_Progression_D8.at(1)=iL1Tau;
                            pt_Taus_Progression_D8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E8)
                        {
                            Index_Taus_Progression_E8.at(1)=Index_Taus_Progression_E8.at(0);
                            pt_Taus_Progression_E8.at(1)=pt_Taus_Progression_E8.at(0);
                            Index_Taus_Progression_E8.at(0)=iL1Tau;
                            pt_Taus_Progression_E8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_E8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_E8)
                        {
                            Index_Taus_Progression_E8.at(1)=iL1Tau;
                            pt_Taus_Progression_E8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F8)
                        {
                            Index_Taus_Progression_F8.at(1)=Index_Taus_Progression_F8.at(0);
                            pt_Taus_Progression_F8.at(1)=pt_Taus_Progression_F8.at(0);
                            Index_Taus_Progression_F8.at(0)=iL1Tau;
                            pt_Taus_Progression_F8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_F8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_F8)
                        {
                            Index_Taus_Progression_F8.at(1)=iL1Tau;
                            pt_Taus_Progression_F8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G8)
                        {
                            Index_Taus_Progression_G8.at(1)=Index_Taus_Progression_G8.at(0);
                            pt_Taus_Progression_G8.at(1)=pt_Taus_Progression_G8.at(0);
                            Index_Taus_Progression_G8.at(0)=iL1Tau;
                            pt_Taus_Progression_G8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_G8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_G8)
                        {
                            Index_Taus_Progression_G8.at(1)=iL1Tau;
                            pt_Taus_Progression_G8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H8)
                        {
                            Index_Taus_Progression_H8.at(1)=Index_Taus_Progression_H8.at(0);
                            pt_Taus_Progression_H8.at(1)=pt_Taus_Progression_H8.at(0);
                            Index_Taus_Progression_H8.at(0)=iL1Tau;
                            pt_Taus_Progression_H8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_H8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_H8)
                        {
                            Index_Taus_Progression_H8.at(1)=iL1Tau;
                            pt_Taus_Progression_H8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I8)
                        {
                            Index_Taus_Progression_I8.at(1)=Index_Taus_Progression_I8.at(0);
                            pt_Taus_Progression_I8.at(1)=pt_Taus_Progression_I8.at(0);
                            Index_Taus_Progression_I8.at(0)=iL1Tau;
                            pt_Taus_Progression_I8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_I8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_I8)
                        {
                            Index_Taus_Progression_I8.at(1)=iL1Tau;
                            pt_Taus_Progression_I8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J8)
                        {
                            Index_Taus_Progression_J8.at(1)=Index_Taus_Progression_J8.at(0);
                            pt_Taus_Progression_J8.at(1)=pt_Taus_Progression_J8.at(0);
                            Index_Taus_Progression_J8.at(0)=iL1Tau;
                            pt_Taus_Progression_J8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_J8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_J8)
                        {
                            Index_Taus_Progression_J8.at(1)=iL1Tau;
                            pt_Taus_Progression_J8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K8)
                        {
                            Index_Taus_Progression_K8.at(1)=Index_Taus_Progression_K8.at(0);
                            pt_Taus_Progression_K8.at(1)=pt_Taus_Progression_K8.at(0);
                            Index_Taus_Progression_K8.at(0)=iL1Tau;
                            pt_Taus_Progression_K8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_K8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_K8)
                        {
                            Index_Taus_Progression_K8.at(1)=iL1Tau;
                            pt_Taus_Progression_K8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L8)
                        {
                            Index_Taus_Progression_L8.at(1)=Index_Taus_Progression_L8.at(0);
                            pt_Taus_Progression_L8.at(1)=pt_Taus_Progression_L8.at(0);
                            Index_Taus_Progression_L8.at(0)=iL1Tau;
                            pt_Taus_Progression_L8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_L8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_L8)
                        {
                            Index_Taus_Progression_L8.at(1)=iL1Tau;
                            pt_Taus_Progression_L8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M8)
                        {
                            Index_Taus_Progression_M8.at(1)=Index_Taus_Progression_M8.at(0);
                            pt_Taus_Progression_M8.at(1)=pt_Taus_Progression_M8.at(0);
                            Index_Taus_Progression_M8.at(0)=iL1Tau;
                            pt_Taus_Progression_M8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_M8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_M8)
                        {
                            Index_Taus_Progression_M8.at(1)=iL1Tau;
                            pt_Taus_Progression_M8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N8)
                        {
                            Index_Taus_Progression_N8.at(1)=Index_Taus_Progression_N8.at(0);
                            pt_Taus_Progression_N8.at(1)=pt_Taus_Progression_N8.at(0);
                            Index_Taus_Progression_N8.at(0)=iL1Tau;
                            pt_Taus_Progression_N8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_N8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_N8)
                        {
                            Index_Taus_Progression_N8.at(1)=iL1Tau;
                            pt_Taus_Progression_N8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O8)
                        {
                            Index_Taus_Progression_O8.at(1)=Index_Taus_Progression_O8.at(0);
                            pt_Taus_Progression_O8.at(1)=pt_Taus_Progression_O8.at(0);
                            Index_Taus_Progression_O8.at(0)=iL1Tau;
                            pt_Taus_Progression_O8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_O8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_O8)
                        {
                            Index_Taus_Progression_O8.at(1)=iL1Tau;
                            pt_Taus_Progression_O8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P8)
                        {
                            Index_Taus_Progression_P8.at(1)=Index_Taus_Progression_P8.at(0);
                            pt_Taus_Progression_P8.at(1)=pt_Taus_Progression_P8.at(0);
                            Index_Taus_Progression_P8.at(0)=iL1Tau;
                            pt_Taus_Progression_P8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_P8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_P8)
                        {
                            Index_Taus_Progression_P8.at(1)=iL1Tau;
                            pt_Taus_Progression_P8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q8)
                        {
                            Index_Taus_Progression_Q8.at(1)=Index_Taus_Progression_Q8.at(0);
                            pt_Taus_Progression_Q8.at(1)=pt_Taus_Progression_Q8.at(0);
                            Index_Taus_Progression_Q8.at(0)=iL1Tau;
                            pt_Taus_Progression_Q8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Q8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Q8)
                        {
                            Index_Taus_Progression_Q8.at(1)=iL1Tau;
                            pt_Taus_Progression_Q8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R8)
                        {
                            Index_Taus_Progression_R8.at(1)=Index_Taus_Progression_R8.at(0);
                            pt_Taus_Progression_R8.at(1)=pt_Taus_Progression_R8.at(0);
                            Index_Taus_Progression_R8.at(0)=iL1Tau;
                            pt_Taus_Progression_R8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_R8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_R8)
                        {
                            Index_Taus_Progression_R8.at(1)=iL1Tau;
                            pt_Taus_Progression_R8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S8)
                        {
                            Index_Taus_Progression_S8.at(1)=Index_Taus_Progression_S8.at(0);
                            pt_Taus_Progression_S8.at(1)=pt_Taus_Progression_S8.at(0);
                            Index_Taus_Progression_S8.at(0)=iL1Tau;
                            pt_Taus_Progression_S8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_S8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_S8)
                        {
                            Index_Taus_Progression_S8.at(1)=iL1Tau;
                            pt_Taus_Progression_S8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T8)
                        {
                            Index_Taus_Progression_T8.at(1)=Index_Taus_Progression_T8.at(0);
                            pt_Taus_Progression_T8.at(1)=pt_Taus_Progression_T8.at(0);
                            Index_Taus_Progression_T8.at(0)=iL1Tau;
                            pt_Taus_Progression_T8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_T8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_T8)
                        {
                            Index_Taus_Progression_T8.at(1)=iL1Tau;
                            pt_Taus_Progression_T8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U8)
                        {
                            Index_Taus_Progression_U8.at(1)=Index_Taus_Progression_U8.at(0);
                            pt_Taus_Progression_U8.at(1)=pt_Taus_Progression_U8.at(0);
                            Index_Taus_Progression_U8.at(0)=iL1Tau;
                            pt_Taus_Progression_U8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_U8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_U8)
                        {
                            Index_Taus_Progression_U8.at(1)=iL1Tau;
                            pt_Taus_Progression_U8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V8)
                        {
                            Index_Taus_Progression_V8.at(1)=Index_Taus_Progression_V8.at(0);
                            pt_Taus_Progression_V8.at(1)=pt_Taus_Progression_V8.at(0);
                            Index_Taus_Progression_V8.at(0)=iL1Tau;
                            pt_Taus_Progression_V8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_V8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_V8)
                        {
                            Index_Taus_Progression_V8.at(1)=iL1Tau;
                            pt_Taus_Progression_V8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W8)
                        {
                            Index_Taus_Progression_W8.at(1)=Index_Taus_Progression_W8.at(0);
                            pt_Taus_Progression_W8.at(1)=pt_Taus_Progression_W8.at(0);
                            Index_Taus_Progression_W8.at(0)=iL1Tau;
                            pt_Taus_Progression_W8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_W8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_W8)
                        {
                            Index_Taus_Progression_W8.at(1)=iL1Tau;
                            pt_Taus_Progression_W8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X8)
                        {
                            Index_Taus_Progression_X8.at(1)=Index_Taus_Progression_X8.at(0);
                            pt_Taus_Progression_X8.at(1)=pt_Taus_Progression_X8.at(0);
                            Index_Taus_Progression_X8.at(0)=iL1Tau;
                            pt_Taus_Progression_X8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_X8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_X8)
                        {
                            Index_Taus_Progression_X8.at(1)=iL1Tau;
                            pt_Taus_Progression_X8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y8)
                        {
                            Index_Taus_Progression_Y8.at(1)=Index_Taus_Progression_Y8.at(0);
                            pt_Taus_Progression_Y8.at(1)=pt_Taus_Progression_Y8.at(0);
                            Index_Taus_Progression_Y8.at(0)=iL1Tau;
                            pt_Taus_Progression_Y8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Y8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Y8)
                        {
                            Index_Taus_Progression_Y8.at(1)=iL1Tau;
                            pt_Taus_Progression_Y8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z8.at(0) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z8)
                        {
                            Index_Taus_Progression_Z8.at(1)=Index_Taus_Progression_Z8.at(0);
                            pt_Taus_Progression_Z8.at(1)=pt_Taus_Progression_Z8.at(0);
                            Index_Taus_Progression_Z8.at(0)=iL1Tau;
                            pt_Taus_Progression_Z8.at(0)=in_CalibPt->at(iL1Tau);
                        }
                    else if(in_CalibPt->at(iL1Tau)>=pt_Taus_Progression_Z8.at(1) && in_l1tEmuIsoEt->at(iL1Tau)<=IsoCut_Progression_Z8)
                        {
                            Index_Taus_Progression_Z8.at(1)=iL1Tau;
                            pt_Taus_Progression_Z8.at(1)=in_CalibPt->at(iL1Tau);
                        }
                    // END OF FULL BLOWN GRID SEARCH 

                }
            
            Bool_t Flag1 = false;
            Bool_t Flag2 = false;

            if(Index_Taus_IsoInf.at(0)>=0 && Index_Taus_IsoInf.at(1)>=0)
                {
                    pt_IsoInf_DiTau->Fill(pt_Taus_IsoInf.at(0),pt_Taus_IsoInf.at(1),weight);
                    if(pt_Taus_IsoInf.at(0)>80. && pt_Taus_IsoInf.at(1)>80.)
                        {
                            Flag1 = true;
                            DiTauCounterAll++;
                            // cout<<"event passing no iso"<<endl;
                        }
                }

            // if(Flag1!=Flag2) cout<<"!!! found it !!!"<<endl;
            if(Index_Taus_90.at(0)>=0 && Index_Taus_90.at(1)>=0)
                {
                    pt_DiTau_90pc->Fill(pt_Taus_90.at(0),pt_Taus_90.at(1),weight);
                }
            if(Index_Taus_80.at(0)>=0 && Index_Taus_80.at(1)>=0)
                {
                    pt_DiTau_80pc->Fill(pt_Taus_80.at(0),pt_Taus_80.at(1),weight);
                }
            if(Index_Taus_70.at(0)>=0 && Index_Taus_70.at(1)>=0)
                {
                    pt_DiTau_70pc->Fill(pt_Taus_70.at(0),pt_Taus_70.at(1),weight);
                }
            if(Index_Taus_60.at(0)>=0 && Index_Taus_60.at(1)>=0)
                {
                    pt_DiTau_60pc->Fill(pt_Taus_60.at(0),pt_Taus_60.at(1),weight);
                }
            if(Index_Taus_50.at(0)>=0 && Index_Taus_50.at(1)>=0)
                {
                    pt_DiTau_50pc->Fill(pt_Taus_50.at(0),pt_Taus_50.at(1),weight);
                }
            if(Index_Taus_Progression_1.at(0)>=0 && Index_Taus_Progression_1.at(1)>=0)
                {
                    pt_DiTau_Progression_1->Fill(pt_Taus_Progression_1.at(0),pt_Taus_Progression_1.at(1),weight);
                }
            if(Index_Taus_Progression_2.at(0)>=0 && Index_Taus_Progression_2.at(1)>=0)
                {
                    pt_DiTau_Progression_2->Fill(pt_Taus_Progression_2.at(0),pt_Taus_Progression_2.at(1),weight);
                }
            if(Index_Taus_Progression_3.at(0)>=0 && Index_Taus_Progression_3.at(1)>=0)
                {
                    pt_DiTau_Progression_3->Fill(pt_Taus_Progression_3.at(0),pt_Taus_Progression_3.at(1),weight);
                }
            if(Index_Taus_Progression_4.at(0)>=0 && Index_Taus_Progression_4.at(1)>=0)
                {
                    pt_DiTau_Progression_4->Fill(pt_Taus_Progression_4.at(0),pt_Taus_Progression_4.at(1),weight);
                }
            if(Index_Taus_Progression_5.at(0)>=0 && Index_Taus_Progression_5.at(1)>=0)
                {
                    pt_DiTau_Progression_5->Fill(pt_Taus_Progression_5.at(0),pt_Taus_Progression_5.at(1),weight);
                }
            if(Index_Taus_Progression_6.at(0)>=0 && Index_Taus_Progression_6.at(1)>=0)
                {
                    pt_DiTau_Progression_6->Fill(pt_Taus_Progression_6.at(0),pt_Taus_Progression_6.at(1),weight);
                }
            if(Index_Taus_Progression_7.at(0)>=0 && Index_Taus_Progression_7.at(1)>=0)
                {
                    pt_DiTau_Progression_7->Fill(pt_Taus_Progression_7.at(0),pt_Taus_Progression_7.at(1),weight);
                }
            if(Index_Taus_Progression_8.at(0)>=0 && Index_Taus_Progression_8.at(1)>=0)
                {
                    pt_DiTau_Progression_8->Fill(pt_Taus_Progression_8.at(0),pt_Taus_Progression_8.at(1),weight);
                }
            if(Index_Taus_Progression_9.at(0)>=0 && Index_Taus_Progression_9.at(1)>=0)
                {
                    pt_DiTau_Progression_9->Fill(pt_Taus_Progression_9.at(0),pt_Taus_Progression_9.at(1),weight);
                }
            if(Index_Taus_Progression_10.at(0)>=0 && Index_Taus_Progression_10.at(1)>=0)
                {
                    pt_DiTau_Progression_10->Fill(pt_Taus_Progression_10.at(0),pt_Taus_Progression_10.at(1),weight);
                }
            if(Index_Taus_Progression_11.at(0)>=0 && Index_Taus_Progression_11.at(1)>=0)
                {
                    pt_DiTau_Progression_11->Fill(pt_Taus_Progression_11.at(0),pt_Taus_Progression_11.at(1),weight);
                }
            if(Index_Taus_Progression_12.at(0)>=0 && Index_Taus_Progression_12.at(1)>=0)
                {
                    pt_DiTau_Progression_12->Fill(pt_Taus_Progression_12.at(0),pt_Taus_Progression_12.at(1),weight);
                }
            if(Index_Taus_Progression_13.at(0)>=0 && Index_Taus_Progression_13.at(1)>=0)
                {
                    pt_DiTau_Progression_13->Fill(pt_Taus_Progression_13.at(0),pt_Taus_Progression_13.at(1),weight);
                }
            if(Index_Taus_Progression_14.at(0)>=0 && Index_Taus_Progression_14.at(1)>=0)
                {
                    pt_DiTau_Progression_14->Fill(pt_Taus_Progression_14.at(0),pt_Taus_Progression_14.at(1),weight);
                }
            if(Index_Taus_Progression_15.at(0)>=0 && Index_Taus_Progression_15.at(1)>=0)
                {
                    pt_DiTau_Progression_15->Fill(pt_Taus_Progression_15.at(0),pt_Taus_Progression_15.at(1),weight);
                }
            if(Index_Taus_Progression_16.at(0)>=0 && Index_Taus_Progression_16.at(1)>=0)
                {
                    pt_DiTau_Progression_16->Fill(pt_Taus_Progression_16.at(0),pt_Taus_Progression_16.at(1),weight);
                }
            if(Index_Taus_Progression_17.at(0)>=0 && Index_Taus_Progression_17.at(1)>=0)
                {
                    pt_DiTau_Progression_17->Fill(pt_Taus_Progression_17.at(0),pt_Taus_Progression_17.at(1),weight);
                }
            if(Index_Taus_Progression_18.at(0)>=0 && Index_Taus_Progression_18.at(1)>=0)
                {
                    pt_DiTau_Progression_18->Fill(pt_Taus_Progression_18.at(0),pt_Taus_Progression_18.at(1),weight);
                }
            if(Index_Taus_Progression_19.at(0)>=0 && Index_Taus_Progression_19.at(1)>=0)
                {
                    pt_DiTau_Progression_19->Fill(pt_Taus_Progression_19.at(0),pt_Taus_Progression_19.at(1),weight);
                }
            if(Index_Taus_Progression_20.at(0)>=0 && Index_Taus_Progression_20.at(1)>=0)
                {
                    pt_DiTau_Progression_20->Fill(pt_Taus_Progression_20.at(0),pt_Taus_Progression_20.at(1),weight);
                }
            if(Index_Taus_Progression_21.at(0)>=0 && Index_Taus_Progression_21.at(1)>=0)
                {
                    pt_DiTau_Progression_21->Fill(pt_Taus_Progression_21.at(0),pt_Taus_Progression_21.at(1),weight);
                }
            if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0)
                {
                    // cout<<"weight = "<<weight<<endl;
                    pt_DiTau_Progression_22->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
                }
            if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0 && abs(eta_Taus_Progression_22.at(0))<1.4 && abs(eta_Taus_Progression_22.at(1))<1.4)
                {
                    pt_DiTau_Progression_22_barrel->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
                }
            if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0 && ((abs(eta_Taus_Progression_22.at(0))<1.4 && abs(eta_Taus_Progression_22.at(1))>1.4) || (abs(eta_Taus_Progression_22.at(0))>1.4 && abs(eta_Taus_Progression_22.at(1))<1.4)))
                {
                    pt_DiTau_Progression_22_barrelendcap->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
                }
            if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0 && abs(eta_Taus_Progression_22.at(0))>1.4 && abs(eta_Taus_Progression_22.at(1))>1.4)
                {
                    pt_DiTau_Progression_22_endcaps->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
                }

            if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0 && isMerged_Taus_Progression_22.at(0) && isMerged_Taus_Progression_22.at(1))
                {
                    pt_DiTau_Progression_22_BothAreMerged->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
                }
            if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0 && ((!isMerged_Taus_Progression_22.at(0) && isMerged_Taus_Progression_22.at(1)) || (isMerged_Taus_Progression_22.at(0) && !isMerged_Taus_Progression_22.at(1))))
                {
                    pt_DiTau_Progression_22_OneIsMerged->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
                }
            if(Index_Taus_Progression_22.at(0)>=0 && Index_Taus_Progression_22.at(1)>=0 && !isMerged_Taus_Progression_22.at(0) && !isMerged_Taus_Progression_22.at(1))
                {
                    pt_DiTau_Progression_22_BothAreNotMerged->Fill(pt_Taus_Progression_22.at(0),pt_Taus_Progression_22.at(1),weight);
                }
            if(Index_Taus_Progression_23.at(0)>=0 && Index_Taus_Progression_23.at(1)>=0)
                {
                    pt_DiTau_Progression_23->Fill(pt_Taus_Progression_23.at(0),pt_Taus_Progression_23.at(1),weight);
                }
            if(Index_Taus_Progression_24.at(0)>=0 && Index_Taus_Progression_24.at(1)>=0)
                {
                    pt_DiTau_Progression_24->Fill(pt_Taus_Progression_24.at(0),pt_Taus_Progression_24.at(1),weight);
                }
            if(Index_Taus_Progression_25.at(0)>=0 && Index_Taus_Progression_25.at(1)>=0)
                {
                    pt_DiTau_Progression_25->Fill(pt_Taus_Progression_25.at(0),pt_Taus_Progression_25.at(1),weight);
                }
            if(Index_Taus_Progression_26.at(0)>=0 && Index_Taus_Progression_26.at(1)>=0)
                {
                    pt_DiTau_Progression_26->Fill(pt_Taus_Progression_26.at(0),pt_Taus_Progression_26.at(1),weight);
                }
            if(Index_Taus_Progression_27.at(0)>=0 && Index_Taus_Progression_27.at(1)>=0)
                {
                    pt_DiTau_Progression_27->Fill(pt_Taus_Progression_27.at(0),pt_Taus_Progression_27.at(1),weight);
                }
            if(Index_Taus_Progression_28.at(0)>=0 && Index_Taus_Progression_28.at(1)>=0)
                {
                    pt_DiTau_Progression_28->Fill(pt_Taus_Progression_28.at(0),pt_Taus_Progression_28.at(1),weight);
                }
            if(Index_Taus_Progression_29.at(0)>=0 && Index_Taus_Progression_29.at(1)>=0)
                {
                    pt_DiTau_Progression_29->Fill(pt_Taus_Progression_29.at(0),pt_Taus_Progression_29.at(1),weight);
                }
            if(Index_Taus_Progression_30.at(0)>=0 && Index_Taus_Progression_30.at(1)>=0)
                {
                    pt_DiTau_Progression_30->Fill(pt_Taus_Progression_30.at(0),pt_Taus_Progression_30.at(1),weight);
                }


            if(Index_Taus_Progression_31.at(0)>=0 && Index_Taus_Progression_31.at(1)>=0)
                {
                    pt_DiTau_Progression_31->Fill(pt_Taus_Progression_31.at(0),pt_Taus_Progression_31.at(1),weight);
                }
            if(Index_Taus_Progression_31.at(0)>=0 && Index_Taus_Progression_31.at(1)>=0 && abs(eta_Taus_Progression_31.at(0))<1.4 && abs(eta_Taus_Progression_31.at(1))<1.4)
                {
                    pt_DiTau_Progression_31_barrel->Fill(pt_Taus_Progression_31.at(0),pt_Taus_Progression_31.at(1),weight);
                }
            if(Index_Taus_Progression_31.at(0)>=0 && Index_Taus_Progression_31.at(1)>=0 && ((abs(eta_Taus_Progression_31.at(0))<1.4 && abs(eta_Taus_Progression_31.at(1))>1.4) || (abs(eta_Taus_Progression_31.at(0))>1.4 && abs(eta_Taus_Progression_31.at(1))<1.4)))
                {
                    pt_DiTau_Progression_31_barrelendcap->Fill(pt_Taus_Progression_31.at(0),pt_Taus_Progression_31.at(1),weight);
                }
            if(Index_Taus_Progression_31.at(0)>=0 && Index_Taus_Progression_31.at(1)>=0 && abs(eta_Taus_Progression_31.at(0))>1.4 && abs(eta_Taus_Progression_31.at(1))>1.4)
                {
                    pt_DiTau_Progression_31_endcaps->Fill(pt_Taus_Progression_31.at(0),pt_Taus_Progression_31.at(1),weight);
                }


            if(Index_Taus_Progression_31_extrap.at(0)>=0 && Index_Taus_Progression_31_extrap.at(1)>=0)
                {
                    pt_DiTau_Progression_31_extrap->Fill(pt_Taus_Progression_31_extrap.at(0),pt_Taus_Progression_31_extrap.at(1),weight);
                }
            if(Index_Taus_Progression_31_extrap.at(0)>=0 && Index_Taus_Progression_31_extrap.at(1)>=0 && abs(eta_Taus_Progression_31_extrap.at(0))<1.4 && abs(eta_Taus_Progression_31_extrap.at(1))<1.4)
                {
                    pt_DiTau_Progression_31_extrap_barrel->Fill(pt_Taus_Progression_31_extrap.at(0),pt_Taus_Progression_31_extrap.at(1),weight);
                }
            if(Index_Taus_Progression_31_extrap.at(0)>=0 && Index_Taus_Progression_31_extrap.at(1)>=0 && ((abs(eta_Taus_Progression_31_extrap.at(0))<1.4 && abs(eta_Taus_Progression_31_extrap.at(1))>1.4) || (abs(eta_Taus_Progression_31_extrap.at(0))>1.4 && abs(eta_Taus_Progression_31_extrap.at(1))<1.4)))
                {
                    pt_DiTau_Progression_31_extrap_barrelendcap->Fill(pt_Taus_Progression_31_extrap.at(0),pt_Taus_Progression_31_extrap.at(1),weight);
                }
            if(Index_Taus_Progression_31_extrap.at(0)>=0 && Index_Taus_Progression_31_extrap.at(1)>=0 && abs(eta_Taus_Progression_31_extrap.at(0))>1.4 && abs(eta_Taus_Progression_31_extrap.at(1))>1.4)
                {
                    pt_DiTau_Progression_31_extrap_endcaps->Fill(pt_Taus_Progression_31_extrap.at(0),pt_Taus_Progression_31_extrap.at(1),weight);
                }

            // START OF FULL BLOWN GRID SEARCH
            if(Index_Taus_Progression_A0.at(0)>=0 && Index_Taus_Progression_A0.at(1)>=0)
                        {
                            pt_DiTau_Progression_A0->Fill(pt_Taus_Progression_A0.at(0),pt_Taus_Progression_A0.at(1),weight);
                        }
            if(Index_Taus_Progression_B0.at(0)>=0 && Index_Taus_Progression_B0.at(1)>=0)
                        {
                            pt_DiTau_Progression_B0->Fill(pt_Taus_Progression_B0.at(0),pt_Taus_Progression_B0.at(1),weight);
                        }
            if(Index_Taus_Progression_C0.at(0)>=0 && Index_Taus_Progression_C0.at(1)>=0)
                        {
                            pt_DiTau_Progression_C0->Fill(pt_Taus_Progression_C0.at(0),pt_Taus_Progression_C0.at(1),weight);
                        }
            if(Index_Taus_Progression_D0.at(0)>=0 && Index_Taus_Progression_D0.at(1)>=0)
                        {
                            pt_DiTau_Progression_D0->Fill(pt_Taus_Progression_D0.at(0),pt_Taus_Progression_D0.at(1),weight);
                        }
            if(Index_Taus_Progression_E0.at(0)>=0 && Index_Taus_Progression_E0.at(1)>=0)
                        {
                            pt_DiTau_Progression_E0->Fill(pt_Taus_Progression_E0.at(0),pt_Taus_Progression_E0.at(1),weight);
                        }
            if(Index_Taus_Progression_F0.at(0)>=0 && Index_Taus_Progression_F0.at(1)>=0)
                        {
                            pt_DiTau_Progression_F0->Fill(pt_Taus_Progression_F0.at(0),pt_Taus_Progression_F0.at(1),weight);
                        }
            if(Index_Taus_Progression_G0.at(0)>=0 && Index_Taus_Progression_G0.at(1)>=0)
                        {
                            pt_DiTau_Progression_G0->Fill(pt_Taus_Progression_G0.at(0),pt_Taus_Progression_G0.at(1),weight);
                        }
            if(Index_Taus_Progression_H0.at(0)>=0 && Index_Taus_Progression_H0.at(1)>=0)
                        {
                            pt_DiTau_Progression_H0->Fill(pt_Taus_Progression_H0.at(0),pt_Taus_Progression_H0.at(1),weight);
                        }
            if(Index_Taus_Progression_I0.at(0)>=0 && Index_Taus_Progression_I0.at(1)>=0)
                        {
                            pt_DiTau_Progression_I0->Fill(pt_Taus_Progression_I0.at(0),pt_Taus_Progression_I0.at(1),weight);
                        }
            if(Index_Taus_Progression_J0.at(0)>=0 && Index_Taus_Progression_J0.at(1)>=0)
                        {
                            pt_DiTau_Progression_J0->Fill(pt_Taus_Progression_J0.at(0),pt_Taus_Progression_J0.at(1),weight);
                        }
            if(Index_Taus_Progression_K0.at(0)>=0 && Index_Taus_Progression_K0.at(1)>=0)
                        {
                            pt_DiTau_Progression_K0->Fill(pt_Taus_Progression_K0.at(0),pt_Taus_Progression_K0.at(1),weight);
                        }
            if(Index_Taus_Progression_L0.at(0)>=0 && Index_Taus_Progression_L0.at(1)>=0)
                        {
                            pt_DiTau_Progression_L0->Fill(pt_Taus_Progression_L0.at(0),pt_Taus_Progression_L0.at(1),weight);
                        }
            if(Index_Taus_Progression_M0.at(0)>=0 && Index_Taus_Progression_M0.at(1)>=0)
                        {
                            pt_DiTau_Progression_M0->Fill(pt_Taus_Progression_M0.at(0),pt_Taus_Progression_M0.at(1),weight);
                        }
            if(Index_Taus_Progression_N0.at(0)>=0 && Index_Taus_Progression_N0.at(1)>=0)
                        {
                            pt_DiTau_Progression_N0->Fill(pt_Taus_Progression_N0.at(0),pt_Taus_Progression_N0.at(1),weight);
                        }
            if(Index_Taus_Progression_O0.at(0)>=0 && Index_Taus_Progression_O0.at(1)>=0)
                        {
                            pt_DiTau_Progression_O0->Fill(pt_Taus_Progression_O0.at(0),pt_Taus_Progression_O0.at(1),weight);
                        }
            if(Index_Taus_Progression_P0.at(0)>=0 && Index_Taus_Progression_P0.at(1)>=0)
                        {
                            pt_DiTau_Progression_P0->Fill(pt_Taus_Progression_P0.at(0),pt_Taus_Progression_P0.at(1),weight);
                        }
            if(Index_Taus_Progression_Q0.at(0)>=0 && Index_Taus_Progression_Q0.at(1)>=0)
                        {
                            pt_DiTau_Progression_Q0->Fill(pt_Taus_Progression_Q0.at(0),pt_Taus_Progression_Q0.at(1),weight);
                        }
            if(Index_Taus_Progression_R0.at(0)>=0 && Index_Taus_Progression_R0.at(1)>=0)
                        {
                            pt_DiTau_Progression_R0->Fill(pt_Taus_Progression_R0.at(0),pt_Taus_Progression_R0.at(1),weight);
                        }
            if(Index_Taus_Progression_S0.at(0)>=0 && Index_Taus_Progression_S0.at(1)>=0)
                        {
                            pt_DiTau_Progression_S0->Fill(pt_Taus_Progression_S0.at(0),pt_Taus_Progression_S0.at(1),weight);
                        }
            if(Index_Taus_Progression_T0.at(0)>=0 && Index_Taus_Progression_T0.at(1)>=0)
                        {
                            pt_DiTau_Progression_T0->Fill(pt_Taus_Progression_T0.at(0),pt_Taus_Progression_T0.at(1),weight);
                        }
            if(Index_Taus_Progression_U0.at(0)>=0 && Index_Taus_Progression_U0.at(1)>=0)
                        {
                            pt_DiTau_Progression_U0->Fill(pt_Taus_Progression_U0.at(0),pt_Taus_Progression_U0.at(1),weight);
                        }
            if(Index_Taus_Progression_V0.at(0)>=0 && Index_Taus_Progression_V0.at(1)>=0)
                        {
                            pt_DiTau_Progression_V0->Fill(pt_Taus_Progression_V0.at(0),pt_Taus_Progression_V0.at(1),weight);
                        }
            if(Index_Taus_Progression_W0.at(0)>=0 && Index_Taus_Progression_W0.at(1)>=0)
                        {
                            pt_DiTau_Progression_W0->Fill(pt_Taus_Progression_W0.at(0),pt_Taus_Progression_W0.at(1),weight);
                        }
            if(Index_Taus_Progression_X0.at(0)>=0 && Index_Taus_Progression_X0.at(1)>=0)
                        {
                            pt_DiTau_Progression_X0->Fill(pt_Taus_Progression_X0.at(0),pt_Taus_Progression_X0.at(1),weight);
                        }
            if(Index_Taus_Progression_Y0.at(0)>=0 && Index_Taus_Progression_Y0.at(1)>=0)
                        {
                            pt_DiTau_Progression_Y0->Fill(pt_Taus_Progression_Y0.at(0),pt_Taus_Progression_Y0.at(1),weight);
                        }
            if(Index_Taus_Progression_Z0.at(0)>=0 && Index_Taus_Progression_Z0.at(1)>=0)
                        {
                            pt_DiTau_Progression_Z0->Fill(pt_Taus_Progression_Z0.at(0),pt_Taus_Progression_Z0.at(1),weight);
                        }
            if(Index_Taus_Progression_A1.at(0)>=0 && Index_Taus_Progression_A1.at(1)>=0)
                        {
                            pt_DiTau_Progression_A1->Fill(pt_Taus_Progression_A1.at(0),pt_Taus_Progression_A1.at(1),weight);
                        }
            if(Index_Taus_Progression_B1.at(0)>=0 && Index_Taus_Progression_B1.at(1)>=0)
                        {
                            pt_DiTau_Progression_B1->Fill(pt_Taus_Progression_B1.at(0),pt_Taus_Progression_B1.at(1),weight);
                        }
            if(Index_Taus_Progression_C1.at(0)>=0 && Index_Taus_Progression_C1.at(1)>=0)
                        {
                            pt_DiTau_Progression_C1->Fill(pt_Taus_Progression_C1.at(0),pt_Taus_Progression_C1.at(1),weight);
                        }
            if(Index_Taus_Progression_D1.at(0)>=0 && Index_Taus_Progression_D1.at(1)>=0)
                        {
                            pt_DiTau_Progression_D1->Fill(pt_Taus_Progression_D1.at(0),pt_Taus_Progression_D1.at(1),weight);
                        }
            if(Index_Taus_Progression_E1.at(0)>=0 && Index_Taus_Progression_E1.at(1)>=0)
                        {
                            pt_DiTau_Progression_E1->Fill(pt_Taus_Progression_E1.at(0),pt_Taus_Progression_E1.at(1),weight);
                        }
            if(Index_Taus_Progression_F1.at(0)>=0 && Index_Taus_Progression_F1.at(1)>=0)
                        {
                            pt_DiTau_Progression_F1->Fill(pt_Taus_Progression_F1.at(0),pt_Taus_Progression_F1.at(1),weight);
                        }
            if(Index_Taus_Progression_G1.at(0)>=0 && Index_Taus_Progression_G1.at(1)>=0)
                        {
                            pt_DiTau_Progression_G1->Fill(pt_Taus_Progression_G1.at(0),pt_Taus_Progression_G1.at(1),weight);
                        }
            if(Index_Taus_Progression_H1.at(0)>=0 && Index_Taus_Progression_H1.at(1)>=0)
                        {
                            pt_DiTau_Progression_H1->Fill(pt_Taus_Progression_H1.at(0),pt_Taus_Progression_H1.at(1),weight);
                        }
            if(Index_Taus_Progression_I1.at(0)>=0 && Index_Taus_Progression_I1.at(1)>=0)
                        {
                            pt_DiTau_Progression_I1->Fill(pt_Taus_Progression_I1.at(0),pt_Taus_Progression_I1.at(1),weight);
                        }
            if(Index_Taus_Progression_J1.at(0)>=0 && Index_Taus_Progression_J1.at(1)>=0)
                        {
                            pt_DiTau_Progression_J1->Fill(pt_Taus_Progression_J1.at(0),pt_Taus_Progression_J1.at(1),weight);
                        }
            if(Index_Taus_Progression_K1.at(0)>=0 && Index_Taus_Progression_K1.at(1)>=0)
                        {
                            pt_DiTau_Progression_K1->Fill(pt_Taus_Progression_K1.at(0),pt_Taus_Progression_K1.at(1),weight);
                        }
            if(Index_Taus_Progression_L1.at(0)>=0 && Index_Taus_Progression_L1.at(1)>=0)
                        {
                            pt_DiTau_Progression_L1->Fill(pt_Taus_Progression_L1.at(0),pt_Taus_Progression_L1.at(1),weight);
                        }
            if(Index_Taus_Progression_M1.at(0)>=0 && Index_Taus_Progression_M1.at(1)>=0)
                        {
                            pt_DiTau_Progression_M1->Fill(pt_Taus_Progression_M1.at(0),pt_Taus_Progression_M1.at(1),weight);
                        }
            if(Index_Taus_Progression_N1.at(0)>=0 && Index_Taus_Progression_N1.at(1)>=0)
                        {
                            pt_DiTau_Progression_N1->Fill(pt_Taus_Progression_N1.at(0),pt_Taus_Progression_N1.at(1),weight);
                        }
            if(Index_Taus_Progression_O1.at(0)>=0 && Index_Taus_Progression_O1.at(1)>=0)
                        {
                            pt_DiTau_Progression_O1->Fill(pt_Taus_Progression_O1.at(0),pt_Taus_Progression_O1.at(1),weight);
                        }
            if(Index_Taus_Progression_P1.at(0)>=0 && Index_Taus_Progression_P1.at(1)>=0)
                        {
                            pt_DiTau_Progression_P1->Fill(pt_Taus_Progression_P1.at(0),pt_Taus_Progression_P1.at(1),weight);
                        }
            if(Index_Taus_Progression_Q1.at(0)>=0 && Index_Taus_Progression_Q1.at(1)>=0)
                        {
                            pt_DiTau_Progression_Q1->Fill(pt_Taus_Progression_Q1.at(0),pt_Taus_Progression_Q1.at(1),weight);
                        }
            if(Index_Taus_Progression_R1.at(0)>=0 && Index_Taus_Progression_R1.at(1)>=0)
                        {
                            pt_DiTau_Progression_R1->Fill(pt_Taus_Progression_R1.at(0),pt_Taus_Progression_R1.at(1),weight);
                        }
            if(Index_Taus_Progression_S1.at(0)>=0 && Index_Taus_Progression_S1.at(1)>=0)
                        {
                            pt_DiTau_Progression_S1->Fill(pt_Taus_Progression_S1.at(0),pt_Taus_Progression_S1.at(1),weight);
                        }
            if(Index_Taus_Progression_T1.at(0)>=0 && Index_Taus_Progression_T1.at(1)>=0)
                        {
                            pt_DiTau_Progression_T1->Fill(pt_Taus_Progression_T1.at(0),pt_Taus_Progression_T1.at(1),weight);
                        }
            if(Index_Taus_Progression_U1.at(0)>=0 && Index_Taus_Progression_U1.at(1)>=0)
                        {
                            pt_DiTau_Progression_U1->Fill(pt_Taus_Progression_U1.at(0),pt_Taus_Progression_U1.at(1),weight);
                        }
            if(Index_Taus_Progression_V1.at(0)>=0 && Index_Taus_Progression_V1.at(1)>=0)
                        {
                            pt_DiTau_Progression_V1->Fill(pt_Taus_Progression_V1.at(0),pt_Taus_Progression_V1.at(1),weight);
                        }
            if(Index_Taus_Progression_W1.at(0)>=0 && Index_Taus_Progression_W1.at(1)>=0)
                        {
                            pt_DiTau_Progression_W1->Fill(pt_Taus_Progression_W1.at(0),pt_Taus_Progression_W1.at(1),weight);
                        }
            if(Index_Taus_Progression_X1.at(0)>=0 && Index_Taus_Progression_X1.at(1)>=0)
                        {
                            pt_DiTau_Progression_X1->Fill(pt_Taus_Progression_X1.at(0),pt_Taus_Progression_X1.at(1),weight);
                        }
            if(Index_Taus_Progression_Y1.at(0)>=0 && Index_Taus_Progression_Y1.at(1)>=0)
                        {
                            pt_DiTau_Progression_Y1->Fill(pt_Taus_Progression_Y1.at(0),pt_Taus_Progression_Y1.at(1),weight);
                        }
            if(Index_Taus_Progression_Z1.at(0)>=0 && Index_Taus_Progression_Z1.at(1)>=0)
                        {
                            pt_DiTau_Progression_Z1->Fill(pt_Taus_Progression_Z1.at(0),pt_Taus_Progression_Z1.at(1),weight);
                        }
            if(Index_Taus_Progression_A2.at(0)>=0 && Index_Taus_Progression_A2.at(1)>=0)
                        {
                            pt_DiTau_Progression_A2->Fill(pt_Taus_Progression_A2.at(0),pt_Taus_Progression_A2.at(1),weight);
                        }
            if(Index_Taus_Progression_B2.at(0)>=0 && Index_Taus_Progression_B2.at(1)>=0)
                        {
                            pt_DiTau_Progression_B2->Fill(pt_Taus_Progression_B2.at(0),pt_Taus_Progression_B2.at(1),weight);
                        }
            if(Index_Taus_Progression_C2.at(0)>=0 && Index_Taus_Progression_C2.at(1)>=0)
                        {
                            pt_DiTau_Progression_C2->Fill(pt_Taus_Progression_C2.at(0),pt_Taus_Progression_C2.at(1),weight);
                        }
            if(Index_Taus_Progression_D2.at(0)>=0 && Index_Taus_Progression_D2.at(1)>=0)
                        {
                            pt_DiTau_Progression_D2->Fill(pt_Taus_Progression_D2.at(0),pt_Taus_Progression_D2.at(1),weight);
                        }
            if(Index_Taus_Progression_E2.at(0)>=0 && Index_Taus_Progression_E2.at(1)>=0)
                        {
                            pt_DiTau_Progression_E2->Fill(pt_Taus_Progression_E2.at(0),pt_Taus_Progression_E2.at(1),weight);
                        }
            if(Index_Taus_Progression_F2.at(0)>=0 && Index_Taus_Progression_F2.at(1)>=0)
                        {
                            pt_DiTau_Progression_F2->Fill(pt_Taus_Progression_F2.at(0),pt_Taus_Progression_F2.at(1),weight);
                        }
            if(Index_Taus_Progression_G2.at(0)>=0 && Index_Taus_Progression_G2.at(1)>=0)
                        {
                            pt_DiTau_Progression_G2->Fill(pt_Taus_Progression_G2.at(0),pt_Taus_Progression_G2.at(1),weight);
                        }
            if(Index_Taus_Progression_H2.at(0)>=0 && Index_Taus_Progression_H2.at(1)>=0)
                        {
                            pt_DiTau_Progression_H2->Fill(pt_Taus_Progression_H2.at(0),pt_Taus_Progression_H2.at(1),weight);
                        }
            if(Index_Taus_Progression_I2.at(0)>=0 && Index_Taus_Progression_I2.at(1)>=0)
                        {
                            pt_DiTau_Progression_I2->Fill(pt_Taus_Progression_I2.at(0),pt_Taus_Progression_I2.at(1),weight);
                        }
            if(Index_Taus_Progression_J2.at(0)>=0 && Index_Taus_Progression_J2.at(1)>=0)
                        {
                            pt_DiTau_Progression_J2->Fill(pt_Taus_Progression_J2.at(0),pt_Taus_Progression_J2.at(1),weight);
                        }
            if(Index_Taus_Progression_K2.at(0)>=0 && Index_Taus_Progression_K2.at(1)>=0)
                        {
                            pt_DiTau_Progression_K2->Fill(pt_Taus_Progression_K2.at(0),pt_Taus_Progression_K2.at(1),weight);
                        }
            if(Index_Taus_Progression_L2.at(0)>=0 && Index_Taus_Progression_L2.at(1)>=0)
                        {
                            pt_DiTau_Progression_L2->Fill(pt_Taus_Progression_L2.at(0),pt_Taus_Progression_L2.at(1),weight);
                        }
            if(Index_Taus_Progression_M2.at(0)>=0 && Index_Taus_Progression_M2.at(1)>=0)
                        {
                            pt_DiTau_Progression_M2->Fill(pt_Taus_Progression_M2.at(0),pt_Taus_Progression_M2.at(1),weight);
                        }
            if(Index_Taus_Progression_N2.at(0)>=0 && Index_Taus_Progression_N2.at(1)>=0)
                        {
                            pt_DiTau_Progression_N2->Fill(pt_Taus_Progression_N2.at(0),pt_Taus_Progression_N2.at(1),weight);
                        }
            if(Index_Taus_Progression_O2.at(0)>=0 && Index_Taus_Progression_O2.at(1)>=0)
                        {
                            pt_DiTau_Progression_O2->Fill(pt_Taus_Progression_O2.at(0),pt_Taus_Progression_O2.at(1),weight);
                        }
            if(Index_Taus_Progression_P2.at(0)>=0 && Index_Taus_Progression_P2.at(1)>=0)
                        {
                            pt_DiTau_Progression_P2->Fill(pt_Taus_Progression_P2.at(0),pt_Taus_Progression_P2.at(1),weight);
                        }
            if(Index_Taus_Progression_Q2.at(0)>=0 && Index_Taus_Progression_Q2.at(1)>=0)
                        {
                            pt_DiTau_Progression_Q2->Fill(pt_Taus_Progression_Q2.at(0),pt_Taus_Progression_Q2.at(1),weight);
                        }
            if(Index_Taus_Progression_R2.at(0)>=0 && Index_Taus_Progression_R2.at(1)>=0)
                        {
                            pt_DiTau_Progression_R2->Fill(pt_Taus_Progression_R2.at(0),pt_Taus_Progression_R2.at(1),weight);
                        }
            if(Index_Taus_Progression_S2.at(0)>=0 && Index_Taus_Progression_S2.at(1)>=0)
                        {
                            pt_DiTau_Progression_S2->Fill(pt_Taus_Progression_S2.at(0),pt_Taus_Progression_S2.at(1),weight);
                        }
            if(Index_Taus_Progression_T2.at(0)>=0 && Index_Taus_Progression_T2.at(1)>=0)
                        {
                            pt_DiTau_Progression_T2->Fill(pt_Taus_Progression_T2.at(0),pt_Taus_Progression_T2.at(1),weight);
                        }
            if(Index_Taus_Progression_U2.at(0)>=0 && Index_Taus_Progression_U2.at(1)>=0)
                        {
                            pt_DiTau_Progression_U2->Fill(pt_Taus_Progression_U2.at(0),pt_Taus_Progression_U2.at(1),weight);
                        }
            if(Index_Taus_Progression_V2.at(0)>=0 && Index_Taus_Progression_V2.at(1)>=0)
                        {
                            pt_DiTau_Progression_V2->Fill(pt_Taus_Progression_V2.at(0),pt_Taus_Progression_V2.at(1),weight);
                        }
            if(Index_Taus_Progression_W2.at(0)>=0 && Index_Taus_Progression_W2.at(1)>=0)
                        {
                            pt_DiTau_Progression_W2->Fill(pt_Taus_Progression_W2.at(0),pt_Taus_Progression_W2.at(1),weight);
                        }
            if(Index_Taus_Progression_X2.at(0)>=0 && Index_Taus_Progression_X2.at(1)>=0)
                        {
                            pt_DiTau_Progression_X2->Fill(pt_Taus_Progression_X2.at(0),pt_Taus_Progression_X2.at(1),weight);
                        }
            if(Index_Taus_Progression_Y2.at(0)>=0 && Index_Taus_Progression_Y2.at(1)>=0)
                        {
                            pt_DiTau_Progression_Y2->Fill(pt_Taus_Progression_Y2.at(0),pt_Taus_Progression_Y2.at(1),weight);
                        }
            if(Index_Taus_Progression_Z2.at(0)>=0 && Index_Taus_Progression_Z2.at(1)>=0)
                        {
                            pt_DiTau_Progression_Z2->Fill(pt_Taus_Progression_Z2.at(0),pt_Taus_Progression_Z2.at(1),weight);
                        }
            if(Index_Taus_Progression_A3.at(0)>=0 && Index_Taus_Progression_A3.at(1)>=0)
                        {
                            pt_DiTau_Progression_A3->Fill(pt_Taus_Progression_A3.at(0),pt_Taus_Progression_A3.at(1),weight);
                        }
            if(Index_Taus_Progression_B3.at(0)>=0 && Index_Taus_Progression_B3.at(1)>=0)
                        {
                            pt_DiTau_Progression_B3->Fill(pt_Taus_Progression_B3.at(0),pt_Taus_Progression_B3.at(1),weight);
                        }
            if(Index_Taus_Progression_C3.at(0)>=0 && Index_Taus_Progression_C3.at(1)>=0)
                        {
                            pt_DiTau_Progression_C3->Fill(pt_Taus_Progression_C3.at(0),pt_Taus_Progression_C3.at(1),weight);
                        }
            if(Index_Taus_Progression_D3.at(0)>=0 && Index_Taus_Progression_D3.at(1)>=0)
                        {
                            pt_DiTau_Progression_D3->Fill(pt_Taus_Progression_D3.at(0),pt_Taus_Progression_D3.at(1),weight);
                        }
            if(Index_Taus_Progression_E3.at(0)>=0 && Index_Taus_Progression_E3.at(1)>=0)
                        {
                            pt_DiTau_Progression_E3->Fill(pt_Taus_Progression_E3.at(0),pt_Taus_Progression_E3.at(1),weight);
                        }
            if(Index_Taus_Progression_F3.at(0)>=0 && Index_Taus_Progression_F3.at(1)>=0)
                        {
                            pt_DiTau_Progression_F3->Fill(pt_Taus_Progression_F3.at(0),pt_Taus_Progression_F3.at(1),weight);
                        }
            if(Index_Taus_Progression_G3.at(0)>=0 && Index_Taus_Progression_G3.at(1)>=0)
                        {
                            pt_DiTau_Progression_G3->Fill(pt_Taus_Progression_G3.at(0),pt_Taus_Progression_G3.at(1),weight);
                        }
            if(Index_Taus_Progression_H3.at(0)>=0 && Index_Taus_Progression_H3.at(1)>=0)
                        {
                            pt_DiTau_Progression_H3->Fill(pt_Taus_Progression_H3.at(0),pt_Taus_Progression_H3.at(1),weight);
                        }
            if(Index_Taus_Progression_I3.at(0)>=0 && Index_Taus_Progression_I3.at(1)>=0)
                        {
                            pt_DiTau_Progression_I3->Fill(pt_Taus_Progression_I3.at(0),pt_Taus_Progression_I3.at(1),weight);
                        }
            if(Index_Taus_Progression_J3.at(0)>=0 && Index_Taus_Progression_J3.at(1)>=0)
                        {
                            pt_DiTau_Progression_J3->Fill(pt_Taus_Progression_J3.at(0),pt_Taus_Progression_J3.at(1),weight);
                        }
            if(Index_Taus_Progression_K3.at(0)>=0 && Index_Taus_Progression_K3.at(1)>=0)
                        {
                            pt_DiTau_Progression_K3->Fill(pt_Taus_Progression_K3.at(0),pt_Taus_Progression_K3.at(1),weight);
                        }
            if(Index_Taus_Progression_L3.at(0)>=0 && Index_Taus_Progression_L3.at(1)>=0)
                        {
                            pt_DiTau_Progression_L3->Fill(pt_Taus_Progression_L3.at(0),pt_Taus_Progression_L3.at(1),weight);
                        }
            if(Index_Taus_Progression_M3.at(0)>=0 && Index_Taus_Progression_M3.at(1)>=0)
                        {
                            pt_DiTau_Progression_M3->Fill(pt_Taus_Progression_M3.at(0),pt_Taus_Progression_M3.at(1),weight);
                        }
            if(Index_Taus_Progression_N3.at(0)>=0 && Index_Taus_Progression_N3.at(1)>=0)
                        {
                            pt_DiTau_Progression_N3->Fill(pt_Taus_Progression_N3.at(0),pt_Taus_Progression_N3.at(1),weight);
                        }
            if(Index_Taus_Progression_O3.at(0)>=0 && Index_Taus_Progression_O3.at(1)>=0)
                        {
                            pt_DiTau_Progression_O3->Fill(pt_Taus_Progression_O3.at(0),pt_Taus_Progression_O3.at(1),weight);
                        }
            if(Index_Taus_Progression_P3.at(0)>=0 && Index_Taus_Progression_P3.at(1)>=0)
                        {
                            pt_DiTau_Progression_P3->Fill(pt_Taus_Progression_P3.at(0),pt_Taus_Progression_P3.at(1),weight);
                        }
            if(Index_Taus_Progression_Q3.at(0)>=0 && Index_Taus_Progression_Q3.at(1)>=0)
                        {
                            pt_DiTau_Progression_Q3->Fill(pt_Taus_Progression_Q3.at(0),pt_Taus_Progression_Q3.at(1),weight);
                        }
            if(Index_Taus_Progression_R3.at(0)>=0 && Index_Taus_Progression_R3.at(1)>=0)
                        {
                            pt_DiTau_Progression_R3->Fill(pt_Taus_Progression_R3.at(0),pt_Taus_Progression_R3.at(1),weight);
                        }
            if(Index_Taus_Progression_S3.at(0)>=0 && Index_Taus_Progression_S3.at(1)>=0)
                        {
                            pt_DiTau_Progression_S3->Fill(pt_Taus_Progression_S3.at(0),pt_Taus_Progression_S3.at(1),weight);
                        }
            if(Index_Taus_Progression_T3.at(0)>=0 && Index_Taus_Progression_T3.at(1)>=0)
                        {
                            pt_DiTau_Progression_T3->Fill(pt_Taus_Progression_T3.at(0),pt_Taus_Progression_T3.at(1),weight);
                        }
            if(Index_Taus_Progression_U3.at(0)>=0 && Index_Taus_Progression_U3.at(1)>=0)
                        {
                            pt_DiTau_Progression_U3->Fill(pt_Taus_Progression_U3.at(0),pt_Taus_Progression_U3.at(1),weight);
                        }
            if(Index_Taus_Progression_V3.at(0)>=0 && Index_Taus_Progression_V3.at(1)>=0)
                        {
                            pt_DiTau_Progression_V3->Fill(pt_Taus_Progression_V3.at(0),pt_Taus_Progression_V3.at(1),weight);
                        }
            if(Index_Taus_Progression_W3.at(0)>=0 && Index_Taus_Progression_W3.at(1)>=0)
                        {
                            pt_DiTau_Progression_W3->Fill(pt_Taus_Progression_W3.at(0),pt_Taus_Progression_W3.at(1),weight);
                        }
            if(Index_Taus_Progression_X3.at(0)>=0 && Index_Taus_Progression_X3.at(1)>=0)
                        {
                            pt_DiTau_Progression_X3->Fill(pt_Taus_Progression_X3.at(0),pt_Taus_Progression_X3.at(1),weight);
                        }
            if(Index_Taus_Progression_Y3.at(0)>=0 && Index_Taus_Progression_Y3.at(1)>=0)
                        {
                            pt_DiTau_Progression_Y3->Fill(pt_Taus_Progression_Y3.at(0),pt_Taus_Progression_Y3.at(1),weight);
                        }
            if(Index_Taus_Progression_Z3.at(0)>=0 && Index_Taus_Progression_Z3.at(1)>=0)
                        {
                            pt_DiTau_Progression_Z3->Fill(pt_Taus_Progression_Z3.at(0),pt_Taus_Progression_Z3.at(1),weight);
                        }
            if(Index_Taus_Progression_A4.at(0)>=0 && Index_Taus_Progression_A4.at(1)>=0)
                        {
                            pt_DiTau_Progression_A4->Fill(pt_Taus_Progression_A4.at(0),pt_Taus_Progression_A4.at(1),weight);
                        }
            if(Index_Taus_Progression_B4.at(0)>=0 && Index_Taus_Progression_B4.at(1)>=0)
                        {
                            pt_DiTau_Progression_B4->Fill(pt_Taus_Progression_B4.at(0),pt_Taus_Progression_B4.at(1),weight);
                        }
            if(Index_Taus_Progression_C4.at(0)>=0 && Index_Taus_Progression_C4.at(1)>=0)
                        {
                            pt_DiTau_Progression_C4->Fill(pt_Taus_Progression_C4.at(0),pt_Taus_Progression_C4.at(1),weight);
                        }
            if(Index_Taus_Progression_D4.at(0)>=0 && Index_Taus_Progression_D4.at(1)>=0)
                        {
                            pt_DiTau_Progression_D4->Fill(pt_Taus_Progression_D4.at(0),pt_Taus_Progression_D4.at(1),weight);
                        }
            if(Index_Taus_Progression_E4.at(0)>=0 && Index_Taus_Progression_E4.at(1)>=0)
                        {
                            pt_DiTau_Progression_E4->Fill(pt_Taus_Progression_E4.at(0),pt_Taus_Progression_E4.at(1),weight);
                        }
            if(Index_Taus_Progression_F4.at(0)>=0 && Index_Taus_Progression_F4.at(1)>=0)
                        {
                            pt_DiTau_Progression_F4->Fill(pt_Taus_Progression_F4.at(0),pt_Taus_Progression_F4.at(1),weight);
                        }
            if(Index_Taus_Progression_G4.at(0)>=0 && Index_Taus_Progression_G4.at(1)>=0)
                        {
                            pt_DiTau_Progression_G4->Fill(pt_Taus_Progression_G4.at(0),pt_Taus_Progression_G4.at(1),weight);
                        }
            if(Index_Taus_Progression_H4.at(0)>=0 && Index_Taus_Progression_H4.at(1)>=0)
                        {
                            pt_DiTau_Progression_H4->Fill(pt_Taus_Progression_H4.at(0),pt_Taus_Progression_H4.at(1),weight);
                        }
            if(Index_Taus_Progression_I4.at(0)>=0 && Index_Taus_Progression_I4.at(1)>=0)
                        {
                            pt_DiTau_Progression_I4->Fill(pt_Taus_Progression_I4.at(0),pt_Taus_Progression_I4.at(1),weight);
                        }
            if(Index_Taus_Progression_J4.at(0)>=0 && Index_Taus_Progression_J4.at(1)>=0)
                        {
                            pt_DiTau_Progression_J4->Fill(pt_Taus_Progression_J4.at(0),pt_Taus_Progression_J4.at(1),weight);
                        }
            if(Index_Taus_Progression_K4.at(0)>=0 && Index_Taus_Progression_K4.at(1)>=0)
                        {
                            pt_DiTau_Progression_K4->Fill(pt_Taus_Progression_K4.at(0),pt_Taus_Progression_K4.at(1),weight);
                        }
            if(Index_Taus_Progression_L4.at(0)>=0 && Index_Taus_Progression_L4.at(1)>=0)
                        {
                            pt_DiTau_Progression_L4->Fill(pt_Taus_Progression_L4.at(0),pt_Taus_Progression_L4.at(1),weight);
                        }
            if(Index_Taus_Progression_M4.at(0)>=0 && Index_Taus_Progression_M4.at(1)>=0)
                        {
                            pt_DiTau_Progression_M4->Fill(pt_Taus_Progression_M4.at(0),pt_Taus_Progression_M4.at(1),weight);
                        }
            if(Index_Taus_Progression_N4.at(0)>=0 && Index_Taus_Progression_N4.at(1)>=0)
                        {
                            pt_DiTau_Progression_N4->Fill(pt_Taus_Progression_N4.at(0),pt_Taus_Progression_N4.at(1),weight);
                        }
            if(Index_Taus_Progression_O4.at(0)>=0 && Index_Taus_Progression_O4.at(1)>=0)
                        {
                            pt_DiTau_Progression_O4->Fill(pt_Taus_Progression_O4.at(0),pt_Taus_Progression_O4.at(1),weight);
                        }
            if(Index_Taus_Progression_P4.at(0)>=0 && Index_Taus_Progression_P4.at(1)>=0)
                        {
                            pt_DiTau_Progression_P4->Fill(pt_Taus_Progression_P4.at(0),pt_Taus_Progression_P4.at(1),weight);
                        }
            if(Index_Taus_Progression_Q4.at(0)>=0 && Index_Taus_Progression_Q4.at(1)>=0)
                        {
                            pt_DiTau_Progression_Q4->Fill(pt_Taus_Progression_Q4.at(0),pt_Taus_Progression_Q4.at(1),weight);
                        }
            if(Index_Taus_Progression_R4.at(0)>=0 && Index_Taus_Progression_R4.at(1)>=0)
                        {
                            pt_DiTau_Progression_R4->Fill(pt_Taus_Progression_R4.at(0),pt_Taus_Progression_R4.at(1),weight);
                        }
            if(Index_Taus_Progression_S4.at(0)>=0 && Index_Taus_Progression_S4.at(1)>=0)
                        {
                            pt_DiTau_Progression_S4->Fill(pt_Taus_Progression_S4.at(0),pt_Taus_Progression_S4.at(1),weight);
                        }
            if(Index_Taus_Progression_T4.at(0)>=0 && Index_Taus_Progression_T4.at(1)>=0)
                        {
                            pt_DiTau_Progression_T4->Fill(pt_Taus_Progression_T4.at(0),pt_Taus_Progression_T4.at(1),weight);
                        }
            if(Index_Taus_Progression_U4.at(0)>=0 && Index_Taus_Progression_U4.at(1)>=0)
                        {
                            pt_DiTau_Progression_U4->Fill(pt_Taus_Progression_U4.at(0),pt_Taus_Progression_U4.at(1),weight);
                        }
            if(Index_Taus_Progression_V4.at(0)>=0 && Index_Taus_Progression_V4.at(1)>=0)
                        {
                            pt_DiTau_Progression_V4->Fill(pt_Taus_Progression_V4.at(0),pt_Taus_Progression_V4.at(1),weight);
                        }
            if(Index_Taus_Progression_W4.at(0)>=0 && Index_Taus_Progression_W4.at(1)>=0)
                        {
                            pt_DiTau_Progression_W4->Fill(pt_Taus_Progression_W4.at(0),pt_Taus_Progression_W4.at(1),weight);
                        }
            if(Index_Taus_Progression_X4.at(0)>=0 && Index_Taus_Progression_X4.at(1)>=0)
                        {
                            pt_DiTau_Progression_X4->Fill(pt_Taus_Progression_X4.at(0),pt_Taus_Progression_X4.at(1),weight);
                        }
            if(Index_Taus_Progression_Y4.at(0)>=0 && Index_Taus_Progression_Y4.at(1)>=0)
                        {
                            pt_DiTau_Progression_Y4->Fill(pt_Taus_Progression_Y4.at(0),pt_Taus_Progression_Y4.at(1),weight);
                        }
            if(Index_Taus_Progression_Z4.at(0)>=0 && Index_Taus_Progression_Z4.at(1)>=0)
                        {
                            pt_DiTau_Progression_Z4->Fill(pt_Taus_Progression_Z4.at(0),pt_Taus_Progression_Z4.at(1),weight);
                        }
            if(Index_Taus_Progression_A5.at(0)>=0 && Index_Taus_Progression_A5.at(1)>=0)
                        {
                            pt_DiTau_Progression_A5->Fill(pt_Taus_Progression_A5.at(0),pt_Taus_Progression_A5.at(1),weight);
                        }
            if(Index_Taus_Progression_B5.at(0)>=0 && Index_Taus_Progression_B5.at(1)>=0)
                        {
                            pt_DiTau_Progression_B5->Fill(pt_Taus_Progression_B5.at(0),pt_Taus_Progression_B5.at(1),weight);
                        }
            if(Index_Taus_Progression_C5.at(0)>=0 && Index_Taus_Progression_C5.at(1)>=0)
                        {
                            pt_DiTau_Progression_C5->Fill(pt_Taus_Progression_C5.at(0),pt_Taus_Progression_C5.at(1),weight);
                        }
            if(Index_Taus_Progression_D5.at(0)>=0 && Index_Taus_Progression_D5.at(1)>=0)
                        {
                            pt_DiTau_Progression_D5->Fill(pt_Taus_Progression_D5.at(0),pt_Taus_Progression_D5.at(1),weight);
                        }
            if(Index_Taus_Progression_E5.at(0)>=0 && Index_Taus_Progression_E5.at(1)>=0)
                        {
                            pt_DiTau_Progression_E5->Fill(pt_Taus_Progression_E5.at(0),pt_Taus_Progression_E5.at(1),weight);
                        }
            if(Index_Taus_Progression_F5.at(0)>=0 && Index_Taus_Progression_F5.at(1)>=0)
                        {
                            pt_DiTau_Progression_F5->Fill(pt_Taus_Progression_F5.at(0),pt_Taus_Progression_F5.at(1),weight);
                        }
            if(Index_Taus_Progression_G5.at(0)>=0 && Index_Taus_Progression_G5.at(1)>=0)
                        {
                            pt_DiTau_Progression_G5->Fill(pt_Taus_Progression_G5.at(0),pt_Taus_Progression_G5.at(1),weight);
                        }
            if(Index_Taus_Progression_H5.at(0)>=0 && Index_Taus_Progression_H5.at(1)>=0)
                        {
                            pt_DiTau_Progression_H5->Fill(pt_Taus_Progression_H5.at(0),pt_Taus_Progression_H5.at(1),weight);
                        }
            if(Index_Taus_Progression_I5.at(0)>=0 && Index_Taus_Progression_I5.at(1)>=0)
                        {
                            pt_DiTau_Progression_I5->Fill(pt_Taus_Progression_I5.at(0),pt_Taus_Progression_I5.at(1),weight);
                        }
            if(Index_Taus_Progression_J5.at(0)>=0 && Index_Taus_Progression_J5.at(1)>=0)
                        {
                            pt_DiTau_Progression_J5->Fill(pt_Taus_Progression_J5.at(0),pt_Taus_Progression_J5.at(1),weight);
                        }
            if(Index_Taus_Progression_K5.at(0)>=0 && Index_Taus_Progression_K5.at(1)>=0)
                        {
                            pt_DiTau_Progression_K5->Fill(pt_Taus_Progression_K5.at(0),pt_Taus_Progression_K5.at(1),weight);
                        }
            if(Index_Taus_Progression_L5.at(0)>=0 && Index_Taus_Progression_L5.at(1)>=0)
                        {
                            pt_DiTau_Progression_L5->Fill(pt_Taus_Progression_L5.at(0),pt_Taus_Progression_L5.at(1),weight);
                        }
            if(Index_Taus_Progression_M5.at(0)>=0 && Index_Taus_Progression_M5.at(1)>=0)
                        {
                            pt_DiTau_Progression_M5->Fill(pt_Taus_Progression_M5.at(0),pt_Taus_Progression_M5.at(1),weight);
                        }
            if(Index_Taus_Progression_N5.at(0)>=0 && Index_Taus_Progression_N5.at(1)>=0)
                        {
                            pt_DiTau_Progression_N5->Fill(pt_Taus_Progression_N5.at(0),pt_Taus_Progression_N5.at(1),weight);
                        }
            if(Index_Taus_Progression_O5.at(0)>=0 && Index_Taus_Progression_O5.at(1)>=0)
                        {
                            pt_DiTau_Progression_O5->Fill(pt_Taus_Progression_O5.at(0),pt_Taus_Progression_O5.at(1),weight);
                        }
            if(Index_Taus_Progression_P5.at(0)>=0 && Index_Taus_Progression_P5.at(1)>=0)
                        {
                            pt_DiTau_Progression_P5->Fill(pt_Taus_Progression_P5.at(0),pt_Taus_Progression_P5.at(1),weight);
                        }
            if(Index_Taus_Progression_Q5.at(0)>=0 && Index_Taus_Progression_Q5.at(1)>=0)
                        {
                            pt_DiTau_Progression_Q5->Fill(pt_Taus_Progression_Q5.at(0),pt_Taus_Progression_Q5.at(1),weight);
                        }
            if(Index_Taus_Progression_R5.at(0)>=0 && Index_Taus_Progression_R5.at(1)>=0)
                        {
                            pt_DiTau_Progression_R5->Fill(pt_Taus_Progression_R5.at(0),pt_Taus_Progression_R5.at(1),weight);
                        }
            if(Index_Taus_Progression_S5.at(0)>=0 && Index_Taus_Progression_S5.at(1)>=0)
                        {
                            pt_DiTau_Progression_S5->Fill(pt_Taus_Progression_S5.at(0),pt_Taus_Progression_S5.at(1),weight);
                        }
            if(Index_Taus_Progression_T5.at(0)>=0 && Index_Taus_Progression_T5.at(1)>=0)
                        {
                            pt_DiTau_Progression_T5->Fill(pt_Taus_Progression_T5.at(0),pt_Taus_Progression_T5.at(1),weight);
                        }
            if(Index_Taus_Progression_U5.at(0)>=0 && Index_Taus_Progression_U5.at(1)>=0)
                        {
                            pt_DiTau_Progression_U5->Fill(pt_Taus_Progression_U5.at(0),pt_Taus_Progression_U5.at(1),weight);
                        }
            if(Index_Taus_Progression_V5.at(0)>=0 && Index_Taus_Progression_V5.at(1)>=0)
                        {
                            pt_DiTau_Progression_V5->Fill(pt_Taus_Progression_V5.at(0),pt_Taus_Progression_V5.at(1),weight);
                        }
            if(Index_Taus_Progression_W5.at(0)>=0 && Index_Taus_Progression_W5.at(1)>=0)
                        {
                            pt_DiTau_Progression_W5->Fill(pt_Taus_Progression_W5.at(0),pt_Taus_Progression_W5.at(1),weight);
                        }
            if(Index_Taus_Progression_X5.at(0)>=0 && Index_Taus_Progression_X5.at(1)>=0)
                        {
                            pt_DiTau_Progression_X5->Fill(pt_Taus_Progression_X5.at(0),pt_Taus_Progression_X5.at(1),weight);
                        }
            if(Index_Taus_Progression_Y5.at(0)>=0 && Index_Taus_Progression_Y5.at(1)>=0)
                        {
                            pt_DiTau_Progression_Y5->Fill(pt_Taus_Progression_Y5.at(0),pt_Taus_Progression_Y5.at(1),weight);
                        }
            if(Index_Taus_Progression_Z5.at(0)>=0 && Index_Taus_Progression_Z5.at(1)>=0)
                        {
                            pt_DiTau_Progression_Z5->Fill(pt_Taus_Progression_Z5.at(0),pt_Taus_Progression_Z5.at(1),weight);
                        }
            if(Index_Taus_Progression_A6.at(0)>=0 && Index_Taus_Progression_A6.at(1)>=0)
                        {
                            pt_DiTau_Progression_A6->Fill(pt_Taus_Progression_A6.at(0),pt_Taus_Progression_A6.at(1),weight);
                        }
            if(Index_Taus_Progression_B6.at(0)>=0 && Index_Taus_Progression_B6.at(1)>=0)
                        {
                            pt_DiTau_Progression_B6->Fill(pt_Taus_Progression_B6.at(0),pt_Taus_Progression_B6.at(1),weight);
                        }
            if(Index_Taus_Progression_C6.at(0)>=0 && Index_Taus_Progression_C6.at(1)>=0)
                        {
                            pt_DiTau_Progression_C6->Fill(pt_Taus_Progression_C6.at(0),pt_Taus_Progression_C6.at(1),weight);
                        }
            if(Index_Taus_Progression_D6.at(0)>=0 && Index_Taus_Progression_D6.at(1)>=0)
                        {
                            pt_DiTau_Progression_D6->Fill(pt_Taus_Progression_D6.at(0),pt_Taus_Progression_D6.at(1),weight);
                        }
            if(Index_Taus_Progression_E6.at(0)>=0 && Index_Taus_Progression_E6.at(1)>=0)
                        {
                            pt_DiTau_Progression_E6->Fill(pt_Taus_Progression_E6.at(0),pt_Taus_Progression_E6.at(1),weight);
                        }
            if(Index_Taus_Progression_F6.at(0)>=0 && Index_Taus_Progression_F6.at(1)>=0)
                        {
                            pt_DiTau_Progression_F6->Fill(pt_Taus_Progression_F6.at(0),pt_Taus_Progression_F6.at(1),weight);
                        }
            if(Index_Taus_Progression_G6.at(0)>=0 && Index_Taus_Progression_G6.at(1)>=0)
                        {
                            pt_DiTau_Progression_G6->Fill(pt_Taus_Progression_G6.at(0),pt_Taus_Progression_G6.at(1),weight);
                        }
            if(Index_Taus_Progression_H6.at(0)>=0 && Index_Taus_Progression_H6.at(1)>=0)
                        {
                            pt_DiTau_Progression_H6->Fill(pt_Taus_Progression_H6.at(0),pt_Taus_Progression_H6.at(1),weight);
                        }
            if(Index_Taus_Progression_I6.at(0)>=0 && Index_Taus_Progression_I6.at(1)>=0)
                        {
                            pt_DiTau_Progression_I6->Fill(pt_Taus_Progression_I6.at(0),pt_Taus_Progression_I6.at(1),weight);
                        }
            if(Index_Taus_Progression_J6.at(0)>=0 && Index_Taus_Progression_J6.at(1)>=0)
                        {
                            pt_DiTau_Progression_J6->Fill(pt_Taus_Progression_J6.at(0),pt_Taus_Progression_J6.at(1),weight);
                        }
            if(Index_Taus_Progression_K6.at(0)>=0 && Index_Taus_Progression_K6.at(1)>=0)
                        {
                            pt_DiTau_Progression_K6->Fill(pt_Taus_Progression_K6.at(0),pt_Taus_Progression_K6.at(1),weight);
                        }
            if(Index_Taus_Progression_L6.at(0)>=0 && Index_Taus_Progression_L6.at(1)>=0)
                        {
                            pt_DiTau_Progression_L6->Fill(pt_Taus_Progression_L6.at(0),pt_Taus_Progression_L6.at(1),weight);
                        }
            if(Index_Taus_Progression_M6.at(0)>=0 && Index_Taus_Progression_M6.at(1)>=0)
                        {
                            pt_DiTau_Progression_M6->Fill(pt_Taus_Progression_M6.at(0),pt_Taus_Progression_M6.at(1),weight);
                        }
            if(Index_Taus_Progression_N6.at(0)>=0 && Index_Taus_Progression_N6.at(1)>=0)
                        {
                            pt_DiTau_Progression_N6->Fill(pt_Taus_Progression_N6.at(0),pt_Taus_Progression_N6.at(1),weight);
                        }
            if(Index_Taus_Progression_O6.at(0)>=0 && Index_Taus_Progression_O6.at(1)>=0)
                        {
                            pt_DiTau_Progression_O6->Fill(pt_Taus_Progression_O6.at(0),pt_Taus_Progression_O6.at(1),weight);
                        }
            if(Index_Taus_Progression_P6.at(0)>=0 && Index_Taus_Progression_P6.at(1)>=0)
                        {
                            pt_DiTau_Progression_P6->Fill(pt_Taus_Progression_P6.at(0),pt_Taus_Progression_P6.at(1),weight);
                        }
            if(Index_Taus_Progression_Q6.at(0)>=0 && Index_Taus_Progression_Q6.at(1)>=0)
                        {
                            pt_DiTau_Progression_Q6->Fill(pt_Taus_Progression_Q6.at(0),pt_Taus_Progression_Q6.at(1),weight);
                        }
            if(Index_Taus_Progression_R6.at(0)>=0 && Index_Taus_Progression_R6.at(1)>=0)
                        {
                            pt_DiTau_Progression_R6->Fill(pt_Taus_Progression_R6.at(0),pt_Taus_Progression_R6.at(1),weight);
                        }
            if(Index_Taus_Progression_S6.at(0)>=0 && Index_Taus_Progression_S6.at(1)>=0)
                        {
                            pt_DiTau_Progression_S6->Fill(pt_Taus_Progression_S6.at(0),pt_Taus_Progression_S6.at(1),weight);
                        }
            if(Index_Taus_Progression_T6.at(0)>=0 && Index_Taus_Progression_T6.at(1)>=0)
                        {
                            pt_DiTau_Progression_T6->Fill(pt_Taus_Progression_T6.at(0),pt_Taus_Progression_T6.at(1),weight);
                        }
            if(Index_Taus_Progression_U6.at(0)>=0 && Index_Taus_Progression_U6.at(1)>=0)
                        {
                            pt_DiTau_Progression_U6->Fill(pt_Taus_Progression_U6.at(0),pt_Taus_Progression_U6.at(1),weight);
                        }
            if(Index_Taus_Progression_V6.at(0)>=0 && Index_Taus_Progression_V6.at(1)>=0)
                        {
                            pt_DiTau_Progression_V6->Fill(pt_Taus_Progression_V6.at(0),pt_Taus_Progression_V6.at(1),weight);
                        }
            if(Index_Taus_Progression_W6.at(0)>=0 && Index_Taus_Progression_W6.at(1)>=0)
                        {
                            pt_DiTau_Progression_W6->Fill(pt_Taus_Progression_W6.at(0),pt_Taus_Progression_W6.at(1),weight);
                        }
            if(Index_Taus_Progression_X6.at(0)>=0 && Index_Taus_Progression_X6.at(1)>=0)
                        {
                            pt_DiTau_Progression_X6->Fill(pt_Taus_Progression_X6.at(0),pt_Taus_Progression_X6.at(1),weight);
                        }
            if(Index_Taus_Progression_Y6.at(0)>=0 && Index_Taus_Progression_Y6.at(1)>=0)
                        {
                            pt_DiTau_Progression_Y6->Fill(pt_Taus_Progression_Y6.at(0),pt_Taus_Progression_Y6.at(1),weight);
                        }
            if(Index_Taus_Progression_Z6.at(0)>=0 && Index_Taus_Progression_Z6.at(1)>=0)
                        {
                            pt_DiTau_Progression_Z6->Fill(pt_Taus_Progression_Z6.at(0),pt_Taus_Progression_Z6.at(1),weight);
                        }
            if(Index_Taus_Progression_A7.at(0)>=0 && Index_Taus_Progression_A7.at(1)>=0)
                        {
                            pt_DiTau_Progression_A7->Fill(pt_Taus_Progression_A7.at(0),pt_Taus_Progression_A7.at(1),weight);
                        }
            if(Index_Taus_Progression_B7.at(0)>=0 && Index_Taus_Progression_B7.at(1)>=0)
                        {
                            pt_DiTau_Progression_B7->Fill(pt_Taus_Progression_B7.at(0),pt_Taus_Progression_B7.at(1),weight);
                        }
            if(Index_Taus_Progression_C7.at(0)>=0 && Index_Taus_Progression_C7.at(1)>=0)
                        {
                            pt_DiTau_Progression_C7->Fill(pt_Taus_Progression_C7.at(0),pt_Taus_Progression_C7.at(1),weight);
                        }
            if(Index_Taus_Progression_D7.at(0)>=0 && Index_Taus_Progression_D7.at(1)>=0)
                        {
                            pt_DiTau_Progression_D7->Fill(pt_Taus_Progression_D7.at(0),pt_Taus_Progression_D7.at(1),weight);
                        }
            if(Index_Taus_Progression_E7.at(0)>=0 && Index_Taus_Progression_E7.at(1)>=0)
                        {
                            pt_DiTau_Progression_E7->Fill(pt_Taus_Progression_E7.at(0),pt_Taus_Progression_E7.at(1),weight);
                        }
            if(Index_Taus_Progression_F7.at(0)>=0 && Index_Taus_Progression_F7.at(1)>=0)
                        {
                            pt_DiTau_Progression_F7->Fill(pt_Taus_Progression_F7.at(0),pt_Taus_Progression_F7.at(1),weight);
                        }
            if(Index_Taus_Progression_G7.at(0)>=0 && Index_Taus_Progression_G7.at(1)>=0)
                        {
                            pt_DiTau_Progression_G7->Fill(pt_Taus_Progression_G7.at(0),pt_Taus_Progression_G7.at(1),weight);
                        }
            if(Index_Taus_Progression_H7.at(0)>=0 && Index_Taus_Progression_H7.at(1)>=0)
                        {
                            pt_DiTau_Progression_H7->Fill(pt_Taus_Progression_H7.at(0),pt_Taus_Progression_H7.at(1),weight);
                        }
            if(Index_Taus_Progression_I7.at(0)>=0 && Index_Taus_Progression_I7.at(1)>=0)
                        {
                            pt_DiTau_Progression_I7->Fill(pt_Taus_Progression_I7.at(0),pt_Taus_Progression_I7.at(1),weight);
                        }
            if(Index_Taus_Progression_J7.at(0)>=0 && Index_Taus_Progression_J7.at(1)>=0)
                        {
                            pt_DiTau_Progression_J7->Fill(pt_Taus_Progression_J7.at(0),pt_Taus_Progression_J7.at(1),weight);
                        }
            if(Index_Taus_Progression_K7.at(0)>=0 && Index_Taus_Progression_K7.at(1)>=0)
                        {
                            pt_DiTau_Progression_K7->Fill(pt_Taus_Progression_K7.at(0),pt_Taus_Progression_K7.at(1),weight);
                        }
            if(Index_Taus_Progression_L7.at(0)>=0 && Index_Taus_Progression_L7.at(1)>=0)
                        {
                            pt_DiTau_Progression_L7->Fill(pt_Taus_Progression_L7.at(0),pt_Taus_Progression_L7.at(1),weight);
                        }
            if(Index_Taus_Progression_M7.at(0)>=0 && Index_Taus_Progression_M7.at(1)>=0)
                        {
                            pt_DiTau_Progression_M7->Fill(pt_Taus_Progression_M7.at(0),pt_Taus_Progression_M7.at(1),weight);
                        }
            if(Index_Taus_Progression_N7.at(0)>=0 && Index_Taus_Progression_N7.at(1)>=0)
                        {
                            pt_DiTau_Progression_N7->Fill(pt_Taus_Progression_N7.at(0),pt_Taus_Progression_N7.at(1),weight);
                        }
            if(Index_Taus_Progression_O7.at(0)>=0 && Index_Taus_Progression_O7.at(1)>=0)
                        {
                            pt_DiTau_Progression_O7->Fill(pt_Taus_Progression_O7.at(0),pt_Taus_Progression_O7.at(1),weight);
                        }
            if(Index_Taus_Progression_P7.at(0)>=0 && Index_Taus_Progression_P7.at(1)>=0)
                        {
                            pt_DiTau_Progression_P7->Fill(pt_Taus_Progression_P7.at(0),pt_Taus_Progression_P7.at(1),weight);
                        }
            if(Index_Taus_Progression_Q7.at(0)>=0 && Index_Taus_Progression_Q7.at(1)>=0)
                        {
                            pt_DiTau_Progression_Q7->Fill(pt_Taus_Progression_Q7.at(0),pt_Taus_Progression_Q7.at(1),weight);
                        }
            if(Index_Taus_Progression_R7.at(0)>=0 && Index_Taus_Progression_R7.at(1)>=0)
                        {
                            pt_DiTau_Progression_R7->Fill(pt_Taus_Progression_R7.at(0),pt_Taus_Progression_R7.at(1),weight);
                        }
            if(Index_Taus_Progression_S7.at(0)>=0 && Index_Taus_Progression_S7.at(1)>=0)
                        {
                            pt_DiTau_Progression_S7->Fill(pt_Taus_Progression_S7.at(0),pt_Taus_Progression_S7.at(1),weight);
                        }
            if(Index_Taus_Progression_T7.at(0)>=0 && Index_Taus_Progression_T7.at(1)>=0)
                        {
                            pt_DiTau_Progression_T7->Fill(pt_Taus_Progression_T7.at(0),pt_Taus_Progression_T7.at(1),weight);
                        }
            if(Index_Taus_Progression_U7.at(0)>=0 && Index_Taus_Progression_U7.at(1)>=0)
                        {
                            pt_DiTau_Progression_U7->Fill(pt_Taus_Progression_U7.at(0),pt_Taus_Progression_U7.at(1),weight);
                        }
            if(Index_Taus_Progression_V7.at(0)>=0 && Index_Taus_Progression_V7.at(1)>=0)
                        {
                            pt_DiTau_Progression_V7->Fill(pt_Taus_Progression_V7.at(0),pt_Taus_Progression_V7.at(1),weight);
                        }
            if(Index_Taus_Progression_W7.at(0)>=0 && Index_Taus_Progression_W7.at(1)>=0)
                        {
                            pt_DiTau_Progression_W7->Fill(pt_Taus_Progression_W7.at(0),pt_Taus_Progression_W7.at(1),weight);
                        }
            if(Index_Taus_Progression_X7.at(0)>=0 && Index_Taus_Progression_X7.at(1)>=0)
                        {
                            pt_DiTau_Progression_X7->Fill(pt_Taus_Progression_X7.at(0),pt_Taus_Progression_X7.at(1),weight);
                        }
            if(Index_Taus_Progression_Y7.at(0)>=0 && Index_Taus_Progression_Y7.at(1)>=0)
                        {
                            pt_DiTau_Progression_Y7->Fill(pt_Taus_Progression_Y7.at(0),pt_Taus_Progression_Y7.at(1),weight);
                        }
            if(Index_Taus_Progression_Z7.at(0)>=0 && Index_Taus_Progression_Z7.at(1)>=0)
                        {
                            pt_DiTau_Progression_Z7->Fill(pt_Taus_Progression_Z7.at(0),pt_Taus_Progression_Z7.at(1),weight);
                        }
            if(Index_Taus_Progression_A8.at(0)>=0 && Index_Taus_Progression_A8.at(1)>=0)
                        {
                            pt_DiTau_Progression_A8->Fill(pt_Taus_Progression_A8.at(0),pt_Taus_Progression_A8.at(1),weight);
                        }
            if(Index_Taus_Progression_B8.at(0)>=0 && Index_Taus_Progression_B8.at(1)>=0)
                        {
                            pt_DiTau_Progression_B8->Fill(pt_Taus_Progression_B8.at(0),pt_Taus_Progression_B8.at(1),weight);
                        }
            if(Index_Taus_Progression_C8.at(0)>=0 && Index_Taus_Progression_C8.at(1)>=0)
                        {
                            pt_DiTau_Progression_C8->Fill(pt_Taus_Progression_C8.at(0),pt_Taus_Progression_C8.at(1),weight);
                        }
            if(Index_Taus_Progression_D8.at(0)>=0 && Index_Taus_Progression_D8.at(1)>=0)
                        {
                            pt_DiTau_Progression_D8->Fill(pt_Taus_Progression_D8.at(0),pt_Taus_Progression_D8.at(1),weight);
                        }
            if(Index_Taus_Progression_E8.at(0)>=0 && Index_Taus_Progression_E8.at(1)>=0)
                        {
                            pt_DiTau_Progression_E8->Fill(pt_Taus_Progression_E8.at(0),pt_Taus_Progression_E8.at(1),weight);
                        }
            if(Index_Taus_Progression_F8.at(0)>=0 && Index_Taus_Progression_F8.at(1)>=0)
                        {
                            pt_DiTau_Progression_F8->Fill(pt_Taus_Progression_F8.at(0),pt_Taus_Progression_F8.at(1),weight);
                        }
            if(Index_Taus_Progression_G8.at(0)>=0 && Index_Taus_Progression_G8.at(1)>=0)
                        {
                            pt_DiTau_Progression_G8->Fill(pt_Taus_Progression_G8.at(0),pt_Taus_Progression_G8.at(1),weight);
                        }
            if(Index_Taus_Progression_H8.at(0)>=0 && Index_Taus_Progression_H8.at(1)>=0)
                        {
                            pt_DiTau_Progression_H8->Fill(pt_Taus_Progression_H8.at(0),pt_Taus_Progression_H8.at(1),weight);
                        }
            if(Index_Taus_Progression_I8.at(0)>=0 && Index_Taus_Progression_I8.at(1)>=0)
                        {
                            pt_DiTau_Progression_I8->Fill(pt_Taus_Progression_I8.at(0),pt_Taus_Progression_I8.at(1),weight);
                        }
            if(Index_Taus_Progression_J8.at(0)>=0 && Index_Taus_Progression_J8.at(1)>=0)
                        {
                            pt_DiTau_Progression_J8->Fill(pt_Taus_Progression_J8.at(0),pt_Taus_Progression_J8.at(1),weight);
                        }
            if(Index_Taus_Progression_K8.at(0)>=0 && Index_Taus_Progression_K8.at(1)>=0)
                        {
                            pt_DiTau_Progression_K8->Fill(pt_Taus_Progression_K8.at(0),pt_Taus_Progression_K8.at(1),weight);
                        }
            if(Index_Taus_Progression_L8.at(0)>=0 && Index_Taus_Progression_L8.at(1)>=0)
                        {
                            pt_DiTau_Progression_L8->Fill(pt_Taus_Progression_L8.at(0),pt_Taus_Progression_L8.at(1),weight);
                        }
            if(Index_Taus_Progression_M8.at(0)>=0 && Index_Taus_Progression_M8.at(1)>=0)
                        {
                            pt_DiTau_Progression_M8->Fill(pt_Taus_Progression_M8.at(0),pt_Taus_Progression_M8.at(1),weight);
                        }
            if(Index_Taus_Progression_N8.at(0)>=0 && Index_Taus_Progression_N8.at(1)>=0)
                        {
                            pt_DiTau_Progression_N8->Fill(pt_Taus_Progression_N8.at(0),pt_Taus_Progression_N8.at(1),weight);
                        }
            if(Index_Taus_Progression_O8.at(0)>=0 && Index_Taus_Progression_O8.at(1)>=0)
                        {
                            pt_DiTau_Progression_O8->Fill(pt_Taus_Progression_O8.at(0),pt_Taus_Progression_O8.at(1),weight);
                        }
            if(Index_Taus_Progression_P8.at(0)>=0 && Index_Taus_Progression_P8.at(1)>=0)
                        {
                            pt_DiTau_Progression_P8->Fill(pt_Taus_Progression_P8.at(0),pt_Taus_Progression_P8.at(1),weight);
                        }
            if(Index_Taus_Progression_Q8.at(0)>=0 && Index_Taus_Progression_Q8.at(1)>=0)
                        {
                            pt_DiTau_Progression_Q8->Fill(pt_Taus_Progression_Q8.at(0),pt_Taus_Progression_Q8.at(1),weight);
                        }
            if(Index_Taus_Progression_R8.at(0)>=0 && Index_Taus_Progression_R8.at(1)>=0)
                        {
                            pt_DiTau_Progression_R8->Fill(pt_Taus_Progression_R8.at(0),pt_Taus_Progression_R8.at(1),weight);
                        }
            if(Index_Taus_Progression_S8.at(0)>=0 && Index_Taus_Progression_S8.at(1)>=0)
                        {
                            pt_DiTau_Progression_S8->Fill(pt_Taus_Progression_S8.at(0),pt_Taus_Progression_S8.at(1),weight);
                        }
            if(Index_Taus_Progression_T8.at(0)>=0 && Index_Taus_Progression_T8.at(1)>=0)
                        {
                            pt_DiTau_Progression_T8->Fill(pt_Taus_Progression_T8.at(0),pt_Taus_Progression_T8.at(1),weight);
                        }
            if(Index_Taus_Progression_U8.at(0)>=0 && Index_Taus_Progression_U8.at(1)>=0)
                        {
                            pt_DiTau_Progression_U8->Fill(pt_Taus_Progression_U8.at(0),pt_Taus_Progression_U8.at(1),weight);
                        }
            if(Index_Taus_Progression_V8.at(0)>=0 && Index_Taus_Progression_V8.at(1)>=0)
                        {
                            pt_DiTau_Progression_V8->Fill(pt_Taus_Progression_V8.at(0),pt_Taus_Progression_V8.at(1),weight);
                        }
            if(Index_Taus_Progression_W8.at(0)>=0 && Index_Taus_Progression_W8.at(1)>=0)
                        {
                            pt_DiTau_Progression_W8->Fill(pt_Taus_Progression_W8.at(0),pt_Taus_Progression_W8.at(1),weight);
                        }
            if(Index_Taus_Progression_X8.at(0)>=0 && Index_Taus_Progression_X8.at(1)>=0)
                        {
                            pt_DiTau_Progression_X8->Fill(pt_Taus_Progression_X8.at(0),pt_Taus_Progression_X8.at(1),weight);
                        }
            if(Index_Taus_Progression_Y8.at(0)>=0 && Index_Taus_Progression_Y8.at(1)>=0)
                        {
                            pt_DiTau_Progression_Y8->Fill(pt_Taus_Progression_Y8.at(0),pt_Taus_Progression_Y8.at(1),weight);
                        }
            if(Index_Taus_Progression_Z8.at(0)>=0 && Index_Taus_Progression_Z8.at(1)>=0)
                        {
                            pt_DiTau_Progression_Z8->Fill(pt_Taus_Progression_Z8.at(0),pt_Taus_Progression_Z8.at(1),weight);
                        }
            // END OF FULL BLOWN GRID SEARCH
        }

    cout<<"CounterAll = "<<CounterAll<<endl;
    cout<<"CounterPass = "<<CounterPass<<endl;
    cout<<"CounterFail = "<<CounterFail<<endl;

    cout<<"DiTauCounterAll = "<<DiTauCounterAll<<endl;
    cout<<"DiTauCounterPass = "<<DiTauCounterPass<<endl;

    // SET RUN INFO
    float nb = 2544.; // Run323755 and Run323775
    float thisLumiRun = 0.;
    if (run == 323755) thisLumiRun = 1.6225E34; // Run323755
    if (run == 323775) thisLumiRun = 1.736E34; // Run323775 --> very smal numer of lumisections!!
    if (thisLumiRun == 0.)
    {
    std::cout << "ERROR: something went wrong with the run selection and the lumi initialization" << std::endl;
    return;
    }
    float scaleToLumi = 2.00E34;
    float scale = 0.001*(nb*11245.6)*scaleToLumi/thisLumiRun;

    // float nb = 2064;
    // float thisLumiRun = 12118.9E30;
    // float scaleToLumi = 1.15E34;
    // float scale = 0.001*(nb*11245.6)*scaleToLumi/thisLumiRun;

    // float scale = 0.001*(517.*11245.6);
    cout<<"Denominator = "<<Denominator<<endl;
    // float scale = 0.001*(2232.*11245.6);
    // rate_IsoInf->Scale(scale/dataL1Upgrade.GetEntries());
    // rate_Iso_50->Scale(scale/dataL1Upgrade.GetEntries());
    // rate_Iso_60->Scale(scale/dataL1Upgrade.GetEntries());
    // rate_Iso_70->Scale(scale/dataL1Upgrade.GetEntries());

    TH1F* rate_noCut = new TH1F("rate_noCut","rate_noCut",240,0.,240.);
    TH1F* rate_IsoCut_50 = new TH1F("rate_IsoCut_50","rate_IsoCut_50",240,0.,240.);
    TH1F* rate_IsoCut_60 = new TH1F("rate_IsoCut_60","rate_IsoCut_60",240,0.,240.);
    TH1F* rate_IsoCut_70 = new TH1F("rate_IsoCut_70","rate_IsoCut_70",240,0.,240.);
    TH1F* rate_IsoCut_80 = new TH1F("rate_IsoCut_80","rate_IsoCut_80",240,0.,240.);
    TH1F* rate_IsoCut_90 = new TH1F("rate_IsoCut_90","rate_IsoCut_90",240,0.,240.);
    TH1F* rate_Stage1 = new TH1F("rate_Stage1","rate_Stage1",240,0.,240.);
    TH1F* rate_Progression_1 = new TH1F("rate_Progression_1","rate_Progression_1",240,0.,240.);
    TH1F* rate_Progression_2 = new TH1F("rate_Progression_2","rate_Progression_2",240,0.,240.);
    TH1F* rate_Progression_3 = new TH1F("rate_Progression_3","rate_Progression_3",240,0.,240.);
    TH1F* rate_Progression_4 = new TH1F("rate_Progression_4","rate_Progression_4",240,0.,240.);
    TH1F* rate_Progression_5 = new TH1F("rate_Progression_5","rate_Progression_5",240,0.,240.);
    TH1F* rate_Progression_6 = new TH1F("rate_Progression_6","rate_Progression_6",240,0.,240.);
    TH1F* rate_Progression_7 = new TH1F("rate_Progression_7","rate_Progression_7",240,0.,240.);
    TH1F* rate_Progression_8 = new TH1F("rate_Progression_8","rate_Progression_8",240,0.,240.);
    TH1F* rate_Progression_9 = new TH1F("rate_Progression_9","rate_Progression_9",240,0.,240.);
    TH1F* rate_Progression_10 = new TH1F("rate_Progression_10","rate_Progression_10",240,0.,240.);
    TH1F* rate_Progression_11 = new TH1F("rate_Progression_11","rate_Progression_11",240,0.,240.);
    TH1F* rate_Progression_12 = new TH1F("rate_Progression_12","rate_Progression_12",240,0.,240.);
    TH1F* rate_Progression_13 = new TH1F("rate_Progression_13","rate_Progression_13",240,0.,240.);
    TH1F* rate_Progression_14 = new TH1F("rate_Progression_14","rate_Progression_14",240,0.,240.);
    TH1F* rate_Progression_15 = new TH1F("rate_Progression_15","rate_Progression_15",240,0.,240.);
    TH1F* rate_Progression_16 = new TH1F("rate_Progression_16","rate_Progression_16",240,0.,240.);
    TH1F* rate_Progression_17 = new TH1F("rate_Progression_17","rate_Progression_17",240,0.,240.);
    TH1F* rate_Progression_18 = new TH1F("rate_Progression_18","rate_Progression_18",240,0.,240.);
    TH1F* rate_Progression_19 = new TH1F("rate_Progression_19","rate_Progression_19",240,0.,240.);
    TH1F* rate_Progression_20 = new TH1F("rate_Progression_20","rate_Progression_20",240,0.,240.);
    TH1F* rate_Progression_21 = new TH1F("rate_Progression_21","rate_Progression_21",240,0.,240.);
    TH1F* rate_Progression_22 = new TH1F("rate_Progression_22","rate_Progression_22",240,0.,240.);

    // START OF FULL BLOWN GRID SEARCH
    TH1F* rate_Progression_A0 = new TH1F("rate_Progression_A0","rate_Progression_A0",240,0.,240.);
    TH1F* rate_Progression_B0 = new TH1F("rate_Progression_B0","rate_Progression_B0",240,0.,240.);
    TH1F* rate_Progression_C0 = new TH1F("rate_Progression_C0","rate_Progression_C0",240,0.,240.);
    TH1F* rate_Progression_D0 = new TH1F("rate_Progression_D0","rate_Progression_D0",240,0.,240.);
    TH1F* rate_Progression_E0 = new TH1F("rate_Progression_E0","rate_Progression_E0",240,0.,240.);
    TH1F* rate_Progression_F0 = new TH1F("rate_Progression_F0","rate_Progression_F0",240,0.,240.);
    TH1F* rate_Progression_G0 = new TH1F("rate_Progression_G0","rate_Progression_G0",240,0.,240.);
    TH1F* rate_Progression_H0 = new TH1F("rate_Progression_H0","rate_Progression_H0",240,0.,240.);
    TH1F* rate_Progression_I0 = new TH1F("rate_Progression_I0","rate_Progression_I0",240,0.,240.);
    TH1F* rate_Progression_J0 = new TH1F("rate_Progression_J0","rate_Progression_J0",240,0.,240.);
    TH1F* rate_Progression_K0 = new TH1F("rate_Progression_K0","rate_Progression_K0",240,0.,240.);
    TH1F* rate_Progression_L0 = new TH1F("rate_Progression_L0","rate_Progression_L0",240,0.,240.);
    TH1F* rate_Progression_M0 = new TH1F("rate_Progression_M0","rate_Progression_M0",240,0.,240.);
    TH1F* rate_Progression_N0 = new TH1F("rate_Progression_N0","rate_Progression_N0",240,0.,240.);
    TH1F* rate_Progression_O0 = new TH1F("rate_Progression_O0","rate_Progression_O0",240,0.,240.);
    TH1F* rate_Progression_P0 = new TH1F("rate_Progression_P0","rate_Progression_P0",240,0.,240.);
    TH1F* rate_Progression_Q0 = new TH1F("rate_Progression_Q0","rate_Progression_Q0",240,0.,240.);
    TH1F* rate_Progression_R0 = new TH1F("rate_Progression_R0","rate_Progression_R0",240,0.,240.);
    TH1F* rate_Progression_S0 = new TH1F("rate_Progression_S0","rate_Progression_S0",240,0.,240.);
    TH1F* rate_Progression_T0 = new TH1F("rate_Progression_T0","rate_Progression_T0",240,0.,240.);
    TH1F* rate_Progression_U0 = new TH1F("rate_Progression_U0","rate_Progression_U0",240,0.,240.);
    TH1F* rate_Progression_V0 = new TH1F("rate_Progression_V0","rate_Progression_V0",240,0.,240.);
    TH1F* rate_Progression_W0 = new TH1F("rate_Progression_W0","rate_Progression_W0",240,0.,240.);
    TH1F* rate_Progression_X0 = new TH1F("rate_Progression_X0","rate_Progression_X0",240,0.,240.);
    TH1F* rate_Progression_Y0 = new TH1F("rate_Progression_Y0","rate_Progression_Y0",240,0.,240.);
    TH1F* rate_Progression_Z0 = new TH1F("rate_Progression_Z0","rate_Progression_Z0",240,0.,240.);
    TH1F* rate_Progression_A1 = new TH1F("rate_Progression_A1","rate_Progression_A1",240,0.,240.);
    TH1F* rate_Progression_B1 = new TH1F("rate_Progression_B1","rate_Progression_B1",240,0.,240.);
    TH1F* rate_Progression_C1 = new TH1F("rate_Progression_C1","rate_Progression_C1",240,0.,240.);
    TH1F* rate_Progression_D1 = new TH1F("rate_Progression_D1","rate_Progression_D1",240,0.,240.);
    TH1F* rate_Progression_E1 = new TH1F("rate_Progression_E1","rate_Progression_E1",240,0.,240.);
    TH1F* rate_Progression_F1 = new TH1F("rate_Progression_F1","rate_Progression_F1",240,0.,240.);
    TH1F* rate_Progression_G1 = new TH1F("rate_Progression_G1","rate_Progression_G1",240,0.,240.);
    TH1F* rate_Progression_H1 = new TH1F("rate_Progression_H1","rate_Progression_H1",240,0.,240.);
    TH1F* rate_Progression_I1 = new TH1F("rate_Progression_I1","rate_Progression_I1",240,0.,240.);
    TH1F* rate_Progression_J1 = new TH1F("rate_Progression_J1","rate_Progression_J1",240,0.,240.);
    TH1F* rate_Progression_K1 = new TH1F("rate_Progression_K1","rate_Progression_K1",240,0.,240.);
    TH1F* rate_Progression_L1 = new TH1F("rate_Progression_L1","rate_Progression_L1",240,0.,240.);
    TH1F* rate_Progression_M1 = new TH1F("rate_Progression_M1","rate_Progression_M1",240,0.,240.);
    TH1F* rate_Progression_N1 = new TH1F("rate_Progression_N1","rate_Progression_N1",240,0.,240.);
    TH1F* rate_Progression_O1 = new TH1F("rate_Progression_O1","rate_Progression_O1",240,0.,240.);
    TH1F* rate_Progression_P1 = new TH1F("rate_Progression_P1","rate_Progression_P1",240,0.,240.);
    TH1F* rate_Progression_Q1 = new TH1F("rate_Progression_Q1","rate_Progression_Q1",240,0.,240.);
    TH1F* rate_Progression_R1 = new TH1F("rate_Progression_R1","rate_Progression_R1",240,0.,240.);
    TH1F* rate_Progression_S1 = new TH1F("rate_Progression_S1","rate_Progression_S1",240,0.,240.);
    TH1F* rate_Progression_T1 = new TH1F("rate_Progression_T1","rate_Progression_T1",240,0.,240.);
    TH1F* rate_Progression_U1 = new TH1F("rate_Progression_U1","rate_Progression_U1",240,0.,240.);
    TH1F* rate_Progression_V1 = new TH1F("rate_Progression_V1","rate_Progression_V1",240,0.,240.);
    TH1F* rate_Progression_W1 = new TH1F("rate_Progression_W1","rate_Progression_W1",240,0.,240.);
    TH1F* rate_Progression_X1 = new TH1F("rate_Progression_X1","rate_Progression_X1",240,0.,240.);
    TH1F* rate_Progression_Y1 = new TH1F("rate_Progression_Y1","rate_Progression_Y1",240,0.,240.);
    TH1F* rate_Progression_Z1 = new TH1F("rate_Progression_Z1","rate_Progression_Z1",240,0.,240.);
    TH1F* rate_Progression_A2 = new TH1F("rate_Progression_A2","rate_Progression_A2",240,0.,240.);
    TH1F* rate_Progression_B2 = new TH1F("rate_Progression_B2","rate_Progression_B2",240,0.,240.);
    TH1F* rate_Progression_C2 = new TH1F("rate_Progression_C2","rate_Progression_C2",240,0.,240.);
    TH1F* rate_Progression_D2 = new TH1F("rate_Progression_D2","rate_Progression_D2",240,0.,240.);
    TH1F* rate_Progression_E2 = new TH1F("rate_Progression_E2","rate_Progression_E2",240,0.,240.);
    TH1F* rate_Progression_F2 = new TH1F("rate_Progression_F2","rate_Progression_F2",240,0.,240.);
    TH1F* rate_Progression_G2 = new TH1F("rate_Progression_G2","rate_Progression_G2",240,0.,240.);
    TH1F* rate_Progression_H2 = new TH1F("rate_Progression_H2","rate_Progression_H2",240,0.,240.);
    TH1F* rate_Progression_I2 = new TH1F("rate_Progression_I2","rate_Progression_I2",240,0.,240.);
    TH1F* rate_Progression_J2 = new TH1F("rate_Progression_J2","rate_Progression_J2",240,0.,240.);
    TH1F* rate_Progression_K2 = new TH1F("rate_Progression_K2","rate_Progression_K2",240,0.,240.);
    TH1F* rate_Progression_L2 = new TH1F("rate_Progression_L2","rate_Progression_L2",240,0.,240.);
    TH1F* rate_Progression_M2 = new TH1F("rate_Progression_M2","rate_Progression_M2",240,0.,240.);
    TH1F* rate_Progression_N2 = new TH1F("rate_Progression_N2","rate_Progression_N2",240,0.,240.);
    TH1F* rate_Progression_O2 = new TH1F("rate_Progression_O2","rate_Progression_O2",240,0.,240.);
    TH1F* rate_Progression_P2 = new TH1F("rate_Progression_P2","rate_Progression_P2",240,0.,240.);
    TH1F* rate_Progression_Q2 = new TH1F("rate_Progression_Q2","rate_Progression_Q2",240,0.,240.);
    TH1F* rate_Progression_R2 = new TH1F("rate_Progression_R2","rate_Progression_R2",240,0.,240.);
    TH1F* rate_Progression_S2 = new TH1F("rate_Progression_S2","rate_Progression_S2",240,0.,240.);
    TH1F* rate_Progression_T2 = new TH1F("rate_Progression_T2","rate_Progression_T2",240,0.,240.);
    TH1F* rate_Progression_U2 = new TH1F("rate_Progression_U2","rate_Progression_U2",240,0.,240.);
    TH1F* rate_Progression_V2 = new TH1F("rate_Progression_V2","rate_Progression_V2",240,0.,240.);
    TH1F* rate_Progression_W2 = new TH1F("rate_Progression_W2","rate_Progression_W2",240,0.,240.);
    TH1F* rate_Progression_X2 = new TH1F("rate_Progression_X2","rate_Progression_X2",240,0.,240.);
    TH1F* rate_Progression_Y2 = new TH1F("rate_Progression_Y2","rate_Progression_Y2",240,0.,240.);
    TH1F* rate_Progression_Z2 = new TH1F("rate_Progression_Z2","rate_Progression_Z2",240,0.,240.);
    TH1F* rate_Progression_A3 = new TH1F("rate_Progression_A3","rate_Progression_A3",240,0.,240.);
    TH1F* rate_Progression_B3 = new TH1F("rate_Progression_B3","rate_Progression_B3",240,0.,240.);
    TH1F* rate_Progression_C3 = new TH1F("rate_Progression_C3","rate_Progression_C3",240,0.,240.);
    TH1F* rate_Progression_D3 = new TH1F("rate_Progression_D3","rate_Progression_D3",240,0.,240.);
    TH1F* rate_Progression_E3 = new TH1F("rate_Progression_E3","rate_Progression_E3",240,0.,240.);
    TH1F* rate_Progression_F3 = new TH1F("rate_Progression_F3","rate_Progression_F3",240,0.,240.);
    TH1F* rate_Progression_G3 = new TH1F("rate_Progression_G3","rate_Progression_G3",240,0.,240.);
    TH1F* rate_Progression_H3 = new TH1F("rate_Progression_H3","rate_Progression_H3",240,0.,240.);
    TH1F* rate_Progression_I3 = new TH1F("rate_Progression_I3","rate_Progression_I3",240,0.,240.);
    TH1F* rate_Progression_J3 = new TH1F("rate_Progression_J3","rate_Progression_J3",240,0.,240.);
    TH1F* rate_Progression_K3 = new TH1F("rate_Progression_K3","rate_Progression_K3",240,0.,240.);
    TH1F* rate_Progression_L3 = new TH1F("rate_Progression_L3","rate_Progression_L3",240,0.,240.);
    TH1F* rate_Progression_M3 = new TH1F("rate_Progression_M3","rate_Progression_M3",240,0.,240.);
    TH1F* rate_Progression_N3 = new TH1F("rate_Progression_N3","rate_Progression_N3",240,0.,240.);
    TH1F* rate_Progression_O3 = new TH1F("rate_Progression_O3","rate_Progression_O3",240,0.,240.);
    TH1F* rate_Progression_P3 = new TH1F("rate_Progression_P3","rate_Progression_P3",240,0.,240.);
    TH1F* rate_Progression_Q3 = new TH1F("rate_Progression_Q3","rate_Progression_Q3",240,0.,240.);
    TH1F* rate_Progression_R3 = new TH1F("rate_Progression_R3","rate_Progression_R3",240,0.,240.);
    TH1F* rate_Progression_S3 = new TH1F("rate_Progression_S3","rate_Progression_S3",240,0.,240.);
    TH1F* rate_Progression_T3 = new TH1F("rate_Progression_T3","rate_Progression_T3",240,0.,240.);
    TH1F* rate_Progression_U3 = new TH1F("rate_Progression_U3","rate_Progression_U3",240,0.,240.);
    TH1F* rate_Progression_V3 = new TH1F("rate_Progression_V3","rate_Progression_V3",240,0.,240.);
    TH1F* rate_Progression_W3 = new TH1F("rate_Progression_W3","rate_Progression_W3",240,0.,240.);
    TH1F* rate_Progression_X3 = new TH1F("rate_Progression_X3","rate_Progression_X3",240,0.,240.);
    TH1F* rate_Progression_Y3 = new TH1F("rate_Progression_Y3","rate_Progression_Y3",240,0.,240.);
    TH1F* rate_Progression_Z3 = new TH1F("rate_Progression_Z3","rate_Progression_Z3",240,0.,240.);
    TH1F* rate_Progression_A4 = new TH1F("rate_Progression_A4","rate_Progression_A4",240,0.,240.);
    TH1F* rate_Progression_B4 = new TH1F("rate_Progression_B4","rate_Progression_B4",240,0.,240.);
    TH1F* rate_Progression_C4 = new TH1F("rate_Progression_C4","rate_Progression_C4",240,0.,240.);
    TH1F* rate_Progression_D4 = new TH1F("rate_Progression_D4","rate_Progression_D4",240,0.,240.);
    TH1F* rate_Progression_E4 = new TH1F("rate_Progression_E4","rate_Progression_E4",240,0.,240.);
    TH1F* rate_Progression_F4 = new TH1F("rate_Progression_F4","rate_Progression_F4",240,0.,240.);
    TH1F* rate_Progression_G4 = new TH1F("rate_Progression_G4","rate_Progression_G4",240,0.,240.);
    TH1F* rate_Progression_H4 = new TH1F("rate_Progression_H4","rate_Progression_H4",240,0.,240.);
    TH1F* rate_Progression_I4 = new TH1F("rate_Progression_I4","rate_Progression_I4",240,0.,240.);
    TH1F* rate_Progression_J4 = new TH1F("rate_Progression_J4","rate_Progression_J4",240,0.,240.);
    TH1F* rate_Progression_K4 = new TH1F("rate_Progression_K4","rate_Progression_K4",240,0.,240.);
    TH1F* rate_Progression_L4 = new TH1F("rate_Progression_L4","rate_Progression_L4",240,0.,240.);
    TH1F* rate_Progression_M4 = new TH1F("rate_Progression_M4","rate_Progression_M4",240,0.,240.);
    TH1F* rate_Progression_N4 = new TH1F("rate_Progression_N4","rate_Progression_N4",240,0.,240.);
    TH1F* rate_Progression_O4 = new TH1F("rate_Progression_O4","rate_Progression_O4",240,0.,240.);
    TH1F* rate_Progression_P4 = new TH1F("rate_Progression_P4","rate_Progression_P4",240,0.,240.);
    TH1F* rate_Progression_Q4 = new TH1F("rate_Progression_Q4","rate_Progression_Q4",240,0.,240.);
    TH1F* rate_Progression_R4 = new TH1F("rate_Progression_R4","rate_Progression_R4",240,0.,240.);
    TH1F* rate_Progression_S4 = new TH1F("rate_Progression_S4","rate_Progression_S4",240,0.,240.);
    TH1F* rate_Progression_T4 = new TH1F("rate_Progression_T4","rate_Progression_T4",240,0.,240.);
    TH1F* rate_Progression_U4 = new TH1F("rate_Progression_U4","rate_Progression_U4",240,0.,240.);
    TH1F* rate_Progression_V4 = new TH1F("rate_Progression_V4","rate_Progression_V4",240,0.,240.);
    TH1F* rate_Progression_W4 = new TH1F("rate_Progression_W4","rate_Progression_W4",240,0.,240.);
    TH1F* rate_Progression_X4 = new TH1F("rate_Progression_X4","rate_Progression_X4",240,0.,240.);
    TH1F* rate_Progression_Y4 = new TH1F("rate_Progression_Y4","rate_Progression_Y4",240,0.,240.);
    TH1F* rate_Progression_Z4 = new TH1F("rate_Progression_Z4","rate_Progression_Z4",240,0.,240.);
    TH1F* rate_Progression_A5 = new TH1F("rate_Progression_A5","rate_Progression_A5",240,0.,240.);
    TH1F* rate_Progression_B5 = new TH1F("rate_Progression_B5","rate_Progression_B5",240,0.,240.);
    TH1F* rate_Progression_C5 = new TH1F("rate_Progression_C5","rate_Progression_C5",240,0.,240.);
    TH1F* rate_Progression_D5 = new TH1F("rate_Progression_D5","rate_Progression_D5",240,0.,240.);
    TH1F* rate_Progression_E5 = new TH1F("rate_Progression_E5","rate_Progression_E5",240,0.,240.);
    TH1F* rate_Progression_F5 = new TH1F("rate_Progression_F5","rate_Progression_F5",240,0.,240.);
    TH1F* rate_Progression_G5 = new TH1F("rate_Progression_G5","rate_Progression_G5",240,0.,240.);
    TH1F* rate_Progression_H5 = new TH1F("rate_Progression_H5","rate_Progression_H5",240,0.,240.);
    TH1F* rate_Progression_I5 = new TH1F("rate_Progression_I5","rate_Progression_I5",240,0.,240.);
    TH1F* rate_Progression_J5 = new TH1F("rate_Progression_J5","rate_Progression_J5",240,0.,240.);
    TH1F* rate_Progression_K5 = new TH1F("rate_Progression_K5","rate_Progression_K5",240,0.,240.);
    TH1F* rate_Progression_L5 = new TH1F("rate_Progression_L5","rate_Progression_L5",240,0.,240.);
    TH1F* rate_Progression_M5 = new TH1F("rate_Progression_M5","rate_Progression_M5",240,0.,240.);
    TH1F* rate_Progression_N5 = new TH1F("rate_Progression_N5","rate_Progression_N5",240,0.,240.);
    TH1F* rate_Progression_O5 = new TH1F("rate_Progression_O5","rate_Progression_O5",240,0.,240.);
    TH1F* rate_Progression_P5 = new TH1F("rate_Progression_P5","rate_Progression_P5",240,0.,240.);
    TH1F* rate_Progression_Q5 = new TH1F("rate_Progression_Q5","rate_Progression_Q5",240,0.,240.);
    TH1F* rate_Progression_R5 = new TH1F("rate_Progression_R5","rate_Progression_R5",240,0.,240.);
    TH1F* rate_Progression_S5 = new TH1F("rate_Progression_S5","rate_Progression_S5",240,0.,240.);
    TH1F* rate_Progression_T5 = new TH1F("rate_Progression_T5","rate_Progression_T5",240,0.,240.);
    TH1F* rate_Progression_U5 = new TH1F("rate_Progression_U5","rate_Progression_U5",240,0.,240.);
    TH1F* rate_Progression_V5 = new TH1F("rate_Progression_V5","rate_Progression_V5",240,0.,240.);
    TH1F* rate_Progression_W5 = new TH1F("rate_Progression_W5","rate_Progression_W5",240,0.,240.);
    TH1F* rate_Progression_X5 = new TH1F("rate_Progression_X5","rate_Progression_X5",240,0.,240.);
    TH1F* rate_Progression_Y5 = new TH1F("rate_Progression_Y5","rate_Progression_Y5",240,0.,240.);
    TH1F* rate_Progression_Z5 = new TH1F("rate_Progression_Z5","rate_Progression_Z5",240,0.,240.);
    TH1F* rate_Progression_A6 = new TH1F("rate_Progression_A6","rate_Progression_A6",240,0.,240.);
    TH1F* rate_Progression_B6 = new TH1F("rate_Progression_B6","rate_Progression_B6",240,0.,240.);
    TH1F* rate_Progression_C6 = new TH1F("rate_Progression_C6","rate_Progression_C6",240,0.,240.);
    TH1F* rate_Progression_D6 = new TH1F("rate_Progression_D6","rate_Progression_D6",240,0.,240.);
    TH1F* rate_Progression_E6 = new TH1F("rate_Progression_E6","rate_Progression_E6",240,0.,240.);
    TH1F* rate_Progression_F6 = new TH1F("rate_Progression_F6","rate_Progression_F6",240,0.,240.);
    TH1F* rate_Progression_G6 = new TH1F("rate_Progression_G6","rate_Progression_G6",240,0.,240.);
    TH1F* rate_Progression_H6 = new TH1F("rate_Progression_H6","rate_Progression_H6",240,0.,240.);
    TH1F* rate_Progression_I6 = new TH1F("rate_Progression_I6","rate_Progression_I6",240,0.,240.);
    TH1F* rate_Progression_J6 = new TH1F("rate_Progression_J6","rate_Progression_J6",240,0.,240.);
    TH1F* rate_Progression_K6 = new TH1F("rate_Progression_K6","rate_Progression_K6",240,0.,240.);
    TH1F* rate_Progression_L6 = new TH1F("rate_Progression_L6","rate_Progression_L6",240,0.,240.);
    TH1F* rate_Progression_M6 = new TH1F("rate_Progression_M6","rate_Progression_M6",240,0.,240.);
    TH1F* rate_Progression_N6 = new TH1F("rate_Progression_N6","rate_Progression_N6",240,0.,240.);
    TH1F* rate_Progression_O6 = new TH1F("rate_Progression_O6","rate_Progression_O6",240,0.,240.);
    TH1F* rate_Progression_P6 = new TH1F("rate_Progression_P6","rate_Progression_P6",240,0.,240.);
    TH1F* rate_Progression_Q6 = new TH1F("rate_Progression_Q6","rate_Progression_Q6",240,0.,240.);
    TH1F* rate_Progression_R6 = new TH1F("rate_Progression_R6","rate_Progression_R6",240,0.,240.);
    TH1F* rate_Progression_S6 = new TH1F("rate_Progression_S6","rate_Progression_S6",240,0.,240.);
    TH1F* rate_Progression_T6 = new TH1F("rate_Progression_T6","rate_Progression_T6",240,0.,240.);
    TH1F* rate_Progression_U6 = new TH1F("rate_Progression_U6","rate_Progression_U6",240,0.,240.);
    TH1F* rate_Progression_V6 = new TH1F("rate_Progression_V6","rate_Progression_V6",240,0.,240.);
    TH1F* rate_Progression_W6 = new TH1F("rate_Progression_W6","rate_Progression_W6",240,0.,240.);
    TH1F* rate_Progression_X6 = new TH1F("rate_Progression_X6","rate_Progression_X6",240,0.,240.);
    TH1F* rate_Progression_Y6 = new TH1F("rate_Progression_Y6","rate_Progression_Y6",240,0.,240.);
    TH1F* rate_Progression_Z6 = new TH1F("rate_Progression_Z6","rate_Progression_Z6",240,0.,240.);
    TH1F* rate_Progression_A7 = new TH1F("rate_Progression_A7","rate_Progression_A7",240,0.,240.);
    TH1F* rate_Progression_B7 = new TH1F("rate_Progression_B7","rate_Progression_B7",240,0.,240.);
    TH1F* rate_Progression_C7 = new TH1F("rate_Progression_C7","rate_Progression_C7",240,0.,240.);
    TH1F* rate_Progression_D7 = new TH1F("rate_Progression_D7","rate_Progression_D7",240,0.,240.);
    TH1F* rate_Progression_E7 = new TH1F("rate_Progression_E7","rate_Progression_E7",240,0.,240.);
    TH1F* rate_Progression_F7 = new TH1F("rate_Progression_F7","rate_Progression_F7",240,0.,240.);
    TH1F* rate_Progression_G7 = new TH1F("rate_Progression_G7","rate_Progression_G7",240,0.,240.);
    TH1F* rate_Progression_H7 = new TH1F("rate_Progression_H7","rate_Progression_H7",240,0.,240.);
    TH1F* rate_Progression_I7 = new TH1F("rate_Progression_I7","rate_Progression_I7",240,0.,240.);
    TH1F* rate_Progression_J7 = new TH1F("rate_Progression_J7","rate_Progression_J7",240,0.,240.);
    TH1F* rate_Progression_K7 = new TH1F("rate_Progression_K7","rate_Progression_K7",240,0.,240.);
    TH1F* rate_Progression_L7 = new TH1F("rate_Progression_L7","rate_Progression_L7",240,0.,240.);
    TH1F* rate_Progression_M7 = new TH1F("rate_Progression_M7","rate_Progression_M7",240,0.,240.);
    TH1F* rate_Progression_N7 = new TH1F("rate_Progression_N7","rate_Progression_N7",240,0.,240.);
    TH1F* rate_Progression_O7 = new TH1F("rate_Progression_O7","rate_Progression_O7",240,0.,240.);
    TH1F* rate_Progression_P7 = new TH1F("rate_Progression_P7","rate_Progression_P7",240,0.,240.);
    TH1F* rate_Progression_Q7 = new TH1F("rate_Progression_Q7","rate_Progression_Q7",240,0.,240.);
    TH1F* rate_Progression_R7 = new TH1F("rate_Progression_R7","rate_Progression_R7",240,0.,240.);
    TH1F* rate_Progression_S7 = new TH1F("rate_Progression_S7","rate_Progression_S7",240,0.,240.);
    TH1F* rate_Progression_T7 = new TH1F("rate_Progression_T7","rate_Progression_T7",240,0.,240.);
    TH1F* rate_Progression_U7 = new TH1F("rate_Progression_U7","rate_Progression_U7",240,0.,240.);
    TH1F* rate_Progression_V7 = new TH1F("rate_Progression_V7","rate_Progression_V7",240,0.,240.);
    TH1F* rate_Progression_W7 = new TH1F("rate_Progression_W7","rate_Progression_W7",240,0.,240.);
    TH1F* rate_Progression_X7 = new TH1F("rate_Progression_X7","rate_Progression_X7",240,0.,240.);
    TH1F* rate_Progression_Y7 = new TH1F("rate_Progression_Y7","rate_Progression_Y7",240,0.,240.);
    TH1F* rate_Progression_Z7 = new TH1F("rate_Progression_Z7","rate_Progression_Z7",240,0.,240.);
    TH1F* rate_Progression_A8 = new TH1F("rate_Progression_A8","rate_Progression_A8",240,0.,240.);
    TH1F* rate_Progression_B8 = new TH1F("rate_Progression_B8","rate_Progression_B8",240,0.,240.);
    TH1F* rate_Progression_C8 = new TH1F("rate_Progression_C8","rate_Progression_C8",240,0.,240.);
    TH1F* rate_Progression_D8 = new TH1F("rate_Progression_D8","rate_Progression_D8",240,0.,240.);
    TH1F* rate_Progression_E8 = new TH1F("rate_Progression_E8","rate_Progression_E8",240,0.,240.);
    TH1F* rate_Progression_F8 = new TH1F("rate_Progression_F8","rate_Progression_F8",240,0.,240.);
    TH1F* rate_Progression_G8 = new TH1F("rate_Progression_G8","rate_Progression_G8",240,0.,240.);
    TH1F* rate_Progression_H8 = new TH1F("rate_Progression_H8","rate_Progression_H8",240,0.,240.);
    TH1F* rate_Progression_I8 = new TH1F("rate_Progression_I8","rate_Progression_I8",240,0.,240.);
    TH1F* rate_Progression_J8 = new TH1F("rate_Progression_J8","rate_Progression_J8",240,0.,240.);
    TH1F* rate_Progression_K8 = new TH1F("rate_Progression_K8","rate_Progression_K8",240,0.,240.);
    TH1F* rate_Progression_L8 = new TH1F("rate_Progression_L8","rate_Progression_L8",240,0.,240.);
    TH1F* rate_Progression_M8 = new TH1F("rate_Progression_M8","rate_Progression_M8",240,0.,240.);
    TH1F* rate_Progression_N8 = new TH1F("rate_Progression_N8","rate_Progression_N8",240,0.,240.);
    TH1F* rate_Progression_O8 = new TH1F("rate_Progression_O8","rate_Progression_O8",240,0.,240.);
    TH1F* rate_Progression_P8 = new TH1F("rate_Progression_P8","rate_Progression_P8",240,0.,240.);
    TH1F* rate_Progression_Q8 = new TH1F("rate_Progression_Q8","rate_Progression_Q8",240,0.,240.);
    TH1F* rate_Progression_R8 = new TH1F("rate_Progression_R8","rate_Progression_R8",240,0.,240.);
    TH1F* rate_Progression_S8 = new TH1F("rate_Progression_S8","rate_Progression_S8",240,0.,240.);
    TH1F* rate_Progression_T8 = new TH1F("rate_Progression_T8","rate_Progression_T8",240,0.,240.);
    TH1F* rate_Progression_U8 = new TH1F("rate_Progression_U8","rate_Progression_U8",240,0.,240.);
    TH1F* rate_Progression_V8 = new TH1F("rate_Progression_V8","rate_Progression_V8",240,0.,240.);
    TH1F* rate_Progression_W8 = new TH1F("rate_Progression_W8","rate_Progression_W8",240,0.,240.);
    TH1F* rate_Progression_X8 = new TH1F("rate_Progression_X8","rate_Progression_X8",240,0.,240.);
    TH1F* rate_Progression_Y8 = new TH1F("rate_Progression_Y8","rate_Progression_Y8",240,0.,240.);
    TH1F* rate_Progression_Z8 = new TH1F("rate_Progression_Z8","rate_Progression_Z8",240,0.,240.);
    // END OF FULL BLOWN GRID SEARCH

    TH1F* rate_noCut_DiTau = new TH1F("rate_noCut_DiTau","rate_noCut_DiTau",240,0.,240.);
    TH1F* rate_DiTau_90 = new TH1F("rate_DiTau_90","rate_DiTau_90",240,0.,240.);
    TH1F* rate_DiTau_80 = new TH1F("rate_DiTau_80","rate_DiTau_80",240,0.,240.);
    TH1F* rate_DiTau_70 = new TH1F("rate_DiTau_70","rate_DiTau_70",240,0.,240.);
    TH1F* rate_DiTau_60 = new TH1F("rate_DiTau_60","rate_DiTau_60",240,0.,240.);
    TH1F* rate_DiTau_50 = new TH1F("rate_DiTau_50","rate_DiTau_50",240,0.,240.);
    TH1F* rate_DiTau_Progression_1 = new TH1F("rate_DiTau_Progression_1","rate_DiTau_Progression_1",240,0.,240.);
    TH1F* rate_DiTau_Progression_2 = new TH1F("rate_DiTau_Progression_2","rate_DiTau_Progression_2",240,0.,240.);
    TH1F* rate_DiTau_Progression_3 = new TH1F("rate_DiTau_Progression_3","rate_DiTau_Progression_3",240,0.,240.);
    TH1F* rate_DiTau_Progression_4 = new TH1F("rate_DiTau_Progression_4","rate_DiTau_Progression_4",240,0.,240.);
    TH1F* rate_DiTau_Progression_5 = new TH1F("rate_DiTau_Progression_5","rate_DiTau_Progression_5",240,0.,240.);
    TH1F* rate_DiTau_Progression_6 = new TH1F("rate_DiTau_Progression_6","rate_DiTau_Progression_6",240,0.,240.);
    TH1F* rate_DiTau_Progression_7 = new TH1F("rate_DiTau_Progression_7","rate_DiTau_Progression_7",240,0.,240.);
    TH1F* rate_DiTau_Progression_8 = new TH1F("rate_DiTau_Progression_8","rate_DiTau_Progression_8",240,0.,240.);
    TH1F* rate_DiTau_Progression_9 = new TH1F("rate_DiTau_Progression_9","rate_DiTau_Progression_9",240,0.,240.);
    TH1F* rate_DiTau_Progression_10 = new TH1F("rate_DiTau_Progression_10","rate_DiTau_Progression_10",240,0.,240.);
    TH1F* rate_DiTau_Progression_11 = new TH1F("rate_DiTau_Progression_11","rate_DiTau_Progression_11",240,0.,240.);
    TH1F* rate_DiTau_Progression_12 = new TH1F("rate_DiTau_Progression_12","rate_DiTau_Progression_12",240,0.,240.);
    TH1F* rate_DiTau_Progression_13 = new TH1F("rate_DiTau_Progression_13","rate_DiTau_Progression_13",240,0.,240.);
    TH1F* rate_DiTau_Progression_14 = new TH1F("rate_DiTau_Progression_14","rate_DiTau_Progression_14",240,0.,240.);
    TH1F* rate_DiTau_Progression_15 = new TH1F("rate_DiTau_Progression_15","rate_DiTau_Progression_15",240,0.,240.);
    TH1F* rate_DiTau_Progression_16 = new TH1F("rate_DiTau_Progression_16","rate_DiTau_Progression_16",240,0.,240.);
    TH1F* rate_DiTau_Progression_17 = new TH1F("rate_DiTau_Progression_17","rate_DiTau_Progression_17",240,0.,240.);
    TH1F* rate_DiTau_Progression_18 = new TH1F("rate_DiTau_Progression_18","rate_DiTau_Progression_18",240,0.,240.);
    TH1F* rate_DiTau_Progression_19 = new TH1F("rate_DiTau_Progression_19","rate_DiTau_Progression_19",240,0.,240.);
    TH1F* rate_DiTau_Progression_20 = new TH1F("rate_DiTau_Progression_20","rate_DiTau_Progression_20",240,0.,240.);
    TH1F* rate_DiTau_Progression_21 = new TH1F("rate_DiTau_Progression_21","rate_DiTau_Progression_21",240,0.,240.);
    TH1F* rate_DiTau_Progression_21_barrel = new TH1F("rate_DiTau_Progression_21_barrel","rate_DiTau_Progression_21_barrel",240,0.,240.);
    TH1F* rate_DiTau_Progression_21_endcaps = new TH1F("rate_DiTau_Progression_21_endcaps","rate_DiTau_Progression_21_endcaps",240,0.,240.);
    TH1F* rate_DiTau_Progression_22 = new TH1F("rate_DiTau_Progression_22","rate_DiTau_Progression_22",240,0.,240.);
    TH1F* rate_DiTau_Progression_22_barrel = new TH1F("rate_DiTau_Progression_22_barrel","rate_DiTau_Progression_22_barrel",240,0.,240.);
    TH1F* rate_DiTau_Progression_22_barrelendcap = new TH1F("rate_DiTau_Progression_22_barrelendcap","rate_DiTau_Progression_22_barrelendcap",240,0.,240.);
    TH1F* rate_DiTau_Progression_22_endcaps = new TH1F("rate_DiTau_Progression_22_endcaps","rate_DiTau_Progression_22_endcaps",240,0.,240.);
    TH1F* rate_DiTau_Progression_22_BothAreMerged = new TH1F("rate_DiTau_Progression_22_BothAreMerged","rate_DiTau_Progression_22_BothAreMerged",240,0.,240.);
    TH1F* rate_DiTau_Progression_22_OneIsMerged = new TH1F("rate_DiTau_Progression_22_OneIsMerged","rate_DiTau_Progression_22_OneIsMerged",240,0.,240.);
    TH1F* rate_DiTau_Progression_22_BothAreNotMerged = new TH1F("rate_DiTau_Progression_22_BothAreNotMerged","rate_DiTau_Progression_22_BothAreNotMerged",240,0.,240.);

    TH1F* rate_DiTau_Progression_23 = new TH1F("rate_DiTau_Progression_23","rate_DiTau_Progression_23",240,0.,240.);
    TH1F* rate_DiTau_Progression_24 = new TH1F("rate_DiTau_Progression_24","rate_DiTau_Progression_24",240,0.,240.);
    TH1F* rate_DiTau_Progression_25 = new TH1F("rate_DiTau_Progression_25","rate_DiTau_Progression_25",240,0.,240.);
    TH1F* rate_DiTau_Progression_26 = new TH1F("rate_DiTau_Progression_26","rate_DiTau_Progression_26",240,0.,240.);
    TH1F* rate_DiTau_Progression_27 = new TH1F("rate_DiTau_Progression_27","rate_DiTau_Progression_27",240,0.,240.);
    TH1F* rate_DiTau_Progression_28 = new TH1F("rate_DiTau_Progression_28","rate_DiTau_Progression_28",240,0.,240.);
    TH1F* rate_DiTau_Progression_29 = new TH1F("rate_DiTau_Progression_29","rate_DiTau_Progression_29",240,0.,240.);
    TH1F* rate_DiTau_Progression_30 = new TH1F("rate_DiTau_Progression_30","rate_DiTau_Progression_30",240,0.,240.);
    TH1F* rate_DiTau_Progression_31 = new TH1F("rate_DiTau_Progression_31","rate_DiTau_Progression_31",240,0.,240.);
    TH1F* rate_DiTau_Progression_31_barrel = new TH1F("rate_DiTau_Progression_31_barrel","rate_DiTau_Progression_31_barrel",240,0.,240.);
    TH1F* rate_DiTau_Progression_31_barrelendcap = new TH1F("rate_DiTau_Progression_31_barrelendcap","rate_DiTau_Progression_31_barrelendcap",240,0.,240.);
    TH1F* rate_DiTau_Progression_31_endcaps = new TH1F("rate_DiTau_Progression_31_endcaps","rate_DiTau_Progression_31_endcaps",240,0.,240.);
    TH1F* rate_DiTau_Progression_31_extrap = new TH1F("rate_DiTau_Progression_31_extrap","rate_DiTau_Progression_31_extrap",240,0.,240.);
    TH1F* rate_DiTau_Progression_31_extrap_barrel = new TH1F("rate_DiTau_Progression_31_extrap_barrel","rate_DiTau_Progression_31_extrap_barrel",240,0.,240.);
    TH1F* rate_DiTau_Progression_31_extrap_barrelendcap = new TH1F("rate_DiTau_Progression_31_extrap_barrelendcap","rate_DiTau_Progression_31_extrap_barrelendcap",240,0.,240.);
    TH1F* rate_DiTau_Progression_31_extrap_endcaps = new TH1F("rate_DiTau_Progression_31_extrap_endcaps","rate_DiTau_Progression_31_extrap_endcaps",240,0.,240.);

    // START OF FULL BLOWN GRID SEARCH
    TH1F* rate_DiTau_Progression_A0 = new TH1F("rate_DiTau_Progression_A0","rate_DiTau_Progression_A0",240,0.,240.);
    TH1F* rate_DiTau_Progression_B0 = new TH1F("rate_DiTau_Progression_B0","rate_DiTau_Progression_B0",240,0.,240.);
    TH1F* rate_DiTau_Progression_C0 = new TH1F("rate_DiTau_Progression_C0","rate_DiTau_Progression_C0",240,0.,240.);
    TH1F* rate_DiTau_Progression_D0 = new TH1F("rate_DiTau_Progression_D0","rate_DiTau_Progression_D0",240,0.,240.);
    TH1F* rate_DiTau_Progression_E0 = new TH1F("rate_DiTau_Progression_E0","rate_DiTau_Progression_E0",240,0.,240.);
    TH1F* rate_DiTau_Progression_F0 = new TH1F("rate_DiTau_Progression_F0","rate_DiTau_Progression_F0",240,0.,240.);
    TH1F* rate_DiTau_Progression_G0 = new TH1F("rate_DiTau_Progression_G0","rate_DiTau_Progression_G0",240,0.,240.);
    TH1F* rate_DiTau_Progression_H0 = new TH1F("rate_DiTau_Progression_H0","rate_DiTau_Progression_H0",240,0.,240.);
    TH1F* rate_DiTau_Progression_I0 = new TH1F("rate_DiTau_Progression_I0","rate_DiTau_Progression_I0",240,0.,240.);
    TH1F* rate_DiTau_Progression_J0 = new TH1F("rate_DiTau_Progression_J0","rate_DiTau_Progression_J0",240,0.,240.);
    TH1F* rate_DiTau_Progression_K0 = new TH1F("rate_DiTau_Progression_K0","rate_DiTau_Progression_K0",240,0.,240.);
    TH1F* rate_DiTau_Progression_L0 = new TH1F("rate_DiTau_Progression_L0","rate_DiTau_Progression_L0",240,0.,240.);
    TH1F* rate_DiTau_Progression_M0 = new TH1F("rate_DiTau_Progression_M0","rate_DiTau_Progression_M0",240,0.,240.);
    TH1F* rate_DiTau_Progression_N0 = new TH1F("rate_DiTau_Progression_N0","rate_DiTau_Progression_N0",240,0.,240.);
    TH1F* rate_DiTau_Progression_O0 = new TH1F("rate_DiTau_Progression_O0","rate_DiTau_Progression_O0",240,0.,240.);
    TH1F* rate_DiTau_Progression_P0 = new TH1F("rate_DiTau_Progression_P0","rate_DiTau_Progression_P0",240,0.,240.);
    TH1F* rate_DiTau_Progression_Q0 = new TH1F("rate_DiTau_Progression_Q0","rate_DiTau_Progression_Q0",240,0.,240.);
    TH1F* rate_DiTau_Progression_R0 = new TH1F("rate_DiTau_Progression_R0","rate_DiTau_Progression_R0",240,0.,240.);
    TH1F* rate_DiTau_Progression_S0 = new TH1F("rate_DiTau_Progression_S0","rate_DiTau_Progression_S0",240,0.,240.);
    TH1F* rate_DiTau_Progression_T0 = new TH1F("rate_DiTau_Progression_T0","rate_DiTau_Progression_T0",240,0.,240.);
    TH1F* rate_DiTau_Progression_U0 = new TH1F("rate_DiTau_Progression_U0","rate_DiTau_Progression_U0",240,0.,240.);
    TH1F* rate_DiTau_Progression_V0 = new TH1F("rate_DiTau_Progression_V0","rate_DiTau_Progression_V0",240,0.,240.);
    TH1F* rate_DiTau_Progression_W0 = new TH1F("rate_DiTau_Progression_W0","rate_DiTau_Progression_W0",240,0.,240.);
    TH1F* rate_DiTau_Progression_X0 = new TH1F("rate_DiTau_Progression_X0","rate_DiTau_Progression_X0",240,0.,240.);
    TH1F* rate_DiTau_Progression_Y0 = new TH1F("rate_DiTau_Progression_Y0","rate_DiTau_Progression_Y0",240,0.,240.);
    TH1F* rate_DiTau_Progression_Z0 = new TH1F("rate_DiTau_Progression_Z0","rate_DiTau_Progression_Z0",240,0.,240.);
    TH1F* rate_DiTau_Progression_A1 = new TH1F("rate_DiTau_Progression_A1","rate_DiTau_Progression_A1",240,0.,240.);
    TH1F* rate_DiTau_Progression_B1 = new TH1F("rate_DiTau_Progression_B1","rate_DiTau_Progression_B1",240,0.,240.);
    TH1F* rate_DiTau_Progression_C1 = new TH1F("rate_DiTau_Progression_C1","rate_DiTau_Progression_C1",240,0.,240.);
    TH1F* rate_DiTau_Progression_D1 = new TH1F("rate_DiTau_Progression_D1","rate_DiTau_Progression_D1",240,0.,240.);
    TH1F* rate_DiTau_Progression_E1 = new TH1F("rate_DiTau_Progression_E1","rate_DiTau_Progression_E1",240,0.,240.);
    TH1F* rate_DiTau_Progression_F1 = new TH1F("rate_DiTau_Progression_F1","rate_DiTau_Progression_F1",240,0.,240.);
    TH1F* rate_DiTau_Progression_G1 = new TH1F("rate_DiTau_Progression_G1","rate_DiTau_Progression_G1",240,0.,240.);
    TH1F* rate_DiTau_Progression_H1 = new TH1F("rate_DiTau_Progression_H1","rate_DiTau_Progression_H1",240,0.,240.);
    TH1F* rate_DiTau_Progression_I1 = new TH1F("rate_DiTau_Progression_I1","rate_DiTau_Progression_I1",240,0.,240.);
    TH1F* rate_DiTau_Progression_J1 = new TH1F("rate_DiTau_Progression_J1","rate_DiTau_Progression_J1",240,0.,240.);
    TH1F* rate_DiTau_Progression_K1 = new TH1F("rate_DiTau_Progression_K1","rate_DiTau_Progression_K1",240,0.,240.);
    TH1F* rate_DiTau_Progression_L1 = new TH1F("rate_DiTau_Progression_L1","rate_DiTau_Progression_L1",240,0.,240.);
    TH1F* rate_DiTau_Progression_M1 = new TH1F("rate_DiTau_Progression_M1","rate_DiTau_Progression_M1",240,0.,240.);
    TH1F* rate_DiTau_Progression_N1 = new TH1F("rate_DiTau_Progression_N1","rate_DiTau_Progression_N1",240,0.,240.);
    TH1F* rate_DiTau_Progression_O1 = new TH1F("rate_DiTau_Progression_O1","rate_DiTau_Progression_O1",240,0.,240.);
    TH1F* rate_DiTau_Progression_P1 = new TH1F("rate_DiTau_Progression_P1","rate_DiTau_Progression_P1",240,0.,240.);
    TH1F* rate_DiTau_Progression_Q1 = new TH1F("rate_DiTau_Progression_Q1","rate_DiTau_Progression_Q1",240,0.,240.);
    TH1F* rate_DiTau_Progression_R1 = new TH1F("rate_DiTau_Progression_R1","rate_DiTau_Progression_R1",240,0.,240.);
    TH1F* rate_DiTau_Progression_S1 = new TH1F("rate_DiTau_Progression_S1","rate_DiTau_Progression_S1",240,0.,240.);
    TH1F* rate_DiTau_Progression_T1 = new TH1F("rate_DiTau_Progression_T1","rate_DiTau_Progression_T1",240,0.,240.);
    TH1F* rate_DiTau_Progression_U1 = new TH1F("rate_DiTau_Progression_U1","rate_DiTau_Progression_U1",240,0.,240.);
    TH1F* rate_DiTau_Progression_V1 = new TH1F("rate_DiTau_Progression_V1","rate_DiTau_Progression_V1",240,0.,240.);
    TH1F* rate_DiTau_Progression_W1 = new TH1F("rate_DiTau_Progression_W1","rate_DiTau_Progression_W1",240,0.,240.);
    TH1F* rate_DiTau_Progression_X1 = new TH1F("rate_DiTau_Progression_X1","rate_DiTau_Progression_X1",240,0.,240.);
    TH1F* rate_DiTau_Progression_Y1 = new TH1F("rate_DiTau_Progression_Y1","rate_DiTau_Progression_Y1",240,0.,240.);
    TH1F* rate_DiTau_Progression_Z1 = new TH1F("rate_DiTau_Progression_Z1","rate_DiTau_Progression_Z1",240,0.,240.);
    TH1F* rate_DiTau_Progression_A2 = new TH1F("rate_DiTau_Progression_A2","rate_DiTau_Progression_A2",240,0.,240.);
    TH1F* rate_DiTau_Progression_B2 = new TH1F("rate_DiTau_Progression_B2","rate_DiTau_Progression_B2",240,0.,240.);
    TH1F* rate_DiTau_Progression_C2 = new TH1F("rate_DiTau_Progression_C2","rate_DiTau_Progression_C2",240,0.,240.);
    TH1F* rate_DiTau_Progression_D2 = new TH1F("rate_DiTau_Progression_D2","rate_DiTau_Progression_D2",240,0.,240.);
    TH1F* rate_DiTau_Progression_E2 = new TH1F("rate_DiTau_Progression_E2","rate_DiTau_Progression_E2",240,0.,240.);
    TH1F* rate_DiTau_Progression_F2 = new TH1F("rate_DiTau_Progression_F2","rate_DiTau_Progression_F2",240,0.,240.);
    TH1F* rate_DiTau_Progression_G2 = new TH1F("rate_DiTau_Progression_G2","rate_DiTau_Progression_G2",240,0.,240.);
    TH1F* rate_DiTau_Progression_H2 = new TH1F("rate_DiTau_Progression_H2","rate_DiTau_Progression_H2",240,0.,240.);
    TH1F* rate_DiTau_Progression_I2 = new TH1F("rate_DiTau_Progression_I2","rate_DiTau_Progression_I2",240,0.,240.);
    TH1F* rate_DiTau_Progression_J2 = new TH1F("rate_DiTau_Progression_J2","rate_DiTau_Progression_J2",240,0.,240.);
    TH1F* rate_DiTau_Progression_K2 = new TH1F("rate_DiTau_Progression_K2","rate_DiTau_Progression_K2",240,0.,240.);
    TH1F* rate_DiTau_Progression_L2 = new TH1F("rate_DiTau_Progression_L2","rate_DiTau_Progression_L2",240,0.,240.);
    TH1F* rate_DiTau_Progression_M2 = new TH1F("rate_DiTau_Progression_M2","rate_DiTau_Progression_M2",240,0.,240.);
    TH1F* rate_DiTau_Progression_N2 = new TH1F("rate_DiTau_Progression_N2","rate_DiTau_Progression_N2",240,0.,240.);
    TH1F* rate_DiTau_Progression_O2 = new TH1F("rate_DiTau_Progression_O2","rate_DiTau_Progression_O2",240,0.,240.);
    TH1F* rate_DiTau_Progression_P2 = new TH1F("rate_DiTau_Progression_P2","rate_DiTau_Progression_P2",240,0.,240.);
    TH1F* rate_DiTau_Progression_Q2 = new TH1F("rate_DiTau_Progression_Q2","rate_DiTau_Progression_Q2",240,0.,240.);
    TH1F* rate_DiTau_Progression_R2 = new TH1F("rate_DiTau_Progression_R2","rate_DiTau_Progression_R2",240,0.,240.);
    TH1F* rate_DiTau_Progression_S2 = new TH1F("rate_DiTau_Progression_S2","rate_DiTau_Progression_S2",240,0.,240.);
    TH1F* rate_DiTau_Progression_T2 = new TH1F("rate_DiTau_Progression_T2","rate_DiTau_Progression_T2",240,0.,240.);
    TH1F* rate_DiTau_Progression_U2 = new TH1F("rate_DiTau_Progression_U2","rate_DiTau_Progression_U2",240,0.,240.);
    TH1F* rate_DiTau_Progression_V2 = new TH1F("rate_DiTau_Progression_V2","rate_DiTau_Progression_V2",240,0.,240.);
    TH1F* rate_DiTau_Progression_W2 = new TH1F("rate_DiTau_Progression_W2","rate_DiTau_Progression_W2",240,0.,240.);
    TH1F* rate_DiTau_Progression_X2 = new TH1F("rate_DiTau_Progression_X2","rate_DiTau_Progression_X2",240,0.,240.);
    TH1F* rate_DiTau_Progression_Y2 = new TH1F("rate_DiTau_Progression_Y2","rate_DiTau_Progression_Y2",240,0.,240.);
    TH1F* rate_DiTau_Progression_Z2 = new TH1F("rate_DiTau_Progression_Z2","rate_DiTau_Progression_Z2",240,0.,240.);
    TH1F* rate_DiTau_Progression_A3 = new TH1F("rate_DiTau_Progression_A3","rate_DiTau_Progression_A3",240,0.,240.);
    TH1F* rate_DiTau_Progression_B3 = new TH1F("rate_DiTau_Progression_B3","rate_DiTau_Progression_B3",240,0.,240.);
    TH1F* rate_DiTau_Progression_C3 = new TH1F("rate_DiTau_Progression_C3","rate_DiTau_Progression_C3",240,0.,240.);
    TH1F* rate_DiTau_Progression_D3 = new TH1F("rate_DiTau_Progression_D3","rate_DiTau_Progression_D3",240,0.,240.);
    TH1F* rate_DiTau_Progression_E3 = new TH1F("rate_DiTau_Progression_E3","rate_DiTau_Progression_E3",240,0.,240.);
    TH1F* rate_DiTau_Progression_F3 = new TH1F("rate_DiTau_Progression_F3","rate_DiTau_Progression_F3",240,0.,240.);
    TH1F* rate_DiTau_Progression_G3 = new TH1F("rate_DiTau_Progression_G3","rate_DiTau_Progression_G3",240,0.,240.);
    TH1F* rate_DiTau_Progression_H3 = new TH1F("rate_DiTau_Progression_H3","rate_DiTau_Progression_H3",240,0.,240.);
    TH1F* rate_DiTau_Progression_I3 = new TH1F("rate_DiTau_Progression_I3","rate_DiTau_Progression_I3",240,0.,240.);
    TH1F* rate_DiTau_Progression_J3 = new TH1F("rate_DiTau_Progression_J3","rate_DiTau_Progression_J3",240,0.,240.);
    TH1F* rate_DiTau_Progression_K3 = new TH1F("rate_DiTau_Progression_K3","rate_DiTau_Progression_K3",240,0.,240.);
    TH1F* rate_DiTau_Progression_L3 = new TH1F("rate_DiTau_Progression_L3","rate_DiTau_Progression_L3",240,0.,240.);
    TH1F* rate_DiTau_Progression_M3 = new TH1F("rate_DiTau_Progression_M3","rate_DiTau_Progression_M3",240,0.,240.);
    TH1F* rate_DiTau_Progression_N3 = new TH1F("rate_DiTau_Progression_N3","rate_DiTau_Progression_N3",240,0.,240.);
    TH1F* rate_DiTau_Progression_O3 = new TH1F("rate_DiTau_Progression_O3","rate_DiTau_Progression_O3",240,0.,240.);
    TH1F* rate_DiTau_Progression_P3 = new TH1F("rate_DiTau_Progression_P3","rate_DiTau_Progression_P3",240,0.,240.);
    TH1F* rate_DiTau_Progression_Q3 = new TH1F("rate_DiTau_Progression_Q3","rate_DiTau_Progression_Q3",240,0.,240.);
    TH1F* rate_DiTau_Progression_R3 = new TH1F("rate_DiTau_Progression_R3","rate_DiTau_Progression_R3",240,0.,240.);
    TH1F* rate_DiTau_Progression_S3 = new TH1F("rate_DiTau_Progression_S3","rate_DiTau_Progression_S3",240,0.,240.);
    TH1F* rate_DiTau_Progression_T3 = new TH1F("rate_DiTau_Progression_T3","rate_DiTau_Progression_T3",240,0.,240.);
    TH1F* rate_DiTau_Progression_U3 = new TH1F("rate_DiTau_Progression_U3","rate_DiTau_Progression_U3",240,0.,240.);
    TH1F* rate_DiTau_Progression_V3 = new TH1F("rate_DiTau_Progression_V3","rate_DiTau_Progression_V3",240,0.,240.);
    TH1F* rate_DiTau_Progression_W3 = new TH1F("rate_DiTau_Progression_W3","rate_DiTau_Progression_W3",240,0.,240.);
    TH1F* rate_DiTau_Progression_X3 = new TH1F("rate_DiTau_Progression_X3","rate_DiTau_Progression_X3",240,0.,240.);
    TH1F* rate_DiTau_Progression_Y3 = new TH1F("rate_DiTau_Progression_Y3","rate_DiTau_Progression_Y3",240,0.,240.);
    TH1F* rate_DiTau_Progression_Z3 = new TH1F("rate_DiTau_Progression_Z3","rate_DiTau_Progression_Z3",240,0.,240.);
    TH1F* rate_DiTau_Progression_A4 = new TH1F("rate_DiTau_Progression_A4","rate_DiTau_Progression_A4",240,0.,240.);
    TH1F* rate_DiTau_Progression_B4 = new TH1F("rate_DiTau_Progression_B4","rate_DiTau_Progression_B4",240,0.,240.);
    TH1F* rate_DiTau_Progression_C4 = new TH1F("rate_DiTau_Progression_C4","rate_DiTau_Progression_C4",240,0.,240.);
    TH1F* rate_DiTau_Progression_D4 = new TH1F("rate_DiTau_Progression_D4","rate_DiTau_Progression_D4",240,0.,240.);
    TH1F* rate_DiTau_Progression_E4 = new TH1F("rate_DiTau_Progression_E4","rate_DiTau_Progression_E4",240,0.,240.);
    TH1F* rate_DiTau_Progression_F4 = new TH1F("rate_DiTau_Progression_F4","rate_DiTau_Progression_F4",240,0.,240.);
    TH1F* rate_DiTau_Progression_G4 = new TH1F("rate_DiTau_Progression_G4","rate_DiTau_Progression_G4",240,0.,240.);
    TH1F* rate_DiTau_Progression_H4 = new TH1F("rate_DiTau_Progression_H4","rate_DiTau_Progression_H4",240,0.,240.);
    TH1F* rate_DiTau_Progression_I4 = new TH1F("rate_DiTau_Progression_I4","rate_DiTau_Progression_I4",240,0.,240.);
    TH1F* rate_DiTau_Progression_J4 = new TH1F("rate_DiTau_Progression_J4","rate_DiTau_Progression_J4",240,0.,240.);
    TH1F* rate_DiTau_Progression_K4 = new TH1F("rate_DiTau_Progression_K4","rate_DiTau_Progression_K4",240,0.,240.);
    TH1F* rate_DiTau_Progression_L4 = new TH1F("rate_DiTau_Progression_L4","rate_DiTau_Progression_L4",240,0.,240.);
    TH1F* rate_DiTau_Progression_M4 = new TH1F("rate_DiTau_Progression_M4","rate_DiTau_Progression_M4",240,0.,240.);
    TH1F* rate_DiTau_Progression_N4 = new TH1F("rate_DiTau_Progression_N4","rate_DiTau_Progression_N4",240,0.,240.);
    TH1F* rate_DiTau_Progression_O4 = new TH1F("rate_DiTau_Progression_O4","rate_DiTau_Progression_O4",240,0.,240.);
    TH1F* rate_DiTau_Progression_P4 = new TH1F("rate_DiTau_Progression_P4","rate_DiTau_Progression_P4",240,0.,240.);
    TH1F* rate_DiTau_Progression_Q4 = new TH1F("rate_DiTau_Progression_Q4","rate_DiTau_Progression_Q4",240,0.,240.);
    TH1F* rate_DiTau_Progression_R4 = new TH1F("rate_DiTau_Progression_R4","rate_DiTau_Progression_R4",240,0.,240.);
    TH1F* rate_DiTau_Progression_S4 = new TH1F("rate_DiTau_Progression_S4","rate_DiTau_Progression_S4",240,0.,240.);
    TH1F* rate_DiTau_Progression_T4 = new TH1F("rate_DiTau_Progression_T4","rate_DiTau_Progression_T4",240,0.,240.);
    TH1F* rate_DiTau_Progression_U4 = new TH1F("rate_DiTau_Progression_U4","rate_DiTau_Progression_U4",240,0.,240.);
    TH1F* rate_DiTau_Progression_V4 = new TH1F("rate_DiTau_Progression_V4","rate_DiTau_Progression_V4",240,0.,240.);
    TH1F* rate_DiTau_Progression_W4 = new TH1F("rate_DiTau_Progression_W4","rate_DiTau_Progression_W4",240,0.,240.);
    TH1F* rate_DiTau_Progression_X4 = new TH1F("rate_DiTau_Progression_X4","rate_DiTau_Progression_X4",240,0.,240.);
    TH1F* rate_DiTau_Progression_Y4 = new TH1F("rate_DiTau_Progression_Y4","rate_DiTau_Progression_Y4",240,0.,240.);
    TH1F* rate_DiTau_Progression_Z4 = new TH1F("rate_DiTau_Progression_Z4","rate_DiTau_Progression_Z4",240,0.,240.);
    TH1F* rate_DiTau_Progression_A5 = new TH1F("rate_DiTau_Progression_A5","rate_DiTau_Progression_A5",240,0.,240.);
    TH1F* rate_DiTau_Progression_B5 = new TH1F("rate_DiTau_Progression_B5","rate_DiTau_Progression_B5",240,0.,240.);
    TH1F* rate_DiTau_Progression_C5 = new TH1F("rate_DiTau_Progression_C5","rate_DiTau_Progression_C5",240,0.,240.);
    TH1F* rate_DiTau_Progression_D5 = new TH1F("rate_DiTau_Progression_D5","rate_DiTau_Progression_D5",240,0.,240.);
    TH1F* rate_DiTau_Progression_E5 = new TH1F("rate_DiTau_Progression_E5","rate_DiTau_Progression_E5",240,0.,240.);
    TH1F* rate_DiTau_Progression_F5 = new TH1F("rate_DiTau_Progression_F5","rate_DiTau_Progression_F5",240,0.,240.);
    TH1F* rate_DiTau_Progression_G5 = new TH1F("rate_DiTau_Progression_G5","rate_DiTau_Progression_G5",240,0.,240.);
    TH1F* rate_DiTau_Progression_H5 = new TH1F("rate_DiTau_Progression_H5","rate_DiTau_Progression_H5",240,0.,240.);
    TH1F* rate_DiTau_Progression_I5 = new TH1F("rate_DiTau_Progression_I5","rate_DiTau_Progression_I5",240,0.,240.);
    TH1F* rate_DiTau_Progression_J5 = new TH1F("rate_DiTau_Progression_J5","rate_DiTau_Progression_J5",240,0.,240.);
    TH1F* rate_DiTau_Progression_K5 = new TH1F("rate_DiTau_Progression_K5","rate_DiTau_Progression_K5",240,0.,240.);
    TH1F* rate_DiTau_Progression_L5 = new TH1F("rate_DiTau_Progression_L5","rate_DiTau_Progression_L5",240,0.,240.);
    TH1F* rate_DiTau_Progression_M5 = new TH1F("rate_DiTau_Progression_M5","rate_DiTau_Progression_M5",240,0.,240.);
    TH1F* rate_DiTau_Progression_N5 = new TH1F("rate_DiTau_Progression_N5","rate_DiTau_Progression_N5",240,0.,240.);
    TH1F* rate_DiTau_Progression_O5 = new TH1F("rate_DiTau_Progression_O5","rate_DiTau_Progression_O5",240,0.,240.);
    TH1F* rate_DiTau_Progression_P5 = new TH1F("rate_DiTau_Progression_P5","rate_DiTau_Progression_P5",240,0.,240.);
    TH1F* rate_DiTau_Progression_Q5 = new TH1F("rate_DiTau_Progression_Q5","rate_DiTau_Progression_Q5",240,0.,240.);
    TH1F* rate_DiTau_Progression_R5 = new TH1F("rate_DiTau_Progression_R5","rate_DiTau_Progression_R5",240,0.,240.);
    TH1F* rate_DiTau_Progression_S5 = new TH1F("rate_DiTau_Progression_S5","rate_DiTau_Progression_S5",240,0.,240.);
    TH1F* rate_DiTau_Progression_T5 = new TH1F("rate_DiTau_Progression_T5","rate_DiTau_Progression_T5",240,0.,240.);
    TH1F* rate_DiTau_Progression_U5 = new TH1F("rate_DiTau_Progression_U5","rate_DiTau_Progression_U5",240,0.,240.);
    TH1F* rate_DiTau_Progression_V5 = new TH1F("rate_DiTau_Progression_V5","rate_DiTau_Progression_V5",240,0.,240.);
    TH1F* rate_DiTau_Progression_W5 = new TH1F("rate_DiTau_Progression_W5","rate_DiTau_Progression_W5",240,0.,240.);
    TH1F* rate_DiTau_Progression_X5 = new TH1F("rate_DiTau_Progression_X5","rate_DiTau_Progression_X5",240,0.,240.);
    TH1F* rate_DiTau_Progression_Y5 = new TH1F("rate_DiTau_Progression_Y5","rate_DiTau_Progression_Y5",240,0.,240.);
    TH1F* rate_DiTau_Progression_Z5 = new TH1F("rate_DiTau_Progression_Z5","rate_DiTau_Progression_Z5",240,0.,240.);
    TH1F* rate_DiTau_Progression_A6 = new TH1F("rate_DiTau_Progression_A6","rate_DiTau_Progression_A6",240,0.,240.);
    TH1F* rate_DiTau_Progression_B6 = new TH1F("rate_DiTau_Progression_B6","rate_DiTau_Progression_B6",240,0.,240.);
    TH1F* rate_DiTau_Progression_C6 = new TH1F("rate_DiTau_Progression_C6","rate_DiTau_Progression_C6",240,0.,240.);
    TH1F* rate_DiTau_Progression_D6 = new TH1F("rate_DiTau_Progression_D6","rate_DiTau_Progression_D6",240,0.,240.);
    TH1F* rate_DiTau_Progression_E6 = new TH1F("rate_DiTau_Progression_E6","rate_DiTau_Progression_E6",240,0.,240.);
    TH1F* rate_DiTau_Progression_F6 = new TH1F("rate_DiTau_Progression_F6","rate_DiTau_Progression_F6",240,0.,240.);
    TH1F* rate_DiTau_Progression_G6 = new TH1F("rate_DiTau_Progression_G6","rate_DiTau_Progression_G6",240,0.,240.);
    TH1F* rate_DiTau_Progression_H6 = new TH1F("rate_DiTau_Progression_H6","rate_DiTau_Progression_H6",240,0.,240.);
    TH1F* rate_DiTau_Progression_I6 = new TH1F("rate_DiTau_Progression_I6","rate_DiTau_Progression_I6",240,0.,240.);
    TH1F* rate_DiTau_Progression_J6 = new TH1F("rate_DiTau_Progression_J6","rate_DiTau_Progression_J6",240,0.,240.);
    TH1F* rate_DiTau_Progression_K6 = new TH1F("rate_DiTau_Progression_K6","rate_DiTau_Progression_K6",240,0.,240.);
    TH1F* rate_DiTau_Progression_L6 = new TH1F("rate_DiTau_Progression_L6","rate_DiTau_Progression_L6",240,0.,240.);
    TH1F* rate_DiTau_Progression_M6 = new TH1F("rate_DiTau_Progression_M6","rate_DiTau_Progression_M6",240,0.,240.);
    TH1F* rate_DiTau_Progression_N6 = new TH1F("rate_DiTau_Progression_N6","rate_DiTau_Progression_N6",240,0.,240.);
    TH1F* rate_DiTau_Progression_O6 = new TH1F("rate_DiTau_Progression_O6","rate_DiTau_Progression_O6",240,0.,240.);
    TH1F* rate_DiTau_Progression_P6 = new TH1F("rate_DiTau_Progression_P6","rate_DiTau_Progression_P6",240,0.,240.);
    TH1F* rate_DiTau_Progression_Q6 = new TH1F("rate_DiTau_Progression_Q6","rate_DiTau_Progression_Q6",240,0.,240.);
    TH1F* rate_DiTau_Progression_R6 = new TH1F("rate_DiTau_Progression_R6","rate_DiTau_Progression_R6",240,0.,240.);
    TH1F* rate_DiTau_Progression_S6 = new TH1F("rate_DiTau_Progression_S6","rate_DiTau_Progression_S6",240,0.,240.);
    TH1F* rate_DiTau_Progression_T6 = new TH1F("rate_DiTau_Progression_T6","rate_DiTau_Progression_T6",240,0.,240.);
    TH1F* rate_DiTau_Progression_U6 = new TH1F("rate_DiTau_Progression_U6","rate_DiTau_Progression_U6",240,0.,240.);
    TH1F* rate_DiTau_Progression_V6 = new TH1F("rate_DiTau_Progression_V6","rate_DiTau_Progression_V6",240,0.,240.);
    TH1F* rate_DiTau_Progression_W6 = new TH1F("rate_DiTau_Progression_W6","rate_DiTau_Progression_W6",240,0.,240.);
    TH1F* rate_DiTau_Progression_X6 = new TH1F("rate_DiTau_Progression_X6","rate_DiTau_Progression_X6",240,0.,240.);
    TH1F* rate_DiTau_Progression_Y6 = new TH1F("rate_DiTau_Progression_Y6","rate_DiTau_Progression_Y6",240,0.,240.);
    TH1F* rate_DiTau_Progression_Z6 = new TH1F("rate_DiTau_Progression_Z6","rate_DiTau_Progression_Z6",240,0.,240.);
    TH1F* rate_DiTau_Progression_A7 = new TH1F("rate_DiTau_Progression_A7","rate_DiTau_Progression_A7",240,0.,240.);
    TH1F* rate_DiTau_Progression_B7 = new TH1F("rate_DiTau_Progression_B7","rate_DiTau_Progression_B7",240,0.,240.);
    TH1F* rate_DiTau_Progression_C7 = new TH1F("rate_DiTau_Progression_C7","rate_DiTau_Progression_C7",240,0.,240.);
    TH1F* rate_DiTau_Progression_D7 = new TH1F("rate_DiTau_Progression_D7","rate_DiTau_Progression_D7",240,0.,240.);
    TH1F* rate_DiTau_Progression_E7 = new TH1F("rate_DiTau_Progression_E7","rate_DiTau_Progression_E7",240,0.,240.);
    TH1F* rate_DiTau_Progression_F7 = new TH1F("rate_DiTau_Progression_F7","rate_DiTau_Progression_F7",240,0.,240.);
    TH1F* rate_DiTau_Progression_G7 = new TH1F("rate_DiTau_Progression_G7","rate_DiTau_Progression_G7",240,0.,240.);
    TH1F* rate_DiTau_Progression_H7 = new TH1F("rate_DiTau_Progression_H7","rate_DiTau_Progression_H7",240,0.,240.);
    TH1F* rate_DiTau_Progression_I7 = new TH1F("rate_DiTau_Progression_I7","rate_DiTau_Progression_I7",240,0.,240.);
    TH1F* rate_DiTau_Progression_J7 = new TH1F("rate_DiTau_Progression_J7","rate_DiTau_Progression_J7",240,0.,240.);
    TH1F* rate_DiTau_Progression_K7 = new TH1F("rate_DiTau_Progression_K7","rate_DiTau_Progression_K7",240,0.,240.);
    TH1F* rate_DiTau_Progression_L7 = new TH1F("rate_DiTau_Progression_L7","rate_DiTau_Progression_L7",240,0.,240.);
    TH1F* rate_DiTau_Progression_M7 = new TH1F("rate_DiTau_Progression_M7","rate_DiTau_Progression_M7",240,0.,240.);
    TH1F* rate_DiTau_Progression_N7 = new TH1F("rate_DiTau_Progression_N7","rate_DiTau_Progression_N7",240,0.,240.);
    TH1F* rate_DiTau_Progression_O7 = new TH1F("rate_DiTau_Progression_O7","rate_DiTau_Progression_O7",240,0.,240.);
    TH1F* rate_DiTau_Progression_P7 = new TH1F("rate_DiTau_Progression_P7","rate_DiTau_Progression_P7",240,0.,240.);
    TH1F* rate_DiTau_Progression_Q7 = new TH1F("rate_DiTau_Progression_Q7","rate_DiTau_Progression_Q7",240,0.,240.);
    TH1F* rate_DiTau_Progression_R7 = new TH1F("rate_DiTau_Progression_R7","rate_DiTau_Progression_R7",240,0.,240.);
    TH1F* rate_DiTau_Progression_S7 = new TH1F("rate_DiTau_Progression_S7","rate_DiTau_Progression_S7",240,0.,240.);
    TH1F* rate_DiTau_Progression_T7 = new TH1F("rate_DiTau_Progression_T7","rate_DiTau_Progression_T7",240,0.,240.);
    TH1F* rate_DiTau_Progression_U7 = new TH1F("rate_DiTau_Progression_U7","rate_DiTau_Progression_U7",240,0.,240.);
    TH1F* rate_DiTau_Progression_V7 = new TH1F("rate_DiTau_Progression_V7","rate_DiTau_Progression_V7",240,0.,240.);
    TH1F* rate_DiTau_Progression_W7 = new TH1F("rate_DiTau_Progression_W7","rate_DiTau_Progression_W7",240,0.,240.);
    TH1F* rate_DiTau_Progression_X7 = new TH1F("rate_DiTau_Progression_X7","rate_DiTau_Progression_X7",240,0.,240.);
    TH1F* rate_DiTau_Progression_Y7 = new TH1F("rate_DiTau_Progression_Y7","rate_DiTau_Progression_Y7",240,0.,240.);
    TH1F* rate_DiTau_Progression_Z7 = new TH1F("rate_DiTau_Progression_Z7","rate_DiTau_Progression_Z7",240,0.,240.);
    TH1F* rate_DiTau_Progression_A8 = new TH1F("rate_DiTau_Progression_A8","rate_DiTau_Progression_A8",240,0.,240.);
    TH1F* rate_DiTau_Progression_B8 = new TH1F("rate_DiTau_Progression_B8","rate_DiTau_Progression_B8",240,0.,240.);
    TH1F* rate_DiTau_Progression_C8 = new TH1F("rate_DiTau_Progression_C8","rate_DiTau_Progression_C8",240,0.,240.);
    TH1F* rate_DiTau_Progression_D8 = new TH1F("rate_DiTau_Progression_D8","rate_DiTau_Progression_D8",240,0.,240.);
    TH1F* rate_DiTau_Progression_E8 = new TH1F("rate_DiTau_Progression_E8","rate_DiTau_Progression_E8",240,0.,240.);
    TH1F* rate_DiTau_Progression_F8 = new TH1F("rate_DiTau_Progression_F8","rate_DiTau_Progression_F8",240,0.,240.);
    TH1F* rate_DiTau_Progression_G8 = new TH1F("rate_DiTau_Progression_G8","rate_DiTau_Progression_G8",240,0.,240.);
    TH1F* rate_DiTau_Progression_H8 = new TH1F("rate_DiTau_Progression_H8","rate_DiTau_Progression_H8",240,0.,240.);
    TH1F* rate_DiTau_Progression_I8 = new TH1F("rate_DiTau_Progression_I8","rate_DiTau_Progression_I8",240,0.,240.);
    TH1F* rate_DiTau_Progression_J8 = new TH1F("rate_DiTau_Progression_J8","rate_DiTau_Progression_J8",240,0.,240.);
    TH1F* rate_DiTau_Progression_K8 = new TH1F("rate_DiTau_Progression_K8","rate_DiTau_Progression_K8",240,0.,240.);
    TH1F* rate_DiTau_Progression_L8 = new TH1F("rate_DiTau_Progression_L8","rate_DiTau_Progression_L8",240,0.,240.);
    TH1F* rate_DiTau_Progression_M8 = new TH1F("rate_DiTau_Progression_M8","rate_DiTau_Progression_M8",240,0.,240.);
    TH1F* rate_DiTau_Progression_N8 = new TH1F("rate_DiTau_Progression_N8","rate_DiTau_Progression_N8",240,0.,240.);
    TH1F* rate_DiTau_Progression_O8 = new TH1F("rate_DiTau_Progression_O8","rate_DiTau_Progression_O8",240,0.,240.);
    TH1F* rate_DiTau_Progression_P8 = new TH1F("rate_DiTau_Progression_P8","rate_DiTau_Progression_P8",240,0.,240.);
    TH1F* rate_DiTau_Progression_Q8 = new TH1F("rate_DiTau_Progression_Q8","rate_DiTau_Progression_Q8",240,0.,240.);
    TH1F* rate_DiTau_Progression_R8 = new TH1F("rate_DiTau_Progression_R8","rate_DiTau_Progression_R8",240,0.,240.);
    TH1F* rate_DiTau_Progression_S8 = new TH1F("rate_DiTau_Progression_S8","rate_DiTau_Progression_S8",240,0.,240.);
    TH1F* rate_DiTau_Progression_T8 = new TH1F("rate_DiTau_Progression_T8","rate_DiTau_Progression_T8",240,0.,240.);
    TH1F* rate_DiTau_Progression_U8 = new TH1F("rate_DiTau_Progression_U8","rate_DiTau_Progression_U8",240,0.,240.);
    TH1F* rate_DiTau_Progression_V8 = new TH1F("rate_DiTau_Progression_V8","rate_DiTau_Progression_V8",240,0.,240.);
    TH1F* rate_DiTau_Progression_W8 = new TH1F("rate_DiTau_Progression_W8","rate_DiTau_Progression_W8",240,0.,240.);
    TH1F* rate_DiTau_Progression_X8 = new TH1F("rate_DiTau_Progression_X8","rate_DiTau_Progression_X8",240,0.,240.);
    TH1F* rate_DiTau_Progression_Y8 = new TH1F("rate_DiTau_Progression_Y8","rate_DiTau_Progression_Y8",240,0.,240.);
    TH1F* rate_DiTau_Progression_Z8 = new TH1F("rate_DiTau_Progression_Z8","rate_DiTau_Progression_Z8",240,0.,240.);
    // END OF FULL BLOWN GRID SEARCH

    for(UInt_t i = 0 ; i < 241 ; ++i)
        {
            rate_noCut->SetBinContent(i+1,pt_IsoInf->Integral(i+1,241)/Denominator*scale);
            rate_IsoCut_50->SetBinContent(i+1,pt_Iso_50->Integral(i+1,241)/Denominator*scale);
            rate_IsoCut_60->SetBinContent(i+1,pt_Iso_60->Integral(i+1,241)/Denominator*scale);
            rate_IsoCut_70->SetBinContent(i+1,pt_Iso_70->Integral(i+1,241)/Denominator*scale);
            rate_IsoCut_80->SetBinContent(i+1,pt_Iso_80->Integral(i+1,241)/Denominator*scale);
            rate_IsoCut_90->SetBinContent(i+1,pt_Iso_90->Integral(i+1,241)/Denominator*scale);
            rate_Progression_1->SetBinContent(i+1,pt_Progression_1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_2->SetBinContent(i+1,pt_Progression_2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_3->SetBinContent(i+1,pt_Progression_3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_4->SetBinContent(i+1,pt_Progression_4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_5->SetBinContent(i+1,pt_Progression_5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_6->SetBinContent(i+1,pt_Progression_6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_7->SetBinContent(i+1,pt_Progression_7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_8->SetBinContent(i+1,pt_Progression_8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_9->SetBinContent(i+1,pt_Progression_9->Integral(i+1,241)/Denominator*scale);
            rate_Progression_10->SetBinContent(i+1,pt_Progression_10->Integral(i+1,241)/Denominator*scale);
            rate_Progression_11->SetBinContent(i+1,pt_Progression_11->Integral(i+1,241)/Denominator*scale);
            rate_Progression_12->SetBinContent(i+1,pt_Progression_12->Integral(i+1,241)/Denominator*scale);
            rate_Progression_13->SetBinContent(i+1,pt_Progression_13->Integral(i+1,241)/Denominator*scale);
            rate_Progression_14->SetBinContent(i+1,pt_Progression_14->Integral(i+1,241)/Denominator*scale);
            rate_Progression_15->SetBinContent(i+1,pt_Progression_15->Integral(i+1,241)/Denominator*scale);
            rate_Progression_16->SetBinContent(i+1,pt_Progression_16->Integral(i+1,241)/Denominator*scale);
            rate_Progression_17->SetBinContent(i+1,pt_Progression_17->Integral(i+1,241)/Denominator*scale);
            rate_Progression_18->SetBinContent(i+1,pt_Progression_18->Integral(i+1,241)/Denominator*scale);
            rate_Progression_19->SetBinContent(i+1,pt_Progression_19->Integral(i+1,241)/Denominator*scale);
            rate_Progression_20->SetBinContent(i+1,pt_Progression_20->Integral(i+1,241)/Denominator*scale);
            rate_Progression_21->SetBinContent(i+1,pt_Progression_21->Integral(i+1,241)/Denominator*scale);
            rate_Progression_22->SetBinContent(i+1,pt_Progression_22->Integral(i+1,241)/Denominator*scale);

            // START OF FULL BLOWN GRID SEARCH
            rate_Progression_A0->SetBinContent(i+1,pt_Progression_A0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_B0->SetBinContent(i+1,pt_Progression_B0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_C0->SetBinContent(i+1,pt_Progression_C0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_D0->SetBinContent(i+1,pt_Progression_D0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_E0->SetBinContent(i+1,pt_Progression_E0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_F0->SetBinContent(i+1,pt_Progression_F0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_G0->SetBinContent(i+1,pt_Progression_G0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_H0->SetBinContent(i+1,pt_Progression_H0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_I0->SetBinContent(i+1,pt_Progression_I0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_J0->SetBinContent(i+1,pt_Progression_J0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_K0->SetBinContent(i+1,pt_Progression_K0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_L0->SetBinContent(i+1,pt_Progression_L0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_M0->SetBinContent(i+1,pt_Progression_M0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_N0->SetBinContent(i+1,pt_Progression_N0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_O0->SetBinContent(i+1,pt_Progression_O0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_P0->SetBinContent(i+1,pt_Progression_P0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Q0->SetBinContent(i+1,pt_Progression_Q0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_R0->SetBinContent(i+1,pt_Progression_R0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_S0->SetBinContent(i+1,pt_Progression_S0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_T0->SetBinContent(i+1,pt_Progression_T0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_U0->SetBinContent(i+1,pt_Progression_U0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_V0->SetBinContent(i+1,pt_Progression_V0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_W0->SetBinContent(i+1,pt_Progression_W0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_X0->SetBinContent(i+1,pt_Progression_X0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Y0->SetBinContent(i+1,pt_Progression_Y0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Z0->SetBinContent(i+1,pt_Progression_Z0->Integral(i+1,241)/Denominator*scale);
            rate_Progression_A1->SetBinContent(i+1,pt_Progression_A1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_B1->SetBinContent(i+1,pt_Progression_B1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_C1->SetBinContent(i+1,pt_Progression_C1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_D1->SetBinContent(i+1,pt_Progression_D1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_E1->SetBinContent(i+1,pt_Progression_E1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_F1->SetBinContent(i+1,pt_Progression_F1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_G1->SetBinContent(i+1,pt_Progression_G1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_H1->SetBinContent(i+1,pt_Progression_H1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_I1->SetBinContent(i+1,pt_Progression_I1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_J1->SetBinContent(i+1,pt_Progression_J1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_K1->SetBinContent(i+1,pt_Progression_K1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_L1->SetBinContent(i+1,pt_Progression_L1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_M1->SetBinContent(i+1,pt_Progression_M1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_N1->SetBinContent(i+1,pt_Progression_N1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_O1->SetBinContent(i+1,pt_Progression_O1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_P1->SetBinContent(i+1,pt_Progression_P1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Q1->SetBinContent(i+1,pt_Progression_Q1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_R1->SetBinContent(i+1,pt_Progression_R1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_S1->SetBinContent(i+1,pt_Progression_S1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_T1->SetBinContent(i+1,pt_Progression_T1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_U1->SetBinContent(i+1,pt_Progression_U1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_V1->SetBinContent(i+1,pt_Progression_V1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_W1->SetBinContent(i+1,pt_Progression_W1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_X1->SetBinContent(i+1,pt_Progression_X1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Y1->SetBinContent(i+1,pt_Progression_Y1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Z1->SetBinContent(i+1,pt_Progression_Z1->Integral(i+1,241)/Denominator*scale);
            rate_Progression_A2->SetBinContent(i+1,pt_Progression_A2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_B2->SetBinContent(i+1,pt_Progression_B2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_C2->SetBinContent(i+1,pt_Progression_C2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_D2->SetBinContent(i+1,pt_Progression_D2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_E2->SetBinContent(i+1,pt_Progression_E2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_F2->SetBinContent(i+1,pt_Progression_F2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_G2->SetBinContent(i+1,pt_Progression_G2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_H2->SetBinContent(i+1,pt_Progression_H2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_I2->SetBinContent(i+1,pt_Progression_I2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_J2->SetBinContent(i+1,pt_Progression_J2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_K2->SetBinContent(i+1,pt_Progression_K2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_L2->SetBinContent(i+1,pt_Progression_L2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_M2->SetBinContent(i+1,pt_Progression_M2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_N2->SetBinContent(i+1,pt_Progression_N2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_O2->SetBinContent(i+1,pt_Progression_O2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_P2->SetBinContent(i+1,pt_Progression_P2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Q2->SetBinContent(i+1,pt_Progression_Q2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_R2->SetBinContent(i+1,pt_Progression_R2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_S2->SetBinContent(i+1,pt_Progression_S2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_T2->SetBinContent(i+1,pt_Progression_T2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_U2->SetBinContent(i+1,pt_Progression_U2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_V2->SetBinContent(i+1,pt_Progression_V2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_W2->SetBinContent(i+1,pt_Progression_W2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_X2->SetBinContent(i+1,pt_Progression_X2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Y2->SetBinContent(i+1,pt_Progression_Y2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Z2->SetBinContent(i+1,pt_Progression_Z2->Integral(i+1,241)/Denominator*scale);
            rate_Progression_A3->SetBinContent(i+1,pt_Progression_A3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_B3->SetBinContent(i+1,pt_Progression_B3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_C3->SetBinContent(i+1,pt_Progression_C3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_D3->SetBinContent(i+1,pt_Progression_D3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_E3->SetBinContent(i+1,pt_Progression_E3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_F3->SetBinContent(i+1,pt_Progression_F3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_G3->SetBinContent(i+1,pt_Progression_G3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_H3->SetBinContent(i+1,pt_Progression_H3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_I3->SetBinContent(i+1,pt_Progression_I3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_J3->SetBinContent(i+1,pt_Progression_J3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_K3->SetBinContent(i+1,pt_Progression_K3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_L3->SetBinContent(i+1,pt_Progression_L3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_M3->SetBinContent(i+1,pt_Progression_M3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_N3->SetBinContent(i+1,pt_Progression_N3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_O3->SetBinContent(i+1,pt_Progression_O3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_P3->SetBinContent(i+1,pt_Progression_P3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Q3->SetBinContent(i+1,pt_Progression_Q3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_R3->SetBinContent(i+1,pt_Progression_R3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_S3->SetBinContent(i+1,pt_Progression_S3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_T3->SetBinContent(i+1,pt_Progression_T3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_U3->SetBinContent(i+1,pt_Progression_U3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_V3->SetBinContent(i+1,pt_Progression_V3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_W3->SetBinContent(i+1,pt_Progression_W3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_X3->SetBinContent(i+1,pt_Progression_X3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Y3->SetBinContent(i+1,pt_Progression_Y3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Z3->SetBinContent(i+1,pt_Progression_Z3->Integral(i+1,241)/Denominator*scale);
            rate_Progression_A4->SetBinContent(i+1,pt_Progression_A4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_B4->SetBinContent(i+1,pt_Progression_B4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_C4->SetBinContent(i+1,pt_Progression_C4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_D4->SetBinContent(i+1,pt_Progression_D4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_E4->SetBinContent(i+1,pt_Progression_E4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_F4->SetBinContent(i+1,pt_Progression_F4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_G4->SetBinContent(i+1,pt_Progression_G4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_H4->SetBinContent(i+1,pt_Progression_H4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_I4->SetBinContent(i+1,pt_Progression_I4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_J4->SetBinContent(i+1,pt_Progression_J4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_K4->SetBinContent(i+1,pt_Progression_K4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_L4->SetBinContent(i+1,pt_Progression_L4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_M4->SetBinContent(i+1,pt_Progression_M4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_N4->SetBinContent(i+1,pt_Progression_N4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_O4->SetBinContent(i+1,pt_Progression_O4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_P4->SetBinContent(i+1,pt_Progression_P4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Q4->SetBinContent(i+1,pt_Progression_Q4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_R4->SetBinContent(i+1,pt_Progression_R4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_S4->SetBinContent(i+1,pt_Progression_S4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_T4->SetBinContent(i+1,pt_Progression_T4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_U4->SetBinContent(i+1,pt_Progression_U4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_V4->SetBinContent(i+1,pt_Progression_V4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_W4->SetBinContent(i+1,pt_Progression_W4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_X4->SetBinContent(i+1,pt_Progression_X4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Y4->SetBinContent(i+1,pt_Progression_Y4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Z4->SetBinContent(i+1,pt_Progression_Z4->Integral(i+1,241)/Denominator*scale);
            rate_Progression_A5->SetBinContent(i+1,pt_Progression_A5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_B5->SetBinContent(i+1,pt_Progression_B5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_C5->SetBinContent(i+1,pt_Progression_C5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_D5->SetBinContent(i+1,pt_Progression_D5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_E5->SetBinContent(i+1,pt_Progression_E5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_F5->SetBinContent(i+1,pt_Progression_F5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_G5->SetBinContent(i+1,pt_Progression_G5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_H5->SetBinContent(i+1,pt_Progression_H5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_I5->SetBinContent(i+1,pt_Progression_I5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_J5->SetBinContent(i+1,pt_Progression_J5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_K5->SetBinContent(i+1,pt_Progression_K5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_L5->SetBinContent(i+1,pt_Progression_L5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_M5->SetBinContent(i+1,pt_Progression_M5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_N5->SetBinContent(i+1,pt_Progression_N5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_O5->SetBinContent(i+1,pt_Progression_O5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_P5->SetBinContent(i+1,pt_Progression_P5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Q5->SetBinContent(i+1,pt_Progression_Q5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_R5->SetBinContent(i+1,pt_Progression_R5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_S5->SetBinContent(i+1,pt_Progression_S5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_T5->SetBinContent(i+1,pt_Progression_T5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_U5->SetBinContent(i+1,pt_Progression_U5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_V5->SetBinContent(i+1,pt_Progression_V5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_W5->SetBinContent(i+1,pt_Progression_W5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_X5->SetBinContent(i+1,pt_Progression_X5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Y5->SetBinContent(i+1,pt_Progression_Y5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Z5->SetBinContent(i+1,pt_Progression_Z5->Integral(i+1,241)/Denominator*scale);
            rate_Progression_A6->SetBinContent(i+1,pt_Progression_A6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_B6->SetBinContent(i+1,pt_Progression_B6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_C6->SetBinContent(i+1,pt_Progression_C6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_D6->SetBinContent(i+1,pt_Progression_D6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_E6->SetBinContent(i+1,pt_Progression_E6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_F6->SetBinContent(i+1,pt_Progression_F6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_G6->SetBinContent(i+1,pt_Progression_G6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_H6->SetBinContent(i+1,pt_Progression_H6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_I6->SetBinContent(i+1,pt_Progression_I6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_J6->SetBinContent(i+1,pt_Progression_J6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_K6->SetBinContent(i+1,pt_Progression_K6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_L6->SetBinContent(i+1,pt_Progression_L6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_M6->SetBinContent(i+1,pt_Progression_M6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_N6->SetBinContent(i+1,pt_Progression_N6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_O6->SetBinContent(i+1,pt_Progression_O6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_P6->SetBinContent(i+1,pt_Progression_P6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Q6->SetBinContent(i+1,pt_Progression_Q6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_R6->SetBinContent(i+1,pt_Progression_R6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_S6->SetBinContent(i+1,pt_Progression_S6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_T6->SetBinContent(i+1,pt_Progression_T6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_U6->SetBinContent(i+1,pt_Progression_U6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_V6->SetBinContent(i+1,pt_Progression_V6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_W6->SetBinContent(i+1,pt_Progression_W6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_X6->SetBinContent(i+1,pt_Progression_X6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Y6->SetBinContent(i+1,pt_Progression_Y6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Z6->SetBinContent(i+1,pt_Progression_Z6->Integral(i+1,241)/Denominator*scale);
            rate_Progression_A7->SetBinContent(i+1,pt_Progression_A7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_B7->SetBinContent(i+1,pt_Progression_B7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_C7->SetBinContent(i+1,pt_Progression_C7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_D7->SetBinContent(i+1,pt_Progression_D7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_E7->SetBinContent(i+1,pt_Progression_E7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_F7->SetBinContent(i+1,pt_Progression_F7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_G7->SetBinContent(i+1,pt_Progression_G7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_H7->SetBinContent(i+1,pt_Progression_H7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_I7->SetBinContent(i+1,pt_Progression_I7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_J7->SetBinContent(i+1,pt_Progression_J7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_K7->SetBinContent(i+1,pt_Progression_K7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_L7->SetBinContent(i+1,pt_Progression_L7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_M7->SetBinContent(i+1,pt_Progression_M7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_N7->SetBinContent(i+1,pt_Progression_N7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_O7->SetBinContent(i+1,pt_Progression_O7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_P7->SetBinContent(i+1,pt_Progression_P7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Q7->SetBinContent(i+1,pt_Progression_Q7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_R7->SetBinContent(i+1,pt_Progression_R7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_S7->SetBinContent(i+1,pt_Progression_S7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_T7->SetBinContent(i+1,pt_Progression_T7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_U7->SetBinContent(i+1,pt_Progression_U7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_V7->SetBinContent(i+1,pt_Progression_V7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_W7->SetBinContent(i+1,pt_Progression_W7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_X7->SetBinContent(i+1,pt_Progression_X7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Y7->SetBinContent(i+1,pt_Progression_Y7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Z7->SetBinContent(i+1,pt_Progression_Z7->Integral(i+1,241)/Denominator*scale);
            rate_Progression_A8->SetBinContent(i+1,pt_Progression_A8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_B8->SetBinContent(i+1,pt_Progression_B8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_C8->SetBinContent(i+1,pt_Progression_C8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_D8->SetBinContent(i+1,pt_Progression_D8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_E8->SetBinContent(i+1,pt_Progression_E8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_F8->SetBinContent(i+1,pt_Progression_F8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_G8->SetBinContent(i+1,pt_Progression_G8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_H8->SetBinContent(i+1,pt_Progression_H8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_I8->SetBinContent(i+1,pt_Progression_I8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_J8->SetBinContent(i+1,pt_Progression_J8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_K8->SetBinContent(i+1,pt_Progression_K8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_L8->SetBinContent(i+1,pt_Progression_L8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_M8->SetBinContent(i+1,pt_Progression_M8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_N8->SetBinContent(i+1,pt_Progression_N8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_O8->SetBinContent(i+1,pt_Progression_O8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_P8->SetBinContent(i+1,pt_Progression_P8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Q8->SetBinContent(i+1,pt_Progression_Q8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_R8->SetBinContent(i+1,pt_Progression_R8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_S8->SetBinContent(i+1,pt_Progression_S8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_T8->SetBinContent(i+1,pt_Progression_T8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_U8->SetBinContent(i+1,pt_Progression_U8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_V8->SetBinContent(i+1,pt_Progression_V8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_W8->SetBinContent(i+1,pt_Progression_W8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_X8->SetBinContent(i+1,pt_Progression_X8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Y8->SetBinContent(i+1,pt_Progression_Y8->Integral(i+1,241)/Denominator*scale);
            rate_Progression_Z8->SetBinContent(i+1,pt_Progression_Z8->Integral(i+1,241)/Denominator*scale);
            // END OF FULL BLOWN GRID SEARCH

            rate_noCut_DiTau->SetBinContent(i+1,pt_IsoInf_DiTau->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_90->SetBinContent(i+1,pt_DiTau_90pc->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_80->SetBinContent(i+1,pt_DiTau_80pc->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_70->SetBinContent(i+1,pt_DiTau_70pc->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_60->SetBinContent(i+1,pt_DiTau_60pc->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_50->SetBinContent(i+1,pt_DiTau_50pc->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_1->SetBinContent(i+1,pt_DiTau_Progression_1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_2->SetBinContent(i+1,pt_DiTau_Progression_2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_3->SetBinContent(i+1,pt_DiTau_Progression_3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_4->SetBinContent(i+1,pt_DiTau_Progression_4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_5->SetBinContent(i+1,pt_DiTau_Progression_5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_6->SetBinContent(i+1,pt_DiTau_Progression_6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_7->SetBinContent(i+1,pt_DiTau_Progression_7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_8->SetBinContent(i+1,pt_DiTau_Progression_8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_9->SetBinContent(i+1,pt_DiTau_Progression_9->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_10->SetBinContent(i+1,pt_DiTau_Progression_10->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_11->SetBinContent(i+1,pt_DiTau_Progression_11->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_12->SetBinContent(i+1,pt_DiTau_Progression_12->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_13->SetBinContent(i+1,pt_DiTau_Progression_13->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_14->SetBinContent(i+1,pt_DiTau_Progression_14->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_15->SetBinContent(i+1,pt_DiTau_Progression_15->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_16->SetBinContent(i+1,pt_DiTau_Progression_16->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_17->SetBinContent(i+1,pt_DiTau_Progression_17->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_18->SetBinContent(i+1,pt_DiTau_Progression_18->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_19->SetBinContent(i+1,pt_DiTau_Progression_19->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_20->SetBinContent(i+1,pt_DiTau_Progression_20->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_21->SetBinContent(i+1,pt_DiTau_Progression_21->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_21_barrel->SetBinContent(i+1,pt_DiTau_Progression_21_barrel->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_21_endcaps->SetBinContent(i+1,pt_DiTau_Progression_21_endcaps->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_22->SetBinContent(i+1,pt_DiTau_Progression_22->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_22_barrel->SetBinContent(i+1,pt_DiTau_Progression_22_barrel->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_22_barrelendcap->SetBinContent(i+1,pt_DiTau_Progression_22_barrelendcap->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_22_endcaps->SetBinContent(i+1,pt_DiTau_Progression_22_endcaps->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_22_BothAreMerged->SetBinContent(i+1,pt_DiTau_Progression_22_BothAreMerged->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_22_OneIsMerged->SetBinContent(i+1,pt_DiTau_Progression_22_OneIsMerged->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_22_BothAreNotMerged->SetBinContent(i+1,pt_DiTau_Progression_22_BothAreNotMerged->Integral(i+1,241,i+1,241)/Denominator*scale);

            rate_DiTau_Progression_23->SetBinContent(i+1,pt_DiTau_Progression_23->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_24->SetBinContent(i+1,pt_DiTau_Progression_24->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_25->SetBinContent(i+1,pt_DiTau_Progression_25->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_26->SetBinContent(i+1,pt_DiTau_Progression_26->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_27->SetBinContent(i+1,pt_DiTau_Progression_27->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_28->SetBinContent(i+1,pt_DiTau_Progression_28->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_29->SetBinContent(i+1,pt_DiTau_Progression_29->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_30->SetBinContent(i+1,pt_DiTau_Progression_30->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_31->SetBinContent(i+1,pt_DiTau_Progression_31->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_31_barrel->SetBinContent(i+1,pt_DiTau_Progression_31_barrel->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_31_barrelendcap->SetBinContent(i+1,pt_DiTau_Progression_31_barrelendcap->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_31_endcaps->SetBinContent(i+1,pt_DiTau_Progression_31_endcaps->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_31_extrap->SetBinContent(i+1,pt_DiTau_Progression_31_extrap->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_31_extrap_barrel->SetBinContent(i+1,pt_DiTau_Progression_31_extrap_barrel->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_31_extrap_barrelendcap->SetBinContent(i+1,pt_DiTau_Progression_31_extrap_barrelendcap->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_31_extrap_endcaps->SetBinContent(i+1,pt_DiTau_Progression_31_extrap_endcaps->Integral(i+1,241,i+1,241)/Denominator*scale);
            // rate_IsoCut_Linear->SetBinContent(i+1,pt_IsoLinear->Integral(i+1,1201)/Denominator*scale);
            // rate_IsoCut_Linear_DiTau->SetBinContent(i+1,pt_IsoLinear_DiTau->Integral(i+1,1201)/Denominator*scale);
            //rate_Stage1->SetBinContent(i+1,pt_Stage1->Integral(i+1,1201)/dataStage1.GetEntries()*scale);

            // START OF FULL BLOWN GRID SEARCH
            rate_DiTau_Progression_A0->SetBinContent(i+1,pt_DiTau_Progression_A0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_B0->SetBinContent(i+1,pt_DiTau_Progression_B0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_C0->SetBinContent(i+1,pt_DiTau_Progression_C0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_D0->SetBinContent(i+1,pt_DiTau_Progression_D0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_E0->SetBinContent(i+1,pt_DiTau_Progression_E0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_F0->SetBinContent(i+1,pt_DiTau_Progression_F0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_G0->SetBinContent(i+1,pt_DiTau_Progression_G0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_H0->SetBinContent(i+1,pt_DiTau_Progression_H0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_I0->SetBinContent(i+1,pt_DiTau_Progression_I0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_J0->SetBinContent(i+1,pt_DiTau_Progression_J0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_K0->SetBinContent(i+1,pt_DiTau_Progression_K0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_L0->SetBinContent(i+1,pt_DiTau_Progression_L0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_M0->SetBinContent(i+1,pt_DiTau_Progression_M0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_N0->SetBinContent(i+1,pt_DiTau_Progression_N0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_O0->SetBinContent(i+1,pt_DiTau_Progression_O0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_P0->SetBinContent(i+1,pt_DiTau_Progression_P0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Q0->SetBinContent(i+1,pt_DiTau_Progression_Q0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_R0->SetBinContent(i+1,pt_DiTau_Progression_R0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_S0->SetBinContent(i+1,pt_DiTau_Progression_S0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_T0->SetBinContent(i+1,pt_DiTau_Progression_T0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_U0->SetBinContent(i+1,pt_DiTau_Progression_U0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_V0->SetBinContent(i+1,pt_DiTau_Progression_V0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_W0->SetBinContent(i+1,pt_DiTau_Progression_W0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_X0->SetBinContent(i+1,pt_DiTau_Progression_X0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Y0->SetBinContent(i+1,pt_DiTau_Progression_Y0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Z0->SetBinContent(i+1,pt_DiTau_Progression_Z0->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_A1->SetBinContent(i+1,pt_DiTau_Progression_A1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_B1->SetBinContent(i+1,pt_DiTau_Progression_B1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_C1->SetBinContent(i+1,pt_DiTau_Progression_C1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_D1->SetBinContent(i+1,pt_DiTau_Progression_D1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_E1->SetBinContent(i+1,pt_DiTau_Progression_E1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_F1->SetBinContent(i+1,pt_DiTau_Progression_F1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_G1->SetBinContent(i+1,pt_DiTau_Progression_G1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_H1->SetBinContent(i+1,pt_DiTau_Progression_H1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_I1->SetBinContent(i+1,pt_DiTau_Progression_I1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_J1->SetBinContent(i+1,pt_DiTau_Progression_J1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_K1->SetBinContent(i+1,pt_DiTau_Progression_K1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_L1->SetBinContent(i+1,pt_DiTau_Progression_L1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_M1->SetBinContent(i+1,pt_DiTau_Progression_M1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_N1->SetBinContent(i+1,pt_DiTau_Progression_N1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_O1->SetBinContent(i+1,pt_DiTau_Progression_O1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_P1->SetBinContent(i+1,pt_DiTau_Progression_P1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Q1->SetBinContent(i+1,pt_DiTau_Progression_Q1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_R1->SetBinContent(i+1,pt_DiTau_Progression_R1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_S1->SetBinContent(i+1,pt_DiTau_Progression_S1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_T1->SetBinContent(i+1,pt_DiTau_Progression_T1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_U1->SetBinContent(i+1,pt_DiTau_Progression_U1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_V1->SetBinContent(i+1,pt_DiTau_Progression_V1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_W1->SetBinContent(i+1,pt_DiTau_Progression_W1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_X1->SetBinContent(i+1,pt_DiTau_Progression_X1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Y1->SetBinContent(i+1,pt_DiTau_Progression_Y1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Z1->SetBinContent(i+1,pt_DiTau_Progression_Z1->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_A2->SetBinContent(i+1,pt_DiTau_Progression_A2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_B2->SetBinContent(i+1,pt_DiTau_Progression_B2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_C2->SetBinContent(i+1,pt_DiTau_Progression_C2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_D2->SetBinContent(i+1,pt_DiTau_Progression_D2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_E2->SetBinContent(i+1,pt_DiTau_Progression_E2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_F2->SetBinContent(i+1,pt_DiTau_Progression_F2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_G2->SetBinContent(i+1,pt_DiTau_Progression_G2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_H2->SetBinContent(i+1,pt_DiTau_Progression_H2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_I2->SetBinContent(i+1,pt_DiTau_Progression_I2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_J2->SetBinContent(i+1,pt_DiTau_Progression_J2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_K2->SetBinContent(i+1,pt_DiTau_Progression_K2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_L2->SetBinContent(i+1,pt_DiTau_Progression_L2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_M2->SetBinContent(i+1,pt_DiTau_Progression_M2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_N2->SetBinContent(i+1,pt_DiTau_Progression_N2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_O2->SetBinContent(i+1,pt_DiTau_Progression_O2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_P2->SetBinContent(i+1,pt_DiTau_Progression_P2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Q2->SetBinContent(i+1,pt_DiTau_Progression_Q2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_R2->SetBinContent(i+1,pt_DiTau_Progression_R2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_S2->SetBinContent(i+1,pt_DiTau_Progression_S2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_T2->SetBinContent(i+1,pt_DiTau_Progression_T2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_U2->SetBinContent(i+1,pt_DiTau_Progression_U2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_V2->SetBinContent(i+1,pt_DiTau_Progression_V2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_W2->SetBinContent(i+1,pt_DiTau_Progression_W2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_X2->SetBinContent(i+1,pt_DiTau_Progression_X2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Y2->SetBinContent(i+1,pt_DiTau_Progression_Y2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Z2->SetBinContent(i+1,pt_DiTau_Progression_Z2->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_A3->SetBinContent(i+1,pt_DiTau_Progression_A3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_B3->SetBinContent(i+1,pt_DiTau_Progression_B3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_C3->SetBinContent(i+1,pt_DiTau_Progression_C3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_D3->SetBinContent(i+1,pt_DiTau_Progression_D3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_E3->SetBinContent(i+1,pt_DiTau_Progression_E3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_F3->SetBinContent(i+1,pt_DiTau_Progression_F3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_G3->SetBinContent(i+1,pt_DiTau_Progression_G3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_H3->SetBinContent(i+1,pt_DiTau_Progression_H3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_I3->SetBinContent(i+1,pt_DiTau_Progression_I3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_J3->SetBinContent(i+1,pt_DiTau_Progression_J3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_K3->SetBinContent(i+1,pt_DiTau_Progression_K3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_L3->SetBinContent(i+1,pt_DiTau_Progression_L3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_M3->SetBinContent(i+1,pt_DiTau_Progression_M3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_N3->SetBinContent(i+1,pt_DiTau_Progression_N3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_O3->SetBinContent(i+1,pt_DiTau_Progression_O3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_P3->SetBinContent(i+1,pt_DiTau_Progression_P3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Q3->SetBinContent(i+1,pt_DiTau_Progression_Q3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_R3->SetBinContent(i+1,pt_DiTau_Progression_R3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_S3->SetBinContent(i+1,pt_DiTau_Progression_S3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_T3->SetBinContent(i+1,pt_DiTau_Progression_T3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_U3->SetBinContent(i+1,pt_DiTau_Progression_U3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_V3->SetBinContent(i+1,pt_DiTau_Progression_V3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_W3->SetBinContent(i+1,pt_DiTau_Progression_W3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_X3->SetBinContent(i+1,pt_DiTau_Progression_X3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Y3->SetBinContent(i+1,pt_DiTau_Progression_Y3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Z3->SetBinContent(i+1,pt_DiTau_Progression_Z3->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_A4->SetBinContent(i+1,pt_DiTau_Progression_A4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_B4->SetBinContent(i+1,pt_DiTau_Progression_B4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_C4->SetBinContent(i+1,pt_DiTau_Progression_C4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_D4->SetBinContent(i+1,pt_DiTau_Progression_D4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_E4->SetBinContent(i+1,pt_DiTau_Progression_E4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_F4->SetBinContent(i+1,pt_DiTau_Progression_F4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_G4->SetBinContent(i+1,pt_DiTau_Progression_G4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_H4->SetBinContent(i+1,pt_DiTau_Progression_H4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_I4->SetBinContent(i+1,pt_DiTau_Progression_I4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_J4->SetBinContent(i+1,pt_DiTau_Progression_J4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_K4->SetBinContent(i+1,pt_DiTau_Progression_K4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_L4->SetBinContent(i+1,pt_DiTau_Progression_L4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_M4->SetBinContent(i+1,pt_DiTau_Progression_M4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_N4->SetBinContent(i+1,pt_DiTau_Progression_N4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_O4->SetBinContent(i+1,pt_DiTau_Progression_O4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_P4->SetBinContent(i+1,pt_DiTau_Progression_P4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Q4->SetBinContent(i+1,pt_DiTau_Progression_Q4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_R4->SetBinContent(i+1,pt_DiTau_Progression_R4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_S4->SetBinContent(i+1,pt_DiTau_Progression_S4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_T4->SetBinContent(i+1,pt_DiTau_Progression_T4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_U4->SetBinContent(i+1,pt_DiTau_Progression_U4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_V4->SetBinContent(i+1,pt_DiTau_Progression_V4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_W4->SetBinContent(i+1,pt_DiTau_Progression_W4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_X4->SetBinContent(i+1,pt_DiTau_Progression_X4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Y4->SetBinContent(i+1,pt_DiTau_Progression_Y4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Z4->SetBinContent(i+1,pt_DiTau_Progression_Z4->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_A5->SetBinContent(i+1,pt_DiTau_Progression_A5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_B5->SetBinContent(i+1,pt_DiTau_Progression_B5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_C5->SetBinContent(i+1,pt_DiTau_Progression_C5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_D5->SetBinContent(i+1,pt_DiTau_Progression_D5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_E5->SetBinContent(i+1,pt_DiTau_Progression_E5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_F5->SetBinContent(i+1,pt_DiTau_Progression_F5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_G5->SetBinContent(i+1,pt_DiTau_Progression_G5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_H5->SetBinContent(i+1,pt_DiTau_Progression_H5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_I5->SetBinContent(i+1,pt_DiTau_Progression_I5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_J5->SetBinContent(i+1,pt_DiTau_Progression_J5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_K5->SetBinContent(i+1,pt_DiTau_Progression_K5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_L5->SetBinContent(i+1,pt_DiTau_Progression_L5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_M5->SetBinContent(i+1,pt_DiTau_Progression_M5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_N5->SetBinContent(i+1,pt_DiTau_Progression_N5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_O5->SetBinContent(i+1,pt_DiTau_Progression_O5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_P5->SetBinContent(i+1,pt_DiTau_Progression_P5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Q5->SetBinContent(i+1,pt_DiTau_Progression_Q5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_R5->SetBinContent(i+1,pt_DiTau_Progression_R5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_S5->SetBinContent(i+1,pt_DiTau_Progression_S5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_T5->SetBinContent(i+1,pt_DiTau_Progression_T5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_U5->SetBinContent(i+1,pt_DiTau_Progression_U5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_V5->SetBinContent(i+1,pt_DiTau_Progression_V5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_W5->SetBinContent(i+1,pt_DiTau_Progression_W5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_X5->SetBinContent(i+1,pt_DiTau_Progression_X5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Y5->SetBinContent(i+1,pt_DiTau_Progression_Y5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Z5->SetBinContent(i+1,pt_DiTau_Progression_Z5->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_A6->SetBinContent(i+1,pt_DiTau_Progression_A6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_B6->SetBinContent(i+1,pt_DiTau_Progression_B6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_C6->SetBinContent(i+1,pt_DiTau_Progression_C6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_D6->SetBinContent(i+1,pt_DiTau_Progression_D6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_E6->SetBinContent(i+1,pt_DiTau_Progression_E6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_F6->SetBinContent(i+1,pt_DiTau_Progression_F6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_G6->SetBinContent(i+1,pt_DiTau_Progression_G6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_H6->SetBinContent(i+1,pt_DiTau_Progression_H6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_I6->SetBinContent(i+1,pt_DiTau_Progression_I6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_J6->SetBinContent(i+1,pt_DiTau_Progression_J6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_K6->SetBinContent(i+1,pt_DiTau_Progression_K6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_L6->SetBinContent(i+1,pt_DiTau_Progression_L6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_M6->SetBinContent(i+1,pt_DiTau_Progression_M6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_N6->SetBinContent(i+1,pt_DiTau_Progression_N6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_O6->SetBinContent(i+1,pt_DiTau_Progression_O6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_P6->SetBinContent(i+1,pt_DiTau_Progression_P6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Q6->SetBinContent(i+1,pt_DiTau_Progression_Q6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_R6->SetBinContent(i+1,pt_DiTau_Progression_R6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_S6->SetBinContent(i+1,pt_DiTau_Progression_S6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_T6->SetBinContent(i+1,pt_DiTau_Progression_T6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_U6->SetBinContent(i+1,pt_DiTau_Progression_U6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_V6->SetBinContent(i+1,pt_DiTau_Progression_V6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_W6->SetBinContent(i+1,pt_DiTau_Progression_W6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_X6->SetBinContent(i+1,pt_DiTau_Progression_X6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Y6->SetBinContent(i+1,pt_DiTau_Progression_Y6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Z6->SetBinContent(i+1,pt_DiTau_Progression_Z6->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_A7->SetBinContent(i+1,pt_DiTau_Progression_A7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_B7->SetBinContent(i+1,pt_DiTau_Progression_B7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_C7->SetBinContent(i+1,pt_DiTau_Progression_C7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_D7->SetBinContent(i+1,pt_DiTau_Progression_D7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_E7->SetBinContent(i+1,pt_DiTau_Progression_E7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_F7->SetBinContent(i+1,pt_DiTau_Progression_F7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_G7->SetBinContent(i+1,pt_DiTau_Progression_G7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_H7->SetBinContent(i+1,pt_DiTau_Progression_H7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_I7->SetBinContent(i+1,pt_DiTau_Progression_I7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_J7->SetBinContent(i+1,pt_DiTau_Progression_J7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_K7->SetBinContent(i+1,pt_DiTau_Progression_K7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_L7->SetBinContent(i+1,pt_DiTau_Progression_L7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_M7->SetBinContent(i+1,pt_DiTau_Progression_M7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_N7->SetBinContent(i+1,pt_DiTau_Progression_N7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_O7->SetBinContent(i+1,pt_DiTau_Progression_O7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_P7->SetBinContent(i+1,pt_DiTau_Progression_P7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Q7->SetBinContent(i+1,pt_DiTau_Progression_Q7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_R7->SetBinContent(i+1,pt_DiTau_Progression_R7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_S7->SetBinContent(i+1,pt_DiTau_Progression_S7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_T7->SetBinContent(i+1,pt_DiTau_Progression_T7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_U7->SetBinContent(i+1,pt_DiTau_Progression_U7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_V7->SetBinContent(i+1,pt_DiTau_Progression_V7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_W7->SetBinContent(i+1,pt_DiTau_Progression_W7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_X7->SetBinContent(i+1,pt_DiTau_Progression_X7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Y7->SetBinContent(i+1,pt_DiTau_Progression_Y7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Z7->SetBinContent(i+1,pt_DiTau_Progression_Z7->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_A8->SetBinContent(i+1,pt_DiTau_Progression_A8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_B8->SetBinContent(i+1,pt_DiTau_Progression_B8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_C8->SetBinContent(i+1,pt_DiTau_Progression_C8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_D8->SetBinContent(i+1,pt_DiTau_Progression_D8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_E8->SetBinContent(i+1,pt_DiTau_Progression_E8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_F8->SetBinContent(i+1,pt_DiTau_Progression_F8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_G8->SetBinContent(i+1,pt_DiTau_Progression_G8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_H8->SetBinContent(i+1,pt_DiTau_Progression_H8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_I8->SetBinContent(i+1,pt_DiTau_Progression_I8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_J8->SetBinContent(i+1,pt_DiTau_Progression_J8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_K8->SetBinContent(i+1,pt_DiTau_Progression_K8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_L8->SetBinContent(i+1,pt_DiTau_Progression_L8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_M8->SetBinContent(i+1,pt_DiTau_Progression_M8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_N8->SetBinContent(i+1,pt_DiTau_Progression_N8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_O8->SetBinContent(i+1,pt_DiTau_Progression_O8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_P8->SetBinContent(i+1,pt_DiTau_Progression_P8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Q8->SetBinContent(i+1,pt_DiTau_Progression_Q8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_R8->SetBinContent(i+1,pt_DiTau_Progression_R8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_S8->SetBinContent(i+1,pt_DiTau_Progression_S8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_T8->SetBinContent(i+1,pt_DiTau_Progression_T8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_U8->SetBinContent(i+1,pt_DiTau_Progression_U8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_V8->SetBinContent(i+1,pt_DiTau_Progression_V8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_W8->SetBinContent(i+1,pt_DiTau_Progression_W8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_X8->SetBinContent(i+1,pt_DiTau_Progression_X8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Y8->SetBinContent(i+1,pt_DiTau_Progression_Y8->Integral(i+1,241,i+1,241)/Denominator*scale);
            rate_DiTau_Progression_Z8->SetBinContent(i+1,pt_DiTau_Progression_Z8->Integral(i+1,241,i+1,241)/Denominator*scale);
            // END OF FULL BLOWN GRID SEARCH
        }


    // TH1F* rate_noCut = new TH1F("rate_noCut","rate_noCut",1200,0.,240.);
    // TH1F* rate_IsoCut_50 = new TH1F("rate_IsoCut_50","rate_IsoCut_50",1200,0.,240.);
    // TH1F* rate_IsoCut_60 = new TH1F("rate_IsoCut_60","rate_IsoCut_60",1200,0.,240.);
    // TH1F* rate_IsoCut_70 = new TH1F("rate_IsoCut_70","rate_IsoCut_70",1200,0.,240.);
    // TH1F* rate_IsoCut_80 = new TH1F("rate_IsoCut_80","rate_IsoCut_80",1200,0.,240.);
    // TH1F* rate_IsoCut_90 = new TH1F("rate_IsoCut_90","rate_IsoCut_90",1200,0.,240.);
    // TH1F* rate_Stage1 = new TH1F("rate_Stage1","rate_Stage1",1200,0.,240.);
    // TH1F* rate_Progression_1 = new TH1F("rate_Progression_1","rate_Progression_1",1200,0.,240.);
    // TH1F* rate_Progression_2 = new TH1F("rate_Progression_2","rate_Progression_2",1200,0.,240.);
    // TH1F* rate_Progression_3 = new TH1F("rate_Progression_3","rate_Progression_3",1200,0.,240.);
    // TH1F* rate_Progression_4 = new TH1F("rate_Progression_4","rate_Progression_4",1200,0.,240.);
    // TH1F* rate_Progression_5 = new TH1F("rate_Progression_5","rate_Progression_5",1200,0.,240.);
    // TH1F* rate_Progression_6 = new TH1F("rate_Progression_6","rate_Progression_6",1200,0.,240.);
    // TH1F* rate_Progression_7 = new TH1F("rate_Progression_7","rate_Progression_7",1200,0.,240.);
    // TH1F* rate_Progression_8 = new TH1F("rate_Progression_8","rate_Progression_8",1200,0.,240.);
    // TH1F* rate_Progression_9 = new TH1F("rate_Progression_9","rate_Progression_9",1200,0.,240.);
    // TH1F* rate_Progression_10 = new TH1F("rate_Progression_10","rate_Progression_10",1200,0.,240.);
    // TH1F* rate_Progression_11 = new TH1F("rate_Progression_11","rate_Progression_11",1200,0.,240.);
    // TH1F* rate_Progression_12 = new TH1F("rate_Progression_12","rate_Progression_12",1200,0.,240.);
    // TH1F* rate_Progression_13 = new TH1F("rate_Progression_13","rate_Progression_13",1200,0.,240.);
    // TH1F* rate_Progression_14 = new TH1F("rate_Progression_14","rate_Progression_14",1200,0.,240.);
    // TH1F* rate_Progression_15 = new TH1F("rate_Progression_15","rate_Progression_15",1200,0.,240.);
    // TH1F* rate_Progression_16 = new TH1F("rate_Progression_16","rate_Progression_16",1200,0.,240.);
    // TH1F* rate_Progression_17 = new TH1F("rate_Progression_17","rate_Progression_17",1200,0.,240.);
    // TH1F* rate_Progression_18 = new TH1F("rate_Progression_18","rate_Progression_18",1200,0.,240.);
    // TH1F* rate_Progression_19 = new TH1F("rate_Progression_19","rate_Progression_19",1200,0.,240.);
    // TH1F* rate_Progression_20 = new TH1F("rate_Progression_20","rate_Progression_20",1200,0.,240.);
    // TH1F* rate_Progression_21 = new TH1F("rate_Progression_21","rate_Progression_21",1200,0.,240.);

    // TH1F* rate_noCut_DiTau = new TH1F("rate_noCut_DiTau","rate_noCut_DiTau",1200,0.,240.);
    // TH1F* rate_DiTau_90 = new TH1F("rate_DiTau_90","rate_DiTau_90",1200,0.,240.);
    // TH1F* rate_DiTau_80 = new TH1F("rate_DiTau_80","rate_DiTau_80",1200,0.,240.);
    // TH1F* rate_DiTau_70 = new TH1F("rate_DiTau_70","rate_DiTau_70",1200,0.,240.);
    // TH1F* rate_DiTau_60 = new TH1F("rate_DiTau_60","rate_DiTau_60",1200,0.,240.);
    // TH1F* rate_DiTau_50 = new TH1F("rate_DiTau_50","rate_DiTau_50",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_1 = new TH1F("rate_DiTau_Progression_1","rate_DiTau_Progression_1",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_2 = new TH1F("rate_DiTau_Progression_2","rate_DiTau_Progression_2",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_3 = new TH1F("rate_DiTau_Progression_3","rate_DiTau_Progression_3",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_4 = new TH1F("rate_DiTau_Progression_4","rate_DiTau_Progression_4",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_5 = new TH1F("rate_DiTau_Progression_5","rate_DiTau_Progression_5",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_6 = new TH1F("rate_DiTau_Progression_6","rate_DiTau_Progression_6",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_7 = new TH1F("rate_DiTau_Progression_7","rate_DiTau_Progression_7",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_8 = new TH1F("rate_DiTau_Progression_8","rate_DiTau_Progression_8",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_9 = new TH1F("rate_DiTau_Progression_9","rate_DiTau_Progression_9",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_10 = new TH1F("rate_DiTau_Progression_10","rate_DiTau_Progression_10",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_11 = new TH1F("rate_DiTau_Progression_11","rate_DiTau_Progression_11",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_12 = new TH1F("rate_DiTau_Progression_12","rate_DiTau_Progression_12",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_13 = new TH1F("rate_DiTau_Progression_13","rate_DiTau_Progression_13",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_14 = new TH1F("rate_DiTau_Progression_14","rate_DiTau_Progression_14",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_15 = new TH1F("rate_DiTau_Progression_15","rate_DiTau_Progression_15",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_16 = new TH1F("rate_DiTau_Progression_16","rate_DiTau_Progression_16",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_17 = new TH1F("rate_DiTau_Progression_17","rate_DiTau_Progression_17",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_18 = new TH1F("rate_DiTau_Progression_18","rate_DiTau_Progression_18",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_19 = new TH1F("rate_DiTau_Progression_19","rate_DiTau_Progression_19",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_20 = new TH1F("rate_DiTau_Progression_20","rate_DiTau_Progression_20",1200,0.,240.);
    // TH1F* rate_DiTau_Progression_21 = new TH1F("rate_DiTau_Progression_21","rate_DiTau_Progression_21",1200,0.,240.);

    // for(UInt_t i = 0 ; i < 1201 ; ++i)
    //   {
    //     rate_noCut->SetBinContent(i+1,pt_IsoInf->Integral(i+1,1201)/Denominator*scale);
    //     rate_IsoCut_50->SetBinContent(i+1,pt_Iso_50->Integral(i+1,1201)/Denominator*scale);
    //     rate_IsoCut_60->SetBinContent(i+1,pt_Iso_60->Integral(i+1,1201)/Denominator*scale);
    //     rate_IsoCut_70->SetBinContent(i+1,pt_Iso_70->Integral(i+1,1201)/Denominator*scale);
    //     rate_IsoCut_80->SetBinContent(i+1,pt_Iso_80->Integral(i+1,1201)/Denominator*scale);
    //     rate_IsoCut_90->SetBinContent(i+1,pt_Iso_90->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_1->SetBinContent(i+1,pt_Progression_1->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_2->SetBinContent(i+1,pt_Progression_2->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_3->SetBinContent(i+1,pt_Progression_3->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_4->SetBinContent(i+1,pt_Progression_4->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_5->SetBinContent(i+1,pt_Progression_5->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_6->SetBinContent(i+1,pt_Progression_6->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_7->SetBinContent(i+1,pt_Progression_7->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_8->SetBinContent(i+1,pt_Progression_8->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_9->SetBinContent(i+1,pt_Progression_9->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_10->SetBinContent(i+1,pt_Progression_10->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_11->SetBinContent(i+1,pt_Progression_11->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_12->SetBinContent(i+1,pt_Progression_12->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_13->SetBinContent(i+1,pt_Progression_13->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_14->SetBinContent(i+1,pt_Progression_14->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_15->SetBinContent(i+1,pt_Progression_15->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_16->SetBinContent(i+1,pt_Progression_16->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_17->SetBinContent(i+1,pt_Progression_17->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_18->SetBinContent(i+1,pt_Progression_18->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_19->SetBinContent(i+1,pt_Progression_19->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_20->SetBinContent(i+1,pt_Progression_20->Integral(i+1,1201)/Denominator*scale);
    //     rate_Progression_21->SetBinContent(i+1,pt_Progression_21->Integral(i+1,1201)/Denominator*scale);

    //     rate_noCut_DiTau->SetBinContent(i+1,pt_IsoInf_DiTau->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_90->SetBinContent(i+1,pt_DiTau_90pc->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_80->SetBinContent(i+1,pt_DiTau_80pc->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_70->SetBinContent(i+1,pt_DiTau_70pc->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_60->SetBinContent(i+1,pt_DiTau_60pc->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_50->SetBinContent(i+1,pt_DiTau_50pc->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_1->SetBinContent(i+1,pt_DiTau_Progression_1->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_2->SetBinContent(i+1,pt_DiTau_Progression_2->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_3->SetBinContent(i+1,pt_DiTau_Progression_3->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_4->SetBinContent(i+1,pt_DiTau_Progression_4->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_5->SetBinContent(i+1,pt_DiTau_Progression_5->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_6->SetBinContent(i+1,pt_DiTau_Progression_6->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_7->SetBinContent(i+1,pt_DiTau_Progression_7->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_8->SetBinContent(i+1,pt_DiTau_Progression_8->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_9->SetBinContent(i+1,pt_DiTau_Progression_9->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_10->SetBinContent(i+1,pt_DiTau_Progression_10->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_11->SetBinContent(i+1,pt_DiTau_Progression_11->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_12->SetBinContent(i+1,pt_DiTau_Progression_12->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_13->SetBinContent(i+1,pt_DiTau_Progression_13->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_14->SetBinContent(i+1,pt_DiTau_Progression_14->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_15->SetBinContent(i+1,pt_DiTau_Progression_15->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_16->SetBinContent(i+1,pt_DiTau_Progression_16->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_17->SetBinContent(i+1,pt_DiTau_Progression_17->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_18->SetBinContent(i+1,pt_DiTau_Progression_18->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_19->SetBinContent(i+1,pt_DiTau_Progression_19->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_20->SetBinContent(i+1,pt_DiTau_Progression_20->Integral(i+1,1201,i+1,1201)/Denominator*scale);
    //     rate_DiTau_Progression_21->SetBinContent(i+1,pt_DiTau_Progression_21->Integral(i+1,1201,i+1,1201)/Denominator*scale);

    //     // rate_IsoCut_Linear->SetBinContent(i+1,pt_IsoLinear->Integral(i+1,1201)/Denominator*scale);
    //     // rate_IsoCut_Linear_DiTau->SetBinContent(i+1,pt_IsoLinear_DiTau->Integral(i+1,1201)/Denominator*scale);
    //     //rate_Stage1->SetBinContent(i+1,pt_Stage1->Integral(i+1,1201)/dataStage1.GetEntries()*scale);
    //   }

    TFile f("histos/histos_rate_ZeroBias_Run"+run_str+"_optimizationV3gs_calibThr"+intgr+"p"+decim+"_"+tag+".root","RECREATE");
    Iso_MinBias->Write();
    Correction_Factor->Write();
    Correction_Factor_IEt_30->Write();


    h_pt_calibrated->Write();
    h_pt_uncalibrated->Write();

    pt_IsoInf->Write();
    pt_Iso_50->Write();
    pt_Iso_60->Write();
    pt_Iso_70->Write();
    pt_Iso_80->Write();
    pt_Iso_90->Write();
    pt_Progression_1->Write();
    pt_Progression_2->Write();
    pt_Progression_3->Write();
    pt_Progression_4->Write();
    pt_Progression_5->Write();
    pt_Progression_6->Write();
    pt_Progression_7->Write();
    pt_Progression_8->Write();
    pt_Progression_9->Write();
    pt_Progression_10->Write();
    pt_Progression_11->Write();
    pt_Progression_12->Write();
    pt_Progression_13->Write();
    pt_Progression_14->Write();
    pt_Progression_15->Write();
    pt_Progression_16->Write();
    pt_Progression_17->Write();
    pt_Progression_18->Write();
    pt_Progression_19->Write();
    pt_Progression_20->Write();
    pt_Progression_21->Write();
    pt_Progression_22->Write();

    // START OF FULL BLOWN GRID SEARCH
    pt_Progression_A0->Write();
    pt_Progression_B0->Write();
    pt_Progression_C0->Write();
    pt_Progression_D0->Write();
    pt_Progression_E0->Write();
    pt_Progression_F0->Write();
    pt_Progression_G0->Write();
    pt_Progression_H0->Write();
    pt_Progression_I0->Write();
    pt_Progression_J0->Write();
    pt_Progression_K0->Write();
    pt_Progression_L0->Write();
    pt_Progression_M0->Write();
    pt_Progression_N0->Write();
    pt_Progression_O0->Write();
    pt_Progression_P0->Write();
    pt_Progression_Q0->Write();
    pt_Progression_R0->Write();
    pt_Progression_S0->Write();
    pt_Progression_T0->Write();
    pt_Progression_U0->Write();
    pt_Progression_V0->Write();
    pt_Progression_W0->Write();
    pt_Progression_X0->Write();
    pt_Progression_Y0->Write();
    pt_Progression_Z0->Write();
    pt_Progression_A1->Write();
    pt_Progression_B1->Write();
    pt_Progression_C1->Write();
    pt_Progression_D1->Write();
    pt_Progression_E1->Write();
    pt_Progression_F1->Write();
    pt_Progression_G1->Write();
    pt_Progression_H1->Write();
    pt_Progression_I1->Write();
    pt_Progression_J1->Write();
    pt_Progression_K1->Write();
    pt_Progression_L1->Write();
    pt_Progression_M1->Write();
    pt_Progression_N1->Write();
    pt_Progression_O1->Write();
    pt_Progression_P1->Write();
    pt_Progression_Q1->Write();
    pt_Progression_R1->Write();
    pt_Progression_S1->Write();
    pt_Progression_T1->Write();
    pt_Progression_U1->Write();
    pt_Progression_V1->Write();
    pt_Progression_W1->Write();
    pt_Progression_X1->Write();
    pt_Progression_Y1->Write();
    pt_Progression_Z1->Write();
    pt_Progression_A2->Write();
    pt_Progression_B2->Write();
    pt_Progression_C2->Write();
    pt_Progression_D2->Write();
    pt_Progression_E2->Write();
    pt_Progression_F2->Write();
    pt_Progression_G2->Write();
    pt_Progression_H2->Write();
    pt_Progression_I2->Write();
    pt_Progression_J2->Write();
    pt_Progression_K2->Write();
    pt_Progression_L2->Write();
    pt_Progression_M2->Write();
    pt_Progression_N2->Write();
    pt_Progression_O2->Write();
    pt_Progression_P2->Write();
    pt_Progression_Q2->Write();
    pt_Progression_R2->Write();
    pt_Progression_S2->Write();
    pt_Progression_T2->Write();
    pt_Progression_U2->Write();
    pt_Progression_V2->Write();
    pt_Progression_W2->Write();
    pt_Progression_X2->Write();
    pt_Progression_Y2->Write();
    pt_Progression_Z2->Write();
    pt_Progression_A3->Write();
    pt_Progression_B3->Write();
    pt_Progression_C3->Write();
    pt_Progression_D3->Write();
    pt_Progression_E3->Write();
    pt_Progression_F3->Write();
    pt_Progression_G3->Write();
    pt_Progression_H3->Write();
    pt_Progression_I3->Write();
    pt_Progression_J3->Write();
    pt_Progression_K3->Write();
    pt_Progression_L3->Write();
    pt_Progression_M3->Write();
    pt_Progression_N3->Write();
    pt_Progression_O3->Write();
    pt_Progression_P3->Write();
    pt_Progression_Q3->Write();
    pt_Progression_R3->Write();
    pt_Progression_S3->Write();
    pt_Progression_T3->Write();
    pt_Progression_U3->Write();
    pt_Progression_V3->Write();
    pt_Progression_W3->Write();
    pt_Progression_X3->Write();
    pt_Progression_Y3->Write();
    pt_Progression_Z3->Write();
    pt_Progression_A4->Write();
    pt_Progression_B4->Write();
    pt_Progression_C4->Write();
    pt_Progression_D4->Write();
    pt_Progression_E4->Write();
    pt_Progression_F4->Write();
    pt_Progression_G4->Write();
    pt_Progression_H4->Write();
    pt_Progression_I4->Write();
    pt_Progression_J4->Write();
    pt_Progression_K4->Write();
    pt_Progression_L4->Write();
    pt_Progression_M4->Write();
    pt_Progression_N4->Write();
    pt_Progression_O4->Write();
    pt_Progression_P4->Write();
    pt_Progression_Q4->Write();
    pt_Progression_R4->Write();
    pt_Progression_S4->Write();
    pt_Progression_T4->Write();
    pt_Progression_U4->Write();
    pt_Progression_V4->Write();
    pt_Progression_W4->Write();
    pt_Progression_X4->Write();
    pt_Progression_Y4->Write();
    pt_Progression_Z4->Write();
    pt_Progression_A5->Write();
    pt_Progression_B5->Write();
    pt_Progression_C5->Write();
    pt_Progression_D5->Write();
    pt_Progression_E5->Write();
    pt_Progression_F5->Write();
    pt_Progression_G5->Write();
    pt_Progression_H5->Write();
    pt_Progression_I5->Write();
    pt_Progression_J5->Write();
    pt_Progression_K5->Write();
    pt_Progression_L5->Write();
    pt_Progression_M5->Write();
    pt_Progression_N5->Write();
    pt_Progression_O5->Write();
    pt_Progression_P5->Write();
    pt_Progression_Q5->Write();
    pt_Progression_R5->Write();
    pt_Progression_S5->Write();
    pt_Progression_T5->Write();
    pt_Progression_U5->Write();
    pt_Progression_V5->Write();
    pt_Progression_W5->Write();
    pt_Progression_X5->Write();
    pt_Progression_Y5->Write();
    pt_Progression_Z5->Write();
    pt_Progression_A6->Write();
    pt_Progression_B6->Write();
    pt_Progression_C6->Write();
    pt_Progression_D6->Write();
    pt_Progression_E6->Write();
    pt_Progression_F6->Write();
    pt_Progression_G6->Write();
    pt_Progression_H6->Write();
    pt_Progression_I6->Write();
    pt_Progression_J6->Write();
    pt_Progression_K6->Write();
    pt_Progression_L6->Write();
    pt_Progression_M6->Write();
    pt_Progression_N6->Write();
    pt_Progression_O6->Write();
    pt_Progression_P6->Write();
    pt_Progression_Q6->Write();
    pt_Progression_R6->Write();
    pt_Progression_S6->Write();
    pt_Progression_T6->Write();
    pt_Progression_U6->Write();
    pt_Progression_V6->Write();
    pt_Progression_W6->Write();
    pt_Progression_X6->Write();
    pt_Progression_Y6->Write();
    pt_Progression_Z6->Write();
    pt_Progression_A7->Write();
    pt_Progression_B7->Write();
    pt_Progression_C7->Write();
    pt_Progression_D7->Write();
    pt_Progression_E7->Write();
    pt_Progression_F7->Write();
    pt_Progression_G7->Write();
    pt_Progression_H7->Write();
    pt_Progression_I7->Write();
    pt_Progression_J7->Write();
    pt_Progression_K7->Write();
    pt_Progression_L7->Write();
    pt_Progression_M7->Write();
    pt_Progression_N7->Write();
    pt_Progression_O7->Write();
    pt_Progression_P7->Write();
    pt_Progression_Q7->Write();
    pt_Progression_R7->Write();
    pt_Progression_S7->Write();
    pt_Progression_T7->Write();
    pt_Progression_U7->Write();
    pt_Progression_V7->Write();
    pt_Progression_W7->Write();
    pt_Progression_X7->Write();
    pt_Progression_Y7->Write();
    pt_Progression_Z7->Write();
    pt_Progression_A8->Write();
    pt_Progression_B8->Write();
    pt_Progression_C8->Write();
    pt_Progression_D8->Write();
    pt_Progression_E8->Write();
    pt_Progression_F8->Write();
    pt_Progression_G8->Write();
    pt_Progression_H8->Write();
    pt_Progression_I8->Write();
    pt_Progression_J8->Write();
    pt_Progression_K8->Write();
    pt_Progression_L8->Write();
    pt_Progression_M8->Write();
    pt_Progression_N8->Write();
    pt_Progression_O8->Write();
    pt_Progression_P8->Write();
    pt_Progression_Q8->Write();
    pt_Progression_R8->Write();
    pt_Progression_S8->Write();
    pt_Progression_T8->Write();
    pt_Progression_U8->Write();
    pt_Progression_V8->Write();
    pt_Progression_W8->Write();
    pt_Progression_X8->Write();
    pt_Progression_Y8->Write();
    pt_Progression_Z8->Write();
    // END OF FULL BLOWN GRID SEARCH

    pt_IsoInf_DiTau->Write();
    pt_DiTau_90pc->Write();
    pt_DiTau_80pc->Write();
    pt_DiTau_70pc->Write();
    pt_DiTau_60pc->Write();
    pt_DiTau_50pc->Write();
    pt_DiTau_Progression_1->Write();
    pt_DiTau_Progression_2->Write();
    pt_DiTau_Progression_4->Write();
    pt_DiTau_Progression_5->Write();
    pt_DiTau_Progression_6->Write();
    pt_DiTau_Progression_7->Write();
    pt_DiTau_Progression_8->Write();
    pt_DiTau_Progression_9->Write();
    pt_DiTau_Progression_10->Write();
    pt_DiTau_Progression_11->Write();
    pt_DiTau_Progression_12->Write();
    pt_DiTau_Progression_13->Write();
    pt_DiTau_Progression_14->Write();
    pt_DiTau_Progression_15->Write();
    pt_DiTau_Progression_16->Write();
    pt_DiTau_Progression_17->Write();
    pt_DiTau_Progression_18->Write();
    pt_DiTau_Progression_19->Write();
    pt_DiTau_Progression_20->Write();
    pt_DiTau_Progression_21->Write();
    pt_DiTau_Progression_22->Write();

    pt_DiTau_Progression_21->Write();
    pt_DiTau_Progression_22->Write();
    pt_DiTau_Progression_23->Write();
    pt_DiTau_Progression_24->Write();
    pt_DiTau_Progression_25->Write();
    pt_DiTau_Progression_26->Write();
    pt_DiTau_Progression_27->Write();
    pt_DiTau_Progression_28->Write();
    pt_DiTau_Progression_29->Write();
    pt_DiTau_Progression_30->Write();
    pt_DiTau_Progression_31->Write();
    pt_DiTau_Progression_31_barrel->Write();
    pt_DiTau_Progression_31_barrelendcap->Write();
    pt_DiTau_Progression_31_endcaps->Write();
    pt_DiTau_Progression_31_extrap->Write();
    pt_DiTau_Progression_31_extrap_barrel->Write();
    pt_DiTau_Progression_31_extrap_barrelendcap->Write();
    pt_DiTau_Progression_31_extrap_endcaps->Write();
    // pt_IsoLinear->Write();
    // pt_IsoLinear_DiTau->Write();

    // START OF FULL BLOWN GRID SEARCH
    pt_DiTau_Progression_A0->Write();
    pt_DiTau_Progression_B0->Write();
    pt_DiTau_Progression_C0->Write();
    pt_DiTau_Progression_D0->Write();
    pt_DiTau_Progression_E0->Write();
    pt_DiTau_Progression_F0->Write();
    pt_DiTau_Progression_G0->Write();
    pt_DiTau_Progression_H0->Write();
    pt_DiTau_Progression_I0->Write();
    pt_DiTau_Progression_J0->Write();
    pt_DiTau_Progression_K0->Write();
    pt_DiTau_Progression_L0->Write();
    pt_DiTau_Progression_M0->Write();
    pt_DiTau_Progression_N0->Write();
    pt_DiTau_Progression_O0->Write();
    pt_DiTau_Progression_P0->Write();
    pt_DiTau_Progression_Q0->Write();
    pt_DiTau_Progression_R0->Write();
    pt_DiTau_Progression_S0->Write();
    pt_DiTau_Progression_T0->Write();
    pt_DiTau_Progression_U0->Write();
    pt_DiTau_Progression_V0->Write();
    pt_DiTau_Progression_W0->Write();
    pt_DiTau_Progression_X0->Write();
    pt_DiTau_Progression_Y0->Write();
    pt_DiTau_Progression_Z0->Write();
    pt_DiTau_Progression_A1->Write();
    pt_DiTau_Progression_B1->Write();
    pt_DiTau_Progression_C1->Write();
    pt_DiTau_Progression_D1->Write();
    pt_DiTau_Progression_E1->Write();
    pt_DiTau_Progression_F1->Write();
    pt_DiTau_Progression_G1->Write();
    pt_DiTau_Progression_H1->Write();
    pt_DiTau_Progression_I1->Write();
    pt_DiTau_Progression_J1->Write();
    pt_DiTau_Progression_K1->Write();
    pt_DiTau_Progression_L1->Write();
    pt_DiTau_Progression_M1->Write();
    pt_DiTau_Progression_N1->Write();
    pt_DiTau_Progression_O1->Write();
    pt_DiTau_Progression_P1->Write();
    pt_DiTau_Progression_Q1->Write();
    pt_DiTau_Progression_R1->Write();
    pt_DiTau_Progression_S1->Write();
    pt_DiTau_Progression_T1->Write();
    pt_DiTau_Progression_U1->Write();
    pt_DiTau_Progression_V1->Write();
    pt_DiTau_Progression_W1->Write();
    pt_DiTau_Progression_X1->Write();
    pt_DiTau_Progression_Y1->Write();
    pt_DiTau_Progression_Z1->Write();
    pt_DiTau_Progression_A2->Write();
    pt_DiTau_Progression_B2->Write();
    pt_DiTau_Progression_C2->Write();
    pt_DiTau_Progression_D2->Write();
    pt_DiTau_Progression_E2->Write();
    pt_DiTau_Progression_F2->Write();
    pt_DiTau_Progression_G2->Write();
    pt_DiTau_Progression_H2->Write();
    pt_DiTau_Progression_I2->Write();
    pt_DiTau_Progression_J2->Write();
    pt_DiTau_Progression_K2->Write();
    pt_DiTau_Progression_L2->Write();
    pt_DiTau_Progression_M2->Write();
    pt_DiTau_Progression_N2->Write();
    pt_DiTau_Progression_O2->Write();
    pt_DiTau_Progression_P2->Write();
    pt_DiTau_Progression_Q2->Write();
    pt_DiTau_Progression_R2->Write();
    pt_DiTau_Progression_S2->Write();
    pt_DiTau_Progression_T2->Write();
    pt_DiTau_Progression_U2->Write();
    pt_DiTau_Progression_V2->Write();
    pt_DiTau_Progression_W2->Write();
    pt_DiTau_Progression_X2->Write();
    pt_DiTau_Progression_Y2->Write();
    pt_DiTau_Progression_Z2->Write();
    pt_DiTau_Progression_A3->Write();
    pt_DiTau_Progression_B3->Write();
    pt_DiTau_Progression_C3->Write();
    pt_DiTau_Progression_D3->Write();
    pt_DiTau_Progression_E3->Write();
    pt_DiTau_Progression_F3->Write();
    pt_DiTau_Progression_G3->Write();
    pt_DiTau_Progression_H3->Write();
    pt_DiTau_Progression_I3->Write();
    pt_DiTau_Progression_J3->Write();
    pt_DiTau_Progression_K3->Write();
    pt_DiTau_Progression_L3->Write();
    pt_DiTau_Progression_M3->Write();
    pt_DiTau_Progression_N3->Write();
    pt_DiTau_Progression_O3->Write();
    pt_DiTau_Progression_P3->Write();
    pt_DiTau_Progression_Q3->Write();
    pt_DiTau_Progression_R3->Write();
    pt_DiTau_Progression_S3->Write();
    pt_DiTau_Progression_T3->Write();
    pt_DiTau_Progression_U3->Write();
    pt_DiTau_Progression_V3->Write();
    pt_DiTau_Progression_W3->Write();
    pt_DiTau_Progression_X3->Write();
    pt_DiTau_Progression_Y3->Write();
    pt_DiTau_Progression_Z3->Write();
    pt_DiTau_Progression_A4->Write();
    pt_DiTau_Progression_B4->Write();
    pt_DiTau_Progression_C4->Write();
    pt_DiTau_Progression_D4->Write();
    pt_DiTau_Progression_E4->Write();
    pt_DiTau_Progression_F4->Write();
    pt_DiTau_Progression_G4->Write();
    pt_DiTau_Progression_H4->Write();
    pt_DiTau_Progression_I4->Write();
    pt_DiTau_Progression_J4->Write();
    pt_DiTau_Progression_K4->Write();
    pt_DiTau_Progression_L4->Write();
    pt_DiTau_Progression_M4->Write();
    pt_DiTau_Progression_N4->Write();
    pt_DiTau_Progression_O4->Write();
    pt_DiTau_Progression_P4->Write();
    pt_DiTau_Progression_Q4->Write();
    pt_DiTau_Progression_R4->Write();
    pt_DiTau_Progression_S4->Write();
    pt_DiTau_Progression_T4->Write();
    pt_DiTau_Progression_U4->Write();
    pt_DiTau_Progression_V4->Write();
    pt_DiTau_Progression_W4->Write();
    pt_DiTau_Progression_X4->Write();
    pt_DiTau_Progression_Y4->Write();
    pt_DiTau_Progression_Z4->Write();
    pt_DiTau_Progression_A5->Write();
    pt_DiTau_Progression_B5->Write();
    pt_DiTau_Progression_C5->Write();
    pt_DiTau_Progression_D5->Write();
    pt_DiTau_Progression_E5->Write();
    pt_DiTau_Progression_F5->Write();
    pt_DiTau_Progression_G5->Write();
    pt_DiTau_Progression_H5->Write();
    pt_DiTau_Progression_I5->Write();
    pt_DiTau_Progression_J5->Write();
    pt_DiTau_Progression_K5->Write();
    pt_DiTau_Progression_L5->Write();
    pt_DiTau_Progression_M5->Write();
    pt_DiTau_Progression_N5->Write();
    pt_DiTau_Progression_O5->Write();
    pt_DiTau_Progression_P5->Write();
    pt_DiTau_Progression_Q5->Write();
    pt_DiTau_Progression_R5->Write();
    pt_DiTau_Progression_S5->Write();
    pt_DiTau_Progression_T5->Write();
    pt_DiTau_Progression_U5->Write();
    pt_DiTau_Progression_V5->Write();
    pt_DiTau_Progression_W5->Write();
    pt_DiTau_Progression_X5->Write();
    pt_DiTau_Progression_Y5->Write();
    pt_DiTau_Progression_Z5->Write();
    pt_DiTau_Progression_A6->Write();
    pt_DiTau_Progression_B6->Write();
    pt_DiTau_Progression_C6->Write();
    pt_DiTau_Progression_D6->Write();
    pt_DiTau_Progression_E6->Write();
    pt_DiTau_Progression_F6->Write();
    pt_DiTau_Progression_G6->Write();
    pt_DiTau_Progression_H6->Write();
    pt_DiTau_Progression_I6->Write();
    pt_DiTau_Progression_J6->Write();
    pt_DiTau_Progression_K6->Write();
    pt_DiTau_Progression_L6->Write();
    pt_DiTau_Progression_M6->Write();
    pt_DiTau_Progression_N6->Write();
    pt_DiTau_Progression_O6->Write();
    pt_DiTau_Progression_P6->Write();
    pt_DiTau_Progression_Q6->Write();
    pt_DiTau_Progression_R6->Write();
    pt_DiTau_Progression_S6->Write();
    pt_DiTau_Progression_T6->Write();
    pt_DiTau_Progression_U6->Write();
    pt_DiTau_Progression_V6->Write();
    pt_DiTau_Progression_W6->Write();
    pt_DiTau_Progression_X6->Write();
    pt_DiTau_Progression_Y6->Write();
    pt_DiTau_Progression_Z6->Write();
    pt_DiTau_Progression_A7->Write();
    pt_DiTau_Progression_B7->Write();
    pt_DiTau_Progression_C7->Write();
    pt_DiTau_Progression_D7->Write();
    pt_DiTau_Progression_E7->Write();
    pt_DiTau_Progression_F7->Write();
    pt_DiTau_Progression_G7->Write();
    pt_DiTau_Progression_H7->Write();
    pt_DiTau_Progression_I7->Write();
    pt_DiTau_Progression_J7->Write();
    pt_DiTau_Progression_K7->Write();
    pt_DiTau_Progression_L7->Write();
    pt_DiTau_Progression_M7->Write();
    pt_DiTau_Progression_N7->Write();
    pt_DiTau_Progression_O7->Write();
    pt_DiTau_Progression_P7->Write();
    pt_DiTau_Progression_Q7->Write();
    pt_DiTau_Progression_R7->Write();
    pt_DiTau_Progression_S7->Write();
    pt_DiTau_Progression_T7->Write();
    pt_DiTau_Progression_U7->Write();
    pt_DiTau_Progression_V7->Write();
    pt_DiTau_Progression_W7->Write();
    pt_DiTau_Progression_X7->Write();
    pt_DiTau_Progression_Y7->Write();
    pt_DiTau_Progression_Z7->Write();
    pt_DiTau_Progression_A8->Write();
    pt_DiTau_Progression_B8->Write();
    pt_DiTau_Progression_C8->Write();
    pt_DiTau_Progression_D8->Write();
    pt_DiTau_Progression_E8->Write();
    pt_DiTau_Progression_F8->Write();
    pt_DiTau_Progression_G8->Write();
    pt_DiTau_Progression_H8->Write();
    pt_DiTau_Progression_I8->Write();
    pt_DiTau_Progression_J8->Write();
    pt_DiTau_Progression_K8->Write();
    pt_DiTau_Progression_L8->Write();
    pt_DiTau_Progression_M8->Write();
    pt_DiTau_Progression_N8->Write();
    pt_DiTau_Progression_O8->Write();
    pt_DiTau_Progression_P8->Write();
    pt_DiTau_Progression_Q8->Write();
    pt_DiTau_Progression_R8->Write();
    pt_DiTau_Progression_S8->Write();
    pt_DiTau_Progression_T8->Write();
    pt_DiTau_Progression_U8->Write();
    pt_DiTau_Progression_V8->Write();
    pt_DiTau_Progression_W8->Write();
    pt_DiTau_Progression_X8->Write();
    pt_DiTau_Progression_Y8->Write();
    pt_DiTau_Progression_Z8->Write();
    // END OF FULL BLOWN GRID SEARCH

    rate_noCut->Write();
    rate_IsoCut_50->Write();
    rate_IsoCut_60->Write();
    rate_IsoCut_70->Write();  
    rate_IsoCut_80->Write();  
    rate_IsoCut_90->Write();  
    rate_Progression_1->Write();  
    rate_Progression_2->Write();  
    rate_Progression_3->Write();  
    rate_Progression_4->Write();  
    rate_Progression_5->Write();  
    rate_Progression_6->Write();  
    rate_Progression_7->Write();  
    rate_Progression_8->Write();  
    rate_Progression_9->Write();  
    rate_Progression_10->Write();  
    rate_Progression_11->Write();  
    rate_Progression_12->Write();  
    rate_Progression_13->Write();  
    rate_Progression_14->Write();  
    rate_Progression_15->Write();  
    rate_Progression_16->Write();  
    rate_Progression_17->Write();  
    rate_Progression_18->Write();  
    rate_Progression_19->Write();  
    rate_Progression_20->Write();  
    rate_Progression_21->Write();  
    rate_Progression_22->Write();

    // START OF FULL BLOWN GRID SEARCH
    rate_Progression_A0->Write();
    rate_Progression_B0->Write();
    rate_Progression_C0->Write();
    rate_Progression_D0->Write();
    rate_Progression_E0->Write();
    rate_Progression_F0->Write();
    rate_Progression_G0->Write();
    rate_Progression_H0->Write();
    rate_Progression_I0->Write();
    rate_Progression_J0->Write();
    rate_Progression_K0->Write();
    rate_Progression_L0->Write();
    rate_Progression_M0->Write();
    rate_Progression_N0->Write();
    rate_Progression_O0->Write();
    rate_Progression_P0->Write();
    rate_Progression_Q0->Write();
    rate_Progression_R0->Write();
    rate_Progression_S0->Write();
    rate_Progression_T0->Write();
    rate_Progression_U0->Write();
    rate_Progression_V0->Write();
    rate_Progression_W0->Write();
    rate_Progression_X0->Write();
    rate_Progression_Y0->Write();
    rate_Progression_Z0->Write();
    rate_Progression_A1->Write();
    rate_Progression_B1->Write();
    rate_Progression_C1->Write();
    rate_Progression_D1->Write();
    rate_Progression_E1->Write();
    rate_Progression_F1->Write();
    rate_Progression_G1->Write();
    rate_Progression_H1->Write();
    rate_Progression_I1->Write();
    rate_Progression_J1->Write();
    rate_Progression_K1->Write();
    rate_Progression_L1->Write();
    rate_Progression_M1->Write();
    rate_Progression_N1->Write();
    rate_Progression_O1->Write();
    rate_Progression_P1->Write();
    rate_Progression_Q1->Write();
    rate_Progression_R1->Write();
    rate_Progression_S1->Write();
    rate_Progression_T1->Write();
    rate_Progression_U1->Write();
    rate_Progression_V1->Write();
    rate_Progression_W1->Write();
    rate_Progression_X1->Write();
    rate_Progression_Y1->Write();
    rate_Progression_Z1->Write();
    rate_Progression_A2->Write();
    rate_Progression_B2->Write();
    rate_Progression_C2->Write();
    rate_Progression_D2->Write();
    rate_Progression_E2->Write();
    rate_Progression_F2->Write();
    rate_Progression_G2->Write();
    rate_Progression_H2->Write();
    rate_Progression_I2->Write();
    rate_Progression_J2->Write();
    rate_Progression_K2->Write();
    rate_Progression_L2->Write();
    rate_Progression_M2->Write();
    rate_Progression_N2->Write();
    rate_Progression_O2->Write();
    rate_Progression_P2->Write();
    rate_Progression_Q2->Write();
    rate_Progression_R2->Write();
    rate_Progression_S2->Write();
    rate_Progression_T2->Write();
    rate_Progression_U2->Write();
    rate_Progression_V2->Write();
    rate_Progression_W2->Write();
    rate_Progression_X2->Write();
    rate_Progression_Y2->Write();
    rate_Progression_Z2->Write();
    rate_Progression_A3->Write();
    rate_Progression_B3->Write();
    rate_Progression_C3->Write();
    rate_Progression_D3->Write();
    rate_Progression_E3->Write();
    rate_Progression_F3->Write();
    rate_Progression_G3->Write();
    rate_Progression_H3->Write();
    rate_Progression_I3->Write();
    rate_Progression_J3->Write();
    rate_Progression_K3->Write();
    rate_Progression_L3->Write();
    rate_Progression_M3->Write();
    rate_Progression_N3->Write();
    rate_Progression_O3->Write();
    rate_Progression_P3->Write();
    rate_Progression_Q3->Write();
    rate_Progression_R3->Write();
    rate_Progression_S3->Write();
    rate_Progression_T3->Write();
    rate_Progression_U3->Write();
    rate_Progression_V3->Write();
    rate_Progression_W3->Write();
    rate_Progression_X3->Write();
    rate_Progression_Y3->Write();
    rate_Progression_Z3->Write();
    rate_Progression_A4->Write();
    rate_Progression_B4->Write();
    rate_Progression_C4->Write();
    rate_Progression_D4->Write();
    rate_Progression_E4->Write();
    rate_Progression_F4->Write();
    rate_Progression_G4->Write();
    rate_Progression_H4->Write();
    rate_Progression_I4->Write();
    rate_Progression_J4->Write();
    rate_Progression_K4->Write();
    rate_Progression_L4->Write();
    rate_Progression_M4->Write();
    rate_Progression_N4->Write();
    rate_Progression_O4->Write();
    rate_Progression_P4->Write();
    rate_Progression_Q4->Write();
    rate_Progression_R4->Write();
    rate_Progression_S4->Write();
    rate_Progression_T4->Write();
    rate_Progression_U4->Write();
    rate_Progression_V4->Write();
    rate_Progression_W4->Write();
    rate_Progression_X4->Write();
    rate_Progression_Y4->Write();
    rate_Progression_Z4->Write();
    rate_Progression_A5->Write();
    rate_Progression_B5->Write();
    rate_Progression_C5->Write();
    rate_Progression_D5->Write();
    rate_Progression_E5->Write();
    rate_Progression_F5->Write();
    rate_Progression_G5->Write();
    rate_Progression_H5->Write();
    rate_Progression_I5->Write();
    rate_Progression_J5->Write();
    rate_Progression_K5->Write();
    rate_Progression_L5->Write();
    rate_Progression_M5->Write();
    rate_Progression_N5->Write();
    rate_Progression_O5->Write();
    rate_Progression_P5->Write();
    rate_Progression_Q5->Write();
    rate_Progression_R5->Write();
    rate_Progression_S5->Write();
    rate_Progression_T5->Write();
    rate_Progression_U5->Write();
    rate_Progression_V5->Write();
    rate_Progression_W5->Write();
    rate_Progression_X5->Write();
    rate_Progression_Y5->Write();
    rate_Progression_Z5->Write();
    rate_Progression_A6->Write();
    rate_Progression_B6->Write();
    rate_Progression_C6->Write();
    rate_Progression_D6->Write();
    rate_Progression_E6->Write();
    rate_Progression_F6->Write();
    rate_Progression_G6->Write();
    rate_Progression_H6->Write();
    rate_Progression_I6->Write();
    rate_Progression_J6->Write();
    rate_Progression_K6->Write();
    rate_Progression_L6->Write();
    rate_Progression_M6->Write();
    rate_Progression_N6->Write();
    rate_Progression_O6->Write();
    rate_Progression_P6->Write();
    rate_Progression_Q6->Write();
    rate_Progression_R6->Write();
    rate_Progression_S6->Write();
    rate_Progression_T6->Write();
    rate_Progression_U6->Write();
    rate_Progression_V6->Write();
    rate_Progression_W6->Write();
    rate_Progression_X6->Write();
    rate_Progression_Y6->Write();
    rate_Progression_Z6->Write();
    rate_Progression_A7->Write();
    rate_Progression_B7->Write();
    rate_Progression_C7->Write();
    rate_Progression_D7->Write();
    rate_Progression_E7->Write();
    rate_Progression_F7->Write();
    rate_Progression_G7->Write();
    rate_Progression_H7->Write();
    rate_Progression_I7->Write();
    rate_Progression_J7->Write();
    rate_Progression_K7->Write();
    rate_Progression_L7->Write();
    rate_Progression_M7->Write();
    rate_Progression_N7->Write();
    rate_Progression_O7->Write();
    rate_Progression_P7->Write();
    rate_Progression_Q7->Write();
    rate_Progression_R7->Write();
    rate_Progression_S7->Write();
    rate_Progression_T7->Write();
    rate_Progression_U7->Write();
    rate_Progression_V7->Write();
    rate_Progression_W7->Write();
    rate_Progression_X7->Write();
    rate_Progression_Y7->Write();
    rate_Progression_Z7->Write();
    rate_Progression_A8->Write();
    rate_Progression_B8->Write();
    rate_Progression_C8->Write();
    rate_Progression_D8->Write();
    rate_Progression_E8->Write();
    rate_Progression_F8->Write();
    rate_Progression_G8->Write();
    rate_Progression_H8->Write();
    rate_Progression_I8->Write();
    rate_Progression_J8->Write();
    rate_Progression_K8->Write();
    rate_Progression_L8->Write();
    rate_Progression_M8->Write();
    rate_Progression_N8->Write();
    rate_Progression_O8->Write();
    rate_Progression_P8->Write();
    rate_Progression_Q8->Write();
    rate_Progression_R8->Write();
    rate_Progression_S8->Write();
    rate_Progression_T8->Write();
    rate_Progression_U8->Write();
    rate_Progression_V8->Write();
    rate_Progression_W8->Write();
    rate_Progression_X8->Write();
    rate_Progression_Y8->Write();
    rate_Progression_Z8->Write();
    // END OF FULL BLOWN GRID SEARCH  

    rate_noCut_DiTau->Write();
    rate_DiTau_90->Write();
    rate_DiTau_80->Write();
    rate_DiTau_70->Write();
    rate_DiTau_60->Write();
    rate_DiTau_50->Write();
    rate_DiTau_Progression_1->Write();
    rate_DiTau_Progression_2->Write();
    rate_DiTau_Progression_3->Write();
    rate_DiTau_Progression_4->Write();
    rate_DiTau_Progression_5->Write();
    rate_DiTau_Progression_6->Write();
    rate_DiTau_Progression_7->Write();
    rate_DiTau_Progression_8->Write();
    rate_DiTau_Progression_9->Write();
    rate_DiTau_Progression_10->Write();
    rate_DiTau_Progression_11->Write();
    rate_DiTau_Progression_12->Write();
    rate_DiTau_Progression_13->Write();
    rate_DiTau_Progression_14->Write();
    rate_DiTau_Progression_15->Write();
    rate_DiTau_Progression_16->Write();
    rate_DiTau_Progression_17->Write();
    rate_DiTau_Progression_18->Write();
    rate_DiTau_Progression_19->Write();
    rate_DiTau_Progression_20->Write();
    rate_DiTau_Progression_21->Write();
    rate_DiTau_Progression_21_endcaps->Write();
    rate_DiTau_Progression_21_barrel->Write();
    rate_DiTau_Progression_22->Write();
    rate_DiTau_Progression_22_barrel->Write();
    rate_DiTau_Progression_22_barrelendcap->Write();
    rate_DiTau_Progression_22_endcaps->Write();
    rate_DiTau_Progression_22_BothAreMerged->Write();
    rate_DiTau_Progression_22_OneIsMerged->Write();
    rate_DiTau_Progression_22_BothAreNotMerged->Write();

    rate_DiTau_Progression_22->Write();
    rate_DiTau_Progression_23->Write();
    rate_DiTau_Progression_24->Write();
    rate_DiTau_Progression_25->Write();
    rate_DiTau_Progression_26->Write();
    rate_DiTau_Progression_27->Write();
    rate_DiTau_Progression_28->Write();
    rate_DiTau_Progression_29->Write();
    rate_DiTau_Progression_30->Write();
    rate_DiTau_Progression_31->Write();
    rate_DiTau_Progression_31_barrel->Write();
    rate_DiTau_Progression_31_barrelendcap->Write();
    rate_DiTau_Progression_31_endcaps->Write();
    rate_DiTau_Progression_31_extrap->Write();
    rate_DiTau_Progression_31_extrap_barrel->Write();
    rate_DiTau_Progression_31_extrap_barrelendcap->Write();
    rate_DiTau_Progression_31_extrap_endcaps->Write();
    rate_Stage1->Write();  

    // START OF FULL BLOWN GRID SEARCH
    rate_DiTau_Progression_A0->Write();
    rate_DiTau_Progression_B0->Write();
    rate_DiTau_Progression_C0->Write();
    rate_DiTau_Progression_D0->Write();
    rate_DiTau_Progression_E0->Write();
    rate_DiTau_Progression_F0->Write();
    rate_DiTau_Progression_G0->Write();
    rate_DiTau_Progression_H0->Write();
    rate_DiTau_Progression_I0->Write();
    rate_DiTau_Progression_J0->Write();
    rate_DiTau_Progression_K0->Write();
    rate_DiTau_Progression_L0->Write();
    rate_DiTau_Progression_M0->Write();
    rate_DiTau_Progression_N0->Write();
    rate_DiTau_Progression_O0->Write();
    rate_DiTau_Progression_P0->Write();
    rate_DiTau_Progression_Q0->Write();
    rate_DiTau_Progression_R0->Write();
    rate_DiTau_Progression_S0->Write();
    rate_DiTau_Progression_T0->Write();
    rate_DiTau_Progression_U0->Write();
    rate_DiTau_Progression_V0->Write();
    rate_DiTau_Progression_W0->Write();
    rate_DiTau_Progression_X0->Write();
    rate_DiTau_Progression_Y0->Write();
    rate_DiTau_Progression_Z0->Write();
    rate_DiTau_Progression_A1->Write();
    rate_DiTau_Progression_B1->Write();
    rate_DiTau_Progression_C1->Write();
    rate_DiTau_Progression_D1->Write();
    rate_DiTau_Progression_E1->Write();
    rate_DiTau_Progression_F1->Write();
    rate_DiTau_Progression_G1->Write();
    rate_DiTau_Progression_H1->Write();
    rate_DiTau_Progression_I1->Write();
    rate_DiTau_Progression_J1->Write();
    rate_DiTau_Progression_K1->Write();
    rate_DiTau_Progression_L1->Write();
    rate_DiTau_Progression_M1->Write();
    rate_DiTau_Progression_N1->Write();
    rate_DiTau_Progression_O1->Write();
    rate_DiTau_Progression_P1->Write();
    rate_DiTau_Progression_Q1->Write();
    rate_DiTau_Progression_R1->Write();
    rate_DiTau_Progression_S1->Write();
    rate_DiTau_Progression_T1->Write();
    rate_DiTau_Progression_U1->Write();
    rate_DiTau_Progression_V1->Write();
    rate_DiTau_Progression_W1->Write();
    rate_DiTau_Progression_X1->Write();
    rate_DiTau_Progression_Y1->Write();
    rate_DiTau_Progression_Z1->Write();
    rate_DiTau_Progression_A2->Write();
    rate_DiTau_Progression_B2->Write();
    rate_DiTau_Progression_C2->Write();
    rate_DiTau_Progression_D2->Write();
    rate_DiTau_Progression_E2->Write();
    rate_DiTau_Progression_F2->Write();
    rate_DiTau_Progression_G2->Write();
    rate_DiTau_Progression_H2->Write();
    rate_DiTau_Progression_I2->Write();
    rate_DiTau_Progression_J2->Write();
    rate_DiTau_Progression_K2->Write();
    rate_DiTau_Progression_L2->Write();
    rate_DiTau_Progression_M2->Write();
    rate_DiTau_Progression_N2->Write();
    rate_DiTau_Progression_O2->Write();
    rate_DiTau_Progression_P2->Write();
    rate_DiTau_Progression_Q2->Write();
    rate_DiTau_Progression_R2->Write();
    rate_DiTau_Progression_S2->Write();
    rate_DiTau_Progression_T2->Write();
    rate_DiTau_Progression_U2->Write();
    rate_DiTau_Progression_V2->Write();
    rate_DiTau_Progression_W2->Write();
    rate_DiTau_Progression_X2->Write();
    rate_DiTau_Progression_Y2->Write();
    rate_DiTau_Progression_Z2->Write();
    rate_DiTau_Progression_A3->Write();
    rate_DiTau_Progression_B3->Write();
    rate_DiTau_Progression_C3->Write();
    rate_DiTau_Progression_D3->Write();
    rate_DiTau_Progression_E3->Write();
    rate_DiTau_Progression_F3->Write();
    rate_DiTau_Progression_G3->Write();
    rate_DiTau_Progression_H3->Write();
    rate_DiTau_Progression_I3->Write();
    rate_DiTau_Progression_J3->Write();
    rate_DiTau_Progression_K3->Write();
    rate_DiTau_Progression_L3->Write();
    rate_DiTau_Progression_M3->Write();
    rate_DiTau_Progression_N3->Write();
    rate_DiTau_Progression_O3->Write();
    rate_DiTau_Progression_P3->Write();
    rate_DiTau_Progression_Q3->Write();
    rate_DiTau_Progression_R3->Write();
    rate_DiTau_Progression_S3->Write();
    rate_DiTau_Progression_T3->Write();
    rate_DiTau_Progression_U3->Write();
    rate_DiTau_Progression_V3->Write();
    rate_DiTau_Progression_W3->Write();
    rate_DiTau_Progression_X3->Write();
    rate_DiTau_Progression_Y3->Write();
    rate_DiTau_Progression_Z3->Write();
    rate_DiTau_Progression_A4->Write();
    rate_DiTau_Progression_B4->Write();
    rate_DiTau_Progression_C4->Write();
    rate_DiTau_Progression_D4->Write();
    rate_DiTau_Progression_E4->Write();
    rate_DiTau_Progression_F4->Write();
    rate_DiTau_Progression_G4->Write();
    rate_DiTau_Progression_H4->Write();
    rate_DiTau_Progression_I4->Write();
    rate_DiTau_Progression_J4->Write();
    rate_DiTau_Progression_K4->Write();
    rate_DiTau_Progression_L4->Write();
    rate_DiTau_Progression_M4->Write();
    rate_DiTau_Progression_N4->Write();
    rate_DiTau_Progression_O4->Write();
    rate_DiTau_Progression_P4->Write();
    rate_DiTau_Progression_Q4->Write();
    rate_DiTau_Progression_R4->Write();
    rate_DiTau_Progression_S4->Write();
    rate_DiTau_Progression_T4->Write();
    rate_DiTau_Progression_U4->Write();
    rate_DiTau_Progression_V4->Write();
    rate_DiTau_Progression_W4->Write();
    rate_DiTau_Progression_X4->Write();
    rate_DiTau_Progression_Y4->Write();
    rate_DiTau_Progression_Z4->Write();
    rate_DiTau_Progression_A5->Write();
    rate_DiTau_Progression_B5->Write();
    rate_DiTau_Progression_C5->Write();
    rate_DiTau_Progression_D5->Write();
    rate_DiTau_Progression_E5->Write();
    rate_DiTau_Progression_F5->Write();
    rate_DiTau_Progression_G5->Write();
    rate_DiTau_Progression_H5->Write();
    rate_DiTau_Progression_I5->Write();
    rate_DiTau_Progression_J5->Write();
    rate_DiTau_Progression_K5->Write();
    rate_DiTau_Progression_L5->Write();
    rate_DiTau_Progression_M5->Write();
    rate_DiTau_Progression_N5->Write();
    rate_DiTau_Progression_O5->Write();
    rate_DiTau_Progression_P5->Write();
    rate_DiTau_Progression_Q5->Write();
    rate_DiTau_Progression_R5->Write();
    rate_DiTau_Progression_S5->Write();
    rate_DiTau_Progression_T5->Write();
    rate_DiTau_Progression_U5->Write();
    rate_DiTau_Progression_V5->Write();
    rate_DiTau_Progression_W5->Write();
    rate_DiTau_Progression_X5->Write();
    rate_DiTau_Progression_Y5->Write();
    rate_DiTau_Progression_Z5->Write();
    rate_DiTau_Progression_A6->Write();
    rate_DiTau_Progression_B6->Write();
    rate_DiTau_Progression_C6->Write();
    rate_DiTau_Progression_D6->Write();
    rate_DiTau_Progression_E6->Write();
    rate_DiTau_Progression_F6->Write();
    rate_DiTau_Progression_G6->Write();
    rate_DiTau_Progression_H6->Write();
    rate_DiTau_Progression_I6->Write();
    rate_DiTau_Progression_J6->Write();
    rate_DiTau_Progression_K6->Write();
    rate_DiTau_Progression_L6->Write();
    rate_DiTau_Progression_M6->Write();
    rate_DiTau_Progression_N6->Write();
    rate_DiTau_Progression_O6->Write();
    rate_DiTau_Progression_P6->Write();
    rate_DiTau_Progression_Q6->Write();
    rate_DiTau_Progression_R6->Write();
    rate_DiTau_Progression_S6->Write();
    rate_DiTau_Progression_T6->Write();
    rate_DiTau_Progression_U6->Write();
    rate_DiTau_Progression_V6->Write();
    rate_DiTau_Progression_W6->Write();
    rate_DiTau_Progression_X6->Write();
    rate_DiTau_Progression_Y6->Write();
    rate_DiTau_Progression_Z6->Write();
    rate_DiTau_Progression_A7->Write();
    rate_DiTau_Progression_B7->Write();
    rate_DiTau_Progression_C7->Write();
    rate_DiTau_Progression_D7->Write();
    rate_DiTau_Progression_E7->Write();
    rate_DiTau_Progression_F7->Write();
    rate_DiTau_Progression_G7->Write();
    rate_DiTau_Progression_H7->Write();
    rate_DiTau_Progression_I7->Write();
    rate_DiTau_Progression_J7->Write();
    rate_DiTau_Progression_K7->Write();
    rate_DiTau_Progression_L7->Write();
    rate_DiTau_Progression_M7->Write();
    rate_DiTau_Progression_N7->Write();
    rate_DiTau_Progression_O7->Write();
    rate_DiTau_Progression_P7->Write();
    rate_DiTau_Progression_Q7->Write();
    rate_DiTau_Progression_R7->Write();
    rate_DiTau_Progression_S7->Write();
    rate_DiTau_Progression_T7->Write();
    rate_DiTau_Progression_U7->Write();
    rate_DiTau_Progression_V7->Write();
    rate_DiTau_Progression_W7->Write();
    rate_DiTau_Progression_X7->Write();
    rate_DiTau_Progression_Y7->Write();
    rate_DiTau_Progression_Z7->Write();
    rate_DiTau_Progression_A8->Write();
    rate_DiTau_Progression_B8->Write();
    rate_DiTau_Progression_C8->Write();
    rate_DiTau_Progression_D8->Write();
    rate_DiTau_Progression_E8->Write();
    rate_DiTau_Progression_F8->Write();
    rate_DiTau_Progression_G8->Write();
    rate_DiTau_Progression_H8->Write();
    rate_DiTau_Progression_I8->Write();
    rate_DiTau_Progression_J8->Write();
    rate_DiTau_Progression_K8->Write();
    rate_DiTau_Progression_L8->Write();
    rate_DiTau_Progression_M8->Write();
    rate_DiTau_Progression_N8->Write();
    rate_DiTau_Progression_O8->Write();
    rate_DiTau_Progression_P8->Write();
    rate_DiTau_Progression_Q8->Write();
    rate_DiTau_Progression_R8->Write();
    rate_DiTau_Progression_S8->Write();
    rate_DiTau_Progression_T8->Write();
    rate_DiTau_Progression_U8->Write();
    rate_DiTau_Progression_V8->Write();
    rate_DiTau_Progression_W8->Write();
    rate_DiTau_Progression_X8->Write();
    rate_DiTau_Progression_Y8->Write();
    rate_DiTau_Progression_Z8->Write();
    // END OF FULL BLOWN GRID SEARCH

    //find rate in kHz for fixed threshold
    Double_t targetA = 32;
    Double_t targetB = 33;

    TVectorF Hz_noIso(2);
    Hz_noIso[0] = rateAtFixedThreshold(targetA, rate_noCut_DiTau);
    Hz_noIso[1] = rateAtFixedThreshold(targetB, rate_noCut_DiTau);
    Hz_noIso.Write("Hz_noIso");
    std::cout << "noIso : rate@thr32 = " << Hz_noIso[0] << "  -  rate@thr33 = " << Hz_noIso[1] << std::endl; 

    TVectorF Hz_Option22(2);
    Hz_Option22[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_22);
    Hz_Option22[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_22);
    Hz_Option22.Write("Hz_Option22");
    std::cout << "Option22 : rate@thr32 = " << Hz_Option22[0] << "  -  rate@thr33 = " << Hz_Option22[1] << std::endl; 

    TVectorF Hz_Option31_extrap(2);
    Hz_Option31_extrap[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_31_extrap);
    Hz_Option31_extrap[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_31_extrap);
    Hz_Option31_extrap.Write("Hz_Option31_extrap");
    std::cout << "Option31_extrap : rate@thr32 = " << Hz_Option31_extrap[0] << "  -  rate@thr33 = " << Hz_Option31_extrap[1] << std::endl; 

    TVectorF Hz_OptionA1(2);
    Hz_OptionA1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_A1);
    Hz_OptionA1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_A1);
    Hz_OptionA1.Write("Hz_OptionA1");
    std::cout << "OptionA1 : rate@thr32 = " << Hz_OptionA1[0] << "  -  rate@thr33 = " << Hz_OptionA1[1] << std::endl; 

    TVectorF Hz_OptionB1(2);
    Hz_OptionB1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_B1);
    Hz_OptionB1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_B1);
    Hz_OptionB1.Write("Hz_OptionB1");
    std::cout << "OptionB1 : rate@thr32 = " << Hz_OptionB1[0] << "  -  rate@thr33 = " << Hz_OptionB1[1] << std::endl; 

    TVectorF Hz_OptionC1(2);
    Hz_OptionC1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_C1);
    Hz_OptionC1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_C1);
    Hz_OptionC1.Write("Hz_OptionC1");
    std::cout << "OptionC1 : rate@thr32 = " << Hz_OptionC1[0] << "  -  rate@thr33 = " << Hz_OptionC1[1] << std::endl; 

    TVectorF Hz_OptionD1(2);
    Hz_OptionD1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_D1);
    Hz_OptionD1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_D1);
    Hz_OptionD1.Write("Hz_OptionD1");
    std::cout << "OptionD1 : rate@thr32 = " << Hz_OptionD1[0] << "  -  rate@thr33 = " << Hz_OptionD1[1] << std::endl; 

    TVectorF Hz_OptionE1(2);
    Hz_OptionE1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_E1);
    Hz_OptionE1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_E1);
    Hz_OptionE1.Write("Hz_OptionE1");
    std::cout << "OptionE1 : rate@thr32 = " << Hz_OptionE1[0] << "  -  rate@thr33 = " << Hz_OptionE1[1] << std::endl; 

    TVectorF Hz_OptionF1(2);
    Hz_OptionF1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_F1);
    Hz_OptionF1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_F1);
    Hz_OptionF1.Write("Hz_OptionF1");
    std::cout << "OptionF1 : rate@thr32 = " << Hz_OptionF1[0] << "  -  rate@thr33 = " << Hz_OptionF1[1] << std::endl; 

    TVectorF Hz_OptionG1(2);
    Hz_OptionG1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_G1);
    Hz_OptionG1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_G1);
    Hz_OptionG1.Write("Hz_OptionG1");
    std::cout << "OptionG1 : rate@thr32 = " << Hz_OptionG1[0] << "  -  rate@thr33 = " << Hz_OptionG1[1] << std::endl; 

    TVectorF Hz_OptionH1(2);
    Hz_OptionH1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_H1);
    Hz_OptionH1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_H1);
    Hz_OptionH1.Write("Hz_OptionH1");
    std::cout << "OptionH1 : rate@thr32 = " << Hz_OptionH1[0] << "  -  rate@thr33 = " << Hz_OptionH1[1] << std::endl; 

    TVectorF Hz_OptionI1(2);
    Hz_OptionI1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_I1);
    Hz_OptionI1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_I1);
    Hz_OptionI1.Write("Hz_OptionI1");
    std::cout << "OptionI1 : rate@thr32 = " << Hz_OptionI1[0] << "  -  rate@thr33 = " << Hz_OptionI1[1] << std::endl; 

    TVectorF Hz_OptionJ1(2);
    Hz_OptionJ1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_J1);
    Hz_OptionJ1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_J1);
    Hz_OptionJ1.Write("Hz_OptionJ1");
    std::cout << "OptionJ1 : rate@thr32 = " << Hz_OptionJ1[0] << "  -  rate@thr33 = " << Hz_OptionJ1[1] << std::endl; 

    TVectorF Hz_OptionK1(2);
    Hz_OptionK1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_K1);
    Hz_OptionK1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_K1);
    Hz_OptionK1.Write("Hz_OptionK1");
    std::cout << "OptionK1 : rate@thr32 = " << Hz_OptionK1[0] << "  -  rate@thr33 = " << Hz_OptionK1[1] << std::endl; 

    TVectorF Hz_OptionL1(2);
    Hz_OptionL1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_L1);
    Hz_OptionL1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_L1);
    Hz_OptionL1.Write("Hz_OptionL1");
    std::cout << "OptionL1 : rate@thr32 = " << Hz_OptionL1[0] << "  -  rate@thr33 = " << Hz_OptionL1[1] << std::endl; 

    TVectorF Hz_OptionM1(2);
    Hz_OptionM1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_M1);
    Hz_OptionM1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_M1);
    Hz_OptionM1.Write("Hz_OptionM1");
    std::cout << "OptionM1 : rate@thr32 = " << Hz_OptionM1[0] << "  -  rate@thr33 = " << Hz_OptionM1[1] << std::endl; 

    TVectorF Hz_OptionN1(2);
    Hz_OptionN1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_N1);
    Hz_OptionN1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_N1);
    Hz_OptionN1.Write("Hz_OptionN1");
    std::cout << "OptionN1 : rate@thr32 = " << Hz_OptionN1[0] << "  -  rate@thr33 = " << Hz_OptionN1[1] << std::endl; 

    TVectorF Hz_OptionO1(2);
    Hz_OptionO1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_O1);
    Hz_OptionO1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_O1);
    Hz_OptionO1.Write("Hz_OptionO1");
    std::cout << "OptionO1 : rate@thr32 = " << Hz_OptionO1[0] << "  -  rate@thr33 = " << Hz_OptionO1[1] << std::endl; 

    TVectorF Hz_OptionP1(2);
    Hz_OptionP1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_P1);
    Hz_OptionP1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_P1);
    Hz_OptionP1.Write("Hz_OptionP1");
    std::cout << "OptionP1 : rate@thr32 = " << Hz_OptionP1[0] << "  -  rate@thr33 = " << Hz_OptionP1[1] << std::endl; 

    TVectorF Hz_OptionQ1(2);
    Hz_OptionQ1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Q1);
    Hz_OptionQ1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Q1);
    Hz_OptionQ1.Write("Hz_OptionQ1");
    std::cout << "OptionQ1 : rate@thr32 = " << Hz_OptionQ1[0] << "  -  rate@thr33 = " << Hz_OptionQ1[1] << std::endl; 

    TVectorF Hz_OptionR1(2);
    Hz_OptionR1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_R1);
    Hz_OptionR1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_R1);
    Hz_OptionR1.Write("Hz_OptionR1");
    std::cout << "OptionR1 : rate@thr32 = " << Hz_OptionR1[0] << "  -  rate@thr33 = " << Hz_OptionR1[1] << std::endl; 

    TVectorF Hz_OptionS1(2);
    Hz_OptionS1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_S1);
    Hz_OptionS1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_S1);
    Hz_OptionS1.Write("Hz_OptionS1");
    std::cout << "OptionS1 : rate@thr32 = " << Hz_OptionS1[0] << "  -  rate@thr33 = " << Hz_OptionS1[1] << std::endl; 

    TVectorF Hz_OptionT1(2);
    Hz_OptionT1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_T1);
    Hz_OptionT1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_T1);
    Hz_OptionT1.Write("Hz_OptionT1");
    std::cout << "OptionT1 : rate@thr32 = " << Hz_OptionT1[0] << "  -  rate@thr33 = " << Hz_OptionT1[1] << std::endl; 

    TVectorF Hz_OptionU1(2);
    Hz_OptionU1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_U1);
    Hz_OptionU1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_U1);
    Hz_OptionU1.Write("Hz_OptionU1");
    std::cout << "OptionU1 : rate@thr32 = " << Hz_OptionU1[0] << "  -  rate@thr33 = " << Hz_OptionU1[1] << std::endl; 

    TVectorF Hz_OptionV1(2);
    Hz_OptionV1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_V1);
    Hz_OptionV1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_V1);
    Hz_OptionV1.Write("Hz_OptionV1");
    std::cout << "OptionV1 : rate@thr32 = " << Hz_OptionV1[0] << "  -  rate@thr33 = " << Hz_OptionV1[1] << std::endl; 

    TVectorF Hz_OptionW1(2);
    Hz_OptionW1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_W1);
    Hz_OptionW1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_W1);
    Hz_OptionW1.Write("Hz_OptionW1");
    std::cout << "OptionW1 : rate@thr32 = " << Hz_OptionW1[0] << "  -  rate@thr33 = " << Hz_OptionW1[1] << std::endl; 

    TVectorF Hz_OptionX1(2);
    Hz_OptionX1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_X1);
    Hz_OptionX1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_X1);
    Hz_OptionX1.Write("Hz_OptionX1");
    std::cout << "OptionX1 : rate@thr32 = " << Hz_OptionX1[0] << "  -  rate@thr33 = " << Hz_OptionX1[1] << std::endl; 

    TVectorF Hz_OptionY1(2);
    Hz_OptionY1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Y1);
    Hz_OptionY1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Y1);
    Hz_OptionY1.Write("Hz_OptionY1");
    std::cout << "OptionY1 : rate@thr32 = " << Hz_OptionY1[0] << "  -  rate@thr33 = " << Hz_OptionY1[1] << std::endl; 

    TVectorF Hz_OptionZ1(2);
    Hz_OptionZ1[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Z1);
    Hz_OptionZ1[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Z1);
    Hz_OptionZ1.Write("Hz_OptionZ1");
    std::cout << "OptionZ1 : rate@thr32 = " << Hz_OptionZ1[0] << "  -  rate@thr33 = " << Hz_OptionZ1[1] << std::endl; 

    TVectorF Hz_OptionA2(2);
    Hz_OptionA2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_A2);
    Hz_OptionA2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_A2);
    Hz_OptionA2.Write("Hz_OptionA2");
    std::cout << "OptionA2 : rate@thr32 = " << Hz_OptionA2[0] << "  -  rate@thr33 = " << Hz_OptionA2[1] << std::endl; 

    TVectorF Hz_OptionB2(2);
    Hz_OptionB2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_B2);
    Hz_OptionB2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_B2);
    Hz_OptionB2.Write("Hz_OptionB2");
    std::cout << "OptionB2 : rate@thr32 = " << Hz_OptionB2[0] << "  -  rate@thr33 = " << Hz_OptionB2[1] << std::endl; 

    TVectorF Hz_OptionC2(2);
    Hz_OptionC2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_C2);
    Hz_OptionC2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_C2);
    Hz_OptionC2.Write("Hz_OptionC2");
    std::cout << "OptionC2 : rate@thr32 = " << Hz_OptionC2[0] << "  -  rate@thr33 = " << Hz_OptionC2[1] << std::endl; 

    TVectorF Hz_OptionD2(2);
    Hz_OptionD2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_D2);
    Hz_OptionD2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_D2);
    Hz_OptionD2.Write("Hz_OptionD2");
    std::cout << "OptionD2 : rate@thr32 = " << Hz_OptionD2[0] << "  -  rate@thr33 = " << Hz_OptionD2[1] << std::endl; 

    TVectorF Hz_OptionE2(2);
    Hz_OptionE2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_E2);
    Hz_OptionE2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_E2);
    Hz_OptionE2.Write("Hz_OptionE2");
    std::cout << "OptionE2 : rate@thr32 = " << Hz_OptionE2[0] << "  -  rate@thr33 = " << Hz_OptionE2[1] << std::endl; 

    TVectorF Hz_OptionF2(2);
    Hz_OptionF2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_F2);
    Hz_OptionF2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_F2);
    Hz_OptionF2.Write("Hz_OptionF2");
    std::cout << "OptionF2 : rate@thr32 = " << Hz_OptionF2[0] << "  -  rate@thr33 = " << Hz_OptionF2[1] << std::endl; 

    TVectorF Hz_OptionG2(2);
    Hz_OptionG2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_G2);
    Hz_OptionG2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_G2);
    Hz_OptionG2.Write("Hz_OptionG2");
    std::cout << "OptionG2 : rate@thr32 = " << Hz_OptionG2[0] << "  -  rate@thr33 = " << Hz_OptionG2[1] << std::endl; 

    TVectorF Hz_OptionH2(2);
    Hz_OptionH2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_H2);
    Hz_OptionH2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_H2);
    Hz_OptionH2.Write("Hz_OptionH2");
    std::cout << "OptionH2 : rate@thr32 = " << Hz_OptionH2[0] << "  -  rate@thr33 = " << Hz_OptionH2[1] << std::endl; 

    TVectorF Hz_OptionI2(2);
    Hz_OptionI2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_I2);
    Hz_OptionI2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_I2);
    Hz_OptionI2.Write("Hz_OptionI2");
    std::cout << "OptionI2 : rate@thr32 = " << Hz_OptionI2[0] << "  -  rate@thr33 = " << Hz_OptionI2[1] << std::endl; 

    TVectorF Hz_OptionJ2(2);
    Hz_OptionJ2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_J2);
    Hz_OptionJ2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_J2);
    Hz_OptionJ2.Write("Hz_OptionJ2");
    std::cout << "OptionJ2 : rate@thr32 = " << Hz_OptionJ2[0] << "  -  rate@thr33 = " << Hz_OptionJ2[1] << std::endl; 

    TVectorF Hz_OptionK2(2);
    Hz_OptionK2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_K2);
    Hz_OptionK2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_K2);
    Hz_OptionK2.Write("Hz_OptionK2");
    std::cout << "OptionK2 : rate@thr32 = " << Hz_OptionK2[0] << "  -  rate@thr33 = " << Hz_OptionK2[1] << std::endl; 

    TVectorF Hz_OptionL2(2);
    Hz_OptionL2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_L2);
    Hz_OptionL2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_L2);
    Hz_OptionL2.Write("Hz_OptionL2");
    std::cout << "OptionL2 : rate@thr32 = " << Hz_OptionL2[0] << "  -  rate@thr33 = " << Hz_OptionL2[1] << std::endl; 

    TVectorF Hz_OptionM2(2);
    Hz_OptionM2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_M2);
    Hz_OptionM2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_M2);
    Hz_OptionM2.Write("Hz_OptionM2");
    std::cout << "OptionM2 : rate@thr32 = " << Hz_OptionM2[0] << "  -  rate@thr33 = " << Hz_OptionM2[1] << std::endl; 

    TVectorF Hz_OptionN2(2);
    Hz_OptionN2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_N2);
    Hz_OptionN2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_N2);
    Hz_OptionN2.Write("Hz_OptionN2");
    std::cout << "OptionN2 : rate@thr32 = " << Hz_OptionN2[0] << "  -  rate@thr33 = " << Hz_OptionN2[1] << std::endl; 

    TVectorF Hz_OptionO2(2);
    Hz_OptionO2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_O2);
    Hz_OptionO2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_O2);
    Hz_OptionO2.Write("Hz_OptionO2");
    std::cout << "OptionO2 : rate@thr32 = " << Hz_OptionO2[0] << "  -  rate@thr33 = " << Hz_OptionO2[1] << std::endl; 

    TVectorF Hz_OptionP2(2);
    Hz_OptionP2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_P2);
    Hz_OptionP2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_P2);
    Hz_OptionP2.Write("Hz_OptionP2");
    std::cout << "OptionP2 : rate@thr32 = " << Hz_OptionP2[0] << "  -  rate@thr33 = " << Hz_OptionP2[1] << std::endl; 

    TVectorF Hz_OptionQ2(2);
    Hz_OptionQ2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Q2);
    Hz_OptionQ2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Q2);
    Hz_OptionQ2.Write("Hz_OptionQ2");
    std::cout << "OptionQ2 : rate@thr32 = " << Hz_OptionQ2[0] << "  -  rate@thr33 = " << Hz_OptionQ2[1] << std::endl; 

    TVectorF Hz_OptionR2(2);
    Hz_OptionR2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_R2);
    Hz_OptionR2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_R2);
    Hz_OptionR2.Write("Hz_OptionR2");
    std::cout << "OptionR2 : rate@thr32 = " << Hz_OptionR2[0] << "  -  rate@thr33 = " << Hz_OptionR2[1] << std::endl; 

    TVectorF Hz_OptionS2(2);
    Hz_OptionS2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_S2);
    Hz_OptionS2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_S2);
    Hz_OptionS2.Write("Hz_OptionS2");
    std::cout << "OptionS2 : rate@thr32 = " << Hz_OptionS2[0] << "  -  rate@thr33 = " << Hz_OptionS2[1] << std::endl; 

    TVectorF Hz_OptionT2(2);
    Hz_OptionT2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_T2);
    Hz_OptionT2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_T2);
    Hz_OptionT2.Write("Hz_OptionT2");
    std::cout << "OptionT2 : rate@thr32 = " << Hz_OptionT2[0] << "  -  rate@thr33 = " << Hz_OptionT2[1] << std::endl; 

    TVectorF Hz_OptionU2(2);
    Hz_OptionU2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_U2);
    Hz_OptionU2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_U2);
    Hz_OptionU2.Write("Hz_OptionU2");
    std::cout << "OptionU2 : rate@thr32 = " << Hz_OptionU2[0] << "  -  rate@thr33 = " << Hz_OptionU2[1] << std::endl; 

    TVectorF Hz_OptionV2(2);
    Hz_OptionV2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_V2);
    Hz_OptionV2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_V2);
    Hz_OptionV2.Write("Hz_OptionV2");
    std::cout << "OptionV2 : rate@thr32 = " << Hz_OptionV2[0] << "  -  rate@thr33 = " << Hz_OptionV2[1] << std::endl; 

    TVectorF Hz_OptionW2(2);
    Hz_OptionW2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_W2);
    Hz_OptionW2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_W2);
    Hz_OptionW2.Write("Hz_OptionW2");
    std::cout << "OptionW2 : rate@thr32 = " << Hz_OptionW2[0] << "  -  rate@thr33 = " << Hz_OptionW2[1] << std::endl; 

    TVectorF Hz_OptionX2(2);
    Hz_OptionX2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_X2);
    Hz_OptionX2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_X2);
    Hz_OptionX2.Write("Hz_OptionX2");
    std::cout << "OptionX2 : rate@thr32 = " << Hz_OptionX2[0] << "  -  rate@thr33 = " << Hz_OptionX2[1] << std::endl; 

    TVectorF Hz_OptionY2(2);
    Hz_OptionY2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Y2);
    Hz_OptionY2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Y2);
    Hz_OptionY2.Write("Hz_OptionY2");
    std::cout << "OptionY2 : rate@thr32 = " << Hz_OptionY2[0] << "  -  rate@thr33 = " << Hz_OptionY2[1] << std::endl; 

    TVectorF Hz_OptionZ2(2);
    Hz_OptionZ2[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Z2);
    Hz_OptionZ2[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Z2);
    Hz_OptionZ2.Write("Hz_OptionZ2");
    std::cout << "OptionZ2 : rate@thr32 = " << Hz_OptionZ2[0] << "  -  rate@thr33 = " << Hz_OptionZ2[1] << std::endl; 

    TVectorF Hz_OptionA3(2);
    Hz_OptionA3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_A3);
    Hz_OptionA3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_A3);
    Hz_OptionA3.Write("Hz_OptionA3");
    std::cout << "OptionA3 : rate@thr32 = " << Hz_OptionA3[0] << "  -  rate@thr33 = " << Hz_OptionA3[1] << std::endl; 

    TVectorF Hz_OptionB3(2);
    Hz_OptionB3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_B3);
    Hz_OptionB3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_B3);
    Hz_OptionB3.Write("Hz_OptionB3");
    std::cout << "OptionB3 : rate@thr32 = " << Hz_OptionB3[0] << "  -  rate@thr33 = " << Hz_OptionB3[1] << std::endl; 

    TVectorF Hz_OptionC3(2);
    Hz_OptionC3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_C3);
    Hz_OptionC3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_C3);
    Hz_OptionC3.Write("Hz_OptionC3");
    std::cout << "OptionC3 : rate@thr32 = " << Hz_OptionC3[0] << "  -  rate@thr33 = " << Hz_OptionC3[1] << std::endl; 

    TVectorF Hz_OptionD3(2);
    Hz_OptionD3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_D3);
    Hz_OptionD3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_D3);
    Hz_OptionD3.Write("Hz_OptionD3");
    std::cout << "OptionD3 : rate@thr32 = " << Hz_OptionD3[0] << "  -  rate@thr33 = " << Hz_OptionD3[1] << std::endl; 

    TVectorF Hz_OptionE3(2);
    Hz_OptionE3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_E3);
    Hz_OptionE3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_E3);
    Hz_OptionE3.Write("Hz_OptionE3");
    std::cout << "OptionE3 : rate@thr32 = " << Hz_OptionE3[0] << "  -  rate@thr33 = " << Hz_OptionE3[1] << std::endl; 

    TVectorF Hz_OptionF3(2);
    Hz_OptionF3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_F3);
    Hz_OptionF3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_F3);
    Hz_OptionF3.Write("Hz_OptionF3");
    std::cout << "OptionF3 : rate@thr32 = " << Hz_OptionF3[0] << "  -  rate@thr33 = " << Hz_OptionF3[1] << std::endl; 

    TVectorF Hz_OptionG3(2);
    Hz_OptionG3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_G3);
    Hz_OptionG3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_G3);
    Hz_OptionG3.Write("Hz_OptionG3");
    std::cout << "OptionG3 : rate@thr32 = " << Hz_OptionG3[0] << "  -  rate@thr33 = " << Hz_OptionG3[1] << std::endl; 

    TVectorF Hz_OptionH3(2);
    Hz_OptionH3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_H3);
    Hz_OptionH3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_H3);
    Hz_OptionH3.Write("Hz_OptionH3");
    std::cout << "OptionH3 : rate@thr32 = " << Hz_OptionH3[0] << "  -  rate@thr33 = " << Hz_OptionH3[1] << std::endl; 

    TVectorF Hz_OptionI3(2);
    Hz_OptionI3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_I3);
    Hz_OptionI3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_I3);
    Hz_OptionI3.Write("Hz_OptionI3");
    std::cout << "OptionI3 : rate@thr32 = " << Hz_OptionI3[0] << "  -  rate@thr33 = " << Hz_OptionI3[1] << std::endl; 

    TVectorF Hz_OptionJ3(2);
    Hz_OptionJ3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_J3);
    Hz_OptionJ3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_J3);
    Hz_OptionJ3.Write("Hz_OptionJ3");
    std::cout << "OptionJ3 : rate@thr32 = " << Hz_OptionJ3[0] << "  -  rate@thr33 = " << Hz_OptionJ3[1] << std::endl; 

    TVectorF Hz_OptionK3(2);
    Hz_OptionK3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_K3);
    Hz_OptionK3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_K3);
    Hz_OptionK3.Write("Hz_OptionK3");
    std::cout << "OptionK3 : rate@thr32 = " << Hz_OptionK3[0] << "  -  rate@thr33 = " << Hz_OptionK3[1] << std::endl; 

    TVectorF Hz_OptionL3(2);
    Hz_OptionL3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_L3);
    Hz_OptionL3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_L3);
    Hz_OptionL3.Write("Hz_OptionL3");
    std::cout << "OptionL3 : rate@thr32 = " << Hz_OptionL3[0] << "  -  rate@thr33 = " << Hz_OptionL3[1] << std::endl; 

    TVectorF Hz_OptionM3(2);
    Hz_OptionM3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_M3);
    Hz_OptionM3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_M3);
    Hz_OptionM3.Write("Hz_OptionM3");
    std::cout << "OptionM3 : rate@thr32 = " << Hz_OptionM3[0] << "  -  rate@thr33 = " << Hz_OptionM3[1] << std::endl; 

    TVectorF Hz_OptionN3(2);
    Hz_OptionN3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_N3);
    Hz_OptionN3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_N3);
    Hz_OptionN3.Write("Hz_OptionN3");
    std::cout << "OptionN3 : rate@thr32 = " << Hz_OptionN3[0] << "  -  rate@thr33 = " << Hz_OptionN3[1] << std::endl; 

    TVectorF Hz_OptionO3(2);
    Hz_OptionO3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_O3);
    Hz_OptionO3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_O3);
    Hz_OptionO3.Write("Hz_OptionO3");
    std::cout << "OptionO3 : rate@thr32 = " << Hz_OptionO3[0] << "  -  rate@thr33 = " << Hz_OptionO3[1] << std::endl; 

    TVectorF Hz_OptionP3(2);
    Hz_OptionP3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_P3);
    Hz_OptionP3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_P3);
    Hz_OptionP3.Write("Hz_OptionP3");
    std::cout << "OptionP3 : rate@thr32 = " << Hz_OptionP3[0] << "  -  rate@thr33 = " << Hz_OptionP3[1] << std::endl; 

    TVectorF Hz_OptionQ3(2);
    Hz_OptionQ3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Q3);
    Hz_OptionQ3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Q3);
    Hz_OptionQ3.Write("Hz_OptionQ3");
    std::cout << "OptionQ3 : rate@thr32 = " << Hz_OptionQ3[0] << "  -  rate@thr33 = " << Hz_OptionQ3[1] << std::endl; 

    TVectorF Hz_OptionR3(2);
    Hz_OptionR3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_R3);
    Hz_OptionR3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_R3);
    Hz_OptionR3.Write("Hz_OptionR3");
    std::cout << "OptionR3 : rate@thr32 = " << Hz_OptionR3[0] << "  -  rate@thr33 = " << Hz_OptionR3[1] << std::endl; 

    TVectorF Hz_OptionS3(2);
    Hz_OptionS3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_S3);
    Hz_OptionS3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_S3);
    Hz_OptionS3.Write("Hz_OptionS3");
    std::cout << "OptionS3 : rate@thr32 = " << Hz_OptionS3[0] << "  -  rate@thr33 = " << Hz_OptionS3[1] << std::endl; 

    TVectorF Hz_OptionT3(2);
    Hz_OptionT3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_T3);
    Hz_OptionT3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_T3);
    Hz_OptionT3.Write("Hz_OptionT3");
    std::cout << "OptionT3 : rate@thr32 = " << Hz_OptionT3[0] << "  -  rate@thr33 = " << Hz_OptionT3[1] << std::endl; 

    TVectorF Hz_OptionU3(2);
    Hz_OptionU3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_U3);
    Hz_OptionU3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_U3);
    Hz_OptionU3.Write("Hz_OptionU3");
    std::cout << "OptionU3 : rate@thr32 = " << Hz_OptionU3[0] << "  -  rate@thr33 = " << Hz_OptionU3[1] << std::endl; 

    TVectorF Hz_OptionV3(2);
    Hz_OptionV3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_V3);
    Hz_OptionV3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_V3);
    Hz_OptionV3.Write("Hz_OptionV3");
    std::cout << "OptionV3 : rate@thr32 = " << Hz_OptionV3[0] << "  -  rate@thr33 = " << Hz_OptionV3[1] << std::endl; 

    TVectorF Hz_OptionW3(2);
    Hz_OptionW3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_W3);
    Hz_OptionW3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_W3);
    Hz_OptionW3.Write("Hz_OptionW3");
    std::cout << "OptionW3 : rate@thr32 = " << Hz_OptionW3[0] << "  -  rate@thr33 = " << Hz_OptionW3[1] << std::endl; 

    TVectorF Hz_OptionX3(2);
    Hz_OptionX3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_X3);
    Hz_OptionX3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_X3);
    Hz_OptionX3.Write("Hz_OptionX3");
    std::cout << "OptionX3 : rate@thr32 = " << Hz_OptionX3[0] << "  -  rate@thr33 = " << Hz_OptionX3[1] << std::endl; 

    TVectorF Hz_OptionY3(2);
    Hz_OptionY3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Y3);
    Hz_OptionY3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Y3);
    Hz_OptionY3.Write("Hz_OptionY3");
    std::cout << "OptionY3 : rate@thr32 = " << Hz_OptionY3[0] << "  -  rate@thr33 = " << Hz_OptionY3[1] << std::endl; 

    TVectorF Hz_OptionZ3(2);
    Hz_OptionZ3[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Z3);
    Hz_OptionZ3[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Z3);
    Hz_OptionZ3.Write("Hz_OptionZ3");
    std::cout << "OptionZ3 : rate@thr32 = " << Hz_OptionZ3[0] << "  -  rate@thr33 = " << Hz_OptionZ3[1] << std::endl; 

    TVectorF Hz_OptionA4(2);
    Hz_OptionA4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_A4);
    Hz_OptionA4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_A4);
    Hz_OptionA4.Write("Hz_OptionA4");
    std::cout << "OptionA4 : rate@thr32 = " << Hz_OptionA4[0] << "  -  rate@thr33 = " << Hz_OptionA4[1] << std::endl; 

    TVectorF Hz_OptionB4(2);
    Hz_OptionB4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_B4);
    Hz_OptionB4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_B4);
    Hz_OptionB4.Write("Hz_OptionB4");
    std::cout << "OptionB4 : rate@thr32 = " << Hz_OptionB4[0] << "  -  rate@thr33 = " << Hz_OptionB4[1] << std::endl; 

    TVectorF Hz_OptionC4(2);
    Hz_OptionC4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_C4);
    Hz_OptionC4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_C4);
    Hz_OptionC4.Write("Hz_OptionC4");
    std::cout << "OptionC4 : rate@thr32 = " << Hz_OptionC4[0] << "  -  rate@thr33 = " << Hz_OptionC4[1] << std::endl; 

    TVectorF Hz_OptionD4(2);
    Hz_OptionD4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_D4);
    Hz_OptionD4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_D4);
    Hz_OptionD4.Write("Hz_OptionD4");
    std::cout << "OptionD4 : rate@thr32 = " << Hz_OptionD4[0] << "  -  rate@thr33 = " << Hz_OptionD4[1] << std::endl; 

    TVectorF Hz_OptionE4(2);
    Hz_OptionE4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_E4);
    Hz_OptionE4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_E4);
    Hz_OptionE4.Write("Hz_OptionE4");
    std::cout << "OptionE4 : rate@thr32 = " << Hz_OptionE4[0] << "  -  rate@thr33 = " << Hz_OptionE4[1] << std::endl; 

    TVectorF Hz_OptionF4(2);
    Hz_OptionF4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_F4);
    Hz_OptionF4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_F4);
    Hz_OptionF4.Write("Hz_OptionF4");
    std::cout << "OptionF4 : rate@thr32 = " << Hz_OptionF4[0] << "  -  rate@thr33 = " << Hz_OptionF4[1] << std::endl; 

    TVectorF Hz_OptionG4(2);
    Hz_OptionG4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_G4);
    Hz_OptionG4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_G4);
    Hz_OptionG4.Write("Hz_OptionG4");
    std::cout << "OptionG4 : rate@thr32 = " << Hz_OptionG4[0] << "  -  rate@thr33 = " << Hz_OptionG4[1] << std::endl; 

    TVectorF Hz_OptionH4(2);
    Hz_OptionH4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_H4);
    Hz_OptionH4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_H4);
    Hz_OptionH4.Write("Hz_OptionH4");
    std::cout << "OptionH4 : rate@thr32 = " << Hz_OptionH4[0] << "  -  rate@thr33 = " << Hz_OptionH4[1] << std::endl; 

    TVectorF Hz_OptionI4(2);
    Hz_OptionI4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_I4);
    Hz_OptionI4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_I4);
    Hz_OptionI4.Write("Hz_OptionI4");
    std::cout << "OptionI4 : rate@thr32 = " << Hz_OptionI4[0] << "  -  rate@thr33 = " << Hz_OptionI4[1] << std::endl; 

    TVectorF Hz_OptionJ4(2);
    Hz_OptionJ4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_J4);
    Hz_OptionJ4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_J4);
    Hz_OptionJ4.Write("Hz_OptionJ4");
    std::cout << "OptionJ4 : rate@thr32 = " << Hz_OptionJ4[0] << "  -  rate@thr33 = " << Hz_OptionJ4[1] << std::endl; 

    TVectorF Hz_OptionK4(2);
    Hz_OptionK4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_K4);
    Hz_OptionK4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_K4);
    Hz_OptionK4.Write("Hz_OptionK4");
    std::cout << "OptionK4 : rate@thr32 = " << Hz_OptionK4[0] << "  -  rate@thr33 = " << Hz_OptionK4[1] << std::endl; 

    TVectorF Hz_OptionL4(2);
    Hz_OptionL4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_L4);
    Hz_OptionL4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_L4);
    Hz_OptionL4.Write("Hz_OptionL4");
    std::cout << "OptionL4 : rate@thr32 = " << Hz_OptionL4[0] << "  -  rate@thr33 = " << Hz_OptionL4[1] << std::endl; 

    TVectorF Hz_OptionM4(2);
    Hz_OptionM4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_M4);
    Hz_OptionM4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_M4);
    Hz_OptionM4.Write("Hz_OptionM4");
    std::cout << "OptionM4 : rate@thr32 = " << Hz_OptionM4[0] << "  -  rate@thr33 = " << Hz_OptionM4[1] << std::endl; 

    TVectorF Hz_OptionN4(2);
    Hz_OptionN4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_N4);
    Hz_OptionN4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_N4);
    Hz_OptionN4.Write("Hz_OptionN4");
    std::cout << "OptionN4 : rate@thr32 = " << Hz_OptionN4[0] << "  -  rate@thr33 = " << Hz_OptionN4[1] << std::endl; 

    TVectorF Hz_OptionO4(2);
    Hz_OptionO4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_O4);
    Hz_OptionO4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_O4);
    Hz_OptionO4.Write("Hz_OptionO4");
    std::cout << "OptionO4 : rate@thr32 = " << Hz_OptionO4[0] << "  -  rate@thr33 = " << Hz_OptionO4[1] << std::endl; 

    TVectorF Hz_OptionP4(2);
    Hz_OptionP4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_P4);
    Hz_OptionP4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_P4);
    Hz_OptionP4.Write("Hz_OptionP4");
    std::cout << "OptionP4 : rate@thr32 = " << Hz_OptionP4[0] << "  -  rate@thr33 = " << Hz_OptionP4[1] << std::endl; 

    TVectorF Hz_OptionQ4(2);
    Hz_OptionQ4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Q4);
    Hz_OptionQ4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Q4);
    Hz_OptionQ4.Write("Hz_OptionQ4");
    std::cout << "OptionQ4 : rate@thr32 = " << Hz_OptionQ4[0] << "  -  rate@thr33 = " << Hz_OptionQ4[1] << std::endl; 

    TVectorF Hz_OptionR4(2);
    Hz_OptionR4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_R4);
    Hz_OptionR4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_R4);
    Hz_OptionR4.Write("Hz_OptionR4");
    std::cout << "OptionR4 : rate@thr32 = " << Hz_OptionR4[0] << "  -  rate@thr33 = " << Hz_OptionR4[1] << std::endl; 

    TVectorF Hz_OptionS4(2);
    Hz_OptionS4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_S4);
    Hz_OptionS4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_S4);
    Hz_OptionS4.Write("Hz_OptionS4");
    std::cout << "OptionS4 : rate@thr32 = " << Hz_OptionS4[0] << "  -  rate@thr33 = " << Hz_OptionS4[1] << std::endl; 

    TVectorF Hz_OptionT4(2);
    Hz_OptionT4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_T4);
    Hz_OptionT4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_T4);
    Hz_OptionT4.Write("Hz_OptionT4");
    std::cout << "OptionT4 : rate@thr32 = " << Hz_OptionT4[0] << "  -  rate@thr33 = " << Hz_OptionT4[1] << std::endl; 

    TVectorF Hz_OptionU4(2);
    Hz_OptionU4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_U4);
    Hz_OptionU4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_U4);
    Hz_OptionU4.Write("Hz_OptionU4");
    std::cout << "OptionU4 : rate@thr32 = " << Hz_OptionU4[0] << "  -  rate@thr33 = " << Hz_OptionU4[1] << std::endl; 

    TVectorF Hz_OptionV4(2);
    Hz_OptionV4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_V4);
    Hz_OptionV4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_V4);
    Hz_OptionV4.Write("Hz_OptionV4");
    std::cout << "OptionV4 : rate@thr32 = " << Hz_OptionV4[0] << "  -  rate@thr33 = " << Hz_OptionV4[1] << std::endl; 

    TVectorF Hz_OptionW4(2);
    Hz_OptionW4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_W4);
    Hz_OptionW4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_W4);
    Hz_OptionW4.Write("Hz_OptionW4");
    std::cout << "OptionW4 : rate@thr32 = " << Hz_OptionW4[0] << "  -  rate@thr33 = " << Hz_OptionW4[1] << std::endl; 

    TVectorF Hz_OptionX4(2);
    Hz_OptionX4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_X4);
    Hz_OptionX4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_X4);
    Hz_OptionX4.Write("Hz_OptionX4");
    std::cout << "OptionX4 : rate@thr32 = " << Hz_OptionX4[0] << "  -  rate@thr33 = " << Hz_OptionX4[1] << std::endl; 

    TVectorF Hz_OptionY4(2);
    Hz_OptionY4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Y4);
    Hz_OptionY4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Y4);
    Hz_OptionY4.Write("Hz_OptionY4");
    std::cout << "OptionY4 : rate@thr32 = " << Hz_OptionY4[0] << "  -  rate@thr33 = " << Hz_OptionY4[1] << std::endl; 

    TVectorF Hz_OptionZ4(2);
    Hz_OptionZ4[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Z4);
    Hz_OptionZ4[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Z4);
    Hz_OptionZ4.Write("Hz_OptionZ4");
    std::cout << "OptionZ4 : rate@thr32 = " << Hz_OptionZ4[0] << "  -  rate@thr33 = " << Hz_OptionZ4[1] << std::endl; 

    TVectorF Hz_OptionA5(2);
    Hz_OptionA5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_A5);
    Hz_OptionA5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_A5);
    Hz_OptionA5.Write("Hz_OptionA5");
    std::cout << "OptionA5 : rate@thr32 = " << Hz_OptionA5[0] << "  -  rate@thr33 = " << Hz_OptionA5[1] << std::endl; 

    TVectorF Hz_OptionB5(2);
    Hz_OptionB5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_B5);
    Hz_OptionB5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_B5);
    Hz_OptionB5.Write("Hz_OptionB5");
    std::cout << "OptionB5 : rate@thr32 = " << Hz_OptionB5[0] << "  -  rate@thr33 = " << Hz_OptionB5[1] << std::endl; 

    TVectorF Hz_OptionC5(2);
    Hz_OptionC5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_C5);
    Hz_OptionC5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_C5);
    Hz_OptionC5.Write("Hz_OptionC5");
    std::cout << "OptionC5 : rate@thr32 = " << Hz_OptionC5[0] << "  -  rate@thr33 = " << Hz_OptionC5[1] << std::endl; 

    TVectorF Hz_OptionD5(2);
    Hz_OptionD5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_D5);
    Hz_OptionD5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_D5);
    Hz_OptionD5.Write("Hz_OptionD5");
    std::cout << "OptionD5 : rate@thr32 = " << Hz_OptionD5[0] << "  -  rate@thr33 = " << Hz_OptionD5[1] << std::endl; 

    TVectorF Hz_OptionE5(2);
    Hz_OptionE5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_E5);
    Hz_OptionE5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_E5);
    Hz_OptionE5.Write("Hz_OptionE5");
    std::cout << "OptionE5 : rate@thr32 = " << Hz_OptionE5[0] << "  -  rate@thr33 = " << Hz_OptionE5[1] << std::endl; 

    TVectorF Hz_OptionF5(2);
    Hz_OptionF5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_F5);
    Hz_OptionF5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_F5);
    Hz_OptionF5.Write("Hz_OptionF5");
    std::cout << "OptionF5 : rate@thr32 = " << Hz_OptionF5[0] << "  -  rate@thr33 = " << Hz_OptionF5[1] << std::endl; 

    TVectorF Hz_OptionG5(2);
    Hz_OptionG5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_G5);
    Hz_OptionG5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_G5);
    Hz_OptionG5.Write("Hz_OptionG5");
    std::cout << "OptionG5 : rate@thr32 = " << Hz_OptionG5[0] << "  -  rate@thr33 = " << Hz_OptionG5[1] << std::endl; 

    TVectorF Hz_OptionH5(2);
    Hz_OptionH5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_H5);
    Hz_OptionH5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_H5);
    Hz_OptionH5.Write("Hz_OptionH5");
    std::cout << "OptionH5 : rate@thr32 = " << Hz_OptionH5[0] << "  -  rate@thr33 = " << Hz_OptionH5[1] << std::endl; 

    TVectorF Hz_OptionI5(2);
    Hz_OptionI5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_I5);
    Hz_OptionI5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_I5);
    Hz_OptionI5.Write("Hz_OptionI5");
    std::cout << "OptionI5 : rate@thr32 = " << Hz_OptionI5[0] << "  -  rate@thr33 = " << Hz_OptionI5[1] << std::endl; 

    TVectorF Hz_OptionJ5(2);
    Hz_OptionJ5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_J5);
    Hz_OptionJ5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_J5);
    Hz_OptionJ5.Write("Hz_OptionJ5");
    std::cout << "OptionJ5 : rate@thr32 = " << Hz_OptionJ5[0] << "  -  rate@thr33 = " << Hz_OptionJ5[1] << std::endl; 

    TVectorF Hz_OptionK5(2);
    Hz_OptionK5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_K5);
    Hz_OptionK5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_K5);
    Hz_OptionK5.Write("Hz_OptionK5");
    std::cout << "OptionK5 : rate@thr32 = " << Hz_OptionK5[0] << "  -  rate@thr33 = " << Hz_OptionK5[1] << std::endl; 

    TVectorF Hz_OptionL5(2);
    Hz_OptionL5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_L5);
    Hz_OptionL5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_L5);
    Hz_OptionL5.Write("Hz_OptionL5");
    std::cout << "OptionL5 : rate@thr32 = " << Hz_OptionL5[0] << "  -  rate@thr33 = " << Hz_OptionL5[1] << std::endl; 

    TVectorF Hz_OptionM5(2);
    Hz_OptionM5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_M5);
    Hz_OptionM5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_M5);
    Hz_OptionM5.Write("Hz_OptionM5");
    std::cout << "OptionM5 : rate@thr32 = " << Hz_OptionM5[0] << "  -  rate@thr33 = " << Hz_OptionM5[1] << std::endl; 

    TVectorF Hz_OptionN5(2);
    Hz_OptionN5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_N5);
    Hz_OptionN5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_N5);
    Hz_OptionN5.Write("Hz_OptionN5");
    std::cout << "OptionN5 : rate@thr32 = " << Hz_OptionN5[0] << "  -  rate@thr33 = " << Hz_OptionN5[1] << std::endl; 

    TVectorF Hz_OptionO5(2);
    Hz_OptionO5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_O5);
    Hz_OptionO5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_O5);
    Hz_OptionO5.Write("Hz_OptionO5");
    std::cout << "OptionO5 : rate@thr32 = " << Hz_OptionO5[0] << "  -  rate@thr33 = " << Hz_OptionO5[1] << std::endl; 

    TVectorF Hz_OptionP5(2);
    Hz_OptionP5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_P5);
    Hz_OptionP5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_P5);
    Hz_OptionP5.Write("Hz_OptionP5");
    std::cout << "OptionP5 : rate@thr32 = " << Hz_OptionP5[0] << "  -  rate@thr33 = " << Hz_OptionP5[1] << std::endl; 

    TVectorF Hz_OptionQ5(2);
    Hz_OptionQ5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Q5);
    Hz_OptionQ5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Q5);
    Hz_OptionQ5.Write("Hz_OptionQ5");
    std::cout << "OptionQ5 : rate@thr32 = " << Hz_OptionQ5[0] << "  -  rate@thr33 = " << Hz_OptionQ5[1] << std::endl; 

    TVectorF Hz_OptionR5(2);
    Hz_OptionR5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_R5);
    Hz_OptionR5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_R5);
    Hz_OptionR5.Write("Hz_OptionR5");
    std::cout << "OptionR5 : rate@thr32 = " << Hz_OptionR5[0] << "  -  rate@thr33 = " << Hz_OptionR5[1] << std::endl; 

    TVectorF Hz_OptionS5(2);
    Hz_OptionS5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_S5);
    Hz_OptionS5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_S5);
    Hz_OptionS5.Write("Hz_OptionS5");
    std::cout << "OptionS5 : rate@thr32 = " << Hz_OptionS5[0] << "  -  rate@thr33 = " << Hz_OptionS5[1] << std::endl; 

    TVectorF Hz_OptionT5(2);
    Hz_OptionT5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_T5);
    Hz_OptionT5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_T5);
    Hz_OptionT5.Write("Hz_OptionT5");
    std::cout << "OptionT5 : rate@thr32 = " << Hz_OptionT5[0] << "  -  rate@thr33 = " << Hz_OptionT5[1] << std::endl; 

    TVectorF Hz_OptionU5(2);
    Hz_OptionU5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_U5);
    Hz_OptionU5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_U5);
    Hz_OptionU5.Write("Hz_OptionU5");
    std::cout << "OptionU5 : rate@thr32 = " << Hz_OptionU5[0] << "  -  rate@thr33 = " << Hz_OptionU5[1] << std::endl; 

    TVectorF Hz_OptionV5(2);
    Hz_OptionV5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_V5);
    Hz_OptionV5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_V5);
    Hz_OptionV5.Write("Hz_OptionV5");
    std::cout << "OptionV5 : rate@thr32 = " << Hz_OptionV5[0] << "  -  rate@thr33 = " << Hz_OptionV5[1] << std::endl; 

    TVectorF Hz_OptionW5(2);
    Hz_OptionW5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_W5);
    Hz_OptionW5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_W5);
    Hz_OptionW5.Write("Hz_OptionW5");
    std::cout << "OptionW5 : rate@thr32 = " << Hz_OptionW5[0] << "  -  rate@thr33 = " << Hz_OptionW5[1] << std::endl; 

    TVectorF Hz_OptionX5(2);
    Hz_OptionX5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_X5);
    Hz_OptionX5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_X5);
    Hz_OptionX5.Write("Hz_OptionX5");
    std::cout << "OptionX5 : rate@thr32 = " << Hz_OptionX5[0] << "  -  rate@thr33 = " << Hz_OptionX5[1] << std::endl; 

    TVectorF Hz_OptionY5(2);
    Hz_OptionY5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Y5);
    Hz_OptionY5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Y5);
    Hz_OptionY5.Write("Hz_OptionY5");
    std::cout << "OptionY5 : rate@thr32 = " << Hz_OptionY5[0] << "  -  rate@thr33 = " << Hz_OptionY5[1] << std::endl; 

    TVectorF Hz_OptionZ5(2);
    Hz_OptionZ5[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Z5);
    Hz_OptionZ5[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Z5);
    Hz_OptionZ5.Write("Hz_OptionZ5");
    std::cout << "OptionZ5 : rate@thr32 = " << Hz_OptionZ5[0] << "  -  rate@thr33 = " << Hz_OptionZ5[1] << std::endl; 

    TVectorF Hz_OptionA6(2);
    Hz_OptionA6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_A6);
    Hz_OptionA6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_A6);
    Hz_OptionA6.Write("Hz_OptionA6");
    std::cout << "OptionA6 : rate@thr32 = " << Hz_OptionA6[0] << "  -  rate@thr33 = " << Hz_OptionA6[1] << std::endl; 

    TVectorF Hz_OptionB6(2);
    Hz_OptionB6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_B6);
    Hz_OptionB6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_B6);
    Hz_OptionB6.Write("Hz_OptionB6");
    std::cout << "OptionB6 : rate@thr32 = " << Hz_OptionB6[0] << "  -  rate@thr33 = " << Hz_OptionB6[1] << std::endl; 

    TVectorF Hz_OptionC6(2);
    Hz_OptionC6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_C6);
    Hz_OptionC6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_C6);
    Hz_OptionC6.Write("Hz_OptionC6");
    std::cout << "OptionC6 : rate@thr32 = " << Hz_OptionC6[0] << "  -  rate@thr33 = " << Hz_OptionC6[1] << std::endl; 

    TVectorF Hz_OptionD6(2);
    Hz_OptionD6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_D6);
    Hz_OptionD6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_D6);
    Hz_OptionD6.Write("Hz_OptionD6");
    std::cout << "OptionD6 : rate@thr32 = " << Hz_OptionD6[0] << "  -  rate@thr33 = " << Hz_OptionD6[1] << std::endl; 

    TVectorF Hz_OptionE6(2);
    Hz_OptionE6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_E6);
    Hz_OptionE6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_E6);
    Hz_OptionE6.Write("Hz_OptionE6");
    std::cout << "OptionE6 : rate@thr32 = " << Hz_OptionE6[0] << "  -  rate@thr33 = " << Hz_OptionE6[1] << std::endl; 

    TVectorF Hz_OptionF6(2);
    Hz_OptionF6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_F6);
    Hz_OptionF6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_F6);
    Hz_OptionF6.Write("Hz_OptionF6");
    std::cout << "OptionF6 : rate@thr32 = " << Hz_OptionF6[0] << "  -  rate@thr33 = " << Hz_OptionF6[1] << std::endl; 

    TVectorF Hz_OptionG6(2);
    Hz_OptionG6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_G6);
    Hz_OptionG6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_G6);
    Hz_OptionG6.Write("Hz_OptionG6");
    std::cout << "OptionG6 : rate@thr32 = " << Hz_OptionG6[0] << "  -  rate@thr33 = " << Hz_OptionG6[1] << std::endl; 

    TVectorF Hz_OptionH6(2);
    Hz_OptionH6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_H6);
    Hz_OptionH6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_H6);
    Hz_OptionH6.Write("Hz_OptionH6");
    std::cout << "OptionH6 : rate@thr32 = " << Hz_OptionH6[0] << "  -  rate@thr33 = " << Hz_OptionH6[1] << std::endl; 

    TVectorF Hz_OptionI6(2);
    Hz_OptionI6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_I6);
    Hz_OptionI6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_I6);
    Hz_OptionI6.Write("Hz_OptionI6");
    std::cout << "OptionI6 : rate@thr32 = " << Hz_OptionI6[0] << "  -  rate@thr33 = " << Hz_OptionI6[1] << std::endl; 

    TVectorF Hz_OptionJ6(2);
    Hz_OptionJ6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_J6);
    Hz_OptionJ6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_J6);
    Hz_OptionJ6.Write("Hz_OptionJ6");
    std::cout << "OptionJ6 : rate@thr32 = " << Hz_OptionJ6[0] << "  -  rate@thr33 = " << Hz_OptionJ6[1] << std::endl; 

    TVectorF Hz_OptionK6(2);
    Hz_OptionK6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_K6);
    Hz_OptionK6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_K6);
    Hz_OptionK6.Write("Hz_OptionK6");
    std::cout << "OptionK6 : rate@thr32 = " << Hz_OptionK6[0] << "  -  rate@thr33 = " << Hz_OptionK6[1] << std::endl; 

    TVectorF Hz_OptionL6(2);
    Hz_OptionL6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_L6);
    Hz_OptionL6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_L6);
    Hz_OptionL6.Write("Hz_OptionL6");
    std::cout << "OptionL6 : rate@thr32 = " << Hz_OptionL6[0] << "  -  rate@thr33 = " << Hz_OptionL6[1] << std::endl; 

    TVectorF Hz_OptionM6(2);
    Hz_OptionM6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_M6);
    Hz_OptionM6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_M6);
    Hz_OptionM6.Write("Hz_OptionM6");
    std::cout << "OptionM6 : rate@thr32 = " << Hz_OptionM6[0] << "  -  rate@thr33 = " << Hz_OptionM6[1] << std::endl; 

    TVectorF Hz_OptionN6(2);
    Hz_OptionN6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_N6);
    Hz_OptionN6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_N6);
    Hz_OptionN6.Write("Hz_OptionN6");
    std::cout << "OptionN6 : rate@thr32 = " << Hz_OptionN6[0] << "  -  rate@thr33 = " << Hz_OptionN6[1] << std::endl; 

    TVectorF Hz_OptionO6(2);
    Hz_OptionO6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_O6);
    Hz_OptionO6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_O6);
    Hz_OptionO6.Write("Hz_OptionO6");
    std::cout << "OptionO6 : rate@thr32 = " << Hz_OptionO6[0] << "  -  rate@thr33 = " << Hz_OptionO6[1] << std::endl; 

    TVectorF Hz_OptionP6(2);
    Hz_OptionP6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_P6);
    Hz_OptionP6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_P6);
    Hz_OptionP6.Write("Hz_OptionP6");
    std::cout << "OptionP6 : rate@thr32 = " << Hz_OptionP6[0] << "  -  rate@thr33 = " << Hz_OptionP6[1] << std::endl; 

    TVectorF Hz_OptionQ6(2);
    Hz_OptionQ6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Q6);
    Hz_OptionQ6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Q6);
    Hz_OptionQ6.Write("Hz_OptionQ6");
    std::cout << "OptionQ6 : rate@thr32 = " << Hz_OptionQ6[0] << "  -  rate@thr33 = " << Hz_OptionQ6[1] << std::endl; 

    TVectorF Hz_OptionR6(2);
    Hz_OptionR6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_R6);
    Hz_OptionR6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_R6);
    Hz_OptionR6.Write("Hz_OptionR6");
    std::cout << "OptionR6 : rate@thr32 = " << Hz_OptionR6[0] << "  -  rate@thr33 = " << Hz_OptionR6[1] << std::endl; 

    TVectorF Hz_OptionS6(2);
    Hz_OptionS6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_S6);
    Hz_OptionS6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_S6);
    Hz_OptionS6.Write("Hz_OptionS6");
    std::cout << "OptionS6 : rate@thr32 = " << Hz_OptionS6[0] << "  -  rate@thr33 = " << Hz_OptionS6[1] << std::endl; 

    TVectorF Hz_OptionT6(2);
    Hz_OptionT6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_T6);
    Hz_OptionT6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_T6);
    Hz_OptionT6.Write("Hz_OptionT6");
    std::cout << "OptionT6 : rate@thr32 = " << Hz_OptionT6[0] << "  -  rate@thr33 = " << Hz_OptionT6[1] << std::endl; 

    TVectorF Hz_OptionU6(2);
    Hz_OptionU6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_U6);
    Hz_OptionU6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_U6);
    Hz_OptionU6.Write("Hz_OptionU6");
    std::cout << "OptionU6 : rate@thr32 = " << Hz_OptionU6[0] << "  -  rate@thr33 = " << Hz_OptionU6[1] << std::endl; 

    TVectorF Hz_OptionV6(2);
    Hz_OptionV6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_V6);
    Hz_OptionV6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_V6);
    Hz_OptionV6.Write("Hz_OptionV6");
    std::cout << "OptionV6 : rate@thr32 = " << Hz_OptionV6[0] << "  -  rate@thr33 = " << Hz_OptionV6[1] << std::endl; 

    TVectorF Hz_OptionW6(2);
    Hz_OptionW6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_W6);
    Hz_OptionW6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_W6);
    Hz_OptionW6.Write("Hz_OptionW6");
    std::cout << "OptionW6 : rate@thr32 = " << Hz_OptionW6[0] << "  -  rate@thr33 = " << Hz_OptionW6[1] << std::endl; 

    TVectorF Hz_OptionX6(2);
    Hz_OptionX6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_X6);
    Hz_OptionX6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_X6);
    Hz_OptionX6.Write("Hz_OptionX6");
    std::cout << "OptionX6 : rate@thr32 = " << Hz_OptionX6[0] << "  -  rate@thr33 = " << Hz_OptionX6[1] << std::endl; 

    TVectorF Hz_OptionY6(2);
    Hz_OptionY6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Y6);
    Hz_OptionY6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Y6);
    Hz_OptionY6.Write("Hz_OptionY6");
    std::cout << "OptionY6 : rate@thr32 = " << Hz_OptionY6[0] << "  -  rate@thr33 = " << Hz_OptionY6[1] << std::endl; 

    TVectorF Hz_OptionZ6(2);
    Hz_OptionZ6[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Z6);
    Hz_OptionZ6[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Z6);
    Hz_OptionZ6.Write("Hz_OptionZ6");
    std::cout << "OptionZ6 : rate@thr32 = " << Hz_OptionZ6[0] << "  -  rate@thr33 = " << Hz_OptionZ6[1] << std::endl; 

    TVectorF Hz_OptionA7(2);
    Hz_OptionA7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_A7);
    Hz_OptionA7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_A7);
    Hz_OptionA7.Write("Hz_OptionA7");
    std::cout << "OptionA7 : rate@thr32 = " << Hz_OptionA7[0] << "  -  rate@thr33 = " << Hz_OptionA7[1] << std::endl; 

    TVectorF Hz_OptionB7(2);
    Hz_OptionB7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_B7);
    Hz_OptionB7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_B7);
    Hz_OptionB7.Write("Hz_OptionB7");
    std::cout << "OptionB7 : rate@thr32 = " << Hz_OptionB7[0] << "  -  rate@thr33 = " << Hz_OptionB7[1] << std::endl; 

    TVectorF Hz_OptionC7(2);
    Hz_OptionC7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_C7);
    Hz_OptionC7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_C7);
    Hz_OptionC7.Write("Hz_OptionC7");
    std::cout << "OptionC7 : rate@thr32 = " << Hz_OptionC7[0] << "  -  rate@thr33 = " << Hz_OptionC7[1] << std::endl; 

    TVectorF Hz_OptionD7(2);
    Hz_OptionD7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_D7);
    Hz_OptionD7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_D7);
    Hz_OptionD7.Write("Hz_OptionD7");
    std::cout << "OptionD7 : rate@thr32 = " << Hz_OptionD7[0] << "  -  rate@thr33 = " << Hz_OptionD7[1] << std::endl; 

    TVectorF Hz_OptionE7(2);
    Hz_OptionE7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_E7);
    Hz_OptionE7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_E7);
    Hz_OptionE7.Write("Hz_OptionE7");
    std::cout << "OptionE7 : rate@thr32 = " << Hz_OptionE7[0] << "  -  rate@thr33 = " << Hz_OptionE7[1] << std::endl; 

    TVectorF Hz_OptionF7(2);
    Hz_OptionF7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_F7);
    Hz_OptionF7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_F7);
    Hz_OptionF7.Write("Hz_OptionF7");
    std::cout << "OptionF7 : rate@thr32 = " << Hz_OptionF7[0] << "  -  rate@thr33 = " << Hz_OptionF7[1] << std::endl; 

    TVectorF Hz_OptionG7(2);
    Hz_OptionG7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_G7);
    Hz_OptionG7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_G7);
    Hz_OptionG7.Write("Hz_OptionG7");
    std::cout << "OptionG7 : rate@thr32 = " << Hz_OptionG7[0] << "  -  rate@thr33 = " << Hz_OptionG7[1] << std::endl; 

    TVectorF Hz_OptionH7(2);
    Hz_OptionH7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_H7);
    Hz_OptionH7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_H7);
    Hz_OptionH7.Write("Hz_OptionH7");
    std::cout << "OptionH7 : rate@thr32 = " << Hz_OptionH7[0] << "  -  rate@thr33 = " << Hz_OptionH7[1] << std::endl; 

    TVectorF Hz_OptionI7(2);
    Hz_OptionI7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_I7);
    Hz_OptionI7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_I7);
    Hz_OptionI7.Write("Hz_OptionI7");
    std::cout << "OptionI7 : rate@thr32 = " << Hz_OptionI7[0] << "  -  rate@thr33 = " << Hz_OptionI7[1] << std::endl; 

    TVectorF Hz_OptionJ7(2);
    Hz_OptionJ7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_J7);
    Hz_OptionJ7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_J7);
    Hz_OptionJ7.Write("Hz_OptionJ7");
    std::cout << "OptionJ7 : rate@thr32 = " << Hz_OptionJ7[0] << "  -  rate@thr33 = " << Hz_OptionJ7[1] << std::endl; 

    TVectorF Hz_OptionK7(2);
    Hz_OptionK7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_K7);
    Hz_OptionK7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_K7);
    Hz_OptionK7.Write("Hz_OptionK7");
    std::cout << "OptionK7 : rate@thr32 = " << Hz_OptionK7[0] << "  -  rate@thr33 = " << Hz_OptionK7[1] << std::endl; 

    TVectorF Hz_OptionL7(2);
    Hz_OptionL7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_L7);
    Hz_OptionL7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_L7);
    Hz_OptionL7.Write("Hz_OptionL7");
    std::cout << "OptionL7 : rate@thr32 = " << Hz_OptionL7[0] << "  -  rate@thr33 = " << Hz_OptionL7[1] << std::endl; 

    TVectorF Hz_OptionM7(2);
    Hz_OptionM7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_M7);
    Hz_OptionM7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_M7);
    Hz_OptionM7.Write("Hz_OptionM7");
    std::cout << "OptionM7 : rate@thr32 = " << Hz_OptionM7[0] << "  -  rate@thr33 = " << Hz_OptionM7[1] << std::endl; 

    TVectorF Hz_OptionN7(2);
    Hz_OptionN7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_N7);
    Hz_OptionN7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_N7);
    Hz_OptionN7.Write("Hz_OptionN7");
    std::cout << "OptionN7 : rate@thr32 = " << Hz_OptionN7[0] << "  -  rate@thr33 = " << Hz_OptionN7[1] << std::endl; 

    TVectorF Hz_OptionO7(2);
    Hz_OptionO7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_O7);
    Hz_OptionO7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_O7);
    Hz_OptionO7.Write("Hz_OptionO7");
    std::cout << "OptionO7 : rate@thr32 = " << Hz_OptionO7[0] << "  -  rate@thr33 = " << Hz_OptionO7[1] << std::endl; 

    TVectorF Hz_OptionP7(2);
    Hz_OptionP7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_P7);
    Hz_OptionP7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_P7);
    Hz_OptionP7.Write("Hz_OptionP7");
    std::cout << "OptionP7 : rate@thr32 = " << Hz_OptionP7[0] << "  -  rate@thr33 = " << Hz_OptionP7[1] << std::endl; 

    TVectorF Hz_OptionQ7(2);
    Hz_OptionQ7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Q7);
    Hz_OptionQ7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Q7);
    Hz_OptionQ7.Write("Hz_OptionQ7");
    std::cout << "OptionQ7 : rate@thr32 = " << Hz_OptionQ7[0] << "  -  rate@thr33 = " << Hz_OptionQ7[1] << std::endl; 

    TVectorF Hz_OptionR7(2);
    Hz_OptionR7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_R7);
    Hz_OptionR7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_R7);
    Hz_OptionR7.Write("Hz_OptionR7");
    std::cout << "OptionR7 : rate@thr32 = " << Hz_OptionR7[0] << "  -  rate@thr33 = " << Hz_OptionR7[1] << std::endl; 

    TVectorF Hz_OptionS7(2);
    Hz_OptionS7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_S7);
    Hz_OptionS7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_S7);
    Hz_OptionS7.Write("Hz_OptionS7");
    std::cout << "OptionS7 : rate@thr32 = " << Hz_OptionS7[0] << "  -  rate@thr33 = " << Hz_OptionS7[1] << std::endl; 

    TVectorF Hz_OptionT7(2);
    Hz_OptionT7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_T7);
    Hz_OptionT7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_T7);
    Hz_OptionT7.Write("Hz_OptionT7");
    std::cout << "OptionT7 : rate@thr32 = " << Hz_OptionT7[0] << "  -  rate@thr33 = " << Hz_OptionT7[1] << std::endl; 

    TVectorF Hz_OptionU7(2);
    Hz_OptionU7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_U7);
    Hz_OptionU7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_U7);
    Hz_OptionU7.Write("Hz_OptionU7");
    std::cout << "OptionU7 : rate@thr32 = " << Hz_OptionU7[0] << "  -  rate@thr33 = " << Hz_OptionU7[1] << std::endl; 

    TVectorF Hz_OptionV7(2);
    Hz_OptionV7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_V7);
    Hz_OptionV7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_V7);
    Hz_OptionV7.Write("Hz_OptionV7");
    std::cout << "OptionV7 : rate@thr32 = " << Hz_OptionV7[0] << "  -  rate@thr33 = " << Hz_OptionV7[1] << std::endl; 

    TVectorF Hz_OptionW7(2);
    Hz_OptionW7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_W7);
    Hz_OptionW7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_W7);
    Hz_OptionW7.Write("Hz_OptionW7");
    std::cout << "OptionW7 : rate@thr32 = " << Hz_OptionW7[0] << "  -  rate@thr33 = " << Hz_OptionW7[1] << std::endl; 

    TVectorF Hz_OptionX7(2);
    Hz_OptionX7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_X7);
    Hz_OptionX7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_X7);
    Hz_OptionX7.Write("Hz_OptionX7");
    std::cout << "OptionX7 : rate@thr32 = " << Hz_OptionX7[0] << "  -  rate@thr33 = " << Hz_OptionX7[1] << std::endl; 

    TVectorF Hz_OptionY7(2);
    Hz_OptionY7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Y7);
    Hz_OptionY7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Y7);
    Hz_OptionY7.Write("Hz_OptionY7");
    std::cout << "OptionY7 : rate@thr32 = " << Hz_OptionY7[0] << "  -  rate@thr33 = " << Hz_OptionY7[1] << std::endl; 

    TVectorF Hz_OptionZ7(2);
    Hz_OptionZ7[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Z7);
    Hz_OptionZ7[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Z7);
    Hz_OptionZ7.Write("Hz_OptionZ7");
    std::cout << "OptionZ7 : rate@thr32 = " << Hz_OptionZ7[0] << "  -  rate@thr33 = " << Hz_OptionZ7[1] << std::endl; 

    TVectorF Hz_OptionA8(2);
    Hz_OptionA8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_A8);
    Hz_OptionA8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_A8);
    Hz_OptionA8.Write("Hz_OptionA8");
    std::cout << "OptionA8 : rate@thr32 = " << Hz_OptionA8[0] << "  -  rate@thr33 = " << Hz_OptionA8[1] << std::endl; 

    TVectorF Hz_OptionB8(2);
    Hz_OptionB8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_B8);
    Hz_OptionB8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_B8);
    Hz_OptionB8.Write("Hz_OptionB8");
    std::cout << "OptionB8 : rate@thr32 = " << Hz_OptionB8[0] << "  -  rate@thr33 = " << Hz_OptionB8[1] << std::endl; 

    TVectorF Hz_OptionC8(2);
    Hz_OptionC8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_C8);
    Hz_OptionC8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_C8);
    Hz_OptionC8.Write("Hz_OptionC8");
    std::cout << "OptionC8 : rate@thr32 = " << Hz_OptionC8[0] << "  -  rate@thr33 = " << Hz_OptionC8[1] << std::endl; 

    TVectorF Hz_OptionD8(2);
    Hz_OptionD8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_D8);
    Hz_OptionD8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_D8);
    Hz_OptionD8.Write("Hz_OptionD8");
    std::cout << "OptionD8 : rate@thr32 = " << Hz_OptionD8[0] << "  -  rate@thr33 = " << Hz_OptionD8[1] << std::endl; 

    TVectorF Hz_OptionE8(2);
    Hz_OptionE8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_E8);
    Hz_OptionE8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_E8);
    Hz_OptionE8.Write("Hz_OptionE8");
    std::cout << "OptionE8 : rate@thr32 = " << Hz_OptionE8[0] << "  -  rate@thr33 = " << Hz_OptionE8[1] << std::endl; 

    TVectorF Hz_OptionF8(2);
    Hz_OptionF8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_F8);
    Hz_OptionF8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_F8);
    Hz_OptionF8.Write("Hz_OptionF8");
    std::cout << "OptionF8 : rate@thr32 = " << Hz_OptionF8[0] << "  -  rate@thr33 = " << Hz_OptionF8[1] << std::endl; 

    TVectorF Hz_OptionG8(2);
    Hz_OptionG8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_G8);
    Hz_OptionG8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_G8);
    Hz_OptionG8.Write("Hz_OptionG8");
    std::cout << "OptionG8 : rate@thr32 = " << Hz_OptionG8[0] << "  -  rate@thr33 = " << Hz_OptionG8[1] << std::endl; 

    TVectorF Hz_OptionH8(2);
    Hz_OptionH8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_H8);
    Hz_OptionH8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_H8);
    Hz_OptionH8.Write("Hz_OptionH8");
    std::cout << "OptionH8 : rate@thr32 = " << Hz_OptionH8[0] << "  -  rate@thr33 = " << Hz_OptionH8[1] << std::endl; 

    TVectorF Hz_OptionI8(2);
    Hz_OptionI8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_I8);
    Hz_OptionI8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_I8);
    Hz_OptionI8.Write("Hz_OptionI8");
    std::cout << "OptionI8 : rate@thr32 = " << Hz_OptionI8[0] << "  -  rate@thr33 = " << Hz_OptionI8[1] << std::endl; 

    TVectorF Hz_OptionJ8(2);
    Hz_OptionJ8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_J8);
    Hz_OptionJ8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_J8);
    Hz_OptionJ8.Write("Hz_OptionJ8");
    std::cout << "OptionJ8 : rate@thr32 = " << Hz_OptionJ8[0] << "  -  rate@thr33 = " << Hz_OptionJ8[1] << std::endl; 

    TVectorF Hz_OptionK8(2);
    Hz_OptionK8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_K8);
    Hz_OptionK8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_K8);
    Hz_OptionK8.Write("Hz_OptionK8");
    std::cout << "OptionK8 : rate@thr32 = " << Hz_OptionK8[0] << "  -  rate@thr33 = " << Hz_OptionK8[1] << std::endl; 

    TVectorF Hz_OptionL8(2);
    Hz_OptionL8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_L8);
    Hz_OptionL8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_L8);
    Hz_OptionL8.Write("Hz_OptionL8");
    std::cout << "OptionL8 : rate@thr32 = " << Hz_OptionL8[0] << "  -  rate@thr33 = " << Hz_OptionL8[1] << std::endl; 

    TVectorF Hz_OptionM8(2);
    Hz_OptionM8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_M8);
    Hz_OptionM8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_M8);
    Hz_OptionM8.Write("Hz_OptionM8");
    std::cout << "OptionM8 : rate@thr32 = " << Hz_OptionM8[0] << "  -  rate@thr33 = " << Hz_OptionM8[1] << std::endl; 

    TVectorF Hz_OptionN8(2);
    Hz_OptionN8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_N8);
    Hz_OptionN8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_N8);
    Hz_OptionN8.Write("Hz_OptionN8");
    std::cout << "OptionN8 : rate@thr32 = " << Hz_OptionN8[0] << "  -  rate@thr33 = " << Hz_OptionN8[1] << std::endl; 

    TVectorF Hz_OptionO8(2);
    Hz_OptionO8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_O8);
    Hz_OptionO8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_O8);
    Hz_OptionO8.Write("Hz_OptionO8");
    std::cout << "OptionO8 : rate@thr32 = " << Hz_OptionO8[0] << "  -  rate@thr33 = " << Hz_OptionO8[1] << std::endl; 

    TVectorF Hz_OptionP8(2);
    Hz_OptionP8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_P8);
    Hz_OptionP8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_P8);
    Hz_OptionP8.Write("Hz_OptionP8");
    std::cout << "OptionP8 : rate@thr32 = " << Hz_OptionP8[0] << "  -  rate@thr33 = " << Hz_OptionP8[1] << std::endl; 

    TVectorF Hz_OptionQ8(2);
    Hz_OptionQ8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Q8);
    Hz_OptionQ8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Q8);
    Hz_OptionQ8.Write("Hz_OptionQ8");
    std::cout << "OptionQ8 : rate@thr32 = " << Hz_OptionQ8[0] << "  -  rate@thr33 = " << Hz_OptionQ8[1] << std::endl; 

    TVectorF Hz_OptionR8(2);
    Hz_OptionR8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_R8);
    Hz_OptionR8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_R8);
    Hz_OptionR8.Write("Hz_OptionR8");
    std::cout << "OptionR8 : rate@thr32 = " << Hz_OptionR8[0] << "  -  rate@thr33 = " << Hz_OptionR8[1] << std::endl; 

    TVectorF Hz_OptionS8(2);
    Hz_OptionS8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_S8);
    Hz_OptionS8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_S8);
    Hz_OptionS8.Write("Hz_OptionS8");
    std::cout << "OptionS8 : rate@thr32 = " << Hz_OptionS8[0] << "  -  rate@thr33 = " << Hz_OptionS8[1] << std::endl; 

    TVectorF Hz_OptionT8(2);
    Hz_OptionT8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_T8);
    Hz_OptionT8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_T8);
    Hz_OptionT8.Write("Hz_OptionT8");
    std::cout << "OptionT8 : rate@thr32 = " << Hz_OptionT8[0] << "  -  rate@thr33 = " << Hz_OptionT8[1] << std::endl; 

    TVectorF Hz_OptionU8(2);
    Hz_OptionU8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_U8);
    Hz_OptionU8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_U8);
    Hz_OptionU8.Write("Hz_OptionU8");
    std::cout << "OptionU8 : rate@thr32 = " << Hz_OptionU8[0] << "  -  rate@thr33 = " << Hz_OptionU8[1] << std::endl; 

    TVectorF Hz_OptionV8(2);
    Hz_OptionV8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_V8);
    Hz_OptionV8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_V8);
    Hz_OptionV8.Write("Hz_OptionV8");
    std::cout << "OptionV8 : rate@thr32 = " << Hz_OptionV8[0] << "  -  rate@thr33 = " << Hz_OptionV8[1] << std::endl; 

    TVectorF Hz_OptionW8(2);
    Hz_OptionW8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_W8);
    Hz_OptionW8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_W8);
    Hz_OptionW8.Write("Hz_OptionW8");
    std::cout << "OptionW8 : rate@thr32 = " << Hz_OptionW8[0] << "  -  rate@thr33 = " << Hz_OptionW8[1] << std::endl; 

    TVectorF Hz_OptionX8(2);
    Hz_OptionX8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_X8);
    Hz_OptionX8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_X8);
    Hz_OptionX8.Write("Hz_OptionX8");
    std::cout << "OptionX8 : rate@thr32 = " << Hz_OptionX8[0] << "  -  rate@thr33 = " << Hz_OptionX8[1] << std::endl; 

    TVectorF Hz_OptionY8(2);
    Hz_OptionY8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Y8);
    Hz_OptionY8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Y8);
    Hz_OptionY8.Write("Hz_OptionY8");
    std::cout << "OptionY8 : rate@thr32 = " << Hz_OptionY8[0] << "  -  rate@thr33 = " << Hz_OptionY8[1] << std::endl; 

    TVectorF Hz_OptionZ8(2);
    Hz_OptionZ8[0] = rateAtFixedThreshold(targetA, rate_DiTau_Progression_Z8);
    Hz_OptionZ8[1] = rateAtFixedThreshold(targetB, rate_DiTau_Progression_Z8);
    Hz_OptionZ8.Write("Hz_OptionZ8");
    std::cout << "OptionZ8 : rate@thr32 = " << Hz_OptionZ8[0] << "  -  rate@thr33 = " << Hz_OptionZ8[1] << std::endl; 

    TCanvas c("c","c",800.,800.);
    rate_noCut->Draw();

    rate_noCut->SetLineColor(kBlack);
    rate_IsoCut_50->SetLineColor(kRed);
    rate_IsoCut_60->SetLineColor(kBlue);
    rate_IsoCut_70->SetLineColor(kMagenta);
    rate_IsoCut_80->SetLineColor(kOrange);
    rate_IsoCut_90->SetLineColor(kViolet);
    // rate_IsoCut_Linear->SetLineColor(kOrange);

    rate_IsoCut_50->Draw("same");
    rate_IsoCut_60->Draw("same");
    rate_IsoCut_70->Draw("same");
    rate_IsoCut_80->Draw("same");
    rate_IsoCut_90->Draw("same");
    // rate_IsoCut_Linear->Draw("same");

    c.SetLogy();

    c.Write();
    //  c.SaveAs("/home/llr/cms/mperez/TauTrigger/CMSSW_9_2_10/src/TauTagAndProbe/TauTagAndProbe/test/Plots_meanparam/PlotRate/canvas_rate_MC_Run305310_mean_newnTT.pdf");
    //  c.SaveAs("/home/llr/cms/mperez/TauTrigger/CMSSW_9_2_10/src/TauTagAndProbe/TauTagAndProbe/test/Trees_meanparam/canvas_rate_MC_Run305310_mean_newnTT.root");

    // c.SaveAs("canvas_rate_new_MC_NewCompression_extendedRange_Layer1_2016_Conditions_Thomas_nTT_NewLayer1_Option5_277069_OlivierFlatWP_SK1616.pdf");
    // c.SaveAs("canvas_rate_new_MC_NewCompression_extendedRange_Layer1_2016_Conditions_Thomas_nTT_NewLayer1_Option5_277069_OlivierFlatWP_SK1616.root");

    return;
}
