from scipy.optimize import curve_fit
from scipy.signal import convolve
from optparse import OptionParser
import scipy.special as sp
import numpy as np
import ROOT
import sys
import os

from matplotlib.ticker import FixedLocator, FixedFormatter
import matplotlib.pyplot as plt
import matplotlib
import mplhep
plt.style.use(mplhep.style.CMS)


# ###################################################################################################################################################################################################################################################################################################################################################
# FUNCTIONS USED BOTH FOR TAU AND EG TURNONS

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# APPROXIMATE ERROR FUNCTION
def ApproxErf(arg):
    erflim = 5.0;
    if arg > erflim:
        return 1.0
    if arg < -erflim:
        return -1.0

    return sp.erf(arg)

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CUMULATIVE CRYSTAL-BALL : this is the most important part of the turnon and can be used for both eg and tau
def CB(x, mean=1, sigma=1, alpha=1, n=1, norm=1):
    pi = np.pi
    sqrt2 = np.sqrt(2)
    sqrtPiOver2 = np.sqrt(np.pi / 2)

    # Variable std deviation
    sig = abs(sigma)
    t = (x - mean)/sig
    if alpha < 0:
        t = -t

    # Crystal Ball part
    absAlpha = abs(alpha)
    A = pow(n / absAlpha, n) * np.exp(-0.5 * absAlpha * absAlpha)
    B = absAlpha - n / absAlpha
    C = n / absAlpha * np.exp(-0.5*absAlpha*absAlpha) / (n - 1)
    D = (1 + ApproxErf(absAlpha / sqrt2)) * sqrtPiOver2
    N = norm / (D + C)

    if t <= absAlpha:
        crystalBall = N * (1 + ApproxErf( t / sqrt2 )) * sqrtPiOver2
    else:
        crystalBall = N * (D +  A * (1/pow(t-B,n-1) - 1/pow(absAlpha - B,n-1)) / (1 - n))

    return crystalBall

vectCB = np.vectorize(CB)
##            [   mean, sigma, alpha,    n, norm]
# p0 =          [thr    ,    3.,   3. , 100., 0.95]
# param_bounds=([thr-10.,    1.,   0.1,   1., 0.9 ],
#               [thr+10.,   10.,  10. , 200., 1.  ])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CRYSTAL-BALL CDF TRUNCATED IN THE LOW TAIL : this can be used to convolute with some other low tail function
def ApproxCB(x, xcut=1, mean=1, sigma=1, alpha=1, n=1, norm=1,  p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1):
    if x <= xcut:
        # cb = x * CB(xcut, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm) / xcut # LINEAR APPROXIMATION
        cb = CB(xcut, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm)            # CONSTANT APPROXIMATION
    if x > xcut:
        cb = CB(x, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm)

    return cb

vectApproxCB = np.vectorize(ApproxCB)
##            [   mean, sigma, alpha,    n, norm]
# p0 =          [thr    ,    3.,   3. , 100., 0.95]
# param_bounds=([thr-10.,    1.,   0.1,   1., 0.9 ],
#               [thr+10.,   10.,  10. , 200., 1.  ])


# ###################################################################################################################################################################################################################################################################################################################################################
# FUNCTIONS USED ONLY FOR TAU TURNONS

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# ARCTAN TRUNCATED IN THE LOW TAIL : used with the CB CDF fits well and almost out of the box the tau turnons
def ApproxATAN(x, xturn=1, p=1, width=1):
    pi = np.pi

    # Arctan part
    arctan = 0.
    if x < xturn:
        arctan = p
    if x >= xturn:
        arctan = pow(ApproxErf((x - xturn) / 5.), 2) * 2. * (1. - p) / pi * np.arctan(pi / 80. * width * (x - xturn)) + p

    return arctan

vectApproxATAN = np.vectorize(ApproxATAN)

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF CB CDF AND ApproxATAN: works pretty well and out of the box to fit tau turnons
def CBconvATAN(x, mean=1, sigma=1, alpha=1, n=1, norm=1, xturn=1, p=1, width=1):
    return convolve(vectCB(x, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm), vectApproxATAN(x, xturn=xturn, p=p, width=width), mode='full', method='direct')

vectCBconvATAN = np.vectorize(CBconvATAN)
##            [   mean, sigma, alpha,    n,     norm, xturn,   p, width]
# p0 =          [thr    ,    3.,   3. , 100.,     0.95,   10., 0.8,   10.]
# param_bounds=([thr-10.,    1.,   0.1,   1.,     0.9 ,    0., 0.2,    1.],
#               [thr+10.,   10.,  10. , 200.,     1.  ,   thr, 1. ,  100.])


# ###################################################################################################################################################################################################################################################################################################################################################
# FUNCTIONS USED ONLY FOR EG TURNONS

def ATAN(x, a=1, b=1, c=1):
    return a*np.arctan(x-c)+b

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF AN ATAN WITH AN APPROXIAMTE ATAN: fits very well the low threshold eg turnons
def ATANconvApproxATAN(x, a=1, b=1, c=1,  xturn=1, p=1, width=1):
    return convolve(ATAN(x, a=a, b=b, c=c), vectApproxATAN(x, xturn=xturn, p=p, width=width), mode='full', method='direct')

vectATANconvApproxATAN = np.vectorize(ATANconvApproxATAN)

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# LOW TAIL POLYNOMIAL CONTINUATED WITH CRYSTAL-BALL CDF (the continuation is not smooth) : this fits the eg turnons pretty well
def CBtimesPOLYlow(x, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1, mean=1, sigma=1, alpha=1, n=1, norm=1):
    # Poly part for the low energy tail
    poly = 0.
    if x >= xturn:
        poly = p + (slope * (xturn-inflation) + square * pow(xturn-inflation,2) + cube * pow(xturn-inflation,3)) / scale + CB(x, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm)
    if x < xturn:
        poly = p + (slope * (x-inflation) + square * pow(x-inflation,2) + cube * pow(x-inflation,3)) / scale

    return poly

