import ROOT
import os
from optparse import OptionParser
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import mplhep
plt.style.use(mplhep.style.CMS)

def read_branch_as_array(inFile, tree_path, branch_name):
    """Reads a float branch from a TTree and returns it as a NumPy array"""
    tree = inFile.Get(tree_path)
    if not tree:
        raise ValueError(f"[ERROR] Tree '{tree_path}' not found in file {inFile.GetName()}")

    values = []
    for entry in tree:
        val = getattr(entry, branch_name, None)
        if val is not None:
            values.append(val)

    return np.array(values)

def get_histogram_values(values, label, bins=50, range=(0, 500), normalize=False):
    counts, bin_edges = np.histogram(values, bins=bins, range=range)
    bin_centers = 0.5 * (bin_edges[:-1] + bin_edges[1:])
    bin_width = bin_edges[1] - bin_edges[0]

    if normalize:
        total = counts.sum()
        counts = counts / (total * bin_width)
        errors = np.sqrt(counts * total * bin_width) / (total * bin_width)
    else:
        errors = np.sqrt(counts)
    
    return bin_centers, counts, errors, label

def plot_histogram(inFile, tree_path, branch_name, label, bins=30, range=(0, 100), normalize=False, var="pt"):
    """Plots a histogram for a given branch in a TTree"""
    values = read_branch_as_array(inFile, tree_path, branch_name)
    print(f"Number of entries in {branch_name}: {len(values)}")
    
    x, y, yerr, label = get_histogram_values(values, label, bins=bins, range=range, normalize=normalize)

    ax.errorbar(x, y, yerr=yerr, fmt='o', label=label)

    if var == "pt":
        ax.set_xlabel(r"$p_T$ [GeV]")
        ax.set_xlim(left=0, right=100)
        ax.legend()

    ax.grid(linestyle=':', alpha=0.7)
    mplhep.cms.text("Work in progress", ax=ax)

def plot_histogram_plt(inFile, tree_path, branch_name, label, color, bins=30, range=(0, 100), var="pt"):
    """Plots a histogram for a given branch in a TTree"""
    values = read_branch_as_array(inFile, tree_path, branch_name)
    print(f"Number of entries in {branch_name}: {len(values)}")
    weights = None
    cmap = matplotlib.colormaps.get_cmap('Set1')

    plt.hist(
        values,
        bins=bins,
        range=range,
        histtype='step',  
        label=label,
        weights=weights,
        alpha=1,
        color=cmap(color),
        density= True,
        linewidth=1.5 if var == "pt" else 1.0
        # linestyle='solid' if var == "pt" else 'dashed'
    )

    if var == "pt":
        plt.xlabel(r"$p_T$ [GeV]")
        # plt.xlim(left=0, right=100)
        plt.legend()
    elif var == "eta":
        plt.xlabel(r"$\eta$")
        plt.legend()
    elif var == "nvtx":
        plt.xlabel(r"Nvtx")
        plt.xlim(left=20, right=90)
        plt.legend()

    plt.grid(linestyle=':', alpha=0.7)
    mplhep.cms.text("Work in progress")

#######################################################################
######################### SCRIPT BODY #################################
#######################################################################
# This script reads branches from ROOT files and plots histograms of their values.
'''python3 plot_ntuple_branches.py --inFile1 HCALcFeb-caloParams_2025_conservative-ZS-MC25W_MATCHED.root  --inFile2 unpacked_2025_eraB_C.root --inFile3 unapcked_2024I.root --tag Unpacked_2025vs2024'''

