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
#include "../Isolate/Fill_Isolation_TH3_MC_newnTT_gridSearch_nTTextrap.C"

using namespace std;

float figureOfMerit(TGraphAsymmErrors* turnon, float threshold) {
    int upperLim = 120; // uper limit of the integral of the curve
    int demarcation = threshold;

    Double_t x1, x2, y1, y2;
    float lowerIntegral = 0.0;
    float upperIntegral = 0.0;

    for (int i = 0; i < turnon->GetN(); ++i) {
        turnon->GetPoint(i, x1, y1);
        turnon->GetPoint(i+1, x2, y2);
        if (x2 > upperLim) break; // stop calculating the integrals at the upper limit

        // calculate binned rectangle+triangle approximation of inegral
        if (x1 < demarcation && x2 < demarcation) lowerIntegral += (x2-x1)*y1 + 0.5*(x2-x1)*(y2-y1);
        else if (x1 < demarcation && x2 > demarcation) {
            float y3 = (y2-y1)/(x2-x1) * demarcation + (x1*y2-x2*y1)/(x2-x1);
            lowerIntegral += (demarcation-x1)*y1 + 0.5*(demarcation-x1)*(y3-y1);
            upperIntegral += (x2-demarcation)*y3 + 0.5*(x2-demarcation)*(y2-y3);
        }
        else upperIntegral += (x2-x1)*y1 + 0.5*(x2-x1)*(y2-y1);
    }

    return upperIntegral - lowerIntegral; // - threshold;
}

bool has100Plateau(TGraphAsymmErrors* turnon) {
    Double_t x, y;
    float point95check = 75; // 75GeV
    float point98check = 95; // 95GeV
    bool pass95check = false;
    bool pass98check = false;
    bool has100Plateau = false;

    for (int i = 0; i < turnon->GetN(); ++i) {
        turnon->GetPoint(i, x, y);

        if (x >= point95check && x <= point98check && y >= 0.95) pass95check = true;
        if (x >= point98check) {
            if(y >= 0.98) pass98check = true; 
            break;
        }
    }

    if (pass95check && pass98check) has100Plateau = true;

    return has100Plateau;
    //return true;
}

bool has0effAt0pt(TGraphAsymmErrors* turnon) {
    bool has0effAt0pt = false;

    Double_t x, y;
    turnon->GetPoint(0, x, y);
    if (y<=0.05) has0effAt0pt = true;

    return has0effAt0pt;

}

void isBetterFM(TString FMtype, std::vector< std::pair<TString, std::pair<float,std::pair<float,float>>> > &fm_ordered, float newFM, float thr, float acc,  TString name) {
    for (long unsigned int i = 0; i < fm_ordered.size(); ++i)
    {
        if (FMtype == "FM") {
            if (newFM >= fm_ordered[i].second.second.first) {
                fm_ordered.insert( fm_ordered.begin()+i, std::pair(name, std::pair(thr,std::pair(newFM,acc))) );
                break;
            }
        }
        else if (FMtype == "FMtACC") {
            if (newFM*acc >= fm_ordered[i].second.second.first*fm_ordered[i].second.second.second) {
                fm_ordered.insert( fm_ordered.begin()+i, std::pair(name, std::pair(thr,std::pair(newFM,acc))) );
                break;
            }
        }        
    }
}

void isLowerRate(std::vector< std::pair<TString, float> > &orderedHz, float newHz, TString name) {
    for (long unsigned int i = 0; i < orderedHz.size(); ++i)
    {
        if (newHz >= orderedHz[i].second) { //} && newHz >= 13 && newHz <= 20) {
            orderedHz.insert( orderedHz.begin()+i, std::pair(name, newHz) );
            break;
        }
    }
}

