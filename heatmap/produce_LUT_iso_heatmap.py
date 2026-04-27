import re
import matplotlib.pyplot as plt
import numpy as np
import argparse
from matplotlib.colors import LogNorm
from matplotlib.ticker import LogLocator
import ROOT
import matplotlib.pyplot as plt
import numpy as np
import mplhep as hep
plt.style.use(hep.style.CMS)

'''python3 produce_LUT_iso_heatmap.py --data_file /home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/NewIsoLutDerivationFramework/LUTs/LUT_0p7_18_27.txt --tag tau_Iso_0p7_18_27_2025'''

parser = argparse.ArgumentParser(description="")
parser.add_argument("--data_file", type=str, help="Path to the data file",
                    default='')
parser.add_argument("--tag", type=str, help="Tag", default='')
args = parser.parse_args()


allLUTS = {
    args.tag: args.data_file
}

for ky in allLUTS:
    fname = allLUTS[ky]
    print(f"processing {ky} [{fname}]")
    with open(fname) as f:
        txt = f.readlines()

    LUT = np.zeros((4, 32, 32), int)

    for l in txt:
        if l.startswith("#") or len(l.strip()) < 2:
            continue
        val = int(l.split(' ')[1])
        items = l.split("=")[1].split(":")
        LUT[int(items[0])][int(items[1])][int(items[2])] = val

    # Create subplots with tighter, well-balanced spacing
    f, axlist = plt.subplots(2, 2, figsize=(16, 16))
    f.subplots_adjust(wspace=0.36, hspace=0.17)
    axlist = np.ndarray.flatten(axlist)

    for eta in range(4):
        ax = axlist[eta]
        lut_proj = LUT[eta, :, :]
        maxIsoEt = 100
        lut_proj_display = np.clip(lut_proj, 0, maxIsoEt)

        # Use extent to center pixels at integers with edges at half-integers
        c = ax.imshow(lut_proj_display, cmap='tab20c', origin='lower', vmin=0, vmax=maxIsoEt,
                    interpolation='nearest',
                    extent=[-0.5, 31.5, -0.5, 31.5], aspect='auto')

        major_ticks = np.arange(0, 33, 3)
        ax.set_xticks(major_ticks)
        ax.set_yticks(major_ticks)

        # Minor ticks at every integer for grid lines
        minor_ticks = np.arange(-0.5, 32, 1)
        ax.set_xticks(minor_ticks, minor=True)
        ax.set_yticks(minor_ticks, minor=True)
        ax.tick_params(axis='both', which='both', length=0, labelsize=14)

        # Grid only on minor ticks for bin edges
        ax.grid(which='minor', color='k', linestyle='-', linewidth=1, alpha=0.5)
        ax.grid(which='major', color='none')

        ax.set_xlabel(r'compressed $n_{\mathrm{TT}}$', fontsize=16)
        ax.set_ylabel(r'compressed $E_{\mathrm{T}}$', fontsize=16)

        t = ax.text(25.2, 12.3, r"$i\eta$ = " + str(eta), fontweight='bold', c='r', alpha=1, fontsize=16)
        t.set_bbox(dict(facecolor='w', alpha=1.0, edgecolor='r'))

        # Colorbar with smaller label
        ax2 = f.add_axes([ax.get_position().x1 + 0.01, ax.get_position().y0, 0.02, ax.get_position().height])
        cb = plt.colorbar(c, cax=ax2)
        cb.set_label('Isolation Energy Threshold', fontsize=16)
        cb.ax.tick_params(labelsize=12)

        # CMS label with smaller font
        hep.cms.label(data=True, label="Private Work", year=ky.split('_')[-1], ax=ax, fontsize=18)

    f.savefig(f'Iso_plots/{ky}.pdf', bbox_inches='tight')
    f.savefig(f'Iso_plots/{ky}.png', bbox_inches='tight')
    print(f'Iso_plots/{ky}.png')