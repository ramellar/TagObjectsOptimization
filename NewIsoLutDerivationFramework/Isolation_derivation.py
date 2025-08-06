#Script to derive the Isolation LUT
import uproot,json,os
import argparse
import ROOT
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import mplhep as hep
from scipy import stats
import itertools
import Compression_arrays as ca
from Isolation_functions import getXProjections, getBinName, getBinForValues, getXProjectionsAtPercentiles, getEfficiency, getEfficiency_args

#Example of command:

'''python3 Isolation_derivation.py --eff_min 0.6 --E_min 11 --E_max 30'''

parser = argparse.ArgumentParser(description='Isolation derivation')
parser.add_argument('-n',          type=int, default=1,         help='Provide the number of events')
parser.add_argument('--eff_min',          type=float, default=1,         help='Minimum efficiency')
parser.add_argument('--E_max',          type=int, default=1,         help='Maximum Energy bin to which the isolation is applied')
parser.add_argument('--E_min',          type=int, default=1,         help='Minimum Energy bin at which the relaxation starts')
parser.add_argument('--d',          type=int, default=2025,         help='Data taking year')
args = parser.parse_args()

#Defining plotting style

hep.style.use("CMS")
matplotlib.rcParams['figure.figsize'] = (10, 10)
matplotlib.rcParams['figure.dpi'] = 300

plots_dir = f"plots_{args.d}"
os.makedirs(plots_dir, exist_ok=True)
LUTs_dir = f"LUTs_{args.d}"
os.makedirs(LUTs_dir, exist_ok=True)
str_effmin = str(args.eff_min).replace('.', 'p')

###############################################################################################################
############################################ Opening the File #################################################
###############################################################################################################

prefix='/data_CMS/cms/amella/Run3_2025/'
# fname='HCALcFeb-caloParams_2025_conservative-ZS-MC25W_CALIBRATED_corrected.root'
# fname=prefix+'MC2025_caloparams-2025/MC2025-caloparams-2025.root'
fname=prefix+'taus_2025C_re_emulated.root'

file= uproot.open(fname)

data=file['Ntuplizer/TagAndProbe']

#Print to check the the keys of the file and if ot crashes replace varsToGet by this
# print(data.keys())

# varsToGet=['tauPt','tauEta','tauPhi','tauCharge',
# 'l1tPt','l1tEta','l1tE','l1tPhi','l1tQual','l1tIso',
# 'l1tEmuPt','l1tEmuEta','l1tEmuPhi','l1tEmuQual','l1tEmuIso',
# 'l1tEmuNTT','l1tEmuHasEM','l1tEmuIsMerged','l1tEmuTowerIEta','Nvtx',
# 'l1tEmuTowerIPhi','l1tEmuRawEt','l1tEmuIsoEt']
varsToGet=['EventNumber', 'RunNumber', 'lumi', 'PS_column', 'MC_weight', 'tauTriggerBits', 'tauPt', 'tauEta', 'tauPhi', 'tauDM', 'mT', 'mVis', 'tau_genindex', 'byLooseCombinedIsolationDeltaBetaCorr3Hits', 'byMediumCombinedIsolationDeltaBetaCorr3Hits', 'byTightCombinedIsolationDeltaBetaCorr3Hits', 'byVVVLooseDeepTau2018v2p5VSe', 'byVVLooseDeepTau2018v2p5VSe', 'byVLooseDeepTau2018v2p5VSe', 'byLooseDeepTau2018v2p5VSe', 'byMediumDeepTau2018v2p5VSe', 'byTightDeepTau2018v2p5VSe', 'byVTightDeepTau2018v2p5VSe', 'byVVTightDeepTau2018v2p5VSe', 'byVVVLooseDeepTau2018v2p5VSjet', 'byVVLooseDeepTau2018v2p5VSjet', 'byVLooseDeepTau2018v2p5VSjet', 'byLooseDeepTau2018v2p5VSjet', 'byMediumDeepTau2018v2p5VSjet', 'byTightDeepTau2018v2p5VSjet', 'byVTightDeepTau2018v2p5VSjet', 'byVVTightDeepTau2018v2p5VSjet', 'byVLooseDeepTau2018v2p5VSmu', 'byLooseDeepTau2018v2p5VSmu', 'byMediumDeepTau2018v2p5VSmu', 'byTightDeepTau2018v2p5VSmu', 'againstMuonLoose3', 'againstMuonTight3', 'muonPt', 'muonEta', 'muonPhi', 'MET', 'hltPt', 'hltEta', 'hltPhi', 'hltL2CaloJetPt', 'hltL2CaloJetEta', 'hltL2CaloJetPhi', 'hltL2CaloJetIso', 'hltL2CaloJetIsoPixPt', 'hltL2CaloJetIsoPixEta', 'hltL2CaloJetIsoPixPhi', 'hltPFTauTrackPt', 'hltPFTauTrackEta', 'hltPFTauTrackPhi', 'hltPFTauTrackRegPt', 'hltPFTauTrackRegEta', 'hltPFTauTrackRegPhi', 'hltPFTau35TrackPt1RegPt', 'hltPFTau35TrackPt1RegEta', 'hltPFTau35TrackPt1RegPhi', 'l1tPt', 'l1tEta', 'l1tPhi', 'l1tQual', 'l1tIso', 'l1tEmuPt', 'l1tEmuEta', 'l1tEmuPhi', 'l1tEmuQual', 'l1tEmuIso', 'l1tEmuNTT', 'l1tEmuHasEM', 'l1tEmuIsMerged', 'l1tEmuTowerIEta', 'l1tEmuTowerIPhi', 'l1tEmuRawEt', 'l1tEmuIsoEt', 'hasTriggerMuonType', 'hasTriggerTauType', 'isMatched', 'isOS', 'foundJet', 'Nvtx', 'nTruePU']


