#Script containing the functions to derive the Isolation LUT

import uproot,json,os
import ROOT
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import mplhep as hep
from scipy import stats
import itertools
import awkward as ak

def getXProjections(x_arr,y_arr,nMinProj=-1):
    xU = np.unique(x_arr)
    xVal     =[]     
    xProj    =[]
    xProj_err=[]
    for x in xU:
        mask= x_arr==x
        if np.sum(mask) < nMinProj:
            continue
        avg = np.average( y_arr[mask] )
        std = np.std( y_arr[mask] )
        xProj.append(avg)
        xProj_err.append(std)
        xVal.append(x)
    return {'x':np.array(xVal) , 'projection' : np.array(xProj) , 'stdev' : np.array(xProj_err) }

def getBinName(bin_dim_names,bin_bounds):
    name="_"
    for bn,bb in zip(bin_dim_names,bin_bounds):
        name+=f"{bn}IN{bb}_"
    return name.replace(" ","")

def getBinForValues(bin_values,bin_splits,bin_dim_names):
    bins=[]
    for i in bin_dim_names:
        if i not in bin_splits:
            print(f"Bin splits does not have {i}")
            raise KeyError
        if i not in bin_values:
            print(f"Bin values does not have {i}")
            raise KeyError
        v=bin_values[i]
        gotBin=False
        for bn in bin_splits[i]:
            if (v<=bn[1]) and (v>=bn[0]):
                bins.append(bn)
                gotBin=True
        if not gotBin:
            print(f"For dim {i}, value {v} did not find a bin match !")
            raise KeyError
    return bins

def getXProjectionsAtPercentiles(x_arr,y_arr,quantile=50,nMinProj=20):
    xU = np.unique(x_arr)
    xVal     =[]     
    xProj    =[]
    xProj_err_up=[]
    xProj_err_dn=[]
    for x in xU:
        mask= x_arr==x
        if np.sum(mask) < nMinProj:
            continue
        val=np.percentile(y_arr[mask],quantile)
        n=np.sum(mask)
        var=y_arr[mask]-val
        varU=var[var>=0]
        varD=var[var<0]
        std_u=np.sqrt(np.sum(varU*varU)/n)
        std_d=np.sqrt(np.sum(varD*varD)/n)
        xProj_err_up.append(std_u)
        xProj_err_dn.append(std_d)
        
        xProj.append(val)
        xVal.append(x)
        
    return {'x':np.array(xVal) , 'projection' : np.array(xProj) ,
            'stdev_u' : np.array(xProj_err_up) ,'stdev_d' : np.array(xProj_err_dn) }

def getEfficiency(ieta,iet):
    if iet <11.0: # 20 GeV ( iet = 40) , compressed et =12 
        return 0.6 #choose eff_min
    if iet <30.0: # 60 GeV ( iet = 120) , compressed et =30  
        return 0.6+0.4*(iet-11)/19 #(eff_min+(1-eff_min)*(iet-Et_min)/(Et_max-Et_min))
    return 1.0

def getEfficiency_args(ieta,iet,args):
    if iet <args.E_min: # 20 GeV ( iet = 40) , compressed et =12 
        return args.eff_min #choose eff_min
    if iet <args.E_max: # 60 GeV ( iet = 120) , compressed et =30  
        return args.eff_min+(1-args.eff_min)*(iet-args.E_min)/(args.E_max-args.E_min) #(eff_min+(1-eff_min)*(iet-Et_min)/(Et_max-Et_min))
    return 1.0