vectCBtimesPOLYlow = np.vectorize(CBtimesPOLYlow)
##            [xturn,    p, slope, square, cube, inflation, scale, integral,  ct        mean, sigma, alpha,    n, norm]
# p0 =          [20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.,    thr    ,    3.,   3. , 100., 0.95]
# param_bounds=([5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.,    thr-10.,    1.,   0.1,   1., 0.9 ],
#               [35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.,    thr+10.,   10.,  10. , 200., 1.  ])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# POLYNOMIAL TO FIT HIGHER TAIL OF THE TURNONS : works pretty well when convoluted with CB CDF and fitting eg turnons only above turnon point
def POLYhigh(x, xturn=1, p=1, slope=1, square=1, cube=1):
    # Poly part for the high energy tail
    poly = 0.
    if x <= xturn:
        poly = p
    if x > xturn:
        poly = p + slope * (x-xturn) + square * pow(x-xturn,2) + cube * pow(x-xturn,3)

    return poly

vectPOLYhigh = np.vectorize(POLYhigh)

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# POLYNOMIAL TO FIT LOWER TAIL OF THE TURNONS AND ERF TURNON CORE : works pretty well alone when fitting eg turnons only below ~50GeV (below the POLYhigh would kick in)
def POLYlow(x, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1):
    # Poly part for the low energy tail
    poly = 0.
    if x >= xturn:
        poly = p + (slope * (xturn-inflation) + square * pow(xturn-inflation,2) + cube * pow(xturn-inflation,3)) / scale + pow(integral*ApproxErf((x-xturn)/ct),2)
    if x < xturn:
        poly = p + (slope * (x-inflation) + square * pow(x-inflation,2) + cube * pow(x-inflation,3)) / scale

    return poly

vectPOLYlow = np.vectorize(POLYlow)
##            [xturn,    p, slope, square, cube, inflation, scale, integral,  ct]
# p0 =          [20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.]
# param_bounds=([5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.],
#               [35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF LOW TAIL POLYNOMIAL CONTINUATED WITH CRYSTAL-BALL CDF AND  ApproxATAN: works ok to fit the low threhsold eg turnons but has a sharp edge at the continuation point
def CBtimesPOLYlowConvATAN(x, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1, mean=1, sigma=1, alpha=1, n=1, norm=1, xturn1=1, p1=1, width1=1):
    return convolve(vectCBtimesPOLYlow(x, xturn=xturn, p=p, slope=slope, square=square, cube=cube, inflation=inflation, scale=scale, integral=integral, ct=ct, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm), vectApproxATAN(x, xturn=xturn1, p=p1, width=width1), mode='full', method='direct')

vectCBtimesPOLYlowConvATAN = np.vectorize(CBtimesPOLYlowConvATAN)
##            [xturn,    p, slope, square, cube, inflation, scale, integral,  ct        mean, sigma, alpha,    n, norm, xturn,   p, width]
# p0 =          [20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.,    thr    ,    3.,   3. , 100., 0.95,   30., 0.8,   10.]
# param_bounds=([5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.,    thr-10.,    1.,   0.1,   1., 0.9 ,   25., 0.2,    1.],
#               [35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.,    thr+10.,   10.,  10. , 200., 1.  ,   40., 1. ,  100.])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF CB CDF AND POLYNOMIAL FOR HIGHER TAIL : works pretty well when fitting eg turnons only above turnon point
def CBconvPOLYhigh(x, mean=1, sigma=1, alpha=1, n=1, norm=1, xturn=1, p=1, slope=1, square=1, cube=1):
    return convolve(vectCB(x, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm), vectPOLYhigh(x, xturn=xturn, p=p, slope=slope, square=square, cube=cube), mode='full', method='direct')

vectCBconvPOLYhigh = np.vectorize(CBconvPOLYhigh)
##            [   mean, sigma, alpha,    n,   norm,          xturn,     p,  slope, square, cube]
# p0 =          [thr    ,    3.,   3. , 100., 0.95,            50.,   0.8,    0.5,     0.,   0.]
# param_bounds=([thr-10.,    1.,   0.1,   1., 0.9 ,            45.,   0.2,    0. ,    -1.,  -1.],
#               [thr+10.,   10.,  10. , 200., 1.  ,            55.,   1. ,    1. ,     1.,   1.])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF CB CDF AND POLYNOMIAL FOR LOWER TAIL : this one does not work due to the fact that 1. there is both and erf and a cb cdf for the higher tail , 2. the low tail gets sent to zero by the cb cdf
def CBconvPOLYlow(x, mean=1, sigma=1, alpha=1, n=1, norm=1, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1):
    return convolve(vectCB(x, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm), vectPOLYlow(x, xturn=xturn, p=p, slope=slope, square=square, cube=cube, inflation=inflation, scale=scale, integral=integral, ct=ct), mode='full', method='direct')

vectCBconvPOLYlow = np.vectorize(CBconvPOLYlow)
##            [   mean, sigma, alpha,    n,   norm,          xturn,    p, slope, square, cube, inflation, scale, integral,  ct]
# p0 =          [thr    ,    3.,   3. , 100., 0.95,            20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.]
# param_bounds=([thr-10.,    1.,   0.1,   1., 0.9 ,            5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.],
#               [thr+10.,   10.,  10. , 200., 1.  ,            35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF APPROXIMATE CB CDF AND POLYNOMIAL FOR LOWER TAIL : this function fits relatively well the turnon and  the high part BUT the fact that the lower part has a CB truncation makes the fit fail in the low tail
def ApproxCBconvPOLYlow(x, xcut=1, mean=1, sigma=1, alpha=1, n=1, norm=1, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1):
    return convolve(vectApproxCB(x, xcut=xcut, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm), vectPOLYlow(x, xturn=xturn, p=p, slope=slope, square=square, cube=cube, inflation=inflation, scale=scale, integral=integral, ct=ct), mode='full', method='direct')

