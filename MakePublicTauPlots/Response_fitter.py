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


def CB(x, mean=1, sigma=1, alpha=1, n=1, norm=1):
    t = (x - mean)/sigma

    # Crystal Ball definitions
    A = pow(n / abs(alpha), n) * np.exp(-0.5 * alpha * alpha)
    B = n / abs(alpha) - abs(alpha)

    if t > -alpha:
        crystalBall = norm * np.exp(-0.5 * pow(t,2))
    else:
        crystalBall = norm * A * pow(B-t,-n)

    return crystalBall

vectCB = np.vectorize(CB)
##              [ mean, sigma, alpha,    n, norm]
# p0 =          [   1.,    1.,    1.,   1.,  0.1]
# param_bounds=([ -10.,  -10.,  -10., -200.,  0. ],
#               [  10.,   10.,   10.,  200.,  1. ])

def DoubleCB(x, mean=1, sigma=1, alphaL=1, nL=1, alphaR=1, nR=1, norm=1):
    t = (x - mean)/sigma

    # Crystal Ball definitions
    Al = pow(nL / abs(alphaL), nL) * np.exp(-0.5 * alphaL * alphaL)
    Bl = nL / abs(alphaL) - abs(alphaL)
    Ar = pow(nR / abs(alphaR), nR) * np.exp(-0.5 * alphaR * alphaR)
    Br = nR / abs(alphaR) - abs(alphaR)

    if t < -alphaL:
        crystalBall = norm * Al * pow(Bl-t,-nL)
    elif t < alphaR:
        crystalBall = norm * np.exp(-0.5 * pow(t,2))
    else:
        crystalBall = norm * Ar * pow(Br+t,-nR)
        
        
    return crystalBall

vectDoubleCB = np.vectorize(DoubleCB)
##              [ mean, sigma, alphaL,   nL, alphaR,   nR, norm]
# p0 =          [   1.,    1.,     1.,   1.,     1.,   1.,  0.1]
# param_bounds=([ -10.,  -10.,   -10., -10.,   -10., -10.,  0. ],
#               [  10.,   10.,    10.,  10.,    10.,  10.,  1. ])

def compute_scale(inFile, inclusive=True):
    scale_barrel      = inFile.Get('pt_barrel_resp_ptInclusive')
    scale_endcap      = inFile.Get('pt_endcap_resp_ptInclusive')
    scale_inclusive   = inFile.Get('pt_response_ptInclusive')

    # CONVERT TO LISTS FOR PYPLOT
    x_scale_barrel = []
    y_scale_barrel = []
    x_err_scale_barrel = []
    y_err_scale_barrel = []
    x_scale_endcap = []
    y_scale_endcap = []
    x_err_scale_endcap = []
    y_err_scale_endcap = []
    x_scale_inclusive = []
    y_scale_inclusive = []
    x_err_scale_inclusive = []
    y_err_scale_inclusive = []
    for ibin in range(scale_barrel.GetNbinsX()):
        x_scale_barrel.append(scale_barrel.GetBinLowEdge(ibin+1) + scale_barrel.GetBinWidth(ibin+1)/2.)
        y_scale_barrel.append(scale_barrel.GetBinContent(ibin+1))
        x_err_scale_barrel.append(scale_barrel.GetBinWidth(ibin+1)/2.)
        y_err_scale_barrel.append(scale_barrel.GetBinError(ibin+1))

        x_scale_endcap.append(scale_endcap.GetBinLowEdge(ibin+1) + scale_endcap.GetBinWidth(ibin+1)/2.)
        y_scale_endcap.append(scale_endcap.GetBinContent(ibin+1))
        x_err_scale_endcap.append(scale_endcap.GetBinWidth(ibin+1)/2.)
        y_err_scale_endcap.append(scale_endcap.GetBinError(ibin+1))

        x_scale_inclusive.append(scale_inclusive.GetBinLowEdge(ibin+1) + scale_inclusive.GetBinWidth(ibin+1)/2.)
        y_scale_inclusive.append(scale_inclusive.GetBinContent(ibin+1))
        x_err_scale_inclusive.append(scale_inclusive.GetBinWidth(ibin+1)/2.)
        y_err_scale_inclusive.append(scale_inclusive.GetBinError(ibin+1))

    return x_scale_inclusive, y_scale_inclusive, x_err_scale_inclusive, y_err_scale_inclusive

