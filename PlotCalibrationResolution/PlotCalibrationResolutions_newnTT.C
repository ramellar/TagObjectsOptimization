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

TF1 fitResponseHisto(TString option, TH1F* hist2fit, float min_range, float max_range, float norm, float mean, float sigma, float alpha=0., float beta=0.) {
    TF1* f;
    if (option=="gaussian")
    {
        f  = new TF1("f","gaus",min_range,max_range);
        f->SetParameters(norm, mean, sigma);
        hist2fit->Fit(f);
    }
    else 
    {
        f  = new TF1("f","crystalball",min_range,max_range);
        f->SetParameters(norm, mean, sigma, alpha, beta);
        hist2fit->Fit(f);
    }
    
    return *f;
}


void plotResolutions(TString date, int vers, float calibThr = 1.7, TString fitOption="crystalball", Bool_t nTTRange = kFALSE)
{
    TString version = to_string(vers);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    const int lenPtBins = 12;
    const int lenEtaBins = 7;
    const int lenSignedEtaBins = 13;
    Double_t ptBins[lenPtBins] = {20, 25, 30, 35, 40, 45, 50, 60, 70, 90, 110, 130};
    Double_t etaBins[lenEtaBins] = {0., 0.5, 1.0, 1.305, 1.479, 1.8, 2.1};
    Double_t signedEtaBins[lenSignedEtaBins] = {-2.1, -1.8, -1.479, -1.305, -1.0, -0.5, 0., 0.5, 1.0, 1.305, 1.479, 1.8, 2.1};

    // PT RESPONSE - PT BINS HISTOGRAMS
    TH1F* pt_response_ptInclusive    = new TH1F("pt_response_ptInclusive_mc","pt_response_ptInclusive_mc",60,0,3);
    TH1F* pt_barrel_resp_ptInclusive = new TH1F("pt_barrel_resp_ptInclusive","pt_barrel_resp_ptInclusive",60,0,3);
    TH1F* pt_endcap_resp_ptInclusive = new TH1F("pt_endcap_resp_ptInclusive","pt_endcap_resp_ptInclusive",60,0,3);

    std::vector<TH1F*> pt_resp_ptBin = {};
    std::vector<TH1F*> pt_barrel_resp_ptBin = {};
    std::vector<TH1F*> pt_endcap_resp_ptBin = {};
    for (Int_t i = 0; i < lenPtBins-1; ++i)
    {
        TString low  = to_string(ptBins[i]);
        TString high = to_string(ptBins[i+1]);
        pt_resp_ptBin.push_back(new TH1F("pt_resp_ptBin"+low+"to"+high,"pt_resp_ptBin"+low+"to"+high,60,0,3));
        pt_barrel_resp_ptBin.push_back(new TH1F("pt_barrel_resp_ptBin"+low+"to"+high,"pt_barrel_resp_ptBin"+low+"to"+high,60,0,3));
        pt_endcap_resp_ptBin.push_back(new TH1F("pt_endcap_resp_ptBin"+low+"to"+high,"pt_endcap_resp_ptBin"+low+"to"+high,60,0,3));
    }

    // PT RESPONSE -  ETA BINS HISTIGRAMS
    std::vector<TH1F*> pt_resp_AbsEtaBin = {};
    std::vector<TH1F*> pt_resp_MinusEtaBin = {};
    std::vector<TH1F*> pt_resp_PlusEtaBin = {};
    for (Int_t i = 0; i < lenEtaBins-1; ++i)
    {
        TString low  = to_string(etaBins[i]);
        TString high = to_string(etaBins[i+1]);
        pt_resp_AbsEtaBin.push_back(new TH1F("pt_resp_AbsEtaBin"+low+"to"+high,"pt_resp_AbsEtaBin"+low+"to"+high,60,0,3));
        pt_resp_MinusEtaBin.push_back(new TH1F("pt_resp_MinusEtaBin"+low+"to"+high,"pt_resp_MinusEtaBin"+low+"to"+high,60,0,3));
        pt_resp_PlusEtaBin.push_back(new TH1F("pt_resp_PlusEtaBin"+low+"to"+high,"pt_resp_PlusEtaBin"+low+"to"+high,60,0,3));
    }    

    // PT RESOLUTION AND SCALE HISTGRAMS
    TH1F* pt_resol_fctPt = new TH1F("pt_resol_fctPt","pt_resol_fctPt",lenPtBins-1,ptBins);
    TH1F* pt_resol_barrel_fctPt = new TH1F("pt_resol_barrel_fctPt","pt_resol_barrel_fctPt",lenPtBins-1, ptBins);
    TH1F* pt_resol_endcap_fctPt = new TH1F("pt_resol_endcap_fctPt","pt_resol_endcap_fctPt",lenPtBins-1, ptBins);
    TH1F* pt_resol_fctEta = new TH1F("pt_resol_fctEta","pt_resol_fctEta",lenSignedEtaBins-1, signedEtaBins);
    TH1F* pt_scale_fctPt = new TH1F("pt_scale_fctPt","pt_scale_fctPt",lenPtBins-1,ptBins);
    TH1F* pt_scale_fctEta = new TH1F("pt_scale_fctEta","pt_scale_fctEta",lenSignedEtaBins-1, signedEtaBins);

    // PT RESOLUTION - PT_ETA BINS HISTOGRAM
    std::vector<TH1F*> pt_resp_PtEtaBin = {};
    for (Int_t i = 0; i < lenPtBins-1; ++i)
    {
        for (Int_t j = 0; j < lenEtaBins-1; ++j)
        {
            TString lowP  = to_string(ptBins[i]);
            TString lowE  = to_string(etaBins[j]);
            TString highP = to_string(ptBins[i+1]);
            TString highE = to_string(etaBins[j+1]);
            pt_resp_PtEtaBin.push_back(new TH1F("pt_resp_PtBin"+lowP+"to"+highP+"_EtaBin"+lowE+"to"+highE,"pt_resp_PtBin"+lowP+"to"+highP+"_EtaBin"+lowE+"to"+highE,60,0,3));
        }
    }

    TH2F* PTvsETA_resolution = new TH2F("PTvsETA_resolution","PTvsETA_resolution",lenPtBins-1, ptBins,lenEtaBins-1, etaBins);
    TH2F* PTvsETA_scale = new TH2F("PTvsETA_events","PTvsETA_events",lenPtBins-1, ptBins,lenEtaBins-1, etaBins);

    TString InputFileName = "/data_CMS/cms/motta/Run3preparation/"+date+"_optimizationV"+version+"_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_CALIBRATED_"+date+".root";
    TFile f(InputFileName.Data(),"READ");
    TTree* inTree = (TTree*)f.Get("outTreeCalibrated");

    Int_t           in_L1Tau_IEta;
    Int_t           in_compressedsortedshape;
    Int_t           in_L1Tau_hasEM;
    Float_t         in_Target;
    Int_t           in_L1Tau_IEt;
    Int_t           in_L1Tau_RawIEt;
    Int_t           in_EventNumber;
    Int_t           in_RunNumber;
    Int_t           in_Nvtx;
    Int_t           in_L1Tau_nTT;
    Float_t         in_L1Tau_pt;
    Float_t         in_L1Tau_CalibPt;
    Float_t         in_OfflineTau_pt;
    Float_t         in_OfflineTau_eta;
    Int_t           in_compressedieta;
    Int_t           in_compressedE;
    Int_t           in_L1Tau_Iso;
    Int_t           in_compressednTT;
    Int_t           in_OfflineTau_isMatched;
    Int_t           in_L1Tau_isMerged;
    Int_t           in_compressedshape;
    Int_t           in_supercompressedE;
    Int_t           in_supercompressednTT;
    Int_t           in_L1Tau_Qual;
    Bool_t          in_L1Tau_PassShapeVeto;
    Bool_t          L1Tau_PassShapeVeto_0p6_10_50;
    Bool_t          L1Tau_PassShapeVeto_0p5_20_45;
    Bool_t          L1Tau_PassShapeVeto_0p2_25_60;
    Bool_t          L1Tau_PassShapeVeto_0p2_20_100;

    inTree->SetBranchAddress("L1Tau_IEta", &in_L1Tau_IEta);
    inTree->SetBranchAddress("compressedsortedshape", &in_compressedsortedshape);
    inTree->SetBranchAddress("L1Tau_hasEM", &in_L1Tau_hasEM);
    inTree->SetBranchAddress("Target", &in_Target);
    inTree->SetBranchAddress("L1Tau_IEt", &in_L1Tau_IEt);
    inTree->SetBranchAddress("L1Tau_RawIEt", &in_L1Tau_RawIEt);
    inTree->SetBranchAddress("EventNumber", &in_EventNumber);
    inTree->SetBranchAddress("RunNumber", &in_RunNumber);
    inTree->SetBranchAddress("Nvtx", &in_Nvtx);
    inTree->SetBranchAddress("L1Tau_nTT", &in_L1Tau_nTT);
    inTree->SetBranchAddress("L1Tau_pt", &in_L1Tau_pt);
    inTree->SetBranchAddress("L1Tau_CalibPt", &in_L1Tau_CalibPt);
    inTree->SetBranchAddress("OfflineTau_pt", &in_OfflineTau_pt);
    inTree->SetBranchAddress("OfflineTau_eta", &in_OfflineTau_eta);
    inTree->SetBranchAddress("compressedieta", &in_compressedieta);
    inTree->SetBranchAddress("compressedE", &in_compressedE);
    inTree->SetBranchAddress("L1Tau_Iso", &in_L1Tau_Iso);
    inTree->SetBranchAddress("compressednTT", &in_compressednTT);
    inTree->SetBranchAddress("OfflineTau_isMatched", &in_OfflineTau_isMatched);
    inTree->SetBranchAddress("L1Tau_isMerged", &in_L1Tau_isMerged);
    inTree->SetBranchAddress("compressedshape", &in_compressedshape);
    inTree->SetBranchAddress("supercompressedE", &in_supercompressedE);
    inTree->SetBranchAddress("supercompressednTT", &in_supercompressednTT);
    inTree->SetBranchAddress("L1Tau_PassShapeVeto", &in_L1Tau_PassShapeVeto);
    inTree->SetBranchAddress("L1Tau_Qual",&in_L1Tau_Qual);

    for(UInt_t i = 0 ; i < inTree->GetEntries() ; ++i)
    {
        inTree->GetEntry(i);
        //if(in_L1Tau_nTT<60 && nTTRange) continue;
        //if(fabs(in_OfflineTau_eta)>2.1) continue;

        // FILL PT RESPONSE - PT INCLUSIVE HISTOGRAMS
        if (fabs(in_OfflineTau_eta)<1.305) pt_barrel_resp_ptInclusive->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt);
        else if (fabs(in_OfflineTau_eta)<5.191 and fabs(in_OfflineTau_eta)>1.479) pt_endcap_resp_ptInclusive->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        pt_response_ptInclusive->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt);

        // FILL PT RESPONSE - PT BINS HISTOGRAMS
        for (Int_t i = 0; i < lenPtBins-1; ++i)
        {
            if (in_OfflineTau_pt > ptBins[i] and in_OfflineTau_pt <= ptBins[i+1])
            {
                pt_resp_ptBin[i]->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt);
                if (fabs(in_OfflineTau_eta)<1.305) pt_barrel_resp_ptBin[i]->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt);
                else if (fabs(in_OfflineTau_eta)<5.191 and fabs(in_OfflineTau_eta)>1.479) pt_endcap_resp_ptBin[i]->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt);
            }
        }

        // FILL PT RESPONSE - ETA BINS HISTOGRAMS
        for (Int_t i = 0; i < lenEtaBins-1; ++i)
        {
            if      (fabs(in_OfflineTau_eta) > etaBins[i] and fabs(in_OfflineTau_eta) < etaBins[i+1]) pt_resp_AbsEtaBin[i]->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            if      (in_OfflineTau_eta > etaBins[i] and in_OfflineTau_eta < etaBins[i+1])             pt_resp_PlusEtaBin[i]->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
            else if (in_OfflineTau_eta < -etaBins[i] and in_OfflineTau_eta > -etaBins[i+1])           pt_resp_MinusEtaBin[i]->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt) ;
        }

        Int_t k = 0;
        for (Int_t i = 0; i < lenPtBins-1; ++i)
        {
            for (Int_t j = 0; j < lenEtaBins-1; ++j)
            {
                if (fabs(in_OfflineTau_eta) > etaBins[j] and fabs(in_OfflineTau_eta) < etaBins[j+1] and in_OfflineTau_pt > ptBins[i] and in_OfflineTau_pt <= ptBins[i+1])
                    {
                        pt_resp_PtEtaBin[k]->Fill(in_L1Tau_CalibPt/in_OfflineTau_pt);
                    }
                k += 1;
            }
        }
    }
    f.Close();

    // NORMALIZE THE HISTOGRAMS
    pt_barrel_resp_ptInclusive->Scale(1.0/pt_barrel_resp_ptInclusive->Integral());
    pt_endcap_resp_ptInclusive->Scale(1.0/pt_endcap_resp_ptInclusive->Integral());
    pt_response_ptInclusive->Scale(1.0/pt_response_ptInclusive->Integral());
    for (Int_t i = 0; i < lenPtBins-1; ++i)
    {
        pt_resp_ptBin[i]->Scale(1.0/pt_resp_ptBin[i]->Integral());
        pt_barrel_resp_ptBin[i]->Scale(1.0/pt_barrel_resp_ptBin[i]->Integral());
        pt_endcap_resp_ptBin[i]->Scale(1.0/pt_endcap_resp_ptBin[i]->Integral());
    }
   
    for (Int_t i = 0; i < lenEtaBins-1; ++i)
    {
        pt_resp_AbsEtaBin[i]->Scale(1.0/pt_resp_AbsEtaBin[i]->Integral());
        pt_resp_PlusEtaBin[i]->Scale(1.0/pt_resp_PlusEtaBin[i]->Integral());
        pt_resp_MinusEtaBin[i]->Scale(1.0/pt_resp_MinusEtaBin[i]->Integral());
    }

    for (Int_t i = 0; i < lenPtBins-1; ++i)
    {
        pt_scale_fctPt->SetBinContent(i+1, pt_resp_ptBin[i]->GetMean());
        pt_scale_fctPt->SetBinError(i+1, pt_resp_ptBin[i]->GetMeanError());

        pt_resol_fctPt->SetBinContent(i+1, pt_resp_ptBin[i]->GetRMS()/pt_resp_ptBin[i]->GetMean());
        pt_resol_fctPt->SetBinError(i+1, pt_resp_ptBin[i]->GetRMSError()/pt_resp_ptBin[i]->GetMean());

        pt_resol_barrel_fctPt->SetBinContent(i+1, pt_barrel_resp_ptBin[i]->GetRMS()/pt_barrel_resp_ptBin[i]->GetMean());
        pt_resol_barrel_fctPt->SetBinError(i+1, pt_barrel_resp_ptBin[i]->GetRMSError()/pt_barrel_resp_ptBin[i]->GetMean());

        pt_resol_endcap_fctPt->SetBinContent(i+1, pt_endcap_resp_ptBin[i]->GetRMS()/pt_endcap_resp_ptBin[i]->GetMean());
        pt_resol_endcap_fctPt->SetBinError(i+1, pt_endcap_resp_ptBin[i]->GetRMSError()/pt_endcap_resp_ptBin[i]->GetMean());
    }

    for (Int_t i = 0; i < lenEtaBins-1; ++i)
    {
        pt_scale_fctEta->SetBinContent(lenEtaBins-1-i, pt_resp_MinusEtaBin[i]->GetMean());
        pt_scale_fctEta->SetBinError(lenEtaBins-1-i, pt_resp_MinusEtaBin[i]->GetMeanError());
        pt_scale_fctEta->SetBinContent(i+lenEtaBins, pt_resp_PlusEtaBin[i]->GetMean());
        pt_scale_fctEta->SetBinError(i+lenEtaBins, pt_resp_PlusEtaBin[i]->GetMeanError());

        pt_resol_fctEta->SetBinContent(lenEtaBins-1-i, pt_resp_MinusEtaBin[i]->GetRMS()/pt_resp_MinusEtaBin[i]->GetMean());
        pt_resol_fctEta->SetBinError(lenEtaBins-1-i, pt_resp_MinusEtaBin[i]->GetRMSError()/pt_resp_MinusEtaBin[i]->GetMean());
        pt_resol_fctEta->SetBinContent(i+lenEtaBins, pt_resp_PlusEtaBin[i]->GetRMS()/pt_resp_PlusEtaBin[i]->GetMean());
        pt_resol_fctEta->SetBinError(i+lenEtaBins, pt_resp_PlusEtaBin[i]->GetRMSError()/pt_resp_PlusEtaBin[i]->GetMean());
    }

    Int_t k = 0;
    for (Int_t i = 0; i < lenPtBins-1; ++i)
    {
        for (Int_t j = 0; j < lenEtaBins-1; ++j)
        {
            PTvsETA_resolution->SetBinContent(i,j,pt_resp_PtEtaBin[k]->GetRMS()/pt_resp_PtEtaBin[k]->GetMean());
            PTvsETA_resolution->SetBinError(i,j,pt_resp_PtEtaBin[k]->GetRMSError()/pt_resp_PtEtaBin[k]->GetMean());
            
            PTvsETA_scale->SetBinContent(i,j,pt_resp_PtEtaBin[k]->GetMean());
            PTvsETA_scale->SetBinError(i,j,pt_resp_PtEtaBin[k]->GetMeanError());

            k += 1;
        }
    }

    // REMOVE STATS PANEL
    gStyle->SetOptStat(000000);

    TPaveText* texl = new TPaveText(0.2,0.87,0.3,0.99,"NDC");
    texl->AddText("CMS Internal");
    texl->SetTextSize(0.03);
    texl->SetFillStyle(0);
    texl->SetBorderSize(0);
    
    //##########
    // response in pt bins

    TCanvas c1("c","c",1000,1000);
    // c1.SetRightMargin(0.15);
    c1.SetLeftMargin(0.15);
    c1.SetGrid(10,10);

    TLegend* leg1 = new TLegend(0.5,0.7,0.8,0.88);
    leg1->SetBorderSize(0);
    leg1->SetTextSize(0.03);

    leg1->SetHeader("pT inclusive");
    leg1->AddEntry(pt_barrel_resp_ptInclusive,"Barrel |#eta|<1.305","L");
    leg1->AddEntry(pt_endcap_resp_ptInclusive,"Endcap 1.479<|#eta|<5.191","L");

    pt_barrel_resp_ptInclusive->SetTitle("");
    pt_barrel_resp_ptInclusive->GetXaxis()->SetTitle("E_{T}^{L1 #tau}/p_{T}^{offline #tau}");
    pt_barrel_resp_ptInclusive->GetYaxis()->SetTitle("a.u");
    pt_barrel_resp_ptInclusive->GetYaxis()->SetRangeUser(0., max(pt_barrel_resp_ptInclusive->GetMaximum(),pt_barrel_resp_ptInclusive->GetMaximum())*1.2 );

    pt_barrel_resp_ptInclusive->SetLineColor(kRed);
    pt_barrel_resp_ptInclusive->SetLineWidth(2);
    pt_barrel_resp_ptInclusive->SetMarkerStyle(8);
    pt_barrel_resp_ptInclusive->SetMarkerColor(kRed);
    pt_endcap_resp_ptInclusive->SetLineColor(kBlack);
    pt_endcap_resp_ptInclusive->SetLineWidth(2);
    pt_endcap_resp_ptInclusive->SetMarkerStyle(8);
    pt_endcap_resp_ptInclusive->SetMarkerColor(kBlack);

    pt_barrel_resp_ptInclusive->Draw("E0");
    pt_endcap_resp_ptInclusive->Draw("E0 same");
    texl->Draw("same");
    leg1->Draw("same");

    c1.SaveAs("plots/optimizationV"+version+"/split/response_ptInclusive_optimizationV"+version+"_calibThr"+intgr+"p"+decim+".pdf");
    

    for (Int_t i = 0; i < lenPtBins-1; ++i)
    {
        TCanvas c("c","c",1000,1000);
        // c.SetRightMargin(0.15);
        c.SetLeftMargin(0.15);
        c.SetGrid(10,10);

        TLegend* leg = new TLegend(0.5,0.7,0.8,0.88);
        leg->SetBorderSize(0);
        leg->SetTextSize(0.03);
        
        leg->SetHeader(Form("%.0f < p_{T}^{offline #tau} < %.0f", ptBins[i], ptBins[i+1]));
        leg->AddEntry(pt_barrel_resp_ptBin[i],"Barrel |#eta|<1.305","L");
        leg->AddEntry(pt_endcap_resp_ptBin[i],"Endcap 1.479<|#eta|<5.191","L");

        pt_barrel_resp_ptBin[i]->SetTitle("");
        pt_barrel_resp_ptBin[i]->GetXaxis()->SetTitle("E_{T}^{L1 #tau}/p_{T}^{offline #tau}");
        pt_barrel_resp_ptBin[i]->GetYaxis()->SetTitle("a.u");
        pt_barrel_resp_ptBin[i]->GetYaxis()->SetRangeUser(0., max(pt_barrel_resp_ptBin[i]->GetMaximum(),pt_barrel_resp_ptBin[i]->GetMaximum())*1.2 );

        pt_barrel_resp_ptBin[i]->SetLineColor(kRed);
        pt_barrel_resp_ptBin[i]->SetLineWidth(2);
        pt_barrel_resp_ptBin[i]->SetMarkerStyle(8);
        pt_barrel_resp_ptBin[i]->SetMarkerColor(kRed);
        pt_endcap_resp_ptBin[i]->SetLineColor(kBlack);
        pt_endcap_resp_ptBin[i]->SetLineWidth(2);
        pt_endcap_resp_ptBin[i]->SetMarkerStyle(8);
        pt_endcap_resp_ptBin[i]->SetMarkerColor(kBlack);

        pt_barrel_resp_ptBin[i]->Draw("E0");
        pt_endcap_resp_ptBin[i]->Draw("E0 same");
        texl->Draw("same");
        leg->Draw("same");
        
        c.SaveAs(Form("plots/optimizationV"+version+"/split/response_ptBin%.0fto%.0f_optimizationV"+version+"_calibThr"+intgr+"p"+decim+".pdf",ptBins[i], ptBins[i+1]));
    }

    //##########
    // plot response in eta bins

    for (Int_t i = 0; i < lenEtaBins-1; ++i)
    {
        TCanvas c("c","c",1000,1000);
        // c.SetRightMargin(0.15);
        c.SetLeftMargin(0.15);
        c.SetGrid(10,10);

        TLegend* leg = new TLegend(0.5,0.7,0.8,0.88);
        leg->SetBorderSize(0);
        leg->SetTextSize(0.03);

        leg->SetHeader(Form("%.1f < eta^{offline #tau} < %.1f", etaBins[i], etaBins[i+1]));
        
        pt_resp_AbsEtaBin[i]->SetTitle("");
        pt_resp_AbsEtaBin[i]->GetXaxis()->SetTitle("E_{T}^{L1 #tau}/p_{T}^{offline #tau}");
        pt_resp_AbsEtaBin[i]->GetYaxis()->SetTitle("a.u");
        pt_resp_AbsEtaBin[i]->GetYaxis()->SetRangeUser(0., max(pt_resp_AbsEtaBin[i]->GetMaximum(),pt_resp_AbsEtaBin[i]->GetMaximum())*1.2 );

        pt_resp_AbsEtaBin[i]->SetLineColor(kBlack);
        pt_resp_AbsEtaBin[i]->SetLineWidth(2);
        pt_resp_AbsEtaBin[i]->SetMarkerStyle(8);
        pt_resp_AbsEtaBin[i]->SetMarkerColor(kBlack);

        pt_resp_AbsEtaBin[i]->Draw("E0");
        texl->Draw("same");
        leg->Draw("same");
        
        c.SaveAs(Form("plots/optimizationV"+version+"/split/response_etaBin%.1fto%.1f_optimizationV"+version+"_calibThr"+intgr+"p"+decim+".pdf", etaBins[i], etaBins[i+1]));
    }

    //##########
    // plot resolution in pt

    TCanvas c2("c2","c2",1000,1000);
    // c2.SetRightMargin(0.15);
    c2.SetLeftMargin(0.15);
    c2.SetGrid(10,10);
    c2.cd();

    pt_resol_endcap_fctPt->SetLineColor(kBlack);
    pt_resol_endcap_fctPt->SetMarkerStyle(8);
    pt_resol_endcap_fctPt->SetMarkerColor(kBlack);
    pt_resol_endcap_fctPt->SetTitle("");
    pt_resol_endcap_fctPt->GetXaxis()->SetTitle("p_{T}^{offline #tau} [GeV]");
    pt_resol_endcap_fctPt->GetYaxis()->SetTitle("p^{L1 #tau}_{T} resolution");
    pt_resol_endcap_fctPt->GetYaxis()->SetRangeUser(0.12,0.27);
    pt_resol_endcap_fctPt->Draw("E0");
    pt_resol_barrel_fctPt->SetLineColor(kRed);
    pt_resol_barrel_fctPt->SetMarkerStyle(8);
    pt_resol_barrel_fctPt->SetMarkerColor(kRed);
    pt_resol_barrel_fctPt->SetTitle("");
    pt_resol_barrel_fctPt->Draw("E0 same");

    TLegend* leg2 = new TLegend(0.5,0.7,0.8,0.88);
    leg2->SetBorderSize(0);
    leg2->SetTextSize(0.03);

    leg2->AddEntry(pt_resol_barrel_fctPt,"Barrel |#eta|<1.305","L");
    leg2->AddEntry(pt_resol_endcap_fctPt,"Endcap 1.479<|#eta|<5.191","L");
    leg2->Draw("same");
    texl->Draw("same");

    c2.SaveAs("plots/optimizationV"+version+"/pt_resolution_optimizationV"+version+"_calibThr"+intgr+"p"+decim+"_fctPt.pdf");

    //##########
    // plot resolution in eta bins

    TCanvas c3("c3","c3",1000,1000);
    // c3.SetRightMargin(0.15);
    c3.SetLeftMargin(0.15);
    c3.SetGrid(10,10);
    c3.cd();

    pt_resol_fctEta->SetLineColor(kRed);
    pt_resol_fctEta->SetMarkerStyle(8);
    pt_resol_fctEta->SetMarkerColor(kRed);
    pt_resol_fctEta->SetTitle("");
    pt_resol_fctEta->GetXaxis()->SetTitle("#eta^{offline #tau}");
    pt_resol_fctEta->GetYaxis()->SetTitle("p^{L1 #tau}_{T} resolution");
    pt_resol_fctEta->GetYaxis()->SetRangeUser(0.2,0.3);
    pt_resol_fctEta->Draw("E0");
    texl->Draw("same");

    c3.SaveAs("plots/optimizationV"+version+"/pt_resolution_optimizationV"+version+"_calibThr"+intgr+"p"+decim+"_fctEta.pdf");

    //##########
    // plot response in pt vs. eta

    TCanvas c4("c4","c4",1000,1000);
    c4.SetRightMargin(0.15);
    c4.SetLeftMargin(0.15);
    c4.SetGrid(10,10);
    c4.cd();

    PTvsETA_resolution->SetTitle("");
    PTvsETA_resolution->GetXaxis()->SetTitle("p_{T}^{offline #tau} [GeV]");
    PTvsETA_resolution->GetYaxis()->SetTitle("#eta^{offline #tau} [GeV]");
    PTvsETA_resolution->GetZaxis()->SetTitle("Resolution");
    PTvsETA_resolution->Draw("colz");
    texl->Draw("same");

    c4.SaveAs("plots/optimizationV"+version+"/PTvsETA_resolution_optimizationV"+version+"_calibThr"+intgr+"p"+decim+".pdf");

    //##########
    // plot scale in pt vs. eta

    TCanvas c5("c4","c4",1000,1000);
    c5.SetRightMargin(0.15);
    c5.SetLeftMargin(0.15);
    c5.SetGrid(10,10);
    c5.cd();

    PTvsETA_scale->SetTitle("");
    PTvsETA_scale->GetXaxis()->SetTitle("p_{T}^{offline #tau} [GeV]");
    PTvsETA_scale->GetYaxis()->SetTitle("#eta^{offline #tau} [GeV]");
    PTvsETA_scale->GetZaxis()->SetTitle("Scale");
    PTvsETA_scale->Draw("colz");
    texl->Draw("same");

    c5.SaveAs("plots/optimizationV"+version+"/ptVSeta_scale_optimizationV"+version+"_calibThr"+intgr+"p"+decim+".pdf");
}
