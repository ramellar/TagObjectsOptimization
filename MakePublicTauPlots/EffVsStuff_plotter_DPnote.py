from scipy.optimize import curve_fit
from scipy.signal import convolve
from optparse import OptionParser
import scipy.special as sp
import numpy as np
import ROOT
import sys
import os

import matplotlib.pyplot as plt
import matplotlib
import mplhep
plt.style.use(mplhep.style.CMS)


#######################################################################
######################### SCRIPT BODY #################################
#######################################################################

''' python3 EffVsStuff_plotter_DPnote.py --inFile efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_reEmulated.root --tag New_2025_LUTfor_data_2024I --thr 33'''
''' python3 EffVsStuff_plotter_DPnote.py --inFile1 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_reEmulated.root --inFile2 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_unpacked.root --tag New_2025_LUTfor_data_2024I --thr 33'''

if __name__ == "__main__" :
    parser = OptionParser()
    parser.add_option("--inFile1",    dest="inFile1",                         default=None)
    parser.add_option("--inFile2",    dest="inFile2",                         default=None)
    parser.add_option("--tag",       dest="tag",                            default=None)
    parser.add_option("--thr",       dest="thr",                            default=None)
    (options, args) = parser.parse_args()
    print(options)

    inFile1 = ROOT.TFile('/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotTurnOns/ROOTs/ROOTs_2024/'+options.inFile1)
    inFile2 = ROOT.TFile('/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotTurnOns/ROOTs/ROOTs_2025/'+options.inFile2)
    
    thr_str = options.thr
    thr = int(thr_str)

    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1')

    ## VERSUS NUMBER OF VERTICES
    eff_noIso_TGraph_1 = inFile1.Get('divide_nvtxProgressionAt'+thr_str+'_noIso_by_nvtx')
    eff_noIso_TGraph_2 = inFile2.Get('divide_nvtxProgressionAt'+thr_str+'_noIso_by_nvtx')
    eff_Iso_TGraph_1 = inFile1.Get('divide_nvtxProgressionAt'+thr_str+'_Iso_by_nvtx')
    eff_Iso_TGraph_2 = inFile2.Get('divide_nvtxProgressionAt'+thr_str+'_Iso_by_nvtx')

    # CONVERT TO LISTS FOR PYPLOT
  
    # x_2 = []
    # y_2 = []
    # x_err_2 = []
    # y_errU_2 = []
    # y_errD_2 = []

    def compute_eff_vs_ntvx(filename):

        x = []
        y = []
        x_err = []
        y_errU = []
        y_errD = []

        for ibin in range(filename.GetN()):
            x.append(filename.GetPointX(ibin))
            y.append(filename.GetPointY(ibin))
            x_err.append(filename.GetErrorX(ibin))
            
            errU = filename.GetErrorYhigh(ibin)
            errD = filename.GetErrorYlow(ibin)
            y_errU.append(errU)
            y_errD.append(errD)
        return x,y, x_err, y_errD, y_errU
    
        # label1= r"2024 Era I ReEmu"+ "\n" + "w/ 2025 conditions"
    # label2= r"Unpacked 2024 Era I"


    def plot_eff_vs_ntvx(filename,filename2, iso=""):

        x, y, x_err, y_errD, y_errU = compute_eff_vs_ntvx(filename)
        x2, y2, x_err2, y_errD2, y_errU2 = compute_eff_vs_ntvx(filename2)
        
        
        if iso=="iso":
            fig, ax = plt.subplots(figsize=(10,10))
            ax.errorbar(x, y, xerr=x_err, yerr=[y_errD, y_errU], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV& Isolation'% (thr) + "\n" + r"2024 Era I ReEmu"+ "\n" + "w/ 2025 conditions", lw=2, marker='o', color=cmap(0))
            ax.errorbar(x2, y2, xerr=x_err2, yerr=[y_errD2, y_errU2], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV & Isolation'% (thr) + "\n" + r"Unpacked 2024 Era I", lw=2, marker='s', color=cmap(1))
            leg = plt.legend(loc = 'lower left', fontsize=20, title=r'$p_{T}^{\tau, offline}>40\ GeV$ & $|\eta^{\tau, offline}|<2.1$')
            leg._legend_box.align = "left"
            plt.ylim(0,1.05)
            plt.xlim(20,60)
            # plt.yscale('log')
            plt.xlabel('Number of vertices')
            plt.ylabel(r'Efficiency')
            plt.grid()
            for xtick in ax.xaxis.get_major_ticks():
                xtick.set_pad(10)
            mplhep.cms.label('Preliminary', data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')
            plt.savefig('various/tau_eff_vs_nvtx_'+options.tag+'_Iso.pdf')
            plt.savefig('various/tau_eff_vs_nvtx_'+options.tag+'_Iso.png')
            print('various/tau_eff_vs_nvtx_'+options.tag+'_Iso.png')
            plt.close()
        else:
            fig, ax = plt.subplots(figsize=(10,10))
            ax.errorbar(x, y, xerr=x_err, yerr=[y_errD, y_errU], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV' % (thr) + "\n" + r"2024 Era I ReEmu"+ "\n" + "w/ 2025 conditions", lw=2, marker='o', color=cmap(0))
            ax.errorbar(x2, y2, xerr=x_err2, yerr=[y_errD2, y_errU2], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV' % (thr) + "\n" + r"Unpacked 2024 Era I", lw=2, marker='s', color=cmap(1))
            leg = plt.legend(loc = 'lower left', fontsize=20, title=r'$p_{T}^{\tau, offline}>40\ GeV$ & $|\eta^{\tau, offline}|<2.1$')
            leg._legend_box.align = "left"
            plt.ylim(0,1.05)
            plt.xlim(20,60)
            # plt.yscale('log')
            plt.xlabel('Number of vertices')
            plt.ylabel(r'Efficiency')
            plt.grid()
            for xtick in ax.xaxis.get_major_ticks():
                xtick.set_pad(10)
            mplhep.cms.label('Preliminary', data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')
            plt.savefig('various/tau_eff_vs_nvtx_'+options.tag+'_noIso.pdf')
            plt.savefig('various/tau_eff_vs_nvtx_'+options.tag+'_noIso.png')
            print('various/tau_eff_vs_nvtx_'+options.tag+'_noIso.png')
            plt.close()



    plot_eff_vs_ntvx(eff_noIso_TGraph_1,eff_noIso_TGraph_2)
    plot_eff_vs_ntvx(eff_Iso_TGraph_1,eff_Iso_TGraph_2,"iso")


    ## VERSUS ETA
    eff_Iso_TGraph = inFile1.Get('divide_etaprogressionAt'+thr_str+'_noIso_by_eta')
    eff_Iso_TGraph_2 = inFile1.Get('divide_etaprogressionAt'+thr_str+'_Iso_by_eta')

    # CONVERT TO LISTS FOR PYPLOT
    x = []
    y = []
    x_err = []
    y_errU = []
    y_errD = []
    x_Iso = []
    y_Iso = []
    x_err_Iso = []
    y_errU_Iso = []
    y_errD_Iso = []
    x_Iso_2 = []
    y_Iso_2 = []
    for ibin in range(eff_Iso_TGraph.GetN()):
        x.append(eff_Iso_TGraph.GetPointX(ibin))
        y.append(eff_Iso_TGraph.GetPointY(ibin))
        x_err.append(eff_Iso_TGraph.GetErrorX(ibin))
        
        errU = eff_Iso_TGraph.GetErrorYhigh(ibin)
        errD = eff_Iso_TGraph.GetErrorYlow(ibin)
        y_errU.append(errU)
        y_errD.append(errD)

        x_Iso.append(eff_Iso_TGraph_2.GetPointX(ibin))
        y_Iso.append(eff_Iso_TGraph_2.GetPointY(ibin))
        x_err_Iso.append(eff_Iso_TGraph_2.GetErrorX(ibin))
        
        errU = eff_Iso_TGraph_2.GetErrorYhigh(ibin)
        errD = eff_Iso_TGraph_2.GetErrorYlow(ibin)
        y_errU_Iso.append(errU)
        y_errD_Iso.append(errD)


    fig, ax = plt.subplots(figsize=(10,10))
    ax.errorbar(x, y, xerr=x_err, yerr=[y_errD, y_errU], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV' % (thr), lw=2, marker='o', color=cmap(0))
    ax.errorbar(x_Iso, y_Iso, xerr=x_err_Iso, yerr=[y_errD_Iso, y_errU_Iso], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV & Isolation' % (thr), lw=2, marker='s', color=cmap(1))
    leg = plt.legend(loc = 'lower left', fontsize=20, title=r'$p_{T}^{\tau, offline}>40\ GeV$ & $|\eta^{\tau, offline}|<2.1$')
    leg._legend_box.align = "left"
    plt.ylim(0,1.05)
    plt.xlim(-2.1,2.1)
    # plt.yscale('log')
    plt.xlabel(r'$|\eta^{\tau, offline}|$')
    plt.ylabel(r'Efficiency')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'18 fb$^{-1}$ (13.6 TeV)')
    plt.savefig('various/tau_eff_vs_eta_'+options.tag+'.pdf')
    plt.savefig('various/tau_eff_vs_eta_'+options.tag+'.png')
    plt.close()

