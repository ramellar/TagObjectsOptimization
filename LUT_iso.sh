#!/usr/bin/bash
set -e
# sh LUT_iso.sh <eff_min> <Etmin> <Etmax> <tag_of_optimization>
# source ~/.bashrc
pwd=$(pwd)
# tau_aod=$(tau_aod)
tau_aod=/home/llr/cms/amella/Run3_tau/CMSSW_14_0_4/src

cd ${pwd}/Isolate

if [ ! -d ${pwd}/Isolate/LUTs/LUTs_2024 ]; then
    mkdir ${pwd}/Isolate/LUTs/LUTs_2024
    echo "Directory created."
else
    echo "Directory already exists."
fi

root -l -b <<EOF
.L MakeTauIsoLUT.C+
MakeTauIsoLUT_Options("ROOTs4LUTs_2024/LUTrelaxation_${4}.root", \
                      "LUTs/LUTs_2024/LUTrelaxation_${4}_progression_effMin0p${1: -1}_eMin${2}_eMax${3}.txt", \
                      ${1},${2},${3})
.q
EOF

echo "LUT created!"
echo "LUTrelaxation_${4}_progression_effMin0p${1: -1}_eMin${2}_eMax${3}.txt"
mv LUTs/LUTs_2024/LUTrelaxation_${4}_progression_effMin0p${1: -1}_eMin${2}_eMax${3}.txt ${tau_aod}/L1Trigger/L1TCalorimeter/data/

cd ${pwd}/Calibrate/
cp LUTs_2024/LUTcalibration_${4}.txt ${tau_aod}/L1Trigger/L1TCalorimeter/data/

echo "LUTcalibration_${4}.txt"