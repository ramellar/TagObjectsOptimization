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

void compare(int run, TString tag, TString opt="0", int fixedThr = 0, float calibThr = 1.7) {
    TString run_str = to_string(run);

    TString fixedThreshold = to_string(fixedThr);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    gStyle->SetOptStat(000000);

    TFile* f;
    if (fixedThr==0) f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_01_28_optimizationV6_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE_Run"+run_str+"_gs_"+tag+"_"+opt+"_2022_01_28.root", "READ");
    else             f = new TFile("/data_CMS/cms/motta/Run3preparation/2022_01_28_optimizationV6_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDTHR"+fixedThreshold+"_Run"+run_str+"_gs_"+tag+"_"+opt+"_2022_01_28.root", "READ");
    

    TGraphAsymmErrors* turnOn_NewLayer1_noIso_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_noIso_by_pt");

    TGraphAsymmErrors* turnOn_NewLayer1_OptionA_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionA"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionB_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionB"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionC_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionC"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionD_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionD"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionE_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionE"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionF_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionF"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionG_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionG"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionH_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionH"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionI_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionI"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionJ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionJ"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionK_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionK"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionL_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionL"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionM_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionM"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionN_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionN"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionO_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionO"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionP_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionP"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionQ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionQ"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionR_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionR"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionS_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionS"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionT_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionT"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionU_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionU"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionV_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionV"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionW_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionW"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionX_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionX"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionY_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionY"+opt+"_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_OptionZ_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_OptionZ"+opt+"_by_pt");

    TGraphAsymmErrors* turnOn_NewLayer1_Option22_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option22_by_pt");
    TGraphAsymmErrors* turnOn_NewLayer1_Option31_extrap_mean = (TGraphAsymmErrors*)f->Get("divide_pt_pass_Option31_extrap_by_pt");


    TString fm_noIso       = std::to_string( int( ((TVectorT<float>*)f->Get("fm_noIso"))[0][1] ) );
    TString fm_Option31_extrap = std::to_string( int( ((TVectorT<float>*)f->Get("fm_Option31_extrap"))[0][1] ) );
    TString fm_Option22 = std::to_string( int( ((TVectorT<float>*)f->Get("fm_Option22"))[0][1] ) );
    TString fm_OptionA = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionA"))[0][1] ) );
    TString fm_OptionB = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionB"))[0][1] ) );
    TString fm_OptionC = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionC"))[0][1] ) );
    TString fm_OptionD = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionD"))[0][1] ) );
    TString fm_OptionE = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionE"))[0][1] ) );
    TString fm_OptionF = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionF"))[0][1] ) );
    TString fm_OptionG = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionG"))[0][1] ) );
    TString fm_OptionH = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionH"))[0][1] ) );
    TString fm_OptionI = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionI"))[0][1] ) );
    TString fm_OptionJ = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionJ"))[0][1] ) );
    TString fm_OptionK = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionK"))[0][1] ) );
    TString fm_OptionL = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionL"))[0][1] ) );
    TString fm_OptionM = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionM"))[0][1] ) );
    TString fm_OptionN = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionN"))[0][1] ) );
    TString fm_OptionO = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionO"))[0][1] ) );
    TString fm_OptionP = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionP"))[0][1] ) );
    TString fm_OptionQ = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionQ"))[0][1] ) );
    TString fm_OptionR = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionR"))[0][1] ) );
    TString fm_OptionS = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionS"))[0][1] ) );
    TString fm_OptionT = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionT"))[0][1] ) );
    TString fm_OptionU = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionU"))[0][1] ) );
    TString fm_OptionV = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionV"))[0][1] ) );
    TString fm_OptionW = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionW"))[0][1] ) );
    TString fm_OptionX = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionX"))[0][1] ) );
    TString fm_OptionY = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionY"))[0][1] ) );
    TString fm_OptionZ = std::to_string( int( ((TVectorT<float>*)f->Get("fm_OptionZ"))[0][1] ) );

    TString thr_noIso       = std::to_string( int( ((TVectorT<float>*)f->Get("thr_noIso"))[0][0] ) +1 );
    TString thr_Option31_extrap = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option31_extrap"))[0][0] ) +1 );
    TString thr_Option22 = std::to_string( int( ((TVectorT<float>*)f->Get("thr_Option22"))[0][0] ) +1 );
    TString thr_OptionA = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionA"))[0][0] ) +1 );
    TString thr_OptionB = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionB"))[0][0] ) +1 );
    TString thr_OptionC = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionC"))[0][0] ) +1 );
    TString thr_OptionD = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionD"))[0][0] ) +1 );
    TString thr_OptionE = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionE"))[0][0] ) +1 );
    TString thr_OptionF = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionF"))[0][0] ) +1 );
    TString thr_OptionG = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionG"))[0][0] ) +1 );
    TString thr_OptionH = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionH"))[0][0] ) +1 );
    TString thr_OptionI = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionI"))[0][0] ) +1 );
    TString thr_OptionJ = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionJ"))[0][0] ) +1 );
    TString thr_OptionK = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionK"))[0][0] ) +1 );
    TString thr_OptionL = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionL"))[0][0] ) +1 );
    TString thr_OptionM = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionM"))[0][0] ) +1 );
    TString thr_OptionN = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionN"))[0][0] ) +1 );
    TString thr_OptionO = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionO"))[0][0] ) +1 );
    TString thr_OptionP = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionP"))[0][0] ) +1 );
    TString thr_OptionQ = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionQ"))[0][0] ) +1 );
    TString thr_OptionR = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionR"))[0][0] ) +1 );
    TString thr_OptionS = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionS"))[0][0] ) +1 );
    TString thr_OptionT = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionT"))[0][0] ) +1 );
    TString thr_OptionU = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionU"))[0][0] ) +1 );
    TString thr_OptionV = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionV"))[0][0] ) +1 );
    TString thr_OptionW = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionW"))[0][0] ) +1 );
    TString thr_OptionX = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionX"))[0][0] ) +1 );
    TString thr_OptionY = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionY"))[0][0] ) +1 );
    TString thr_OptionZ = std::to_string( int( ((TVectorT<float>*)f->Get("thr_OptionZ"))[0][0] ) +1 );


    TString CanvasName = "Comparison_TurnOn_Run"+run_str+"_newnTT_unpacked_optimizationV6gs";
    if (fixedThr==0) CanvasName = CanvasName+"_FIXEDRATE";
    else             CanvasName = CanvasName+"_FIXEDTHR"+fixedThreshold;
    CanvasName = CanvasName+"_calibThr"+intgr+"p"+decim+"_"+tag+"_"+opt;
    TString CanvasNamePdf = CanvasName ;
    CanvasNamePdf += ".pdf";

    TCanvas c(CanvasName.Data(),CanvasName.Data(),650,600);
    c.SetLeftMargin(0.15);
    c.SetGrid();
    c.SetLogy();

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0., 1, 1.0);
    //pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
    pad1->SetLeftMargin(0.15);
    pad1->SetGridx();         // Vertical grid
    pad1->SetGridy();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    //pad1->SetLogy();

    turnOn_NewLayer1_noIso_mean->SetLineColor(kBlack);
    turnOn_NewLayer1_noIso_mean->SetLineWidth(1);
    turnOn_NewLayer1_noIso_mean->GetXaxis()->SetLimits(15,120.);
    turnOn_NewLayer1_noIso_mean->SetTitle("");
    turnOn_NewLayer1_noIso_mean->GetYaxis()->SetTitle("Efficiency");
    turnOn_NewLayer1_noIso_mean->GetXaxis()->SetTitle("Offline p_{T}(#tau) [GeV]");
    turnOn_NewLayer1_noIso_mean->Draw();

    turnOn_NewLayer1_OptionA_mean->SetLineColor(kRed);
    turnOn_NewLayer1_OptionA_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionA_mean->Draw("same");

    turnOn_NewLayer1_OptionB_mean->SetLineColor(kRed-4);
    turnOn_NewLayer1_OptionB_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionB_mean->Draw("same");

    turnOn_NewLayer1_OptionC_mean->SetLineColor(kRed-7);
    turnOn_NewLayer1_OptionC_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionC_mean->Draw("same");

    turnOn_NewLayer1_OptionD_mean->SetLineColor(kRed-9);
    turnOn_NewLayer1_OptionD_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionD_mean->Draw("same");

    turnOn_NewLayer1_OptionE_mean->SetLineColor(kRed-10);
    turnOn_NewLayer1_OptionE_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionE_mean->Draw("same");

    turnOn_NewLayer1_OptionF_mean->SetLineColor(kMagenta);
    turnOn_NewLayer1_OptionF_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionF_mean->Draw("same");

    turnOn_NewLayer1_OptionG_mean->SetLineColor(kMagenta-4);
    turnOn_NewLayer1_OptionG_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionG_mean->Draw("same");

    turnOn_NewLayer1_OptionH_mean->SetLineColor(kMagenta-7);
    turnOn_NewLayer1_OptionH_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionH_mean->Draw("same");

    turnOn_NewLayer1_OptionI_mean->SetLineColor(kMagenta-9);
    turnOn_NewLayer1_OptionI_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionI_mean->Draw("same");

    turnOn_NewLayer1_OptionJ_mean->SetLineColor(kMagenta-10);
    turnOn_NewLayer1_OptionJ_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionJ_mean->Draw("same");

    turnOn_NewLayer1_OptionK_mean->SetLineColor(kBlue);
    turnOn_NewLayer1_OptionK_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionK_mean->Draw("same");

    turnOn_NewLayer1_OptionL_mean->SetLineColor(kBlue);
    turnOn_NewLayer1_OptionL_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionL_mean->Draw("same");

    turnOn_NewLayer1_OptionM_mean->SetLineColor(kBlue-7);
    turnOn_NewLayer1_OptionM_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionM_mean->Draw("same");

    turnOn_NewLayer1_OptionN_mean->SetLineColor(kBlue-9);
    turnOn_NewLayer1_OptionN_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionN_mean->Draw("same");

    turnOn_NewLayer1_OptionO_mean->SetLineColor(kBlue-10);
    turnOn_NewLayer1_OptionO_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionO_mean->Draw("same");

    turnOn_NewLayer1_OptionP_mean->SetLineColor(kCyan);
    turnOn_NewLayer1_OptionP_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionP_mean->Draw("same");

    turnOn_NewLayer1_OptionQ_mean->SetLineColor(kCyan-4);
    turnOn_NewLayer1_OptionQ_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionQ_mean->Draw("same");

    turnOn_NewLayer1_OptionR_mean->SetLineColor(kCyan-7);
    turnOn_NewLayer1_OptionR_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionR_mean->Draw("same");

    turnOn_NewLayer1_OptionS_mean->SetLineColor(kCyan-9);
    turnOn_NewLayer1_OptionS_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionS_mean->Draw("same");

    turnOn_NewLayer1_OptionT_mean->SetLineColor(kCyan-10);
    turnOn_NewLayer1_OptionT_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionT_mean->Draw("same");

    turnOn_NewLayer1_OptionU_mean->SetLineColor(kGreen);
    turnOn_NewLayer1_OptionU_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionU_mean->Draw("same");

    turnOn_NewLayer1_OptionV_mean->SetLineColor(kGreen-4);
    turnOn_NewLayer1_OptionV_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionV_mean->Draw("same");

    turnOn_NewLayer1_OptionW_mean->SetLineColor(kGreen-7);
    turnOn_NewLayer1_OptionW_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionW_mean->Draw("same");

    turnOn_NewLayer1_OptionX_mean->SetLineColor(kGreen-9);
    turnOn_NewLayer1_OptionX_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionX_mean->Draw("same");

    turnOn_NewLayer1_OptionY_mean->SetLineColor(kGreen-10);
    turnOn_NewLayer1_OptionY_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionY_mean->Draw("same");

    turnOn_NewLayer1_OptionZ_mean->SetLineColor(kYellow);
    turnOn_NewLayer1_OptionZ_mean->SetLineWidth(1);
    turnOn_NewLayer1_OptionZ_mean->Draw("same");

    turnOn_NewLayer1_Option22_mean->SetLineColor(kGray+1);
    turnOn_NewLayer1_Option22_mean->SetLineWidth(1);
    turnOn_NewLayer1_Option22_mean->Draw("same");

    turnOn_NewLayer1_Option31_extrap_mean->SetLineColor(kGray+2);
    turnOn_NewLayer1_Option31_extrap_mean->SetLineWidth(1);
    turnOn_NewLayer1_Option31_extrap_mean->Draw("same");

    TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
    texl->AddText("CMS Internal, #sqrt{s}=13 TeV, Run #"+run_str+" (2018)");
    texl->SetTextSize(0.04);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    texl->Draw("same");

    TLegend* leg = new TLegend(0.5,0.15,0.89,0.62);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);
    leg->SetHeader("Linearly scaled to 2.0E34");

    leg->AddEntry(turnOn_NewLayer1_noIso_mean,"Di-#tau no-iso, mean - fm="+fm_noIso,"L");

    leg->AddEntry(turnOn_NewLayer1_OptionA_mean,"Di-#tau iso (Option A"+opt+") - FM="+fm_OptionA+" - THR="+thr_OptionA,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionB_mean,"Di-#tau iso (Option B"+opt+") - FM="+fm_OptionB+" - THR="+thr_OptionB,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionC_mean,"Di-#tau iso (Option C"+opt+") - FM="+fm_OptionC+" - THR="+thr_OptionC,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionD_mean,"Di-#tau iso (Option D"+opt+") - FM="+fm_OptionD+" - THR="+thr_OptionD,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionE_mean,"Di-#tau iso (Option E"+opt+") - FM="+fm_OptionE+" - THR="+thr_OptionE,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionF_mean,"Di-#tau iso (Option F"+opt+") - FM="+fm_OptionF+" - THR="+thr_OptionF,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionG_mean,"Di-#tau iso (Option G"+opt+") - FM="+fm_OptionG+" - THR="+thr_OptionG,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionH_mean,"Di-#tau iso (Option H"+opt+") - FM="+fm_OptionH+" - THR="+thr_OptionH,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionI_mean,"Di-#tau iso (Option I"+opt+") - FM="+fm_OptionI+" - THR="+thr_OptionI,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionJ_mean,"Di-#tau iso (Option J"+opt+") - FM="+fm_OptionJ+" - THR="+thr_OptionJ,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionK_mean,"Di-#tau iso (Option K"+opt+") - FM="+fm_OptionK+" - THR="+thr_OptionK,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionL_mean,"Di-#tau iso (Option L"+opt+") - FM="+fm_OptionL+" - THR="+thr_OptionL,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionM_mean,"Di-#tau iso (Option M"+opt+") - FM="+fm_OptionM+" - THR="+thr_OptionM,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionN_mean,"Di-#tau iso (Option N"+opt+") - FM="+fm_OptionN+" - THR="+thr_OptionN,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionO_mean,"Di-#tau iso (Option O"+opt+") - FM="+fm_OptionO+" - THR="+thr_OptionO,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionP_mean,"Di-#tau iso (Option P"+opt+") - FM="+fm_OptionP+" - THR="+thr_OptionP,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionQ_mean,"Di-#tau iso (Option Q"+opt+") - FM="+fm_OptionQ+" - THR="+thr_OptionQ,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionR_mean,"Di-#tau iso (Option R"+opt+") - FM="+fm_OptionR+" - THR="+thr_OptionR,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionS_mean,"Di-#tau iso (Option S"+opt+") - FM="+fm_OptionS+" - THR="+thr_OptionS,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionT_mean,"Di-#tau iso (Option T"+opt+") - FM="+fm_OptionT+" - THR="+thr_OptionT,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionU_mean,"Di-#tau iso (Option U"+opt+") - FM="+fm_OptionU+" - THR="+thr_OptionU,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionV_mean,"Di-#tau iso (Option V"+opt+") - FM="+fm_OptionV+" - THR="+thr_OptionV,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionW_mean,"Di-#tau iso (Option W"+opt+") - FM="+fm_OptionW+" - THR="+thr_OptionW,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionX_mean,"Di-#tau iso (Option X"+opt+") - FM="+fm_OptionX+" - THR="+thr_OptionX,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionY_mean,"Di-#tau iso (Option Y"+opt+") - FM="+fm_OptionY+" - THR="+thr_OptionY,"L");
    leg->AddEntry(turnOn_NewLayer1_OptionZ_mean,"Di-#tau iso (Option Z"+opt+") - FM="+fm_OptionZ+" - THR="+thr_OptionZ,"L");

    leg->AddEntry(turnOn_NewLayer1_Option22_mean,"Di-#tau iso (Option 22) - fm="+fm_Option22,"L");
    leg->AddEntry(turnOn_NewLayer1_Option31_extrap_mean,"Di-#tau iso (Option 31 extrap) - fm="+fm_Option31_extrap,"L");

    leg->Draw("same");

    TString PDFs = "PDFs/";
    c.SaveAs(PDFs+CanvasNamePdf.Data());

    f->Close();

}
