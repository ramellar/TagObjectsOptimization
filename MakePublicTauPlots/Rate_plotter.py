from scipy.optimize import curve_fit
from scipy.signal import convolve
from optparse import OptionParser
import scipy.special as sp
import numpy as np
import ROOT
import sys
import os

from matplotlib.ticker import FixedLocator, FixedFormatter
import matplotlib.pyplot as plt
import matplotlib
import mplhep
plt.style.use(mplhep.style.CMS)


#######################################################################
######################### SCRIPT BODY #################################
#######################################################################

if __name__ == "__main__" :
    parser = OptionParser()
    parser.add_option("--inFile",      dest="inFile",                                     default=None)
    parser.add_option("--inFile2",     dest="inFile2",                                    default=None)
    parser.add_option("--tag",       dest="tag",                            default=None)
    (options, args) = parser.parse_args()
    print(options)

    #inFile = ROOT.TFile('/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/'+options.inFile)
    #inFile2 = ROOT.TFile('/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/'+options.inFile2)

    #inFile10 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/histos_rate_ZeroBias_Run366727_reEmulated_Tau2023v1.root')
    inFile2 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/histos_rate_ZeroBias_Run367079_zeroBias_unpacked.root')
    #inFile3 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/histos_rate_ZeroBias_Run366801_reEmulated_Tau2023v1.root')
    #inFile4 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/histos_rate_ZeroBias_Run366820_reEmulated_Tau2023v1.root')
    #inFile5 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/histos_rate_ZeroBias_Run366821_reEmulated_Tau2023v1.root')
    #inFile6 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/histos_rate_ZeroBias_Run366833_reEmulated_Tau2023v1.root')
    #inFile7 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/histos_rate_ZeroBias_Run366874_reEmulated_Tau2023v1.root')
    #inFile8 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/histos_rate_ZeroBias_Run366895_reEmulated_Tau2023v1.root')
    #inFile9 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/histos_rate_ZeroBias_Run367079_reEmulated_Tau2023v1.root')
    inFile = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/histos_rate_ZeroBias_Run367079_unpacked.root')
    
    plt.rcParams['legend.title_fontsize'] = 'x-small'
    cmap = matplotlib.cm.get_cmap('Set1'); imap=-1
    markers = ['o', 's', '^', 'D']

    #DoubleTau = inFile.Get('DiTauRate_noIso')
    #DoubleTau_Iso_10 = inFile10.Get('DiTauRate_Iso')
    DoubleTau_Iso_2 = inFile2.Get('DiTauRate_Iso')
    #DoubleTau_Iso_3 = inFile3.Get('DiTauRate_Iso')
    #DoubleTau_Iso_4 = inFile4.Get('DiTauRate_Iso')
    #DoubleTau_Iso_5 = inFile5.Get('DiTauRate_Iso')
    #DoubleTau_Iso_6 = inFile6.Get('DiTauRate_Iso')
    #DoubleTau_Iso_7 = inFile7.Get('DiTauRate_Iso')
    #DoubleTau_Iso_8 = inFile8.Get('DiTauRate_Iso')
    #DoubleTau_Iso_9 = inFile9.Get('DiTauRate_Iso')
    DoubleTau_Iso = inFile.Get('DiTauRate_Iso')

    legend_title=r'Double-$\tau$ & Isolation rate comparison'
    #legend_title=r'Inst. Lumi = $3.5\times10^{33}\ cm^{-2}s^{-1}$'
    #label_DoubleIso_Iso_10 = r'Double-$\tau$ & Iso Run 366727 398b'
    label_DoubleIso_Iso_2 = r'Double-$\tau$ & Iso Run 367079 ZeroBias'
    #label_DoubleIso_Iso_3 = r'Double-$\tau$ & Iso Run 366801 398b'
    #label_DoubleIso_Iso_4 = r'Double-$\tau$ & Iso Run 366820 398b'
    #label_DoubleIso_Iso_5 = r'Double-$\tau$ & Iso Run 366821 398b'
    #label_DoubleIso_Iso_6 = r'Double-$\tau$ & Iso Run 366833 386b'
    #label_DoubleIso_Iso_7 = r'Double-$\tau$ & Iso Run 366874 898b'
    #label_DoubleIso_Iso_8 = r'Double-$\tau$ & Iso Run 366895 986b'
    #label_DoubleIso_Iso_9 = r'Double-$\tau$ & Iso Run 367079 986b'
    label_DoubleIso_Iso = r'Double-$\tau$ & Iso Run 367079 Ephemeral'

    fig, ax = plt.subplots(figsize=(10,10))
    
    #for rate_TH1, label in zip([DoubleTau_Iso, DoubleTau_Iso_2, DoubleTau_Iso_5, DoubleTau_Iso_6, DoubleTau_Iso_8, DoubleTau_Iso_9, DoubleTau_Iso_10], [label_DoubleIso_Iso, label_DoubleIso_Iso_2, label_DoubleIso_Iso_5, label_DoubleIso_Iso_6, label_DoubleIso_Iso_8, label_DoubleIso_Iso_9, label_DoubleIso_Iso_10]):
    for rate_TH1, label in zip([DoubleTau_Iso, DoubleTau_Iso_2], [label_DoubleIso_Iso, label_DoubleIso_Iso_2]):
        x = []
        y = []
        x_err = []
        y_err = []
        for ibin in range(rate_TH1.GetNbinsX()):
            x.append(rate_TH1.GetBinLowEdge(ibin+1) + rate_TH1.GetBinWidth(ibin+1)/2.)
            y.append(rate_TH1.GetBinContent(ibin+1))
            x_err.append(rate_TH1.GetBinWidth(ibin+1)/2.)
            y_err.append(rate_TH1.GetBinError(ibin+1))

        print(len(y))
        if imap == -1: color = 'black'
        else:         color = cmap(imap)

        ax.errorbar(x, y, xerr=x_err, yerr=y_err, ls='None', label=label, lw=2) #, marker=markers[imap], color=color)
        imap += 1

    leg = plt.legend(loc='upper right', fontsize=16, title=legend_title)
    leg._legend_box.align = "left"
    plt.ylim(1,4E4)
    plt.xlim(0,60)
    plt.yscale('log')
    ax.yaxis.set_major_locator(FixedLocator([1, 10, 100, 1000, 10000]))
    ax.yaxis.set_major_formatter(FixedFormatter([r'1',r'10',r'$10^2$',r'$10^3$',r'$10^4$']))
    plt.xlabel(r'$E_{T}^{\tau, L1}\ [GeV]$')
    plt.ylabel(r'Rate[kHz]')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    #mplhep.cms.label('Preliminary', data=True, rlabel=r'107 pb$^{-1}$ (13.6 TeV)')
    mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
    plt.savefig('various/tau_rate_'+options.tag+'.pdf')
    plt.savefig('various/tau_rate_'+options.tag+'.png')
    plt.close()
