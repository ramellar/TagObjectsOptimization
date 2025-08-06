import ROOT
import os
from optparse import OptionParser
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm 
import mplhep
import uproot
import numpy as np
plt.style.use(mplhep.style.CMS)

def read_branch_as_array(inFile, tree_path, branch_name):
    """
    Reads a float branch from a TTree using uproot and returns it as a NumPy array.

    Parameters:
        file_path (str): Path to the ROOT file.
        tree_path (str): Path to the TTree inside the ROOT file.
        branch_name (str): Name of the branch to read.

    Returns:
        np.ndarray: Array of branch values.
    """
    file = uproot.open(inFile)
    print(f"Opened file: {inFile}")
    tree = file[tree_path]
    print(f"Accessing tree: {tree_path}")
    values = tree[branch_name].array(library="np")
    print(values[20:30])  

    # with uproot.open(file_path) as file:
    #     try:
    #         tree = file[tree_path]
    #     except KeyError:
    #         raise ValueError(f"[ERROR] Tree '{tree_path}' not found in file {file_path}")

    #     try:
    #         array = tree[branch_name].array(library="np")
    #     except KeyError:
    #         raise ValueError(f"[ERROR] Branch '{branch_name}' not found in tree '{tree_path}'")

    return values

def plot_2d_histogram(inFile, tree_name, branch_name_x, branch_name_y, bins=30, xlabel='X-axis', ylabel='Y-axis', x_lim=(0, 125), ylim=(0, 80),title='2D Histogram'):
    """
    Plots a 2D histogram from two NumPy arrays.
    
    Parameters:
    - x, y: 1D numpy arrays of the same length
    - bins: number of bins or [xbins, ybins]
    - xlabel, ylabel, title: labels for the plot
    """

    x = read_branch_as_array(inFile, tree_name, branch_name_x)
    print(f"Read {len(x)} entries for {branch_name_x} from {tree_name}")
    y = read_branch_as_array(inFile, tree_name, branch_name_y)
    print(f"Read {len(y)} entries for {branch_name_y} from {tree_name}")

    plt.scatter(x, y, s=1, alpha=0.5)
    plt.hist2d(x, y, bins=bins, cmap='viridis',norm="log",density=True)
    plt.xlim(x_lim)
    plt.ylim(ylim)
    plt.colorbar(label='Counts')
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.tight_layout()

#######################################################################
######################### SCRIPT BODY #################################
#######################################################################
# This script reads branches from ROOT files and plots histograms of their values.
'''python3 plot_variables_2D.py --inFile1 HCALcFeb-caloParams_2025_conservative-ZS-MC25W_MATCHED.root \ 
                                   --inFile2 TandP_reEmul_EraC_2025C_EG0.root '''
'''python3 plot_variables_2D.py --inFile1 HCALcFeb-caloParams_2025_conservative-ZS-MC25W_MATCHED.root --inFile2 TandP_reEmul_EraC_2025C_EG0.root '''


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
    folder_2 = '/data_CMS/cms/amella/Run3_2025/EG-samples/'
    folder_3 = '/data_CMS/cms/amella/Run3_2025/EG-samples/'
    # folder_2 = '/data_CMS/cms/amella/Run3_2025/unpacked_2025/Run2025_EraBfrom391884_EraC_MINIAOD/'
    # folder_3 = '/data_CMS/cms/amella/Run3_2025/2024I-data/'

    output_folder = '2025_plots/'

    # inFile1 = ROOT.TFile(folder_1 + options.inFile1)
    # inFile2 = ROOT.TFile(folder_2 + options.inFile2) 
    # inFile3 = ROOT.TFile(folder_3 + options.inFile3) 
    inFile1 = folder_1 + options.inFile1
    inFile2 = folder_2 + options.inFile2
    # inFile3 = folder_3 + options.inFile3

    fig, ax = plt.subplots(figsize=(10, 10))

    tree_name_mc = "outTreeForCalibration" 
    tree_name_unpacked = "Ntuplizer/TagAndProbe" 
    
    # fig= plt.figure(figsize=(10,10))
    # plot_2d_histogram(inFile1, tree_name= tree_name_mc , 
    #                     branch_name_x="Nvtx", 
    #                     bins=[np.arange(-0.5,100.5,1) , np.arange(-0.5,100.5,1)], 
    #                     branch_name_y="L1Tau_nTT", xlabel='Nvtx', ylabel='nTT', 
    #                     x_lim=(0, 125), ylim=(0, 80),
    #                     title='2D Histogram of MC25')
    # fig.savefig(output_folder+"MC_distribution.png")
    
    # fig1= plt.figure(figsize=(10,10))
    # plot_2d_histogram(inFile2, tree_name=tree_name_unpacked , 
    #                     branch_name_x="Nvtx", bins=[np.arange(-0.5,100.5,1) , np.arange(-0.5,100.5,1)],  
    #                     branch_name_y="l1tEmuNTT" , xlabel='Nvtx', ylabel='nTT', 
    #                     x_lim=(0, 125), ylim=(0, 80),
    #                     title='2D Histogram Unpacked 2025')
    # fig1.savefig(output_folder+"2025CRe-emu.png")
    
    fig2= plt.figure(figsize=(10,10))
    plot_2d_histogram(inFile1, tree_name= tree_name_mc , 
                        branch_name_x="Nvtx", bins=[np.arange(-0.5,100.5,1) , np.arange(-0.5,100.5,1)], branch_name_y="L1Tau_Iso", 
                        xlabel='Iso Et', ylabel='nTT', 
                        x_lim=(0, 100), ylim=(0, 100),
                        title='2D Histogram of MC25')
    fig2.savefig(output_folder+"MC_distributionIsoE.png")
    
    fig3= plt.figure(figsize=(10,10))
    plot_2d_histogram(inFile2, tree_name=tree_name_unpacked , 
                        branch_name_x="Nvtx", bins=[np.arange(-0.5,100.5,1) , np.arange(-0.5,100.5,1)], branch_name_y="l1tIso" , 
                        xlabel='Iso Et', ylabel='nTT', 
                        x_lim=(0,100 ), ylim=(0, 100),
                        title='2D Histogram Unpacked 2025')
    fig3.savefig(output_folder+"2025CRe-emuIsoE.png")





