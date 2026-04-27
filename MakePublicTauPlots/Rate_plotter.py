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
    ''' python3 Rate_plotter.py --inFile1 histos_rate_ZeroBias_Run386604_MC_conservative_ZS_unpacked.root --inFile2 histos_rate_ZeroBias_Run386604_MC_conservative_ZS_unpacked.root --tag 2023LUTs_2024conditions_HCAL_new_calib '''
    
    
    ''' python3 Rate_plotter.py --inFile1 histos_rate_ZeroBias_Run386604_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_reEmulated.root --inFile2 histos_rate_ZeroBias_Run386604_Zero_bias_noHCALc_2024calop_unpacked.root  --tag 2024I_data_2025calopHCAlc_vs_2024calopnHCALc '''
    ''' python3 Rate_plotter.py --inFile1 histos_rate_ZeroBias_Run386604_HCALcFeb-caloParams_2025_conservative-ZS-MC25W_target_unpacked.root --inFile2 histos_rate_ZeroBias_Run386604_HCALcFeb-caloParams_2025_conservative-ZS-MC25W_target_optimisation.root  --tag Different_training_rates'''
    ''' python3 Rate_plotter.py --inFile1 histos_rate_ZeroBias_Run386604_HCALcFeb-caloParams_2025_conservative-ZS-MC25W_optimisation.root --inFile2 histos_rate_ZeroBias_Run386604_HCALcFeb-caloParams_2025_conservative-ZS-MC25W_corrected_optimisation.root  --tag Corrected_post_BDT_response'''
    ''' python3 Rate_plotter.py --inFile1 histos_rate_ZeroBias_Run386604_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_unpacked.root --inFile2 histos_rate_ZeroBias_Run386604_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_reEmulated.root --inFile3 histos_rate_ZeroBias_Run386604_Corrected-re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_reEmulated.root --tag Re-emu-comparison-correction'''


    ''' python3 Rate_plotter.py --inFile1 histos_rate_ZeroBias_Run386604_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_unpacked.root 
    --inFile2  histos_rate_ZeroBias_Run386604_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p7_eMin16_eMax31_corrected_resolution_reEmulated.root 
    --inFile3  histos_rate_ZeroBias_Run386604_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin19_eMax34_corrected_resolution_reEmulated.root 
    --inFile4  histos_rate_ZeroBias_Run386604_Corrected-re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_reEmulated.root --tag Comparison_Isolation_correction'''
    
    

    ''' python3 Rate_plotter.py --inFile1 histos_rate_ZeroBias_Run392295_TAUReEmul_v4_unpacked.root \
                                --inFile2 histos_rate_ZeroBias_Run392295_ZeroBias_2025C_392295_0p7_12_26_reEmulated.root \
                                --inFile3 histos_rate_ZeroBias_Run386604_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_unpacked.root \
                                --inFile4 histos_rate_ZeroBias_Run392295_ZeroBias_2025C_392295_0p6_11_30_reEmulated.root \
                                --inFile5 histos_rate_ZeroBias_Run392295_crab_ZeroBiasrun3_2025C_392295_0p7_17_30_reEmulated.root \
                                --inFile6 histos_rate_ZeroBias_Run392295_Muon_1_2025C_0p7_18_27_reEmulated.root \
                                --tag 2025C-tighterIsoLUT-2025-final-comparisons '''
    
    ''' python3 Rate_plotter.py --inFile1 histos_rate_ZeroBias_Run392295_TAUReEmul_v4_unpacked.root \
                                --inFile2 histos_rate_ZeroBias_Run386604_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_unpacked.root \
                                --inFile3 histos_rate_ZeroBias_Run392295_crab_ZeroBiasrun3_2025C_392295_0p7_17_30_reEmulated.root \
                                --inFile4 histos_rate_ZeroBias_Run392295_Muon_1_2025C_0p7_18_27_reEmulated.root \
                                --tag 2025C-tighterIsoLUT-2025-final-comparisons-f '''
    
    ''' python3 Rate_plotter.py --inFile1 histos_rate_ZeroBias_Run392295_TAUReEmul_v4_unpacked.root \
                                --inFile2 histos_rate_ZeroBias_Run386604_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_unpacked.root \
                                --inFile3 histos_rate_ZeroBias_Run394959_crab_unpacked_2025_eraD_unpacked.root \
                                --tag Monitoring_Run3_2025D '''
    
    ''' python3 Rate_plotter.py --inFile1 histos_rate_ZeroBias_Run392295_TAUReEmul_v4_unpacked.root \
                                --inFile2 histos_rate_ZeroBias_Run386604_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_unpacked.root \
                                --inFile3 histos_rate_ZeroBias_Run394959_crab_unpacked_2025_eraD_unpacked.root \
                                --tag Monitoring_Run3_2025D '''
                                
    ''' python3 Rate_plotter.py --inFile1 histos_rate_ZeroBias_Run392295_TAUReEmul_v4_unpacked.root \
                                --inFile2 histos_rate_ZeroBias_Run386604_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_unpacked.root \
                                --inFile3 histos_rate_ZeroBias_Run394959_crab_unpacked_2025_eraD_unpacked.root \
                                --tag Monitoring_Run3_2025D '''



    parser = OptionParser()
    parser.add_option("--inFile1", dest="inFile1", default=None)
    parser.add_option("--inFile2", dest="inFile2", default=None)
    parser.add_option("--inFile3", dest="inFile3", default=None)
    parser.add_option("--inFile4", dest="inFile4", default=None)
    parser.add_option("--inFile5", dest="inFile5", default=None)
    parser.add_option("--inFile6", dest="inFile6", default=None)
    parser.add_option("--tag",     dest="tag",     default=None)
    (options, args) = parser.parse_args()
    print(options)

    main_folder = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/MakeRates/histos_2025/'
    main_folder2 = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/MakeRates/histos_2025/'
    inFile1 = ROOT.TFile(main_folder + options.inFile1)
    inFile2 = ROOT.TFile(main_folder + options.inFile2)
    inFile3 = ROOT.TFile(main_folder + options.inFile3)
    inFile4 = ROOT.TFile(main_folder + options.inFile4)
    # inFile5 = ROOT.TFile(main_folder + options.inFile5)
    # inFile6 = ROOT.TFile(main_folder + options.inFile6)

    plt.rcParams['legend.title_fontsize'] = 'xx-small'
    # cmap = matplotlib.colormaps.get_cmap('Set1'); imap=-1
    cmap = matplotlib.colormaps.get_cmap('Set1')
    markers = ['o', 's', '^', 'D']

    #DoubleTau = inFile.Get('DiTauRate_noIso')
    DoubleTau_Iso_1 = inFile1.Get('DiTauRate_Iso')
    DoubleTau_Iso_2 = inFile2.Get('DiTauRate_Iso')
    DoubleTau_Iso_3 = inFile3.Get('DiTauRate_Iso')
    DoubleTau_Iso_4 = inFile4.Get('DiTauRate_Iso')
    # DoubleTau_Iso_5 = inFile5.Get('DiTauRate_Iso')
    # DoubleTau_Iso_6 = inFile6.Get('DiTauRate_Iso')
    # DoubleTau_Iso_1 = inFile1.Get('DiTauRate_effMin0p9_eMin28_eMax43')
    # DoubleTau_Iso_2 = inFile2.Get('DiTauRate_effMin0p9_eMin28_eMax43')
    # DoubleTau_Iso_1 = inFile1.Get('DiTauRate_effMin0p9_eMin22_eMax37')
    # DoubleTau_Iso_2 = inFile2.Get('DiTauRate_effMin0p9_eMin22_eMax37')
    # DoubleTau_Iso_3 = inFile2.Get('DiTauRate_effMin0p9_eMin22_eMax37')

    print(DoubleTau_Iso_1.ClassName())
    plt.rcParams['legend.title_fontsize'] = 'xx-small'
    legend_title=r'Inclusive |$\eta$| < 2.1'
    # legend_title=r'Inclusive |$\eta$| < 2.1'+ "\n" + "2024 Era I ReEmu w/ 2025 conditions"
    #legend_title=r'Inst. Lumi = $3.5\times10^{33}\ cm^{-2}s^{-1}$'
    # label_DoubleIso_Iso_1 = r'No Iso'
    # label_DoubleIso_Iso_2 = r'Iso'
    # label_DoubleIso_Iso_1= r"No Iso"
    # label_DoubleIso_Iso_2= r"Iso"
    # label_DoubleIso_Iso_1 = r'Double-$\tau$ & Iso' +"\n" + 'Unpacked 2024 Era I '
    # label_DoubleIso_Iso_2 = r'Double-$\tau$ & Iso' +"\n" + '2024 Era I ReEmu w/ 2025 conditions'
    # label_DoubleIso_Iso_3 = r'Double-$\tau$ & Iso' +"\n" + '2024 Era I ReEmu w/ 2025 conditions ' + "\n" + 'and corrected BDT response'
    label_DoubleIso_Iso_1 = r'Double-$\tau$ & Iso' +"\n" + 'Unpacked 2025 Era C'
    label_DoubleIso_Iso_2 = r'Double-$\tau$ & Iso' +"\n" + 'Unpacked 2024 Era I' 
    # label_DoubleIso_Iso_3 = r'Double-$\tau$ & Iso' +"\n" + 'Unpacked 2025 Era D'
    # label_DoubleIso_Iso_4 = r'Double-$\tau$ & Iso' +"\n" + 'Re-emu 2025C w/ Iso params'  +"\n" +r'$\epsilon_{min}$ 0p6, $E_{T}^{min}$ 18 GeV, $E_{T}^{max}$ 60 GeV' 
    label_DoubleIso_Iso_3 = r'Double-$\tau$ & Iso' +"\n" + 'Re-emu 2025C w/ Iso params'  +"\n" +r'$\epsilon_{min}$ 0p7, $E_{T}^{min}$ 25 GeV, $E_{T}^{max}$ 60 GeV'
    label_DoubleIso_Iso_4 = r'Double-$\tau$ & Iso' +"\n" + 'Re-emu 2025C w/ Iso params'  +"\n" +r'$\epsilon_{min}$ 0p7, $E_{T}^{min}$ 26 GeV, $E_{T}^{max}$ 44 GeV'
    # label_DoubleIso_Iso_4 = r'Double-$\tau$ & Iso' +"\n" + '2024 Era I ReEmu w/ new Iso nTT and iEt binning' + "\n" + 'and corrected BDT response'

    # label_DoubleIso_Iso_1 = r'Double-$\tau$' +"\n" + r"MC25W w/ BDT calibration"
    # label_DoubleIso_Iso_2 = r'Double-$\tau$' +"\n" + r"MC25W w/ corrected" + "\n" + "BDT calibration"

    fig, ax = plt.subplots(figsize=(10,10))

    i=0
    for rate_TH1, label , color in zip([DoubleTau_Iso_1,DoubleTau_Iso_2, DoubleTau_Iso_3,DoubleTau_Iso_4], [label_DoubleIso_Iso_1,label_DoubleIso_Iso_2,label_DoubleIso_Iso_3,label_DoubleIso_Iso_4],[1,0,7,8]):
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
        #print elements 30 to 50 from the x and y lists
        print(i)
        i=i+1
        print("x",x[30:36])
        print("y",y[30:36])
        # if imap == -1: color = 'black'
        # else:         color = cmap(imap)
        # print(color)
        imap =-1
        ax.errorbar(x, y, xerr=x_err, yerr=y_err, ls='None', color=cmap(color), label=label, lw=2,  marker=markers[imap]) #, marker=markers[imap], color=color)
        imap += 1

    leg = plt.legend(loc='upper right', fontsize=12, title=legend_title)
    leg._legend_box.align = "left"
    plt.ylim(1,4E4)
    # ax.set_ylim(7, 30)
    # plt.xlim(32,38)
    plt.xlim(0,60)
    plt.yscale('log')
    ax.axhline(y=14, color='black', linestyle='--', linewidth=1)
    ax.yaxis.set_major_locator(FixedLocator([1, 10, 100, 1000, 10000]))
    ax.yaxis.set_major_formatter(FixedFormatter([r'1',r'10',r'$10^2$',r'$10^3$',r'$10^4$']))
    plt.xlabel(r'$E_{T}^{\tau, L1}\ [GeV]$')
    plt.ylabel(r'Rate[kHz]')
    plt.grid()
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    #mplhep.cms.label('Preliminary', data=True, rlabel=r'107 pb$^{-1}$ (13.6 TeV)')
    mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
    plt.savefig('rates/tau_rate_'+options.tag+'.pdf')
    plt.savefig('rates/tau_rate_'+options.tag+'.png')
    print('rates/tau_rate_'+options.tag+'.png')
    plt.close()
