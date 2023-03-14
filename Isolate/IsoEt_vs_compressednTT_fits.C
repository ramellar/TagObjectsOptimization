#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <iostream>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TMath.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TProfile.h>
#include <sstream>
#include <TBranchElement.h>
#include <TGraphAsymmErrors.h>
#include <fstream>
#include "../Calibrate/ApplyCalibration_newnTT.C"

using namespace std;

// IF WE WANT TO USE THE FULL GRANULARUTY IN THE LUT, THEN HERE WE MUST USE THE COMPRESSED VALUES

void IsoEt_vs_compressednTT_fits(TString compression, float calibThr = 1.7)
{
    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    TChain data("outTreeCalibrated");
    data.Add("/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/2023_03_04_optimizationVx_calibThr"+intgr+"p"+decim+"/Tau_MC_CALIBRATED_2023_03_04.root");

    TFile f_out("ROOTs4LUTs/ROOTs4LUTs_2023/IsoEt_vs_compressednTT_fits_Trigger_Stage2_Run3_MC_optimizationVx_calibThr"+intgr+"p"+decim+".root","RECREATE");

    UInt_t tmpIEt;
    UInt_t tmpnTT;
    if (compression == "compressed")
    {
        tmpIEt = compressedNbinsIEt;
        tmpnTT = compressedNbinsnTT;
    }
    else if (compression == "supercompressed")
    {
        tmpIEt = supercompressedNbinsIEt;
        tmpnTT = supercompressedNbinsnTT;
    }
    else
    {
        std::cout << "Wrong compression request: compressed or supercompressed?" << std::endl;
        std::cout << "EXITING!" << std::endl;
        return;
    }
    const UInt_t NbinsIEt = tmpIEt;
    const UInt_t NbinsnTT = tmpnTT;


    // loop over all the bins in iEta, and iEt
    for(UInt_t i = 0 ; i < NbinsIEta-1 ; ++i)
    {
        for(UInt_t j = 0 ; j < NbinsIEt-1 ; ++j)
        {
            
            // make TString names
            TString twoDName = "isoEt_vs_compressednTT_eta"+to_string(i)+"_e"+to_string(j);
            TString profileName = "iso_vs_compressednTT_profile_eta"+to_string(i)+"_e"+to_string(j);
            TString fitName = "iso_vs_compressednTT_fit_eta"+to_string(i)+"_e"+to_string(j);
            TString selection = "(compressedieta=="+to_string(i)+") &&";

            if (compression == "compressed")           { selection += "(compressedE=="+to_string(j)+")"; }
            else if (compression == "supercompressed") { selection += "(supercompressedE=="+to_string(j)+")"; }

            // fill TH2 with isoET and nTT
            TH2F* isoEt_vs_compressednTT = new TH2F(twoDName,twoDName,31,0.,31.,100,0.,100.);
            data.Draw("L1Tau_Iso:compressednTT>>"+twoDName, selection, "colz");

            // profile and fit
            TProfile* iso_vs_compressednTT_profile = isoEt_vs_compressednTT->ProfileX(profileName);
            TF1* iso_vs_compressednTT_fit = new TF1(fitName,"[0]+[1]*x",0,31);
            iso_vs_compressednTT_profile->Fit(iso_vs_compressednTT_fit);

            // save
            isoEt_vs_compressednTT->Write();
            iso_vs_compressednTT_profile->Write();
            iso_vs_compressednTT_fit->Write();

            delete isoEt_vs_compressednTT;
            delete iso_vs_compressednTT_profile;
            delete iso_vs_compressednTT_fit;

        }
    }

    // loop over all the bins in iEta
    for(UInt_t i = 0 ; i < NbinsIEta-1 ; ++i)
    {       
        // make TString names
        TString twoDName = "isoEt_vs_compressednTT_eta"+to_string(i);
        TString profileName = "iso_vs_compressednTT_profile_eta"+to_string(i);
        TString fitName = "iso_vs_compressednTT_fit_eta"+to_string(i);
        TString selection = "compressedieta=="+to_string(i);

        // fill TH2 with isoET and nTT
        TH2F* isoEt_vs_compressednTT = new TH2F(twoDName,twoDName,31,0.,31.,100,0.,100.);
        data.Draw("L1Tau_Iso:compressednTT>>"+twoDName, selection, "colz");

        // profile and fit
        TProfile* iso_vs_compressednTT_profile = isoEt_vs_compressednTT->ProfileX(profileName);
        TF1* iso_vs_compressednTT_fit = new TF1(fitName,"[0]+[1]*x",0,31);
        iso_vs_compressednTT_profile->Fit(iso_vs_compressednTT_fit);

        // save
        iso_vs_compressednTT_profile->Write();
        iso_vs_compressednTT_fit->Write();

        delete isoEt_vs_compressednTT;
        delete iso_vs_compressednTT_profile;
        delete iso_vs_compressednTT_fit;
    }

    f_out.Close();


}