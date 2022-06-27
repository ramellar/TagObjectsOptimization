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

void MakeTauIsoLUT_Options(TString effMin, string option, bool includeCompression, float calibThr = 1.7)
{
  TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
  TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

  TFile* fLUTS = new TFile ("ROOTs4LUTs/LUTrelaxation_Trigger_Stage2_Run3_MC_VBFHToTauTau_M125_optimizationV11gs_calibThr"+intgr+"p"+decim+"_"+effMin+".root","READ");
  TString outFile = "LUTs/LUTrelaxation_Trigger_Stage2_Run3_MC_VBFHToTauTau_M125_optimizationV11gs_calibThr"+intgr+"p"+decim+"_"+effMin+"_"+option+".txt";
  cout<<"OUTFILE = "<<outFile<<endl;

  // TH3F* LUTS[100];
  // for (int i = 0; i < 100; i++)
  // {
  //     TString name = Form("Option_postfit_%i_percent", option);
  //     LUTS[i] = (TH3F*) fLUTS->Get(name);
  // }
  
  TH3F* LUT ;
  TString LUT_name = "LUT_Progression_"+option;
  LUT = (TH3F*) fLUTS->Get ( LUT_name.Data());
  //else LUT = (TH3F*) fLUTS->Get ( "LUT_WP"+option);
  cout << "LUT name: " << LUT->GetName() << endl;

  cout << "Max LUT CuT: " << LUT->GetMaximum() << endl;

  //float hardcodedIetaBins[]    = {0, 5, 9, 17, 28};
  //float hardcodedIetBins[]     = {0, 20, 28, 37, 42, 52, 63, 73};
  //float hardcodednTTBins[]     = {0, 15, 17, 20, 25, 32, 40, 60};

  // float hardcodedIetaBins[5]  = {0, 5, 9, 17, 28};
  // float hardcodedIetBins [17] = {0, 18, 20, 22, 28, 32, 37, 42, 52, 63, 73, 81, 87, 91, 111, 151, 256};
  // float hardcodednTTBins [8]  = {0, 15, 17, 20, 25, 32, 40, 60};

  // const Float_t hardcodedIetaBins[] = {0, 6, 12, 18, 33};
  // const Float_t hardcodedIetBins[]  = {0, 15, 18, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 50, 53, 56, 59, 62, 65, 69, 73, 77, 82, 88, 95, 105, 120, 157, 255};
  // const Float_t hardcodednTTBins[] = {0, 9, 11, 13, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 35, 37, 39, 41, 43, 46, 49, 53, 60, 255};

  const Float_t hardcodedIetaBins[] = {0, 6, 12, 18, 33};
  const Float_t hardcodedIetBins[]  = {0, 15, 18, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 50, 53, 56, 59, 62, 65, 69, 73, 77, 82, 88, 95, 105, 120, 157, 255};
  const Float_t hardcodednTTBins[] = {0, 6, 11, 16, 21, 26, 31, 36, 41, 46, 51, 56, 61, 66, 71, 76, 81, 86, 91, 96, 101, 106, 111, 116, 121, 126, 131, 136, 141, 146, 161, 161, 255};

  //cout<<"here"<<endl;

  int neta = sizeof(hardcodedIetaBins) / sizeof(int) -1 ;
  int nEt  = sizeof(hardcodedIetBins) / sizeof(int)-1;
  int nnTT = sizeof(hardcodednTTBins) / sizeof(int)-1;

  TH3F* binning = new TH3F ("binning", "binning", neta, hardcodedIetaBins, nEt, hardcodedIetBins, nnTT, hardcodednTTBins);

  float maxeta = binning->GetXaxis()->GetBinLowEdge(LUT->GetNbinsX()+1);
  float maxEt  = binning->GetYaxis()->GetBinLowEdge(LUT->GetNbinsY()+1);
  float maxnTT = binning->GetZaxis()->GetBinLowEdge(LUT->GetNbinsZ()+1);

  cout << maxeta << " " << maxEt << " " << maxnTT << endl;

  cout << "Output cmpr bins: " << neta << " " << nEt << " " << nnTT << endl;  

  std::ofstream LUTfile(outFile.Data());

  ////////////////////////////////////////////////////////////////////////////
  //// print compression block on top of the LUT /////////////////////////////

  int etaBits = 6;
  int etBits  = 13;
  int nTTBits = 10;

  int TotalIDX = 0;

  ////////////////////////////////////////////////////////////////////////////
  ///// print header
  ////////////////////////////////////////////////////////////////////////////
  //# comments...
  //#<header> V1 12 7 </header>
  // eta compr bits: 2
  // Et compr bits:  3
  // nTT compr bits: 3
  // input bits: 2**6 + 2**13 + 2**10 + 2**(2+3+3) = 271k ==> stays on 14 bits
    
  int cmprEta = 2;
  //int cmprEt = 3;
  int cmprEt = 5;
  int cmprnTT = 5;

  // int cmprEta = 2;
  // //int cmprEt = 3;
  // int cmprEt = 4;
  // int cmprnTT = 3;

  //int totInBits  = 12 ;
  int totInBits  = 12 ;
  int totOutBits = 9 ;  // i.e. cut max 127, but it is largely overestimated: max in LUT is 27 --> 5 bits are enough
  // int totOutBits = 8 ;  // i.e. cut max 127, but it is largely overestimated: max in LUT is 27 --> 5 bits are enough
  LUTfile << "# Tau isolation LUT from V9 optimization for " << effMin << " and option " << option << "" << endl;
  if(includeCompression) LUTfile << "# on top of the table there is the in/out compression scheme, followed by the LUT" << endl;
  LUTfile << "# iso LUT structure is ieta --> iEt -->  nTT" << endl;
  // LUTfile << "# iso LUT structure is Et --> nTT -->  eta" << endl;
  if(includeCompression) LUTfile << "# Input bits: ieta: " << etaBits << " iEt " << etBits << " nTT: " << nTTBits << endl;
  // if(includeCompression) LUTfile << "# Input bits: et: " << etBits << " nTT: " << nTTBits << " eta: " << etaBits << endl;
  LUTfile << "# Compr bits: ieta: " << cmprEta << " iEt: " << cmprEt << " nTT: " << cmprnTT  << endl;
  // LUTfile << "# Compr bits: et: " << cmprEt << " nTT: " << cmprnTT << " eta: " << cmprEta  << endl;
  // LUTfile << "# iso LUT structure is Et --> eta -->  nTT" << endl;
  // LUTfile << "# Input bits: eta: " << etaBits << " et: " << etBits << " nTT: " << nTTBits << endl;
  // LUTfile << "# Compr bits: eta: " << cmprEta << " et: " << cmprEt << " nTT: " << cmprnTT << endl;
  LUTfile << "#<header> V10.0 " << totInBits << " " << totOutBits << " </header>"<<endl;
  LUTfile << "\n";

  cout<<"here"<<endl;

  if(includeCompression)
    {

      ////////// iEt compr
      for (int i = 0; i < pow(2, etBits); i++, TotalIDX++)
	{
	  int BinEt = binning->GetYaxis()->FindBin (i);
	  if (BinEt < 1) BinEt = 1;
	  if (BinEt > nEt) BinEt = nEt;
	  LUTfile << TotalIDX << " " << BinEt-1;
	  if (i == 0) LUTfile << " # begin of ET compr block";
	  LUTfile << endl;
	}

      ////////// inTT compr
      for (int i = 0; i < pow(2, nTTBits); i++, TotalIDX++)
	{
	  int BinnTT = binning->GetZaxis()->FindBin (i);
	  if (BinnTT < 1) BinnTT = 1;
	  if (BinnTT > nnTT) BinnTT = nnTT;
	  LUTfile << TotalIDX << " " << BinnTT-1;
	  if (i == 0) LUTfile << " # begin of nTT compr block";
	  LUTfile << endl;
	}

      ////////// iEta compr
      for (int i = 0; i < pow(2, etaBits); i++, TotalIDX++)
	{
	  int BinEta = binning->GetXaxis()->FindBin (i);
	  if (BinEta < 1) BinEta = 1;
	  if (BinEta > neta) BinEta = neta;
	  LUTfile << TotalIDX << " " << BinEta-1;
	  if (i == 0) LUTfile << " # begin of eta compr block";
	  LUTfile << endl;
	}

    }

  ////////////////////////////////////////////////////////////////////////////
  //// print isolation LUT ///////////////////////////////////////////////////

  cout<<"here"<<endl;

  // structure: Et -- eta -- nTT
  int onlyIsoAddr = 0;
  for (int ieta = 0; ieta < pow(2, cmprEta); ieta++)
    {
      for (int iEt = 0; iEt < pow(2, cmprEt); iEt++)
	{
	  for (int inTT = 0; inTT < pow(2, cmprnTT); inTT++)
	    {
	      int binEt = iEt;
	      int bineta = ieta;
	      int binnTT = inTT;
                
	      if (binEt >= nEt)   binEt  = nEt -1;   // these are never used -- values that are outside the compression
	      if (bineta >= neta) bineta = neta -1;
	      if (binnTT >= nnTT) binnTT = nnTT -1;

	      int thr = LUT->GetBinContent(bineta+1,binEt+1,binnTT+1)+1;
	      // if(ieta==3 && ((iEt==2) || (iEt==3)) && inTT>=11) thr = LUT->GetBinContent(bineta,binEt+1,binnTT+1)+1;
	      if(thr>=pow(2,totOutBits)-1) thr=pow(2,totOutBits)-1;
	      LUTfile << TotalIDX << " " << thr;
	      if (iEt == 0 && ieta == 0 && inTT == 0) LUTfile << " # start of isolation LUT -- ieta : iEt : nTT = " << ieta << " : " << iEt << " : " << inTT; 
	      // if (iEt == 0 && ieta == 0 && inTT == 0) LUTfile << " # start of isolation LUT -- iEt : ieta : inTT = " << iEt << " : " << ieta << " : " << inTT; 
	      else LUTfile << " # ieta : iEt : inTT = " << ieta << " : " << iEt << " : " << inTT; 
	      // else LUTfile << " # iEt : inTT : ieta = " << iEt << " : " << inTT << " : " << ieta; 
	      LUTfile << endl;
	      cout << onlyIsoAddr << " # ieta : iEt : inTT = " << ieta << " : " << iEt << " : " << inTT <<  " --> thr: " << thr << endl; 
	      // cout << onlyIsoAddr << " iEt : inTT : ieta = " << iEt << " : " << inTT << " : " << ieta <<  " --> thr: " << thr << endl; 
	      TotalIDX++;
	      onlyIsoAddr++;
            }
        }
    }

  int totIDXexpected = pow(2,etaBits) + pow(2,etBits) + pow (2, nTTBits) + pow (2, cmprEta+cmprEt+cmprnTT);
  cout << "TOTAL IDX " << TotalIDX << " expected: " << totIDXexpected << endl;
  cout << "Max allowed by input LUT bits: " << pow(2, totInBits) << endl;
  if (TotalIDX >= pow(2, totInBits))
    {
      cout << " **** WARNING!!! too few input bits " << totInBits << " , please increase value in header" << endl;
    } 


}
