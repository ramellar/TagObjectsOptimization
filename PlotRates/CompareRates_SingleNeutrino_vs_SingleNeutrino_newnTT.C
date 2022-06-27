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

void compare(TString isoOpt = "31_extrap", float calibThr = 1.7) {
  TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
  TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

  bool Draw_Options = kTRUE;
  
  gStyle->SetOptStat(000000);

  TFile f_neutrino112X("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_SingleNeutrino112X_Run3_MC_optimizationV10_calibThr1p7.root","READ");
  TFile f_neutrino112XpuRew("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_SingleNeutrino112XpuReweighted_Run3_MC_optimizationV10_calibThr1p7.root","READ");
  TFile f_neutrino120X("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_SingleNeutrino120X_Run3_MC_optimizationV10_calibThr1p7.root","READ");
   

  TH1F* rate_NewLayer1_noIso_112X      = (TH1F*)f_neutrino112X.Get("rate_noCut_DiTau");
  TH1F* rate_NewLayer1_noIso_112XpuRew = (TH1F*)f_neutrino112XpuRew.Get("rate_noCut_DiTau");
  TH1F* rate_NewLayer1_noIso_120X      = (TH1F*)f_neutrino120X.Get("rate_noCut_DiTau");

  TH1F* rate_NewLayer1_Iso_112X      = (TH1F*)f_neutrino112X.Get("rate_DiTau_Progression_"+isoOpt);
  TH1F* rate_NewLayer1_Iso_112XpuRew = (TH1F*)f_neutrino112XpuRew.Get("rate_DiTau_Progression_"+isoOpt);
  TH1F* rate_NewLayer1_Iso_120X      = (TH1F*)f_neutrino120X.Get("rate_DiTau_Progression_"+isoOpt);

  
  TString CanvasName = "Comparison_Rate_SingleNeutrino_vs_SingleNeutrino_newnTT_Option"+isoOpt+"_optimizationV10_calibThr"+intgr+"p"+decim;
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


  rate_NewLayer1_noIso_112X->SetTitle("");
  rate_NewLayer1_noIso_112X->GetXaxis()->SetRangeUser(20.,60.);
  rate_NewLayer1_noIso_112X->GetYaxis()->SetRangeUser(2.,5000.);
  rate_NewLayer1_noIso_112X->GetYaxis()->SetTitle("Di-#tau rate [kHz]");
  rate_NewLayer1_noIso_112X->SetLineColor(kRed);
  rate_NewLayer1_noIso_112X->SetLineWidth(2);
  rate_NewLayer1_noIso_112X->SetLineStyle(1);
  rate_NewLayer1_noIso_112X->Draw();

  rate_NewLayer1_noIso_112XpuRew->SetTitle("");
  rate_NewLayer1_noIso_112XpuRew->SetLineColor(kMagenta);
  rate_NewLayer1_noIso_112XpuRew->SetLineWidth(2);
  rate_NewLayer1_noIso_112XpuRew->SetLineStyle(1);
  rate_NewLayer1_noIso_112XpuRew->Draw("same");

  rate_NewLayer1_noIso_120X->SetTitle("");
  rate_NewLayer1_noIso_120X->SetLineColor(kBlue);
  rate_NewLayer1_noIso_120X->SetLineWidth(2);
  rate_NewLayer1_noIso_120X->SetLineStyle(1);
  rate_NewLayer1_noIso_120X->Draw("same");

  
  rate_NewLayer1_Iso_112X->SetLineColor(kRed);
  rate_NewLayer1_Iso_112X->SetLineWidth(2);
  rate_NewLayer1_Iso_112X->SetLineStyle(2);
  rate_NewLayer1_Iso_112X->Draw("same");

  rate_NewLayer1_Iso_112XpuRew->SetLineColor(kMagenta);
  rate_NewLayer1_Iso_112XpuRew->SetLineWidth(2);
  rate_NewLayer1_Iso_112XpuRew->SetLineStyle(2);
  rate_NewLayer1_Iso_112XpuRew->Draw("same");  

  rate_NewLayer1_Iso_120X->SetLineColor(kBlue);
  rate_NewLayer1_Iso_120X->SetLineWidth(2);
  rate_NewLayer1_Iso_120X->SetLineStyle(2);
  rate_NewLayer1_Iso_120X->Draw("same");


  TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
  texl->AddText("CMS Internal, #sqrt{s}=13 TeV, SingeNeutrino Run3 MC");
  // texl->AddText("CMS Internal, #sqrt{s}=13 TeV, Run #277069 (2064 bunches), 81<lumi<300");

  texl->SetTextSize(0.04);
  texl->SetFillStyle(0);
  texl->SetBorderSize(0);
  texl->Draw("same");

  TLegend* leg = new TLegend(0.29,0.54,0.88,0.87);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.025);
  leg->SetHeader("Data linearly scaled to 2.0E34");
  
  leg->AddEntry(rate_NewLayer1_noIso_112X,"Di-#tau no-iso, 112X","L");
  leg->AddEntry(rate_NewLayer1_noIso_112XpuRew, "Di-#tau no-iso, 112X PU reweighted","L");
  leg->AddEntry(rate_NewLayer1_noIso_120X, "Di-#tau no-iso, 120X","L");

  leg->AddEntry(rate_NewLayer1_Iso_112X,"Di-#tau iso (Option "+isoOpt+"), 112X","L");
  leg->AddEntry(rate_NewLayer1_Iso_112XpuRew,"Di-#tau iso (Option "+isoOpt+"), 112X PU reweighted","L");
  leg->AddEntry(rate_NewLayer1_Iso_120X,"Di-#tau iso (Option "+isoOpt+"), 120X","L");


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

  TH1F* ratioPlotIso_112X = (TH1F*)rate_NewLayer1_Iso_112X->Clone("ratioPlot");
  ratioPlotIso_112X->Divide(rate_NewLayer1_noIso_112X);
  ratioPlotIso_112X->SetLineColor(kRed);
  ratioPlotIso_112X->SetLineWidth(2);
  ratioPlotIso_112X->GetXaxis()->SetLabelSize(0.09);
  ratioPlotIso_112X->GetXaxis()->SetTitleSize(0.11);
  ratioPlotIso_112X->GetXaxis()->SetRangeUser(20.,60.);
  ratioPlotIso_112X->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) threshold [GeV]");
  ratioPlotIso_112X->GetYaxis()->SetLabelSize(0.09);
  ratioPlotIso_112X->GetYaxis()->SetTitleSize(0.09);
  ratioPlotIso_112X->GetYaxis()->SetTitleOffset(0.5);
  ratioPlotIso_112X->GetYaxis()->SetRangeUser(0.,2.);
  ratioPlotIso_112X->GetYaxis()->SetTitle("iso(Option)/non-iso");
  ratioPlotIso_112X->SetTitle("");
  ratioPlotIso_112X->Draw();

  TH1F* ratioPlotIso_112XpuRew = (TH1F*)rate_NewLayer1_Iso_112XpuRew->Clone("ratioPlot");
  ratioPlotIso_112XpuRew->Divide(rate_NewLayer1_noIso_112XpuRew);
  ratioPlotIso_112XpuRew->SetLineColor(kMagenta);
  ratioPlotIso_112XpuRew->SetLineWidth(2);
  ratioPlotIso_112XpuRew->SetLineStyle(2);
  ratioPlotIso_112XpuRew->Draw("same");

  TH1F* ratioPlotIso_120X = (TH1F*)rate_NewLayer1_Iso_120X->Clone("ratioPlot");
  ratioPlotIso_120X->Divide(rate_NewLayer1_noIso_120X);
  ratioPlotIso_120X->SetLineColor(kBlue);
  ratioPlotIso_120X->SetLineWidth(2);
  ratioPlotIso_120X->SetLineStyle(2);
  ratioPlotIso_120X->Draw("same");

  TString PDFs = "PDFs/";
  TString ROOTs = "ROOTs/";
  c.SaveAs(PDFs+CanvasNamePdf.Data());
  c.SaveAs(ROOTs+CanvasNameRoot.Data());

  //find first threshold giving < XX kHz.
  Double_t Target = 14;
  // Double_t Target = 14.*1.8/2.0;
  // Double_t Target = 14.;

  Double_t Threshold_NewLayer1_noIso_112X = 0.;
  Double_t Threshold_NewLayer1_noIso_112XpuRew = 0.;
  Double_t Threshold_NewLayer1_noIso_120X = 0.;
  Double_t Threshold_NewLayer1_Iso_112X = 0.;
  Double_t Threshold_NewLayer1_Iso_112XpuRew = 0.;
  Double_t Threshold_NewLayer1_Iso_120X = 0.;
  

  for(Int_t i = 1 ; i <= rate_NewLayer1_noIso_112X->GetNbinsX() ; ++i)
  {
    if(rate_NewLayer1_noIso_112X->GetBinContent(i)<=Target)
      {
        Threshold_NewLayer1_noIso_112X = rate_NewLayer1_noIso_112X->GetBinLowEdge(i);
        break;
      }
  }

  for(Int_t i = 1 ; i <= rate_NewLayer1_noIso_112XpuRew->GetNbinsX() ; ++i)
  {
    if(rate_NewLayer1_noIso_112XpuRew->GetBinContent(i)<=Target)
      {
        Threshold_NewLayer1_noIso_112XpuRew = rate_NewLayer1_noIso_112XpuRew->GetBinLowEdge(i);
        break;
      }
  }

  for(Int_t i = 1 ; i <= rate_NewLayer1_noIso_120X->GetNbinsX() ; ++i)
  {
    if(rate_NewLayer1_noIso_120X->GetBinContent(i)<=Target)
      {
        Threshold_NewLayer1_noIso_120X = rate_NewLayer1_noIso_120X->GetBinLowEdge(i);
        break;
      }
  }

  for(Int_t i = 1 ; i <= rate_NewLayer1_Iso_112X->GetNbinsX() ; ++i)
  {
    if(rate_NewLayer1_Iso_112X->GetBinContent(i)<=Target)
      {
        Threshold_NewLayer1_Iso_112X = rate_NewLayer1_Iso_112X->GetBinLowEdge(i);
        break;
      }
  }

  for(Int_t i = 1 ; i <= rate_NewLayer1_Iso_112XpuRew->GetNbinsX() ; ++i)
  {
    if(rate_NewLayer1_Iso_112XpuRew->GetBinContent(i)<=Target)
      {
        Threshold_NewLayer1_Iso_112XpuRew = rate_NewLayer1_Iso_112XpuRew->GetBinLowEdge(i);
        break;
      }
  }

  for(Int_t i = 1 ; i <= rate_NewLayer1_Iso_120X->GetNbinsX() ; ++i)
  {
    if(rate_NewLayer1_Iso_120X->GetBinContent(i)<=Target)
      {
        Threshold_NewLayer1_Iso_120X = rate_NewLayer1_Iso_120X->GetBinLowEdge(i);
        break;
      }
  }



  cout<<"Double_t Threshold_NewLayer1_noIso_unpacked = "<<Threshold_NewLayer1_noIso_112X-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_noIso_mean = "<<Threshold_NewLayer1_noIso_112XpuRew-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_noIso_sn = "<<Threshold_NewLayer1_noIso_120X-0.49<<";"<<endl;
  cout<<""<<endl;
  cout<<"Double_t Threshold_NewLayer1_Iso_unpacked = "<<Threshold_NewLayer1_Iso_112X-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Iso_mean = "<<Threshold_NewLayer1_Iso_112XpuRew-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Iso_sn = "<<Threshold_NewLayer1_Iso_120X-0.49<<";"<<endl;
}
