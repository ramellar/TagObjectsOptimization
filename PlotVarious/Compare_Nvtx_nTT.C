#include <TROOT.h>
#include <TFile.h>
#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <sstream>
#include <TBranchElement.h>
#include <TGraphAsymmErrors.h>
#include <TVector.h>
#include <fstream>
#include <map>

using namespace std;

void compare(bool nTTRange = false, float calibThr = 1.7) {
    TString nTTthr = "60";

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    gStyle->SetOptStat(000000);

    TFile* f_Zb323775 = new TFile("/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr"+intgr+"p"+decim+"/EphemeralZeroBias_2018D_Run323775_reEmuTPs_CALIBRATED.root", "READ");
    TFile* f_Zb323755 = new TFile("/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr"+intgr+"p"+decim+"/EphemeralZeroBias_2018D_Run323755_reEmuTPs_CALIBRATED.root", "READ");
    TFile* f_Run3_Snu112X = new TFile("/data_CMS/cms/motta/Run3preparation/2022_04_21_optimizationV11_calibThr"+intgr+"p"+decim+"/Run3_MC_SingleNeutrinoGun112X_CALIBRATED_2022_04_21.root", "READ");
    TFile* f_Run3_Snu120X = new TFile("/data_CMS/cms/motta/Run3preparation/2022_06_10_optimizationV12_calibThr"+intgr+"p"+decim+"/Run3_MC_SingleNeutrinoGun120X_CALIBRATED_2022_06_10.root", "READ");
    TFile* f_Run3_Snu122X = new TFile("/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr"+intgr+"p"+decim+"/Run3_MC_SingleNeutrinoGun122X_CALIBRATED_2022_06_13.root", "READ");
    TFile* f_Run3_VBF = new TFile("/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_CALIBRATED_2022_06_13.root", "READ");
    TFile* f_Run3_AOD = new TFile("/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_MINIAOD_2022_06_13/Run3_MC_VBFHToTauTau_M125_MINIAOD_2022_06_13.root", "READ");
    TFile* f_Run2_VBF = new TFile("/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr"+intgr+"p"+decim+"/Run2_MC_VBFHToTauTau_M125_MINIAOD102X_2022_06_13.root", "READ");
  
    TTree* inTree_Zb323775 = (TTree*)f_Zb323775->Get("outTreeCalibrated");
    TTree* inTree_Zb323755 = (TTree*)f_Zb323755->Get("outTreeCalibrated");
    TTree* inTree_Run3_Snu112X = (TTree*)f_Run3_Snu112X->Get("outTreeCalibrated");
    TTree* inTree_Run3_Snu120X = (TTree*)f_Run3_Snu120X->Get("outTreeCalibrated");
    TTree* inTree_Run3_Snu122X = (TTree*)f_Run3_Snu122X->Get("outTreeCalibrated");
    TTree* inTree_Run3_VBF = (TTree*)f_Run3_VBF->Get("outTreeCalibrated");
    TTree* inTree_Run3_AOD = (TTree*)f_Run3_AOD->Get("Ntuplizer_noTagAndProbe/TagAndProbe");
    TTree* inTree_Run2_VBF = (TTree*)f_Run2_VBF->Get("Ntuplizer_noTagAndProbe/TagAndProbe");

    // namings and renamings of variables for Nvtx and nTT
    //    RAW           MINI           CALIB               
    // l1tEmu_nTT        -           L1Tau_nTT
    //     -            Nvtx           Nvtx
    //
    // available datasets 
    // ZeroBias         -> RAW, CALIB
    // SingleNeutrino   -> RAW, CALIB
    // Run3VBF          -> RAW, MINI, CALIB
    // Run2VBF          -> MINI

    TH1F* histo_Nvtx_Run3_AOD  = new TH1F("histo_Nvtx_Run3_AOD", "histo_Nvtx_Run3_AOD", 45, 0, 90);
    TH1F* histo_Nvtx_Run2_VBF  = new TH1F("histo_Nvtx_Run2_VBF", "histo_Nvtx_Run2_VBF", 45, 0, 90);
    TH1F* histo_nTT_Zb323775   = new TH1F("histo_nTT_Zb323775", "histo_nTT_Zb323775", 60, 0, 180);
    TH1F* histo_nTT_Zb323755   = new TH1F("histo_nTT_Zb323755", "histo_nTT_Zb323755", 60, 0, 180);
    TH1F* histo_nTT_Run3_Snu112X   = new TH1F("histo_nTT_Run3_Snu112X", "histo_nTT_Run3_Snu112X", 60, 0, 180);
    TH1F* histo_nTT_Run3_Snu120X   = new TH1F("histo_nTT_Run3_Snu120X", "histo_nTT_Run3_Snu120X", 60, 0, 180);
    TH1F* histo_nTT_Run3_Snu122X   = new TH1F("histo_nTT_Run3_Snu122X", "histo_nTT_Run3_Snu122X", 60, 0, 180);
    TH1F* histo_nTT_Run3_VBF   = new TH1F("histo_nTT_Run3_VBF", "histo_nTT_Run3_VBF", 60, 0, 180);

    TString CanvasName = ""; 
    if (!nTTRange) CanvasName = "Comparison_Nvtx_nTT_ZeroBias_Run2MC_Run3MC_optimizationV13_calibThr"+intgr+"p"+decim;
    else          CanvasName = "Comparison_Nvtx_nTT>"+nTTthr+"_ZeroBias_Run2MC_Run3MC_optimizationV13_calibThr"+intgr+"p"+decim;
    TString CanvasNamePdf = CanvasName ;
    CanvasNamePdf += ".pdf";

    TCanvas c(CanvasName.Data(),CanvasName.Data(),1300,600);
    c.SetLeftMargin(0.15);

    TPad *pad1 = new TPad("pad1", "pad1", 0., 0., 0.5, 1.);
    //pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
    pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad

    if (nTTRange) {
        inTree_Zb323775->Draw("L1Tau_nTT>>histo_nTT_Zb323775", "L1Tau_nTT>"+nTTthr);
        inTree_Zb323755->Draw("L1Tau_nTT>>histo_nTT_Zb323755", "L1Tau_nTT>"+nTTthr);
        inTree_Run3_Snu112X->Draw("L1Tau_nTT>>histo_nTT_Run3_Snu112X", "L1Tau_nTT>"+nTTthr, "Run3RateWeight");
        inTree_Run3_Snu120X->Draw("L1Tau_nTT>>histo_nTT_Run3_Snu120X", "L1Tau_nTT>"+nTTthr);
        inTree_Run3_Snu122X->Draw("L1Tau_nTT>>histo_nTT_Run3_Snu122X", "L1Tau_nTT>"+nTTthr);
        inTree_Run3_VBF->Draw("L1Tau_nTT>>histo_nTT_Run3_VBF", "L1Tau_nTT>"+nTTthr);
    }
    else {
        inTree_Zb323775->Draw("L1Tau_nTT>>histo_nTT_Zb323775");
        inTree_Zb323755->Draw("L1Tau_nTT>>histo_nTT_Zb323755");
        inTree_Run3_Snu112X->Draw("L1Tau_nTT>>histo_nTT_Run3_Snu112X", "Run3RateWeight");
        inTree_Run3_Snu120X->Draw("L1Tau_nTT>>histo_nTT_Run3_Snu120X");
        inTree_Run3_Snu122X->Draw("L1Tau_nTT>>histo_nTT_Run3_Snu122X");
        inTree_Run3_VBF->Draw("L1Tau_nTT>>histo_nTT_Run3_VBF");
    }

    histo_nTT_Zb323775->Scale(1/histo_nTT_Zb323775->Integral());
    histo_nTT_Zb323755->Scale(1/histo_nTT_Zb323755->Integral());
    histo_nTT_Run3_Snu112X->Scale(1/histo_nTT_Run3_Snu112X->Integral());
    histo_nTT_Run3_Snu120X->Scale(1/histo_nTT_Run3_Snu120X->Integral());
    histo_nTT_Run3_Snu122X->Scale(1/histo_nTT_Run3_Snu122X->Integral());
    histo_nTT_Run3_VBF->Scale(1/histo_nTT_Run3_VBF->Integral());

    histo_nTT_Run3_Snu120X->SetTitle("");
    histo_nTT_Run3_Snu120X->GetXaxis()->SetTitle("nTT");
    histo_nTT_Run3_Snu120X->GetYaxis()->SetTitle("a.u.");
    histo_nTT_Run3_Snu120X->SetLineColor(kBlue);
    histo_nTT_Run3_Snu120X->SetLineWidth(1);
    histo_nTT_Run3_Snu120X->Draw("hist");

    histo_nTT_Run3_Snu122X->SetTitle("");
    histo_nTT_Run3_Snu122X->GetXaxis()->SetTitle("nTT");
    histo_nTT_Run3_Snu122X->GetYaxis()->SetTitle("a.u.");
    histo_nTT_Run3_Snu122X->SetLineColor(kGreen);
    histo_nTT_Run3_Snu122X->SetLineWidth(1);
    histo_nTT_Run3_Snu122X->Draw("same hist");

    histo_nTT_Run3_Snu112X->SetLineColor(kRed);
    histo_nTT_Run3_Snu112X->SetLineWidth(1);
    histo_nTT_Run3_Snu112X->Draw("same hist");

    histo_nTT_Run3_VBF->SetLineColor(kMagenta);
    histo_nTT_Run3_VBF->SetLineWidth(1);
    histo_nTT_Run3_VBF->Draw("same hist");

    histo_nTT_Zb323775->SetLineColor(kOrange);
    histo_nTT_Zb323775->SetLineWidth(1);
    histo_nTT_Zb323775->Draw("same hist");

    histo_nTT_Zb323755->SetLineColor(kYellow);
    histo_nTT_Zb323755->SetLineWidth(1);
    histo_nTT_Zb323755->Draw("same hist");

    TLegend* leg1 = new TLegend(0.58,0.70,0.89,0.89);
    leg1->SetBorderSize(0);
    leg1->SetTextSize(0.02);
    leg1->SetHeader("Linearly->scaled to 2.0E34");
    leg1->AddEntry(histo_nTT_Zb323775, "ZeroBias Run 323775", "L");
    leg1->AddEntry(histo_nTT_Zb323755, "ZeroBias Run 323755", "L");
    leg1->AddEntry(histo_nTT_Run3_Snu112X, "SingleNeutrino Run3 MC 112X", "L");
    leg1->AddEntry(histo_nTT_Run3_Snu120X, "SingleNeutrino Run3 MC 120X", "L");
    leg1->AddEntry(histo_nTT_Run3_Snu122X, "SingleNeutrino Run3 MC 122X", "L");
    leg1->AddEntry(histo_nTT_Run3_VBF, "VBFHToTauTau Run3 MC", "L");
    leg1->Draw("same");

    TPaveText* texl1 = new TPaveText(0.05,0.87,0.60,0.99,"NDC");
    texl1->AddText("CMS Internal, #sqrt{s}=13 TeV");
    texl1->SetTextSize(0.04);
    texl1->SetFillStyle(0);
    texl1->SetBorderSize(0);
    texl1->Draw("same");
    
    c.cd();
    TPad *pad2 = new TPad("pad2", "pad2", 0.5, 0., 1., 1.);
    //pad2->SetBottomMargin(0.05); // Upper and lower plot are joined
    pad2->SetLeftMargin(0.15);
    pad2->SetGridx();         // Vertical grid
    pad2->SetGridy();         // Vertical grid
    pad2->Draw();             // Draw the upper pad: pad1
    pad2->cd();               // pad1 becomes the current pad

    inTree_Run3_AOD->Draw("Nvtx>>histo_Nvtx_Run3_AOD", "");
    inTree_Run2_VBF->Draw("Nvtx>>histo_Nvtx_Run2_VBF", "");

    histo_Nvtx_Run3_AOD->Scale(1/histo_Nvtx_Run3_AOD->Integral());
    histo_Nvtx_Run2_VBF->Scale(1/histo_Nvtx_Run2_VBF->Integral());

    histo_Nvtx_Run3_AOD->SetTitle("");
    histo_Nvtx_Run3_AOD->GetXaxis()->SetTitle("Nvtx");
    histo_Nvtx_Run3_AOD->GetYaxis()->SetTitle("a.u.");
    histo_Nvtx_Run3_AOD->SetLineColor(kBlack);
    histo_Nvtx_Run3_AOD->SetLineWidth(1);
    histo_Nvtx_Run3_AOD->Draw("hist");

    histo_Nvtx_Run2_VBF->SetLineColor(kGray);
    histo_Nvtx_Run2_VBF->SetLineWidth(1);
    histo_Nvtx_Run2_VBF->Draw("same hist");

    TLegend* leg2 = new TLegend(0.60,0.75,0.89,0.89);
    leg2->SetBorderSize(0);
    leg2->SetTextSize(0.02);
    leg2->AddEntry(histo_Nvtx_Run3_AOD, "VBFHToTauTau Run3 MC", "L");
    leg2->AddEntry(histo_Nvtx_Run2_VBF, "VBFHToTauTau Run2 MC", "L");
    leg2->Draw("same");

    TPaveText* texl2 = new TPaveText(0.05,0.87,0.60,0.99,"NDC");
    texl2->AddText("CMS Internal, #sqrt{s}=13 TeV");
    texl2->SetTextSize(0.04);
    texl2->SetFillStyle(0);
    texl2->SetBorderSize(0);
    texl2->Draw("same");

    c.cd();

    TString PDFs = "PDFs/";
    c.SaveAs(PDFs+CanvasNamePdf.Data());

    f_Zb323775->Close();
    f_Zb323755->Close();
    f_Run3_Snu112X->Close();
    f_Run3_Snu120X->Close();
    f_Run3_Snu122X->Close();
    f_Run3_VBF->Close();
    f_Run2_VBF->Close();

}
