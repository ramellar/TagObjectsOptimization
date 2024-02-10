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

if __name__ == "__main__" :
    parser = OptionParser()
    parser.add_option("--inFile",    dest="inFile",                         default=None)
    parser.add_option("--inFile2",   dest="inFile2",                        default=None)
    parser.add_option("--inFile3",   dest="inFile3",                        default=None)
    parser.add_option("--tag",       dest="tag",                            default=None)
    parser.add_option("--thr",       dest="thr",                            default=None)
    (options, args) = parser.parse_args()
    print(options)

    inFile = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/PlotTurnOns/ROOTs/ROOTs_2023/'+options.inFile)
    inFile2 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/PlotTurnOns/ROOTs/ROOTs_2023/'+options.inFile2)
    inFile3 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/PlotTurnOns/ROOTs/ROOTs_2023/'+options.inFile3)
    
    thr_str = options.thr
    thr = int(thr_str)

    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1')

    ## VERSUS NUMBER OF VERTICES
    eff_Iso_TGraph = inFile.Get('divide_nvtxProgressionAt'+thr_str+'_noIso_by_nvtx')
    eff_Iso_TGraph_2 = inFile2.Get('divide_nvtxProgressionAt'+thr_str+'_noIso_by_nvtx')
    eff_Iso_TGraph_3 = inFile3.Get('divide_nvtxProgressionAt'+thr_str+'_noIso_by_nvtx')

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
    x_err_Iso_2 = []
    y_errU_Iso_2 = []
    y_errD_Iso_2 = []
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

        x_Iso_2.append(eff_Iso_TGraph_3.GetPointX(ibin))
        y_Iso_2.append(eff_Iso_TGraph_3.GetPointY(ibin))
        x_err_Iso_2.append(eff_Iso_TGraph_3.GetErrorX(ibin))
        
        errU_2 = eff_Iso_TGraph_3.GetErrorYhigh(ibin)
        errD_2 = eff_Iso_TGraph_3.GetErrorYlow(ibin)
        y_errU_Iso_2.append(errU_2)
        y_errD_Iso_2.append(errD_2)


    fig, ax = plt.subplots(figsize=(10,10))
    ax.errorbar(x, y, xerr=x_err, yerr=[y_errD, y_errU], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV - 2018 Unpacked' % (thr), lw=2, marker='o', color='black')
    ax.errorbar(x_Iso, y_Iso, xerr=x_err_Iso, yerr=[y_errD_Iso, y_errU_Iso], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV - 2022 Unpacked' % (thr), lw=2, marker='s', color=cmap(0))
    ax.errorbar(x_Iso_2, y_Iso_2, xerr=x_err_Iso_2, yerr=[y_errD_Iso_2, y_errU_Iso_2], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV - 2023 Unpacked' % (thr), lw=2, marker='s', color=cmap(1))
    leg = plt.legend(loc = 'lower left', fontsize=20, title=r'$p_{T}^{\tau, offline}>40\ GeV$ & $|\eta^{\tau, offline}|<2.1$')
    leg._legend_box.align = "left"
    plt.ylim(0,1.05)
    plt.xlim(10,60)
    # plt.yscale('log')
    plt.xlabel('Number of vertices')
    plt.ylabel(r'Efficiency')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'3.2 fb$^{-1}$ (13.6 TeV)')
    plt.savefig('various/tau_eff_vs_nvtx_'+options.tag+'.pdf')
    plt.savefig('various/tau_eff_vs_nvtx_'+options.tag+'.png')
    plt.close()












    ## VERSUS ETA
    eff_Iso_TGraph = inFile.Get('divide_etaprogressionAt'+thr_str+'_noIso_by_eta')
    eff_Iso_TGraph_2 = inFile2.Get('divide_etaprogressionAt'+thr_str+'_noIso_by_eta')
    eff_Iso_TGraph_3 = inFile3.Get('divide_etaprogressionAt'+thr_str+'_noIso_by_eta')

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
    x_err_Iso_2 = []
    y_errU_Iso_2 = []
    y_errD_Iso_2 = []
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

        x_Iso_2.append(eff_Iso_TGraph_3.GetPointX(ibin))
        y_Iso_2.append(eff_Iso_TGraph_3.GetPointY(ibin))
        x_err_Iso_2.append(eff_Iso_TGraph_3.GetErrorX(ibin))
        
        errU_2 = eff_Iso_TGraph_3.GetErrorYhigh(ibin)
        errD_2 = eff_Iso_TGraph_3.GetErrorYlow(ibin)
        y_errU_Iso_2.append(errU_2)
        y_errD_Iso_2.append(errD_2)


    fig, ax = plt.subplots(figsize=(10,10))
    ax.errorbar(x, y, xerr=x_err, yerr=[y_errD, y_errU], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV - 2018 Unpacked' % (thr), lw=2, marker='o', color='black')
    ax.errorbar(x_Iso, y_Iso, xerr=x_err_Iso, yerr=[y_errD_Iso, y_errU_Iso], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV - 2022 Unpacked' % (thr), lw=2, marker='s', color=cmap(0))
    ax.errorbar(x_Iso_2, y_Iso_2, xerr=x_err_Iso_2, yerr=[y_errD_Iso_2, y_errU_Iso_2], ls='None', label=r'$E_{T}^{\tau, L1} > %i$ GeV - 2023 Unpacked' % (thr), lw=2, marker='s', color=cmap(1))
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
    mplhep.cms.label('Preliminary', data=True, rlabel=r'3.2 fb$^{-1}$ (13.6 TeV)')
    plt.savefig('various/tau_eff_vs_eta_'+options.tag+'.pdf')
    plt.savefig('various/tau_eff_vs_eta_'+options.tag+'.png')
    plt.close()

