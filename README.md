# L1TauObjectsOptimization
Set of tools to evaluate tau trigger performance on T&amp;P

## Foreword
This package is based on the developments done by Olivier Davignon, Luca Cadamuro, Jean-Baptiste Sauvan, Jona Motta, Marco Chiusi and Ruth Amella Ranz.
The original work done by the first three can be found mainly at these two links: [NTuples production](https://github.com/davignon/TauTagAndProbe/tree/master/TauTagAndProbe), [Objects calibration](https://github.com/jbsauvan/RegressionTraining)

This forlder is an attempt to put together all of the developments done on top of these two folders in one single repository and one tool. In May 2024, new optimization scripts were developed to streamline the processes of running optimization and re-emulation automatically.

** This branch contains the new framework to derive the Isolation LUTs **

## Install instructions
These installation instructions are tailored for CMSSW_13_2_0_pre3, the latest version of CMSSW available as of December 2024 in the [L1T Offline](https://github.com/cms-l1t-offline/cmssw) repository. Since the original version used CMSSW_11, several updates have been implemented, and the entire codebase has undergone validation.


```bash
cmsrel CMSSW_13_2_0_pre3
cd CMSSW_13_2_0_pre3/src
cmsenv
git cms-init
git remote add cms-l1t-offline git@github.com:cms-l1t-offline/cmssw.git
git fetch cms-l1t-offline l1t-integration-CMSSW_13_2_0_pre3
git cms-merge-topic -u cms-l1t-offline:l1t-integration-v164-CMSSW_13_2_0_pre3
git cms-addpkg L1Trigger/L1TCommon
git cms-addpkg L1Trigger/L1TMuon
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TMuon.git L1Trigger/L1TMuon/data
git cms-addpkg L1Trigger/L1TCalorimeter
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TCalorimeter.git L1Trigger/L1TCalorimeter/data

mkdir HiggsAnalysis
cd HiggsAnalysis
git clone -b fix-cmssw13-comp git@github.com:adewit/GBRLikelihood.git
cd ../

git cms-checkdeps -A -a

scram b -j 10

git clone -b fix_CMSSW13 git@github.com:ramellar/TagObjectsOptimization.git
```

## Deriving Calibration using bash script
To produce the input objects use the `TagAndProbe` or `TagAndProbeInegrated` packages. In the last update of these packages, the crab submission performs automatically the merging and the matching so these steps can be skipped now. To derive the calibration LUT we can directly use the script `calibration_optimization.sh` That as inputs the MC RAW and MiniAOD

## Deriving Isolation LUTs

In this branch 'NewIsoLutDerivationFramework/' has been added. This allows to obtain Isolation LUTs using new python scripts. These allowed to solve the problems the derivation of isolation LUTs in 2025. This should also de used in the future. Unfortuanetly for the moment no figure of merit has been established, so in order to choose the LUT we have 3 scripts:

- `Isolation_derivation.py` allows to create the 'LUT.txt' fixing $\epsilon_min$, $E_{min}$ and $E_{max}$ as well as an image that shows the isolation threshold in the iEt vs inTT bins for each ieta bin. 
- `Apply_Iso_Rates.py` and `Efficiency_validation.py` allow to test the LUT that has just been created. The first one gives a quick estimation of the rate that this LUT will give without having to re emulate the raw data and the second one gives an estimation of how the turn on will look like without needing to re emulate data. These can be used to compare the performances of different LUTs produced and once we have one that gives the best efficiency but maitining the rates under control, we can re emulate using that LUT validate our study.

However, a figure of merit will hopefully be impelemented soon in order to avoid the by hand grid search

To run this new isolation framework it might be necessary to install an environment, here are the instruction for the installation:

```bash
conda create -n tau_env -c conda-forge python=3.10 root 
pip install uproot
pip install matplotlib
pip install mplhep
pip install scipy

```