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
// #include "../Calibrate/ApplyCalibration.C"

using namespace std;
#include <TCanvas.h>
#include <TLegend.h>

double Median(const TH1F* histogram) {
    int nBins = histogram->GetNbinsX();
    vector<double> binContents;
    vector<double> binPosition;
    for (int i = 1; i <= nBins; ++i) {
        if (histogram->GetBinContent(i) == 0) { continue; }
        double content = histogram->GetBinContent(i);
        double position = histogram->GetBinCenter(i);
        binContents.push_back(content);
        binPosition.push_back(position);
    }
    return TMath::Median(binPosition.size(), &binPosition[0], &binContents[0]);
}

double MAD(const TH1F* histogram) {
    double median = Median(histogram);

    int nBins = histogram->GetNbinsX();
    vector<double> binContents;
    vector<double> binPosition;
    for (int i = 1; i <= nBins; ++i) {
        if (histogram->GetBinContent(i) == 0) { continue; }
        double content = histogram->GetBinContent(i);
        double position = histogram->GetBinCenter(i);
        binContents.push_back(content);
        binPosition.push_back(abs(position-median));
    }
    return TMath::Median(binPosition.size(), &binPosition[0], &binContents[0]);
}

void DrawHistogramsWithFits(std::vector<TH1F*> barrel_response_ptBins,
                            std::vector<TF1*> fit_barrel_response_ptBins,
                            std::vector<TH1F*> endcap_response_ptBins,
                            std::vector<TF1*> fit_endcap_response_ptBins) {
    TCanvas* canvas = new TCanvas("canvas", "Histograms with Fits", 1000, 1000);
    canvas->Divide(3, 4);
    
    for (size_t i = 0; i < barrel_response_ptBins.size(); ++i) {
        canvas->cd(i + 1);
        barrel_response_ptBins[i]->Draw("hist");
        fit_barrel_response_ptBins[i]->Draw("same"); 
       
    	TLegend* legend = new TLegend(0.65, 0.55, 0.85, 0.7);
    	legend->AddEntry(barrel_response_ptBins[i], "Data", "l");
    	legend->AddEntry(fit_barrel_response_ptBins[i], "Fit", "l");
    	legend->Draw();
        }
 
    canvas->Update();
    canvas->SaveAs("various/fits_FWHM_barrel.png");
    canvas->SaveAs("variuos/fits_FWHM_barrel.pdf");
   
    delete canvas;
    
    TCanvas* canvas_endcap = new TCanvas("canvas", "Histograms with Fits", 1000, 1000);
    canvas_endcap->Divide(3, 4);
    
    for (size_t i = 0; i < endcap_response_ptBins.size(); ++i) {
        canvas_endcap->cd(i + 1);
        endcap_response_ptBins[i]->Draw("hist");
        fit_endcap_response_ptBins[i]->Draw("same"); 
       
    	TLegend* legend = new TLegend(0.65, 0.55, 0.85, 0.7);
    	legend->AddEntry(endcap_response_ptBins[i], "Data", "l");
    	legend->AddEntry(fit_endcap_response_ptBins[i], "Fit", "l");
    	legend->Draw();
        }
 
    canvas_endcap->Update();
    canvas_endcap->SaveAs("various/fits_FWHM_endcap.png");
    canvas_endcap->SaveAs("variuos/fits_FWHM_endcap.pdf");
   
    delete canvas;
    }


void createHistograms(const std::vector<double>& tauPt,
                      const std::vector<double>& l1tTauPt,
                      const std::vector<double>& tauEta,
                      const std::vector<double>& tauPhi,
                      const std::vector<int>& nvtx) {

    TCanvas *canvas_tauPt = new TCanvas("canvas_tauPt", "Histograms for tauPt and L1 Pt", 800, 600);
    TCanvas *canvas_tauEta_Phi = new TCanvas("canvas_tauEta_Phi", "Histograms for tauEta and tauPhi", 800, 600);

    TH1D *hist_l1tTauPt = new TH1D("hist_l1tTauPt", "Histogram for L1 Pt", 50, 0, 50);
    hist_l1tTauPt->SetLineColor(kBlue);
    for (const auto& value : l1tTauPt) {
        hist_l1tTauPt->Fill(value);
    }
    canvas_tauPt->cd();
    hist_l1tTauPt->Draw();

    TH1D *hist_tauPt = new TH1D("hist_tauPt", "Histogram for tauPt", 50, 0, 50);
    hist_tauPt->SetLineColor(kRed);
    for (const auto& value : tauPt) {
        hist_tauPt->Fill(value);
    }
    canvas_tauPt->cd();
    hist_tauPt->Draw("SAME");

    TH1D *hist_tauEta = new TH1D("hist_tauEta", "Histogram for tauEta", 50, -3, 3);
    hist_tauEta->SetLineColor(kGreen);
    for (const auto& value : tauEta) {
        hist_tauEta->Fill(value);
    }
    canvas_tauEta_Phi->cd();
    hist_tauEta->Draw();

    TH1D *hist_tauPhi = new TH1D("hist_tauPhi", "Histogram for tauPhi", 50, -3.14, 3.14);
    hist_tauPhi->SetLineColor(kOrange);
    for (const auto& value : tauPhi) {
        hist_tauPhi->Fill(value);
    }
    canvas_tauEta_Phi->cd();
    hist_tauPhi->Draw("SAME");

    canvas_tauPt->cd();
    TLegend *legend_tauPt = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend_tauPt->AddEntry(hist_tauPt, "tauPt", "l");
    legend_tauPt->AddEntry(hist_l1tTauPt, "L1 Pt", "l");
    legend_tauPt->Draw();

    canvas_tauEta_Phi->cd();
    TLegend *legend_tauEta_Phi = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend_tauEta_Phi->AddEntry(hist_tauEta, "tauEta", "l");
    legend_tauEta_Phi->AddEntry(hist_tauPhi, "tauPhi", "l");
    legend_tauEta_Phi->Draw();

    canvas_tauPt->Update();
    canvas_tauEta_Phi->Update();
    canvas_tauPt->SaveAs("various/discarted_events_tauPt.png");
    canvas_tauEta_Phi->SaveAs("various/discarted_events_tauEta_Phi.png");
    }

 
