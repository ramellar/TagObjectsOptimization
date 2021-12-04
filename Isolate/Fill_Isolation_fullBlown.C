#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <iostream>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TF1.h>
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
#include "../Calibrate/ApplyCalibration_newnTT.C"

using namespace std;

Double_t FindEfficiency_Progression(Double_t IEt, Double_t MinPt, Double_t Efficiency_low_MinPt, Double_t Reaching_100pc_at)
{
    Double_t Efficiency = 0; 
    Double_t Pt = IEt/2.;

    if(Pt>=Reaching_100pc_at) Efficiency = 1.;
    else if(Pt<MinPt) Efficiency = Efficiency_low_MinPt;
    else
        {
            Double_t Slope = (1.-Efficiency_low_MinPt)/(Reaching_100pc_at-MinPt);
            Efficiency = Slope*Pt + (1. - Slope*Reaching_100pc_at);
            // Efficiency = (Effiency_low_MinPt-(1.-Effiency_low_MinPt)) + (1.-Effiency_low_MinPt)/(Reaching_100pc_at-MinPt)*Pt;
        }

    if(Efficiency<0) Efficiency = 0.;
    if(Efficiency>=1) Efficiency = 1.;

    return Efficiency ;
}


void Fill_Isolation_TH3(float effMin, TString tag)
{
    std::map<TString,TH3F*> histosIsolation;
    TFile f_Isolation("LUTs/LUTisolation_Trigger_Stage2_Run3_MC_VBFHToTauTau_M125_optimizationV1.root","READ");
    TFile LUTs_Options("LUTs/LUTrelaxation_Trigger_Stage2_Run3_MC_VBFHToTauTau_M125_optimizationV1_"+tag+".root","RECREATE");

    for(UInt_t i = 0 ; i < 101 ; ++i)
        {
            TString CurrentNameHisto = "Eff_";
            ostringstream convert;
            convert << i;
            CurrentNameHisto += TString(convert.str());
            TH3F* current_Histo = (TH3F*)f_Isolation.Get(CurrentNameHisto.Data());
            histosIsolation.insert(make_pair(TString(convert.str()),current_Histo));
        }  


    TF1* extrap_function_barrel = (TF1*)f_Isolation.Get("iso_vs_compressednTT_barrel_fit");
    TF1* extrap_function_endcaps = (TF1*)f_Isolation.Get("iso_vs_compressednTT_endcaps_fit");

    Float_t par0_barrel = extrap_function_barrel->GetParameter(0);
    Float_t par1_barrel = 1.1;
    // Float_t par1_barrel = 1.1;

    Float_t par0_endcaps = extrap_function_endcaps->GetParameter(0);
    Float_t par1_endcaps = 1.0;
    // Float_t par1_endcaps = 1.5;

    // Float_t par0_barrel = extrap_function_barrel->GetParameter(0);
    // Float_t par1_barrel = extrap_function_barrel->GetParameter(1);

    // Float_t par0_endcaps = extrap_function_endcaps->GetParameter(0);
    // Float_t par1_endcaps = extrap_function_endcaps->GetParameter(1);

    TH3F* LUT_Progression_1 = new TH3F("LUT_Progression_1","LUT_Progression_1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_2 = new TH3F("LUT_Progression_2","LUT_Progression_2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_3 = new TH3F("LUT_Progression_3","LUT_Progression_3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_4 = new TH3F("LUT_Progression_4","LUT_Progression_4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_5 = new TH3F("LUT_Progression_5","LUT_Progression_5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_6 = new TH3F("LUT_Progression_6","LUT_Progression_6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_7 = new TH3F("LUT_Progression_7","LUT_Progression_7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_8 = new TH3F("LUT_Progression_8","LUT_Progression_8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_9 = new TH3F("LUT_Progression_9","LUT_Progression_9",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_10 = new TH3F("LUT_Progression_10","LUT_Progression_10",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_11 = new TH3F("LUT_Progression_11","LUT_Progression_11",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_12 = new TH3F("LUT_Progression_12","LUT_Progression_12",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_13 = new TH3F("LUT_Progression_13","LUT_Progression_13",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_14 = new TH3F("LUT_Progression_14","LUT_Progression_14",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_15 = new TH3F("LUT_Progression_15","LUT_Progression_15",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_16 = new TH3F("LUT_Progression_16","LUT_Progression_16",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_17 = new TH3F("LUT_Progression_17","LUT_Progression_17",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_18 = new TH3F("LUT_Progression_18","LUT_Progression_18",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_19 = new TH3F("LUT_Progression_19","LUT_Progression_19",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_20 = new TH3F("LUT_Progression_20","LUT_Progression_20",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_21 = new TH3F("LUT_Progression_21","LUT_Progression_21",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_22 = new TH3F("LUT_Progression_22","LUT_Progression_22",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_23 = new TH3F("LUT_Progression_23","LUT_Progression_23",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_24 = new TH3F("LUT_Progression_24","LUT_Progression_24",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_25 = new TH3F("LUT_Progression_25","LUT_Progression_25",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_26 = new TH3F("LUT_Progression_26","LUT_Progression_26",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_27 = new TH3F("LUT_Progression_27","LUT_Progression_27",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_28 = new TH3F("LUT_Progression_28","LUT_Progression_28",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_29 = new TH3F("LUT_Progression_29","LUT_Progression_29",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_30 = new TH3F("LUT_Progression_30","LUT_Progression_30",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_31 = new TH3F("LUT_Progression_31","LUT_Progression_31",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);

    TH3F* LUT_Progression_A0 = new TH3F("LUT_Progression_A0","LUT_Progression_A0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_B0 = new TH3F("LUT_Progression_B0","LUT_Progression_B0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_C0 = new TH3F("LUT_Progression_C0","LUT_Progression_C0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_D0 = new TH3F("LUT_Progression_D0","LUT_Progression_D0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_E0 = new TH3F("LUT_Progression_E0","LUT_Progression_E0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_F0 = new TH3F("LUT_Progression_F0","LUT_Progression_F0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_G0 = new TH3F("LUT_Progression_G0","LUT_Progression_G0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_H0 = new TH3F("LUT_Progression_H0","LUT_Progression_H0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_I0 = new TH3F("LUT_Progression_I0","LUT_Progression_I0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_J0 = new TH3F("LUT_Progression_J0","LUT_Progression_J0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_K0 = new TH3F("LUT_Progression_K0","LUT_Progression_K0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_L0 = new TH3F("LUT_Progression_L0","LUT_Progression_L0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_M0 = new TH3F("LUT_Progression_M0","LUT_Progression_M0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_N0 = new TH3F("LUT_Progression_N0","LUT_Progression_N0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_O0 = new TH3F("LUT_Progression_O0","LUT_Progression_O0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_P0 = new TH3F("LUT_Progression_P0","LUT_Progression_P0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Q0 = new TH3F("LUT_Progression_Q0","LUT_Progression_Q0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_R0 = new TH3F("LUT_Progression_R0","LUT_Progression_R0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_S0 = new TH3F("LUT_Progression_S0","LUT_Progression_S0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_T0 = new TH3F("LUT_Progression_T0","LUT_Progression_T0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_U0 = new TH3F("LUT_Progression_U0","LUT_Progression_U0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_V0 = new TH3F("LUT_Progression_V0","LUT_Progression_V0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_W0 = new TH3F("LUT_Progression_W0","LUT_Progression_W0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_X0 = new TH3F("LUT_Progression_X0","LUT_Progression_X0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Y0 = new TH3F("LUT_Progression_Y0","LUT_Progression_Y0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Z0 = new TH3F("LUT_Progression_Z0","LUT_Progression_Z0",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_A1 = new TH3F("LUT_Progression_A1","LUT_Progression_A1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_B1 = new TH3F("LUT_Progression_B1","LUT_Progression_B1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_C1 = new TH3F("LUT_Progression_C1","LUT_Progression_C1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_D1 = new TH3F("LUT_Progression_D1","LUT_Progression_D1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_E1 = new TH3F("LUT_Progression_E1","LUT_Progression_E1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_F1 = new TH3F("LUT_Progression_F1","LUT_Progression_F1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_G1 = new TH3F("LUT_Progression_G1","LUT_Progression_G1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_H1 = new TH3F("LUT_Progression_H1","LUT_Progression_H1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_I1 = new TH3F("LUT_Progression_I1","LUT_Progression_I1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_J1 = new TH3F("LUT_Progression_J1","LUT_Progression_J1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_K1 = new TH3F("LUT_Progression_K1","LUT_Progression_K1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_L1 = new TH3F("LUT_Progression_L1","LUT_Progression_L1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_M1 = new TH3F("LUT_Progression_M1","LUT_Progression_M1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_N1 = new TH3F("LUT_Progression_N1","LUT_Progression_N1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_O1 = new TH3F("LUT_Progression_O1","LUT_Progression_O1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_P1 = new TH3F("LUT_Progression_P1","LUT_Progression_P1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Q1 = new TH3F("LUT_Progression_Q1","LUT_Progression_Q1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_R1 = new TH3F("LUT_Progression_R1","LUT_Progression_R1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_S1 = new TH3F("LUT_Progression_S1","LUT_Progression_S1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_T1 = new TH3F("LUT_Progression_T1","LUT_Progression_T1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_U1 = new TH3F("LUT_Progression_U1","LUT_Progression_U1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_V1 = new TH3F("LUT_Progression_V1","LUT_Progression_V1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_W1 = new TH3F("LUT_Progression_W1","LUT_Progression_W1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_X1 = new TH3F("LUT_Progression_X1","LUT_Progression_X1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Y1 = new TH3F("LUT_Progression_Y1","LUT_Progression_Y1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Z1 = new TH3F("LUT_Progression_Z1","LUT_Progression_Z1",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_A2 = new TH3F("LUT_Progression_A2","LUT_Progression_A2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_B2 = new TH3F("LUT_Progression_B2","LUT_Progression_B2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_C2 = new TH3F("LUT_Progression_C2","LUT_Progression_C2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_D2 = new TH3F("LUT_Progression_D2","LUT_Progression_D2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_E2 = new TH3F("LUT_Progression_E2","LUT_Progression_E2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_F2 = new TH3F("LUT_Progression_F2","LUT_Progression_F2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_G2 = new TH3F("LUT_Progression_G2","LUT_Progression_G2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_H2 = new TH3F("LUT_Progression_H2","LUT_Progression_H2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_I2 = new TH3F("LUT_Progression_I2","LUT_Progression_I2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_J2 = new TH3F("LUT_Progression_J2","LUT_Progression_J2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_K2 = new TH3F("LUT_Progression_K2","LUT_Progression_K2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_L2 = new TH3F("LUT_Progression_L2","LUT_Progression_L2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_M2 = new TH3F("LUT_Progression_M2","LUT_Progression_M2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_N2 = new TH3F("LUT_Progression_N2","LUT_Progression_N2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_O2 = new TH3F("LUT_Progression_O2","LUT_Progression_O2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_P2 = new TH3F("LUT_Progression_P2","LUT_Progression_P2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Q2 = new TH3F("LUT_Progression_Q2","LUT_Progression_Q2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_R2 = new TH3F("LUT_Progression_R2","LUT_Progression_R2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_S2 = new TH3F("LUT_Progression_S2","LUT_Progression_S2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_T2 = new TH3F("LUT_Progression_T2","LUT_Progression_T2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_U2 = new TH3F("LUT_Progression_U2","LUT_Progression_U2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_V2 = new TH3F("LUT_Progression_V2","LUT_Progression_V2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_W2 = new TH3F("LUT_Progression_W2","LUT_Progression_W2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_X2 = new TH3F("LUT_Progression_X2","LUT_Progression_X2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Y2 = new TH3F("LUT_Progression_Y2","LUT_Progression_Y2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Z2 = new TH3F("LUT_Progression_Z2","LUT_Progression_Z2",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_A3 = new TH3F("LUT_Progression_A3","LUT_Progression_A3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_B3 = new TH3F("LUT_Progression_B3","LUT_Progression_B3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_C3 = new TH3F("LUT_Progression_C3","LUT_Progression_C3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_D3 = new TH3F("LUT_Progression_D3","LUT_Progression_D3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_E3 = new TH3F("LUT_Progression_E3","LUT_Progression_E3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_F3 = new TH3F("LUT_Progression_F3","LUT_Progression_F3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_G3 = new TH3F("LUT_Progression_G3","LUT_Progression_G3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_H3 = new TH3F("LUT_Progression_H3","LUT_Progression_H3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_I3 = new TH3F("LUT_Progression_I3","LUT_Progression_I3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_J3 = new TH3F("LUT_Progression_J3","LUT_Progression_J3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_K3 = new TH3F("LUT_Progression_K3","LUT_Progression_K3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_L3 = new TH3F("LUT_Progression_L3","LUT_Progression_L3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_M3 = new TH3F("LUT_Progression_M3","LUT_Progression_M3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_N3 = new TH3F("LUT_Progression_N3","LUT_Progression_N3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_O3 = new TH3F("LUT_Progression_O3","LUT_Progression_O3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_P3 = new TH3F("LUT_Progression_P3","LUT_Progression_P3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Q3 = new TH3F("LUT_Progression_Q3","LUT_Progression_Q3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_R3 = new TH3F("LUT_Progression_R3","LUT_Progression_R3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_S3 = new TH3F("LUT_Progression_S3","LUT_Progression_S3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_T3 = new TH3F("LUT_Progression_T3","LUT_Progression_T3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_U3 = new TH3F("LUT_Progression_U3","LUT_Progression_U3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_V3 = new TH3F("LUT_Progression_V3","LUT_Progression_V3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_W3 = new TH3F("LUT_Progression_W3","LUT_Progression_W3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_X3 = new TH3F("LUT_Progression_X3","LUT_Progression_X3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Y3 = new TH3F("LUT_Progression_Y3","LUT_Progression_Y3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Z3 = new TH3F("LUT_Progression_Z3","LUT_Progression_Z3",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_A4 = new TH3F("LUT_Progression_A4","LUT_Progression_A4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_B4 = new TH3F("LUT_Progression_B4","LUT_Progression_B4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_C4 = new TH3F("LUT_Progression_C4","LUT_Progression_C4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_D4 = new TH3F("LUT_Progression_D4","LUT_Progression_D4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_E4 = new TH3F("LUT_Progression_E4","LUT_Progression_E4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_F4 = new TH3F("LUT_Progression_F4","LUT_Progression_F4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_G4 = new TH3F("LUT_Progression_G4","LUT_Progression_G4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_H4 = new TH3F("LUT_Progression_H4","LUT_Progression_H4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_I4 = new TH3F("LUT_Progression_I4","LUT_Progression_I4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_J4 = new TH3F("LUT_Progression_J4","LUT_Progression_J4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_K4 = new TH3F("LUT_Progression_K4","LUT_Progression_K4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_L4 = new TH3F("LUT_Progression_L4","LUT_Progression_L4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_M4 = new TH3F("LUT_Progression_M4","LUT_Progression_M4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_N4 = new TH3F("LUT_Progression_N4","LUT_Progression_N4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_O4 = new TH3F("LUT_Progression_O4","LUT_Progression_O4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_P4 = new TH3F("LUT_Progression_P4","LUT_Progression_P4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Q4 = new TH3F("LUT_Progression_Q4","LUT_Progression_Q4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_R4 = new TH3F("LUT_Progression_R4","LUT_Progression_R4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_S4 = new TH3F("LUT_Progression_S4","LUT_Progression_S4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_T4 = new TH3F("LUT_Progression_T4","LUT_Progression_T4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_U4 = new TH3F("LUT_Progression_U4","LUT_Progression_U4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_V4 = new TH3F("LUT_Progression_V4","LUT_Progression_V4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_W4 = new TH3F("LUT_Progression_W4","LUT_Progression_W4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_X4 = new TH3F("LUT_Progression_X4","LUT_Progression_X4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Y4 = new TH3F("LUT_Progression_Y4","LUT_Progression_Y4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Z4 = new TH3F("LUT_Progression_Z4","LUT_Progression_Z4",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_A5 = new TH3F("LUT_Progression_A5","LUT_Progression_A5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_B5 = new TH3F("LUT_Progression_B5","LUT_Progression_B5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_C5 = new TH3F("LUT_Progression_C5","LUT_Progression_C5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_D5 = new TH3F("LUT_Progression_D5","LUT_Progression_D5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_E5 = new TH3F("LUT_Progression_E5","LUT_Progression_E5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_F5 = new TH3F("LUT_Progression_F5","LUT_Progression_F5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_G5 = new TH3F("LUT_Progression_G5","LUT_Progression_G5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_H5 = new TH3F("LUT_Progression_H5","LUT_Progression_H5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_I5 = new TH3F("LUT_Progression_I5","LUT_Progression_I5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_J5 = new TH3F("LUT_Progression_J5","LUT_Progression_J5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_K5 = new TH3F("LUT_Progression_K5","LUT_Progression_K5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_L5 = new TH3F("LUT_Progression_L5","LUT_Progression_L5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_M5 = new TH3F("LUT_Progression_M5","LUT_Progression_M5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_N5 = new TH3F("LUT_Progression_N5","LUT_Progression_N5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_O5 = new TH3F("LUT_Progression_O5","LUT_Progression_O5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_P5 = new TH3F("LUT_Progression_P5","LUT_Progression_P5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Q5 = new TH3F("LUT_Progression_Q5","LUT_Progression_Q5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_R5 = new TH3F("LUT_Progression_R5","LUT_Progression_R5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_S5 = new TH3F("LUT_Progression_S5","LUT_Progression_S5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_T5 = new TH3F("LUT_Progression_T5","LUT_Progression_T5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_U5 = new TH3F("LUT_Progression_U5","LUT_Progression_U5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_V5 = new TH3F("LUT_Progression_V5","LUT_Progression_V5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_W5 = new TH3F("LUT_Progression_W5","LUT_Progression_W5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_X5 = new TH3F("LUT_Progression_X5","LUT_Progression_X5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Y5 = new TH3F("LUT_Progression_Y5","LUT_Progression_Y5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Z5 = new TH3F("LUT_Progression_Z5","LUT_Progression_Z5",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_A6 = new TH3F("LUT_Progression_A6","LUT_Progression_A6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_B6 = new TH3F("LUT_Progression_B6","LUT_Progression_B6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_C6 = new TH3F("LUT_Progression_C6","LUT_Progression_C6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_D6 = new TH3F("LUT_Progression_D6","LUT_Progression_D6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_E6 = new TH3F("LUT_Progression_E6","LUT_Progression_E6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_F6 = new TH3F("LUT_Progression_F6","LUT_Progression_F6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_G6 = new TH3F("LUT_Progression_G6","LUT_Progression_G6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_H6 = new TH3F("LUT_Progression_H6","LUT_Progression_H6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_I6 = new TH3F("LUT_Progression_I6","LUT_Progression_I6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_J6 = new TH3F("LUT_Progression_J6","LUT_Progression_J6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_K6 = new TH3F("LUT_Progression_K6","LUT_Progression_K6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_L6 = new TH3F("LUT_Progression_L6","LUT_Progression_L6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_M6 = new TH3F("LUT_Progression_M6","LUT_Progression_M6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_N6 = new TH3F("LUT_Progression_N6","LUT_Progression_N6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_O6 = new TH3F("LUT_Progression_O6","LUT_Progression_O6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_P6 = new TH3F("LUT_Progression_P6","LUT_Progression_P6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Q6 = new TH3F("LUT_Progression_Q6","LUT_Progression_Q6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_R6 = new TH3F("LUT_Progression_R6","LUT_Progression_R6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_S6 = new TH3F("LUT_Progression_S6","LUT_Progression_S6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_T6 = new TH3F("LUT_Progression_T6","LUT_Progression_T6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_U6 = new TH3F("LUT_Progression_U6","LUT_Progression_U6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_V6 = new TH3F("LUT_Progression_V6","LUT_Progression_V6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_W6 = new TH3F("LUT_Progression_W6","LUT_Progression_W6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_X6 = new TH3F("LUT_Progression_X6","LUT_Progression_X6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Y6 = new TH3F("LUT_Progression_Y6","LUT_Progression_Y6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Z6 = new TH3F("LUT_Progression_Z6","LUT_Progression_Z6",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_A7 = new TH3F("LUT_Progression_A7","LUT_Progression_A7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_B7 = new TH3F("LUT_Progression_B7","LUT_Progression_B7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_C7 = new TH3F("LUT_Progression_C7","LUT_Progression_C7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_D7 = new TH3F("LUT_Progression_D7","LUT_Progression_D7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_E7 = new TH3F("LUT_Progression_E7","LUT_Progression_E7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_F7 = new TH3F("LUT_Progression_F7","LUT_Progression_F7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_G7 = new TH3F("LUT_Progression_G7","LUT_Progression_G7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_H7 = new TH3F("LUT_Progression_H7","LUT_Progression_H7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_I7 = new TH3F("LUT_Progression_I7","LUT_Progression_I7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_J7 = new TH3F("LUT_Progression_J7","LUT_Progression_J7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_K7 = new TH3F("LUT_Progression_K7","LUT_Progression_K7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_L7 = new TH3F("LUT_Progression_L7","LUT_Progression_L7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_M7 = new TH3F("LUT_Progression_M7","LUT_Progression_M7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_N7 = new TH3F("LUT_Progression_N7","LUT_Progression_N7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_O7 = new TH3F("LUT_Progression_O7","LUT_Progression_O7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_P7 = new TH3F("LUT_Progression_P7","LUT_Progression_P7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Q7 = new TH3F("LUT_Progression_Q7","LUT_Progression_Q7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_R7 = new TH3F("LUT_Progression_R7","LUT_Progression_R7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_S7 = new TH3F("LUT_Progression_S7","LUT_Progression_S7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_T7 = new TH3F("LUT_Progression_T7","LUT_Progression_T7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_U7 = new TH3F("LUT_Progression_U7","LUT_Progression_U7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_V7 = new TH3F("LUT_Progression_V7","LUT_Progression_V7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_W7 = new TH3F("LUT_Progression_W7","LUT_Progression_W7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_X7 = new TH3F("LUT_Progression_X7","LUT_Progression_X7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Y7 = new TH3F("LUT_Progression_Y7","LUT_Progression_Y7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Z7 = new TH3F("LUT_Progression_Z7","LUT_Progression_Z7",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_A8 = new TH3F("LUT_Progression_A8","LUT_Progression_A8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_B8 = new TH3F("LUT_Progression_B8","LUT_Progression_B8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_C8 = new TH3F("LUT_Progression_C8","LUT_Progression_C8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_D8 = new TH3F("LUT_Progression_D8","LUT_Progression_D8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_E8 = new TH3F("LUT_Progression_E8","LUT_Progression_E8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_F8 = new TH3F("LUT_Progression_F8","LUT_Progression_F8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_G8 = new TH3F("LUT_Progression_G8","LUT_Progression_G8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_H8 = new TH3F("LUT_Progression_H8","LUT_Progression_H8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_I8 = new TH3F("LUT_Progression_I8","LUT_Progression_I8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_J8 = new TH3F("LUT_Progression_J8","LUT_Progression_J8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_K8 = new TH3F("LUT_Progression_K8","LUT_Progression_K8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_L8 = new TH3F("LUT_Progression_L8","LUT_Progression_L8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_M8 = new TH3F("LUT_Progression_M8","LUT_Progression_M8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_N8 = new TH3F("LUT_Progression_N8","LUT_Progression_N8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_O8 = new TH3F("LUT_Progression_O8","LUT_Progression_O8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_P8 = new TH3F("LUT_Progression_P8","LUT_Progression_P8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Q8 = new TH3F("LUT_Progression_Q8","LUT_Progression_Q8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_R8 = new TH3F("LUT_Progression_R8","LUT_Progression_R8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_S8 = new TH3F("LUT_Progression_S8","LUT_Progression_S8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_T8 = new TH3F("LUT_Progression_T8","LUT_Progression_T8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_U8 = new TH3F("LUT_Progression_U8","LUT_Progression_U8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_V8 = new TH3F("LUT_Progression_V8","LUT_Progression_V8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_W8 = new TH3F("LUT_Progression_W8","LUT_Progression_W8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_X8 = new TH3F("LUT_Progression_X8","LUT_Progression_X8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Y8 = new TH3F("LUT_Progression_Y8","LUT_Progression_Y8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    TH3F* LUT_Progression_Z8 = new TH3F("LUT_Progression_Z8","LUT_Progression_Z8",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);


    TH3F* LUT_Progression_31_extrap = new TH3F("LUT_Progression_31_extrap","LUT_Progression_31_extrap",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);

    std::vector<TH3F*> LUT_WP ;
    for(UInt_t iEff = 0 ; iEff <= 100 ; ++iEff)
        {
            stringstream ss_i;
            ss_i << iEff;
            TString Appendix_i = TString(ss_i.str());

            TString NameHisto = "LUT_WP";
            NameHisto += Appendix_i ;
            TH3F* LUT_temp = new TH3F(NameHisto.Data(),NameHisto.Data(),NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
            LUT_WP.push_back(LUT_temp);
        }

    // TH3F* LUT_WP70 = new TH3F("LUT_WP70","LUT_WP70",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    // TH3F* LUT_WP80 = new TH3F("LUT_WP80","LUT_WP80",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    // TH3F* LUT_WP90 = new TH3F("LUT_WP90","LUT_WP90",NbinsIEta-1,0,NbinsIEta-1,NbinsIEt2-1,0,NbinsIEt2-1,NbinsnTT2-1,0,NbinsnTT2-1);
    
    for(Int_t i = 0 ; i < NbinsIEta-1 ; ++i)
    {
        for(Int_t j = 0 ; j < NbinsIEt2-1 ; ++j)
        {
            for(Int_t k = 0 ; k < NbinsnTT2-1 ; ++k)
            // for(Int_t k = 0 ; k < NbinsnTT-1 ; ++k)
            {
                //Progression_1
                Double_t Efficiency_Progression_1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.1, 50.);
                if(Efficiency_Progression_1>=0.9999) Efficiency_Progression_1 = 1.0001;
                Int_t Int_Efficiency_Progression_1 = int(Efficiency_Progression_1*100);
                ostringstream convert_Progression_1;
                convert_Progression_1 << Int_Efficiency_Progression_1 ;
                TString Result_Progression_1 = TString(convert_Progression_1.str());
                Int_t IsoCut_Progression_1 = histosIsolation[Result_Progression_1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_1==100) IsoCut_Progression_1 = 1000;
                LUT_Progression_1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_1);

                //        cout<<"after progression 1"<<endl;

                //Progression_2
                Double_t Efficiency_Progression_2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.4, 50.);
                if(Efficiency_Progression_2>=0.9999) Efficiency_Progression_2 = 1.0001;
                Int_t Int_Efficiency_Progression_2 = int(Efficiency_Progression_2*100);
                ostringstream convert_Progression_2;
                convert_Progression_2 << Int_Efficiency_Progression_2 ;
                TString Result_Progression_2 = TString(convert_Progression_2.str());
                Int_t IsoCut_Progression_2 = histosIsolation[Result_Progression_2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_2==100) IsoCut_Progression_2 = 1000;
                LUT_Progression_2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_2);

                //        cout<<"after progression 2"<<endl;

                //Progression_3
                Double_t Efficiency_Progression_3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.5, 50.);
                if(Efficiency_Progression_3>=0.9999) Efficiency_Progression_3 = 1.0001;
                Int_t Int_Efficiency_Progression_3 = int(Efficiency_Progression_3*100);
                ostringstream convert_Progression_3;
                convert_Progression_3 << Int_Efficiency_Progression_3 ;
                TString Result_Progression_3 = TString(convert_Progression_3.str());
                Int_t IsoCut_Progression_3 = histosIsolation[Result_Progression_3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_3==100) IsoCut_Progression_3 = 1000;
                LUT_Progression_3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_3);


                //Progression_4
                Double_t Efficiency_Progression_4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.6, 50.);
                if(Efficiency_Progression_4>=0.9999) Efficiency_Progression_4 = 1.0001;
                Int_t Int_Efficiency_Progression_4 = int(Efficiency_Progression_4*100);
                ostringstream convert_Progression_4;
                convert_Progression_4 << Int_Efficiency_Progression_4 ;
                TString Result_Progression_4 = TString(convert_Progression_4.str());
                Int_t IsoCut_Progression_4 = histosIsolation[Result_Progression_4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_4==100) IsoCut_Progression_4 = 1000;
                LUT_Progression_4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_4);

                // if(j==3)
                //  {
                //    cout<<"iet bin (4 bits) = "<<FindBinCorrespondenceIEt(hardcodedIetBins2[j])<<endl;
                //    cout<<"iet bin (6 bits) = "<<j<<endl;
                //    cout<<"ieta bin = "<<i<<endl;
                //    cout<<"nTT bin = "<<k<<endl;
                //    cout<<"iso = "<<IsoCut_Progression_4<<endl;
                //  }

                //Progression_5
                Double_t Efficiency_Progression_5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.7, 50.);
                if(Efficiency_Progression_5>=0.9999) Efficiency_Progression_5 = 1.0001;
                Int_t Int_Efficiency_Progression_5 = int(Efficiency_Progression_5*100);
                ostringstream convert_Progression_5;
                convert_Progression_5 << Int_Efficiency_Progression_5 ;
                TString Result_Progression_5 = TString(convert_Progression_5.str());
                Int_t IsoCut_Progression_5 = histosIsolation[Result_Progression_5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_5==100) IsoCut_Progression_5 = 1000;
                LUT_Progression_5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_5);

                //Progression_6
                Double_t Efficiency_Progression_6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.5, 40.);
                if(Efficiency_Progression_6>=0.9999) Efficiency_Progression_6 = 1.0001;
                Int_t Int_Efficiency_Progression_6 = int(Efficiency_Progression_6*100);
                ostringstream convert_Progression_6;
                convert_Progression_6 << Int_Efficiency_Progression_6 ;
                TString Result_Progression_6 = TString(convert_Progression_6.str());
                Int_t IsoCut_Progression_6 = histosIsolation[Result_Progression_6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_6==100) IsoCut_Progression_6 = 1000;
                LUT_Progression_6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_6);

                //        cout<<"after progression 6"<<endl;

                //Progression_7
                Double_t Efficiency_Progression_7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.5, 40.);
                if(Efficiency_Progression_7>=0.9999) Efficiency_Progression_7 = 1.0001;
                Int_t Int_Efficiency_Progression_7 = int(Efficiency_Progression_7*100);
                ostringstream convert_Progression_7;
                convert_Progression_7 << Int_Efficiency_Progression_7 ;
                TString Result_Progression_7 = TString(convert_Progression_7.str());
                Int_t IsoCut_Progression_7 = histosIsolation[Result_Progression_7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_7==100) IsoCut_Progression_7 = 1000;
                LUT_Progression_7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_7);

                //Progression_8
                Double_t Efficiency_Progression_8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.5, 60.);
                if(Efficiency_Progression_8>=0.9999) Efficiency_Progression_8 = 1.0001;
                Int_t Int_Efficiency_Progression_8 = int(Efficiency_Progression_8*100);
                ostringstream convert_Progression_8;
                convert_Progression_8 << Int_Efficiency_Progression_8 ;
                TString Result_Progression_8 = TString(convert_Progression_8.str());
                Int_t IsoCut_Progression_8 = histosIsolation[Result_Progression_8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_8==100) IsoCut_Progression_8 = 1000;
                LUT_Progression_8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_8);

                //Progression_9
                Double_t Efficiency_Progression_9 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.6, 40.);
                if(Efficiency_Progression_9>=0.9999) Efficiency_Progression_9 = 1.0001;
                Int_t Int_Efficiency_Progression_9 = int(Efficiency_Progression_9*100);
                ostringstream convert_Progression_9;
                convert_Progression_9 << Int_Efficiency_Progression_9 ;
                TString Result_Progression_9 = TString(convert_Progression_9.str());
                Int_t IsoCut_Progression_9 = histosIsolation[Result_Progression_9]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_9==100) IsoCut_Progression_9 = 1000;
                LUT_Progression_9->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_9);

                //Progression_10
                Double_t Efficiency_Progression_10 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.7, 40.);
                if(Efficiency_Progression_10>=0.9999) Efficiency_Progression_10 = 1.0001;
                Int_t Int_Efficiency_Progression_10 = int(Efficiency_Progression_10*100);
                ostringstream convert_Progression_10;
                convert_Progression_10 << Int_Efficiency_Progression_10 ;
                TString Result_Progression_10 = TString(convert_Progression_10.str());
                Int_t IsoCut_Progression_10 = histosIsolation[Result_Progression_10]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_10==100) IsoCut_Progression_10 = 1000;
                LUT_Progression_10->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_10);

                //Progression_11
                Double_t Efficiency_Progression_11 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.7, 40.);
                if(Efficiency_Progression_11>=0.9999) Efficiency_Progression_11 = 1.0001;
                Int_t Int_Efficiency_Progression_11 = int(Efficiency_Progression_11*100);
                ostringstream convert_Progression_11;
                convert_Progression_11 << Int_Efficiency_Progression_11 ;
                TString Result_Progression_11 = TString(convert_Progression_11.str());
                Int_t IsoCut_Progression_11 = histosIsolation[Result_Progression_11]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_11==100) IsoCut_Progression_11 = 1000;
                LUT_Progression_11->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_11);

                //        cout<<"after progression 11"<<endl;

                //Progression_12
                Double_t Efficiency_Progression_12 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.7, 35.);
                if(Efficiency_Progression_12>=0.9999) Efficiency_Progression_12 = 1.0001;
                Int_t Int_Efficiency_Progression_12 = int(Efficiency_Progression_12*100);
                ostringstream convert_Progression_12;
                convert_Progression_12 << Int_Efficiency_Progression_12 ;
                TString Result_Progression_12 = TString(convert_Progression_12.str());
                Int_t IsoCut_Progression_12 = histosIsolation[Result_Progression_12]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_12==100) IsoCut_Progression_12 = 1000;
                LUT_Progression_12->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_12);

                //Progression_13
                Double_t Efficiency_Progression_13 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.6, 35.);
                if(Efficiency_Progression_13>=0.9999) Efficiency_Progression_13 = 1.0001;
                Int_t Int_Efficiency_Progression_13 = int(Efficiency_Progression_13*100);
                ostringstream convert_Progression_13;
                convert_Progression_13 << Int_Efficiency_Progression_13 ;
                TString Result_Progression_13 = TString(convert_Progression_13.str());
                Int_t IsoCut_Progression_13 = histosIsolation[Result_Progression_13]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_13==100) IsoCut_Progression_13 = 1000;
                LUT_Progression_13->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_13);

                //Progression_14
                Double_t Efficiency_Progression_14 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.5, 35.);
                if(Efficiency_Progression_14>=0.9999) Efficiency_Progression_14 = 1.0001;
                Int_t Int_Efficiency_Progression_14 = int(Efficiency_Progression_14*100);
                ostringstream convert_Progression_14;
                convert_Progression_14 << Int_Efficiency_Progression_14 ;
                TString Result_Progression_14 = TString(convert_Progression_14.str());
                Int_t IsoCut_Progression_14 = histosIsolation[Result_Progression_14]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_14==100) IsoCut_Progression_14 = 1000;
                LUT_Progression_14->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_14);

                //Progression_15
                Double_t Efficiency_Progression_15 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.7, 50.);
                if(Efficiency_Progression_15>=0.9999) Efficiency_Progression_15 = 1.0001;
                Int_t Int_Efficiency_Progression_15 = int(Efficiency_Progression_15*100);
                ostringstream convert_Progression_15;
                convert_Progression_15 << Int_Efficiency_Progression_15 ;
                TString Result_Progression_15 = TString(convert_Progression_15.str());
                Int_t IsoCut_Progression_15 = histosIsolation[Result_Progression_15]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_15==100) IsoCut_Progression_15 = 1000;
                LUT_Progression_15->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_15);

                //        cout<<"after progression 15"<<endl;

                //Progression_16
                Double_t Efficiency_Progression_16 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 23., 0., 30.);
                // Double_t Efficiency_Progression_16 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0., 30.);
                //cout<<"Efficiency_Progression_16 = "<<Efficiency_Progression_16<<endl;
                if(Efficiency_Progression_16>=0.9999) Efficiency_Progression_16 = 1.0001;
                Int_t Int_Efficiency_Progression_16 = int(Efficiency_Progression_16*100);
                ostringstream convert_Progression_16;
                convert_Progression_16 << Int_Efficiency_Progression_16 ;
                TString Result_Progression_16 = TString(convert_Progression_16.str());
                Int_t IsoCut_Progression_16 = histosIsolation[Result_Progression_16]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_16==100) IsoCut_Progression_16 = 1000;
                LUT_Progression_16->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_16);

                //Progression_17
                Double_t Efficiency_Progression_17 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 15., 0., 30.);
                if(Efficiency_Progression_17>=0.9999) Efficiency_Progression_17 = 1.0001;
                Int_t Int_Efficiency_Progression_17 = int(Efficiency_Progression_17*100);
                ostringstream convert_Progression_17;
                convert_Progression_17 << Int_Efficiency_Progression_17 ;
                TString Result_Progression_17 = TString(convert_Progression_17.str());
                Int_t IsoCut_Progression_17 = histosIsolation[Result_Progression_17]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_17==100) IsoCut_Progression_17 = 1000;
                LUT_Progression_17->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_17);

                //Progression_18
                Double_t Efficiency_Progression_18 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0., 35.);
                if(Efficiency_Progression_18>=0.9999) Efficiency_Progression_18 = 1.0001;
                Int_t Int_Efficiency_Progression_18 = int(Efficiency_Progression_18*100);
                ostringstream convert_Progression_18;
                convert_Progression_18 << Int_Efficiency_Progression_18 ;
                TString Result_Progression_18 = TString(convert_Progression_18.str());
                Int_t IsoCut_Progression_18 = histosIsolation[Result_Progression_18]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_18==100) IsoCut_Progression_18 = 1000;
                LUT_Progression_18->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_18);
                
                //Progression_19
                Double_t Efficiency_Progression_19 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 15., 0., 35.);
                //cout<<"Efficiency_Progression_19 = "<<Efficiency_Progression_19<<endl;
                if(Efficiency_Progression_19>=0.9999) Efficiency_Progression_19 = 1.0001;
                Int_t Int_Efficiency_Progression_19 = int(Efficiency_Progression_19*100);
                ostringstream convert_Progression_19;
                convert_Progression_19 << Int_Efficiency_Progression_19 ;
                TString Result_Progression_19 = TString(convert_Progression_19.str());
                Int_t IsoCut_Progression_19 = histosIsolation[Result_Progression_19]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_19==100) IsoCut_Progression_19 = 1000;
                LUT_Progression_19->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_19);

                //Progression_20
                Double_t Efficiency_Progression_20 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10., 0., 35.);
                if(Efficiency_Progression_20>=0.9999) Efficiency_Progression_20 = 1.0001;
                Int_t Int_Efficiency_Progression_20 = int(Efficiency_Progression_20*100);
                ostringstream convert_Progression_20;
                convert_Progression_20 << Int_Efficiency_Progression_20 ;
                TString Result_Progression_20 = TString(convert_Progression_20.str());
                Int_t IsoCut_Progression_20 = histosIsolation[Result_Progression_20]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_20==100) IsoCut_Progression_20 = 1000;
                LUT_Progression_20->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_20);

                //        cout<<"after progression 20"<<endl;

                //Progression_21
                //cout<<"i = "<<i<<endl;
                //        cout<<"j = "<<j<<endl;
                //        cout<<"k = "<<k<<endl;
                Double_t Efficiency_Progression_21 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.80, 50.);
                if(Efficiency_Progression_21>=0.9999) Efficiency_Progression_21 = 1.0001;
                Int_t Int_Efficiency_Progression_21 = int(Efficiency_Progression_21*100);
                //        cout<<"Int_Efficiency_Progression_21 = "<<Int_Efficiency_Progression_21<<endl;
                ostringstream convert_Progression_21;
                convert_Progression_21 << Int_Efficiency_Progression_21 ;
                TString Result_Progression_21 = TString(convert_Progression_21.str());
                Int_t IsoCut_Progression_21 = histosIsolation[Result_Progression_21]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_21==100) IsoCut_Progression_21 = 1000;
                //        cout<<"IsoCut_Progression_21 = "<<IsoCut_Progression_21<<endl;

                if(hardcodedIetBins2[j]/2.>30.&&hardcodedIetBins2[j]/2.<55. && k==10 && i==0)
                {
                    cout<<"ieta bin = "<<i<<endl;
                    cout<<"pT bin = "<<j<<endl;
                    cout<<"nTT bin = "<<k<<endl;

                    cout<<"looking for..."<<endl;
                    cout<<"ieta bin = "<<i<<endl;
                    cout<<"pT bin = "<<j<<endl;
                    cout<<"nTT bin = "<<k<<endl;

                    cout<<"pT = "<<hardcodedIetBins2[j]/2.<<endl;
                    // cout<<"pT = "<<hardcodedIetBins[FindBinCorrespondenceIEt((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2.)]/2.<<endl;
                    cout<<"Target efficiency = "<<Efficiency_Progression_5<<endl;
                    cout<<"Isolation cut = "<<IsoCut_Progression_5<<endl;
                    cout<<"--"<<endl;
                }
                
                if(IsoCut_Progression_21==0)
                {
                    cout<<"j compressed = "<<FindBinCorrespondenceIEt(hardcodedIetBins2[j])<<endl;
                    cout<<"k compressed = "<<FindBinCorrespondencenTT(hardcodednTTBins2[k])<<endl;
                }
                LUT_Progression_21->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_21);

                Double_t Efficiency_Progression_22 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.70, 70.);
                cout<<"Bin j = "<<j<<", TargetEfficiency = "<<Efficiency_Progression_22<<endl;
                // Double_t Efficiency_Progression_22 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.60, 80.);
                if(Efficiency_Progression_22>=0.9999) Efficiency_Progression_22 = 1.0001;
                Int_t Int_Efficiency_Progression_22 = int(Efficiency_Progression_22*100);
                ostringstream convert_Progression_22;
                convert_Progression_22 << Int_Efficiency_Progression_22 ;
                TString Result_Progression_22 = TString(convert_Progression_22.str());
                Int_t IsoCut_Progression_22 = histosIsolation[Result_Progression_22]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_22==100) IsoCut_Progression_22 = 1000;

                LUT_Progression_22->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_22);



                Double_t Efficiency_Progression_23 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.60, 70.);
                if(Efficiency_Progression_23>=0.9999) Efficiency_Progression_23 = 1.0001;
                Int_t Int_Efficiency_Progression_23 = int(Efficiency_Progression_23*100);
                ostringstream convert_Progression_23;
                convert_Progression_23 << Int_Efficiency_Progression_23 ;
                TString Result_Progression_23 = TString(convert_Progression_23.str());
                Int_t IsoCut_Progression_23 = histosIsolation[Result_Progression_23]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_23==100) IsoCut_Progression_23 = 1000;
                LUT_Progression_23->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_23);

                Double_t Efficiency_Progression_24 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.50, 70.);
                if(Efficiency_Progression_24>=0.9999) Efficiency_Progression_24 = 1.0001;
                Int_t Int_Efficiency_Progression_24 = int(Efficiency_Progression_24*100);
                ostringstream convert_Progression_24;
                convert_Progression_24 << Int_Efficiency_Progression_24 ;
                TString Result_Progression_24 = TString(convert_Progression_24.str());
                Int_t IsoCut_Progression_24 = histosIsolation[Result_Progression_24]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_24==100) IsoCut_Progression_24 = 1000;
                LUT_Progression_24->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_24);

                Double_t Efficiency_Progression_25 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.20, 50.);
                if(Efficiency_Progression_25>=0.9999) Efficiency_Progression_25 = 1.0001;
                Int_t Int_Efficiency_Progression_25 = int(Efficiency_Progression_25*100);
                ostringstream convert_Progression_25;
                convert_Progression_25 << Int_Efficiency_Progression_25 ;
                TString Result_Progression_25 = TString(convert_Progression_25.str());
                Int_t IsoCut_Progression_25 = histosIsolation[Result_Progression_25]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_25==100) IsoCut_Progression_25 = 1000;
                LUT_Progression_25->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_25);

                Double_t Efficiency_Progression_26 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.40, 70.);
                if(Efficiency_Progression_26>=0.9999) Efficiency_Progression_26 = 1.0001;
                Int_t Int_Efficiency_Progression_26 = int(Efficiency_Progression_26*100);
                ostringstream convert_Progression_26;
                convert_Progression_26 << Int_Efficiency_Progression_26 ;
                TString Result_Progression_26 = TString(convert_Progression_26.str());
                Int_t IsoCut_Progression_26 = histosIsolation[Result_Progression_26]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_26==100) IsoCut_Progression_26 = 1000;
                LUT_Progression_26->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_26);

                Double_t Efficiency_Progression_27 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.50, 50.);
                if(Efficiency_Progression_27>=0.9999) Efficiency_Progression_27 = 1.0001;
                Int_t Int_Efficiency_Progression_27 = int(Efficiency_Progression_27*100);
                ostringstream convert_Progression_27;
                convert_Progression_27 << Int_Efficiency_Progression_27 ;
                TString Result_Progression_27 = TString(convert_Progression_27.str());
                Int_t IsoCut_Progression_27 = histosIsolation[Result_Progression_27]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_27==100) IsoCut_Progression_27 = 1000;
                LUT_Progression_27->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_27);

                Double_t Efficiency_Progression_28 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.60, 80.);
                if(Efficiency_Progression_28>=0.9999) Efficiency_Progression_28 = 1.0001;
                Int_t Int_Efficiency_Progression_28 = int(Efficiency_Progression_28*100);
                ostringstream convert_Progression_28;
                convert_Progression_28 << Int_Efficiency_Progression_28 ;
                TString Result_Progression_28 = TString(convert_Progression_28.str());
                Int_t IsoCut_Progression_28 = histosIsolation[Result_Progression_28]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_28==100) IsoCut_Progression_28 = 1000;
                LUT_Progression_28->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_28);

                Double_t Efficiency_Progression_29 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.50, 60.);
                if(Efficiency_Progression_29>=0.9999) Efficiency_Progression_29 = 1.0001;
                Int_t Int_Efficiency_Progression_29 = int(Efficiency_Progression_29*100);
                ostringstream convert_Progression_29;
                convert_Progression_29 << Int_Efficiency_Progression_29 ;
                TString Result_Progression_29 = TString(convert_Progression_29.str());
                Int_t IsoCut_Progression_29 = histosIsolation[Result_Progression_29]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_29==100) IsoCut_Progression_29 = 1000;
                LUT_Progression_29->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_29);

                Double_t Efficiency_Progression_30 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.30, 50.);
                if(Efficiency_Progression_30>=0.9999) Efficiency_Progression_30 = 1.0001;
                Int_t Int_Efficiency_Progression_30 = int(Efficiency_Progression_30*100);
                ostringstream convert_Progression_30;
                convert_Progression_30 << Int_Efficiency_Progression_30 ;
                TString Result_Progression_30 = TString(convert_Progression_30.str());
                Int_t IsoCut_Progression_30 = histosIsolation[Result_Progression_30]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_30==100) IsoCut_Progression_30 = 1000;
                LUT_Progression_30->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_30);


                Double_t Efficiency_Progression_31 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.78, 70.);
                // Double_t Efficiency_Progression_31 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.75, 70.);
                // Double_t Efficiency_Progression_31 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.40, 60.);
                if(Efficiency_Progression_31>=0.9999) Efficiency_Progression_31 = 1.0001;
                Int_t Int_Efficiency_Progression_31 = int(Efficiency_Progression_31*100);
                if(Int_Efficiency_Progression_31<99 && (i+1)==4) Int_Efficiency_Progression_31 = Int_Efficiency_Progression_31-10;
                ostringstream convert_Progression_31;
                convert_Progression_31 << Int_Efficiency_Progression_31 ;
                TString Result_Progression_31 = TString(convert_Progression_31.str());
                Int_t IsoCut_Progression_31 = histosIsolation[Result_Progression_31]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_31==100) IsoCut_Progression_31 = 1000;
                // LUT_Progression_31->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_31);
                Float_t RelaxingParameter = 1.;
                Int_t StartingFrom = 18;
                if(k+1<=StartingFrom) LUT_Progression_31->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_31);
                else  LUT_Progression_31->SetBinContent(i+1,j+1,k+1,Int_t(IsoCut_Progression_31+RelaxingParameter*(k+1-StartingFrom)));

                // Double_t Efficiency_Progression_31 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.80, 70.);
                // // Double_t Efficiency_Progression_31 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.75, 70.);
                // // Double_t Efficiency_Progression_31 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 20., 0.40, 60.);
                // if(Efficiency_Progression_31>=0.9999) Efficiency_Progression_31 = 1.0001;
                // Int_t Int_Efficiency_Progression_31 = int(Efficiency_Progression_31*100);
                // ostringstream convert_Progression_31;
                // convert_Progression_31 << Int_Efficiency_Progression_31 ;
                // TString Result_Progression_31 = TString(convert_Progression_31.str());
                // Int_t IsoCut_Progression_31 = histosIsolation[Result_Progression_31]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                // if(Int_Efficiency_Progression_31==100) IsoCut_Progression_31 = 1000;
                // LUT_Progression_31->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_31);


                // Double_t Efficiency_Progression_31_bin15 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.70, 70.);
                // // Double_t Efficiency_Progression_31_bin15 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.85, 70.);
                // if(Efficiency_Progression_31_bin15>=0.9999) Efficiency_Progression_31_bin15 = 1.0001;
                // Int_t Int_Efficiency_Progression_31_bin15 = int(Efficiency_Progression_31_bin15*100);
                // if(Int_Efficiency_Progression_31_bin15<99 && (i+1)==4) Int_Efficiency_Progression_31_bin15 = Int_Efficiency_Progression_31_bin15-12;
                // if((j+1)<=21) Int_Efficiency_Progression_31_bin15 = Int_Efficiency_Progression_31_bin15-100;
                // if(Int_Efficiency_Progression_31_bin15<0) Int_Efficiency_Progression_31_bin15 = 0;
                // ostringstream convert_Progression_31_bin15;
                // convert_Progression_31_bin15 << Int_Efficiency_Progression_31_bin15 ;
                // TString Result_Progression_31_bin15 = TString(convert_Progression_31_bin15.str());
                // Int_t IsoCut_Progression_31_bin15 = histosIsolation[Result_Progression_31_bin15]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[15])+1);
                // //barrel
                // if((i+1)!=4 && j+1!=32)
                //  {
                //    LUT_Progression_31_extrap->SetBinContent(i+1,j+1,k+1,Int_t(IsoCut_Progression_31_bin15+par1_barrel*(k-15)));
                //  }
                // else if((i+1)==4 && j+1!=32)
                //  {
                //    LUT_Progression_31_extrap->SetBinContent(i+1,j+1,k+1,Int_t(IsoCut_Progression_31_bin15+par1_endcaps*(k-15)));       
                //  }
                // else
                //  {
                //    LUT_Progression_31_extrap->SetBinContent(i+1,j+1,k+1,1000);   
                //  }

                Double_t Efficiency_Progression_31_bin15 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 30., 0.84, 75.);
                // Double_t Efficiency_Progression_31_bin15 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25., 0.85, 70.);
                if(Efficiency_Progression_31_bin15>=0.9999) Efficiency_Progression_31_bin15 = 1.0001;
                Int_t Int_Efficiency_Progression_31_bin15 = int(Efficiency_Progression_31_bin15*100);
                if(Int_Efficiency_Progression_31_bin15<99 && (i+1)==4) Int_Efficiency_Progression_31_bin15 = Int_Efficiency_Progression_31_bin15-6;
                if((j+1)<20) Int_Efficiency_Progression_31_bin15 = Int_Efficiency_Progression_31_bin15-10;
                ostringstream convert_Progression_31_bin15;
                convert_Progression_31_bin15 << Int_Efficiency_Progression_31_bin15 ;
                TString Result_Progression_31_bin15 = TString(convert_Progression_31_bin15.str());
                Int_t IsoCut_Progression_31_bin15 = histosIsolation[Result_Progression_31_bin15]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[15])+1);
                //barrel
                if((i+1)!=4 && j+1!=32)
                {
                    LUT_Progression_31_extrap->SetBinContent(i+1,j+1,k+1,Int_t(IsoCut_Progression_31_bin15+par1_barrel*(k-15)));
                }
                else if((i+1)==4 && j+1!=32)
                {
                    LUT_Progression_31_extrap->SetBinContent(i+1,j+1,k+1,Int_t(IsoCut_Progression_31_bin15+par1_endcaps*(k-15)));         
                }
                else
                {
                    LUT_Progression_31_extrap->SetBinContent(i+1,j+1,k+1,1000); 
                }

                // if((i+1)==4 && j+1!=32 && LUT_Progression_31_extrap->GetBinContent(i+1,j+1,k+1)>1) LUT_Progression_31_extrap->SetBinContent(i+1,j+1,k+1,LUT_Progression_31_extrap->GetBinContent(i+1,j+1,k+1)-1);        


                //***************************************************************************************************************************************************************************************************************************************************************************
                //***************************************************************************************************************************************************************************************************************************************************************************
                // START OF FULL BLOWN GRID SEARCH

                //Progression_A0
                Double_t Efficiency_Progression_A0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 25);
                if(Efficiency_Progression_A0>=0.9999) Efficiency_Progression_A0 = 1.0001;
                Int_t Int_Efficiency_Progression_A0 = int(Efficiency_Progression_A0*100);
                ostringstream convert_Progression_A0;
                convert_Progression_A0 << Int_Efficiency_Progression_A0 ;
                TString Result_Progression_A0 = TString(convert_Progression_A0.str());
                Int_t IsoCut_Progression_A0 = histosIsolation[Result_Progression_A0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_A0==100) IsoCut_Progression_A0 = 1000;
                LUT_Progression_A0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_A0);

                //Progression_B0
                Double_t Efficiency_Progression_B0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 28);
                if(Efficiency_Progression_B0>=0.9999) Efficiency_Progression_B0 = 1.0001;
                Int_t Int_Efficiency_Progression_B0 = int(Efficiency_Progression_B0*100);
                ostringstream convert_Progression_B0;
                convert_Progression_B0 << Int_Efficiency_Progression_B0 ;
                TString Result_Progression_B0 = TString(convert_Progression_B0.str());
                Int_t IsoCut_Progression_B0 = histosIsolation[Result_Progression_B0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_B0==100) IsoCut_Progression_B0 = 1000;
                LUT_Progression_B0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_B0);

                //Progression_C0
                Double_t Efficiency_Progression_C0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 31);
                if(Efficiency_Progression_C0>=0.9999) Efficiency_Progression_C0 = 1.0001;
                Int_t Int_Efficiency_Progression_C0 = int(Efficiency_Progression_C0*100);
                ostringstream convert_Progression_C0;
                convert_Progression_C0 << Int_Efficiency_Progression_C0 ;
                TString Result_Progression_C0 = TString(convert_Progression_C0.str());
                Int_t IsoCut_Progression_C0 = histosIsolation[Result_Progression_C0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_C0==100) IsoCut_Progression_C0 = 1000;
                LUT_Progression_C0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_C0);

                //Progression_D0
                Double_t Efficiency_Progression_D0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 34);
                if(Efficiency_Progression_D0>=0.9999) Efficiency_Progression_D0 = 1.0001;
                Int_t Int_Efficiency_Progression_D0 = int(Efficiency_Progression_D0*100);
                ostringstream convert_Progression_D0;
                convert_Progression_D0 << Int_Efficiency_Progression_D0 ;
                TString Result_Progression_D0 = TString(convert_Progression_D0.str());
                Int_t IsoCut_Progression_D0 = histosIsolation[Result_Progression_D0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_D0==100) IsoCut_Progression_D0 = 1000;
                LUT_Progression_D0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_D0);

                //Progression_E0
                Double_t Efficiency_Progression_E0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 37);
                if(Efficiency_Progression_E0>=0.9999) Efficiency_Progression_E0 = 1.0001;
                Int_t Int_Efficiency_Progression_E0 = int(Efficiency_Progression_E0*100);
                ostringstream convert_Progression_E0;
                convert_Progression_E0 << Int_Efficiency_Progression_E0 ;
                TString Result_Progression_E0 = TString(convert_Progression_E0.str());
                Int_t IsoCut_Progression_E0 = histosIsolation[Result_Progression_E0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_E0==100) IsoCut_Progression_E0 = 1000;
                LUT_Progression_E0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_E0);

                //Progression_F0
                Double_t Efficiency_Progression_F0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 40);
                if(Efficiency_Progression_F0>=0.9999) Efficiency_Progression_F0 = 1.0001;
                Int_t Int_Efficiency_Progression_F0 = int(Efficiency_Progression_F0*100);
                ostringstream convert_Progression_F0;
                convert_Progression_F0 << Int_Efficiency_Progression_F0 ;
                TString Result_Progression_F0 = TString(convert_Progression_F0.str());
                Int_t IsoCut_Progression_F0 = histosIsolation[Result_Progression_F0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_F0==100) IsoCut_Progression_F0 = 1000;
                LUT_Progression_F0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_F0);

                //Progression_G0
                Double_t Efficiency_Progression_G0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 43);
                if(Efficiency_Progression_G0>=0.9999) Efficiency_Progression_G0 = 1.0001;
                Int_t Int_Efficiency_Progression_G0 = int(Efficiency_Progression_G0*100);
                ostringstream convert_Progression_G0;
                convert_Progression_G0 << Int_Efficiency_Progression_G0 ;
                TString Result_Progression_G0 = TString(convert_Progression_G0.str());
                Int_t IsoCut_Progression_G0 = histosIsolation[Result_Progression_G0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_G0==100) IsoCut_Progression_G0 = 1000;
                LUT_Progression_G0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_G0);

                //Progression_H0
                Double_t Efficiency_Progression_H0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 46);
                if(Efficiency_Progression_H0>=0.9999) Efficiency_Progression_H0 = 1.0001;
                Int_t Int_Efficiency_Progression_H0 = int(Efficiency_Progression_H0*100);
                ostringstream convert_Progression_H0;
                convert_Progression_H0 << Int_Efficiency_Progression_H0 ;
                TString Result_Progression_H0 = TString(convert_Progression_H0.str());
                Int_t IsoCut_Progression_H0 = histosIsolation[Result_Progression_H0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_H0==100) IsoCut_Progression_H0 = 1000;
                LUT_Progression_H0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_H0);

                //Progression_I0
                Double_t Efficiency_Progression_I0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 49);
                if(Efficiency_Progression_I0>=0.9999) Efficiency_Progression_I0 = 1.0001;
                Int_t Int_Efficiency_Progression_I0 = int(Efficiency_Progression_I0*100);
                ostringstream convert_Progression_I0;
                convert_Progression_I0 << Int_Efficiency_Progression_I0 ;
                TString Result_Progression_I0 = TString(convert_Progression_I0.str());
                Int_t IsoCut_Progression_I0 = histosIsolation[Result_Progression_I0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_I0==100) IsoCut_Progression_I0 = 1000;
                LUT_Progression_I0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_I0);

                //Progression_J0
                Double_t Efficiency_Progression_J0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 51);
                if(Efficiency_Progression_J0>=0.9999) Efficiency_Progression_J0 = 1.0001;
                Int_t Int_Efficiency_Progression_J0 = int(Efficiency_Progression_J0*100);
                ostringstream convert_Progression_J0;
                convert_Progression_J0 << Int_Efficiency_Progression_J0 ;
                TString Result_Progression_J0 = TString(convert_Progression_J0.str());
                Int_t IsoCut_Progression_J0 = histosIsolation[Result_Progression_J0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_J0==100) IsoCut_Progression_J0 = 1000;
                LUT_Progression_J0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_J0);

                //Progression_K0
                Double_t Efficiency_Progression_K0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 54);
                if(Efficiency_Progression_K0>=0.9999) Efficiency_Progression_K0 = 1.0001;
                Int_t Int_Efficiency_Progression_K0 = int(Efficiency_Progression_K0*100);
                ostringstream convert_Progression_K0;
                convert_Progression_K0 << Int_Efficiency_Progression_K0 ;
                TString Result_Progression_K0 = TString(convert_Progression_K0.str());
                Int_t IsoCut_Progression_K0 = histosIsolation[Result_Progression_K0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_K0==100) IsoCut_Progression_K0 = 1000;
                LUT_Progression_K0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_K0);

                //Progression_L0
                Double_t Efficiency_Progression_L0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 57);
                if(Efficiency_Progression_L0>=0.9999) Efficiency_Progression_L0 = 1.0001;
                Int_t Int_Efficiency_Progression_L0 = int(Efficiency_Progression_L0*100);
                ostringstream convert_Progression_L0;
                convert_Progression_L0 << Int_Efficiency_Progression_L0 ;
                TString Result_Progression_L0 = TString(convert_Progression_L0.str());
                Int_t IsoCut_Progression_L0 = histosIsolation[Result_Progression_L0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_L0==100) IsoCut_Progression_L0 = 1000;
                LUT_Progression_L0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_L0);

                //Progression_M0
                Double_t Efficiency_Progression_M0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 60);
                if(Efficiency_Progression_M0>=0.9999) Efficiency_Progression_M0 = 1.0001;
                Int_t Int_Efficiency_Progression_M0 = int(Efficiency_Progression_M0*100);
                ostringstream convert_Progression_M0;
                convert_Progression_M0 << Int_Efficiency_Progression_M0 ;
                TString Result_Progression_M0 = TString(convert_Progression_M0.str());
                Int_t IsoCut_Progression_M0 = histosIsolation[Result_Progression_M0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_M0==100) IsoCut_Progression_M0 = 1000;
                LUT_Progression_M0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_M0);

                //Progression_N0
                Double_t Efficiency_Progression_N0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 63);
                if(Efficiency_Progression_N0>=0.9999) Efficiency_Progression_N0 = 1.0001;
                Int_t Int_Efficiency_Progression_N0 = int(Efficiency_Progression_N0*100);
                ostringstream convert_Progression_N0;
                convert_Progression_N0 << Int_Efficiency_Progression_N0 ;
                TString Result_Progression_N0 = TString(convert_Progression_N0.str());
                Int_t IsoCut_Progression_N0 = histosIsolation[Result_Progression_N0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_N0==100) IsoCut_Progression_N0 = 1000;
                LUT_Progression_N0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_N0);

                //Progression_O0
                Double_t Efficiency_Progression_O0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 66);
                if(Efficiency_Progression_O0>=0.9999) Efficiency_Progression_O0 = 1.0001;
                Int_t Int_Efficiency_Progression_O0 = int(Efficiency_Progression_O0*100);
                ostringstream convert_Progression_O0;
                convert_Progression_O0 << Int_Efficiency_Progression_O0 ;
                TString Result_Progression_O0 = TString(convert_Progression_O0.str());
                Int_t IsoCut_Progression_O0 = histosIsolation[Result_Progression_O0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_O0==100) IsoCut_Progression_O0 = 1000;
                LUT_Progression_O0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_O0);

                //Progression_P0
                Double_t Efficiency_Progression_P0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 69);
                if(Efficiency_Progression_P0>=0.9999) Efficiency_Progression_P0 = 1.0001;
                Int_t Int_Efficiency_Progression_P0 = int(Efficiency_Progression_P0*100);
                ostringstream convert_Progression_P0;
                convert_Progression_P0 << Int_Efficiency_Progression_P0 ;
                TString Result_Progression_P0 = TString(convert_Progression_P0.str());
                Int_t IsoCut_Progression_P0 = histosIsolation[Result_Progression_P0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_P0==100) IsoCut_Progression_P0 = 1000;
                LUT_Progression_P0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_P0);

                //Progression_Q0
                Double_t Efficiency_Progression_Q0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 71);
                if(Efficiency_Progression_Q0>=0.9999) Efficiency_Progression_Q0 = 1.0001;
                Int_t Int_Efficiency_Progression_Q0 = int(Efficiency_Progression_Q0*100);
                ostringstream convert_Progression_Q0;
                convert_Progression_Q0 << Int_Efficiency_Progression_Q0 ;
                TString Result_Progression_Q0 = TString(convert_Progression_Q0.str());
                Int_t IsoCut_Progression_Q0 = histosIsolation[Result_Progression_Q0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Q0==100) IsoCut_Progression_Q0 = 1000;
                LUT_Progression_Q0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Q0);

                //Progression_R0
                Double_t Efficiency_Progression_R0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 10, effMin, 74);
                if(Efficiency_Progression_R0>=0.9999) Efficiency_Progression_R0 = 1.0001;
                Int_t Int_Efficiency_Progression_R0 = int(Efficiency_Progression_R0*100);
                ostringstream convert_Progression_R0;
                convert_Progression_R0 << Int_Efficiency_Progression_R0 ;
                TString Result_Progression_R0 = TString(convert_Progression_R0.str());
                Int_t IsoCut_Progression_R0 = histosIsolation[Result_Progression_R0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_R0==100) IsoCut_Progression_R0 = 1000;
                LUT_Progression_R0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_R0);

                //Progression_S0
                Double_t Efficiency_Progression_S0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 28);
                if(Efficiency_Progression_S0>=0.9999) Efficiency_Progression_S0 = 1.0001;
                Int_t Int_Efficiency_Progression_S0 = int(Efficiency_Progression_S0*100);
                ostringstream convert_Progression_S0;
                convert_Progression_S0 << Int_Efficiency_Progression_S0 ;
                TString Result_Progression_S0 = TString(convert_Progression_S0.str());
                Int_t IsoCut_Progression_S0 = histosIsolation[Result_Progression_S0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_S0==100) IsoCut_Progression_S0 = 1000;
                LUT_Progression_S0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_S0);

                //Progression_T0
                Double_t Efficiency_Progression_T0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 31);
                if(Efficiency_Progression_T0>=0.9999) Efficiency_Progression_T0 = 1.0001;
                Int_t Int_Efficiency_Progression_T0 = int(Efficiency_Progression_T0*100);
                ostringstream convert_Progression_T0;
                convert_Progression_T0 << Int_Efficiency_Progression_T0 ;
                TString Result_Progression_T0 = TString(convert_Progression_T0.str());
                Int_t IsoCut_Progression_T0 = histosIsolation[Result_Progression_T0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_T0==100) IsoCut_Progression_T0 = 1000;
                LUT_Progression_T0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_T0);

                //Progression_U0
                Double_t Efficiency_Progression_U0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 34);
                if(Efficiency_Progression_U0>=0.9999) Efficiency_Progression_U0 = 1.0001;
                Int_t Int_Efficiency_Progression_U0 = int(Efficiency_Progression_U0*100);
                ostringstream convert_Progression_U0;
                convert_Progression_U0 << Int_Efficiency_Progression_U0 ;
                TString Result_Progression_U0 = TString(convert_Progression_U0.str());
                Int_t IsoCut_Progression_U0 = histosIsolation[Result_Progression_U0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_U0==100) IsoCut_Progression_U0 = 1000;
                LUT_Progression_U0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_U0);

                //Progression_V0
                Double_t Efficiency_Progression_V0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 37);
                if(Efficiency_Progression_V0>=0.9999) Efficiency_Progression_V0 = 1.0001;
                Int_t Int_Efficiency_Progression_V0 = int(Efficiency_Progression_V0*100);
                ostringstream convert_Progression_V0;
                convert_Progression_V0 << Int_Efficiency_Progression_V0 ;
                TString Result_Progression_V0 = TString(convert_Progression_V0.str());
                Int_t IsoCut_Progression_V0 = histosIsolation[Result_Progression_V0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_V0==100) IsoCut_Progression_V0 = 1000;
                LUT_Progression_V0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_V0);

                //Progression_W0
                Double_t Efficiency_Progression_W0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 40);
                if(Efficiency_Progression_W0>=0.9999) Efficiency_Progression_W0 = 1.0001;
                Int_t Int_Efficiency_Progression_W0 = int(Efficiency_Progression_W0*100);
                ostringstream convert_Progression_W0;
                convert_Progression_W0 << Int_Efficiency_Progression_W0 ;
                TString Result_Progression_W0 = TString(convert_Progression_W0.str());
                Int_t IsoCut_Progression_W0 = histosIsolation[Result_Progression_W0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_W0==100) IsoCut_Progression_W0 = 1000;
                LUT_Progression_W0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_W0);

                //Progression_X0
                Double_t Efficiency_Progression_X0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 43);
                if(Efficiency_Progression_X0>=0.9999) Efficiency_Progression_X0 = 1.0001;
                Int_t Int_Efficiency_Progression_X0 = int(Efficiency_Progression_X0*100);
                ostringstream convert_Progression_X0;
                convert_Progression_X0 << Int_Efficiency_Progression_X0 ;
                TString Result_Progression_X0 = TString(convert_Progression_X0.str());
                Int_t IsoCut_Progression_X0 = histosIsolation[Result_Progression_X0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_X0==100) IsoCut_Progression_X0 = 1000;
                LUT_Progression_X0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_X0);

                //Progression_Y0
                Double_t Efficiency_Progression_Y0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 46);
                if(Efficiency_Progression_Y0>=0.9999) Efficiency_Progression_Y0 = 1.0001;
                Int_t Int_Efficiency_Progression_Y0 = int(Efficiency_Progression_Y0*100);
                ostringstream convert_Progression_Y0;
                convert_Progression_Y0 << Int_Efficiency_Progression_Y0 ;
                TString Result_Progression_Y0 = TString(convert_Progression_Y0.str());
                Int_t IsoCut_Progression_Y0 = histosIsolation[Result_Progression_Y0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Y0==100) IsoCut_Progression_Y0 = 1000;
                LUT_Progression_Y0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Y0);

                //Progression_Z0
                Double_t Efficiency_Progression_Z0 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 49);
                if(Efficiency_Progression_Z0>=0.9999) Efficiency_Progression_Z0 = 1.0001;
                Int_t Int_Efficiency_Progression_Z0 = int(Efficiency_Progression_Z0*100);
                ostringstream convert_Progression_Z0;
                convert_Progression_Z0 << Int_Efficiency_Progression_Z0 ;
                TString Result_Progression_Z0 = TString(convert_Progression_Z0.str());
                Int_t IsoCut_Progression_Z0 = histosIsolation[Result_Progression_Z0]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Z0==100) IsoCut_Progression_Z0 = 1000;
                LUT_Progression_Z0->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Z0);

                //Progression_A1
                Double_t Efficiency_Progression_A1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 52);
                if(Efficiency_Progression_A1>=0.9999) Efficiency_Progression_A1 = 1.0001;
                Int_t Int_Efficiency_Progression_A1 = int(Efficiency_Progression_A1*100);
                ostringstream convert_Progression_A1;
                convert_Progression_A1 << Int_Efficiency_Progression_A1 ;
                TString Result_Progression_A1 = TString(convert_Progression_A1.str());
                Int_t IsoCut_Progression_A1 = histosIsolation[Result_Progression_A1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_A1==100) IsoCut_Progression_A1 = 1000;
                LUT_Progression_A1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_A1);

                //Progression_B1
                Double_t Efficiency_Progression_B1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 54);
                if(Efficiency_Progression_B1>=0.9999) Efficiency_Progression_B1 = 1.0001;
                Int_t Int_Efficiency_Progression_B1 = int(Efficiency_Progression_B1*100);
                ostringstream convert_Progression_B1;
                convert_Progression_B1 << Int_Efficiency_Progression_B1 ;
                TString Result_Progression_B1 = TString(convert_Progression_B1.str());
                Int_t IsoCut_Progression_B1 = histosIsolation[Result_Progression_B1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_B1==100) IsoCut_Progression_B1 = 1000;
                LUT_Progression_B1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_B1);

                //Progression_C1
                Double_t Efficiency_Progression_C1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 57);
                if(Efficiency_Progression_C1>=0.9999) Efficiency_Progression_C1 = 1.0001;
                Int_t Int_Efficiency_Progression_C1 = int(Efficiency_Progression_C1*100);
                ostringstream convert_Progression_C1;
                convert_Progression_C1 << Int_Efficiency_Progression_C1 ;
                TString Result_Progression_C1 = TString(convert_Progression_C1.str());
                Int_t IsoCut_Progression_C1 = histosIsolation[Result_Progression_C1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_C1==100) IsoCut_Progression_C1 = 1000;
                LUT_Progression_C1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_C1);

                //Progression_D1
                Double_t Efficiency_Progression_D1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 60);
                if(Efficiency_Progression_D1>=0.9999) Efficiency_Progression_D1 = 1.0001;
                Int_t Int_Efficiency_Progression_D1 = int(Efficiency_Progression_D1*100);
                ostringstream convert_Progression_D1;
                convert_Progression_D1 << Int_Efficiency_Progression_D1 ;
                TString Result_Progression_D1 = TString(convert_Progression_D1.str());
                Int_t IsoCut_Progression_D1 = histosIsolation[Result_Progression_D1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_D1==100) IsoCut_Progression_D1 = 1000;
                LUT_Progression_D1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_D1);

                //Progression_E1
                Double_t Efficiency_Progression_E1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 63);
                if(Efficiency_Progression_E1>=0.9999) Efficiency_Progression_E1 = 1.0001;
                Int_t Int_Efficiency_Progression_E1 = int(Efficiency_Progression_E1*100);
                ostringstream convert_Progression_E1;
                convert_Progression_E1 << Int_Efficiency_Progression_E1 ;
                TString Result_Progression_E1 = TString(convert_Progression_E1.str());
                Int_t IsoCut_Progression_E1 = histosIsolation[Result_Progression_E1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_E1==100) IsoCut_Progression_E1 = 1000;
                LUT_Progression_E1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_E1);

                //Progression_F1
                Double_t Efficiency_Progression_F1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 66);
                if(Efficiency_Progression_F1>=0.9999) Efficiency_Progression_F1 = 1.0001;
                Int_t Int_Efficiency_Progression_F1 = int(Efficiency_Progression_F1*100);
                ostringstream convert_Progression_F1;
                convert_Progression_F1 << Int_Efficiency_Progression_F1 ;
                TString Result_Progression_F1 = TString(convert_Progression_F1.str());
                Int_t IsoCut_Progression_F1 = histosIsolation[Result_Progression_F1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_F1==100) IsoCut_Progression_F1 = 1000;
                LUT_Progression_F1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_F1);

                //Progression_G1
                Double_t Efficiency_Progression_G1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 69);
                if(Efficiency_Progression_G1>=0.9999) Efficiency_Progression_G1 = 1.0001;
                Int_t Int_Efficiency_Progression_G1 = int(Efficiency_Progression_G1*100);
                ostringstream convert_Progression_G1;
                convert_Progression_G1 << Int_Efficiency_Progression_G1 ;
                TString Result_Progression_G1 = TString(convert_Progression_G1.str());
                Int_t IsoCut_Progression_G1 = histosIsolation[Result_Progression_G1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_G1==100) IsoCut_Progression_G1 = 1000;
                LUT_Progression_G1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_G1);

                //Progression_H1
                Double_t Efficiency_Progression_H1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 72);
                if(Efficiency_Progression_H1>=0.9999) Efficiency_Progression_H1 = 1.0001;
                Int_t Int_Efficiency_Progression_H1 = int(Efficiency_Progression_H1*100);
                ostringstream convert_Progression_H1;
                convert_Progression_H1 << Int_Efficiency_Progression_H1 ;
                TString Result_Progression_H1 = TString(convert_Progression_H1.str());
                Int_t IsoCut_Progression_H1 = histosIsolation[Result_Progression_H1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_H1==100) IsoCut_Progression_H1 = 1000;
                LUT_Progression_H1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_H1);

                //Progression_I1
                Double_t Efficiency_Progression_I1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 74);
                if(Efficiency_Progression_I1>=0.9999) Efficiency_Progression_I1 = 1.0001;
                Int_t Int_Efficiency_Progression_I1 = int(Efficiency_Progression_I1*100);
                ostringstream convert_Progression_I1;
                convert_Progression_I1 << Int_Efficiency_Progression_I1 ;
                TString Result_Progression_I1 = TString(convert_Progression_I1.str());
                Int_t IsoCut_Progression_I1 = histosIsolation[Result_Progression_I1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_I1==100) IsoCut_Progression_I1 = 1000;
                LUT_Progression_I1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_I1);

                //Progression_J1
                Double_t Efficiency_Progression_J1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 13, effMin, 77);
                if(Efficiency_Progression_J1>=0.9999) Efficiency_Progression_J1 = 1.0001;
                Int_t Int_Efficiency_Progression_J1 = int(Efficiency_Progression_J1*100);
                ostringstream convert_Progression_J1;
                convert_Progression_J1 << Int_Efficiency_Progression_J1 ;
                TString Result_Progression_J1 = TString(convert_Progression_J1.str());
                Int_t IsoCut_Progression_J1 = histosIsolation[Result_Progression_J1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_J1==100) IsoCut_Progression_J1 = 1000;
                LUT_Progression_J1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_J1);

                //Progression_K1
                Double_t Efficiency_Progression_K1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 31);
                if(Efficiency_Progression_K1>=0.9999) Efficiency_Progression_K1 = 1.0001;
                Int_t Int_Efficiency_Progression_K1 = int(Efficiency_Progression_K1*100);
                ostringstream convert_Progression_K1;
                convert_Progression_K1 << Int_Efficiency_Progression_K1 ;
                TString Result_Progression_K1 = TString(convert_Progression_K1.str());
                Int_t IsoCut_Progression_K1 = histosIsolation[Result_Progression_K1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_K1==100) IsoCut_Progression_K1 = 1000;
                LUT_Progression_K1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_K1);

                //Progression_L1
                Double_t Efficiency_Progression_L1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 34);
                if(Efficiency_Progression_L1>=0.9999) Efficiency_Progression_L1 = 1.0001;
                Int_t Int_Efficiency_Progression_L1 = int(Efficiency_Progression_L1*100);
                ostringstream convert_Progression_L1;
                convert_Progression_L1 << Int_Efficiency_Progression_L1 ;
                TString Result_Progression_L1 = TString(convert_Progression_L1.str());
                Int_t IsoCut_Progression_L1 = histosIsolation[Result_Progression_L1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_L1==100) IsoCut_Progression_L1 = 1000;
                LUT_Progression_L1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_L1);

                //Progression_M1
                Double_t Efficiency_Progression_M1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 37);
                if(Efficiency_Progression_M1>=0.9999) Efficiency_Progression_M1 = 1.0001;
                Int_t Int_Efficiency_Progression_M1 = int(Efficiency_Progression_M1*100);
                ostringstream convert_Progression_M1;
                convert_Progression_M1 << Int_Efficiency_Progression_M1 ;
                TString Result_Progression_M1 = TString(convert_Progression_M1.str());
                Int_t IsoCut_Progression_M1 = histosIsolation[Result_Progression_M1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_M1==100) IsoCut_Progression_M1 = 1000;
                LUT_Progression_M1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_M1);

                //Progression_N1
                Double_t Efficiency_Progression_N1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 40);
                if(Efficiency_Progression_N1>=0.9999) Efficiency_Progression_N1 = 1.0001;
                Int_t Int_Efficiency_Progression_N1 = int(Efficiency_Progression_N1*100);
                ostringstream convert_Progression_N1;
                convert_Progression_N1 << Int_Efficiency_Progression_N1 ;
                TString Result_Progression_N1 = TString(convert_Progression_N1.str());
                Int_t IsoCut_Progression_N1 = histosIsolation[Result_Progression_N1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_N1==100) IsoCut_Progression_N1 = 1000;
                LUT_Progression_N1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_N1);

                //Progression_O1
                Double_t Efficiency_Progression_O1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 43);
                if(Efficiency_Progression_O1>=0.9999) Efficiency_Progression_O1 = 1.0001;
                Int_t Int_Efficiency_Progression_O1 = int(Efficiency_Progression_O1*100);
                ostringstream convert_Progression_O1;
                convert_Progression_O1 << Int_Efficiency_Progression_O1 ;
                TString Result_Progression_O1 = TString(convert_Progression_O1.str());
                Int_t IsoCut_Progression_O1 = histosIsolation[Result_Progression_O1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_O1==100) IsoCut_Progression_O1 = 1000;
                LUT_Progression_O1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_O1);

                //Progression_P1
                Double_t Efficiency_Progression_P1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 46);
                if(Efficiency_Progression_P1>=0.9999) Efficiency_Progression_P1 = 1.0001;
                Int_t Int_Efficiency_Progression_P1 = int(Efficiency_Progression_P1*100);
                ostringstream convert_Progression_P1;
                convert_Progression_P1 << Int_Efficiency_Progression_P1 ;
                TString Result_Progression_P1 = TString(convert_Progression_P1.str());
                Int_t IsoCut_Progression_P1 = histosIsolation[Result_Progression_P1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_P1==100) IsoCut_Progression_P1 = 1000;
                LUT_Progression_P1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_P1);

                //Progression_Q1
                Double_t Efficiency_Progression_Q1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 49);
                if(Efficiency_Progression_Q1>=0.9999) Efficiency_Progression_Q1 = 1.0001;
                Int_t Int_Efficiency_Progression_Q1 = int(Efficiency_Progression_Q1*100);
                ostringstream convert_Progression_Q1;
                convert_Progression_Q1 << Int_Efficiency_Progression_Q1 ;
                TString Result_Progression_Q1 = TString(convert_Progression_Q1.str());
                Int_t IsoCut_Progression_Q1 = histosIsolation[Result_Progression_Q1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Q1==100) IsoCut_Progression_Q1 = 1000;
                LUT_Progression_Q1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Q1);

                //Progression_R1
                Double_t Efficiency_Progression_R1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 52);
                if(Efficiency_Progression_R1>=0.9999) Efficiency_Progression_R1 = 1.0001;
                Int_t Int_Efficiency_Progression_R1 = int(Efficiency_Progression_R1*100);
                ostringstream convert_Progression_R1;
                convert_Progression_R1 << Int_Efficiency_Progression_R1 ;
                TString Result_Progression_R1 = TString(convert_Progression_R1.str());
                Int_t IsoCut_Progression_R1 = histosIsolation[Result_Progression_R1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_R1==100) IsoCut_Progression_R1 = 1000;
                LUT_Progression_R1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_R1);

                //Progression_S1
                Double_t Efficiency_Progression_S1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 55);
                if(Efficiency_Progression_S1>=0.9999) Efficiency_Progression_S1 = 1.0001;
                Int_t Int_Efficiency_Progression_S1 = int(Efficiency_Progression_S1*100);
                ostringstream convert_Progression_S1;
                convert_Progression_S1 << Int_Efficiency_Progression_S1 ;
                TString Result_Progression_S1 = TString(convert_Progression_S1.str());
                Int_t IsoCut_Progression_S1 = histosIsolation[Result_Progression_S1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_S1==100) IsoCut_Progression_S1 = 1000;
                LUT_Progression_S1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_S1);

                //Progression_T1
                Double_t Efficiency_Progression_T1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 57);
                if(Efficiency_Progression_T1>=0.9999) Efficiency_Progression_T1 = 1.0001;
                Int_t Int_Efficiency_Progression_T1 = int(Efficiency_Progression_T1*100);
                ostringstream convert_Progression_T1;
                convert_Progression_T1 << Int_Efficiency_Progression_T1 ;
                TString Result_Progression_T1 = TString(convert_Progression_T1.str());
                Int_t IsoCut_Progression_T1 = histosIsolation[Result_Progression_T1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_T1==100) IsoCut_Progression_T1 = 1000;
                LUT_Progression_T1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_T1);

                //Progression_U1
                Double_t Efficiency_Progression_U1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 60);
                if(Efficiency_Progression_U1>=0.9999) Efficiency_Progression_U1 = 1.0001;
                Int_t Int_Efficiency_Progression_U1 = int(Efficiency_Progression_U1*100);
                ostringstream convert_Progression_U1;
                convert_Progression_U1 << Int_Efficiency_Progression_U1 ;
                TString Result_Progression_U1 = TString(convert_Progression_U1.str());
                Int_t IsoCut_Progression_U1 = histosIsolation[Result_Progression_U1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_U1==100) IsoCut_Progression_U1 = 1000;
                LUT_Progression_U1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_U1);

                //Progression_V1
                Double_t Efficiency_Progression_V1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 63);
                if(Efficiency_Progression_V1>=0.9999) Efficiency_Progression_V1 = 1.0001;
                Int_t Int_Efficiency_Progression_V1 = int(Efficiency_Progression_V1*100);
                ostringstream convert_Progression_V1;
                convert_Progression_V1 << Int_Efficiency_Progression_V1 ;
                TString Result_Progression_V1 = TString(convert_Progression_V1.str());
                Int_t IsoCut_Progression_V1 = histosIsolation[Result_Progression_V1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_V1==100) IsoCut_Progression_V1 = 1000;
                LUT_Progression_V1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_V1);

                //Progression_W1
                Double_t Efficiency_Progression_W1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 66);
                if(Efficiency_Progression_W1>=0.9999) Efficiency_Progression_W1 = 1.0001;
                Int_t Int_Efficiency_Progression_W1 = int(Efficiency_Progression_W1*100);
                ostringstream convert_Progression_W1;
                convert_Progression_W1 << Int_Efficiency_Progression_W1 ;
                TString Result_Progression_W1 = TString(convert_Progression_W1.str());
                Int_t IsoCut_Progression_W1 = histosIsolation[Result_Progression_W1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_W1==100) IsoCut_Progression_W1 = 1000;
                LUT_Progression_W1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_W1);

                //Progression_X1
                Double_t Efficiency_Progression_X1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 69);
                if(Efficiency_Progression_X1>=0.9999) Efficiency_Progression_X1 = 1.0001;
                Int_t Int_Efficiency_Progression_X1 = int(Efficiency_Progression_X1*100);
                ostringstream convert_Progression_X1;
                convert_Progression_X1 << Int_Efficiency_Progression_X1 ;
                TString Result_Progression_X1 = TString(convert_Progression_X1.str());
                Int_t IsoCut_Progression_X1 = histosIsolation[Result_Progression_X1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_X1==100) IsoCut_Progression_X1 = 1000;
                LUT_Progression_X1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_X1);

                //Progression_Y1
                Double_t Efficiency_Progression_Y1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 72);
                if(Efficiency_Progression_Y1>=0.9999) Efficiency_Progression_Y1 = 1.0001;
                Int_t Int_Efficiency_Progression_Y1 = int(Efficiency_Progression_Y1*100);
                ostringstream convert_Progression_Y1;
                convert_Progression_Y1 << Int_Efficiency_Progression_Y1 ;
                TString Result_Progression_Y1 = TString(convert_Progression_Y1.str());
                Int_t IsoCut_Progression_Y1 = histosIsolation[Result_Progression_Y1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Y1==100) IsoCut_Progression_Y1 = 1000;
                LUT_Progression_Y1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Y1);

                //Progression_Z1
                Double_t Efficiency_Progression_Z1 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 75);
                if(Efficiency_Progression_Z1>=0.9999) Efficiency_Progression_Z1 = 1.0001;
                Int_t Int_Efficiency_Progression_Z1 = int(Efficiency_Progression_Z1*100);
                ostringstream convert_Progression_Z1;
                convert_Progression_Z1 << Int_Efficiency_Progression_Z1 ;
                TString Result_Progression_Z1 = TString(convert_Progression_Z1.str());
                Int_t IsoCut_Progression_Z1 = histosIsolation[Result_Progression_Z1]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Z1==100) IsoCut_Progression_Z1 = 1000;
                LUT_Progression_Z1->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Z1);

                //Progression_A2
                Double_t Efficiency_Progression_A2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 77);
                if(Efficiency_Progression_A2>=0.9999) Efficiency_Progression_A2 = 1.0001;
                Int_t Int_Efficiency_Progression_A2 = int(Efficiency_Progression_A2*100);
                ostringstream convert_Progression_A2;
                convert_Progression_A2 << Int_Efficiency_Progression_A2 ;
                TString Result_Progression_A2 = TString(convert_Progression_A2.str());
                Int_t IsoCut_Progression_A2 = histosIsolation[Result_Progression_A2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_A2==100) IsoCut_Progression_A2 = 1000;
                LUT_Progression_A2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_A2);

                //Progression_B2
                Double_t Efficiency_Progression_B2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 16, effMin, 80);
                if(Efficiency_Progression_B2>=0.9999) Efficiency_Progression_B2 = 1.0001;
                Int_t Int_Efficiency_Progression_B2 = int(Efficiency_Progression_B2*100);
                ostringstream convert_Progression_B2;
                convert_Progression_B2 << Int_Efficiency_Progression_B2 ;
                TString Result_Progression_B2 = TString(convert_Progression_B2.str());
                Int_t IsoCut_Progression_B2 = histosIsolation[Result_Progression_B2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_B2==100) IsoCut_Progression_B2 = 1000;
                LUT_Progression_B2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_B2);

                //Progression_C2
                Double_t Efficiency_Progression_C2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 34);
                if(Efficiency_Progression_C2>=0.9999) Efficiency_Progression_C2 = 1.0001;
                Int_t Int_Efficiency_Progression_C2 = int(Efficiency_Progression_C2*100);
                ostringstream convert_Progression_C2;
                convert_Progression_C2 << Int_Efficiency_Progression_C2 ;
                TString Result_Progression_C2 = TString(convert_Progression_C2.str());
                Int_t IsoCut_Progression_C2 = histosIsolation[Result_Progression_C2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_C2==100) IsoCut_Progression_C2 = 1000;
                LUT_Progression_C2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_C2);

                //Progression_D2
                Double_t Efficiency_Progression_D2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 37);
                if(Efficiency_Progression_D2>=0.9999) Efficiency_Progression_D2 = 1.0001;
                Int_t Int_Efficiency_Progression_D2 = int(Efficiency_Progression_D2*100);
                ostringstream convert_Progression_D2;
                convert_Progression_D2 << Int_Efficiency_Progression_D2 ;
                TString Result_Progression_D2 = TString(convert_Progression_D2.str());
                Int_t IsoCut_Progression_D2 = histosIsolation[Result_Progression_D2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_D2==100) IsoCut_Progression_D2 = 1000;
                LUT_Progression_D2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_D2);

                //Progression_E2
                Double_t Efficiency_Progression_E2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 40);
                if(Efficiency_Progression_E2>=0.9999) Efficiency_Progression_E2 = 1.0001;
                Int_t Int_Efficiency_Progression_E2 = int(Efficiency_Progression_E2*100);
                ostringstream convert_Progression_E2;
                convert_Progression_E2 << Int_Efficiency_Progression_E2 ;
                TString Result_Progression_E2 = TString(convert_Progression_E2.str());
                Int_t IsoCut_Progression_E2 = histosIsolation[Result_Progression_E2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_E2==100) IsoCut_Progression_E2 = 1000;
                LUT_Progression_E2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_E2);

                //Progression_F2
                Double_t Efficiency_Progression_F2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 43);
                if(Efficiency_Progression_F2>=0.9999) Efficiency_Progression_F2 = 1.0001;
                Int_t Int_Efficiency_Progression_F2 = int(Efficiency_Progression_F2*100);
                ostringstream convert_Progression_F2;
                convert_Progression_F2 << Int_Efficiency_Progression_F2 ;
                TString Result_Progression_F2 = TString(convert_Progression_F2.str());
                Int_t IsoCut_Progression_F2 = histosIsolation[Result_Progression_F2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_F2==100) IsoCut_Progression_F2 = 1000;
                LUT_Progression_F2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_F2);

                //Progression_G2
                Double_t Efficiency_Progression_G2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 46);
                if(Efficiency_Progression_G2>=0.9999) Efficiency_Progression_G2 = 1.0001;
                Int_t Int_Efficiency_Progression_G2 = int(Efficiency_Progression_G2*100);
                ostringstream convert_Progression_G2;
                convert_Progression_G2 << Int_Efficiency_Progression_G2 ;
                TString Result_Progression_G2 = TString(convert_Progression_G2.str());
                Int_t IsoCut_Progression_G2 = histosIsolation[Result_Progression_G2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_G2==100) IsoCut_Progression_G2 = 1000;
                LUT_Progression_G2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_G2);

                //Progression_H2
                Double_t Efficiency_Progression_H2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 49);
                if(Efficiency_Progression_H2>=0.9999) Efficiency_Progression_H2 = 1.0001;
                Int_t Int_Efficiency_Progression_H2 = int(Efficiency_Progression_H2*100);
                ostringstream convert_Progression_H2;
                convert_Progression_H2 << Int_Efficiency_Progression_H2 ;
                TString Result_Progression_H2 = TString(convert_Progression_H2.str());
                Int_t IsoCut_Progression_H2 = histosIsolation[Result_Progression_H2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_H2==100) IsoCut_Progression_H2 = 1000;
                LUT_Progression_H2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_H2);

                //Progression_I2
                Double_t Efficiency_Progression_I2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 52);
                if(Efficiency_Progression_I2>=0.9999) Efficiency_Progression_I2 = 1.0001;
                Int_t Int_Efficiency_Progression_I2 = int(Efficiency_Progression_I2*100);
                ostringstream convert_Progression_I2;
                convert_Progression_I2 << Int_Efficiency_Progression_I2 ;
                TString Result_Progression_I2 = TString(convert_Progression_I2.str());
                Int_t IsoCut_Progression_I2 = histosIsolation[Result_Progression_I2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_I2==100) IsoCut_Progression_I2 = 1000;
                LUT_Progression_I2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_I2);

                //Progression_J2
                Double_t Efficiency_Progression_J2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 55);
                if(Efficiency_Progression_J2>=0.9999) Efficiency_Progression_J2 = 1.0001;
                Int_t Int_Efficiency_Progression_J2 = int(Efficiency_Progression_J2*100);
                ostringstream convert_Progression_J2;
                convert_Progression_J2 << Int_Efficiency_Progression_J2 ;
                TString Result_Progression_J2 = TString(convert_Progression_J2.str());
                Int_t IsoCut_Progression_J2 = histosIsolation[Result_Progression_J2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_J2==100) IsoCut_Progression_J2 = 1000;
                LUT_Progression_J2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_J2);

                //Progression_K2
                Double_t Efficiency_Progression_K2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 58);
                if(Efficiency_Progression_K2>=0.9999) Efficiency_Progression_K2 = 1.0001;
                Int_t Int_Efficiency_Progression_K2 = int(Efficiency_Progression_K2*100);
                ostringstream convert_Progression_K2;
                convert_Progression_K2 << Int_Efficiency_Progression_K2 ;
                TString Result_Progression_K2 = TString(convert_Progression_K2.str());
                Int_t IsoCut_Progression_K2 = histosIsolation[Result_Progression_K2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_K2==100) IsoCut_Progression_K2 = 1000;
                LUT_Progression_K2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_K2);

                //Progression_L2
                Double_t Efficiency_Progression_L2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 60);
                if(Efficiency_Progression_L2>=0.9999) Efficiency_Progression_L2 = 1.0001;
                Int_t Int_Efficiency_Progression_L2 = int(Efficiency_Progression_L2*100);
                ostringstream convert_Progression_L2;
                convert_Progression_L2 << Int_Efficiency_Progression_L2 ;
                TString Result_Progression_L2 = TString(convert_Progression_L2.str());
                Int_t IsoCut_Progression_L2 = histosIsolation[Result_Progression_L2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_L2==100) IsoCut_Progression_L2 = 1000;
                LUT_Progression_L2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_L2);

                //Progression_M2
                Double_t Efficiency_Progression_M2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 63);
                if(Efficiency_Progression_M2>=0.9999) Efficiency_Progression_M2 = 1.0001;
                Int_t Int_Efficiency_Progression_M2 = int(Efficiency_Progression_M2*100);
                ostringstream convert_Progression_M2;
                convert_Progression_M2 << Int_Efficiency_Progression_M2 ;
                TString Result_Progression_M2 = TString(convert_Progression_M2.str());
                Int_t IsoCut_Progression_M2 = histosIsolation[Result_Progression_M2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_M2==100) IsoCut_Progression_M2 = 1000;
                LUT_Progression_M2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_M2);

                //Progression_N2
                Double_t Efficiency_Progression_N2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 66);
                if(Efficiency_Progression_N2>=0.9999) Efficiency_Progression_N2 = 1.0001;
                Int_t Int_Efficiency_Progression_N2 = int(Efficiency_Progression_N2*100);
                ostringstream convert_Progression_N2;
                convert_Progression_N2 << Int_Efficiency_Progression_N2 ;
                TString Result_Progression_N2 = TString(convert_Progression_N2.str());
                Int_t IsoCut_Progression_N2 = histosIsolation[Result_Progression_N2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_N2==100) IsoCut_Progression_N2 = 1000;
                LUT_Progression_N2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_N2);

                //Progression_O2
                Double_t Efficiency_Progression_O2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 69);
                if(Efficiency_Progression_O2>=0.9999) Efficiency_Progression_O2 = 1.0001;
                Int_t Int_Efficiency_Progression_O2 = int(Efficiency_Progression_O2*100);
                ostringstream convert_Progression_O2;
                convert_Progression_O2 << Int_Efficiency_Progression_O2 ;
                TString Result_Progression_O2 = TString(convert_Progression_O2.str());
                Int_t IsoCut_Progression_O2 = histosIsolation[Result_Progression_O2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_O2==100) IsoCut_Progression_O2 = 1000;
                LUT_Progression_O2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_O2);

                //Progression_P2
                Double_t Efficiency_Progression_P2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 72);
                if(Efficiency_Progression_P2>=0.9999) Efficiency_Progression_P2 = 1.0001;
                Int_t Int_Efficiency_Progression_P2 = int(Efficiency_Progression_P2*100);
                ostringstream convert_Progression_P2;
                convert_Progression_P2 << Int_Efficiency_Progression_P2 ;
                TString Result_Progression_P2 = TString(convert_Progression_P2.str());
                Int_t IsoCut_Progression_P2 = histosIsolation[Result_Progression_P2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_P2==100) IsoCut_Progression_P2 = 1000;
                LUT_Progression_P2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_P2);

                //Progression_Q2
                Double_t Efficiency_Progression_Q2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 75);
                if(Efficiency_Progression_Q2>=0.9999) Efficiency_Progression_Q2 = 1.0001;
                Int_t Int_Efficiency_Progression_Q2 = int(Efficiency_Progression_Q2*100);
                ostringstream convert_Progression_Q2;
                convert_Progression_Q2 << Int_Efficiency_Progression_Q2 ;
                TString Result_Progression_Q2 = TString(convert_Progression_Q2.str());
                Int_t IsoCut_Progression_Q2 = histosIsolation[Result_Progression_Q2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Q2==100) IsoCut_Progression_Q2 = 1000;
                LUT_Progression_Q2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Q2);

                //Progression_R2
                Double_t Efficiency_Progression_R2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 78);
                if(Efficiency_Progression_R2>=0.9999) Efficiency_Progression_R2 = 1.0001;
                Int_t Int_Efficiency_Progression_R2 = int(Efficiency_Progression_R2*100);
                ostringstream convert_Progression_R2;
                convert_Progression_R2 << Int_Efficiency_Progression_R2 ;
                TString Result_Progression_R2 = TString(convert_Progression_R2.str());
                Int_t IsoCut_Progression_R2 = histosIsolation[Result_Progression_R2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_R2==100) IsoCut_Progression_R2 = 1000;
                LUT_Progression_R2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_R2);

                //Progression_S2
                Double_t Efficiency_Progression_S2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 80);
                if(Efficiency_Progression_S2>=0.9999) Efficiency_Progression_S2 = 1.0001;
                Int_t Int_Efficiency_Progression_S2 = int(Efficiency_Progression_S2*100);
                ostringstream convert_Progression_S2;
                convert_Progression_S2 << Int_Efficiency_Progression_S2 ;
                TString Result_Progression_S2 = TString(convert_Progression_S2.str());
                Int_t IsoCut_Progression_S2 = histosIsolation[Result_Progression_S2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_S2==100) IsoCut_Progression_S2 = 1000;
                LUT_Progression_S2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_S2);

                //Progression_T2
                Double_t Efficiency_Progression_T2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 19, effMin, 83);
                if(Efficiency_Progression_T2>=0.9999) Efficiency_Progression_T2 = 1.0001;
                Int_t Int_Efficiency_Progression_T2 = int(Efficiency_Progression_T2*100);
                ostringstream convert_Progression_T2;
                convert_Progression_T2 << Int_Efficiency_Progression_T2 ;
                TString Result_Progression_T2 = TString(convert_Progression_T2.str());
                Int_t IsoCut_Progression_T2 = histosIsolation[Result_Progression_T2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_T2==100) IsoCut_Progression_T2 = 1000;
                LUT_Progression_T2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_T2);

                //Progression_U2
                Double_t Efficiency_Progression_U2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 37);
                if(Efficiency_Progression_U2>=0.9999) Efficiency_Progression_U2 = 1.0001;
                Int_t Int_Efficiency_Progression_U2 = int(Efficiency_Progression_U2*100);
                ostringstream convert_Progression_U2;
                convert_Progression_U2 << Int_Efficiency_Progression_U2 ;
                TString Result_Progression_U2 = TString(convert_Progression_U2.str());
                Int_t IsoCut_Progression_U2 = histosIsolation[Result_Progression_U2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_U2==100) IsoCut_Progression_U2 = 1000;
                LUT_Progression_U2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_U2);

                //Progression_V2
                Double_t Efficiency_Progression_V2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 40);
                if(Efficiency_Progression_V2>=0.9999) Efficiency_Progression_V2 = 1.0001;
                Int_t Int_Efficiency_Progression_V2 = int(Efficiency_Progression_V2*100);
                ostringstream convert_Progression_V2;
                convert_Progression_V2 << Int_Efficiency_Progression_V2 ;
                TString Result_Progression_V2 = TString(convert_Progression_V2.str());
                Int_t IsoCut_Progression_V2 = histosIsolation[Result_Progression_V2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_V2==100) IsoCut_Progression_V2 = 1000;
                LUT_Progression_V2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_V2);

                //Progression_W2
                Double_t Efficiency_Progression_W2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 43);
                if(Efficiency_Progression_W2>=0.9999) Efficiency_Progression_W2 = 1.0001;
                Int_t Int_Efficiency_Progression_W2 = int(Efficiency_Progression_W2*100);
                ostringstream convert_Progression_W2;
                convert_Progression_W2 << Int_Efficiency_Progression_W2 ;
                TString Result_Progression_W2 = TString(convert_Progression_W2.str());
                Int_t IsoCut_Progression_W2 = histosIsolation[Result_Progression_W2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_W2==100) IsoCut_Progression_W2 = 1000;
                LUT_Progression_W2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_W2);

                //Progression_X2
                Double_t Efficiency_Progression_X2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 46);
                if(Efficiency_Progression_X2>=0.9999) Efficiency_Progression_X2 = 1.0001;
                Int_t Int_Efficiency_Progression_X2 = int(Efficiency_Progression_X2*100);
                ostringstream convert_Progression_X2;
                convert_Progression_X2 << Int_Efficiency_Progression_X2 ;
                TString Result_Progression_X2 = TString(convert_Progression_X2.str());
                Int_t IsoCut_Progression_X2 = histosIsolation[Result_Progression_X2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_X2==100) IsoCut_Progression_X2 = 1000;
                LUT_Progression_X2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_X2);

                //Progression_Y2
                Double_t Efficiency_Progression_Y2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 49);
                if(Efficiency_Progression_Y2>=0.9999) Efficiency_Progression_Y2 = 1.0001;
                Int_t Int_Efficiency_Progression_Y2 = int(Efficiency_Progression_Y2*100);
                ostringstream convert_Progression_Y2;
                convert_Progression_Y2 << Int_Efficiency_Progression_Y2 ;
                TString Result_Progression_Y2 = TString(convert_Progression_Y2.str());
                Int_t IsoCut_Progression_Y2 = histosIsolation[Result_Progression_Y2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Y2==100) IsoCut_Progression_Y2 = 1000;
                LUT_Progression_Y2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Y2);

                //Progression_Z2
                Double_t Efficiency_Progression_Z2 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 52);
                if(Efficiency_Progression_Z2>=0.9999) Efficiency_Progression_Z2 = 1.0001;
                Int_t Int_Efficiency_Progression_Z2 = int(Efficiency_Progression_Z2*100);
                ostringstream convert_Progression_Z2;
                convert_Progression_Z2 << Int_Efficiency_Progression_Z2 ;
                TString Result_Progression_Z2 = TString(convert_Progression_Z2.str());
                Int_t IsoCut_Progression_Z2 = histosIsolation[Result_Progression_Z2]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Z2==100) IsoCut_Progression_Z2 = 1000;
                LUT_Progression_Z2->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Z2);

                //Progression_A3
                Double_t Efficiency_Progression_A3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 55);
                if(Efficiency_Progression_A3>=0.9999) Efficiency_Progression_A3 = 1.0001;
                Int_t Int_Efficiency_Progression_A3 = int(Efficiency_Progression_A3*100);
                ostringstream convert_Progression_A3;
                convert_Progression_A3 << Int_Efficiency_Progression_A3 ;
                TString Result_Progression_A3 = TString(convert_Progression_A3.str());
                Int_t IsoCut_Progression_A3 = histosIsolation[Result_Progression_A3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_A3==100) IsoCut_Progression_A3 = 1000;
                LUT_Progression_A3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_A3);

                //Progression_B3
                Double_t Efficiency_Progression_B3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 58);
                if(Efficiency_Progression_B3>=0.9999) Efficiency_Progression_B3 = 1.0001;
                Int_t Int_Efficiency_Progression_B3 = int(Efficiency_Progression_B3*100);
                ostringstream convert_Progression_B3;
                convert_Progression_B3 << Int_Efficiency_Progression_B3 ;
                TString Result_Progression_B3 = TString(convert_Progression_B3.str());
                Int_t IsoCut_Progression_B3 = histosIsolation[Result_Progression_B3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_B3==100) IsoCut_Progression_B3 = 1000;
                LUT_Progression_B3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_B3);

                //Progression_C3
                Double_t Efficiency_Progression_C3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 61);
                if(Efficiency_Progression_C3>=0.9999) Efficiency_Progression_C3 = 1.0001;
                Int_t Int_Efficiency_Progression_C3 = int(Efficiency_Progression_C3*100);
                ostringstream convert_Progression_C3;
                convert_Progression_C3 << Int_Efficiency_Progression_C3 ;
                TString Result_Progression_C3 = TString(convert_Progression_C3.str());
                Int_t IsoCut_Progression_C3 = histosIsolation[Result_Progression_C3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_C3==100) IsoCut_Progression_C3 = 1000;
                LUT_Progression_C3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_C3);

                //Progression_D3
                Double_t Efficiency_Progression_D3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 63);
                if(Efficiency_Progression_D3>=0.9999) Efficiency_Progression_D3 = 1.0001;
                Int_t Int_Efficiency_Progression_D3 = int(Efficiency_Progression_D3*100);
                ostringstream convert_Progression_D3;
                convert_Progression_D3 << Int_Efficiency_Progression_D3 ;
                TString Result_Progression_D3 = TString(convert_Progression_D3.str());
                Int_t IsoCut_Progression_D3 = histosIsolation[Result_Progression_D3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_D3==100) IsoCut_Progression_D3 = 1000;
                LUT_Progression_D3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_D3);

                //Progression_E3
                Double_t Efficiency_Progression_E3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 66);
                if(Efficiency_Progression_E3>=0.9999) Efficiency_Progression_E3 = 1.0001;
                Int_t Int_Efficiency_Progression_E3 = int(Efficiency_Progression_E3*100);
                ostringstream convert_Progression_E3;
                convert_Progression_E3 << Int_Efficiency_Progression_E3 ;
                TString Result_Progression_E3 = TString(convert_Progression_E3.str());
                Int_t IsoCut_Progression_E3 = histosIsolation[Result_Progression_E3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_E3==100) IsoCut_Progression_E3 = 1000;
                LUT_Progression_E3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_E3);

                //Progression_F3
                Double_t Efficiency_Progression_F3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 69);
                if(Efficiency_Progression_F3>=0.9999) Efficiency_Progression_F3 = 1.0001;
                Int_t Int_Efficiency_Progression_F3 = int(Efficiency_Progression_F3*100);
                ostringstream convert_Progression_F3;
                convert_Progression_F3 << Int_Efficiency_Progression_F3 ;
                TString Result_Progression_F3 = TString(convert_Progression_F3.str());
                Int_t IsoCut_Progression_F3 = histosIsolation[Result_Progression_F3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_F3==100) IsoCut_Progression_F3 = 1000;
                LUT_Progression_F3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_F3);

                //Progression_G3
                Double_t Efficiency_Progression_G3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 72);
                if(Efficiency_Progression_G3>=0.9999) Efficiency_Progression_G3 = 1.0001;
                Int_t Int_Efficiency_Progression_G3 = int(Efficiency_Progression_G3*100);
                ostringstream convert_Progression_G3;
                convert_Progression_G3 << Int_Efficiency_Progression_G3 ;
                TString Result_Progression_G3 = TString(convert_Progression_G3.str());
                Int_t IsoCut_Progression_G3 = histosIsolation[Result_Progression_G3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_G3==100) IsoCut_Progression_G3 = 1000;
                LUT_Progression_G3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_G3);

                //Progression_H3
                Double_t Efficiency_Progression_H3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 75);
                if(Efficiency_Progression_H3>=0.9999) Efficiency_Progression_H3 = 1.0001;
                Int_t Int_Efficiency_Progression_H3 = int(Efficiency_Progression_H3*100);
                ostringstream convert_Progression_H3;
                convert_Progression_H3 << Int_Efficiency_Progression_H3 ;
                TString Result_Progression_H3 = TString(convert_Progression_H3.str());
                Int_t IsoCut_Progression_H3 = histosIsolation[Result_Progression_H3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_H3==100) IsoCut_Progression_H3 = 1000;
                LUT_Progression_H3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_H3);

                //Progression_I3
                Double_t Efficiency_Progression_I3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 78);
                if(Efficiency_Progression_I3>=0.9999) Efficiency_Progression_I3 = 1.0001;
                Int_t Int_Efficiency_Progression_I3 = int(Efficiency_Progression_I3*100);
                ostringstream convert_Progression_I3;
                convert_Progression_I3 << Int_Efficiency_Progression_I3 ;
                TString Result_Progression_I3 = TString(convert_Progression_I3.str());
                Int_t IsoCut_Progression_I3 = histosIsolation[Result_Progression_I3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_I3==100) IsoCut_Progression_I3 = 1000;
                LUT_Progression_I3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_I3);

                //Progression_J3
                Double_t Efficiency_Progression_J3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 81);
                if(Efficiency_Progression_J3>=0.9999) Efficiency_Progression_J3 = 1.0001;
                Int_t Int_Efficiency_Progression_J3 = int(Efficiency_Progression_J3*100);
                ostringstream convert_Progression_J3;
                convert_Progression_J3 << Int_Efficiency_Progression_J3 ;
                TString Result_Progression_J3 = TString(convert_Progression_J3.str());
                Int_t IsoCut_Progression_J3 = histosIsolation[Result_Progression_J3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_J3==100) IsoCut_Progression_J3 = 1000;
                LUT_Progression_J3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_J3);

                //Progression_K3
                Double_t Efficiency_Progression_K3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 83);
                if(Efficiency_Progression_K3>=0.9999) Efficiency_Progression_K3 = 1.0001;
                Int_t Int_Efficiency_Progression_K3 = int(Efficiency_Progression_K3*100);
                ostringstream convert_Progression_K3;
                convert_Progression_K3 << Int_Efficiency_Progression_K3 ;
                TString Result_Progression_K3 = TString(convert_Progression_K3.str());
                Int_t IsoCut_Progression_K3 = histosIsolation[Result_Progression_K3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_K3==100) IsoCut_Progression_K3 = 1000;
                LUT_Progression_K3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_K3);

                //Progression_L3
                Double_t Efficiency_Progression_L3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 22, effMin, 86);
                if(Efficiency_Progression_L3>=0.9999) Efficiency_Progression_L3 = 1.0001;
                Int_t Int_Efficiency_Progression_L3 = int(Efficiency_Progression_L3*100);
                ostringstream convert_Progression_L3;
                convert_Progression_L3 << Int_Efficiency_Progression_L3 ;
                TString Result_Progression_L3 = TString(convert_Progression_L3.str());
                Int_t IsoCut_Progression_L3 = histosIsolation[Result_Progression_L3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_L3==100) IsoCut_Progression_L3 = 1000;
                LUT_Progression_L3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_L3);

                //Progression_M3
                Double_t Efficiency_Progression_M3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 40);
                if(Efficiency_Progression_M3>=0.9999) Efficiency_Progression_M3 = 1.0001;
                Int_t Int_Efficiency_Progression_M3 = int(Efficiency_Progression_M3*100);
                ostringstream convert_Progression_M3;
                convert_Progression_M3 << Int_Efficiency_Progression_M3 ;
                TString Result_Progression_M3 = TString(convert_Progression_M3.str());
                Int_t IsoCut_Progression_M3 = histosIsolation[Result_Progression_M3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_M3==100) IsoCut_Progression_M3 = 1000;
                LUT_Progression_M3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_M3);

                //Progression_N3
                Double_t Efficiency_Progression_N3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 43);
                if(Efficiency_Progression_N3>=0.9999) Efficiency_Progression_N3 = 1.0001;
                Int_t Int_Efficiency_Progression_N3 = int(Efficiency_Progression_N3*100);
                ostringstream convert_Progression_N3;
                convert_Progression_N3 << Int_Efficiency_Progression_N3 ;
                TString Result_Progression_N3 = TString(convert_Progression_N3.str());
                Int_t IsoCut_Progression_N3 = histosIsolation[Result_Progression_N3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_N3==100) IsoCut_Progression_N3 = 1000;
                LUT_Progression_N3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_N3);

                //Progression_O3
                Double_t Efficiency_Progression_O3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 46);
                if(Efficiency_Progression_O3>=0.9999) Efficiency_Progression_O3 = 1.0001;
                Int_t Int_Efficiency_Progression_O3 = int(Efficiency_Progression_O3*100);
                ostringstream convert_Progression_O3;
                convert_Progression_O3 << Int_Efficiency_Progression_O3 ;
                TString Result_Progression_O3 = TString(convert_Progression_O3.str());
                Int_t IsoCut_Progression_O3 = histosIsolation[Result_Progression_O3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_O3==100) IsoCut_Progression_O3 = 1000;
                LUT_Progression_O3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_O3);

                //Progression_P3
                Double_t Efficiency_Progression_P3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 49);
                if(Efficiency_Progression_P3>=0.9999) Efficiency_Progression_P3 = 1.0001;
                Int_t Int_Efficiency_Progression_P3 = int(Efficiency_Progression_P3*100);
                ostringstream convert_Progression_P3;
                convert_Progression_P3 << Int_Efficiency_Progression_P3 ;
                TString Result_Progression_P3 = TString(convert_Progression_P3.str());
                Int_t IsoCut_Progression_P3 = histosIsolation[Result_Progression_P3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_P3==100) IsoCut_Progression_P3 = 1000;
                LUT_Progression_P3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_P3);

                //Progression_Q3
                Double_t Efficiency_Progression_Q3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 52);
                if(Efficiency_Progression_Q3>=0.9999) Efficiency_Progression_Q3 = 1.0001;
                Int_t Int_Efficiency_Progression_Q3 = int(Efficiency_Progression_Q3*100);
                ostringstream convert_Progression_Q3;
                convert_Progression_Q3 << Int_Efficiency_Progression_Q3 ;
                TString Result_Progression_Q3 = TString(convert_Progression_Q3.str());
                Int_t IsoCut_Progression_Q3 = histosIsolation[Result_Progression_Q3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Q3==100) IsoCut_Progression_Q3 = 1000;
                LUT_Progression_Q3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Q3);

                //Progression_R3
                Double_t Efficiency_Progression_R3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 55);
                if(Efficiency_Progression_R3>=0.9999) Efficiency_Progression_R3 = 1.0001;
                Int_t Int_Efficiency_Progression_R3 = int(Efficiency_Progression_R3*100);
                ostringstream convert_Progression_R3;
                convert_Progression_R3 << Int_Efficiency_Progression_R3 ;
                TString Result_Progression_R3 = TString(convert_Progression_R3.str());
                Int_t IsoCut_Progression_R3 = histosIsolation[Result_Progression_R3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_R3==100) IsoCut_Progression_R3 = 1000;
                LUT_Progression_R3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_R3);

                //Progression_S3
                Double_t Efficiency_Progression_S3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 58);
                if(Efficiency_Progression_S3>=0.9999) Efficiency_Progression_S3 = 1.0001;
                Int_t Int_Efficiency_Progression_S3 = int(Efficiency_Progression_S3*100);
                ostringstream convert_Progression_S3;
                convert_Progression_S3 << Int_Efficiency_Progression_S3 ;
                TString Result_Progression_S3 = TString(convert_Progression_S3.str());
                Int_t IsoCut_Progression_S3 = histosIsolation[Result_Progression_S3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_S3==100) IsoCut_Progression_S3 = 1000;
                LUT_Progression_S3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_S3);

                //Progression_T3
                Double_t Efficiency_Progression_T3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 61);
                if(Efficiency_Progression_T3>=0.9999) Efficiency_Progression_T3 = 1.0001;
                Int_t Int_Efficiency_Progression_T3 = int(Efficiency_Progression_T3*100);
                ostringstream convert_Progression_T3;
                convert_Progression_T3 << Int_Efficiency_Progression_T3 ;
                TString Result_Progression_T3 = TString(convert_Progression_T3.str());
                Int_t IsoCut_Progression_T3 = histosIsolation[Result_Progression_T3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_T3==100) IsoCut_Progression_T3 = 1000;
                LUT_Progression_T3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_T3);

                //Progression_U3
                Double_t Efficiency_Progression_U3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 64);
                if(Efficiency_Progression_U3>=0.9999) Efficiency_Progression_U3 = 1.0001;
                Int_t Int_Efficiency_Progression_U3 = int(Efficiency_Progression_U3*100);
                ostringstream convert_Progression_U3;
                convert_Progression_U3 << Int_Efficiency_Progression_U3 ;
                TString Result_Progression_U3 = TString(convert_Progression_U3.str());
                Int_t IsoCut_Progression_U3 = histosIsolation[Result_Progression_U3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_U3==100) IsoCut_Progression_U3 = 1000;
                LUT_Progression_U3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_U3);

                //Progression_V3
                Double_t Efficiency_Progression_V3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 66);
                if(Efficiency_Progression_V3>=0.9999) Efficiency_Progression_V3 = 1.0001;
                Int_t Int_Efficiency_Progression_V3 = int(Efficiency_Progression_V3*100);
                ostringstream convert_Progression_V3;
                convert_Progression_V3 << Int_Efficiency_Progression_V3 ;
                TString Result_Progression_V3 = TString(convert_Progression_V3.str());
                Int_t IsoCut_Progression_V3 = histosIsolation[Result_Progression_V3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_V3==100) IsoCut_Progression_V3 = 1000;
                LUT_Progression_V3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_V3);

                //Progression_W3
                Double_t Efficiency_Progression_W3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 69);
                if(Efficiency_Progression_W3>=0.9999) Efficiency_Progression_W3 = 1.0001;
                Int_t Int_Efficiency_Progression_W3 = int(Efficiency_Progression_W3*100);
                ostringstream convert_Progression_W3;
                convert_Progression_W3 << Int_Efficiency_Progression_W3 ;
                TString Result_Progression_W3 = TString(convert_Progression_W3.str());
                Int_t IsoCut_Progression_W3 = histosIsolation[Result_Progression_W3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_W3==100) IsoCut_Progression_W3 = 1000;
                LUT_Progression_W3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_W3);

                //Progression_X3
                Double_t Efficiency_Progression_X3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 72);
                if(Efficiency_Progression_X3>=0.9999) Efficiency_Progression_X3 = 1.0001;
                Int_t Int_Efficiency_Progression_X3 = int(Efficiency_Progression_X3*100);
                ostringstream convert_Progression_X3;
                convert_Progression_X3 << Int_Efficiency_Progression_X3 ;
                TString Result_Progression_X3 = TString(convert_Progression_X3.str());
                Int_t IsoCut_Progression_X3 = histosIsolation[Result_Progression_X3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_X3==100) IsoCut_Progression_X3 = 1000;
                LUT_Progression_X3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_X3);

                //Progression_Y3
                Double_t Efficiency_Progression_Y3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 75);
                if(Efficiency_Progression_Y3>=0.9999) Efficiency_Progression_Y3 = 1.0001;
                Int_t Int_Efficiency_Progression_Y3 = int(Efficiency_Progression_Y3*100);
                ostringstream convert_Progression_Y3;
                convert_Progression_Y3 << Int_Efficiency_Progression_Y3 ;
                TString Result_Progression_Y3 = TString(convert_Progression_Y3.str());
                Int_t IsoCut_Progression_Y3 = histosIsolation[Result_Progression_Y3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Y3==100) IsoCut_Progression_Y3 = 1000;
                LUT_Progression_Y3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Y3);

                //Progression_Z3
                Double_t Efficiency_Progression_Z3 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 78);
                if(Efficiency_Progression_Z3>=0.9999) Efficiency_Progression_Z3 = 1.0001;
                Int_t Int_Efficiency_Progression_Z3 = int(Efficiency_Progression_Z3*100);
                ostringstream convert_Progression_Z3;
                convert_Progression_Z3 << Int_Efficiency_Progression_Z3 ;
                TString Result_Progression_Z3 = TString(convert_Progression_Z3.str());
                Int_t IsoCut_Progression_Z3 = histosIsolation[Result_Progression_Z3]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Z3==100) IsoCut_Progression_Z3 = 1000;
                LUT_Progression_Z3->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Z3);

                //Progression_A4
                Double_t Efficiency_Progression_A4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 81);
                if(Efficiency_Progression_A4>=0.9999) Efficiency_Progression_A4 = 1.0001;
                Int_t Int_Efficiency_Progression_A4 = int(Efficiency_Progression_A4*100);
                ostringstream convert_Progression_A4;
                convert_Progression_A4 << Int_Efficiency_Progression_A4 ;
                TString Result_Progression_A4 = TString(convert_Progression_A4.str());
                Int_t IsoCut_Progression_A4 = histosIsolation[Result_Progression_A4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_A4==100) IsoCut_Progression_A4 = 1000;
                LUT_Progression_A4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_A4);

                //Progression_B4
                Double_t Efficiency_Progression_B4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 84);
                if(Efficiency_Progression_B4>=0.9999) Efficiency_Progression_B4 = 1.0001;
                Int_t Int_Efficiency_Progression_B4 = int(Efficiency_Progression_B4*100);
                ostringstream convert_Progression_B4;
                convert_Progression_B4 << Int_Efficiency_Progression_B4 ;
                TString Result_Progression_B4 = TString(convert_Progression_B4.str());
                Int_t IsoCut_Progression_B4 = histosIsolation[Result_Progression_B4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_B4==100) IsoCut_Progression_B4 = 1000;
                LUT_Progression_B4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_B4);

                //Progression_C4
                Double_t Efficiency_Progression_C4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 86);
                if(Efficiency_Progression_C4>=0.9999) Efficiency_Progression_C4 = 1.0001;
                Int_t Int_Efficiency_Progression_C4 = int(Efficiency_Progression_C4*100);
                ostringstream convert_Progression_C4;
                convert_Progression_C4 << Int_Efficiency_Progression_C4 ;
                TString Result_Progression_C4 = TString(convert_Progression_C4.str());
                Int_t IsoCut_Progression_C4 = histosIsolation[Result_Progression_C4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_C4==100) IsoCut_Progression_C4 = 1000;
                LUT_Progression_C4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_C4);

                //Progression_D4
                Double_t Efficiency_Progression_D4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 25, effMin, 89);
                if(Efficiency_Progression_D4>=0.9999) Efficiency_Progression_D4 = 1.0001;
                Int_t Int_Efficiency_Progression_D4 = int(Efficiency_Progression_D4*100);
                ostringstream convert_Progression_D4;
                convert_Progression_D4 << Int_Efficiency_Progression_D4 ;
                TString Result_Progression_D4 = TString(convert_Progression_D4.str());
                Int_t IsoCut_Progression_D4 = histosIsolation[Result_Progression_D4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_D4==100) IsoCut_Progression_D4 = 1000;
                LUT_Progression_D4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_D4);

                //Progression_E4
                Double_t Efficiency_Progression_E4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 43);
                if(Efficiency_Progression_E4>=0.9999) Efficiency_Progression_E4 = 1.0001;
                Int_t Int_Efficiency_Progression_E4 = int(Efficiency_Progression_E4*100);
                ostringstream convert_Progression_E4;
                convert_Progression_E4 << Int_Efficiency_Progression_E4 ;
                TString Result_Progression_E4 = TString(convert_Progression_E4.str());
                Int_t IsoCut_Progression_E4 = histosIsolation[Result_Progression_E4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_E4==100) IsoCut_Progression_E4 = 1000;
                LUT_Progression_E4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_E4);

                //Progression_F4
                Double_t Efficiency_Progression_F4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 46);
                if(Efficiency_Progression_F4>=0.9999) Efficiency_Progression_F4 = 1.0001;
                Int_t Int_Efficiency_Progression_F4 = int(Efficiency_Progression_F4*100);
                ostringstream convert_Progression_F4;
                convert_Progression_F4 << Int_Efficiency_Progression_F4 ;
                TString Result_Progression_F4 = TString(convert_Progression_F4.str());
                Int_t IsoCut_Progression_F4 = histosIsolation[Result_Progression_F4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_F4==100) IsoCut_Progression_F4 = 1000;
                LUT_Progression_F4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_F4);

                //Progression_G4
                Double_t Efficiency_Progression_G4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 49);
                if(Efficiency_Progression_G4>=0.9999) Efficiency_Progression_G4 = 1.0001;
                Int_t Int_Efficiency_Progression_G4 = int(Efficiency_Progression_G4*100);
                ostringstream convert_Progression_G4;
                convert_Progression_G4 << Int_Efficiency_Progression_G4 ;
                TString Result_Progression_G4 = TString(convert_Progression_G4.str());
                Int_t IsoCut_Progression_G4 = histosIsolation[Result_Progression_G4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_G4==100) IsoCut_Progression_G4 = 1000;
                LUT_Progression_G4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_G4);

                //Progression_H4
                Double_t Efficiency_Progression_H4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 52);
                if(Efficiency_Progression_H4>=0.9999) Efficiency_Progression_H4 = 1.0001;
                Int_t Int_Efficiency_Progression_H4 = int(Efficiency_Progression_H4*100);
                ostringstream convert_Progression_H4;
                convert_Progression_H4 << Int_Efficiency_Progression_H4 ;
                TString Result_Progression_H4 = TString(convert_Progression_H4.str());
                Int_t IsoCut_Progression_H4 = histosIsolation[Result_Progression_H4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_H4==100) IsoCut_Progression_H4 = 1000;
                LUT_Progression_H4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_H4);

                //Progression_I4
                Double_t Efficiency_Progression_I4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 55);
                if(Efficiency_Progression_I4>=0.9999) Efficiency_Progression_I4 = 1.0001;
                Int_t Int_Efficiency_Progression_I4 = int(Efficiency_Progression_I4*100);
                ostringstream convert_Progression_I4;
                convert_Progression_I4 << Int_Efficiency_Progression_I4 ;
                TString Result_Progression_I4 = TString(convert_Progression_I4.str());
                Int_t IsoCut_Progression_I4 = histosIsolation[Result_Progression_I4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_I4==100) IsoCut_Progression_I4 = 1000;
                LUT_Progression_I4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_I4);

                //Progression_J4
                Double_t Efficiency_Progression_J4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 58);
                if(Efficiency_Progression_J4>=0.9999) Efficiency_Progression_J4 = 1.0001;
                Int_t Int_Efficiency_Progression_J4 = int(Efficiency_Progression_J4*100);
                ostringstream convert_Progression_J4;
                convert_Progression_J4 << Int_Efficiency_Progression_J4 ;
                TString Result_Progression_J4 = TString(convert_Progression_J4.str());
                Int_t IsoCut_Progression_J4 = histosIsolation[Result_Progression_J4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_J4==100) IsoCut_Progression_J4 = 1000;
                LUT_Progression_J4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_J4);

                //Progression_K4
                Double_t Efficiency_Progression_K4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 61);
                if(Efficiency_Progression_K4>=0.9999) Efficiency_Progression_K4 = 1.0001;
                Int_t Int_Efficiency_Progression_K4 = int(Efficiency_Progression_K4*100);
                ostringstream convert_Progression_K4;
                convert_Progression_K4 << Int_Efficiency_Progression_K4 ;
                TString Result_Progression_K4 = TString(convert_Progression_K4.str());
                Int_t IsoCut_Progression_K4 = histosIsolation[Result_Progression_K4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_K4==100) IsoCut_Progression_K4 = 1000;
                LUT_Progression_K4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_K4);

                //Progression_L4
                Double_t Efficiency_Progression_L4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 64);
                if(Efficiency_Progression_L4>=0.9999) Efficiency_Progression_L4 = 1.0001;
                Int_t Int_Efficiency_Progression_L4 = int(Efficiency_Progression_L4*100);
                ostringstream convert_Progression_L4;
                convert_Progression_L4 << Int_Efficiency_Progression_L4 ;
                TString Result_Progression_L4 = TString(convert_Progression_L4.str());
                Int_t IsoCut_Progression_L4 = histosIsolation[Result_Progression_L4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_L4==100) IsoCut_Progression_L4 = 1000;
                LUT_Progression_L4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_L4);

                //Progression_M4
                Double_t Efficiency_Progression_M4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 67);
                if(Efficiency_Progression_M4>=0.9999) Efficiency_Progression_M4 = 1.0001;
                Int_t Int_Efficiency_Progression_M4 = int(Efficiency_Progression_M4*100);
                ostringstream convert_Progression_M4;
                convert_Progression_M4 << Int_Efficiency_Progression_M4 ;
                TString Result_Progression_M4 = TString(convert_Progression_M4.str());
                Int_t IsoCut_Progression_M4 = histosIsolation[Result_Progression_M4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_M4==100) IsoCut_Progression_M4 = 1000;
                LUT_Progression_M4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_M4);

                //Progression_N4
                Double_t Efficiency_Progression_N4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 69);
                if(Efficiency_Progression_N4>=0.9999) Efficiency_Progression_N4 = 1.0001;
                Int_t Int_Efficiency_Progression_N4 = int(Efficiency_Progression_N4*100);
                ostringstream convert_Progression_N4;
                convert_Progression_N4 << Int_Efficiency_Progression_N4 ;
                TString Result_Progression_N4 = TString(convert_Progression_N4.str());
                Int_t IsoCut_Progression_N4 = histosIsolation[Result_Progression_N4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_N4==100) IsoCut_Progression_N4 = 1000;
                LUT_Progression_N4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_N4);

                //Progression_O4
                Double_t Efficiency_Progression_O4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 72);
                if(Efficiency_Progression_O4>=0.9999) Efficiency_Progression_O4 = 1.0001;
                Int_t Int_Efficiency_Progression_O4 = int(Efficiency_Progression_O4*100);
                ostringstream convert_Progression_O4;
                convert_Progression_O4 << Int_Efficiency_Progression_O4 ;
                TString Result_Progression_O4 = TString(convert_Progression_O4.str());
                Int_t IsoCut_Progression_O4 = histosIsolation[Result_Progression_O4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_O4==100) IsoCut_Progression_O4 = 1000;
                LUT_Progression_O4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_O4);

                //Progression_P4
                Double_t Efficiency_Progression_P4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 75);
                if(Efficiency_Progression_P4>=0.9999) Efficiency_Progression_P4 = 1.0001;
                Int_t Int_Efficiency_Progression_P4 = int(Efficiency_Progression_P4*100);
                ostringstream convert_Progression_P4;
                convert_Progression_P4 << Int_Efficiency_Progression_P4 ;
                TString Result_Progression_P4 = TString(convert_Progression_P4.str());
                Int_t IsoCut_Progression_P4 = histosIsolation[Result_Progression_P4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_P4==100) IsoCut_Progression_P4 = 1000;
                LUT_Progression_P4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_P4);

                //Progression_Q4
                Double_t Efficiency_Progression_Q4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 78);
                if(Efficiency_Progression_Q4>=0.9999) Efficiency_Progression_Q4 = 1.0001;
                Int_t Int_Efficiency_Progression_Q4 = int(Efficiency_Progression_Q4*100);
                ostringstream convert_Progression_Q4;
                convert_Progression_Q4 << Int_Efficiency_Progression_Q4 ;
                TString Result_Progression_Q4 = TString(convert_Progression_Q4.str());
                Int_t IsoCut_Progression_Q4 = histosIsolation[Result_Progression_Q4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Q4==100) IsoCut_Progression_Q4 = 1000;
                LUT_Progression_Q4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Q4);

                //Progression_R4
                Double_t Efficiency_Progression_R4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 81);
                if(Efficiency_Progression_R4>=0.9999) Efficiency_Progression_R4 = 1.0001;
                Int_t Int_Efficiency_Progression_R4 = int(Efficiency_Progression_R4*100);
                ostringstream convert_Progression_R4;
                convert_Progression_R4 << Int_Efficiency_Progression_R4 ;
                TString Result_Progression_R4 = TString(convert_Progression_R4.str());
                Int_t IsoCut_Progression_R4 = histosIsolation[Result_Progression_R4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_R4==100) IsoCut_Progression_R4 = 1000;
                LUT_Progression_R4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_R4);

                //Progression_S4
                Double_t Efficiency_Progression_S4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 84);
                if(Efficiency_Progression_S4>=0.9999) Efficiency_Progression_S4 = 1.0001;
                Int_t Int_Efficiency_Progression_S4 = int(Efficiency_Progression_S4*100);
                ostringstream convert_Progression_S4;
                convert_Progression_S4 << Int_Efficiency_Progression_S4 ;
                TString Result_Progression_S4 = TString(convert_Progression_S4.str());
                Int_t IsoCut_Progression_S4 = histosIsolation[Result_Progression_S4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_S4==100) IsoCut_Progression_S4 = 1000;
                LUT_Progression_S4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_S4);

                //Progression_T4
                Double_t Efficiency_Progression_T4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 87);
                if(Efficiency_Progression_T4>=0.9999) Efficiency_Progression_T4 = 1.0001;
                Int_t Int_Efficiency_Progression_T4 = int(Efficiency_Progression_T4*100);
                ostringstream convert_Progression_T4;
                convert_Progression_T4 << Int_Efficiency_Progression_T4 ;
                TString Result_Progression_T4 = TString(convert_Progression_T4.str());
                Int_t IsoCut_Progression_T4 = histosIsolation[Result_Progression_T4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_T4==100) IsoCut_Progression_T4 = 1000;
                LUT_Progression_T4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_T4);

                //Progression_U4
                Double_t Efficiency_Progression_U4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 89);
                if(Efficiency_Progression_U4>=0.9999) Efficiency_Progression_U4 = 1.0001;
                Int_t Int_Efficiency_Progression_U4 = int(Efficiency_Progression_U4*100);
                ostringstream convert_Progression_U4;
                convert_Progression_U4 << Int_Efficiency_Progression_U4 ;
                TString Result_Progression_U4 = TString(convert_Progression_U4.str());
                Int_t IsoCut_Progression_U4 = histosIsolation[Result_Progression_U4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_U4==100) IsoCut_Progression_U4 = 1000;
                LUT_Progression_U4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_U4);

                //Progression_V4
                Double_t Efficiency_Progression_V4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 28, effMin, 92);
                if(Efficiency_Progression_V4>=0.9999) Efficiency_Progression_V4 = 1.0001;
                Int_t Int_Efficiency_Progression_V4 = int(Efficiency_Progression_V4*100);
                ostringstream convert_Progression_V4;
                convert_Progression_V4 << Int_Efficiency_Progression_V4 ;
                TString Result_Progression_V4 = TString(convert_Progression_V4.str());
                Int_t IsoCut_Progression_V4 = histosIsolation[Result_Progression_V4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_V4==100) IsoCut_Progression_V4 = 1000;
                LUT_Progression_V4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_V4);

                //Progression_W4
                Double_t Efficiency_Progression_W4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 46);
                if(Efficiency_Progression_W4>=0.9999) Efficiency_Progression_W4 = 1.0001;
                Int_t Int_Efficiency_Progression_W4 = int(Efficiency_Progression_W4*100);
                ostringstream convert_Progression_W4;
                convert_Progression_W4 << Int_Efficiency_Progression_W4 ;
                TString Result_Progression_W4 = TString(convert_Progression_W4.str());
                Int_t IsoCut_Progression_W4 = histosIsolation[Result_Progression_W4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_W4==100) IsoCut_Progression_W4 = 1000;
                LUT_Progression_W4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_W4);

                //Progression_X4
                Double_t Efficiency_Progression_X4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 49);
                if(Efficiency_Progression_X4>=0.9999) Efficiency_Progression_X4 = 1.0001;
                Int_t Int_Efficiency_Progression_X4 = int(Efficiency_Progression_X4*100);
                ostringstream convert_Progression_X4;
                convert_Progression_X4 << Int_Efficiency_Progression_X4 ;
                TString Result_Progression_X4 = TString(convert_Progression_X4.str());
                Int_t IsoCut_Progression_X4 = histosIsolation[Result_Progression_X4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_X4==100) IsoCut_Progression_X4 = 1000;
                LUT_Progression_X4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_X4);

                //Progression_Y4
                Double_t Efficiency_Progression_Y4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 52);
                if(Efficiency_Progression_Y4>=0.9999) Efficiency_Progression_Y4 = 1.0001;
                Int_t Int_Efficiency_Progression_Y4 = int(Efficiency_Progression_Y4*100);
                ostringstream convert_Progression_Y4;
                convert_Progression_Y4 << Int_Efficiency_Progression_Y4 ;
                TString Result_Progression_Y4 = TString(convert_Progression_Y4.str());
                Int_t IsoCut_Progression_Y4 = histosIsolation[Result_Progression_Y4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Y4==100) IsoCut_Progression_Y4 = 1000;
                LUT_Progression_Y4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Y4);

                //Progression_Z4
                Double_t Efficiency_Progression_Z4 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 55);
                if(Efficiency_Progression_Z4>=0.9999) Efficiency_Progression_Z4 = 1.0001;
                Int_t Int_Efficiency_Progression_Z4 = int(Efficiency_Progression_Z4*100);
                ostringstream convert_Progression_Z4;
                convert_Progression_Z4 << Int_Efficiency_Progression_Z4 ;
                TString Result_Progression_Z4 = TString(convert_Progression_Z4.str());
                Int_t IsoCut_Progression_Z4 = histosIsolation[Result_Progression_Z4]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Z4==100) IsoCut_Progression_Z4 = 1000;
                LUT_Progression_Z4->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Z4);

                //Progression_A5
                Double_t Efficiency_Progression_A5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 58);
                if(Efficiency_Progression_A5>=0.9999) Efficiency_Progression_A5 = 1.0001;
                Int_t Int_Efficiency_Progression_A5 = int(Efficiency_Progression_A5*100);
                ostringstream convert_Progression_A5;
                convert_Progression_A5 << Int_Efficiency_Progression_A5 ;
                TString Result_Progression_A5 = TString(convert_Progression_A5.str());
                Int_t IsoCut_Progression_A5 = histosIsolation[Result_Progression_A5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_A5==100) IsoCut_Progression_A5 = 1000;
                LUT_Progression_A5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_A5);

                //Progression_B5
                Double_t Efficiency_Progression_B5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 61);
                if(Efficiency_Progression_B5>=0.9999) Efficiency_Progression_B5 = 1.0001;
                Int_t Int_Efficiency_Progression_B5 = int(Efficiency_Progression_B5*100);
                ostringstream convert_Progression_B5;
                convert_Progression_B5 << Int_Efficiency_Progression_B5 ;
                TString Result_Progression_B5 = TString(convert_Progression_B5.str());
                Int_t IsoCut_Progression_B5 = histosIsolation[Result_Progression_B5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_B5==100) IsoCut_Progression_B5 = 1000;
                LUT_Progression_B5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_B5);

                //Progression_C5
                Double_t Efficiency_Progression_C5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 64);
                if(Efficiency_Progression_C5>=0.9999) Efficiency_Progression_C5 = 1.0001;
                Int_t Int_Efficiency_Progression_C5 = int(Efficiency_Progression_C5*100);
                ostringstream convert_Progression_C5;
                convert_Progression_C5 << Int_Efficiency_Progression_C5 ;
                TString Result_Progression_C5 = TString(convert_Progression_C5.str());
                Int_t IsoCut_Progression_C5 = histosIsolation[Result_Progression_C5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_C5==100) IsoCut_Progression_C5 = 1000;
                LUT_Progression_C5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_C5);

                //Progression_D5
                Double_t Efficiency_Progression_D5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 67);
                if(Efficiency_Progression_D5>=0.9999) Efficiency_Progression_D5 = 1.0001;
                Int_t Int_Efficiency_Progression_D5 = int(Efficiency_Progression_D5*100);
                ostringstream convert_Progression_D5;
                convert_Progression_D5 << Int_Efficiency_Progression_D5 ;
                TString Result_Progression_D5 = TString(convert_Progression_D5.str());
                Int_t IsoCut_Progression_D5 = histosIsolation[Result_Progression_D5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_D5==100) IsoCut_Progression_D5 = 1000;
                LUT_Progression_D5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_D5);

                //Progression_E5
                Double_t Efficiency_Progression_E5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 70);
                if(Efficiency_Progression_E5>=0.9999) Efficiency_Progression_E5 = 1.0001;
                Int_t Int_Efficiency_Progression_E5 = int(Efficiency_Progression_E5*100);
                ostringstream convert_Progression_E5;
                convert_Progression_E5 << Int_Efficiency_Progression_E5 ;
                TString Result_Progression_E5 = TString(convert_Progression_E5.str());
                Int_t IsoCut_Progression_E5 = histosIsolation[Result_Progression_E5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_E5==100) IsoCut_Progression_E5 = 1000;
                LUT_Progression_E5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_E5);

                //Progression_F5
                Double_t Efficiency_Progression_F5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 72);
                if(Efficiency_Progression_F5>=0.9999) Efficiency_Progression_F5 = 1.0001;
                Int_t Int_Efficiency_Progression_F5 = int(Efficiency_Progression_F5*100);
                ostringstream convert_Progression_F5;
                convert_Progression_F5 << Int_Efficiency_Progression_F5 ;
                TString Result_Progression_F5 = TString(convert_Progression_F5.str());
                Int_t IsoCut_Progression_F5 = histosIsolation[Result_Progression_F5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_F5==100) IsoCut_Progression_F5 = 1000;
                LUT_Progression_F5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_F5);

                //Progression_G5
                Double_t Efficiency_Progression_G5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 75);
                if(Efficiency_Progression_G5>=0.9999) Efficiency_Progression_G5 = 1.0001;
                Int_t Int_Efficiency_Progression_G5 = int(Efficiency_Progression_G5*100);
                ostringstream convert_Progression_G5;
                convert_Progression_G5 << Int_Efficiency_Progression_G5 ;
                TString Result_Progression_G5 = TString(convert_Progression_G5.str());
                Int_t IsoCut_Progression_G5 = histosIsolation[Result_Progression_G5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_G5==100) IsoCut_Progression_G5 = 1000;
                LUT_Progression_G5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_G5);

                //Progression_H5
                Double_t Efficiency_Progression_H5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 78);
                if(Efficiency_Progression_H5>=0.9999) Efficiency_Progression_H5 = 1.0001;
                Int_t Int_Efficiency_Progression_H5 = int(Efficiency_Progression_H5*100);
                ostringstream convert_Progression_H5;
                convert_Progression_H5 << Int_Efficiency_Progression_H5 ;
                TString Result_Progression_H5 = TString(convert_Progression_H5.str());
                Int_t IsoCut_Progression_H5 = histosIsolation[Result_Progression_H5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_H5==100) IsoCut_Progression_H5 = 1000;
                LUT_Progression_H5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_H5);

                //Progression_I5
                Double_t Efficiency_Progression_I5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 81);
                if(Efficiency_Progression_I5>=0.9999) Efficiency_Progression_I5 = 1.0001;
                Int_t Int_Efficiency_Progression_I5 = int(Efficiency_Progression_I5*100);
                ostringstream convert_Progression_I5;
                convert_Progression_I5 << Int_Efficiency_Progression_I5 ;
                TString Result_Progression_I5 = TString(convert_Progression_I5.str());
                Int_t IsoCut_Progression_I5 = histosIsolation[Result_Progression_I5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_I5==100) IsoCut_Progression_I5 = 1000;
                LUT_Progression_I5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_I5);

                //Progression_J5
                Double_t Efficiency_Progression_J5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 84);
                if(Efficiency_Progression_J5>=0.9999) Efficiency_Progression_J5 = 1.0001;
                Int_t Int_Efficiency_Progression_J5 = int(Efficiency_Progression_J5*100);
                ostringstream convert_Progression_J5;
                convert_Progression_J5 << Int_Efficiency_Progression_J5 ;
                TString Result_Progression_J5 = TString(convert_Progression_J5.str());
                Int_t IsoCut_Progression_J5 = histosIsolation[Result_Progression_J5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_J5==100) IsoCut_Progression_J5 = 1000;
                LUT_Progression_J5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_J5);

                //Progression_K5
                Double_t Efficiency_Progression_K5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 87);
                if(Efficiency_Progression_K5>=0.9999) Efficiency_Progression_K5 = 1.0001;
                Int_t Int_Efficiency_Progression_K5 = int(Efficiency_Progression_K5*100);
                ostringstream convert_Progression_K5;
                convert_Progression_K5 << Int_Efficiency_Progression_K5 ;
                TString Result_Progression_K5 = TString(convert_Progression_K5.str());
                Int_t IsoCut_Progression_K5 = histosIsolation[Result_Progression_K5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_K5==100) IsoCut_Progression_K5 = 1000;
                LUT_Progression_K5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_K5);

                //Progression_L5
                Double_t Efficiency_Progression_L5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 90);
                if(Efficiency_Progression_L5>=0.9999) Efficiency_Progression_L5 = 1.0001;
                Int_t Int_Efficiency_Progression_L5 = int(Efficiency_Progression_L5*100);
                ostringstream convert_Progression_L5;
                convert_Progression_L5 << Int_Efficiency_Progression_L5 ;
                TString Result_Progression_L5 = TString(convert_Progression_L5.str());
                Int_t IsoCut_Progression_L5 = histosIsolation[Result_Progression_L5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_L5==100) IsoCut_Progression_L5 = 1000;
                LUT_Progression_L5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_L5);

                //Progression_M5
                Double_t Efficiency_Progression_M5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 92);
                if(Efficiency_Progression_M5>=0.9999) Efficiency_Progression_M5 = 1.0001;
                Int_t Int_Efficiency_Progression_M5 = int(Efficiency_Progression_M5*100);
                ostringstream convert_Progression_M5;
                convert_Progression_M5 << Int_Efficiency_Progression_M5 ;
                TString Result_Progression_M5 = TString(convert_Progression_M5.str());
                Int_t IsoCut_Progression_M5 = histosIsolation[Result_Progression_M5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_M5==100) IsoCut_Progression_M5 = 1000;
                LUT_Progression_M5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_M5);

                //Progression_N5
                Double_t Efficiency_Progression_N5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 31, effMin, 95);
                if(Efficiency_Progression_N5>=0.9999) Efficiency_Progression_N5 = 1.0001;
                Int_t Int_Efficiency_Progression_N5 = int(Efficiency_Progression_N5*100);
                ostringstream convert_Progression_N5;
                convert_Progression_N5 << Int_Efficiency_Progression_N5 ;
                TString Result_Progression_N5 = TString(convert_Progression_N5.str());
                Int_t IsoCut_Progression_N5 = histosIsolation[Result_Progression_N5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_N5==100) IsoCut_Progression_N5 = 1000;
                LUT_Progression_N5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_N5);

                //Progression_O5
                Double_t Efficiency_Progression_O5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 49);
                if(Efficiency_Progression_O5>=0.9999) Efficiency_Progression_O5 = 1.0001;
                Int_t Int_Efficiency_Progression_O5 = int(Efficiency_Progression_O5*100);
                ostringstream convert_Progression_O5;
                convert_Progression_O5 << Int_Efficiency_Progression_O5 ;
                TString Result_Progression_O5 = TString(convert_Progression_O5.str());
                Int_t IsoCut_Progression_O5 = histosIsolation[Result_Progression_O5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_O5==100) IsoCut_Progression_O5 = 1000;
                LUT_Progression_O5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_O5);

                //Progression_P5
                Double_t Efficiency_Progression_P5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 52);
                if(Efficiency_Progression_P5>=0.9999) Efficiency_Progression_P5 = 1.0001;
                Int_t Int_Efficiency_Progression_P5 = int(Efficiency_Progression_P5*100);
                ostringstream convert_Progression_P5;
                convert_Progression_P5 << Int_Efficiency_Progression_P5 ;
                TString Result_Progression_P5 = TString(convert_Progression_P5.str());
                Int_t IsoCut_Progression_P5 = histosIsolation[Result_Progression_P5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_P5==100) IsoCut_Progression_P5 = 1000;
                LUT_Progression_P5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_P5);

                //Progression_Q5
                Double_t Efficiency_Progression_Q5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 55);
                if(Efficiency_Progression_Q5>=0.9999) Efficiency_Progression_Q5 = 1.0001;
                Int_t Int_Efficiency_Progression_Q5 = int(Efficiency_Progression_Q5*100);
                ostringstream convert_Progression_Q5;
                convert_Progression_Q5 << Int_Efficiency_Progression_Q5 ;
                TString Result_Progression_Q5 = TString(convert_Progression_Q5.str());
                Int_t IsoCut_Progression_Q5 = histosIsolation[Result_Progression_Q5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Q5==100) IsoCut_Progression_Q5 = 1000;
                LUT_Progression_Q5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Q5);

                //Progression_R5
                Double_t Efficiency_Progression_R5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 58);
                if(Efficiency_Progression_R5>=0.9999) Efficiency_Progression_R5 = 1.0001;
                Int_t Int_Efficiency_Progression_R5 = int(Efficiency_Progression_R5*100);
                ostringstream convert_Progression_R5;
                convert_Progression_R5 << Int_Efficiency_Progression_R5 ;
                TString Result_Progression_R5 = TString(convert_Progression_R5.str());
                Int_t IsoCut_Progression_R5 = histosIsolation[Result_Progression_R5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_R5==100) IsoCut_Progression_R5 = 1000;
                LUT_Progression_R5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_R5);

                //Progression_S5
                Double_t Efficiency_Progression_S5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 61);
                if(Efficiency_Progression_S5>=0.9999) Efficiency_Progression_S5 = 1.0001;
                Int_t Int_Efficiency_Progression_S5 = int(Efficiency_Progression_S5*100);
                ostringstream convert_Progression_S5;
                convert_Progression_S5 << Int_Efficiency_Progression_S5 ;
                TString Result_Progression_S5 = TString(convert_Progression_S5.str());
                Int_t IsoCut_Progression_S5 = histosIsolation[Result_Progression_S5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_S5==100) IsoCut_Progression_S5 = 1000;
                LUT_Progression_S5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_S5);

                //Progression_T5
                Double_t Efficiency_Progression_T5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 64);
                if(Efficiency_Progression_T5>=0.9999) Efficiency_Progression_T5 = 1.0001;
                Int_t Int_Efficiency_Progression_T5 = int(Efficiency_Progression_T5*100);
                ostringstream convert_Progression_T5;
                convert_Progression_T5 << Int_Efficiency_Progression_T5 ;
                TString Result_Progression_T5 = TString(convert_Progression_T5.str());
                Int_t IsoCut_Progression_T5 = histosIsolation[Result_Progression_T5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_T5==100) IsoCut_Progression_T5 = 1000;
                LUT_Progression_T5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_T5);

                //Progression_U5
                Double_t Efficiency_Progression_U5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 67);
                if(Efficiency_Progression_U5>=0.9999) Efficiency_Progression_U5 = 1.0001;
                Int_t Int_Efficiency_Progression_U5 = int(Efficiency_Progression_U5*100);
                ostringstream convert_Progression_U5;
                convert_Progression_U5 << Int_Efficiency_Progression_U5 ;
                TString Result_Progression_U5 = TString(convert_Progression_U5.str());
                Int_t IsoCut_Progression_U5 = histosIsolation[Result_Progression_U5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_U5==100) IsoCut_Progression_U5 = 1000;
                LUT_Progression_U5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_U5);

                //Progression_V5
                Double_t Efficiency_Progression_V5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 70);
                if(Efficiency_Progression_V5>=0.9999) Efficiency_Progression_V5 = 1.0001;
                Int_t Int_Efficiency_Progression_V5 = int(Efficiency_Progression_V5*100);
                ostringstream convert_Progression_V5;
                convert_Progression_V5 << Int_Efficiency_Progression_V5 ;
                TString Result_Progression_V5 = TString(convert_Progression_V5.str());
                Int_t IsoCut_Progression_V5 = histosIsolation[Result_Progression_V5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_V5==100) IsoCut_Progression_V5 = 1000;
                LUT_Progression_V5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_V5);

                //Progression_W5
                Double_t Efficiency_Progression_W5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 73);
                if(Efficiency_Progression_W5>=0.9999) Efficiency_Progression_W5 = 1.0001;
                Int_t Int_Efficiency_Progression_W5 = int(Efficiency_Progression_W5*100);
                ostringstream convert_Progression_W5;
                convert_Progression_W5 << Int_Efficiency_Progression_W5 ;
                TString Result_Progression_W5 = TString(convert_Progression_W5.str());
                Int_t IsoCut_Progression_W5 = histosIsolation[Result_Progression_W5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_W5==100) IsoCut_Progression_W5 = 1000;
                LUT_Progression_W5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_W5);

                //Progression_X5
                Double_t Efficiency_Progression_X5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 75);
                if(Efficiency_Progression_X5>=0.9999) Efficiency_Progression_X5 = 1.0001;
                Int_t Int_Efficiency_Progression_X5 = int(Efficiency_Progression_X5*100);
                ostringstream convert_Progression_X5;
                convert_Progression_X5 << Int_Efficiency_Progression_X5 ;
                TString Result_Progression_X5 = TString(convert_Progression_X5.str());
                Int_t IsoCut_Progression_X5 = histosIsolation[Result_Progression_X5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_X5==100) IsoCut_Progression_X5 = 1000;
                LUT_Progression_X5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_X5);

                //Progression_Y5
                Double_t Efficiency_Progression_Y5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 78);
                if(Efficiency_Progression_Y5>=0.9999) Efficiency_Progression_Y5 = 1.0001;
                Int_t Int_Efficiency_Progression_Y5 = int(Efficiency_Progression_Y5*100);
                ostringstream convert_Progression_Y5;
                convert_Progression_Y5 << Int_Efficiency_Progression_Y5 ;
                TString Result_Progression_Y5 = TString(convert_Progression_Y5.str());
                Int_t IsoCut_Progression_Y5 = histosIsolation[Result_Progression_Y5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Y5==100) IsoCut_Progression_Y5 = 1000;
                LUT_Progression_Y5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Y5);

                //Progression_Z5
                Double_t Efficiency_Progression_Z5 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 81);
                if(Efficiency_Progression_Z5>=0.9999) Efficiency_Progression_Z5 = 1.0001;
                Int_t Int_Efficiency_Progression_Z5 = int(Efficiency_Progression_Z5*100);
                ostringstream convert_Progression_Z5;
                convert_Progression_Z5 << Int_Efficiency_Progression_Z5 ;
                TString Result_Progression_Z5 = TString(convert_Progression_Z5.str());
                Int_t IsoCut_Progression_Z5 = histosIsolation[Result_Progression_Z5]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Z5==100) IsoCut_Progression_Z5 = 1000;
                LUT_Progression_Z5->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Z5);

                //Progression_A6
                Double_t Efficiency_Progression_A6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 84);
                if(Efficiency_Progression_A6>=0.9999) Efficiency_Progression_A6 = 1.0001;
                Int_t Int_Efficiency_Progression_A6 = int(Efficiency_Progression_A6*100);
                ostringstream convert_Progression_A6;
                convert_Progression_A6 << Int_Efficiency_Progression_A6 ;
                TString Result_Progression_A6 = TString(convert_Progression_A6.str());
                Int_t IsoCut_Progression_A6 = histosIsolation[Result_Progression_A6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_A6==100) IsoCut_Progression_A6 = 1000;
                LUT_Progression_A6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_A6);

                //Progression_B6
                Double_t Efficiency_Progression_B6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 87);
                if(Efficiency_Progression_B6>=0.9999) Efficiency_Progression_B6 = 1.0001;
                Int_t Int_Efficiency_Progression_B6 = int(Efficiency_Progression_B6*100);
                ostringstream convert_Progression_B6;
                convert_Progression_B6 << Int_Efficiency_Progression_B6 ;
                TString Result_Progression_B6 = TString(convert_Progression_B6.str());
                Int_t IsoCut_Progression_B6 = histosIsolation[Result_Progression_B6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_B6==100) IsoCut_Progression_B6 = 1000;
                LUT_Progression_B6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_B6);

                //Progression_C6
                Double_t Efficiency_Progression_C6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 90);
                if(Efficiency_Progression_C6>=0.9999) Efficiency_Progression_C6 = 1.0001;
                Int_t Int_Efficiency_Progression_C6 = int(Efficiency_Progression_C6*100);
                ostringstream convert_Progression_C6;
                convert_Progression_C6 << Int_Efficiency_Progression_C6 ;
                TString Result_Progression_C6 = TString(convert_Progression_C6.str());
                Int_t IsoCut_Progression_C6 = histosIsolation[Result_Progression_C6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_C6==100) IsoCut_Progression_C6 = 1000;
                LUT_Progression_C6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_C6);

                //Progression_D6
                Double_t Efficiency_Progression_D6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 93);
                if(Efficiency_Progression_D6>=0.9999) Efficiency_Progression_D6 = 1.0001;
                Int_t Int_Efficiency_Progression_D6 = int(Efficiency_Progression_D6*100);
                ostringstream convert_Progression_D6;
                convert_Progression_D6 << Int_Efficiency_Progression_D6 ;
                TString Result_Progression_D6 = TString(convert_Progression_D6.str());
                Int_t IsoCut_Progression_D6 = histosIsolation[Result_Progression_D6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_D6==100) IsoCut_Progression_D6 = 1000;
                LUT_Progression_D6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_D6);

                //Progression_E6
                Double_t Efficiency_Progression_E6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 95);
                if(Efficiency_Progression_E6>=0.9999) Efficiency_Progression_E6 = 1.0001;
                Int_t Int_Efficiency_Progression_E6 = int(Efficiency_Progression_E6*100);
                ostringstream convert_Progression_E6;
                convert_Progression_E6 << Int_Efficiency_Progression_E6 ;
                TString Result_Progression_E6 = TString(convert_Progression_E6.str());
                Int_t IsoCut_Progression_E6 = histosIsolation[Result_Progression_E6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_E6==100) IsoCut_Progression_E6 = 1000;
                LUT_Progression_E6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_E6);

                //Progression_F6
                Double_t Efficiency_Progression_F6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 34, effMin, 98);
                if(Efficiency_Progression_F6>=0.9999) Efficiency_Progression_F6 = 1.0001;
                Int_t Int_Efficiency_Progression_F6 = int(Efficiency_Progression_F6*100);
                ostringstream convert_Progression_F6;
                convert_Progression_F6 << Int_Efficiency_Progression_F6 ;
                TString Result_Progression_F6 = TString(convert_Progression_F6.str());
                Int_t IsoCut_Progression_F6 = histosIsolation[Result_Progression_F6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_F6==100) IsoCut_Progression_F6 = 1000;
                LUT_Progression_F6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_F6);

                //Progression_G6
                Double_t Efficiency_Progression_G6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 52);
                if(Efficiency_Progression_G6>=0.9999) Efficiency_Progression_G6 = 1.0001;
                Int_t Int_Efficiency_Progression_G6 = int(Efficiency_Progression_G6*100);
                ostringstream convert_Progression_G6;
                convert_Progression_G6 << Int_Efficiency_Progression_G6 ;
                TString Result_Progression_G6 = TString(convert_Progression_G6.str());
                Int_t IsoCut_Progression_G6 = histosIsolation[Result_Progression_G6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_G6==100) IsoCut_Progression_G6 = 1000;
                LUT_Progression_G6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_G6);

                //Progression_H6
                Double_t Efficiency_Progression_H6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 55);
                if(Efficiency_Progression_H6>=0.9999) Efficiency_Progression_H6 = 1.0001;
                Int_t Int_Efficiency_Progression_H6 = int(Efficiency_Progression_H6*100);
                ostringstream convert_Progression_H6;
                convert_Progression_H6 << Int_Efficiency_Progression_H6 ;
                TString Result_Progression_H6 = TString(convert_Progression_H6.str());
                Int_t IsoCut_Progression_H6 = histosIsolation[Result_Progression_H6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_H6==100) IsoCut_Progression_H6 = 1000;
                LUT_Progression_H6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_H6);

                //Progression_I6
                Double_t Efficiency_Progression_I6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 58);
                if(Efficiency_Progression_I6>=0.9999) Efficiency_Progression_I6 = 1.0001;
                Int_t Int_Efficiency_Progression_I6 = int(Efficiency_Progression_I6*100);
                ostringstream convert_Progression_I6;
                convert_Progression_I6 << Int_Efficiency_Progression_I6 ;
                TString Result_Progression_I6 = TString(convert_Progression_I6.str());
                Int_t IsoCut_Progression_I6 = histosIsolation[Result_Progression_I6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_I6==100) IsoCut_Progression_I6 = 1000;
                LUT_Progression_I6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_I6);

                //Progression_J6
                Double_t Efficiency_Progression_J6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 61);
                if(Efficiency_Progression_J6>=0.9999) Efficiency_Progression_J6 = 1.0001;
                Int_t Int_Efficiency_Progression_J6 = int(Efficiency_Progression_J6*100);
                ostringstream convert_Progression_J6;
                convert_Progression_J6 << Int_Efficiency_Progression_J6 ;
                TString Result_Progression_J6 = TString(convert_Progression_J6.str());
                Int_t IsoCut_Progression_J6 = histosIsolation[Result_Progression_J6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_J6==100) IsoCut_Progression_J6 = 1000;
                LUT_Progression_J6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_J6);

                //Progression_K6
                Double_t Efficiency_Progression_K6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 64);
                if(Efficiency_Progression_K6>=0.9999) Efficiency_Progression_K6 = 1.0001;
                Int_t Int_Efficiency_Progression_K6 = int(Efficiency_Progression_K6*100);
                ostringstream convert_Progression_K6;
                convert_Progression_K6 << Int_Efficiency_Progression_K6 ;
                TString Result_Progression_K6 = TString(convert_Progression_K6.str());
                Int_t IsoCut_Progression_K6 = histosIsolation[Result_Progression_K6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_K6==100) IsoCut_Progression_K6 = 1000;
                LUT_Progression_K6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_K6);

                //Progression_L6
                Double_t Efficiency_Progression_L6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 67);
                if(Efficiency_Progression_L6>=0.9999) Efficiency_Progression_L6 = 1.0001;
                Int_t Int_Efficiency_Progression_L6 = int(Efficiency_Progression_L6*100);
                ostringstream convert_Progression_L6;
                convert_Progression_L6 << Int_Efficiency_Progression_L6 ;
                TString Result_Progression_L6 = TString(convert_Progression_L6.str());
                Int_t IsoCut_Progression_L6 = histosIsolation[Result_Progression_L6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_L6==100) IsoCut_Progression_L6 = 1000;
                LUT_Progression_L6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_L6);

                //Progression_M6
                Double_t Efficiency_Progression_M6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 70);
                if(Efficiency_Progression_M6>=0.9999) Efficiency_Progression_M6 = 1.0001;
                Int_t Int_Efficiency_Progression_M6 = int(Efficiency_Progression_M6*100);
                ostringstream convert_Progression_M6;
                convert_Progression_M6 << Int_Efficiency_Progression_M6 ;
                TString Result_Progression_M6 = TString(convert_Progression_M6.str());
                Int_t IsoCut_Progression_M6 = histosIsolation[Result_Progression_M6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_M6==100) IsoCut_Progression_M6 = 1000;
                LUT_Progression_M6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_M6);

                //Progression_N6
                Double_t Efficiency_Progression_N6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 73);
                if(Efficiency_Progression_N6>=0.9999) Efficiency_Progression_N6 = 1.0001;
                Int_t Int_Efficiency_Progression_N6 = int(Efficiency_Progression_N6*100);
                ostringstream convert_Progression_N6;
                convert_Progression_N6 << Int_Efficiency_Progression_N6 ;
                TString Result_Progression_N6 = TString(convert_Progression_N6.str());
                Int_t IsoCut_Progression_N6 = histosIsolation[Result_Progression_N6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_N6==100) IsoCut_Progression_N6 = 1000;
                LUT_Progression_N6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_N6);

                //Progression_O6
                Double_t Efficiency_Progression_O6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 76);
                if(Efficiency_Progression_O6>=0.9999) Efficiency_Progression_O6 = 1.0001;
                Int_t Int_Efficiency_Progression_O6 = int(Efficiency_Progression_O6*100);
                ostringstream convert_Progression_O6;
                convert_Progression_O6 << Int_Efficiency_Progression_O6 ;
                TString Result_Progression_O6 = TString(convert_Progression_O6.str());
                Int_t IsoCut_Progression_O6 = histosIsolation[Result_Progression_O6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_O6==100) IsoCut_Progression_O6 = 1000;
                LUT_Progression_O6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_O6);

                //Progression_P6
                Double_t Efficiency_Progression_P6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 78);
                if(Efficiency_Progression_P6>=0.9999) Efficiency_Progression_P6 = 1.0001;
                Int_t Int_Efficiency_Progression_P6 = int(Efficiency_Progression_P6*100);
                ostringstream convert_Progression_P6;
                convert_Progression_P6 << Int_Efficiency_Progression_P6 ;
                TString Result_Progression_P6 = TString(convert_Progression_P6.str());
                Int_t IsoCut_Progression_P6 = histosIsolation[Result_Progression_P6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_P6==100) IsoCut_Progression_P6 = 1000;
                LUT_Progression_P6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_P6);

                //Progression_Q6
                Double_t Efficiency_Progression_Q6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 81);
                if(Efficiency_Progression_Q6>=0.9999) Efficiency_Progression_Q6 = 1.0001;
                Int_t Int_Efficiency_Progression_Q6 = int(Efficiency_Progression_Q6*100);
                ostringstream convert_Progression_Q6;
                convert_Progression_Q6 << Int_Efficiency_Progression_Q6 ;
                TString Result_Progression_Q6 = TString(convert_Progression_Q6.str());
                Int_t IsoCut_Progression_Q6 = histosIsolation[Result_Progression_Q6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Q6==100) IsoCut_Progression_Q6 = 1000;
                LUT_Progression_Q6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Q6);

                //Progression_R6
                Double_t Efficiency_Progression_R6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 84);
                if(Efficiency_Progression_R6>=0.9999) Efficiency_Progression_R6 = 1.0001;
                Int_t Int_Efficiency_Progression_R6 = int(Efficiency_Progression_R6*100);
                ostringstream convert_Progression_R6;
                convert_Progression_R6 << Int_Efficiency_Progression_R6 ;
                TString Result_Progression_R6 = TString(convert_Progression_R6.str());
                Int_t IsoCut_Progression_R6 = histosIsolation[Result_Progression_R6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_R6==100) IsoCut_Progression_R6 = 1000;
                LUT_Progression_R6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_R6);

                //Progression_S6
                Double_t Efficiency_Progression_S6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 87);
                if(Efficiency_Progression_S6>=0.9999) Efficiency_Progression_S6 = 1.0001;
                Int_t Int_Efficiency_Progression_S6 = int(Efficiency_Progression_S6*100);
                ostringstream convert_Progression_S6;
                convert_Progression_S6 << Int_Efficiency_Progression_S6 ;
                TString Result_Progression_S6 = TString(convert_Progression_S6.str());
                Int_t IsoCut_Progression_S6 = histosIsolation[Result_Progression_S6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_S6==100) IsoCut_Progression_S6 = 1000;
                LUT_Progression_S6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_S6);

                //Progression_T6
                Double_t Efficiency_Progression_T6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 90);
                if(Efficiency_Progression_T6>=0.9999) Efficiency_Progression_T6 = 1.0001;
                Int_t Int_Efficiency_Progression_T6 = int(Efficiency_Progression_T6*100);
                ostringstream convert_Progression_T6;
                convert_Progression_T6 << Int_Efficiency_Progression_T6 ;
                TString Result_Progression_T6 = TString(convert_Progression_T6.str());
                Int_t IsoCut_Progression_T6 = histosIsolation[Result_Progression_T6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_T6==100) IsoCut_Progression_T6 = 1000;
                LUT_Progression_T6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_T6);

                //Progression_U6
                Double_t Efficiency_Progression_U6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 93);
                if(Efficiency_Progression_U6>=0.9999) Efficiency_Progression_U6 = 1.0001;
                Int_t Int_Efficiency_Progression_U6 = int(Efficiency_Progression_U6*100);
                ostringstream convert_Progression_U6;
                convert_Progression_U6 << Int_Efficiency_Progression_U6 ;
                TString Result_Progression_U6 = TString(convert_Progression_U6.str());
                Int_t IsoCut_Progression_U6 = histosIsolation[Result_Progression_U6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_U6==100) IsoCut_Progression_U6 = 1000;
                LUT_Progression_U6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_U6);

                //Progression_V6
                Double_t Efficiency_Progression_V6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 96);
                if(Efficiency_Progression_V6>=0.9999) Efficiency_Progression_V6 = 1.0001;
                Int_t Int_Efficiency_Progression_V6 = int(Efficiency_Progression_V6*100);
                ostringstream convert_Progression_V6;
                convert_Progression_V6 << Int_Efficiency_Progression_V6 ;
                TString Result_Progression_V6 = TString(convert_Progression_V6.str());
                Int_t IsoCut_Progression_V6 = histosIsolation[Result_Progression_V6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_V6==100) IsoCut_Progression_V6 = 1000;
                LUT_Progression_V6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_V6);

                //Progression_W6
                Double_t Efficiency_Progression_W6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 98);
                if(Efficiency_Progression_W6>=0.9999) Efficiency_Progression_W6 = 1.0001;
                Int_t Int_Efficiency_Progression_W6 = int(Efficiency_Progression_W6*100);
                ostringstream convert_Progression_W6;
                convert_Progression_W6 << Int_Efficiency_Progression_W6 ;
                TString Result_Progression_W6 = TString(convert_Progression_W6.str());
                Int_t IsoCut_Progression_W6 = histosIsolation[Result_Progression_W6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_W6==100) IsoCut_Progression_W6 = 1000;
                LUT_Progression_W6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_W6);

                //Progression_X6
                Double_t Efficiency_Progression_X6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 37, effMin, 101);
                if(Efficiency_Progression_X6>=0.9999) Efficiency_Progression_X6 = 1.0001;
                Int_t Int_Efficiency_Progression_X6 = int(Efficiency_Progression_X6*100);
                ostringstream convert_Progression_X6;
                convert_Progression_X6 << Int_Efficiency_Progression_X6 ;
                TString Result_Progression_X6 = TString(convert_Progression_X6.str());
                Int_t IsoCut_Progression_X6 = histosIsolation[Result_Progression_X6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_X6==100) IsoCut_Progression_X6 = 1000;
                LUT_Progression_X6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_X6);

                //Progression_Y6
                Double_t Efficiency_Progression_Y6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 55);
                if(Efficiency_Progression_Y6>=0.9999) Efficiency_Progression_Y6 = 1.0001;
                Int_t Int_Efficiency_Progression_Y6 = int(Efficiency_Progression_Y6*100);
                ostringstream convert_Progression_Y6;
                convert_Progression_Y6 << Int_Efficiency_Progression_Y6 ;
                TString Result_Progression_Y6 = TString(convert_Progression_Y6.str());
                Int_t IsoCut_Progression_Y6 = histosIsolation[Result_Progression_Y6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Y6==100) IsoCut_Progression_Y6 = 1000;
                LUT_Progression_Y6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Y6);

                //Progression_Z6
                Double_t Efficiency_Progression_Z6 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 58);
                if(Efficiency_Progression_Z6>=0.9999) Efficiency_Progression_Z6 = 1.0001;
                Int_t Int_Efficiency_Progression_Z6 = int(Efficiency_Progression_Z6*100);
                ostringstream convert_Progression_Z6;
                convert_Progression_Z6 << Int_Efficiency_Progression_Z6 ;
                TString Result_Progression_Z6 = TString(convert_Progression_Z6.str());
                Int_t IsoCut_Progression_Z6 = histosIsolation[Result_Progression_Z6]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Z6==100) IsoCut_Progression_Z6 = 1000;
                LUT_Progression_Z6->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Z6);

                //Progression_A7
                Double_t Efficiency_Progression_A7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 61);
                if(Efficiency_Progression_A7>=0.9999) Efficiency_Progression_A7 = 1.0001;
                Int_t Int_Efficiency_Progression_A7 = int(Efficiency_Progression_A7*100);
                ostringstream convert_Progression_A7;
                convert_Progression_A7 << Int_Efficiency_Progression_A7 ;
                TString Result_Progression_A7 = TString(convert_Progression_A7.str());
                Int_t IsoCut_Progression_A7 = histosIsolation[Result_Progression_A7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_A7==100) IsoCut_Progression_A7 = 1000;
                LUT_Progression_A7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_A7);

                //Progression_B7
                Double_t Efficiency_Progression_B7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 64);
                if(Efficiency_Progression_B7>=0.9999) Efficiency_Progression_B7 = 1.0001;
                Int_t Int_Efficiency_Progression_B7 = int(Efficiency_Progression_B7*100);
                ostringstream convert_Progression_B7;
                convert_Progression_B7 << Int_Efficiency_Progression_B7 ;
                TString Result_Progression_B7 = TString(convert_Progression_B7.str());
                Int_t IsoCut_Progression_B7 = histosIsolation[Result_Progression_B7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_B7==100) IsoCut_Progression_B7 = 1000;
                LUT_Progression_B7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_B7);

                //Progression_C7
                Double_t Efficiency_Progression_C7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 67);
                if(Efficiency_Progression_C7>=0.9999) Efficiency_Progression_C7 = 1.0001;
                Int_t Int_Efficiency_Progression_C7 = int(Efficiency_Progression_C7*100);
                ostringstream convert_Progression_C7;
                convert_Progression_C7 << Int_Efficiency_Progression_C7 ;
                TString Result_Progression_C7 = TString(convert_Progression_C7.str());
                Int_t IsoCut_Progression_C7 = histosIsolation[Result_Progression_C7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_C7==100) IsoCut_Progression_C7 = 1000;
                LUT_Progression_C7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_C7);

                //Progression_D7
                Double_t Efficiency_Progression_D7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 70);
                if(Efficiency_Progression_D7>=0.9999) Efficiency_Progression_D7 = 1.0001;
                Int_t Int_Efficiency_Progression_D7 = int(Efficiency_Progression_D7*100);
                ostringstream convert_Progression_D7;
                convert_Progression_D7 << Int_Efficiency_Progression_D7 ;
                TString Result_Progression_D7 = TString(convert_Progression_D7.str());
                Int_t IsoCut_Progression_D7 = histosIsolation[Result_Progression_D7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_D7==100) IsoCut_Progression_D7 = 1000;
                LUT_Progression_D7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_D7);

                //Progression_E7
                Double_t Efficiency_Progression_E7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 73);
                if(Efficiency_Progression_E7>=0.9999) Efficiency_Progression_E7 = 1.0001;
                Int_t Int_Efficiency_Progression_E7 = int(Efficiency_Progression_E7*100);
                ostringstream convert_Progression_E7;
                convert_Progression_E7 << Int_Efficiency_Progression_E7 ;
                TString Result_Progression_E7 = TString(convert_Progression_E7.str());
                Int_t IsoCut_Progression_E7 = histosIsolation[Result_Progression_E7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_E7==100) IsoCut_Progression_E7 = 1000;
                LUT_Progression_E7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_E7);

                //Progression_F7
                Double_t Efficiency_Progression_F7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 76);
                if(Efficiency_Progression_F7>=0.9999) Efficiency_Progression_F7 = 1.0001;
                Int_t Int_Efficiency_Progression_F7 = int(Efficiency_Progression_F7*100);
                ostringstream convert_Progression_F7;
                convert_Progression_F7 << Int_Efficiency_Progression_F7 ;
                TString Result_Progression_F7 = TString(convert_Progression_F7.str());
                Int_t IsoCut_Progression_F7 = histosIsolation[Result_Progression_F7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_F7==100) IsoCut_Progression_F7 = 1000;
                LUT_Progression_F7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_F7);

                //Progression_G7
                Double_t Efficiency_Progression_G7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 79);
                if(Efficiency_Progression_G7>=0.9999) Efficiency_Progression_G7 = 1.0001;
                Int_t Int_Efficiency_Progression_G7 = int(Efficiency_Progression_G7*100);
                ostringstream convert_Progression_G7;
                convert_Progression_G7 << Int_Efficiency_Progression_G7 ;
                TString Result_Progression_G7 = TString(convert_Progression_G7.str());
                Int_t IsoCut_Progression_G7 = histosIsolation[Result_Progression_G7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_G7==100) IsoCut_Progression_G7 = 1000;
                LUT_Progression_G7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_G7);

                //Progression_H7
                Double_t Efficiency_Progression_H7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 81);
                if(Efficiency_Progression_H7>=0.9999) Efficiency_Progression_H7 = 1.0001;
                Int_t Int_Efficiency_Progression_H7 = int(Efficiency_Progression_H7*100);
                ostringstream convert_Progression_H7;
                convert_Progression_H7 << Int_Efficiency_Progression_H7 ;
                TString Result_Progression_H7 = TString(convert_Progression_H7.str());
                Int_t IsoCut_Progression_H7 = histosIsolation[Result_Progression_H7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_H7==100) IsoCut_Progression_H7 = 1000;
                LUT_Progression_H7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_H7);

                //Progression_I7
                Double_t Efficiency_Progression_I7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 84);
                if(Efficiency_Progression_I7>=0.9999) Efficiency_Progression_I7 = 1.0001;
                Int_t Int_Efficiency_Progression_I7 = int(Efficiency_Progression_I7*100);
                ostringstream convert_Progression_I7;
                convert_Progression_I7 << Int_Efficiency_Progression_I7 ;
                TString Result_Progression_I7 = TString(convert_Progression_I7.str());
                Int_t IsoCut_Progression_I7 = histosIsolation[Result_Progression_I7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_I7==100) IsoCut_Progression_I7 = 1000;
                LUT_Progression_I7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_I7);

                //Progression_J7
                Double_t Efficiency_Progression_J7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 87);
                if(Efficiency_Progression_J7>=0.9999) Efficiency_Progression_J7 = 1.0001;
                Int_t Int_Efficiency_Progression_J7 = int(Efficiency_Progression_J7*100);
                ostringstream convert_Progression_J7;
                convert_Progression_J7 << Int_Efficiency_Progression_J7 ;
                TString Result_Progression_J7 = TString(convert_Progression_J7.str());
                Int_t IsoCut_Progression_J7 = histosIsolation[Result_Progression_J7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_J7==100) IsoCut_Progression_J7 = 1000;
                LUT_Progression_J7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_J7);

                //Progression_K7
                Double_t Efficiency_Progression_K7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 90);
                if(Efficiency_Progression_K7>=0.9999) Efficiency_Progression_K7 = 1.0001;
                Int_t Int_Efficiency_Progression_K7 = int(Efficiency_Progression_K7*100);
                ostringstream convert_Progression_K7;
                convert_Progression_K7 << Int_Efficiency_Progression_K7 ;
                TString Result_Progression_K7 = TString(convert_Progression_K7.str());
                Int_t IsoCut_Progression_K7 = histosIsolation[Result_Progression_K7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_K7==100) IsoCut_Progression_K7 = 1000;
                LUT_Progression_K7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_K7);

                //Progression_L7
                Double_t Efficiency_Progression_L7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 93);
                if(Efficiency_Progression_L7>=0.9999) Efficiency_Progression_L7 = 1.0001;
                Int_t Int_Efficiency_Progression_L7 = int(Efficiency_Progression_L7*100);
                ostringstream convert_Progression_L7;
                convert_Progression_L7 << Int_Efficiency_Progression_L7 ;
                TString Result_Progression_L7 = TString(convert_Progression_L7.str());
                Int_t IsoCut_Progression_L7 = histosIsolation[Result_Progression_L7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_L7==100) IsoCut_Progression_L7 = 1000;
                LUT_Progression_L7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_L7);

                //Progression_M7
                Double_t Efficiency_Progression_M7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 96);
                if(Efficiency_Progression_M7>=0.9999) Efficiency_Progression_M7 = 1.0001;
                Int_t Int_Efficiency_Progression_M7 = int(Efficiency_Progression_M7*100);
                ostringstream convert_Progression_M7;
                convert_Progression_M7 << Int_Efficiency_Progression_M7 ;
                TString Result_Progression_M7 = TString(convert_Progression_M7.str());
                Int_t IsoCut_Progression_M7 = histosIsolation[Result_Progression_M7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_M7==100) IsoCut_Progression_M7 = 1000;
                LUT_Progression_M7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_M7);

                //Progression_N7
                Double_t Efficiency_Progression_N7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 99);
                if(Efficiency_Progression_N7>=0.9999) Efficiency_Progression_N7 = 1.0001;
                Int_t Int_Efficiency_Progression_N7 = int(Efficiency_Progression_N7*100);
                ostringstream convert_Progression_N7;
                convert_Progression_N7 << Int_Efficiency_Progression_N7 ;
                TString Result_Progression_N7 = TString(convert_Progression_N7.str());
                Int_t IsoCut_Progression_N7 = histosIsolation[Result_Progression_N7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_N7==100) IsoCut_Progression_N7 = 1000;
                LUT_Progression_N7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_N7);

                //Progression_O7
                Double_t Efficiency_Progression_O7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 101);
                if(Efficiency_Progression_O7>=0.9999) Efficiency_Progression_O7 = 1.0001;
                Int_t Int_Efficiency_Progression_O7 = int(Efficiency_Progression_O7*100);
                ostringstream convert_Progression_O7;
                convert_Progression_O7 << Int_Efficiency_Progression_O7 ;
                TString Result_Progression_O7 = TString(convert_Progression_O7.str());
                Int_t IsoCut_Progression_O7 = histosIsolation[Result_Progression_O7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_O7==100) IsoCut_Progression_O7 = 1000;
                LUT_Progression_O7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_O7);

                //Progression_P7
                Double_t Efficiency_Progression_P7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 40, effMin, 104);
                if(Efficiency_Progression_P7>=0.9999) Efficiency_Progression_P7 = 1.0001;
                Int_t Int_Efficiency_Progression_P7 = int(Efficiency_Progression_P7*100);
                ostringstream convert_Progression_P7;
                convert_Progression_P7 << Int_Efficiency_Progression_P7 ;
                TString Result_Progression_P7 = TString(convert_Progression_P7.str());
                Int_t IsoCut_Progression_P7 = histosIsolation[Result_Progression_P7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_P7==100) IsoCut_Progression_P7 = 1000;
                LUT_Progression_P7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_P7);

                //Progression_Q7
                Double_t Efficiency_Progression_Q7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 58);
                if(Efficiency_Progression_Q7>=0.9999) Efficiency_Progression_Q7 = 1.0001;
                Int_t Int_Efficiency_Progression_Q7 = int(Efficiency_Progression_Q7*100);
                ostringstream convert_Progression_Q7;
                convert_Progression_Q7 << Int_Efficiency_Progression_Q7 ;
                TString Result_Progression_Q7 = TString(convert_Progression_Q7.str());
                Int_t IsoCut_Progression_Q7 = histosIsolation[Result_Progression_Q7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Q7==100) IsoCut_Progression_Q7 = 1000;
                LUT_Progression_Q7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Q7);

                //Progression_R7
                Double_t Efficiency_Progression_R7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 61);
                if(Efficiency_Progression_R7>=0.9999) Efficiency_Progression_R7 = 1.0001;
                Int_t Int_Efficiency_Progression_R7 = int(Efficiency_Progression_R7*100);
                ostringstream convert_Progression_R7;
                convert_Progression_R7 << Int_Efficiency_Progression_R7 ;
                TString Result_Progression_R7 = TString(convert_Progression_R7.str());
                Int_t IsoCut_Progression_R7 = histosIsolation[Result_Progression_R7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_R7==100) IsoCut_Progression_R7 = 1000;
                LUT_Progression_R7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_R7);

                //Progression_S7
                Double_t Efficiency_Progression_S7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 64);
                if(Efficiency_Progression_S7>=0.9999) Efficiency_Progression_S7 = 1.0001;
                Int_t Int_Efficiency_Progression_S7 = int(Efficiency_Progression_S7*100);
                ostringstream convert_Progression_S7;
                convert_Progression_S7 << Int_Efficiency_Progression_S7 ;
                TString Result_Progression_S7 = TString(convert_Progression_S7.str());
                Int_t IsoCut_Progression_S7 = histosIsolation[Result_Progression_S7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_S7==100) IsoCut_Progression_S7 = 1000;
                LUT_Progression_S7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_S7);

                //Progression_T7
                Double_t Efficiency_Progression_T7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 67);
                if(Efficiency_Progression_T7>=0.9999) Efficiency_Progression_T7 = 1.0001;
                Int_t Int_Efficiency_Progression_T7 = int(Efficiency_Progression_T7*100);
                ostringstream convert_Progression_T7;
                convert_Progression_T7 << Int_Efficiency_Progression_T7 ;
                TString Result_Progression_T7 = TString(convert_Progression_T7.str());
                Int_t IsoCut_Progression_T7 = histosIsolation[Result_Progression_T7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_T7==100) IsoCut_Progression_T7 = 1000;
                LUT_Progression_T7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_T7);

                //Progression_U7
                Double_t Efficiency_Progression_U7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 70);
                if(Efficiency_Progression_U7>=0.9999) Efficiency_Progression_U7 = 1.0001;
                Int_t Int_Efficiency_Progression_U7 = int(Efficiency_Progression_U7*100);
                ostringstream convert_Progression_U7;
                convert_Progression_U7 << Int_Efficiency_Progression_U7 ;
                TString Result_Progression_U7 = TString(convert_Progression_U7.str());
                Int_t IsoCut_Progression_U7 = histosIsolation[Result_Progression_U7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_U7==100) IsoCut_Progression_U7 = 1000;
                LUT_Progression_U7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_U7);

                //Progression_V7
                Double_t Efficiency_Progression_V7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 73);
                if(Efficiency_Progression_V7>=0.9999) Efficiency_Progression_V7 = 1.0001;
                Int_t Int_Efficiency_Progression_V7 = int(Efficiency_Progression_V7*100);
                ostringstream convert_Progression_V7;
                convert_Progression_V7 << Int_Efficiency_Progression_V7 ;
                TString Result_Progression_V7 = TString(convert_Progression_V7.str());
                Int_t IsoCut_Progression_V7 = histosIsolation[Result_Progression_V7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_V7==100) IsoCut_Progression_V7 = 1000;
                LUT_Progression_V7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_V7);

                //Progression_W7
                Double_t Efficiency_Progression_W7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 76);
                if(Efficiency_Progression_W7>=0.9999) Efficiency_Progression_W7 = 1.0001;
                Int_t Int_Efficiency_Progression_W7 = int(Efficiency_Progression_W7*100);
                ostringstream convert_Progression_W7;
                convert_Progression_W7 << Int_Efficiency_Progression_W7 ;
                TString Result_Progression_W7 = TString(convert_Progression_W7.str());
                Int_t IsoCut_Progression_W7 = histosIsolation[Result_Progression_W7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_W7==100) IsoCut_Progression_W7 = 1000;
                LUT_Progression_W7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_W7);

                //Progression_X7
                Double_t Efficiency_Progression_X7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 79);
                if(Efficiency_Progression_X7>=0.9999) Efficiency_Progression_X7 = 1.0001;
                Int_t Int_Efficiency_Progression_X7 = int(Efficiency_Progression_X7*100);
                ostringstream convert_Progression_X7;
                convert_Progression_X7 << Int_Efficiency_Progression_X7 ;
                TString Result_Progression_X7 = TString(convert_Progression_X7.str());
                Int_t IsoCut_Progression_X7 = histosIsolation[Result_Progression_X7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_X7==100) IsoCut_Progression_X7 = 1000;
                LUT_Progression_X7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_X7);

                //Progression_Y7
                Double_t Efficiency_Progression_Y7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 82);
                if(Efficiency_Progression_Y7>=0.9999) Efficiency_Progression_Y7 = 1.0001;
                Int_t Int_Efficiency_Progression_Y7 = int(Efficiency_Progression_Y7*100);
                ostringstream convert_Progression_Y7;
                convert_Progression_Y7 << Int_Efficiency_Progression_Y7 ;
                TString Result_Progression_Y7 = TString(convert_Progression_Y7.str());
                Int_t IsoCut_Progression_Y7 = histosIsolation[Result_Progression_Y7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Y7==100) IsoCut_Progression_Y7 = 1000;
                LUT_Progression_Y7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Y7);

                //Progression_Z7
                Double_t Efficiency_Progression_Z7 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 84);
                if(Efficiency_Progression_Z7>=0.9999) Efficiency_Progression_Z7 = 1.0001;
                Int_t Int_Efficiency_Progression_Z7 = int(Efficiency_Progression_Z7*100);
                ostringstream convert_Progression_Z7;
                convert_Progression_Z7 << Int_Efficiency_Progression_Z7 ;
                TString Result_Progression_Z7 = TString(convert_Progression_Z7.str());
                Int_t IsoCut_Progression_Z7 = histosIsolation[Result_Progression_Z7]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Z7==100) IsoCut_Progression_Z7 = 1000;
                LUT_Progression_Z7->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Z7);

                //Progression_A8
                Double_t Efficiency_Progression_A8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 87);
                if(Efficiency_Progression_A8>=0.9999) Efficiency_Progression_A8 = 1.0001;
                Int_t Int_Efficiency_Progression_A8 = int(Efficiency_Progression_A8*100);
                ostringstream convert_Progression_A8;
                convert_Progression_A8 << Int_Efficiency_Progression_A8 ;
                TString Result_Progression_A8 = TString(convert_Progression_A8.str());
                Int_t IsoCut_Progression_A8 = histosIsolation[Result_Progression_A8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_A8==100) IsoCut_Progression_A8 = 1000;
                LUT_Progression_A8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_A8);

                //Progression_B8
                Double_t Efficiency_Progression_B8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 90);
                if(Efficiency_Progression_B8>=0.9999) Efficiency_Progression_B8 = 1.0001;
                Int_t Int_Efficiency_Progression_B8 = int(Efficiency_Progression_B8*100);
                ostringstream convert_Progression_B8;
                convert_Progression_B8 << Int_Efficiency_Progression_B8 ;
                TString Result_Progression_B8 = TString(convert_Progression_B8.str());
                Int_t IsoCut_Progression_B8 = histosIsolation[Result_Progression_B8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_B8==100) IsoCut_Progression_B8 = 1000;
                LUT_Progression_B8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_B8);

                //Progression_C8
                Double_t Efficiency_Progression_C8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 93);
                if(Efficiency_Progression_C8>=0.9999) Efficiency_Progression_C8 = 1.0001;
                Int_t Int_Efficiency_Progression_C8 = int(Efficiency_Progression_C8*100);
                ostringstream convert_Progression_C8;
                convert_Progression_C8 << Int_Efficiency_Progression_C8 ;
                TString Result_Progression_C8 = TString(convert_Progression_C8.str());
                Int_t IsoCut_Progression_C8 = histosIsolation[Result_Progression_C8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_C8==100) IsoCut_Progression_C8 = 1000;
                LUT_Progression_C8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_C8);

                //Progression_D8
                Double_t Efficiency_Progression_D8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 96);
                if(Efficiency_Progression_D8>=0.9999) Efficiency_Progression_D8 = 1.0001;
                Int_t Int_Efficiency_Progression_D8 = int(Efficiency_Progression_D8*100);
                ostringstream convert_Progression_D8;
                convert_Progression_D8 << Int_Efficiency_Progression_D8 ;
                TString Result_Progression_D8 = TString(convert_Progression_D8.str());
                Int_t IsoCut_Progression_D8 = histosIsolation[Result_Progression_D8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_D8==100) IsoCut_Progression_D8 = 1000;
                LUT_Progression_D8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_D8);

                //Progression_E8
                Double_t Efficiency_Progression_E8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 99);
                if(Efficiency_Progression_E8>=0.9999) Efficiency_Progression_E8 = 1.0001;
                Int_t Int_Efficiency_Progression_E8 = int(Efficiency_Progression_E8*100);
                ostringstream convert_Progression_E8;
                convert_Progression_E8 << Int_Efficiency_Progression_E8 ;
                TString Result_Progression_E8 = TString(convert_Progression_E8.str());
                Int_t IsoCut_Progression_E8 = histosIsolation[Result_Progression_E8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_E8==100) IsoCut_Progression_E8 = 1000;
                LUT_Progression_E8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_E8);

                //Progression_F8
                Double_t Efficiency_Progression_F8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 102);
                if(Efficiency_Progression_F8>=0.9999) Efficiency_Progression_F8 = 1.0001;
                Int_t Int_Efficiency_Progression_F8 = int(Efficiency_Progression_F8*100);
                ostringstream convert_Progression_F8;
                convert_Progression_F8 << Int_Efficiency_Progression_F8 ;
                TString Result_Progression_F8 = TString(convert_Progression_F8.str());
                Int_t IsoCut_Progression_F8 = histosIsolation[Result_Progression_F8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_F8==100) IsoCut_Progression_F8 = 1000;
                LUT_Progression_F8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_F8);

                //Progression_G8
                Double_t Efficiency_Progression_G8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 104);
                if(Efficiency_Progression_G8>=0.9999) Efficiency_Progression_G8 = 1.0001;
                Int_t Int_Efficiency_Progression_G8 = int(Efficiency_Progression_G8*100);
                ostringstream convert_Progression_G8;
                convert_Progression_G8 << Int_Efficiency_Progression_G8 ;
                TString Result_Progression_G8 = TString(convert_Progression_G8.str());
                Int_t IsoCut_Progression_G8 = histosIsolation[Result_Progression_G8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_G8==100) IsoCut_Progression_G8 = 1000;
                LUT_Progression_G8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_G8);

                //Progression_H8
                Double_t Efficiency_Progression_H8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 43, effMin, 107);
                if(Efficiency_Progression_H8>=0.9999) Efficiency_Progression_H8 = 1.0001;
                Int_t Int_Efficiency_Progression_H8 = int(Efficiency_Progression_H8*100);
                ostringstream convert_Progression_H8;
                convert_Progression_H8 << Int_Efficiency_Progression_H8 ;
                TString Result_Progression_H8 = TString(convert_Progression_H8.str());
                Int_t IsoCut_Progression_H8 = histosIsolation[Result_Progression_H8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_H8==100) IsoCut_Progression_H8 = 1000;
                LUT_Progression_H8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_H8);

                //Progression_I8
                Double_t Efficiency_Progression_I8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 61);
                if(Efficiency_Progression_I8>=0.9999) Efficiency_Progression_I8 = 1.0001;
                Int_t Int_Efficiency_Progression_I8 = int(Efficiency_Progression_I8*100);
                ostringstream convert_Progression_I8;
                convert_Progression_I8 << Int_Efficiency_Progression_I8 ;
                TString Result_Progression_I8 = TString(convert_Progression_I8.str());
                Int_t IsoCut_Progression_I8 = histosIsolation[Result_Progression_I8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_I8==100) IsoCut_Progression_I8 = 1000;
                LUT_Progression_I8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_I8);

                //Progression_J8
                Double_t Efficiency_Progression_J8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 64);
                if(Efficiency_Progression_J8>=0.9999) Efficiency_Progression_J8 = 1.0001;
                Int_t Int_Efficiency_Progression_J8 = int(Efficiency_Progression_J8*100);
                ostringstream convert_Progression_J8;
                convert_Progression_J8 << Int_Efficiency_Progression_J8 ;
                TString Result_Progression_J8 = TString(convert_Progression_J8.str());
                Int_t IsoCut_Progression_J8 = histosIsolation[Result_Progression_J8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_J8==100) IsoCut_Progression_J8 = 1000;
                LUT_Progression_J8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_J8);

                //Progression_K8
                Double_t Efficiency_Progression_K8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 67);
                if(Efficiency_Progression_K8>=0.9999) Efficiency_Progression_K8 = 1.0001;
                Int_t Int_Efficiency_Progression_K8 = int(Efficiency_Progression_K8*100);
                ostringstream convert_Progression_K8;
                convert_Progression_K8 << Int_Efficiency_Progression_K8 ;
                TString Result_Progression_K8 = TString(convert_Progression_K8.str());
                Int_t IsoCut_Progression_K8 = histosIsolation[Result_Progression_K8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_K8==100) IsoCut_Progression_K8 = 1000;
                LUT_Progression_K8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_K8);

                //Progression_L8
                Double_t Efficiency_Progression_L8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 70);
                if(Efficiency_Progression_L8>=0.9999) Efficiency_Progression_L8 = 1.0001;
                Int_t Int_Efficiency_Progression_L8 = int(Efficiency_Progression_L8*100);
                ostringstream convert_Progression_L8;
                convert_Progression_L8 << Int_Efficiency_Progression_L8 ;
                TString Result_Progression_L8 = TString(convert_Progression_L8.str());
                Int_t IsoCut_Progression_L8 = histosIsolation[Result_Progression_L8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_L8==100) IsoCut_Progression_L8 = 1000;
                LUT_Progression_L8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_L8);

                //Progression_M8
                Double_t Efficiency_Progression_M8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 73);
                if(Efficiency_Progression_M8>=0.9999) Efficiency_Progression_M8 = 1.0001;
                Int_t Int_Efficiency_Progression_M8 = int(Efficiency_Progression_M8*100);
                ostringstream convert_Progression_M8;
                convert_Progression_M8 << Int_Efficiency_Progression_M8 ;
                TString Result_Progression_M8 = TString(convert_Progression_M8.str());
                Int_t IsoCut_Progression_M8 = histosIsolation[Result_Progression_M8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_M8==100) IsoCut_Progression_M8 = 1000;
                LUT_Progression_M8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_M8);

                //Progression_N8
                Double_t Efficiency_Progression_N8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 76);
                if(Efficiency_Progression_N8>=0.9999) Efficiency_Progression_N8 = 1.0001;
                Int_t Int_Efficiency_Progression_N8 = int(Efficiency_Progression_N8*100);
                ostringstream convert_Progression_N8;
                convert_Progression_N8 << Int_Efficiency_Progression_N8 ;
                TString Result_Progression_N8 = TString(convert_Progression_N8.str());
                Int_t IsoCut_Progression_N8 = histosIsolation[Result_Progression_N8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_N8==100) IsoCut_Progression_N8 = 1000;
                LUT_Progression_N8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_N8);

                //Progression_O8
                Double_t Efficiency_Progression_O8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 79);
                if(Efficiency_Progression_O8>=0.9999) Efficiency_Progression_O8 = 1.0001;
                Int_t Int_Efficiency_Progression_O8 = int(Efficiency_Progression_O8*100);
                ostringstream convert_Progression_O8;
                convert_Progression_O8 << Int_Efficiency_Progression_O8 ;
                TString Result_Progression_O8 = TString(convert_Progression_O8.str());
                Int_t IsoCut_Progression_O8 = histosIsolation[Result_Progression_O8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_O8==100) IsoCut_Progression_O8 = 1000;
                LUT_Progression_O8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_O8);

                //Progression_P8
                Double_t Efficiency_Progression_P8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 82);
                if(Efficiency_Progression_P8>=0.9999) Efficiency_Progression_P8 = 1.0001;
                Int_t Int_Efficiency_Progression_P8 = int(Efficiency_Progression_P8*100);
                ostringstream convert_Progression_P8;
                convert_Progression_P8 << Int_Efficiency_Progression_P8 ;
                TString Result_Progression_P8 = TString(convert_Progression_P8.str());
                Int_t IsoCut_Progression_P8 = histosIsolation[Result_Progression_P8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_P8==100) IsoCut_Progression_P8 = 1000;
                LUT_Progression_P8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_P8);

                //Progression_Q8
                Double_t Efficiency_Progression_Q8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 85);
                if(Efficiency_Progression_Q8>=0.9999) Efficiency_Progression_Q8 = 1.0001;
                Int_t Int_Efficiency_Progression_Q8 = int(Efficiency_Progression_Q8*100);
                ostringstream convert_Progression_Q8;
                convert_Progression_Q8 << Int_Efficiency_Progression_Q8 ;
                TString Result_Progression_Q8 = TString(convert_Progression_Q8.str());
                Int_t IsoCut_Progression_Q8 = histosIsolation[Result_Progression_Q8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Q8==100) IsoCut_Progression_Q8 = 1000;
                LUT_Progression_Q8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Q8);

                //Progression_R8
                Double_t Efficiency_Progression_R8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 87);
                if(Efficiency_Progression_R8>=0.9999) Efficiency_Progression_R8 = 1.0001;
                Int_t Int_Efficiency_Progression_R8 = int(Efficiency_Progression_R8*100);
                ostringstream convert_Progression_R8;
                convert_Progression_R8 << Int_Efficiency_Progression_R8 ;
                TString Result_Progression_R8 = TString(convert_Progression_R8.str());
                Int_t IsoCut_Progression_R8 = histosIsolation[Result_Progression_R8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_R8==100) IsoCut_Progression_R8 = 1000;
                LUT_Progression_R8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_R8);

                //Progression_S8
                Double_t Efficiency_Progression_S8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 90);
                if(Efficiency_Progression_S8>=0.9999) Efficiency_Progression_S8 = 1.0001;
                Int_t Int_Efficiency_Progression_S8 = int(Efficiency_Progression_S8*100);
                ostringstream convert_Progression_S8;
                convert_Progression_S8 << Int_Efficiency_Progression_S8 ;
                TString Result_Progression_S8 = TString(convert_Progression_S8.str());
                Int_t IsoCut_Progression_S8 = histosIsolation[Result_Progression_S8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_S8==100) IsoCut_Progression_S8 = 1000;
                LUT_Progression_S8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_S8);

                //Progression_T8
                Double_t Efficiency_Progression_T8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 93);
                if(Efficiency_Progression_T8>=0.9999) Efficiency_Progression_T8 = 1.0001;
                Int_t Int_Efficiency_Progression_T8 = int(Efficiency_Progression_T8*100);
                ostringstream convert_Progression_T8;
                convert_Progression_T8 << Int_Efficiency_Progression_T8 ;
                TString Result_Progression_T8 = TString(convert_Progression_T8.str());
                Int_t IsoCut_Progression_T8 = histosIsolation[Result_Progression_T8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_T8==100) IsoCut_Progression_T8 = 1000;
                LUT_Progression_T8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_T8);

                //Progression_U8
                Double_t Efficiency_Progression_U8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 96);
                if(Efficiency_Progression_U8>=0.9999) Efficiency_Progression_U8 = 1.0001;
                Int_t Int_Efficiency_Progression_U8 = int(Efficiency_Progression_U8*100);
                ostringstream convert_Progression_U8;
                convert_Progression_U8 << Int_Efficiency_Progression_U8 ;
                TString Result_Progression_U8 = TString(convert_Progression_U8.str());
                Int_t IsoCut_Progression_U8 = histosIsolation[Result_Progression_U8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_U8==100) IsoCut_Progression_U8 = 1000;
                LUT_Progression_U8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_U8);

                //Progression_V8
                Double_t Efficiency_Progression_V8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 99);
                if(Efficiency_Progression_V8>=0.9999) Efficiency_Progression_V8 = 1.0001;
                Int_t Int_Efficiency_Progression_V8 = int(Efficiency_Progression_V8*100);
                ostringstream convert_Progression_V8;
                convert_Progression_V8 << Int_Efficiency_Progression_V8 ;
                TString Result_Progression_V8 = TString(convert_Progression_V8.str());
                Int_t IsoCut_Progression_V8 = histosIsolation[Result_Progression_V8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_V8==100) IsoCut_Progression_V8 = 1000;
                LUT_Progression_V8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_V8);

                //Progression_W8
                Double_t Efficiency_Progression_W8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 102);
                if(Efficiency_Progression_W8>=0.9999) Efficiency_Progression_W8 = 1.0001;
                Int_t Int_Efficiency_Progression_W8 = int(Efficiency_Progression_W8*100);
                ostringstream convert_Progression_W8;
                convert_Progression_W8 << Int_Efficiency_Progression_W8 ;
                TString Result_Progression_W8 = TString(convert_Progression_W8.str());
                Int_t IsoCut_Progression_W8 = histosIsolation[Result_Progression_W8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_W8==100) IsoCut_Progression_W8 = 1000;
                LUT_Progression_W8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_W8);

                //Progression_X8
                Double_t Efficiency_Progression_X8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 105);
                if(Efficiency_Progression_X8>=0.9999) Efficiency_Progression_X8 = 1.0001;
                Int_t Int_Efficiency_Progression_X8 = int(Efficiency_Progression_X8*100);
                ostringstream convert_Progression_X8;
                convert_Progression_X8 << Int_Efficiency_Progression_X8 ;
                TString Result_Progression_X8 = TString(convert_Progression_X8.str());
                Int_t IsoCut_Progression_X8 = histosIsolation[Result_Progression_X8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_X8==100) IsoCut_Progression_X8 = 1000;
                LUT_Progression_X8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_X8);

                //Progression_Y8
                Double_t Efficiency_Progression_Y8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 107);
                if(Efficiency_Progression_Y8>=0.9999) Efficiency_Progression_Y8 = 1.0001;
                Int_t Int_Efficiency_Progression_Y8 = int(Efficiency_Progression_Y8*100);
                ostringstream convert_Progression_Y8;
                convert_Progression_Y8 << Int_Efficiency_Progression_Y8 ;
                TString Result_Progression_Y8 = TString(convert_Progression_Y8.str());
                Int_t IsoCut_Progression_Y8 = histosIsolation[Result_Progression_Y8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Y8==100) IsoCut_Progression_Y8 = 1000;
                LUT_Progression_Y8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Y8);

                //Progression_Z8
                Double_t Efficiency_Progression_Z8 = FindEfficiency_Progression((hardcodedIetBins2[j]+hardcodedIetBins2[j+1])/2., 46, effMin, 110);
                if(Efficiency_Progression_Z8>=0.9999) Efficiency_Progression_Z8 = 1.0001;
                Int_t Int_Efficiency_Progression_Z8 = int(Efficiency_Progression_Z8*100);
                ostringstream convert_Progression_Z8;
                convert_Progression_Z8 << Int_Efficiency_Progression_Z8 ;
                TString Result_Progression_Z8 = TString(convert_Progression_Z8.str());
                Int_t IsoCut_Progression_Z8 = histosIsolation[Result_Progression_Z8]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                if(Int_Efficiency_Progression_Z8==100) IsoCut_Progression_Z8 = 1000;
                LUT_Progression_Z8->SetBinContent(i+1,j+1,k+1,IsoCut_Progression_Z8);

                // END OF FULL BLOWN GRID SEARCH
                //***************************************************************************************************************************************************************************************************************************************************************************
                //***************************************************************************************************************************************************************************************************************************************************************************

                // LUT_WP
                for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
                {
                    //        cout<<"iEff = "<<iEff<<endl;
                    Double_t Efficiency = iEff/100.;
                    if(Efficiency>=0.9999) Efficiency = 1.0001;
                    Int_t Int_Efficiency = int(Efficiency*100);
                    ostringstream convert;
                    convert << Int_Efficiency ;
                    TString Result = TString(convert.str());
                    //        cout<<"Result = "<<Result<<endl;
                    Int_t IsoCut = histosIsolation[Result]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,FindBinCorrespondencenTT(hardcodednTTBins2[k])+1);
                    if(Int_Efficiency==100) IsoCut = 1000;
                    //        cout<<"IsoCut = "<<IsoCut<<endl;
                    LUT_WP.at(iEff)->SetBinContent(i+1,j+1,k+1,IsoCut);
                    //        cout<<"after at.iEff"<<endl;
                }

                /*
                //WP90
                Double_t Efficiency_WP90 = 0.9;
                if(Efficiency_WP90>=0.9999) Efficiency_WP90 = 1.0001;
                Int_t Int_Efficiency_WP90 = int(Efficiency_WP90*100);
                ostringstream convert_WP90;
                convert_WP90 << Int_Efficiency_WP90 ;
                TString Result_WP90 = TString(convert_WP90.str());
                Int_t IsoCut_WP90 = histosIsolation[Result_WP90]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,k+1);
                if(Int_Efficiency_WP90==100) IsoCut_WP90 = 1000;
                LUT_WP90->SetBinContent(i+1,j+1,k+1,IsoCut_WP90);

                //WP80
                Double_t Efficiency_WP80 = 0.8;
                if(Efficiency_WP80>=0.9999) Efficiency_WP80 = 1.0001;
                Int_t Int_Efficiency_WP80 = int(Efficiency_WP80*100);
                ostringstream convert_WP80;
                convert_WP80 << Int_Efficiency_WP80 ;
                TString Result_WP80 = TString(convert_WP80.str());
                Int_t IsoCut_WP80 = histosIsolation[Result_WP80]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,k+1);
                if(Int_Efficiency_WP80==100) IsoCut_WP80 = 1000;
                LUT_WP80->SetBinContent(i+1,j+1,k+1,IsoCut_WP80);

                //WP70
                Double_t Efficiency_WP70 = 0.7;
                if(Efficiency_WP70>=0.9999) Efficiency_WP70 = 1.0001;
                Int_t Int_Efficiency_WP70 = int(Efficiency_WP70*100);
                ostringstream convert_WP70;
                convert_WP70 << Int_Efficiency_WP70 ;
                TString Result_WP70 = TString(convert_WP70.str());
                Int_t IsoCut_WP70 = histosIsolation[Result_WP70]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedIetBins2[j])+1,k+1);
                if(Int_Efficiency_WP70==100) IsoCut_WP70 = 1000;
                LUT_WP70->SetBinContent(i+1,j+1,k+1,IsoCut_WP70);
                */
            }
        }
    }


    LUT_Progression_1->Write();
    LUT_Progression_2->Write();
    LUT_Progression_3->Write();
    LUT_Progression_4->Write();
    LUT_Progression_5->Write();
    LUT_Progression_6->Write();
    LUT_Progression_7->Write();
    LUT_Progression_8->Write();
    LUT_Progression_9->Write();
    LUT_Progression_10->Write();
    LUT_Progression_11->Write();
    LUT_Progression_12->Write();
    LUT_Progression_13->Write();
    LUT_Progression_14->Write();
    LUT_Progression_15->Write();
    LUT_Progression_16->Write();
    LUT_Progression_17->Write();
    LUT_Progression_18->Write();
    LUT_Progression_19->Write();
    LUT_Progression_20->Write();
    LUT_Progression_21->Write();
    LUT_Progression_22->Write();
    LUT_Progression_23->Write();
    LUT_Progression_24->Write();
    LUT_Progression_25->Write();
    LUT_Progression_26->Write();
    LUT_Progression_27->Write();
    LUT_Progression_28->Write();
    LUT_Progression_29->Write();
    LUT_Progression_30->Write();
    // LUT_Progression_31->Write();


    for(Int_t i = 0 ; i < NbinsIEta-1 ; ++i)
        {
            for(Int_t j = 1 ; j < NbinsIEt2-1 ; ++j)
    {
        for(Int_t k = 0 ; k < NbinsnTT2-1 ; ++k)
        // for(Int_t k = 0 ; k < NbinsnTT-1 ; ++k)
            {
                if(LUT_Progression_31_extrap->GetBinContent(i+1,j+1,k+1)<LUT_Progression_31_extrap->GetBinContent(i+1,j,k+1))
        {
            LUT_Progression_31_extrap->SetBinContent(i+1,j+1,k+1,LUT_Progression_31_extrap->GetBinContent(i+1,j,k+1));
        }
                if(LUT_Progression_31->GetBinContent(i+1,j+1,k+1)<LUT_Progression_31->GetBinContent(i+1,j,k+1))
        {
            LUT_Progression_31->SetBinContent(i+1,j+1,k+1,LUT_Progression_31->GetBinContent(i+1,j,k+1));
        }
            }

    }
            
        }
    LUT_Progression_31->Write();
    LUT_Progression_31_extrap->Write();

    for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
        {
            LUT_WP.at(iEff)->Write();
        }

    // LUT_WP90->Write();
    // LUT_WP80->Write();
    // LUT_WP70->Write();

    // START OF FULL BLOWN GRID SEARCH
    LUT_Progression_A0->Write();
    LUT_Progression_B0->Write();
    LUT_Progression_C0->Write();
    LUT_Progression_D0->Write();
    LUT_Progression_E0->Write();
    LUT_Progression_F0->Write();
    LUT_Progression_G0->Write();
    LUT_Progression_H0->Write();
    LUT_Progression_I0->Write();
    LUT_Progression_J0->Write();
    LUT_Progression_K0->Write();
    LUT_Progression_L0->Write();
    LUT_Progression_M0->Write();
    LUT_Progression_N0->Write();
    LUT_Progression_O0->Write();
    LUT_Progression_P0->Write();
    LUT_Progression_Q0->Write();
    LUT_Progression_R0->Write();
    LUT_Progression_S0->Write();
    LUT_Progression_T0->Write();
    LUT_Progression_U0->Write();
    LUT_Progression_V0->Write();
    LUT_Progression_W0->Write();
    LUT_Progression_X0->Write();
    LUT_Progression_Y0->Write();
    LUT_Progression_Z0->Write();
    LUT_Progression_A1->Write();
    LUT_Progression_B1->Write();
    LUT_Progression_C1->Write();
    LUT_Progression_D1->Write();
    LUT_Progression_E1->Write();
    LUT_Progression_F1->Write();
    LUT_Progression_G1->Write();
    LUT_Progression_H1->Write();
    LUT_Progression_I1->Write();
    LUT_Progression_J1->Write();
    LUT_Progression_K1->Write();
    LUT_Progression_L1->Write();
    LUT_Progression_M1->Write();
    LUT_Progression_N1->Write();
    LUT_Progression_O1->Write();
    LUT_Progression_P1->Write();
    LUT_Progression_Q1->Write();
    LUT_Progression_R1->Write();
    LUT_Progression_S1->Write();
    LUT_Progression_T1->Write();
    LUT_Progression_U1->Write();
    LUT_Progression_V1->Write();
    LUT_Progression_W1->Write();
    LUT_Progression_X1->Write();
    LUT_Progression_Y1->Write();
    LUT_Progression_Z1->Write();
    LUT_Progression_A2->Write();
    LUT_Progression_B2->Write();
    LUT_Progression_C2->Write();
    LUT_Progression_D2->Write();
    LUT_Progression_E2->Write();
    LUT_Progression_F2->Write();
    LUT_Progression_G2->Write();
    LUT_Progression_H2->Write();
    LUT_Progression_I2->Write();
    LUT_Progression_J2->Write();
    LUT_Progression_K2->Write();
    LUT_Progression_L2->Write();
    LUT_Progression_M2->Write();
    LUT_Progression_N2->Write();
    LUT_Progression_O2->Write();
    LUT_Progression_P2->Write();
    LUT_Progression_Q2->Write();
    LUT_Progression_R2->Write();
    LUT_Progression_S2->Write();
    LUT_Progression_T2->Write();
    LUT_Progression_U2->Write();
    LUT_Progression_V2->Write();
    LUT_Progression_W2->Write();
    LUT_Progression_X2->Write();
    LUT_Progression_Y2->Write();
    LUT_Progression_Z2->Write();
    LUT_Progression_A3->Write();
    LUT_Progression_B3->Write();
    LUT_Progression_C3->Write();
    LUT_Progression_D3->Write();
    LUT_Progression_E3->Write();
    LUT_Progression_F3->Write();
    LUT_Progression_G3->Write();
    LUT_Progression_H3->Write();
    LUT_Progression_I3->Write();
    LUT_Progression_J3->Write();
    LUT_Progression_K3->Write();
    LUT_Progression_L3->Write();
    LUT_Progression_M3->Write();
    LUT_Progression_N3->Write();
    LUT_Progression_O3->Write();
    LUT_Progression_P3->Write();
    LUT_Progression_Q3->Write();
    LUT_Progression_R3->Write();
    LUT_Progression_S3->Write();
    LUT_Progression_T3->Write();
    LUT_Progression_U3->Write();
    LUT_Progression_V3->Write();
    LUT_Progression_W3->Write();
    LUT_Progression_X3->Write();
    LUT_Progression_Y3->Write();
    LUT_Progression_Z3->Write();
    LUT_Progression_A4->Write();
    LUT_Progression_B4->Write();
    LUT_Progression_C4->Write();
    LUT_Progression_D4->Write();
    LUT_Progression_E4->Write();
    LUT_Progression_F4->Write();
    LUT_Progression_G4->Write();
    LUT_Progression_H4->Write();
    LUT_Progression_I4->Write();
    LUT_Progression_J4->Write();
    LUT_Progression_K4->Write();
    LUT_Progression_L4->Write();
    LUT_Progression_M4->Write();
    LUT_Progression_N4->Write();
    LUT_Progression_O4->Write();
    LUT_Progression_P4->Write();
    LUT_Progression_Q4->Write();
    LUT_Progression_R4->Write();
    LUT_Progression_S4->Write();
    LUT_Progression_T4->Write();
    LUT_Progression_U4->Write();
    LUT_Progression_V4->Write();
    LUT_Progression_W4->Write();
    LUT_Progression_X4->Write();
    LUT_Progression_Y4->Write();
    LUT_Progression_Z4->Write();
    LUT_Progression_A5->Write();
    LUT_Progression_B5->Write();
    LUT_Progression_C5->Write();
    LUT_Progression_D5->Write();
    LUT_Progression_E5->Write();
    LUT_Progression_F5->Write();
    LUT_Progression_G5->Write();
    LUT_Progression_H5->Write();
    LUT_Progression_I5->Write();
    LUT_Progression_J5->Write();
    LUT_Progression_K5->Write();
    LUT_Progression_L5->Write();
    LUT_Progression_M5->Write();
    LUT_Progression_N5->Write();
    LUT_Progression_O5->Write();
    LUT_Progression_P5->Write();
    LUT_Progression_Q5->Write();
    LUT_Progression_R5->Write();
    LUT_Progression_S5->Write();
    LUT_Progression_T5->Write();
    LUT_Progression_U5->Write();
    LUT_Progression_V5->Write();
    LUT_Progression_W5->Write();
    LUT_Progression_X5->Write();
    LUT_Progression_Y5->Write();
    LUT_Progression_Z5->Write();
    LUT_Progression_A6->Write();
    LUT_Progression_B6->Write();
    LUT_Progression_C6->Write();
    LUT_Progression_D6->Write();
    LUT_Progression_E6->Write();
    LUT_Progression_F6->Write();
    LUT_Progression_G6->Write();
    LUT_Progression_H6->Write();
    LUT_Progression_I6->Write();
    LUT_Progression_J6->Write();
    LUT_Progression_K6->Write();
    LUT_Progression_L6->Write();
    LUT_Progression_M6->Write();
    LUT_Progression_N6->Write();
    LUT_Progression_O6->Write();
    LUT_Progression_P6->Write();
    LUT_Progression_Q6->Write();
    LUT_Progression_R6->Write();
    LUT_Progression_S6->Write();
    LUT_Progression_T6->Write();
    LUT_Progression_U6->Write();
    LUT_Progression_V6->Write();
    LUT_Progression_W6->Write();
    LUT_Progression_X6->Write();
    LUT_Progression_Y6->Write();
    LUT_Progression_Z6->Write();
    LUT_Progression_A7->Write();
    LUT_Progression_B7->Write();
    LUT_Progression_C7->Write();
    LUT_Progression_D7->Write();
    LUT_Progression_E7->Write();
    LUT_Progression_F7->Write();
    LUT_Progression_G7->Write();
    LUT_Progression_H7->Write();
    LUT_Progression_I7->Write();
    LUT_Progression_J7->Write();
    LUT_Progression_K7->Write();
    LUT_Progression_L7->Write();
    LUT_Progression_M7->Write();
    LUT_Progression_N7->Write();
    LUT_Progression_O7->Write();
    LUT_Progression_P7->Write();
    LUT_Progression_Q7->Write();
    LUT_Progression_R7->Write();
    LUT_Progression_S7->Write();
    LUT_Progression_T7->Write();
    LUT_Progression_U7->Write();
    LUT_Progression_V7->Write();
    LUT_Progression_W7->Write();
    LUT_Progression_X7->Write();
    LUT_Progression_Y7->Write();
    LUT_Progression_Z7->Write();
    LUT_Progression_A8->Write();
    LUT_Progression_B8->Write();
    LUT_Progression_C8->Write();
    LUT_Progression_D8->Write();
    LUT_Progression_E8->Write();
    LUT_Progression_F8->Write();
    LUT_Progression_G8->Write();
    LUT_Progression_H8->Write();
    LUT_Progression_I8->Write();
    LUT_Progression_J8->Write();
    LUT_Progression_K8->Write();
    LUT_Progression_L8->Write();
    LUT_Progression_M8->Write();
    LUT_Progression_N8->Write();
    LUT_Progression_O8->Write();
    LUT_Progression_P8->Write();
    LUT_Progression_Q8->Write();
    LUT_Progression_R8->Write();
    LUT_Progression_S8->Write();
    LUT_Progression_T8->Write();
    LUT_Progression_U8->Write();
    LUT_Progression_V8->Write();
    LUT_Progression_W8->Write();
    LUT_Progression_X8->Write();
    LUT_Progression_Y8->Write();
    LUT_Progression_Z8->Write();
    // END OF FULL BLOWN GRID SEARCH
}