vectApproxCBconvPOLYlow = np.vectorize(ApproxCBconvPOLYlow)
##            [xcut,    mean, sigma, alpha,    n,   norm,          xturn,    p, slope, square, cube, inflation, scale, integral,  ct]
# p0 =          [ 25., thr    ,    3.,   3. , 100., 0.95,            20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.]
# param_bounds=([ 20., thr-10.,    1.,   0.1,   1., 0.9 ,            5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.],
#               [ 30., thr+10.,   10.,  10. , 200., 1.  ,            35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF POLYNOMIAL TO FIT LOWER TAIL OF THE TURNONS WITH ERF TURNON CORE AND POLYNOMIAL FOR HIGHER TAIL : this gets the shape of the function but has too sharp corners that do not make much sense
def POLYlowConvPOLYhigh(x, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1, xturn1=1, p1=1, slope1=1, square1=1, cube1=1):
    return convolve(vectPOLYlow(x, xturn=xturn, p=p, slope=slope, square=square, cube=cube, inflation=inflation, scale=scale, integral=integral, ct=ct), vectPOLYhigh(x, xturn=xturn1, p=p1, slope=slope1, square=square1, cube=cube1), mode='full', method='direct')

vectPOLYlowConvPOLYhigh = np.vectorize(POLYlowConvPOLYhigh)
##            [xturn,    p, slope, square, cube, inflation, scale, integral,  ct,        xturn1,     p1,  slope1, square1, cube1]
# p0 =          [20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.,           50.,    0.8,     0.5,      0.,    0.]
# param_bounds=([5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.,           45.,    0.2,     0. ,     -1.,   -1.],
#               [35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.,           55.,    1. ,     1. ,      1.,    1.])

# ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
# CONVOLUTION OF LOW TAIL POLYNOMIAL CONTINUATED WITH CRYSTAL-BALL CDF AND POLYNOMIAL FOR HIGHER TAIL : this does not work like this
def CBtimesPOLYlowConvPOLYhigh(x, xturn=1, p=1, slope=1, square=1, cube=1, inflation=1, scale=1, integral=1, ct=1,    mean=1, sigma=1, alpha=1, n=1, norm=1,   xturn1=1, p1=1, slope1=1, square1=1, cube1=1):
    return convolve(vectCBtimesPOLYlow(x, xturn=xturn, p=p, slope=slope, square=square, cube=cube, inflation=inflation, scale=scale, integral=integral, ct=ct, mean=mean, sigma=sigma, alpha=alpha, n=n, norm=norm), vectPOLYhigh(x, xturn=xturn1, p=p1, slope=slope1, square=square1, cube=cube1), mode='full', method='direct')

vectCBtimesPOLYlowConvPOLYhigh = np.vectorize(CBtimesPOLYlowConvPOLYhigh)
##            [xturn,    p, slope, square, cube, inflation, scale, integral,  ct        mean, sigma, alpha,    n, norm,        xturn1,     p1,  slope1, square1, cube1]
# p0 =          [20.,   0.05, 0.005,     0.,   0.,        15,   1E5,      0.8,  3.,    thr    ,    3.,   3. , 100., 0.95,           50.,    0.8,     0.5,      0.,    0.]
# param_bounds=([5. ,   0.  , 0.   ,    -1.,  -2.,        10,   1E4,      0.6,  1.,    thr-10.,    1.,   0.1,   1., 0.9 ,           45.,    0.2,     0. ,     -0.00001,   -0.00001],
#               [35.,   0.1 , 0.01 ,     1.,   2.,        20,   1E6,      1. , 10.,    thr+10.,   10.,  10. , 200., 1.  ,           55.,    1. ,     1. ,      0.00001,   0.00001])

def compute_TurnOn(eff_TGraph):
    x = []
    y = []
    x_err = []
    y_errU = []
    y_errD = []
    y_errFit = []

    for ibin in range(eff_TGraph.GetN()):
        x.append(eff_TGraph.GetPointX(ibin))
        y.append(eff_TGraph.GetPointY(ibin))
        x_err.append(eff_TGraph.GetErrorX(ibin))
        
        errU = eff_TGraph.GetErrorYhigh(ibin)
        errD = eff_TGraph.GetErrorYlow(ibin)
        y_errU.append(errU)
        y_errD.append(errD)

        if errU > errD:
            y_errS = errD
            y_errL = errU
        else:
            y_errS = errU
            y_errL = errD
        if options.smallFitErr: y_errFit.append(y_errS)
        else:                   y_errFit.append(y_errL)
        # print("y",y)
    return x, y, x_err, [y_errD, y_errU] 


