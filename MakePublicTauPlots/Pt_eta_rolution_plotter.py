import ROOT
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import matplotlib.patches as patches
import mplhep
plt.style.use(mplhep.style.CMS)
import os


ptBins = [20,25,30,35,40,45,50,60,70,90,110]
etaBins=[0.000,0.500,1.000,1.305,1.479,1.800,2.100]

def compute_resolution_pt_for_etabins(inFile, low_eta, high_eta):
    # Open ROOT file
    file = ROOT.TFile(inFile, "READ")
    h2d = file.Get("PTvsETA_resolution")

    # Check if histogram exists
    if not h2d or not isinstance(h2d, ROOT.TH2):
        print("Error: Histogram 'PTvsETA' not found in the ROOT file!")
        exit()

    # Get bin indices for eta range
    bin_low = h2d.GetYaxis().FindBin(low_eta)
    bin_high = h2d.GetYaxis().FindBin(high_eta)

    n_bins = []
    bin_centers = []
    bin_contents = []
    bin_errors = []
    bin_width =[]

    # Project X-axis (pt) for the given eta range
    h1d_pt = h2d.ProjectionX(f"pt_eta_{low_eta}_to_{high_eta}", bin_low, bin_high)

    n_bins=h1d_pt.GetNbinsX()
    for i in range(n_bins):
        bin_centers.append(h1d_pt.GetBinCenter(i+1))  # Pt values
        bin_contents.append(h1d_pt.GetBinContent(i+1))  # Energy resolution values
        bin_errors.append(h1d_pt.GetBinError(i+1))  # Energy resolution errors
        bin_width.append(h1d_pt.GetBinWidth(i+1)/2)  # Half bin width
    return bin_centers, bin_contents, bin_errors, bin_width


def plot_resolution_pt_for_etabins(inFile,label,color,ax, low_eta, high_eta):
    bin_centers, bin_contents, bin_errors, bin_width = compute_resolution_pt_for_etabins(inFile, low_eta, high_eta)
    x_lim = (20.,110.)
    plt.rcParams['legend.title_fontsize'] = 'x-small'
    cmap = matplotlib.colormaps.get_cmap('Set1')
    # y_lim=(0.05,0.3)
    
    ax.errorbar(bin_centers, bin_contents, xerr=bin_width, yerr=bin_errors, 
                ls='None', label=label, lw=2, marker='o', color=cmap(color))
    
    leg = plt.legend(title= f"{low_eta} <" +r"$ \eta $" +f"< {high_eta}:",loc = 'upper right', fontsize=18)
    # leg = plt.legend(loc = 'upper right', fontsize=18)
    leg._legend_box.align = "left"
    # plt.ylim(y_lim)
    plt.xlim(x_lim)
    plt.xlabel(r"$p_T^{\tau,{offline}} [GeV]$")
    plt.ylabel(r'Energy resolution')
    plt.grid(linestyle=":")
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
    
def compute_resolution_eta_for_ptbins(inFile, low_pt, high_pt):
    # Open ROOT file
    file = ROOT.TFile(inFile, "READ")
    h2d = file.Get("PTvsETA_resolution")

    print("lowpt",low_pt)
    print("highpt",high_pt)
    # Check if histogram exists
    if not h2d or not isinstance(h2d, ROOT.TH2):
        print("Error: Histogram 'PTvsETA' not found in the ROOT file!")
        exit()

    # Get bin indices for eta range
    bin_low = h2d.GetXaxis().FindBin(low_pt)
    bin_high = h2d.GetXaxis().FindBin(high_pt)

    print("bin_low",bin_low)
    print("bin_high",bin_high)

    n_bins = []
    bin_centers = []
    bin_contents = []
    bin_errors = []
    bin_width =[]

    # Project X-axis (pt) for the given eta range
    h1d_pt = h2d.ProjectionY(f"eta_pt_{low_pt}_to_{high_pt}", bin_low, bin_high)

    n_bins=h1d_pt.GetNbinsX()

    for i in range(n_bins):
        bin_centers.append(h1d_pt.GetBinCenter(i+1))  # Pt values
        bin_contents.append(h1d_pt.GetBinContent(i+1))  # Energy resolution values
        bin_errors.append(h1d_pt.GetBinError(i+1))  # Energy resolution errors
        bin_width.append(h1d_pt.GetBinWidth(i+1)/2)  # Energy resolution errora

    return bin_centers, bin_contents, bin_errors, bin_width


