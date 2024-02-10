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

def compute_eta(inFile, inclusive=True):
    etaResp_barrel    = inFile.Get('eta_resp_barrel')
    etaResp_endcap    = inFile.Get('eta_resp_endcap')
    etaResp_inclusive = inFile.Get('eta_resp_inclusive')

    # CONVERT TO LISTS FOR PYPLOT
    x_etaResp_barrel = []
    y_etaResp_barrel = []
    x_err_etaResp_barrel = []
    y_err_etaResp_barrel = []
    x_etaResp_endcap = []
    y_etaResp_endcap = []
    x_err_etaResp_endcap = []
    y_err_etaResp_endcap = []
    x_etaResp_inclusive = []
    y_etaResp_inclusive = []
    x_err_etaResp_inclusive = []
    y_err_etaResp_inclusive = []
    for ibin in range(etaResp_barrel.GetNbinsX()):
        x_etaResp_barrel.append(etaResp_barrel.GetBinLowEdge(ibin+1) + etaResp_barrel.GetBinWidth(ibin+1)/2.)
        y_etaResp_barrel.append(etaResp_barrel.GetBinContent(ibin+1))
        x_err_etaResp_barrel.append(etaResp_barrel.GetBinWidth(ibin+1)/2.)
        y_err_etaResp_barrel.append(etaResp_barrel.GetBinError(ibin+1))

        x_etaResp_endcap.append(etaResp_endcap.GetBinLowEdge(ibin+1) + etaResp_endcap.GetBinWidth(ibin+1)/2.)
        y_etaResp_endcap.append(etaResp_endcap.GetBinContent(ibin+1))
        x_err_etaResp_endcap.append(etaResp_endcap.GetBinWidth(ibin+1)/2.)
        y_err_etaResp_endcap.append(etaResp_endcap.GetBinError(ibin+1))

        x_etaResp_inclusive.append(etaResp_inclusive.GetBinLowEdge(ibin+1) + etaResp_inclusive.GetBinWidth(ibin+1)/2.)
        y_etaResp_inclusive.append(etaResp_inclusive.GetBinContent(ibin+1))
        x_err_etaResp_inclusive.append(etaResp_inclusive.GetBinWidth(ibin+1)/2.)
        y_err_etaResp_inclusive.append(etaResp_inclusive.GetBinError(ibin+1))

    return x_etaResp_inclusive, y_etaResp_inclusive, x_err_etaResp_inclusive, y_err_etaResp_inclusive

def compute_phi(inFile, inclusive=True):
    phiResp_barrel    = inFile.Get('phi_resp_barrel')
    phiResp_endcap    = inFile.Get('phi_resp_endcap')
    phiResp_inclusive = inFile.Get('phi_resp_inclusive')

    # CONVERT TO LISTS FOR PYPLOT
    x_phiResp_barrel = []
    y_phiResp_barrel = []
    x_err_phiResp_barrel = []
    y_err_phiResp_barrel = []
    x_phiResp_endcap = []
    y_phiResp_endcap = []
    x_err_phiResp_endcap = []
    y_err_phiResp_endcap = []
    x_phiResp_inclusive = []
    y_phiResp_inclusive = []
    x_err_phiResp_inclusive = []
    y_err_phiResp_inclusive = []
    for ibin in range(phiResp_barrel.GetNbinsX()):
        x_phiResp_barrel.append(phiResp_barrel.GetBinLowEdge(ibin+1) + phiResp_barrel.GetBinWidth(ibin+1)/2.)
        y_phiResp_barrel.append(phiResp_barrel.GetBinContent(ibin+1))
        x_err_phiResp_barrel.append(phiResp_barrel.GetBinWidth(ibin+1)/2.)
        y_err_phiResp_barrel.append(phiResp_barrel.GetBinError(ibin+1))

        x_phiResp_endcap.append(phiResp_endcap.GetBinLowEdge(ibin+1) + phiResp_endcap.GetBinWidth(ibin+1)/2.)
        y_phiResp_endcap.append(phiResp_endcap.GetBinContent(ibin+1))
        x_err_phiResp_endcap.append(phiResp_endcap.GetBinWidth(ibin+1)/2.)
        y_err_phiResp_endcap.append(phiResp_endcap.GetBinError(ibin+1))

        x_phiResp_inclusive.append(phiResp_inclusive.GetBinLowEdge(ibin+1) + phiResp_inclusive.GetBinWidth(ibin+1)/2.)
        y_phiResp_inclusive.append(phiResp_inclusive.GetBinContent(ibin+1))
        x_err_phiResp_inclusive.append(phiResp_inclusive.GetBinWidth(ibin+1)/2.)
        y_err_phiResp_inclusive.append(phiResp_inclusive.GetBinError(ibin+1))

    return x_phiResp_inclusive, y_phiResp_inclusive, x_err_phiResp_inclusive, y_err_phiResp_inclusive


