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
import matplotlib.patches as patches
import mplhep
plt.style.use(mplhep.style.CMS)

single_pt_bins = [
    'pt_resp_ptBin20.0to25.0',
    'pt_resp_ptBin25.0to30.0',
    'pt_resp_ptBin30.0to35.0',
    'pt_resp_ptBin35.0to40.0',
    'pt_resp_ptBin40.0to45.0',
    'pt_resp_ptBin45.0to50.0',
    'pt_resp_ptBin50.0to60.0',
    'pt_resp_ptBin60.0to70.0',
    'pt_resp_ptBin70.0to90.0',
    'pt_resp_ptBin90.0to110.0'
]

pt_bins_edges=[20,25,30,35,40,45,50,60,70,90,110]
pt_bin_centers = [(pt_bins_edges[i] + pt_bins_edges[i+1]) / 2 for i in range(len(pt_bins_edges) - 1)]

single_eta_bins = [
    'pt_resp_AbsEtaBin0.000to0.500',
    'pt_resp_AbsEtaBin0.500to1.000',
    'pt_resp_AbsEtaBin1.000to1.305',
    'pt_resp_AbsEtaBin1.305to1.479',
    'pt_resp_AbsEtaBin1.479to1.800',
    'pt_resp_AbsEtaBin1.800to2.100'
]

eta_bin_edges=[0.000,0.500,1.000,1.305,1.479,1.800,2.100]
eta_bin_centers = [(eta_bin_edges[i] + eta_bin_edges[i+1]) / 2 for i in range(len(eta_bin_edges) - 1)]

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

def compute_scale(inFile, inclusive='pt_response_ptInclusive'):
    scale_barrel      = inFile.Get('pt_barrel_resp_ptInclusive')
    scale_endcap      = inFile.Get('pt_endcap_resp_ptInclusive')
    scale_inclusive   = inFile.Get(inclusive)

    print("Trial !!!!", inclusive)
    # histo_test = scale_inclusive.GetHistogram()
    print(scale_inclusive.GetMean(), scale_inclusive.GetStdDev())

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

def compute_resol(inFile, inclusive='pt_resol_fctPt'):
    ptResol_barrel    = inFile.Get('pt_resol_barrel_fctPt')
    ptResol_endcap    = inFile.Get('pt_resol_endcap_fctPt')
    ptResol_inclusive = inFile.Get(inclusive)

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

        print("mean",ptResol_inclusive.GetMean())

    return x_ptResol_inclusive, y_ptResol_inclusive, x_err_ptResol_inclusive, y_err_ptResol_inclusive


def plot_pt_scale(inFile, label, color, ax, bin_tree='pt_response_ptInclusive'):
    x_lim = (0.,2.)
    x_label = r'$E_{T}^{\tau, L1}/p_{T}^{\tau, offline}$'
    # barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
    # endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
    # inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'    
    # inclusive_label = ""  
    plt.rcParams['legend.title_fontsize'] = 'xx-small'
    cmap = matplotlib.colormaps.get_cmap('Set1')
    plot_x = np.linspace(-3,3,4000)

    x_scale_inclusive, y_scale_inclusive, x_err_scale_inclusive, y_err_scale_inclusive = compute_scale(inFile, bin_tree)

    ax.errorbar(x_scale_inclusive, y_scale_inclusive, xerr=x_err_scale_inclusive, yerr=y_err_scale_inclusive, 
                ls='None', label=label, lw=2, marker='s', color=cmap(color))
    


    ##            [ mean, sigma, alphaL,    nL, alphaR,    nR,  norm]
    p0 =          [   1.,   0.2,     1.,    1.,     1.,    1.,  0.1]
    param_bounds=([ -10.,   0. ,     0.,    0.,     0.,    0.,  0. ],
                  [  10.,   1. ,    10.,   25.,    10.,   25.,  1. ])
    popt, pcov = curve_fit(vectDoubleCB, x_scale_inclusive[2:], y_scale_inclusive[2:], p0, maxfev=5000, bounds=param_bounds)
    ax.plot(plot_x, vectDoubleCB(plot_x, *popt), '-', label='_', lw=2, color=cmap(color))
    
    fit_mean = popt[0]
    variance= popt[1]
    ax.axvline(x=fit_mean, color='grey', linestyle='--', label=f'Mean: {fit_mean:.2f}')

    # Title legend setup
    inclusive_leg = ax.legend(title=r'$Inclusive\ |\eta^{\tau, offline}|<2.1$:', loc='upper left', fontsize=15)
    # barrel = ax.legend(title=r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$', loc='upper left', fontsize=15)
    # endcap_leg = ax.legend(title=r'$Barrel\ |\eta^{\tau, offline}|<1.305$', loc='upper left', fontsize=15) 
    inclusive_leg._legend_box.align = "left"

    
    # leg = plt.legend(loc = 'upper left', fontsize=15)
    # leg._legend_box.align = "left"
    # plt.ylim(0.,0.167)
    # plt.ylim(0.,0.200)
    plt.xlim(x_lim)
    plt.xlabel(x_label)
    plt.ylabel(r'a.u.')
    plt.grid(linestyle=":")
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')

    return fit_mean, variance