void compare(TString date, TString version, TString tag, int run, TString FMtype, int targetRate = 14, int fixedThr = 0, float calibThr = 1.7)
{
    TString run_str = to_string(run);

    TString fixedRate = to_string(targetRate);
    TString fixedThreshold = to_string(fixedThr);

    TString intgr = to_string(calibThr).substr(0, to_string(calibThr).find("."));
    TString decim = to_string(calibThr).substr(2, to_string(calibThr).find("."));
    
    // target remap
    std::map<int, int> targetRemap;
    targetRemap.insert(make_pair(12,0));
    targetRemap.insert(make_pair(13,1));
    targetRemap.insert(make_pair(14,2));
    targetRemap.insert(make_pair(15,3));
    targetRemap.insert(make_pair(16,4));
    int targetIdx =  targetRemap[targetRate];

    // threshold remap
    std::map<int, int> thresholdRemap;
    thresholdRemap.insert(make_pair(30,0));
    thresholdRemap.insert(make_pair(31,1));
    thresholdRemap.insert(make_pair(32,2));
    thresholdRemap.insert(make_pair(33,3));
    thresholdRemap.insert(make_pair(34,4));
    thresholdRemap.insert(make_pair(35,5));
    thresholdRemap.insert(make_pair(36,6));
    int thresholdBin = thresholdRemap[fixedThr];

    int minThrAllowed = 0;
    int maxThrAllowed = 100;

    std::vector< std::pair<TString, std::pair<float,std::pair<float,float>>> > FM_ordered = { std::pair("init", std::pair(0,std::pair(0,0))) };
    std::vector< std::pair<TString, float> > orderedHzAtThr = { std::pair("init", 0) };

    TString FileName = "/data_CMS/cms/motta/Run3preparation/Run3preparation_2023/"+date+"_optimization"+version+"_calibThr"+intgr+"p"+decim+"/Tau_MC_TURNONS";
    if (fixedThr==0) FileName += "_FIXEDRATE"+fixedRate+"kHz";
    else             FileName += "_FIXEDTHR"+fixedThreshold+"GeV";
    FileName += "_Run"+run_str;
    FileName += "_"+version+"gs_"+tag+".root";
    TFile* f_Turnons = new TFile(FileName, "READ");

    TFile f_Thresholds("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/thresholds_fixedrate_ZeroBias_Run"+run_str+"_optimization"+version+"gs_calibThr"+intgr+"p"+decim+"_"+tag+".root","READ");
    TFile f_rate("/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/histos_rate_ZeroBias_Run"+run_str+"_optimization"+version+"gs_calibThr"+intgr+"p"+decim+"_"+tag+".root","READ");

    // START OF GRID SEARCH
    for (UInt_t iEff = 0; iEff < NEffsMin; ++iEff)
    {
        for (UInt_t iEmin = 0; iEmin < NEmins; ++iEmin)
        {
            for (UInt_t iEmax = 0; iEmax < NEmaxs_sum; ++iEmax)
            {
                Float_t effMin = EffsMin[iEff];
                TString effMin_intgr = to_string(effMin).substr(0, to_string(effMin).find("."));
                TString effMin_decim = to_string(effMin).substr(2, to_string(effMin).find("."));
                Int_t Emin = Emins[iEmin];
                Int_t Emax = Emins[iEmin] + Emaxs_sum[iEmax];
                TString TurnonName = "TurnOn_progression_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                std::cout << " Running " << TurnonName << std::endl;

                TGraphAsymmErrors* currentTurnon = (TGraphAsymmErrors*)f_Turnons->Get(TurnonName);

                Float_t Threshold;
                if (fixedThr != 0) { Threshold = fixedThr; }
                else
                {
                    TString ThresholdsName = "Thresholds_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                    TVectorD* ThresholdsVect  = (TVectorD*)f_Thresholds.Get(ThresholdsName);
                    Threshold = ThresholdsVect[0][targetIdx];
                }

                TString AcceptanceName = "Acceptance_progression_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                TVectorD* AcceptancesVect  = (TVectorD*)f_Turnons->Get(AcceptanceName);
                Float_t Acceptance = AcceptancesVect[0][0];

                float FMcurrentTurnon = figureOfMerit(currentTurnon, Threshold);

                bool Has0EffAt0Pt = has0effAt0pt(currentTurnon);
                bool Has100Plateau = has100Plateau(currentTurnon);

                if (Threshold>=minThrAllowed && Threshold<=maxThrAllowed && Has0EffAt0Pt && Has100Plateau)
                {
                    isBetterFM(FMtype, FM_ordered, FMcurrentTurnon, Threshold, Acceptance, TurnonName);

                    if (fixedThr != 0)
                    { 
                        Threshold = fixedThr;
                        TString FixedThresholdDiTauRateName = "FixedThresholdDiTauRates_effMin"+effMin_intgr+"p"+effMin_decim+"_eMin"+to_string(Emin)+"_eMax"+to_string(Emax);
                        TVectorD* FixedThresholdDiTauRate = (TVectorD*)f_rate.Get(FixedThresholdDiTauRateName);
                        Float_t currentRate = FixedThresholdDiTauRate[0][thresholdBin];

                        isLowerRate(orderedHzAtThr, currentRate, FixedThresholdDiTauRateName);
                    }
                }

            }
        }
    }

    FM_ordered.pop_back(); // remove init entry

    TString Filename = "FMs/FMs_2023/optimization"+version+"/"; 
    if (FMtype=="FM") Filename += "FM_orderd_turnons";
    else if (FMtype=="FMtACC") Filename += "FMtACC_orderd_turnons";
    if (targetRate!=0) Filename += "_FIXEDRATE"+fixedRate+"kHz";
    if (fixedThr!=0)   Filename += "_FIXEDTHR"+fixedThreshold+"kHz";
    Filename += "_Run"+run_str;
    Filename += "_"+version+"gs_"+tag+".txt";
    ofstream file;
    file.open(Filename, std::ofstream::trunc);
    file << "## HEARDER ##" << std::endl;
    for (long unsigned int i = 0; i < FM_ordered.size(); ++i)
    {
        file << FM_ordered[i].first << " : FM=" << FM_ordered[i].second.second.first << " - THR=" << FM_ordered[i].second.first << " - ACC=" << FM_ordered[i].second.second.second << " - FMtACC=" << FM_ordered[i].second.second.first*FM_ordered[i].second.second.second << std::endl;
    }
    file.close();

    orderedHzAtThr.pop_back(); // remove init entry

    TString FilenameRate = "FMs/FMs_2023/optimization"+version+"/BestRatesAt"+to_string(fixedThr)+"Thr_Run"+run_str+"_optimization"+version+".txt";
    ofstream fileRate;
    fileRate.open(FilenameRate, std::ofstream::trunc);
    fileRate << std::endl << std::endl;
    for (long unsigned int i = 0; i < orderedHzAtThr.size(); ++i)
    {
        fileRate << orderedHzAtThr[i].first << " : kHz=" << orderedHzAtThr[i].second << std::endl;
    }
    fileRate.close();

    return;
}
