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

  TFile f_Run356375("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias_Run2022C_Run356375_reEmuTPs"+scaledToLumi+".root");
  TFile f_Run356378("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias_Run2022C_Run356378_reEmuTPs"+scaledToLumi+".root");
  TFile f_Run356381("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias_Run2022C_Run356381_reEmuTPs"+scaledToLumi+".root");

  TH1F* rate_Run356375_NoIso = (TH1F*)f_Run356375.Get("rate_noCut_DiTau");
  TH1F* rate_Run356378_NoIso = (TH1F*)f_Run356378.Get("rate_noCut_DiTau");
  TH1F* rate_Run356381_NoIso = (TH1F*)f_Run356381.Get("rate_noCut_DiTau");
  
  TH1F* rate_Run356375_Iso = (TH1F*)f_Run356375.Get("rate_Iso_DiTau");
  TH1F* rate_Run356378_Iso = (TH1F*)f_Run356378.Get("rate_Iso_DiTau");
  TH1F* rate_Run356381_Iso = (TH1F*)f_Run356381.Get("rate_Iso_DiTau");

  Double_t NEntries_Run356375 = ( (TH1F*)f_Run356375.Get("pt_Iso_DiTau") )->GetEntries();
  Double_t NEntries_Run356378 = ( (TH1F*)f_Run356378.Get("pt_Iso_DiTau") )->GetEntries();
  Double_t NEntries_Run356381 = ( (TH1F*)f_Run356381.Get("pt_Iso_DiTau") )->GetEntries();
  Double_t TotNEntries = NEntries_Run356375 + NEntries_Run356378 + NEntries_Run356381;
  
  Double_t weight_Run356375 = NEntries_Run356375 * 2 * 0.03;
  Double_t weight_Run356378 = NEntries_Run356378 * 2 * 0.015;
  Double_t weight_Run356381 = NEntries_Run356381 * 3 * 0.04;
  Double_t TotWeight = weight_Run356375 + weight_Run356378 + weight_Run356381;

  TH1F* rate_mean_NoIso = new TH1F("rate_mean_NoIso","rate_mean_NoIso",240,0.,240.);
  TH1F* rate_mean_Iso = new TH1F("rate_mean_Iso","rate_mean_Iso",240,0.,240.);

  for (Int_t i = 1; i <= 240; ++i)
  {
    float weightedMeanRate_NoIso = (rate_Run356375_NoIso->GetBinContent(i) * weight_Run356375 + rate_Run356378_NoIso->GetBinContent(i) * weight_Run356378 + rate_Run356381_NoIso->GetBinContent(i) * weight_Run356381) / TotWeight;
    float weightedMeanRate_Iso = (rate_Run356375_Iso->GetBinContent(i) * weight_Run356375 + rate_Run356378_Iso->GetBinContent(i) * weight_Run356378 + rate_Run356381_Iso->GetBinContent(i) * weight_Run356381) / TotWeight;
  
    rate_mean_NoIso->SetBinContent(i, weightedMeanRate_NoIso);
    rate_mean_Iso->SetBinContent(i, weightedMeanRate_Iso);
  }

  TString CanvasName = "Comparison_Rate_ZeroBias_Run2022C_Fill8063_reEmuTPs"+scaledToLumi;
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

  rate_Run356381_NoIso->SetTitle("");
  rate_Run356381_NoIso->GetXaxis()->SetRangeUser(10.,60.);
  rate_Run356381_NoIso->GetYaxis()->SetTitle("Di-#tau rate [kHz]");
  rate_Run356381_NoIso->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) threshold [GeV]");
  rate_Run356381_NoIso->SetLineColor(kBlue);
  rate_Run356381_NoIso->SetLineWidth(2);
  rate_Run356381_NoIso->Draw();

  rate_Run356375_NoIso->SetLineColor(kRed);
  rate_Run356375_NoIso->SetLineWidth(2);
  rate_Run356375_NoIso->Draw("same");

  rate_Run356378_NoIso->SetLineColor(kGreen-7);
  rate_Run356378_NoIso->SetLineWidth(2);
  rate_Run356378_NoIso->Draw("same");

  rate_Run356381_Iso->SetLineColor(kBlue-10);
  rate_Run356381_Iso->SetLineWidth(2);
  rate_Run356381_Iso->Draw("same");

  rate_Run356375_Iso->SetLineColor(kRed-10);
  rate_Run356375_Iso->SetLineWidth(2);
  rate_Run356375_Iso->Draw("same");

  rate_Run356378_Iso->SetLineColor(kGreen-10);
  rate_Run356378_Iso->SetLineWidth(2);
  rate_Run356378_Iso->Draw("same");

  rate_mean_NoIso->SetLineColor(kBlack);
  rate_mean_NoIso->SetLineWidth(2);
  //rate_mean_NoIso->Draw("same");

  rate_mean_Iso->SetLineColor(kGray);
  rate_mean_Iso->SetLineWidth(2);
  //rate_mean_Iso->Draw("same");
  
  TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
  texl->AddText("CMS Internal, #sqrt{s}=13.6 TeV, Fill 8063 (2022)");
  texl->SetTextSize(0.03);
  texl->SetFillStyle(0);
  texl->SetBorderSize(0);
  texl->Draw("same");

  TLegend* leg = new TLegend(0.45,0.64,0.81,0.87);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.02);
  if (doScaleToLumi) leg->SetHeader("Linearly scaled to 2.0E34");
  leg->AddEntry(rate_Run356381_NoIso,"Di-#tau No-Iso (Run 356381)","L");
  leg->AddEntry(rate_Run356375_NoIso,"Di-#tau No-Iso (Run 356375)","L");
  leg->AddEntry(rate_Run356378_NoIso,"Di-#tau No-Iso (Run 356378)","L");
  leg->AddEntry(rate_Run356381_Iso,"Di-#tau Iso (Run 356381)","L");
  leg->AddEntry(rate_Run356375_Iso,"Di-#tau Iso (Run 356375)","L");
  leg->AddEntry(rate_Run356378_Iso,"Di-#tau Iso (Run 356378)","L");
  //leg->AddEntry(rate_mean_NoIso,"Di-#tau No-Iso (weighted average)","L");
  //leg->AddEntry(rate_mean_Iso,"Di-#tau Iso (weighted average)","L");

  leg->Draw("same");

  c.cd();

  TString PDFs = "PDFs/Run3_13p6TeV/";
  TString ROOTs = "ROOTs/";
  c.SaveAs(PDFs+CanvasNamePdf.Data());
  c.SaveAs(ROOTs+CanvasNameRoot.Data());

  //##############################################################

  TString CanvasName1 = "Comparison_Rate_ZeroBias_Run2022C_Fill8063_reEmuTPs"+scaledToLumi+"_mean";
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
  rate_mean_NoIso->GetXaxis()->SetRangeUser(10.,60.);
  rate_mean_NoIso->GetYaxis()->SetTitle("Di-#tau rate [kHz]");
  rate_mean_NoIso->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) threshold [GeV]");
  rate_mean_NoIso->SetLineColor(kBlack);
  rate_mean_NoIso->SetLineWidth(2);
  rate_mean_NoIso->Draw();

  rate_mean_Iso->SetLineColor(kGray);
  rate_mean_Iso->SetLineWidth(2);
  rate_mean_Iso->Draw("same");
  
  TPaveText* texl1 = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
  texl1->AddText("CMS Internal, #sqrt{s}=13.6 TeV, Fill 8063 (2022)");
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

  Double_t Threshold_Run356375_NoIso = 0.;
  Double_t Threshold_Run356378_NoIso = 0.;
  Double_t Threshold_Run356381_NoIso = 0.;
  Double_t Threshold_Run356375_Iso = 0.;
  Double_t Threshold_Run356378_Iso = 0.;
  Double_t Threshold_Run356381_Iso = 0.;
  Double_t Threshold_mean_NoIso = 0.;
  Double_t Threshold_mean_Iso = 0.;
  
  for(Int_t i = 1 ; i <= rate_Run356375_NoIso->GetNbinsX() ; ++i)
    {
      if(rate_Run356375_NoIso->GetBinContent(i)<=Target)
  	{
  	  Threshold_Run356375_NoIso = rate_Run356375_NoIso->GetBinLowEdge(i);
  	  break;
  	}
    }
  
  for(Int_t i = 1 ; i <= rate_Run356378_NoIso->GetNbinsX() ; ++i)
    {
      if(rate_Run356378_NoIso->GetBinContent(i)<=Target)
  	{
  	  Threshold_Run356378_NoIso = rate_Run356378_NoIso->GetBinLowEdge(i);
  	  break;
  	}
    }
  
  for(Int_t i = 1 ; i <= rate_Run356381_NoIso->GetNbinsX() ; ++i)
    {
      if(rate_Run356381_NoIso->GetBinContent(i)<=Target)
  	{
  	  Threshold_Run356381_NoIso = rate_Run356381_NoIso->GetBinLowEdge(i);
  	  break;
  	}
    }
  
  for(Int_t i = 1 ; i <= rate_Run356375_Iso->GetNbinsX() ; ++i)
    {
      if(rate_Run356375_Iso->GetBinContent(i)<=Target)
  	{
  	  Threshold_Run356375_Iso = rate_Run356375_Iso->GetBinLowEdge(i);
  	  break;
  	}
    }
  
  for(Int_t i = 1 ; i <= rate_Run356378_Iso->GetNbinsX() ; ++i)
    {
      if(rate_Run356378_Iso->GetBinContent(i)<=Target)
  	{
  	  Threshold_Run356378_Iso = rate_Run356378_Iso->GetBinLowEdge(i);
  	  break;
  	}
    }
    
  for(Int_t i = 1 ; i <= rate_Run356381_Iso->GetNbinsX() ; ++i)
    {
      if(rate_Run356381_Iso->GetBinContent(i)<=Target)
  	{
  	  Threshold_Run356381_Iso = rate_Run356381_Iso->GetBinLowEdge(i);
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

  cout<<"Threshold_Run356375_NoIso   = "<<Threshold_Run356375_NoIso-0.49<<";"<<endl;
  cout<<"Threshold_Run356378_NoIso = "<<Threshold_Run356378_NoIso-0.49<<";"<<endl;
  cout<<"Threshold_Run356381_NoIso = "<<Threshold_Run356381_NoIso-0.49<<";"<<endl;
  cout<<"Threshold_Run356375_Iso = "<<Threshold_Run356375_Iso-0.49<<";"<<endl;
  cout<<"Threshold_Run356378_Iso = "<<Threshold_Run356378_Iso-0.49<<";"<<endl;
  cout<<"Threshold_Run356381_Iso = "<<Threshold_Run356381_Iso-0.49<<";"<<endl;
  cout<<"Threshold_mean_NoIso = "<<Threshold_mean_NoIso-0.49<<";"<<endl;
  cout<<"Threshold_mean_Iso = "<<Threshold_mean_Iso-0.49<<";"<<endl;

}
