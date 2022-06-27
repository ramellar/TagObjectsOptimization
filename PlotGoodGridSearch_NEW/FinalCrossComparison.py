from array import array
import ROOT
import sys
import os

file_linear    = ROOT.TFile("/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr1p7/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE14kHz_SingleNeutrino122X_gs_linear_effMin0p9_0_2022_06_13.root", "READ");
file_quadratic = ROOT.TFile("/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr1p7/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE14kHz_SingleNeutrino122X_gs_quadratic0p7_effMin0p9_3_2022_06_13.root", "READ");
file_sigmoid   = ROOT.TFile("/data_CMS/cms/motta/Run3preparation/2022_06_13_optimizationV13_calibThr1p7/Run3_MC_VBFHToTauTau_M125_TURNONS_FIXEDRATE14kHz_SingleNeutrino122X_gs_sigmoid0p1_effMin0p9_1_2022_06_13.root", "READ");

c = ROOT.TCanvas("c","c",650,600);
c.SetLeftMargin(0.15);
c.SetGrid();
c.SetLogy();

leg = ROOT.TLegend(0.3,0.15,0.89,0.45);
leg.SetBorderSize(0);
leg.SetTextSize(0.018);
leg.SetHeader("Linearly scaled to 2.0E34");

pad1 = ROOT.TPad("pad1", "pad1", 0, 0., 1, 1.0);
pad1.SetLeftMargin(0.15);
pad1.SetGridx();
pad1.SetGridy();
pad1.Draw();
pad1.cd();

turnon_linear    = file_linear.Get("divide_pt_pass_OptionQ0_by_pt")
turnon_quadratic = file_quadratic.Get("divide_pt_pass_OptionG3_by_pt")
turnon_sigmoid   = file_sigmoid.Get("divide_pt_pass_OptionC1_by_pt")

turnon_linear.SetTitle("");
turnon_linear.SetLineColor(1);
turnon_linear.SetLineWidth(1);
turnon_linear.GetXaxis().SetLimits(15,120.);
turnon_linear.GetYaxis().SetRangeUser(0.0,1.01);
turnon_linear.GetYaxis().SetTitle("Efficiency");
turnon_linear.GetXaxis().SetTitle("Offline p_{T}(#tau) [GeV]");
turnon_linear.Draw();

turnon_quadratic.SetLineColor(4);
turnon_quadratic.SetLineWidth(1);
turnon_quadratic.Draw("same");

turnon_sigmoid.SetLineColor(3);
turnon_sigmoid.SetLineWidth(1);
turnon_sigmoid.Draw("same");

leg.AddEntry(turnon_linear,"Linear - effMin0p9 Q0 - THR=35GeV - #epsilon_{TOT}=67.44%","L");
leg.AddEntry(turnon_quadratic,"Quadratic (K=0.75) - effMin0p9 G3 - THR=35GeV - #epsilon_{TOT}=67.59%","L");
leg.AddEntry(turnon_linear,"Sigmoid (K=0.1) - effMin0p9 C1 - THR=35GeV - #epsilon_{TOT}=67.86%","L");
leg.Draw("same")

texl = ROOT.TPaveText(0.15,0.87,0.35,0.99,"NDC");
texl.AddText("CMS Internal");
texl.SetTextSize(0.04);
texl.SetFillStyle(0);
texl.SetBorderSize(0);
pad1.cd();
texl.Draw("same"); 


c.SaveAs("PDFs/optimizationV13/proposal/fixedRate14kHz_parametrisationsComparison.pdf");












