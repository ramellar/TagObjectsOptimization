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


    inFile2 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2023/'+options.inFile2)

    scale_barrel_2      = inFile2.Get('pt_barrel_resp_ptInclusive')
    scale_endcap_2      = inFile2.Get('pt_endcap_resp_ptInclusive')
    scale_inclusive_2   = inFile2.Get('pt_response_ptInclusive')
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
    x_scale_barrel_2 = []
    y_scale_barrel_2 = []
    x_err_scale_barrel_2 = []
    y_err_scale_barrel_2 = []
    x_scale_endcap_2 = []
    y_scale_endcap_2 = []
    x_err_scale_endcap_2 = []
    y_err_scale_endcap_2 = []
    x_scale_inclusive_2 = []
    y_scale_inclusive_2 = []
    x_err_scale_inclusive_2 = []
    y_err_scale_inclusive_2 = []
    for ibin in range(scale_barrel.GetNbinsX()):
        x_scale_barrel_2.append(scale_barrel_2.GetBinLowEdge(ibin+1) + scale_barrel_2.GetBinWidth(ibin+1)/2.)
        y_scale_barrel_2.append(scale_barrel_2.GetBinContent(ibin+1))
        x_err_scale_barrel_2.append(scale_barrel_2.GetBinWidth(ibin+1)/2.)
        y_err_scale_barrel_2.append(scale_barrel_2.GetBinError(ibin+1))

        x_scale_endcap_2.append(scale_endcap_2.GetBinLowEdge(ibin+1) + scale_endcap_2.GetBinWidth(ibin+1)/2.)
        y_scale_endcap_2.append(scale_endcap_2.GetBinContent(ibin+1))
        x_err_scale_endcap_2.append(scale_endcap_2.GetBinWidth(ibin+1)/2.)
        y_err_scale_endcap_2.append(scale_endcap_2.GetBinError(ibin+1))

        x_scale_inclusive_2.append(scale_inclusive_2.GetBinLowEdge(ibin+1) + scale_inclusive_2.GetBinWidth(ibin+1)/2.)
        y_scale_inclusive_2.append(scale_inclusive_2.GetBinContent(ibin+1))
        x_err_scale_inclusive_2.append(scale_inclusive_2.GetBinWidth(ibin+1)/2.)
        y_err_scale_inclusive_2.append(scale_inclusive_2.GetBinError(ibin+1))

    # CONVERT TO LISTS FOR PYPLOT
    x_ptResol_barrel_2 = []
    y_ptResol_barrel_2 = []
    x_err_ptResol_barrel_2 = []
    y_err_ptResol_barrel_2 = []
    x_ptResol_endcap_2 = []
    y_ptResol_endcap_2 = []
    x_err_ptResol_endcap_2 = []
    y_err_ptResol_endcap_2 = []
    x_ptResol_inclusive_2 = []
    y_ptResol_inclusive_2 = []
    x_err_ptResol_inclusive_2 = []
    y_err_ptResol_inclusive_2 = []
    for ibin in range(ptResol_barrel.GetNbinsX()):
        x_ptResol_barrel_2.append(ptResol_barrel_2.GetBinLowEdge(ibin+1) + ptResol_barrel_2.GetBinWidth(ibin+1)/2.)
        y_ptResol_barrel_2.append(ptResol_barrel_2.GetBinContent(ibin+1))
        x_err_ptResol_barrel_2.append(ptResol_barrel_2.GetBinWidth(ibin+1)/2.)
        y_err_ptResol_barrel_2.append(ptResol_barrel_2.GetBinError(ibin+1))

        x_ptResol_endcap_2.append(ptResol_endcap_2.GetBinLowEdge(ibin+1) + ptResol_endcap_2.GetBinWidth(ibin+1)/2.)
        y_ptResol_endcap_2.append(ptResol_endcap_2.GetBinContent(ibin+1))
        x_err_ptResol_endcap_2.append(ptResol_endcap_2.GetBinWidth(ibin+1)/2.)
        y_err_ptResol_endcap_2.append(ptResol_endcap_2.GetBinError(ibin+1))

        x_ptResol_inclusive_2.append(ptResol_inclusive_2.GetBinLowEdge(ibin+1) + ptResol_inclusive_2.GetBinWidth(ibin+1)/2.)
        y_ptResol_inclusive_2.append(ptResol_inclusive_2.GetBinContent(ibin+1))
        x_err_ptResol_inclusive_2.append(ptResol_inclusive_2.GetBinWidth(ibin+1)/2.)
        y_err_ptResol_inclusive_2.append(ptResol_inclusive_2.GetBinError(ibin+1))


    # inFile3 = ROOT.TFile('/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2023/'+options.inFile3)

    # scale_barrel_3      = inFile3.Get('pt_barrel_resp_ptInclusive')
    # scale_endcap_3      = inFile3.Get('pt_endcap_resp_ptInclusive')
    # scale_inclusive_3   = inFile3.Get('pt_response_ptInclusive')
    # ptResol_barrel_3    = inFile3.Get('pt_resol_barrel_fctPt')
    # ptResol_endcap_3    = inFile3.Get('pt_resol_endcap_fctPt')
    # ptResol_inclusive_3 = inFile3.Get('pt_resol_fctPt')
    # etaResp_barrel_3    = inFile3.Get('eta_resp_barrel')
    # etaResp_endcap_3    = inFile3.Get('eta_resp_endcap')
    # etaResp_inclusive_3 = inFile3.Get('eta_resp_inclusive')
    # phiResp_barrel_3    = inFile3.Get('phi_resp_barrel')
    # phiResp_endcap_3    = inFile3.Get('phi_resp_endcap')
    # phiResp_inclusive_3 = inFile3.Get('phi_resp_inclusive')

    # # CONVERT TO LISTS FOR PYPLOT
    # x_scale_barrel_3 = []
    # y_scale_barrel_3 = []
    # x_err_scale_barrel_3 = []
    # y_err_scale_barrel_3 = []
    # x_scale_endcap_3 = []
    # y_scale_endcap_3 = []
    # x_err_scale_endcap_3 = []
    # y_err_scale_endcap_3 = []
    # x_scale_inclusive_3 = []
    # y_scale_inclusive_3 = []
    # x_err_scale_inclusive_3 = []
    # y_err_scale_inclusive_3 = []
    # for ibin in range(scale_barrel.GetNbinsX()):
    #     x_scale_barrel_3.append(scale_barrel_3.GetBinLowEdge(ibin+1) + scale_barrel_3.GetBinWidth(ibin+1)/2.)
    #     y_scale_barrel_3.append(scale_barrel_3.GetBinContent(ibin+1))
    #     x_err_scale_barrel_3.append(scale_barrel_3.GetBinWidth(ibin+1)/2.)
    #     y_err_scale_barrel_3.append(scale_barrel_3.GetBinError(ibin+1))

    #     x_scale_endcap_3.append(scale_endcap_3.GetBinLowEdge(ibin+1) + scale_endcap_3.GetBinWidth(ibin+1)/2.)
    #     y_scale_endcap_3.append(scale_endcap_3.GetBinContent(ibin+1))
    #     x_err_scale_endcap_3.append(scale_endcap_3.GetBinWidth(ibin+1)/2.)
    #     y_err_scale_endcap_3.append(scale_endcap_3.GetBinError(ibin+1))

    #     x_scale_inclusive_3.append(scale_inclusive_3.GetBinLowEdge(ibin+1) + scale_inclusive_3.GetBinWidth(ibin+1)/2.)
    #     y_scale_inclusive_3.append(scale_inclusive_3.GetBinContent(ibin+1))
    #     x_err_scale_inclusive_3.append(scale_inclusive_3.GetBinWidth(ibin+1)/2.)
    #     y_err_scale_inclusive_3.append(scale_inclusive_3.GetBinError(ibin+1))

    # # CONVERT TO LISTS FOR PYPLOT
    # x_ptResol_barrel_3 = []
    # y_ptResol_barrel_3 = []
    # x_err_ptResol_barrel_3 = []
    # y_err_ptResol_barrel_3 = []
    # x_ptResol_endcap_3 = []
    # y_ptResol_endcap_3 = []
    # x_err_ptResol_endcap_3 = []
    # y_err_ptResol_endcap_3 = []
    # x_ptResol_inclusive_3 = []
    # y_ptResol_inclusive_3 = []
    # x_err_ptResol_inclusive_3 = []
    # y_err_ptResol_inclusive_3 = []
    # for ibin in range(ptResol_barrel.GetNbinsX()):
    #     x_ptResol_barrel_3.append(ptResol_barrel_3.GetBinLowEdge(ibin+1) + ptResol_barrel_3.GetBinWidth(ibin+1)/2.)
    #     y_ptResol_barrel_3.append(ptResol_barrel_3.GetBinContent(ibin+1))
    #     x_err_ptResol_barrel_3.append(ptResol_barrel_3.GetBinWidth(ibin+1)/2.)
    #     y_err_ptResol_barrel_3.append(ptResol_barrel_3.GetBinError(ibin+1))

    #     x_ptResol_endcap_3.append(ptResol_endcap_3.GetBinLowEdge(ibin+1) + ptResol_endcap_3.GetBinWidth(ibin+1)/2.)
    #     y_ptResol_endcap_3.append(ptResol_endcap_3.GetBinContent(ibin+1))
    #     x_err_ptResol_endcap_3.append(ptResol_endcap_3.GetBinWidth(ibin+1)/2.)
    #     y_err_ptResol_endcap_3.append(ptResol_endcap_3.GetBinError(ibin+1))

    #     x_ptResol_inclusive_3.append(ptResol_inclusive_3.GetBinLowEdge(ibin+1) + ptResol_inclusive_3.GetBinWidth(ibin+1)/2.)
    #     y_ptResol_inclusive_3.append(ptResol_inclusive_3.GetBinContent(ibin+1))
    #     x_err_ptResol_inclusive_3.append(ptResol_inclusive_3.GetBinWidth(ibin+1)/2.)
    #     y_err_ptResol_inclusive_3.append(ptResol_inclusive_3.GetBinError(ibin+1))


    # PLOT PT SCALE
    x_lim = (0.,2.)
    x_label = r'$E_{T}^{\tau, L1}/p_{T}^{\tau, offline}$'
    barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
    endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
    inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
    legend_title = r'$p_{T}^{\tau, offline}>30\ GeV$'
    plot_name = 'responses/tau_pt_scale_'+options.tag
    if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'

    plt.rcParams['legend.title_fontsize'] = 'xx-small'
    cmap = matplotlib.cm.get_cmap('Set1')
    plot_x = np.linspace(-3,3,4000)
    fig, ax = plt.subplots(figsize=(10,10))

    ax.errorbar(x_scale_inclusive, y_scale_inclusive, xerr=x_err_scale_inclusive, yerr=y_err_scale_inclusive, ls='None', label=inclusive_label+r' - new SF + new opt LUT1', lw=2, marker='s', color=cmap(0))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   1.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_scale_inclusive[2:], y_scale_inclusive[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(0))

    ax.errorbar(x_scale_inclusive_2, y_scale_inclusive_2, xerr=x_err_scale_inclusive_2, yerr=y_err_scale_inclusive_2, ls='None', label=inclusive_label+r' - curr SF + curr LUT1', lw=2, marker='s', color=cmap(1))
    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   1.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_scale_inclusive_2[2:], y_scale_inclusive_2[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(1))

    # ax.errorbar(x_scale_inclusive_3, y_scale_inclusive_3, xerr=x_err_scale_inclusive_3, yerr=y_err_scale_inclusive_3, ls='None', label=inclusive_label+r' - curr SF + curr opt LUT1', lw=2, marker='s', color=cmap(2))
    # ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    # p0 =          [   1.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    # param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
    #               [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    # popt, pcov = curve_fit(vectDoubleCB, x_scale_inclusive_3[2:], y_scale_inclusive_3[2:], p0, maxfev=5000, bounds=param_bounds)
    # ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(2))

    leg = plt.legend(loc = 'upper left', fontsize=18)
    leg._legend_box.align = "left"
    plt.ylim(0.,0.167)# max(max(y_scale_inclusive), max(y_scale_inclusive_2)) * 1.4)
    plt.xlim(x_lim)
    # plt.yscale('log')
    plt.xlabel(x_label)
    plt.ylabel(r'a.u.')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()


    # PLOT PT RESOLUTION
    x_lim = (20.,110.)
    y_lim = (min(y_ptResol_barrel_2[9], y_ptResol_endcap_2[9]) * 0.6, max(max(y_ptResol_barrel_2), max(y_ptResol_endcap_2)) * 1.1)
    x_label = r'$p_{T}^{\tau, offline}\ [GeV]$'
    barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
    endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
    inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
    plot_name = 'responses/tau_pt_resolution_'+options.tag
    if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'

    # PLOT PT RESOLUTION
    x_lim = (20.,110.)
    y_lim = (min(y_ptResol_inclusive[9], y_ptResol_inclusive_2[9]) * 0.6, max(max(y_ptResol_inclusive), max(y_ptResol_inclusive_2)) * 0.6)
    x_label = r'$p_{T}^{\tau, offline}\ [GeV]$'
    barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
    endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
    inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
    plot_name = 'responses/tau_pt_resolution_'+options.tag
    if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'

    fig, ax = plt.subplots(figsize=(10,10))
    
    ax.errorbar(x_ptResol_inclusive, y_ptResol_inclusive, xerr=x_err_ptResol_inclusive, yerr=y_err_ptResol_inclusive, ls='None', label=inclusive_label+r' - new SF + new opt LUT1', lw=2, marker='o', color=cmap(0))
    ax.errorbar(x_ptResol_inclusive_2, y_ptResol_inclusive_2, xerr=x_err_ptResol_inclusive_2, yerr=y_err_ptResol_inclusive_2, ls='None', label=inclusive_label+r' - curr SF + curr LUT1', lw=2, marker='s', color=cmap(1))
    # ax.errorbar(x_ptResol_inclusive_3, y_ptResol_inclusive_3, xerr=x_err_ptResol_inclusive_3, yerr=y_err_ptResol_inclusive_3, ls='None', label=inclusive_label+r' - curr SF + curr opt LUT1', lw=2, marker='s', color=cmap(2))

    leg = plt.legend(loc = 'upper right', fontsize=18)
    leg._legend_box.align = "left"
    plt.ylim(0.05,0.30)
    plt.xlim(x_lim)
    # plt.yscale('log')
    plt.xlabel(x_label)
    plt.ylabel(r'Energy resolution')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()
 
