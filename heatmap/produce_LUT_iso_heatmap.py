import re
import matplotlib.pyplot as plt
import numpy as np
import argparse
from matplotlib.colors import LogNorm
from matplotlib.ticker import LogLocator

conversion = {0: 0.0, 1: 15.0, 2: 18.0, 3: 21.0, 4: 23.0, 5: 25.0, 6: 27.0, 7: 29.0, 8: 31.0, 9: 33.0, 10: 35.0, 11: 37.0, 12: 39.0, 13: 41.0, 14: 43.0, 15: 45.0, 16: 47.0, 17: 50.0, 18: 53.0, 19: 56.0, 20: 59.0, 21: 62.0, 22: 65.0, 23: 69.0, 24: 73.0, 25: 77.0, 26: 82.0, 27: 88.0, 28: 95.0, 29: 105.0, 30: 120.0, 31: 157.0}

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="")
    parser.add_argument("--data_file", type=str, help="Path to the data file",
                        default='../Calibrate/LUTs_2024/LUTcalibration_2023Summer_caloParams_2023_v0_4.txt')
    parser.add_argument("--tag", type=str, help="Tag", default='')
    args = parser.parse_args()

    eta_values = []
    energy_values = []
    inTT_values = []
    isolation = []
    
    with open(args.data_file, "r") as file:
        for line_number, line in enumerate(file, 1):
            if line_number >= 7:
                split_line = line.split()
                ieta = int(split_line[9])
                iEt = int(split_line[11])
                inTT = int(split_line[13])
                isolation_energy = int(split_line[1])
                eta_values.append(ieta)
                energy_values.append(iEt)
                inTT_values.append(inTT)
                isolation.append(isolation_energy)
    
    min_ieta = min(eta_values)
    max_ieta = max(eta_values)
    min_iEt = min(energy_values)
    max_iEt = max(energy_values)
    
    num_eta_bins = max_ieta - min_ieta + 1
    num_energy_bins = max_iEt - min_iEt + 1
    
    for inTT_val in list(set(inTT_values)):
        print(f"Analysing inTT = {inTT_val}")
        heat_map_calibration_eta = np.zeros((num_eta_bins, num_energy_bins))
        for ieta, iEt, inTT, isolation_energy in zip(eta_values, energy_values, inTT_values, isolation):
            if inTT == inTT_val:
                eta_bin = ieta - min_ieta
                energy_bin = iEt - min_iEt
                heat_map_calibration_eta[eta_bin][energy_bin] = isolation_energy
    
        energy_labels = [int(conversion[energy_bin]) for energy_bin in range(num_energy_bins)]
        plt.imshow(heat_map_calibration_eta, cmap="viridis", origin="lower", aspect="auto", norm=LogNorm(vmin=min(isolation), vmax=max(isolation)))
        plt.xlabel("Et [GeV]")
        plt.ylabel("ieta")
        plt.title(f"Heat Map of ieta vs. iEt (inTT={inTT_val})")
        plt.xticks(range(num_energy_bins), energy_labels, rotation=45, fontsize=7)
        plt.colorbar(label="Isolation Energy GeV")
        plt.savefig(f"plots/iso/{args.tag}_LUT_iso_heatmap_iEt_vs_iEta_inTT{inTT_val}.pdf", format="pdf")
        plt.savefig(f"plots/iso/{args.tag}_LUT_iso_heatmap_iEt_vs_iEta_inTT{inTT_val}.png", format="png")
        plt.clf()  # Clear the plot for the next iteration
