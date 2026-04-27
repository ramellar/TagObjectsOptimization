import re
import matplotlib.pyplot as plt
import numpy as np
import argparse

conversion = {0: 0.0, 1: 15.0, 2: 18.0, 3: 21.0, 4: 23.0, 5: 25.0, 6: 27.0, 7: 29.0, 8: 31.0, 9: 33.0, 10: 35.0, 11: 37.0, 12: 39.0, 13: 41.0, 14: 43.0, 15: 45.0, 16: 47.0, 17: 50.0, 18: 53.0, 19: 56.0, 20: 59.0, 21: 62.0, 22: 65.0, 23: 69.0, 24: 73.0, 25: 77.0, 26: 82.0, 27: 88.0, 28: 95.0, 29: 105.0, 30: 120.0, 31: 157.0}

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Your script description here.")
    parser.add_argument("--data_file", type=str, help="Path to the data file",
                        default='')
    args = parser.parse_args()

    eta_values = []
    energy_values = []
    ihasEM_values = []
    iisMerged_values = []
    calibration_factors = []
    
    with open(args.data_file, "r") as file:
        for line_number, line in enumerate(file, 1):
            if line_number >= 11:
                split_line = line.split()
                ieta = int(split_line[11])
                iEt = int(split_line[13])
                ihasEM = int(split_line[15])
                iisMerged = int(split_line[17])
                calibration_factor = int(split_line[1])
                eta_values.append(ieta)
                energy_values.append(iEt)
                ihasEM_values.append(ihasEM)
                iisMerged_values.append(iisMerged)
                calibration_factors.append(calibration_factor/512)
    
    min_ieta = min(eta_values)
    max_ieta = max(eta_values)
    min_iEt = min(energy_values)
    max_iEt = max(energy_values)
    
    num_eta_bins = max_ieta - min_ieta + 1
    num_energy_bins = max_iEt - min_iEt + 1
    
    data_combinations = [(0, 0), (0, 1), (1, 0), (1, 1)]
    
    for ihasEM_val, iisMerged_val in data_combinations:
        heat_map_calibration_eta = np.zeros((num_eta_bins, num_energy_bins))
        for ieta, iEt, ihasEM, iisMerged, calibration_factor in zip(eta_values, energy_values, ihasEM_values, iisMerged_values, calibration_factors):
            if ihasEM == ihasEM_val and iisMerged == iisMerged_val:
                eta_bin = ieta - min_ieta
                energy_bin = iEt - min_iEt
                heat_map_calibration_eta[eta_bin][energy_bin] = calibration_factor
    
        energy_labels = [int(conversion[energy_bin]) for energy_bin in range(num_energy_bins)]

        fig, ax = plt.subplots(figsize=(12, 5), dpi=300)

        im = ax.imshow(
            heat_map_calibration_eta,
            cmap="PuRd",
            origin="lower",
            aspect="auto",
            vmin=min(calibration_factors),
            vmax=max(calibration_factors)
        )

        energy_labels = [int(conversion[energy_bin]) for energy_bin in range(num_energy_bins)]
        ax.set_xlabel("Et [GeV]")
        ax.set_ylabel("ieta")
        ax.set_title(f"Heat Map of ieta vs. iEt (ihasEM={ihasEM_val}, iisMerged={iisMerged_val})")
        ax.set_xticks(range(num_energy_bins))
        ax.set_xticklabels(energy_labels, rotation=45, fontsize=8)

        fig.colorbar(im, ax=ax, label="Calibration Factor")

        for i in range(num_eta_bins):
            for j in range(num_energy_bins):
                val = heat_map_calibration_eta[i, j]
                if val != 0:  # only annotate non-empty bins
                    ax.text(
                        j, i, f"{val:.2f}",  # format with 2 decimals
                        ha="center", va="center",
                        rotation=90,
                        color="white" if val > 1.50 else "black",
                        fontsize=6
                    )

        # plt.imshow(heat_map_calibration_eta, cmap="viridis", origin="lower", aspect="auto", vmin=min(calibration_factors), vmax=max(calibration_factors))
        # plt.xlabel("Et [GeV]")
        # plt.ylabel("ieta")
        # plt.title(f"Heat Map of ieta vs. iEt (ihasEM={ihasEM_val}, iisMerged={iisMerged_val})")
        # plt.xticks(range(num_energy_bins), energy_labels, rotation=45, fontsize=8)
        # plt.colorbar(label="Calibration Factor")

        plt.savefig(f"plots/2025_LUT_corrected_calib_heatmap_iEt_vs_iEta_ihasEM{ihasEM_val}_iisMerged{iisMerged_val}.pdf", format="pdf", bbox_inches="tight")
        plt.savefig(f"plots/2025_LUT_corrected_calib_heatmap_iEt_vs_iEta_ihasEM{ihasEM_val}_iisMerged{iisMerged_val}.png", format="png")
        print(f"plots/2025_LUT_corrected_calib_heatmap_iEt_vs_iEta_ihasEM{ihasEM_val}_iisMerged{iisMerged_val}.png")
        plt.clf()  # Clear the plot for the next iteration
