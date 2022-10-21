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

void compare(TString fill_str, bool doScaleToLumi) {
  gStyle->SetOptStat(000000);

  TString scaledToLumi = "";
  if (doScaleToLumi) scaledToLumi = "_scaledTo2e34Lumi";

  TFile f("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias_Run2022C_Fill"+fill_str+"_unpacked"+scaledToLumi+".root");

  TH1F* rate_NoIso = (TH1F*)f.Get("rate_noCut_DiTau");
  
  TH1F* rate_Iso = (TH1F*)f.Get("rate_Iso_DiTau");

  //##############################################################

  TString PDFs = "PDFs/Run3_13p6TeV/";
  TString ROOTs = "ROOTs/";

  TString CanvasName1 = "Comparison_Rate_ZeroBias_Run2022C_Fill"+fill_str+"_unpacked"+scaledToLumi;
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

  rate_NoIso->SetTitle("");
  rate_NoIso->GetXaxis()->SetRangeUser(10.,60.);
  rate_NoIso->GetYaxis()->SetTitle("Di-#tau rate [kHz]");
  rate_NoIso->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) threshold [GeV]");
  rate_NoIso->SetLineColor(kBlack);
  rate_NoIso->SetLineWidth(2);
  rate_NoIso->Draw();

  rate_Iso->SetLineColor(kGray);
  rate_Iso->SetLineWidth(2);
  rate_Iso->Draw("same");
  
  TPaveText* texl1 = new TPaveText(0.15,0.87,0.2,0.99,"NDC");
  texl1->AddText("CMS Internal");
  texl1->SetTextSize(0.03);
  texl1->SetFillStyle(0);
  texl1->SetBorderSize(0);
  texl1->Draw("same");

  TPaveText* texl2 = new TPaveText(0.68,0.87,0.89,0.99,"NDC");
  texl2->AddText("#sqrt{s} = 13.6 TeV (2022)");
  texl2->SetTextSize(0.03);
  texl2->SetFillStyle(0);
  texl2->SetBorderSize(0);
  texl2->Draw("same");

  TLegend* leg1 = new TLegend(0.45,0.74,0.81,0.87);
  leg1->SetBorderSize(0);
  leg1->SetTextSize(0.02);
  if (doScaleToLumi) leg1->SetHeader("Linearly scaled to 2.0E34");
  leg1->AddEntry(rate_NoIso,"Di-#tau No-Iso (Fill "+fill_str+")","L");
  leg1->AddEntry(rate_Iso,"Di-#tau Iso (Fill "+fill_str+")","L");

  leg1->Draw("same");

  c1.cd();

  c1.SaveAs(PDFs+CanvasNamePdf1.Data());
  c1.SaveAs(ROOTs+CanvasNameRoot1.Data());

  //find first threshold giving < 14 kHz.
  Double_t Target = 14;

  Double_t Threshold_NoIso = 0.;
  Double_t Threshold_Iso = 0.;

  for(Int_t i = 1 ; i <= rate_NoIso->GetNbinsX() ; ++i)
    {
      if(rate_NoIso->GetBinContent(i)<=Target)
  	{
  	  Threshold_NoIso = rate_NoIso->GetBinLowEdge(i);
  	  break;
  	}
    }
  
  for(Int_t i = 1 ; i <= rate_Iso->GetNbinsX() ; ++i)
    {
      if(rate_Iso->GetBinContent(i)<=Target)
  	{
  	  Threshold_Iso = rate_Iso->GetBinLowEdge(i);
  	  break;
  	}
    }

  cout<<"Threshold_NoIso = "<<Threshold_NoIso-0.49<<";"<<endl;
  cout<<"Threshold_Iso = "<<Threshold_Iso-0.49<<";"<<endl;

}
