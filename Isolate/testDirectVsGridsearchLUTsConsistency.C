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


void test(TString oldName, TString newName)
{

    // #############################################################
    // #############################################################
    // test LUTs

    TFile oldFile("ROOTs4LUTs/ROOTs4LUTs_2023/LUTrelaxation_Trigger_Stage2_Run3_MC_optimizationV0p2_calibThr1p7_linear_old.root");
    TFile newFile("ROOTs4LUTs/ROOTs4LUTs_2023/LUTrelaxation_Trigger_Stage2_Run3_MC_optimizationV0p2gs_calibThr1p7_linear_byBin.root");

    TH1F* oldH = (TH1F*)oldFile.Get("LUT_Progression_"+oldName);
    TH1F* newH = (TH1F*)newFile.Get("LUT_progression_"+newName);

    Int_t counter = 0;
    Int_t total = 0;
    // LOOP OVER THE ETA-E-NTT BINS
    for(UInt_t i = 0 ; i < NbinsIEta-1 ; ++i)
    {
        for(UInt_t j = 0 ; j < compressedNbinsIEt-1 ; ++j)
        {
            for(UInt_t k = 0 ; k < compressedNbinsnTT-1 ; ++k)
            {
                total += 1; 
                
                Float_t oldF = oldH->GetBinContent(i+1, j+1, k+1);
                Float_t newF = newH->GetBinContent(i+1, j+1, k+1);

                if (oldF != newF)
                { 
                    std::cout << "Bin " << i+1 << "-" << j+1 << "-" << k+1 << " differs: oldF=" << oldF << " newF=" << newF <<std::endl;
                    counter += 1;
                }
            }
        }
    }

    std::cout << "counter/total = " << counter << "/" << total <<std::endl;

}