dataStore=data.arrays(varsToGet)
# dataStore=data.arrays(['l1tEmuPt','l1tEmuNTT','l1tEmuRawEt','RunNumber','l1tEmuTowerIEta','l1tEmuEta'])

puMask=dataStore['Nvtx'] > 0

###############################################################################################################
################################# Applying the compression maps to the data ###################################
###############################################################################################################


# Defines the list of values to compress from data
intNTT=np.array(dataStore['l1tEmuNTT'],dtype=int)
# Compressing the NTT values using the compression map. 
# By definition of this mapping, the values of NTT should me maximimum of 1024 and these values are mapped into 32 bins
dataStore['compressedNTT']=ca.compressionNTTMap[intNTT]

intRawEt=np.array(dataStore['l1tEmuRawEt'],dtype=int)
# By definition of this mapping, the values of NTT should me maximimum of 255, hence the following masking is applied and then these values are mapped into 32 bins
intRawEt[intRawEt>255]=255
dataStore['compressedRawEt']=ca.compressionRawEtMap[intRawEt]

intIEta=np.abs(np.array(dataStore['l1tEmuTowerIEta'],dtype=int))
dataStore['compressedIEta']=ca.compressionEtaMap[intIEta]

###############################################################################################################
########################################### Extracting data ###################################################
###############################################################################################################

#Applying super compression where we pair up bins two by two and go from 32 to 16 bins in Et and from 4 to 2 in iEta
bin_splits={
#     'compressedIEta' : [[0,0],[1,1],[2,2],[3,3]],
    'compressedIEta' : [[0,1],[2,3]],
    'compressedRawEt' : [[2*i,2*i+1] for i in range(16)],
}

binned_data={}
bin_dim_names=list(bin_splits.keys()) #['compressedIEta', 'compressedRawEt']
#This creates all combinations of IEta × RawEt bin ranges (Cartesian product)
bins_bounds_=list(itertools.product(*[bin_splits[ky] for ky in bin_dim_names]))
#This creates a dictionnary where for each bin_bound tuple it will asign a name based on the bin_dim_names
bins_bounds={getBinName(bin_dim_names,bb) : bb for bb in bins_bounds_}
nbins=len(bins_bounds)
print(f"Number of bins : {nbins}") #This should be 16*2=32 bins

extracted_data={}
data_split_inBins={}
nbins=len(bins_bounds)

bin_mask=np.ones(len(dataStore['Nvtx']),dtype=bool)

