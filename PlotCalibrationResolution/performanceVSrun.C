#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <iostream>
#include <TLorentzVector.h>
#include <TVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <TROOT.h>
#include <sstream>
#include <TBranchElement.h>
#include <fstream>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <stdio.h>
#include <math.h>
#include "../Calibrate/ApplyCalibration_newnTT.C"

using namespace std;

void performanceVSrun()
{
    std::vector<int> runs = {357079, 357080, 357081, 357101, 357102, 357104, 357106, 357112, 357268, 357271, 357328, 357329, 357330, 357331, 357332, 357333, 357401, 357406, 357438, 357440, 357441, 357442, 357447, 357472, 357478, 357479, 357482, 357538, 357542, 357550, 357610, 357611, 357612, 357613, 357688, 357696, 357697, 357698, 357699, 357700, 357720, 357732};

    std::vector<int> rumpuphistory = {357077, 357390, 357609, 357681}; // 357785}
    std::vector<TString> nbunches= {"2160", "2400", "1922", "2400"}; //2448}


    int lowerBound = runs[0]-100;
    int upperBound = runs[runs.size()-1]+100;
    int nbins = upperBound - lowerBound;

    TString lowerBound_str = to_string(runs[0]);
    TString upperBound_str = to_string(runs[runs.size()-1]);

    TH1F* inclusive_resolVSrun = new TH1F("inclusive_resolVSrun", "inclusiveresolVSrun", nbins, lowerBound, upperBound);
    TH1F* inclusive_scaleVSrun = new TH1F("inclusive_scaleVSrun", "inclusivescaleVSrun", nbins, lowerBound, upperBound);

    TH1F* barrel_resolVSrun = new TH1F("barrel_resolVSrun", "barrel_resolVSrun", nbins, lowerBound, upperBound);
    TH1F* barrel_scaleVSrun = new TH1F("barrel_scaleVSrun", "barrel_scaleVSrun", nbins, lowerBound, upperBound);

    TH1F* endcap_resolVSrun = new TH1F("endcap_resolVSrun", "endcap_resolVSrun", nbins, lowerBound, upperBound);
    TH1F* endcap_scaleVSrun = new TH1F("endcap_scaleVSrun", "endcap_scaleVSrun", nbins, lowerBound, upperBound);

    TH1F* nvtxVSrun = new TH1F("nvtxVSrun", "nvtxVSrun", nbins, lowerBound, upperBound);    

    for (long unsigned int i = 0; i < runs.size()-1; ++i)
    {
        int run = runs[i];
        TString run_str = to_string(run);
        TFile f("ROOTs/resolutions_of_Run"+run_str+".root","READ");

        TH1F* pt_response_ptInclusive    = (TH1F*)f.Get("pt_response_ptInclusive");
        TH1F* pt_barrel_resp_ptInclusive = (TH1F*)f.Get("pt_barrel_resp_ptInclusive");
        TH1F* pt_endcap_resp_ptInclusive = (TH1F*)f.Get("pt_endcap_resp_ptInclusive");
        TH1F* Nvtx = (TH1F*)f.Get("Nvtx");

        if (pt_response_ptInclusive->GetMean() != 0.0) {inclusive_resolVSrun->SetBinContent(run-lowerBound+1, pt_response_ptInclusive->GetRMS()/pt_response_ptInclusive->GetMean()); }
        if (pt_barrel_resp_ptInclusive->GetMean() != 0.0) {barrel_resolVSrun->SetBinContent(run-lowerBound+1, pt_barrel_resp_ptInclusive->GetRMS()/pt_barrel_resp_ptInclusive->GetMean()); }
        if (pt_endcap_resp_ptInclusive->GetMean() != 0.0) {endcap_resolVSrun->SetBinContent(run-lowerBound+1, pt_endcap_resp_ptInclusive->GetRMS()/pt_endcap_resp_ptInclusive->GetMean()); }

        if (pt_response_ptInclusive->GetMean() != 0.0) {inclusive_resolVSrun->SetBinError(run-lowerBound+1, pt_response_ptInclusive->GetRMSError()/pt_response_ptInclusive->GetMean()); }
        if (pt_barrel_resp_ptInclusive->GetMean() != 0.0) {barrel_resolVSrun->SetBinError(run-lowerBound+1, pt_barrel_resp_ptInclusive->GetRMSError()/pt_barrel_resp_ptInclusive->GetMean()); }
        if (pt_endcap_resp_ptInclusive->GetMean() != 0.0) {endcap_resolVSrun->SetBinError(run-lowerBound+1, pt_endcap_resp_ptInclusive->GetRMSError()/pt_endcap_resp_ptInclusive->GetMean()); }

        inclusive_scaleVSrun->SetBinContent(run-lowerBound+1, pt_response_ptInclusive->GetMean());
        barrel_scaleVSrun->SetBinContent(run-lowerBound+1, pt_barrel_resp_ptInclusive->GetMean());
        endcap_scaleVSrun->SetBinContent(run-lowerBound+1, pt_endcap_resp_ptInclusive->GetMean());

        inclusive_scaleVSrun->SetBinError(run-lowerBound+1, pt_response_ptInclusive->GetMeanError());
        barrel_scaleVSrun->SetBinError(run-lowerBound+1, pt_barrel_resp_ptInclusive->GetMeanError());
        endcap_scaleVSrun->SetBinError(run-lowerBound+1, pt_endcap_resp_ptInclusive->GetMeanError());

        nvtxVSrun->SetBinContent(run-lowerBound+1, Nvtx->GetMean());
        nvtxVSrun->SetBinContent(run-lowerBound+1, Nvtx->GetRMS());
    }

    


    TCanvas canvas("c","c",2400,1200);
    // canvas.SetLeftMargin(0.15);
    canvas.SetGrid();
    // canvas.SetLogy();

    // use dummy histogram to define style
    inclusive_resolVSrun->GetXaxis()->SetMaxDigits(7);
    inclusive_resolVSrun->GetXaxis()->SetRangeUser(runs[0]-25, runs[runs.size()-1]+25);
    inclusive_resolVSrun->GetYaxis()->SetRangeUser(0.12, 0.45);
    inclusive_resolVSrun->GetXaxis()->SetTitle("Run Number");
    inclusive_resolVSrun->GetYaxis()->SetTitle("Resolution");
    // inclusive_resolVSrun->GetXaxis()->SetTitleOffset(1.3);
    // inclusive_resolVSrun->GetYaxis()->SetTitleOffset(1.3);
    inclusive_resolVSrun->SetTitle("");
    inclusive_resolVSrun->SetStats(0);

    inclusive_resolVSrun->SetLineWidth(1);
    inclusive_resolVSrun->SetLineColor(4);
    inclusive_resolVSrun->SetMarkerStyle(8);
    inclusive_resolVSrun->SetMarkerColor(4);

    barrel_resolVSrun->SetLineWidth(1);
    barrel_resolVSrun->SetLineColor(1);
    barrel_resolVSrun->SetMarkerStyle(8);
    barrel_resolVSrun->SetMarkerColor(1);

    endcap_resolVSrun->SetLineWidth(1);
    endcap_resolVSrun->SetLineColor(2);
    endcap_resolVSrun->SetMarkerStyle(8);
    endcap_resolVSrun->SetMarkerColor(2);

    inclusive_resolVSrun->Draw("LPE1");
    barrel_resolVSrun->Draw("same LPE1");
    endcap_resolVSrun->Draw("same LPE1");

    TLegend legend(0.22,0.75,0.45,0.88);
    legend.SetBorderSize(0);
    legend.AddEntry(inclusive_resolVSrun, "Inclusive |#eta^{Offline #tau}|<2.1", "LPE1");
    legend.AddEntry(barrel_resolVSrun, "Barrel |#eta^{Offline #tau}|<1.305", "LPE1");
    legend.AddEntry(endcap_resolVSrun, "Endcap 1.479<|#eta^{Offline #tau}|<2.1", "LPE1");
    legend.Draw("same");

    TPaveText* texl1 = new TPaveText(0.05,0.87,0.2,0.99,"NDC");
    texl1->AddText("CMS Internal");
    texl1->SetTextSize(0.03);
    texl1->SetFillStyle(0);
    texl1->SetBorderSize(0);
    texl1->Draw("same");

    TPaveText* texl2 = new TPaveText(0.84,0.87,0.89,0.99,"NDC");
    texl2->AddText("(13.6 TeV)");
    texl2->SetTextSize(0.03);
    texl2->SetFillStyle(0);
    texl2->SetBorderSize(0);
    texl2->Draw("same");

    for (long unsigned int j = 0; j < rumpuphistory.size(); ++j)
    {
        TPaveText* texl3 = new TPaveText(rumpuphistory[j]+1,0.41,rumpuphistory[j]+50,0.44);
        texl3->AddText("#splitline{Nb="+nbunches[j]+"}{#rightarrow}");
        texl3->SetTextAlign(11);
        texl3->SetTextSize(0.03);
        texl3->SetTextColor(8);
        texl3->SetFillColor(10);
        texl3->Draw("same");

        TLine* line = new TLine(rumpuphistory[j], 0.12, rumpuphistory[j], 0.45);
        line->SetLineColor(8);
        line->SetLineWidth(2);
        line->Draw("same");
    }

    canvas.SaveAs("PDFs/resolution_Run"+lowerBound_str+"_to_Run"+upperBound_str+".pdf");

    // ---------------------------------------------------------------------------------------------

    TCanvas canvas1("c1","c1",2400,1200);
    // canvas1.SetLeftMargin(0.15);
    canvas1.SetGrid();
    // canvas.SetLogy();

    // use dummy histogram to define style
    inclusive_scaleVSrun->GetXaxis()->SetMaxDigits(7);
    inclusive_scaleVSrun->GetXaxis()->CenterLabels();
    inclusive_scaleVSrun->GetXaxis()->SetRangeUser(runs[0]-25, runs[runs.size()-1]+25);
    inclusive_scaleVSrun->GetYaxis()->SetRangeUser(0.7, 1.2);
    inclusive_scaleVSrun->GetXaxis()->SetTitle("Run Number");
    inclusive_scaleVSrun->GetYaxis()->SetTitle("Scale");
    // inclusive_scaleVSrun->GetXaxis()->SetTitleOffset(1.3);
    // inclusive_scaleVSrun->GetYaxis()->SetTitleOffset(1.3);
    inclusive_scaleVSrun->SetTitle("");
    inclusive_scaleVSrun->SetStats(0);

    inclusive_scaleVSrun->SetLineWidth(1);
    inclusive_scaleVSrun->SetLineColor(4);
    inclusive_scaleVSrun->SetMarkerStyle(8);
    inclusive_scaleVSrun->SetMarkerColor(4);

    barrel_scaleVSrun->SetLineWidth(1);
    barrel_scaleVSrun->SetLineColor(1);
    barrel_scaleVSrun->SetMarkerStyle(8);
    barrel_scaleVSrun->SetMarkerColor(1);

    endcap_scaleVSrun->SetLineWidth(1);
    endcap_scaleVSrun->SetLineColor(2);
    endcap_scaleVSrun->SetMarkerStyle(8);
    endcap_scaleVSrun->SetMarkerColor(2);

    inclusive_scaleVSrun->Draw("LPE1");
    barrel_scaleVSrun->Draw("same LPE1");
    endcap_scaleVSrun->Draw("same LPE1");
    
    legend.Draw("same");
    texl1->Draw("same");
    texl2->Draw("same");

    for (long unsigned int j = 0; j < rumpuphistory.size(); ++j)
    {
        TPaveText* texl3 = new TPaveText(rumpuphistory[j]+1,1.151,rumpuphistory[j]+50,1.198);
        texl3->AddText("#splitline{Nb="+nbunches[j]+"}{#rightarrow}");
        texl3->SetTextAlign(11);
        texl3->SetTextSize(0.03);
        texl3->SetTextColor(8);
        texl3->SetFillColor(10);
        texl3->Draw("same");

        TLine* line = new TLine(rumpuphistory[j], 0.7, rumpuphistory[j], 1.2);
        line->SetLineColor(8);
        line->SetLineWidth(2);
        line->Draw("same");
    }

    canvas1.SaveAs("PDFs/scale_Run"+lowerBound_str+"_to_Run"+upperBound_str+".pdf");

    // ---------------------------------------------------------------------------------------------

    TCanvas canvas2("c","c",2400,1200);
    // canvas.SetLeftMargin(0.15);
    canvas2.SetGrid();
    // canvas.SetLogy();

    // use dummy histogram to define style
    nvtxVSrun->GetXaxis()->SetMaxDigits(7);
    // nvtxVSrun->GetYaxis()->SetRangeUser(0.12, 0.45);
    nvtxVSrun->GetXaxis()->SetTitle("Run Number");
    nvtxVSrun->GetYaxis()->SetTitle("Number Vertexes");
    // nvtxVSrun->GetXaxis()->SetTitleOffset(1.3);
    // nvtxVSrun->GetYaxis()->SetTitleOffset(1.3);
    nvtxVSrun->SetTitle("");
    nvtxVSrun->SetStats(0);

    nvtxVSrun->SetLineWidth(1);
    nvtxVSrun->SetLineColor(1);
    nvtxVSrun->SetMarkerStyle(8);
    nvtxVSrun->SetMarkerColor(1);

    nvtxVSrun->Draw("LPE1");

    texl1->Draw("same");
    texl2->Draw("same");

    canvas2.SaveAs("PDFs/Nvtx_Run"+lowerBound_str+"_to_Run"+upperBound_str+".pdf");

    // ---------------------------------------------------------------------------------------------

}

