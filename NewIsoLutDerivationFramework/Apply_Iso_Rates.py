#Applying the rates

import uproot,json,os
import ROOT
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import mplhep as hep
from scipy import stats
import itertools
from functools import partial
import argparse

from matplotlib.ticker import FixedLocator, FixedFormatter
import matplotlib.pyplot as plt
import matplotlib
import mplhep
import Compression_arrays as ca
from Isolation_functions import doubleIEtDoubelIsoSeed

import awkward as ak
plt.style.use(mplhep.style.CMS)

##################################################################################
#################################### Opening  LUT ################################
##################################################################################

parser = argparse.ArgumentParser(description='Isolation derivation')
parser.add_argument('--d',          type=int, default=2025,         help='Data taking year')
args = parser.parse_args()

#If you want to do t with all the LUTs in the file
LUTs_dir = f"LUTs_{args.d}" 
lut_files = [f for f in os.listdir(LUTs_dir) if f.endswith(".txt")]


# #If you want to do it manually just select the LUTs you want here 
# LUTs_dir="LUTs/"
# # lut_files = ['LUT_0p7_17_30.txt', 'LUT_0p6_11_29.txt','LUT_0p6_11_30.txt','LUT_0p7_17_29.txt','LUT_0p7_17_28.txt','LUT_0p7_12_26.txt','LUT_0p7_16_26.txt'] 
# lut_files = ['LUT_0p7_17_30.txt','LUT_0p6_11_30.txt','LUT_0p7_18_27.txt','LUT_0p7_12_26.txt'] 

lut_dict = {}


for fname in lut_files:
    isoLUT = []
    file_path = os.path.join(LUTs_dir, fname)
    with open(file_path) as f:
        txt = f.readlines()
    idx = 0
    for l in txt:
        if len(l) < 2 or l.startswith('#'):
            continue
        l = l.split('#')[0].strip().split() #makes a list where [0] is the the iindex and [1] the threshold
        index = int(l[0])
        if index != idx:
            raise ValueError(f"LUT not proper! Expected index {idx}, got {index}")
        isoLUT.append(int(l[1]))
        idx += 1
    lut_dict[fname] = ak.to_numpy(isoLUT) #dictionary LUT_name : [Iso_thr_values]


##################################################################################
################################### File info ####################################
##################################################################################
prefix='/data_CMS/cms/amella/Run3_2025/'
fname=prefix+'ZeroBias_2025C_392295.root'
file= uproot.open(fname)
file.keys()
tree=file['ZeroBias/ZeroBias']
branches=[]
for i in tree.keys():
    if 'l1tEmu' in i :
        branches.append(i)

data=tree.arrays(branches,entry_stop=100000) #dictionnary branch_name : values_of_the_branch
print("data",data)

N_EVTS=len(data)

##################################################################################
################################### Variables ####################################
##################################################################################
dataStore={}

counts=ak.num(data.l1tEmuRawEt) #returns the number of elements in each sublist (number of taus per event?)
dataStore['counts']=counts

# Compressing and flattening Raw Et
intRawEt = ak.to_numpy(ak.flatten(data.l1tEmuRawEt))
intRawEt[ intRawEt > 255 ] =255
dataStore['l1tEmuRawEt'] = intRawEt
dataStore['compressedRawEt']=ca.compressionRawEtMap[intRawEt]
data['compressedRawEt']=ak.unflatten(dataStore['compressedRawEt'],counts) # adding a new key to the branches dictionary with the compressed values

# Compressing and flattening NTT
counts=ak.num(data.l1tEmuNTT)
intNTT = ak.to_numpy(ak.flatten( data.l1tEmuNTT))
intNTT[ intNTT > 1023 ] =1023
dataStore['l1tEmuNTT']=intNTT
dataStore['compressedNTT']=ca.compressionNTTMap[intNTT]
data['compressedNTT']=ak.unflatten(dataStore['compressedNTT'],counts)

#Masking
gMask=dataStore['l1tEmuNTT']>0
intNTT=np.array(dataStore['l1tEmuNTT'][gMask],dtype=int)
compressedNTT=dataStore['compressedNTT']

# Compressing IEta
intIEta = ak.to_numpy(ak.flatten( data.l1tEmuTowerIEta))
dataStore['l1tEmuTowerIEta']=intIEta
cbis=np.abs(intIEta)
cbis[cbis>31]=31
dataStore['compressedIEta']=ca.compressionEtaMap[cbis]
data['compressedIEta']=ak.unflatten(dataStore['compressedIEta'],counts)

