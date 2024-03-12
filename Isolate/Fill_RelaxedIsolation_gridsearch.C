#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <iostream>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TF1.h>
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
#include "../Calibrate/ApplyCalibration.C"

using namespace std;

// VARIABLES DEFINING THE GRID SEARCH PHASE SPACE
const UInt_t NEffsMin = 10;
const UInt_t NEmins = 13;
const UInt_t NEmaxs_sum = 18;
const Float_t EffsMin[NEffsMin] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
const Float_t Emins[NEmins] = {10., 13., 16., 19., 22., 25., 28., 31., 34., 37., 40., 43., 46.};
const Float_t Emaxs_sum[NEmaxs_sum] = {15., 18., 21., 24., 27., 30., 33., 36., 39., 41., 44., 47., 50., 53., 56., 59., 61., 64.};

// for comparisons and debugging with old code 
// const UInt_t NEffsMin = 5;
// const UInt_t NEmins = 2;
// const UInt_t NEmaxs_sum = 2;
// const Double_t EffsMin[NEffsMin] = {0.1, 0.4, 0.5, 0.6, 0.7};
// const Double_t Emins[NEmins] = {20., 25.};
// const Double_t Emaxs_sum[NEmaxs_sum] = {15., 25.};

Double_t FindEfficiency_Progression(Double_t IEt, Double_t MinPt, Double_t Efficiency_low_MinPt, Double_t Reaching_100pc_at, TString parametrisation, Double_t Kfact)
{
  Double_t Efficiency = 0; 
  Double_t Pt = IEt/2.;

  if(parametrisation=="linear")
    {
      if(Pt>=Reaching_100pc_at) Efficiency = 1.;
      else if(Pt<MinPt) Efficiency = Efficiency_low_MinPt;
      else
        {
          Double_t Slope = (1.-Efficiency_low_MinPt)/(Reaching_100pc_at-MinPt);
          Efficiency = Slope*Pt + (1. - Slope*Reaching_100pc_at);
        }
    }
  else if(parametrisation=="quadratic")
    {
      if(Pt>=Reaching_100pc_at) Efficiency = 1.;
      else if(Pt<MinPt) Efficiency = Efficiency_low_MinPt;
      else
        {
          Double_t Slope = (1.-Efficiency_low_MinPt)/(Reaching_100pc_at-MinPt);
          Double_t Kmax  = Slope/(MinPt-Reaching_100pc_at);

          Efficiency = Slope*Pt + (1. - Slope*Reaching_100pc_at) + Kmax*Kfact * (Pt - MinPt) * (Pt - Reaching_100pc_at);
        }
    }
  else if(parametrisation=="sigmoid")
    {
      Efficiency = (1-Efficiency_low_MinPt) / (1+exp(-(Pt-(Reaching_100pc_at+MinPt)/2)*Kfact)) + Efficiency_low_MinPt;
    }

  if(Efficiency<0) Efficiency = 0.;
  if(Efficiency>=1) Efficiency = 1.;

  return Efficiency ;
}


