#!/usr/bin/bash
set -e
# sh run_optimisation.sh <tag_to_name_folder> <tag_given_to_zerobias> <miniaod_file> <run>

working_dir='/data_CMS/cms/amella/Run3_2024/Run3_2024/MC24_Winter_optimization/'
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

create_file_merge() {
    cat <<EOF >"${1}/MergeTrees/run_2024/${2}/${2}_${3}.config"
TreeClass: TauStage2Trees

MainFiles: ${5}
MainTree: Ntuplizer_noTagAndProbe/TagAndProbe

SecondaryTrees.N: 1
SecondaryTrees.0.Tree: ZeroBias/ZeroBias
SecondaryTrees.0.Files: ${6}

OutputFile:  ${4}${2}_MERGED_${3}.root
EOF
}

# # merging
# echo 'Merging..'

# cd ${pwd}/MergeTrees
# make clean &> /dev/null; make &> /dev/null

# if [ ! -d ${pwd}/MergeTrees/run_2024/${1} ]; then
#     mkdir ${pwd}/MergeTrees/run_2024/${1}
#     echo "Directory created."
# else
#     echo "Directory already exists."
# fi


# # prefixes=("VBF_" "GluGlu" "VBFp")
# prefixes=("VBF_")
# for prefix in "${prefixes[@]}"
# do
#   raw_file=$(ls ${working_dir}${prefix}*RAW*.root 2>/dev/null)
#   aod_file=$(ls ${working_dir}${prefix}*MINIAOD*.root 2>/dev/null)
#   echo 'raw file'
#   echo ${raw_file}
#   if [[ -n "$raw_file" && -n "$aod_file" ]]; then
#     echo "Processing RAW file: $raw_file and MINIAOD file: $aod_file..."
#     create_file_merge "${pwd}" "${1}" "${prefix}" "${working_dir}" "${aod_file}" "${raw_file}"
#     ./merge.exe ${pwd}/MergeTrees/run_2024/${1}/${1}_${prefix}.config
#   else
#    echo "No matching RAW and MINIAOD files found for prefix ${prefix}"
#    continue
#   fi
# done


# # hadd files
# echo 'Hadding merged files..'
# hadd ${working_dir}${1}_MERGED.root ${working_dir}${1}_MERGED_*.root


# # matching
# echo 'Matching..'

# cd ${pwd}/MatchAndCompress
# root -l -b <<EOF
# .L MakeTreeForCalibration.C+
# MakeTreeForCalibration("${working_dir}${1}_MERGED.root", "${working_dir}${1}_MATCHED.root", "Ntuplizer_noTagAndProbe_TagAndProbe")
# .q
# EOF

# #Compressed

# python produceTreeWithCompressedVars.py -i "${working_dir}${1}"_MATCHED.root -o "${working_dir}${1}"_COMPRESSED.root

# # Calibration
# echo 'Calibrating..'

# if [ ! -d ${pwd}/Calibrate/forests_${4} ]; then
#     mkdir ${pwd}/MergeTrees/forests_${4}
#     echo "Directory created."
# else
#     echo "Directory already exists."
# fi

# if [ ! -d ${pwd}/Calibrate/forests_${4} ]; then
#     mkdir ${pwd}/MergeTrees/corrections_${4}
#     echo "Directory created."
# else
#     echo "Directory already exists."
# fi

# cd ${pwd}/Calibrate/RegressionTraining
# make clean &> /dev/null; make &> /dev/null
# create_config_file "${pwd}" "${1}" "${working_dir}"
# ./regression.exe run_2024/${1}.config

# cd ${pwd}/Calibrate/

# python makeTH4_LUT.py -i forests_${4}/BDT_training_optimization_"${1}"_results.root \
#                       -o corrections_${4}/corrections_BDT_training_"${1}".root

# root -l -b <<EOF
# .L ApplyCalibration.C+
# ApplyCalibration("${working_dir}${1}_COMPRESSED.root", "${working_dir}${1}_CALIBRATED.root", \
#                  "corrections_${4}/corrections_BDT_training_${1}.root")
# .q
# EOF

