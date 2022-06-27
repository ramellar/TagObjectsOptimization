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

void compare(int run, int targetRate = 14, float calibThr = 1.7) {
    TString run_str = to_string(run);

    TString fixedRate = to_string(targetRate);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    gStyle->SetOptStat(000000);

    TFile* f;
    if (run==-1) f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_03_15_optimizationV10_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE"+fixedRate+"kHz_SingleNeutrino112XpuReweighted_2022_03_15.root", "READ");
    else         f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_03_15_optimizationV10_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE"+fixedRate+"kHz_Run"+run_str+"_2022_03_15.root", "READ");
  
    TGraphAsymmErrors* turnOn_NewLayer1_Run2unpacked_Iso = (TGraphAsymmErrors*)f->Get("divide_Run2unpacked_Iso_by_Run2unpacked");
    TGraphAsymmErrors* turnOn_NewLayer1_noIso_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_noIso_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option22_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option22_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option24_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option24_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option26_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option26_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option28_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option28_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option31_extrap_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option31_extrap_by_pt");

    TString thr_Run2unpackedIso = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Run2unpackedIso"))->Max() )+1 ); //+1 cause the saved values are XX.51
    TString thr_noIso_mean = std::to_string( int( ((TVectorT<float>*)f->Get("thr_noIso"))->Max() )+1 ); //+1 cause the saved values are XX.51
    TString thr_Option22_mean = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option22"))->Max() )+1 ); //+1 cause the saved values are XX.51
    TString thr_Option24_mean = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option24"))->Max() )+1 ); //+1 cause the saved values are XX.51
    TString thr_Option26_mean = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option26"))->Max() )+1 ); //+1 cause the saved values are XX.51
    TString thr_Option28_mean = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option28"))->Max() )+1 ); //+1 cause the saved values are XX.51
    TString thr_Option31_extrap_mean = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option31_extrap"))->Max() )+1 ); //+1 cause the saved values are XX.51

    // TString thr_noIso_mean = std::to_string( int( ((TVectorT<float>*)f->Get("thr_noIso"))->Max() ) );
    // TString thr_Option22_mean = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option22"))->Max() ) );
    // TString thr_Option31_extrap_mean = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option31_extrap"))->Max() ) );

    TString CanvasName = ""; 
    if (run==-1) CanvasName = "Comparison_TurnOn_fixedRate"+fixedRate+"kHz_SingleNeutrino112XpuReweighted_newnTT_Run2unpacked_optimizationV10_calibThr"+intgr+"p"+decim;
    else         CanvasName = "Comparison_TurnOn_fixedRate"+fixedRate+"kHz_Run"+run_str+"_newnTT_Run2unpacked_optimizationV10_calibThr"+intgr+"p"+decim;
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

    turnOn_NewLayer1_noIso_mean->SetTitle("");
    turnOn_NewLayer1_noIso_mean->SetLineColor(kBlack);
    turnOn_NewLayer1_noIso_mean->SetLineWidth(2);
    turnOn_NewLayer1_noIso_mean->GetXaxis()->SetLimits(15,120.);
    turnOn_NewLayer1_noIso_mean->Draw();

    turnOn_NewLayer1_Run2unpacked_Iso->SetLineColor(kRed);
    turnOn_NewLayer1_Run2unpacked_Iso->SetLineWidth(2);
    turnOn_NewLayer1_Run2unpacked_Iso->Draw("same");

    turnOn_NewLayer1_Option22_mean->SetLineColor(kBlue);
    turnOn_NewLayer1_Option22_mean->SetLineWidth(2);
    turnOn_NewLayer1_Option22_mean->Draw("same");

    turnOn_NewLayer1_Option24_mean->SetLineColor(kMagenta);
    turnOn_NewLayer1_Option24_mean->SetLineWidth(2);
    turnOn_NewLayer1_Option24_mean->Draw("same");

    turnOn_NewLayer1_Option26_mean->SetLineColor(kGreen);
    turnOn_NewLayer1_Option26_mean->SetLineWidth(2);
    turnOn_NewLayer1_Option26_mean->Draw("same");

    turnOn_NewLayer1_Option28_mean->SetLineColor(kYellow);
    turnOn_NewLayer1_Option28_mean->SetLineWidth(2);
    turnOn_NewLayer1_Option28_mean->Draw("same");

    turnOn_NewLayer1_Option31_extrap_mean->SetLineColor(kOrange);
    turnOn_NewLayer1_Option31_extrap_mean->SetLineWidth(2);
    turnOn_NewLayer1_Option31_extrap_mean->Draw("same");

    TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
    if (run ==-1) texl->AddText("CMS Internal, #sqrt{s}=13 TeV, Run3 MC (2018)");
    else          texl->AddText("CMS Internal, #sqrt{s}=13 TeV, Run #"+run_str+" (2018)");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    texl->Draw("same");

    TLegend* leg = new TLegend(0.65,0.1,0.81,0.62);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);
    leg->SetHeader("Linearly scaled to 2.0E34");

    leg->AddEntry(turnOn_NewLayer1_noIso_mean,"Di-#tau no-iso, mean - thr="+thr_noIso_mean,"L");
    leg->AddEntry(turnOn_NewLayer1_Run2unpacked_Iso,"Di-#tau Run2unpacked iso, thr="+thr_Run2unpackedIso,"L");
    leg->AddEntry(turnOn_NewLayer1_Option22_mean,"Di-#tau iso (Option 22) - thr="+thr_Option22_mean,"L");
    leg->AddEntry(turnOn_NewLayer1_Option24_mean,"Di-#tau iso (Option 24) - thr="+thr_Option24_mean,"L");
    leg->AddEntry(turnOn_NewLayer1_Option26_mean,"Di-#tau iso (Option 26) - thr="+thr_Option26_mean,"L");
    leg->AddEntry(turnOn_NewLayer1_Option28_mean,"Di-#tau iso (Option 28) - thr="+thr_Option28_mean,"L");
    leg->AddEntry(turnOn_NewLayer1_Option31_extrap_mean,"Di-#tau iso (Option 31) - thr="+thr_Option31_extrap_mean,"L");

    leg->Draw("same");

    TString PDFs = "PDFs/";
    c.SaveAs(PDFs+CanvasNamePdf.Data());

    f->Close();

}