# From ieta to compressed ieta (eta=ieta/2)
# compressed ieta: 0 -> ieta 0
# compressed ieta: 1 -> ieta 15
# compressed ieta: 2 -> ieta 18
# compressed ieta: 3 -> ieta 21
# compressed ieta: 4 -> ieta 23
# compressed ieta: 5 -> ieta 25
# compressed ieta: 6 -> ieta 27
# compressed ieta: 7 -> ieta 29
# compressed ieta: 8 -> ieta 31 
# compressed ieta: 9 -> ieta 33 
# compressed ieta: 10 -> ieta 35
# compressed ieta: 11 -> ieta 37 37/2=18.5
# compressed ieta: 12 -> ieta 39
# compressed ieta: 13 -> ieta 41
# compressed ieta: 14 -> ieta 43
# compressed ieta: 15 -> ieta 45
# compressed ieta: 16 -> ieta 47 47/2=23.5
# compressed ieta: 17 -> ieta 50 50/2=25
# compressed ieta: 18 -> ieta 53
# compressed ieta: 19 -> ieta 56
# compressed ieta: 20 -> ieta 59
# compressed ieta: 21 -> ieta 62
# compressed ieta: 22 -> ieta 65
# compressed ieta: 23 -> ieta 69
# compressed ieta: 24 -> ieta 73
# compressed ieta: 25 -> ieta 77
# compressed ieta: 26 -> ieta 82
# compressed ieta: 27 -> ieta 88
# compressed ieta: 28 -> ieta 95
# compressed ieta: 29 -> ieta 105 
# compressed ieta: 30 -> ieta 120 60
# compressed ieta: 31 -> ieta 157 
# compressed ieta: 32 -> ieta 255 (this is just the maximum value, not a real eta bin) 125

def singleIEtSeed(data,iet):
    seed_cond=  data['l1tEmuPt'] >= iet
    return ak.sum(seed_cond,axis=1) > 0

def singleIEtIsoSeed(data,iet):
    seed_cond = data['is_iso'] & (data['l1tEmuPt'] >= iet)
    return ak.sum(seed_cond,axis=1) > 0


def doubleIEtSeed(data,iet1,iet2):
    ltau1 = data['l1tEmuPt'] >= iet1
    leg2 = data['l1tEmuPt'] >= iet2
    nSeedPass= ( ak.sum(leg1,axis=1) > 0) & ( ak.sum(leg2,axis=1) > 1)
    return nSeedPass

def doubleIEtSingleIsoSeed(data,iet1,iet2):
    leg1 = (data['l1tEmuPt'] >= iet1) & data['is_iso']
    leg2 = (data['l1tEmuPt'] >= iet2) 
    nSeedPass= ( ak.sum(leg1,axis=1) > 0) & ( ak.sum(leg2,axis=1) > 1)
    return nSeedPass


# To compute it with the 2 highest pt taus like in the previous code, but in principle, it is not necessary

# def doubleIEtDoubelIsoSeed(data,iet1,iet2):
#     pt = data['l1tEmuPt']
#     iso = data['is_iso']
#     #select the 2 highest pt taus
#     # Keep only isolated taus
#     pt_iso = pt[iso]
#     print(pt_iso)
#     # Sort isolated taus by pt descending
#     pt_iso_sorted = ak.sort(pt_iso, axis=1, ascending=False)
#     lead1 = ak.firsts(pt_iso_sorted)
#     lead2 = ak.firsts(pt_iso_sorted[:, 1:])
#     print(lead1)
#     print(lead2)

#     # Apply pt threshold on each
#     pass1 = lead1 >= iet1
#     pass2 = lead2 >= iet2

#     nSeedPass= pass1 & pass2

#     #Sanity check without using the 2 highest pt taus

#     leg1 = (data['l1tEmuPt'] >= iet1) & data['is_iso']
#     leg2 = (data['l1tEmuPt'] >= iet2) & data['is_iso']
#     nSeedlgPass= ( ak.sum(leg1,axis=1) > 0) & ( ak.sum(leg2,axis=1) > 1)

#     return nSeedPass

def doubleIEtDoubelIsoSeed(data,iet1,iet2):
    leg1 = (data['l1tEmuPt'] >= iet1) & data['is_iso']
    leg2 = (data['l1tEmuPt'] >= iet2) & data['is_iso']
    nSeedPass= ( ak.sum(leg1,axis=1) > 0) & ( ak.sum(leg2,axis=1) > 1)
    return nSeedPass