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
#include "../Calibrate/ApplyCalibration.C"

using namespace std;


// THE DEFAULT WE HAVE BEEN USING IN 2022 IS BUILD WITH SUPERCOMPRESSED, AND FILL+LUT WITH COMPRESSED

// The goal of the isaltion and relaxation procedure is to find the value of the Isolation threashold that then will be applied in the L1 level 2 algorithm

/*This function outputs:
  -Inclusive histograms:
    -done with the compressed variables in "../Calibrate/ApplyCalibration.C"
    -done with the calibrated variables given as input
  -It defines histograms per bins in eta, Et and nTT and 2 more main maps which are defined as :
    - HistosPerBin: which gives as output a string Histo_eta_Et_nTT associated to the corresponding histogram (in bins of eta, Et and nTT) showing the number of counts as a function of the isaltion energy
    - Isocut_Per_bin: which for a value of the efficiency gives the 3D histogram with eta, Et and nTT as axis containing the information of the IsoCuts for that efficiency
    - Iso_Per_effciiency_Per_bin: which gives the value of the isolation cut for each value of efficnecy and each histogram
  -From these maps ("dictionnaries") we plot in the output:
    -Each histogram (inclusive and per bins of eta, Et and nTT)
    -The efficiencies as a fct of pt, eta and nTT, that we refer to as flat efficiencies
    -The Iso Cut as a fct of nTT and we perform a fit in it to deal with the low statitics*/

