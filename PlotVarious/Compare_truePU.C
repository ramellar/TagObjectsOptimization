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

void compare(float calibThr = 1.7) {
    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    gStyle->SetOptStat(000000);

    TFile* f_Run3_Snu112X = new TFile("/data_CMS/cms/motta/Run3preparation/2022_04_21_optimizationV11_calibThr"+intgr+"p"+decim+"/Run3_MC_SingleNeutrinoGun_RAW112X_2022_04_21/Run3_MC_SingleNeutrinoGun_RAW112X_2022_04_21.root", "READ");
    TFile* f_Run3_Snu120X = new TFile("/data_CMS/cms/motta/Run3preparation/2022_06_10_optimizationV12_calibThr"+intgr+"p"+decim+"/Run3_MC_SingleNeutrinoGun_RAW120X_2022_06_10/Run3_MC_SingleNeutrinoGun_RAW120X_2022_06_10.root", "READ");
    TFile* f_Run3_Snu122X = new TFile("/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr"+intgr+"p"+decim+"/Run3_MC_SingleNeutrinoGun_RAW122X_2022_06_13/Run3_MC_SingleNeutrinoGun_RAW122X_2022_06_13.root", "READ");
  
    TTree* inTree_Run3_Snu112X = (TTree*)f_Run3_Snu112X->Get("Run3nuMC/ZeroBias");
    TTree* inTree_Run3_Snu120X = (TTree*)f_Run3_Snu120X->Get("Run3nuMC/ZeroBias");
    TTree* inTree_Run3_Snu122X = (TTree*)f_Run3_Snu122X->Get("Run3nuMC/ZeroBias");

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

    TH1F* histo_truePU_Run3_Snu112Xrew = new TH1F("histo_truePU_Run3_Snu112Xrew", "histo_truePU_Run3_Snu112Xrew", 130, 0, 130);
    TH1F* histo_truePU_Run3_Snu120X    = new TH1F("histo_truePU_Run3_Snu120X", "histo_truePU_Run3_Snu120X", 130, 0, 130);
    TH1F* histo_truePU_Run3_Snu122X    = new TH1F("histo_truePU_Run3_Snu122X", "histo_truePU_Run3_Snu122X", 130, 0, 130);

    TString CanvasName = ""; 
    CanvasName = "Comparison_truePU_Run3nuMC_optimizationV13_calibThr"+intgr+"p"+decim;
    TString CanvasNamePdf = CanvasName ;
    CanvasNamePdf += ".pdf";

    TCanvas c(CanvasName.Data(),CanvasName.Data(),700,600);
    c.SetLeftMargin(0.15);

    TPad *pad1 = new TPad("pad1", "pad1", 0., 0., 1., 1.);
    //pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
    pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad

    inTree_Run3_Snu112X->Draw("TruePU>>histo_truePU_Run3_Snu112Xrew", "Run3RateWeight");
    inTree_Run3_Snu120X->Draw("TruePU>>histo_truePU_Run3_Snu120X");
    inTree_Run3_Snu122X->Draw("TruePU>>histo_truePU_Run3_Snu122X");

    histo_truePU_Run3_Snu112Xrew->Scale(1/histo_truePU_Run3_Snu112Xrew->Integral());
    histo_truePU_Run3_Snu120X->Scale(1/histo_truePU_Run3_Snu120X->Integral());
    histo_truePU_Run3_Snu122X->Scale(1/histo_truePU_Run3_Snu122X->Integral());

    histo_truePU_Run3_Snu112Xrew->SetTitle("");
    histo_truePU_Run3_Snu112Xrew->GetXaxis()->SetTitle("nTruePU");
    histo_truePU_Run3_Snu112Xrew->GetYaxis()->SetTitle("a.u.");
    histo_truePU_Run3_Snu112Xrew->SetLineColor(kRed);
    histo_truePU_Run3_Snu112Xrew->SetLineWidth(2);
    histo_truePU_Run3_Snu112Xrew->Draw("hist");

    histo_truePU_Run3_Snu120X->SetLineColor(kBlue);
    histo_truePU_Run3_Snu120X->SetLineWidth(2);
    histo_truePU_Run3_Snu120X->Draw("same hist");

    histo_truePU_Run3_Snu122X->SetLineColor(kGreen+2);
    histo_truePU_Run3_Snu122X->SetLineWidth(2);
    histo_truePU_Run3_Snu122X->Draw("same hist");

    TLegend* leg1 = new TLegend(0.49,0.70,0.89,0.89);
    leg1->SetBorderSize(0);
    leg1->SetTextSize(0.02);
    leg1->AddEntry(histo_truePU_Run3_Snu112Xrew, "SingleNeutrino Run3 MC 112X reweighted", "L");
    leg1->AddEntry(histo_truePU_Run3_Snu120X, "SingleNeutrino Run3 MC 120X", "L");
    leg1->AddEntry(histo_truePU_Run3_Snu122X, "SingleNeutrino Run3 MC 122X", "L");
    leg1->Draw("same");

    TPaveText* texl1 = new TPaveText(0.05,0.87,0.60,0.99,"NDC");
    texl1->AddText("CMS Internal, #sqrt{s}=13 TeV");
    texl1->SetTextSize(0.04);
    texl1->SetFillStyle(0);
    texl1->SetBorderSize(0);
    texl1->Draw("same");

    TString PDFs = "PDFs/";
    c.cd();
    c.SaveAs(PDFs+CanvasNamePdf.Data());

    f_Run3_Snu112X->Close();
    f_Run3_Snu120X->Close();
    f_Run3_Snu122X->Close();

}