def plot_TurnOn(eff_TGraph, thr, label, color, ax, options):
    plt.ylim(0.000, 1.05)
    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1')
    plot_x = np.linspace(0,1000,2000)
    markers = ['o', 's', '^', 'D', 'v', '*', 'p', 'X', 'h']
    marker=markers[color]
    plt.rcParams['legend.title_fontsize'] = 'xx-small'
    # pt_leg = ax.legend(title=r'ReEmu-2024I-HCALcFeb-ZS2025-effMin0p9_eMin22_eMax37', loc='lower right', fontsize=15)
    x, y, x_err, y_err = compute_TurnOn(eff_TGraph)
    ax.errorbar(x, y, xerr=x_err,yerr=y_err, ls='None', label=label, lw=2, marker=marker, color=cmap(color), zorder=color+1)
    # pt_leg._legend_box.align = "right"
    ##vectCBconvATAN unpacked 2025 d 24
    p0 =          [thr+25  ,  9.,   6. , 180.,     0.94,   30., 0.7,   80.]
    param_bounds=([thr-25.,    1.,   0.1,  70.,     0.9 ,    0., 0.2,   10.],
                  [thr+25.,    10.,  10. ,180.,     1.  ,  110., 1. ,   100.])
    print(x[:-2], y[:-2])
    popt, pcov = curve_fit(vectCBconvATAN, x, y, p0, maxfev=5000, bounds=param_bounds)
    print(popt)

        #             [   mean, sigma, alpha,    n,     norm, xturn,   p, width]
    # p0 =          [thr+15  ,  1.,   3. , 80.,     0.95,   3., 0.8,   80.]
    # param_bounds=([thr-20.,    1.,   0.1,  70.,     0.9 ,    0., 0.2,   10.],
    #               [thr+20.,    10.,  10. ,180.,     1.  ,  110., 1. ,   100.])

    ax.plot(plot_x, vectCBconvATAN(plot_x, *popt), '-', label='_', lw=2, color=cmap(color), zorder=color+1)
        
    if options.logx:
        plt.xlim(10., 500.)
        plt.xscale('log')
        ax.xaxis.set_major_locator(FixedLocator([10, 100, 500]))
        ax.xaxis.set_major_formatter(FixedFormatter(['10',r'$100$', r'$500$']))
    else:
        plt.xlim(15., 150.)
    # leg = plt.legend(loc = 'lower right', fontsize=16, title=r'$|\eta^{\tau, offline}|<2.1$'+"\n"+r'2024 Era I ReEmu w/ 2025 conditions')
    leg = plt.legend(loc = 'lower right', fontsize=16, title=r'$|\eta^{\tau, offline}|<2.1$')
    leg._legend_box.align = "left"
    plt.xlabel(r'$p_{T}^{\tau, offline}\ [GeV]$')
    plt.ylabel(r'Efficiency')
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'2025 - 13.6 TeV')

def plot_TurnOn3(eff_TGraph, thr, label, color, ax, options):
    plt.ylim(0.000, 1.05)
    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1')
    plot_x = np.linspace(0,1000,2000)
    markers = ['o', 's', '^', 'D', 'v', '*', 'p', 'X', 'h']
    marker=markers[color]
    plt.rcParams['legend.title_fontsize'] = 'xx-small'
    # pt_leg = ax.legend(title=r'ReEmu-2024I-HCALcFeb-ZS2025-effMin0p9_eMin22_eMax37', loc='lower right', fontsize=15)
    x, y, x_err, y_err = compute_TurnOn(eff_TGraph)

    ax.errorbar(x, y, xerr=x_err,yerr=y_err, ls='None', label=label, lw=2, marker=marker, color=cmap(color), zorder=color+1)
    # pt_leg._legend_box.align = "right"
     #vectCBconvATAN for unpacked 2025 26
    #             [   mean, sigma, alpha,    n,     norm, xturn,   p, width]
    p0 =          [thr+10  ,  2.,   2. , 80.,     0.95,   30., 0.4,   80.]
    param_bounds=([thr-15.,    1.,   0.1,  70.,     0.9 ,    0., 0.2,   10.],
                  [thr+15.,    10.,  10. ,180.,     1.  ,  110., 1. ,   100.])
    # p0 =          [thr+10  ,  2.,   0.3 , 90.,     0.97,   80., 0.3,   70.]
    # param_bounds=([thr-20.,    1.,   0.1,  70.,     0.9 ,    0., 0.2,   10.],
    #               [thr+20.,    10.,  10. ,180.,     1.  ,  110., 1. ,   100.])
    # print(x[:-2], y[:-2])
    popt, pcov = curve_fit(vectCBconvATAN, x[:-2], y[:-2], p0, maxfev=5000, bounds=param_bounds)
    print(popt)

    #  ##vectCBconvATAN for unpacked 2025 32
    # #             [   mean, sigma, alpha,    n,     norm, xturn,   p, width]
    # p0 =          [thr+10  ,  1.,   5. , 80.,     0.95,   70., 0.8,   80.]
    # param_bounds=([thr-20.,    1.,   0.1,  70.,     0.9 ,    0., 0.2,   10.],
    #               [thr+20.,    10.,  10. ,180.,     1.  ,  110., 1. ,   100.])
    # print(x[:-2], y[:-2])
    # popt, pcov = curve_fit(vectCBconvATAN, x[:-2], y[:-2], p0, maxfev=5000, bounds=param_bounds)
    # print(popt)
    #  ##vectCBconvATAN for 
    # #             [   mean, sigma, alpha,    n,     norm, xturn,   p, width]
    # p0 =          [thr+10  ,  1.,   3. , 80.,     0.95,   80., 0.9,   80.]
    # param_bounds=([thr-15.,    1.,   0.1,  70.,     0.9 ,    0., 0.2,   10.],
    #               [thr+15.,    10.,  10. ,180.,     1.  ,  110., 1. ,   100.])
    # print(x[:-2], y[:-2])
    # popt, pcov = curve_fit(vectCBconvATAN, x[:-2], y[:-2], p0, maxfev=5000, bounds=param_bounds)
    # print(popt)
    ax.plot(plot_x, vectCBconvATAN(plot_x, *popt), '-', label='_', lw=2, color=cmap(color), zorder=color+1)
        
    if options.logx:
        plt.xlim(10., 500.)
        plt.xscale('log')
        ax.xaxis.set_major_locator(FixedLocator([10, 100, 500]))
        ax.xaxis.set_major_formatter(FixedFormatter(['10',r'$100$', r'$500$']))
    else:
        plt.xlim(15., 150.)
    # leg = plt.legend(loc = 'lower right', fontsize=16, title=r'$|\eta^{\tau, offline}|<2.1$'+"\n"+r'2024 Era I ReEmu w/ 2025 conditions')
    leg = plt.legend(loc = 'lower right', fontsize=16, title=r'$|\eta^{\tau, offline}|<2.1$')
    leg._legend_box.align = "left"
    plt.xlabel(r'$p_{T}^{\tau, offline}\ [GeV]$')
    plt.ylabel(r'Efficiency')
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'2025 - 13.6 TeV')

