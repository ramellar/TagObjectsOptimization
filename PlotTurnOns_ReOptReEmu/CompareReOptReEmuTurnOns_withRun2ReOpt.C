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

void compare(bool zoom=false) {

    gStyle->SetOptStat(000000);

    //#############################################
    // SingleNeutrino 112X PU reweighted
    std::vector< std::pair<TString, TString> > points = {};
    // points = {std::pair("14","effMin0p9 G3"), std::pair("16","effMin0p9 Q0"), std::pair("18","effMin0p9 D1"), std::pair("20","effMin0p9 B1")};
    points = {std::pair("14","effMin0p9 G3")};
    // points = {std::pair("16","effMin0p9 Q0")};

    TCanvas c("c","c",650,600);
    c.SetLeftMargin(0.15);
    c.SetGrid();
    c.SetLogy();

    TLegend* leg = new TLegend(0.50,0.15,0.89,0.45);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.015);
    leg->SetHeader("Linearly scaled to 2.0E34");

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0., 1, 1.0);
    //pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
    pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    //pad1->SetLogy();
    //pad1->SetLogx();

    TFile* f_unpacked = new TFile("/data_CMS/cms/motta/Run3preparation/2022_04_21_optimizationV11_ReOptReEmu/Run3_MC_VBFHToTauTau_M125_TURNONS_ReOptReEmu14kHz_2022_04_21.root", "READ");
    TGraphAsymmErrors* turnOn_NewLayer1_Run2unpacked_Iso = (TGraphAsymmErrors*)f_unpacked->Get("divide_Run2unpacked_Iso_by_Run2unpacked");
    TString thr_Run2unpackedIso = std::to_string( int( ((TVectorT<float>*)f_unpacked->Get("thr_Run2unpackedIso"))[0][0] ) +1 );

    turnOn_NewLayer1_Run2unpacked_Iso->SetTitle("");
    turnOn_NewLayer1_Run2unpacked_Iso->SetLineColor(kBlack);
    turnOn_NewLayer1_Run2unpacked_Iso->SetLineWidth(1);
    turnOn_NewLayer1_Run2unpacked_Iso->GetXaxis()->SetLimits(15,120.);
    turnOn_NewLayer1_Run2unpacked_Iso->GetYaxis()->SetRangeUser(0.0,1.01);
    if (zoom) 
    {
        turnOn_NewLayer1_Run2unpacked_Iso->GetXaxis()->SetLimits(40,120.);
        turnOn_NewLayer1_Run2unpacked_Iso->GetYaxis()->SetRangeUser(0.8,1.01);
    }
    turnOn_NewLayer1_Run2unpacked_Iso->GetYaxis()->SetTitle("Efficiency");
    turnOn_NewLayer1_Run2unpacked_Iso->GetXaxis()->SetTitle("Offline p_{T}(#tau) [GeV]");
    turnOn_NewLayer1_Run2unpacked_Iso->Draw();
    leg->AddEntry(turnOn_NewLayer1_Run2unpacked_Iso,"Run2unpacked iso, THR="+thr_Run2unpackedIso,"L");

    f_unpacked->Close();

    std::vector<int> colors = {kBlue, kRed, kCyan, kOrange, kBlue-9, kRed-9, kCyan-9, kOrange-9};

    for (long unsigned int i = 0; i < points.size(); ++i)
    {
        TString fixedRate = points[i].first;
        TString tag = points[i].second;

        TString fileName = "/data_CMS/cms/motta/Run3preparation/2022_04_21_optimizationV11_ReOptReEmu/Run3_MC_VBFHToTauTau_M125_TURNONS_ReOptReEmu"+fixedRate+"kHz_2022_04_21.root";

        TFile* f;
        f = new TFile(fileName.Data(), "READ");

        TGraphAsymmErrors* turnon = (TGraphAsymmErrors*)f->Get("divide_Run3unpacked_Iso_by_Run3unpacked");
        TString THR = std::to_string( int( ( (TVectorT<float>*)f->Get("thr_Run3unpackedIso") )[0][0] ) +1 );
        turnon->SetLineColor(colors[i]);
        turnon->SetLineWidth(1);
        turnon->Draw("same");
        leg->AddEntry(turnon, "Iso ("+tag+") - THR="+THR+" - RATE="+fixedRate+"KHz", "L");

        TGraphAsymmErrors* turnon_noIso = (TGraphAsymmErrors*)f->Get("divide_Run3unpacked_noIso_by_Run3unpacked");
        TString THR_noIso = std::to_string( int( ( (TVectorT<float>*)f->Get("thr_Run3unpacked_noIso") )[0][0] ) +1 );
        turnon_noIso->SetLineColor(colors[i+4]);
        turnon_noIso->SetLineWidth(1);
        turnon_noIso->Draw("same");
        leg->AddEntry(turnon_noIso, "No-iso - THR="+THR_noIso+" - RATE="+fixedRate+"KHz", "L");

        f->Close();


        TFile* f_Run2OptReEmu = new TFile("/data_CMS/cms/motta/Run3preparation/2022_04_21_optimizationV11_calibThr1p7/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE"+fixedRate+"kHz_SingleNeutrino112XpuReweighted_gs_effMin0p0_0_2022_04_21.root", "READ");
        TGraphAsymmErrors* turnOn_NewLayer1_Run2OptReEmu_Iso = (TGraphAsymmErrors*)f_Run2OptReEmu->Get("divide_pt_pass_Option31_extrap_by_pt");
        TString thr_Run2OptReEmu = std::to_string( int( ((TVectorT<float>*)f_Run2OptReEmu->Get("thr_Option31_extrap"))[0][0] ) +1 );

        turnOn_NewLayer1_Run2OptReEmu_Iso->SetTitle("");
        turnOn_NewLayer1_Run2OptReEmu_Iso->SetLineColor(kGray);
        turnOn_NewLayer1_Run2OptReEmu_Iso->SetLineWidth(1);
        turnOn_NewLayer1_Run2OptReEmu_Iso->Draw("same");
        leg->AddEntry(turnOn_NewLayer1_Run2OptReEmu_Iso,"Run2 re-emulated iso, THR="+thr_Run2OptReEmu+" - RATE="+fixedRate+"KHz","L");

        f_Run2OptReEmu->Close();

    }

    leg->Draw("same");

    TPaveText* texl = new TPaveText(0.15,0.87,0.35,0.99,"NDC");
    texl->AddText("CMS Internal");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    pad1->cd();
    texl->Draw("same");   

    TString _tag = "";
    if (zoom) _tag = "_zoom";
    c.SaveAs("PDFs/optimizationV11_ReOptReEmu/proposals_comparison_newlayout.pdf");

}





