#We create a mask to filter the dataStore by bins of [iEta and RawEt] defined previously and then store the data in a dictionary
# The output data_split_inBins will be a dictionary where each key is a bin name and the value is the dataStore filtered by that bin
for i,bn_name in enumerate(bins_bounds):
    print(f"\rProcessing bin {i+1}/{nbins}",end="")
    bin_mask=dataStore['compressedIEta'] > -1
    # For example for the first bin_dim_names 'compressedIEta' and bounds [0,1]
    # we will have bin_mask=np.logical_and(bin_mask,dataStore['compressedIEta']>0)
    # and bin_mask=np.logical_and(bin_mask,dataStore['compressedIEta']<1)
    # which will filter the dataStore to only include events where 'compressedIEta' is between 0 and 1
    for bn,bb in zip(bin_dim_names,bins_bounds[bn_name]):
        bin_mask=np.logical_and(bin_mask,dataStore[bn]>=bb[0])
        bin_mask=np.logical_and(bin_mask,dataStore[bn]<=bb[1])
    n=np.sum(bin_mask)
    if n < 2000:
        print("   > NEvents : ",n)
        print()
    data_split_inBins[bn_name]=dataStore[bin_mask]

#For each bin of [iEta and RawEt] in data_split_inBins, we will extract the compressed NTT and IsoEt values
# and then we will fit a linear regression to the data to get the slope and intercept
# The output extracted_data will be a dictionary where each key is a bin name and the value is 
# another dictionary containing the compressed NTT, IsoEt, IsoEt standard deviation, and linear fit
for i,bin_name in enumerate(data_split_inBins):
    print(f"\rProcessing bin {i+1}/{nbins}",end="")
    ldta=data_split_inBins[bin_name]
    x,y=np.array(ldta['compressedNTT']),np.array(ldta['l1tEmuIsoEt'])
    projection_default=getXProjections(x,y) # projection_default is a dictionary with keys 'x', 'projection'(the mean of y per x value), and 'stdev'
    extracted_data[bin_name]={}
    extracted_data[bin_name]['c_ntt']  =projection_default['x']
    extracted_data[bin_name]['c_isoet']=projection_default['projection']
    extracted_data[bin_name]['c_isoet_std']=projection_default['stdev']
    fit_range_mask = x  > -100 # TRIVIAL MASK
    res = stats.linregress(x[fit_range_mask],y[fit_range_mask])
    extracted_data[bin_name]['linear_fit']={}
    extracted_data[bin_name]['linear_fit']["Remarks"] = "m*x+c"
    extracted_data[bin_name]['linear_fit']["c"] = res.intercept
    extracted_data[bin_name]['linear_fit']["m"] = res.slope

# Plotting Iso Et vs NTT


ieta_bins = {
    'iEta_0_1': lambda name: 'compressedIEtaIN[0,1]' in name,
    'iEta_2_3': lambda name: 'compressedIEtaIN[2,3]' in name,
}

for label, condition in ieta_bins.items():
    fig, ax = None, None
    ifig = -1
    nfig = 0


    for i, bin_name in enumerate(extracted_data):
        if not condition(bin_name):
            continue

        ifig += 1
        if ifig == 16 or fig is None:
            nfig += 1
            # breakpoint()
            fig, ax = plt.subplots(4, 4, figsize=(20, 20))
            ax = np.ndarray.flatten(ax)
            # ax[ifig].grid(":")
            ifig = 0


        x = extracted_data[bin_name]['c_ntt']
        y = extracted_data[bin_name]['c_isoet']
        yerr = extracted_data[bin_name]['c_isoet_std']

        ax[ifig].errorbar(x, y, yerr=yerr, fmt='o', markersize=5, color='deeppink', ecolor='lightpink')
        ax[ifig].set_xlim([0, 16])
        ax[ifig].set_ylim([0, 20])
        ax[ifig].set_xlabel(r'Compressed NTT',fontsize=10)
        ax[ifig].set_ylabel(r'$IsoE^{L_1}$',fontsize=10)

        lbl = "\n".join(bin_name[1:-1].replace("compressed", "").replace("IN", r'${\in}$').split("_"))
        ax[ifig].text(0.05, 0.95, lbl, transform=ax[ifig].transAxes,
                      fontsize=12, verticalalignment='top', horizontalalignment='left')
        ax[ifig].grid(linestyle=":")

    plt.tight_layout()
    plt.savefig(f'{plots_dir}/IsoEt_vs_nTT_{label}_effmin{str_effmin}_Emin{args.E_max}__Emin{args.E_max}.png')
    print(f"Saved figure: {plots_dir}/IsoEt_vs_nTT_{label}_effmin{str_effmin}_Emin{args.E_max}__Emin{args.E_max}.png")
    plt.close(fig)


###############################################################################################################
################################# Finding the IsoEt threshold per bin #########################################
###############################################################################################################

compressedEta_nbits=2
compressedEt_nbits=5
compressednTT_nbits=5

