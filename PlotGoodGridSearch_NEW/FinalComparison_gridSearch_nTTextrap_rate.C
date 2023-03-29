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

void compare(int targetRate = 14, int fixedThr = 0, TString parametrisation = "linear", Double_t Kfact = 0.0, bool zoom=false, int run=-1, float calibThr = 1.7) {
    TString run_str = to_string(run);

    TString fixedRate = to_string(targetRate);
    TString fixedThreshold = to_string(fixedThr);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    TString Kintgr = to_string(Kfact).substr(0, to_string(Kfact).find("."));
    TString Kdecim = to_string(Kfact).substr(2, to_string(Kfact).find("."));

    gStyle->SetOptStat(000000);

    //#############################################
    TString bestFixedRate = "_effMin0p9_eMin25_eMax40";
    TString bestFixedThr  = "_effMin0p0_eMin22_eMax37";
    
    TFile f_Turnons1("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/histos_rate_ZeroBias_Run362617_optimizationV0gs_calibThr1p7_linear.root","READ");
    TFile f_Turnons3("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/histos_rate_ZeroBias_Run362617_unpacked.root", "READ");
    // TFile f_Thresholds("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/thresholds_fixedrate_ZeroBias_Run"+run_str+"_optimization"+version+"gs_calibThr"+intgr+"p"+decim+"_"+tag+".root","READ");

    TString TurnonName1 = "DiTauRate"+bestFixedRate;
    TH1F* bestFixedRateTurnon = (TH1F*)f_Turnons1.Get(TurnonName1);

    TString TurnonName2 = "DiTauRate"+bestFixedThr;
    TH1F* bestFixedThrTrunon = (TH1F*)f_Turnons1.Get(TurnonName2);

    TString TurnonName3 = "DiTauRate_Iso";
    TH1F* unpackedData = (TH1F*)f_Turnons3.Get(TurnonName3);


    TCanvas c("c","c",650,600);
    c.SetLeftMargin(0.15);
    c.SetGrid();
    c.SetLogy();

    TLegend* leg = new TLegend(0.45,0.79,0.89,0.89);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.015);

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0., 1, 1.0);
    //pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
    pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    pad1->SetLogy();
    // pad1->SetLogx();

    bestFixedRateTurnon->SetTitle("");
    bestFixedRateTurnon->SetLineColor(kBlue);
    bestFixedRateTurnon->SetLineWidth(2);
    bestFixedRateTurnon->GetXaxis()->SetLimits(0,60.);
    bestFixedRateTurnon->GetYaxis()->SetRangeUser(1.0,1E5);
    bestFixedRateTurnon->GetYaxis()->SetTitle("Rate [kHz]");
    bestFixedRateTurnon->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) [GeV]");
    bestFixedRateTurnon->Draw();

    bestFixedThrTrunon->SetLineColor(kRed);
    bestFixedThrTrunon->SetLineWidth(2);
    bestFixedThrTrunon->Draw("same");

    unpackedData->SetLineColor(kBlack);
    unpackedData->SetLineWidth(2);
    unpackedData->Draw("same");
    
    leg->AddEntry(bestFixedRateTurnon,"Fixed rate opt. - THR = 46 GeV - RATE = 14 kHz","L");
    leg->AddEntry(bestFixedThrTrunon,"Fixed threshold opt. - THR = 34 GeV - RATE = 19kHz ","L");
    leg->AddEntry(unpackedData,"Unpacked Data 2022 - THR = 34 GeV - RATE = 11kHz ","L");
    leg->Draw("same");

    TPaveText* texl = new TPaveText(0.15,0.87,0.35,0.99,"NDC");
    texl->AddText("CMS Internal");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    pad1->cd();
    texl->Draw("same");   

    c.SaveAs("PDFs/done2.pdf");

}





