def plot_TurnOn4(eff_TGraph, thr, label, color, ax, options):
    plt.ylim(0.000, 1.05)
    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1')
    plot_x = np.linspace(0,1000,2000)
    markers = ['o', 's', '^', 'D', 'v', '*', 'p', 'X', 'h']
    marker=markers[color]
    plt.rcParams['legend.title_fontsize'] = 'xx-small'
    # pt_leg = ax.legend(title=r'ReEmu-2024I-HCALcFeb-ZS2025-effMin0p9_eMin22_eMax37', loc='lower right', fontsize=15)
    x, y, x_err, y_err = compute_TurnOn(eff_TGraph)

    ax.errorbar(x, y, xerr=x_err,yerr=y_err, ls='None', label=label, lw=2, marker=marker, color=cmap(color), zorder=color+1)
    # pt_leg._legend_box.align = "right"
    #vectCBconvATAN for unpacked 2025 26
    #             [   mean, sigma, alpha,    n,     norm, xturn,   p, width]
    p0 =          [thr+25  ,  9.,   6. , 80.,     0.94,   30., 0.7,   80.]
    param_bounds=([thr-25.,    1.,   0.1,  70.,     0.9 ,    0., 0.2,   10.],
                  [thr+25.,    10.,  10. ,180.,     1.  ,  110., 1. ,   100.])
    print(x[:-2], y[:-2])
    popt, pcov = curve_fit(vectCBconvATAN, x[:-2], y[:-2], p0, maxfev=5000, bounds=param_bounds)
    print(popt)

    #  ##vectCBconvATAN for unpacked 2025 32
    # #             [   mean, sigma, alpha,    n,     norm, xturn,   p, width]
    # p0 =          [thr+10  ,  1.,   5. , 80.,     0.95,   70., 0.8,   80.]
    # param_bounds=([thr-20.,    1.,   0.1,  70.,     0.9 ,    0., 0.2,   10.],
    #               [thr+20.,    10.,  10. ,180.,     1.  ,  110., 1. ,   100.])
    # print(x[:-2], y[:-2])
    # popt, pcov = curve_fit(vectCBconvATAN, x[:-2], y[:-2], p0, maxfev=5000, bounds=param_bounds)
    # print(popt)
    #  ##vectCBconvATAN for new LUT 32
    # #             [   mean, sigma, alpha,    n,     norm, xturn,   p, width]
    # p0 =          [thr+15  ,  1.,   3. , 80.,     0.95,   30., 0.9,   80.]
    # param_bounds=([thr-20.,    1.,   0.1,  70.,     0.9 ,    0., 0.2,   10.],
    #               [thr+20.,    10.,  10. ,180.,     1.  ,  110., 1. ,   100.])
    # print(x[:-2], y[:-2])
    # popt, pcov = curve_fit(vectCBconvATAN, x[:-2], y[:-2], p0, maxfev=5000, bounds=param_bounds)
    # print(popt)

      # pt_leg._legend_box.align = "right"
     #vectCBconvATAN for grey LUT 26
    #             [   mean, sigma, alpha,    n,     norm, xturn,   p, width]
    # p0 =          [thr+5  ,  8.,   8. , 80.,     0.95,   100., 0.3,   80.]
    # param_bounds=([thr-15.,    1.,   0.1,  70.,     0.9 ,    0., 0.2,   10.],
    #               [thr+15.,    10.,  10. ,180.,     1.  ,  110., 1. ,   100.])
    # print(x[:-2], y[:-2])
    # popt, pcov = curve_fit(vectCBconvATAN, x[:-2], y[:-2], p0, maxfev=5000, bounds=param_bounds)
    # print(popt)

    ax.plot(plot_x, vectCBconvATAN(plot_x, *popt), '-', label='_', lw=2, color=cmap(color), zorder=color+1)
        
    if options.logx:
        plt.xlim(10., 500.)
        plt.xscale('log')
        ax.xaxis.set_major_locator(FixedLocator([10, 100, 500]))
        ax.xaxis.set_major_formatter(FixedFormatter(['10',r'$100$', r'$500$']))
    else:
        plt.xlim(15., 150.)
    # leg = plt.legend(loc = 'lower right', fontsize=16, title=r'$|\eta^{\tau, offline}|<2.1$'+"\n"+r'2024 Era I ReEmu w/ 2025 conditions')
    leg = plt.legend(loc = 'lower right', fontsize=16, title=r'$|\eta^{\tau, offline}|<2.1$')
    leg._legend_box.align = "left"
    plt.xlabel(r'$p_{T}^{\tau, offline}\ [GeV]$')
    plt.ylabel(r'Efficiency')
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'2025 - 13.6 TeV')

