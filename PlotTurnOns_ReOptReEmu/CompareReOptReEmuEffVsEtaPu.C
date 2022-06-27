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

void compare(TString versus = "eta", int targetRate = 14) {

    TString fixedRate = to_string(targetRate);

    gStyle->SetOptStat(000000);


    TCanvas c("c","c",650,600);
    c.SetLeftMargin(0.15);
    c.SetGrid();
    c.SetLogy();

    TLegend* leg = new TLegend(0.50,0.15,0.89,0.45);
    leg->SetBorderSize(0);
    //leg->SetTextSize(0.015);
    leg->SetHeader("p_{T}^{offline}>50GeV ; E_{T}^{L1}>35GeV");

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0., 1, 1.0);
    //pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
    pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    //pad1->SetLogy();
    //pad1->SetLogx();

    TString fileName = "/data_CMS/cms/motta/Run3preparation/2022_04_21_optimizationV11_ReOptReEmu/Run3_MC_VBFHToTauTau_M125_TURNONS_ReOptReEmu"+fixedRate+"kHz_2022_04_21.root";

    TFile* f;
    f = new TFile(fileName.Data(), "READ");

    TGraphAsymmErrors* turnon = (TGraphAsymmErrors*)f->Get("divide_Run3unpacked_Iso_"+versus+"_by_Run3unpacked_"+versus);
    TString THR = std::to_string( int( ( (TVectorT<float>*)f->Get("thr_Run3unpackedIso") )[0][0] ) +1 );
    turnon->SetLineColor(kBlue);
    turnon->SetLineWidth(1);
    turnon->SetTitle("");
    turnon->GetYaxis()->SetTitle("Efficiency");
    if(versus=="eta") turnon->GetXaxis()->SetTitle("Offline #eta(#tau)");
    else if(versus=="nVtx") turnon->GetXaxis()->SetTitle("nVtx");
    turnon->GetYaxis()->SetRangeUser(0.0,1.01);
    turnon->Draw();
    leg->AddEntry(turnon, "Run3 MC re-emulated Iso", "L");

    TGraphAsymmErrors* turnon_noIso = (TGraphAsymmErrors*)f->Get("divide_Run3unpacked_noIso_"+versus+"_by_Run3unpacked_"+versus);
    TString THR_noIso = std::to_string( int( ( (TVectorT<float>*)f->Get("thr_Run3unpacked_noIso") )[0][0] ) +1 );
    turnon_noIso->SetLineColor(kRed);
    turnon_noIso->SetLineWidth(1);
    turnon_noIso->Draw("same");
    leg->AddEntry(turnon_noIso, "Run3 MC re-emulated noIso", "L");

    f->Close();


    leg->Draw("same");

    TPaveText* texl = new TPaveText(0.15,0.87,0.35,0.99,"NDC");
    texl->AddText("CMS Internal");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    pad1->cd();
    texl->Draw("same");   

    TString _tag = "";
    c.SaveAs("PDFs/optimizationV11_ReOptReEmu/efficencyVS"+versus+".pdf");

}





































