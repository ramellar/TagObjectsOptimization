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

void compare(int run, float calibThr = 1.7) {
  TString run_str = to_string(run);

  TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
  TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

  bool Draw_Options = kTRUE;
  
  gStyle->SetOptStat(000000);
  //TFile f_mode("./Trees_modeparam/histos_rate_Run305310_92X_mode.root","READ");
  TFile f_mean("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias_Run"+run_str+"_optimizationV10_calibThr"+intgr+"p"+decim+".root","READ");
  TFile f_unpacked("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias_Run"+run_str+"_optimizationV10_calibThr"+intgr+"p"+decim+"_unpacked.root","READ");  

  //TH1F* rate_NewLayer1_noIso_mode   = (TH1F*)f_mode.Get("rate_noCut_EGTau");    
  TH1F* rate_NewLayer1_noIso_mean   = (TH1F*)f_mean.Get("rate_noCut_DiTau");
  //TH1F* rate_NewLayer1_Option22_mode = (TH1F*)f_mode.Get("rate_EGTau_Progression_22");
  TH1F* rate_NewLayer1_Option1_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_1");
  TH1F* rate_NewLayer1_Option2_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_2");
  TH1F* rate_NewLayer1_Option3_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_3");
  TH1F* rate_NewLayer1_Option4_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_4");
  TH1F* rate_NewLayer1_Option5_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_5");
  TH1F* rate_NewLayer1_Option6_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_6");
  TH1F* rate_NewLayer1_Option7_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_7");
  TH1F* rate_NewLayer1_Option8_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_8");
  TH1F* rate_NewLayer1_Option9_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_9");
  TH1F* rate_NewLayer1_Option10_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_10");
  TH1F* rate_NewLayer1_Option11_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_11");
  TH1F* rate_NewLayer1_Option12_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_12");
  TH1F* rate_NewLayer1_Option13_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_13");
  TH1F* rate_NewLayer1_Option14_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_14");
  TH1F* rate_NewLayer1_Option15_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_15");
  TH1F* rate_NewLayer1_Option16_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_16");
  TH1F* rate_NewLayer1_Option17_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_17");
  TH1F* rate_NewLayer1_Option18_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_18");
  TH1F* rate_NewLayer1_Option19_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_19");
  TH1F* rate_NewLayer1_Option20_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_20");
  TH1F* rate_NewLayer1_Option21_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_21");
  TH1F* rate_NewLayer1_Option22_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_22");
  TH1F* rate_NewLayer1_Option23_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_23");
  TH1F* rate_NewLayer1_Option24_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_24");
  TH1F* rate_NewLayer1_Option25_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_25");
  TH1F* rate_NewLayer1_Option26_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_26");
  TH1F* rate_NewLayer1_Option27_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_27");
  TH1F* rate_NewLayer1_Option28_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_28");
  TH1F* rate_NewLayer1_Option29_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_29");
  TH1F* rate_NewLayer1_Option30_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_30");
  TH1F* rate_NewLayer1_Option31_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_31");
  TH1F* rate_NewLayer1_Option31_extrap_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_31_extrap");
  TH1F* rate_NewLayer1_noIso_unpacked   = (TH1F*)f_unpacked.Get("rate_noCut_DiTau");
  TH1F* rate_NewLayer1_Iso_unpacked     = (TH1F*)f_unpacked.Get("rate_Iso_DiTau");

  //TString CanvasName = "./Trees/Comparison_Rate_DiTau_Iso_Run305310";
  TString CanvasName = "Comparison_Rate_ZeroBias_Run"+run_str+"_newnTT_unpacked_optimizationV10_calibThr"+intgr+"p"+decim;
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
  //rate_NewLayer1_Option1_mean->GetYaxis()->SetRangeUser(0.5,8.5);
  rate_NewLayer1_noIso_unpacked->GetYaxis()->SetTitle("Di-#tau rate [kHz]");
  //rate_NewLayer1_noIso_mean->GetYaxis()->SetRangeUser(20.,30.);  
  rate_NewLayer1_noIso_unpacked->SetLineColor(kBlack);
  rate_NewLayer1_noIso_unpacked->SetLineWidth(2);
//  rate_NewLayer1_noIso_unpacked->SetLineStyle(2);
  rate_NewLayer1_noIso_unpacked->Draw();

  //rate_NewLayer1_noIso_mode->SetLineWidth(3);
  //rate_NewLayer1_noIso->SetLineStyle(2);
  //rate_NewLayer1_noIso_mode->SetLineColor(kRed-9);
  //rate_NewLayer1_noIso_mode->Draw("same");
  
  //rate_NewLayer1_noIso_mean->SetLineWidth(3);
  //rate_NewLayer1_noIso->SetLineStyle(2);
  //rate_NewLayer1_noIso_mean->SetLineColor(kBlack);
  //rate_NewLayer1_noIso_mean->Draw();
  
  rate_NewLayer1_Iso_unpacked->SetLineColor(kRed);
  rate_NewLayer1_Iso_unpacked->SetLineWidth(2);
//  rate_NewLayer1_Iso_unpacked->SetLineStyle(9);
  rate_NewLayer1_Iso_unpacked->Draw("same");  

  //rate_NewLayer1_Option22_mode->SetLineColor(kRed+1);
  //rate_NewLayer1_Option22_mode->SetLineWidth(3);
  //rate_NewLayer1_Option22->SetLineStyle(9);
  //rate_NewLayer1_Option22_mode->Draw("same");

  rate_NewLayer1_Option1_mean->SetLineColor(kYellow-7);
  rate_NewLayer1_Option1_mean->SetLineWidth(2);
  //rate_NewLayer1_Option1_mean->Draw();

  rate_NewLayer1_Option2_mean->SetLineColor(kYellow+1);
  rate_NewLayer1_Option2_mean->SetLineWidth(2);
  //rate_NewLayer1_Option2_mean->Draw("same");

  rate_NewLayer1_Option3_mean->SetLineColor(kYellow+3);
  rate_NewLayer1_Option3_mean->SetLineWidth(2);
  //rate_NewLayer1_Option3_mean->Draw("same");

  rate_NewLayer1_Option4_mean->SetLineColor(kGreen-7);
  rate_NewLayer1_Option4_mean->SetLineWidth(2);
  //rate_NewLayer1_Option4_mean->Draw("same");

  rate_NewLayer1_Option5_mean->SetLineColor(kGreen+1);
  rate_NewLayer1_Option5_mean->SetLineWidth(2);
  //rate_NewLayer1_Option5_mean->Draw("same");

  rate_NewLayer1_Option6_mean->SetLineColor(kGreen+3);
  rate_NewLayer1_Option6_mean->SetLineWidth(2);
  //rate_NewLayer1_Option6_mean->Draw("same");

  rate_NewLayer1_Option7_mean->SetLineColor(kCyan-7);
  rate_NewLayer1_Option7_mean->SetLineWidth(2);
  //rate_NewLayer1_Option7_mean->Draw("same");

  rate_NewLayer1_Option8_mean->SetLineColor(kCyan);
  rate_NewLayer1_Option8_mean->SetLineWidth(2);
  // rate_NewLayer1_Option8_mean->Draw("same");

  rate_NewLayer1_Option9_mean->SetLineColor(kCyan+3);
  rate_NewLayer1_Option9_mean->SetLineWidth(2);
  //rate_NewLayer1_Option9_mean->Draw("same");

  rate_NewLayer1_Option10_mean->SetLineColor(kBlue-7);
  rate_NewLayer1_Option10_mean->SetLineWidth(2);
  //rate_NewLayer1_Option10_mean->Draw("same");

  rate_NewLayer1_Option11_mean->SetLineColor(kBlue+1);
  rate_NewLayer1_Option11_mean->SetLineWidth(2);
  //rate_NewLayer1_Option11_mean->Draw("same");

  rate_NewLayer1_Option12_mean->SetLineColor(kBlue+3);
  rate_NewLayer1_Option12_mean->SetLineWidth(2);
  //rate_NewLayer1_Option12_mean->Draw("same");

  rate_NewLayer1_Option13_mean->SetLineColor(kMagenta-7);
  rate_NewLayer1_Option13_mean->SetLineWidth(2);
  //rate_NewLayer1_Option13_mean->Draw("same");

  rate_NewLayer1_Option14_mean->SetLineColor(kMagenta+1);
  rate_NewLayer1_Option14_mean->SetLineWidth(2);
  //rate_NewLayer1_Option14_mean->Draw("same");

  rate_NewLayer1_Option15_mean->SetLineColor(kMagenta);
  rate_NewLayer1_Option15_mean->SetLineWidth(2);
  // rate_NewLayer1_Option15_mean->Draw("same");

  rate_NewLayer1_Option16_mean->SetLineColor(kRed-7);
  rate_NewLayer1_Option16_mean->SetLineWidth(2);
  //rate_NewLayer1_Option16_mean->Draw("same");

  rate_NewLayer1_Option17_mean->SetLineColor(kRed+1);
  rate_NewLayer1_Option17_mean->SetLineWidth(2);
  //rate_NewLayer1_Option17_mean->Draw("same");

  rate_NewLayer1_Option18_mean->SetLineColor(kRed+3);
  rate_NewLayer1_Option18_mean->SetLineWidth(2);
  //rate_NewLayer1_Option18_mean->Draw("same");

  rate_NewLayer1_Option19_mean->SetLineColor(kRed-5);
  rate_NewLayer1_Option19_mean->SetLineWidth(2);
  //rate_NewLayer1_Option19_mean->Draw("same");

  rate_NewLayer1_Option20_mean->SetLineColor(kGray);
  rate_NewLayer1_Option20_mean->SetLineWidth(2);
  //rate_NewLayer1_Option20_mean->Draw("same");

  rate_NewLayer1_Option21_mean->SetLineColor(kGray+1);
  rate_NewLayer1_Option21_mean->SetLineWidth(2);
  //rate_NewLayer1_Option21_mean->Draw("same");

  rate_NewLayer1_Option22_mean->SetLineColor(4);
  rate_NewLayer1_Option22_mean->SetLineWidth(2);
  rate_NewLayer1_Option22_mean->Draw("same");

  rate_NewLayer1_Option31_mean->SetLineColor(11);
  rate_NewLayer1_Option31_mean->SetLineWidth(2);
  // rate_NewLayer1_Option31_mean->Draw("same");

  rate_NewLayer1_Option31_extrap_mean->SetLineColor(kGreen-5);
  rate_NewLayer1_Option31_extrap_mean->SetLineWidth(2);
  rate_NewLayer1_Option31_extrap_mean->Draw("same");
  
  rate_NewLayer1_noIso_mean->SetLineWidth(2);
  //rate_NewLayer1_noIso->SetLineStyle(2);
  rate_NewLayer1_noIso_mean->SetLineColor(kBlack);
  //rate_NewLayer1_noIso_mean->Draw("same");
   
  TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
  texl->AddText("CMS Internal, #sqrt{s}=13 TeV, Run #"+run_str+" (2018)");
  // texl->AddText("CMS Internal, #sqrt{s}=13 TeV, Run #277069 (2064 bunches), 81<lumi<300");

  texl->SetTextSize(0.04);
  texl->SetFillStyle(0);
  texl->SetBorderSize(0);
  texl->Draw("same");

  TLegend* leg = new TLegend(0.29,0.64,0.81,0.87);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.035);
  leg->SetHeader("Linearly scaled to 2.0E34");
  //leg->AddEntry(rate_NewLayer1_noIso,"Di-#tau no-iso","L");
  //leg->AddEntry(rate_NewLayer1_Option8 ,"#bf{Di-#tau iso (Option 8)  - 2017 Layer-1}","L");
  //leg->AddEntry(rate_NewLayer1_noIso_unpacked,"Di-#tau no-iso, old param.","L");
  //leg->AddEntry(rate_NewLayer1_noIso_mode,"Di-#tau no-iso, mode","L");
  leg->AddEntry(rate_NewLayer1_noIso_unpacked,"Di-#tau no-iso, unpacked","L");
  // leg->AddEntry(rate_NewLayer1_noIso_mean,"Di-#tau no-iso, reemulated","L");
  leg->AddEntry(rate_NewLayer1_Iso_unpacked,"Di-#tau iso, unpacked","L");
  //leg->AddEntry(rate_NewLayer1_noIso,"Di-#tau no-iso, reemulated","L");
  //leg->AddEntry(rate_NewLayer1_Option1,"Di-#tau iso (Option 1)","L");
  //leg->AddEntry(rate_NewLayer1_Option2,"Di-#tau iso (Option 2)","L");
  //leg->AddEntry(rate_NewLayer1_Option8,"Di-#tau iso (Option 8)","L");
  //leg->AddEntry(rate_NewLayer1_Option15,"Di-#tau iso (Option 15)","L");
  //leg->AddEntry(rate_NewLayer1_Option22_mode,"Di-#tau iso (Option 22), mode","L");
  /*leg->AddEntry(rate_NewLayer1_Option1_mean,"Di-#tau iso (Option 1)","L");
  leg->AddEntry(rate_NewLayer1_Option2_mean,"Di-#tau iso (Option 2)","L");
  leg->AddEntry(rate_NewLayer1_Option3_mean,"Di-#tau iso (Option 3)","L");
  leg->AddEntry(rate_NewLayer1_Option4_mean,"Di-#tau iso (Option 4)","L");
  leg->AddEntry(rate_NewLayer1_Option5_mean,"Di-#tau iso (Option 5)","L");
  leg->AddEntry(rate_NewLayer1_Option6_mean,"Di-#tau iso (Option 6)","L");
  leg->AddEntry(rate_NewLayer1_Option7_mean,"Di-#tau iso (Option 7)","L");
  leg->AddEntry(rate_NewLayer1_Option8_mean,"Di-#tau iso (Option 8)","L");
  leg->AddEntry(rate_NewLayer1_Option9_mean,"Di-#tau iso (Option 9)","L");
  leg->AddEntry(rate_NewLayer1_Option10_mean,"Di-#tau iso (Option 10)","L");
  leg->AddEntry(rate_NewLayer1_Option11_mean,"Di-#tau iso (Option 11)","L");
  leg->AddEntry(rate_NewLayer1_Option12_mean,"Di-#tau iso (Option 12)","L");
  leg->AddEntry(rate_NewLayer1_Option13_mean,"Di-#tau iso (Option 13)","L");
  leg->AddEntry(rate_NewLayer1_Option14_mean,"Di-#tau iso (Option 14)","L");
  leg->AddEntry(rate_NewLayer1_Option15_mean,"Di-#tau iso (Option 15)","L");
  leg->AddEntry(rate_NewLayer1_Option16_mean,"Di-#tau iso (Option 16)","L");
  leg->AddEntry(rate_NewLayer1_Option17_mean,"Di-#tau iso (Option 17)","L");
  leg->AddEntry(rate_NewLayer1_Option18_mean,"Di-#tau iso (Option 18)","L");
  leg->AddEntry(rate_NewLayer1_Option19_mean,"Di-#tau iso (Option 19)","L");
  leg->AddEntry(rate_NewLayer1_Option20_mean,"Di-#tau iso (Option 20)","L");
  leg->AddEntry(rate_NewLayer1_Option21_mean,"Di-#tau iso (Option 21)","L");
  */
  // leg->AddEntry(rate_NewLayer1_Option8_mean,"Di-#tau iso (Option 8), reemulated (new nTT)","L");
  // leg->AddEntry(rate_NewLayer1_Option15_mean,"Di-#tau iso (Option 15), reemulated (new nTT)","L");
  leg->AddEntry(rate_NewLayer1_Option22_mean,"Di-#tau iso (Option 22), reemulated (new nTT)","L");
  // leg->AddEntry(rate_NewLayer1_Option31_mean,"Di-#tau iso (Option 31), reemulated (new nTT)","L");
  leg->AddEntry(rate_NewLayer1_Option31_extrap_mean,"Di-#tau iso (Option 31_extrap), reemulated (new nTT)","L");

  TLine line(0., 18., 120., 18.);
  line.SetLineColor(kRed);
  line.SetLineWidth(4);
  line.SetLineStyle(2);
  // line.Draw("same");

  leg->Draw("same");

  c.cd();

  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
  pad2->SetTopMargin(0.05);
  pad2->SetLeftMargin(0.15);
  pad2->SetBottomMargin(0.30);
  // pad2->SetBottomMargin(0.25);//was here

  pad2->SetGridx(); // vertical grid
  pad2->Draw();
  pad2->cd();       // pad2 becomes the current pad

 // TH1F* ratioPlot22_mode = (TH1F*)rate_NewLayer1_noIso_mode->Clone("ratioPlot");
 // ratioPlot22_mode->Divide(rate_NewLayer1_Option22_mode);
 // ratioPlot22_mode->SetLineColor(kGreen+1);
 // ratioPlot22_mode->Draw();
 //
  
  TH1F* ratioPlot31extrap_mean = (TH1F*)rate_NewLayer1_Option31_extrap_mean->Clone("ratioPlot");
  ratioPlot31extrap_mean->Divide(rate_NewLayer1_noIso_mean);
  // TH1F* ratioPlot31extrap_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  // ratioPlot31extrap_mean->Divide(rate_NewLayer1_Option31_extrap_mean);
  ratioPlot31extrap_mean->SetLineColor(kGreen-5);
  ratioPlot31extrap_mean->Draw();

  TH1F* ratioPlot21_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot21_mean->Divide(rate_NewLayer1_Option21_mean);
  ratioPlot21_mean->SetLineColor(kGray+1);
  //ratioPlot21_mean->Draw("same");

  TH1F* ratioPlot20_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot20_mean->Divide(rate_NewLayer1_Option20_mean);
  ratioPlot20_mean->SetLineColor(kGray);
  //ratioPlot20_mean->Draw("same");
  
  TH1F* ratioPlot19_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot19_mean->Divide(rate_NewLayer1_Option19_mean);
  ratioPlot19_mean->SetLineColor(kRed-5);
  //ratioPlot19_mean->Draw("same");

  TH1F* ratioPlot18_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot18_mean->Divide(rate_NewLayer1_Option18_mean);
  ratioPlot18_mean->SetLineColor(kRed+3);
  //ratioPlot18_mean->Draw("same");

  TH1F* ratioPlot17_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot17_mean->Divide(rate_NewLayer1_Option17_mean);
  ratioPlot17_mean->SetLineColor(kRed+1);
  //ratioPlot17_mean->Draw("same");

  TH1F* ratioPlot16_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot16_mean->Divide(rate_NewLayer1_Option16_mean);
  ratioPlot16_mean->SetLineColor(kRed-7);
  //ratioPlot16_mean->Draw("same");
  
  TH1F* ratioPlot15_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot15_mean->Divide(rate_NewLayer1_Option15_mean);
  ratioPlot15_mean->SetLineColor(kMagenta+3);
  //ratioPlot15_mean->Draw("same");

  TH1F* ratioPlot14_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot14_mean->Divide(rate_NewLayer1_Option14_mean);
  ratioPlot14_mean->SetLineColor(kMagenta+1);
  //ratioPlot14_mean->Draw("same");

  TH1F* ratioPlot13_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot13_mean->Divide(rate_NewLayer1_Option13_mean);
  ratioPlot13_mean->SetLineColor(kMagenta-7);
  //ratioPlot13_mean->Draw("same");
  
  TH1F* ratioPlot12_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot12_mean->Divide(rate_NewLayer1_Option12_mean);
  ratioPlot12_mean->SetLineColor(kBlue+3);
  //ratioPlot12_mean->Draw("same");
 
  TH1F* ratioPlot11_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot11_mean->Divide(rate_NewLayer1_Option11_mean);
  ratioPlot11_mean->SetLineColor(kBlue+1);
  //ratioPlot11_mean->Draw("same");
  
  TH1F* ratioPlot10_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot10_mean->Divide(rate_NewLayer1_Option10_mean);
  ratioPlot10_mean->SetLineColor(kBlue-7);
  //ratioPlot10_mean->Draw("same");

  TH1F* ratioPlot9_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot9_mean->Divide(rate_NewLayer1_Option9_mean);
  ratioPlot9_mean->SetLineColor(kCyan+3);
  //ratioPlot9_mean->Draw("same");

  TH1F* ratioPlot8_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot8_mean->Divide(rate_NewLayer1_Option8_mean);
  ratioPlot8_mean->SetLineColor(kCyan+1);
  //ratioPlot8_mean->Draw("same");

  TH1F* ratioPlot7_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot7_mean->Divide(rate_NewLayer1_Option7_mean);
  ratioPlot7_mean->SetLineColor(kCyan-7);
  //ratioPlot7_mean->Draw("same");

  TH1F* ratioPlot6_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot6_mean->Divide(rate_NewLayer1_Option6_mean);
  ratioPlot6_mean->SetLineColor(kGreen+3);
  //ratioPlot6_mean->Draw("same");

  TH1F* ratioPlot5_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot5_mean->Divide(rate_NewLayer1_Option5_mean);
  ratioPlot5_mean->SetLineColor(kGreen+1);
  //ratioPlot5_mean->Draw("same");

  TH1F* ratioPlot4_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot4_mean->Divide(rate_NewLayer1_Option4_mean);
  ratioPlot4_mean->SetLineColor(kGreen-7);
  //ratioPlot4_mean->Draw("same");

  TH1F* ratioPlot3_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot3_mean->Divide(rate_NewLayer1_Option3_mean);
  ratioPlot3_mean->SetLineColor(kYellow+3);
  //ratioPlot3_mean->Draw("same");

  TH1F* ratioPlot2_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot2_mean->Divide(rate_NewLayer1_Option2_mean);
  ratioPlot2_mean->SetLineColor(kYellow+1);
  //ratioPlot2_mean->Draw("same");

  TH1F* ratioPlot1_mean = (TH1F*)rate_NewLayer1_noIso_mean->Clone("ratioPlot");
  ratioPlot1_mean->Divide(rate_NewLayer1_Option1_mean);
  ratioPlot1_mean->SetLineColor(kYellow-7);
  //ratioPlot1_mean->Draw("same");
  
  TH1F* ratioPlot_unpacked = (TH1F*)rate_NewLayer1_Iso_unpacked->Clone("ratioPlot");
  ratioPlot_unpacked->Divide(rate_NewLayer1_noIso_unpacked);
 // TH1F* ratioPlot_unpacked = (TH1F*)rate_NewLayer1_noIso_unpacked->Clone("ratioPlot");
 //  ratioPlot_unpacked->Divide(rate_NewLayer1_Iso_unpacked);
  ratioPlot_unpacked->SetLineColor(kRed);
  ratioPlot_unpacked->Draw("same");
  
  ratioPlot31extrap_mean->GetXaxis()->SetLabelSize(0.09);
  ratioPlot31extrap_mean->GetYaxis()->SetLabelSize(0.09);

  ratioPlot31extrap_mean->GetYaxis()->SetTitleSize(0.09);
  ratioPlot31extrap_mean->SetTitle("");
  // ratioPlot->GetXaxis()->SetRangeUser(20.,100.);
  ratioPlot31extrap_mean->GetXaxis()->SetRangeUser(20.,60.);
  // ratioPlot->GetXaxis()->SetRangeUser(0.,40.);
  ratioPlot31extrap_mean->GetYaxis()->SetRangeUser(0.,2.);
  ratioPlot31extrap_mean->GetYaxis()->SetTitle("iso(Option)/non-iso");
  ratioPlot31extrap_mean->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) threshold [GeV]");
  // ratioPlot->GetXaxis()->SetTitleOffset(1.3);
  ratioPlot31extrap_mean->GetXaxis()->SetTitleSize(0.11);
  // ratioPlot->GetYaxis()->SetTitle("New/Currently online");
  ratioPlot31extrap_mean->GetYaxis()->SetTitleOffset(0.5);

  // TLine line2(20., 1., 100., 1.);
  TLine line2(20., 1., 60., 1.);
  // TLine line2(0., 1., 40., 1.);
  line2.SetLineColor(kGreen);
  line2.SetLineStyle(2);
  //line2.Draw("same");

  TString PDFs = "PDFs/";
  TString ROOTs = "ROOTs/";
  c.SaveAs(PDFs+CanvasNamePdf.Data());
  c.SaveAs(ROOTs+CanvasNameRoot.Data());

  //find first threshold giving < 10 kHz.
  Double_t Target = 14;
  // Double_t Target = 14.*1.8/2.0;
  // Double_t Target = 14.;

  Double_t Threshold_NewLayer1_noIso = 0.;
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

  Double_t Threshold_NewLayer1_noIso_unpacked = 0.;
  Double_t Threshold_NewLayer1_Iso_unpacked = 0.;
  
    for(Int_t i = 1 ; i <= rate_NewLayer1_noIso_mean->GetNbinsX() ; ++i)
    {
      if(rate_NewLayer1_noIso_mean->GetBinContent(i)<=Target)
  	{
  	  Threshold_NewLayer1_noIso = rate_NewLayer1_noIso_mean->GetBinLowEdge(i);
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

   
  // cout<<"Double_t Threshold_Layer1_Option21 = "<<Threshold_Layer1_Option21-0.49<<";"<<endl;

  cout<<"Double_t Threshold_NewLayer1_noIso   = "<<Threshold_NewLayer1_noIso-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option1 = "<<Threshold_NewLayer1_Option1-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option2 = "<<Threshold_NewLayer1_Option2-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option3 = "<<Threshold_NewLayer1_Option3-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option4 = "<<Threshold_NewLayer1_Option4-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option5 = "<<Threshold_NewLayer1_Option5-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option6 = "<<Threshold_NewLayer1_Option6-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option7 = "<<Threshold_NewLayer1_Option7-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option8 = "<<Threshold_NewLayer1_Option8-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option9 = "<<Threshold_NewLayer1_Option9-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option10 = "<<Threshold_NewLayer1_Option10-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option11 = "<<Threshold_NewLayer1_Option11-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option12 = "<<Threshold_NewLayer1_Option12-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option13 = "<<Threshold_NewLayer1_Option13-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option14 = "<<Threshold_NewLayer1_Option14-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option15 = "<<Threshold_NewLayer1_Option15-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option16 = "<<Threshold_NewLayer1_Option16-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option17 = "<<Threshold_NewLayer1_Option17-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option18 = "<<Threshold_NewLayer1_Option18-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option19 = "<<Threshold_NewLayer1_Option19-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option20 = "<<Threshold_NewLayer1_Option20-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option21 = "<<Threshold_NewLayer1_Option21-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option22 = "<<Threshold_NewLayer1_Option22-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option23 = "<<Threshold_NewLayer1_Option23-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option24 = "<<Threshold_NewLayer1_Option24-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option25 = "<<Threshold_NewLayer1_Option25-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option26 = "<<Threshold_NewLayer1_Option26-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option27 = "<<Threshold_NewLayer1_Option27-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option28 = "<<Threshold_NewLayer1_Option28-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option29 = "<<Threshold_NewLayer1_Option29-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option30 = "<<Threshold_NewLayer1_Option30-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option31 = "<<Threshold_NewLayer1_Option31-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Option31_extrap = "<<Threshold_NewLayer1_Option31_extrap-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_noIso_unpacked = "<<Threshold_NewLayer1_noIso_unpacked-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Iso_unpacked = "<<Threshold_NewLayer1_Iso_unpacked-0.49<<";"<<endl;

  cout<<endl;
  cout<<"TString Threshold_NewLayer1_noIso_newnTT = \""<<Threshold_NewLayer1_noIso<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option1_newnTT = \""<<Threshold_NewLayer1_Option1<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option2_newnTT = \""<<Threshold_NewLayer1_Option2<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option3_newnTT = \""<<Threshold_NewLayer1_Option3<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option4_newnTT = \""<<Threshold_NewLayer1_Option4<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option5_newnTT = \""<<Threshold_NewLayer1_Option5<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option6_newnTT = \""<<Threshold_NewLayer1_Option6<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option7_newnTT = \""<<Threshold_NewLayer1_Option7<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option8_newnTT = \""<<Threshold_NewLayer1_Option8<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option9_newnTT = \""<<Threshold_NewLayer1_Option9<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option10_newnTT = \""<<Threshold_NewLayer1_Option10<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option11_newnTT = \""<<Threshold_NewLayer1_Option11<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option12_newnTT = \""<<Threshold_NewLayer1_Option12<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option13_newnTT = \""<<Threshold_NewLayer1_Option13<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option14_newnTT = \""<<Threshold_NewLayer1_Option13<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option15_newnTT = \""<<Threshold_NewLayer1_Option15<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option16_newnTT = \""<<Threshold_NewLayer1_Option16<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option17_newnTT = \""<<Threshold_NewLayer1_Option17<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option18_newnTT = \""<<Threshold_NewLayer1_Option18<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option19_newnTT = \""<<Threshold_NewLayer1_Option19<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option20_newnTT = \""<<Threshold_NewLayer1_Option20<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option21_newnTT = \""<<Threshold_NewLayer1_Option21<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option22_newnTT = \""<<Threshold_NewLayer1_Option22<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option23_newnTT = \""<<Threshold_NewLayer1_Option23<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option24_newnTT = \""<<Threshold_NewLayer1_Option24<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option25_newnTT = \""<<Threshold_NewLayer1_Option25<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option26_newnTT = \""<<Threshold_NewLayer1_Option26<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option27_newnTT = \""<<Threshold_NewLayer1_Option27<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option28_newnTT = \""<<Threshold_NewLayer1_Option28<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option29_newnTT = \""<<Threshold_NewLayer1_Option29<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option30_newnTT = \""<<Threshold_NewLayer1_Option30<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option31_newnTT = \""<<Threshold_NewLayer1_Option31<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Option31_extrap_newnTT = \""<<Threshold_NewLayer1_Option31_extrap<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_noIso_unpacked = \""<<Threshold_NewLayer1_noIso_unpacked<<"\";"<<endl;
  cout<<"TString Threshold_NewLayer1_Iso_unpacked = \""<<Threshold_NewLayer1_Iso_unpacked<<"\";"<<endl;

}
