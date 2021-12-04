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
#include "../Calibrate/ApplyCalibration_newnTT.C"

using namespace std;

void ApplyIsolationForTurnOns(TString tag, TString opt="0", Bool_t nTTRange = kFALSE)
{

    TString InputFileName = "/data_CMS/cms/motta/Run3preparation/2021_11_22_optimizationV1/Run3_MC_VBFHToTauTau_M125_CALIBRATED_2021_11_22.root";

    TString FileNameOut = "/data_CMS/cms/motta/Run3preparation/2021_11_22_optimizationV1/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_"+tag+"_"+opt+"_2021_11_22";
    if(nTTRange) FileNameOut += "_nTTRange";
    FileNameOut += ".root";

    std::map<TString,TH3F*> histosIsolation;
    TFile f_Isolation("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/Isolate/LUTs/LUTrelaxation_Trigger_Stage2_Run3_MC_VBFHToTauTau_M125_optimizationV1_"+tag+".root");

    for(UInt_t i = 0 ; i < 101 ; ++i)
    {
        TString CurrentNameHisto = "LUT_WP";
        ostringstream convert;
        convert << i;
        CurrentNameHisto += convert.str();
        TH3F* current_Histo = (TH3F*)f_Isolation.Get(CurrentNameHisto.Data());
        histosIsolation.insert(make_pair(CurrentNameHisto,current_Histo));
    }

    for(UInt_t i = 1 ; i < 32 ; ++i)
    {
        TString CurrentNameHisto = "LUT_Progression_";
        ostringstream convert;
        convert << i;
        CurrentNameHisto += convert.str();
        TH3F* current_Histo = (TH3F*)f_Isolation.Get(CurrentNameHisto.Data());
        histosIsolation.insert(make_pair(CurrentNameHisto,current_Histo));
    }

    std::vector<TString> gridSearch = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    for(UInt_t i = 0 ; i < gridSearch.size() ; ++i)
    {
        TString CurrentNameHisto = "LUT_Progression_"+gridSearch[i]+opt;
        TH3F* current_Histo = (TH3F*)f_Isolation.Get(CurrentNameHisto.Data());
        histosIsolation.insert(make_pair(CurrentNameHisto,current_Histo));
    }

    TH3F* current_Histo_extrap = (TH3F*)f_Isolation.Get("LUT_Progression_31_extrap");
    histosIsolation.insert(make_pair("LUT_Progression_31_extrap",current_Histo_extrap));

    TFile f(InputFileName.Data(),"READ");
    TTree* inTree = (TTree*)f.Get("outTreeCalibrated");

    Int_t           in_L1Tau_IEta;
    Int_t           in_compressedsortedshape;
    Int_t           in_L1Tau_hasEM;
    Float_t         in_Target;
    Int_t           in_L1Tau_IEt;
    Int_t           in_L1Tau_RawIEt;
    Int_t           in_EventNumber;
    Int_t           in_RunNumber;
    Int_t           in_Nvtx;
    Int_t           in_L1Tau_nTT;
    Float_t         in_L1Tau_pt;
    Float_t         in_L1Tau_CalibPt;
    Float_t         in_OfflineTau_pt;
    Float_t         in_OfflineTau_eta;
    Int_t           in_compressedieta;
    Int_t           in_compressedE;
    Int_t           in_L1Tau_Iso;
    Int_t           in_compressednTT;
    Int_t           in_OfflineTau_isMatched;
    Int_t           in_L1Tau_isMerged;
    Int_t           in_compressedshape;
    Int_t           in_supercompressedE;
    Int_t           in_supercompressednTT;
    Int_t           in_L1Tau_Qual;
    Bool_t          in_L1Tau_PassShapeVeto;
    Bool_t          L1Tau_PassShapeVeto_0p6_10_50;
    Bool_t          L1Tau_PassShapeVeto_0p5_20_45;
    Bool_t          L1Tau_PassShapeVeto_0p2_25_60;
    Bool_t          L1Tau_PassShapeVeto_0p2_20_100;

    inTree->SetBranchAddress("L1Tau_IEta", &in_L1Tau_IEta);
    inTree->SetBranchAddress("compressedsortedshape", &in_compressedsortedshape);
    inTree->SetBranchAddress("L1Tau_hasEM", &in_L1Tau_hasEM);
    inTree->SetBranchAddress("Target", &in_Target);
    inTree->SetBranchAddress("L1Tau_IEt", &in_L1Tau_IEt);
    inTree->SetBranchAddress("L1Tau_RawIEt", &in_L1Tau_RawIEt);
    inTree->SetBranchAddress("EventNumber", &in_EventNumber);
    inTree->SetBranchAddress("RunNumber", &in_RunNumber);
    inTree->SetBranchAddress("Nvtx", &in_Nvtx);
    inTree->SetBranchAddress("L1Tau_nTT", &in_L1Tau_nTT);
    inTree->SetBranchAddress("L1Tau_pt", &in_L1Tau_pt);
    inTree->SetBranchAddress("L1Tau_CalibPt", &in_L1Tau_CalibPt);
    inTree->SetBranchAddress("OfflineTau_pt", &in_OfflineTau_pt);
    inTree->SetBranchAddress("OfflineTau_eta", &in_OfflineTau_eta);
    inTree->SetBranchAddress("compressedieta", &in_compressedieta);
    inTree->SetBranchAddress("compressedE", &in_compressedE);
    inTree->SetBranchAddress("L1Tau_Iso", &in_L1Tau_Iso);
    inTree->SetBranchAddress("compressednTT", &in_compressednTT);
    inTree->SetBranchAddress("OfflineTau_isMatched", &in_OfflineTau_isMatched);
    inTree->SetBranchAddress("L1Tau_isMerged", &in_L1Tau_isMerged);
    inTree->SetBranchAddress("compressedshape", &in_compressedshape);
    inTree->SetBranchAddress("supercompressedE", &in_supercompressedE);
    inTree->SetBranchAddress("supercompressednTT", &in_supercompressednTT);
    inTree->SetBranchAddress("L1Tau_PassShapeVeto", &in_L1Tau_PassShapeVeto);
    inTree->SetBranchAddress("L1Tau_Qual",&in_L1Tau_Qual);

    Double_t binning[22] = {18,20,22,24,26,28,30,32,35,40,45,50,60,70,90,110,210,350,500,700,1000,2000};
    TH1F* pt = new TH1F("pt","pt",21,binning);
    TH1F* pt_barrel = new TH1F("pt_barrel","pt_barrel",21,binning);
    TH1F* pt_endcaps = new TH1F("pt_endcaps","pt_endcaps",21,binning);
    TH1F* pt_pass_noIso   = new TH1F("pt_pass_noIso"  ,"pt_pass_noIso"  ,21,binning);
    
    TH1F* pt_pass_OptionA  = new TH1F("pt_pass_OptionA"+opt ,"pt_pass_OptionA"+opt ,21,binning);
    TH1F* pt_pass_OptionB  = new TH1F("pt_pass_OptionB"+opt ,"pt_pass_OptionB"+opt ,21,binning);
    TH1F* pt_pass_OptionC  = new TH1F("pt_pass_OptionC"+opt ,"pt_pass_OptionC"+opt ,21,binning);
    TH1F* pt_pass_OptionD  = new TH1F("pt_pass_OptionD"+opt ,"pt_pass_OptionD"+opt ,21,binning);
    TH1F* pt_pass_OptionE  = new TH1F("pt_pass_OptionE"+opt ,"pt_pass_OptionE"+opt ,21,binning);
    TH1F* pt_pass_OptionF  = new TH1F("pt_pass_OptionF"+opt ,"pt_pass_OptionF"+opt ,21,binning);
    TH1F* pt_pass_OptionG  = new TH1F("pt_pass_OptionG"+opt ,"pt_pass_OptionG"+opt ,21,binning);
    TH1F* pt_pass_OptionH  = new TH1F("pt_pass_OptionH"+opt ,"pt_pass_OptionH"+opt ,21,binning);
    TH1F* pt_pass_OptionI  = new TH1F("pt_pass_OptionI"+opt ,"pt_pass_OptionI"+opt ,21,binning);
    TH1F* pt_pass_OptionJ  = new TH1F("pt_pass_OptionJ"+opt ,"pt_pass_OptionJ"+opt ,21,binning);
    TH1F* pt_pass_OptionK  = new TH1F("pt_pass_OptionK"+opt ,"pt_pass_OptionK"+opt ,21,binning);
    TH1F* pt_pass_OptionL  = new TH1F("pt_pass_OptionL"+opt ,"pt_pass_OptionL"+opt ,21,binning);
    TH1F* pt_pass_OptionM  = new TH1F("pt_pass_OptionM"+opt ,"pt_pass_OptionM"+opt ,21,binning);
    TH1F* pt_pass_OptionN  = new TH1F("pt_pass_OptionN"+opt ,"pt_pass_OptionN"+opt ,21,binning);
    TH1F* pt_pass_OptionO  = new TH1F("pt_pass_OptionO"+opt ,"pt_pass_OptionO"+opt ,21,binning);
    TH1F* pt_pass_OptionP  = new TH1F("pt_pass_OptionP"+opt ,"pt_pass_OptionP"+opt ,21,binning);
    TH1F* pt_pass_OptionQ  = new TH1F("pt_pass_OptionQ"+opt ,"pt_pass_OptionQ"+opt ,21,binning);
    TH1F* pt_pass_OptionR  = new TH1F("pt_pass_OptionR"+opt ,"pt_pass_OptionR"+opt ,21,binning);
    TH1F* pt_pass_OptionS  = new TH1F("pt_pass_OptionS"+opt ,"pt_pass_OptionS"+opt ,21,binning);
    TH1F* pt_pass_OptionT  = new TH1F("pt_pass_OptionT"+opt ,"pt_pass_OptionT"+opt ,21,binning);
    TH1F* pt_pass_OptionU  = new TH1F("pt_pass_OptionU"+opt ,"pt_pass_OptionU"+opt ,21,binning);
    TH1F* pt_pass_OptionV  = new TH1F("pt_pass_OptionV"+opt ,"pt_pass_OptionV"+opt ,21,binning);
    TH1F* pt_pass_OptionW  = new TH1F("pt_pass_OptionW"+opt ,"pt_pass_OptionW"+opt ,21,binning);
    TH1F* pt_pass_OptionX  = new TH1F("pt_pass_OptionX"+opt ,"pt_pass_OptionX"+opt ,21,binning);
    TH1F* pt_pass_OptionY  = new TH1F("pt_pass_OptionY"+opt ,"pt_pass_OptionY"+opt ,21,binning);
    TH1F* pt_pass_OptionZ  = new TH1F("pt_pass_OptionZ"+opt ,"pt_pass_OptionZ"+opt ,21,binning);

    TH1F* pt_pass_Option22  = new TH1F("pt_pass_Option22" ,"pt_pass_Option22" ,21,binning);
    TH1F* pt_pass_Option31_extrap  = new TH1F("pt_pass_Option31_extrap" ,"pt_pass_Option31_extrap" ,21,binning);

    TH1F* nVtx_pass_Option22 = new TH1F("nVtx_pass_Option22" ,"nVtx_pass_Option22" ,50,0.,100.);
    TH1F* nVtx_pass_Option31 = new TH1F("nVtx_pass_Option31" ,"nVtx_pass_Option31" ,50,0.,100.);
    TH1F* nVtx_pass_Option31_extrap = new TH1F("nVtx_pass_Option31_extrap" ,"nVtx_pass_Option31_extrap" ,50,0.,100.);
    TH1F* nVtx  = new TH1F("nVtx" ,"nVtx" ,50,0.,100.);

    TH1F* eta_pass_Option22 = new TH1F("eta_pass_Option22" ,"eta_pass_Option22" ,43,-2.1,2.1);
    TH1F* eta_pass_Option31 = new TH1F("eta_pass_Option31" ,"eta_pass_Option31" ,43,-2.1,2.1);
    TH1F* eta_pass_Option31_extrap = new TH1F("eta_pass_Option31_extrap" ,"eta_pass_Option31_extrap" ,43,-2.1,2.1);
    TH1F* eta  = new TH1F("eta" ,"eta" ,43,-2.1,2.1);
 
    map<int, int> remap;
    remap[0] = 6 ;
    remap[1] = 5 ;
    remap[2] = 1 ;
    remap[3] = 0 ;
    remap[4] = 4 ;
    remap[5] = 3 ;
    remap[6] = 2 ;
    
    for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
        {
            inTree->GetEntry(i);
            if(in_L1Tau_nTT<60 && nTTRange) continue;
            //if(in_Nvtx<40 && in_Nvtx>50) continue;      

            if(fabs(in_OfflineTau_eta)>2.1) continue;

            pt->Fill(in_OfflineTau_pt);
            if(fabs(in_OfflineTau_eta)<1.5) pt_barrel->Fill(in_OfflineTau_pt);
            else pt_endcaps->Fill(in_OfflineTau_pt);

            Int_t Cut_L1Tau_Iso_OptionA  = histosIsolation["LUT_Progression_A"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionB  = histosIsolation["LUT_Progression_B"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionC  = histosIsolation["LUT_Progression_C"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionD  = histosIsolation["LUT_Progression_D"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionE  = histosIsolation["LUT_Progression_E"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionF  = histosIsolation["LUT_Progression_F"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionG  = histosIsolation["LUT_Progression_G"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionH  = histosIsolation["LUT_Progression_H"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionI  = histosIsolation["LUT_Progression_I"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionJ  = histosIsolation["LUT_Progression_J"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionK  = histosIsolation["LUT_Progression_K"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionL  = histosIsolation["LUT_Progression_L"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionM  = histosIsolation["LUT_Progression_M"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionN  = histosIsolation["LUT_Progression_N"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionO  = histosIsolation["LUT_Progression_O"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionP  = histosIsolation["LUT_Progression_P"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionQ  = histosIsolation["LUT_Progression_Q"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionR  = histosIsolation["LUT_Progression_R"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionS  = histosIsolation["LUT_Progression_S"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionT  = histosIsolation["LUT_Progression_T"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionU  = histosIsolation["LUT_Progression_U"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionV  = histosIsolation["LUT_Progression_V"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionW  = histosIsolation["LUT_Progression_W"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionX  = histosIsolation["LUT_Progression_X"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionY  = histosIsolation["LUT_Progression_Y"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_OptionZ  = histosIsolation["LUT_Progression_Z"+opt] ->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);

            Int_t Cut_L1Tau_Iso_Option22  = histosIsolation["LUT_Progression_22"]->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);
            Int_t Cut_L1Tau_Iso_Option31_extrap  = histosIsolation["LUT_Progression_31_extrap"]->GetBinContent(in_compressedieta+1,in_compressedE+1,in_compressednTT+1);

            Double_t Threshold_NewLayer1_noIso   = 0.0;
            Double_t Threshold_NewLayer1_noIso_unpacked = 0.0;
            Double_t Threshold_NewLayer1_Iso_unpacked = 0.0;
            Double_t Threshold_NewLayer1_OptionA = 0.0;
            Double_t Threshold_NewLayer1_OptionB = 0.0;
            Double_t Threshold_NewLayer1_OptionC = 0.0;
            Double_t Threshold_NewLayer1_OptionD = 0.0;
            Double_t Threshold_NewLayer1_OptionE = 0.0;
            Double_t Threshold_NewLayer1_OptionF = 0.0;
            Double_t Threshold_NewLayer1_OptionG = 0.0;
            Double_t Threshold_NewLayer1_OptionH = 0.0;
            Double_t Threshold_NewLayer1_OptionI = 0.0;
            Double_t Threshold_NewLayer1_OptionJ = 0.0;
            Double_t Threshold_NewLayer1_OptionK = 0.0;
            Double_t Threshold_NewLayer1_OptionL = 0.0;
            Double_t Threshold_NewLayer1_OptionM = 0.0;
            Double_t Threshold_NewLayer1_OptionN = 0.0;
            Double_t Threshold_NewLayer1_OptionO = 0.0;
            Double_t Threshold_NewLayer1_OptionP = 0.0;
            Double_t Threshold_NewLayer1_OptionQ = 0.0;
            Double_t Threshold_NewLayer1_OptionR = 0.0;
            Double_t Threshold_NewLayer1_OptionS = 0.0;
            Double_t Threshold_NewLayer1_OptionT = 0.0;
            Double_t Threshold_NewLayer1_OptionU = 0.0;
            Double_t Threshold_NewLayer1_OptionV = 0.0;
            Double_t Threshold_NewLayer1_OptionW = 0.0;
            Double_t Threshold_NewLayer1_OptionX = 0.0;
            Double_t Threshold_NewLayer1_OptionY = 0.0;
            Double_t Threshold_NewLayer1_OptionZ = 0.0;
            Double_t Threshold_NewLayer1_Option22 = 0.0;
            Double_t Threshold_NewLayer1_Option31_extrap = 0.0;


            if (tag=="00" and opt=="0")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 42.51;
                Double_t Threshold_NewLayer1_OptionB = 42.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 42.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 41.51;
                Double_t Threshold_NewLayer1_OptionI = 39.51;
                Double_t Threshold_NewLayer1_OptionJ = 38.51;
                Double_t Threshold_NewLayer1_OptionK = 37.51;
                Double_t Threshold_NewLayer1_OptionL = 35.51;
                Double_t Threshold_NewLayer1_OptionM = 32.51;
                Double_t Threshold_NewLayer1_OptionN = 30.51;
                Double_t Threshold_NewLayer1_OptionO = 30.51;
                Double_t Threshold_NewLayer1_OptionP = 28.51;
                Double_t Threshold_NewLayer1_OptionQ = 26.51;
                Double_t Threshold_NewLayer1_OptionR = 26.51;
                Double_t Threshold_NewLayer1_OptionS = 42.51;
                Double_t Threshold_NewLayer1_OptionT = 42.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 42.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 41.51;
                Double_t Threshold_NewLayer1_OptionZ = 39.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="00" and opt=="1")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 37.51;
                Double_t Threshold_NewLayer1_OptionB = 36.51;
                Double_t Threshold_NewLayer1_OptionC = 34.51;
                Double_t Threshold_NewLayer1_OptionD = 32.51;
                Double_t Threshold_NewLayer1_OptionE = 30.51;
                Double_t Threshold_NewLayer1_OptionF = 28.51;
                Double_t Threshold_NewLayer1_OptionG = 28.51;
                Double_t Threshold_NewLayer1_OptionH = 26.51;
                Double_t Threshold_NewLayer1_OptionI = 25.51;
                Double_t Threshold_NewLayer1_OptionJ = 25.51;
                Double_t Threshold_NewLayer1_OptionK = 42.51;
                Double_t Threshold_NewLayer1_OptionL = 42.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 42.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 41.51;
                Double_t Threshold_NewLayer1_OptionQ = 39.51;
                Double_t Threshold_NewLayer1_OptionR = 37.51;
                Double_t Threshold_NewLayer1_OptionS = 35.51;
                Double_t Threshold_NewLayer1_OptionT = 33.51;
                Double_t Threshold_NewLayer1_OptionU = 31.51;
                Double_t Threshold_NewLayer1_OptionV = 29.51;
                Double_t Threshold_NewLayer1_OptionW = 27.51;
                Double_t Threshold_NewLayer1_OptionX = 26.51;
                Double_t Threshold_NewLayer1_OptionY = 25.51;
                Double_t Threshold_NewLayer1_OptionZ = 25.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="00" and opt=="2")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 25.51;
                Double_t Threshold_NewLayer1_OptionB = 25.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 39.51;
                Double_t Threshold_NewLayer1_OptionI = 36.51;
                Double_t Threshold_NewLayer1_OptionJ = 35.51;
                Double_t Threshold_NewLayer1_OptionK = 31.51;
                Double_t Threshold_NewLayer1_OptionL = 29.51;
                Double_t Threshold_NewLayer1_OptionM = 27.51;
                Double_t Threshold_NewLayer1_OptionN = 26.51;
                Double_t Threshold_NewLayer1_OptionO = 25.51;
                Double_t Threshold_NewLayer1_OptionP = 23.51;
                Double_t Threshold_NewLayer1_OptionQ = 20.51;
                Double_t Threshold_NewLayer1_OptionR = 18.51;
                Double_t Threshold_NewLayer1_OptionS = 18.51;
                Double_t Threshold_NewLayer1_OptionT = 17.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 41.51;
                Double_t Threshold_NewLayer1_OptionX = 40.51;
                Double_t Threshold_NewLayer1_OptionY = 39.51;
                Double_t Threshold_NewLayer1_OptionZ = 36.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="00" and opt=="3")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 33.51;
                Double_t Threshold_NewLayer1_OptionB = 29.51;
                Double_t Threshold_NewLayer1_OptionC = 25.51;
                Double_t Threshold_NewLayer1_OptionD = 25.51;
                Double_t Threshold_NewLayer1_OptionE = 22.51;
                Double_t Threshold_NewLayer1_OptionF = 20.51;
                Double_t Threshold_NewLayer1_OptionG = 16.51;
                Double_t Threshold_NewLayer1_OptionH = 16.51;
                Double_t Threshold_NewLayer1_OptionI = 15.51;
                Double_t Threshold_NewLayer1_OptionJ = 15.51;
                Double_t Threshold_NewLayer1_OptionK = 15.51;
                Double_t Threshold_NewLayer1_OptionL = 15.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 41.51;
                Double_t Threshold_NewLayer1_OptionO = 40.51;
                Double_t Threshold_NewLayer1_OptionP = 38.51;
                Double_t Threshold_NewLayer1_OptionQ = 34.51;
                Double_t Threshold_NewLayer1_OptionR = 32.51;
                Double_t Threshold_NewLayer1_OptionS = 24.51;
                Double_t Threshold_NewLayer1_OptionT = 18.51;
                Double_t Threshold_NewLayer1_OptionU = 17.51;
                Double_t Threshold_NewLayer1_OptionV = 16.51;
                Double_t Threshold_NewLayer1_OptionW = 16.51;
                Double_t Threshold_NewLayer1_OptionX = 15.51;
                Double_t Threshold_NewLayer1_OptionY = 15.51;
                Double_t Threshold_NewLayer1_OptionZ = 15.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="00" and opt=="4")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 14.51;
                Double_t Threshold_NewLayer1_OptionB = 14.51;
                Double_t Threshold_NewLayer1_OptionC = 14.51;
                Double_t Threshold_NewLayer1_OptionD = 14.51;
                Double_t Threshold_NewLayer1_OptionE = 41.51;
                Double_t Threshold_NewLayer1_OptionF = 40.51;
                Double_t Threshold_NewLayer1_OptionG = 38.51;
                Double_t Threshold_NewLayer1_OptionH = 32.51;
                Double_t Threshold_NewLayer1_OptionI = 25.51;
                Double_t Threshold_NewLayer1_OptionJ = 18.51;
                Double_t Threshold_NewLayer1_OptionK = 16.51;
                Double_t Threshold_NewLayer1_OptionL = 16.51;
                Double_t Threshold_NewLayer1_OptionM = 15.51;
                Double_t Threshold_NewLayer1_OptionN = 15.51;
                Double_t Threshold_NewLayer1_OptionO = 14.51;
                Double_t Threshold_NewLayer1_OptionP = 14.51;
                Double_t Threshold_NewLayer1_OptionQ = 14.51;
                Double_t Threshold_NewLayer1_OptionR = 13.51;
                Double_t Threshold_NewLayer1_OptionS = 13.51;
                Double_t Threshold_NewLayer1_OptionT = 13.51;
                Double_t Threshold_NewLayer1_OptionU = 13.51;
                Double_t Threshold_NewLayer1_OptionV = 13.51;
                Double_t Threshold_NewLayer1_OptionW = 40.51;
                Double_t Threshold_NewLayer1_OptionX = 37.51;
                Double_t Threshold_NewLayer1_OptionY = 23.51;
                Double_t Threshold_NewLayer1_OptionZ = 17.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="00" and opt=="5")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 16.51;
                Double_t Threshold_NewLayer1_OptionB = 15.51;
                Double_t Threshold_NewLayer1_OptionC = 15.51;
                Double_t Threshold_NewLayer1_OptionD = 15.51;
                Double_t Threshold_NewLayer1_OptionE = 14.51;
                Double_t Threshold_NewLayer1_OptionF = 14.51;
                Double_t Threshold_NewLayer1_OptionG = 13.51;
                Double_t Threshold_NewLayer1_OptionH = 13.51;
                Double_t Threshold_NewLayer1_OptionI = 13.51;
                Double_t Threshold_NewLayer1_OptionJ = 13.51;
                Double_t Threshold_NewLayer1_OptionK = 13.51;
                Double_t Threshold_NewLayer1_OptionL = 13.51;
                Double_t Threshold_NewLayer1_OptionM = 13.51;
                Double_t Threshold_NewLayer1_OptionN = 13.51;
                Double_t Threshold_NewLayer1_OptionO = 33.51;
                Double_t Threshold_NewLayer1_OptionP = 17.51;
                Double_t Threshold_NewLayer1_OptionQ = 16.51;
                Double_t Threshold_NewLayer1_OptionR = 15.51;
                Double_t Threshold_NewLayer1_OptionS = 15.51;
                Double_t Threshold_NewLayer1_OptionT = 15.51;
                Double_t Threshold_NewLayer1_OptionU = 15.51;
                Double_t Threshold_NewLayer1_OptionV = 14.51;
                Double_t Threshold_NewLayer1_OptionW = 13.51;
                Double_t Threshold_NewLayer1_OptionX = 13.51;
                Double_t Threshold_NewLayer1_OptionY = 13.51;
                Double_t Threshold_NewLayer1_OptionZ = 13.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="00" and opt=="6")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 13.51;
                Double_t Threshold_NewLayer1_OptionB = 13.51;
                Double_t Threshold_NewLayer1_OptionC = 13.51;
                Double_t Threshold_NewLayer1_OptionD = 13.51;
                Double_t Threshold_NewLayer1_OptionE = 13.51;
                Double_t Threshold_NewLayer1_OptionF = 13.51;
                Double_t Threshold_NewLayer1_OptionG = 16.51;
                Double_t Threshold_NewLayer1_OptionH = 16.51;
                Double_t Threshold_NewLayer1_OptionI = 15.51;
                Double_t Threshold_NewLayer1_OptionJ = 15.51;
                Double_t Threshold_NewLayer1_OptionK = 15.51;
                Double_t Threshold_NewLayer1_OptionL = 14.51;
                Double_t Threshold_NewLayer1_OptionM = 13.51;
                Double_t Threshold_NewLayer1_OptionN = 13.51;
                Double_t Threshold_NewLayer1_OptionO = 13.51;
                Double_t Threshold_NewLayer1_OptionP = 13.51;
                Double_t Threshold_NewLayer1_OptionQ = 13.51;
                Double_t Threshold_NewLayer1_OptionR = 13.51;
                Double_t Threshold_NewLayer1_OptionS = 13.51;
                Double_t Threshold_NewLayer1_OptionT = 13.51;
                Double_t Threshold_NewLayer1_OptionU = 13.51;
                Double_t Threshold_NewLayer1_OptionV = 13.51;
                Double_t Threshold_NewLayer1_OptionW = 13.51;
                Double_t Threshold_NewLayer1_OptionX = 13.51;
                Double_t Threshold_NewLayer1_OptionY = 15.51;
                Double_t Threshold_NewLayer1_OptionZ = 15.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="00" and opt=="7")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 15.51;
                Double_t Threshold_NewLayer1_OptionB = 14.51;
                Double_t Threshold_NewLayer1_OptionC = 14.51;
                Double_t Threshold_NewLayer1_OptionD = 13.51;
                Double_t Threshold_NewLayer1_OptionE = 13.51;
                Double_t Threshold_NewLayer1_OptionF = 13.51;
                Double_t Threshold_NewLayer1_OptionG = 13.51;
                Double_t Threshold_NewLayer1_OptionH = 13.51;
                Double_t Threshold_NewLayer1_OptionI = 13.51;
                Double_t Threshold_NewLayer1_OptionJ = 13.51;
                Double_t Threshold_NewLayer1_OptionK = 13.51;
                Double_t Threshold_NewLayer1_OptionL = 13.51;
                Double_t Threshold_NewLayer1_OptionM = 13.51;
                Double_t Threshold_NewLayer1_OptionN = 13.51;
                Double_t Threshold_NewLayer1_OptionO = 13.51;
                Double_t Threshold_NewLayer1_OptionP = 13.51;
                Double_t Threshold_NewLayer1_OptionQ = 15.51;
                Double_t Threshold_NewLayer1_OptionR = 14.51;
                Double_t Threshold_NewLayer1_OptionS = 14.51;
                Double_t Threshold_NewLayer1_OptionT = 14.51;
                Double_t Threshold_NewLayer1_OptionU = 13.51;
                Double_t Threshold_NewLayer1_OptionV = 13.51;
                Double_t Threshold_NewLayer1_OptionW = 13.51;
                Double_t Threshold_NewLayer1_OptionX = 13.51;
                Double_t Threshold_NewLayer1_OptionY = 13.51;
                Double_t Threshold_NewLayer1_OptionZ = 13.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="00" and opt=="8")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 13.51;
                Double_t Threshold_NewLayer1_OptionB = 13.51;
                Double_t Threshold_NewLayer1_OptionC = 13.51;
                Double_t Threshold_NewLayer1_OptionD = 13.51;
                Double_t Threshold_NewLayer1_OptionE = 13.51;
                Double_t Threshold_NewLayer1_OptionF = 13.51;
                Double_t Threshold_NewLayer1_OptionG = 13.51;
                Double_t Threshold_NewLayer1_OptionH = 13.51;
                Double_t Threshold_NewLayer1_OptionI = 14.51;
                Double_t Threshold_NewLayer1_OptionJ = 14.51;
                Double_t Threshold_NewLayer1_OptionK = 13.51;
                Double_t Threshold_NewLayer1_OptionL = 13.51;
                Double_t Threshold_NewLayer1_OptionM = 13.51;
                Double_t Threshold_NewLayer1_OptionN = 13.51;
                Double_t Threshold_NewLayer1_OptionO = 13.51;
                Double_t Threshold_NewLayer1_OptionP = 13.51;
                Double_t Threshold_NewLayer1_OptionQ = 13.51;
                Double_t Threshold_NewLayer1_OptionR = 13.51;
                Double_t Threshold_NewLayer1_OptionS = 13.51;
                Double_t Threshold_NewLayer1_OptionT = 13.51;
                Double_t Threshold_NewLayer1_OptionU = 13.51;
                Double_t Threshold_NewLayer1_OptionV = 13.51;
                Double_t Threshold_NewLayer1_OptionW = 13.51;
                Double_t Threshold_NewLayer1_OptionX = 13.51;
                Double_t Threshold_NewLayer1_OptionY = 13.51;
                Double_t Threshold_NewLayer1_OptionZ = 12.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="01" and opt=="0")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 42.51;
                Double_t Threshold_NewLayer1_OptionB = 42.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 42.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 41.51;
                Double_t Threshold_NewLayer1_OptionI = 40.51;
                Double_t Threshold_NewLayer1_OptionJ = 38.51;
                Double_t Threshold_NewLayer1_OptionK = 37.51;
                Double_t Threshold_NewLayer1_OptionL = 35.51;
                Double_t Threshold_NewLayer1_OptionM = 34.51;
                Double_t Threshold_NewLayer1_OptionN = 32.51;
                Double_t Threshold_NewLayer1_OptionO = 31.51;
                Double_t Threshold_NewLayer1_OptionP = 30.51;
                Double_t Threshold_NewLayer1_OptionQ = 28.51;
                Double_t Threshold_NewLayer1_OptionR = 27.51;
                Double_t Threshold_NewLayer1_OptionS = 42.51;
                Double_t Threshold_NewLayer1_OptionT = 42.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 42.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 41.51;
                Double_t Threshold_NewLayer1_OptionZ = 39.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="01" and opt=="1")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 37.51;
                Double_t Threshold_NewLayer1_OptionB = 37.51;
                Double_t Threshold_NewLayer1_OptionC = 35.51;
                Double_t Threshold_NewLayer1_OptionD = 33.51;
                Double_t Threshold_NewLayer1_OptionE = 31.51;
                Double_t Threshold_NewLayer1_OptionF = 30.51;
                Double_t Threshold_NewLayer1_OptionG = 29.51;
                Double_t Threshold_NewLayer1_OptionH = 27.51;
                Double_t Threshold_NewLayer1_OptionI = 26.51;
                Double_t Threshold_NewLayer1_OptionJ = 26.51;
                Double_t Threshold_NewLayer1_OptionK = 42.51;
                Double_t Threshold_NewLayer1_OptionL = 42.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 42.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 41.51;
                Double_t Threshold_NewLayer1_OptionQ = 39.51;
                Double_t Threshold_NewLayer1_OptionR = 37.51;
                Double_t Threshold_NewLayer1_OptionS = 36.51;
                Double_t Threshold_NewLayer1_OptionT = 34.51;
                Double_t Threshold_NewLayer1_OptionU = 32.51;
                Double_t Threshold_NewLayer1_OptionV = 30.51;
                Double_t Threshold_NewLayer1_OptionW = 29.51;
                Double_t Threshold_NewLayer1_OptionX = 28.51;
                Double_t Threshold_NewLayer1_OptionY = 26.51;
                Double_t Threshold_NewLayer1_OptionZ = 26.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="01" and opt=="2")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 26.51;
                Double_t Threshold_NewLayer1_OptionB = 25.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 39.51;
                Double_t Threshold_NewLayer1_OptionI = 37.51;
                Double_t Threshold_NewLayer1_OptionJ = 35.51;
                Double_t Threshold_NewLayer1_OptionK = 32.51;
                Double_t Threshold_NewLayer1_OptionL = 31.51;
                Double_t Threshold_NewLayer1_OptionM = 30.51;
                Double_t Threshold_NewLayer1_OptionN = 28.51;
                Double_t Threshold_NewLayer1_OptionO = 28.51;
                Double_t Threshold_NewLayer1_OptionP = 26.51;
                Double_t Threshold_NewLayer1_OptionQ = 25.51;
                Double_t Threshold_NewLayer1_OptionR = 25.51;
                Double_t Threshold_NewLayer1_OptionS = 25.51;
                Double_t Threshold_NewLayer1_OptionT = 25.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 41.51;
                Double_t Threshold_NewLayer1_OptionX = 40.51;
                Double_t Threshold_NewLayer1_OptionY = 39.51;
                Double_t Threshold_NewLayer1_OptionZ = 36.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="01" and opt=="3")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 35.51;
                Double_t Threshold_NewLayer1_OptionB = 31.51;
                Double_t Threshold_NewLayer1_OptionC = 29.51;
                Double_t Threshold_NewLayer1_OptionD = 28.51;
                Double_t Threshold_NewLayer1_OptionE = 27.51;
                Double_t Threshold_NewLayer1_OptionF = 26.51;
                Double_t Threshold_NewLayer1_OptionG = 25.51;
                Double_t Threshold_NewLayer1_OptionH = 25.51;
                Double_t Threshold_NewLayer1_OptionI = 25.51;
                Double_t Threshold_NewLayer1_OptionJ = 25.51;
                Double_t Threshold_NewLayer1_OptionK = 25.51;
                Double_t Threshold_NewLayer1_OptionL = 25.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 41.51;
                Double_t Threshold_NewLayer1_OptionO = 40.51;
                Double_t Threshold_NewLayer1_OptionP = 39.51;
                Double_t Threshold_NewLayer1_OptionQ = 36.51;
                Double_t Threshold_NewLayer1_OptionR = 34.51;
                Double_t Threshold_NewLayer1_OptionS = 30.51;
                Double_t Threshold_NewLayer1_OptionT = 27.51;
                Double_t Threshold_NewLayer1_OptionU = 26.51;
                Double_t Threshold_NewLayer1_OptionV = 26.51;
                Double_t Threshold_NewLayer1_OptionW = 25.51;
                Double_t Threshold_NewLayer1_OptionX = 25.51;
                Double_t Threshold_NewLayer1_OptionY = 25.51;
                Double_t Threshold_NewLayer1_OptionZ = 25.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="01" and opt=="4")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 25.51;
                Double_t Threshold_NewLayer1_OptionB = 25.51;
                Double_t Threshold_NewLayer1_OptionC = 25.51;
                Double_t Threshold_NewLayer1_OptionD = 25.51;
                Double_t Threshold_NewLayer1_OptionE = 41.51;
                Double_t Threshold_NewLayer1_OptionF = 40.51;
                Double_t Threshold_NewLayer1_OptionG = 38.51;
                Double_t Threshold_NewLayer1_OptionH = 34.51;
                Double_t Threshold_NewLayer1_OptionI = 32.51;
                Double_t Threshold_NewLayer1_OptionJ = 27.51;
                Double_t Threshold_NewLayer1_OptionK = 26.51;
                Double_t Threshold_NewLayer1_OptionL = 25.51;
                Double_t Threshold_NewLayer1_OptionM = 25.51;
                Double_t Threshold_NewLayer1_OptionN = 25.51;
                Double_t Threshold_NewLayer1_OptionO = 25.51;
                Double_t Threshold_NewLayer1_OptionP = 25.51;
                Double_t Threshold_NewLayer1_OptionQ = 25.51;
                Double_t Threshold_NewLayer1_OptionR = 25.51;
                Double_t Threshold_NewLayer1_OptionS = 25.51;
                Double_t Threshold_NewLayer1_OptionT = 25.51;
                Double_t Threshold_NewLayer1_OptionU = 25.51;
                Double_t Threshold_NewLayer1_OptionV = 25.51;
                Double_t Threshold_NewLayer1_OptionW = 40.51;
                Double_t Threshold_NewLayer1_OptionX = 37.51;
                Double_t Threshold_NewLayer1_OptionY = 32.51;
                Double_t Threshold_NewLayer1_OptionZ = 28.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="01" and opt=="5")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 26.51;
                Double_t Threshold_NewLayer1_OptionB = 25.51;
                Double_t Threshold_NewLayer1_OptionC = 25.51;
                Double_t Threshold_NewLayer1_OptionD = 25.51;
                Double_t Threshold_NewLayer1_OptionE = 25.51;
                Double_t Threshold_NewLayer1_OptionF = 25.51;
                Double_t Threshold_NewLayer1_OptionG = 25.51;
                Double_t Threshold_NewLayer1_OptionH = 25.51;
                Double_t Threshold_NewLayer1_OptionI = 25.51;
                Double_t Threshold_NewLayer1_OptionJ = 25.51;
                Double_t Threshold_NewLayer1_OptionK = 25.51;
                Double_t Threshold_NewLayer1_OptionL = 25.51;
                Double_t Threshold_NewLayer1_OptionM = 25.51;
                Double_t Threshold_NewLayer1_OptionN = 25.51;
                Double_t Threshold_NewLayer1_OptionO = 36.51;
                Double_t Threshold_NewLayer1_OptionP = 29.51;
                Double_t Threshold_NewLayer1_OptionQ = 26.51;
                Double_t Threshold_NewLayer1_OptionR = 25.51;
                Double_t Threshold_NewLayer1_OptionS = 25.51;
                Double_t Threshold_NewLayer1_OptionT = 25.51;
                Double_t Threshold_NewLayer1_OptionU = 25.51;
                Double_t Threshold_NewLayer1_OptionV = 25.51;
                Double_t Threshold_NewLayer1_OptionW = 25.51;
                Double_t Threshold_NewLayer1_OptionX = 25.51;
                Double_t Threshold_NewLayer1_OptionY = 25.51;
                Double_t Threshold_NewLayer1_OptionZ = 25.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="01" and opt=="6")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 25.51;
                Double_t Threshold_NewLayer1_OptionB = 25.51;
                Double_t Threshold_NewLayer1_OptionC = 25.51;
                Double_t Threshold_NewLayer1_OptionD = 25.51;
                Double_t Threshold_NewLayer1_OptionE = 25.51;
                Double_t Threshold_NewLayer1_OptionF = 25.51;
                Double_t Threshold_NewLayer1_OptionG = 26.51;
                Double_t Threshold_NewLayer1_OptionH = 25.51;
                Double_t Threshold_NewLayer1_OptionI = 25.51;
                Double_t Threshold_NewLayer1_OptionJ = 25.51;
                Double_t Threshold_NewLayer1_OptionK = 25.51;
                Double_t Threshold_NewLayer1_OptionL = 25.51;
                Double_t Threshold_NewLayer1_OptionM = 25.51;
                Double_t Threshold_NewLayer1_OptionN = 25.51;
                Double_t Threshold_NewLayer1_OptionO = 25.51;
                Double_t Threshold_NewLayer1_OptionP = 25.51;
                Double_t Threshold_NewLayer1_OptionQ = 25.51;
                Double_t Threshold_NewLayer1_OptionR = 25.51;
                Double_t Threshold_NewLayer1_OptionS = 25.51;
                Double_t Threshold_NewLayer1_OptionT = 25.51;
                Double_t Threshold_NewLayer1_OptionU = 25.51;
                Double_t Threshold_NewLayer1_OptionV = 25.51;
                Double_t Threshold_NewLayer1_OptionW = 25.51;
                Double_t Threshold_NewLayer1_OptionX = 25.51;
                Double_t Threshold_NewLayer1_OptionY = 25.51;
                Double_t Threshold_NewLayer1_OptionZ = 25.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="01" and opt=="7")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 25.51;
                Double_t Threshold_NewLayer1_OptionB = 25.51;
                Double_t Threshold_NewLayer1_OptionC = 25.51;
                Double_t Threshold_NewLayer1_OptionD = 25.51;
                Double_t Threshold_NewLayer1_OptionE = 25.51;
                Double_t Threshold_NewLayer1_OptionF = 25.51;
                Double_t Threshold_NewLayer1_OptionG = 25.51;
                Double_t Threshold_NewLayer1_OptionH = 25.51;
                Double_t Threshold_NewLayer1_OptionI = 25.51;
                Double_t Threshold_NewLayer1_OptionJ = 25.51;
                Double_t Threshold_NewLayer1_OptionK = 25.51;
                Double_t Threshold_NewLayer1_OptionL = 25.51;
                Double_t Threshold_NewLayer1_OptionM = 25.51;
                Double_t Threshold_NewLayer1_OptionN = 25.51;
                Double_t Threshold_NewLayer1_OptionO = 25.51;
                Double_t Threshold_NewLayer1_OptionP = 25.51;
                Double_t Threshold_NewLayer1_OptionQ = 25.51;
                Double_t Threshold_NewLayer1_OptionR = 25.51;
                Double_t Threshold_NewLayer1_OptionS = 25.51;
                Double_t Threshold_NewLayer1_OptionT = 25.51;
                Double_t Threshold_NewLayer1_OptionU = 25.51;
                Double_t Threshold_NewLayer1_OptionV = 25.51;
                Double_t Threshold_NewLayer1_OptionW = 25.51;
                Double_t Threshold_NewLayer1_OptionX = 25.51;
                Double_t Threshold_NewLayer1_OptionY = 25.51;
                Double_t Threshold_NewLayer1_OptionZ = 25.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="01" and opt=="8")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 25.51;
                Double_t Threshold_NewLayer1_OptionB = 25.51;
                Double_t Threshold_NewLayer1_OptionC = 25.51;
                Double_t Threshold_NewLayer1_OptionD = 25.51;
                Double_t Threshold_NewLayer1_OptionE = 25.51;
                Double_t Threshold_NewLayer1_OptionF = 25.51;
                Double_t Threshold_NewLayer1_OptionG = 25.51;
                Double_t Threshold_NewLayer1_OptionH = 24.51;
                Double_t Threshold_NewLayer1_OptionI = 25.51;
                Double_t Threshold_NewLayer1_OptionJ = 25.51;
                Double_t Threshold_NewLayer1_OptionK = 25.51;
                Double_t Threshold_NewLayer1_OptionL = 25.51;
                Double_t Threshold_NewLayer1_OptionM = 25.51;
                Double_t Threshold_NewLayer1_OptionN = 25.51;
                Double_t Threshold_NewLayer1_OptionO = 25.51;
                Double_t Threshold_NewLayer1_OptionP = 25.51;
                Double_t Threshold_NewLayer1_OptionQ = 25.51;
                Double_t Threshold_NewLayer1_OptionR = 25.51;
                Double_t Threshold_NewLayer1_OptionS = 25.51;
                Double_t Threshold_NewLayer1_OptionT = 25.51;
                Double_t Threshold_NewLayer1_OptionU = 25.51;
                Double_t Threshold_NewLayer1_OptionV = 25.51;
                Double_t Threshold_NewLayer1_OptionW = 25.51;
                Double_t Threshold_NewLayer1_OptionX = 24.51;
                Double_t Threshold_NewLayer1_OptionY = 24.51;
                Double_t Threshold_NewLayer1_OptionZ = 24.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="02" and opt=="0")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 42.51;
                Double_t Threshold_NewLayer1_OptionB = 42.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 42.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 41.51;
                Double_t Threshold_NewLayer1_OptionI = 40.51;
                Double_t Threshold_NewLayer1_OptionJ = 39.51;
                Double_t Threshold_NewLayer1_OptionK = 37.51;
                Double_t Threshold_NewLayer1_OptionL = 36.51;
                Double_t Threshold_NewLayer1_OptionM = 35.51;
                Double_t Threshold_NewLayer1_OptionN = 33.51;
                Double_t Threshold_NewLayer1_OptionO = 32.51;
                Double_t Threshold_NewLayer1_OptionP = 32.51;
                Double_t Threshold_NewLayer1_OptionQ = 29.51;
                Double_t Threshold_NewLayer1_OptionR = 29.51;
                Double_t Threshold_NewLayer1_OptionS = 42.51;
                Double_t Threshold_NewLayer1_OptionT = 42.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 42.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 41.51;
                Double_t Threshold_NewLayer1_OptionZ = 40.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="02" and opt=="1")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 38.51;
                Double_t Threshold_NewLayer1_OptionB = 37.51;
                Double_t Threshold_NewLayer1_OptionC = 36.51;
                Double_t Threshold_NewLayer1_OptionD = 34.51;
                Double_t Threshold_NewLayer1_OptionE = 33.51;
                Double_t Threshold_NewLayer1_OptionF = 32.51;
                Double_t Threshold_NewLayer1_OptionG = 31.51;
                Double_t Threshold_NewLayer1_OptionH = 29.51;
                Double_t Threshold_NewLayer1_OptionI = 28.51;
                Double_t Threshold_NewLayer1_OptionJ = 28.51;
                Double_t Threshold_NewLayer1_OptionK = 42.51;
                Double_t Threshold_NewLayer1_OptionL = 42.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 42.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 41.51;
                Double_t Threshold_NewLayer1_OptionQ = 40.51;
                Double_t Threshold_NewLayer1_OptionR = 37.51;
                Double_t Threshold_NewLayer1_OptionS = 37.51;
                Double_t Threshold_NewLayer1_OptionT = 35.51;
                Double_t Threshold_NewLayer1_OptionU = 34.51;
                Double_t Threshold_NewLayer1_OptionV = 32.51;
                Double_t Threshold_NewLayer1_OptionW = 31.51;
                Double_t Threshold_NewLayer1_OptionX = 30.51;
                Double_t Threshold_NewLayer1_OptionY = 28.51;
                Double_t Threshold_NewLayer1_OptionZ = 27.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="02" and opt=="2")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 27.51;
                Double_t Threshold_NewLayer1_OptionB = 26.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 39.51;
                Double_t Threshold_NewLayer1_OptionI = 37.51;
                Double_t Threshold_NewLayer1_OptionJ = 36.51;
                Double_t Threshold_NewLayer1_OptionK = 34.51;
                Double_t Threshold_NewLayer1_OptionL = 32.51;
                Double_t Threshold_NewLayer1_OptionM = 31.51;
                Double_t Threshold_NewLayer1_OptionN = 30.51;
                Double_t Threshold_NewLayer1_OptionO = 29.51;
                Double_t Threshold_NewLayer1_OptionP = 27.51;
                Double_t Threshold_NewLayer1_OptionQ = 26.51;
                Double_t Threshold_NewLayer1_OptionR = 26.51;
                Double_t Threshold_NewLayer1_OptionS = 26.51;
                Double_t Threshold_NewLayer1_OptionT = 26.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 41.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 39.51;
                Double_t Threshold_NewLayer1_OptionZ = 37.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="02" and opt=="3")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 36.51;
                Double_t Threshold_NewLayer1_OptionB = 33.51;
                Double_t Threshold_NewLayer1_OptionC = 31.51;
                Double_t Threshold_NewLayer1_OptionD = 30.51;
                Double_t Threshold_NewLayer1_OptionE = 29.51;
                Double_t Threshold_NewLayer1_OptionF = 28.51;
                Double_t Threshold_NewLayer1_OptionG = 26.51;
                Double_t Threshold_NewLayer1_OptionH = 26.51;
                Double_t Threshold_NewLayer1_OptionI = 26.51;
                Double_t Threshold_NewLayer1_OptionJ = 26.51;
                Double_t Threshold_NewLayer1_OptionK = 26.51;
                Double_t Threshold_NewLayer1_OptionL = 26.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 41.51;
                Double_t Threshold_NewLayer1_OptionO = 40.51;
                Double_t Threshold_NewLayer1_OptionP = 39.51;
                Double_t Threshold_NewLayer1_OptionQ = 36.51;
                Double_t Threshold_NewLayer1_OptionR = 35.51;
                Double_t Threshold_NewLayer1_OptionS = 32.51;
                Double_t Threshold_NewLayer1_OptionT = 30.51;
                Double_t Threshold_NewLayer1_OptionU = 28.51;
                Double_t Threshold_NewLayer1_OptionV = 28.51;
                Double_t Threshold_NewLayer1_OptionW = 27.51;
                Double_t Threshold_NewLayer1_OptionX = 26.51;
                Double_t Threshold_NewLayer1_OptionY = 26.51;
                Double_t Threshold_NewLayer1_OptionZ = 26.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="02" and opt=="4")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 25.51;
                Double_t Threshold_NewLayer1_OptionB = 25.51;
                Double_t Threshold_NewLayer1_OptionC = 25.51;
                Double_t Threshold_NewLayer1_OptionD = 25.51;
                Double_t Threshold_NewLayer1_OptionE = 41.51;
                Double_t Threshold_NewLayer1_OptionF = 40.51;
                Double_t Threshold_NewLayer1_OptionG = 39.51;
                Double_t Threshold_NewLayer1_OptionH = 36.51;
                Double_t Threshold_NewLayer1_OptionI = 34.51;
                Double_t Threshold_NewLayer1_OptionJ = 30.51;
                Double_t Threshold_NewLayer1_OptionK = 28.51;
                Double_t Threshold_NewLayer1_OptionL = 27.51;
                Double_t Threshold_NewLayer1_OptionM = 26.51;
                Double_t Threshold_NewLayer1_OptionN = 26.51;
                Double_t Threshold_NewLayer1_OptionO = 26.51;
                Double_t Threshold_NewLayer1_OptionP = 25.51;
                Double_t Threshold_NewLayer1_OptionQ = 25.51;
                Double_t Threshold_NewLayer1_OptionR = 25.51;
                Double_t Threshold_NewLayer1_OptionS = 25.51;
                Double_t Threshold_NewLayer1_OptionT = 25.51;
                Double_t Threshold_NewLayer1_OptionU = 25.51;
                Double_t Threshold_NewLayer1_OptionV = 25.51;
                Double_t Threshold_NewLayer1_OptionW = 40.51;
                Double_t Threshold_NewLayer1_OptionX = 38.51;
                Double_t Threshold_NewLayer1_OptionY = 34.51;
                Double_t Threshold_NewLayer1_OptionZ = 32.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="02" and opt=="5")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 28.51;
                Double_t Threshold_NewLayer1_OptionB = 26.51;
                Double_t Threshold_NewLayer1_OptionC = 26.51;
                Double_t Threshold_NewLayer1_OptionD = 26.51;
                Double_t Threshold_NewLayer1_OptionE = 26.51;
                Double_t Threshold_NewLayer1_OptionF = 25.51;
                Double_t Threshold_NewLayer1_OptionG = 25.51;
                Double_t Threshold_NewLayer1_OptionH = 25.51;
                Double_t Threshold_NewLayer1_OptionI = 25.51;
                Double_t Threshold_NewLayer1_OptionJ = 25.51;
                Double_t Threshold_NewLayer1_OptionK = 25.51;
                Double_t Threshold_NewLayer1_OptionL = 25.51;
                Double_t Threshold_NewLayer1_OptionM = 25.51;
                Double_t Threshold_NewLayer1_OptionN = 25.51;
                Double_t Threshold_NewLayer1_OptionO = 37.51;
                Double_t Threshold_NewLayer1_OptionP = 32.51;
                Double_t Threshold_NewLayer1_OptionQ = 30.51;
                Double_t Threshold_NewLayer1_OptionR = 27.51;
                Double_t Threshold_NewLayer1_OptionS = 26.51;
                Double_t Threshold_NewLayer1_OptionT = 26.51;
                Double_t Threshold_NewLayer1_OptionU = 26.51;
                Double_t Threshold_NewLayer1_OptionV = 25.51;
                Double_t Threshold_NewLayer1_OptionW = 25.51;
                Double_t Threshold_NewLayer1_OptionX = 25.51;
                Double_t Threshold_NewLayer1_OptionY = 25.51;
                Double_t Threshold_NewLayer1_OptionZ = 25.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="02" and opt=="6")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 25.51;
                Double_t Threshold_NewLayer1_OptionB = 25.51;
                Double_t Threshold_NewLayer1_OptionC = 25.51;
                Double_t Threshold_NewLayer1_OptionD = 25.51;
                Double_t Threshold_NewLayer1_OptionE = 25.51;
                Double_t Threshold_NewLayer1_OptionF = 25.51;
                Double_t Threshold_NewLayer1_OptionG = 29.51;
                Double_t Threshold_NewLayer1_OptionH = 28.51;
                Double_t Threshold_NewLayer1_OptionI = 26.51;
                Double_t Threshold_NewLayer1_OptionJ = 26.51;
                Double_t Threshold_NewLayer1_OptionK = 26.51;
                Double_t Threshold_NewLayer1_OptionL = 26.51;
                Double_t Threshold_NewLayer1_OptionM = 25.51;
                Double_t Threshold_NewLayer1_OptionN = 25.51;
                Double_t Threshold_NewLayer1_OptionO = 25.51;
                Double_t Threshold_NewLayer1_OptionP = 25.51;
                Double_t Threshold_NewLayer1_OptionQ = 25.51;
                Double_t Threshold_NewLayer1_OptionR = 25.51;
                Double_t Threshold_NewLayer1_OptionS = 25.51;
                Double_t Threshold_NewLayer1_OptionT = 25.51;
                Double_t Threshold_NewLayer1_OptionU = 25.51;
                Double_t Threshold_NewLayer1_OptionV = 25.51;
                Double_t Threshold_NewLayer1_OptionW = 25.51;
                Double_t Threshold_NewLayer1_OptionX = 25.51;
                Double_t Threshold_NewLayer1_OptionY = 26.51;
                Double_t Threshold_NewLayer1_OptionZ = 26.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="02" and opt=="7")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 26.51;
                Double_t Threshold_NewLayer1_OptionB = 26.51;
                Double_t Threshold_NewLayer1_OptionC = 25.51;
                Double_t Threshold_NewLayer1_OptionD = 25.51;
                Double_t Threshold_NewLayer1_OptionE = 25.51;
                Double_t Threshold_NewLayer1_OptionF = 25.51;
                Double_t Threshold_NewLayer1_OptionG = 25.51;
                Double_t Threshold_NewLayer1_OptionH = 25.51;
                Double_t Threshold_NewLayer1_OptionI = 25.51;
                Double_t Threshold_NewLayer1_OptionJ = 25.51;
                Double_t Threshold_NewLayer1_OptionK = 25.51;
                Double_t Threshold_NewLayer1_OptionL = 25.51;
                Double_t Threshold_NewLayer1_OptionM = 25.51;
                Double_t Threshold_NewLayer1_OptionN = 25.51;
                Double_t Threshold_NewLayer1_OptionO = 25.51;
                Double_t Threshold_NewLayer1_OptionP = 25.51;
                Double_t Threshold_NewLayer1_OptionQ = 26.51;
                Double_t Threshold_NewLayer1_OptionR = 26.51;
                Double_t Threshold_NewLayer1_OptionS = 25.51;
                Double_t Threshold_NewLayer1_OptionT = 25.51;
                Double_t Threshold_NewLayer1_OptionU = 25.51;
                Double_t Threshold_NewLayer1_OptionV = 25.51;
                Double_t Threshold_NewLayer1_OptionW = 25.51;
                Double_t Threshold_NewLayer1_OptionX = 25.51;
                Double_t Threshold_NewLayer1_OptionY = 25.51;
                Double_t Threshold_NewLayer1_OptionZ = 25.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="02" and opt=="8")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 25.51;
                Double_t Threshold_NewLayer1_OptionB = 25.51;
                Double_t Threshold_NewLayer1_OptionC = 25.51;
                Double_t Threshold_NewLayer1_OptionD = 25.51;
                Double_t Threshold_NewLayer1_OptionE = 25.51;
                Double_t Threshold_NewLayer1_OptionF = 25.51;
                Double_t Threshold_NewLayer1_OptionG = 25.51;
                Double_t Threshold_NewLayer1_OptionH = 25.51;
                Double_t Threshold_NewLayer1_OptionI = 25.51;
                Double_t Threshold_NewLayer1_OptionJ = 25.51;
                Double_t Threshold_NewLayer1_OptionK = 25.51;
                Double_t Threshold_NewLayer1_OptionL = 25.51;
                Double_t Threshold_NewLayer1_OptionM = 25.51;
                Double_t Threshold_NewLayer1_OptionN = 25.51;
                Double_t Threshold_NewLayer1_OptionO = 25.51;
                Double_t Threshold_NewLayer1_OptionP = 25.51;
                Double_t Threshold_NewLayer1_OptionQ = 25.51;
                Double_t Threshold_NewLayer1_OptionR = 25.51;
                Double_t Threshold_NewLayer1_OptionS = 25.51;
                Double_t Threshold_NewLayer1_OptionT = 25.51;
                Double_t Threshold_NewLayer1_OptionU = 25.51;
                Double_t Threshold_NewLayer1_OptionV = 25.51;
                Double_t Threshold_NewLayer1_OptionW = 25.51;
                Double_t Threshold_NewLayer1_OptionX = 25.51;
                Double_t Threshold_NewLayer1_OptionY = 25.51;
                Double_t Threshold_NewLayer1_OptionZ = 25.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="03" and opt=="0")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 42.51;
                Double_t Threshold_NewLayer1_OptionB = 42.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 42.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 41.51;
                Double_t Threshold_NewLayer1_OptionI = 40.51;
                Double_t Threshold_NewLayer1_OptionJ = 39.51;
                Double_t Threshold_NewLayer1_OptionK = 37.51;
                Double_t Threshold_NewLayer1_OptionL = 36.51;
                Double_t Threshold_NewLayer1_OptionM = 35.51;
                Double_t Threshold_NewLayer1_OptionN = 34.51;
                Double_t Threshold_NewLayer1_OptionO = 33.51;
                Double_t Threshold_NewLayer1_OptionP = 33.51;
                Double_t Threshold_NewLayer1_OptionQ = 31.51;
                Double_t Threshold_NewLayer1_OptionR = 30.51;
                Double_t Threshold_NewLayer1_OptionS = 42.51;
                Double_t Threshold_NewLayer1_OptionT = 42.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 42.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 41.51;
                Double_t Threshold_NewLayer1_OptionZ = 40.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="03" and opt=="1")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 38.51;
                Double_t Threshold_NewLayer1_OptionB = 37.51;
                Double_t Threshold_NewLayer1_OptionC = 36.51;
                Double_t Threshold_NewLayer1_OptionD = 35.51;
                Double_t Threshold_NewLayer1_OptionE = 34.51;
                Double_t Threshold_NewLayer1_OptionF = 33.51;
                Double_t Threshold_NewLayer1_OptionG = 32.51;
                Double_t Threshold_NewLayer1_OptionH = 30.51;
                Double_t Threshold_NewLayer1_OptionI = 29.51;
                Double_t Threshold_NewLayer1_OptionJ = 29.51;
                Double_t Threshold_NewLayer1_OptionK = 42.51;
                Double_t Threshold_NewLayer1_OptionL = 42.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 42.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 41.51;
                Double_t Threshold_NewLayer1_OptionQ = 40.51;
                Double_t Threshold_NewLayer1_OptionR = 38.51;
                Double_t Threshold_NewLayer1_OptionS = 37.51;
                Double_t Threshold_NewLayer1_OptionT = 36.51;
                Double_t Threshold_NewLayer1_OptionU = 35.51;
                Double_t Threshold_NewLayer1_OptionV = 34.51;
                Double_t Threshold_NewLayer1_OptionW = 33.51;
                Double_t Threshold_NewLayer1_OptionX = 32.51;
                Double_t Threshold_NewLayer1_OptionY = 30.51;
                Double_t Threshold_NewLayer1_OptionZ = 29.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="03" and opt=="2")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 28.51;
                Double_t Threshold_NewLayer1_OptionB = 28.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 40.51;
                Double_t Threshold_NewLayer1_OptionI = 37.51;
                Double_t Threshold_NewLayer1_OptionJ = 37.51;
                Double_t Threshold_NewLayer1_OptionK = 35.51;
                Double_t Threshold_NewLayer1_OptionL = 34.51;
                Double_t Threshold_NewLayer1_OptionM = 33.51;
                Double_t Threshold_NewLayer1_OptionN = 32.51;
                Double_t Threshold_NewLayer1_OptionO = 31.51;
                Double_t Threshold_NewLayer1_OptionP = 29.51;
                Double_t Threshold_NewLayer1_OptionQ = 28.51;
                Double_t Threshold_NewLayer1_OptionR = 28.51;
                Double_t Threshold_NewLayer1_OptionS = 28.51;
                Double_t Threshold_NewLayer1_OptionT = 27.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 41.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 39.51;
                Double_t Threshold_NewLayer1_OptionZ = 37.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="03" and opt=="3")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 36.51;
                Double_t Threshold_NewLayer1_OptionB = 34.51;
                Double_t Threshold_NewLayer1_OptionC = 33.51;
                Double_t Threshold_NewLayer1_OptionD = 32.51;
                Double_t Threshold_NewLayer1_OptionE = 31.51;
                Double_t Threshold_NewLayer1_OptionF = 30.51;
                Double_t Threshold_NewLayer1_OptionG = 28.51;
                Double_t Threshold_NewLayer1_OptionH = 28.51;
                Double_t Threshold_NewLayer1_OptionI = 27.51;
                Double_t Threshold_NewLayer1_OptionJ = 27.51;
                Double_t Threshold_NewLayer1_OptionK = 26.51;
                Double_t Threshold_NewLayer1_OptionL = 26.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 41.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 39.51;
                Double_t Threshold_NewLayer1_OptionQ = 37.51;
                Double_t Threshold_NewLayer1_OptionR = 36.51;
                Double_t Threshold_NewLayer1_OptionS = 33.51;
                Double_t Threshold_NewLayer1_OptionT = 32.51;
                Double_t Threshold_NewLayer1_OptionU = 31.51;
                Double_t Threshold_NewLayer1_OptionV = 30.51;
                Double_t Threshold_NewLayer1_OptionW = 29.51;
                Double_t Threshold_NewLayer1_OptionX = 27.51;
                Double_t Threshold_NewLayer1_OptionY = 27.51;
                Double_t Threshold_NewLayer1_OptionZ = 26.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="03" and opt=="4")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 26.51;
                Double_t Threshold_NewLayer1_OptionB = 26.51;
                Double_t Threshold_NewLayer1_OptionC = 26.51;
                Double_t Threshold_NewLayer1_OptionD = 26.51;
                Double_t Threshold_NewLayer1_OptionE = 41.51;
                Double_t Threshold_NewLayer1_OptionF = 40.51;
                Double_t Threshold_NewLayer1_OptionG = 39.51;
                Double_t Threshold_NewLayer1_OptionH = 36.51;
                Double_t Threshold_NewLayer1_OptionI = 35.51;
                Double_t Threshold_NewLayer1_OptionJ = 32.51;
                Double_t Threshold_NewLayer1_OptionK = 31.51;
                Double_t Threshold_NewLayer1_OptionL = 30.51;
                Double_t Threshold_NewLayer1_OptionM = 29.51;
                Double_t Threshold_NewLayer1_OptionN = 28.51;
                Double_t Threshold_NewLayer1_OptionO = 27.51;
                Double_t Threshold_NewLayer1_OptionP = 26.51;
                Double_t Threshold_NewLayer1_OptionQ = 26.51;
                Double_t Threshold_NewLayer1_OptionR = 26.51;
                Double_t Threshold_NewLayer1_OptionS = 26.51;
                Double_t Threshold_NewLayer1_OptionT = 26.51;
                Double_t Threshold_NewLayer1_OptionU = 26.51;
                Double_t Threshold_NewLayer1_OptionV = 26.51;
                Double_t Threshold_NewLayer1_OptionW = 40.51;
                Double_t Threshold_NewLayer1_OptionX = 39.51;
                Double_t Threshold_NewLayer1_OptionY = 36.51;
                Double_t Threshold_NewLayer1_OptionZ = 34.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="03" and opt=="5")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 31.51;
                Double_t Threshold_NewLayer1_OptionB = 29.51;
                Double_t Threshold_NewLayer1_OptionC = 28.51;
                Double_t Threshold_NewLayer1_OptionD = 28.51;
                Double_t Threshold_NewLayer1_OptionE = 26.51;
                Double_t Threshold_NewLayer1_OptionF = 26.51;
                Double_t Threshold_NewLayer1_OptionG = 26.51;
                Double_t Threshold_NewLayer1_OptionH = 26.51;
                Double_t Threshold_NewLayer1_OptionI = 26.51;
                Double_t Threshold_NewLayer1_OptionJ = 26.51;
                Double_t Threshold_NewLayer1_OptionK = 26.51;
                Double_t Threshold_NewLayer1_OptionL = 26.51;
                Double_t Threshold_NewLayer1_OptionM = 26.51;
                Double_t Threshold_NewLayer1_OptionN = 26.51;
                Double_t Threshold_NewLayer1_OptionO = 38.51;
                Double_t Threshold_NewLayer1_OptionP = 34.51;
                Double_t Threshold_NewLayer1_OptionQ = 32.51;
                Double_t Threshold_NewLayer1_OptionR = 30.51;
                Double_t Threshold_NewLayer1_OptionS = 28.51;
                Double_t Threshold_NewLayer1_OptionT = 28.51;
                Double_t Threshold_NewLayer1_OptionU = 27.51;
                Double_t Threshold_NewLayer1_OptionV = 26.51;
                Double_t Threshold_NewLayer1_OptionW = 26.51;
                Double_t Threshold_NewLayer1_OptionX = 26.51;
                Double_t Threshold_NewLayer1_OptionY = 26.51;
                Double_t Threshold_NewLayer1_OptionZ = 26.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="03" and opt=="6")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 26.51;
                Double_t Threshold_NewLayer1_OptionB = 26.51;
                Double_t Threshold_NewLayer1_OptionC = 26.51;
                Double_t Threshold_NewLayer1_OptionD = 26.51;
                Double_t Threshold_NewLayer1_OptionE = 26.51;
                Double_t Threshold_NewLayer1_OptionF = 26.51;
                Double_t Threshold_NewLayer1_OptionG = 32.51;
                Double_t Threshold_NewLayer1_OptionH = 30.51;
                Double_t Threshold_NewLayer1_OptionI = 28.51;
                Double_t Threshold_NewLayer1_OptionJ = 27.51;
                Double_t Threshold_NewLayer1_OptionK = 27.51;
                Double_t Threshold_NewLayer1_OptionL = 26.51;
                Double_t Threshold_NewLayer1_OptionM = 26.51;
                Double_t Threshold_NewLayer1_OptionN = 26.51;
                Double_t Threshold_NewLayer1_OptionO = 26.51;
                Double_t Threshold_NewLayer1_OptionP = 26.51;
                Double_t Threshold_NewLayer1_OptionQ = 26.51;
                Double_t Threshold_NewLayer1_OptionR = 26.51;
                Double_t Threshold_NewLayer1_OptionS = 26.51;
                Double_t Threshold_NewLayer1_OptionT = 26.51;
                Double_t Threshold_NewLayer1_OptionU = 26.51;
                Double_t Threshold_NewLayer1_OptionV = 26.51;
                Double_t Threshold_NewLayer1_OptionW = 26.51;
                Double_t Threshold_NewLayer1_OptionX = 25.51;
                Double_t Threshold_NewLayer1_OptionY = 29.51;
                Double_t Threshold_NewLayer1_OptionZ = 27.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="03" and opt=="7")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 26.51;
                Double_t Threshold_NewLayer1_OptionB = 26.51;
                Double_t Threshold_NewLayer1_OptionC = 26.51;
                Double_t Threshold_NewLayer1_OptionD = 26.51;
                Double_t Threshold_NewLayer1_OptionE = 26.51;
                Double_t Threshold_NewLayer1_OptionF = 26.51;
                Double_t Threshold_NewLayer1_OptionG = 26.51;
                Double_t Threshold_NewLayer1_OptionH = 26.51;
                Double_t Threshold_NewLayer1_OptionI = 26.51;
                Double_t Threshold_NewLayer1_OptionJ = 26.51;
                Double_t Threshold_NewLayer1_OptionK = 26.51;
                Double_t Threshold_NewLayer1_OptionL = 25.51;
                Double_t Threshold_NewLayer1_OptionM = 25.51;
                Double_t Threshold_NewLayer1_OptionN = 25.51;
                Double_t Threshold_NewLayer1_OptionO = 25.51;
                Double_t Threshold_NewLayer1_OptionP = 25.51;
                Double_t Threshold_NewLayer1_OptionQ = 27.51;
                Double_t Threshold_NewLayer1_OptionR = 26.51;
                Double_t Threshold_NewLayer1_OptionS = 26.51;
                Double_t Threshold_NewLayer1_OptionT = 26.51;
                Double_t Threshold_NewLayer1_OptionU = 26.51;
                Double_t Threshold_NewLayer1_OptionV = 26.51;
                Double_t Threshold_NewLayer1_OptionW = 26.51;
                Double_t Threshold_NewLayer1_OptionX = 26.51;
                Double_t Threshold_NewLayer1_OptionY = 26.51;
                Double_t Threshold_NewLayer1_OptionZ = 26.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="03" and opt=="8")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 25.51;
                Double_t Threshold_NewLayer1_OptionB = 25.51;
                Double_t Threshold_NewLayer1_OptionC = 25.51;
                Double_t Threshold_NewLayer1_OptionD = 25.51;
                Double_t Threshold_NewLayer1_OptionE = 25.51;
                Double_t Threshold_NewLayer1_OptionF = 25.51;
                Double_t Threshold_NewLayer1_OptionG = 25.51;
                Double_t Threshold_NewLayer1_OptionH = 25.51;
                Double_t Threshold_NewLayer1_OptionI = 26.51;
                Double_t Threshold_NewLayer1_OptionJ = 26.51;
                Double_t Threshold_NewLayer1_OptionK = 26.51;
                Double_t Threshold_NewLayer1_OptionL = 26.51;
                Double_t Threshold_NewLayer1_OptionM = 26.51;
                Double_t Threshold_NewLayer1_OptionN = 26.51;
                Double_t Threshold_NewLayer1_OptionO = 26.51;
                Double_t Threshold_NewLayer1_OptionP = 25.51;
                Double_t Threshold_NewLayer1_OptionQ = 25.51;
                Double_t Threshold_NewLayer1_OptionR = 25.51;
                Double_t Threshold_NewLayer1_OptionS = 25.51;
                Double_t Threshold_NewLayer1_OptionT = 25.51;
                Double_t Threshold_NewLayer1_OptionU = 25.51;
                Double_t Threshold_NewLayer1_OptionV = 25.51;
                Double_t Threshold_NewLayer1_OptionW = 25.51;
                Double_t Threshold_NewLayer1_OptionX = 25.51;
                Double_t Threshold_NewLayer1_OptionY = 25.51;
                Double_t Threshold_NewLayer1_OptionZ = 25.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="04" and opt=="0")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 42.51;
                Double_t Threshold_NewLayer1_OptionB = 42.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 42.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 41.51;
                Double_t Threshold_NewLayer1_OptionI = 40.51;
                Double_t Threshold_NewLayer1_OptionJ = 39.51;
                Double_t Threshold_NewLayer1_OptionK = 38.51;
                Double_t Threshold_NewLayer1_OptionL = 37.51;
                Double_t Threshold_NewLayer1_OptionM = 36.51;
                Double_t Threshold_NewLayer1_OptionN = 35.51;
                Double_t Threshold_NewLayer1_OptionO = 35.51;
                Double_t Threshold_NewLayer1_OptionP = 34.51;
                Double_t Threshold_NewLayer1_OptionQ = 32.51;
                Double_t Threshold_NewLayer1_OptionR = 32.51;
                Double_t Threshold_NewLayer1_OptionS = 42.51;
                Double_t Threshold_NewLayer1_OptionT = 42.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 42.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 41.51;
                Double_t Threshold_NewLayer1_OptionZ = 40.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="04" and opt=="1")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 39.51;
                Double_t Threshold_NewLayer1_OptionB = 38.51;
                Double_t Threshold_NewLayer1_OptionC = 37.51;
                Double_t Threshold_NewLayer1_OptionD = 36.51;
                Double_t Threshold_NewLayer1_OptionE = 35.51;
                Double_t Threshold_NewLayer1_OptionF = 34.51;
                Double_t Threshold_NewLayer1_OptionG = 34.51;
                Double_t Threshold_NewLayer1_OptionH = 32.51;
                Double_t Threshold_NewLayer1_OptionI = 31.51;
                Double_t Threshold_NewLayer1_OptionJ = 30.51;
                Double_t Threshold_NewLayer1_OptionK = 42.51;
                Double_t Threshold_NewLayer1_OptionL = 42.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 42.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 41.51;
                Double_t Threshold_NewLayer1_OptionQ = 40.51;
                Double_t Threshold_NewLayer1_OptionR = 38.51;
                Double_t Threshold_NewLayer1_OptionS = 37.51;
                Double_t Threshold_NewLayer1_OptionT = 36.51;
                Double_t Threshold_NewLayer1_OptionU = 36.51;
                Double_t Threshold_NewLayer1_OptionV = 35.51;
                Double_t Threshold_NewLayer1_OptionW = 34.51;
                Double_t Threshold_NewLayer1_OptionX = 33.51;
                Double_t Threshold_NewLayer1_OptionY = 31.51;
                Double_t Threshold_NewLayer1_OptionZ = 30.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="04" and opt=="2")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 30.51;
                Double_t Threshold_NewLayer1_OptionB = 29.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 40.51;
                Double_t Threshold_NewLayer1_OptionI = 38.51;
                Double_t Threshold_NewLayer1_OptionJ = 37.51;
                Double_t Threshold_NewLayer1_OptionK = 36.51;
                Double_t Threshold_NewLayer1_OptionL = 35.51;
                Double_t Threshold_NewLayer1_OptionM = 34.51;
                Double_t Threshold_NewLayer1_OptionN = 33.51;
                Double_t Threshold_NewLayer1_OptionO = 33.51;
                Double_t Threshold_NewLayer1_OptionP = 31.51;
                Double_t Threshold_NewLayer1_OptionQ = 30.51;
                Double_t Threshold_NewLayer1_OptionR = 29.51;
                Double_t Threshold_NewLayer1_OptionS = 29.51;
                Double_t Threshold_NewLayer1_OptionT = 29.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 41.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 40.51;
                Double_t Threshold_NewLayer1_OptionZ = 38.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="04" and opt=="3")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 37.51;
                Double_t Threshold_NewLayer1_OptionB = 36.51;
                Double_t Threshold_NewLayer1_OptionC = 34.51;
                Double_t Threshold_NewLayer1_OptionD = 34.51;
                Double_t Threshold_NewLayer1_OptionE = 33.51;
                Double_t Threshold_NewLayer1_OptionF = 32.51;
                Double_t Threshold_NewLayer1_OptionG = 30.51;
                Double_t Threshold_NewLayer1_OptionH = 29.51;
                Double_t Threshold_NewLayer1_OptionI = 29.51;
                Double_t Threshold_NewLayer1_OptionJ = 28.51;
                Double_t Threshold_NewLayer1_OptionK = 28.51;
                Double_t Threshold_NewLayer1_OptionL = 28.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 41.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 39.51;
                Double_t Threshold_NewLayer1_OptionQ = 37.51;
                Double_t Threshold_NewLayer1_OptionR = 37.51;
                Double_t Threshold_NewLayer1_OptionS = 35.51;
                Double_t Threshold_NewLayer1_OptionT = 34.51;
                Double_t Threshold_NewLayer1_OptionU = 33.51;
                Double_t Threshold_NewLayer1_OptionV = 32.51;
                Double_t Threshold_NewLayer1_OptionW = 32.51;
                Double_t Threshold_NewLayer1_OptionX = 29.51;
                Double_t Threshold_NewLayer1_OptionY = 29.51;
                Double_t Threshold_NewLayer1_OptionZ = 28.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="04" and opt=="4")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 28.51;
                Double_t Threshold_NewLayer1_OptionB = 28.51;
                Double_t Threshold_NewLayer1_OptionC = 28.51;
                Double_t Threshold_NewLayer1_OptionD = 28.51;
                Double_t Threshold_NewLayer1_OptionE = 41.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 39.51;
                Double_t Threshold_NewLayer1_OptionH = 37.51;
                Double_t Threshold_NewLayer1_OptionI = 36.51;
                Double_t Threshold_NewLayer1_OptionJ = 34.51;
                Double_t Threshold_NewLayer1_OptionK = 33.51;
                Double_t Threshold_NewLayer1_OptionL = 32.51;
                Double_t Threshold_NewLayer1_OptionM = 31.51;
                Double_t Threshold_NewLayer1_OptionN = 31.51;
                Double_t Threshold_NewLayer1_OptionO = 29.51;
                Double_t Threshold_NewLayer1_OptionP = 28.51;
                Double_t Threshold_NewLayer1_OptionQ = 28.51;
                Double_t Threshold_NewLayer1_OptionR = 28.51;
                Double_t Threshold_NewLayer1_OptionS = 28.51;
                Double_t Threshold_NewLayer1_OptionT = 27.51;
                Double_t Threshold_NewLayer1_OptionU = 27.51;
                Double_t Threshold_NewLayer1_OptionV = 27.51;
                Double_t Threshold_NewLayer1_OptionW = 40.51;
                Double_t Threshold_NewLayer1_OptionX = 39.51;
                Double_t Threshold_NewLayer1_OptionY = 37.51;
                Double_t Threshold_NewLayer1_OptionZ = 35.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="04" and opt=="5")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 33.51;
                Double_t Threshold_NewLayer1_OptionB = 32.51;
                Double_t Threshold_NewLayer1_OptionC = 31.51;
                Double_t Threshold_NewLayer1_OptionD = 30.51;
                Double_t Threshold_NewLayer1_OptionE = 29.51;
                Double_t Threshold_NewLayer1_OptionF = 28.51;
                Double_t Threshold_NewLayer1_OptionG = 28.51;
                Double_t Threshold_NewLayer1_OptionH = 28.51;
                Double_t Threshold_NewLayer1_OptionI = 27.51;
                Double_t Threshold_NewLayer1_OptionJ = 27.51;
                Double_t Threshold_NewLayer1_OptionK = 27.51;
                Double_t Threshold_NewLayer1_OptionL = 27.51;
                Double_t Threshold_NewLayer1_OptionM = 27.51;
                Double_t Threshold_NewLayer1_OptionN = 27.51;
                Double_t Threshold_NewLayer1_OptionO = 39.51;
                Double_t Threshold_NewLayer1_OptionP = 36.51;
                Double_t Threshold_NewLayer1_OptionQ = 34.51;
                Double_t Threshold_NewLayer1_OptionR = 32.51;
                Double_t Threshold_NewLayer1_OptionS = 31.51;
                Double_t Threshold_NewLayer1_OptionT = 30.51;
                Double_t Threshold_NewLayer1_OptionU = 29.51;
                Double_t Threshold_NewLayer1_OptionV = 28.51;
                Double_t Threshold_NewLayer1_OptionW = 28.51;
                Double_t Threshold_NewLayer1_OptionX = 27.51;
                Double_t Threshold_NewLayer1_OptionY = 27.51;
                Double_t Threshold_NewLayer1_OptionZ = 27.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="04" and opt=="6")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 27.51;
                Double_t Threshold_NewLayer1_OptionB = 27.51;
                Double_t Threshold_NewLayer1_OptionC = 27.51;
                Double_t Threshold_NewLayer1_OptionD = 27.51;
                Double_t Threshold_NewLayer1_OptionE = 26.51;
                Double_t Threshold_NewLayer1_OptionF = 26.51;
                Double_t Threshold_NewLayer1_OptionG = 34.51;
                Double_t Threshold_NewLayer1_OptionH = 33.51;
                Double_t Threshold_NewLayer1_OptionI = 31.51;
                Double_t Threshold_NewLayer1_OptionJ = 30.51;
                Double_t Threshold_NewLayer1_OptionK = 29.51;
                Double_t Threshold_NewLayer1_OptionL = 29.51;
                Double_t Threshold_NewLayer1_OptionM = 28.51;
                Double_t Threshold_NewLayer1_OptionN = 27.51;
                Double_t Threshold_NewLayer1_OptionO = 27.51;
                Double_t Threshold_NewLayer1_OptionP = 27.51;
                Double_t Threshold_NewLayer1_OptionQ = 27.51;
                Double_t Threshold_NewLayer1_OptionR = 27.51;
                Double_t Threshold_NewLayer1_OptionS = 26.51;
                Double_t Threshold_NewLayer1_OptionT = 26.51;
                Double_t Threshold_NewLayer1_OptionU = 26.51;
                Double_t Threshold_NewLayer1_OptionV = 26.51;
                Double_t Threshold_NewLayer1_OptionW = 26.51;
                Double_t Threshold_NewLayer1_OptionX = 26.51;
                Double_t Threshold_NewLayer1_OptionY = 32.51;
                Double_t Threshold_NewLayer1_OptionZ = 30.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="04" and opt=="7")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 29.51;
                Double_t Threshold_NewLayer1_OptionB = 29.51;
                Double_t Threshold_NewLayer1_OptionC = 29.51;
                Double_t Threshold_NewLayer1_OptionD = 28.51;
                Double_t Threshold_NewLayer1_OptionE = 27.51;
                Double_t Threshold_NewLayer1_OptionF = 27.51;
                Double_t Threshold_NewLayer1_OptionG = 26.51;
                Double_t Threshold_NewLayer1_OptionH = 26.51;
                Double_t Threshold_NewLayer1_OptionI = 26.51;
                Double_t Threshold_NewLayer1_OptionJ = 26.51;
                Double_t Threshold_NewLayer1_OptionK = 26.51;
                Double_t Threshold_NewLayer1_OptionL = 26.51;
                Double_t Threshold_NewLayer1_OptionM = 26.51;
                Double_t Threshold_NewLayer1_OptionN = 26.51;
                Double_t Threshold_NewLayer1_OptionO = 26.51;
                Double_t Threshold_NewLayer1_OptionP = 26.51;
                Double_t Threshold_NewLayer1_OptionQ = 29.51;
                Double_t Threshold_NewLayer1_OptionR = 29.51;
                Double_t Threshold_NewLayer1_OptionS = 28.51;
                Double_t Threshold_NewLayer1_OptionT = 28.51;
                Double_t Threshold_NewLayer1_OptionU = 27.51;
                Double_t Threshold_NewLayer1_OptionV = 27.51;
                Double_t Threshold_NewLayer1_OptionW = 26.51;
                Double_t Threshold_NewLayer1_OptionX = 26.51;
                Double_t Threshold_NewLayer1_OptionY = 26.51;
                Double_t Threshold_NewLayer1_OptionZ = 26.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="04" and opt=="8")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 26.51;
                Double_t Threshold_NewLayer1_OptionB = 26.51;
                Double_t Threshold_NewLayer1_OptionC = 26.51;
                Double_t Threshold_NewLayer1_OptionD = 26.51;
                Double_t Threshold_NewLayer1_OptionE = 26.51;
                Double_t Threshold_NewLayer1_OptionF = 26.51;
                Double_t Threshold_NewLayer1_OptionG = 26.51;
                Double_t Threshold_NewLayer1_OptionH = 26.51;
                Double_t Threshold_NewLayer1_OptionI = 28.51;
                Double_t Threshold_NewLayer1_OptionJ = 28.51;
                Double_t Threshold_NewLayer1_OptionK = 28.51;
                Double_t Threshold_NewLayer1_OptionL = 27.51;
                Double_t Threshold_NewLayer1_OptionM = 27.51;
                Double_t Threshold_NewLayer1_OptionN = 26.51;
                Double_t Threshold_NewLayer1_OptionO = 26.51;
                Double_t Threshold_NewLayer1_OptionP = 26.51;
                Double_t Threshold_NewLayer1_OptionQ = 26.51;
                Double_t Threshold_NewLayer1_OptionR = 26.51;
                Double_t Threshold_NewLayer1_OptionS = 26.51;
                Double_t Threshold_NewLayer1_OptionT = 26.51;
                Double_t Threshold_NewLayer1_OptionU = 26.51;
                Double_t Threshold_NewLayer1_OptionV = 26.51;
                Double_t Threshold_NewLayer1_OptionW = 26.51;
                Double_t Threshold_NewLayer1_OptionX = 26.51;
                Double_t Threshold_NewLayer1_OptionY = 26.51;
                Double_t Threshold_NewLayer1_OptionZ = 26.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="05" and opt=="0")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 42.51;
                Double_t Threshold_NewLayer1_OptionB = 42.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 42.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 41.51;
                Double_t Threshold_NewLayer1_OptionI = 40.51;
                Double_t Threshold_NewLayer1_OptionJ = 40.51;
                Double_t Threshold_NewLayer1_OptionK = 39.51;
                Double_t Threshold_NewLayer1_OptionL = 37.51;
                Double_t Threshold_NewLayer1_OptionM = 36.51;
                Double_t Threshold_NewLayer1_OptionN = 36.51;
                Double_t Threshold_NewLayer1_OptionO = 36.51;
                Double_t Threshold_NewLayer1_OptionP = 35.51;
                Double_t Threshold_NewLayer1_OptionQ = 33.51;
                Double_t Threshold_NewLayer1_OptionR = 33.51;
                Double_t Threshold_NewLayer1_OptionS = 42.51;
                Double_t Threshold_NewLayer1_OptionT = 42.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 42.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 41.51;
                Double_t Threshold_NewLayer1_OptionZ = 40.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="05" and opt=="1")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 39.51;
                Double_t Threshold_NewLayer1_OptionB = 39.51;
                Double_t Threshold_NewLayer1_OptionC = 37.51;
                Double_t Threshold_NewLayer1_OptionD = 36.51;
                Double_t Threshold_NewLayer1_OptionE = 36.51;
                Double_t Threshold_NewLayer1_OptionF = 35.51;
                Double_t Threshold_NewLayer1_OptionG = 35.51;
                Double_t Threshold_NewLayer1_OptionH = 33.51;
                Double_t Threshold_NewLayer1_OptionI = 33.51;
                Double_t Threshold_NewLayer1_OptionJ = 32.51;
                Double_t Threshold_NewLayer1_OptionK = 42.51;
                Double_t Threshold_NewLayer1_OptionL = 42.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 42.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 41.51;
                Double_t Threshold_NewLayer1_OptionQ = 40.51;
                Double_t Threshold_NewLayer1_OptionR = 39.51;
                Double_t Threshold_NewLayer1_OptionS = 38.51;
                Double_t Threshold_NewLayer1_OptionT = 37.51;
                Double_t Threshold_NewLayer1_OptionU = 36.51;
                Double_t Threshold_NewLayer1_OptionV = 36.51;
                Double_t Threshold_NewLayer1_OptionW = 35.51;
                Double_t Threshold_NewLayer1_OptionX = 35.51;
                Double_t Threshold_NewLayer1_OptionY = 33.51;
                Double_t Threshold_NewLayer1_OptionZ = 32.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="05" and opt=="2")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 32.51;
                Double_t Threshold_NewLayer1_OptionB = 31.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 40.51;
                Double_t Threshold_NewLayer1_OptionI = 39.51;
                Double_t Threshold_NewLayer1_OptionJ = 38.51;
                Double_t Threshold_NewLayer1_OptionK = 36.51;
                Double_t Threshold_NewLayer1_OptionL = 36.51;
                Double_t Threshold_NewLayer1_OptionM = 35.51;
                Double_t Threshold_NewLayer1_OptionN = 35.51;
                Double_t Threshold_NewLayer1_OptionO = 34.51;
                Double_t Threshold_NewLayer1_OptionP = 32.51;
                Double_t Threshold_NewLayer1_OptionQ = 32.51;
                Double_t Threshold_NewLayer1_OptionR = 31.51;
                Double_t Threshold_NewLayer1_OptionS = 31.51;
                Double_t Threshold_NewLayer1_OptionT = 30.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 41.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 40.51;
                Double_t Threshold_NewLayer1_OptionZ = 38.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="05" and opt=="3")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 37.51;
                Double_t Threshold_NewLayer1_OptionB = 36.51;
                Double_t Threshold_NewLayer1_OptionC = 35.51;
                Double_t Threshold_NewLayer1_OptionD = 35.51;
                Double_t Threshold_NewLayer1_OptionE = 34.51;
                Double_t Threshold_NewLayer1_OptionF = 34.51;
                Double_t Threshold_NewLayer1_OptionG = 32.51;
                Double_t Threshold_NewLayer1_OptionH = 31.51;
                Double_t Threshold_NewLayer1_OptionI = 30.51;
                Double_t Threshold_NewLayer1_OptionJ = 30.51;
                Double_t Threshold_NewLayer1_OptionK = 30.51;
                Double_t Threshold_NewLayer1_OptionL = 30.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 41.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 40.51;
                Double_t Threshold_NewLayer1_OptionQ = 38.51;
                Double_t Threshold_NewLayer1_OptionR = 37.51;
                Double_t Threshold_NewLayer1_OptionS = 36.51;
                Double_t Threshold_NewLayer1_OptionT = 35.51;
                Double_t Threshold_NewLayer1_OptionU = 34.51;
                Double_t Threshold_NewLayer1_OptionV = 34.51;
                Double_t Threshold_NewLayer1_OptionW = 33.51;
                Double_t Threshold_NewLayer1_OptionX = 31.51;
                Double_t Threshold_NewLayer1_OptionY = 30.51;
                Double_t Threshold_NewLayer1_OptionZ = 30.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="05" and opt=="4")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 30.51;
                Double_t Threshold_NewLayer1_OptionB = 29.51;
                Double_t Threshold_NewLayer1_OptionC = 29.51;
                Double_t Threshold_NewLayer1_OptionD = 29.51;
                Double_t Threshold_NewLayer1_OptionE = 41.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 40.51;
                Double_t Threshold_NewLayer1_OptionH = 37.51;
                Double_t Threshold_NewLayer1_OptionI = 37.51;
                Double_t Threshold_NewLayer1_OptionJ = 36.51;
                Double_t Threshold_NewLayer1_OptionK = 34.51;
                Double_t Threshold_NewLayer1_OptionL = 34.51;
                Double_t Threshold_NewLayer1_OptionM = 33.51;
                Double_t Threshold_NewLayer1_OptionN = 33.51;
                Double_t Threshold_NewLayer1_OptionO = 31.51;
                Double_t Threshold_NewLayer1_OptionP = 30.51;
                Double_t Threshold_NewLayer1_OptionQ = 30.51;
                Double_t Threshold_NewLayer1_OptionR = 29.51;
                Double_t Threshold_NewLayer1_OptionS = 29.51;
                Double_t Threshold_NewLayer1_OptionT = 29.51;
                Double_t Threshold_NewLayer1_OptionU = 29.51;
                Double_t Threshold_NewLayer1_OptionV = 28.51;
                Double_t Threshold_NewLayer1_OptionW = 41.51;
                Double_t Threshold_NewLayer1_OptionX = 39.51;
                Double_t Threshold_NewLayer1_OptionY = 37.51;
                Double_t Threshold_NewLayer1_OptionZ = 36.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="05" and opt=="5")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 35.51;
                Double_t Threshold_NewLayer1_OptionB = 34.51;
                Double_t Threshold_NewLayer1_OptionC = 33.51;
                Double_t Threshold_NewLayer1_OptionD = 32.51;
                Double_t Threshold_NewLayer1_OptionE = 31.51;
                Double_t Threshold_NewLayer1_OptionF = 30.51;
                Double_t Threshold_NewLayer1_OptionG = 29.51;
                Double_t Threshold_NewLayer1_OptionH = 29.51;
                Double_t Threshold_NewLayer1_OptionI = 29.51;
                Double_t Threshold_NewLayer1_OptionJ = 28.51;
                Double_t Threshold_NewLayer1_OptionK = 28.51;
                Double_t Threshold_NewLayer1_OptionL = 28.51;
                Double_t Threshold_NewLayer1_OptionM = 28.51;
                Double_t Threshold_NewLayer1_OptionN = 28.51;
                Double_t Threshold_NewLayer1_OptionO = 39.51;
                Double_t Threshold_NewLayer1_OptionP = 37.51;
                Double_t Threshold_NewLayer1_OptionQ = 36.51;
                Double_t Threshold_NewLayer1_OptionR = 34.51;
                Double_t Threshold_NewLayer1_OptionS = 33.51;
                Double_t Threshold_NewLayer1_OptionT = 32.51;
                Double_t Threshold_NewLayer1_OptionU = 32.51;
                Double_t Threshold_NewLayer1_OptionV = 30.51;
                Double_t Threshold_NewLayer1_OptionW = 29.51;
                Double_t Threshold_NewLayer1_OptionX = 29.51;
                Double_t Threshold_NewLayer1_OptionY = 29.51;
                Double_t Threshold_NewLayer1_OptionZ = 28.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="05" and opt=="6")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 28.51;
                Double_t Threshold_NewLayer1_OptionB = 28.51;
                Double_t Threshold_NewLayer1_OptionC = 28.51;
                Double_t Threshold_NewLayer1_OptionD = 28.51;
                Double_t Threshold_NewLayer1_OptionE = 28.51;
                Double_t Threshold_NewLayer1_OptionF = 28.51;
                Double_t Threshold_NewLayer1_OptionG = 36.51;
                Double_t Threshold_NewLayer1_OptionH = 35.51;
                Double_t Threshold_NewLayer1_OptionI = 33.51;
                Double_t Threshold_NewLayer1_OptionJ = 32.51;
                Double_t Threshold_NewLayer1_OptionK = 31.51;
                Double_t Threshold_NewLayer1_OptionL = 31.51;
                Double_t Threshold_NewLayer1_OptionM = 29.51;
                Double_t Threshold_NewLayer1_OptionN = 29.51;
                Double_t Threshold_NewLayer1_OptionO = 28.51;
                Double_t Threshold_NewLayer1_OptionP = 28.51;
                Double_t Threshold_NewLayer1_OptionQ = 28.51;
                Double_t Threshold_NewLayer1_OptionR = 28.51;
                Double_t Threshold_NewLayer1_OptionS = 28.51;
                Double_t Threshold_NewLayer1_OptionT = 28.51;
                Double_t Threshold_NewLayer1_OptionU = 28.51;
                Double_t Threshold_NewLayer1_OptionV = 28.51;
                Double_t Threshold_NewLayer1_OptionW = 28.51;
                Double_t Threshold_NewLayer1_OptionX = 28.51;
                Double_t Threshold_NewLayer1_OptionY = 34.51;
                Double_t Threshold_NewLayer1_OptionZ = 32.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="05" and opt=="7")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 31.51;
                Double_t Threshold_NewLayer1_OptionB = 30.51;
                Double_t Threshold_NewLayer1_OptionC = 30.51;
                Double_t Threshold_NewLayer1_OptionD = 29.51;
                Double_t Threshold_NewLayer1_OptionE = 29.51;
                Double_t Threshold_NewLayer1_OptionF = 28.51;
                Double_t Threshold_NewLayer1_OptionG = 28.51;
                Double_t Threshold_NewLayer1_OptionH = 28.51;
                Double_t Threshold_NewLayer1_OptionI = 28.51;
                Double_t Threshold_NewLayer1_OptionJ = 28.51;
                Double_t Threshold_NewLayer1_OptionK = 28.51;
                Double_t Threshold_NewLayer1_OptionL = 28.51;
                Double_t Threshold_NewLayer1_OptionM = 28.51;
                Double_t Threshold_NewLayer1_OptionN = 28.51;
                Double_t Threshold_NewLayer1_OptionO = 28.51;
                Double_t Threshold_NewLayer1_OptionP = 27.51;
                Double_t Threshold_NewLayer1_OptionQ = 31.51;
                Double_t Threshold_NewLayer1_OptionR = 30.51;
                Double_t Threshold_NewLayer1_OptionS = 30.51;
                Double_t Threshold_NewLayer1_OptionT = 30.51;
                Double_t Threshold_NewLayer1_OptionU = 29.51;
                Double_t Threshold_NewLayer1_OptionV = 28.51;
                Double_t Threshold_NewLayer1_OptionW = 28.51;
                Double_t Threshold_NewLayer1_OptionX = 28.51;
                Double_t Threshold_NewLayer1_OptionY = 28.51;
                Double_t Threshold_NewLayer1_OptionZ = 28.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="05" and opt=="8")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 28.51;
                Double_t Threshold_NewLayer1_OptionB = 28.51;
                Double_t Threshold_NewLayer1_OptionC = 28.51;
                Double_t Threshold_NewLayer1_OptionD = 27.51;
                Double_t Threshold_NewLayer1_OptionE = 27.51;
                Double_t Threshold_NewLayer1_OptionF = 27.51;
                Double_t Threshold_NewLayer1_OptionG = 27.51;
                Double_t Threshold_NewLayer1_OptionH = 27.51;
                Double_t Threshold_NewLayer1_OptionI = 30.51;
                Double_t Threshold_NewLayer1_OptionJ = 30.51;
                Double_t Threshold_NewLayer1_OptionK = 29.51;
                Double_t Threshold_NewLayer1_OptionL = 29.51;
                Double_t Threshold_NewLayer1_OptionM = 28.51;
                Double_t Threshold_NewLayer1_OptionN = 28.51;
                Double_t Threshold_NewLayer1_OptionO = 28.51;
                Double_t Threshold_NewLayer1_OptionP = 28.51;
                Double_t Threshold_NewLayer1_OptionQ = 28.51;
                Double_t Threshold_NewLayer1_OptionR = 28.51;
                Double_t Threshold_NewLayer1_OptionS = 27.51;
                Double_t Threshold_NewLayer1_OptionT = 27.51;
                Double_t Threshold_NewLayer1_OptionU = 27.51;
                Double_t Threshold_NewLayer1_OptionV = 27.51;
                Double_t Threshold_NewLayer1_OptionW = 27.51;
                Double_t Threshold_NewLayer1_OptionX = 27.51;
                Double_t Threshold_NewLayer1_OptionY = 27.51;
                Double_t Threshold_NewLayer1_OptionZ = 26.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="06" and opt=="0")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 42.51;
                Double_t Threshold_NewLayer1_OptionB = 42.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 42.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 41.51;
                Double_t Threshold_NewLayer1_OptionI = 41.51;
                Double_t Threshold_NewLayer1_OptionJ = 40.51;
                Double_t Threshold_NewLayer1_OptionK = 39.51;
                Double_t Threshold_NewLayer1_OptionL = 38.51;
                Double_t Threshold_NewLayer1_OptionM = 37.51;
                Double_t Threshold_NewLayer1_OptionN = 37.51;
                Double_t Threshold_NewLayer1_OptionO = 36.51;
                Double_t Threshold_NewLayer1_OptionP = 36.51;
                Double_t Threshold_NewLayer1_OptionQ = 35.51;
                Double_t Threshold_NewLayer1_OptionR = 34.51;
                Double_t Threshold_NewLayer1_OptionS = 42.51;
                Double_t Threshold_NewLayer1_OptionT = 42.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 42.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 41.51;
                Double_t Threshold_NewLayer1_OptionZ = 41.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="06" and opt=="1")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 39.51;
                Double_t Threshold_NewLayer1_OptionB = 39.51;
                Double_t Threshold_NewLayer1_OptionC = 38.51;
                Double_t Threshold_NewLayer1_OptionD = 37.51;
                Double_t Threshold_NewLayer1_OptionE = 37.51;
                Double_t Threshold_NewLayer1_OptionF = 36.51;
                Double_t Threshold_NewLayer1_OptionG = 36.51;
                Double_t Threshold_NewLayer1_OptionH = 35.51;
                Double_t Threshold_NewLayer1_OptionI = 34.51;
                Double_t Threshold_NewLayer1_OptionJ = 33.51;
                Double_t Threshold_NewLayer1_OptionK = 42.51;
                Double_t Threshold_NewLayer1_OptionL = 42.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 42.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 41.51;
                Double_t Threshold_NewLayer1_OptionQ = 41.51;
                Double_t Threshold_NewLayer1_OptionR = 39.51;
                Double_t Threshold_NewLayer1_OptionS = 39.51;
                Double_t Threshold_NewLayer1_OptionT = 38.51;
                Double_t Threshold_NewLayer1_OptionU = 37.51;
                Double_t Threshold_NewLayer1_OptionV = 36.51;
                Double_t Threshold_NewLayer1_OptionW = 36.51;
                Double_t Threshold_NewLayer1_OptionX = 36.51;
                Double_t Threshold_NewLayer1_OptionY = 34.51;
                Double_t Threshold_NewLayer1_OptionZ = 33.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="06" and opt=="2")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 33.51;
                Double_t Threshold_NewLayer1_OptionB = 33.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 40.51;
                Double_t Threshold_NewLayer1_OptionI = 39.51;
                Double_t Threshold_NewLayer1_OptionJ = 39.51;
                Double_t Threshold_NewLayer1_OptionK = 37.51;
                Double_t Threshold_NewLayer1_OptionL = 37.51;
                Double_t Threshold_NewLayer1_OptionM = 36.51;
                Double_t Threshold_NewLayer1_OptionN = 36.51;
                Double_t Threshold_NewLayer1_OptionO = 36.51;
                Double_t Threshold_NewLayer1_OptionP = 33.51;
                Double_t Threshold_NewLayer1_OptionQ = 33.51;
                Double_t Threshold_NewLayer1_OptionR = 33.51;
                Double_t Threshold_NewLayer1_OptionS = 32.51;
                Double_t Threshold_NewLayer1_OptionT = 32.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 41.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 40.51;
                Double_t Threshold_NewLayer1_OptionZ = 39.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="06" and opt=="3")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 38.51;
                Double_t Threshold_NewLayer1_OptionB = 37.51;
                Double_t Threshold_NewLayer1_OptionC = 36.51;
                Double_t Threshold_NewLayer1_OptionD = 36.51;
                Double_t Threshold_NewLayer1_OptionE = 36.51;
                Double_t Threshold_NewLayer1_OptionF = 35.51;
                Double_t Threshold_NewLayer1_OptionG = 33.51;
                Double_t Threshold_NewLayer1_OptionH = 33.51;
                Double_t Threshold_NewLayer1_OptionI = 32.51;
                Double_t Threshold_NewLayer1_OptionJ = 32.51;
                Double_t Threshold_NewLayer1_OptionK = 32.51;
                Double_t Threshold_NewLayer1_OptionL = 32.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 41.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 40.51;
                Double_t Threshold_NewLayer1_OptionQ = 39.51;
                Double_t Threshold_NewLayer1_OptionR = 38.51;
                Double_t Threshold_NewLayer1_OptionS = 36.51;
                Double_t Threshold_NewLayer1_OptionT = 36.51;
                Double_t Threshold_NewLayer1_OptionU = 36.51;
                Double_t Threshold_NewLayer1_OptionV = 35.51;
                Double_t Threshold_NewLayer1_OptionW = 35.51;
                Double_t Threshold_NewLayer1_OptionX = 33.51;
                Double_t Threshold_NewLayer1_OptionY = 32.51;
                Double_t Threshold_NewLayer1_OptionZ = 32.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="06" and opt=="4")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 32.51;
                Double_t Threshold_NewLayer1_OptionB = 31.51;
                Double_t Threshold_NewLayer1_OptionC = 31.51;
                Double_t Threshold_NewLayer1_OptionD = 31.51;
                Double_t Threshold_NewLayer1_OptionE = 41.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 40.51;
                Double_t Threshold_NewLayer1_OptionH = 38.51;
                Double_t Threshold_NewLayer1_OptionI = 37.51;
                Double_t Threshold_NewLayer1_OptionJ = 36.51;
                Double_t Threshold_NewLayer1_OptionK = 36.51;
                Double_t Threshold_NewLayer1_OptionL = 35.51;
                Double_t Threshold_NewLayer1_OptionM = 35.51;
                Double_t Threshold_NewLayer1_OptionN = 34.51;
                Double_t Threshold_NewLayer1_OptionO = 33.51;
                Double_t Threshold_NewLayer1_OptionP = 32.51;
                Double_t Threshold_NewLayer1_OptionQ = 32.51;
                Double_t Threshold_NewLayer1_OptionR = 31.51;
                Double_t Threshold_NewLayer1_OptionS = 31.51;
                Double_t Threshold_NewLayer1_OptionT = 31.51;
                Double_t Threshold_NewLayer1_OptionU = 31.51;
                Double_t Threshold_NewLayer1_OptionV = 30.51;
                Double_t Threshold_NewLayer1_OptionW = 41.51;
                Double_t Threshold_NewLayer1_OptionX = 40.51;
                Double_t Threshold_NewLayer1_OptionY = 38.51;
                Double_t Threshold_NewLayer1_OptionZ = 37.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="06" and opt=="5")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 36.51;
                Double_t Threshold_NewLayer1_OptionB = 35.51;
                Double_t Threshold_NewLayer1_OptionC = 35.51;
                Double_t Threshold_NewLayer1_OptionD = 34.51;
                Double_t Threshold_NewLayer1_OptionE = 33.51;
                Double_t Threshold_NewLayer1_OptionF = 32.51;
                Double_t Threshold_NewLayer1_OptionG = 31.51;
                Double_t Threshold_NewLayer1_OptionH = 31.51;
                Double_t Threshold_NewLayer1_OptionI = 31.51;
                Double_t Threshold_NewLayer1_OptionJ = 31.51;
                Double_t Threshold_NewLayer1_OptionK = 30.51;
                Double_t Threshold_NewLayer1_OptionL = 30.51;
                Double_t Threshold_NewLayer1_OptionM = 30.51;
                Double_t Threshold_NewLayer1_OptionN = 30.51;
                Double_t Threshold_NewLayer1_OptionO = 39.51;
                Double_t Threshold_NewLayer1_OptionP = 37.51;
                Double_t Threshold_NewLayer1_OptionQ = 37.51;
                Double_t Threshold_NewLayer1_OptionR = 36.51;
                Double_t Threshold_NewLayer1_OptionS = 35.51;
                Double_t Threshold_NewLayer1_OptionT = 34.51;
                Double_t Threshold_NewLayer1_OptionU = 33.51;
                Double_t Threshold_NewLayer1_OptionV = 32.51;
                Double_t Threshold_NewLayer1_OptionW = 32.51;
                Double_t Threshold_NewLayer1_OptionX = 31.51;
                Double_t Threshold_NewLayer1_OptionY = 31.51;
                Double_t Threshold_NewLayer1_OptionZ = 30.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="06" and opt=="6")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 30.51;
                Double_t Threshold_NewLayer1_OptionB = 30.51;
                Double_t Threshold_NewLayer1_OptionC = 30.51;
                Double_t Threshold_NewLayer1_OptionD = 30.51;
                Double_t Threshold_NewLayer1_OptionE = 30.51;
                Double_t Threshold_NewLayer1_OptionF = 30.51;
                Double_t Threshold_NewLayer1_OptionG = 37.51;
                Double_t Threshold_NewLayer1_OptionH = 36.51;
                Double_t Threshold_NewLayer1_OptionI = 35.51;
                Double_t Threshold_NewLayer1_OptionJ = 34.51;
                Double_t Threshold_NewLayer1_OptionK = 33.51;
                Double_t Threshold_NewLayer1_OptionL = 33.51;
                Double_t Threshold_NewLayer1_OptionM = 32.51;
                Double_t Threshold_NewLayer1_OptionN = 31.51;
                Double_t Threshold_NewLayer1_OptionO = 30.51;
                Double_t Threshold_NewLayer1_OptionP = 30.51;
                Double_t Threshold_NewLayer1_OptionQ = 30.51;
                Double_t Threshold_NewLayer1_OptionR = 30.51;
                Double_t Threshold_NewLayer1_OptionS = 30.51;
                Double_t Threshold_NewLayer1_OptionT = 29.51;
                Double_t Threshold_NewLayer1_OptionU = 29.51;
                Double_t Threshold_NewLayer1_OptionV = 29.51;
                Double_t Threshold_NewLayer1_OptionW = 29.51;
                Double_t Threshold_NewLayer1_OptionX = 29.51;
                Double_t Threshold_NewLayer1_OptionY = 36.51;
                Double_t Threshold_NewLayer1_OptionZ = 34.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="06" and opt=="7")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 33.51;
                Double_t Threshold_NewLayer1_OptionB = 33.51;
                Double_t Threshold_NewLayer1_OptionC = 33.51;
                Double_t Threshold_NewLayer1_OptionD = 31.51;
                Double_t Threshold_NewLayer1_OptionE = 30.51;
                Double_t Threshold_NewLayer1_OptionF = 30.51;
                Double_t Threshold_NewLayer1_OptionG = 30.51;
                Double_t Threshold_NewLayer1_OptionH = 30.51;
                Double_t Threshold_NewLayer1_OptionI = 29.51;
                Double_t Threshold_NewLayer1_OptionJ = 29.51;
                Double_t Threshold_NewLayer1_OptionK = 29.51;
                Double_t Threshold_NewLayer1_OptionL = 29.51;
                Double_t Threshold_NewLayer1_OptionM = 29.51;
                Double_t Threshold_NewLayer1_OptionN = 29.51;
                Double_t Threshold_NewLayer1_OptionO = 29.51;
                Double_t Threshold_NewLayer1_OptionP = 28.51;
                Double_t Threshold_NewLayer1_OptionQ = 33.51;
                Double_t Threshold_NewLayer1_OptionR = 33.51;
                Double_t Threshold_NewLayer1_OptionS = 32.51;
                Double_t Threshold_NewLayer1_OptionT = 32.51;
                Double_t Threshold_NewLayer1_OptionU = 31.51;
                Double_t Threshold_NewLayer1_OptionV = 30.51;
                Double_t Threshold_NewLayer1_OptionW = 30.51;
                Double_t Threshold_NewLayer1_OptionX = 29.51;
                Double_t Threshold_NewLayer1_OptionY = 29.51;
                Double_t Threshold_NewLayer1_OptionZ = 29.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="06" and opt=="8")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 29.51;
                Double_t Threshold_NewLayer1_OptionB = 29.51;
                Double_t Threshold_NewLayer1_OptionC = 29.51;
                Double_t Threshold_NewLayer1_OptionD = 29.51;
                Double_t Threshold_NewLayer1_OptionE = 29.51;
                Double_t Threshold_NewLayer1_OptionF = 29.51;
                Double_t Threshold_NewLayer1_OptionG = 28.51;
                Double_t Threshold_NewLayer1_OptionH = 28.51;
                Double_t Threshold_NewLayer1_OptionI = 32.51;
                Double_t Threshold_NewLayer1_OptionJ = 32.51;
                Double_t Threshold_NewLayer1_OptionK = 32.51;
                Double_t Threshold_NewLayer1_OptionL = 30.51;
                Double_t Threshold_NewLayer1_OptionM = 30.51;
                Double_t Threshold_NewLayer1_OptionN = 29.51;
                Double_t Threshold_NewLayer1_OptionO = 29.51;
                Double_t Threshold_NewLayer1_OptionP = 29.51;
                Double_t Threshold_NewLayer1_OptionQ = 29.51;
                Double_t Threshold_NewLayer1_OptionR = 29.51;
                Double_t Threshold_NewLayer1_OptionS = 29.51;
                Double_t Threshold_NewLayer1_OptionT = 29.51;
                Double_t Threshold_NewLayer1_OptionU = 29.51;
                Double_t Threshold_NewLayer1_OptionV = 29.51;
                Double_t Threshold_NewLayer1_OptionW = 29.51;
                Double_t Threshold_NewLayer1_OptionX = 28.51;
                Double_t Threshold_NewLayer1_OptionY = 28.51;
                Double_t Threshold_NewLayer1_OptionZ = 28.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="07" and opt=="0")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 42.51;
                Double_t Threshold_NewLayer1_OptionB = 42.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 42.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 41.51;
                Double_t Threshold_NewLayer1_OptionI = 41.51;
                Double_t Threshold_NewLayer1_OptionJ = 40.51;
                Double_t Threshold_NewLayer1_OptionK = 40.51;
                Double_t Threshold_NewLayer1_OptionL = 39.51;
                Double_t Threshold_NewLayer1_OptionM = 38.51;
                Double_t Threshold_NewLayer1_OptionN = 37.51;
                Double_t Threshold_NewLayer1_OptionO = 37.51;
                Double_t Threshold_NewLayer1_OptionP = 37.51;
                Double_t Threshold_NewLayer1_OptionQ = 36.51;
                Double_t Threshold_NewLayer1_OptionR = 35.51;
                Double_t Threshold_NewLayer1_OptionS = 42.51;
                Double_t Threshold_NewLayer1_OptionT = 42.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 42.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 41.51;
                Double_t Threshold_NewLayer1_OptionZ = 41.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="07" and opt=="1")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 40.51;
                Double_t Threshold_NewLayer1_OptionB = 39.51;
                Double_t Threshold_NewLayer1_OptionC = 39.51;
                Double_t Threshold_NewLayer1_OptionD = 38.51;
                Double_t Threshold_NewLayer1_OptionE = 37.51;
                Double_t Threshold_NewLayer1_OptionF = 37.51;
                Double_t Threshold_NewLayer1_OptionG = 37.51;
                Double_t Threshold_NewLayer1_OptionH = 35.51;
                Double_t Threshold_NewLayer1_OptionI = 35.51;
                Double_t Threshold_NewLayer1_OptionJ = 35.51;
                Double_t Threshold_NewLayer1_OptionK = 42.51;
                Double_t Threshold_NewLayer1_OptionL = 42.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 42.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 41.51;
                Double_t Threshold_NewLayer1_OptionQ = 41.51;
                Double_t Threshold_NewLayer1_OptionR = 40.51;
                Double_t Threshold_NewLayer1_OptionS = 39.51;
                Double_t Threshold_NewLayer1_OptionT = 38.51;
                Double_t Threshold_NewLayer1_OptionU = 38.51;
                Double_t Threshold_NewLayer1_OptionV = 37.51;
                Double_t Threshold_NewLayer1_OptionW = 37.51;
                Double_t Threshold_NewLayer1_OptionX = 37.51;
                Double_t Threshold_NewLayer1_OptionY = 35.51;
                Double_t Threshold_NewLayer1_OptionZ = 35.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="07" and opt=="2")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 35.51;
                Double_t Threshold_NewLayer1_OptionB = 34.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 41.51;
                Double_t Threshold_NewLayer1_OptionI = 40.51;
                Double_t Threshold_NewLayer1_OptionJ = 39.51;
                Double_t Threshold_NewLayer1_OptionK = 38.51;
                Double_t Threshold_NewLayer1_OptionL = 38.51;
                Double_t Threshold_NewLayer1_OptionM = 37.51;
                Double_t Threshold_NewLayer1_OptionN = 37.51;
                Double_t Threshold_NewLayer1_OptionO = 37.51;
                Double_t Threshold_NewLayer1_OptionP = 35.51;
                Double_t Threshold_NewLayer1_OptionQ = 35.51;
                Double_t Threshold_NewLayer1_OptionR = 34.51;
                Double_t Threshold_NewLayer1_OptionS = 34.51;
                Double_t Threshold_NewLayer1_OptionT = 34.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 41.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 41.51;
                Double_t Threshold_NewLayer1_OptionZ = 39.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="07" and opt=="3")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 39.51;
                Double_t Threshold_NewLayer1_OptionB = 38.51;
                Double_t Threshold_NewLayer1_OptionC = 37.51;
                Double_t Threshold_NewLayer1_OptionD = 37.51;
                Double_t Threshold_NewLayer1_OptionE = 37.51;
                Double_t Threshold_NewLayer1_OptionF = 36.51;
                Double_t Threshold_NewLayer1_OptionG = 35.51;
                Double_t Threshold_NewLayer1_OptionH = 34.51;
                Double_t Threshold_NewLayer1_OptionI = 34.51;
                Double_t Threshold_NewLayer1_OptionJ = 34.51;
                Double_t Threshold_NewLayer1_OptionK = 33.51;
                Double_t Threshold_NewLayer1_OptionL = 33.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 41.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 40.51;
                Double_t Threshold_NewLayer1_OptionQ = 39.51;
                Double_t Threshold_NewLayer1_OptionR = 39.51;
                Double_t Threshold_NewLayer1_OptionS = 37.51;
                Double_t Threshold_NewLayer1_OptionT = 37.51;
                Double_t Threshold_NewLayer1_OptionU = 37.51;
                Double_t Threshold_NewLayer1_OptionV = 36.51;
                Double_t Threshold_NewLayer1_OptionW = 36.51;
                Double_t Threshold_NewLayer1_OptionX = 35.51;
                Double_t Threshold_NewLayer1_OptionY = 34.51;
                Double_t Threshold_NewLayer1_OptionZ = 34.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="07" and opt=="4")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 33.51;
                Double_t Threshold_NewLayer1_OptionB = 33.51;
                Double_t Threshold_NewLayer1_OptionC = 33.51;
                Double_t Threshold_NewLayer1_OptionD = 33.51;
                Double_t Threshold_NewLayer1_OptionE = 41.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 40.51;
                Double_t Threshold_NewLayer1_OptionH = 39.51;
                Double_t Threshold_NewLayer1_OptionI = 39.51;
                Double_t Threshold_NewLayer1_OptionJ = 37.51;
                Double_t Threshold_NewLayer1_OptionK = 36.51;
                Double_t Threshold_NewLayer1_OptionL = 36.51;
                Double_t Threshold_NewLayer1_OptionM = 36.51;
                Double_t Threshold_NewLayer1_OptionN = 36.51;
                Double_t Threshold_NewLayer1_OptionO = 34.51;
                Double_t Threshold_NewLayer1_OptionP = 34.51;
                Double_t Threshold_NewLayer1_OptionQ = 33.51;
                Double_t Threshold_NewLayer1_OptionR = 33.51;
                Double_t Threshold_NewLayer1_OptionS = 33.51;
                Double_t Threshold_NewLayer1_OptionT = 32.51;
                Double_t Threshold_NewLayer1_OptionU = 32.51;
                Double_t Threshold_NewLayer1_OptionV = 32.51;
                Double_t Threshold_NewLayer1_OptionW = 41.51;
                Double_t Threshold_NewLayer1_OptionX = 40.51;
                Double_t Threshold_NewLayer1_OptionY = 39.51;
                Double_t Threshold_NewLayer1_OptionZ = 38.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="07" and opt=="5")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 37.51;
                Double_t Threshold_NewLayer1_OptionB = 36.51;
                Double_t Threshold_NewLayer1_OptionC = 36.51;
                Double_t Threshold_NewLayer1_OptionD = 36.51;
                Double_t Threshold_NewLayer1_OptionE = 35.51;
                Double_t Threshold_NewLayer1_OptionF = 34.51;
                Double_t Threshold_NewLayer1_OptionG = 33.51;
                Double_t Threshold_NewLayer1_OptionH = 33.51;
                Double_t Threshold_NewLayer1_OptionI = 33.51;
                Double_t Threshold_NewLayer1_OptionJ = 32.51;
                Double_t Threshold_NewLayer1_OptionK = 32.51;
                Double_t Threshold_NewLayer1_OptionL = 32.51;
                Double_t Threshold_NewLayer1_OptionM = 32.51;
                Double_t Threshold_NewLayer1_OptionN = 32.51;
                Double_t Threshold_NewLayer1_OptionO = 40.51;
                Double_t Threshold_NewLayer1_OptionP = 38.51;
                Double_t Threshold_NewLayer1_OptionQ = 37.51;
                Double_t Threshold_NewLayer1_OptionR = 36.51;
                Double_t Threshold_NewLayer1_OptionS = 36.51;
                Double_t Threshold_NewLayer1_OptionT = 36.51;
                Double_t Threshold_NewLayer1_OptionU = 35.51;
                Double_t Threshold_NewLayer1_OptionV = 34.51;
                Double_t Threshold_NewLayer1_OptionW = 33.51;
                Double_t Threshold_NewLayer1_OptionX = 33.51;
                Double_t Threshold_NewLayer1_OptionY = 33.51;
                Double_t Threshold_NewLayer1_OptionZ = 32.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="07" and opt=="6")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 32.51;
                Double_t Threshold_NewLayer1_OptionB = 32.51;
                Double_t Threshold_NewLayer1_OptionC = 32.51;
                Double_t Threshold_NewLayer1_OptionD = 32.51;
                Double_t Threshold_NewLayer1_OptionE = 32.51;
                Double_t Threshold_NewLayer1_OptionF = 32.51;
                Double_t Threshold_NewLayer1_OptionG = 38.51;
                Double_t Threshold_NewLayer1_OptionH = 37.51;
                Double_t Threshold_NewLayer1_OptionI = 36.51;
                Double_t Threshold_NewLayer1_OptionJ = 36.51;
                Double_t Threshold_NewLayer1_OptionK = 35.51;
                Double_t Threshold_NewLayer1_OptionL = 35.51;
                Double_t Threshold_NewLayer1_OptionM = 33.51;
                Double_t Threshold_NewLayer1_OptionN = 33.51;
                Double_t Threshold_NewLayer1_OptionO = 32.51;
                Double_t Threshold_NewLayer1_OptionP = 32.51;
                Double_t Threshold_NewLayer1_OptionQ = 32.51;
                Double_t Threshold_NewLayer1_OptionR = 32.51;
                Double_t Threshold_NewLayer1_OptionS = 32.51;
                Double_t Threshold_NewLayer1_OptionT = 32.51;
                Double_t Threshold_NewLayer1_OptionU = 32.51;
                Double_t Threshold_NewLayer1_OptionV = 31.51;
                Double_t Threshold_NewLayer1_OptionW = 31.51;
                Double_t Threshold_NewLayer1_OptionX = 31.51;
                Double_t Threshold_NewLayer1_OptionY = 37.51;
                Double_t Threshold_NewLayer1_OptionZ = 36.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="07" and opt=="7")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 35.51;
                Double_t Threshold_NewLayer1_OptionB = 35.51;
                Double_t Threshold_NewLayer1_OptionC = 34.51;
                Double_t Threshold_NewLayer1_OptionD = 33.51;
                Double_t Threshold_NewLayer1_OptionE = 33.51;
                Double_t Threshold_NewLayer1_OptionF = 32.51;
                Double_t Threshold_NewLayer1_OptionG = 32.51;
                Double_t Threshold_NewLayer1_OptionH = 32.51;
                Double_t Threshold_NewLayer1_OptionI = 32.51;
                Double_t Threshold_NewLayer1_OptionJ = 32.51;
                Double_t Threshold_NewLayer1_OptionK = 31.51;
                Double_t Threshold_NewLayer1_OptionL = 31.51;
                Double_t Threshold_NewLayer1_OptionM = 31.51;
                Double_t Threshold_NewLayer1_OptionN = 31.51;
                Double_t Threshold_NewLayer1_OptionO = 31.51;
                Double_t Threshold_NewLayer1_OptionP = 30.51;
                Double_t Threshold_NewLayer1_OptionQ = 35.51;
                Double_t Threshold_NewLayer1_OptionR = 35.51;
                Double_t Threshold_NewLayer1_OptionS = 34.51;
                Double_t Threshold_NewLayer1_OptionT = 34.51;
                Double_t Threshold_NewLayer1_OptionU = 33.51;
                Double_t Threshold_NewLayer1_OptionV = 32.51;
                Double_t Threshold_NewLayer1_OptionW = 32.51;
                Double_t Threshold_NewLayer1_OptionX = 32.51;
                Double_t Threshold_NewLayer1_OptionY = 32.51;
                Double_t Threshold_NewLayer1_OptionZ = 32.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="07" and opt=="8")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 31.51;
                Double_t Threshold_NewLayer1_OptionB = 31.51;
                Double_t Threshold_NewLayer1_OptionC = 31.51;
                Double_t Threshold_NewLayer1_OptionD = 31.51;
                Double_t Threshold_NewLayer1_OptionE = 31.51;
                Double_t Threshold_NewLayer1_OptionF = 31.51;
                Double_t Threshold_NewLayer1_OptionG = 30.51;
                Double_t Threshold_NewLayer1_OptionH = 30.51;
                Double_t Threshold_NewLayer1_OptionI = 34.51;
                Double_t Threshold_NewLayer1_OptionJ = 34.51;
                Double_t Threshold_NewLayer1_OptionK = 34.51;
                Double_t Threshold_NewLayer1_OptionL = 33.51;
                Double_t Threshold_NewLayer1_OptionM = 32.51;
                Double_t Threshold_NewLayer1_OptionN = 32.51;
                Double_t Threshold_NewLayer1_OptionO = 32.51;
                Double_t Threshold_NewLayer1_OptionP = 31.51;
                Double_t Threshold_NewLayer1_OptionQ = 31.51;
                Double_t Threshold_NewLayer1_OptionR = 31.51;
                Double_t Threshold_NewLayer1_OptionS = 31.51;
                Double_t Threshold_NewLayer1_OptionT = 31.51;
                Double_t Threshold_NewLayer1_OptionU = 31.51;
                Double_t Threshold_NewLayer1_OptionV = 31.51;
                Double_t Threshold_NewLayer1_OptionW = 31.51;
                Double_t Threshold_NewLayer1_OptionX = 30.51;
                Double_t Threshold_NewLayer1_OptionY = 30.51;
                Double_t Threshold_NewLayer1_OptionZ = 30.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="08" and opt=="0")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 42.51;
                Double_t Threshold_NewLayer1_OptionB = 42.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 42.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 41.51;
                Double_t Threshold_NewLayer1_OptionI = 41.51;
                Double_t Threshold_NewLayer1_OptionJ = 40.51;
                Double_t Threshold_NewLayer1_OptionK = 40.51;
                Double_t Threshold_NewLayer1_OptionL = 39.51;
                Double_t Threshold_NewLayer1_OptionM = 39.51;
                Double_t Threshold_NewLayer1_OptionN = 39.51;
                Double_t Threshold_NewLayer1_OptionO = 38.51;
                Double_t Threshold_NewLayer1_OptionP = 38.51;
                Double_t Threshold_NewLayer1_OptionQ = 37.51;
                Double_t Threshold_NewLayer1_OptionR = 37.51;
                Double_t Threshold_NewLayer1_OptionS = 42.51;
                Double_t Threshold_NewLayer1_OptionT = 42.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 42.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 41.51;
                Double_t Threshold_NewLayer1_OptionZ = 41.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="08" and opt=="1")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 40.51;
                Double_t Threshold_NewLayer1_OptionB = 40.51;
                Double_t Threshold_NewLayer1_OptionC = 39.51;
                Double_t Threshold_NewLayer1_OptionD = 39.51;
                Double_t Threshold_NewLayer1_OptionE = 38.51;
                Double_t Threshold_NewLayer1_OptionF = 38.51;
                Double_t Threshold_NewLayer1_OptionG = 38.51;
                Double_t Threshold_NewLayer1_OptionH = 37.51;
                Double_t Threshold_NewLayer1_OptionI = 37.51;
                Double_t Threshold_NewLayer1_OptionJ = 36.51;
                Double_t Threshold_NewLayer1_OptionK = 42.51;
                Double_t Threshold_NewLayer1_OptionL = 42.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 42.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 41.51;
                Double_t Threshold_NewLayer1_OptionQ = 41.51;
                Double_t Threshold_NewLayer1_OptionR = 40.51;
                Double_t Threshold_NewLayer1_OptionS = 40.51;
                Double_t Threshold_NewLayer1_OptionT = 39.51;
                Double_t Threshold_NewLayer1_OptionU = 39.51;
                Double_t Threshold_NewLayer1_OptionV = 38.51;
                Double_t Threshold_NewLayer1_OptionW = 38.51;
                Double_t Threshold_NewLayer1_OptionX = 38.51;
                Double_t Threshold_NewLayer1_OptionY = 37.51;
                Double_t Threshold_NewLayer1_OptionZ = 36.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="08" and opt=="2")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 36.51;
                Double_t Threshold_NewLayer1_OptionB = 36.51;
                Double_t Threshold_NewLayer1_OptionC = 42.51;
                Double_t Threshold_NewLayer1_OptionD = 42.51;
                Double_t Threshold_NewLayer1_OptionE = 42.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 41.51;
                Double_t Threshold_NewLayer1_OptionI = 40.51;
                Double_t Threshold_NewLayer1_OptionJ = 40.51;
                Double_t Threshold_NewLayer1_OptionK = 39.51;
                Double_t Threshold_NewLayer1_OptionL = 39.51;
                Double_t Threshold_NewLayer1_OptionM = 38.51;
                Double_t Threshold_NewLayer1_OptionN = 38.51;
                Double_t Threshold_NewLayer1_OptionO = 38.51;
                Double_t Threshold_NewLayer1_OptionP = 36.51;
                Double_t Threshold_NewLayer1_OptionQ = 36.51;
                Double_t Threshold_NewLayer1_OptionR = 36.51;
                Double_t Threshold_NewLayer1_OptionS = 36.51;
                Double_t Threshold_NewLayer1_OptionT = 35.51;
                Double_t Threshold_NewLayer1_OptionU = 42.51;
                Double_t Threshold_NewLayer1_OptionV = 42.51;
                Double_t Threshold_NewLayer1_OptionW = 41.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 41.51;
                Double_t Threshold_NewLayer1_OptionZ = 40.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="08" and opt=="3")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 40.51;
                Double_t Threshold_NewLayer1_OptionB = 39.51;
                Double_t Threshold_NewLayer1_OptionC = 38.51;
                Double_t Threshold_NewLayer1_OptionD = 38.51;
                Double_t Threshold_NewLayer1_OptionE = 38.51;
                Double_t Threshold_NewLayer1_OptionF = 37.51;
                Double_t Threshold_NewLayer1_OptionG = 36.51;
                Double_t Threshold_NewLayer1_OptionH = 36.51;
                Double_t Threshold_NewLayer1_OptionI = 36.51;
                Double_t Threshold_NewLayer1_OptionJ = 35.51;
                Double_t Threshold_NewLayer1_OptionK = 35.51;
                Double_t Threshold_NewLayer1_OptionL = 35.51;
                Double_t Threshold_NewLayer1_OptionM = 42.51;
                Double_t Threshold_NewLayer1_OptionN = 41.51;
                Double_t Threshold_NewLayer1_OptionO = 41.51;
                Double_t Threshold_NewLayer1_OptionP = 41.51;
                Double_t Threshold_NewLayer1_OptionQ = 40.51;
                Double_t Threshold_NewLayer1_OptionR = 39.51;
                Double_t Threshold_NewLayer1_OptionS = 39.51;
                Double_t Threshold_NewLayer1_OptionT = 38.51;
                Double_t Threshold_NewLayer1_OptionU = 37.51;
                Double_t Threshold_NewLayer1_OptionV = 37.51;
                Double_t Threshold_NewLayer1_OptionW = 37.51;
                Double_t Threshold_NewLayer1_OptionX = 36.51;
                Double_t Threshold_NewLayer1_OptionY = 36.51;
                Double_t Threshold_NewLayer1_OptionZ = 35.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="08" and opt=="4")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 35.51;
                Double_t Threshold_NewLayer1_OptionB = 35.51;
                Double_t Threshold_NewLayer1_OptionC = 35.51;
                Double_t Threshold_NewLayer1_OptionD = 35.51;
                Double_t Threshold_NewLayer1_OptionE = 41.51;
                Double_t Threshold_NewLayer1_OptionF = 41.51;
                Double_t Threshold_NewLayer1_OptionG = 41.51;
                Double_t Threshold_NewLayer1_OptionH = 40.51;
                Double_t Threshold_NewLayer1_OptionI = 39.51;
                Double_t Threshold_NewLayer1_OptionJ = 38.51;
                Double_t Threshold_NewLayer1_OptionK = 38.51;
                Double_t Threshold_NewLayer1_OptionL = 37.51;
                Double_t Threshold_NewLayer1_OptionM = 37.51;
                Double_t Threshold_NewLayer1_OptionN = 37.51;
                Double_t Threshold_NewLayer1_OptionO = 36.51;
                Double_t Threshold_NewLayer1_OptionP = 36.51;
                Double_t Threshold_NewLayer1_OptionQ = 35.51;
                Double_t Threshold_NewLayer1_OptionR = 35.51;
                Double_t Threshold_NewLayer1_OptionS = 35.51;
                Double_t Threshold_NewLayer1_OptionT = 35.51;
                Double_t Threshold_NewLayer1_OptionU = 34.51;
                Double_t Threshold_NewLayer1_OptionV = 34.51;
                Double_t Threshold_NewLayer1_OptionW = 41.51;
                Double_t Threshold_NewLayer1_OptionX = 41.51;
                Double_t Threshold_NewLayer1_OptionY = 40.51;
                Double_t Threshold_NewLayer1_OptionZ = 39.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="08" and opt=="5")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 38.51;
                Double_t Threshold_NewLayer1_OptionB = 37.51;
                Double_t Threshold_NewLayer1_OptionC = 37.51;
                Double_t Threshold_NewLayer1_OptionD = 37.51;
                Double_t Threshold_NewLayer1_OptionE = 36.51;
                Double_t Threshold_NewLayer1_OptionF = 36.51;
                Double_t Threshold_NewLayer1_OptionG = 35.51;
                Double_t Threshold_NewLayer1_OptionH = 35.51;
                Double_t Threshold_NewLayer1_OptionI = 35.51;
                Double_t Threshold_NewLayer1_OptionJ = 35.51;
                Double_t Threshold_NewLayer1_OptionK = 34.51;
                Double_t Threshold_NewLayer1_OptionL = 34.51;
                Double_t Threshold_NewLayer1_OptionM = 34.51;
                Double_t Threshold_NewLayer1_OptionN = 34.51;
                Double_t Threshold_NewLayer1_OptionO = 40.51;
                Double_t Threshold_NewLayer1_OptionP = 39.51;
                Double_t Threshold_NewLayer1_OptionQ = 39.51;
                Double_t Threshold_NewLayer1_OptionR = 38.51;
                Double_t Threshold_NewLayer1_OptionS = 37.51;
                Double_t Threshold_NewLayer1_OptionT = 37.51;
                Double_t Threshold_NewLayer1_OptionU = 37.51;
                Double_t Threshold_NewLayer1_OptionV = 36.51;
                Double_t Threshold_NewLayer1_OptionW = 35.51;
                Double_t Threshold_NewLayer1_OptionX = 35.51;
                Double_t Threshold_NewLayer1_OptionY = 35.51;
                Double_t Threshold_NewLayer1_OptionZ = 35.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="08" and opt=="6")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 34.51;
                Double_t Threshold_NewLayer1_OptionB = 34.51;
                Double_t Threshold_NewLayer1_OptionC = 34.51;
                Double_t Threshold_NewLayer1_OptionD = 34.51;
                Double_t Threshold_NewLayer1_OptionE = 34.51;
                Double_t Threshold_NewLayer1_OptionF = 34.51;
                Double_t Threshold_NewLayer1_OptionG = 39.51;
                Double_t Threshold_NewLayer1_OptionH = 39.51;
                Double_t Threshold_NewLayer1_OptionI = 37.51;
                Double_t Threshold_NewLayer1_OptionJ = 37.51;
                Double_t Threshold_NewLayer1_OptionK = 37.51;
                Double_t Threshold_NewLayer1_OptionL = 36.51;
                Double_t Threshold_NewLayer1_OptionM = 35.51;
                Double_t Threshold_NewLayer1_OptionN = 35.51;
                Double_t Threshold_NewLayer1_OptionO = 35.51;
                Double_t Threshold_NewLayer1_OptionP = 35.51;
                Double_t Threshold_NewLayer1_OptionQ = 34.51;
                Double_t Threshold_NewLayer1_OptionR = 34.51;
                Double_t Threshold_NewLayer1_OptionS = 34.51;
                Double_t Threshold_NewLayer1_OptionT = 34.51;
                Double_t Threshold_NewLayer1_OptionU = 34.51;
                Double_t Threshold_NewLayer1_OptionV = 34.51;
                Double_t Threshold_NewLayer1_OptionW = 34.51;
                Double_t Threshold_NewLayer1_OptionX = 34.51;
                Double_t Threshold_NewLayer1_OptionY = 38.51;
                Double_t Threshold_NewLayer1_OptionZ = 37.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="08" and opt=="7")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 37.51;
                Double_t Threshold_NewLayer1_OptionB = 36.51;
                Double_t Threshold_NewLayer1_OptionC = 36.51;
                Double_t Threshold_NewLayer1_OptionD = 35.51;
                Double_t Threshold_NewLayer1_OptionE = 35.51;
                Double_t Threshold_NewLayer1_OptionF = 35.51;
                Double_t Threshold_NewLayer1_OptionG = 34.51;
                Double_t Threshold_NewLayer1_OptionH = 34.51;
                Double_t Threshold_NewLayer1_OptionI = 34.51;
                Double_t Threshold_NewLayer1_OptionJ = 34.51;
                Double_t Threshold_NewLayer1_OptionK = 34.51;
                Double_t Threshold_NewLayer1_OptionL = 34.51;
                Double_t Threshold_NewLayer1_OptionM = 33.51;
                Double_t Threshold_NewLayer1_OptionN = 33.51;
                Double_t Threshold_NewLayer1_OptionO = 33.51;
                Double_t Threshold_NewLayer1_OptionP = 32.51;
                Double_t Threshold_NewLayer1_OptionQ = 37.51;
                Double_t Threshold_NewLayer1_OptionR = 36.51;
                Double_t Threshold_NewLayer1_OptionS = 36.51;
                Double_t Threshold_NewLayer1_OptionT = 36.51;
                Double_t Threshold_NewLayer1_OptionU = 35.51;
                Double_t Threshold_NewLayer1_OptionV = 34.51;
                Double_t Threshold_NewLayer1_OptionW = 34.51;
                Double_t Threshold_NewLayer1_OptionX = 34.51;
                Double_t Threshold_NewLayer1_OptionY = 34.51;
                Double_t Threshold_NewLayer1_OptionZ = 34.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }

            else if (tag=="08" and opt=="8")
            {
                Double_t Threshold_NewLayer1_noIso   = 42.51;
                Double_t Threshold_NewLayer1_noIso_unpacked = 42.51;
                Double_t Threshold_NewLayer1_Iso_unpacked = 34.51;
                Double_t Threshold_NewLayer1_OptionA = 34.51;
                Double_t Threshold_NewLayer1_OptionB = 33.51;
                Double_t Threshold_NewLayer1_OptionC = 33.51;
                Double_t Threshold_NewLayer1_OptionD = 33.51;
                Double_t Threshold_NewLayer1_OptionE = 33.51;
                Double_t Threshold_NewLayer1_OptionF = 33.51;
                Double_t Threshold_NewLayer1_OptionG = 32.51;
                Double_t Threshold_NewLayer1_OptionH = 33.51;
                Double_t Threshold_NewLayer1_OptionI = 36.51;
                Double_t Threshold_NewLayer1_OptionJ = 36.51;
                Double_t Threshold_NewLayer1_OptionK = 36.51;
                Double_t Threshold_NewLayer1_OptionL = 35.51;
                Double_t Threshold_NewLayer1_OptionM = 34.51;
                Double_t Threshold_NewLayer1_OptionN = 34.51;
                Double_t Threshold_NewLayer1_OptionO = 34.51;
                Double_t Threshold_NewLayer1_OptionP = 33.51;
                Double_t Threshold_NewLayer1_OptionQ = 33.51;
                Double_t Threshold_NewLayer1_OptionR = 33.51;
                Double_t Threshold_NewLayer1_OptionS = 33.51;
                Double_t Threshold_NewLayer1_OptionT = 33.51;
                Double_t Threshold_NewLayer1_OptionU = 33.51;
                Double_t Threshold_NewLayer1_OptionV = 33.51;
                Double_t Threshold_NewLayer1_OptionW = 33.51;
                Double_t Threshold_NewLayer1_OptionX = 32.51;
                Double_t Threshold_NewLayer1_OptionY = 32.51;
                Double_t Threshold_NewLayer1_OptionZ = 32.51;
                Double_t Threshold_NewLayer1_Option22 = 35.51;
                Double_t Threshold_NewLayer1_Option31_extrap = 36.51;
            }


            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_noIso) pt_pass_noIso->Fill(in_OfflineTau_pt);

            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionA && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionA) pt_pass_OptionA->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionB && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionB) pt_pass_OptionB->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionC && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionC) pt_pass_OptionC->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionD && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionD) pt_pass_OptionD->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionE && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionE) pt_pass_OptionE->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionF && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionF) pt_pass_OptionF->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionG && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionG) pt_pass_OptionG->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionH && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionH) pt_pass_OptionH->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionI && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionI) pt_pass_OptionI->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionJ && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionJ) pt_pass_OptionJ->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionK && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionK) pt_pass_OptionK->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionL && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionL) pt_pass_OptionL->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionM && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionM) pt_pass_OptionM->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionN && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionN) pt_pass_OptionN->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionO && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionO) pt_pass_OptionO->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionP && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionP) pt_pass_OptionP->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionQ && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionQ) pt_pass_OptionQ->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionR && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionR) pt_pass_OptionR->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionS && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionS) pt_pass_OptionS->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionT && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionT) pt_pass_OptionT->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionU && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionU) pt_pass_OptionU->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionV && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionV) pt_pass_OptionV->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionW && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionW) pt_pass_OptionW->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionX && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionX) pt_pass_OptionX->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionY && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionY) pt_pass_OptionY->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_OptionZ && in_L1Tau_Iso<=Cut_L1Tau_Iso_OptionZ) pt_pass_OptionZ->Fill(in_OfflineTau_pt);


            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22) pt_pass_Option22->Fill(in_OfflineTau_pt);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31_extrap && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31_extrap) pt_pass_Option31_extrap->Fill(in_OfflineTau_pt);

            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22 && in_OfflineTau_pt>44.) nVtx_pass_Option22->Fill(in_Nvtx);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option22 && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option22 && in_OfflineTau_pt>44.) eta_pass_Option22->Fill(in_OfflineTau_eta);

            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31_extrap && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31_extrap && in_OfflineTau_pt>44.) nVtx_pass_Option31_extrap->Fill(in_Nvtx);
            if(in_L1Tau_CalibPt>=Threshold_NewLayer1_Option31_extrap && in_L1Tau_Iso<=Cut_L1Tau_Iso_Option31_extrap && in_OfflineTau_pt>44.) eta_pass_Option31_extrap->Fill(in_OfflineTau_eta);

            if(in_OfflineTau_pt>44.) nVtx->Fill(in_Nvtx);
            if(in_OfflineTau_pt>44.) eta->Fill(in_OfflineTau_eta);            
            
        }


    TFile turnOns(FileNameOut.Data(),"RECREATE");

    TGraphAsymmErrors* turnOn_noIso = new TGraphAsymmErrors(pt_pass_noIso,pt,"cp");
    turnOn_noIso->Write();

    TGraphAsymmErrors* turnOn_OptionA = new TGraphAsymmErrors(pt_pass_OptionA,pt,"cp");
    turnOn_OptionA->Write();

    TGraphAsymmErrors* turnOn_OptionB = new TGraphAsymmErrors(pt_pass_OptionB,pt,"cp");
    turnOn_OptionB->Write();

    TGraphAsymmErrors* turnOn_OptionC = new TGraphAsymmErrors(pt_pass_OptionC,pt,"cp");
    turnOn_OptionC->Write();

    TGraphAsymmErrors* turnOn_OptionD = new TGraphAsymmErrors(pt_pass_OptionD,pt,"cp");
    turnOn_OptionD->Write();

    TGraphAsymmErrors* turnOn_OptionE = new TGraphAsymmErrors(pt_pass_OptionE,pt,"cp");
    turnOn_OptionE->Write();

    TGraphAsymmErrors* turnOn_OptionF = new TGraphAsymmErrors(pt_pass_OptionF,pt,"cp");
    turnOn_OptionF->Write();

    TGraphAsymmErrors* turnOn_OptionG = new TGraphAsymmErrors(pt_pass_OptionG,pt,"cp");
    turnOn_OptionG->Write();

    TGraphAsymmErrors* turnOn_OptionH = new TGraphAsymmErrors(pt_pass_OptionH,pt,"cp");
    turnOn_OptionH->Write();

    TGraphAsymmErrors* turnOn_OptionI = new TGraphAsymmErrors(pt_pass_OptionI,pt,"cp");
    turnOn_OptionI->Write();

    TGraphAsymmErrors* turnOn_OptionJ = new TGraphAsymmErrors(pt_pass_OptionJ,pt,"cp");
    turnOn_OptionJ->Write();

    TGraphAsymmErrors* turnOn_OptionK = new TGraphAsymmErrors(pt_pass_OptionK,pt,"cp");
    turnOn_OptionK->Write();

    TGraphAsymmErrors* turnOn_OptionL = new TGraphAsymmErrors(pt_pass_OptionL,pt,"cp");
    turnOn_OptionL->Write();

    TGraphAsymmErrors* turnOn_OptionM = new TGraphAsymmErrors(pt_pass_OptionM,pt,"cp");
    turnOn_OptionM->Write();

    TGraphAsymmErrors* turnOn_OptionN = new TGraphAsymmErrors(pt_pass_OptionN,pt,"cp");
    turnOn_OptionN->Write();

    TGraphAsymmErrors* turnOn_OptionO = new TGraphAsymmErrors(pt_pass_OptionO,pt,"cp");
    turnOn_OptionO->Write();

    TGraphAsymmErrors* turnOn_OptionP = new TGraphAsymmErrors(pt_pass_OptionP,pt,"cp");
    turnOn_OptionP->Write();

    TGraphAsymmErrors* turnOn_OptionQ = new TGraphAsymmErrors(pt_pass_OptionQ,pt,"cp");
    turnOn_OptionQ->Write();

    TGraphAsymmErrors* turnOn_OptionR = new TGraphAsymmErrors(pt_pass_OptionR,pt,"cp");
    turnOn_OptionR->Write();

    TGraphAsymmErrors* turnOn_OptionS = new TGraphAsymmErrors(pt_pass_OptionS,pt,"cp");
    turnOn_OptionS->Write();

    TGraphAsymmErrors* turnOn_OptionT = new TGraphAsymmErrors(pt_pass_OptionT,pt,"cp");
    turnOn_OptionT->Write();

    TGraphAsymmErrors* turnOn_OptionU = new TGraphAsymmErrors(pt_pass_OptionU,pt,"cp");
    turnOn_OptionU->Write();

    TGraphAsymmErrors* turnOn_OptionV = new TGraphAsymmErrors(pt_pass_OptionV,pt,"cp");
    turnOn_OptionV->Write();

    TGraphAsymmErrors* turnOn_OptionW = new TGraphAsymmErrors(pt_pass_OptionW,pt,"cp");
    turnOn_OptionW->Write();

    TGraphAsymmErrors* turnOn_OptionX = new TGraphAsymmErrors(pt_pass_OptionX,pt,"cp");
    turnOn_OptionX->Write();

    TGraphAsymmErrors* turnOn_OptionY = new TGraphAsymmErrors(pt_pass_OptionY,pt,"cp");
    turnOn_OptionY->Write();

    TGraphAsymmErrors* turnOn_OptionZ = new TGraphAsymmErrors(pt_pass_OptionZ,pt,"cp");
    turnOn_OptionZ->Write();


    TGraphAsymmErrors* turnOn_Option22 = new TGraphAsymmErrors(pt_pass_Option22,pt,"cp");
    turnOn_Option22->Write();

    TGraphAsymmErrors* nVtx_Option22 = new TGraphAsymmErrors(nVtx_pass_Option22,nVtx,"cp");
    nVtx_Option22->Write();

    TGraphAsymmErrors* eta_Option22 = new TGraphAsymmErrors(eta_pass_Option22,eta,"cp");
    eta_Option22->Write();

    TGraphAsymmErrors* turnOn_Option31_extrap = new TGraphAsymmErrors(pt_pass_Option31_extrap,pt,"cp");
    turnOn_Option31_extrap->Write();

    TGraphAsymmErrors* nVtx_Option31_extrap = new TGraphAsymmErrors(nVtx_pass_Option31_extrap,nVtx,"cp");
    nVtx_Option31_extrap->Write();

    TGraphAsymmErrors* eta_Option31_extrap = new TGraphAsymmErrors(eta_pass_Option31_extrap,eta,"cp");
    eta_Option31_extrap->Write();

}
