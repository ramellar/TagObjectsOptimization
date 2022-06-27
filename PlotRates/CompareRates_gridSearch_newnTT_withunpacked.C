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
#include <fstream>
#include <map>

using namespace std;

void compare(TString tag="effMin0p0", TString opt="0", TString parametrisation = "linear", Double_t Kfact = 0.0, float targetRate = 14, int run=323755, TString nuGlobalTag = "122X", bool Run3reweight = false, float calibThr = 1.7) {
    TString run_str = to_string(run);

    TString isPUReweighted = "";
    if(Run3reweight) isPUReweighted = "puReweighted";

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    TString Kintgr = to_string(Kfact).substr(0, to_string(Kfact).find("."));
    TString Kdecim = to_string(Kfact).substr(2, to_string(Kfact).find("."));

    gStyle->SetOptStat(000000);

    TFile* f_mean;
    if(nuGlobalTag!="0"){ 
        if(parametrisation=="linear"){ f_mean = new TFile("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_SingleNeutrino"+nuGlobalTag+isPUReweighted+"_Run3_MC_optimizationV13gs_calibThr"+intgr+"p"+decim+"_linear_"+tag+".root","READ"); }
        else                         { f_mean = new TFile("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_SingleNeutrino"+nuGlobalTag+isPUReweighted+"_Run3_MC_optimizationV13gs_calibThr"+intgr+"p"+decim+"_"+parametrisation+Kintgr+"p"+Kdecim+"_"+tag+".root","READ"); }
    }
    else {
        if(parametrisation=="linear"){ f_mean = new TFile("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias_Run"+run_str+"_optimizationV13gs_calibThr"+intgr+"p"+decim+"_linear_"+tag+".root","READ"); }
        else                         { f_mean = new TFile("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias_Run"+run_str+"_optimizationV13gs_calibThr"+intgr+"p"+decim+"_"+parametrisation+Kintgr+"p"+Kdecim+"_"+tag+".root","READ"); }
    }
    TFile* f_unpacked = new TFile("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias_Run"+run_str+"_optimizationV13_calibThr"+intgr+"p"+decim+"_unpacked.root","READ");

    TH1F* rate_NewLayer1_noIso_mean       = (TH1F*)f_mean->Get("rate_noCut_DiTau");
    TH1F* rate_NewLayer1_noIso_unpacked   = (TH1F*)f_unpacked->Get("rate_noCut_DiTau");
    TH1F* rate_NewLayer1_Iso_unpacked     = (TH1F*)f_unpacked->Get("rate_Iso_DiTau");

    TH1F* rate_NewLayer1_OptionA_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_A"+opt);
    TH1F* rate_NewLayer1_OptionB_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_B"+opt);
    TH1F* rate_NewLayer1_OptionC_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_C"+opt);
    TH1F* rate_NewLayer1_OptionD_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_D"+opt);
    TH1F* rate_NewLayer1_OptionE_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_E"+opt);
    TH1F* rate_NewLayer1_OptionF_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_F"+opt);
    TH1F* rate_NewLayer1_OptionG_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_G"+opt);
    TH1F* rate_NewLayer1_OptionH_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_H"+opt);
    TH1F* rate_NewLayer1_OptionI_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_I"+opt);
    TH1F* rate_NewLayer1_OptionJ_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_J"+opt);
    TH1F* rate_NewLayer1_OptionK_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_K"+opt);
    TH1F* rate_NewLayer1_OptionL_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_L"+opt);
    TH1F* rate_NewLayer1_OptionM_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_M"+opt);
    TH1F* rate_NewLayer1_OptionN_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_N"+opt);
    TH1F* rate_NewLayer1_OptionO_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_O"+opt);
    TH1F* rate_NewLayer1_OptionP_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_P"+opt);
    TH1F* rate_NewLayer1_OptionQ_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_Q"+opt);
    TH1F* rate_NewLayer1_OptionR_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_R"+opt);
    TH1F* rate_NewLayer1_OptionS_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_S"+opt);
    TH1F* rate_NewLayer1_OptionT_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_T"+opt);
    TH1F* rate_NewLayer1_OptionU_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_U"+opt);
    TH1F* rate_NewLayer1_OptionV_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_V"+opt);
    TH1F* rate_NewLayer1_OptionW_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_W"+opt);
    TH1F* rate_NewLayer1_OptionX_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_X"+opt);
    TH1F* rate_NewLayer1_OptionY_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_Y"+opt);
    TH1F* rate_NewLayer1_OptionZ_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_Z"+opt);

    TH1F* rate_NewLayer1_Option1_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_1");
    TH1F* rate_NewLayer1_Option2_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_2");
    TH1F* rate_NewLayer1_Option3_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_3");
    TH1F* rate_NewLayer1_Option4_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_4");
    TH1F* rate_NewLayer1_Option5_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_5");
    TH1F* rate_NewLayer1_Option6_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_6");
    TH1F* rate_NewLayer1_Option7_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_7");
    TH1F* rate_NewLayer1_Option8_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_8");
    TH1F* rate_NewLayer1_Option9_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_9");
    TH1F* rate_NewLayer1_Option10_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_10");
    TH1F* rate_NewLayer1_Option11_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_11");
    TH1F* rate_NewLayer1_Option12_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_12");
    TH1F* rate_NewLayer1_Option13_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_13");
    TH1F* rate_NewLayer1_Option14_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_14");
    TH1F* rate_NewLayer1_Option15_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_15");
    TH1F* rate_NewLayer1_Option16_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_16");
    TH1F* rate_NewLayer1_Option17_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_17");
    TH1F* rate_NewLayer1_Option18_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_18");
    TH1F* rate_NewLayer1_Option19_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_19");
    TH1F* rate_NewLayer1_Option20_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_20");
    TH1F* rate_NewLayer1_Option21_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_21");
    TH1F* rate_NewLayer1_Option22_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_22");
    TH1F* rate_NewLayer1_Option23_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_23");
    TH1F* rate_NewLayer1_Option24_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_24");
    TH1F* rate_NewLayer1_Option25_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_25");
    TH1F* rate_NewLayer1_Option26_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_26");
    TH1F* rate_NewLayer1_Option27_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_27");
    TH1F* rate_NewLayer1_Option28_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_28");
    TH1F* rate_NewLayer1_Option29_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_29");
    TH1F* rate_NewLayer1_Option30_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_30");
    TH1F* rate_NewLayer1_Option31_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_31");
    TH1F* rate_NewLayer1_Option31_extrap_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_31_extrap");

/*
    TString CanvasName = "";
    if(nuGlobalTag!="0") {
        if(parametrisation=="linear"){ CanvasName = "Comparison_Rate_SingleNeutrino"+nuGlobalTag+isPUReweighted+"_Run"+run_str+"_newnTT_unpacked_optimizationV13gs_calibThr"+intgr+"p"+decim+"_linear_"+tag+"_"+opt; }
        else                         { CanvasName = "Comparison_Rate_SingleNeutrino"+nuGlobalTag+isPUReweighted+"_Run"+run_str+"_newnTT_unpacked_optimizationV13gs_calibThr"+intgr+"p"+decim+"_"+parametrisation+Kintgr+"p"+Kdecim+"_"+tag+"_"+opt; }
    }
    else {
        if(parametrisation=="linear"){ CanvasName = "Comparison_Rate_Run"+run_str+"_newnTT_unpacked_optimizationV13gs_calibThr"+intgr+"p"+decim+"_linear_"+tag+"_"+opt; }
        else                         { CanvasName = "Comparison_Rate_Run"+run_str+"_newnTT_unpacked_optimizationV13gs_calibThr"+intgr+"p"+decim+"_"+parametrisation+Kintgr+"p"+Kdecim+"_"+tag+"_"+opt; }
    }
    TString CanvasNamePdf = CanvasName ;
    CanvasNamePdf += ".pdf";
    TString CanvasNameRoot = CanvasName ;
    CanvasNameRoot += ".root";

    TCanvas c(CanvasName.Data(),CanvasName.Data(),650,600);
    c.SetLeftMargin(0.15);
    c.SetGrid();
    c.SetLogy();

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
    pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    pad1->SetLogy();

    rate_NewLayer1_noIso_unpacked->SetTitle("");
    rate_NewLayer1_noIso_unpacked->GetXaxis()->SetRangeUser(20.,60.);
    rate_NewLayer1_noIso_unpacked->GetYaxis()->SetTitle("Di-#tau rate [kHz]");
    rate_NewLayer1_noIso_unpacked->SetLineColor(kBlack);
    rate_NewLayer1_noIso_unpacked->SetLineWidth(1);
    rate_NewLayer1_noIso_unpacked->Draw();

    rate_NewLayer1_noIso_mean->SetLineColor(kBlack);
    rate_NewLayer1_noIso_mean->SetLineWidth(1);
    rate_NewLayer1_noIso_mean->SetLineStyle(5);
    rate_NewLayer1_noIso_mean->Draw("same");

    rate_NewLayer1_OptionA_mean->SetLineColor(kRed);
    rate_NewLayer1_OptionA_mean->SetLineWidth(1);
    rate_NewLayer1_OptionA_mean->Draw("same");

    rate_NewLayer1_OptionB_mean->SetLineColor(kRed-4);
    rate_NewLayer1_OptionB_mean->SetLineWidth(1);
    rate_NewLayer1_OptionB_mean->Draw("same");

    rate_NewLayer1_OptionC_mean->SetLineColor(kRed-7);
    rate_NewLayer1_OptionC_mean->SetLineWidth(1);
    rate_NewLayer1_OptionC_mean->Draw("same");

    rate_NewLayer1_OptionD_mean->SetLineColor(kRed-9);
    rate_NewLayer1_OptionD_mean->SetLineWidth(1);
    rate_NewLayer1_OptionD_mean->Draw("same");

    rate_NewLayer1_OptionE_mean->SetLineColor(kRed-10);
    rate_NewLayer1_OptionE_mean->SetLineWidth(1);
    rate_NewLayer1_OptionE_mean->Draw("same");

    rate_NewLayer1_OptionF_mean->SetLineColor(kMagenta);
    rate_NewLayer1_OptionF_mean->SetLineWidth(1);
    rate_NewLayer1_OptionF_mean->Draw("same");

    rate_NewLayer1_OptionG_mean->SetLineColor(kMagenta-4);
    rate_NewLayer1_OptionG_mean->SetLineWidth(1);
    rate_NewLayer1_OptionG_mean->Draw("same");

    rate_NewLayer1_OptionH_mean->SetLineColor(kMagenta-7);
    rate_NewLayer1_OptionH_mean->SetLineWidth(1);
    rate_NewLayer1_OptionH_mean->Draw("same");

    rate_NewLayer1_OptionI_mean->SetLineColor(kMagenta-9);
    rate_NewLayer1_OptionI_mean->SetLineWidth(1);
    rate_NewLayer1_OptionI_mean->Draw("same");

    rate_NewLayer1_OptionJ_mean->SetLineColor(kMagenta-10);
    rate_NewLayer1_OptionJ_mean->SetLineWidth(1);
    rate_NewLayer1_OptionJ_mean->Draw("same");

    rate_NewLayer1_OptionK_mean->SetLineColor(kBlue);
    rate_NewLayer1_OptionK_mean->SetLineWidth(1);
    rate_NewLayer1_OptionK_mean->Draw("same");

    rate_NewLayer1_OptionL_mean->SetLineColor(kBlue);
    rate_NewLayer1_OptionL_mean->SetLineWidth(1);
    rate_NewLayer1_OptionL_mean->Draw("same");

    rate_NewLayer1_OptionM_mean->SetLineColor(kBlue-7);
    rate_NewLayer1_OptionM_mean->SetLineWidth(1);
    rate_NewLayer1_OptionM_mean->Draw("same");

    rate_NewLayer1_OptionN_mean->SetLineColor(kBlue-9);
    rate_NewLayer1_OptionN_mean->SetLineWidth(1);
    rate_NewLayer1_OptionN_mean->Draw("same");

    rate_NewLayer1_OptionO_mean->SetLineColor(kBlue-10);
    rate_NewLayer1_OptionO_mean->SetLineWidth(1);
    rate_NewLayer1_OptionO_mean->Draw("same");

    rate_NewLayer1_OptionP_mean->SetLineColor(kCyan);
    rate_NewLayer1_OptionP_mean->SetLineWidth(1);
    rate_NewLayer1_OptionP_mean->Draw("same");

    rate_NewLayer1_OptionQ_mean->SetLineColor(kCyan-4);
    rate_NewLayer1_OptionQ_mean->SetLineWidth(1);
    rate_NewLayer1_OptionQ_mean->Draw("same");

    rate_NewLayer1_OptionR_mean->SetLineColor(kCyan-7);
    rate_NewLayer1_OptionR_mean->SetLineWidth(1);
    rate_NewLayer1_OptionR_mean->Draw("same");

    rate_NewLayer1_OptionS_mean->SetLineColor(kCyan-9);
    rate_NewLayer1_OptionS_mean->SetLineWidth(1);
    rate_NewLayer1_OptionS_mean->Draw("same");

    rate_NewLayer1_OptionT_mean->SetLineColor(kCyan-10);
    rate_NewLayer1_OptionT_mean->SetLineWidth(1);
    rate_NewLayer1_OptionT_mean->Draw("same");

    rate_NewLayer1_OptionU_mean->SetLineColor(kGreen);
    rate_NewLayer1_OptionU_mean->SetLineWidth(1);
    rate_NewLayer1_OptionU_mean->Draw("same");

    rate_NewLayer1_OptionV_mean->SetLineColor(kGreen-4);
    rate_NewLayer1_OptionV_mean->SetLineWidth(1);
    rate_NewLayer1_OptionV_mean->Draw("same");

    rate_NewLayer1_OptionW_mean->SetLineColor(kGreen-7);
    rate_NewLayer1_OptionW_mean->SetLineWidth(1);
    rate_NewLayer1_OptionW_mean->Draw("same");

    rate_NewLayer1_OptionX_mean->SetLineColor(kGreen-9);
    rate_NewLayer1_OptionX_mean->SetLineWidth(1);
    rate_NewLayer1_OptionX_mean->Draw("same");

    rate_NewLayer1_OptionY_mean->SetLineColor(kGreen-10);
    rate_NewLayer1_OptionY_mean->SetLineWidth(1);
    rate_NewLayer1_OptionY_mean->Draw("same");

    rate_NewLayer1_OptionZ_mean->SetLineColor(kYellow);
    rate_NewLayer1_OptionZ_mean->SetLineWidth(1);
    rate_NewLayer1_OptionZ_mean->Draw("same");

    rate_NewLayer1_Option22_mean->SetLineColor(kGray+1);
    rate_NewLayer1_Option22_mean->SetLineWidth(1);
    rate_NewLayer1_Option22_mean->Draw("same");

    rate_NewLayer1_Option31_extrap_mean->SetLineColor(kGray+2);
    rate_NewLayer1_Option31_extrap_mean->SetLineWidth(1);
    rate_NewLayer1_Option31_extrap_mean->Draw("same");

    TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
    texl->AddText("CMS Internal, #sqrt{s}=13 TeV, Run #"+run_str+" (2018)");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    texl->Draw("same");

    TLegend* leg = new TLegend(0.65,0.35,0.81,0.87);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);
    leg->SetHeader("Linearly scaled to 2.0E34");

    leg->AddEntry(rate_NewLayer1_noIso_unpacked,"Di-#tau no-iso, unpacked","L");
    leg->AddEntry(rate_NewLayer1_noIso_mean,"Di-#tau no-iso, mean","L");

    leg->AddEntry(rate_NewLayer1_OptionA_mean,"Di-#tau iso (Option A"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionB_mean,"Di-#tau iso (Option B"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionC_mean,"Di-#tau iso (Option C"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionD_mean,"Di-#tau iso (Option D"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionE_mean,"Di-#tau iso (Option E"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionF_mean,"Di-#tau iso (Option F"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionG_mean,"Di-#tau iso (Option G"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionH_mean,"Di-#tau iso (Option H"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionI_mean,"Di-#tau iso (Option I"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionJ_mean,"Di-#tau iso (Option J"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionK_mean,"Di-#tau iso (Option K"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionL_mean,"Di-#tau iso (Option L"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionM_mean,"Di-#tau iso (Option M"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionN_mean,"Di-#tau iso (Option N"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionO_mean,"Di-#tau iso (Option O"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionP_mean,"Di-#tau iso (Option P"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionQ_mean,"Di-#tau iso (Option Q"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionR_mean,"Di-#tau iso (Option R"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionS_mean,"Di-#tau iso (Option S"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionT_mean,"Di-#tau iso (Option T"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionU_mean,"Di-#tau iso (Option U"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionV_mean,"Di-#tau iso (Option V"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionW_mean,"Di-#tau iso (Option W"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionX_mean,"Di-#tau iso (Option X"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionY_mean,"Di-#tau iso (Option Y"+opt+")","L");
    leg->AddEntry(rate_NewLayer1_OptionZ_mean,"Di-#tau iso (Option Z"+opt+")","L");

    leg->AddEntry(rate_NewLayer1_Option22_mean,"Di-#tau iso (Option 22)","L");
    leg->AddEntry(rate_NewLayer1_Option31_extrap_mean,"Di-#tau iso (Option 31)","L");

    leg->Draw("same");

    c.cd();

    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0.05);
    pad2->SetLeftMargin(0.15);
    pad2->SetBottomMargin(0.30);
    pad2->SetGridx(); // vertical grid
    pad2->Draw();
    pad2->cd();

    TH1F* ratioPlotA_mean = (TH1F*)rate_NewLayer1_OptionA_mean->Clone("ratioPlot");
    ratioPlotA_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotA_mean->SetLineColor(kRed);
    ratioPlotA_mean->Draw("same");

    TH1F* ratioPlotB_mean = (TH1F*)rate_NewLayer1_OptionB_mean->Clone("ratioPlot");
    ratioPlotB_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotB_mean->SetLineColor(kRed-4);
    ratioPlotB_mean->Draw("same");

    TH1F* ratioPlotC_mean = (TH1F*)rate_NewLayer1_OptionC_mean->Clone("ratioPlot");
    ratioPlotC_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotC_mean->SetLineColor(kRed-7);
    ratioPlotC_mean->Draw("same");

    TH1F* ratioPlotD_mean = (TH1F*)rate_NewLayer1_OptionD_mean->Clone("ratioPlot");
    ratioPlotD_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotD_mean->SetLineColor(kRed-9);
    ratioPlotD_mean->Draw("same");

    TH1F* ratioPlotE_mean = (TH1F*)rate_NewLayer1_OptionE_mean->Clone("ratioPlot");
    ratioPlotE_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotE_mean->SetLineColor(kRed-10);
    ratioPlotE_mean->Draw("same");

    TH1F* ratioPlotF_mean = (TH1F*)rate_NewLayer1_OptionF_mean->Clone("ratioPlot");
    ratioPlotF_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotF_mean->SetLineColor(kMagenta);
    ratioPlotF_mean->Draw("same");

    TH1F* ratioPlotG_mean = (TH1F*)rate_NewLayer1_OptionG_mean->Clone("ratioPlot");
    ratioPlotG_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotG_mean->SetLineColor(kMagenta-4);
    ratioPlotG_mean->Draw("same");

    TH1F* ratioPlotH_mean = (TH1F*)rate_NewLayer1_OptionH_mean->Clone("ratioPlot");
    ratioPlotH_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotH_mean->SetLineColor(kMagenta-7);
    ratioPlotH_mean->Draw("same");

    TH1F* ratioPlotI_mean = (TH1F*)rate_NewLayer1_OptionI_mean->Clone("ratioPlot");
    ratioPlotI_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotI_mean->SetLineColor(kMagenta-9);
    ratioPlotI_mean->Draw("same");

    TH1F* ratioPlotJ_mean = (TH1F*)rate_NewLayer1_OptionJ_mean->Clone("ratioPlot");
    ratioPlotJ_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotJ_mean->SetLineColor(kMagenta-10);
    ratioPlotJ_mean->Draw("same");

    TH1F* ratioPlotK_mean = (TH1F*)rate_NewLayer1_OptionK_mean->Clone("ratioPlot");
    ratioPlotK_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotK_mean->SetLineColor(kBlue);
    ratioPlotK_mean->Draw("same");

    TH1F* ratioPlotL_mean = (TH1F*)rate_NewLayer1_OptionL_mean->Clone("ratioPlot");
    ratioPlotL_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotL_mean->SetLineColor(kBlue);
    ratioPlotL_mean->Draw("same");

    TH1F* ratioPlotM_mean = (TH1F*)rate_NewLayer1_OptionM_mean->Clone("ratioPlot");
    ratioPlotM_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotM_mean->SetLineColor(kBlue-7);
    ratioPlotM_mean->Draw("same");

    TH1F* ratioPlotN_mean = (TH1F*)rate_NewLayer1_OptionN_mean->Clone("ratioPlot");
    ratioPlotN_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotN_mean->SetLineColor(kBlue-9);
    ratioPlotN_mean->Draw("same");

    TH1F* ratioPlotO_mean = (TH1F*)rate_NewLayer1_OptionO_mean->Clone("ratioPlot");
    ratioPlotO_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotO_mean->SetLineColor(kBlue-10);
    ratioPlotO_mean->Draw("same");

    TH1F* ratioPlotP_mean = (TH1F*)rate_NewLayer1_OptionP_mean->Clone("ratioPlot");
    ratioPlotP_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotP_mean->SetLineColor(kCyan);
    ratioPlotP_mean->Draw("same");

    TH1F* ratioPlotQ_mean = (TH1F*)rate_NewLayer1_OptionQ_mean->Clone("ratioPlot");
    ratioPlotQ_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotQ_mean->SetLineColor(kCyan-4);
    ratioPlotQ_mean->Draw("same");

    TH1F* ratioPlotR_mean = (TH1F*)rate_NewLayer1_OptionR_mean->Clone("ratioPlot");
    ratioPlotR_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotR_mean->SetLineColor(kCyan-7);
    ratioPlotR_mean->Draw("same");

    TH1F* ratioPlotS_mean = (TH1F*)rate_NewLayer1_OptionS_mean->Clone("ratioPlot");
    ratioPlotS_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotS_mean->SetLineColor(kCyan-9);
    ratioPlotS_mean->Draw("same");

    TH1F* ratioPlotT_mean = (TH1F*)rate_NewLayer1_OptionT_mean->Clone("ratioPlot");
    ratioPlotT_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotT_mean->SetLineColor(kCyan-10);
    ratioPlotT_mean->Draw("same");

    TH1F* ratioPlotU_mean = (TH1F*)rate_NewLayer1_OptionU_mean->Clone("ratioPlot");
    ratioPlotU_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotU_mean->SetLineColor(kGreen);
    ratioPlotU_mean->Draw("same");

    TH1F* ratioPlotV_mean = (TH1F*)rate_NewLayer1_OptionV_mean->Clone("ratioPlot");
    ratioPlotV_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotV_mean->SetLineColor(kGreen-4);
    ratioPlotV_mean->Draw("same");

    TH1F* ratioPlotW_mean = (TH1F*)rate_NewLayer1_OptionW_mean->Clone("ratioPlot");
    ratioPlotW_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotW_mean->SetLineColor(kGreen-7);
    ratioPlotW_mean->Draw("same");

    TH1F* ratioPlotX_mean = (TH1F*)rate_NewLayer1_OptionX_mean->Clone("ratioPlot");
    ratioPlotX_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotX_mean->SetLineColor(kGreen-9);
    ratioPlotX_mean->Draw("same");

    TH1F* ratioPlotY_mean = (TH1F*)rate_NewLayer1_OptionY_mean->Clone("ratioPlot");
    ratioPlotY_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotY_mean->SetLineColor(kGreen-10);
    ratioPlotY_mean->Draw("same");

    TH1F* ratioPlotZ_mean = (TH1F*)rate_NewLayer1_OptionZ_mean->Clone("ratioPlot");
    ratioPlotZ_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlotZ_mean->SetLineColor(kYellow);
    ratioPlotZ_mean->Draw("same");

    TH1F* ratioPlot22_mean = (TH1F*)rate_NewLayer1_Option22_mean->Clone("ratioPlot");
    ratioPlot22_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlot22_mean->SetLineColor(kGray+1);
    ratioPlot22_mean->Draw("same");

    TH1F* ratioPlot31_extrap_mean = (TH1F*)rate_NewLayer1_Option31_extrap_mean->Clone("ratioPlot");
    ratioPlot31_extrap_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlot31_extrap_mean->SetLineColor(kGray+2);
    ratioPlot31_extrap_mean->Draw("same");

    ratioPlotA_mean->GetXaxis()->SetLabelSize(0.09);
    ratioPlotA_mean->GetYaxis()->SetLabelSize(0.09);
    ratioPlotA_mean->GetYaxis()->SetTitleSize(0.09);
    ratioPlotA_mean->SetTitle("");
    ratioPlotA_mean->GetXaxis()->SetRangeUser(20.,60.);
    ratioPlotA_mean->GetYaxis()->SetRangeUser(0.,2.);
    ratioPlotA_mean->GetYaxis()->SetTitle("iso(Option)/non-iso");
    ratioPlotA_mean->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) threshold [GeV]");
    ratioPlotA_mean->GetXaxis()->SetTitleSize(0.11);
    ratioPlotA_mean->GetYaxis()->SetTitleOffset(0.5);

    TString PDFs = "PDFs/optimizationV13/";
    TString ROOTs = "ROOTs/";
    c.SaveAs(PDFs+CanvasNamePdf.Data());
    c.SaveAs(ROOTs+CanvasNameRoot.Data());
*/

    //find first threshold giving < targetRate kHz.
    Double_t Target = targetRate;

    Double_t Threshold_NewLayer1_noIso = 0.;
    Double_t Threshold_NewLayer1_noIso_unpacked = 0.;
    Double_t Threshold_NewLayer1_Iso_unpacked = 0.;
    Double_t Threshold_NewLayer1_OptionA = 0.;
    Double_t Threshold_NewLayer1_OptionB = 0.;
    Double_t Threshold_NewLayer1_OptionC = 0.;
    Double_t Threshold_NewLayer1_OptionD = 0.;
    Double_t Threshold_NewLayer1_OptionE = 0.;
    Double_t Threshold_NewLayer1_OptionF = 0.;
    Double_t Threshold_NewLayer1_OptionG = 0.;
    Double_t Threshold_NewLayer1_OptionH = 0.;
    Double_t Threshold_NewLayer1_OptionI = 0.;
    Double_t Threshold_NewLayer1_OptionJ = 0.;
    Double_t Threshold_NewLayer1_OptionK = 0.;
    Double_t Threshold_NewLayer1_OptionL = 0.;
    Double_t Threshold_NewLayer1_OptionM = 0.;
    Double_t Threshold_NewLayer1_OptionN = 0.;
    Double_t Threshold_NewLayer1_OptionO = 0.;
    Double_t Threshold_NewLayer1_OptionP = 0.;
    Double_t Threshold_NewLayer1_OptionQ = 0.;
    Double_t Threshold_NewLayer1_OptionR = 0.;
    Double_t Threshold_NewLayer1_OptionS = 0.;
    Double_t Threshold_NewLayer1_OptionT = 0.;
    Double_t Threshold_NewLayer1_OptionU = 0.;
    Double_t Threshold_NewLayer1_OptionV = 0.;
    Double_t Threshold_NewLayer1_OptionW = 0.;
    Double_t Threshold_NewLayer1_OptionX = 0.;
    Double_t Threshold_NewLayer1_OptionY = 0.;
    Double_t Threshold_NewLayer1_OptionZ = 0.;

    Double_t Threshold_NewLayer1_Option1 = 0.;
    Double_t Threshold_NewLayer1_Option2 = 0.;
    Double_t Threshold_NewLayer1_Option3 = 0.;
    Double_t Threshold_NewLayer1_Option4 = 0.;
    Double_t Threshold_NewLayer1_Option5 = 0.;
    Double_t Threshold_NewLayer1_Option6 = 0.;
    Double_t Threshold_NewLayer1_Option7 = 0.;
    Double_t Threshold_NewLayer1_Option8 = 0.;
    Double_t Threshold_NewLayer1_Option9 = 0.;
    Double_t Threshold_NewLayer1_Option10 = 0.;
    Double_t Threshold_NewLayer1_Option11 = 0.;
    Double_t Threshold_NewLayer1_Option12 = 0.;
    Double_t Threshold_NewLayer1_Option13 = 0.;
    Double_t Threshold_NewLayer1_Option14 = 0.;
    Double_t Threshold_NewLayer1_Option15 = 0.;
    Double_t Threshold_NewLayer1_Option16 = 0.;
    Double_t Threshold_NewLayer1_Option17 = 0.;
    Double_t Threshold_NewLayer1_Option18 = 0.;
    Double_t Threshold_NewLayer1_Option19 = 0.;
    Double_t Threshold_NewLayer1_Option20 = 0.;
    Double_t Threshold_NewLayer1_Option21 = 0.;
    Double_t Threshold_NewLayer1_Option22 = 0.;
    Double_t Threshold_NewLayer1_Option23 = 0.;
    Double_t Threshold_NewLayer1_Option24 = 0.;
    Double_t Threshold_NewLayer1_Option25 = 0.;
    Double_t Threshold_NewLayer1_Option26 = 0.;
    Double_t Threshold_NewLayer1_Option27 = 0.;
    Double_t Threshold_NewLayer1_Option28 = 0.;
    Double_t Threshold_NewLayer1_Option29 = 0.;
    Double_t Threshold_NewLayer1_Option30 = 0.;
    Double_t Threshold_NewLayer1_Option31 = 0.;
    Double_t Threshold_NewLayer1_Option31_extrap = 0.;


    for(Int_t i = 1 ; i <= rate_NewLayer1_noIso_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_noIso_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_noIso = rate_NewLayer1_noIso_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_noIso_unpacked->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_noIso_unpacked->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_noIso_unpacked = rate_NewLayer1_noIso_unpacked->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Iso_unpacked->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Iso_unpacked->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_Iso_unpacked = rate_NewLayer1_Iso_unpacked->GetBinLowEdge(i);
            break;
        }
    }

    
    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionA_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionA_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionA = rate_NewLayer1_OptionA_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionB_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionB_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionB = rate_NewLayer1_OptionB_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionC_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionC_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionC = rate_NewLayer1_OptionC_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionD_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionD_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionD = rate_NewLayer1_OptionD_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionE_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionE_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionE = rate_NewLayer1_OptionE_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionF_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionF_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionF = rate_NewLayer1_OptionF_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionG_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionG_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionG = rate_NewLayer1_OptionG_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionH_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionH_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionH = rate_NewLayer1_OptionH_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionI_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionI_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionI = rate_NewLayer1_OptionI_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionJ_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionJ_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionJ = rate_NewLayer1_OptionJ_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionK_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionK_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionK = rate_NewLayer1_OptionK_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionL_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionL_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionL = rate_NewLayer1_OptionL_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionM_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionM_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionM = rate_NewLayer1_OptionM_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionN_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionN_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionN = rate_NewLayer1_OptionN_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionO_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionO_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionO = rate_NewLayer1_OptionO_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionP_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionP_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionP = rate_NewLayer1_OptionP_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionQ_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionQ_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionQ = rate_NewLayer1_OptionQ_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionR_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionR_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionR = rate_NewLayer1_OptionR_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionS_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionS_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionS = rate_NewLayer1_OptionS_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionT_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionT_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionT = rate_NewLayer1_OptionT_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionU_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionU_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionU = rate_NewLayer1_OptionU_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionV_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionV_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionV = rate_NewLayer1_OptionV_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionW_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionW_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionW = rate_NewLayer1_OptionW_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionX_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionX_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionX = rate_NewLayer1_OptionX_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionY_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionY_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionY = rate_NewLayer1_OptionY_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_OptionZ_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_OptionZ_mean->GetBinContent(i)<=Target)
        {
            Threshold_NewLayer1_OptionZ = rate_NewLayer1_OptionZ_mean->GetBinLowEdge(i);
            break;
        }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option1_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option1_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option1 = rate_NewLayer1_Option1_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option2_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option2_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option2 = rate_NewLayer1_Option2_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option3_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option3_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option3 = rate_NewLayer1_Option3_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option4_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option4_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option4 = rate_NewLayer1_Option4_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option5_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option5_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option5 = rate_NewLayer1_Option5_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option6_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option6_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option6 = rate_NewLayer1_Option6_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option7_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option7_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option7 = rate_NewLayer1_Option7_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option8_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option8_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option8 = rate_NewLayer1_Option8_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option9_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option9_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option9 = rate_NewLayer1_Option9_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option10_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option10_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option10 = rate_NewLayer1_Option10_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option11_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option11_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option11 = rate_NewLayer1_Option11_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option12_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option12_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option12 = rate_NewLayer1_Option12_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option13_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option13_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option13 = rate_NewLayer1_Option13_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option14_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option14_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option14 = rate_NewLayer1_Option14_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option15_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option15_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option15 = rate_NewLayer1_Option15_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option16_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option16_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option16 = rate_NewLayer1_Option16_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option17_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option17_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option17 = rate_NewLayer1_Option17_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option18_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option18_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option18 = rate_NewLayer1_Option18_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option19_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option19_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option19 = rate_NewLayer1_Option19_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option20_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option20_mean->GetBinContent(i)<=Target)
    {
        Threshold_NewLayer1_Option20 = rate_NewLayer1_Option20_mean->GetBinLowEdge(i);
        break;
    }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option21_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option21_mean->GetBinContent(i)<=Target)
            {
                Threshold_NewLayer1_Option21 = rate_NewLayer1_Option21_mean->GetBinLowEdge(i);
                break;
            }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option22_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option22_mean->GetBinContent(i)<=Target)
            {
                Threshold_NewLayer1_Option22 = rate_NewLayer1_Option22_mean->GetBinLowEdge(i);
                break;
            }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option23_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option23_mean->GetBinContent(i)<=Target)
            {
                Threshold_NewLayer1_Option23 = rate_NewLayer1_Option23_mean->GetBinLowEdge(i);
                break;
            }
    }


    for(Int_t i = 1 ; i <= rate_NewLayer1_Option24_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option24_mean->GetBinContent(i)<=Target)
            {
                Threshold_NewLayer1_Option24 = rate_NewLayer1_Option24_mean->GetBinLowEdge(i);
                break;
            }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option25_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option25_mean->GetBinContent(i)<=Target)
            {
                Threshold_NewLayer1_Option25 = rate_NewLayer1_Option25_mean->GetBinLowEdge(i);
                break;
            }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option26_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option26_mean->GetBinContent(i)<=Target)
            {
                Threshold_NewLayer1_Option26 = rate_NewLayer1_Option26_mean->GetBinLowEdge(i);
                break;
            }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option27_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option27_mean->GetBinContent(i)<=Target)
            {
                Threshold_NewLayer1_Option27 = rate_NewLayer1_Option27_mean->GetBinLowEdge(i);
                break;
            }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option28_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option28_mean->GetBinContent(i)<=Target)
            {
                Threshold_NewLayer1_Option28 = rate_NewLayer1_Option28_mean->GetBinLowEdge(i);
                break;
            }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option29_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option29_mean->GetBinContent(i)<=Target)
            {
                Threshold_NewLayer1_Option29 = rate_NewLayer1_Option29_mean->GetBinLowEdge(i);
                break;
            }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option30_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option30_mean->GetBinContent(i)<=Target)
            {
                Threshold_NewLayer1_Option30 = rate_NewLayer1_Option30_mean->GetBinLowEdge(i);
                break;
            }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option31_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option31_mean->GetBinContent(i)<=Target)
            {
                Threshold_NewLayer1_Option31 = rate_NewLayer1_Option31_mean->GetBinLowEdge(i);
                break;
            }
    }

    for(Int_t i = 1 ; i <= rate_NewLayer1_Option31_extrap_mean->GetNbinsX() ; ++i)
    {
        if(rate_NewLayer1_Option31_extrap_mean->GetBinContent(i)<=Target)
            {
                Threshold_NewLayer1_Option31_extrap = rate_NewLayer1_Option31_extrap_mean->GetBinLowEdge(i);
                break;
            }
    }

    if (tag=="effMin0p0" && opt=="0")
    {
        cout<<"Threshold_NewLayer1_noIso   = "<<Threshold_NewLayer1_noIso-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_noIso_unpacked = "<<Threshold_NewLayer1_noIso_unpacked-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Iso_unpacked = "<<Threshold_NewLayer1_Iso_unpacked-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option1 = "<<Threshold_NewLayer1_Option1-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option2 = "<<Threshold_NewLayer1_Option2-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option3 = "<<Threshold_NewLayer1_Option3-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option4 = "<<Threshold_NewLayer1_Option4-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option5 = "<<Threshold_NewLayer1_Option5-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option6 = "<<Threshold_NewLayer1_Option6-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option7 = "<<Threshold_NewLayer1_Option7-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option8 = "<<Threshold_NewLayer1_Option8-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option9 = "<<Threshold_NewLayer1_Option9-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option10 = "<<Threshold_NewLayer1_Option10-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option11 = "<<Threshold_NewLayer1_Option11-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option12 = "<<Threshold_NewLayer1_Option12-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option13 = "<<Threshold_NewLayer1_Option13-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option14 = "<<Threshold_NewLayer1_Option14-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option15 = "<<Threshold_NewLayer1_Option15-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option16 = "<<Threshold_NewLayer1_Option16-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option17 = "<<Threshold_NewLayer1_Option17-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option18 = "<<Threshold_NewLayer1_Option18-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option19 = "<<Threshold_NewLayer1_Option19-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option20 = "<<Threshold_NewLayer1_Option20-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option21 = "<<Threshold_NewLayer1_Option21-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option22 = "<<Threshold_NewLayer1_Option22-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option23 = "<<Threshold_NewLayer1_Option23-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option24 = "<<Threshold_NewLayer1_Option24-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option25 = "<<Threshold_NewLayer1_Option25-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option26 = "<<Threshold_NewLayer1_Option26-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option27 = "<<Threshold_NewLayer1_Option27-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option28 = "<<Threshold_NewLayer1_Option28-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option29 = "<<Threshold_NewLayer1_Option29-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option30 = "<<Threshold_NewLayer1_Option30-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option31 = "<<Threshold_NewLayer1_Option31-0.49<<";"<<endl;
        cout<<"Threshold_NewLayer1_Option31_extrap = "<<Threshold_NewLayer1_Option31_extrap-0.49<<";"<<endl;

        cout<<"if (tag==\""+tag+"\" and opt==\""+opt+"\")" << endl;
    }
    else {
        cout<<"else if (tag==\""+tag+"\" and opt==\""+opt+"\")" << endl;
    }
    
    cout<< "{" << endl;
    cout<<"    Threshold_NewLayer1_OptionA = "<<Threshold_NewLayer1_OptionA-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionB = "<<Threshold_NewLayer1_OptionB-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionC = "<<Threshold_NewLayer1_OptionC-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionD = "<<Threshold_NewLayer1_OptionD-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionE = "<<Threshold_NewLayer1_OptionE-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionF = "<<Threshold_NewLayer1_OptionF-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionG = "<<Threshold_NewLayer1_OptionG-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionH = "<<Threshold_NewLayer1_OptionH-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionI = "<<Threshold_NewLayer1_OptionI-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionJ = "<<Threshold_NewLayer1_OptionJ-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionK = "<<Threshold_NewLayer1_OptionK-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionL = "<<Threshold_NewLayer1_OptionL-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionM = "<<Threshold_NewLayer1_OptionM-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionN = "<<Threshold_NewLayer1_OptionN-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionO = "<<Threshold_NewLayer1_OptionO-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionP = "<<Threshold_NewLayer1_OptionP-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionQ = "<<Threshold_NewLayer1_OptionQ-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionR = "<<Threshold_NewLayer1_OptionR-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionS = "<<Threshold_NewLayer1_OptionS-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionT = "<<Threshold_NewLayer1_OptionT-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionU = "<<Threshold_NewLayer1_OptionU-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionV = "<<Threshold_NewLayer1_OptionV-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionW = "<<Threshold_NewLayer1_OptionW-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionX = "<<Threshold_NewLayer1_OptionX-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionY = "<<Threshold_NewLayer1_OptionY-0.49<<";"<<endl;
    cout<<"    Threshold_NewLayer1_OptionZ = "<<Threshold_NewLayer1_OptionZ-0.49<<";"<<endl;
    cout<< "}" << endl;
    

    // cout<<endl;
    // cout<<"TString Threshold_NewLayer1_noIso_newnTT = \""<<Threshold_NewLayer1_noIso<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_noIso_unpacked_newnTT = \""<<Threshold_NewLayer1_noIso_unpacked<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_Iso_unpacked_newnTT = \""<<Threshold_NewLayer1_Iso_unpacked<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionA_newnTT = \""<<Threshold_NewLayer1_OptionA<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionB_newnTT = \""<<Threshold_NewLayer1_OptionB<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionC_newnTT = \""<<Threshold_NewLayer1_OptionC<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionD_newnTT = \""<<Threshold_NewLayer1_OptionD<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionE_newnTT = \""<<Threshold_NewLayer1_OptionE<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionF_newnTT = \""<<Threshold_NewLayer1_OptionF<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionG_newnTT = \""<<Threshold_NewLayer1_OptionG<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionH_newnTT = \""<<Threshold_NewLayer1_OptionH<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionI_newnTT = \""<<Threshold_NewLayer1_OptionI<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionJ_newnTT = \""<<Threshold_NewLayer1_OptionJ<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionK_newnTT = \""<<Threshold_NewLayer1_OptionK<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionL_newnTT = \""<<Threshold_NewLayer1_OptionL<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionM_newnTT = \""<<Threshold_NewLayer1_OptionM<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionN_newnTT = \""<<Threshold_NewLayer1_OptionN<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionO_newnTT = \""<<Threshold_NewLayer1_OptionO<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionP_newnTT = \""<<Threshold_NewLayer1_OptionP<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionQ_newnTT = \""<<Threshold_NewLayer1_OptionQ<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionR_newnTT = \""<<Threshold_NewLayer1_OptionR<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionS_newnTT = \""<<Threshold_NewLayer1_OptionS<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionT_newnTT = \""<<Threshold_NewLayer1_OptionT<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionU_newnTT = \""<<Threshold_NewLayer1_OptionU<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionV_newnTT = \""<<Threshold_NewLayer1_OptionV<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionW_newnTT = \""<<Threshold_NewLayer1_OptionW<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionX_newnTT = \""<<Threshold_NewLayer1_OptionX<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionY_newnTT = \""<<Threshold_NewLayer1_OptionY<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_OptionZ_newnTT = \""<<Threshold_NewLayer1_OptionZ<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_Option22_newnTT = \""<<Threshold_NewLayer1_Option22<<"\";"<<endl;
    // cout<<"TString Threshold_NewLayer1_Option31_extrap_newnTT = \""<<Threshold_NewLayer1_Option31_extrap<<"\";"<<endl;

    f_mean->Close();
    f_unpacked->Close();

}
