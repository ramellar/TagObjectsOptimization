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
    std::vector<int> runs = {356426, 356428, 356433, 356434, 356435, 356446, 356523, 356531, 356563, 356568, 356569, 356570, 356576, 356578, 356580, 356582, 356614, 356615, 356619, 356811, 356812, 356813, 356814, 356815, 356824, 356908, 356919, 356937, 356946, 356947, 356948, 356949, 356951, 356954, 356955, 356956, 356968, 356969, 356970, 356998, 356999, 357000, 357001, 357079, 357080, 357081, 357101, 357102, 357104, 357106, 357112, 357268, 357271, 357328, 357329, 357330, 357331, 357332, 357333, 357401, 357406, 357438, 357440, 357441, 357442, 357447, 357472, 357478, 357479, 357482, 357538, 357542, 357550, 357610, 357611, 357612, 357613, 357688, 357696, 357697, 357698, 357699, 357700, 357720, 357732, 357734, 357735, 357754, 357756, 357757, 357758, 357759, 357766, 357777, 357778, 357779, 357781, 357802, 357803, 357804, 357805, 357806, 357807, 357808, 357809, 357812, 357813, 357814, 357815, 357898, 357899, 357900, 359569, 359571, 359575, 359595, 359597, 359602, 359609, 359612, 359661, 359685, 359686, 359691, 359693, 359694, 359699, 359718, 359750, 359751, 359762, 359764, 359776, 359806, 359808, 359809, 359810, 359812, 359814, 359870, 359871, 359887, 359899, 359903, 359998, 360017, 360019, 360075, 360090, 360116, 360125, 360126, 360127, 360128, 360130, 360131, 360141, 360224, 360225, 360389, 360390, 360391, 360392, 360393, 360400, 360413, 360428, 360429, 360430, 360432, 360433, 360434, 360435, 360437, 360438, 360439, 360440, 360441, 360442, 360444, 360458, 360459, 360460, 360486, 360490, 360491};

    std::vector<int> nbunches_history = {356424, 356484, 356715, 357035, 357391, 357610, 357681, 357762, /*359246, 359336,*/ 359563, 359589, 359631, 359691, 359890, 359985, 360389}; //, 356703, 360789, 360814, 360880}
    std::vector<TString> nbunches= {"1214", "1538", "1922", "2160", "2400", "1922", "2400", "2448", "48", /*"144", "2740",*/ "578", "1154", "2448", "302", "2448", "2450", "302"}; //, "2450", "302", "2450"}

    std::vector<int> various_history = {356633, 356778, 357900, 359100, 360389};
    std::vector<TString> various = {"New L1/L2 LUTs", "HCAL PFA1'", "LHC stop", "EE+ H2O leak", "HB LUTs update"};

    std::vector<int> era_history = {356426, 357538, 359500, 360335};
    std::vector<TString> era = {"Era C", "Era D", "Era E", "Era F"};

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
    inclusive_resolVSrun->GetXaxis()->SetRangeUser(runs[0]-200, runs[runs.size()-1]+50);
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
    // barrel_resolVSrun->Draw("same LPE1");
    // endcap_resolVSrun->Draw("same LPE1");

    TLegend legend(0.45,0.8,0.68,0.88);
    legend.SetBorderSize(0);
    legend.AddEntry(inclusive_resolVSrun, "Inclusive |#eta^{Offline #tau}|<2.1", "LPE1");
    legend.AddEntry(barrel_resolVSrun, "Barrel |#eta^{Offline #tau}|<1.305", "LPE1");
    legend.AddEntry(endcap_resolVSrun, "Endcap 1.479<|#eta^{Offline #tau}|<2.1", "LPE1");
    // legend.Draw("same");

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

    for (long unsigned int j = 0; j < nbunches_history.size(); ++j)
    {
        TText* texl3 = new TText(nbunches_history[j]+35,0.41, "Nb="+nbunches[j]);
        texl3->SetTextAlign(11);
        texl3->SetTextSize(0.02);
        texl3->SetTextColor(14);
        // texl3->SetFillColor(10);
        texl3->SetTextAngle(90);
        texl3->Draw("same");

        TLine* line = new TLine(nbunches_history[j]-0.5, 0.12, nbunches_history[j]-0.5, 0.45);
        line->SetLineColor(14);
        line->SetLineWidth(1);
        line->Draw("same");
    }

    for (long unsigned int j = 0; j < various_history.size(); ++j)
    {
        TText* texl3 = new TText(various_history[j]+35,0.13, various[j]);
        texl3->SetTextAlign(11);
        texl3->SetTextSize(0.02);
        texl3->SetTextColor(1);
        // texl3->SetFillColor(10);
        texl3->SetTextAngle(90);
        texl3->Draw("same");

        TLine* line = new TLine(various_history[j]-0.5, 0.12, various_history[j]-0.5, 0.45);
        line->SetLineColor(1);
        line->SetLineWidth(1);
        line->Draw("same");
    }

    for (long unsigned int j = 0; j < era_history.size(); ++j)
    {
        TText* texl3 = new TText(era_history[j]+35,0.13, era[j]);
        texl3->SetTextAlign(11);
        texl3->SetTextSize(0.02);
        texl3->SetTextColor(2);
        // texl3->SetFillColor(10);
        texl3->SetTextAngle(90);
        texl3->Draw("same");

        TLine* line = new TLine(era_history[j]-0.5, 0.12, era_history[j]-0.5, 0.45);
        line->SetLineColor(2);
        line->SetLineWidth(1);
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
    inclusive_scaleVSrun->GetXaxis()->SetRangeUser(runs[0]-200, runs[runs.size()-1]+50);
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
    // barrel_scaleVSrun->Draw("same LPE1");
    // endcap_scaleVSrun->Draw("same LPE1");
    
    // legend.Draw("same");
    texl1->Draw("same");
    texl2->Draw("same");

    for (long unsigned int j = 0; j < nbunches_history.size(); ++j)
    {
        TText* texl3 = new TText(nbunches_history[j]+35,1.13, "Nb="+nbunches[j]);
        texl3->SetTextAlign(11);
        texl3->SetTextSize(0.02);
        texl3->SetTextColor(14);
        // texl3->SetFillColor(10);
        texl3->SetTextAngle(90);
        texl3->Draw("same");

        TLine* line = new TLine(nbunches_history[j]-0.5, 0.7, nbunches_history[j]-0.5, 1.2);
        line->SetLineColor(14);
        line->SetLineWidth(1);
        line->Draw("same");
    }

    for (long unsigned int j = 0; j < various_history.size(); ++j)
    {
        TText* texl3 = new TText(various_history[j]+35,0.73, various[j]);
        texl3->SetTextAlign(11);
        texl3->SetTextSize(0.02);
        texl3->SetTextColor(1);
        // texl3->SetFillColor(10);
        texl3->SetTextAngle(90);
        texl3->Draw("same");

        TLine* line = new TLine(various_history[j]-0.5, 0.7, various_history[j]-0.5, 1.2);
        line->SetLineColor(1);
        line->SetLineWidth(1);
        line->Draw("same");
    }

    for (long unsigned int j = 0; j < era_history.size(); ++j)
    {
        TText* texl3 = new TText(era_history[j]+35,0.73, era[j]);
        texl3->SetTextAlign(11);
        texl3->SetTextSize(0.02);
        texl3->SetTextColor(2);
        // texl3->SetFillColor(10);
        texl3->SetTextAngle(90);
        texl3->Draw("same");

        TLine* line = new TLine(era_history[j]-0.5, 0.7, era_history[j]-0.5, 1.2);
        line->SetLineColor(2);
        line->SetLineWidth(1);
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