void Fill_RelaxedIsolation_TH3(TString isolation_file, TString relaxation_file, TString compression = "compressed", TString parametrisation = "linear", Double_t Kfact = 0.0, bool byBin = false, float calibThr = 1.7)
{
    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    TString Kintgr = to_string(Kfact).substr(0, to_string(Kfact).find("."));
    TString Kdecim = to_string(Kfact).substr(2, to_string(Kfact).find("."));

    TString byBin_tag = "";
    if (byBin) { byBin_tag = "_byBin"; }

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

    std::map<TString,TF1*> fitsIsolation;
    TFile f_Isolation(isolation_file,"READ");
    TString TFileName = "";
    TFileName = relaxation_file;
    // "ROOTs4LUTs_2024/LUTrelaxation_Run3Summer23_caloParams_2023_v0_4.root";
    TFile LUTs_Options(TFileName,"RECREATE");

    for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
    {
        for(UInt_t i = 0 ; i < NbinsIEta-1 ; ++i)
        {
            for(UInt_t j = 0 ; j < NbinsIEt-1 ; ++j)
            {
                TString fitName = "fit_pz_"+to_string(iEff)+"_eta"+to_string(i)+"_e"+to_string(j);
                TF1* current_Fit = (TF1*)f_Isolation.Get(fitName);
                fitsIsolation.insert(make_pair(fitName,current_Fit));
            }
        }
    }

    std::map<TString,TH3F*> histosIsolation;
    for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
    {
        TString currentHistoName = "Eff_"+to_string(iEff);
        TH3F* currentHisto = (TH3F*)f_Isolation.Get(currentHistoName);
        histosIsolation.insert(make_pair(currentHistoName,currentHisto));
    }

    // START OF GRID SEARCH
    for (UInt_t iEff = 0; iEff < NEffsMin; ++iEff)
    {
        for (UInt_t iEmin = 0; iEmin < NEmins; ++iEmin)
        {
            for (UInt_t iEmax = 0; iEmax < NEmaxs_sum; ++iEmax)
            {
                Double_t effMin = EffsMin[iEff];
                TString effMin_intgr = to_string(effMin).substr(0, to_string(effMin).find("."));
                TString effMin_decim = to_string(effMin).substr(2, to_string(effMin).find("."));
                Double_t Emin = Emins[iEmin];
                Double_t Emax = Emins[iEmin] + Emaxs_sum[iEmax];
                TString HistoName = "LUT_progression_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(int(Emin))+"_eMax"+to_string(int(Emax));
                TH3F* currentHisto = new TH3F(HistoName, HistoName, NbinsIEta-1 , 0, NbinsIEta-1, NbinsIEt-1, 0, NbinsIEt-1, NbinsnTT-1, 0, NbinsnTT-1);
                // std::cout << " Running " << HistoName << std::endl;

                // LOOP OVER THE ETA-E-NTT BINS
                for(UInt_t i = 0 ; i < NbinsIEta-1 ; ++i)
                {
                    for(UInt_t j = 0 ; j < NbinsIEt-1 ; ++j)
                    {
                        for(UInt_t k = 0 ; k < NbinsnTT-1 ; ++k)
                        {
                            if (!byBin)
                            {
                                // New fit-based filling of the LUT for checks
                                Double_t EfficiencyProgression = FindEfficiency_Progression((hardcodedCompressedIetBins[j]+hardcodedCompressedIetBins[j+1])/2., Emin, effMin, Emax, parametrisation, Kfact);
                                if(EfficiencyProgression>=0.9999) EfficiencyProgression = 1.0001;
                                Int_t IntEfficiencyProgression = int(EfficiencyProgression*100);
                                Int_t IsoCut;
                                if(IntEfficiencyProgression==100) { IsoCut = 1000; }
                                else
                                {
                                    TString IsoProjectionFitName = "fit_pz_"+to_string(IntEfficiencyProgression)+"_eta"+to_string(i)+"_e"+to_string(FindBinCorrespondenceIEt(hardcodedCompressedIetBins[j]));
                                    TF1* currentFit = (TF1*)fitsIsolation[IsoProjectionFitName];
                                    IsoCut = Int_t(currentFit->GetParameter(0) + currentFit->GetParameter(1) * FindBinCorrespondencenTT(hardcodedCompressednTTBins[k]));
                                    if (IsoCut < 0.) { IsoCut = 0; } // safety against negative fitte values
                                }
                                currentHisto->SetBinContent(i+1,j+1,k+1,IsoCut);
                                // if(j+1!=32) { currentHisto->SetBinContent(i+1,j+1,k+1,IsoCut); } // full relaxation in last energy bin
                                // else        { currentHisto->SetBinContent(i+1,j+1,k+1,1000); }
                            }
                            else
                            {
                                // Old per-bin filling of the LUT for checks
                                Double_t EfficiencyProgression = FindEfficiency_Progression((hardcodedCompressedIetBins[j]+hardcodedCompressedIetBins[j+1])/2., Emin, effMin, Emax, parametrisation, Kfact);
                                if(EfficiencyProgression>=0.9999) EfficiencyProgression = 1.0001;
                                Int_t IntEfficiencyProgression = int(EfficiencyProgression*100);
                                Int_t IsoCut;
                                if(IntEfficiencyProgression==100) { IsoCut = 1000; }
                                else
                                {
                                    TString EfficiencyName = "Eff_"+to_string(IntEfficiencyProgression);
                                    IsoCut = histosIsolation[EfficiencyName]->GetBinContent(i+1,FindBinCorrespondenceIEt(hardcodedCompressedIetBins[j])+1,FindBinCorrespondencenTT(hardcodedCompressednTTBins[k])+1);
                                }
                                currentHisto->SetBinContent(i+1,j+1,k+1,IsoCut);
                            }

                        }
                    }
                }
                // END OF LOOP OVER THE ETA-E-NTT BINS

                // store LUT
                currentHisto->Write();
            }
        }
    }
    // END OF GRID SEARCH

    LUTs_Options.Close();

}
