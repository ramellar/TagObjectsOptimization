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
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <TROOT.h>
#include <sstream>
#include <TBranchElement.h>
#include <fstream>
#include "../Calibrate/ApplyCalibration.C"

void MakeTauIsoLUT_Options(TString inFile, TString outFile, Float_t effMin, Int_t Emin, Int_t Emax, float calibThr = 1.7, bool includeCompression = false)
{
  TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
  TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

  TString effMin_intgr = to_string(effMin).substr(0, to_string(effMin).find("."));
  TString effMin_decim = to_string(effMin).substr(2, to_string(effMin).find("."));

  TFile* fLUTS = new TFile (inFile, "READ");

  std::cout<<"INFILE = " << inFile << std::endl;
  std::cout<<"OUTFILE = " << outFile << std::endl;
  
  TH3F* LUT ;
 //   TString LUT_name = "LUT_progression_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
 //   TString LUT_name = "LUT_progression_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
    //   TString LUT_name = "LUTs/LUTs_2024/LUTrelaxation_optimization24_23Summer_progression_effMin" +
    //                   effMin_intgr + "p" + 
    //                   effMin_decim + 
    //                   "_eMin" + TString(to_string(Emin).c_str()) + 
    //                   "_eMax" + TString(to_string(Emax).c_str()) + ".txt";
 TString LUT_name = "LUT_progression_effMin" + effMin_intgr + "p" + 
                   effMin_decim + "_eMin" + TString::Format("%d", Emin) + 
                   "_eMax" + TString::Format("%d", Emax);


  LUT = (TH3F*) fLUTS->Get ( LUT_name.Data());
  
  TH3F* binning = new TH3F ("binning", "binning", NbinsIEta-1, hardcodedIetaBinsFloat, compressedNbinsIEt-1, hardcodedCompressedIetBinsFloat, compressedNbinsnTT-1, hardcodedCompressednTTBinsFloat);
  std::cout << "LUT name: " << LUT->GetName() << std::endl;
  std::cout << "Max LUT CuT: " << LUT->GetMaximum() << std::endl;
  std::cout << "Output cmpr bins: " << NbinsIEta-1 << " " << compressedNbinsIEt-1 << " " << compressedNbinsnTT-1 << std::endl;  

  std::ofstream LUTfile(outFile.Data());  

  ////////////////////////////////////////////////////////////////////////////
  //// print header block on top of the LUT //////////////////////////////////

  int etaBits = 6;
  int etBits  = 13;
  int nTTBits = 10;
  int TotalIDX = 0;
    
  int cmprEta = 2;
  int cmprEt = 5;
  int cmprnTT = 5;

  int totInBits  = 12 ;
  int totOutBits = 9 ;  // i.e. cut max 127, but it is largely overestimated: max in LUT is 27 --> 5 bits are enough

  LUTfile << "# Tau isolation LUT from V9 optimization for effMin " << effMin << " Emin " << Emin << " Emax " << Emax << "" << std::endl;
  if(includeCompression) LUTfile << "# on top of the table there is the in/out compression scheme, followed by the LUT" << std::endl;
  LUTfile << "# iso LUT structure is ieta --> iEt -->  nTT" << std::endl;
  if(includeCompression) LUTfile << "# Input bits: ieta: " << etaBits << " iEt " << etBits << " nTT: " << nTTBits << std::endl;
  LUTfile << "# Compr bits: ieta: " << cmprEta << " iEt: " << cmprEt << " nTT: " << cmprnTT  << std::endl;
  LUTfile << "#<header> V10.0 " << totInBits << " " << totOutBits << " </header>" << std::endl;
  LUTfile << "\n";


  ////////////////////////////////////////////////////////////////////////////
  //// print compression block on top of the LUT /////////////////////////////

    if(includeCompression)
    {
        ////////// iEt compr
        for (int i = 0; i < pow(2, etBits); i++, TotalIDX++)
        {
            int BinEt = binning->GetYaxis()->FindBin (i);
            if (BinEt < 1) BinEt = 1;
            if (BinEt > compressedNbinsIEt-1) BinEt = compressedNbinsIEt-1;
            LUTfile << TotalIDX << " " << BinEt-1;
            if (i == 0) LUTfile << " # begin of ET compr block";
            LUTfile << std::endl;
        }

        ////////// inTT compr
        for (int i = 0; i < pow(2, nTTBits); i++, TotalIDX++)
        {
            int BinnTT = binning->GetZaxis()->FindBin (i);
            if (BinnTT < 1) BinnTT = 1;
            if (BinnTT > compressedNbinsnTT-1) BinnTT = compressedNbinsnTT-1;
            LUTfile << TotalIDX << " " << BinnTT-1;
            if (i == 0) LUTfile << " # begin of nTT compr block";
            LUTfile << std::endl;
        }

        ////////// iEta compr
        for (int i = 0; i < pow(2, etaBits); i++, TotalIDX++)
        {
            int BinEta = binning->GetXaxis()->FindBin (i);
            if (BinEta < 1) BinEta = 1;
            if (BinEta > NbinsIEta-1) BinEta = NbinsIEta-1;
            LUTfile << TotalIDX << " " << BinEta-1;
            if (i == 0) LUTfile << " # begin of eta compr block";
            LUTfile << std::endl;
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    //// print isolation LUT ///////////////////////////////////////////////////

    // structure: Et -- eta -- nTT
    int onlyIsoAddr = 0;
    for (int ieta = 0; ieta < pow(2, cmprEta); ieta++)
    {
        for (int iEt = 0; iEt < pow(2, cmprEt); iEt++)
        {
            for (int inTT = 0; inTT < pow(2, cmprnTT); inTT++)
            {
                int thr = LUT->GetBinContent(ieta+1, iEt+1, inTT+1)+1; //why was there this +1 ????
                if(thr>=pow(2,totOutBits)-1) thr=pow(2,totOutBits)-1;
                LUTfile << TotalIDX << " " << thr;
                if (iEt == 0 && ieta == 0 && inTT == 0) LUTfile << " # start of isolation LUT -- ieta : iEt : nTT = " << ieta << " : " << iEt << " : " << inTT; 
                else LUTfile << " # ieta : iEt : inTT = " << ieta << " : " << iEt << " : " << inTT; 
                LUTfile << std::endl;
                std::cout << onlyIsoAddr << " # ieta : iEt : inTT = " << ieta << " : " << iEt << " : " << inTT <<  " --> thr: " << thr << std::endl; 
                TotalIDX++;
                onlyIsoAddr++;
            }
        }
    }

    int totIDXexpected = pow(2,etaBits) + pow(2,etBits) + pow (2, nTTBits) + pow (2, cmprEta+cmprEt+cmprnTT);
    std::cout << "TOTAL IDX " << TotalIDX << " expected: " << totIDXexpected << std::endl;
    std::cout << "Max allowed by input LUT bits: " << pow(2, totInBits) << std::endl;
    if (TotalIDX > pow(2, totInBits))  { std::cout << " **** WARNING: too few input bits " << totInBits << " , please increase value in header" << std::endl; }
    if (TotalIDX == pow(2, totInBits)) { std::cout << " **** INFO: matched exact number of input bits " << totInBits << std::endl; }


}
