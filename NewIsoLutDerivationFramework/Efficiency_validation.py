import uproot,json,os
import ROOT
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import mplhep as hep
import argparse
from scipy import stats
import itertools
import Compression_arrays as ca
from Isolation_functions import getXProjections, getBinName, getBinForValues, getXProjectionsAtPercentiles, getEfficiency


#Example of command
''' python3 Efficiency_validation.py --eff_min 0.6 --E_min 11 --E_max 30 '''

hep.style.use("CMS")
matplotlib.rcParams['figure.figsize'] = (6, 5.5)
matplotlib.rcParams['figure.dpi'] = 100

parser = argparse.ArgumentParser(description='Isolation derivation')
parser.add_argument('-n',          type=int, default=1,         help='Provide the number of events')
parser.add_argument('--eff_min',          type=float, default=1,         help='Minimum efficiency')
parser.add_argument('--E_max',          type=int, default=1,         help='Maximum Energy bin to which the isolation is applied')
parser.add_argument('--E_min',          type=int, default=1,         help='Minimum Energy bin at which the relaxation starts')
parser.add_argument('--d',          type=int, default=2025,         help='Data taking year')
args = parser.parse_args()

str_effmin = str(args.eff_min).replace('.', 'p')
plots_dir = f"plots_{args.d}"
LUTs_dir = f"LUTs_{args.d}"

prefix='/data_CMS/cms/amella/Run3_2025/'
fname=prefix+'Re-emu-2025C-caloparams2025-newIsoLUT.root'

file= uproot.open(fname)
data=file['Ntuplizer/TagAndProbe']

varsToGet=['tauPt','tauEta','tauPhi',
'l1tPt','l1tEta','l1tPhi','l1tQual','l1tIso',
'l1tEmuPt','l1tEmuEta','l1tEmuPhi','l1tEmuQual','l1tEmuIso',
'l1tEmuNTT','l1tEmuHasEM','l1tEmuIsMerged','l1tEmuTowerIEta','Nvtx',
'l1tEmuTowerIPhi','l1tEmuRawEt','l1tEmuIsoEt']


dataStore=data.arrays(varsToGet)
# dataStore=data.arrays(['l1tEmuPt','l1tEmuNTT','l1tEmuRawEt','RunNumber','l1tEmuTowerIEta','l1tEmuEta'])


puMask=dataStore['Nvtx'] > 0
# print(sum(puMask))

# os.system(f'mkdir -p {prefix}')

#################################################################
##################### Applying compression ######################
#################################################################

print("Number of events : ",len(dataStore))

intNTT=np.array(dataStore['l1tEmuNTT'],dtype=int)
dataStore['compressedNTT']=ca.compressionNTTMap[intNTT]

intRawEt=np.array(dataStore['l1tEmuRawEt'],dtype=int)
intRawEt[intRawEt>255]=255
intRawEt[intRawEt<0]=0
dataStore['compressedRawEt']=ca.compressionRawEtMap[intRawEt]

intIEta=np.abs(np.array(dataStore['l1tEmuTowerIEta'],dtype=int))
intIEta[intIEta>31]=0
dataStore['compressedIEta']=ca.compressionEtaMap[intIEta]

ieta=dataStore['compressedIEta']
iet =dataStore['compressedRawEt']
intt=dataStore['compressedNTT']

lut_index = ieta<<10 | iet <<5 | intt

isolationFname=f"{LUTs_dir}/LUT_eff{str_effmin}_Emin{args.E_min}_Emax{args.E_max}.txt"

with open(isolationFname) as f:
    txt=f.readlines()

idx=0
isoLUT=[]
for l in txt:
    if len(l) <2 : continue
    if l.startswith('#') : continue
    l=l.split('#')[0].strip().split()
    index=int(l[0])
    if index!=idx:
        print(f"LUT not proper ! for expected index {idx} , the value obtained is {index}")
        raise ValueError
    isoLUT.append(int(l[1]))
    idx+=1
isoLUT=np.array(isoLUT)    

isolation_thr = isoLUT[lut_index]
isolation_bit = (dataStore['l1tEmuIsoEt'] < isolation_thr) & ( dataStore.l1tEmuIsoEt >-1)
np.sum(isolation_bit)/len(dataStore['l1tEmuIsoEt'])
np.unique(dataStore.l1tEmuIso,return_counts=True)