# Example usage
if __name__ == "__main__":
    parser = OptionParser()
    parser.add_option("--inFile1", dest="inFile1", default=None)
    parser.add_option("--inFile2", dest="inFile2", default=None)
    parser.add_option("--inFile3", dest="inFile3", default=None)
    parser.add_option("--tag", dest="tag", default="pt_distribution")
    parser.add_option("--logx", dest="logx", action="store_true", default=False)
    (options, args) = parser.parse_args()

    folder_1 = '/data_CMS/cms/amella/Run3_2025/MC25_Winter_optmization/'
    folder_2 = '/data_CMS/cms/amella/Run3_2025/unpacked_2025/Run2025_EraBfrom391884_EraC_MINIAOD/'
    folder_3 = '/data_CMS/cms/amella/Run3_2025/2024I-data/'

    output_folder = 'variable_plots/'

    inFile1 = ROOT.TFile(folder_1 + options.inFile1)
    inFile2 = ROOT.TFile(folder_2 + options.inFile2) 
    inFile3 = ROOT.TFile(folder_3 + options.inFile3) 

    fig, ax = plt.subplots(figsize=(10, 10))

    tree_name_mc = "outTreeForCalibration" 
    tree_name_unpacked = "Ntuplizer/TagAndProbe" 
    binspt=50
    bins_eta=20
    bins_nvtx=30
    
    fig1= plt.figure(figsize=(10,10))
    plot_histogram_plt(inFile1, tree_name_mc,"L1Tau_pt", "MC25", 0, bins=binspt, range=(0, 100), var="pt")
    plot_histogram_plt(inFile2, tree_name_unpacked,"l1tPt", "Unpacked 2025", 1, bins=binspt, range=(0, 100), var="pt")
    plot_histogram_plt(inFile3, tree_name_unpacked,"l1tPt", "Unpacked 2024", 2,  bins=binspt, range=(0, 100), var="pt")
    fig1.savefig(output_folder+"l1tPt_distribution.png")

    # fig2= plt.figure(figsize=(10,10))
    # plot_histogram_plt(inFile1, tree_name_mc,"L1Tau_eta", "MC25", 0, bins=bins_eta, range=(-2.5, 2.5), var="eta")
    # plot_histogram_plt(inFile2, tree_name_unpacked,"l1tEta", "Unpacked 2025", 1, bins=bins_eta, range=(-2.5, 2.5), var="eta")
    # plot_histogram_plt(inFile3, tree_name_unpacked,"l1tEta", "Unpacked 2024", 2,  bins=bins_eta, range=(-2.5, 2.5), var="eta")
    # fig2.savefig("output_folder+l1tEta_distribution.png")

    # fig3= plt.figure(figsize=(10,10))
    # plot_histogram_plt(inFile1, tree_name_mc,"Nvtx", "MC25", 0, bins=bins_nvtx, range=(20, 90), var="nvtx")
    # plot_histogram_plt(inFile2, tree_name_unpacked,"Nvtx", "Unpacked 2025", 1, bins=bins_nvtx, range=(20, 90), var="nvtx")
    # plot_histogram_plt(inFile3, tree_name_unpacked,"Nvtx", "Unpacked 2024", 2,  bins=bins_nvtx, range=(20, 90), var="nvtx")
    # fig3.savefig("output_folder+l1tnvtx_distribution.png")

    fig4= plt.figure(figsize=(10,10))
    plot_histogram_plt(inFile1, tree_name_mc,"OfflineTau_pt", "MC25", 0, bins=50, range=(20, 100), var="pt")
    plot_histogram_plt(inFile2, tree_name_unpacked,"tauPt", "Unpacked 2025", 1, bins=50, range=(20, 100), var="pt")
    plot_histogram_plt(inFile3, tree_name_unpacked,"tauPt", "Unpacked 2024", 2,  bins=50, range=(20, 100), var="pt")
    fig4.savefig(output_folder+"offlinePt_distribution.png")

    fig2= plt.figure(figsize=(10,10))
    plot_histogram_plt(inFile1, tree_name_mc,"OfflineTau_eta", "MC25", 0, bins=bins_eta, range=(-2.5, 2.5), var="eta")
    plot_histogram_plt(inFile2, tree_name_unpacked,"tauEta", "Unpacked 2025", 1, bins=bins_eta, range=(-2.5, 2.5), var="eta")
    plot_histogram_plt(inFile3, tree_name_unpacked,"tauEta", "Unpacked 2024", 2,  bins=bins_eta, range=(-2.5, 2.5), var="eta")
    fig2.savefig(output_folder+"OfflineEta_distribution.png")