void MakeResolutions(TString file, TString tree, int run_nmbr, TString era = "", int DecayMode = -1, float l1tTauPt_cut=0., TString method = "RMS", TString fit_option = "crystalball")
{
    std::cout << "Starting" << std::endl;
    // TString run_nmbr_str = to_string(run_nmbr);
    TString run_nmbr_str = TString::Format("%d", run_nmbr);
    if(era != "" && run_nmbr == -1) { run_nmbr_str = era; }


    TString InputFileName = file;
    TFile f(InputFileName.Data(),"READ");
    if (f.IsZombie()) {
    std::cerr << "Error: Could not open file " << InputFileName << std::endl;
    return;
    }
    TTree* inTree = (TTree*)f.Get(tree); // "Ntuplizer/TagAndProbe" "Ntuplizer_noTagAndProbe/TagAndProbe"

    if (!inTree) {
    std::cerr << "Error: Could not find tree " << tree << " in file " << InputFileName << std::endl;
    return;
    }


    Int_t   in_RunNumber =  0;
    Float_t tauPt = 0;
    Float_t tauEta = 0;
    Float_t tauPhi = 0;
    Int_t tauDM = -1;
    Float_t l1tTauPt = 0;
    Float_t l1tTauEta = 0;
    Float_t l1tTauPhi = 0;
    Int_t   l1tTauIso = 0;
    Int_t   nvtx = 0;
    inTree->SetBranchAddress("RunNumber", &in_RunNumber);
    inTree->SetBranchAddress("tauPt",     &tauPt);
    inTree->SetBranchAddress("tauEta",    &tauEta);
    inTree->SetBranchAddress("tauPhi",    &tauPhi);
    inTree->SetBranchAddress("tauDM",     &tauDM);
    inTree->SetBranchAddress("l1tPt",     &l1tTauPt);
    inTree->SetBranchAddress("l1tEta",    &l1tTauEta);
    inTree->SetBranchAddress("l1tPhi",    &l1tTauPhi);
    inTree->SetBranchAddress("l1tIso",    &l1tTauIso);
    inTree->SetBranchAddress("Nvtx",      &nvtx);

    TH1F* pt = new TH1F("pt","pt",50,0,100);
    TH1F* eta = new TH1F("eta","eta",12,-2.1,2.1);
    TH1F* l1tpt = new TH1F("l1tpt","l1tpt",50,0,100);

    std::cout << " Initializing bins" << std::endl;

    std::vector<float> ptBins        = {20, 25, 30, 35, 40, 45, 50, 60, 70, 90, 110, 130, 160, 200, 500};
    std::vector<float> etaBins       = {0., 0.5, 1.0, 1.305, 1.479, 1.8, 2.1};
    std::vector<float> signedEtaBins = {-2.1, -1.8, -1.479, -1.305, -1.0, -0.5, 0., 0.5, 1.0, 1.305, 1.479, 1.8, 2.1};

    Double_t ptBins_t[15]        = {20, 25, 30, 35, 40, 45, 50, 60, 70, 90, 110, 130, 160, 200, 500};
    // New Binning
    // Double_t ptBins_t[15]        = {20, 27, 34, 41, 48, 55, 60, 70, 90, 110, 130, 160, 200, 400, 500};
    Double_t etaBins_t[7]        = {0., 0.5, 1.0, 1.305, 1.479, 1.8, 2.1};
    Double_t signedEtaBins_t[13] = {-2.1, -1.8, -1.479, -1.305, -1.0, -0.5, 0., 0.5, 1.0, 1.305, 1.479, 1.8, 2.1};

    TH1F* empty = new TH1F("empty","empty",60,0.,3.);

    // PT RESPONSE - INCLUSIVE HISTOGRAMS

    std::cout << "Pt response histograms" << std::endl;
    TH1F* pt_response_ptInclusive = new TH1F("pt_response_ptInclusive","pt_response_ptInclusive",60,0,3);
    TH1F* pt_barrel_resp_ptInclusive = new TH1F("pt_barrel_resp_ptInclusive","pt_barrel_resp_ptInclusive",60,0,3);
    TH1F* pt_endcap_resp_ptInclusive = new TH1F("pt_endcap_resp_ptInclusive","pt_endcap_resp_ptInclusive",60,0,3);

    // PT RESPONSE - PT BINS HISTOGRAMS
    std::vector<TH1F*> response_ptBins = {};
    std::vector<TH1F*> barrel_response_ptBins = {};
    std::vector<TH1F*> endcap_response_ptBins = {};
    for(long unsigned int i = 0; i < ptBins.size()-1; ++i)
    {
        TString lowP;
        lowP.Form("%.1f", ptBins[i]);
        TString highP;
        highP.Form("%.1f", ptBins[i+1]);
        response_ptBins.push_back(new TH1F("pt_resp_ptBin"+lowP+"to"+highP,"pt_resp_ptBin"+lowP+"to"+highP,60,0,3));
        barrel_response_ptBins.push_back(new TH1F("pt_barrel_resp_ptBin"+lowP+"to"+highP,"pt_barrel_resp_ptBin"+lowP+"to"+highP,60,0,3));
        endcap_response_ptBins.push_back(new TH1F("pt_endcap_resp_ptBin"+lowP+"to"+highP,"pt_endcap_resp_ptBin"+lowP+"to"+highP,60,0,3));
    }

    // FIT PT RESPONSE - PT BINS HISTOGRAMS
    std::vector<TF1*> fit_response_ptBins = {};
    std::vector<TF1*> fit_barrel_response_ptBins = {};
    std::vector<TF1*> fit_endcap_response_ptBins = {};
    for(long unsigned int i = 0; i < ptBins.size()-1; ++i)
    {
        TString lowP;
        lowP.Form("%.1f", ptBins[i]);
        TString highP;
        highP.Form("%.1f", ptBins[i+1]);
        fit_response_ptBins.push_back(new TF1("fit_pt_resp_ptBin"+lowP+"to"+highP, fit_option, 0, 3));
        fit_barrel_response_ptBins.push_back(new TF1("fit_pt_barrel_resp_ptBin"+lowP+"to"+highP, fit_option, 0, 3));
        fit_endcap_response_ptBins.push_back(new TF1("fit_pt_endcap_resp_ptBin"+lowP+"to"+highP, fit_option, 0, 3));
    }


    // PT RESPONSE -  ETA BINS HISTIGRAMS
    std::vector<TH1F*> absEta_response_ptBins = {};
    std::vector<TH1F*> minusEta_response_ptBins = {};
    std::vector<TH1F*> plusEta_response_ptBins = {};
    for(long unsigned int i = 0; i < etaBins.size()-1; ++i)
    {
        TString lowE;
        lowE.Form("%.3f", etaBins[i]);
        TString highE;
        highE.Form("%.3f", etaBins[i+1]);
        absEta_response_ptBins.push_back(new TH1F("pt_resp_AbsEtaBin"+lowE+"to"+highE,"pt_resp_AbsEtaBin"+lowE+"to"+highE,60,0,3));
        minusEta_response_ptBins.push_back(new TH1F("pt_resp_MinusEtaBin"+lowE+"to"+highE,"pt_resp_MinusEtaBin"+lowE+"to"+highE,60,0,3));
        plusEta_response_ptBins.push_back(new TH1F("pt_resp_PlusEtaBin"+lowE+"to"+highE,"pt_resp_PlusEtaBin"+lowE+"to"+highE,60,0,3));
    }

    // FIT PT RESPONSE -  ETA BINS HISTIGRAMS
    std::vector<TF1*> fit_absEta_response_ptBins = {};
    std::vector<TF1*> fit_minusEta_response_ptBins = {};
    std::vector<TF1*> fit_plusEta_response_ptBins = {};
    for(long unsigned int i = 0; i < etaBins.size()-1; ++i)
    {
        TString lowE;
        lowE.Form("%.3f", etaBins[i]);
        TString highE;
        highE.Form("%.3f", etaBins[i+1]);
        fit_absEta_response_ptBins.push_back(new TF1("fit_pt_resp_AbsEtaBin"+lowE+"to"+highE, fit_option, 0, 3));
        fit_minusEta_response_ptBins.push_back(new TF1("fit_pt_resp_MinusEtaBin"+lowE+"to"+highE, fit_option, 0, 3));
        fit_plusEta_response_ptBins.push_back(new TF1("fit_pt_resp_PlusEtaBin"+lowE+"to"+highE, fit_option, 0, 3));
    }

    std::vector<TH1F*> pt_resp_PtEtaBin = {};
    for(long unsigned int i = 0; i < ptBins.size()-1; ++i)
    {
        for(long unsigned int j = 0; j < etaBins.size()-1; ++j)
        {
            TString lowP;
            lowP.Form("%.1f", ptBins[i]);
            TString lowE ;
            lowE.Form("%.3f", etaBins[j]);
            TString highP;
            highP.Form("%.1f", ptBins[i+1]);
            TString highE;
            highE.Form("%.3f", etaBins[j+1]);
            pt_resp_PtEtaBin.push_back(new TH1F("pt_resp_PtBin"+lowP+"to"+highP+"_EtaBin"+lowE+"to"+highE,"pt_resp_PtBin"+lowP+"to"+highP+"_EtaBin"+lowE+"to"+highE,60,0,3));
        }
    }

    // ETA AND PHI RESPONSES
    TH1F* eta_resp_barrel = new TH1F("eta_resp_barrel","eta_resp_barrel",200,-1,1);
    TH1F* eta_resp_endcap = new TH1F("eta_resp_endcap","eta_resp_endcap",200,-1,1);
    TH1F* eta_resp_inclusive = new TH1F("eta_resp_inclusive","eta_resp_inclusive",200,-1,1);

    TH1F* phi_resp_barrel = new TH1F("phi_resp_barrel","phi_resp_barrel",200,-1,1);
    TH1F* phi_resp_endcap = new TH1F("phi_resp_endcap","phi_resp_endcap",200,-1,1);
    TH1F* phi_resp_inclusive = new TH1F("phi_resp_inclusive","phi_resp_inclusive",200,-1,1);

    TH1F* l1tau_histo = new TH1F("l1tau","l1tau",20,30,100);
    TH1F* tauPt_histo = new TH1F("tauPt","tauPt",20,30,100);
    // ADDITIONAL USEFULL PLOTS
    TH1F* Nvtx = new TH1F("Nvtx","Nvtx",70,0,70);
    // std::vector<double> collection_tauPt;
    // std::vector<double> collection_l1tTauPt;
    // std::vector<double> collection_tauEta;
    // std::vector<double> collection_tauPhi;
    // std::vector<int> collection_nvtx;

    for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    {
        inTree->GetEntry(i);

        // either process the full dataset or just the events with a specific run number
        if (run_nmbr != -1) { if (run_nmbr != in_RunNumber)  { continue; } }

        if(l1tTauPt<l1tTauPt_cut) { continue; }  // modify here to change tail in unpacked

        if(l1tTauPt>128.) { continue; } // skip saturated objects
        if(DecayMode != -1) {
            if (tauDM != DecayMode) { continue; }
        }
        // if(l1tTauIso<1) { continue; } // isolated taus (l1tTauIso = 1)
        // if(l1tTauPt/tauPt<0.6)
        // { 
        //     pt->Fill(tauPt);
        //     eta->Fill(tauEta);
        //     l1tpt->Fill(l1tTauPt);
        //     continue;
        // }

        Nvtx->Fill(nvtx);
        // if (nvtx < 30 and nvtx > 40) { continue; }

        // fill inclusive distributions skipping low energy taus
        if(tauPt<30)
        {
            pt_response_ptInclusive->Fill(l1tTauPt/tauPt);
            eta_resp_inclusive->Fill(l1tTauEta - tauEta);
            phi_resp_inclusive->Fill(l1tTauPhi - tauPhi);
            if(abs(tauEta) < 1.305)
            {
                pt_barrel_resp_ptInclusive->Fill(l1tTauPt/tauPt);
                eta_resp_barrel->Fill(l1tTauEta - tauEta);
                phi_resp_barrel->Fill(l1tTauPhi - tauPhi);
            }
            else if (abs(tauEta) < 2.1 and abs(tauEta) > 1.479)
            {
                pt_endcap_resp_ptInclusive->Fill(l1tTauPt/tauPt);
                eta_resp_endcap->Fill(l1tTauEta - tauEta);
                phi_resp_endcap->Fill(l1tTauPhi - tauPhi);
            }
        }

        for(long unsigned int i = 0; i < ptBins.size()-1; ++i)
        {
            if(tauPt > ptBins[i] and tauPt <= ptBins[i+1])
            {
                // if (l1tTauPt/tauPt < 0.5 or l1tTauPt/tauPt >= 1.5) { 
                //     // cout << tauPt << ", L1 Pt" << l1tTauPt << ", eta" << tauEta << ", phi" << tauPhi << ", vertices" << nvtx << " => bad event" << endl;
                //     collection_tauPt.push_back(tauPt);
                //     collection_l1tTauPt.push_back(l1tTauPt);
                //     collection_tauEta.push_back(tauEta);
                //     collection_tauPhi.push_back(tauPhi);
                //     collection_nvtx.push_back(nvtx);
                //     //continue; 
                //     }
                response_ptBins[i]->Fill(l1tTauPt/tauPt);
                if (tauPt > 30 and tauPt < 50) {
                   l1tau_histo->Fill(l1tTauPt);
                   tauPt_histo->Fill(tauPt);
                }
                if (abs(tauEta) < 1.305) { barrel_response_ptBins[i]->Fill(l1tTauPt/tauPt); }
                else if (abs(tauEta) < 2.1 and abs(tauEta) > 1.479) { endcap_response_ptBins[i]->Fill(l1tTauPt/tauPt); }
            }
        }

        for(long unsigned int i = 0; i < etaBins.size()-1; ++i)
        {
            if(abs(tauEta) > etaBins[i] and abs(tauEta) < etaBins[i+1]) { absEta_response_ptBins[i]->Fill(l1tTauPt/tauPt); }

            if(tauEta > etaBins[i] and tauEta < etaBins[i+1]) { plusEta_response_ptBins[i]->Fill(l1tTauPt/tauPt); }
            else if (tauEta < -etaBins[i] and tauEta > -etaBins[i+1]) { minusEta_response_ptBins[i]->Fill(l1tTauPt/tauPt); }
        }
        
        int k = 0;
        for(long unsigned int i = 0; i < ptBins.size()-1; ++i)
        {
            for(long unsigned int j = 0; j < etaBins.size()-1; ++j)
            {
                if (abs(tauEta) > etaBins[j] and abs(tauEta) < etaBins[j+1] and tauPt > ptBins[i] and tauPt < ptBins[i+1])
                {
                    pt_resp_PtEtaBin[k]->Fill(l1tTauPt/tauPt);
                }

                k += 1;
            }
        }
    }

    // scale everything to unity
    pt_response_ptInclusive->Scale(1.0/pt_response_ptInclusive->Integral());
    pt_barrel_resp_ptInclusive->Scale(1.0/pt_barrel_resp_ptInclusive->Integral());
    pt_endcap_resp_ptInclusive->Scale(1.0/pt_endcap_resp_ptInclusive->Integral());
    eta_resp_barrel->Scale(1.0/eta_resp_barrel->Integral());
    eta_resp_endcap->Scale(1.0/eta_resp_endcap->Integral());
    eta_resp_inclusive->Scale(1.0/eta_resp_inclusive->Integral());
    phi_resp_barrel->Scale(1.0/phi_resp_barrel->Integral());
    phi_resp_endcap->Scale(1.0/phi_resp_endcap->Integral());
    phi_resp_inclusive->Scale(1.0/phi_resp_inclusive->Integral());

    for(long unsigned int i = 0; i < ptBins.size()-1; ++i)
    {
        response_ptBins[i]->Scale(1.0/response_ptBins[i]->Integral());
        barrel_response_ptBins[i]->Scale(1.0/barrel_response_ptBins[i]->Integral());
        endcap_response_ptBins[i]->Scale(1.0/endcap_response_ptBins[i]->Integral());
    }

    for(long unsigned int i = 0; i < etaBins.size()-1; ++i)
    {
        minusEta_response_ptBins[i]->Scale(1.0/minusEta_response_ptBins[i]->Integral());
        plusEta_response_ptBins[i]->Scale(1.0/plusEta_response_ptBins[i]->Integral());
        absEta_response_ptBins[i]->Scale(1.0/absEta_response_ptBins[i]->Integral());
    }

    // perform fits
    for(long unsigned int i = 0; i < ptBins.size()-1; ++i)
    {
        if (fit_option=="gauss")
        {
            fit_response_ptBins[i]->SetParameters(response_ptBins[i]->GetBinContent(20), 1, response_ptBins[i]->GetRMS());
            fit_barrel_response_ptBins[i]->SetParameters(barrel_response_ptBins[i]->GetBinContent(20), 1, barrel_response_ptBins[i]->GetRMS());
            fit_endcap_response_ptBins[i]->SetParameters(endcap_response_ptBins[i]->GetBinContent(20), 1, endcap_response_ptBins[i]->GetRMS());
        }
        if (fit_option=="crystalball")
        { 
            fit_response_ptBins[i]->SetParameters(response_ptBins[i]->GetBinContent(20), 1, response_ptBins[i]->GetRMS(), 1, 1);
            fit_barrel_response_ptBins[i]->SetParameters(barrel_response_ptBins[i]->GetBinContent(20), 1, barrel_response_ptBins[i]->GetRMS(), 1, 1);
            fit_endcap_response_ptBins[i]->SetParameters(endcap_response_ptBins[i]->GetBinContent(20), 1, endcap_response_ptBins[i]->GetRMS(), 1, 1);
        }

        response_ptBins[i]->Fit(fit_response_ptBins[i]);
        barrel_response_ptBins[i]->Fit(fit_barrel_response_ptBins[i]);
        endcap_response_ptBins[i]->Fit(fit_endcap_response_ptBins[i]);
    }

    // DrawHistogramsWithFits(barrel_response_ptBins, fit_barrel_response_ptBins,
    //                        endcap_response_ptBins, fit_endcap_response_ptBins);
    // createHistograms(collection_tauPt, collection_l1tTauPt, collection_tauEta, collection_tauPhi, collection_nvtx);
    
    for(long unsigned int i = 0; i < etaBins.size()-1; ++i)
    {
        if (fit_option=="gauss")
        {
            fit_minusEta_response_ptBins[i]->SetParameters(minusEta_response_ptBins[i]->GetBinContent(20), 1, minusEta_response_ptBins[i]->GetRMS());
            fit_plusEta_response_ptBins[i]->SetParameters(plusEta_response_ptBins[i]->GetBinContent(20), 1, plusEta_response_ptBins[i]->GetRMS());
            fit_absEta_response_ptBins[i]->SetParameters(absEta_response_ptBins[i]->GetBinContent(20), 1, absEta_response_ptBins[i]->GetRMS());
        }
        if (fit_option=="crystalball")
        { 
            fit_minusEta_response_ptBins[i]->SetParameters(minusEta_response_ptBins[i]->GetBinContent(20), 1, minusEta_response_ptBins[i]->GetRMS(), 1, 1);
            fit_plusEta_response_ptBins[i]->SetParameters(plusEta_response_ptBins[i]->GetBinContent(20), 1, plusEta_response_ptBins[i]->GetRMS(), 1, 1);
            fit_absEta_response_ptBins[i]->SetParameters(absEta_response_ptBins[i]->GetBinContent(20), 1, absEta_response_ptBins[i]->GetRMS(), 1, 1);
        }

        minusEta_response_ptBins[i]->Fit(fit_minusEta_response_ptBins[i]);
        plusEta_response_ptBins[i]->Fit(fit_plusEta_response_ptBins[i]);
        absEta_response_ptBins[i]->Fit(fit_absEta_response_ptBins[i]);
    }

    // make resolution plots
    TH1F* pt_resol_fctPt = new TH1F("pt_resol_fctPt","pt_resol_fctPt",ptBins.size()-1, ptBins_t);
    TH1F* pt_resol_barrel_fctPt = new TH1F("pt_resol_barrel_fctPt","pt_resol_barrel_fctPt",ptBins.size()-1, ptBins_t);
    TH1F* pt_resol_endcap_fctPt = new TH1F("pt_resol_endcap_fctPt","pt_resol_endcap_fctPt",ptBins.size()-1, ptBins_t);
    TH1F* pt_resol_fctAbsEta = new TH1F("pt_resol_fctAbsEta","pt_resol_fctAbsEta",etaBins.size()-1, etaBins_t);
    TH1F* pt_resol_fctEta = new TH1F("pt_resol_fctEta","pt_resol_fctEta",signedEtaBins.size()-1, signedEtaBins_t);
    TH1F* pt_scale_fctPt = new TH1F("pt_scale_fctPt","pt_scale_fctPt",ptBins.size()-1, ptBins_t);
    TH1F* pt_scale_fctEta = new TH1F("pt_scale_fctEta","pt_scale_fctEta",signedEtaBins.size()-1, signedEtaBins_t);
    TH2F* PTvsETA_resolution = new TH2F("PTvsETA_resolution","PTvsETA_resolution",ptBins.size()-1, ptBins_t ,etaBins.size()-1, etaBins_t);
    TH2F* PTvsETA_scale = new TH2F("PTvsETA_events","PTvsETA_events",ptBins.size()-1, ptBins_t ,etaBins.size()-1, etaBins_t);


    double median_barrel;
    double mad_barrel;
    double median_endcap;
    double mad_endcap;
    for(long unsigned int i = 0; i < ptBins.size()-1; ++i){
        
        std::cout << "Processing ptBin " << i << std::endl;

        if (response_ptBins[i] && response_ptBins[i]->GetMean() != 0.0) {
        // Safe to access the histogram
        }

        if (response_ptBins.size() != ptBins.size() - 1) {
            std::cerr << "Error: Mismatched array sizes!" << std::endl;
            return;
        }
        
        std::cout << "Median Barrel: " << median_barrel << ", MAD Barrel: " << mad_barrel << std::endl;

        if (method == "median")
        {
        if (response_ptBins[i]->GetMean() != 0.0) {median_barrel = Median(barrel_response_ptBins[i]);}
        if (response_ptBins[i]->GetMean() != 0.0) {mad_barrel = MAD(barrel_response_ptBins[i]);}
        if (response_ptBins[i]->GetMean() != 0.0) {median_endcap = Median(endcap_response_ptBins[i]);}
        if (response_ptBins[i]->GetMean() != 0.0) {mad_endcap = MAD(endcap_response_ptBins[i]);}
        
        std::cout << "Median " << median_barrel << " MAD: " << mad_barrel << std::endl;
        pt_scale_fctPt->SetBinContent(i+1, response_ptBins[i]->GetMean());
        pt_scale_fctPt->SetBinError(i+1, response_ptBins[i]->GetMeanError());

        if (response_ptBins[i]->GetMean() != 0.0) { pt_resol_fctPt->SetBinContent(i+1, response_ptBins[i]->GetRMS()/response_ptBins[i]->GetMean()); }
        if (response_ptBins[i]->GetMean() != 0.0) { pt_resol_fctPt->SetBinError(i+1, response_ptBins[i]->GetRMSError()/response_ptBins[i]->GetMean()); }

        if (barrel_response_ptBins[i]->GetMean() != 0.0) { pt_resol_barrel_fctPt->SetBinContent(i+1, mad_barrel/median_barrel); }
        if (barrel_response_ptBins[i]->GetMean() != 0.0) { pt_resol_barrel_fctPt->SetBinError(i+1, barrel_response_ptBins[i]->GetRMSError()/barrel_response_ptBins[i]->GetMean()); }
        
        if (endcap_response_ptBins[i]->GetMean() != 0.0) { pt_resol_endcap_fctPt->SetBinContent(i+1, mad_endcap/median_endcap); }
        if (endcap_response_ptBins[i]->GetMean() != 0.0) { pt_resol_endcap_fctPt->SetBinError(i+1, endcap_response_ptBins[i]->GetRMSError()/endcap_response_ptBins[i]->GetMean()); }
        }
        else 
        {
        pt_scale_fctPt->SetBinContent(i+1, response_ptBins[i]->GetMean());
        pt_scale_fctPt->SetBinError(i+1, response_ptBins[i]->GetMeanError());

        if (response_ptBins[i]->GetMean() != 0.0) { pt_resol_fctPt->SetBinContent(i+1, response_ptBins[i]->GetRMS()/response_ptBins[i]->GetMean()); }
        if (response_ptBins[i]->GetMean() != 0.0) { pt_resol_fctPt->SetBinError(i+1, response_ptBins[i]->GetRMSError()/response_ptBins[i]->GetMean()); }

        if (barrel_response_ptBins[i]->GetMean() != 0.0) { pt_resol_barrel_fctPt->SetBinContent(i+1, barrel_response_ptBins[i]->GetRMS()/barrel_response_ptBins[i]->GetMean()); }
        if (barrel_response_ptBins[i]->GetMean() != 0.0) { pt_resol_barrel_fctPt->SetBinError(i+1, barrel_response_ptBins[i]->GetRMSError()/barrel_response_ptBins[i]->GetMean()); }

        if (endcap_response_ptBins[i]->GetMean() != 0.0) { pt_resol_endcap_fctPt->SetBinContent(i+1, endcap_response_ptBins[i]->GetRMS()/endcap_response_ptBins[i]->GetMean()); }
        if (endcap_response_ptBins[i]->GetMean() != 0.0) { pt_resol_endcap_fctPt->SetBinError(i+1, endcap_response_ptBins[i]->GetRMSError()/endcap_response_ptBins[i]->GetMean()); }
        }
    }

    for(long unsigned int i = 0; i < etaBins.size()-1; ++i)
    {
        pt_scale_fctEta->SetBinContent(etaBins.size()-1-i, minusEta_response_ptBins[i]->GetMean());
        pt_scale_fctEta->SetBinError(etaBins.size()-1-i, minusEta_response_ptBins[i]->GetMeanError());
        pt_scale_fctEta->SetBinContent(i+etaBins.size(), plusEta_response_ptBins[i]->GetMean());
        pt_scale_fctEta->SetBinError(i+etaBins.size(), plusEta_response_ptBins[i]->GetMeanError());

        if (minusEta_response_ptBins[i]->GetMean() != 0.0) { pt_resol_fctEta->SetBinContent(etaBins.size()-1-i, minusEta_response_ptBins[i]->GetRMS()*2.35/minusEta_response_ptBins[i]->GetMean()); }
        if (minusEta_response_ptBins[i]->GetMean() != 0.0) { pt_resol_fctEta->SetBinError(etaBins.size()-1-i, minusEta_response_ptBins[i]->GetRMSError()/minusEta_response_ptBins[i]->GetMean()); }
        if (plusEta_response_ptBins[i]->GetMean() != 0.0) { pt_resol_fctEta->SetBinContent(i+etaBins.size(), plusEta_response_ptBins[i]->GetRMS()*2.35/plusEta_response_ptBins[i]->GetMean()); }
        if (plusEta_response_ptBins[i]->GetMean() != 0.0) { pt_resol_fctEta->SetBinError(i+etaBins.size(), plusEta_response_ptBins[i]->GetRMSError()/plusEta_response_ptBins[i]->GetMean()); }
    }

    int k = 0;
    for(long unsigned int i = 0; i < ptBins.size()-1; ++i)
    {
        for(long unsigned int j = 0; j < etaBins.size()-1; ++j)
        {
            if (pt_resp_PtEtaBin[k]->GetMean() != 0.)
            {
                if (pt_resp_PtEtaBin[k]->GetMean() != 0.0) { PTvsETA_resolution->SetBinContent(i,j,pt_resp_PtEtaBin[k]->GetRMS()*2.35/pt_resp_PtEtaBin[k]->GetMean()); }
                if (pt_resp_PtEtaBin[k]->GetMean() != 0.0) { PTvsETA_resolution->SetBinError(i,j,pt_resp_PtEtaBin[k]->GetRMSError()/pt_resp_PtEtaBin[k]->GetMean()); }
                
                PTvsETA_scale->SetBinContent(i,j,pt_resp_PtEtaBin[k]->GetMean());
                PTvsETA_scale->SetBinError(i,j,pt_resp_PtEtaBin[k]->GetMeanError());
            }

            k += 1;
        }
    }

    // make resolution plots from fits
    TH1F* fit_pt_resol_fctPt = new TH1F("fit_pt_resol_fctPt","fit_pt_resol_fctPt",ptBins.size()-1, ptBins_t);
    TH1F* fit_pt_resol_barrel_fctPt = new TH1F("fit_pt_resol_barrel_fctPt","fit_pt_resol_barrel_fctPt",ptBins.size()-1, ptBins_t);
    TH1F* fit_pt_resol_endcap_fctPt = new TH1F("fit_pt_resol_endcap_fctPt","fit_pt_resol_endcap_fctPt",ptBins.size()-1, ptBins_t);
    TH1F* fit_pt_resol_fctAbsEta = new TH1F("fit_pt_resol_fctAbsEta","fit_pt_resol_fctAbsEta",etaBins.size()-1, etaBins_t);
    TH1F* fit_pt_resol_fctEta = new TH1F("fit_pt_resol_fctEta","fit_pt_resol_fctEta",signedEtaBins.size()-1, signedEtaBins_t);
    TH1F* fit_pt_scale_fctPt = new TH1F("fit_pt_scale_fctPt","fit_pt_scale_fctPt",ptBins.size()-1, ptBins_t);
    TH1F* fit_pt_scale_fctEta = new TH1F("fit_pt_scale_fctEta","fit_pt_scale_fctEta",signedEtaBins.size()-1, signedEtaBins_t);

    for(long unsigned int i = 0; i < ptBins.size()-1; ++i)
    {
        fit_pt_scale_fctPt->SetBinContent(i+1, fit_response_ptBins[i]->GetParameter("Mean"));
        fit_pt_scale_fctPt->SetBinError(i+1, fit_response_ptBins[i]->GetParError(2));

        fit_pt_resol_fctPt->SetBinContent(i+1, fit_response_ptBins[i]->GetParameter("Sigma")/fit_response_ptBins[i]->GetParameter("Mean"));
        fit_pt_resol_fctPt->SetBinError(i+1, fit_response_ptBins[i]->GetParError(3)/fit_response_ptBins[i]->GetParameter("Mean"));

        fit_pt_resol_barrel_fctPt->SetBinContent(i+1, fit_barrel_response_ptBins[i]->GetParameter("Sigma")/fit_barrel_response_ptBins[i]->GetParameter("Mean"));
        fit_pt_resol_barrel_fctPt->SetBinError(i+1, fit_barrel_response_ptBins[i]->GetParError(3)/fit_barrel_response_ptBins[i]->GetParameter("Mean"));

        fit_pt_resol_endcap_fctPt->SetBinContent(i+1, fit_endcap_response_ptBins[i]->GetParameter("Sigma")/fit_endcap_response_ptBins[i]->GetParameter("Mean"));
        fit_pt_resol_endcap_fctPt->SetBinError(i+1, fit_endcap_response_ptBins[i]->GetParError(3)/fit_endcap_response_ptBins[i]->GetParameter("Mean"));
    }

    for(long unsigned int i = 0; i < etaBins.size()-1; ++i)
    {
        fit_pt_scale_fctEta->SetBinContent(etaBins.size()-1-i, fit_minusEta_response_ptBins[i]->GetParameter("Mean"));
        fit_pt_scale_fctEta->SetBinError(etaBins.size()-1-i, fit_minusEta_response_ptBins[i]->GetParError(2));
        fit_pt_scale_fctEta->SetBinContent(i+etaBins.size(), fit_plusEta_response_ptBins[i]->GetParameter("Mean"));
        fit_pt_scale_fctEta->SetBinError(i+etaBins.size(), fit_plusEta_response_ptBins[i]->GetParError(2));

        fit_pt_resol_fctEta->SetBinContent(etaBins.size()-1-i, fit_minusEta_response_ptBins[i]->GetParameter("Sigma")/fit_minusEta_response_ptBins[i]->GetParameter("Mean"));
        fit_pt_resol_fctEta->SetBinError(etaBins.size()-1-i, fit_minusEta_response_ptBins[i]->GetParError(3)/fit_minusEta_response_ptBins[i]->GetParameter("Mean"));
        fit_pt_resol_fctEta->SetBinContent(i+etaBins.size(), fit_plusEta_response_ptBins[i]->GetParameter("Sigma")/fit_plusEta_response_ptBins[i]->GetParameter("Mean"));
        fit_pt_resol_fctEta->SetBinError(i+etaBins.size(), fit_plusEta_response_ptBins[i]->GetParError(3)/fit_plusEta_response_ptBins[i]->GetParameter("Mean"));
    }

    // ----------------------------------------------------------------------------    
    // save in root file for future necessity
    TFile* fileout = new TFile("ROOTs/ROOTs_2024/resolutions_of_Run"+run_nmbr_str+"_unpacked.root","RECREATE");
    pt_scale_fctPt->Write();
    pt_scale_fctEta->Write();
    pt_resol_fctPt->Write();
    pt_resol_barrel_fctPt->Write();
    pt_resol_endcap_fctPt->Write();
    pt_resol_fctAbsEta->Write();
    pt_resol_fctEta->Write();
    pt_response_ptInclusive->Write();
    pt_barrel_resp_ptInclusive->Write();
    pt_endcap_resp_ptInclusive->Write();
    eta_resp_barrel->Write();
    eta_resp_endcap->Write();
    eta_resp_inclusive->Write();
    phi_resp_barrel->Write();
    phi_resp_endcap->Write();
    phi_resp_inclusive->Write();
    PTvsETA_resolution->Write();
    PTvsETA_scale->Write();
    l1tau_histo->Write();
    for(long unsigned int i = 0; i < ptBins.size()-1; ++i)
    {
        response_ptBins[i]->Write();
        barrel_response_ptBins[i]->Write();
        endcap_response_ptBins[i]->Write();
    }
    for(long unsigned int i = 0; i < etaBins.size()-1; ++i)
    {
        absEta_response_ptBins[i]->Write();
        minusEta_response_ptBins[i]->Write();
        plusEta_response_ptBins[i]->Write();
    }
    Nvtx->Write();
    fileout->Close();
/*
    // ----------------------------------------------------------------------------
    // plot performance

    gStyle->SetOptStat(000000);

    for(long unsigned int i = 0; i < ptBins.size()-1; ++i)
    {
        TString lowP ;
        lowP.Form("%.1f", ptBins[i]);
        TString highP;
        highP.Form("%.1f", ptBins[i+1]);

        TCanvas canvas("c","c",800,800);
        canvas.SetLeftMargin(0.15);
        canvas.SetGrid();
        // canvas.SetLogy();

        // use dummy histogram to define style
        empty->GetXaxis()->SetTitle("E_{T}^{L1 #tau} / p_{T}^{Offline #tau}");
        empty->SetTitle("");

        // empty->GetXaxis()->SetRangeUser(0.,250.);
        empty->GetYaxis()->SetRangeUser(0., max(barrel_response_ptBins[i]->GetMaximum(),endcap_response_ptBins[i]->GetMaximum())*1.3 );

        empty->GetXaxis()->SetTitleOffset(1.3);
        empty->GetYaxis()->SetTitle("a.u.");
        empty->GetYaxis()->SetTitleOffset(1.3);
        empty->SetTitle("");
        empty->SetStats(0);

        empty->Draw();

        barrel_response_ptBins[i]->SetLineWidth(2);
        barrel_response_ptBins[i]->SetLineColor(1);
        barrel_response_ptBins[i]->SetMarkerStyle(8);
        barrel_response_ptBins[i]->SetMarkerColor(1);

        endcap_response_ptBins[i]->SetLineWidth(2);
        endcap_response_ptBins[i]->SetLineColor(2);
        endcap_response_ptBins[i]->SetMarkerStyle(8);
        endcap_response_ptBins[i]->SetMarkerColor(2);

        barrel_response_ptBins[i]->Draw("same");
        endcap_response_ptBins[i]->Draw("same");

        fit_barrel_response_ptBins[i]->SetLineWidth(2);
        fit_barrel_response_ptBins[i]->SetLineColor(1);

        fit_endcap_response_ptBins[i]->SetLineWidth(2);
        fit_endcap_response_ptBins[i]->SetLineColor(2);

        fit_barrel_response_ptBins[i]->Draw("same");
        fit_endcap_response_ptBins[i]->Draw("same");

        TPaveText* texl1 = new TPaveText(0.27,0.87,0.2,0.99,"NDC");
        texl1->AddText("CMS Internal");
        texl1->SetTextSize(0.03);
        texl1->SetFillStyle(0);
        texl1->SetBorderSize(0);
        texl1->Draw("same");

        TPaveText* texl2 = new TPaveText(0.60,0.87,0.89,0.99,"NDC");
        if (run_nmbr != -1) { texl2->AddText("Run "+run_nmbr_str+" (13.6 TeV)"); }
        else                { texl2->AddText("Runs 2022 (13.6 TeV)"); }
        texl2->SetTextSize(0.03);
        texl2->SetFillStyle(0);
        texl2->SetBorderSize(0);
        texl2->Draw("same");

        TLegend legend(0.55,0.75,0.88,0.88);
        legend.SetBorderSize(0);
        legend.SetHeader(lowP+" < p_{T}^{Offline #tau} < "+highP+" [GeV]");
        legend.AddEntry(barrel_response_ptBins[i],"Barrel |#eta^{Offline #tau}|<1.305","LPE");
        legend.AddEntry(endcap_response_ptBins[i],"Endcap 1.479<|#eta^{Offline #tau}|<2.1","LPE");
        legend.Draw("same");

        canvas.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/response_"+lowP+"pt"+highP+".pdf");
    }

    for(long unsigned int i = 0; i < etaBins.size()-1; ++i)
    {
        TString lowE;
        lowE.Form("%.3f", etaBins[i]);
        TString highE;
        highE.Form("%.3f", etaBins[i+1]);

        TCanvas canvas("c","c",800,800);
        canvas.SetLeftMargin(0.15);
        canvas.SetGrid();
        // canvas.SetLogy();

        // use dummy histogram to define style
        empty->GetXaxis()->SetTitle("E_{T}^{L1 #tau} / p_{T}^{Offline #tau}");
        empty->SetTitle("");

        // empty->GetXaxis()->SetRangeUser(0.,250.);
        empty->GetYaxis()->SetRangeUser(0., absEta_response_ptBins[i]->GetMaximum()*1.3 );

        empty->GetXaxis()->SetTitleOffset(1.3);
        empty->GetYaxis()->SetTitle("a.u.");
        empty->GetYaxis()->SetTitleOffset(1.3);
        empty->SetTitle("");
        empty->SetStats(0);

        empty->Draw();

        absEta_response_ptBins[i]->SetLineWidth(2);
        absEta_response_ptBins[i]->SetLineColor(1);
        absEta_response_ptBins[i]->SetMarkerStyle(8);
        absEta_response_ptBins[i]->SetMarkerColor(1);

        absEta_response_ptBins[i]->Draw("same");

        fit_absEta_response_ptBins[i]->SetLineWidth(2);
        fit_absEta_response_ptBins[i]->SetLineColor(1);

        fit_absEta_response_ptBins[i]->Draw("same");

        TLegend legend(0.55,0.75,0.88,0.88);
        legend.SetBorderSize(0);
        legend.AddEntry(absEta_response_ptBins[i],lowE+"<|#eta^{Offline #tau}|<"+highE,"LPE");
        legend.Draw("same");

        TPaveText* texl1 = new TPaveText(0.27,0.87,0.2,0.99,"NDC");
        texl1->AddText("CMS Internal");
        texl1->SetTextSize(0.03);
        texl1->SetFillStyle(0);
        texl1->SetBorderSize(0);
        texl1->Draw("same");

        TPaveText* texl2 = new TPaveText(0.60,0.87,0.89,0.99,"NDC");
        if (run_nmbr != -1) { texl2->AddText("Run "+run_nmbr_str+" (13.6 TeV)"); }
        else                { texl2->AddText("Runs 2022 (13.6 TeV)"); }
        texl2->SetTextSize(0.03);
        texl2->SetFillStyle(0);
        texl2->SetBorderSize(0);
        texl2->Draw("same");

        canvas.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/response_"+lowE+"eta"+highE+".pdf");
    }

    // ##############

    TCanvas canvas("c","c",800,800);
    canvas.SetLeftMargin(0.15);
    canvas.SetGrid();
    // canvas.SetLogy();

    pt_resol_endcap_fctPt->GetXaxis()->SetTitle("p^{Offline #tau}_{T} [GeV]");
    pt_resol_endcap_fctPt->SetTitle("");
    pt_resol_endcap_fctPt->GetXaxis()->SetTitleOffset(1.3);
    pt_resol_endcap_fctPt->GetYaxis()->SetTitle("E^{L1 #tau}_{T} resolution");
    pt_resol_endcap_fctPt->GetYaxis()->SetTitleOffset(1.5);
    pt_resol_endcap_fctPt->SetTitle("");
    pt_resol_endcap_fctPt->SetStats(0);

    pt_resol_endcap_fctPt->GetXaxis()->SetRangeUser(20.,100.);
    // pt_resol_endcap_fctPt->GetYaxis()->SetRangeUser(0.12,0.36);

    pt_resol_barrel_fctPt->SetLineWidth(2);
    pt_resol_barrel_fctPt->SetLineColor(1);
    pt_resol_barrel_fctPt->SetMarkerStyle(8);
    pt_resol_barrel_fctPt->SetMarkerColor(1);

    pt_resol_endcap_fctPt->SetLineWidth(2);
    pt_resol_endcap_fctPt->SetLineColor(2);
    pt_resol_endcap_fctPt->SetMarkerStyle(8);
    pt_resol_endcap_fctPt->SetMarkerColor(2);

    // fit_pt_resol_barrel_fctPt->Draw("LPE");
    pt_resol_endcap_fctPt->Draw("LPE");
    pt_resol_barrel_fctPt->Draw("same LPE");

    TLegend legend(0.55,0.75,0.88,0.88);
    legend.SetBorderSize(0);
    legend.AddEntry(pt_resol_barrel_fctPt,"Barrel |#eta^{Offline #tau}|<1.305","LPE");
    legend.AddEntry(pt_resol_endcap_fctPt,"Endcap 1.479<|#eta^{Offline #tau}|<2.1","LPE");
    legend.Draw("same");

    TPaveText* texl1 = new TPaveText(0.27,0.87,0.2,0.99,"NDC");
    texl1->AddText("CMS Internal");
    texl1->SetTextSize(0.03);
    texl1->SetFillStyle(0);
    texl1->SetBorderSize(0);
    texl1->Draw("same");

    TPaveText* texl2 = new TPaveText(0.60,0.87,0.89,0.99,"NDC");
    if (run_nmbr != -1) { texl2->AddText("Run "+run_nmbr_str+" (13.6 TeV)"); }
    else                { texl2->AddText("Runs 2022 (13.6 TeV)"); }
    texl2->SetTextSize(0.03);
    texl2->SetFillStyle(0);
    texl2->SetBorderSize(0);
    texl2->Draw("same");

    canvas.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/resolution_ptBins.pdf");

    // ##############

    TCanvas canvasA("cA","cA",800,800);
    canvasA.SetLeftMargin(0.15);
    canvasA.SetGrid();
    // canvas.SetLogy();

    fit_pt_resol_barrel_fctPt->GetXaxis()->SetTitle("p^{Offline #tau}_{T} [GeV]");
    fit_pt_resol_barrel_fctPt->SetTitle("");
    fit_pt_resol_barrel_fctPt->GetXaxis()->SetTitleOffset(1.3);
    fit_pt_resol_barrel_fctPt->GetYaxis()->SetTitle("E^{L1 #tau}_{T} resolution");
    fit_pt_resol_barrel_fctPt->GetYaxis()->SetTitleOffset(1.5);
    fit_pt_resol_barrel_fctPt->SetTitle("");
    fit_pt_resol_barrel_fctPt->SetStats(0);

    fit_pt_resol_barrel_fctPt->GetXaxis()->SetRangeUser(20.,100.);
    // fit_pt_resol_barrel_fctPt->GetYaxis()->SetRangeUser(0.12,0.36);

    fit_pt_resol_barrel_fctPt->SetLineWidth(2);
    fit_pt_resol_barrel_fctPt->SetLineColor(1);
    fit_pt_resol_barrel_fctPt->SetMarkerStyle(8);
    fit_pt_resol_barrel_fctPt->SetMarkerColor(1);

    fit_pt_resol_endcap_fctPt->SetLineWidth(2);
    fit_pt_resol_endcap_fctPt->SetLineColor(2);
    fit_pt_resol_endcap_fctPt->SetMarkerStyle(8);
    fit_pt_resol_endcap_fctPt->SetMarkerColor(2);

    fit_pt_resol_barrel_fctPt->Draw("LPE");
    fit_pt_resol_endcap_fctPt->Draw("same LPE");

    TLegend legendA(0.55,0.75,0.88,0.88);
    legendA.SetBorderSize(0);
    legendA.AddEntry(fit_pt_resol_barrel_fctPt,"Barrel |#eta^{Offline #tau}|<1.305","LPE");
    legendA.AddEntry(fit_pt_resol_endcap_fctPt,"Endcap 1.479<|#eta^{Offline #tau}|<2.1","LPE");
    legendA.Draw("same");
    
    texl1->Draw("same");
    texl2->Draw("same");

    canvasA.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/resolution_ptBins_fromFit.pdf");

    // ##############

    TCanvas canvas1("c","c",800,800);
    canvas1.SetLeftMargin(0.15);
    canvas1.SetGrid();
    // canvas1.SetLogy();

    pt_resol_fctEta->GetXaxis()->SetTitle("#eta^{Offline #tau}");
    pt_resol_fctEta->SetTitle("");

    pt_resol_fctEta->GetYaxis()->SetRangeUser(0., 0.6);

    pt_resol_fctEta->GetXaxis()->SetTitleOffset(1.3);
    pt_resol_fctEta->GetYaxis()->SetTitle("E^{L1 #tau}_{T} resolution");
    pt_resol_fctEta->GetYaxis()->SetTitleOffset(1.3);
    pt_resol_fctEta->SetTitle("");
    pt_resol_fctEta->SetStats(0);

    pt_resol_fctEta->SetLineWidth(2);
    pt_resol_fctEta->SetLineColor(1);
    pt_resol_fctEta->SetMarkerStyle(8);
    pt_resol_fctEta->SetMarkerColor(1);

    pt_resol_fctEta->Draw("LPE");

    TBox b1(1.305,0.,1.479,0.6);
    b1.SetFillColor(16);
    b1.Draw("same");
    TBox b2(-1.479,0.,-1.305,0.6);
    b2.SetFillColor(16);
    b2.Draw("same");
    TBox b3(1.305,0.,1.479,0.6);
    b3.SetFillColor(1);
    b3.SetFillStyle(3004);
    b3.Draw("same");
    TBox b4(-1.479,0.,-1.305,0.6);
    b4.SetFillColor(1);
    b4.SetFillStyle(3004);
    b4.Draw("same");

    // TLegend legend1(0.55,0.75,0.88,0.88);
    // legend1.SetBorderSize(0);
    // legend1.AddEntry(pt_resol_fctEta,"Barrel |#eta^{Offline #tau}|<1.305","LPE");
    // legend1.Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");

    canvas1.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/resolution_etaBins.pdf");

    // ##############

    TCanvas canvasB("cB","cB",800,800);
    canvasB.SetLeftMargin(0.15);
    canvasB.SetGrid();
    // canvas1.SetLogy();

    fit_pt_resol_fctEta->GetXaxis()->SetTitle("#eta^{Offline #tau}");
    fit_pt_resol_fctEta->SetTitle("");
    fit_pt_resol_fctEta->GetYaxis()->SetRangeUser(0., 0.6);
    fit_pt_resol_fctEta->GetXaxis()->SetTitleOffset(1.3);
    fit_pt_resol_fctEta->GetYaxis()->SetTitle("E^{L1 #tau}_{T} resolution");
    fit_pt_resol_fctEta->GetYaxis()->SetTitleOffset(1.3);
    fit_pt_resol_fctEta->SetTitle("");
    fit_pt_resol_fctEta->SetStats(0);

    fit_pt_resol_fctEta->SetLineWidth(2);
    fit_pt_resol_fctEta->SetLineColor(1);
    fit_pt_resol_fctEta->SetMarkerStyle(8);
    fit_pt_resol_fctEta->SetMarkerColor(1);

    fit_pt_resol_fctEta->Draw("LPE");

    b1.Draw("same");
    b2.Draw("same");
    b3.Draw("same");
    b4.Draw("same");

    // TLegend legend1(0.55,0.75,0.88,0.88);
    // legend1.SetBorderSize(0);
    // legend1.AddEntry(pt_resol_fctEta,"Barrel |#eta^{Offline #tau}|<1.305","LPE");
    // legend1.Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");

    canvasB.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/resolution_etaBins_fromFit.pdf");

    // ##############

    TCanvas canvas2("c","c",800,800);
    canvas2.SetLeftMargin(0.15);
    canvas2.SetGrid();
    // canvas2.SetLogy();

    pt_scale_fctEta->GetXaxis()->SetTitle("#eta^{Offline #tau}");
    pt_scale_fctEta->SetTitle("");

    pt_scale_fctEta->GetYaxis()->SetRangeUser(0., 2.);

    pt_scale_fctEta->GetXaxis()->SetTitleOffset(1.3);
    pt_scale_fctEta->GetYaxis()->SetTitle("E^{L1 #tau}_{T} scale");
    pt_scale_fctEta->GetYaxis()->SetTitleOffset(1.3);
    pt_scale_fctEta->SetTitle("");
    pt_scale_fctEta->SetStats(0);

    pt_scale_fctEta->SetLineWidth(2);
    pt_scale_fctEta->SetLineColor(1);
    pt_scale_fctEta->SetMarkerStyle(8);
    pt_scale_fctEta->SetMarkerColor(1);

    pt_scale_fctEta->Draw("LPE");

    TBox b5(1.305,0.,1.479,2);
    b5.SetFillColor(16);
    b5.Draw("same");
    TBox b6(-1.479,0.,-1.305,2);
    b6.SetFillColor(16);
    b6.Draw("same");
    TBox b7(1.305,0.,1.479,2);
    b7.SetFillColor(1);
    b7.SetFillStyle(3004);
    b7.Draw("same");
    TBox b8(-1.479,0.,-1.305,2);
    b8.SetFillColor(1);
    b8.SetFillStyle(3004);
    b8.Draw("same");

    // TLegend legend2(0.55,0.75,0.88,0.88);
    // legend2.SetBorderSize(0);
    // legend2.AddEntry(pt_scale_fctEta,"Barrel |#eta^{Offline #tau}|<1.305","LPE");
    // legend2.Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");

    canvas2.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/scale_etaBins.pdf");

    // ##############

    TCanvas canvas3("c","c",800,800);
    canvas3.SetLeftMargin(0.15);
    canvas3.SetGrid();
    // canvas3.SetLogy();

    pt_scale_fctPt->GetXaxis()->SetTitle("p^{Offline #tau}_{T} [GeV]");
    pt_scale_fctPt->SetTitle("");

    pt_scale_fctPt->GetYaxis()->SetRangeUser(20., 100.);
    pt_scale_fctPt->GetYaxis()->SetRangeUser(0., 2.);

    pt_scale_fctPt->GetXaxis()->SetTitleOffset(1.3);
    pt_scale_fctPt->GetYaxis()->SetTitle("E^{L1 #tau}_{T} scale");
    pt_scale_fctPt->GetYaxis()->SetTitleOffset(1.3);
    pt_scale_fctPt->SetTitle("");
    pt_scale_fctPt->SetStats(0);

    pt_scale_fctPt->SetLineWidth(2);
    pt_scale_fctPt->SetLineColor(1);
    pt_scale_fctPt->SetMarkerStyle(8);
    pt_scale_fctPt->SetMarkerColor(1);

    pt_scale_fctPt->Draw("LPE");

    // TLegend legend3(0.55,0.75,0.88,0.88);
    // legend3.SetBorderSize(0);
    // legend3.AddEntry(pt_scale_fctPt,"Barrel |#eta^{Offline #tau}|<1.305","LPE");
    // legend3.Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");

    canvas3.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/scale_ptBins.pdf");

    // ##############

    TCanvas canvas4("c","c",800,800);
    canvas4.SetLeftMargin(0.15);
    canvas4.SetGrid();
    // canvas4.SetLogy();

    pt_response_ptInclusive->GetXaxis()->SetTitle("E^{L1 #tau}_{T} / p^{Offline #tau}_{T}");
    pt_response_ptInclusive->SetTitle("");

    pt_response_ptInclusive->GetXaxis()->SetTitleOffset(1.3);
    pt_response_ptInclusive->GetYaxis()->SetTitle("a.u.");
    pt_response_ptInclusive->GetYaxis()->SetTitleOffset(1.3);
    pt_response_ptInclusive->SetTitle("");
    pt_response_ptInclusive->SetStats(0);

    pt_response_ptInclusive->GetYaxis()->SetRangeUser(0., max(pt_barrel_resp_ptInclusive->GetMaximum(),pt_endcap_resp_ptInclusive->GetMaximum())*1.3 );

    pt_barrel_resp_ptInclusive->SetLineWidth(2);
    pt_barrel_resp_ptInclusive->SetLineColor(1);
    pt_barrel_resp_ptInclusive->SetMarkerStyle(8);
    pt_barrel_resp_ptInclusive->SetMarkerColor(1);

    pt_endcap_resp_ptInclusive->SetLineWidth(2);
    pt_endcap_resp_ptInclusive->SetLineColor(2);
    pt_endcap_resp_ptInclusive->SetMarkerStyle(8);
    pt_endcap_resp_ptInclusive->SetMarkerColor(2);

    pt_response_ptInclusive->SetLineWidth(2);
    pt_response_ptInclusive->SetLineColor(4);
    pt_response_ptInclusive->SetMarkerStyle(8);
    pt_response_ptInclusive->SetMarkerColor(4);

    pt_response_ptInclusive->Draw();
    pt_endcap_resp_ptInclusive->Draw("same");
    pt_barrel_resp_ptInclusive->Draw("same");

    TLegend legend4(0.55,0.75,0.88,0.88);
    legend4.SetBorderSize(0);
    legend4.AddEntry(pt_response_ptInclusive,"Inclusive |#eta^{Offline #tau}|<2.1","LPE");
    legend4.AddEntry(pt_barrel_resp_ptInclusive,"Barrel |#eta^{Offline #tau}|<1.305","LPE");
    legend4.AddEntry(pt_endcap_resp_ptInclusive,"Endcap 1.479<|#eta^{Offline #tau}|<2.1","LPE");
    legend4.Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");

    canvas4.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/response_inclusive.pdf");

    // ##############

    TCanvas canvas5("c","c",800,800);
    canvas5.SetLeftMargin(0.15);
    canvas5.SetGrid();
    // canvas5.SetLogy();

    PTvsETA_resolution->SetTitle("");
    PTvsETA_resolution->GetXaxis()->SetRangeUser(15.,200.);
    PTvsETA_resolution->GetXaxis()->SetTitle("p_{T}^{Offline #tau} [GeV]");
    PTvsETA_resolution->GetYaxis()->SetTitle("#eta^{Offline #tau} [GeV]");
    PTvsETA_resolution->GetZaxis()->SetTitle("Resolution");
    PTvsETA_resolution->Draw("colz");

    TBox b9(15., 1.305,200.,1.479);
    b9.SetFillColor(16);
    b9.Draw("same");
    TBox b10(15., 1.305,200.,1.479);
    b10.SetFillColor(1);
    b10.SetFillStyle(3004);
    b10.Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");

    canvas5.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/resolution_ptVSeta.pdf");

    // ##############

    TCanvas canvas6("c","c",800,800);
    canvas6.SetLeftMargin(0.15);
    canvas6.SetGrid();
    // canvas6.SetLogy();

    PTvsETA_scale->SetTitle("");
    PTvsETA_scale->GetXaxis()->SetRangeUser(15.,200.);
    PTvsETA_scale->GetXaxis()->SetTitle("p_{T}^{Offline #tau} [GeV]");
    PTvsETA_scale->GetYaxis()->SetTitle("#eta^{Offline #tau} [GeV]");
    PTvsETA_scale->GetZaxis()->SetTitle("Scale");
    PTvsETA_scale->Draw("colz");

    TBox b11(15., 1.305,200.,1.479);
    b11.SetFillColor(16);
    b11.Draw("same");
    TBox b12(15., 1.305,200.,1.479);
    b12.SetFillColor(1);
    b12.SetFillStyle(3004);
    b12.Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");

    canvas6.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/scale_ptVSeta.pdf");


    // TCanvas canvas7("c","c",800,800);
    // canvas7.SetLeftMargin(0.15);
    // canvas7.SetGrid();
    // pt->Draw();
    // canvas7.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/pt.pdf");

    // TCanvas canvas8("c","c",800,800);
    // canvas8.SetLeftMargin(0.15);
    // canvas8.SetGrid();
    // eta->Draw();
    // canvas8.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/eta.pdf");

    // TCanvas canvas9("c","c",800,800);
    // canvas9.SetLeftMargin(0.15);
    // canvas9.SetGrid();
    // l1tpt->Draw();
    // canvas9.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/l1tpt.pdf");

    // ----------------------------------------------------------------------------
    // plot performance VS Run2

    gStyle->SetOptStat(000000);

    for(long unsigned int i = 0; i < ptBins.size()-1; ++i)
    {
        TString lowP ;
        lowP.Form("%.1f", ptBins[i]);
        TString highP;
        highP.Form("%.1f", ptBins[i+1]);

        TCanvas canvas("c","c",800,800);
        canvas.SetLeftMargin(0.15);
        canvas.SetGrid();
        // canvas.SetLogy();

        // use dummy histogram to define style
        empty->GetXaxis()->SetTitle("E_{T}^{L1 #tau} / p_{T}^{Offline #tau}");
        empty->SetTitle("");

        // empty->GetXaxis()->SetRangeUser(0.,250.);
        empty->GetYaxis()->SetRangeUser(0., max(barrel_response_ptBins[i]->GetMaximum(),endcap_response_ptBins[i]->GetMaximum())*1.3 );

        empty->GetXaxis()->SetTitleOffset(1.3);
        empty->GetYaxis()->SetTitle("a.u.");
        empty->GetYaxis()->SetTitleOffset(1.3);
        empty->SetTitle("");
        empty->SetStats(0);

        empty->Draw();

        barrel_response_ptBins[i]->SetLineWidth(2);
        barrel_response_ptBins[i]->SetLineColor(1);
        barrel_response_ptBins[i]->SetMarkerStyle(8);
        barrel_response_ptBins[i]->SetMarkerColor(1);
        
        barrel_response_ptBins[i]->Draw("same");

        fit_barrel_response_ptBins[i]->SetLineWidth(2);
        fit_barrel_response_ptBins[i]->SetLineColor(1);

        fit_barrel_response_ptBins[i]->Draw("same");

        TPaveText* texl1 = new TPaveText(0.27,0.87,0.2,0.99,"NDC");
        texl1->AddText("CMS Internal");
        texl1->SetTextSize(0.03);
        texl1->SetFillStyle(0);
        texl1->SetBorderSize(0);
        texl1->Draw("same");

        TPaveText* texl2 = new TPaveText(0.60,0.87,0.89,0.99,"NDC");
        if (run_nmbr != -1) { texl2->AddText("Run "+run_nmbr_str+" (13.6 TeV)"); }
        else                { texl2->AddText("Runs 2022 (13.6 TeV)"); }
        texl2->SetTextSize(0.03);
        texl2->SetFillStyle(0);
        texl2->SetBorderSize(0);
        texl2->Draw("same");

        TLegend legend(0.55,0.75,0.88,0.88);
        legend.SetBorderSize(0);
        legend.SetHeader(lowP+" < p_{T}^{Offline #tau} < "+highP+" [GeV]");
        legend.AddEntry(barrel_response_ptBins[i],"Run-3 (2022)","LPE");
        legend.Draw("same");

        canvas.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/barrel_response_"+lowP+"pt"+highP+"_vsRun2.pdf");
    }


    for(long unsigned int i = 0; i < ptBins.size()-1; ++i)
    {
        TString lowP ;
        lowP.Form("%.1f", ptBins[i]);
        TString highP;
        highP.Form("%.1f", ptBins[i+1]);

        TCanvas canvas("c","c",800,800);
        canvas.SetLeftMargin(0.15);
        canvas.SetGrid();
        // canvas.SetLogy();

        // use dummy histogram to define style
        empty->GetXaxis()->SetTitle("E_{T}^{L1 #tau} / p_{T}^{Offline #tau}");
        empty->SetTitle("");

        // empty->GetXaxis()->SetRangeUser(0.,250.);
        empty->GetYaxis()->SetRangeUser(0., max(barrel_response_ptBins[i]->GetMaximum(),endcap_response_ptBins[i]->GetMaximum())*1.3 );

        empty->GetXaxis()->SetTitleOffset(1.3);
        empty->GetYaxis()->SetTitle("a.u.");
        empty->GetYaxis()->SetTitleOffset(1.3);
        empty->SetTitle("");
        empty->SetStats(0);

        empty->Draw();

        endcap_response_ptBins[i]->SetLineWidth(2);
        endcap_response_ptBins[i]->SetLineColor(1);
        endcap_response_ptBins[i]->SetMarkerStyle(8);
        endcap_response_ptBins[i]->SetMarkerColor(1);

        endcap_response_ptBins[i]->Draw("same");

        fit_endcap_response_ptBins[i]->SetLineWidth(2);
        fit_endcap_response_ptBins[i]->SetLineColor(1);

        fit_endcap_response_ptBins[i]->Draw("same");

        texl1->Draw("same");
        texl2->Draw("same");

        TLegend legend(0.55,0.75,0.88,0.88);
        legend.SetBorderSize(0);
        legend.SetHeader(lowP+" < p_{T}^{Offline #tau} < "+highP+" [GeV]");
        legend.AddEntry(endcap_response_ptBins[i],"Run-3 (2022)","LPE");
        legend.Draw("same");

        canvas.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/endcap_response_"+lowP+"pt"+highP+"_vsRun2.pdf");
    }

    for(long unsigned int i = 0; i < etaBins.size()-1; ++i)
    {
        TString lowE;
        lowE.Form("%.3f", etaBins[i]);
        TString highE;
        highE.Form("%.3f", etaBins[i+1]);

        TCanvas canvas("c","c",800,800);
        canvas.SetLeftMargin(0.15);
        canvas.SetGrid();
        // canvas.SetLogy();

        // use dummy histogram to define style
        empty->GetXaxis()->SetTitle("E_{T}^{L1 #tau} / p_{T}^{Offline #tau}");
        empty->SetTitle("");

        // empty->GetXaxis()->SetRangeUser(0.,250.);
        empty->GetYaxis()->SetRangeUser(0., absEta_response_ptBins[i]->GetMaximum()*1.3 );

        empty->GetXaxis()->SetTitleOffset(1.3);
        empty->GetYaxis()->SetTitle("a.u.");
        empty->GetYaxis()->SetTitleOffset(1.3);
        empty->SetTitle("");
        empty->SetStats(0);

        empty->Draw();

        absEta_response_ptBins[i]->SetLineWidth(2);
        absEta_response_ptBins[i]->SetLineColor(1);
        absEta_response_ptBins[i]->SetMarkerStyle(8);
        absEta_response_ptBins[i]->SetMarkerColor(1);

        absEta_response_ptBins[i]->Draw("same");

        fit_absEta_response_ptBins[i]->SetLineWidth(2);
        fit_absEta_response_ptBins[i]->SetLineColor(1);

        fit_absEta_response_ptBins[i]->Draw("same");

        TLegend legend(0.55,0.75,0.88,0.88);
        legend.SetBorderSize(0);
        legend.SetHeader(lowE+"<|#eta^{Offline #tau}|<"+highE);
        legend.AddEntry(absEta_response_ptBins[i],"Run-3 (2022)","LPE");
        legend.Draw("same");

        TPaveText* texl1 = new TPaveText(0.27,0.87,0.2,0.99,"NDC");
        texl1->AddText("CMS Internal");
        texl1->SetTextSize(0.03);
        texl1->SetFillStyle(0);
        texl1->SetBorderSize(0);
        texl1->Draw("same");

        TPaveText* texl2 = new TPaveText(0.60,0.87,0.89,0.99,"NDC");
        if (run_nmbr != -1) { texl2->AddText("Run "+run_nmbr_str+" (13.6 TeV)"); }
        else                { texl2->AddText("Runs 2022 (13.6 TeV)"); }
        texl2->SetTextSize(0.03);
        texl2->SetFillStyle(0);
        texl2->SetBorderSize(0);
        texl2->Draw("same");

        canvas.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/response_"+lowE+"eta"+highE+".pdf");
    }


    // ##############

    TCanvas canvasZ("cZ","cZ",800,800);
    canvasZ.SetLeftMargin(0.15);
    canvasZ.SetGrid();
    // canvasZ.SetLogy();

    pt_resol_endcap_fctPt->GetXaxis()->SetTitle("p^{Offline #tau}_{T} [GeV]");
    pt_resol_endcap_fctPt->SetTitle("");
    pt_resol_endcap_fctPt->GetXaxis()->SetTitleOffset(1.3);
    pt_resol_endcap_fctPt->GetYaxis()->SetTitle("E^{L1 #tau}_{T} resolution");
    pt_resol_endcap_fctPt->GetYaxis()->SetTitleOffset(1.5);
    pt_resol_endcap_fctPt->SetTitle("");
    pt_resol_endcap_fctPt->SetStats(0);

    pt_resol_endcap_fctPt->GetXaxis()->SetRangeUser(20.,100.);
    // pt_resol_endcap_fctPt->GetYaxis()->SetRangeUser(0.12,0.36);

    pt_resol_endcap_fctPt->SetLineWidth(2);
    pt_resol_endcap_fctPt->SetLineColor(1);
    pt_resol_endcap_fctPt->SetMarkerStyle(8);
    pt_resol_endcap_fctPt->SetMarkerColor(1);

    // fit_pt_resol_barrel_fctPt->Draw("LPE");
    pt_resol_endcap_fctPt->Draw("LPE");

    TLegend legendZ(0.55,0.75,0.88,0.88);
    legendZ.SetBorderSize(0);
    legendZ.SetHeader("Barrel |#eta^{Offline #tau}|<1.305");
    legendZ.AddEntry(pt_resol_endcap_fctPt,"Run-3 (2022)","LPE");
    legendZ.Draw("same");

    TPaveText* texl1Z = new TPaveText(0.27,0.87,0.2,0.99,"NDC");
    texl1Z->AddText("CMS Internal");
    texl1Z->SetTextSize(0.03);
    texl1Z->SetFillStyle(0);
    texl1Z->SetBorderSize(0);
    texl1Z->Draw("same");

    TPaveText* texl2Z = new TPaveText(0.60,0.87,0.89,0.99,"NDC");
    if (run_nmbr != -1) { texl2Z->AddText("Run "+run_nmbr_str+" (13.6 TeV)"); }
    else                { texl2Z->AddText("Runs 2022 (13.6 TeV)"); }
    texl2Z->SetTextSize(0.03);
    texl2Z->SetFillStyle(0);
    texl2Z->SetBorderSize(0);
    texl2Z->Draw("same");

    canvasZ.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/endcap_resolution_ptBins_vsRun2.pdf");

    // ##############

    TCanvas canvasY("cY","cY",800,800);
    canvasY.SetLeftMargin(0.15);
    canvasY.SetGrid();
    // canvasY.SetLogy();

    pt_resol_barrel_fctPt->GetXaxis()->SetTitle("p^{Offline #tau}_{T} [GeV]");
    pt_resol_barrel_fctPt->SetTitle("");
    pt_resol_barrel_fctPt->GetXaxis()->SetTitleOffset(1.3);
    pt_resol_barrel_fctPt->GetYaxis()->SetTitle("E^{L1 #tau}_{T} resolution");
    pt_resol_barrel_fctPt->GetYaxis()->SetTitleOffset(1.5);
    pt_resol_barrel_fctPt->SetTitle("");
    pt_resol_barrel_fctPt->SetStats(0);

    pt_resol_barrel_fctPt->GetXaxis()->SetRangeUser(20.,100.);
    // pt_resol_barrel_fctPt->GetYaxis()->SetRangeUser(0.12,0.36);

    pt_resol_barrel_fctPt->SetLineWidth(2);
    pt_resol_barrel_fctPt->SetLineColor(1);
    pt_resol_barrel_fctPt->SetMarkerStyle(8);
    pt_resol_barrel_fctPt->SetMarkerColor(1);

    // fit_pt_resol_barrel_fctPt->Draw("LPE");
    pt_resol_barrel_fctPt->Draw("LPE");

    TLegend legendY(0.55,0.75,0.88,0.88);
    legendY.SetBorderSize(0);
    legendY.SetHeader("Barrel |#eta^{Offline #tau}|<1.305");
    legendY.AddEntry(pt_resol_barrel_fctPt,"Run-3 (2022)","LPE");
    legendY.Draw("same");

    texl1Z->Draw("same");
    texl2Z->Draw("same");

    canvasY.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/barrel_resolution_ptBins_vsRun2.pdf");

    // ##############

    TCanvas canvasYX("cYX","cYX",800,800);
    canvasYX.SetLeftMargin(0.15);
    canvasYX.SetGrid();
    // canvasY.SetLogy();

    pt_resol_fctPt->GetXaxis()->SetTitle("p^{Offline #tau}_{T} [GeV]");
    pt_resol_fctPt->SetTitle("");
    pt_resol_fctPt->GetXaxis()->SetTitleOffset(1.3);
    pt_resol_fctPt->GetYaxis()->SetTitle("E^{L1 #tau}_{T} resolution");
    pt_resol_fctPt->GetYaxis()->SetTitleOffset(1.5);
    pt_resol_fctPt->SetTitle("");
    pt_resol_fctPt->SetStats(0);

    pt_resol_fctPt->GetXaxis()->SetRangeUser(20.,100.);
    // pt_resol_barrel_fctPt->GetYaxis()->SetRangeUser(0.12,0.36);

    pt_resol_fctPt->SetLineWidth(2);
    pt_resol_fctPt->SetLineColor(1);
    pt_resol_fctPt->SetMarkerStyle(8);
    pt_resol_fctPt->SetMarkerColor(1);

    // fit_pt_resol_barrel_fctPt->Draw("LPE");
    pt_resol_fctPt->Draw("LPE");

    TLegend legendYX(0.55,0.75,0.88,0.88);
    legendYX.SetBorderSize(0);
    legendYX.AddEntry(pt_resol_fctPt,"Run-3 (2022)","LPE");
    legendYX.Draw("same");

    texl1Z->Draw("same");
    texl2Z->Draw("same");

    canvasYX.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/resolution_ptBins_vsRun2.pdf");

    // ##############

    TCanvas canvas1Y("c1Y","c1Y",800,800);
    canvas1Y.SetLeftMargin(0.15);
    canvas1Y.SetGrid();
    // canvas1.SetLogy();

    pt_resol_fctEta->GetXaxis()->SetTitle("#eta^{Offline #tau}");
    pt_resol_fctEta->SetTitle("");

    pt_resol_fctEta->GetYaxis()->SetRangeUser(0., 0.6);

    pt_resol_fctEta->GetXaxis()->SetTitleOffset(1.3);
    pt_resol_fctEta->GetYaxis()->SetTitle("E^{L1 #tau}_{T} resolution");
    pt_resol_fctEta->GetYaxis()->SetTitleOffset(1.3);
    pt_resol_fctEta->SetTitle("");
    pt_resol_fctEta->SetStats(0);

    pt_resol_fctEta->SetLineWidth(2);
    pt_resol_fctEta->SetLineColor(1);
    pt_resol_fctEta->SetMarkerStyle(8);
    pt_resol_fctEta->SetMarkerColor(1);

    pt_resol_fctEta->Draw("LPE");

    b1.Draw("same");
    b2.Draw("same");
    b3.Draw("same");
    b4.Draw("same");

    TLegend legendB(0.55,0.75,0.88,0.88);
    legendB.SetBorderSize(0);
    legendB.AddEntry(pt_resol_fctEta,"Run-3 (2022)","LPE");
    legendB.Draw("same");

    texl1Z->Draw("same");
    texl2Z->Draw("same");

    canvas1Y.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/resolution_etaBins_vsRun2.pdf");

    // ##############

    TCanvas canvas2W("c2W","c2W",800,800);
    canvas2W.SetLeftMargin(0.15);
    canvas2W.SetGrid();
    // canvas2.SetLogy();

    pt_scale_fctEta->GetXaxis()->SetTitle("#eta^{Offline #tau}");
    pt_scale_fctEta->SetTitle("");

    pt_scale_fctEta->GetYaxis()->SetRangeUser(0., 2.);

    pt_scale_fctEta->GetXaxis()->SetTitleOffset(1.3);
    pt_scale_fctEta->GetYaxis()->SetTitle("E^{L1 #tau}_{T} scale");
    pt_scale_fctEta->GetYaxis()->SetTitleOffset(1.3);
    pt_scale_fctEta->SetTitle("");
    pt_scale_fctEta->SetStats(0);

    pt_scale_fctEta->SetLineWidth(2);
    pt_scale_fctEta->SetLineColor(1);
    pt_scale_fctEta->SetMarkerStyle(8);
    pt_scale_fctEta->SetMarkerColor(1);

    pt_scale_fctEta->Draw("LPE");

    b5.Draw("same");
    b6.Draw("same");
    b7.Draw("same");
    b8.Draw("same");

    texl1->Draw("same");
    texl2->Draw("same");
    legendB.Draw("same");

    canvas2W.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/scale_etaBins_vsRun2.pdf");

    // ##############

    TCanvas canvas3V("c3V","c3V",800,800);
    canvas3V.SetLeftMargin(0.15);
    canvas3V.SetGrid();
    // canvas3.SetLogy();

    pt_scale_fctPt->GetXaxis()->SetTitle("p^{Offline #tau}_{T} [GeV]");
    pt_scale_fctPt->SetTitle("");

    pt_scale_fctPt->GetYaxis()->SetRangeUser(20., 100.);
    pt_scale_fctPt->GetYaxis()->SetRangeUser(0., 2.);

    pt_scale_fctPt->GetXaxis()->SetTitleOffset(1.3);
    pt_scale_fctPt->GetYaxis()->SetTitle("E^{L1 #tau}_{T} scale");
    pt_scale_fctPt->GetYaxis()->SetTitleOffset(1.3);
    pt_scale_fctPt->SetTitle("");
    pt_scale_fctPt->SetStats(0);

    pt_scale_fctPt->SetLineWidth(2);
    pt_scale_fctPt->SetLineColor(1);
    pt_scale_fctPt->SetMarkerStyle(8);
    pt_scale_fctPt->SetMarkerColor(1);

    pt_scale_fctPt->Draw("LPE");

    texl1Z->Draw("same");
    texl2Z->Draw("same");
    legendB.Draw("same");

    canvas3V.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/scale_ptBins_vsRun2.pdf");

    // ##############

    TCanvas canvas4U("4U","c4U",800,800);
    canvas4U.SetLeftMargin(0.15);
    canvas4U.SetGrid();
    // canvas4U.SetLogy();

    pt_response_ptInclusive->GetXaxis()->SetTitle("E^{L1 #tau}_{T} / p^{Offline #tau}_{T}");
    pt_response_ptInclusive->SetTitle("");

    pt_response_ptInclusive->GetXaxis()->SetTitleOffset(1.3);
    pt_response_ptInclusive->GetYaxis()->SetTitle("a.u.");
    pt_response_ptInclusive->GetYaxis()->SetTitleOffset(1.3);
    pt_response_ptInclusive->SetTitle("");
    pt_response_ptInclusive->SetStats(0);

    pt_response_ptInclusive->GetYaxis()->SetRangeUser(0., pt_response_ptInclusive->GetMaximum()*1.3 );

    pt_response_ptInclusive->SetLineWidth(2);
    pt_response_ptInclusive->SetLineColor(1);
    pt_response_ptInclusive->SetMarkerStyle(8);
    pt_response_ptInclusive->SetMarkerColor(1);

    pt_response_ptInclusive->Draw();

    TLegend legend4U(0.55,0.75,0.88,0.88);
    legend4U.SetBorderSize(0);
    legend4U.AddEntry(pt_response_ptInclusive,"Run-3 (2022)","LPE");
    legend4U.Draw("same");

    texl1Z->Draw("same");
    texl2Z->Draw("same");

    canvas4U.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/response_inclusive_vsRun2.pdf");


    // ##############

    TCanvas canvas5T("c5T","c5T",800,800);
    canvas5T.SetLeftMargin(0.15);
    canvas5T.SetGrid();
    // canvas4.SetLogy();

    pt_barrel_resp_ptInclusive->GetXaxis()->SetTitle("E^{L1 #tau}_{T} / p^{Offline #tau}_{T}");
    pt_barrel_resp_ptInclusive->SetTitle("");

    pt_barrel_resp_ptInclusive->GetXaxis()->SetTitleOffset(1.3);
    pt_barrel_resp_ptInclusive->GetYaxis()->SetTitle("a.u.");
    pt_barrel_resp_ptInclusive->GetYaxis()->SetTitleOffset(1.3);
    pt_barrel_resp_ptInclusive->SetTitle("");
    pt_barrel_resp_ptInclusive->SetStats(0);

    pt_barrel_resp_ptInclusive->GetYaxis()->SetRangeUser(0., pt_barrel_resp_ptInclusive->GetMaximum()*1.3 );

    pt_barrel_resp_ptInclusive->SetLineWidth(2);
    pt_barrel_resp_ptInclusive->SetLineColor(1);
    pt_barrel_resp_ptInclusive->SetMarkerStyle(8);
    pt_barrel_resp_ptInclusive->SetMarkerColor(1);

    pt_response_ptInclusive->Draw();

    TLegend legend5T(0.55,0.75,0.88,0.88);
    legend5T.SetBorderSize(0);
    legend5T.SetHeader("Barrel |#eta^{Offline #tau}|<1.305");
    legend5T.AddEntry(pt_barrel_resp_ptInclusive,"Run-3 (2022)","LPE");
    legend5T.Draw("same");

    texl1Z->Draw("same");
    texl2Z->Draw("same");

    canvas5T.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/barrel_response_inclusive_vsRun2.pdf");

    // ##############

    TCanvas canvas5S("c5S","c5S",800,800);
    canvas5S.SetLeftMargin(0.15);
    canvas5S.SetGrid();

    pt_endcap_resp_ptInclusive->GetXaxis()->SetTitle("E^{L1 #tau}_{T} / p^{Offline #tau}_{T}");
    pt_endcap_resp_ptInclusive->SetTitle("");

    pt_endcap_resp_ptInclusive->GetXaxis()->SetTitleOffset(1.3);
    pt_endcap_resp_ptInclusive->GetYaxis()->SetTitle("a.u.");
    pt_endcap_resp_ptInclusive->GetYaxis()->SetTitleOffset(1.3);
    pt_endcap_resp_ptInclusive->SetTitle("");
    pt_endcap_resp_ptInclusive->SetStats(0);

    pt_endcap_resp_ptInclusive->GetYaxis()->SetRangeUser(0., pt_endcap_resp_ptInclusive->GetMaximum()*1.3 );

    pt_endcap_resp_ptInclusive->SetLineWidth(2);
    pt_endcap_resp_ptInclusive->SetLineColor(1);
    pt_endcap_resp_ptInclusive->SetMarkerStyle(8);
    pt_endcap_resp_ptInclusive->SetMarkerColor(1);

    pt_response_ptInclusive->Draw();

    TLegend legend5S(0.55,0.75,0.88,0.88);
    legend5S.SetBorderSize(0);
    legend5S.SetHeader("Endcap 1.479<|#eta^{Offline #tau}|<2.1");
    legend5S.AddEntry(pt_endcap_resp_ptInclusive,"Run-3 (2022)","LPE");
    legend5S.Draw("same");

    texl1Z->Draw("same");
    texl2Z->Draw("same");

    canvas5S.SaveAs("PDFs/PDFs_2023/Run3_13p6TeV_Run"+run_nmbr_str+"/endcap_response_inclusive_vsRun2.pdf");
 
*/
}