def plot_TurnOn2(eff_TGraph, thr, label, color, ax, options):
    plt.ylim(0.000, 1.05)
    plt.rcParams['legend.title_fontsize'] = 'small'
    cmap = matplotlib.cm.get_cmap('Set1')
    plot_x = np.linspace(0,1000,2000)
    markers = ['o', 's', '^', 'D', 'v', '*', 'p', 'X', 'h']
    marker=markers[color]
    plt.rcParams['legend.title_fontsize'] = 'xx-small'
    # pt_leg = ax.legend(title=r'ReEmu-2024I-HCALcFeb-ZS2025-effMin0p9_eMin22_eMax37', loc='lower right', fontsize=15)
    x, y, x_err, y_err = compute_TurnOn(eff_TGraph)
    ax.errorbar(x, y, xerr=x_err,yerr=y_err, ls='None', label=label, lw=2, marker=marker, color=cmap(color), zorder=color+1)
    # pt_leg._legend_box.align = "right"
    # ##vectCBconvATAN
    # #             [   mean, sigma, alpha,    n,     norm, xturn,   p, width]
    # p0 =          [thr+15  ,  1.,   2. , 80.,     0.95,   10., 0.8,   80.]
    # param_bounds=([thr-20.,    1.,   0.1,  70.,     0.9 ,    0., 0.2,   10.],
    #               [thr+20.,    10.,  10. ,180.,     1.  ,  110., 1. ,   100.])
    # print(x[:-2], y[:-2])
    # i_to_skip=[8,9]
    # x_fit = np.delete(x, i_to_skip)
    # y_fit = np.delete(y, i_to_skip)
    # popt, pcov = curve_fit(vectCBconvATAN, x_fit, y_fit, p0, maxfev=5000, bounds=param_bounds)
    # print(popt)

    # ##vectCBconvATAN for unpacked 2025 fixed rate
    # #             [   mean, sigma, alpha,    n,     norm, xturn,   p, width]
    # p0 =          [thr+20  ,  2.,   3. , 90.,     0.95,   100., 0.6,   80.]
    # param_bounds=([thr-20.,    1.,   0.1,  70.,     0.9 ,    0., 0.2,   10.],
    #               [thr+20.,    10.,  10. ,180.,     1.  ,  110., 1. ,   100.])
    # # print(x[:-2], y[:-2])
    # popt, pcov = curve_fit(vectCBconvATAN, x[:-2], y[:-2], p0, maxfev=5000, bounds=param_bounds)
    # # print(popt)

    # worked for 23
    #             [mean, sigma, alpha,    n,     norm, xturn,   p, width]
    p0 =          [thr+15  ,  8.,   5. , 80.,     0.95,   30., 0.5,   80.]
    param_bounds=([thr-20.,    1.,   0.1,  70.,     0.9 ,    0., 0.2,   10.],
                  [thr+20.,    10.,  10. ,180.,     1.  ,  110., 1. ,   100.])
    
    print(x, y)
    i_to_skip=[8,9]
    x_fit = np.delete(x, i_to_skip)
    y_fit = np.delete(y, i_to_skip)
    popt, pcov = curve_fit(vectCBconvATAN, x_fit, y_fit, p0, maxfev=5000, bounds=param_bounds)
    # print(popt)

    ##vectApproxATAN
    #             # [xturn,    p, width]
    # p0 =          [10., 0.8,   80.]
    # param_bounds=([0., 0.2,   10.],
    #               [110., 1. ,   100.])
    # popt, pcov = curve_fit(vectApproxATAN, x, y, p0, maxfev=5000, bounds=param_bounds)
    # print(popt)
    # ax.plot(plot_x, vectApproxATAN(plot_x, *popt), '-', label='_', lw=2, color=cmap(color), zorder=color+1)

    ax.plot(plot_x, vectCBconvATAN(plot_x, *popt), '-', label='_', lw=2, color=cmap(color), zorder=color+1)
        
    if options.logx:
        plt.xlim(10., 500.)
        plt.xscale('log')
        ax.xaxis.set_major_locator(FixedLocator([10, 100, 500]))
        ax.xaxis.set_major_formatter(FixedFormatter(['10',r'$100$', r'$500$']))
    else:
        plt.xlim(15., 150.)
    # leg = plt.legend(loc = 'lower right', fontsize=16, title=r'$|\eta^{\tau, offline}|<2.1$'+"\n"+r'2024 Era I ReEmu w/ 2025 conditions')
    leg = plt.legend(loc = 'lower right', fontsize=16, title=r'$|\eta^{\tau, offline}|<2.1$')
    leg._legend_box.align = "left"
    plt.xlabel(r'$p_{T}^{\tau, offline}\ [GeV]$')
    plt.ylabel(r'Efficiency')
    for xtick in ax.xaxis.get_major_ticks():
        xtick.set_pad(10)
    mplhep.cms.label('Preliminary', data=True, rlabel=r'2025 - 13.6 TeV')

 
def read_file(inFile, iso, thr, label, opt=False, tunrOn=''):
    if iso:
        eff_TGraph = inFile.Get('divide_ptProgressionAt'+thr+'_Iso_by_pt') if not opt else \
                     inFile.Get(tunrOn)
        label = r'$E_{T}^{\tau, L1} > %s$ GeV & Iso  %s' % (thr, label)
    else:
        eff_TGraph = inFile.Get('divide_ptProgressionAt'+thr+'_noIso_by_pt') if not opt else \
                     inFile.Get('TurnOn_noIso')
        label = r'$E_{T}^{\tau, L1} > %s$ GeV  %s' % (thr, label)

    return eff_TGraph, label
    
