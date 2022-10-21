#include <TROOT.h>
#include <TFile.h>
#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TProfile.h>
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
    
    TFile f1("./SingleMuonTAPRun323775_Run3MCVBFHToTauTau_SingleMuonTAPFill8063_SingleMuonTAPFill8063ReEmuTP.root");

    TProfile* isoVSpt1 = (TProfile*)f1.Get("AverageIso1_vs_pt");
    TProfile* isoVSpt2 = (TProfile*)f1.Get("AverageIso2_vs_pt");
    TProfile* isoVSpt3 = (TProfile*)f1.Get("AverageIso3_vs_pt");
    TProfile* isoVSpt4 = (TProfile*)f1.Get("AverageIso4_vs_pt");

    TH1F* isoMerged1 = (TH1F*)f1.Get("Iso1_Merged");
    TH1F* isoMerged2 = (TH1F*)f1.Get("Iso2_Merged");
    TH1F* isoMerged3 = (TH1F*)f1.Get("Iso3_Merged");
    TH1F* isoMerged4 = (TH1F*)f1.Get("Iso4_Merged");

    TH1F* isoNotMerged1 = (TH1F*)f1.Get("Iso1_NotMerged");
    TH1F* isoNotMerged2 = (TH1F*)f1.Get("Iso2_NotMerged");
    TH1F* isoNotMerged3 = (TH1F*)f1.Get("Iso3_NotMerged");
    TH1F* isoNotMerged4 = (TH1F*)f1.Get("Iso4_NotMerged");

    TString CanvasName = "Comparison_isoVSpt";
    TString CanvasNamePdf = CanvasName ;
    CanvasNamePdf += ".pdf";

    TCanvas c(CanvasName.Data(),CanvasName.Data(),650,600);
    c.SetLeftMargin(0.12);
    c.SetGrid();

    isoVSpt1->SetTitle("");
    isoVSpt1->GetXaxis()->SetRangeUser(20.,100.);
    isoVSpt1->GetYaxis()->SetRangeUser(0.,14.);
    isoVSpt1->GetYaxis()->SetTitle("E_{T}^{ISO} [GeV]");
    isoVSpt1->GetXaxis()->SetTitle("E_{T}^{L1 #tau} [GeV]");
    isoVSpt1->SetLineColor(kRed);
    isoVSpt1->SetLineWidth(2);
    isoVSpt1->Draw();

    isoVSpt2->SetLineColor(kGreen);
    isoVSpt2->SetLineWidth(2);
    isoVSpt2->Draw("same");

    isoVSpt3->SetLineColor(kBlue);
    isoVSpt3->SetLineWidth(2);
    isoVSpt3->Draw("same");

    isoVSpt4->SetLineColor(kYellow);
    isoVSpt4->SetLineWidth(2);
    isoVSpt4->Draw("same");

    TPaveText* texl = new TPaveText(0.13,0.87,0.3,0.99,"NDC");
    texl->AddText("CMS Internal");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    texl->Draw("same");

    TLegend* leg = new TLegend(0.13,0.75,0.5,0.89);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);
    leg->AddEntry(isoVSpt1,"Run2 Data (Run 323775)","L");
    leg->AddEntry(isoVSpt2,"Run3 MC VBFHToTauTau","L");
    leg->AddEntry(isoVSpt3,"Run3 Data (Fill 8063) - ReEmu","L");
    leg->AddEntry(isoVSpt4,"Run3 Data (Fill 8063) - ReEmuTP","L");
    
    leg->Draw("same");

    TString PDFs = "./";
    c.SaveAs(PDFs+CanvasNamePdf.Data());



    CanvasName = "Comparison_isoMerged";
    CanvasNamePdf = CanvasName ;
    CanvasNamePdf += ".pdf";

    TCanvas c1(CanvasName.Data(),CanvasName.Data(),650,600);
    c1.SetLeftMargin(0.12);
    c1.SetGrid();

    isoMerged1->SetTitle("");
    // isoMerged1->GetYaxis()->SetLimits(0.,0.15);
    isoMerged1->GetXaxis()->SetRangeUser(0.,14.);
    isoMerged1->GetYaxis()->SetTitle("E_{T}^{ISO} [GeV]");
    isoMerged1->GetXaxis()->SetTitle("E_{T}^{L1 #tau} [GeV]");
    isoMerged1->SetLineColor(kRed);
    isoMerged1->SetLineWidth(2);
    isoMerged1->DrawNormalized();

    isoMerged2->SetLineColor(kGreen);
    isoMerged2->SetLineWidth(2);
    isoMerged2->DrawNormalized("same");

    isoMerged3->SetLineColor(kBlue);
    isoMerged3->SetLineWidth(2);
    isoMerged3->DrawNormalized("same"); 

    isoMerged4->SetLineColor(kYellow);
    isoMerged4->SetLineWidth(2);
    isoMerged4->DrawNormalized("same"); 

    TLegend* leg2 = new TLegend(0.5,0.75,0.89,0.89);
    leg2->SetBorderSize(0);
    leg2->SetTextSize(0.02);
    leg2->AddEntry(isoVSpt1,"Run2 Data (Run 323775)","L");
    leg2->AddEntry(isoVSpt2,"Run3 MC VBFHToTauTau","L");
    leg2->AddEntry(isoVSpt3,"Run3 Data (Fill 8063) - ReEmu","L");
    leg2->AddEntry(isoVSpt4,"Run3 Data (Fill 8063) - ReEmuTP","L");
    leg2->Draw("same");
    texl->Draw("same");

    c1.SaveAs(PDFs+CanvasNamePdf.Data());





    CanvasName = "Comparison_isoNotMerged";
    CanvasNamePdf = CanvasName ;
    CanvasNamePdf += ".pdf";

    TCanvas c2(CanvasName.Data(),CanvasName.Data(),650,600);
    c2.SetLeftMargin(0.12);
    c2.SetGrid();

    isoNotMerged1->SetTitle("");
    // isoNotMerged1->GetYaxis()->SetRangeUser(0.,isoNotMerged1->GetMaximum()/isoNotMerged1->Integral()*0.5);
    isoNotMerged1->GetXaxis()->SetRangeUser(0.,14.);
    isoNotMerged1->GetYaxis()->SetTitle("a.u.");
    isoNotMerged1->GetXaxis()->SetTitle("E_{T}^{ISO} [GeV]");
    isoNotMerged1->SetLineColor(kRed);
    isoNotMerged1->SetLineWidth(2);
    isoNotMerged1->DrawNormalized();

    isoNotMerged2->SetLineColor(kGreen);
    isoNotMerged2->SetLineWidth(2);
    isoNotMerged2->DrawNormalized("same");

    isoNotMerged3->SetLineColor(kBlue);
    isoNotMerged3->SetLineWidth(2);
    isoNotMerged3->DrawNormalized("same"); 

    isoNotMerged4->SetLineColor(kYellow);
    isoNotMerged4->SetLineWidth(2);
    isoNotMerged4->DrawNormalized("same"); 

    leg2->Draw("same");
    texl->Draw("same");

    c2.SaveAs(PDFs+CanvasNamePdf.Data());





    f1.Close();

}