def plot_eta(inFile, label, color, ax):
    x_lim = (-0.15,0.15)
    x_label = r'$\eta^{\tau, L1} - \eta^{\tau, offline}$'
    barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
    endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
    inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
    plt.rcParams['legend.title_fontsize'] = 'xx-small'
    cmap = matplotlib.cm.get_cmap('Set1')
    plot_x = np.linspace(-3,3,4000)

    x_etaResp_inclusive, y_etaResp_inclusive, x_err_etaResp_inclusive, y_err_etaResp_inclusive = compute_eta(inFile)

    ax.errorbar(x_etaResp_inclusive, y_etaResp_inclusive, xerr=x_err_etaResp_inclusive, yerr=y_err_etaResp_inclusive, 
                ls='None', label=inclusive_label+label, lw=2, marker='s', color=cmap(color))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   0.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_etaResp_inclusive[2:], y_etaResp_inclusive[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(color))

    leg = plt.legend(loc = 'upper left', fontsize=18)
    leg._legend_box.align = "left"
    plt.ylim(0., max(max(y_etaResp_inclusive), max(y_etaResp_inclusive)) * 1.4)
    plt.xlim(x_lim)
    plt.xlabel(x_label)
    plt.ylabel(r'a.u.')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')


def plot_phi(inFile, label, color, ax):
    x_lim = (-0.25,0.25)
    x_label = r'$\phi^{\tau, L1} - \phi^{\tau, offline}$'
    barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
    endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
    inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
    cmap = matplotlib.cm.get_cmap('Set1')
    plot_x = np.linspace(-3,3,4000)

    x_phiResp_inclusive, y_phiResp_inclusive, x_err_phiResp_inclusive, y_err_phiResp_inclusive = compute_phi(inFile)

    ax.errorbar(x_phiResp_inclusive, y_phiResp_inclusive, xerr=x_err_phiResp_inclusive, yerr=y_err_phiResp_inclusive, 
                ls='None', label=inclusive_label+label, lw=2, marker='s', color=cmap(color))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   0.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_phiResp_inclusive[2:], y_phiResp_inclusive[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(color))

    leg = plt.legend(loc = 'upper right', fontsize=18)
    leg._legend_box.align = "left"
    plt.ylim(min(y_phiResp_inclusive[9], y_phiResp_inclusive[9]) * 0.6, 
             max(max(y_phiResp_inclusive), max(y_phiResp_inclusive)) * 1.4)
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

''' python3 Response_fitter_eta_phi.py --inFile1 resolutions_of_Run2023_all_eras_unpacked.root --inFile2 resolutions_of_Run2024W-MC_caloParams_2023_v0_4_cfi_reEmulated.root --tag 2024W-MC_caloParams_2023_v0_4_cfi '''

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

    # PLOT ETA RESPONSE
    fig, ax = plt.subplots(figsize=(10,10))
    plot_eta(inFile1, label1, 0, ax)
    plot_eta(inFile2, label2, 1, ax)

    plot_name = 'responses/2024/tau_eta_'+options.tag
    if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()

    # PLOT PHI RESPONSE
    fig, ax = plt.subplots(figsize=(10,10))
    plot_phi(inFile1, label1, 0, ax)
    plot_phi(inFile2, label2, 1, ax)
    
    plot_name = 'responses/2024/tau_phi_'+options.tag
    if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()
 
