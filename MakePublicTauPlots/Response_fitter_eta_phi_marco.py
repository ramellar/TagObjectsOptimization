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



#######################################################################
######################### SCRIPT BODY #################################
#######################################################################

if __name__ == "__main__" :
    parser = OptionParser()
    parser = OptionParser()
    parser.add_option("--inFile",    dest="inFile",                         default=None)
    parser.add_option("--inFile2",   dest="inFile2",                        default=None)
    parser.add_option("--inFile3",   dest="inFile3",                        default=None)
    parser.add_option("--tag",       dest="tag",                            default=None)
    parser.add_option("--inclusive", dest="inclusive", action='store_true', default=False)
    (options, args) = parser.parse_args()
    print(options)

    inFile = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2023/'+options.inFile)

    scale_barrel      = inFile.Get('pt_barrel_resp_ptInclusive')
    scale_endcap      = inFile.Get('pt_endcap_resp_ptInclusive')
    scale_inclusive   = inFile.Get('pt_response_ptInclusive')
    ptResol_barrel    = inFile.Get('pt_resol_barrel_fctPt')
    ptResol_endcap    = inFile.Get('pt_resol_endcap_fctPt')
    ptResol_inclusive = inFile.Get('pt_resol_fctPt')
    etaResp_barrel    = inFile.Get('eta_resp_barrel')
    etaResp_endcap    = inFile.Get('eta_resp_endcap')
    etaResp_inclusive = inFile.Get('eta_resp_inclusive')
    phiResp_barrel    = inFile.Get('phi_resp_barrel')
    phiResp_endcap    = inFile.Get('phi_resp_endcap')
    phiResp_inclusive = inFile.Get('phi_resp_inclusive')

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


    inFile2 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2023/'+options.inFile2)

    etaResp_barrel_2      = inFile2.Get('pt_barrel_resp_ptInclusive')
    etaResp_endcap_2      = inFile2.Get('pt_endcap_resp_ptInclusive')
    etaResp_inclusive_2   = inFile2.Get('pt_response_ptInclusive')
    ptResol_barrel_2    = inFile2.Get('pt_resol_barrel_fctPt')
    ptResol_endcap_2    = inFile2.Get('pt_resol_endcap_fctPt')
    ptResol_inclusive_2 = inFile2.Get('pt_resol_fctPt')
    etaResp_barrel_2    = inFile2.Get('eta_resp_barrel')
    etaResp_endcap_2    = inFile2.Get('eta_resp_endcap')
    etaResp_inclusive_2 = inFile2.Get('eta_resp_inclusive')
    phiResp_barrel_2    = inFile2.Get('phi_resp_barrel')
    phiResp_endcap_2    = inFile2.Get('phi_resp_endcap')
    phiResp_inclusive_2 = inFile2.Get('phi_resp_inclusive')

    # CONVERT TO LISTS FOR PYPLOT
    x_etaResp_barrel_2 = []
    y_etaResp_barrel_2 = []
    x_err_etaResp_barrel_2 = []
    y_err_etaResp_barrel_2 = []
    x_etaResp_endcap_2 = []
    y_etaResp_endcap_2 = []
    x_err_etaResp_endcap_2 = []
    y_err_etaResp_endcap_2 = []
    x_etaResp_inclusive_2 = []
    y_etaResp_inclusive_2 = []
    x_err_etaResp_inclusive_2 = []
    y_err_etaResp_inclusive_2 = []
    for ibin in range(etaResp_barrel.GetNbinsX()):
        x_etaResp_barrel_2.append(etaResp_barrel_2.GetBinLowEdge(ibin+1) + etaResp_barrel_2.GetBinWidth(ibin+1)/2.)
        y_etaResp_barrel_2.append(etaResp_barrel_2.GetBinContent(ibin+1))
        x_err_etaResp_barrel_2.append(etaResp_barrel_2.GetBinWidth(ibin+1)/2.)
        y_err_etaResp_barrel_2.append(etaResp_barrel_2.GetBinError(ibin+1))

        x_etaResp_endcap_2.append(etaResp_endcap_2.GetBinLowEdge(ibin+1) + etaResp_endcap_2.GetBinWidth(ibin+1)/2.)
        y_etaResp_endcap_2.append(etaResp_endcap_2.GetBinContent(ibin+1))
        x_err_etaResp_endcap_2.append(etaResp_endcap_2.GetBinWidth(ibin+1)/2.)
        y_err_etaResp_endcap_2.append(etaResp_endcap_2.GetBinError(ibin+1))

        x_etaResp_inclusive_2.append(etaResp_inclusive_2.GetBinLowEdge(ibin+1) + etaResp_inclusive_2.GetBinWidth(ibin+1)/2.)
        y_etaResp_inclusive_2.append(etaResp_inclusive_2.GetBinContent(ibin+1))
        x_err_etaResp_inclusive_2.append(etaResp_inclusive_2.GetBinWidth(ibin+1)/2.)
        y_err_etaResp_inclusive_2.append(etaResp_inclusive_2.GetBinError(ibin+1))

    # CONVERT TO LISTS FOR PYPLOT
    x_phiResp_barrel_2 = []
    y_phiResp_barrel_2 = []
    x_err_phiResp_barrel_2 = []
    y_err_phiResp_barrel_2 = []
    x_phiResp_endcap_2 = []
    y_phiResp_endcap_2 = []
    x_err_phiResp_endcap_2 = []
    y_err_phiResp_endcap_2 = []
    x_phiResp_inclusive_2 = []
    y_phiResp_inclusive_2 = []
    x_err_phiResp_inclusive_2 = []
    y_err_phiResp_inclusive_2 = []
    for ibin in range(phiResp_barrel.GetNbinsX()):
        x_phiResp_barrel_2.append(phiResp_barrel_2.GetBinLowEdge(ibin+1) + phiResp_barrel_2.GetBinWidth(ibin+1)/2.)
        y_phiResp_barrel_2.append(phiResp_barrel_2.GetBinContent(ibin+1))
        x_err_phiResp_barrel_2.append(phiResp_barrel_2.GetBinWidth(ibin+1)/2.)
        y_err_phiResp_barrel_2.append(phiResp_barrel_2.GetBinError(ibin+1))

        x_phiResp_endcap_2.append(phiResp_endcap_2.GetBinLowEdge(ibin+1) + phiResp_endcap_2.GetBinWidth(ibin+1)/2.)
        y_phiResp_endcap_2.append(phiResp_endcap_2.GetBinContent(ibin+1))
        x_err_phiResp_endcap_2.append(phiResp_endcap_2.GetBinWidth(ibin+1)/2.)
        y_err_phiResp_endcap_2.append(phiResp_endcap_2.GetBinError(ibin+1))

        x_phiResp_inclusive_2.append(phiResp_inclusive_2.GetBinLowEdge(ibin+1) + phiResp_inclusive_2.GetBinWidth(ibin+1)/2.)
        y_phiResp_inclusive_2.append(phiResp_inclusive_2.GetBinContent(ibin+1))
        x_err_phiResp_inclusive_2.append(phiResp_inclusive_2.GetBinWidth(ibin+1)/2.)
        y_err_phiResp_inclusive_2.append(phiResp_inclusive_2.GetBinError(ibin+1))


    inFile3 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2023/'+options.inFile3)

    etaResp_barrel_3      = inFile3.Get('pt_barrel_resp_ptInclusive')
    etaResp_endcap_3      = inFile3.Get('pt_endcap_resp_ptInclusive')
    etaResp_inclusive_3   = inFile3.Get('pt_response_ptInclusive')
    phiResp_barrel_3    = inFile3.Get('pt_resol_barrel_fctPt')
    phiResp_endcap_3    = inFile3.Get('pt_resol_endcap_fctPt')
    phiResp_inclusive_3 = inFile3.Get('pt_resol_fctPt')
    etaResp_barrel_3    = inFile3.Get('eta_resp_barrel')
    etaResp_endcap_3    = inFile3.Get('eta_resp_endcap')
    etaResp_inclusive_3 = inFile3.Get('eta_resp_inclusive')
    phiResp_barrel_3    = inFile3.Get('phi_resp_barrel')
    phiResp_endcap_3    = inFile3.Get('phi_resp_endcap')
    phiResp_inclusive_3 = inFile3.Get('phi_resp_inclusive')

    # CONVERT TO LISTS FOR PYPLOT
    x_etaResp_barrel_3 = []
    y_etaResp_barrel_3 = []
    x_err_etaResp_barrel_3 = []
    y_err_etaResp_barrel_3 = []
    x_etaResp_endcap_3 = []
    y_etaResp_endcap_3 = []
    x_err_etaResp_endcap_3 = []
    y_err_etaResp_endcap_3 = []
    x_etaResp_inclusive_3 = []
    y_etaResp_inclusive_3 = []
    x_err_etaResp_inclusive_3 = []
    y_err_etaResp_inclusive_3 = []
    for ibin in range(etaResp_barrel.GetNbinsX()):
        x_etaResp_barrel_3.append(etaResp_barrel_3.GetBinLowEdge(ibin+1) + etaResp_barrel_3.GetBinWidth(ibin+1)/2.)
        y_etaResp_barrel_3.append(etaResp_barrel_3.GetBinContent(ibin+1))
        x_err_etaResp_barrel_3.append(etaResp_barrel_3.GetBinWidth(ibin+1)/2.)
        y_err_etaResp_barrel_3.append(etaResp_barrel_3.GetBinError(ibin+1))

        x_etaResp_endcap_3.append(etaResp_endcap_3.GetBinLowEdge(ibin+1) + etaResp_endcap_3.GetBinWidth(ibin+1)/2.)
        y_etaResp_endcap_3.append(etaResp_endcap_3.GetBinContent(ibin+1))
        x_err_etaResp_endcap_3.append(etaResp_endcap_3.GetBinWidth(ibin+1)/2.)
        y_err_etaResp_endcap_3.append(etaResp_endcap_3.GetBinError(ibin+1))

        x_etaResp_inclusive_3.append(etaResp_inclusive_3.GetBinLowEdge(ibin+1) + etaResp_inclusive_3.GetBinWidth(ibin+1)/2.)
        y_etaResp_inclusive_3.append(etaResp_inclusive_3.GetBinContent(ibin+1))
        x_err_etaResp_inclusive_3.append(etaResp_inclusive_3.GetBinWidth(ibin+1)/2.)
        y_err_etaResp_inclusive_3.append(etaResp_inclusive_3.GetBinError(ibin+1))

    # CONVERT TO LISTS FOR PYPLOT
    x_phiResp_barrel_3 = []
    y_phiResp_barrel_3 = []
    x_err_phiResp_barrel_3 = []
    y_err_phiResp_barrel_3 = []
    x_phiResp_endcap_3 = []
    y_phiResp_endcap_3 = []
    x_err_phiResp_endcap_3 = []
    y_err_phiResp_endcap_3 = []
    x_phiResp_inclusive_3 = []
    y_phiResp_inclusive_3 = []
    x_err_phiResp_inclusive_3 = []
    y_err_phiResp_inclusive_3 = []
    for ibin in range(phiResp_barrel.GetNbinsX()):
        x_phiResp_barrel_3.append(phiResp_barrel_3.GetBinLowEdge(ibin+1) + phiResp_barrel_3.GetBinWidth(ibin+1)/2.)
        y_phiResp_barrel_3.append(phiResp_barrel_3.GetBinContent(ibin+1))
        x_err_phiResp_barrel_3.append(phiResp_barrel_3.GetBinWidth(ibin+1)/2.)
        y_err_phiResp_barrel_3.append(phiResp_barrel_3.GetBinError(ibin+1))

        x_phiResp_endcap_3.append(phiResp_endcap_3.GetBinLowEdge(ibin+1) + phiResp_endcap_3.GetBinWidth(ibin+1)/2.)
        y_phiResp_endcap_3.append(phiResp_endcap_3.GetBinContent(ibin+1))
        x_err_phiResp_endcap_3.append(phiResp_endcap_3.GetBinWidth(ibin+1)/2.)
        y_err_phiResp_endcap_3.append(phiResp_endcap_3.GetBinError(ibin+1))

        x_phiResp_inclusive_3.append(phiResp_inclusive_3.GetBinLowEdge(ibin+1) + phiResp_inclusive_3.GetBinWidth(ibin+1)/2.)
        y_phiResp_inclusive_3.append(phiResp_inclusive_3.GetBinContent(ibin+1))
        x_err_phiResp_inclusive_3.append(phiResp_inclusive_3.GetBinWidth(ibin+1)/2.)
        y_err_phiResp_inclusive_3.append(phiResp_inclusive_3.GetBinError(ibin+1))


    # PLOT ETA RESPONSE
    x_lim = (-0.15,0.15)
    x_label = r'$\eta^{\tau, L1} - \eta^{\tau, offline}$'
    barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
    endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
    inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
    legend_title = r'$p_{T}^{\tau, offline}>30\ GeV$'
    plot_name = 'responses/tau_eta_'+options.tag
    if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'

    plt.rcParams['legend.title_fontsize'] = 'xx-small'
    cmap = matplotlib.cm.get_cmap('Set1')
    plot_x = np.linspace(-3,3,4000)