# Flattening Isolation energy
dataStore['l1tEmuIsoEt']=ak.flatten(data['l1tEmuIsoEt'])
# dataStore keys:(['counts', 'l1tEmuRawEt', 'compressedRawEt', 'l1tEmuNTT', 'compressedNTT', 'l1tEmuTowerIEta', 'compressedIEta', 'l1tEmuIsoEt'])

##################################################################################
############################### Applying Iso E ###################################
##################################################################################

ieta=dataStore['compressedIEta']
iet =dataStore['compressedRawEt']
intt=dataStore['compressedNTT']

#Applying Isolation for each bin of eta, ntt and et

rateStore = {}
# rate_scale_factor = 11.245 * 2340 / number of events 
# rate_scale_factor = freq of revolution * nb of max bunches / number of events 
rate_scale_factor = 26.5 * 1e3 / N_EVTS


for fname, isoLUT in lut_dict.items():
    print(fname)
    # for each tau we compute the index telling us to what eta et and ntt bin it belongs to
    lut_index = dataStore['compressedIEta'] << 10 | dataStore['compressedRawEt'] << 5 | dataStore['compressedNTT']
    # to recover the original ieta, iet and intt  using this : the first 2 bits will be from ieta, the 5 in th emiddel iet and the last 5 intt
    # lut_index corresponds to the number of the index in the LUT
    isolation_thr = isoLUT[lut_index]
    #Apply the Iso thr
    dataStore['isolation_bit'] = dataStore['l1tEmuIsoEt'] < isolation_thr #Will return a list with true or false
    data['is_iso'] = ak.unflatten(dataStore['isolation_bit'], dataStore['counts'])
    n_events_with_some_false = ak.sum(ak.any(~data['is_iso'], axis=1))
    print("Number of events with at least one non-isolated tau:", n_events_with_some_false)
    all_seeds_to_probe = {'Double': {'DoubleIsoEG': doubleIEtDoubelIsoSeed}}
    selector = all_seeds_to_probe['Double']['DoubleIsoEG'] # like doing selector = doubleIEtDoubelIsoSeed and then the function is calles by doing selector(args)

    #Computing the rates for each threshold: rates = (nb ev | l1tPt > thr)/ total_nb of ev *nb of bunches * freq of rev
    rateDict = {'threshold': [], 'rate': [], 'rate_err': [], 'pass': [], 'n_events': int(N_EVTS)}
    #For each possible threshold (0-80) how many events pass the requirement of having at least 2 isolated taus data['is_iso'] True
    for i in range(81):
        seed_events = selector(data, i, i)
        pass_count = int(ak.sum(seed_events))
        rateDict['pass'].append(pass_count)
        rateDict['rate'].append(pass_count * rate_scale_factor)
        rateDict['rate_err'].append(pass_count**0.5 * rate_scale_factor)
        rateDict['threshold'].append(i)

    label = fname.replace(".txt", "").replace("LUT_", "Iso_")
    rateStore[label] = rateDict


##################################################################################
#################################### Plotting ###################################
##################################################################################

fig, ax = plt.subplots(figsize=(10,10))

cmap = matplotlib.colormaps.get_cmap('Set1')
n = len(rateStore)
colors = list(range(len(rateStore)))
for (label, color) in zip(rateStore, colors):
    print(label)
    plt.errorbar(rateStore[label]['threshold'],
                 rateStore[label]['rate'],
                 rateStore[label]['rate_err'],
                 ls='None',
                 ecolor=cmap(color),
                 color=cmap(color),
                 label=label,
                 lw=2,
                 marker='D')


# plt.xlabel("Threshold")
plt.ylabel("Rate [kHz]")
mplhep.cms.label('Preliminary', data=True, rlabel=r'13.6 TeV')
plt.axhline(y=14, color='black', linestyle='--', linewidth=1)
ax.yaxis.set_major_locator(FixedLocator([1, 10, 100, 1000, 10000]))
ax.yaxis.set_major_formatter(FixedFormatter([r'1',r'10',r'$10^2$',r'$10^3$',r'$10^4$']))
# plt.title("Rate vs Threshold for Multiple LUTs")
plt.legend()
plt.grid(True)
plt.ylim(1,4E4)
# plt.xlim(30,40)
# plt.ylim(4,30)
# ax.set_ylim(7, 30)
# plt.xlim(32,38)
plt.xlim(0,60)
plt.yscale('log')
plt.xlabel(r'$E_{T}^{\tau, L1}\ [GeV]$')
# plt.show()
plt.savefig(f"plots_{args.d}/Estimation_of_rate_plot.png")
print(f"plots_{args.d}/Estimation_of_rate_plot.png")

