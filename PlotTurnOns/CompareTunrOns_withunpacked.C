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

void compare(int run, float calibThr = 1.7) {
    TString run_str = to_string(run);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    gStyle->SetOptStat(000000);

    TFile* f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_01_15_optimizationV3_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_Run"+run_str+"_2022_01_15.root","READ");

    TGraphAsymmErrors* rate_NewLayer1_noIso_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_noIso_by_pt");

    TGraphAsymmErrors* rate_NewLayer1_Option22_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option22_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_Option31_extrap_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option31_extrap_by_pt");

    TString fm_noIso_mean = std::to_string( int( ((TVectorT<float>*)f->Get("fm_noIso"))->Max() ) );
    TString fm_Option22_mean = std::to_string( int( ((TVectorT<float>*)f->Get("fm_Option22"))->Max() ) );
    TString fm_Option31_extrap_mean = std::to_string( int( ((TVectorT<float>*)f->Get("fm_Option31_extrap"))->Max() ) );

    // TString thr_noIso_mean = std::to_string( int( ((TVectorT<float>*)f->Get("thr_noIso"))->Max() ) );
    // TString thr_Option22_mean = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option22"))->Max() ) );
    // TString thr_Option31_extrap_mean = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option31_extrap"))->Max() ) );

    TString CanvasName = "Comparison_TurnOn_Run"+run_str+"_newnTT_unpacked_optimizationV3_calibThr"+intgr+"p"+decim;
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

    rate_NewLayer1_noIso_mean->SetLineColor(kBlack);
    rate_NewLayer1_noIso_mean->SetLineWidth(2);
    rate_NewLayer1_noIso_mean->GetXaxis()->SetLimits(15,120.);
    rate_NewLayer1_noIso_mean->Draw();

    rate_NewLayer1_Option22_mean->SetLineColor(kGray+1);
    rate_NewLayer1_Option22_mean->SetLineWidth(2);
    rate_NewLayer1_Option22_mean->Draw("same");

    rate_NewLayer1_Option31_extrap_mean->SetLineColor(kGray+2);
    rate_NewLayer1_Option31_extrap_mean->SetLineWidth(2);
    rate_NewLayer1_Option31_extrap_mean->Draw("same");

    TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
    texl->AddText("CMS Internal, #sqrt{s}=13 TeV, Run #"+run_str+" (2018)");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    texl->Draw("same");

    TLegend* leg = new TLegend(0.65,0.1,0.81,0.62);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);
    leg->SetHeader("Linearly scaled to 2.0E34");

    leg->AddEntry(rate_NewLayer1_noIso_mean,"Di-#tau no-iso, mean - FM="+fm_noIso_mean,"L");
    leg->AddEntry(rate_NewLayer1_Option22_mean,"Di-#tau iso (Option 22) - FM="+fm_Option22_mean,"L");
    leg->AddEntry(rate_NewLayer1_Option31_extrap_mean,"Di-#tau iso (Option 31) - FM="+fm_Option31_extrap_mean,"L");

    leg->Draw("same");

    TString PDFs = "PDFs/";
    c.SaveAs(PDFs+CanvasNamePdf.Data());

    f->Close();

}
