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

void compare(TString tag, TString run_str, TString fill_str) {
    gStyle->SetOptStat(000000);
    
    TFile f("/data_CMS/cms/motta/Run3preparation/Muon_Run2022"+tag+"_"+run_str+"_MINIAOD/Muon_Run2022"+tag+"_Fill"+fill_str+"_TURNONS.root");

    TGraphAsymmErrors* turnon_NoIso = (TGraphAsymmErrors*)f.Get("divide_pt_pass_noIso_by_pt");
    TGraphAsymmErrors* turnon_Iso = (TGraphAsymmErrors*)f.Get("divide_pt_pass_Iso_by_pt");

    TString CanvasName = "Comparison_TurnOn_ZeroBias_Run2022C_Fill"+fill_str+"_unpacked";
    TString CanvasNamePdf = CanvasName ;
    CanvasNamePdf += ".pdf";

    TCanvas c(CanvasName.Data(),CanvasName.Data(),650,600);
    c.SetLeftMargin(0.15);
    c.SetGrid();
    c.SetLogy();

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0., 1, 1.0);
    // pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
    // pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    //pad1->SetLogy();

    turnon_NoIso->SetTitle("");
    turnon_NoIso->GetYaxis()->SetTitle("Efficiency");
    turnon_NoIso->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) [GeV]");
    turnon_NoIso->SetLineColor(kRed);
    turnon_NoIso->SetLineWidth(2);
    turnon_NoIso->GetXaxis()->SetLimits(15,120.);
    turnon_NoIso->Draw();

    turnon_Iso->SetLineColor(kBlue-10);
    turnon_Iso->SetLineWidth(2);
    turnon_Iso->Draw("same");  

    TPaveText* texl1 = new TPaveText(0.15,0.87,0.2,0.99,"NDC");
    texl1->AddText("CMS Internal");
    texl1->SetTextSize(0.03);
    texl1->SetFillStyle(0);
    texl1->SetBorderSize(0);
    texl1->Draw("same");

    TPaveText* texl2 = new TPaveText(0.68,0.87,0.89,0.99,"NDC");
    texl2->AddText("#sqrt{s} = 13.6 TeV (2022)");
    texl2->SetTextSize(0.03);
    texl2->SetFillStyle(0);
    texl2->SetBorderSize(0);
    texl2->Draw("same");

    TLegend* leg = new TLegend(0.4,0.15,0.89,0.3);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);
    leg->AddEntry(turnon_NoIso,"Di-#tau No-Iso p_{T}^{L1}>36 GeV (Fill "+fill_str+")","L");
    leg->AddEntry(turnon_Iso,"Di-#tau Iso p_{T}^{L1}>29 GeV (Fill "+fill_str+")","L");
    leg->Draw("same");

    TString PDFs = "PDFs/Run3_13p6TeV/";
    c.SaveAs(PDFs+CanvasNamePdf.Data());

    f.Close();

}
