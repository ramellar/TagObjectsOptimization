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

void compare(bool doScaleToLumi) {
  gStyle->SetOptStat(000000);

  TString scaledToLumi = "";
  if (doScaleToLumi) scaledToLumi = "_scaledTo2e34Lumi";

  TFile f_Runs2bunches0p03lumi("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias1_Run2022A_Runs2bunches0p03lumi_optimizationV11gs_effMin0p9_G3_calibThr1p7_V11_unpacked"+scaledToLumi+".root");
  TFile f_Runs2bunches0p015lumi("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias1_Run2022A_Runs2bunches0p015lumi_optimizationV11gs_effMin0p9_G3_calibThr1p7_V11_unpacked"+scaledToLumi+".root");
  TFile f_Runs3bunches0p04lumi("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias1_Run2022A_Runs3bunches0p04lumi_optimizationV11gs_effMin0p9_G3_calibThr1p7_V11_unpacked"+scaledToLumi+".root");

  TH1F* rate_Runs2bunches0p03lumi_NoIso = (TH1F*)f_Runs2bunches0p03lumi.Get("rate_noCut_DiTau");
  TH1F* rate_Runs2bunches0p015lumi_NoIso = (TH1F*)f_Runs2bunches0p015lumi.Get("rate_noCut_DiTau");
  TH1F* rate_Runs3bunches0p04lumi_NoIso = (TH1F*)f_Runs3bunches0p04lumi.Get("rate_noCut_DiTau");
  
  TH1F* rate_Runs2bunches0p03lumi_Iso = (TH1F*)f_Runs2bunches0p03lumi.Get("rate_Iso_DiTau");
  TH1F* rate_Runs2bunches0p015lumi_Iso = (TH1F*)f_Runs2bunches0p015lumi.Get("rate_Iso_DiTau");
  TH1F* rate_Runs3bunches0p04lumi_Iso = (TH1F*)f_Runs3bunches0p04lumi.Get("rate_Iso_DiTau");

  Double_t NEntries_Runs2bunches0p03lumi = ( (TH1F*)f_Runs2bunches0p03lumi.Get("pt_Iso_DiTau") )->GetEntries();
  Double_t NEntries_Runs2bunches0p015lumi = ( (TH1F*)f_Runs2bunches0p015lumi.Get("pt_Iso_DiTau") )->GetEntries();
  Double_t NEntries_Runs3bunches0p04lumi = ( (TH1F*)f_Runs3bunches0p04lumi.Get("pt_Iso_DiTau") )->GetEntries();
  Double_t TotNEntries = NEntries_Runs2bunches0p03lumi + NEntries_Runs2bunches0p015lumi + NEntries_Runs3bunches0p04lumi;
  
  Double_t weight_Runs2bunches0p03lumi = NEntries_Runs2bunches0p03lumi * 2 * 0.03;
  Double_t weight_Runs2bunches0p015lumi = NEntries_Runs2bunches0p015lumi * 2 * 0.015;
  Double_t weight_Runs3bunches0p04lumi = NEntries_Runs3bunches0p04lumi * 3 * 0.04;
  Double_t TotWeight = weight_Runs2bunches0p03lumi + weight_Runs2bunches0p015lumi + weight_Runs3bunches0p04lumi;

  TH1F* rate_mean_NoIso = new TH1F("rate_mean_NoIso","rate_mean_NoIso",240,0.,240.);
  TH1F* rate_mean_Iso = new TH1F("rate_mean_Iso","rate_mean_Iso",240,0.,240.);

  for (Int_t i = 1; i <= 240; ++i)
  {
    float weightedMeanRate_NoIso = (rate_Runs2bunches0p03lumi_NoIso->GetBinContent(i) * weight_Runs2bunches0p03lumi + rate_Runs2bunches0p015lumi_NoIso->GetBinContent(i) * weight_Runs2bunches0p015lumi + rate_Runs3bunches0p04lumi_NoIso->GetBinContent(i) * weight_Runs3bunches0p04lumi) / TotWeight;
    float weightedMeanRate_Iso = (rate_Runs2bunches0p03lumi_Iso->GetBinContent(i) * weight_Runs2bunches0p03lumi + rate_Runs2bunches0p015lumi_Iso->GetBinContent(i) * weight_Runs2bunches0p015lumi + rate_Runs3bunches0p04lumi_Iso->GetBinContent(i) * weight_Runs3bunches0p04lumi) / TotWeight;
  
    rate_mean_NoIso->SetBinContent(i, weightedMeanRate_NoIso);
    rate_mean_Iso->SetBinContent(i, weightedMeanRate_Iso);
  }

  TString CanvasName = "Comparison_Rate_ZeroBias1_Run2022A_optimizationV11gs_effMin0p9_G3_calibThr1p7_V11_unpacked"+scaledToLumi;
  TString CanvasNamePdf = CanvasName ;
  CanvasNamePdf += ".pdf";
  TString CanvasNameRoot = CanvasName ;
  CanvasNameRoot += ".root";

  TCanvas c(CanvasName.Data(),CanvasName.Data(),650,600);
  //c.SetLeftMargin(0.15);
  c.SetGrid();
  c.SetLogy();

  TPad *pad1 = new TPad("pad1", "pad1", 0., 0., 1., 1.);
  //pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
  //pad1->SetLeftMargin(0.15);
  pad1->SetGridx();         // Vertical grid
  pad1->SetGridy();         // Vertical grid
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();               // pad1 becomes the current pad
  pad1->SetLogy();

  rate_Runs3bunches0p04lumi_NoIso->SetTitle("");
  rate_Runs3bunches0p04lumi_NoIso->GetXaxis()->SetRangeUser(0.,35.);
  rate_Runs3bunches0p04lumi_NoIso->GetYaxis()->SetTitle("Di-#tau rate [kHz]");
  rate_Runs3bunches0p04lumi_NoIso->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) threshold [GeV]");
  rate_Runs3bunches0p04lumi_NoIso->SetLineColor(kBlue);
  rate_Runs3bunches0p04lumi_NoIso->SetLineWidth(2);
  rate_Runs3bunches0p04lumi_NoIso->Draw();

  rate_Runs2bunches0p03lumi_NoIso->SetLineColor(kRed);
  rate_Runs2bunches0p03lumi_NoIso->SetLineWidth(2);
  rate_Runs2bunches0p03lumi_NoIso->Draw("same");

  rate_Runs2bunches0p015lumi_NoIso->SetLineColor(kGreen-7);
  rate_Runs2bunches0p015lumi_NoIso->SetLineWidth(2);
  rate_Runs2bunches0p015lumi_NoIso->Draw("same");

  rate_Runs3bunches0p04lumi_Iso->SetLineColor(kBlue-10);
  rate_Runs3bunches0p04lumi_Iso->SetLineWidth(2);
  rate_Runs3bunches0p04lumi_Iso->Draw("same");

  rate_Runs2bunches0p03lumi_Iso->SetLineColor(kRed-10);
  rate_Runs2bunches0p03lumi_Iso->SetLineWidth(2);
  rate_Runs2bunches0p03lumi_Iso->Draw("same");

  rate_Runs2bunches0p015lumi_Iso->SetLineColor(kGreen-10);
  rate_Runs2bunches0p015lumi_Iso->SetLineWidth(2);
  rate_Runs2bunches0p015lumi_Iso->Draw("same");

  rate_mean_NoIso->SetLineColor(kBlack);
  rate_mean_NoIso->SetLineWidth(2);
  //rate_mean_NoIso->Draw("same");

  rate_mean_Iso->SetLineColor(kGray);
  rate_mean_Iso->SetLineWidth(2);
  //rate_mean_Iso->Draw("same");
  
  TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
  texl->AddText("CMS Internal, #sqrt{s}=900 GeV, Runs 352417..353709 (2022)");
  texl->SetTextSize(0.03);
  texl->SetFillStyle(0);
  texl->SetBorderSize(0);
  texl->Draw("same");

  TLegend* leg = new TLegend(0.45,0.64,0.81,0.87);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.02);
  if (doScaleToLumi) leg->SetHeader("Linearly scaled to 2.0E34");
  leg->AddEntry(rate_Runs3bunches0p04lumi_NoIso,"Di-#tau No-Iso (NB=3, L=0.04E30)","L");
  leg->AddEntry(rate_Runs2bunches0p03lumi_NoIso,"Di-#tau No-Iso (NB=2, L=0.03E30)","L");
  leg->AddEntry(rate_Runs2bunches0p015lumi_NoIso,"Di-#tau No-Iso (NB=2, L=0.015E30)","L");
  leg->AddEntry(rate_Runs3bunches0p04lumi_Iso,"Di-#tau Iso (NB=3, L=0.04E30)","L");
  leg->AddEntry(rate_Runs2bunches0p03lumi_Iso,"Di-#tau Iso (NB=2, L=0.03E30)","L");
  leg->AddEntry(rate_Runs2bunches0p015lumi_Iso,"Di-#tau Iso (NB=2, L=0.015E30)","L");
  //leg->AddEntry(rate_mean_NoIso,"Di-#tau No-Iso (weighted average)","L");
  //leg->AddEntry(rate_mean_Iso,"Di-#tau Iso (weighted average)","L");

  leg->Draw("same");

  c.cd();

  TString PDFs = "PDFs/";
  TString ROOTs = "ROOTs/";
  c.SaveAs(PDFs+CanvasNamePdf.Data());
  c.SaveAs(ROOTs+CanvasNameRoot.Data());

  //##############################################################

  TString CanvasName1 = "Comparison_Rate_ZeroBias1_Run2022A_optimizationV11gs_effMin0p9_G3_calibThr1p7_V11_unpacked"+scaledToLumi+"_mean";
  TString CanvasNamePdf1 = CanvasName1 ;
  CanvasNamePdf1 += ".pdf";
  TString CanvasNameRoot1 = CanvasName1 ;
  CanvasNameRoot1 += ".root";

  TCanvas c1(CanvasName1.Data(),CanvasName1.Data(),650,600);
  //c1.SetLeftMargin(0.15);
  c1.SetGrid();
  c1.SetLogy();

  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.0, 1, 1.0);
  //pad1->SetBottomMargin(0.05); // Upper and lower plot are joined
  //pad1->SetLeftMargin(0.15);
  pad2->SetGridx();         // Vertical grid
  pad2->SetGridy();         // Vertical grid
  pad2->Draw();             // Draw the upper pad: pad1
  pad2->cd();               // pad1 becomes the current pad
  pad2->SetLogy();

  rate_mean_NoIso->SetTitle("");
  rate_mean_NoIso->GetXaxis()->SetRangeUser(0.,35.);
  rate_mean_NoIso->GetYaxis()->SetTitle("Di-#tau rate [kHz]");
  rate_mean_NoIso->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) threshold [GeV]");
  rate_mean_NoIso->SetLineColor(kBlack);
  rate_mean_NoIso->SetLineWidth(2);
  rate_mean_NoIso->Draw();

  rate_mean_Iso->SetLineColor(kGray);
  rate_mean_Iso->SetLineWidth(2);
  rate_mean_Iso->Draw("same");
  
  TPaveText* texl1 = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
  texl1->AddText("CMS Internal, #sqrt{s}=900 GeV, Runs 352417..353709 (2022)");
  texl1->SetTextSize(0.03);
  texl1->SetFillStyle(0);
  texl1->SetBorderSize(0);
  texl1->Draw("same");

  TLegend* leg1 = new TLegend(0.45,0.64,0.81,0.87);
  leg1->SetBorderSize(0);
  leg1->SetTextSize(0.02);
  if (doScaleToLumi) leg1->SetHeader("Linearly scaled to 2.0E34");
  leg1->AddEntry(rate_mean_NoIso,"Di-#tau No-Iso (weighted average)","L");
  leg1->AddEntry(rate_mean_Iso,"Di-#tau Iso (weighted average)","L");

  leg1->Draw("same");

  c1.cd();

  c1.SaveAs(PDFs+CanvasNamePdf1.Data());
  c1.SaveAs(ROOTs+CanvasNameRoot1.Data());




  //find first threshold giving < 14 kHz.
  Double_t Target = 14;

  Double_t Threshold_Runs2bunches0p03lumi_NoIso = 0.;
  Double_t Threshold_Runs2bunches0p015lumi_NoIso = 0.;
  Double_t Threshold_Runs3bunches0p04lumi_NoIso = 0.;
  Double_t Threshold_Runs2bunches0p03lumi_Iso = 0.;
  Double_t Threshold_Runs2bunches0p015lumi_Iso = 0.;
  Double_t Threshold_Runs3bunches0p04lumi_Iso = 0.;
  Double_t Threshold_mean_NoIso = 0.;
  Double_t Threshold_mean_Iso = 0.;
  
  for(Int_t i = 1 ; i <= rate_Runs2bunches0p03lumi_NoIso->GetNbinsX() ; ++i)
    {
      if(rate_Runs2bunches0p03lumi_NoIso->GetBinContent(i)<=Target)
  	{
  	  Threshold_Runs2bunches0p03lumi_NoIso = rate_Runs2bunches0p03lumi_NoIso->GetBinLowEdge(i);
  	  break;
  	}
    }
  
  for(Int_t i = 1 ; i <= rate_Runs2bunches0p015lumi_NoIso->GetNbinsX() ; ++i)
    {
      if(rate_Runs2bunches0p015lumi_NoIso->GetBinContent(i)<=Target)
  	{
  	  Threshold_Runs2bunches0p015lumi_NoIso = rate_Runs2bunches0p015lumi_NoIso->GetBinLowEdge(i);
  	  break;
  	}
    }
  
  for(Int_t i = 1 ; i <= rate_Runs3bunches0p04lumi_NoIso->GetNbinsX() ; ++i)
    {
      if(rate_Runs3bunches0p04lumi_NoIso->GetBinContent(i)<=Target)
  	{
  	  Threshold_Runs3bunches0p04lumi_NoIso = rate_Runs3bunches0p04lumi_NoIso->GetBinLowEdge(i);
  	  break;
  	}
    }
  
  for(Int_t i = 1 ; i <= rate_Runs2bunches0p03lumi_Iso->GetNbinsX() ; ++i)
    {
      if(rate_Runs2bunches0p03lumi_Iso->GetBinContent(i)<=Target)
  	{
  	  Threshold_Runs2bunches0p03lumi_Iso = rate_Runs2bunches0p03lumi_Iso->GetBinLowEdge(i);
  	  break;
  	}
    }
  
  for(Int_t i = 1 ; i <= rate_Runs2bunches0p015lumi_Iso->GetNbinsX() ; ++i)
    {
      if(rate_Runs2bunches0p015lumi_Iso->GetBinContent(i)<=Target)
  	{
  	  Threshold_Runs2bunches0p015lumi_Iso = rate_Runs2bunches0p015lumi_Iso->GetBinLowEdge(i);
  	  break;
  	}
    }
    
  for(Int_t i = 1 ; i <= rate_Runs3bunches0p04lumi_Iso->GetNbinsX() ; ++i)
    {
      if(rate_Runs3bunches0p04lumi_Iso->GetBinContent(i)<=Target)
  	{
  	  Threshold_Runs3bunches0p04lumi_Iso = rate_Runs3bunches0p04lumi_Iso->GetBinLowEdge(i);
  	  break;
  	}
    }

  for(Int_t i = 1 ; i <= rate_mean_NoIso->GetNbinsX() ; ++i)
    {
      if(rate_mean_NoIso->GetBinContent(i)<=Target)
  	{
  	  Threshold_mean_NoIso = rate_mean_NoIso->GetBinLowEdge(i);
  	  break;
  	}
    }
  
  for(Int_t i = 1 ; i <= rate_mean_Iso->GetNbinsX() ; ++i)
    {
      if(rate_mean_Iso->GetBinContent(i)<=Target)
  	{
  	  Threshold_mean_Iso = rate_mean_Iso->GetBinLowEdge(i);
  	  break;
  	}
    }

  cout<<"Threshold_Runs2bunches0p03lumi_NoIso   = "<<Threshold_Runs2bunches0p03lumi_NoIso-0.49<<";"<<endl;
  cout<<"Threshold_Runs2bunches0p015lumi_NoIso = "<<Threshold_Runs2bunches0p015lumi_NoIso-0.49<<";"<<endl;
  cout<<"Threshold_Runs3bunches0p04lumi_NoIso = "<<Threshold_Runs3bunches0p04lumi_NoIso-0.49<<";"<<endl;
  cout<<"Threshold_Runs2bunches0p03lumi_Iso = "<<Threshold_Runs2bunches0p03lumi_Iso-0.49<<";"<<endl;
  cout<<"Threshold_Runs2bunches0p015lumi_Iso = "<<Threshold_Runs2bunches0p015lumi_Iso-0.49<<";"<<endl;
  cout<<"Threshold_Runs3bunches0p04lumi_Iso = "<<Threshold_Runs3bunches0p04lumi_Iso-0.49<<";"<<endl;
  cout<<"Threshold_mean_NoIso = "<<Threshold_mean_NoIso-0.49<<";"<<endl;
  cout<<"Threshold_mean_Iso = "<<Threshold_mean_Iso-0.49<<";"<<endl;

}