def plot_pt_resolution(inFile, label, color, ax, bin_tree='pt_resol_fctPt', observable="pt"):
    barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
    endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
    inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
    # inclusive_label = ''
   

    # cmap = matplotlib.cm.get_cmap('Set1')
    cmap = matplotlib.colormaps.get_cmap('Set1')

    x_ptResol_inclusive, y_ptResol_inclusive, x_err_ptResol_inclusive, y_err_ptResol_inclusive = compute_resol(inFile, bin_tree)

    ax.errorbar(x_ptResol_inclusive, y_ptResol_inclusive, xerr=x_err_ptResol_inclusive, yerr=y_err_ptResol_inclusive, 
                ls='None', label=inclusive_label+ "\n" +label, lw=2, marker='o', color=cmap(color))

    if observable == "pt":
        x_lim = (20.,110.)
        y_lim=(0.05,0.3)
        x_label = r'$p_{T}^{\tau, offline}\ [GeV]$'

        leg = plt.legend(loc = 'lower right', fontsize=18)
        leg._legend_box.align = "left"
        plt.ylim(y_lim)
        plt.xlim(x_lim)
        plt.xlabel(x_label)
        plt.ylabel(r'Energy resolution')
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
    
    if observable == "eta":
        x_lim=(-2.1,2.1)
        y_lim=(0.4,0.65)
        x_label = r'$\eta^{\tau, offline}$'

        #Defining barrel and endacp regions on the plot
        barrel_range = (-1.305, 1.305)  
        endcap_range = (1.479, 2.1)  
        endcap_range_negative = (-1.479, -2.1)  

        ax.axvspan(barrel_range[0], barrel_range[1], color='lightblue', alpha=0.3, label='Barrel Region')
        ax.axvspan(endcap_range[0], endcap_range[1], color='lightgreen', alpha=0.3, label='Endcap Region')
        ax.axvspan(endcap_range_negative[0], endcap_range_negative[1], color='lightgreen', alpha=0.3)
        
        ax.axvline(barrel_range[1], color='lightblue', linestyle='--')
        ax.axvline(barrel_range[0], color='lightblue', linestyle='--')
        ax.axvline(endcap_range[0], color='lightgreen', linestyle='--')
        ax.axvline(endcap_range_negative[0], color='lightgreen', linestyle='--')

        # leg = plt.legend(loc = 'lower right', fontsize=18)
        # Create the legend with custom positioning and font size
        leg = plt.legend(loc='upper center', bbox_to_anchor=(0.5, 0.95), fontsize=18, frameon=True)

        # Set a white background for the legend
        leg.get_frame().set_facecolor('white')

        # Optionally, add a border around the legend (if desired)
        leg.get_frame().set_edgecolor('black')
        # leg._legend_box.align = "left"
        plt.ylim(y_lim)
        plt.xlim(x_lim)
        plt.xlabel(x_label)
        plt.ylabel(r'Energy resolution')
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')


