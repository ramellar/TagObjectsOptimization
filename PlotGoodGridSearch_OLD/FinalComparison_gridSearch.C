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

void compare(int run, bool zoom=false, float calibThr = 1.7) {
    TString run_str = to_string(run);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    gStyle->SetOptStat(000000);

    //#############################################
    // CMSSW_11_0_2 NTuples
    // RUN 323755
    // best to the eye first iteration
    //std::vector< std::pair<TString, TString> > points = {std::pair("effMin0p8","E5"), std::pair("effMin0p6","S3"), std::pair("effMin0p9","O4"), std::pair("effMin0p8","I6"), std::pair("effMin0p9","I1"), std::pair("effMin0p8","M6"), std::pair("effMin0p9","J1"), std::pair("effMin0p9","B4"), std::pair("effMin0p9","I1"), std::pair("effMin0p9","G7")};
    // best to the eye second iteration
    //std::vector< std::pair<TString, TString> > points = {std::pair("effMin0p8","E5"), std::pair("effMin0p9","O4"), std::pair("effMin0p8","I6"), std::pair("effMin0p8","M6")};
    // best to the eye third iteration
    //std::vector< std::pair<TString, TString> > points = {std::pair("effMin0p8","E5"), std::pair("effMin0p9","O4"), std::pair("effMin0p8","M6")};

    // RUN 323775
    // best to the eye with threshold 34
    //std::vector< std::pair<TString, TString> > points = {std::pair("effMin0p6","W1")};
    // best to the eye with threshold 35
    //std::vector< std::pair<TString, TString> > points = {std::pair("effMin0p8","A2"), std::pair("effMin0p8","E5"), std::pair("effMin0p6","L2")};
    // best to the eye with threshold 36
    //std::vector< std::pair<TString, TString> > points = {std::pair("effMin0p9","W5"), std::pair("effMin0p3","A1"), std::pair("effMin0p8","T5")};

    // final proposal plot
    //std::vector< std::pair<TString, TString> > points = {std::pair("effMin0p9","W5"), std::pair("effMin0p6","L2")};

    // rate at fixed thr check (use ~final proposal)
    //std::vector< std::pair<TString, TString> > points = {std::pair("effMin0p6","W1"), std::pair("effMin0p9","W5"), std::pair("effMin0p6","L2")};

    //#############################################
    // CMSSW_12_0_2 NTuples
    // RUN 323755
    // best to the eye first iteration
    //std::vector< std::pair<TString, TString> > points = {std::pair("effMin0p5","M0"), std::pair("effMin0p8","D7"), std::pair("effMin0p8","M6"), std::pair("effMin0p8","Q0"), std::pair("effMin0p6","C1"), std::pair("effMin0p7","K2"), std::pair("effMin0p6","L2"), std::pair("effMin0p6","W1")};
    // best to the eye second iteration
    //std::vector< std::pair<TString, TString> > points = {std::pair("effMin0p5","M0"), std::pair("effMin0p8","M6"), std::pair("effMin0p8","Q0"), std::pair("effMin0p6","C1")};
    // best to the eye third iteration
    //std::vector< std::pair<TString, TString> > points = {std::pair("effMin0p5","M0"), std::pair("effMin0p8","Q0")};

    // RUN 323775
    // best to the eye with threshold 33
    //std::vector< std::pair<TString, TString> > points = {std::pair("effMin0p5","M0"), std::pair("effMin0p8","D7"), std::pair("effMin0p8","M6")};
    // best to the eye with threshold 34
    std::vector< std::pair<TString, TString> > points = {std::pair("effMin0p8","Q0"), std::pair("effMin0p6","C1"), std::pair("effMin0p7","K2")};
    // best to the eye with threshold 35
    //std::vector< std::pair<TString, TString> > points = {};

    // final proposal plot
    //std::vector< std::pair<TString, TString> > points = {};

    // rate at fixed thr check (use ~final proposal)
    //std::vector< std::pair<TString, TString> > points = {};


    TCanvas c("c","c",650,600);
    c.SetLeftMargin(0.15);
    c.SetGrid();
    c.SetLogy();

    TLegend* leg = new TLegend(0.4,0.15,0.89,0.62);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);
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

    TFile* f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_02_02_optimizationV7_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_Run"+run_str+"_gs_effMin0p0_0_2022_02_02.root","READ");
    TGraphAsymmErrors* turnon_NewLayer1_Option22_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option22_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option31_extrap_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option31_extrap_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_noIso_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_noIso_by_pt");
    
    float fm_noIso       = ( (TVectorT<float>*)f->Get("fm_noIso") )->Max();                   // here we use max to EXTRACT THE ACCEPTANCE (the other entry is the 80% thr to calculate the acceptance and will always be smaller)
    float fm_Option31_extrap = ( (TVectorT<float>*)f->Get("fm_Option31_extrap") )->Max();     // "
    float fm_Option22 = ( (TVectorT<float>*)f->Get("fm_Option22") )->Max();                   // "
    float thr_noIso       = ( (TVectorT<float>*)f->Get("thr_noIso") )->Max();                 // here we use max to extract the ONLY VALUE contained in the vector 
    float thr_Option31_extrap = ( (TVectorT<float>*)f->Get("thr_Option31_extrap") )->Max();   // "
    float thr_Option22 = ( (TVectorT<float>*)f->Get("thr_Option22") )->Max();                 // "

    TString fm = "FM";
    turnon_NewLayer1_noIso_mean->SetTitle("");
    turnon_NewLayer1_noIso_mean->SetLineColor(kBlack);
    turnon_NewLayer1_noIso_mean->SetLineWidth(1);
    turnon_NewLayer1_noIso_mean->GetXaxis()->SetLimits(15,120.);
    if (zoom) 
    {
        turnon_NewLayer1_noIso_mean->GetXaxis()->SetLimits(40,120.);
        turnon_NewLayer1_noIso_mean->GetYaxis()->SetRangeUser(0.8,1.01);
    }
    turnon_NewLayer1_noIso_mean->GetYaxis()->SetTitle("Efficiency");
    turnon_NewLayer1_noIso_mean->GetXaxis()->SetTitle("Offline p_{T}(#tau) [GeV]");
    turnon_NewLayer1_noIso_mean->Draw();
    leg->AddEntry(turnon_NewLayer1_noIso_mean,"Di-#tau no-iso, mean - "+fm+"="+fm_noIso+" - THR="+round(thr_noIso),"L");

    turnon_NewLayer1_Option22_mean->SetLineColor(kGray+1);
    turnon_NewLayer1_Option22_mean->SetLineWidth(1);
    turnon_NewLayer1_Option22_mean->Draw("same");
    leg->AddEntry(turnon_NewLayer1_Option22_mean,"Di-#tau iso (Option 22) - "+fm+"="+fm_Option22+" - THR="+round(thr_Option22),"L");

    // turnon_NewLayer1_Option31_extrap_mean->SetLineColor(kGray+2);
    // turnon_NewLayer1_Option31_extrap_mean->SetLineWidth(1);
    // turnon_NewLayer1_Option31_extrap_mean->Draw("same");
    // leg->AddEntry(turnon_NewLayer1_Option31_extrap_mean,"Di-#tau iso (Option 31 extrap) - "+fm+"="+fm_Option31_extrap+" - THR="+round(thr_Option31_extrap),"L");

    f->Close();

    int j = 0;
    std::vector<int> colors = {kBlue, kRed, kCyan, kGreen, kYellow, kBlue-1, kCyan-1, kGreen-1, kYellow-1, kRed-1, kBlue-2, kCyan-2, kGreen-2, kYellow-2, kRed-2 , kBlue-2, kCyan-2, kGreen-2, kYellow-2, kRed-2, kBlue-3, kCyan-3, kGreen-3, kYellow-3, kRed-3, kBlue-4, kCyan-4, kGreen-4, kYellow-4, kRed-4, kBlue-5, kCyan-5, kGreen-5, kYellow-5, kRed-5, kBlue-6, kCyan-6, kGreen-6, kYellow-6, kRed-7, kBlue-7, kCyan-7, kGreen-7, kYellow-7, kRed-7};

    for (long unsigned int i = 0; i < points.size(); ++i)
    {
        TString tag = points[i].first;
        TString point = points[i].second;
        TString opt = std::string(points[i].second).front();
        TString num = std::string(points[i].second).back();

        TFile* f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_02_02_optimizationV7_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_Run"+run_str+"_gs_"+tag+"_"+num+"_2022_02_02.root","READ");
        TGraphAsymmErrors* turnon = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option"+point+"_by_pt");
        float THR = ( (TVectorT<float>*)f->Get("thr_Option"+opt) )->Max(); // here we use max to EXTRACT THE ACCEPTANCE (the other entry is the 80% thr to calculate the acceptance and will always be smaller) 
        float FM = ( (TVectorT<float>*)f->Get("fm_Option"+opt) )->Max(); // here we use max to extract the ONLY VALUE contained in the vector 
        turnon->SetLineColor(colors[j]); ++j;
        turnon->SetLineWidth(1);
        turnon->Draw("same");
        leg->AddEntry(turnon, "Di-#tau iso ("+tag+" Option "+point+") - FM="+FM+" - THR="+round(THR), "L");

        f->Close();

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
    c.SaveAs("plotsFinalComparison_Run"+run_str+"/Comparison_visualSelection_proposal_optimizationV7_eq34"+_tag+".pdf");
    //c.SaveAs("plotsFinalComparison_Run"+run_str+"/turnOns_fixedThr32"+_tag+".pdf");

}





































