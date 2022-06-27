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
#include <TGraphAsymmErrors.h>
#include <TVector.h>
#include <fstream>
#include <map>

using namespace std;

void compare(bool nTTRange = false, float calibThr = 1.7) {
    TString nTTthr = "60";

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

    gStyle->SetOptStat(000000);

    TFile* f_Run3_VBF = new TFile("/data_CMS/cms/motta/Run3preparation/2022_03_15_optimizationV10_calibThr"+intgr+"p"+decim+"/Run3_MC_VBFHToTauTau_M125_COMPRESSED_2022_03_15.root", "READ");
  
    TTree* inTree_Run3_VBF = (TTree*)f_Run3_VBF->Get("outTreeForCalibration");

    TCanvas c0("c0", "c0", 1300,600);
    inTree_Run3_VBF->Draw("compressedE:OfflineTau_pt", "OfflineTau_pt<200");
    c0.SaveAs("PDFs/compressedEvsOfflinePt.pdf");

    //------------------------------------------------------------------------------------------

    TCanvas c1("c1", "c1", 1300,600);
    inTree_Run3_VBF->Draw("compressedE:L1Tau_pt", "L1Tau_pt<100");
    c1.SaveAs("PDFs/compressedEvsL1TauPt.pdf");

    f_Run3_VBF->Close();

}
