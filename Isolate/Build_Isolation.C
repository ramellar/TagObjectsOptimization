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

void Build_Isolation(TString inputFile, TString outFile, UInt_t tmpFitMin = 3, UInt_t tmpFitMax = 14, TString compression = "supercompressed", float calibThr = 1.7)
{
  TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
  TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));

  UInt_t tmpIEt;
  UInt_t tmpnTT;
  // UInt_t tmpFitMin;
  // UInt_t tmpFitMax;
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
  const UInt_t NbinsIEt = tmpIEt;
  const UInt_t NbinsnTT = tmpnTT;
  const UInt_t FitMin = tmpFitMin;
  const UInt_t FitMax = tmpFitMax;

  TChain data("outTreeCalibrated");
  data.Add(inputFile);
  // "/data_CMS/cms/mchiusi/Run3preparation/Run3_2024/2023S-MC_caloParams_2023_v0_4/Run3Summer23_caloParams_2023_v0_4_CALIBRATED.root"


  TH2F* isoEt_vs_nVtx = new TH2F("isoEt_vs_nVtx","isoEt_vs_nVtx",150,0.,150.,100,0.,100.);
  TH2F* isoEt_vs_nVtx_barrel = new TH2F("isoEt_vs_nVtx_barrel","isoEt_vs_nVtx_barrel",150,0.,150.,100,0.,100.);
  TH2F* isoEt_vs_nVtx_endcaps = new TH2F("isoEt_vs_nVtx_endcaps","isoEt_vs_nVtx_endcaps",150,0.,150.,100,0.,100.);

  TH2F* isoEt_vs_nTT = new TH2F("isoEt_vs_nTT","isoEt_vs_nTT",150,0.,150.,100,0.,100.);
  TH2F* isoEt_vs_nTT_barrel = new TH2F("isoEt_vs_nTT_barrel","isoEt_vs_nTT_barrel",150,0.,150.,100,0.,100.);
  TH2F* isoEt_vs_nTT_endcaps = new TH2F("isoEt_vs_nTT_endcaps","isoEt_vs_nTT_endcaps",150,0.,150.,100,0.,100.);

  TH2F* isoEt_vs_compressednTT = new TH2F("isoEt_vs_compressednTT","isoEt_vs_compressednTT",31,0.,31.,100,0.,100.);
  TH2F* isoEt_vs_compressednTT_barrel = new TH2F("isoEt_vs_compressednTT_barrel","isoEt_vs_compressednTT_barrel",31,0.,31.,100,0.,100.);
  TH2F* isoEt_vs_compressednTT_endcaps = new TH2F("isoEt_vs_compressednTT_endcaps","isoEt_vs_compressednTT_endcaps",31,0.,31.,100,0.,100.);

  data.Draw("L1Tau_Iso:Nvtx>>isoEt_vs_nVtx","","colz");
  data.Draw("L1Tau_Iso:Nvtx>>isoEt_vs_nVtx_barrel","abs(OfflineTau_eta)<1.5","colz");
  data.Draw("L1Tau_Iso:Nvtx>>isoEt_vs_nVtx_endcaps","abs(OfflineTau_eta)>1.5","colz");

  data.Draw("L1Tau_Iso:L1Tau_nTT>>isoEt_vs_nTT","","colz");
  data.Draw("L1Tau_Iso:L1Tau_nTT>>isoEt_vs_nTT_barrel","abs(OfflineTau_eta)<1.5","colz");
  data.Draw("L1Tau_Iso:L1Tau_nTT>>isoEt_vs_nTT_endcaps","abs(OfflineTau_eta)>1.5","colz");

  data.Draw("L1Tau_Iso:compressednTT>>isoEt_vs_compressednTT","","colz");
  data.Draw("L1Tau_Iso:compressednTT>>isoEt_vs_compressednTT_barrel","abs(OfflineTau_eta)<1.5","colz");
  data.Draw("L1Tau_Iso:compressednTT>>isoEt_vs_compressednTT_endcaps","abs(OfflineTau_eta)>1.5","colz");

  TProfile* iso_vs_nvtx_profile = isoEt_vs_nVtx->ProfileX("iso_vs_nvtx_profile");
  TProfile* iso_vs_nvtx_barrel_profile = isoEt_vs_nVtx_barrel->ProfileX("iso_vs_nvtx_barrel_profile");
  TProfile* iso_vs_nvtx_endcaps_profile = isoEt_vs_nVtx_endcaps->ProfileX("iso_vs_nvtx_endcaps_profile");

  TProfile* iso_vs_nTT_profile = isoEt_vs_nTT->ProfileX("iso_vs_nTT_profile");
  TProfile* iso_vs_nTT_barrel_profile = isoEt_vs_nTT_barrel->ProfileX("iso_vs_nTT_barrel_profile");
  TProfile* iso_vs_nTT_endcaps_profile = isoEt_vs_nTT_endcaps->ProfileX("iso_vs_nTT_endcaps_profile");

  TProfile* iso_vs_compressednTT_profile = isoEt_vs_compressednTT->ProfileX("iso_vs_compressednTT_profile");
  TProfile* iso_vs_compressednTT_barrel_profile = isoEt_vs_compressednTT_barrel->ProfileX("iso_vs_compressednTT_barrel_profile");
  TProfile* iso_vs_compressednTT_endcaps_profile = isoEt_vs_compressednTT_endcaps->ProfileX("iso_vs_compressednTT_endcaps_profile");

  TF1* iso_vs_compressednTT_fit = new TF1("iso_vs_compressednTT_fit","[0]+[1]*x",0,31);
  TF1* iso_vs_compressednTT_barrel_fit = new TF1("iso_vs_compressednTT_barrel_fit","[0]+[1]*x",0,31);
  TF1* iso_vs_compressednTT_endcaps_fit = new TF1("iso_vs_compressednTT_endcaps_fit","[0]+[1]*x",0,31);

  iso_vs_compressednTT_profile->Fit(iso_vs_compressednTT_fit, "Q");
  iso_vs_compressednTT_barrel_profile->Fit(iso_vs_compressednTT_barrel_fit, "Q");
  iso_vs_compressednTT_endcaps_profile->Fit(iso_vs_compressednTT_endcaps_fit, "Q");

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

  std::map<TString,TH1F*> Histos_PerBin ;
  std::map<Int_t,TH3F*> IsoCut_PerBin ;
  std::map<Int_t,std::map<TString,Int_t>> IsoCut_PerEfficiency_PerBin;

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
              Histos_PerBin.insert(make_pair(Name_Histo,temp_histo));
            }
        }
    }
  
  cout<<"entering loop"<<endl;

  TProfile* hprof_IEt  = new TProfile("hprof_IEt","Profile L1_Iso vs. L1_IEt",100,0.,200.,0,20);
  TProfile* hprof_IEta  = new TProfile("hprof_IEta","Profile L1_Iso vs. L1_IEta",28,0.,28.,0,20);
  TProfile* hprof_nTT  = new TProfile("hprof_nTT","Profile L1_Iso vs. L1_nTT",150,0.,150.,0,20);//70,0.,70.,0,20

  for(UInt_t i = 0 ; i < data.GetEntries() ; ++i)
    {
      data.GetEntry(i);
      if(!OfflineTau_isMatched) continue;

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

  for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
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

  for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
    {
      ostringstream convert;
      convert << iEff ;
      TString NameEff = "Eff_";
      TString TempStr(convert.str());
      NameEff += TempStr;
      TH3F* temp = new TH3F(NameEff.Data(),NameEff.Data(),NbinsIEta-1,0,NbinsIEta-1,NbinsIEt-1,0,NbinsIEt-1,NbinsnTT-1,0,NbinsnTT-1);
      IsoCut_PerBin.insert(make_pair(iEff,temp));
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


              for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
                {
                  Float_t Efficiency = 0.01*iEff;

                  for(UInt_t iIso = 0 ; iIso < 100 ; ++iIso)
                    {
                      if(Histos_PerBin[Name_Histo]->Integral(1,iIso+1)/Histos_PerBin[Name_Histo]->Integral(1,100+1)>=Efficiency)
                        {
                          if(IsoCut_PerEfficiency_PerBin[iEff][Name_Histo]==-1)
                            {
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

  //Efficiency as function of pT
  std::map<Int_t,TH1F*> pt_pass_efficiency ;
  std::map<Int_t,TH1F*> pt_pass_efficiency_TH3 ;
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

  std::map<Int_t,TH1F*> eta_pass_efficiency ;
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

  std::map<Int_t,TH1F*> nTT_pass_efficiency ;
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

  TH1F* pt = new TH1F("pt","pt",100,0,200);
  TH1F* eta = new TH1F("eta","eta",100,0,100);
  TH1F* nTT = new TH1F("nTT","nTT",100,0,100);

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

  for(UInt_t iEff = 0 ; iEff < 101 ; ++iEff)
    {
      IsoCut_PerBin[iEff]->Write();

      // for each bin in eta/e project isoCut on nTT and make a fit of it
      // this approach makes the isoCut strictly increasing and less dependent on statistic fluctuations
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
