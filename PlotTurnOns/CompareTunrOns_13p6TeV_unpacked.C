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

void compare() {
    gStyle->SetOptStat(000000);
    
    TFile f_Run356375("/data_CMS/cms/motta/Run3preparation/SingleMuon_Run2022C_Run356375_MINIAOD/SingleMuon_Run2022C_Run356375_TURNONS.root");
    TFile f_Run356378("/data_CMS/cms/motta/Run3preparation/SingleMuon_Run2022C_Run356378_MINIAOD/SingleMuon_Run2022C_Run356378_TURNONS.root");
    TFile f_Run356381("/data_CMS/cms/motta/Run3preparation/SingleMuon_Run2022C_Run356381_MINIAOD/SingleMuon_Run2022C_Run356381_TURNONS.root");

    TGraphAsymmErrors* turnon_Run356375_NoIso = (TGraphAsymmErrors*)f_Run356375.Get("divide_pt_pass_noIso_by_pt");
    TGraphAsymmErrors* turnon_Run356378_NoIso = (TGraphAsymmErrors*)f_Run356378.Get("divide_pt_pass_noIso_by_pt");
    TGraphAsymmErrors* turnon_Run356381_NoIso = (TGraphAsymmErrors*)f_Run356381.Get("divide_pt_pass_noIso_by_pt");

    TGraphAsymmErrors* turnon_Run356375_Iso = (TGraphAsymmErrors*)f_Run356375.Get("divide_pt_pass_Iso_by_pt");
    TGraphAsymmErrors* turnon_Run356378_Iso = (TGraphAsymmErrors*)f_Run356378.Get("divide_pt_pass_Iso_by_pt");
    TGraphAsymmErrors* turnon_Run356381_Iso = (TGraphAsymmErrors*)f_Run356381.Get("divide_pt_pass_Iso_by_pt");

    TString CanvasName = "Comparison_TurnOn_ZeroBias_Run2022C_Fill8063_unpacked";
    TString CanvasNamePdf = CanvasName ;
    CanvasNamePdf += ".pdf";

    TCanvas c(CanvasName.Data(),CanvasName.Data(),650,600);
    c.SetLeftMargin(0.15);
    c.SetGrid();
    c.SetLogy();

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0., 1, 1.0);
    pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
    pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    //pad1->SetLogy();

    turnon_Run356381_NoIso->SetTitle("");
    // turnon_Run356375_NoIso->GetXaxis()->SetRangeUser(0.,120.);
    turnon_Run356381_NoIso->GetYaxis()->SetTitle("Efficiency");
    turnon_Run356381_NoIso->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) [GeV]");
    turnon_Run356381_NoIso->SetLineColor(kRed);
    turnon_Run356381_NoIso->SetLineWidth(2);
    turnon_Run356381_NoIso->GetXaxis()->SetLimits(15,120.);
    // turnon_Run356375_NoIso->Draw();

    turnon_Run356378_NoIso->SetLineColor(kGreen-7);
    turnon_Run356378_NoIso->SetLineWidth(2);
    // turnon_Run356378_NoIso->Draw("same");

    turnon_Run356381_NoIso->SetLineColor(kBlue);
    turnon_Run356381_NoIso->SetLineWidth(2);
    turnon_Run356381_NoIso->Draw();

    turnon_Run356375_Iso->SetLineColor(kRed-10);
    turnon_Run356375_Iso->SetLineWidth(2);
    // turnon_Run356375_Iso->Draw("same");

    turnon_Run356378_Iso->SetLineColor(kGreen-10);
    turnon_Run356378_Iso->SetLineWidth(2);
    // turnon_Run356378_Iso->Draw("same");

    turnon_Run356381_Iso->SetLineColor(kBlue-10);
    turnon_Run356381_Iso->SetLineWidth(2);
    turnon_Run356381_Iso->Draw("same");  

    TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
    texl->AddText("CMS Internal, #sqrt{s}=13.6 TeV, Fill 8063 (2022)");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    texl->Draw("same");

    TLegend* leg = new TLegend(0.45,0.07,0.89,0.35);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);
    leg->SetHeader("Linearly scaled to 2.0E34");
    // leg->AddEntry(turnon_Run356375_NoIso,"Di-#tau No-Iso p_{T}^{L1}>40 GeV (Run356375)","L");
    // leg->AddEntry(turnon_Run356378_NoIso,"Di-#tau No-Iso p_{T}^{L1}>41 GeV (Run356378)","L");
    leg->AddEntry(turnon_Run356381_NoIso,"Di-#tau No-Iso p_{T}^{L1}>41 GeV (Run356381)","L");
    // leg->AddEntry(turnon_Run356375_Iso,"Di-#tau Iso p_{T}^{L1}>30 GeV (Run356375)","L");
    // leg->AddEntry(turnon_Run356378_Iso,"Di-#tau Iso p_{T}^{L1}>31 GeV (Run356378)","L");
    leg->AddEntry(turnon_Run356381_Iso,"Di-#tau Iso p_{T}^{L1}>31 GeV (Run356381)","L");
    leg->Draw("same");

    TString PDFs = "PDFs/Run3_13p6TeV/";
    c.SaveAs(PDFs+CanvasNamePdf.Data());

    f_Run356375.Close();
    f_Run356378.Close();
    f_Run356381.Close();

}