# root -l -b <<EOF
# .L ApplyCalibration_ZeroBias.C+
# ApplyCalibrationZeroBias("${working_dir}${2}.root", "${working_dir}${2}_CALIBRATED.root", \
#                          "corrections_2024/corrections_BDT_training_${1}.root")
# .q
# EOF

# root -l -b <<EOF
# .L MakeTauCalibLUT.C+
# MakeTauCalibLUT("LUTs_2024/LUTcalibration_${1}.txt", "corrections_2024/corrections_BDT_training_${1}.root")
# .q
# EOF

# Isolation
echo 'Isolation..'

cd ${pwd}/Isolate

if [ ! -d ${pwd}/Isolate/ROOTs4LUTs_${4} ]; then
    mkdir ${pwd}/Isolate/ROOTs4LUTs_${4}
    echo "Directory created."
else
    echo "Directory already exists."
fi

root -l -b <<EOF
.L Build_Isolation.C+
Build_Isolation("${working_dir}${1}_CALIBRATED.root", "ROOTs4LUTs_2024/LUTisolation_${1}.root", 3 , 14)
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

if [ ! -d ${pwd}/MakeRates/Isolate/histos_${4} ]; then
    mkdir ${pwd}/MakeRates/Isolate/histos_${4}
    echo "Directory created."
else
    echo "Directory already exists."
fi
root -l -b <<EOF
.L Rate_ZeroBias_unpacked.C+
Rate("${working_dir}${2}.root", "histos_2024/histos_rate_ZeroBias_Run${4}_${1}_unpacked.root", ${4})
.q
EOF

root -l -b <<EOF
.L Rate_ZeroBias_gridSearch.C+
Rate("${working_dir}${2}_CALIBRATED.root", "histos_2024/histos_rate_ZeroBias_Run${4}_${1}_optimisation.root", \
     "../Isolate/ROOTs4LUTs_2024/LUTrelaxation_${1}.root", ${4})
.q
EOF

# # Threshold at Fixed rate 14kHz
# echo 'Computing Thresholds..'

# cd ${pwd}/CompareRates
# root -l -b <<EOF
# .L CompareRates_ZeroBias_gridSearch_withUnpacked.C+
# compare("../MakeRates/histos_2024/histos_rate_ZeroBias_Run${4}_${1}_unpacked.root", \
#         "../MakeRates/histos_2024/histos_rate_ZeroBias_Run${4}_${1}_optimisation.root", \
#         "../MakeRates/histos_2024/thresholds_fixedrate_ZeroBias_Run${4}_${1}unpacked_optimization.root")
# .q
# EOF
 
# # TunrOns
# echo 'Making TunrOns..'

# cd ${pwd}/MakeTurnOns
# root -l -b <<EOF
# .L ApplyIsolationForTurnOns_gridSearch.C+
# ApplyIsolationForTurnOns("${working_dir}${1}_CALIBRATED.root", \
#                          "${working_dir}Tau_MC_TURNONS_FIXEDRATE_14kHz_${1}.root", \
#                          "../Isolate/ROOTs4LUTs_2024/LUTrelaxation_${1}.root", \
#                          "../MakeRates/histos_2024/thresholds_fixedrate_ZeroBias_Run${4}_${1}unpacked_optimization.root", \
#                          "${working_dir}${3}", 14, 0)
# .q
# EOF

# cd ${pwd}/CompareGridSearchTrunons
# root -l -b <<EOF
# .L BestFMturnOns_gridSearch.C+
# compare("${working_dir}Tau_MC_TURNONS_FIXEDRATE_14kHz_${1}.root", \
#         "FMs/FMs_2024/FM_orderd_turnons_FIXEDRATE_14kHz_Run${4}_${1}.txt", \
#         "../MakeRates/histos_2024/thresholds_fixedrate_ZeroBias_Run${4}_${1}unpacked_optimization.root", \
#         "../MakeRates/histos_2024/histos_rate_ZeroBias_Run${4}_${1}_unpacked.root", "FM")
# .q
# EOF

echo "Finish -- All good, best turnOns in CompareGridSearchTrunons/FMs/FMs_2024/FM_orderd_turnons_FIXEDRATE_14kHz_Run$4_$1.txt"
