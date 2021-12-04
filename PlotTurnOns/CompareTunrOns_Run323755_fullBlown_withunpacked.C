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
#include <fstream>
#include <map>

using namespace std;

void compare(TString tag, TString opt="0") {    
    gStyle->SetOptStat(000000);

    TFile* f = new TFile("/data_CMS/cms/motta/Run3preparation/2021_11_22_optimizationV1/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_"+tag+"_"+opt+"_2021_11_22.root","READ");

    TGraphAsymmErrors* rate_NewLayer1_noIso_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_noIso_by_pt");

    TGraphAsymmErrors* rate_NewLayer1_OptionA_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionA"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionB_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionB"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionC_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionC"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionD_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionD"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionE_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionE"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionF_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionF"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionG_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionG"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionH_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionH"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionI_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionI"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionJ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionJ"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionK_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionK"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionL_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionL"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionM_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionM"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionN_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionN"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionO_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionO"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionP_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionP"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionQ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionQ"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionR_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionR"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionS_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionS"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionT_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionT"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionU_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionU"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionV_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionV"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionW_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionW"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionX_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionX"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionY_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionY"+opt+"_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_OptionZ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionZ"+opt+"_by_pt");

    TGraphAsymmErrors* rate_NewLayer1_Option22_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option22_by_pt");
    TGraphAsymmErrors* rate_NewLayer1_Option31_extrap_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option31_extrap_by_pt");

    TString CanvasName = "Comparison_TurnOn_"+tag+"_"+opt+"_Run323755_newnTT_unpacked";
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

    rate_NewLayer1_OptionA_mean->SetLineColor(kRed);
    rate_NewLayer1_OptionA_mean->SetLineWidth(2);
    rate_NewLayer1_OptionA_mean->Draw("same");

    rate_NewLayer1_OptionB_mean->SetLineColor(kRed-4);
    rate_NewLayer1_OptionB_mean->SetLineWidth(2);
    rate_NewLayer1_OptionB_mean->Draw("same");

    rate_NewLayer1_OptionC_mean->SetLineColor(kRed-7);
    rate_NewLayer1_OptionC_mean->SetLineWidth(2);
    rate_NewLayer1_OptionC_mean->Draw("same");

    rate_NewLayer1_OptionD_mean->SetLineColor(kRed-9);
    rate_NewLayer1_OptionD_mean->SetLineWidth(2);
    rate_NewLayer1_OptionD_mean->Draw("same");

    rate_NewLayer1_OptionE_mean->SetLineColor(kRed-10);
    rate_NewLayer1_OptionE_mean->SetLineWidth(2);
    rate_NewLayer1_OptionE_mean->Draw("same");

    rate_NewLayer1_OptionF_mean->SetLineColor(kMagenta);
    rate_NewLayer1_OptionF_mean->SetLineWidth(2);
    rate_NewLayer1_OptionF_mean->Draw("same");

    rate_NewLayer1_OptionG_mean->SetLineColor(kMagenta-4);
    rate_NewLayer1_OptionG_mean->SetLineWidth(2);
    rate_NewLayer1_OptionG_mean->Draw("same");

    rate_NewLayer1_OptionH_mean->SetLineColor(kMagenta-7);
    rate_NewLayer1_OptionH_mean->SetLineWidth(2);
    rate_NewLayer1_OptionH_mean->Draw("same");

    rate_NewLayer1_OptionI_mean->SetLineColor(kMagenta-9);
    rate_NewLayer1_OptionI_mean->SetLineWidth(2);
    rate_NewLayer1_OptionI_mean->Draw("same");

    rate_NewLayer1_OptionJ_mean->SetLineColor(kMagenta-10);
    rate_NewLayer1_OptionJ_mean->SetLineWidth(2);
    rate_NewLayer1_OptionJ_mean->Draw("same");

    rate_NewLayer1_OptionK_mean->SetLineColor(kBlue);
    rate_NewLayer1_OptionK_mean->SetLineWidth(2);
    rate_NewLayer1_OptionK_mean->Draw("same");

    rate_NewLayer1_OptionL_mean->SetLineColor(kBlue);
    rate_NewLayer1_OptionL_mean->SetLineWidth(2);
    rate_NewLayer1_OptionL_mean->Draw("same");

    rate_NewLayer1_OptionM_mean->SetLineColor(kBlue-7);
    rate_NewLayer1_OptionM_mean->SetLineWidth(2);
    rate_NewLayer1_OptionM_mean->Draw("same");

    rate_NewLayer1_OptionN_mean->SetLineColor(kBlue-9);
    rate_NewLayer1_OptionN_mean->SetLineWidth(2);
    rate_NewLayer1_OptionN_mean->Draw("same");

    rate_NewLayer1_OptionO_mean->SetLineColor(kBlue-10);
    rate_NewLayer1_OptionO_mean->SetLineWidth(2);
    rate_NewLayer1_OptionO_mean->Draw("same");

    rate_NewLayer1_OptionP_mean->SetLineColor(kCyan);
    rate_NewLayer1_OptionP_mean->SetLineWidth(2);
    rate_NewLayer1_OptionP_mean->Draw("same");

    rate_NewLayer1_OptionQ_mean->SetLineColor(kCyan-4);
    rate_NewLayer1_OptionQ_mean->SetLineWidth(2);
    rate_NewLayer1_OptionQ_mean->Draw("same");

    rate_NewLayer1_OptionR_mean->SetLineColor(kCyan-7);
    rate_NewLayer1_OptionR_mean->SetLineWidth(2);
    rate_NewLayer1_OptionR_mean->Draw("same");

    rate_NewLayer1_OptionS_mean->SetLineColor(kCyan-9);
    rate_NewLayer1_OptionS_mean->SetLineWidth(2);
    rate_NewLayer1_OptionS_mean->Draw("same");

    rate_NewLayer1_OptionT_mean->SetLineColor(kCyan-10);
    rate_NewLayer1_OptionT_mean->SetLineWidth(2);
    rate_NewLayer1_OptionT_mean->Draw("same");

    rate_NewLayer1_OptionU_mean->SetLineColor(kGreen);
    rate_NewLayer1_OptionU_mean->SetLineWidth(2);
    rate_NewLayer1_OptionU_mean->Draw("same");

    rate_NewLayer1_OptionV_mean->SetLineColor(kGreen-4);
    rate_NewLayer1_OptionV_mean->SetLineWidth(2);
    rate_NewLayer1_OptionV_mean->Draw("same");

    rate_NewLayer1_OptionW_mean->SetLineColor(kGreen-7);
    rate_NewLayer1_OptionW_mean->SetLineWidth(2);
    rate_NewLayer1_OptionW_mean->Draw("same");

    rate_NewLayer1_OptionX_mean->SetLineColor(kGreen-9);
    rate_NewLayer1_OptionX_mean->SetLineWidth(2);
    rate_NewLayer1_OptionX_mean->Draw("same");

    rate_NewLayer1_OptionY_mean->SetLineColor(kGreen-10);
    rate_NewLayer1_OptionY_mean->SetLineWidth(2);
    rate_NewLayer1_OptionY_mean->Draw("same");

    rate_NewLayer1_OptionZ_mean->SetLineColor(kYellow);
    rate_NewLayer1_OptionZ_mean->SetLineWidth(2);
    rate_NewLayer1_OptionZ_mean->Draw("same");

    rate_NewLayer1_Option22_mean->SetLineColor(kGray+1);
    rate_NewLayer1_Option22_mean->SetLineWidth(2);
    rate_NewLayer1_Option22_mean->Draw("same");

    rate_NewLayer1_Option31_extrap_mean->SetLineColor(kGray+2);
    rate_NewLayer1_Option31_extrap_mean->SetLineWidth(2);
    rate_NewLayer1_Option31_extrap_mean->Draw("same");

    TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
    texl->AddText("CMS Internal, #sqrt{s}=13 TeV, Run #323755 (2018)");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    texl->Draw("same");

    // TLegend* leg = new TLegend(0.29,0.64,0.81,0.87);
    // leg->SetBorderSize(0);
    // leg->SetTextSize(0.035);
    // leg->SetHeader("Linearly scaled to 2.0E34");

    // leg->AddEntry(rate_NewLayer1_noIso_mean,"Di-#tau no-iso, mean","L");

    // leg->AddEntry(rate_NewLayer1_OptionA_mean,"Di-#tau iso (Option A)","L");
    // leg->AddEntry(rate_NewLayer1_OptionB_mean,"Di-#tau iso (Option B)","L");
    // leg->AddEntry(rate_NewLayer1_OptionC_mean,"Di-#tau iso (Option C)","L");
    // leg->AddEntry(rate_NewLayer1_OptionD_mean,"Di-#tau iso (Option D)","L");
    // leg->AddEntry(rate_NewLayer1_OptionE_mean,"Di-#tau iso (Option E)","L");
    // leg->AddEntry(rate_NewLayer1_OptionF_mean,"Di-#tau iso (Option F)","L");
    // leg->AddEntry(rate_NewLayer1_OptionG_mean,"Di-#tau iso (Option G)","L");
    // leg->AddEntry(rate_NewLayer1_OptionH_mean,"Di-#tau iso (Option H)","L");
    // leg->AddEntry(rate_NewLayer1_OptionI_mean,"Di-#tau iso (Option I)","L");
    // leg->AddEntry(rate_NewLayer1_OptionJ_mean,"Di-#tau iso (Option J)","L");
    // leg->AddEntry(rate_NewLayer1_OptionK_mean,"Di-#tau iso (Option K)","L");
    // leg->AddEntry(rate_NewLayer1_OptionL_mean,"Di-#tau iso (Option L)","L");
    // leg->AddEntry(rate_NewLayer1_OptionM_mean,"Di-#tau iso (Option M)","L");
    // leg->AddEntry(rate_NewLayer1_OptionN_mean,"Di-#tau iso (Option N)","L");
    // leg->AddEntry(rate_NewLayer1_OptionO_mean,"Di-#tau iso (Option O)","L");
    // leg->AddEntry(rate_NewLayer1_OptionP_mean,"Di-#tau iso (Option P)","L");
    // leg->AddEntry(rate_NewLayer1_OptionQ_mean,"Di-#tau iso (Option Q)","L");
    // leg->AddEntry(rate_NewLayer1_OptionR_mean,"Di-#tau iso (Option R)","L");
    // leg->AddEntry(rate_NewLayer1_OptionS_mean,"Di-#tau iso (Option S)","L");
    // leg->AddEntry(rate_NewLayer1_OptionT_mean,"Di-#tau iso (Option T)","L");
    // leg->AddEntry(rate_NewLayer1_OptionU_mean,"Di-#tau iso (Option U)","L");
    // leg->AddEntry(rate_NewLayer1_OptionV_mean,"Di-#tau iso (Option V)","L");
    // leg->AddEntry(rate_NewLayer1_OptionW_mean,"Di-#tau iso (Option W)","L");
    // leg->AddEntry(rate_NewLayer1_OptionX_mean,"Di-#tau iso (Option X)","L");
    // leg->AddEntry(rate_NewLayer1_OptionY_mean,"Di-#tau iso (Option Y)","L");
    // leg->AddEntry(rate_NewLayer1_OptionZ_mean,"Di-#tau iso (Option Z)","L");

    // leg->AddEntry(rate_NewLayer1_Option22_mean,"Di-#tau iso (Option 22)","L");
    // leg->AddEntry(rate_NewLayer1_Option31_extrap_mean,"Di-#tau iso (Option 31)","L");

    // leg->Draw("same");

    TString PDFs = "PDFs/";
    c.SaveAs(PDFs+CanvasNamePdf.Data());

    f->Close();

}