#######################################################################
######################### SCRIPT BODY #################################
#######################################################################
# This script is used to fit the turnon curves of the tau trigger
''' python3 TurnOn_fitter.py --inFile1 efficiencies_of_MC25W_conservative_MiniAOD_unpacked.root --inFile2 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_unpacked.root --tag RunMC25W_full_process_new_production '''
''' python3 TurnOn_fitter.py --inFile1 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_unpacked.root --inFile2 "" --tag RunMC25W_full_process_new_production '''
''' python3 TurnOn_fitter.py --inFile1 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_reEmulated.root --inFile2 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_unpacked.root --tag Re-emu-vs-unpacked-2024I-HCALcFeb-ZS2025-effMin0p9_eMin22_eMax37'''
''' python3 TurnOn_fitter.py --inFile1 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_corrected_resolution_unpacked.root --inFile2 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_reEmulated.root --inFile3 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_corrected_resolution_reEmulated.root --tag Re-emu-vs-unpacked-2024I-HCALcFeb-ZS2025-effMin0p9_eMin22_eMax37'''
''' python3 TurnOn_fitter.py --inFile1 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_corrected_resolution_unpacked.root --inFile2 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_reEmulated.root --inFile3 efficiencies_of_23GeVturnon_reEmulated.root --tag Re-emu-vs-unpacked-2024I-HCALcFeb-ZS2025-effMin0p9_eMin22_eMax37'''
''' python3 TurnOn_fitter.py --inFile1 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_corrected_resolution_unpacked.root --inFile2 efficiencies_of_23GeVturnon_uncorrected_reEmulated.root --inFile3 efficiencies_of_23GeVturnon_corrected_reEmulated.root --tag Re-emu-vs-unpacked-2024I-HCALcFeb-ZS2025-effMin0p9_eMin22_eMax37'''
''' python3 TurnOn_fitter.py --inFile1 efficiencies_of_2025B_run391884_2025C_unpacked.root --inFile2 efficiencies_of_2024_unpacked_unpacked.root --tag Unpacked_2025vs2024'''
''' python3 TurnOn_fitter.py --inFile1 efficiencies_of_2025B_run391884_2025C_unpacked.root \
                             --inFile2 efficiencies_of_2024_unpacked_unpacked.root \ 
                             --inFile3 efficiencies_of_TAUReEmul_v4_reEmulated.root \
                             --tag Unpacked_2025newLUT_crab'''
''' python3 TurnOn_fitter.py --inFile1 efficiencies_of_2025B_run391884_2025C_unpacked.root  --inFile2 efficiencies_of_2024_unpacked_unpacked.root --inFile3 efficiencies_of_Re-emu-2025C-caloparams2025-newIsoLUT_reEmulated.root --tag Unpacked_2025newLUT_crab'''
''' python3 TurnOn_fitter.py --inFile1 efficiencies_of_2025B_run391884_2025C_unpacked.root --inFile2 efficiencies_of_2024_unpacked_unpacked.root --inFile3 efficiencies_of_2025C-Re-emu-newLUT_unpacked.root --tag Unpacked_2025newLUT'''
''' python3 TurnOn_fitter.py --inFile1 efficiencies_of_2025B_run391884_2025C_unpacked.root --inFile2 efficiencies_of_2024_unpacked_unpacked.root --inFile3 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p7_eMin16_eMax31_corrected_resolution_reEmulated.root --inFile4 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_reEmulated.root  --tag Unpacked_2025vs2024vsnewReEmuvsOldRemu'''
''' python3 TurnOn_fitter.py --inFile1 efficiencies_of_2025B_run391884_2025C_unpacked.root --inFile2 efficiencies_of_2024_unpacked_unpacked.root --inFile3 efficiencies_of_Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p7_eMin16_eMax31_corrected_resolution_reEmulated.root --inFile4 efficiencies_of_Muon_0_2025C_0p6_11_30_reEmulated.root --tag Unpacked_2025vs2024vstestsReEmu'''
 
'''python3 TurnOn_fitter.py --inFile1 efficiencies_of_Muon_1_2025C_0p6_11_30_LLR_unpacked.root --inFile2 efficiencies_of_2024_unpacked_unpacked.root --inFile3 efficiencies_of_TAUReEmul_v4_reEmulated.root --inFile4 efficiencies_of_Muon_1_2025C_0p6_11_30_LLR_reEmulated.root --inFile5 efficiencies_of_Muon_1_2025C_0p7_17_30_LLR_reEmulated.root --inFile6 efficiencies_of_Muon_1_2025C_0p7_18_27_reEmulated.root --tag Unpacked_2025newLUT_0p7_18_27'''

''' python3 TurnOn_fitter.py --inFile1 efficiencies_of_2025B_run391884_2025C_unpacked.root --inFile2 efficiencies_of_2024_unpacked_unpacked.root --inFile3 efficiencies_of_crab_unpacked_2025_eraD_unpacked.root --tag Monitoring_Run3_2025'''