nEtaBins=1<<compressedEta_nbits # nEtaBins = 1 << 2  = 2 ** 2 = 4
nNTTBins=1<<compressednTT_nbits # nNTTBins = 1 << 5  = 2 ** 5 = 32
nEtBins =1<<compressedEt_nbits # nEtBins = 1 << 5  = 2 ** 5 = 32

#Looping over iEta, RawEt and NTT bins to create the LUT to build a data structure called LUT_MAP that will hold the index and threshold values for each bin

k=-1
LUT_MAP={'index':[],'threshold':[],'map':{}}
for ieta in range(nEtaBins):
    if ieta not in LUT_MAP['map']:
        LUT_MAP['map'][ieta]={}
    for iet in range(nEtBins):
        # print(f"\r Processing Bin {ieta=:>7} {iet=:>8}",end="")
        if iet not in LUT_MAP['map'][ieta]:
            LUT_MAP['map'][ieta][iet]={}
        
        bin_values={'compressedIEta':ieta,'compressedRawEt':iet}
        bin_bounds=getBinForValues(bin_values,bin_splits,bin_dim_names)
        bin_name=getBinName(bin_dim_names,bin_bounds)
        ldta=data_split_inBins[bin_name]
        x,y=np.array(ldta['compressedNTT']),np.array(ldta['l1tEmuIsoEt'])
        
        
        qtile=getEfficiency_args(ieta,iet,args)*100
        print(f"\n - Processing bin {bin_name} with qtile={qtile}")
        projection_default=getXProjectionsAtPercentiles( x,y,qtile)
        extracted_data[bin_name]={}
        extracted_data[bin_name]['c_ntt']  =projection_default['x']
        extracted_data[bin_name]['c_isoet']=projection_default['projection']
        extracted_data[bin_name]['c_isoet_std_u']=projection_default['stdev_u']
        extracted_data[bin_name]['c_isoet_std_d']=projection_default['stdev_d']
#         fit_range_mask = projection_default['x']  > -100 # TRIVIAL MASK
        fit_range_mask = np.logical_and(projection_default['x']>=0,projection_default['x'] < 6 ) # TRIVIAL MASK
        res = stats.linregress(projection_default['x'][fit_range_mask],
                               projection_default['projection'][fit_range_mask]
                              )
        
        extracted_data[bin_name]['linear_fit']={}
        extracted_data[bin_name]['linear_fit']["Remarks"] = "m*x+c"
        extracted_data[bin_name]['linear_fit']["c"] = res.intercept
        extracted_data[bin_name]['linear_fit']["m"] = res.slope
        if(  res.slope < 0):
            print("\n -ve slope for ",bin_name,", ",qtile)
        if qtile==100:
            extracted_data[bin_name]['linear_fit']["c"]=255
            extracted_data[bin_name]['linear_fit']["m"]=255
            
        
        edata=extracted_data[bin_name]
        
        for intt in range(nNTTBins):
            index= (ieta<<10) | (iet<<5) | intt
            threshold=edata['linear_fit']['m']*intt+edata['linear_fit']['c']
            if threshold<0:
                threshold=0
            if threshold>127:
                threshold=127
                
            threshold=int(threshold+1)
            if intt not in LUT_MAP['map'][ieta][iet]:
                LUT_MAP['map'][ieta][iet][intt]={}
            LUT_MAP['map'][ieta][iet][intt]['threshold']=threshold
            LUT_MAP['map'][ieta][iet][intt]['index']     =index
            # print(f"\n  ieta={ieta}, iet={iet}, intt={intt}, index={index}, threshold={threshold}")
            # print("map",LUT_MAP['map'][ieta][iet][intt])
            LUT_MAP['index'].append(index)
            LUT_MAP['threshold'].append(threshold)

###############################################################################################################
################################# Plotting the Iso ET vs NTT fits #############################################
###############################################################################################################


f,ax=plt.subplots(4,4,figsize=(16,16),dpi=150)
ax=np.ndarray.flatten(ax)
for ietas in bin_splits['compressedIEta']:
    ieta=ietas[0]
    for iet in LUT_MAP['map'][ieta]:
        ietI=int(ieta*4 + iet/8)
        x=[]
        y=[]
        for intt in LUT_MAP['map'][ieta][iet]:
            x.append(intt)
            thr=LUT_MAP['map'][ieta][iet][intt]['threshold']
            if thr>39: thr=39
            y.append(thr)
        ax[ietI].scatter(x,y,s=5,label=f"ieta={ieta},iet={iet}")

