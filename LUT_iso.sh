#!/usr/bin/bash
set -e
# sh LUT_iso.sh <eff_min> <Etmin> <Etmax> <tag_of_optimization>

# sh LUT_iso.sh 0.9 22 37 MC_conservative_ZS
# sh LUT_iso.sh 0.9 22 37 HCALcMarch-caloParams_2025_conservative-ZS-MC25W
# sh LUT_iso.sh 0.9 22 37 HCALcFeb-caloParams_2025_conservative-ZS-MC25W

# source ~/.bashrc
pwd=$(pwd)
# tau_aod=$(tau_aod)
tau_aod=/home/llr/cms/amella/Run3_tau_test/CMSSW_14_2_2/src

cd ${pwd}/Isolate

if [ ! -d ${pwd}/Isolate/LUTs/LUTs_2025 ]; then
    mkdir ${pwd}/Isolate/LUTs/LUTs_2025
    echo "Directory created."
else
    echo "Directory already exists."
fi

root -l -b <<EOF
.L MakeTauIsoLUT.C+
MakeTauIsoLUT_Options("ROOTs4LUTs_2025/LUTrelaxation_${4}.root", \
                      "LUTs/LUTs_2025/LUTrelaxation_${4}_progression_effMin0p${1: -1}_eMin${2}_eMax${3}.txt", \
                      ${1},${2},${3})
.q
EOF

echo "LUT created!"
echo "LUTrelaxation_${4}_progression_effMin0p${1: -1}_eMin${2}_eMax${3}.txt"
cp LUTs/LUTs_2025/LUTrelaxation_${4}_progression_effMin0p${1: -1}_eMin${2}_eMax${3}.txt ${tau_aod}/L1Trigger/L1TCalorimeter/data/

cd ${pwd}/Calibrate/
cp LUTs_2025/LUTcalibration_${4}.txt ${tau_aod}/L1Trigger/L1TCalorimeter/data/

echo "LUTcalibration_${4}.txt"