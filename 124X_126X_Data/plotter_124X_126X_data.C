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
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TProfile.h>
#include <sstream>
#include <TBranchElement.h>
#include <TGraphAsymmErrors.h>
#include <fstream>

using namespace std;

void plotter_124X_126X_data()
{
    gStyle->SetOptStat(000000);
    gROOT->SetBatch(true);

    TChain data("Ntuplizer_TagAndProbe");
    TChain mc124X("Ntuplizer_noTagAndProbe_TagAndProbe");
    TChain mc126X("Ntuplizer_noTagAndProbe_TagAndProbe");

    data.Add("/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_04_optimizationV0_calibThr1p7/Muon_Run2022G-v1_MERGED_2023_03_13_0.root");
    mc124X.Add("/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_04_optimizationV0_calibThr1p7/*124X*MERGED_2023_03_04_0.root");
    // mc126X.Add("/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_13_optimizationV1_calibThr1p7/*126X*MERGED_2023_03_13_0.root"); // with HCAL re-emu integration branch 147
    // mc126X.Add("/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_21_optimizationVtest/*126X*forPU65_MERGED_2023_03_13_0.root"); // without HCAL re-emu
    // mc126X.Add("/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_21_optimizationVtest/*126X*forPU65_ZS0p5_MERGED_2023_03_13_0.root"); // without HCAL re-emu & with ZS
    // mc126X.Add("/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_22_optimizationVtest/*126X*forPU65_MERGED_2023_03_13_0.root"); // with HCAL re-emu integration branch 147
    mc126X.Add("/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_22_optimizationVtest/*126X*forPU65_ZS0p5_MERGED_2023_03_13_0.root"); // with HCAL re-emu integration branch 147 & with ZS

    TH1F* nTT_data = new TH1F("nTT_data","nTT_data",470,0.,470);
    TH1F* nTT_mc124X = new TH1F("nTT_mc124X","nTT_mc124X",470,0.,470);
    TH1F* nTT_mc126X = new TH1F("nTT_mc126X","nTT_mc126X",470,0.,470);

    data.Draw("l1tEmuNTT>>nTT_data","","hist");
    mc124X.Draw("l1tEmuNTT>>nTT_mc124X","","hist");
    mc126X.Draw("l1tEmuNTT>>nTT_mc126X","","hist");
    nTT_data->Scale(1.0/nTT_data->Integral());
    nTT_mc124X->Scale(1.0/nTT_mc124X->Integral());
    nTT_mc126X->Scale(1.0/nTT_mc126X->Integral());

    TCanvas c("c","c",650,600);
    TPad *pad = new TPad("pad1", "pad1", 0, 0., 1, 1.0);
    pad->SetGridx();         // Vertical grid
    pad->SetGridy();         // Vertical grid
    pad->Draw();             // Draw the upper pad: pad1
    pad->cd();               // pad1 becomes the current pad
    pad->SetLogy();

    nTT_mc124X->SetTitle("");
    nTT_mc124X->GetXaxis()->SetRangeUser(0.,470.);
    nTT_mc124X->GetYaxis()->SetTitle("a.u.");
    nTT_mc124X->GetXaxis()->SetTitle("nTT");
    nTT_mc124X->SetLineColor(kGreen);
    nTT_mc124X->SetLineWidth(2);
    nTT_mc124X->Draw();

    nTT_mc126X->SetLineColor(kRed);
    nTT_mc126X->SetLineWidth(2);
    nTT_mc126X->Draw("same");

    nTT_data->SetLineColor(kBlack);
    nTT_data->SetLineWidth(2);
    nTT_data->Draw("same");

    TPaveText* texl = new TPaveText(0.05,0.87,0.35,0.99,"NDC");
    texl->AddText("CMS Internal");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    texl->Draw("same");

    TLegend* leg = new TLegend(0.4,0.8,0.7,0.89);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);
    leg->SetHeader("Normalised histograms");
    leg->AddEntry(nTT_data,"Era G Data","L");
    leg->AddEntry(nTT_mc124X,"MC 124X","L");
    leg->AddEntry(nTT_mc126X,"MC 126X","L");
    leg->Draw("same");
    
    c.SaveAs("./124X_126X_data_nTT.pdf");

    //#################################################################################
    //#################################################################################

    TH1F* isoEt_data = new TH1F("isoEt_data","isoEt_data",150,0.,150.);
    TH1F* isoEt_mc124X = new TH1F("isoEt_mc124X","isoEt_mc124X",150,0.,150.);
    TH1F* isoEt_mc126X = new TH1F("isoEt_mc126X","isoEt_mc126X",150,0.,150.);

    data.Draw("l1tEmuIsoEt>>isoEt_data","","hist");
    mc124X.Draw("l1tEmuIsoEt>>isoEt_mc124X","","hist");
    mc126X.Draw("l1tEmuIsoEt>>isoEt_mc126X","","hist");
    isoEt_data->Scale(1.0/isoEt_data->Integral());
    isoEt_mc124X->Scale(1.0/isoEt_mc124X->Integral());
    isoEt_mc126X->Scale(1.0/isoEt_mc126X->Integral());

    TCanvas c1("c1","c1",650,600);
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0., 1, 1.0);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    pad1->SetLogy();

    isoEt_mc124X->SetTitle("");
    isoEt_mc124X->GetXaxis()->SetRangeUser(0.,150.);
    isoEt_mc124X->GetYaxis()->SetTitle("a.u.");
    isoEt_mc124X->GetXaxis()->SetTitle("E_{T}^{Iso} [GeV]");
    isoEt_mc124X->SetLineColor(kGreen);
    isoEt_mc124X->SetLineWidth(2);
    isoEt_mc124X->Draw();

    isoEt_mc126X->SetLineColor(kRed);
    isoEt_mc126X->SetLineWidth(2);
    isoEt_mc126X->Draw("same");

    isoEt_data->SetLineColor(kBlack);
    isoEt_data->SetLineWidth(2);
    isoEt_data->Draw("same");

    TLegend* leg1 = new TLegend(0.7,0.8,0.89,0.89);
    leg1->SetBorderSize(0);
    leg1->SetTextSize(0.02);
    leg1->SetHeader("Normalised histograms");
    leg1->AddEntry(isoEt_data,"Era G Data","L");
    leg1->AddEntry(isoEt_mc124X,"MC 124X","L");
    leg1->AddEntry(isoEt_mc126X,"MC 126X","L");
    leg1->Draw("same");

    texl->Draw("same");
    
    c1.SaveAs("./124X_126X_data_isoEt.pdf");

    //#################################################################################
    //#################################################################################

    TH2F* nTT_vs_nVtx_data = new TH2F("nTT_vs_nVtx_data","nTT_vs_nVtx_data",100,0.,100,470,0.,470.);
    TH2F* nTT_vs_nVtx_mc124X = new TH2F("nTT_vs_nVtx_mc124X","nTT_vs_nVtx_mc124X",100,0.,100,470,0.,470.);
    TH2F* nTT_vs_nVtx_mc126X = new TH2F("nTT_vs_nVtx_mc126X","nTT_vs_nVtx_mc126X",100,0.,100,470,0.,470.);

    data.Draw("l1tEmuNTT:Nvtx>>nTT_vs_nVtx_data","","colz");
    mc124X.Draw("l1tEmuNTT:Nvtx>>nTT_vs_nVtx_mc124X","","colz");
    mc126X.Draw("l1tEmuNTT:Nvtx>>nTT_vs_nVtx_mc126X","","colz");

    TProfile* nTT_vs_nVtx_profile_data = nTT_vs_nVtx_data->ProfileX("nTT_vs_nVtx_profile_data");
    TProfile* nTT_vs_nVtx_profile_mc124X = nTT_vs_nVtx_mc124X->ProfileX("nTT_vs_nVtx_profile_mc124X");
    TProfile* nTT_vs_nVtx_profile_mc126X = nTT_vs_nVtx_mc126X->ProfileX("nTT_vs_nVtx_profile_mc126X");

    TCanvas c2("c2","c2",650,600);
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0., 1, 1.0);
    pad2->SetGridx();         // Vertical grid
    pad2->SetGridy();         // Vertical grid
    pad2->Draw();             // Draw the upper pad: pad1
    pad2->cd();               // pad1 becomes the current pad
    // pad2->SetLogy();

    nTT_vs_nVtx_profile_mc124X->SetTitle("");
    nTT_vs_nVtx_profile_mc124X->GetXaxis()->SetRangeUser(0.,150.);
    nTT_vs_nVtx_profile_mc124X->GetYaxis()->SetRangeUser(0.,470.);
    nTT_vs_nVtx_profile_mc124X->GetXaxis()->SetTitle("nVtx");
    nTT_vs_nVtx_profile_mc124X->GetYaxis()->SetTitle("nTT");
    nTT_vs_nVtx_profile_mc124X->SetLineColor(kGreen);
    nTT_vs_nVtx_profile_mc124X->SetLineWidth(2);
    nTT_vs_nVtx_profile_mc124X->Draw();

    nTT_vs_nVtx_profile_mc126X->SetLineColor(kRed);
    nTT_vs_nVtx_profile_mc126X->SetLineWidth(2);
    nTT_vs_nVtx_profile_mc126X->Draw("same");

    nTT_vs_nVtx_profile_data->SetLineColor(kBlack);
    nTT_vs_nVtx_profile_data->SetLineWidth(2);
    nTT_vs_nVtx_profile_data->Draw("same");

    TLegend* leg2 = new TLegend(0.7,0.5,0.89,0.59);
    leg2->SetBorderSize(0);
    leg2->SetTextSize(0.02);
    leg2->SetHeader("Normalised histograms");
    leg2->AddEntry(nTT_vs_nVtx_profile_data,"Era G Data","L");
    leg2->AddEntry(nTT_vs_nVtx_profile_mc124X,"MC 124X","L");
    leg2->AddEntry(nTT_vs_nVtx_profile_mc126X,"MC 126X","L");
    leg2->Draw("same");

    texl->Draw("same");
    
    c2.SaveAs("./124X_126X_data_nTT_vs_nVtx.pdf");

    //#################################################################################
    //#################################################################################

    TH2F* isoEt_vs_nVtx_data = new TH2F("isoEt_vs_nVtx_data","isoEt_vs_nVtx_data",100,0.,100.,150,0.,150.);
    TH2F* isoEt_vs_nVtx_mc124X = new TH2F("isoEt_vs_nVtx_mc124X","isoEt_vs_nVtx_mc124X",100,0.,100.,150,0.,150.);
    TH2F* isoEt_vs_nVtx_mc126X = new TH2F("isoEt_vs_nVtx_mc126X","isoEt_vs_nVtx_mc126X",100,0.,100.,150,0.,150.);

    data.Draw("l1tEmuIsoEt:Nvtx>>isoEt_vs_nVtx_data","","colz");
    mc124X.Draw("l1tEmuIsoEt:Nvtx>>isoEt_vs_nVtx_mc124X","","colz");
    mc126X.Draw("l1tEmuIsoEt:Nvtx>>isoEt_vs_nVtx_mc126X","","colz");

    TProfile* isoEt_vs_nVtx_profile_data = isoEt_vs_nVtx_data->ProfileX("iso_vs_nvtx_profile_data");
    TProfile* isoEt_vs_nVtx_profile_mc124X = isoEt_vs_nVtx_mc124X->ProfileX("iso_vs_nvtx_profile_mc124X");
    TProfile* isoEt_vs_nVtx_profile_mc126X = isoEt_vs_nVtx_mc126X->ProfileX("iso_vs_nvtx_profile_mc126X");

    TCanvas c3("c3","c3",650,600);
    TPad *pad3 = new TPad("pad3", "pad3", 0, 0., 1, 1.0);
    pad3->SetGridx();         // Vertical grid
    pad3->SetGridy();         // Vertical grid
    pad3->Draw();             // Draw the upper pad: pad1
    pad3->cd();               // pad1 becomes the current pad
    // pad2->SetLogy();

    isoEt_vs_nVtx_profile_mc124X->SetTitle("");
    isoEt_vs_nVtx_profile_mc124X->GetXaxis()->SetRangeUser(0.,100.);
    isoEt_vs_nVtx_profile_mc124X->GetYaxis()->SetRangeUser(0.,80.);
    isoEt_vs_nVtx_profile_mc124X->GetXaxis()->SetTitle("nVtx");
    isoEt_vs_nVtx_profile_mc124X->GetYaxis()->SetTitle("E_{T}^{Iso} [GeV]");
    isoEt_vs_nVtx_profile_mc124X->SetLineColor(kGreen);
    isoEt_vs_nVtx_profile_mc124X->SetLineWidth(2);
    isoEt_vs_nVtx_profile_mc124X->Draw();

    isoEt_vs_nVtx_profile_mc126X->SetLineColor(kRed);
    isoEt_vs_nVtx_profile_mc126X->SetLineWidth(2);
    isoEt_vs_nVtx_profile_mc126X->Draw("same");

    isoEt_vs_nVtx_profile_data->SetLineColor(kBlack);
    isoEt_vs_nVtx_profile_data->SetLineWidth(2);
    isoEt_vs_nVtx_profile_data->Draw("same");

    TLegend* leg3 = new TLegend(0.7,0.8,0.89,0.89);
    leg3->SetBorderSize(0);
    leg3->SetTextSize(0.02);
    leg3->SetHeader("Normalised histograms");
    leg3->AddEntry(isoEt_vs_nVtx_profile_data,"Era G Data","L");
    leg3->AddEntry(isoEt_vs_nVtx_profile_mc124X,"MC 124X","L");
    leg3->AddEntry(isoEt_vs_nVtx_profile_mc126X,"MC 126X","L");
    leg3->Draw("same");

    texl->Draw("same");
    
    c3.SaveAs("./124X_126X_data_isoEt_vs_nVtx.pdf");
    
    //#################################################################################
    //#################################################################################

    TH2F* isoEt_vs_nTT_data = new TH2F("isoEt_vs_nTT_data","isoEt_vs_nTT_data",470,0.,470.,150,0.,150.);
    TH2F* isoEt_vs_nTT_mc124X = new TH2F("isoEt_vs_nTT_mc124X","isoEt_vs_nTT_mc124X",470,0.,470.,150,0.,150.);
    TH2F* isoEt_vs_nTT_mc126X = new TH2F("isoEt_vs_nTT_mc126X","isoEt_vs_nTT_mc126X",470,0.,470.,150,0.,150.);

    data.Draw("l1tEmuIsoEt:l1tEmuNTT>>isoEt_vs_nTT_data","","colz");
    mc124X.Draw("l1tEmuIsoEt:l1tEmuNTT>>isoEt_vs_nTT_mc124X","","colz");
    mc126X.Draw("l1tEmuIsoEt:l1tEmuNTT>>isoEt_vs_nTT_mc126X","","colz");    

    TProfile* isoEt_vs_nTT_profile_data = isoEt_vs_nTT_data->ProfileX("iso_vs_nvtx_profile_data");
    TProfile* isoEt_vs_nTT_profile_mc124X = isoEt_vs_nTT_mc124X->ProfileX("iso_vs_nvtx_profile_mc124X");
    TProfile* isoEt_vs_nTT_profile_mc126X = isoEt_vs_nTT_mc126X->ProfileX("iso_vs_nvtx_profile_mc126X");

    TCanvas c4("c4","c4",650,600);
    TPad *pad4 = new TPad("pad4", "pad4", 0, 0., 1, 1.0);
    pad4->SetGridx();         // Vertical grid
    pad4->SetGridy();         // Vertical grid
    pad4->Draw();             // Draw the upper pad: pad1
    pad4->cd();               // pad1 becomes the current pad
    // pad2->SetLogy();

    isoEt_vs_nTT_profile_mc124X->SetTitle("");
    isoEt_vs_nTT_profile_mc124X->GetXaxis()->SetRangeUser(0.,470.);
    isoEt_vs_nTT_profile_mc124X->GetYaxis()->SetRangeUser(0.,80.);
    isoEt_vs_nTT_profile_mc124X->GetXaxis()->SetTitle("nTT");
    isoEt_vs_nTT_profile_mc124X->GetYaxis()->SetTitle("E_{T}^{Iso} [GeV]");
    isoEt_vs_nTT_profile_mc124X->SetLineColor(kGreen);
    isoEt_vs_nTT_profile_mc124X->SetLineWidth(2);
    isoEt_vs_nTT_profile_mc124X->Draw();

    isoEt_vs_nTT_profile_mc126X->SetLineColor(kRed);
    isoEt_vs_nTT_profile_mc126X->SetLineWidth(2);
    isoEt_vs_nTT_profile_mc126X->Draw("same");

    isoEt_vs_nTT_profile_data->SetLineColor(kBlack);
    isoEt_vs_nTT_profile_data->SetLineWidth(2);
    isoEt_vs_nTT_profile_data->Draw("same");

    TLegend* leg4 = new TLegend(0.7,0.8,0.89,0.89);
    leg4->SetBorderSize(0);
    leg4->SetTextSize(0.02);
    leg4->SetHeader("Normalised histograms");
    leg4->AddEntry(isoEt_vs_nTT_profile_data,"Era G Data","L");
    leg4->AddEntry(isoEt_vs_nTT_profile_mc124X,"MC 124X","L");
    leg4->AddEntry(isoEt_vs_nTT_profile_mc126X,"MC 126X","L");
    leg4->Draw("same");

    texl->Draw("same");
    
    c4.SaveAs("./124X_126X_data_isoEt_vs_nTT.pdf");


}