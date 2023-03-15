import numpy as np
import ROOT
import sys
import os

import matplotlib.pyplot as plt
import matplotlib
import mplhep
plt.style.use(mplhep.style.CMS)



inFile = ROOT.TFile('/home/llr/cms/motta/Run3preparation/CMSSW_11_0_2/src/TauObjectsOptimization/Isolate/ROOTs4LUTs/ROOTs4LUTs_2023/LUTisolation_Trigger_Stage2_Run3_MC_optimizationVx_calibThr1p7.root')

# Eff = inFile.Get('Eff_64')

# x_eta = []
# y_ene = []
# z_ntt = []
# iso_cut = []
# size = []

# for ieta in range(1, Eff.GetNbinsX()+1):
#     for ie in range(1, Eff.GetNbinsY()+1):
#         for intt in range(1, Eff.GetNbinsZ()+1):
#             x_eta.append(ieta)
#             y_ene.append(ie)
#             z_ntt.append(intt)
#             iso_cut.append(Eff.GetBinContent(ieta, ie, intt))
#             size.append(Eff.GetBinContent(ieta, ie, intt)*10)



# cmap = 'Set1'

# fig = plt.figure(figsize = (15, 15))
# ax = fig.add_subplot(111, projection='3d')
# img = ax.scatter(x_eta, y_ene, z_ntt, s=size, c=iso_cut, marker='o', cmap='viridis', alpha=1)
# ax.set_xlabel('iEta', labelpad=25)
# ax.set_ylabel('iE', labelpad=25)
# ax.set_zlabel('inTT', labelpad=25)
# ax.view_init(elev=20., azim=-30)
# colorbar = plt.colorbar(img, location='bottom', orientation='horizontal')
# colorbar.set_label(label=r'IsoEt cut')
# # mplhep.cms.label('Preliminary', data=True, rlabel=r'34 fb$^{-1}$ (13.6 TeV)')
# plt.savefig('./Eff_64.pdf')
# plt.close()



# Hist_0_15_7 = inFile.Get('Hist_0_15_7')
# x = []
# y = []
# err_y = []
# for i in range(1, int(Hist_0_15_7.GetNbinsX())+1):
#     x.append(i)
#     y.append(Hist_0_15_7.GetBinContent(i))
#     err_y.append(Hist_0_15_7.GetBinError(i))

# fig = plt.figure(figsize = (15, 15))
# plt.errorbar(x, y, yerr=err_y, ls='None', label='Number of entries='+str(int(np.sum(y))), lw=2, marker='o', color='black')
# leg = plt.legend(loc='upper right', fontsize=30, title="56 < nTT < 61")
# leg._legend_box.align = "left"
# plt.xlabel('IsoEt')
# plt.ylabel('a.u.')
# # mplhep.cms.label('Preliminary', data=True, rlabel=r'34 fb$^{-1}$ (13.6 TeV)')
# plt.savefig('./Hist_0_15_7.pdf')
# plt.close()



# Hist_0_15_14 = inFile.Get('Hist_0_15_14')
# x = []
# y = []
# err_y = []
# for i in range(1, int(Hist_0_15_14.GetNbinsX())+1):
#     x.append(i)
#     y.append(Hist_0_15_14.GetBinContent(i))
#     err_y.append(Hist_0_15_14.GetBinError(i))

# fig = plt.figure(figsize = (15, 15))
# plt.errorbar(x, y, yerr=err_y, ls='None', label='Number of entries='+str(int(np.sum(y))), lw=2, marker='o', color='black')
# leg = plt.legend(loc='upper right', fontsize=30, title="106 < nTT < 255")
# leg._legend_box.align = "left"
# plt.xlabel('IsoEt')
# plt.ylabel('a.u.')
# # mplhep.cms.label('Preliminary', data=True, rlabel=r'34 fb$^{-1}$ (13.6 TeV)')
# plt.savefig('./Hist_0_15_14.pdf')
# plt.close()


isoEt_vs_nTT_profile = inFile.Get('iso_vs_compressednTT_profile')
x = []
y = []
err_y = []
for i in range(1, isoEt_vs_nTT_profile.GetNbinsX()+1):
    x.append(i)
    y.append(isoEt_vs_nTT_profile.GetBinContent(i))
    err_y.append(isoEt_vs_nTT_profile.GetBinError(i))

fit = inFile.Get('iso_vs_compressednTT_fit')
m = fit.GetParameter(1)
q = fit.GetParameter(0)
x_line = np.linspace(0,32,33)
y_line = x_line*m + q

fig = plt.figure(figsize = (15, 15))
plt.errorbar(x, y, yerr=err_y, ls='None', lw=2, marker='o', color='black')
plt.plot(x_line, y_line, ls='-', color='red')
plt.ylabel('IsoEt')
plt.xlabel('Compressed nTT')
# mplhep.cms.label('Preliminary', data=True, rlabel=r'34 fb$^{-1}$ (13.6 TeV)')
plt.savefig('./isoEt_vs_nTT_profile.pdf')
plt.close()




# pz_64_eta1_e0 = inFile.Get('pz_72_eta1_e11')
# x = []
# y = []
# err_y = []
# for i in range(1, pz_64_eta1_e0.GetNbinsX()+1):
#     x.append(i)
#     y.append(pz_64_eta1_e0.GetBinContent(i))
#     err_y.append(pz_64_eta1_e0.GetBinError(i))

# fit = inFile.Get('fit_pz_64_eta1_e0')
# m = fit.GetParameter(1)
# q = fit.GetParameter(0)
# x_line = np.linspace(0,15,16)
# y_line = x_line*m + q

# fig = plt.figure(figsize = (15, 15))
# plt.errorbar(x, y, yerr=err_y, ls='None', lw=2, marker='o', color='black')
# plt.plot(x_line, y_line, ls='-', color='red')
# plt.ylabel('IsoEt')
# plt.xlabel('nTT')
# # mplhep.cms.label('Preliminary', data=True, rlabel=r'34 fb$^{-1}$ (13.6 TeV)')
# plt.savefig('./pz_64_eta1_e0.pdf')
# plt.close()


