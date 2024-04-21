# L1TauObjectsOptimization
Set of tools to evaluate tau trigger performance on T&amp;P

## Foreword
This package is based on the developments done by Olivier Davignon, Luca Cadamuro, Jean-Baptiste Sauvan, and Jona Motta.
The original work done by the first three can be found mainly at these two links: [NTuples production](https://github.com/davignon/TauTagAndProbe/tree/master/TauTagAndProbe), [Objects calibration](https://github.com/jbsauvan/RegressionTraining)

This forlder is an attempt to put together all of the developments done on top of these two folders in one single repositoty and one tool: ONE TOOL TO OPTIMIZE THEM ALL - CIT.

## Install instructions
These instalation instructions work only for CMSSW_11_0_2. To use the updated CMSSW_12_0_2 version for the TauTagAndProbe package and the production of the NTuples refer to [these instructions](https://github.com/jonamotta/TauTagAndProbe/tree/CMSSW_12_0_2-l1t-integration-v110.0). On the other hand, still use the following instructions to do the optimization of the objects (even if NTuples are produced with CMSSW_12_0_2).


```bash
cmsrel CMSSW_11_0_2
cd CMSSW_11_0_2/src
cmsenv
git cms-init
git remote add cms-l1t-offline git@github.com:cms-l1t-offline/cmssw.git
git fetch cms-l1t-offline l1t-integration-CMSSW_11_0_2
git cms-merge-topic -u cms-l1t-offline:l1t-integration-v104.5
git cms-addpkg L1Trigger/L1TCommon
git cms-addpkg L1Trigger/L1TMuon
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TMuon.git L1Trigger/L1TMuon/data
git cms-addpkg L1Trigger/L1TCalorimeter
git clone https://github.com/cms-l1t-offline/L1Trigger-L1TCalorimeter.git L1Trigger/L1TCalorimeter/data

mkdir HiggsAnalysis
cd HiggsAnalysis
git clone git@github.com:bendavid/GBRLikelihood.git
# modify the first line of `HiggsAnalysis/GBRLikelihood/BuildFile.xml` to have `-std=c++17`

git cms-checkdeps -A -a

scram b -j 10

git clone https://github.com/jonamotta/TauObjectsOptimization # package for the full optimization
```

## Deriving Calibration and Isolation LUTs using bash script

### Production of the input objects
To produce the input objects use the `TagAndProbe` or `TagAndProbeInegrated` packages. Generally what you need to derive the calibration and isolation LUTs are the following files:
* `MERGED` file : resulting from the merging of RAW ntuples with `MINIAOD` 
* `MINIAOD` : offline reconstructed taus
* `ZeroBias` : RAW ntuples reEmulated using same caloParams of the RAW ntuples. Generally a high pile-up and high rate (110kHz) run is choosen

### Merging, matching, and compression
Enter `MergeTrees` and run `make clean ; make`.

To merge the files first create/modify the needed `.config` file inside the `MergeTrees/run` directory. There the `MINIAOD` file has to be specified as primary and the `RAW` one as secondary (always use absolute paths). Check that the files really contain the TTrees that the executable will look for. 
Then jus run:
```bash
./merge.exe run_<year>/<optimization_version>/<config>.config 
```

### Running the optimisation
Once you have the `MERGED` file in your folder, simply lauch: 
```bash
sh run_optimisation.sh <tag_given_to_merged> <tag_given_to_zerobias> <miniaod_file>
```

An example is provided in the bash script itself. The `working\_directory` in the basj script has to be configured.
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
Enter `MergeTrees` and run `make clean ; make`.

To merge the files first create/modify the needed `.config` file inside the `MergeTrees/run` directory. There the `MINIAOD` file has to be specified as primary and the `RAW` one as secondary (always use absolute paths). Check that the files really contain the TTrees that the executable will look for. 
Then jus run:
```bash
./merge.exe run_<year>/<optimization_version>/<config>.config 
```

After that we need to match the reco taus to the L1 taus. To do so edit the `MakeTreeForCalibration.C` file (inside the `MatchAndCompress` folder) with the correct in and out files, then just run:
```bash
root -l
.L MakeTreeForCalibration.C+
MakeTreeForCalibration()
```

After the matching the compression needs to be performed. To do so edit the `produceTreeWithCompressedVars.py` file (inside the `MatchAndCompress` folder) with the correct in and out files, then just run:
```bash
python produceTreeWithCompressedVars.py
```

### Calibration
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
```bash
root -l
.L MakeTauCalibLUT.C+
MakeTauCalibLUT() # insert needed arguments
```

### Isolation
Now that the calibration has been done the isolation needs to be compute and applied.
To do so go to the `Isolate` folder.

To compute the isolation, adapt to your needs the `Build_Isolation_WPs.C` file and run:
```bash
root -l
.L Build_Isolation_WPs.C+
Build_Isolation_WPs() # insert needed arguments
```
This one has the possibility of building the WPs based on `compressed` or `supercompressed` variables. Due to statistics limits, it is always better to run in `supercompressed` mode.

Then the relaxation of the isolation needs to be performed. To do so, adapt to your needs `Fill_RelaxedIsolation.C` and run:
```bash
root -l
.L Fill_RelaxedIsolation.C+
Fill_RelaxedIsolation_TH3() # insert needed arguments
```
or adapt to your needs `Fill_RelaxedIsolation_gridsearch_nTTextrap.C` and run:
```bash
root -l
.L Fill_RelaxedIsolation_gridsearch_nTTextrap.C+
Fill_RelaxedIsolation_TH3() # insert needed arguments
```
to start a a grid search over the possible relaxation schemes that give rise to different turnON shapes.
In here, the IsoEt cuts are constructed either by reading the bin's contents or making the IsoEt vs. nTT fit, the option regulating this is `byBin`.

### Rates
To produce rates go to the `MakeRates` folder.

If you are running the 'old' version of the code, adapt to your needs `Rate_ZeroBias_unpacked.C` and `Rate_ZeroBias.C` and run:
```bash
root -l
.L Rate_ZeroBias_unpacked.C+
Rate() # insert needed arguments
```
and 
```bash
root -l
.L Rate_ZeroBias.C+
Rate() # insert needed arguments
```

Else, if you are running the gridsearch, adapt to your needs `Rate_ZeroBias_unpacked.C` and `Rate_ZeroBias_gridSearch.C` and run:
```bash
root -l
.L Rate_ZeroBias_unpacked.C+
Rate() # insert needed arguments
```
and
```bash
root -l
.L Rate_ZeroBias_newnTT_gridSearch.C+
Rate() # insert needed arguments
```

## Thresholds
Having computed the rates, the next step is to compute either the thresholds at fixed rates or the rates at fixed thresholds, by going to the `CompareRates` folder.

If you are running the 'old' version of the code, adapt to your needs `CompareRates_ZeroBias_withUnpacked.C`, and run:
```bash
root -l 
.L CompareRates_ZeroBias_withUnpacked.C
compare() # insert needed arguments
```

Else, if you are running the gridsearch, adapt to your needs `CompareRates_ZeroBias_gridSearch_withUnpacked.C`, and run:
```bash
root -l 
.L CompareRates_ZeroBias_gridSearch_withUnpacked.C
compare() # insert needed arguments
```

### TurnONs
Now that also the isolation has been created we can test everything on the turn-on curves by going to the `MakeTurnOns` folder.
Here the turnons can be made aother at fixed threshold or at fixed rates by applying the threshold computed at the previous step.

If you are running the 'old' version of the code, adapt to to your needs `ApplyIsolationForTurnOns.C` and run:
```bash
root -l
.L ApplyIsolationForTurnOns.C+
ApplyIsolationForTurnOns() # insert needed arguments
```

Else, if you are running the gridsearch, adapt to to your needs `ApplyIsolationForTurnOns_gridSearch.C` and run:
```bash
root -l
.L ApplyIsolationForTurnOns_gridSearch.C+
ApplyIsolationForTurnOns() # insert needed arguments
```

### Gridsearch best options evaluation
If in the previous steps we have been using, the gridsearch approach to theoptimisation, we can now compare the different ptions to decide which one is the best for our needs.

To do so go into the `CompareGridSearchTrunons`, adapt to your needs `BestFMturnOns_gridSearch.C`, and run:
```bash
root -l 
.L BestFMturnOns_gridSearch.C
compare() # insert needed arguments
```
This one can compare the turnons at fixed threshold or at fixed rate.
In both cases quality requirements are made on the turnon, and all information is saved in `.txt` files containing the optimisation figures of merit and the rates.

### Validate performance on data
All of the abve has been done on MC and needs to be validated on data. Threforre, after having re-emulated the data with the new options we need to produce the turnons.
We can do this in the `PlotTurnOns` folder.


We need adapting to your needs one of the three following codes:
* `MakeEfficiencies_Data_reEmulated.C` : make performance on re-emulated data with the re-optimised taus
* `MakeEfficiencies_Data_unpacked.C` : make performance on unpacked data from Run3 only
* `MakeEfficiencies_Data_unpacked_withRun2.C` : make performance on unpacked data from Run3 and Run2 at the same time
and running:
```bash
root -l
.L MakeEfficiencies_Data_<tag>.C
compare() # insert needed arguments
```

