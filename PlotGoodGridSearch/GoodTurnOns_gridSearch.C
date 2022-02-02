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

bool isGoodTurnON(TGraphAsymmErrors* baseline, TGraphAsymmErrors* newOpt) {
    Int_t point1 = 8;
    Int_t point2 = 14;
    Double_t y;
    Double_t th;
    Double_t _;
    bool good = true;

    for (int i = point1; i < point2; ++i)
    {
        baseline->GetPoint(i, _, th);
        newOpt->GetPoint(i, _, y);
        if (y < th) 
        {
            good = false;
            break;
        }
    }

    return good;
}

void compare(int run, TString tag, TString opt="0", TString baseline="22", int fixedThr = 0, float calibThr = 1.7) {
    TString run_str = to_string(run);

    TString fixedThreshold = to_string(fixedThr);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));
    
    gStyle->SetOptStat(000000);

    TFile* f;
    if (fixedThr==0) f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_01_28_optimizationV6_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_Run"+run_str+"_gs_"+tag+"_"+opt+"_2022_01_28.root", "READ");
    else             f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_01_28_optimizationV6_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDTHR"+fixedThreshold+"_Run"+run_str+"_gs_"+tag+"_"+opt+"_2022_01_28.root", "READ");
    

    TGraphAsymmErrors* turnon_NewLayer1_noIso_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_noIso_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option22_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option22_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_Option31_extrap_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option31_extrap_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionA_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionA"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionB_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionB"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionC_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionC"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionD_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionD"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionE_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionE"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionF_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionF"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionG_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionG"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionH_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionH"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionI_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionI"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionJ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionJ"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionK_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionK"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionL_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionL"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionM_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionM"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionN_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionN"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionO_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionO"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionP_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionP"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionQ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionQ"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionR_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionR"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionS_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionS"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionT_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionT"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionU_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionU"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionV_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionV"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionW_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionW"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionX_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionX"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionY_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionY"+opt+"_by_pt");
    TGraphAsymmErrors* turnon_NewLayer1_OptionZ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionZ"+opt+"_by_pt");

    TFile* f_mean = new TFile("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias_Run"+run_str+"_optimizationV6gs_calibThr"+intgr+"p"+decim+"_"+tag+".root","READ");
    TFile* f_unpacked = new TFile("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias_Run"+run_str+"_optimizationV6_calibThr"+intgr+"p"+decim+"_unpacked.root","READ");  

    TH1F* rate_NewLayer1_noIso_mean       = (TH1F*)f_mean->Get("rate_noCut_DiTau");
    TH1F* rate_NewLayer1_noIso_unpacked   = (TH1F*)f_unpacked->Get("rate_noCut_DiTau");
    TH1F* rate_NewLayer1_Iso_unpacked     = (TH1F*)f_unpacked->Get("rate_Iso_DiTau");
    TH1F* rate_NewLayer1_Option31_extrap_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_31_extrap");
    TH1F* rate_NewLayer1_Option22_mean = (TH1F*)f_mean->Get("rate_DiTau_Progression_22");
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


    TString fm_noIso       = std::to_string( int( ((TVectorT<float>*)f->Get("fm_noIso"))->Max() ) );
    TString fm_Option31_extrap = std::to_string( int( ((TVectorT<float>*)f->Get("fm_Option31_extrap"))->Max() ) );
    TString fm_Option22 = std::to_string( int( ((TVectorT<float>*)f->Get("fm_Option22"))->Max() ) );
    TString fm_OptionA = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionA"))->Max() ) );
    TString fm_OptionB = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionB"))->Max() ) );
    TString fm_OptionC = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionC"))->Max() ) );
    TString fm_OptionD = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionD"))->Max() ) );
    TString fm_OptionE = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionE"))->Max() ) );
    TString fm_OptionF = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionF"))->Max() ) );
    TString fm_OptionG = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionG"))->Max() ) );
    TString fm_OptionH = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionH"))->Max() ) );
    TString fm_OptionI = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionI"))->Max() ) );
    TString fm_OptionJ = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionJ"))->Max() ) );
    TString fm_OptionK = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionK"))->Max() ) );
    TString fm_OptionL = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionL"))->Max() ) );
    TString fm_OptionM = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionM"))->Max() ) );
    TString fm_OptionN = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionN"))->Max() ) );
    TString fm_OptionO = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionO"))->Max() ) );
    TString fm_OptionP = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionP"))->Max() ) );
    TString fm_OptionQ = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionQ"))->Max() ) );
    TString fm_OptionR = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionR"))->Max() ) );
    TString fm_OptionS = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionS"))->Max() ) );
    TString fm_OptionT = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionT"))->Max() ) );
    TString fm_OptionU = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionU"))->Max() ) );
    TString fm_OptionV = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionV"))->Max() ) );
    TString fm_OptionW = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionW"))->Max() ) );
    TString fm_OptionX = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionX"))->Max() ) );
    TString fm_OptionY = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionY"))->Max() ) );
    TString fm_OptionZ = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionZ"))->Max() ) );

    TString CanvasName = "Comparison_TurnOn_Run"+run_str+"_newnTT_unpacked_optimizationV6gs";
    if (fixedThr==0) CanvasName = CanvasName+"_FIXEDRATE";
    else             CanvasName = CanvasName+"_FIXEDTHR"+fixedThreshold;
    CanvasName = CanvasName+"_calibThr"+intgr+"p"+decim+"_"+tag+"_"+opt;
    TString CanvasNamePdf = CanvasName ;
    CanvasNamePdf += ".pdf";

    TCanvas c(CanvasName.Data(),CanvasName.Data(),2600,1200);
    c.SetLeftMargin(0.15);
    c.SetGrid();
    c.SetLogy();

    TPad *pad1 = new TPad("pad1", "pad1", 0., 0., 0.5, 1.);
    pad1->SetLeftMargin(0.15);
    pad1->SetTitle("");
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    //pad1->SetLogy();

    TPad *pad2 = new TPad("pad2", "pad2", 0.5, 0.3, 1., 1.);
    pad2->SetBottomMargin(0.05); // Upper and lower plot are joined
    pad2->SetLeftMargin(0.15);
    pad2->SetTitle("");
    pad2->SetGridx();         // Vertical grid
    pad2->SetGridy();         // Vertical grid
    pad2->Draw();             // Draw the upper pad: pad2
    pad2->SetLogy();

    TPad *pad3 = new TPad("pad3", "pad3", 0.5, 0., 1, 0.3);
    //pad3->SetTopMargin(0.05);
    pad3->SetLeftMargin(0.15);
    pad3->SetBottomMargin(0.30);
    pad3->SetGridx(); // vertical grid
    pad3->Draw();

    bool somethingGood = false;

    TLegend* leg = new TLegend(0.5,0.15,0.89,0.62);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);
    leg->SetHeader("Linearly scaled to 2.0E34");

    pad1->cd();
    turnon_NewLayer1_noIso_mean->SetTitle("");
    turnon_NewLayer1_noIso_mean->SetLineColor(kBlack);
    turnon_NewLayer1_noIso_mean->SetLineWidth(1);
    turnon_NewLayer1_noIso_mean->GetXaxis()->SetLimits(15,120.);
    turnon_NewLayer1_noIso_mean->GetYaxis()->SetTitle("Efficiency");
    turnon_NewLayer1_noIso_mean->GetXaxis()->SetTitle("Offline p_{T}(#tau) [GeV]");
    turnon_NewLayer1_noIso_mean->Draw();
    pad2->cd();
    rate_NewLayer1_noIso_mean->SetTitle("");
    rate_NewLayer1_noIso_mean->GetXaxis()->SetRangeUser(20.,60.);
    rate_NewLayer1_noIso_mean->GetYaxis()->SetTitle("Di-#tau rate [kHz]");
    rate_NewLayer1_noIso_mean->SetLineColor(kBlack);
    rate_NewLayer1_noIso_mean->SetLineWidth(1);
    rate_NewLayer1_noIso_mean->Draw();
    pad3->cd();
    TH1F* ratio_noIso_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
    ratio_noIso_mean->Divide(rate_NewLayer1_noIso_mean);
    ratio_noIso_mean->SetLineColor(kBlack);
    ratio_noIso_mean->GetXaxis()->SetLabelSize(0.09);
    ratio_noIso_mean->GetYaxis()->SetLabelSize(0.09);
    ratio_noIso_mean->GetYaxis()->SetTitleSize(0.09);
    ratio_noIso_mean->SetTitle("");
    ratio_noIso_mean->GetXaxis()->SetRangeUser(20.,60.);
    ratio_noIso_mean->GetYaxis()->SetRangeUser(0.,1.2);
    ratio_noIso_mean->GetYaxis()->SetTitle("iso(Option)/non-iso");
    ratio_noIso_mean->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) threshold [GeV]");
    ratio_noIso_mean->GetXaxis()->SetTitleSize(0.11);
    ratio_noIso_mean->GetYaxis()->SetTitleOffset(0.5);
    ratio_noIso_mean->Draw();
    leg->AddEntry(turnon_NewLayer1_noIso_mean,"Di-#tau no-iso, mean - fm="+fm_noIso,"L");

    pad1->cd();
    turnon_NewLayer1_Option22_mean->SetLineColor(kGray+1);
    turnon_NewLayer1_Option22_mean->SetLineWidth(1);
    turnon_NewLayer1_Option22_mean->Draw("same");
    pad2->cd();
    rate_NewLayer1_Option22_mean->SetLineColor(kGray+1);
    rate_NewLayer1_Option22_mean->SetLineWidth(1);
    rate_NewLayer1_Option22_mean->Draw("same");
    pad3->cd();
    TH1F* ratioPlot22_mean = (TH1F*)rate_NewLayer1_Option22_mean->Clone("ratioPlot");
    ratioPlot22_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlot22_mean->SetLineColor(kGray+1);
    ratioPlot22_mean->Draw("same");
    leg->AddEntry(turnon_NewLayer1_Option22_mean,"Di-#tau iso (Option 22) - fm="+fm_Option22,"L");

    pad1->cd();
    turnon_NewLayer1_Option31_extrap_mean->SetLineColor(kGray+2);
    turnon_NewLayer1_Option31_extrap_mean->SetLineWidth(1);
    turnon_NewLayer1_Option31_extrap_mean->Draw("same");
    pad2->cd();
    rate_NewLayer1_Option31_extrap_mean->SetLineColor(kGray+2);
    rate_NewLayer1_Option31_extrap_mean->SetLineWidth(1);
    rate_NewLayer1_Option31_extrap_mean->Draw("same");
    pad3->cd();
    TH1F* ratioPlot31_extrap_mean = (TH1F*)rate_NewLayer1_Option31_extrap_mean->Clone("ratioPlot");
    ratioPlot31_extrap_mean->Divide(rate_NewLayer1_noIso_mean);
    ratioPlot31_extrap_mean->SetLineColor(kGray+2);
    ratioPlot31_extrap_mean->Draw("same");
    leg->AddEntry(turnon_NewLayer1_Option31_extrap_mean,"Di-#tau iso (Option 31 extrap) - fm="+fm_Option31_extrap,"L");

    TGraphAsymmErrors* betterThan;
    if (baseline == "22") betterThan = turnon_NewLayer1_Option22_mean;
    else betterThan = turnon_NewLayer1_Option31_extrap_mean;

    turnon_NewLayer1_OptionA_mean->SetLineColor(kRed);
    turnon_NewLayer1_OptionA_mean->SetLineWidth(1);
    rate_NewLayer1_OptionA_mean->SetLineColor(kRed);
    rate_NewLayer1_OptionA_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionA_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionA_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionA_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotA_mean = (TH1F*)rate_NewLayer1_OptionA_mean->Clone("ratioPlot");
        ratioPlotA_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotA_mean->SetLineColor(kRed);
        ratioPlotA_mean->SetLineWidth(1);
        ratioPlotA_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionA_mean,"Di-#tau iso (Option A"+opt+") - fm="+fm_OptionA,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionB_mean->SetLineColor(kRed-4);
    turnon_NewLayer1_OptionB_mean->SetLineWidth(1);
    rate_NewLayer1_OptionB_mean->SetLineColor(kRed-4);
    rate_NewLayer1_OptionB_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionB_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionB_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionB_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotB_mean = (TH1F*)rate_NewLayer1_OptionB_mean->Clone("ratioPlot");
        ratioPlotB_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotB_mean->SetLineColor(kRed-4);
        ratioPlotB_mean->SetLineWidth(1);
        ratioPlotB_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionB_mean,"Di-#tau iso (Option B"+opt+") - fm="+fm_OptionB,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionC_mean->SetLineColor(kRed-7);
    turnon_NewLayer1_OptionC_mean->SetLineWidth(1);
    rate_NewLayer1_OptionC_mean->SetLineColor(kRed-7);
    rate_NewLayer1_OptionC_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionC_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionC_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionC_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotC_mean = (TH1F*)rate_NewLayer1_OptionC_mean->Clone("ratioPlot");
        ratioPlotC_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotC_mean->SetLineColor(kRed-7);
        ratioPlotC_mean->SetLineWidth(1);
        ratioPlotC_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionC_mean,"Di-#tau iso (Option C"+opt+") - fm="+fm_OptionC,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionD_mean->SetLineColor(kRed-9);
    turnon_NewLayer1_OptionD_mean->SetLineWidth(1);
    rate_NewLayer1_OptionD_mean->SetLineColor(kRed-9);
    rate_NewLayer1_OptionD_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionD_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionD_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionD_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotD_mean = (TH1F*)rate_NewLayer1_OptionD_mean->Clone("ratioPlot");
        ratioPlotD_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotD_mean->SetLineColor(kRed-9);
        ratioPlotD_mean->SetLineWidth(1);
        ratioPlotD_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionD_mean,"Di-#tau iso (Option D"+opt+") - fm="+fm_OptionD,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionE_mean->SetLineColor(kRed-10);
    turnon_NewLayer1_OptionE_mean->SetLineWidth(1);
    rate_NewLayer1_OptionE_mean->SetLineColor(kRed-10);
    rate_NewLayer1_OptionE_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionE_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionE_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionE_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotE_mean = (TH1F*)rate_NewLayer1_OptionE_mean->Clone("ratioPlot");
        ratioPlotE_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotE_mean->SetLineColor(kRed-10);
        ratioPlotE_mean->SetLineWidth(1);
        ratioPlotE_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionE_mean,"Di-#tau iso (Option E"+opt+") - fm="+fm_OptionE,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionF_mean->SetLineColor(kMagenta);
    turnon_NewLayer1_OptionF_mean->SetLineWidth(1);
    rate_NewLayer1_OptionF_mean->SetLineColor(kMagenta);
    rate_NewLayer1_OptionF_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionF_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionF_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionF_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotF_mean = (TH1F*)rate_NewLayer1_OptionF_mean->Clone("ratioPlot");
        ratioPlotF_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotF_mean->SetLineColor(kMagenta);
        ratioPlotF_mean->SetLineWidth(1);
        ratioPlotF_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionF_mean,"Di-#tau iso (Option F"+opt+") - fm="+fm_OptionF,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionG_mean->SetLineColor(kMagenta-4);
    turnon_NewLayer1_OptionG_mean->SetLineWidth(1);
    rate_NewLayer1_OptionG_mean->SetLineColor(kMagenta-4);
    rate_NewLayer1_OptionG_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionG_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionG_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionG_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotG_mean = (TH1F*)rate_NewLayer1_OptionG_mean->Clone("ratioPlot");
        ratioPlotG_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotG_mean->SetLineColor(kMagenta-4);
        ratioPlotG_mean->SetLineWidth(1);
        ratioPlotG_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionG_mean,"Di-#tau iso (Option G"+opt+") - fm="+fm_OptionG,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionH_mean->SetLineColor(kMagenta-7);
    turnon_NewLayer1_OptionH_mean->SetLineWidth(1);
    rate_NewLayer1_OptionH_mean->SetLineColor(kMagenta-7);
    rate_NewLayer1_OptionH_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionH_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionH_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionH_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotH_mean = (TH1F*)rate_NewLayer1_OptionH_mean->Clone("ratioPlot");
        ratioPlotH_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotH_mean->SetLineColor(kMagenta-7);
        ratioPlotH_mean->SetLineWidth(1);
        ratioPlotH_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionH_mean,"Di-#tau iso (Option H"+opt+") - fm="+fm_OptionH,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionI_mean->SetLineColor(kMagenta-9);
    turnon_NewLayer1_OptionI_mean->SetLineWidth(1);
    rate_NewLayer1_OptionI_mean->SetLineColor(kMagenta-9);
    rate_NewLayer1_OptionI_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionI_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionI_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionI_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotI_mean = (TH1F*)rate_NewLayer1_OptionI_mean->Clone("ratioPlot");
        ratioPlotI_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotI_mean->SetLineColor(kMagenta-9);
        ratioPlotI_mean->SetLineWidth(1);
        ratioPlotI_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionI_mean,"Di-#tau iso (Option I"+opt+") - fm="+fm_OptionI,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionJ_mean->SetLineColor(kMagenta-10);
    turnon_NewLayer1_OptionJ_mean->SetLineWidth(1);
    rate_NewLayer1_OptionJ_mean->SetLineColor(kMagenta-10);
    rate_NewLayer1_OptionJ_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionJ_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionJ_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionJ_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotJ_mean = (TH1F*)rate_NewLayer1_OptionJ_mean->Clone("ratioPlot");
        ratioPlotJ_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotJ_mean->SetLineColor(kMagenta-10);
        ratioPlotJ_mean->SetLineWidth(1);
        ratioPlotJ_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionJ_mean,"Di-#tau iso (Option J"+opt+") - fm="+fm_OptionJ,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionK_mean->SetLineColor(kBlue);
    turnon_NewLayer1_OptionK_mean->SetLineWidth(1);
    rate_NewLayer1_OptionK_mean->SetLineColor(kBlue);
    rate_NewLayer1_OptionK_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionK_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionK_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionK_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotK_mean = (TH1F*)rate_NewLayer1_OptionA_mean->Clone("ratioPlot");
        ratioPlotK_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotK_mean->SetLineColor(kBlue);
        ratioPlotK_mean->SetLineWidth(1);
        ratioPlotK_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionK_mean,"Di-#tau iso (Option K"+opt+") - fm="+fm_OptionK,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionL_mean->SetLineColor(kBlue);
    turnon_NewLayer1_OptionL_mean->SetLineWidth(1);
    rate_NewLayer1_OptionL_mean->SetLineColor(kBlue);
    rate_NewLayer1_OptionL_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionL_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionL_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionL_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotL_mean = (TH1F*)rate_NewLayer1_OptionL_mean->Clone("ratioPlot");
        ratioPlotL_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotL_mean->SetLineColor(kBlue);
        ratioPlotL_mean->SetLineWidth(1);
        ratioPlotL_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionL_mean,"Di-#tau iso (Option L"+opt+") - fm="+fm_OptionL,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionM_mean->SetLineColor(kBlue-7);
    turnon_NewLayer1_OptionM_mean->SetLineWidth(1);
    rate_NewLayer1_OptionM_mean->SetLineColor(kBlue-7);
    rate_NewLayer1_OptionM_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionM_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionM_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionM_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotM_mean = (TH1F*)rate_NewLayer1_OptionM_mean->Clone("ratioPlot");
        ratioPlotM_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotM_mean->SetLineColor(kBlue-7);
        ratioPlotM_mean->SetLineWidth(1);
        ratioPlotM_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionM_mean,"Di-#tau iso (Option M"+opt+") - fm="+fm_OptionM,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionN_mean->SetLineColor(kBlue-9);
    turnon_NewLayer1_OptionN_mean->SetLineWidth(1);
    rate_NewLayer1_OptionN_mean->SetLineColor(kBlue-9);
    rate_NewLayer1_OptionN_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionN_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionN_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionN_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotN_mean = (TH1F*)rate_NewLayer1_OptionN_mean->Clone("ratioPlot");
        ratioPlotN_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotN_mean->SetLineColor(kBlue-9);
        ratioPlotN_mean->SetLineWidth(1);
        ratioPlotN_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionN_mean,"Di-#tau iso (Option N"+opt+") - fm="+fm_OptionN,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionO_mean->SetLineColor(kBlue-10);
    turnon_NewLayer1_OptionO_mean->SetLineWidth(1);
    rate_NewLayer1_OptionO_mean->SetLineColor(kBlue-10);
    rate_NewLayer1_OptionO_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionO_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionO_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionO_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotO_mean = (TH1F*)rate_NewLayer1_OptionO_mean->Clone("ratioPlot");
        ratioPlotO_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotO_mean->SetLineColor(kBlue-10);
        ratioPlotO_mean->SetLineWidth(1);
        ratioPlotO_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionO_mean,"Di-#tau iso (Option O"+opt+") - fm="+fm_OptionO,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionP_mean->SetLineColor(kCyan);
    turnon_NewLayer1_OptionP_mean->SetLineWidth(1);
    rate_NewLayer1_OptionP_mean->SetLineColor(kCyan);
    rate_NewLayer1_OptionP_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionP_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionP_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionP_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotP_mean = (TH1F*)rate_NewLayer1_OptionP_mean->Clone("ratioPlot");
        ratioPlotP_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotP_mean->SetLineColor(kCyan);
        ratioPlotP_mean->SetLineWidth(1);
        ratioPlotP_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionP_mean,"Di-#tau iso (Option P"+opt+") - fm="+fm_OptionP,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionQ_mean->SetLineColor(kCyan-4);
    turnon_NewLayer1_OptionQ_mean->SetLineWidth(1);
    rate_NewLayer1_OptionQ_mean->SetLineColor(kCyan-4);
    rate_NewLayer1_OptionQ_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionQ_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionQ_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionQ_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotQ_mean = (TH1F*)rate_NewLayer1_OptionQ_mean->Clone("ratioPlot");
        ratioPlotQ_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotQ_mean->SetLineColor(kCyan-4);
        ratioPlotQ_mean->SetLineWidth(1);
        ratioPlotQ_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionQ_mean,"Di-#tau iso (Option Q"+opt+") - fm="+fm_OptionQ,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionR_mean->SetLineColor(kCyan-7);
    turnon_NewLayer1_OptionR_mean->SetLineWidth(1);
    rate_NewLayer1_OptionR_mean->SetLineColor(kCyan-7);
    rate_NewLayer1_OptionR_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionR_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionR_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionR_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotR_mean = (TH1F*)rate_NewLayer1_OptionR_mean->Clone("ratioPlot");
        ratioPlotR_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotR_mean->SetLineColor(kCyan-7);
        ratioPlotR_mean->SetLineWidth(1);
        ratioPlotR_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionR_mean,"Di-#tau iso (Option R"+opt+") - fm="+fm_OptionR,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionS_mean->SetLineColor(kCyan-9);
    turnon_NewLayer1_OptionS_mean->SetLineWidth(1);
    rate_NewLayer1_OptionS_mean->SetLineColor(kCyan-9);
    rate_NewLayer1_OptionS_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionS_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionS_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionS_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotS_mean = (TH1F*)rate_NewLayer1_OptionS_mean->Clone("ratioPlot");
        ratioPlotS_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotS_mean->SetLineColor(kCyan-9);
        ratioPlotS_mean->SetLineWidth(1);
        ratioPlotS_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionS_mean,"Di-#tau iso (Option S"+opt+") - fm="+fm_OptionS,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionT_mean->SetLineColor(kCyan-10);
    turnon_NewLayer1_OptionT_mean->SetLineWidth(1);
    rate_NewLayer1_OptionT_mean->SetLineColor(kCyan-10);
    rate_NewLayer1_OptionT_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionT_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionT_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionT_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotT_mean = (TH1F*)rate_NewLayer1_OptionT_mean->Clone("ratioPlot");
        ratioPlotT_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotT_mean->SetLineColor(kCyan-10);
        ratioPlotT_mean->SetLineWidth(1);
        ratioPlotT_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionT_mean,"Di-#tau iso (Option T"+opt+") - fm="+fm_OptionT,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionU_mean->SetLineColor(kGreen);
    turnon_NewLayer1_OptionU_mean->SetLineWidth(1);
    rate_NewLayer1_OptionU_mean->SetLineColor(kGreen);
    rate_NewLayer1_OptionU_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionU_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionU_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionU_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotU_mean = (TH1F*)rate_NewLayer1_OptionU_mean->Clone("ratioPlot");
        ratioPlotU_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotU_mean->SetLineColor(kGreen);
        ratioPlotU_mean->SetLineWidth(1);
        ratioPlotU_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionU_mean,"Di-#tau iso (Option U"+opt+") - fm="+fm_OptionU,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionV_mean->SetLineColor(kGreen-4);
    turnon_NewLayer1_OptionV_mean->SetLineWidth(1);
    rate_NewLayer1_OptionV_mean->SetLineColor(kGreen-4);
    rate_NewLayer1_OptionV_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionV_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionV_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionV_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotV_mean = (TH1F*)rate_NewLayer1_OptionV_mean->Clone("ratioPlot");
        ratioPlotV_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotV_mean->SetLineColor(kGreen-4);
        ratioPlotV_mean->SetLineWidth(1);
        ratioPlotV_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionV_mean,"Di-#tau iso (Option V"+opt+") - fm="+fm_OptionV,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionW_mean->SetLineColor(kGreen-7);
    turnon_NewLayer1_OptionW_mean->SetLineWidth(1);
    rate_NewLayer1_OptionW_mean->SetLineColor(kGreen-7);
    rate_NewLayer1_OptionW_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionW_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionW_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionW_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotW_mean = (TH1F*)rate_NewLayer1_OptionW_mean->Clone("ratioPlot");
        ratioPlotW_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotW_mean->SetLineColor(kGreen-7);
        ratioPlotW_mean->SetLineWidth(1);
        ratioPlotW_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionW_mean,"Di-#tau iso (Option W"+opt+") - fm="+fm_OptionW,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionX_mean->SetLineColor(kGreen-9);
    turnon_NewLayer1_OptionX_mean->SetLineWidth(1);
    rate_NewLayer1_OptionX_mean->SetLineColor(kGreen-9);
    rate_NewLayer1_OptionX_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionX_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionX_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionX_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotX_mean = (TH1F*)rate_NewLayer1_OptionX_mean->Clone("ratioPlot");
        ratioPlotX_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotX_mean->SetLineColor(kGreen-9);
        ratioPlotX_mean->SetLineWidth(1);
        ratioPlotX_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionX_mean,"Di-#tau iso (Option X"+opt+") - fm="+fm_OptionX,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionY_mean->SetLineColor(kGreen-10);
    turnon_NewLayer1_OptionY_mean->SetLineWidth(1);
    rate_NewLayer1_OptionY_mean->SetLineColor(kGreen-10);
    rate_NewLayer1_OptionY_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionY_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionY_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionY_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotY_mean = (TH1F*)rate_NewLayer1_OptionY_mean->Clone("ratioPlot");
        ratioPlotY_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotY_mean->SetLineColor(kGreen-10);
        ratioPlotY_mean->SetLineWidth(1);
        ratioPlotY_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionY_mean,"Di-#tau iso (Option Y"+opt+") - fm="+fm_OptionY,"L");
        somethingGood = true;
    }

    turnon_NewLayer1_OptionZ_mean->SetLineColor(kYellow);
    turnon_NewLayer1_OptionZ_mean->SetLineWidth(1);
    rate_NewLayer1_OptionZ_mean->SetLineColor(kYellow);
    rate_NewLayer1_OptionZ_mean->SetLineWidth(1);
    if ( isGoodTurnON(betterThan, turnon_NewLayer1_OptionZ_mean) ) 
    { 
        pad1->cd();
        turnon_NewLayer1_OptionZ_mean->Draw("same");
        
        pad2->cd();
        rate_NewLayer1_OptionZ_mean->Draw("same");

        pad3->cd();
        TH1F* ratioPlotZ_mean = (TH1F*)rate_NewLayer1_OptionZ_mean->Clone("ratioPlot");
        ratioPlotZ_mean->Divide(rate_NewLayer1_noIso_mean);
        ratioPlotZ_mean->SetLineColor(kYellow);
        ratioPlotZ_mean->SetLineWidth(1);
        ratioPlotZ_mean->Draw("same");

        leg->AddEntry(turnon_NewLayer1_OptionZ_mean,"Di-#tau iso (Option Z"+opt+") - fm="+fm_OptionZ,"L");
        somethingGood = true;
    }


    TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
    texl->AddText("CMS Internal, #sqrt{s}=13 TeV, Run #"+run_str+" (2018)");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    pad1->cd();
    texl->Draw("same");    

    pad1->cd();
    leg->Draw("same");

    TString PDFs = "plotsGoodTurnons_beeterThan"+baseline+"_Run"+run_str+"/";
    if (somethingGood) c.SaveAs(PDFs+CanvasNamePdf.Data());

    f->Close();

}