for ieta in LUT_MAP['map']:
    for iet in LUT_MAP['map'][ieta]:
        ietI=int(ieta*4 + iet/8)
        ax[ietI].legend(ncol=2,fontsize=12)
        ax[ietI].set_ylim([-20,120])
        ax[ietI].set_xlim([-25,40])
#         ax[ietI].text(-15,20+(iet%2)*10,f"comp. iET : {iet}",fontsize=12)
        
        if (ietI%4)==0:
            ax[ietI].set_ylabel("IsoEt thr.",fontsize=12)
        if  ietI>11:
            ax[ietI].set_xlabel("Compressed NTT",fontsize=12)


###############################################################################################################
################################# Plotting the ISO LUTs #############################################
###############################################################################################################

if True:
    LUT=np.zeros((4,32,32),int)
    for i in LUT_MAP['map']:
        for j in LUT_MAP['map'][i]:
            for k in LUT_MAP['map'][i][j]:
                LUT[i][j][k]=LUT_MAP['map'][i][j][k]['threshold']

    f,axlist=plt.subplots(2,2,figsize=(14,10))
    axlist=np.ndarray.flatten(axlist)
    for eta in range(4):
        ax=axlist[eta]
        lut_proj=LUT[eta,:,:]
    #     f,ax=plt.subplots(figsize=(32/4,16/4))
        maxIsoEt=90
        if eta > 8:
            maxIsoEt=120
        lut_proj[lut_proj>maxIsoEt]=maxIsoEt
        c=ax.imshow(lut_proj,cmap='tab20c',origin='lower',vmin=0, vmax=maxIsoEt)
        t=ax.set_xticks(np.arange(0.0,32,4)-0.5,np.arange(0.0,32,4),minor=True)
        t=ax.set_yticks(np.arange(0.0,32,2)-0.5,np.arange(0.0,32,2),minor=True)
        t=ax.set_xticks(np.arange(0.0,32,4)-0.5,np.arange(0.0,32,4),fontsize=11)
        t=ax.set_yticks(np.arange(0.0,32,4)-0.5,np.arange(0.0,32,4),color='w',fontsize=9)
        ax2 = f.add_axes([ax.get_position().x1+0.005,ax.get_position().y0,0.02,ax.get_position().height])
        ax.grid(color='k',alpha=1,which='both')
        t=ax.text(15.2,12.3,"I$\eta$ = "+str(eta) ,fontweight='bold',c='r',alpha=1)
        t.set_bbox(dict(facecolor='w',alpha=1.0, edgecolor='r'))
        plt.colorbar(c,cax=ax2)
        if (eta%2)==0:
            ax.set_ylabel("comp. IEt")
        if eta>1:
            ax.set_xlabel("comp. NTT",fontsize=12)
    plt.savefig(f'{plots_dir}/LUT_eff{args.eff_min}_Emin{args.E_min}_Emax{args.E_max}.png')
    plt.savefig(f'{plots_dir}/LUT_eff{args.eff_min}_Emin{args.E_min}_Emax{args.E_max}.pdf')
    print(f"Saved figure : {plots_dir}/LUT_eff{str_effmin}_Emin{args.E_min}_Emax{args.E_max}.png'")



###############################################################################################################
################################# Writing the LUT #############################################
###############################################################################################################

k=0
LUT_TEXT="""\
# Tau isolation LUT from V9 optimization for effMin 0.9 Emin 16 Emax 60
# iso LUT structure is ieta --> iEt -->  nTT
# Compr bits: ieta: 2 iEt: 5 nTT: 5
#<header> V10.0 12 9 </header>

"""
for ieta in range(nEtaBins):
    for iet in range(nEtBins):
        for intt in range(nNTTBins):
            index=LUT_MAP['index'][k]
            thr=LUT_MAP['threshold'][k]
            
            ostr=f"{index} {thr} # ieta : iEt : inTT = {ieta} : {iet} : {intt}"
            LUT_TEXT+=ostr
            LUT_TEXT+="\n"
            k+=1

foutname=f"{LUTs_dir}/LUT_eff{str_effmin}_Emin{args.E_min}_Emax{args.E_max}.txt"
print(foutname)
with open(foutname,'w') as f:
    f.write(LUT_TEXT)

print(f"Saved LUT: {LUTs_dir}/LUT_eff{str_effmin}_Emin{args.E_min}_Emax{args.E_max}.txt")