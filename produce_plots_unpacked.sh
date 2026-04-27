#!/usr/bin/bash
set -e

# sh produce_plots_unpacked.sh <tag> <miniaod_root_file> <rate_file> <run_number>

#sh produce_plots_unpacked.sh 2025B_run391884_2025C unpacked_2025_eraB_C.root whatever.root 391884
#sh produce_plots_unpacked.sh 2024_unpacked unapcked_2024I.root whatever.root 2024

# sh produce_plots_unpacked.sh MC25W_conservative_ZS MC25_MiniAOD.root Zerobias_Run386604.root 386604
# sh produce_plots_unpacked.sh 2024I_HCALcFeb_calop_0p9_22_37 Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37.root Zerobias_Run386604.root 386604


# sh produce_plots_unpacked.sh 2025C-Re-emu-newLUT muon01_reEmul_caloparams_2025_v0_2_newIsoLUT.root Zerobias_Run386604.root 386604
# sh produce_plots_unpacked.sh crab_unpacked_2025_eraD crab_unpacked_2025_eraD.root ZeroBias_2025_eraD_394959.root 394959
# sh produce_plots_unpacked.sh crab_unpacked_2025_eraF crab_unpacked_2025_eraF.root 
# sh produce_plots_unpacked.sh crab_unpacked_2025_eraG crab_unpacked_2025_eraG.root 


# sh produce_plots_unpacked.sh 2025_all_eras FullRun2025.root ZeroBias_2025E_396102.root 396102
# sh produce_plots_unpacked.sh 2025_pre_correction Run2025_preIsoCorrection.root 
# sh produce_plots_unpacked.sh 2025_pre_correction Run2025_preIsoCorrection.root 

# sh produce_plots_unpacked.sh 2026_HcalNZS_3 HcalNZS_3_Run2025G.root
# sh produce_plots_unpacked.sh 2026_HcalNZS_0 crab_HCALNZS_26D_0.root
# sh produce_plots_unpacked.sh 2026_HcalNZS_3 crab_HCALNZS_7J_3.root
# sh produce_plots_unpacked.sh 2026_HcalNZS_C_D_E_G_3 crab_HCALNZS_C_D_E_G_3.root
# sh produce_plots_unpacked.sh 2026_HcalNZS_C_D_E_G_0 crab_HCALNZS_C_D_E_G_0.root
# sh produce_plots_unpacked.sh Run2026_eraB_miniAOD Run2026_eraB_miniAOD.root
# sh produce_plots_unpacked.sh Run2026_eraB_miniAOD_new Run2026_eraB_miniAOD_new.root
# sh produce_plots_unpacked.sh Run2026_eraC_miniAOD Run2026_eraC_miniAOD.root



# working_dir='/data_CMS/cms/amella/Run3_2024/Run3_2024/unpacked_2024/Muon_Run2024I_MINIAOD/'
# working_dir='/data_CMS/cms/amella/Run3_2024/Run3_2024/'
# working_dir='/data_CMS/cms/amella/Run3_2025/2024I-data/'
# working_dir='/data_CMS/cms/amella/Run3_2025/DPNote_samples/'
working_dir='/data_CMS/cms/amella/Run3_2026/monitoring/'
pwd=$(pwd)

# Responses
echo 'Making responses..'
cd ${pwd}/PlotCalibrationResolution
root -l -b <<EOF
.L MakeResolutions_Data_reEMulated.C+
MakeResolutions("${working_dir}${2}", -1, "${1}")
.q
EOF

#TurnOns
echo 'Making turnOns..'
cd ${pwd}/PlotTurnOns
root -l -b <<EOF
.L MakeEfficiencies_Data_unpacked.C+
MakeEfficiencies("${working_dir}${2}", "Ntuplizer/TagAndProbe", -1, "${1}")
.q
EOF


# # Rates
# echo 'Making rates..'

# cd ${pwd}/MakeRates
# root -l -b <<EOF
# .L Rate_ZeroBias_unpacked.C+
# Rate("${working_dir}${3}", "histos_2025/histos_rate_ZeroBias_Run${4}_${1}_unpacked.root", ${4})
# .q
# EOF



echo "Finish -- All good"

