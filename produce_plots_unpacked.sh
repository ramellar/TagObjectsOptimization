#!/usr/bin/bash
set -e

# sh produce_plots_unpacked.sh <tag> <miniaod_root_file> <rate_file> <run_number>

#sh produce_plots_unpacked.sh 2025B_run391884_2025C unpacked_2025_eraB_C.root whatever.root 391884
#sh produce_plots_unpacked.sh 2024_unpacked unapcked_2024I.root whatever.root 2024

# sh produce_plots_unpacked.sh MC25W_conservative_ZS MC25_MiniAOD.root Zerobias_Run386604.root 386604
# sh produce_plots_unpacked.sh 2024I_HCALcFeb_calop_0p9_22_37 Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37.root Zerobias_Run386604.root 386604


# sh produce_plots_unpacked.sh 2025C-Re-emu-newLUT muon01_reEmul_caloparams_2025_v0_2_newIsoLUT.root Zerobias_Run386604.root 386604
# sh produce_plots_unpacked.sh crab_unpacked_2025_eraD crab_unpacked_2025_eraD.root ZeroBias_2025_eraD_394959.root 394959



# working_dir='/data_CMS/cms/amella/Run3_2024/Run3_2024/unpacked_2024/Muon_Run2024I_MINIAOD/'
# working_dir='/data_CMS/cms/amella/Run3_2024/Run3_2024/'
# working_dir='/data_CMS/cms/amella/Run3_2025/2024I-data/'
working_dir='/data_CMS/cms/amella/Run3_2025/'
pwd=$(pwd)

# # Responses
# echo 'Making responses..'
# cd ${pwd}/PlotCalibrationResolution
# root -l -b <<EOF
# .L MakeResolutions_Data_reEMulated.C+
# MakeResolutions("${working_dir}${2}", -1, "${1}")
# .q
# EOF

# #TurnOns
# echo 'Making turnOns..'
# cd ${pwd}/PlotTurnOns
# root -l -b <<EOF
# .L MakeEfficiencies_Data_unpacked.C+
# MakeEfficiencies("${working_dir}${2}", "Ntuplizer/TagAndProbe", -1, "${1}")
# .q
# EOF


# Rates
echo 'Making rates..'

cd ${pwd}/MakeRates
root -l -b <<EOF
.L Rate_ZeroBias_unpacked.C+
Rate("${working_dir}${3}", "histos_2025/histos_rate_ZeroBias_Run${4}_${1}_unpacked.root", ${4})
.q
EOF

echo "Finish -- All good"