if __name__ == "__main__" :
    parser = OptionParser()
    parser.add_option("--inFile1",     dest="inFile1",                                    default=None)
    parser.add_option("--inFile2",     dest="inFile2",                                    default=None)
    parser.add_option("--inFile3",     dest="inFile3",                                    default=None)
    parser.add_option("--inFile4",     dest="inFile4",                                    default=None)
    parser.add_option("--inFile5",     dest="inFile5",                                    default=None)
    parser.add_option("--inFile6",     dest="inFile6",                                    default=None)
    parser.add_option("--tag",         dest="tag",                                        default=None)
    parser.add_option("--logx",        dest="logx",                  action='store_true', default=False)
    parser.add_option("--smallFitErr", dest="smallFitErr",           action='store_true', default=False)
    (options, args) = parser.parse_args()
    # print(options)

    
    # main_folder = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_11_0_2/src/HiggsAnalysis/TagObjectsOptimization/PlotTurnOns/ROOTs/ROOTs_2024/'
    # main_folder = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotTurnOns/ROOTs/ROOTs_2025/'
    main_folder2 = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotTurnOns/ROOTs/ROOTs_2024/'
    main_folder = '/home/llr/cms/amella/Plotting_efficiency/CMSSW_13_2_0_pre3/src/HiggsAnalysis/TagObjectsOptimization/PlotTurnOns/ROOTs/ROOTs_Run3_2025/'
    inFile1 = ROOT.TFile(main_folder+options.inFile1)
    inFile2 = ROOT.TFile(main_folder+options.inFile2)
    inFile3 = ROOT.TFile(main_folder+options.inFile3)
    # inFile4 = ROOT.TFile(main_folder+options.inFile4)
    # inFile5= ROOT.TFile(main_folder+options.inFile5)
    # inFile6= ROOT.TFile(main_folder+options.inFile6)
    # inFile2 = ROOT.TFile(opt_folder + 'MC22_Summer_optimization_june' + options.inFile2)
    # inFile3 = ROOT.TFile(opt_folder + 'MC24_Winter_optimization_june' + options.inFile3)
   
    # label1 = r'Run2025W-MC-MiniAOD'
    # label2 = r'Run2025W-MC-MiniAOD Iso'
    # label1 = r'ReEmu-2024I-HCALcFeb-ZS2025-effMin0p9_eMin22_eMax37'
    # label2 = r'ReEmu-2024I-HCALcFeb-ZS2025-effMin0p9_eMin22_eMax3'
    # label3 = r'ReEmu-2024I-HCALcFeb-ZS2025-effMin0p9_eMin22_eMax3'
    # label1 = "ReEmu-2024I-HCALcFeb-ZS2025-effMin0p9_eMin22_eMax37"
    label1= r"Unpacked 2025 Eras B&C"
    label2= r"Unpacked 2024 Era I"
    label3= r"Unpacked 2025 Era D"
    # label3= 'ReEmu 2025 Era C w/' + "\n" + 'Iso LUT with high rates' 
    # label4= 'ReEmu 2025 Era C w/' + "\n" + 'Iso LUT 0p6 18.5 GeV 60 GeV'
    # label5= 'ReEmu 2025 Era C w/' + "\n" + 'Iso LUT 0p7 25 GeV 60 GeV'
    # label6= 'ReEmu 2025 Era C w/' + "\n" + 'Iso LUT 0p7 26 GeV 44 GeV'
    # label4= '2024 Era I ReEmu w/' + "\n" + '2025 conditions' + "\n" + 'and corrected BDT response'
    # label1 = r'Unpacked 2024 EraI'
    # label2 = r'Unpacked 2024 EraI Iso'
    # label3 = r'Re-Emu pedestals + corrections'
    

    thr1 = '25'
    thr2 = '35'
    thr3 = '34'
    # thr4 = '32'
    # thr5 = '32'
    # thr6 = '32'
    # thr3 = '34Iso'

    thr1_string, iso1_string = thr1[:2], thr1[2:]
    thr2_string, iso2_string = thr2[:2], thr2[2:]
    thr3_string, iso3_string = thr3[:2], thr3[2:]
    # thr4_string, iso4_string = thr4[:2], thr4[2:]
    # thr5_string, iso5_string = thr5[:2], thr5[2:]
    # thr6_string, iso6_string = thr6[:2], thr6[2:]
    # print(thr2_string, iso2_string)
    eff_TGraph1, label1 = read_file(inFile1, iso1_string, thr1_string, label1) #, True, 'TurnOn_progression_effMin0p1_eMin25_eMax43')
    eff_TGraph2, label2 = read_file(inFile2, iso2_string, thr2_string, label2) #, True, 'TurnOn_noIso') # progression_effMin0p0_eMin10_eMax25')
    eff_TGraph3, label3 = read_file(inFile3, iso3_string, thr3_string, label3) #, True, 'TurnOn_noIso') # progression_effMin0p9_eMin10_eMax25')
    # eff_TGraph4, label4 = read_file(inFile4, iso4_string, thr4_string, label4) #, True, 'TurnOn_noIso') # progression_effMin0p9_eMin10_eMax25')
    # eff_TGraph5, label5 = read_file(inFile5, iso5_string, thr5_string, label5) #, True, 'TurnOn_noIso') # progression_effMin0p9_eMin10_eMax25')
    # eff_TGraph6, label6 = read_file(inFile6, iso6_string, thr6_string, label6) #, True, 'TurnOn_noIso') # progression_effMin0p9_eMin10_eMax25')
    

    print('------------------------------\n')
    # PLOT TURNONS
    fig, ax = plt.subplots(figsize=(10,10))
    plot_TurnOn4(eff_TGraph1, int(thr1_string), label1, 1, ax, options)
    plot_TurnOn4(eff_TGraph2, int(thr2_string), label2, 0, ax, options)
    plot_TurnOn3(eff_TGraph3, int(thr3_string), label3, 2, ax, options)
    # plot_TurnOn3(eff_TGraph4, int(thr4_string), label4, 3, ax, options)
    # plot_TurnOn3(eff_TGraph5, int(thr5_string), label5, 7, ax, options)
    # plot_TurnOn3(eff_TGraph6, int(thr6_string), label6, 8, ax, options)

    plot_name = 'turnons/Run3_2025_monitoring/turnons_Run_fixed_rate'+options.tag+"_"+thr1_string
    plot_name += '_iso' if iso2_string else '_no_iso'
    print(plot_name+'.png')
    plt.grid()
    if options.logx: plot_name += 'log_'
    plt.savefig(plot_name+'.pdf')
    plt.savefig(plot_name+'.png')
    plt.close()

