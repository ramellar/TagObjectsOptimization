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

    inFile = ROOT.TFile('/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/'+options.inFile)
    inFile2 = ROOT.TFile('/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/MakeRates/histos_2023/'+options.inFile2)

    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1'); imap=-1
    markers = ['o', 's', '^', 'D']

    DoubleTau = inFile.Get('DiTauRate_noIso')
    DoubleTau_Iso = inFile.Get('DiTauRate_Iso')
    DoubleTau_Iso_2 = inFile2.Get('DiTauRate_Iso')

    legend_title=r'Inst. Lumi = $2.5\times10^{34}\ cm^{-2}s^{-1}$'
    label_DoubleIso = r'Double-$\tau$ - 2022 Unpacked'
    label_DoubleIso_Iso = r'Double-$\tau$ & Isolation - 2022 Unpacked'
    label_DoubleIso_Iso_2 = r'Double-$\tau$ & Isolation - 2023 Re-Emulated'

    fig, ax = plt.subplots(figsize=(10,10))
    
    for rate_TH1, label in zip([DoubleTau, DoubleTau_Iso, DoubleTau_Iso_2], [label_DoubleIso, label_DoubleIso_Iso, label_DoubleIso_Iso_2]):
        x = []
        y = []
        x_err = []
        y_err = []
        for ibin in range(rate_TH1.GetNbinsX()):
            x.append(rate_TH1.GetBinLowEdge(ibin+1) + rate_TH1.GetBinWidth(ibin+1)/2.)
            y.append(rate_TH1.GetBinContent(ibin+1))
            x_err.append(rate_TH1.GetBinWidth(ibin+1)/2.)
            y_err.append(rate_TH1.GetBinError(ibin+1))

        if imap == -1: color = 'black'
        else:         color = cmap(imap)

        ax.errorbar(x, y, xerr=x_err, yerr=y_err, ls='None', label=label, lw=2, marker=markers[imap], color=color)
        imap += 1

    leg = plt.legend(loc='upper right', fontsize=18, title=legend_title)
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
    mplhep.cms.label('Preliminary', data=True, rlabel=r'107 pb$^{-1}$ (13.6 TeV)')
    plt.savefig('various/tau_rate_'+options.tag+'.pdf')
    plt.savefig('various/tau_rate_'+options.tag+'.png')
    plt.close()