def compute_resol(inFile, inclusive=True):
    ptResol_barrel    = inFile.Get('pt_resol_barrel_fctPt')
    ptResol_endcap    = inFile.Get('pt_resol_endcap_fctPt')
    ptResol_inclusive = inFile.Get('pt_resol_fctPt')

    # CONVERT TO LISTS FOR PYPLOT
    x_ptResol_barrel = []
    y_ptResol_barrel = []
    x_err_ptResol_barrel = []
    y_err_ptResol_barrel = []
    x_ptResol_endcap = []
    y_ptResol_endcap = []
    x_err_ptResol_endcap = []
    y_err_ptResol_endcap = []
    x_ptResol_inclusive = []
    y_ptResol_inclusive = []
    x_err_ptResol_inclusive = []
    y_err_ptResol_inclusive = []
    for ibin in range(ptResol_barrel.GetNbinsX()):
        x_ptResol_barrel.append(ptResol_barrel.GetBinLowEdge(ibin+1) + ptResol_barrel.GetBinWidth(ibin+1)/2.)
        y_ptResol_barrel.append(ptResol_barrel.GetBinContent(ibin+1))
        x_err_ptResol_barrel.append(ptResol_barrel.GetBinWidth(ibin+1)/2.)
        y_err_ptResol_barrel.append(ptResol_barrel.GetBinError(ibin+1))

        x_ptResol_endcap.append(ptResol_endcap.GetBinLowEdge(ibin+1) + ptResol_endcap.GetBinWidth(ibin+1)/2.)
        y_ptResol_endcap.append(ptResol_endcap.GetBinContent(ibin+1))
        x_err_ptResol_endcap.append(ptResol_endcap.GetBinWidth(ibin+1)/2.)
        y_err_ptResol_endcap.append(ptResol_endcap.GetBinError(ibin+1))

        x_ptResol_inclusive.append(ptResol_inclusive.GetBinLowEdge(ibin+1) + ptResol_inclusive.GetBinWidth(ibin+1)/2.)
        y_ptResol_inclusive.append(ptResol_inclusive.GetBinContent(ibin+1))
        x_err_ptResol_inclusive.append(ptResol_inclusive.GetBinWidth(ibin+1)/2.)
        y_err_ptResol_inclusive.append(ptResol_inclusive.GetBinError(ibin+1))

    return x_ptResol_inclusive, y_ptResol_inclusive, x_err_ptResol_inclusive, y_err_ptResol_inclusive


def plot_pt_scale(inFile, label, color, ax):
    x_lim = (0.,2.)
    x_label = r'$E_{T}^{\tau, L1}/p_{T}^{\tau, offline}$'
    barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
    endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
    inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'    
    plt.rcParams['legend.title_fontsize'] = 'xx-small'
    cmap = matplotlib.cm.get_cmap('Set1')
    plot_x = np.linspace(-3,3,4000)

    x_scale_inclusive, y_scale_inclusive, x_err_scale_inclusive, y_err_scale_inclusive = compute_scale(inFile)

    ax.errorbar(x_scale_inclusive, y_scale_inclusive, xerr=x_err_scale_inclusive, yerr=y_err_scale_inclusive, 
                ls='None', label=inclusive_label+label, lw=2, marker='s', color=cmap(color))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   1.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_scale_inclusive[2:], y_scale_inclusive[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(color))
    
    leg = plt.legend(loc = 'upper left', fontsize=18)
    leg._legend_box.align = "left"
    plt.ylim(0.,0.167)
    plt.xlim(x_lim)
    plt.xlabel(x_label)
    plt.ylabel(r'a.u.')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')


def plot_pt_resolution(inFile, label, color, ax):
    x_lim = (20.,110.)
    x_label = r'$p_{T}^{\tau, offline}\ [GeV]$'
    barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
    endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
    inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
    cmap = matplotlib.cm.get_cmap('Set1')

    x_ptResol_inclusive, y_ptResol_inclusive, x_err_ptResol_inclusive, y_err_ptResol_inclusive = compute_resol(inFile)

    ax.errorbar(x_ptResol_inclusive, y_ptResol_inclusive, xerr=x_err_ptResol_inclusive, yerr=y_err_ptResol_inclusive, 
                ls='None', label=inclusive_label+label, lw=2, marker='o', color=cmap(color))

    leg = plt.legend(loc = 'upper right', fontsize=18)
    leg._legend_box.align = "left"
    plt.ylim(0.05,0.30)
    plt.xlim(x_lim)
    plt.xlabel(x_label)
    plt.ylabel(r'Energy resolution')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
 

#######################################################################
######################### SCRIPT BODY #################################
#######################################################################

''' python3 Response_fitter.py --inFile1 resolutions_of_Run2023_all_eras_unpacked.root --inFile2 resolutions_of_Run2024W-MC_caloParams_2023_v0_4_cfi_reEmulated.root --tag 2024W-MC_caloParams_2023_v0_4_cfi '''

if __name__ == "__main__" :
    parser = OptionParser()
    parser = OptionParser()
    parser.add_option("--inFile1",   dest="inFile1",                        default=None)
    parser.add_option("--inFile2",   dest="inFile2",                        default=None)
    parser.add_option("--tag",       dest="tag",                            default=None)
    parser.add_option("--inclusive", dest="inclusive", action='store_true', default=False)
    (options, args) = parser.parse_args()
    print(options)

    main_folder = '/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2023/'
    inFile1 = ROOT.TFile(main_folder+options.inFile1)
    inFile2 = ROOT.TFile(main_folder+options.inFile2)

    label1 = r' - 2023 unpacked'
    label2 = r' - 2024 MC re-Emulated'

    # PLOT PT SCALE
    fig, ax = plt.subplots(figsize=(10,10))
    plot_pt_scale(inFile1, label1, 0, ax)
    plot_pt_scale(inFile2, label2, 1, ax)

    plot_name = 'responses/2024/tau_pt_scale_'+options.tag
    if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()

    # PLOT PT RESOLUTION
    fig, ax = plt.subplots(figsize=(10,10))
    plot_pt_resolution(inFile1, label1, 0, ax)
    plot_pt_resolution(inFile2, label2, 1, ax)
    
    plot_name = 'responses/2024/tau_pt_resolution_'+options.tag
    if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()
 
