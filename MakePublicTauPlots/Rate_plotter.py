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


if __name__ == "__main__" :
    ''' python3 Rate_plotter.py --inFile1 histos_rate_ZeroBias_Run369978_reEmulated_Tau2023_v4.root --inFile2 histos_rate_ZeroBias_Run369978_reEmulated_Tau2023_v4_HCAL_corr.root --tag 2023LUTs_2024conditions_HCAL_new_calib '''

    parser = OptionParser()
    parser.add_option("--inFile1", dest="inFile1", default=None)
    parser.add_option("--inFile2", dest="inFile2", default=None)
    parser.add_option("--tag",     dest="tag",     default=None)
    (options, args) = parser.parse_args()
    print(options)

    main_folder = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_11_0_2/src/HiggsAnalysis/TagObjectsOptimization/MakeRates/histos_2024/'
    inFile1 = ROOT.TFile(main_folder + options.inFile1)
    inFile2 = ROOT.TFile(main_folder + options.inFile2)

    plt.rcParams['legend.title_fontsize'] = 'x-small'
    cmap = matplotlib.cm.get_cmap('Set1'); imap=-1
    markers = ['o', 's', '^', 'D']

    #DoubleTau = inFile.Get('DiTauRate_noIso')
    DoubleTau_Iso_1 = inFile1.Get('DiTauRate')
    DoubleTau_Iso_2 = inFile2.Get('DiTauRate_Iso')


    legend_title=r'Double-$\tau$ & Isolation rate comparison'
    #legend_title=r'Inst. Lumi = $3.5\times10^{33}\ cm^{-2}s^{-1}$'
    label_DoubleIso_Iso_1 = r'Double-$\tau$ & Iso Run 369978 re-Emu w/o HCAL'
    label_DoubleIso_Iso_2 = r'Double-$\tau$ & Iso Run 369978 re-Emu w/ HCAL'

    fig, ax = plt.subplots(figsize=(10,10))
    
    for rate_TH1, label in zip([DoubleTau_Iso_1, DoubleTau_Iso_2], [label_DoubleIso_Iso_1, label_DoubleIso_Iso_2]):
        x = []
        y = []
        x_err = []
        y_err = []
        print(rate_TH1.ClassName())
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