void Build_Isolation(TString inputFile, TString outFile, UInt_t tmpFitMin = 3, UInt_t tmpFitMax = 14, TString compression = "supercompressed", float calibThr = 1.7)
{
  //To get the integer and decimal parts of the calibration threshold
  TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
  TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

  UInt_t tmpIEt;
  UInt_t tmpnTT;
  // UInt_t tmpFitMin;
  // UInt_t tmpFitMax;

  //Choosing the scheme: binning variables defined in "../Calibrate/ApplyCalibration.C"
  if (compression == "compressed")
  {
    tmpIEt = compressedNbinsIEt;
    tmpnTT = compressedNbinsnTT;
    // tmpFitMin = 6;
    // tmpFitMax = 25;
  }
  else if (compression == "supercompressed")
  {
    tmpIEt = supercompressedNbinsIEt;
    tmpnTT = supercompressedNbinsnTT;
    // tmpFitMin = 0;
    // tmpFitMax = 8;
  }
  else
  {
    std::cout << "Wrong compression request: compressed or supercompressed?" << std::endl;
    std::cout << "EXITING!" << std::endl;
    return;
  }

  // Defining binning and limits of the fit 
  const UInt_t NbinsIEt = tmpIEt;
  const UInt_t NbinsnTT = tmpnTT;
  const UInt_t FitMin = tmpFitMin;
  const UInt_t FitMax = tmpFitMax;

  /*TChain is a class in ROOT that acts like a "chain" of ROOT trees. It allows you to process 
  all files as if they were a single, continuous dataset.*/
  TChain data("outTreeCalibrated");
  data.Add(inputFile); //in this case only the input file is udes but other files can be added and chained

  // "/data_CMS/cms/mchiusi/Run3preparation/Run3_2024/2023S-MC_caloParams_2023_v0_4/Run3Summer23_caloParams_2023_v0_4_CALIBRATED.root"

  // We define the 2D histograms to inclusively plot the isolation energy as a function of other variables
  TH2F* isoEt_vs_nVtx = new TH2F("isoEt_vs_nVtx","isoEt_vs_nVtx",150,0.,150.,100,0.,100.);
  TH2F* isoEt_vs_nVtx_barrel = new TH2F("isoEt_vs_nVtx_barrel","isoEt_vs_nVtx_barrel",150,0.,150.,100,0.,100.);
  TH2F* isoEt_vs_nVtx_endcaps = new TH2F("isoEt_vs_nVtx_endcaps","isoEt_vs_nVtx_endcaps",150,0.,150.,100,0.,100.);

  TH2F* isoEt_vs_nTT = new TH2F("isoEt_vs_nTT","isoEt_vs_nTT",150,0.,150.,100,0.,100.);
  TH2F* isoEt_vs_nTT_barrel = new TH2F("isoEt_vs_nTT_barrel","isoEt_vs_nTT_barrel",150,0.,150.,100,0.,100.);
  TH2F* isoEt_vs_nTT_endcaps = new TH2F("isoEt_vs_nTT_endcaps","isoEt_vs_nTT_endcaps",150,0.,150.,100,0.,100.);

  TH2F* isoEt_vs_compressednTT = new TH2F("isoEt_vs_compressednTT","isoEt_vs_compressednTT",31,0.,31.,100,0.,100.);
  TH2F* isoEt_vs_compressednTT_barrel = new TH2F("isoEt_vs_compressednTT_barrel","isoEt_vs_compressednTT_barrel",31,0.,31.,100,0.,100.);
  TH2F* isoEt_vs_compressednTT_endcaps = new TH2F("isoEt_vs_compressednTT_endcaps","isoEt_vs_compressednTT_endcaps",31,0.,31.,100,0.,100.);

  /*Plotting the 2D histograms: we define the y axis as L1Tau and x axis as NvtX, ntt or compressed ntt
  There is a cut on eta for in order to defines barrel and endcap regions, colz is just a color scheme
  The data.Draw("varY:varX>>histName", "cut", "option") syntax is a convenient way in ROOT 
  to both project data into a histogram and fill it at the same time
  The variables with which the histo is filled are defined in "../Calibrate/ApplyCalibration.C" or form the initialized tree? */

  data.Draw("L1Tau_Iso:Nvtx>>isoEt_vs_nVtx","","colz");
  data.Draw("L1Tau_Iso:Nvtx>>isoEt_vs_nVtx_barrel","abs(OfflineTau_eta)<1.5","colz");
  data.Draw("L1Tau_Iso:Nvtx>>isoEt_vs_nVtx_endcaps","abs(OfflineTau_eta)>1.5","colz");

  data.Draw("L1Tau_Iso:L1Tau_nTT>>isoEt_vs_nTT","","colz");
  data.Draw("L1Tau_Iso:L1Tau_nTT>>isoEt_vs_nTT_barrel","abs(OfflineTau_eta)<1.5","colz");
  data.Draw("L1Tau_Iso:L1Tau_nTT>>isoEt_vs_nTT_endcaps","abs(OfflineTau_eta)>1.5","colz");

  data.Draw("L1Tau_Iso:compressednTT>>isoEt_vs_compressednTT","","colz");
  data.Draw("L1Tau_Iso:compressednTT>>isoEt_vs_compressednTT_barrel","abs(OfflineTau_eta)<1.5","colz");
  data.Draw("L1Tau_Iso:compressednTT>>isoEt_vs_compressednTT_endcaps","abs(OfflineTau_eta)>1.5","colz");

  /*Profiling of a 2D histogram : display the mean value of Y and its error for each bin in X 
  ( the standard deviation divided by the sqrt(n))*/

  TProfile* iso_vs_nvtx_profile = isoEt_vs_nVtx->ProfileX("iso_vs_nvtx_profile");
  TProfile* iso_vs_nvtx_barrel_profile = isoEt_vs_nVtx_barrel->ProfileX("iso_vs_nvtx_barrel_profile");
  TProfile* iso_vs_nvtx_endcaps_profile = isoEt_vs_nVtx_endcaps->ProfileX("iso_vs_nvtx_endcaps_profile");

  TProfile* iso_vs_nTT_profile = isoEt_vs_nTT->ProfileX("iso_vs_nTT_profile");
  TProfile* iso_vs_nTT_barrel_profile = isoEt_vs_nTT_barrel->ProfileX("iso_vs_nTT_barrel_profile");
  TProfile* iso_vs_nTT_endcaps_profile = isoEt_vs_nTT_endcaps->ProfileX("iso_vs_nTT_endcaps_profile");

  TProfile* iso_vs_compressednTT_profile = isoEt_vs_compressednTT->ProfileX("iso_vs_compressednTT_profile");
  TProfile* iso_vs_compressednTT_barrel_profile = isoEt_vs_compressednTT_barrel->ProfileX("iso_vs_compressednTT_barrel_profile");
  TProfile* iso_vs_compressednTT_endcaps_profile = isoEt_vs_compressednTT_endcaps->ProfileX("iso_vs_compressednTT_endcaps_profile");

  /*One dimensional function, with the name of the function, its functional form where parameters are left to be specified in the fitting and the range*/
  TF1* iso_vs_compressednTT_fit = new TF1("iso_vs_compressednTT_fit","[0]+[1]*x",0,31);
  TF1* iso_vs_compressednTT_barrel_fit = new TF1("iso_vs_compressednTT_barrel_fit","[0]+[1]*x",0,31);
  TF1* iso_vs_compressednTT_endcaps_fit = new TF1("iso_vs_compressednTT_endcaps_fit","[0]+[1]*x",0,31);

  /* Fitting process:
  Applies a fit to the histogram data using the specified function, in this case linear function */
  iso_vs_compressednTT_profile->Fit(iso_vs_compressednTT_fit, "Q");
  iso_vs_compressednTT_barrel_profile->Fit(iso_vs_compressednTT_barrel_fit, "Q");
  iso_vs_compressednTT_endcaps_profile->Fit(iso_vs_compressednTT_endcaps_fit, "Q");

  /*Now that the inclusive fits and histograms are done, we will proceed to define new histograms in
  bins of eta, et and nTT
  For this we will start by defining the variables that will be used
  The valu of these variables will be given by the output file we consider*/
  int   L1Tau_IEt      = -99;
  int   compressedE      = -99;
  int   compressednTT      = -99;
  int   L1Tau_IEta     = -99;
  int   L1Tau_compressed_IEta     = -99;
  int   L1Tau_hasEM    = -99;
  int   L1Tau_isMerged = -99;
  int   L1Tau_nTT      = -99;
  int   L1Tau_Iso      = -99;
  float OfflineTau_pt = -99.;
  int OfflineTau_isMatched = 0;
  int supercompressedE = -99;
  int supercompressednTT = -99;

  data.SetBranchAddress("L1Tau_IEt", &L1Tau_IEt);
  data.SetBranchAddress("compressedE", &compressedE);
  data.SetBranchAddress("compressednTT", &compressednTT);
  data.SetBranchAddress("supercompressedE", &supercompressedE);
  data.SetBranchAddress("supercompressednTT", &supercompressednTT);
  data.SetBranchAddress("L1Tau_IEta", &L1Tau_IEta);
  data.SetBranchAddress("compressedieta", &L1Tau_compressed_IEta);
  data.SetBranchAddress("L1Tau_isMerged", &L1Tau_isMerged);
  data.SetBranchAddress("L1Tau_hasEM", &L1Tau_hasEM);
  data.SetBranchAddress("L1Tau_nTT", &L1Tau_nTT);
  data.SetBranchAddress("L1Tau_Iso", &L1Tau_Iso);
  data.SetBranchAddress("OfflineTau_pt",&OfflineTau_pt);
  data.SetBranchAddress("OfflineTau_isMatched",&OfflineTau_isMatched);

  //creating dictionaries 
  std::map<TString,TH1F*> Histos_PerBin ; //we associate a string to a histogram 
  std::map<Int_t,TH3F*> IsoCut_PerBin ; //we associate an integer to a 3D histogram
  std::map<Int_t,std::map<TString,Int_t>> IsoCut_PerEfficiency_PerBin; //we associate an integer to another dictionarry in which a string is associated to and int

  //Defining the name of the histograms in bins of Eta, energy and ntt (number of trigger towers)
  for(UInt_t i = 0 ; i < NbinsIEta-1 ; ++i)
    {
      for(UInt_t j = 0 ; j < NbinsIEt-1 ; ++j)
        {
          for(UInt_t k = 0 ; k < NbinsnTT-1 ; ++k)
            {
              TString Name_Histo = "Hist_";

              stringstream ss_i;
              ss_i << i;
              TString Appendix_i = TString(ss_i.str());
              Name_Histo += Appendix_i;
              Name_Histo += "_";

              stringstream ss_j;
              ss_j << j;
              TString Appendix_j = TString(ss_j.str());
              Name_Histo += Appendix_j;
              Name_Histo += "_";

              stringstream ss_k;
              ss_k << k;
              TString Appendix_k = TString(ss_k.str());
              Name_Histo += Appendix_k;

              TH1F* temp_histo = new TH1F(Name_Histo.Data(),Name_Histo.Data(),100,0.,100.);
              //we include in the map Histos_PerBin the name of the histogram and the associated TH1
              Histos_PerBin.insert(make_pair(Name_Histo,temp_histo));
            }
        }
    }
  
  cout<<"entering loop"<<endl;

  //Defining profiling of the 1D histograms: L1Tau_IEt/L1Tau_IEta/L1Tau_nTT and L1Tau_Iso
  TProfile* hprof_IEt  = new TProfile("hprof_IEt","Profile L1_Iso vs. L1_IEt",100,0.,200.,0,20);
  TProfile* hprof_IEta  = new TProfile("hprof_IEta","Profile L1_Iso vs. L1_IEta",28,0.,28.,0,20);
  TProfile* hprof_nTT  = new TProfile("hprof_nTT","Profile L1_Iso vs. L1_nTT",150,0.,150.,0,20);//70,0.,70.,0,20

  /*Defining the names of all the separeted histograms, filling them in with the callibrated values for matched events
  We start by getting , the events and filling the profiled histograms
  We continue by defining the compressed or supercompressed binning for the histograms (super by default)
  We finish by filling the histos previuously defined in the dictionnary Histos_PerBin with values of
  the iso energy: therefore the output will be counts vs L1Tauiso E */
  for(UInt_t i = 0 ; i < data.GetEntries() ; ++i)
    {
      data.GetEntry(i);
      if(!OfflineTau_isMatched) continue;

      //Filling in the profiled histograms
      hprof_IEt->Fill(L1Tau_IEt,L1Tau_Iso,1);
      hprof_IEta->Fill(L1Tau_IEta,L1Tau_Iso,1);
      hprof_nTT->Fill(L1Tau_nTT,L1Tau_Iso,1);

      std::vector<Int_t> binForIsolation ;
      binForIsolation.clear();
      binForIsolation.push_back(L1Tau_compressed_IEta);
      if (compression == "compressed")
      {  
        binForIsolation.push_back(compressedE);
        binForIsolation.push_back(compressednTT);
      }
      if (compression == "supercompressed")
      {  
        binForIsolation.push_back(supercompressedE);
        binForIsolation.push_back(supercompressednTT);
      }
      
      //
      TString Name_Histo = "Hist_";

      stringstream ss_i;
      ss_i << binForIsolation.at(0);
      TString Appendix_i = TString(ss_i.str());
      Name_Histo += Appendix_i;
      Name_Histo += "_";
      
      stringstream ss_j;
      ss_j << binForIsolation.at(1);
      TString Appendix_j = TString(ss_j.str());
      Name_Histo += Appendix_j;
      Name_Histo += "_";
      
      stringstream ss_k;
      ss_k << binForIsolation.at(2);
      TString Appendix_k = TString(ss_k.str());
      Name_Histo += Appendix_k;

      Histos_PerBin[Name_Histo]->Fill(L1Tau_Iso);
    }
  

  
  // A new output file is created and all the inclusive histograms are stored

  // TFile f_out("ROOTs4LUTs/ROOTs4LUTs_2023/LUTisolation_Trigger_Stage2_Run3_MC_optimization"+version+"_calibThr"+intgr+"p"+decim+".root","RECREATE");
  TFile f_out(outFile,"RECREATE");
  // "ROOTs4LUTs_2024/LUTisolation_Run3Summer23_caloParams_2023_v0_4.root"

  isoEt_vs_nVtx->Write();
  isoEt_vs_nVtx_barrel->Write();
  isoEt_vs_nVtx_endcaps->Write();

  iso_vs_nvtx_profile->Write();
  iso_vs_nvtx_barrel_profile->Write();
  iso_vs_nvtx_endcaps_profile->Write();

  isoEt_vs_nTT->Write();
  isoEt_vs_nTT_barrel->Write();
  isoEt_vs_nTT_endcaps->Write();

  iso_vs_nTT_profile->Write();
  iso_vs_nTT_barrel_profile->Write();
  iso_vs_nTT_endcaps_profile->Write();

  iso_vs_compressednTT_profile->Write();
  iso_vs_compressednTT_barrel_profile->Write();
  iso_vs_compressednTT_endcaps_profile->Write();

  iso_vs_compressednTT_fit->Write();
  iso_vs_compressednTT_barrel_fit->Write();
  iso_vs_compressednTT_endcaps_fit->Write();

  hprof_IEt->Write();
  hprof_IEta->Write();
  hprof_nTT->Write();

  Int_t NumberOfHistosWithLowStats = 0;

  /*Here we define: IsoCut_PerEfficiency_PerBin
  For each value of the efficiency we have the name of the assiociated histogram and a value initialized for the iso cut
  for that efficiency in that specific histrogram and initialized to -1 for the moment*/

  for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff) //start loopiing over efficiency values
    {
      std::map<TString,Int_t> temp;

      for(UInt_t i = 0 ; i < NbinsIEta-1 ; ++i)
        {
          for(UInt_t j = 0 ; j < NbinsIEt-1 ; ++j)
            {
              for(UInt_t k = 0 ; k < NbinsnTT-1 ; ++k)
                {
                  TString Name_Histo = "Hist_";
                  
                  stringstream ss_i;
                  ss_i << i;
                  TString Appendix_i = TString(ss_i.str());
                  Name_Histo += Appendix_i;
                  Name_Histo += "_";
                  
                  stringstream ss_j;
                  ss_j << j;
                  TString Appendix_j = TString(ss_j.str());
                  Name_Histo += Appendix_j;
                  Name_Histo += "_";
                  
                  stringstream ss_k;
                  ss_k << k;
                  TString Appendix_k = TString(ss_k.str());
                  Name_Histo += Appendix_k;
                  
                  temp.insert(make_pair(Name_Histo,-1));
                }
            }
        }
      IsoCut_PerEfficiency_PerBin.insert(make_pair(iEff,temp));
      temp.clear();

    }
  
  /*We are defining the last map which to a value assocoaited a 3D histogram
  So for each value of efficiency we associate a 3D histogram*/
  for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
    {
      ostringstream convert; //convert the integer iEff into a string
      convert << iEff ; //convert << iEff;, it takes the value of iEff (which is likely an Int_t or an integer) and converts it into a string
      TString NameEff = "Eff_";
      TString TempStr(convert.str());
      NameEff += TempStr;
      TH3F* temp = new TH3F(NameEff.Data(),NameEff.Data(),NbinsIEta-1,0,NbinsIEta-1,NbinsIEt-1,0,NbinsIEt-1,NbinsnTT-1,0,NbinsnTT-1);
      IsoCut_PerBin.insert(make_pair(iEff,temp));
      /* NameEff.Data() retrieves the C-style string (const char*) representation of the TString object NameEff.
      It's necessary when passing the string to functions, such as TH3F, that expect const char*.
      We define the bin configuration for the x, y and z axis
      In NbinsIEta-1,0,NbinsIEta-1: the first value defines the number of bins and the other 2 are the range of the axis
      So a map entry becomes : IsoCut_PerBin[eff] = pointer_to_TH3F_named_Eff_eff */
    }

  for(UInt_t i = 0 ; i < NbinsIEta-1 ; ++i)
    {
      for(UInt_t j = 0 ; j < NbinsIEt-1 ; ++j)
        {
          for(UInt_t k = 0 ; k < NbinsnTT-1 ; ++k)
            {
              TString Name_Histo = "Hist_";

              stringstream ss_i;
              ss_i << i;
              TString Appendix_i = TString(ss_i.str());
              Name_Histo += Appendix_i;
              Name_Histo += "_";

              stringstream ss_j;
              ss_j << j;
              TString Appendix_j = TString(ss_j.str());
              Name_Histo += Appendix_j;
              Name_Histo += "_";

              stringstream ss_k;
              ss_k << k;
              TString Appendix_k = TString(ss_k.str());
              Name_Histo += Appendix_k;
              ////////////
              /*This next part of the code is the core of determining the isolation cut (iIso) 
              that corresponds to a given efficiency level (iEff) for the specific histo_i_j_k*/
              ////////////
              for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
                {
                  Float_t Efficiency = 0.01*iEff;

                  for(UInt_t iIso = 0 ; iIso < 100 ; ++iIso)
                    {
                      if(Histos_PerBin[Name_Histo]->Integral(1,iIso+1)/Histos_PerBin[Name_Histo]->Integral(1,100+1)>=Efficiency)
                      /*Represents the fraction of events passing the isolation cut up to iIso/ total number of events: this i sthe cumulative efficiency
                      The cumulative effciency is checked againts the target effciiency  */
                        {
                          if(IsoCut_PerEfficiency_PerBin[iEff][Name_Histo]==-1) //check to be sure there is no overwriting
                            {
                              //Maps are updated 
                              IsoCut_PerEfficiency_PerBin[iEff][Name_Histo]=iIso;
                              IsoCut_PerBin[iEff]->SetBinContent(i+1,j+1,k+1,iIso);
                            }
                        }
                    }
                }

              if(Histos_PerBin[Name_Histo]->GetEntries()<40)
                {
                  NumberOfHistosWithLowStats++;
                  cout<<"Histo "<<Name_Histo<<" has low stat.: "<<Histos_PerBin[Name_Histo]->GetEntries()<<endl;
                }

              Histos_PerBin[Name_Histo]->Write();
            }
        }

    }


  ///////////////////
  //FLAT EFFICIENCY PLOTS
  //////////////////


  //Efficiency as function of pT
  std::map<Int_t,TH1F*> pt_pass_efficiency ;
  std::map<Int_t,TH1F*> pt_pass_efficiency_TH3 ;
  //Defining the histograms
  for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
    {
      TString nameHisto = "pt_pass_efficiency_";
      ostringstream convert;
      convert << iEff;
      TString temp(convert.str());
      nameHisto += temp;

      TString nameHisto_TH3 = "pt_pass_efficiency_TH3_";
      nameHisto_TH3 += temp;
      
      TH1F* temp_histo = new TH1F(nameHisto.Data(),nameHisto.Data(),100,0,200);
      TH1F* temp_histo_TH3 = new TH1F(nameHisto_TH3.Data(),nameHisto_TH3.Data(),100,0,200);

      pt_pass_efficiency.insert(make_pair(iEff,temp_histo));
      pt_pass_efficiency_TH3.insert(make_pair(iEff,temp_histo_TH3));
    }

  //Efficiency as function of eta
  std::map<Int_t,TH1F*> eta_pass_efficiency ;
  //Defining the histograms
  for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
    {
      TString nameHisto = "eta_pass_efficiency_";
      ostringstream convert;
      convert << iEff;
      TString temp(convert.str());
      nameHisto += temp;
      
      TH1F* temp_histo = new TH1F(nameHisto.Data(),nameHisto.Data(),100,0,100);
      eta_pass_efficiency.insert(make_pair(iEff,temp_histo));
    }

  //Efficiency as function of nTT
  std::map<Int_t,TH1F*> nTT_pass_efficiency ;
  //Defining the histograms
  for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
    {
      TString nameHisto = "nTT_pass_efficiency_";
      ostringstream convert;
      convert << iEff;
      TString temp(convert.str());
      nameHisto += temp;
      
      TH1F* temp_histo = new TH1F(nameHisto.Data(),nameHisto.Data(),100,0,100);
      nTT_pass_efficiency.insert(make_pair(iEff,temp_histo));

    }

  //Defining pt, eta ,nTT histograms
  TH1F* pt = new TH1F("pt","pt",100,0,200);
  TH1F* eta = new TH1F("eta","eta",100,0,100);
  TH1F* nTT = new TH1F("nTT","nTT",100,0,100);

  /*For each event of the input file, we will go to a specific histogram in eta, E and nTT
  For that event we will see if it passes the isolation threashold found before for that specific histogram
  for the efficiency we want to obtain.*/
  for(UInt_t i = 0 ; i < data.GetEntries() ; ++i)
    {
      data.GetEntry(i);
      if(!OfflineTau_isMatched) continue;

      std::vector<Int_t> binForIsolation ;
      binForIsolation.clear();
      binForIsolation.push_back(L1Tau_compressed_IEta);
      if (compression == "compressed")
      {  
        binForIsolation.push_back(compressedE);
        binForIsolation.push_back(compressednTT);
      }
      if (compression == "supercompressed")
      {  
        binForIsolation.push_back(supercompressedE);
        binForIsolation.push_back(supercompressednTT);
      }

      TString Name_Histo = "Hist_";

      stringstream ss_i;
      ss_i << binForIsolation.at(0);
      TString Appendix_i = TString(ss_i.str());
      Name_Histo += Appendix_i;
      Name_Histo += "_";
      
      stringstream ss_j;
      ss_j << binForIsolation.at(1);
      TString Appendix_j = TString(ss_j.str());
      Name_Histo += Appendix_j;
      Name_Histo += "_";
      
      stringstream ss_k;
      ss_k << binForIsolation.at(2);
      TString Appendix_k = TString(ss_k.str());
      Name_Histo += Appendix_k;

      for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
        {    
          if(L1Tau_Iso<=IsoCut_PerEfficiency_PerBin[iEff][Name_Histo])
            {
              eta_pass_efficiency[iEff]->Fill(L1Tau_IEta);
              pt_pass_efficiency[iEff]->Fill(L1Tau_IEt);
              nTT_pass_efficiency[iEff]->Fill(L1Tau_nTT);
            }

          if(L1Tau_Iso<=IsoCut_PerBin[iEff]->GetBinContent(binForIsolation.at(0)+1,binForIsolation.at(1)+1,binForIsolation.at(2)+1)) pt_pass_efficiency_TH3[iEff]->Fill(L1Tau_IEt);
        }

      pt->Fill(L1Tau_IEt);
      eta->Fill(L1Tau_IEta);
      nTT->Fill(L1Tau_nTT);
      

    }

    /*Once this is done for all the events, the pt, eta, nTT histograms with all the events are also filled.
    As a next step, we will use TGraphAsymmErrors to divide the 2 histograms: pt_pass_efficiency/pt (same for eta and nTT)
    If the isolation threashold for each histogram has been stablished correctly the efficiency should be flat
    wrt to pt, eta and phi since the iso cut should only let pass the number of events up to the target efficiency.
    Indeed, the way this is built the efficiency should not depend on the kinematics of the event bu only on the isolation
    cut we establish. The latter does depend on the kinematics of the event (pt,eta,nTT)*/

  //Computation of the flat effciiency
  for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
    {
      TString nameHisto = "pt_pass_efficiency_";
      ostringstream convert;
      convert << iEff;
      TString temp(convert.str());
      nameHisto += temp;
      
      TGraphAsymmErrors* temp_histo = new TGraphAsymmErrors(pt_pass_efficiency[iEff],pt,"cp");
      TGraphAsymmErrors* temp_histo_TH3 = new TGraphAsymmErrors(pt_pass_efficiency_TH3[iEff],pt,"cp");
      temp_histo->Write();
      temp_histo_TH3->Write();
    }

  for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
    {
      TString nameHisto = "eta_pass_efficiency_";
      ostringstream convert;
      convert << iEff;
      TString temp(convert.str());
      nameHisto += temp;
      
      TGraphAsymmErrors* temp_histo = new TGraphAsymmErrors(eta_pass_efficiency[iEff],eta,"cp");
      temp_histo->Write();
    }

  for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
    {
      TString nameHisto = "nTT_pass_efficiency_";
      ostringstream convert;
      convert << iEff;
      TString temp(convert.str());
      nameHisto += temp;
      
      TGraphAsymmErrors* temp_histo = new TGraphAsymmErrors(nTT_pass_efficiency[iEff],nTT,"cp");
      temp_histo->Write();

    }

  ////////////
  //Iso E as function of nTT histogram and fit
  ////////////

  /*For eah value of the effciency we want to plot the value of the iso cut as a function of nTT.
  For that we will use the 3D histogram IsoCut_PerBin and project in in nTT for a specific efficiency.
  We select and effciiency and for each possible combination of eta and Et we plot IsoCut vs nTT,
  and then we loop over the other possible efficiencies. Finally we can perform a linear 
  fit on this histogram*/

  for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
    {
      IsoCut_PerBin[iEff]->Write();

      /*for each bin in eta/e project isoCut on nTT and make a fit of it
      this approach makes the isoCut strictly increasing and less dependent on statistic fluctuations*/
      for(UInt_t i = 0 ; i < NbinsIEta-1 ; ++i)
      {
        for(UInt_t j = 0 ; j < NbinsIEt-1 ; ++j)
          {
            TString projName = "pz_"+to_string(iEff)+"_eta"+to_string(i)+"_e"+to_string(j);
            TH1D* projection = IsoCut_PerBin[iEff]->ProjectionZ(projName, i+1, i+1, j+1, j+1, "e");
            projection->Write();

            TString fitName = "fit_pz_"+to_string(iEff)+"_eta"+to_string(i)+"_e"+to_string(j);
            TF1* projection_fit = new TF1(fitName,"[0]+[1]*x", FitMin, FitMax);
            projection_fit->SetParLimits(1, 0, 10000);
            projection->Fit(projection_fit, "QR");
            projection_fit->Write();
          }
      }
    }

  pt->Write();
  eta->Write();
  nTT->Write();

  cout<<"NumberOfHistosWithLowStats/Tot = "<<NumberOfHistosWithLowStats<<"/"<<NbinsIEta*NbinsIEt*NbinsnTT<<endl;
}
