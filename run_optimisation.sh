#!/usr/bin/bash
set -e
# sh run_optimisation.sh 2023S-MC_caloParams_2023_v0_4 EphemeralZeroBias_2023D_RAW_369978_optimization23_v4_HCAL_corr 369978 Run3Summer23_MINIAOD.root

working_dir='/data_CMS/cms/mchiusi/Run3preparation/Run3_2024/2024W-MC_caloParams_2023_v0_4_cfi/Winter24_GT_140/test/'
# /data_CMS/cms/mchiusi/Run3preparation/Run3_2024/2024W-MC_caloParams_2023_v0_4_cfi/test/'
pwd=$(pwd)

create_config_file() {
    cat <<EOF >"${1}/Calibrate/RegressionTraining/run_2024/${2}.config"
Trainer: GBRTrain
Trainer: GBRLikelihoodTrain
NumberOfRegressions: 1
OutputDirectory: ${1}/Calibrate/forests_2024

Regression.1.Name: BDT_training_optimization_${2}
Regression.1.InputFiles: ${3}${2}_COMPRESSED.root
Regression.1.Tree: outTreeForCalibration
Regression.1.Method: BDT
Regression.1.Options: MinEvents=200:Shrinkage=0.1:NTrees=1000:MinSignificance=5.0:RandomSeed=EventNumber
Regression.1.DoErrors: False
Regression.1.DoCombine: False
Regression.1.VariablesEB: abs(compressedieta):compressedE:L1Tau_hasEM:L1Tau_isMerged
Regression.1.VariablesEB: abs(compressedieta):compressedE:compressedsortedshape
Regression.1.VariablesEE: abs(compressedieta):compressedE:L1Tau_hasEM:L1Tau_isMerged
Regression.1.VariablesEE: abs(compressedieta):compressedE:compressedsortedshape
Regression.1.Target: Target
Regression.1.CutBase: OfflineTau_pt>18
Regression.1.CutEB: OfflineTau_pt>18
Regression.1.CutEE: OfflineTau_pt>18
EOF
}

# matching
echo 'Matching..'

cd ${pwd}/MatchAndCompress
root -l -b <<EOF
.L MakeTreeForCalibration.C+
MakeTreeForCalibration("${working_dir}${1}_MERGED.root", "${working_dir}${1}_MATCHED.root")
.q
EOF

python produceTreeWithCompressedVars.py -i "${working_dir}${1}"_MATCHED.root -o "${working_dir}${1}"_COMPRESSED.root

# Calibration
echo 'Calibrating..'

cd ${pwd}/Calibrate/RegressionTraining
make clean &> /dev/null; make &> /dev/null
create_config_file "${pwd}" "${1}" "${working_dir}"
./regression.exe run_2024/${1}.config

cd ${pwd}/Calibrate/
python makeTH4_LUT.py -i forests_2024/BDT_training_optimization_"${1}"_results.root \
                      -o corrections_2024/corrections_BDT_training_"${1}".root

root -l -b <<EOF
.L ApplyCalibration.C+
ApplyCalibration("${working_dir}${1}_COMPRESSED.root", "${working_dir}${1}_CALIBRATED.root", \
                 "corrections_2024/corrections_BDT_training_${1}.root")
.q
EOF

root -l -b <<EOF
.L ApplyCalibration_ZeroBias.C+
ApplyCalibrationZeroBias("${working_dir}${2}.root", "${working_dir}${2}_CALIBRATED.root", \
                         "corrections_2024/corrections_BDT_training_${1}.root")
.q
EOF

root -l -b <<EOF
.L MakeTauCalibLUT.C+
MakeTauCalibLUT("LUTs_2024/LUTcalibration_${1}.txt", "corrections_2024/corrections_BDT_training_${1}.root")
.q
EOF

# Isolation
echo 'Isolation..'

cd ${pwd}/Isolate
root -l -b <<EOF
.L Build_Isolation.C+
Build_Isolation("${working_dir}${1}_CALIBRATED.root", "ROOTs4LUTs_2024/LUTisolation_${1}.root")
.q
EOF

root -l -b <<EOF
.L Fill_RelaxedIsolation_gridsearch.C+
Fill_RelaxedIsolation_TH3("ROOTs4LUTs_2024/LUTisolation_${1}.root", "ROOTs4LUTs_2024/LUTrelaxation_${1}.root")
.q
EOF

# Rates
echo 'Making rates..'

cd ${pwd}/MakeRates
root -l -b <<EOF
.L Rate_ZeroBias_unpacked.C+
Rate("${working_dir}${2}.root", "histos_2024/histos_rate_ZeroBias_Run${3}_${1}_unpacked.root", ${3})
.q
EOF

root -l -b <<EOF
.L Rate_ZeroBias_gridSearch.C+
Rate("${working_dir}${2}_CALIBRATED.root", "histos_2024/histos_rate_ZeroBias_Run${3}_${1}_optimisation.root", \
     "../Isolate/ROOTs4LUTs_2024/LUTrelaxation_${1}.root", ${3})
.q
EOF

# Threshold at Fixed rate 14kHz
echo 'Computing Thresholds..'

cd ${pwd}/CompareRates
root -l -b <<EOF
.L CompareRates_ZeroBias_gridSearch_withUnpacked.C+
compare("../MakeRates/histos_2024/histos_rate_ZeroBias_Run${3}_${1}_unpacked.root", \
        "../MakeRates/histos_2024/histos_rate_ZeroBias_Run${3}_${1}_optimisation.root", \
        "../MakeRates/histos_2024/thresholds_fixedrate_ZeroBias_Run${3}_${1}unpacked_optimization.root")
.q
EOF

# TunrOns
echo 'Making TunrOns..'

cd ${pwd}/MakeTurnOns
root -l -b <<EOF
.L ApplyIsolationForTurnOns_gridSearch.C+
ApplyIsolationForTurnOns("${working_dir}${1}_CALIBRATED.root", \
                         "${working_dir}Tau_MC_TURNONS_FIXEDRATE_14kHz_${1}.root", \
                         "../Isolate/ROOTs4LUTs_2024/LUTrelaxation_${1}.root", \
                         "../MakeRates/histos_2024/thresholds_fixedrate_ZeroBias_Run${3}_${1}unpacked_optimization.root", \
                         "${working_dir}${4}", 14, 0)
.q
EOF

cd ${pwd}/CompareGridSearchTrunons
root -l -b <<EOF
.L BestFMturnOns_gridSearch.C+
compare("${working_dir}Tau_MC_TURNONS_FIXEDRATE_14kHz_${1}.root", \
        "FMs/FMs_2024/FM_orderd_turnons_FIXEDRATE_14kHz_Run${3}_${1}.txt", \
        "../MakeRates/histos_2024/thresholds_fixedrate_ZeroBias_Run${3}_${1}unpacked_optimization.root", \
        "../MakeRates/histos_2024/histos_rate_ZeroBias_Run${3}_${1}_unpacked.root", "FM")
.q
EOF

echo "Finish -- All good, best turnOns in CompareGridSearchTrunons/FMs/FMs_2024/FM_orderd_turnons_FIXEDRATE_14kHz_Run$3_$1.txt"