#    fig, ax = plt.subplots(figsize=(10,10))
#    
#    ax.errorbar(x_etaResp_barrel, y_etaResp_barrel, xerr=x_err_etaResp_barrel, yerr=y_err_etaResp_barrel, ls='None', label=barrel_label+' - Unpacked 2018 - Isolation', lw=2, marker='o', color=cmap(0))
#    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
#    p0 =          [   1.,   0.2,     1.,    1.,     1.,    1.,  0.1]
#    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
#                  [  10.,   1. ,    10.,   50.,    10.,   50.,  1. ])
#    popt, pcov = curve_fit(vectDoubleCB, x_etaResp_barrel[2:], y_etaResp_barrel[2:], p0, maxfev=5000, bounds=param_bounds)
#    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(0))
#
#    ax.errorbar(x_etaResp_barrel_2, y_etaResp_barrel_2, xerr=x_err_etaResp_barrel_2, yerr=y_err_etaResp_barrel_2, ls='None', label=barrel_label+' - Unpacked 2022 - Isolation', lw=2, marker='o', color=cmap(1))
#    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
#    p0 =          [   1.,   0.2,     1.,    1.,     1.,    1.,  0.1]
#    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
#                  [  10.,   1. ,    10.,   50.,    10.,   50.,  1. ])
#    popt, pcov = curve_fit(vectDoubleCB, x_etaResp_barrel_2[2:], y_etaResp_barrel_2[2:], p0, maxfev=5000, bounds=param_bounds)
#    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(1))
#
#    ax.errorbar(x_etaResp_barrel_3, y_etaResp_barrel_3, xerr=x_err_etaResp_barrel_3, yerr=y_err_etaResp_barrel_3, ls='None', label=barrel_label+' - Unpacked 2023 - Isolation', lw=2, marker='o', color=cmap(1))
#    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
#    p0 =          [   1.,   0.2,     1.,    1.,     1.,    1.,  0.1]
#    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
#                  [  10.,   1. ,    10.,   50.,    10.,   50.,  1. ])
#    popt, pcov = curve_fit(vectDoubleCB, x_etaResp_barrel_3[2:], y_etaResp_barrel_3[2:], p0, maxfev=5000, bounds=param_bounds)
#    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(1))
#    leg = plt.legend(loc = 'upper left', fontsize=20, title=legend_title)
#    leg._legend_box.align = "left"
#    plt.ylim(0., max(max(y_etaResp_barrel_2), max(y_etaResp_endcap_2)) * 1.4)
#    plt.xlim(x_lim)
#    # plt.yscale('log')
#    plt.xlabel(x_label)
#    plt.ylabel(r'a.u.')
#    plt.grid()
#    for xtick in ax.xaxis.get_major_ticks():
#        xtick.set_pad(10)
#    mplhep.cms.label('Preliminary', data=True, rlabel=r'3.2 fb$^{-1}$ (13.6 TeV)')
#    plt.savefig(plot_name+'_barrel.pdf')
#    plt.savefig(plot_name+'_barrel.png')
#    plt.close()
#
#    fig, ax = plt.subplots(figsize=(10,10))
#
#    ax.errorbar(x_etaResp_endcap, y_etaResp_endcap, xerr=x_err_etaResp_endcap, yerr=y_err_etaResp_endcap, ls='None', label=endcap_label+' - Unpacked 2022 - Isolation $p_{T}^{	au}$>0', lw=2, marker='s', color=cmap(0))
#    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
#    p0 =          [   1.,   0.2,     1.,    1.,     1.,    1.,  0.1]
#    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
#                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
#    popt, pcov = curve_fit(vectDoubleCB, x_etaResp_endcap[2:], y_etaResp_endcap[2:], p0, maxfev=5000, bounds=param_bounds)
#    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(0))
#
#    ax.errorbar(x_scale_endcap_2, y_scale_endcap_2, xerr=x_err_scale_endcap_2, yerr=y_err_scale_endcap_2, ls='None', label=endcap_label+' - Unpacked 2022 - $p_{T}^{	au}$>0', lw=2, marker='s', color=cmap(1))
#    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
#    p0 =          [   1.,   0.2,     1.,    1.,     1.,    1.,  0.1]
#    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
#                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
#    popt, pcov = curve_fit(vectDoubleCB, x_scale_endcap_2[2:], y_scale_endcap_2[2:], p0, maxfev=5000, bounds=param_bounds)
#    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(1))
#
#
#    leg = plt.legend(loc = 'upper left', fontsize=20, title=legend_title)
#    leg._legend_box.align = "left"
#    plt.ylim(0., max(max(y_scale_barrel_2), max(y_scale_endcap_2)) * 1.4)
#    plt.xlim(x_lim)
#    # plt.yscale('log')
#    plt.xlabel(x_label)
#    plt.ylabel(r'a.u.')
#    plt.grid()
#    for xtick in ax.xaxis.get_major_ticks():
#        xtick.set_pad(10)
#    mplhep.cms.label('Preliminary', data=True, rlabel=r'3.2 fb$^{-1}$ (13.6 TeV)')
#    plt.savefig(plot_name+'_endcap.pdf')
#    plt.savefig(plot_name+'_endcap.png')
#    plt.close()


    fig, ax = plt.subplots(figsize=(10,10))

    ax.errorbar(x_etaResp_inclusive, y_etaResp_inclusive, xerr=x_err_etaResp_inclusive, yerr=y_err_etaResp_inclusive, ls='None', label=inclusive_label+r' - Unpacked 2018 - Isolation', lw=2, marker='s', color='black')
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   0.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_etaResp_inclusive[2:], y_etaResp_inclusive[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color='black')

    ax.errorbar(x_etaResp_inclusive_2, y_etaResp_inclusive_2, xerr=x_err_etaResp_inclusive_2, yerr=y_err_etaResp_inclusive_2, ls='None', label=inclusive_label+r' - Unpacked 2022 - Isolation', lw=2, marker='s', color=cmap(0))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   0.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_etaResp_inclusive_2[2:], y_etaResp_inclusive_2[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(0))

    ax.errorbar(x_etaResp_inclusive_3, y_etaResp_inclusive_3, xerr=x_err_etaResp_inclusive_3, yerr=y_err_etaResp_inclusive_3, ls='None', label=inclusive_label+r' - Unpacked 2023 - Isolation', lw=2, marker='s', color=cmap(1))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   0.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_etaResp_inclusive_3[2:], y_etaResp_inclusive_3[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(1))

    leg = plt.legend(loc = 'upper left', fontsize=18)
    leg._legend_box.align = "left"
    plt.ylim(0., max(max(y_etaResp_inclusive), max(y_etaResp_inclusive_2)) * 1.4)
    plt.xlim(x_lim)
    # plt.yscale('log')
    plt.xlabel(x_label)
    plt.ylabel(r'a.u.')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'2022 EraG (13.6 TeV)')
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()






    # PLOT PHI RESPONSE
