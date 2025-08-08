#!/usr/bin/bash
set -e
# sh run_optimisation.sh <tag_to_name_folder> <tag_given_to_RAW_MC> <miniaod_file> 
# sh calibration_optimization.sh TEST_NEW_FRAMEWORK MC2025-caloparams-2025 MC25_MiniAOD.root

working_dir='/data_CMS/cms/amella/Run3_2025/MC2025_caloparams-2025/'
pwd=$(pwd)

create_config_file() {
    cat <<EOF >"${1}/Calibrate/RegressionTraining/run_2025/${2}.config"
Trainer: GBRTrain
Trainer: GBRLikelihoodTrain
NumberOfRegressions: 1
OutputDirectory: ${1}/Calibrate/forests_2025_TEST

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
    cat <<EOF >"${1}/MergeTrees/run_2025/${2}/${2}_${3}.config"
TreeClass: TauStage2Trees

MainFiles: ${5}
MainTree: Ntuplizer_noTagAndProbe/TagAndProbe

SecondaryTrees.N: 1
SecondaryTrees.0.Tree: ZeroBias/ZeroBias
SecondaryTrees.0.Files: ${6}

OutputFile:  ${4}${2}_MERGED_${3}.root
EOF
}

cd ${pwd}/MatchAndCompress

#Compressed
python3 produceTreeWithCompressedVars_matched.py -i "${working_dir}${2}".root -o "${working_dir}${1}"_COMPRESSED.root

# Calibration
echo 'Calibrating..'

if [ ! -d ${pwd}/Calibrate/forests_2025_TEST ]; then
    mkdir ${pwd}/Calibrate/forests_2025_TEST
    echo "directory Calibrate/forests_2025_TEST created."
fi

if [ ! -d ${pwd}/Calibrate/corrections_2025_TEST ]; then
    mkdir ${pwd}/Calibrate/corrections_2025_TEST
    echo "Directory Calibrate/corrections_2025_TEST created."
fi

cd ${pwd}/Calibrate/RegressionTraining
make clean &> /dev/null || { echo "make clean failed"; exit 1; }
make
# make &> /dev/null || { echo "make failed"; exit 1; }
# make clean &> /dev/null; make &> /dev/null
echo "Making."
create_config_file "${pwd}" "${1}" "${working_dir}"
echo "Creating config file."
./regression.exe run_2025/${1}.config

echo "Apply Calibration"
cd ${pwd}/Calibrate/

python3 makeTH4_LUT.py -i forests_2025_TEST/BDT_training_optimization_"${1}"_results.root \
                       -o corrections_2025_TEST/corrections_BDT_training_"${1}".root


##In order to shift the values by a certain percentage for the calibration
#add in ApplyCalibration and MakeTauCalibLUT an extra argumnet that says 
##by how much you should increase these values
root -l -b <<EOF
.L ApplyCalibration.C+
ApplyCalibration("${working_dir}${1}_COMPRESSED.root", "${working_dir}${1}_CALIBRATED.root", \
                 "corrections_2025_TEST/corrections_BDT_training_${1}.root")
.q
EOF

root -l -b <<EOF
.L MakeTauCalibLUT.C+
MakeTauCalibLUT("LUTs_2025/LUTcalibration_${1}.txt", "corrections_2025_TEST/corrections_BDT_training_${1}.root")
.q
EOF

echo "Calibration finished ! To derive the Isolation LUTs go to NewIsoLutDerivationFramework/"

# # root -l -b <<EOF
# # .L ApplyCalibration_ZeroBias.C+
# # ApplyCalibrationZeroBias("${working_dir}${2}.root", "${working_dir}${2}_CALIBRATED.root", \
# #                          "corrections_2025/corrections_BDT_training_${1}.root")
# # .q
# # EOF