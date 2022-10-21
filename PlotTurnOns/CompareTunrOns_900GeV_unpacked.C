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

    TFile* f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_04_21_optimizationV11_ReOptReEmu/ZeroBias1_Run2022A_reEmuTPs_TURNONS.root", "READ");
  
    TGraphAsymmErrors* turnOn_noIso = (TGraphAsymmErrors*)f->Get("divide_pt_pass_noIso_by_pt");
    TGraphAsymmErrors* turnOn_OptionG3 = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionG3_by_pt");
    //TGraphAsymmErrors* turnOn_fixed15 = (TGraphAsymmErrors*)f->Get("divide_pt_pass_fixed15_by_pt");
    //TGraphAsymmErrors* turnOn_fixed20 = (TGraphAsymmErrors*)f->Get("divide_pt_pass_fixed20_by_pt");
    //TGraphAsymmErrors* turnOn_fixed25 = (TGraphAsymmErrors*)f->Get("divide_pt_pass_fixed25_by_pt");

    TString CanvasName = "Comparison_TurnOn_ZeroBias1_Run2022A_optimizationV11gs_effMin0p9_G3_calibThr1p7_V11_unpacked";
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


    turnOn_noIso->SetTitle("");
    turnOn_noIso->GetXaxis()->SetRangeUser(0.,120.);
    turnOn_noIso->GetYaxis()->SetTitle("Efficiency");
    turnOn_noIso->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) [GeV]");
    turnOn_noIso->SetLineColor(kBlack);
    turnOn_noIso->SetLineWidth(2);
    turnOn_noIso->GetXaxis()->SetLimits(15,120.);
    turnOn_noIso->Draw();

    turnOn_OptionG3->SetLineColor(kGray);
    turnOn_OptionG3->SetLineWidth(2);
    turnOn_OptionG3->Draw("same");

    //turnOn_fixed15->SetLineColor(kBlue);
    //turnOn_fixed15->SetLineWidth(2);
    //turnOn_fixed15->Draw("same");

    //turnOn_fixed20->SetLineColor(kMagenta);
    //turnOn_fixed20->SetLineWidth(2);
    //turnOn_fixed20->Draw("same");

    //turnOn_fixed25->SetLineColor(kGreen);
    //turnOn_fixed25->SetLineWidth(2);
    //turnOn_fixed25->Draw("same");

    TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
    texl->AddText("CMS Internal, #sqrt{s}=900 GeV, Runs 352417..353709 (2022)");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    texl->Draw("same");

    TLegend* leg = new TLegend(0.65,0.1,0.81,0.62);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);
    leg->SetHeader("Linearly scaled to 2.0E34");

    leg->AddEntry(turnOn_noIso,"Di-#tau No-Iso  ","L");
    leg->AddEntry(turnOn_OptionG3,"Di-#tau Iso p_T^{L1}>35 GeV (nominal option)","L");
    //leg->AddEntry(turnOn_fixed15,"Di-#tau Iso p_T^{L1}>15 GeV","L");
    //leg->AddEntry(turnOn_fixed20,"Di-#tau Iso p_T^{L1}>20 GeV","L");
    //leg->AddEntry(turnOn_fixed25,"Di-#tau Iso p_T^{L1}>25 GeV","L");

    leg->Draw("same");

    TString PDFs = "PDFs/Run3_900GeV/";
    c.SaveAs(PDFs+CanvasNamePdf.Data());

    f->Close();

}