#    x_lim = (20.,110.)
#    y_lim = (min(y_phiResp_barrel_2[9], y_phiResp_endcap_2[9]) * 0.6, max(max(y_phiResp_barrel_2), max(y_phiResp_endcap_2)) * 1.1)
#    x_label = r'$p_{T}^{\tau, offline}\ [GeV]$'
#    barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
#    endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
#    inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
#    plot_name = 'responses/tau_eta_'+options.tag
#    if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'
#
#    fig, ax = plt.subplots(figsize=(10,10))
#    
#    ax.errorbar(x_ptResol_barrel, y_ptResol_barrel, xerr=x_err_ptResol_barrel, yerr=y_err_ptResol_barrel, ls='None', label=barrel_label+' - Unpacked 2022 - Isolation $p_{T}^{	au}$>0', lw=2, marker='o', color=cmap(0))
#    ax.errorbar(x_ptResol_endcap_2, y_ptResol_endcap_2, xerr=x_err_ptResol_endcap_2, yerr=y_err_ptResol_endcap_2, ls='None', label=endcap_label+' - Unpacked 2022 - $p_{T}^{	au}$>0', lw=2, marker='s', color=cmap(1))
#
#    leg = plt.legend(loc = 'upper right', fontsize=20)
#    leg._legend_box.align = "left"
#    plt.ylim(y_lim)
#    plt.xlim(x_lim)
#    # plt.yscale('log')
#    plt.xlabel(x_label)
#    plt.ylabel(r'Energy resolution')
#    plt.grid()
#    for xtick in ax.xaxis.get_major_ticks():
#        xtick.set_pad(10)
#    mplhep.cms.label('Preliminary', data=True, rlabel=r'3.2 fb$^{-1}$ (13.6 TeV)')
#    plt.savefig(plot_name+'_barrel.pdf')
#    plt.savefig(plot_name+'_barrel.png')
#    plt.close()
#    
#
#    fig, ax = plt.subplots(figsize=(10,10))
#
#    ax.errorbar(x_ptResol_endcap, y_ptResol_endcap, xerr=x_err_ptResol_endcap, yerr=y_err_ptResol_endcap, ls='None', label=endcap_label+' - Unpacked 2022 - Isolation $p_{T}^{	au}$>0', lw=2, marker='s', color=cmap(0))
#    ax.errorbar(x_ptResol_barrel_2, y_ptResol_barrel_2, xerr=x_err_ptResol_barrel_2, yerr=y_err_ptResol_barrel_2, ls='None', label=barrel_label+' - Unpacked 2022 - $p_{T}^{	au}$>0', lw=2, marker='o', color=cmap(1))    
#
#    leg = plt.legend(loc = 'upper right', fontsize=20)
#    leg._legend_box.align = "left"
#    plt.ylim(y_lim)
#    plt.xlim(x_lim)
#    # plt.yscale('log')
#    plt.xlabel(x_label)
#    plt.ylabel(r'Energy resolution')
#    plt.grid()
#    for xtick in ax.xaxis.get_major_ticks():
#        xtick.set_pad(10)
#    mplhep.cms.label('Preliminary', data=True, rlabel=r'3.2 fb$^{-1}$ (13.6 TeV)')
#    plt.savefig(plot_name+'_endcap.pdf')
#    plt.savefig(plot_name+'_endcap.png')
#    plt.close()


    # PLOT PHI RESPONSE
    x_lim = (-0.25,0.25)
    y_lim = (min(y_phiResp_inclusive[9], y_phiResp_inclusive_2[9]) * 0.6, max(max(y_phiResp_inclusive), max(y_phiResp_inclusive_2)) * 1.4)
    x_label = r'$\phi^{\tau, L1} - \phi^{\tau, offline}$'
    barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
    endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
    inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
    plot_name = 'responses/tau_phi_'+options.tag
    if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'

    fig, ax = plt.subplots(figsize=(10,10))
    
    ax.errorbar(x_phiResp_inclusive, y_phiResp_inclusive, xerr=x_err_phiResp_inclusive, yerr=y_err_phiResp_inclusive, ls='None', label=inclusive_label+r' - Unpacked 2018 - Isolation', lw=2, marker='s', color='black')
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   0.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_phiResp_inclusive[2:], y_phiResp_inclusive[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color='black')

    ax.errorbar(x_phiResp_inclusive_2, y_phiResp_inclusive_2, xerr=x_err_phiResp_inclusive_2, yerr=y_err_phiResp_inclusive_2, ls='None', label=inclusive_label+r' - Unpacked 2022 - Isolation', lw=2, marker='s', color=cmap(0))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   0.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_phiResp_inclusive_2[2:], y_phiResp_inclusive_2[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(0))

    ax.errorbar(x_phiResp_inclusive_3, y_phiResp_inclusive_3, xerr=x_err_phiResp_inclusive_3, yerr=y_err_phiResp_inclusive_3, ls='None', label=inclusive_label+r' - Unpacked 2023 - Isolation', lw=2, marker='s', color=cmap(1))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   0.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_phiResp_inclusive_3[2:], y_phiResp_inclusive_3[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(1))
    
    leg = plt.legend(loc = 'upper right', fontsize=18)
    leg._legend_box.align = "left"
    plt.ylim(y_lim)
    plt.xlim(x_lim)
    # plt.yscale('log')
    plt.xlabel(x_label)
    plt.ylabel(r'Energy resolution')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'2022 EraG 13.6 TeV')
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()
 
