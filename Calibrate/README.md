This part of the package comes from the following:

```bash
cp RegressionTraining/makeTH4_<whatevertag.py> .
scp -r <username>@lxplus.cern.ch:/data_CMS/cms/davignon/TauTagAndProbe_92X_for2018_May2018/CMSSW_9_2_8/src/TauTagAndProbe/TauTagAndProbe/test/Optimization/MakeCalibLUT/MakeTauCalibLUT_MC_NewCompression_WithMarch2017Layer1.C .
scp -r <username>@lxplus.cern.ch:/data_CMS/cms/davignon/TauTagAndProbe_92X_for2018_May2018/CMSSW_9_2_8/src/TauTagAndProbe/TauTagAndProbe/test/Optimization/ApplyCalibration/ApplyCalibration_newnTT.C
scp -r <username>@lxplus.cern.ch:/data_CMS/cms/davignon/TauTagAndProbe_92X_for2018_May2018/CMSSW_9_2_8/src/TauTagAndProbe/TauTagAndProbe/test/Optimization/ApplyCalibration/GBR2LUT.py .
scp -r <username>@lxplus.cern.ch:/data_CMS/cms/davignon/TauTagAndProbe_92X_for2018_May2018/CMSSW_9_2_8/src/TauTagAndProbe/TauTagAndProbe/test/Optimization/ApplyCalibration/GBR2LUT.pyc .
scp -r <username>@lxplus.cern.ch:/data_CMS/cms/davignon/TauTagAndProbe_92X_for2018_May2018/CMSSW_9_2_8/src/TauTagAndProbe/TauTagAndProbe/test/Optimization/ApplyCalibration/GBR2LUTEmulator.py .
scp -r <username>@lxplus.cern.ch:/data_CMS/cms/davignon/TauTagAndProbe_92X_for2018_May2018/CMSSW_9_2_8/src/TauTagAndProbe/TauTagAndProbe/test/Optimization/ApplyCalibration/GBR2LUTEmulator.pyc .
cp /data_CMS/cms/davignon/TauTagAndProbe_101X_Rate_May2018/CMSSW_10_1_2/src/TauTagAndProbe/TauTagAndProbe/test/ApplyCalibration/ApplyCalibrationZeroBias_nTT.C .
```

with subsequent cleaning and adaptation.