loose_isoMask = (dataStore.l1tIso ==1 ) | (dataStore.l1tIso ==3 )
tight_isoMask = (dataStore.l1tIso ==1 ) | (dataStore.l1tIso ==3 )

np.sum(loose_isoMask)

BINS=np.arange(0.0,100.1,1.0)-0.5

preselection_mask = np.abs(dataStore['tauEta']) < 2.5
print("Preliminary selection : ",np.sum(preselection_mask)," / ",len(preselection_mask))

count_all,binx_=np.histogram(dataStore['tauPt'][preselection_mask],bins=BINS)

COUNT_ALL=count_all
BIN_X=0.5*(BINS[1:]+BINS[:-1])

efficiencyMap={}

for et in range(81):
    effDict={}
    eff_name=f"DoubleTau_{et}"
    print(f"\rProcessing {eff_name}   !",end="")
    triggerSeedMask = dataStore['l1tEmuPt'] >= et
    triggerSeedMask = np.logical_and(triggerSeedMask,preselection_mask)
    
    count_trig,binx_= np.histogram(dataStore['tauPt'][triggerSeedMask],bins=BINS)
    effDict['pass'] = [int(i) for i in count_trig ]
    effDict['all']  = COUNT_ALL
    eff=count_trig/(COUNT_ALL+1e-9)
    effDict['efficiency']  = eff
    effDict['bin']  = BINS
    effDict['bin_center'] = BIN_X
    
    efficiencyMap[eff_name] = effDict

unpackedIsoefficiencyMap={}

for et in range(81):
    effDict={}
    eff_name=f"DoubleTau_{et}"
    print(f"\rProcessing {eff_name}   !",end="")
    triggerSeedMask = dataStore['l1tEmuPt'] >= et
    triggerSeedMask = np.logical_and(triggerSeedMask,preselection_mask)
    triggerSeedMask = np.logical_and(triggerSeedMask,loose_isoMask)
    
    count_trig,binx_= np.histogram(dataStore['tauPt'][triggerSeedMask],bins=BINS)
    effDict['pass'] = [int(i) for i in count_trig ]
    effDict['all']  = COUNT_ALL
    eff=count_trig/(COUNT_ALL+1e-9)
    effDict['efficiency']  = eff
    effDict['bin']  = BINS
    effDict['bin_center'] = BIN_X
    
    unpackedIsoefficiencyMap[eff_name] = effDict
    
isoefficiencyMap={}

for et in range(81):
    effDict={}
    eff_name=f"DoubleTau_{et}"
    print(f"\rProcessing {eff_name}   !",end="")
    triggerSeedMask = dataStore['l1tEmuPt'] >= et
    triggerSeedMask = np.logical_and(triggerSeedMask,preselection_mask)
    triggerSeedMask = np.logical_and(triggerSeedMask,isolation_bit)
    
    count_trig,binx_= np.histogram(dataStore['tauPt'][triggerSeedMask],bins=BINS)
    effDict['pass'] = [int(i) for i in count_trig ]
    effDict['all']  = COUNT_ALL
    eff=count_trig/(COUNT_ALL+1e-9)
    effDict['efficiency']  = eff
    effDict['bin']  = BINS
    effDict['bin_center'] = BIN_X
    
    isoefficiencyMap[eff_name] = effDict
    

et=26
eff_name=f"DoubleTau_{et}"
hep.style.use("CMS")
plt.scatter(efficiencyMap[eff_name]['bin_center'],efficiencyMap[eff_name]['efficiency'],label='No-Iso')
plt.scatter(unpackedIsoefficiencyMap[eff_name]['bin_center'],unpackedIsoefficiencyMap[eff_name]['efficiency'],label='Unpacked-Iso')
plt.scatter(isoefficiencyMap[eff_name]['bin_center'],isoefficiencyMap[eff_name]['efficiency'],label='New-Iso')
plt.xlim(20,100)
plt.legend()
plt.savefig(f"{plots_dir}/efficiency_plot_effmin{str_effmin}_Emin{args.E_min}_Emax{args.E_max}.png")
print(f"\n Saved figure: {plots_dir}/efficiency_plot_effmin{str_effmin}_Emin{args.E_min}_Emax{args.E_max}.png")