def plot_resolution_eta_for_ptbins(inFile,label,color,ax, low_pt, high_pt):
    bin_centers2, bin_contents2, bin_errors2, bin_width2 = compute_resolution_eta_for_ptbins(inFile, low_pt, high_pt)
    print("bin_centers2",bin_centers2)
    print("bin_contents2",bin_contents2)
    x_lim=(-2.1,2.1)
    plt.rcParams['legend.title_fontsize'] = 'x-small'
    cmap = matplotlib.colormaps.get_cmap('Set1')
    y_lim=(0.2,0.55)
    
    ax.errorbar(bin_centers2, bin_contents2, xerr=bin_width2, yerr=bin_errors2, 
                ls='None', label=label, lw=2, marker='o', color=cmap(color))
    
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

    leg = plt.legend(title= f"{low_pt} <" +r"$p_T$" +f"< {high_pt}:", loc='upper right', bbox_to_anchor=(0.5, 0.95), fontsize=18, frameon=True)
    # plt.tight_layout()
    # leg = plt.legend(title= f"{low_pt} <" +r"$p_T$" +f"< {high_pt}:",loc = 'lower left', fontsize=18)
    # leg = plt.legend(loc = 'upper right', fontsize=18)
    leg._legend_box.align = "left"
    # plt.ylim(y_lim)
    plt.xlim(x_lim)
    plt.xlabel(r"$\eta^{\tau,{offline}}$")
    plt.ylabel(r'Energy resolution')
    plt.grid(linestyle=":")
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
    


#######################################################################
######################### SCRIPT BODY #################################
#######################################################################


file0 = "/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2025/resolutions_of_RunMC25W_conservative_VBF_unpacked.root"
file1 = "/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2025/resolutions_of_RunMC25W_conservative_GluGlu_unpacked.root"
file2 = "/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotCalibrationResolution/ROOTs/ROOTs_2025/resolutions_of_RunMC25W_conservative_DY_unpacked.root"


output_folder = "EtavsPttests"
single_eta_bins = [0,1,2,3,4,5]
single_pt_bins = [0,1,2,3,4,5,6,7,8,9]

for single_bin in single_eta_bins:
    # print(f"Processing eta bin: {single_bin}")
    fig, ax = plt.subplots(figsize=(10,10))
    low_eta=etaBins[single_bin]
    high_eta=etaBins[single_bin + 1]
    label_1_bins = "MiniAOD-MC25W-VBF" 
    label_2_bins = "MiniAOD-MC25W-GluGlu"
    label_3_bins = "MiniAOD-MC25W-DY"  
    plot_resolution_pt_for_etabins(file0, label_1_bins, 0 ,ax,low_eta, high_eta)
    plot_resolution_pt_for_etabins(file1, label_2_bins, 1 ,ax,low_eta, high_eta)
    plot_resolution_pt_for_etabins(file2, label_3_bins, 2 ,ax,low_eta, high_eta)
    
    # Save
    plot_path_png = f"{output_folder}/energy_resolution_eta_{low_eta}_to_{high_eta}.png"
    plot_path_pdf = f"{output_folder}/energy_resolution_eta_{low_eta}_to_{high_eta}.pdf"
    plt.savefig(plot_path_png)
    plt.savefig(plot_path_pdf)
    plt.close()

    print(f"Saved: {plot_path_png}") 

for single_bin in single_pt_bins:
    print(f"Processing pt bin: {single_bin}")
    fig, ax = plt.subplots(figsize=(10,10))
    low_pt=ptBins[single_bin]
    high_pt=ptBins[single_bin + 1]
    label_1_bins = "MiniAOD-MC25W-VBF" 
    label_2_bins = "MiniAOD-MC25W-GluGlu"
    label_3_bins = "MiniAOD-MC25W-DY"  
    plot_resolution_eta_for_ptbins(file0, label_1_bins, 0 ,ax,low_pt, high_pt)
    plot_resolution_eta_for_ptbins(file1, label_2_bins, 1 ,ax,low_pt, high_pt)
    plot_resolution_eta_for_ptbins(file2, label_3_bins, 2 ,ax,low_pt, high_pt)
    
    # Save
    plot_path_png = f"{output_folder}/energy_resolution_pt_{low_pt}_to_{high_pt}.png"
    plot_path_pdf = f"{output_folder}/energy_resolution_pt_{low_pt}_to_{high_pt}.pdf"
    plt.savefig(plot_path_png)
    plt.savefig(plot_path_pdf)
    plt.clf()

    print(f"Saved: {plot_path_png}") 

print("Done!")