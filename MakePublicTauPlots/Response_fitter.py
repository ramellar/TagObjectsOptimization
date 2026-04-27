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

    # print("Trial !!!!", inclusive)
    # histo_test = scale_inclusive.GetHistogram()
    # print(scale_inclusive.GetMean(), scale_inclusive.GetStdDev())

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

        print("mean",ptResol_inclusive.GetBinContent(ibin+1))

    return x_ptResol_inclusive, y_ptResol_inclusive, x_err_ptResol_inclusive, y_err_ptResol_inclusive


def plot_pt_scale(inFile, label, color, ax, bin_tree='pt_response_ptInclusive'):
    x_lim = (0.,2.)
    x_label = r'$E_{T}^{\tau, L1}/p_{T}^{\tau, offline}$'

    barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
    endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
    # inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'    
    # inclusive_label = r"Gen Matched MC25W-" +"\n"+ r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
    plt.rcParams['legend.title_fontsize'] = 'xx-small'
    # cmap = matplotlib.colormaps.get_cmap('tab20')
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
    ax.axvline(x=fit_mean, color=cmap(color), linestyle='--', label=f'Mean: {fit_mean:.2f}')
    # ax.axvline(x=fit_mean+variance, color=cmap(color), linestyle='--', label=f'Sigma: $\pm${variance:.2f}')
    # ax.axvline(x=fit_mean-variance, color=cmap(color), linestyle='--')

    # Title legend setup
    if bin_tree=='pt_response_ptInclusive':
        pt_leg = ax.legend(title=r'$p_t^{\tau,offline} > 30 $ GeV'+ "\n" +r'$Inclusive\ |\eta^{\tau, offline}|<2.1$:', loc='upper right', fontsize=12)
        # pt_leg = ax.legend(title= r'$Inclusive\ |\eta^{\tau, offline}|<2.1$' + "\n" + r"Gen Matched MC25W cut on", loc='upper right', fontsize=15)
    elif bin_tree=='pt_barrel_resp_ptInclusive':
        # pt_leg = ax.legend(loc='upper right', fontsize=12)
        # pt_leg = ax.legend(title=r'$Inclusive\ |\eta^{\tau, offline}|<2.1$:', loc='upper right', fontsize=12)
        pt_leg = ax.legend(title=r'$p_t^{\tau,offline} > 30 $ GeV'+ "\n" +r'$Barrel\ |\eta^{\tau, offline}|<1.305$:', loc='upper right', fontsize=12)
    else:
        pt_leg = ax.legend(title=r'$p_t^{\tau,offline} > 30 $ GeV'+ "\n" +r'Endcaps '+ "\n" +r'$1.479<|\eta^{\tau, offline}|<2.1$:', loc='upper right', fontsize=12)
        # pt_leg = ax.legend(title=r'$p_t^{\tau,offline} > 30 $ GeV'+ "\n" +r'$Barrel\ |\eta^{\tau, offline}|<1.305$:', loc='upper right', fontsize=15)
    # inclusive_leg = ax.legend(title=r'$Inclusive\ |\eta^{\tau, offline}|<2.1$:', loc='upper left', fontsize=15)
    # barrel = ax.legend(title=r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$', loc='upper left', fontsize=15)
    # endcap_leg = ax.legend(title=r'$Barrel\ |\eta^{\tau, offline}|<1.305$', loc='upper left', fontsize=15) 
    pt_leg._legend_box.align = "left"
    # inclusive_leg._legend_box.align = "left"

    
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
    # barrel_label = r'$Barrel\ |\eta^{\tau, offline}|<1.305$'
    # endcap_label = r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$'
    # inclusive_label = r'$Inclusive\ |\eta^{\tau, offline}|<2.1$'
    inclusive_label = ''
   

    # cmap = matplotlib.cm.get_cmap('Set1')
    cmap = matplotlib.colormaps.get_cmap('Set1')

    x_ptResol_inclusive, y_ptResol_inclusive, x_err_ptResol_inclusive, y_err_ptResol_inclusive = compute_resol(inFile, bin_tree)

    # print(color, y_ptResol_inclusive)

    ax.errorbar(x_ptResol_inclusive, y_ptResol_inclusive, xerr=x_err_ptResol_inclusive, yerr=y_err_ptResol_inclusive, 
                ls='None', label= inclusive_label +label, lw=2, marker='o', color=cmap(color))

    if observable == "pt":
        x_lim = (20.,110.)
        y_lim=(0.0,0.45)
        x_label = r'$p_{T}^{\tau, offline}\ [GeV]$'
        if  bin_tree=='pt_resol_fctPt':
            pt_leg = ax.legend(title= r'$Inclusive\ |\eta^{\tau, offline}|<2.1$', loc='upper right', fontsize=16, title_fontsize=16)
        elif bin_tree=='pt_resol_barrel_fctPt':
           pt_leg = ax.legend(title= r'$Barrel\ |\eta^{\tau, offline}|<1.305$', loc='upper right', fontsize=16, title_fontsize=16)
        else:
          pt_leg = ax.legend(title= r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$', loc='upper right', fontsize=16, title_fontsize=16)
        pt_leg._legend_box.align = "left"

        # leg = plt.legend(title= r'$MiniAOD-MC25W-DY$',loc = 'lower right', fontsize=18)
        # leg._legend_box.align = "left"
        # plt.ylim(y_lim)
        plt.xlim(x_lim)
        plt.xlabel(x_label)
        plt.ylabel(r'Energy resolution')
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')

    if observable == "pt_scale":
        y_lim=(0.8,1.1)
        x_lim = (20.,110.)
        x_label = r'$p_{T}^{\tau, offline}\ [GeV]$'
        pt_leg = ax.legend(title= r'$Inclusive\ |\eta^{\tau, offline}|<2.1$', loc='upper right', fontsize=12)
        pt_leg.get_title().set_fontsize(16)
        pt_leg._legend_box.align = "left"
        # pt_leg = ax.legend(title= r'$Barrel\ |\eta^{\tau, offline}|<1.305$', loc='upper right', fontsize=16)
        # pt_leg = ax.legend(title= r'$Endcaps\ 1.479<|\eta^{\tau, offline}|<2.1$', loc='upper right', fontsize=16)

        # leg = plt.legend(title= r'$MiniAOD-MC25W-DY$',loc = 'lower right', fontsize=18)
        # leg._legend_box.align = "left"
        # plt.ylim(y_lim)
        plt.xlim(x_lim)
        plt.ylim(y_lim)
        plt.xlabel(x_label)
        plt.ylabel(r"$\langle E_{T}^{\tau, L1}/p_{T}^{\tau, offline}\rangle$")
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
        plt.grid(linestyle=":")
    
    if observable == "eta":
        x_lim=(-2.1,2.1)
        y_lim=(0.4,0.65)
        x_label = r'$\eta^{\tau, offline}$'

        #Defining barrel and endacp regions on the plot
        barrel_range = (-1.305, 1.305)  
        endcap_range = (1.479, 2.1)  
        endcap_range_negative = (-1.479, -2.1)  

        ax.axvspan(barrel_range[0], barrel_range[1], color='lightblue', alpha=0.3)
        ax.axvspan(endcap_range[0], endcap_range[1], color='lightgreen', alpha=0.3)
        ax.axvspan(endcap_range_negative[0], endcap_range_negative[1], color='lightgreen', alpha=0.3)
        
        ax.axvline(barrel_range[1], color='lightblue', linestyle='--')
        ax.axvline(barrel_range[0], color='lightblue', linestyle='--')
        ax.axvline(endcap_range[0], color='lightgreen', linestyle='--')
        ax.axvline(endcap_range_negative[0], color='lightgreen', linestyle='--')

        # leg = plt.legend(loc = 'lower right', fontsize=18)
        # Create the legend with custom positioning and font size
        leg = plt.legend(loc='center', bbox_to_anchor=(0.5, 0.2), fontsize=18, frameon=True)
        # pt_leg = ax.legend(title= r"Gen Matched MC25W cut on", loc='upper center', bbox_to_anchor=(0.5, 0.95), fontsize=18, frameon=True)

        # Set a white background for the legend
        leg.get_frame().set_facecolor('white')

        # Optionally, add a border around the legend (if desired)
        leg.get_frame().set_edgecolor('black')
        # leg._legend_box.align = "left"
        # plt.ylim(y_lim)
        plt.xlim(x_lim)
        plt.xlabel(x_label)
        plt.ylabel(r'Energy resolution')
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')


def create_and_save_plot(single_bin, label_1, in_file_1, plot_tag,color_1,in_file_2="", label_2="", color_2=1, in_file_3="", label_3="", color_3=2, in_file_4="", label_4="", color_4=3,in_file_5="", label_5="", color_5=4, in_file_6="", label_6="", color_6=6):
    print('Producing plot for', single_bin)
    fig, ax = plt.subplots(figsize=(10, 10))
    # plot_pt_scale(in_file, label, 0, ax, single_bin)
    mean1, variance1=plot_pt_scale(in_file_1, label_1, color_1, ax, single_bin)
    mean2, variance2=plot_pt_scale(in_file_2, label_2, color_2, ax, single_bin)
    mean3, variance3 = plot_pt_scale(in_file_3, label_3, color_3, ax, single_bin)
    mean4, variance4 = plot_pt_scale(in_file_4, label_4, color_4, ax, single_bin)
    mean5, variance5 = plot_pt_scale(in_file_5, label_5, color_5, ax, single_bin)
    mean6, variance6 = plot_pt_scale(in_file_6, label_6, color_6, ax, single_bin)
    plot_name = f'responses/Run3_2025_monitoring/tau_pt_scale_{plot_tag}_{single_bin}'
    print(plot_name+'.png')
    plt.savefig(f'{plot_name}.pdf')
    plt.savefig(f'{plot_name}.png')
    # plot_pt_resolution(in_file, label, 0, ax, single_bin)
    # plot_name = f'responses/2024/tau_pt_resolution_{plot_tag}_{single_bin}'
    # plt.savefig(f'{plot_name}.pdf')
    # plt.savefig(f'{plot_name}.png')
    # plt.close()

    plt.clf()
    return mean1, variance1, mean2, variance2, mean3, variance3, mean4, variance4, mean5, variance5, mean6, variance6
    # return mean, variance
 

#######################################################################
######################### SCRIPT BODY #################################
#######################################################################

''' python3 Response_fitter.py --inFile1 resolutions_of_Rungen_matched_MC25_unpacked.root --inFile2 resolutions_of_RunMC25_unpacked.root --tag matched_vs_nonmatched_MC25 '''
''' python3 Response_fitter.py --inFile1 resolutions_of_RunMC25_VBF_GluGlu_below30_unpacked.root --inFile2 "" --tag MC25_below30_unpacked --bins --inclusive '''
''' python3 Response_fitter.py --inFile1 resolutions_of_Rungen_matched_MC25_gen_cut_20_unpacked.root --inFile2 resolutions_of_Rungen_matched_MC25_gen_cut_40_unpacked.root --inFile3 resolutions_of_Rungen_matched_MC25_gen_cut_60_unpacked.root --tag MC25_cut_on_gen_pt --bins --inclusive '''
''' python3 Response_fitter.py --inFile1 resolutions_of_Rungen_matched_MC25_gen_cut_20_unpacked.root --inFile2 resolutions_of_Rungen_matched_MC25_gen_cut_40_unpacked.root --inFile3 resolutions_of_Rungen_matched_MC25_gen_cut_60_unpacked.root --tag MC25_cut_on_gen_pt --bins --inclusive '''
''' python3 Response_fitter.py --inFile1 resolutions_of_Rungen_matched_MC25_gen_eta_barrel_unpacked.root --inFile2 resolutions_of_Rungen_matched_MC25_gen_eta_endcap_unpacked.root --tag MC25_cut_on_gen_eta --bins --inclusive '''
''' python3 Response_fitter.py --inFile1 resolutions_of_RunMC25_no_gen_match_unpacked.root --inFile2 resolutions_of_Runl1_to_gen_matching_MC25_unpacked.root --tag MC25_no_gen_match --bins --inclusive '''
''' python3 Response_fitter.py --inFile1 resolutions_of_RunMC25_CompleteProcesses_unpacked.root --inFile2 "" --tag MC25_CompleteProcesses '''
''' python3 Response_fitter.py --inFile1 resolutions_of_RunMC25W_conservative_GluGlu_unpacked.root --inFile2 resolutions_of_RunMC25W_conservative_VBF_unpacked.root --inFile3 resolutions_of_RunMC25W_conservative_DY_unpacked.root --tag MC25_Processes_comparisons_new_production '''

''' python3 Response_fitter.py --inFile1 resolutions_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_reEmulated_calibrated.root --inFile2 resolutions_of_RunRe-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_unpacked.root --tag New_2025_LUTfor_data_2024I '''
''' python3 Response_fitter.py --inFile1 resolutions_of_RunHCALcFeb-caloParams_2025_conservative-ZS-MC25W_test_CALIBRATED_500_trees_reEmulated.root --inFile2 resolutions_of_RunHCALcFeb-caloParams_2025_conservative-ZS-MC25W_test_CALIBRATED_reEmulated.root --tag MC25W_20GeV_Tree_comp '''
''' python3 Response_fitter.py --inFile1 resolutions_of_RunHCALcFeb-caloParams_2025_conservative-ZS-MC25W_CALIBRATED_30GeV_reEmulated_test.root --inFile2 resolutions_of_RunHCALcFeb-caloParams_2025_conservative-ZS-MC25W_test_target_CALIBRATED_30GeV_reEmulated_test.root --tag MC25W_diff_training_comparison_30GeV '''
''' python3 Response_fitter.py --inFile1 resolutions_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_30GeV_reEmulated_calibrated.root --inFile2 resolutions_of_RunRe-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_30GeV_unpacked.root --tag L1taupt_gt_10_vs_unpacked '''
''' python3 Response_fitter.py --inFile1 resolutions_of_RunHCALcFeb-caloParams_2025_conservative-ZS-MC25W_test_CALIBRATED_500_trees_reEmulated.root --inFile2 resolutions_of_RunHCALcFeb-caloParams_2025_conservative-ZS-MC25W_mean_fit_1_target_reEmulated_test.root --tag L1taupt_gt_10_vs_unpacked '''
''' python3 Response_fitter.py --inFile1 resolutions_of_RunHCALcFeb-caloParams_2025_conservative-ZS-MC25W_test_CALIBRATED_reEmulated.root --inFile2 resolutions_of_Runinclusive_correction_reEmulated_test.root --tag L1taupt_gt_10_vs_unpacked '''
''' python3 Response_fitter.py --inFile1 resolutions_of_RunRe-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_corrected_resolution_unpacked.root --inFile2 resolutions_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_reEmulated_calibrated.root --inFile3 resolutions_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_corrected_resolution_reEmulated_calibrated.root  --tag New_corrected_responses_2025_LUTfor_data_2024I '''
''' python3 Response_fitter.py --inFile1 resolutions_of_Rununpacked_20GeV_unpacked.root --inFile2 resolutions_of_uncorrected_20GeV_reEmulated_calibrated.root --inFile3 resolutions_of_uncorrected_20GeV_reEmulated_calibrated.root  --tag 20GeV_new_corrected_responses_2025_LUTfor_data_2024I '''
''' python3 Response_fitter.py --inFile1 resolutions_of_Run2025B_run391884_2025C_unpacked.root 
                                --inFile2 resolutions_of_Run2024_unpacked_unpacked.root
                                --inFile3 resolutions_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p7_eMin16_eMax31_corrected_resolution_reEmulated_calibrated.root 
                                --tag Unpacked_2025vs2024vsRemu '''
''' python3 Response_fitter.py --inFile1 resolutions_of_RunMuon_1_2025C_0p7_17_30_LLR_unpacked.root \
                                --inFile2 resolutions_of_Run2024_unpacked_unpacked.root \
                                --inFile3 resolutions_of_Muon_1_2025C_0p7_18_27_reEmulated_calibrated.root \
                                --inFile4 resolutions_of_Muon_1_2025C_0p7_17_30_LLR_reEmulated_calibrated.root \
                                --tag Unpacked_2025vs2024vsNewLUTs'''
''' python3 Response_fitter.py --inFile1 resolutions_of_Run2025B_run391884_2025C_unpacked.root --inFile2 resolutions_of_Run2024_unpacked_unpacked.root --inFile3 resolutions_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p7_eMin16_eMax31_corrected_resolution_reEmulated_calibrated.root --tag Unpacked_2025vs2024vsRemu '''
''' python3 Response_fitter.py --inFile1 resolutions_of_RunMuon_1_2025C_0p7_17_30_LLR_unpacked.root \
                                --inFile2 resolutions_of_Run2024_unpacked_unpacked.root \
                                --inFile3 resolutions_of_Runcrab_unpacked_2025_eraD_unpacked.root\
                                --inFile4 resolutions_of_crab_unpacked_2025_eraD_withremufile_reEmulated_calibrated.root \
                                --tag Monitoring_Run3_2025'''
''' python3 Response_fitter.py --inFile1 resolutions_of_Run2025B_run391884_2025C_unpacked.root --inFile2 resolutions_of_Run2024_unpacked_unpacked.root --inFile3 resolutions_of_Runcrab_unpacked_2025_eraD_unpacked.root --inFile4 resolutions_of_unpacked_2025_eraE_unpacked.root --inFile5 resolutions_of_crab_unpacked_2025_eraF_unpacked.root --InFile6 resolutions_of_crab_unpacked_2025_eraG_unpacked.root --tag Monitoring_Run3_2025_erasGFEDCB'''
''' python3 Response_fitter.py --inFile1 resolutions_of_2026_HcalNZS_0_unpacked.root --inFile2 resolutions_of_2026_HcalNZS_3_unpacked.root --tag HcalNZS_Run3_2026'''
''' python3 Response_fitter.py --inFile1 resolutions_of_crab_HCALNZS_C_D_E_G_0_reEmulated_calibrated.root --inFile2 resolutions_of_crab_HCALNZS_C_D_E_G_3_reEmulated_calibrated.root --tag HcalNZS_Run3_2026_C_D_E_G'''
''' python3 Response_fitter.py --inFile1 resolutions_of_2025_all_eras_unpacked.root --inFile2 resolutions_of_2025_all_eras_unpacked.root --tag PU_10 cut'''
''' python3 Response_fitter.py --inFile1 resolutions_of_Run2026_eraB_miniAOD_unpacked.root --inFile2 resolutions_of_crab_unpacked_2025_eraG_unpacked.root --tag EraB_2026'''
''' python3 Response_fitter.py --inFile1 resolutions_of_Run2026_eraB_miniAOD_unpacked.root --inFile2 resolutions_of_crab_unpacked_2025_eraG_unpacked.root --inFile3 resolutions_of_Run2026_eraB_miniAOD_new_unpacked.root --tag EraBnew_2026'''
''' python3 Response_fitter.py --inFile1 resolutions_of_Run2026_eraC_miniAOD_unpacked.root --inFile2 resolutions_of_crab_unpacked_2025_eraG_unpacked.root --tag EraC_2026'''

if __name__ == "__main__" :
    parser = OptionParser()
    parser = OptionParser()
    parser.add_option("--inFile1",   dest="inFile1",                        default=None)
    parser.add_option("--inFile2",   dest="inFile2",                        default=None)
    parser.add_option("--inFile3",   dest="inFile3",                        default=None)
    parser.add_option("--inFile4",   dest="inFile4",                        default=None)
    parser.add_option("--inFile5",   dest="inFile5",                        default=None)
    parser.add_option("--inFile6",   dest="inFile6",                        default=None)
    parser.add_option("--tag",       dest="tag",                            default=None)
    parser.add_option("--bins", dest="bins", action="store_true", default=False)
    parser.add_option("--inclusive", dest="inclusive", action='store_true', default=False)
    (options, args) = parser.parse_args()
    print(options)

    # main_folder = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2025/'
    # main_folder2 = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2024/'
    # main_folder_2 = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2025/gen_studies/'
    main_folder = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_Run3_2025/'
    # main_folder = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_DPNOTE_Run3_2025/'
    main_folder_2026 = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2026/'
    inFile1 = ROOT.TFile(main_folder_2026+options.inFile1)
    inFile2 = ROOT.TFile(main_folder+options.inFile2)
    # inFile3 = ROOT.TFile(main_folder_2026+options.inFile3)
    # inFile4 = ROOT.TFile(main_folder+options.inFile4)
    # inFile5 = ROOT.TFile(main_folder+options.inFile5)
    # inFile6 = ROOT.TFile(main_folder+options.inFile6)

    # label1 = r'Unpacked 2024 EraH'
    # label2 = r'Re-Emu 2024H w/ 24W opt'
    # label1 = r'2024 MC ReEmu'+ "\n" + "w/ LUTs 2023"
    # label2 = r'Muon_Run2024I_MINIAOD'
    # label1 =  r"GluGlu-2025-MC-MINIAOD" 
    # label2 =  r"VBF-2025-MC-MINIAOD" 
    # label3 =  r"DY-2025-MC-MINIAOD" 
    # label1 =  r"MC-2025W-MINIAOD" 
    # label2 = "taking taupt > 30 "
    # label3 = r'Re-Emu pedestals + corrections'
    # label1= r"$p_t^{gen} > 20$ GeV"
    # label2= r"$p_t^{gen} > 40$ GeV"
    # label3= r"$p_t^{gen} > 60$ GeV"
    # label1= r"$barrel\ |\eta^{gen}|<1.305$"
    # label2= r"$endcap\ 1.479<|\eta^{gen}|<2.1$"
    # label1= r"2024 Era I ReEmu"+ "\n" + "w/ 2025 conditions gt10"
    # label1= r"Unpacked 2025 Eras B&C"
    # label2= r"Unpacked 2024 Era I"
    # label3= r"Unpacked 2024 Era D"
    # label4= r"Unpacked 2025 Era E"
    # label5= r"Unpacked 2025 Era F"
    # label6= r"Unpacked 2025 Era G"
    # label4= r"Unpacked 2025 Era D with the other file"
    # label3= '2024 Era I ReEmu w/' + "\n" + 'Iso LUT 0p7 26 GeV 44 GeV' 
    # label4= '2024 Era I ReEmu w/' + "\n" + 'Iso LUT 0p7 25 GeV 60 GeV'
    # label1= r"MC25W w/ BDT calibration"
    # label2= r"MC25W w/ corrected" + "\n" + "BDT calibration"
    # label1= r"Unpacked 2024 Era I"
    # label2= r"2024 Era I ReEmu"+ "\n" + "w/ 2025 conditions"
    # label3= r"2024 Era I ReEmu"+ "\n" + "w/ 2025 conditions" + "\n" + "corrected BDT response"
    # label1=r"Re-emu 2025 HcalNZS (C,D,E,G) - parameter 0"
    # label2=r"Re-emu 2025 HcalNZS (C,D,E,G) - parameter 3"
    # label1=r"Unpacked 2025"
    # label2=r"Unpacked 2025 for PU $\in [0,10]$"
    label1=r"Unpacked 2026 Era B"
    label2=r"Unpacked 2025 Era G"
    # label3=r"Unpacked 2026 Era B New"

    # PLOT PT SCALE

    fig, ax = plt.subplots(figsize=(10,10))
    plot_pt_scale(inFile1, label1, 1, ax, 'pt_response_ptInclusive') # , 'pt_barrel_resp_ptInclusive')
    plot_pt_scale(inFile2, label2, 0, ax, 'pt_response_ptInclusive') # , 'pt_barrel_resp_ptInclusive')
    # plot_pt_scale(inFile3, label3, 2, ax, 'pt_response_ptInclusive') # , 'pt_barrel_resp_ptInclusive')
    # plot_pt_scale(inFile4, label4, 3, ax, 'pt_endcap_resp_ptInclusive') # , 'pt_barrel_resp_ptInclusive')
    # plot_pt_scale(inFile5, label5, 4, ax, 'pt_endcap_resp_ptInclusive') # , 'pt_barrel_resp_ptInclusive')
    # plot_pt_scale(inFile6, label6, 6, ax, 'pt_endcap_resp_ptInclusive') # , 'pt_barrel_resp_ptInclusive')
    # plot_pt_scale(inFile2, label2, 1, ax, "pt_endcap_resp_ptInclusive") # , 'pt_barrel_resp_ptInclusive')
    # plot_pt_scale(inFile2, label2, 1, ax) # , 'pt_endcap_resp_ptInclusive')
    # plot_pt_scale(inFile3, label3, 2, ax)

    plot_name = 'responses/Run3-2026-monitoring/tau_pt_scale_'+options.tag
    print(plot_name+'.png') 
    if options.inclusive: plot_name = plot_name[:-4] + '_inclusive'
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()

    # PLOT PT RESOLUTION

    # First Plot: Plot pt_resolution with 'pt_resol_fctPt'
    fig1, ax1 = plt.subplots(figsize=(10, 10))
    plot_pt_resolution(inFile1, label1, 1, ax1, "pt_resol_fctPt")
    plot_pt_resolution(inFile2, label2, 0, ax1, "pt_resol_fctPt")
    # plot_pt_resolution(inFile3, label3, 2, ax1, "pt_resol_fctPt")
    # plot_pt_resolution(inFile4, label4, 3, ax1, "pt_resol_barrel_fctPt")
    # plot_pt_resolution(inFile5, label5, 4, ax1, "pt_resol_barrel_fctPt")
    # plot_pt_resolution(inFile6, label6, 6, ax1, "pt_resol_barrel_fctPt")
    # plot_pt_resolution(inFile1, label2, 1, ax1, "pt_resol_endcap_fctPt")
    # plot_pt_resolution(inFile2, label2, 1, ax1 ,"pt_resol_fctPt") 
    # plot_pt_resolution(inFile3, label3, 2, ax1 ,"pt_resol_fctPt") 
    plot_name1 = 'responses/Run3-2026-monitoring/tau_pt_resolution_' + options.tag + '_fctPt'
    print(f"Saving {plot_name1}.png")
    ax1.grid()
    plt.savefig(plot_name1+'.pdf')
    plt.savefig(plot_name1+'.png')
    plt.close()
    # if options.inclusive:
    #     plot_name1 = plot_name1[:-4] + '_inclusive'
    


    fig3, ax3 = plt.subplots(figsize=(10, 10))
    plot_pt_resolution(inFile1, label1, 1, ax3, "pt_scale_fctPt","pt_scale")
    plot_pt_resolution(inFile2, label2, 0, ax3, "pt_scale_fctPt","pt_scale")
    # plot_pt_resolution(inFile3, label3, 2, ax3, "pt_scale_fctPt","pt_scale")
    # plot_pt_resolution(inFile4, label4, 3, ax3, "pt_scale_fctPt","pt_scale")
    # plot_pt_resolution(inFile5, label5, 4, ax3, "pt_scale_fctPt","pt_scale")
    # plot_pt_resolution(inFile6, label6, 6, ax3, "pt_scale_fctPt","pt_scale")
    # plot_pt_resolution(inFile1, label2, 1, ax1, "pt_resol_endcap_fctPt")
    # plot_pt_resolution(inFile2, label2, 1, ax1 ,"pt_resol_fctPt") 
    # plot_pt_resolution(inFile3, label3, 2, ax3 ,"pt_resol_fctPt") 
    plot_name1 = 'responses/Run3-2026-monitoring/tau_pt_scale_' + options.tag + '_fctPt'
    print(f"Saving {plot_name1}.png")
    plt.savefig(plot_name1 + '.pdf')
    plt.savefig(plot_name1 + '.png')
    plt.close()
    ax3.grid()


    # # # Second Plot: Plot pt_resolution with 'pt_resol_fctEta'
    fig2, ax2 = plt.subplots(figsize=(10, 10))
    plot_pt_resolution(inFile1, label1, 1, ax2, 'pt_resol_fctEta', "eta")  # Plot for fctEta
    plot_pt_resolution(inFile2, label2, 0, ax2, 'pt_resol_fctEta', "eta")  # Plot for fctEta
    # plot_pt_resolution(inFile3, label3, 2, ax2, 'pt_resol_fctEta', "eta")  # Plot for fctEta
    # plot_pt_resolution(inFile3, label3, 2, ax2, 'pt_resol_fctEta', "eta")  # Plot for fctEta
    # plot_pt_resolution(inFile4, label4, 3, ax2, 'pt_resol_fctEta', "eta")  # Plot for fctEta
    # plot_pt_resolution(inFile5, label5, 4, ax2, 'pt_resol_fctEta', "eta")  # Plot for fctEta
    # plot_pt_resolution(inFile6, label6, 6, ax2, 'pt_resol_fctEta', "eta")  # Plot for fctEta
    plot_name2 = 'responses/Run3-2026-monitoring/tau_pt_resolution_' + options.tag + '_fctEta'
    plt.savefig(plot_name2 + '.pdf')
    plt.savefig(plot_name2 + '.png')
    print(f"Saving {plot_name2}.png")
    ax2.grid()

    # if options.inclusive:
    #     plot_name2 = plot_name2[:-4] + '_inclusive'
    # plt.savefig(plot_name2 + '.pdf')
    # plt.savefig(plot_name2 + '.png')
    # plt.close()

    if options.bins:
        # bins_to_process = single_pt_bins + single_eta_bins
        mean_pt=[]
        mean_pt_2=[]
        mean_eta=[]
        variance_pt=[]
        variance_pt_2=[]
        variance_eta=[]
        mean_pt_3=[]
        variance_pt_3=[]
        mean_pt_4=[]
        variance_pt_4=[]
        mean_pt_5=[]
        variance_pt_5=[]
        mean_pt_6=[]
        variance_pt_6=[]

        for single_bin in single_pt_bins:
            # label_1_bins = "MC25W-MINIAOD\n" + \
            #     "caloparams 2023\n" + \
            #    f"{single_bin.split('pt_resp_')[-1]}"
            # label_1_bins = r"2024 Era I ReEmu"+ "\n" + "w/ 2025 conditions\n" + \
            #    f"{single_bin.split('pt_resp_')[-1]}"
            # label_2_bins = r"Unpacked 2024 Era I" + "\n" + f"{single_bin.split('pt_resp_')[-1]}"
            label_1_bins = label1 + "\n" + \
               f"{single_bin.split('pt_resp_')[-1]}"
            label_2_bins =  label2 + "\n" + f"{single_bin.split('pt_resp_')[-1]}"
            label_3_bins =  label3 + "\n" + f"{single_bin.split('pt_resp_')[-1]}"
            label_4_bins =  label4 + "\n" + f"{single_bin.split('pt_resp_')[-1]}"
            label_5_bins =  label5 + "\n" + f"{single_bin.split('pt_resp_')[-1]}"
            label_6_bins =  label6 + "\n" + f"{single_bin.split('pt_resp_')[-1]}"
            # label1 = f'Muon Run2024I MINIAOD_{single_bin.split("pt_resp_")[-1]}'
            # create_and_save_plot(single_bin, label_1_bins, inFile1, options.tag)
            mean_i,variance_i,mean_i2,variance_i2, mean_i3, variance_i3, mean_i4, variance_i4, mean_i5, variance_i5, mean_i6, variance_i6=create_and_save_plot(single_bin, label_1_bins, inFile1, options.tag, 1, inFile2,label_2_bins, 0, inFile3,label_3_bins, 2, inFile4,label_4_bins, 3, inFile5,label_5_bins,4,inFile6,label_6_bins,6)
            # mean_i,variance_i,mean_i2,variance_i2, mean_i3, variance_i3=create_and_save_plot(single_bin, label_1_bins, inFile1, options.tag, 1, inFile2,label_2_bins, 0, inFile3,label_3_bins, 2)
            mean_pt.append(mean_i)
            variance_pt.append(variance_i)
            mean_pt_2.append(mean_i2)
            variance_pt_2.append(variance_i2)
            mean_pt_3.append(mean_i3)
            variance_pt_3.append(variance_i3)
            mean_pt_4.append(mean_i4)
            variance_pt_4.append(variance_i4)
            mean_pt_5.append(mean_i5)
            variance_pt_5.append(variance_i5)
            mean_pt_6.append(mean_i6)
            variance_pt_6.append(variance_i6)


        # for single_bin in single_eta_bins:
        #     # label_1_bins = "MC25W-MINIAOD\n" + \
        #     #     "caloparams 2023\n" + \
        #     #    f"{single_bin.split('pt_resp_')[-1]}"
        #     # label_1_bins = r"2024 Era I ReEmu"+ "\n" + "w/ 2025 conditions\n" + \
        #     #    f"{single_bin.split('pt_resp_')[-1]}"
        #     # label_2_bins = r"Unpacked 2024 Era I" + "\n" + f"{single_bin.split('pt_resp_')[-1]}"
        #     label_1_bins = label1 + "\n" + \
        #        f"{single_bin.split('pt_resp_')[-1]}"
        #     label_2_bins =  label2 + "\n" + f"{single_bin.split('pt_resp_')[-1]}"
        #     # label1 = f'Muon Run2024I MINIAOD_{single_bin.split("pt_resp_")[-1]}'
        #     # create_and_save_plot(single_bin, label_1_bins, inFile1, options.tag)
        #     mean_i,variance_i,mean_i2,variance_i2=create_and_save_plot(single_bin, label_1_bins, inFile1, options.tag, 0, inFile2,label_2_bins, 1)
        #     mean_pt.append(mean_i)
        #     variance_pt.append(variance_i)
        #     mean_pt_2.append(mean_i2)
        #     variance_pt_2.append(variance_i2)

        # print("mean1",len(mean_pt))
        # # print("mean",mean_pt)
        # # print(len(variance_pt))
        # print("mean2",len(mean_pt_2))
        # print("mean3",len(mean_pt_3))
        # print("mean2",mean_pt_2)
        # print(len(variance_pt_2))
        # print((pt_bin_centers))

        # print("variance",variance_pt)

        # for single_bin in single_eta_bins:
        #     # label_1_bins = "2025W-MC-VBF-MINIAOD\n" + \
        #     #     "caloparams 2023\n" + \
        #     #    f"{single_bin.split('pt_resp_')[-1]}"
        #     label_1_bins = "2025W-MC-VBF-MINIAOD\n" + \
        #         f"{single_bin.split('pt_resp_')[-1]}"
        #     # label1 = f'Muon Run2024I MINIAOD_{single_bin.split("pt_resp_")[-1]}'
        #     # create_and_save_plot(single_bin, label_1_bins, inFile1, options.tag)
        #     mean_i,variance_i=create_and_save_plot(single_bin, label_1_bins, inFile1, options.tag, 0)
        #     mean_eta.append(mean_i)
        #     variance_eta.append(variance_i)

        # Plot the mean as a function of pt bin

        # plt.scatter(pt_bin_centers, mean_pt, color='orange', label='Mean')
        # plt.plot(pt_bin_centers, mean_pt, linestyle='--', color='orange', alpha=0.7)
        cmap = matplotlib.colormaps.get_cmap('Set1')
        plt.errorbar(pt_bin_centers, mean_pt, yerr=variance_pt, fmt='o', color=cmap(1), 
             ecolor=cmap(1), elinewidth=1.5, capsize=4,label=label1)
        plt.errorbar(pt_bin_centers, mean_pt_2, yerr=variance_pt_2, fmt='o', color=cmap(0), 
             ecolor=cmap(0), elinewidth=1.5, capsize=4, label=label2)
        plt.errorbar(pt_bin_centers, mean_pt_3, yerr=variance_pt_3, fmt='o', color=cmap(2), 
             ecolor=cmap(2), elinewidth=1.5, capsize=4, label=label3)
        plt.errorbar(pt_bin_centers, mean_pt_4, yerr=variance_pt_4, fmt='o', color=cmap(3), 
             ecolor=cmap(3), elinewidth=1.5, capsize=4, label=label4)
        plt.errorbar(pt_bin_centers, mean_pt_5, yerr=variance_pt_5, fmt='o', color=cmap(4), 
             ecolor=cmap(4), elinewidth=1.5, capsize=4, label=label5)
        plt.errorbar(pt_bin_centers, mean_pt_6, yerr=variance_pt_6, fmt='o', color=cmap(6), 
             ecolor=(6), elinewidth=1.5, capsize=4, label=label6)
        plt.xlabel(r'$p_t^{offfline}$')
        plt.ylabel(r'$\langle E_{T}^{\tau, L1}/p_{T}^{\tau, offline}\rangle_{fit}$')
        plt.legend(fontsize=18)
        plt.grid(True, linestyle=':', alpha=0.6)
        for xtick in ax.xaxis.get_major_ticks():
            xtick.set_pad(10)
        mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
        plot_name = f'responses/Run3-2026-monitoring/tau_mean_pt_scale_{options.tag}'
        plt.savefig(f'{plot_name}.pdf')
        plt.savefig(f'{plot_name}.png')
        print(f'{plot_name}.png')
        plt.close()

        # Plot the mean as a function of eta bin

        # plt.scatter(pt_bin_centers, mean_pt, color='blue', label='Mean')
        # plt.plot(pt_bin_centers, mean_pt, linestyle='--', color='blue', alpha=0.7)
        # plt.errorbar(eta_bin_centers, mean_eta, yerr=variance_eta, fmt='o', color='orange', 
        #      ecolor='orange', elinewidth=1.5, capsize=4)
        # plt.xlabel(r'$|\eta|^{\tau, offline}$')
        # plt.ylabel(r"$\langle E_{T}^{\tau, L1}/p_{T}^{\tau, offline}\rangle$")
        # plt.legend()
        # plt.grid(True, linestyle=':', alpha=0.6)
        # for xtick in ax.xaxis.get_major_ticks():
        #     xtick.set_pad(10)
        # mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
        # plot_name = f'responses/Run3_2025_monitoring/tau_mean_eta_scale_{options.tag}'
        # plt.savefig(f'{plot_name}.pdf')
        # plt.savefig(f'{plot_name}.png')
        # plt.close()

        # Handle inclusive plot saving
        if options.inclusive:
            plot_name = f'responses/Run3-2026-monitoring/tau_pt_scale_{options.tag}_{single_bin}_inclusive'
            plt.savefig(f'{plot_name}.pdf')
            plt.savefig(f'{plot_name}.png')
            plt.close()