def create_and_save_plot(single_bin, label, in_file, plot_tag):
    print('Producing plot for', single_bin)
    fig, ax = plt.subplots(figsize=(10, 10))
    plot_pt_scale(in_file, label, 0, ax, single_bin)
    mean, variance=plot_pt_scale(in_file, label, 0, ax, single_bin)
    plot_name = f'responses/2024/tau_pt_scale_{plot_tag}_{single_bin}'
    plt.savefig(f'{plot_name}.pdf')
    plt.savefig(f'{plot_name}.png')
    # plot_pt_resolution(in_file, label, 0, ax, single_bin)
    # plot_name = f'responses/2024/tau_pt_resolution_{plot_tag}_{single_bin}'
    # plt.savefig(f'{plot_name}.pdf')
    # plt.savefig(f'{plot_name}.png')
    plt.close()
    return mean, variance
 

#######################################################################
######################### SCRIPT BODY #################################
#######################################################################

''' python3 Response_fitter.py --inFile1 --inFile2 --tag  '''

if __name__ == "__main__" :
    parser = OptionParser()
    parser = OptionParser()
    parser.add_option("--inFile1",   dest="inFile1",                        default=None)
    parser.add_option("--inFile2",   dest="inFile2",                        default=None)
    # parser.add_option("--inFile3",   dest="inFile3",                        default=None)
    parser.add_option("--tag",       dest="tag",                            default=None)
    parser.add_option("--bins", dest="bins", action="store_true", default=False)
    parser.add_option("--inclusive", dest="inclusive", action='store_true', default=False)
    (options, args) = parser.parse_args()
    print(options)

    main_folder = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2024/'
    main_folder = '/data_CMS/cms/mchiusi/Run3preparation/Run3_2024/'
    inFile1 = ROOT.TFile(main_folder+options.inFile1)
    # inFile2 = ROOT.TFile(main_folder+options.inFile2)
    # inFile3 = ROOT.TFile(main_folder+options.inFile3)

    # label1 = r'Unpacked 2024 EraH'
    # label2 = r'Re-Emu 2024H w/ 24W opt'
    label1 = r'2024 MC ReEmu'+ "\n" + "w/ LUTs 2023"
    label2 = r'Muon_Run2024I_MINIAOD'
    # label3 = r'Re-Emu pedestals + corrections'

    # PLOT PT SCALE
    fig, ax = plt.subplots(figsize=(10,10))
    plot_pt_scale(inFile1, label1, 0, ax) # , 'pt_barrel_resp_ptInclusive')
    # plot_pt_scale(inFile2, label2, 1, ax) # , 'pt_endcap_resp_ptInclusive')
    # plot_pt_scale(inFile3, label3, 2, ax)

    plot_name = 'responses/2024/tau_pt_scale_'+options.tag
    print(plot_name+'.pdf') 
    if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()

    if options.bins:
        # bins_to_process = single_pt_bins + single_eta_bins
        mean_pt=[]
        mean_eta=[]
        variance_pt=[]
        variance_eta=[]

        for single_bin in single_pt_bins:
            label_1_bins = "Run2024W-MC-MINIAOD\n" + \
                "caloparams 2023\n" + \
               f"{single_bin.split('pt_resp_')[-1]}"
            # label1 = f'Muon Run2024I MINIAOD_{single_bin.split("pt_resp_")[-1]}'
            # create_and_save_plot(single_bin, label_1_bins, inFile1, options.tag)
            mean_i,variance_i=create_and_save_plot(single_bin, label_1_bins, inFile1, options.tag)
            mean_pt.append(mean_i)
            variance_pt.append(variance_i)
        

        print("variance",variance_pt)

        for single_bin in single_eta_bins:
            label_1_bins = "Run2024W-MC-MINIAOD\n" + \
                "caloparams 2023\n" + \
               f"{single_bin.split('pt_resp_')[-1]}"
            # label1 = f'Muon Run2024I MINIAOD_{single_bin.split("pt_resp_")[-1]}'
            # create_and_save_plot(single_bin, label_1_bins, inFile1, options.tag)
            mean_i,variance_i=create_and_save_plot(single_bin, label_1_bins, inFile1, options.tag)
            mean_eta.append(mean_i)
            variance_eta.append(variance_i)

        # Plot the mean as a function of pt bin

        # plt.scatter(pt_bin_centers, mean_pt, color='orange', label='Mean')
        # plt.plot(pt_bin_centers, mean_pt, linestyle='--', color='orange', alpha=0.7)
        plt.errorbar(pt_bin_centers, mean_pt, yerr=variance_pt, fmt='o', color='orange', 
             ecolor='orange', elinewidth=1.5, capsize=4)
        plt.xlabel(r'$p_t^{\tau, offline}$')
        plt.ylabel(r"$\langle E_{T}^{\tau, L1}/p_{T}^{\tau, offline}\rangle$")
        plt.legend()
        plt.grid(True, linestyle=':', alpha=0.6)
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
        plot_name = f'responses/2024/tau_mean_pt_scale_{options.tag}'
        plt.savefig(f'{plot_name}.pdf')
        plt.savefig(f'{plot_name}.png')
        plt.close()

        # Plot the mean as a function of eta bin

        # plt.scatter(pt_bin_centers, mean_pt, color='blue', label='Mean')
        # plt.plot(pt_bin_centers, mean_pt, linestyle='--', color='blue', alpha=0.7)
        plt.errorbar(eta_bin_centers, mean_eta, yerr=variance_eta, fmt='o', color='orange', 
             ecolor='orange', elinewidth=1.5, capsize=4)
        plt.xlabel(r'$|\eta|^{\tau, offline}$')
        plt.ylabel(r"$\langle E_{T}^{\tau, L1}/p_{T}^{\tau, offline}\rangle$")
        plt.legend()
        plt.grid(True, linestyle=':', alpha=0.6)
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
        plot_name = f'responses/2024/tau_mean_eta_scale_{options.tag}'
        plt.savefig(f'{plot_name}.pdf')
        plt.savefig(f'{plot_name}.png')
        plt.close()

        # Handle inclusive plot saving
        if options.inclusive:
            plot_name = f'responses/2024/tau_pt_scale_{options.tag}_{single_bin}_inclusive'
            plt.savefig(f'{plot_name}.pdf')
            plt.savefig(f'{plot_name}.png')
            plt.close()

    # PLOT PT RESOLUTION

    # First Plot: Plot pt_resolution with 'pt_resol_fctPt'
    fig1, ax1 = plt.subplots(figsize=(10, 10))
    plot_pt_resolution(inFile1, label1, 0, ax1)  # Plot for fctPt
    plot_name1 = 'responses/2024/tau_pt_resolution_' + options.tag + '_fctPt'
    print(f"Saving {plot_name1}.pdf")
    ax1.grid()
    if options.inclusive:
        plot_name1 = plot_name1[:-4] + '_inclusive'
    plt.savefig(plot_name1 + '.pdf')
    plt.savefig(plot_name1 + '.png')
    plt.close()

    # Second Plot: Plot pt_resolution with 'pt_resol_fctEta'
    fig2, ax2 = plt.subplots(figsize=(10, 10))
    plot_pt_resolution(inFile1, label1, 0, ax2, 'pt_resol_fctEta', "eta")  # Plot for fctEta
    plot_name2 = 'responses/2024/tau_pt_resolution_' + options.tag + '_fctEta'
    print(f"Saving {plot_name2}.pdf")
    ax2.grid()
    if options.inclusive:
        plot_name2 = plot_name2[:-4] + '_inclusive'
    plt.savefig(plot_name2 + '.pdf')
    plt.savefig(plot_name2 + '.png')
    plt.close()
