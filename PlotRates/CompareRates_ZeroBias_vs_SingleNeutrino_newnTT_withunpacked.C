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

void compare(int run, TString nuGlobalTag, bool Run3reweight, TString isoOpt = "31_extrap", float calibThr = 1.7) {
  TString run_str = to_string(run);

  TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
  TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

  TString isPUReweighted = "";
  if(Run3reweight) isPUReweighted = "puReweighted";

  bool Draw_Options = kTRUE;
  
  gStyle->SetOptStat(000000);
  //TFile f_mode("./Trees_modeparam/histos_rate_Run305310_92X_mode.root","READ");
  TFile f_mean("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias_Run"+run_str+"_optimizationV13_calibThr"+intgr+"p"+decim+"_linear.root","READ");
  TFile f_unpacked("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_ZeroBias_Run"+run_str+"_optimizationV13_calibThr"+intgr+"p"+decim+"_unpacked.root","READ");  
  TFile f_neutrino("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos/histos_rate_SingleNeutrino"+nuGlobalTag+isPUReweighted+"_Run3_MC_optimizationV13_calibThr"+intgr+"p"+decim+"_linear.root","READ");
    
  TH1F* rate_NewLayer1_noIso_unpacked   = (TH1F*)f_unpacked.Get("rate_noCut_DiTau");
  TH1F* rate_NewLayer1_Iso_unpacked     = (TH1F*)f_unpacked.Get("rate_Iso_DiTau");

  TH1F* rate_NewLayer1_noIso_mean   = (TH1F*)f_mean.Get("rate_noCut_DiTau");
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

  TH1F* rate_NewLayer1_noIso_sn   = (TH1F*)f_neutrino.Get("rate_noCut_DiTau");
  TH1F* rate_NewLayer1_Option1_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_1");
  TH1F* rate_NewLayer1_Option2_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_2");
  TH1F* rate_NewLayer1_Option3_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_3");
  TH1F* rate_NewLayer1_Option4_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_4");
  TH1F* rate_NewLayer1_Option5_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_5");
  TH1F* rate_NewLayer1_Option6_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_6");
  TH1F* rate_NewLayer1_Option7_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_7");
  TH1F* rate_NewLayer1_Option8_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_8");
  TH1F* rate_NewLayer1_Option9_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_9");
  TH1F* rate_NewLayer1_Option10_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_10");
  TH1F* rate_NewLayer1_Option11_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_11");
  TH1F* rate_NewLayer1_Option12_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_12");
  TH1F* rate_NewLayer1_Option13_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_13");
  TH1F* rate_NewLayer1_Option14_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_14");
  TH1F* rate_NewLayer1_Option15_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_15");
  TH1F* rate_NewLayer1_Option16_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_16");
  TH1F* rate_NewLayer1_Option17_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_17");
  TH1F* rate_NewLayer1_Option18_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_18");
  TH1F* rate_NewLayer1_Option19_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_19");
  TH1F* rate_NewLayer1_Option20_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_20");
  TH1F* rate_NewLayer1_Option21_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_21");
  TH1F* rate_NewLayer1_Option22_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_22");
  TH1F* rate_NewLayer1_Option23_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_23");
  TH1F* rate_NewLayer1_Option24_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_24");
  TH1F* rate_NewLayer1_Option25_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_25");
  TH1F* rate_NewLayer1_Option26_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_26");
  TH1F* rate_NewLayer1_Option27_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_27");
  TH1F* rate_NewLayer1_Option28_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_28");
  TH1F* rate_NewLayer1_Option29_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_29");
  TH1F* rate_NewLayer1_Option30_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_30");
  TH1F* rate_NewLayer1_Option31_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_31");
  TH1F* rate_NewLayer1_Option31_extrap_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_31_extrap");
  
  TH1F* rate_NewLayer1_Iso_mean = (TH1F*)f_mean.Get("rate_DiTau_Progression_"+isoOpt);
  TH1F* rate_NewLayer1_Iso_sn = (TH1F*)f_neutrino.Get("rate_DiTau_Progression_"+isoOpt);

  TString CanvasName = "Comparison_Rate_ZeroBiasRun"+run_str+"_vs_SingleNeutrino"+nuGlobalTag+isPUReweighted+"_newnTT_Option"+isoOpt+"_withunpacked_optimizationV13_calibThr"+intgr+"p"+decim;
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
  rate_NewLayer1_noIso_unpacked->GetYaxis()->SetRangeUser(2.,5000.);
  rate_NewLayer1_noIso_unpacked->GetYaxis()->SetTitle("Di-#tau rate [kHz]");
  rate_NewLayer1_noIso_unpacked->SetLineColor(kBlack);
  rate_NewLayer1_noIso_unpacked->SetLineWidth(2);
  rate_NewLayer1_noIso_unpacked->SetLineStyle(1);
  rate_NewLayer1_noIso_unpacked->Draw();

  rate_NewLayer1_noIso_mean->SetTitle("");
  rate_NewLayer1_noIso_mean->SetLineColor(kBlue);
  rate_NewLayer1_noIso_mean->SetLineWidth(2);
  rate_NewLayer1_noIso_mean->SetLineStyle(1);
  rate_NewLayer1_noIso_mean->Draw("same");

  rate_NewLayer1_noIso_sn->SetTitle("");
  rate_NewLayer1_noIso_sn->SetLineColor(kRed);
  rate_NewLayer1_noIso_sn->SetLineWidth(2);
  rate_NewLayer1_noIso_sn->SetLineStyle(1);
  rate_NewLayer1_noIso_sn->Draw("same");

  
  rate_NewLayer1_Iso_unpacked->SetLineColor(kGray);
  rate_NewLayer1_Iso_unpacked->SetLineWidth(2);
  rate_NewLayer1_Iso_unpacked->SetLineStyle(1);
  rate_NewLayer1_Iso_unpacked->Draw("same");

  // rate_NewLayer1_Option22_mean->SetLineColor(kBlue);
  // rate_NewLayer1_Option22_mean->SetLineWidth(2);
  // rate_NewLayer1_Option22_mean->SetLineStyle(2);
  // rate_NewLayer1_Option22_mean->Draw("same");  

  // rate_NewLayer1_Option22_sn->SetLineColor(kRed);
  // rate_NewLayer1_Option22_sn->SetLineWidth(2);
  // rate_NewLayer1_Option22_sn->SetLineStyle(2);
  // rate_NewLayer1_Option22_sn->Draw("same");  


  rate_NewLayer1_Iso_mean->SetLineColor(kBlue);
  rate_NewLayer1_Iso_mean->SetLineWidth(2);
  rate_NewLayer1_Iso_mean->SetLineStyle(2);
  rate_NewLayer1_Iso_mean->Draw("same");  

  rate_NewLayer1_Iso_sn->SetLineColor(kRed);
  rate_NewLayer1_Iso_sn->SetLineWidth(2);
  rate_NewLayer1_Iso_sn->SetLineStyle(2);
  rate_NewLayer1_Iso_sn->Draw("same");

  TPaveText* texl = new TPaveText(0.05,0.87,0.95,0.99,"NDC");
  texl->AddText("CMS Internal, #sqrt{s}=13 TeV, Run #"+run_str+" (2018)");
  // texl->AddText("CMS Internal, #sqrt{s}=13 TeV, Run #277069 (2064 bunches), 81<lumi<300");

  texl->SetTextSize(0.04);
  texl->SetFillStyle(0);
  texl->SetBorderSize(0);
  texl->Draw("same");

  TLegend* leg = new TLegend(0.29,0.54,0.88,0.87);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.025);
  leg->SetHeader("Data linearly scaled to 2.0E34");
  
  leg->AddEntry(rate_NewLayer1_noIso_unpacked,"Di-#tau no-iso, unpacked","L");
  leg->AddEntry(rate_NewLayer1_noIso_mean, "Di-#tau no-iso, data reemulated (new nTT)","L");
  leg->AddEntry(rate_NewLayer1_noIso_sn, "Di-#tau no-iso, Run3 NuGun MC 122X reemulated (new nTT)","L");

  leg->AddEntry(rate_NewLayer1_Iso_unpacked,"Di-#tau iso, unpacked","L");

  // leg->AddEntry(rate_NewLayer1_Option22_mean,"Di-#tau iso (Option 22), reemulated (new nTT) - rate from data","L");
  // leg->AddEntry(rate_NewLayer1_Option22_sn,"Di-#tau iso (Option 22), reemulated (new nTT) - rate from MC","L");

  leg->AddEntry(rate_NewLayer1_Iso_mean,"Di-#tau iso (Option "+isoOpt+"), data reemulated (new nTT)","L");
  leg->AddEntry(rate_NewLayer1_Iso_sn,"Di-#tau iso (Option "+isoOpt+"), Run3 NuGun MC 122X reemulated (new nTT)","L");


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

  TH1F* ratioPlotIso_unpacked = (TH1F*)rate_NewLayer1_Iso_unpacked->Clone("ratioPlot");
  ratioPlotIso_unpacked->Divide(rate_NewLayer1_noIso_unpacked);
  ratioPlotIso_unpacked->SetLineColor(kGray);
  ratioPlotIso_unpacked->SetLineWidth(2);
  ratioPlotIso_unpacked->GetXaxis()->SetLabelSize(0.09);
  ratioPlotIso_unpacked->GetXaxis()->SetTitleSize(0.11);
  ratioPlotIso_unpacked->GetXaxis()->SetRangeUser(20.,60.);
  ratioPlotIso_unpacked->GetXaxis()->SetTitle("E_{T}^{L1}(#tau) threshold [GeV]");
  ratioPlotIso_unpacked->GetYaxis()->SetLabelSize(0.09);
  ratioPlotIso_unpacked->GetYaxis()->SetTitleSize(0.09);
  ratioPlotIso_unpacked->GetYaxis()->SetTitleOffset(0.5);
  ratioPlotIso_unpacked->GetYaxis()->SetRangeUser(0.,2.);
  ratioPlotIso_unpacked->GetYaxis()->SetTitle("iso(Option)/non-iso");
  ratioPlotIso_unpacked->SetTitle("");
  ratioPlotIso_unpacked->Draw();

  TH1F* ratioPlot_Iso_mean = (TH1F*)rate_NewLayer1_Iso_mean->Clone("ratioPlot");
  ratioPlot_Iso_mean->Divide(rate_NewLayer1_noIso_mean);
  ratioPlot_Iso_mean->SetLineColor(kBlue);
  ratioPlot_Iso_mean->SetLineWidth(2);
  ratioPlot_Iso_mean->SetLineStyle(2);
  ratioPlot_Iso_mean->Draw("same");

  TH1F* ratioPlot_Iso_sn = (TH1F*)rate_NewLayer1_Iso_sn->Clone("ratioPlot");
  ratioPlot_Iso_sn->Divide(rate_NewLayer1_noIso_sn);
  ratioPlot_Iso_sn->SetLineColor(kRed);
  ratioPlot_Iso_sn->SetLineWidth(2);
  ratioPlot_Iso_sn->SetLineStyle(2);
  ratioPlot_Iso_sn->Draw("same");

  TString PDFs = "PDFs/";
  TString ROOTs = "ROOTs/";
  c.SaveAs(PDFs+CanvasNamePdf.Data());
  c.SaveAs(ROOTs+CanvasNameRoot.Data());

  //find first threshold giving < XX kHz.
  Double_t Target = 14;
  // Double_t Target = 14.*1.8/2.0;
  // Double_t Target = 14.;

  Double_t Threshold_NewLayer1_noIso_unpacked = 0.;
  Double_t Threshold_NewLayer1_noIso_mean = 0.;
  Double_t Threshold_NewLayer1_noIso_sn = 0.;
  Double_t Threshold_NewLayer1_Iso_unpacked = 0.;
  Double_t Threshold_NewLayer1_Iso_mean = 0.;
  Double_t Threshold_NewLayer1_Iso_sn = 0.;
  

  for(Int_t i = 1 ; i <= rate_NewLayer1_noIso_unpacked->GetNbinsX() ; ++i)
  {
    if(rate_NewLayer1_noIso_unpacked->GetBinContent(i)<=Target)
      {
        Threshold_NewLayer1_noIso_unpacked = rate_NewLayer1_noIso_unpacked->GetBinLowEdge(i);
        break;
      }
  }

  for(Int_t i = 1 ; i <= rate_NewLayer1_noIso_mean->GetNbinsX() ; ++i)
  {
    if(rate_NewLayer1_noIso_mean->GetBinContent(i)<=Target)
      {
        Threshold_NewLayer1_noIso_mean = rate_NewLayer1_noIso_mean->GetBinLowEdge(i);
        break;
      }
  }

  for(Int_t i = 1 ; i <= rate_NewLayer1_noIso_sn->GetNbinsX() ; ++i)
  {
    if(rate_NewLayer1_noIso_sn->GetBinContent(i)<=Target)
      {
        Threshold_NewLayer1_noIso_sn = rate_NewLayer1_noIso_sn->GetBinLowEdge(i);
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

  for(Int_t i = 1 ; i <= rate_NewLayer1_Iso_mean->GetNbinsX() ; ++i)
  {
    if(rate_NewLayer1_Iso_mean->GetBinContent(i)<=Target)
      {
        Threshold_NewLayer1_Iso_mean = rate_NewLayer1_Iso_mean->GetBinLowEdge(i);
        break;
      }
  }

  for(Int_t i = 1 ; i <= rate_NewLayer1_Iso_sn->GetNbinsX() ; ++i)
  {
    if(rate_NewLayer1_Iso_sn->GetBinContent(i)<=Target)
      {
        Threshold_NewLayer1_Iso_sn = rate_NewLayer1_Iso_sn->GetBinLowEdge(i);
        break;
      }
  }



  cout<<"Double_t Threshold_NewLayer1_noIso_unpacked = "<<Threshold_NewLayer1_noIso_unpacked-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_noIso_mean = "<<Threshold_NewLayer1_noIso_mean-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_noIso_sn = "<<Threshold_NewLayer1_noIso_sn-0.49<<";"<<endl;
  cout<<""<<endl;
  cout<<"Double_t Threshold_NewLayer1_Iso_unpacked = "<<Threshold_NewLayer1_Iso_unpacked-0.49<<";"<<endl;
  cout<<""<<endl;
  cout<<"Double_t Threshold_NewLayer1_Iso_mean = "<<Threshold_NewLayer1_Iso_mean-0.49<<";"<<endl;
  cout<<"Double_t Threshold_NewLayer1_Iso_sn = "<<Threshold_NewLayer1_Iso_sn-0.49<<";"<<endl;
}
