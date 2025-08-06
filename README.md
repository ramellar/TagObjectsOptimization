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

git clone -b fix_CMSSW13 git@github.com:mchiusi/TagObjectsOptimization.git
```

## Deriving Calibration and Isolation LUTs using bash script

### Production of the input objects
To produce the input objects use the `TagAndProbe` or `TagAndProbeInegrated` packages. Generally what you need to derive the calibration and isolation LUTs are the following files:
* `MERGED` file : resulting from the merging of RAW ntuples with `MINIAOD` 
* `MINIAOD` : offline reconstructed taus
* `ZeroBias` : RAW ntuples reEmulated using same caloParams of the RAW ntuples. Generally a high pile-up and high rate (110kHz) run is choosen. Do NOT forget to include, in the key points of the scripts, the number of bunches, the good lumisections, and the average instantaneous luminosity (available on [CMSOMS](https://cmsoms.cern.ch/cms/run_3/index)).

### Running the optimisation
Once you have all the inredients in your folder (for example in `/data_CMS/cms/user/`), simply lauch the following script: 
```bash
sh run_optimisation.sh <tag_given_to_merged> <tag_given_to_zerobias> <miniaod_file> <run_number>
```

An example of command is provided in the bash script itself. The `working_directory` in the bash script has to be configured.
The script will produce in a few hours the following files:
* `ROOT` file in you working directory containing all the TurnOns at 14kHz resulting from the gridsearch
* `txt` file in `CompareGridSearchTrunons/FMs/FMs_2024/` containing each processed TunrOns and the corresponding FigureOfMerit
* LUTs are stored in `Calibration` and `Isolate` folders

## Computing reponses and efficiencies from Data

### Production of the input objects
To produce the input objects use the `TagAndProbe` or `TagAndProbeInegrated` packages. Input files:
* `ROOT` file resulting from the `hadd` of the reEmulated ntuples
* `MINIAOD` : offline reconstructed taus

### Running the plot productions
```bash
sh produce_plots.sh <tag_given_to_data_file> <miniaod_file>
```

To produce public plots, new python files have been included in `MakePublicTauPlots` folder.


## Else, without bash script

### Merging, matching, and compression
In the 'TagAndProbe' branch the jobs are now sent on crab which directly performs the merging and matching. Hence if this has been used no need to do this step. 
The starting point will be the matched file so inly the compression needs to be applied

### Calibration LUT derivation
Enter `Calibrate/RegressionTraining` and run `make clean ; make`.

To do the calibration first create/modify the needed `.config` file inside the `Calibrate/RegressionTraining/run` directory, then just run:
```bash
./regression.exe run_<year>/<config>.config
```

Now that the regression has been trained, adapt to your needs the `makeTH4_LUT.py` file  and run:
```bash
python makeTH4_LUT.py
```

Now that the TH4 LUTs have been created, we can apply the calibration to teh L1 objects. Adapt to your needs `ApplyCalibration.C` and `ApplyCalibrationZeroBias.C`
```bash
root -l
.L ApplyCalibration.C+
ApplyCalibration() # insert needed arguments
```

and

```bash
root -l
.L ApplyCalibrationZeroBias.C+
ApplyCalibrationZeroBias() # insert needed arguments
```

After the TH4 histos LUTs have been created we can make the LUTs that then go online; adapt to your needs the `MakeTauCalibLUT.C` file and run:

```
bash
root -l
.L MakeTauCalibLUT.C+
MakeTauCalibLUT() # insert needed arguments
```

### Isolation

In this branch 'NewIsoLutDerivationFramework/' has been added. This allows to obtain Isolation LUTs using new python scripts. These allowed to solve the problems the derivation of isolation LUTs in 2025. This should also de used in the future. Unfortuanetly for the moment no figure of merit has been established, so in order to choose the LUT we have 3 scripts:

- 'Isolation_derivation.py' allows to create the 'LUT.txt' fixing $\epsilon_min$, $E_{min}$ and $E_{max}$ as well as an image that shows the isolation threshold in the iEt vs inTT bins for each ieta bin. 
- 'Apply_Iso_Rates.py' and 'Efficiency_validation.py' allow to test the LUT that has just been created. The first one gives a quick estimation of the rate that this LUT will give without having to re emulate the raw data and the second one gives an estimation of how the turn on will look like without needing to re emulate data. These can be used to compare the performances of different LUTs produced and once we have one that gives the best efficiency but maitining the rates under control, we can re emulate using that LUT validate our study.

However, a figure of merit will hopefully be impelemented soon in order to avoid the by hand grid search

To run this new isolation framework it might be necessary to install an environment, here are the instruction for the installation:

```conda create -n tau_env -c conda-forge python=3.10 root 
pip install uproot
pip install matplotlib
pip install mplhep
pip install scipy```