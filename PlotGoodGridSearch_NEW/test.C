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

void compare(int targetRate = 14, int fixedThr = 0, TString parametrisation = "linear", Double_t Kfact = 0.0, bool zoom=false, int run=-1, float calibThr = 1.7) {
    TString run_str = to_string(run);

    TString fixedRate = to_string(targetRate);
    TString fixedThreshold = to_string(fixedThr);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    TString Kintgr = to_string(Kfact).substr(0, to_string(Kfact).find("."));
    TString Kdecim = to_string(Kfact).substr(2, to_string(Kfact).find("."));

    gStyle->SetOptStat(000000);

    //#############################################
    std::vector< std::pair<TString, TString> > points = {};
    
    if(Kfact==1.0) points = {std::pair("effMin0p6","O3"), std::pair("effMin0p3","H0"), std::pair("effMin0p4","P1"), std::pair("effMin0p9","Z2"), std::pair("effMin0p9","S1")};
    if(Kfact==2.0) points = {std::pair("effMin0p9","Q0"), std::pair("effMin0p9","R0"), std::pair("effMin0p9","T3"), std::pair("effMin0p9","E3"), std::pair("effMin0p9","I1")};

    TCanvas c("c","c",650,600);
    c.SetLeftMargin(0.15);
    c.SetGrid();
    c.SetLogy();

    TLegend* leg = new TLegend(0.45,0.15,0.89,0.45);
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

    TString fileName = "/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS";
    if (fixedThr==0) fileName += "_FIXEDRATE"+fixedRate+"kHz";
    else             fileName += "_FIXEDTHR"+fixedThreshold+"GeV";
    if (run==-1)     fileName += "_SingleNeutrino122X";
    else             fileName += "_Run"+run_str;
    if(parametrisation=="linear") fileName += "_gs_linear_effMin0p0_0_2022_06_13.root";
    else                          fileName += "_gs_"+parametrisation+Kintgr+"p"+Kdecim+"_effMin0p0_0_2022_06_13.root";

    TFile* f;
    f = new TFile(fileName.Data(), "READ");

    TGraphAsymmErrors* turnon_NewLayer1_noIso_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_noIso_by_pt");
    TString thr_noIso = std::to_string( int( ( (TVectorT<float>*)f->Get("thr_noIso") )[0][0]) +1 );

    TString fm = "FM";
    turnon_NewLayer1_noIso_mean->SetTitle("");
    turnon_NewLayer1_noIso_mean->SetLineColor(kBlack);
    turnon_NewLayer1_noIso_mean->SetLineWidth(1);
    turnon_NewLayer1_noIso_mean->GetXaxis()->SetLimits(15,120.);
    turnon_NewLayer1_noIso_mean->GetYaxis()->SetRangeUser(0.0,1.01);
    if (zoom) 
    {
        turnon_NewLayer1_noIso_mean->GetXaxis()->SetLimits(40,120.);
        turnon_NewLayer1_noIso_mean->GetYaxis()->SetRangeUser(0.8,1.01);
    }
    turnon_NewLayer1_noIso_mean->GetYaxis()->SetTitle("Efficiency");
    turnon_NewLayer1_noIso_mean->GetXaxis()->SetTitle("Offline p_{T}(#tau) [GeV]");
    turnon_NewLayer1_noIso_mean->Draw();
    leg->AddEntry(turnon_NewLayer1_noIso_mean,"No-iso, mean - THR="+thr_noIso,"L");

    TGraphAsymmErrors* turnOn_NewLayer1_Run2unpacked_Iso = (TGraphAsymmErrors*)f->Get("divide_Run2unpacked_Iso_by_Run2unpacked");
    TString thr_Run2unpackedIso = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Run2unpackedIso"))[0][0] ) +1 );

    turnOn_NewLayer1_Run2unpacked_Iso->SetLineColor(kGray);
    turnOn_NewLayer1_Run2unpacked_Iso->SetLineWidth(2);
    turnOn_NewLayer1_Run2unpacked_Iso->Draw("same");
    leg->AddEntry(turnOn_NewLayer1_Run2unpacked_Iso,"Run2unpacked iso, THR="+thr_Run2unpackedIso,"L");

    f->Close();

    std::vector<int> colors = {kBlue, kRed, kCyan, kOrange, kBlue-9, kRed-9, kCyan-9, kOrange-9};

    for (long unsigned int i = 0; i < points.size(); ++i)
    {
        pad1->cd();

        TString tag = points[i].first;
        TString point = points[i].second;
        TString opt = std::string(points[i].second).front();
        TString num = std::string(points[i].second).back();

        if (tag=="dummy") { continue; } // this makes the same threshold turnons of the same color throughout the plots

        TString fileName = "/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS";
        if (fixedThr==0) fileName += "_FIXEDRATE"+fixedRate+"kHz";
        else             fileName += "_FIXEDTHR"+fixedThreshold+"GeV";
        if (run==-1)     fileName += "_SingleNeutrino122X";
        else             fileName += "_Run"+run_str;
        if(parametrisation=="linear") fileName += "_gs_linear_"+tag+"_"+num+"_2022_06_13";
        else                          fileName += "_gs_"+parametrisation+Kintgr+"p"+Kdecim+"_"+tag+"_"+num+"_2022_06_13";
        fileName += ".root";

        TFile* f;
        f = new TFile(fileName.Data(), "READ");

        TGraphAsymmErrors* turnon = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option"+point+"_by_pt");
        TString THR = std::to_string( int( ( (TVectorT<float>*)f->Get("thr_Option"+opt) )[0][0] ) +1 );
        float ACC =  ( (TVectorT<float>*)f->Get("acc_Option"+opt) )[0][0] *100 ;
        turnon->SetLineColor(colors[i]);
        turnon->SetLineWidth(1);
        turnon->Draw("same");
        leg->AddEntry(turnon, "Iso ("+tag+" Option "+point+") - THR="+THR+Form(" - #epsilon_{TOT}=%.2f",ACC), "L");

        f->Close();

        if (targetRate!=0) {
            TFile* f_rate;
            if(parametrisation=="linear") f_rate = new TFile("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_SingleNeutrino122X_Run3_MC_optimizationV13gs_calibThr"+intgr+"p"+decim+"_linear_"+tag+".root","READ");
            else                          f_rate = new TFile("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_SingleNeutrino122X_Run3_MC_optimizationV13gs_calibThr"+intgr+"p"+decim+"_"+parametrisation+Kintgr+"p"+Kdecim+"_"+tag+".root","READ");
            TH1F* rate_noIso = (TH1F*)f_rate->Get("rate_noCut_DiTau");
            TH1F* rate_Iso = (TH1F*)f_rate->Get("rate_DiTau_Progression_"+opt+num);
            
            TCanvas cRate("cRate","cRate",650,600);
            cRate.SetLeftMargin(0.15);
            cRate.SetGrid();
            cRate.SetLogy();
            cRate.cd();
            TPad *pad1 = new TPad("pad1", "pad1", 0, 0., 1, 1.);
            //pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
            //pad1->SetLeftMargin(0.15);
            pad1->SetGridx();         // Vertical grid
            pad1->SetGridy();         // Vertical grid
            pad1->Draw();             // Draw the upper pad: pad1
            pad1->cd();               // pad1 becomes the current pad
            pad1->SetLogy();
            rate_noIso->SetTitle("");
            rate_noIso->GetXaxis()->SetRangeUser(20.,60.);
            rate_noIso->GetYaxis()->SetTitle("Di-#tau rate [kHz]");
            rate_noIso->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) threshold [GeV]");
            rate_noIso->SetLineColor(kBlack);
            rate_noIso->SetLineWidth(3);
            rate_noIso->Draw();
            rate_Iso->SetLineColor(kRed);
            rate_Iso->SetLineWidth(3);
            rate_Iso->Draw("same");

            TPaveText* texl1 = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
            texl1->AddText("CMS Internal, Run3 MC");
            texl1->SetTextSize(0.04);
            texl1->SetFillStyle(0);
            texl1->SetBorderSize(0);
            texl1->Draw("same");
            TLegend* leg1 = new TLegend(0.6,0.7,0.89,0.89);
            leg1->SetBorderSize(0);
            leg1->SetTextSize(0.02);
            leg1->SetHeader("Linearly scaled to 2.0E34");
            leg1->AddEntry(rate_noIso,"Di-#tau no-iso","L");
            leg1->AddEntry(rate_Iso,"Di-#tau iso","L");
            texl1->Draw("same");
            leg1->Draw("same");

            if(parametrisation=="linear") cRate.SaveAs("PDFs/optimizationV13/proposal/rate_"+tag+"_"+point+"_fixedRate"+fixedRate+"kHz_linear"+to_string(int(Kfact))+".pdf");
            else                          cRate.SaveAs("PDFs/optimizationV13/proposal/rate_"+tag+"_"+point+"_fixedRate"+fixedRate+"kHz_"+parametrisation+Kintgr+"p"+Kdecim+".pdf");
        }
    }

    c.cd();
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
    if (fixedThr==0) {
        if(parametrisation=="linear")  c.SaveAs("PDFs/optimizationV13/proposal/fixedRate"+fixedRate+"kHz_linear"+to_string(int(Kfact))+".pdf");
        else                           c.SaveAs("PDFs/optimizationV13/proposal/fixedRate"+fixedRate+"kHz_"+parametrisation+Kintgr+"p"+Kdecim+".pdf");
    }
    else {
        if(parametrisation=="linear")  c.SaveAs("PDFs/optimizationV13/proposal/fixedThr"+fixedThreshold+"GeV_linear"+to_string(int(Kfact))+".pdf");
        else                           c.SaveAs("PDFs/optimizationV13/proposal/fixedThr"+fixedThreshold+"GeV_"+parametrisation+Kintgr+"p"+Kdecim+".pdf");
    }

}





































