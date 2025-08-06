#!/usr/bin/bash
set -e

# sh produce_plots.sh <tag_re-emulation> <miniaod.root> <rate.root> <run_number>
# sh produce_plots.sh Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37 unapcked_2025I.root Zero_bias_run_386604_caloParams_2025_conservative_HCALcFeb_v3_iET_0p9_22_37.root 386604
# sh produce_plots.sh Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin22_eMax37_corrected_resolution unapcked_2025I.root Zero_bias_run_386604_caloParams_2025_conservative_HCALcFeb_v3_iET_0p9_22_37_cfi_corrected_resolution.root 386604


# sh produce_plots.sh Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p9_eMin19_eMax34_corrected_resolution unapcked_2025I.root Zero_bias_run_386604_caloParams_2025_conservative_HCALcFeb_v3_iET_0p9_19_34_cfi_corrected_resolution.root 386604
# sh produce_plots.sh Re-emu-caloParams_2025_conservative_HCALcFeb_v3_iET_effMin0p7_eMin16_eMax31_corrected_resolution unapcked_2024I.root Zero_bias_run_386604_caloParams_2025_conservative_HCALcFeb_v3_iET_0p7_16_31_cfi_corrected_resolution.root 386604

# sh produce_plots.sh Re-emu-Run2025C_conditions2025_Iso_0p7_RAW unpacked_2025_eraB_C.root ZeroBiasIdonthave 2025
# sh produce_plots.sh Ntuple_10000 unpacked_2025_eraB_C.root ZeroBiasIdonthave 2025
# sh produce_plots.sh Re-emu-2025C-caloparams2025-newIsoLUT unpacked_2025_eraB_C.root ZeroBiasIdonthave 2025
# sh produce_plots.sh ZeroBias_2025C_392295_0p7_17_30 unpacked_2025_eraB_C.root ZeroBias_2025C_392295_0p7_17_30.root 392295
# sh produce_plots.sh ZeroBias_2025C_392295_0p7_12_26 unpacked_2025_eraB_C.root ZeroBias_2025C_392295_0p7_12_26.root 392295

# sh produce_plots.sh Muon_1_2025C_0p6_11_30_LLR unpacked_2025_eraB_C.root ZeroBias_2025C_392295_0p6_11_30.root 392295
# sh produce_plots.sh Muon_1_2025C_0p7_17_30_LLR unpacked_2025_eraB_C.root ZeroBias_2025C_392295_0p7_17_30.root 392295
# sh produce_plots.sh Muon_1_2025C_0p7_18_27 unpacked_2025_eraB_C.root ZeroBias_2025C_392295_0p7_18_27.root 392295

working_dir='/data_CMS/cms/amella/Run3_2025/'
pwd=$(pwd)
create_file_merge() {
    cat <<EOF >"${1}/MergeTrees/run_2025/${2}/${2}_${3}.config"
TreeClass: TauStage2Trees

MainFiles: ${4}${5}
MainTree: Ntuplizer/TagAndProbe

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
# # make clean &> /dev/null; make 
# # mkdir ${pwd}/MergeTrees/run_2025/${1}

# i=0
# for f in ${working_dir}${1}.root
# do
#   i=$(( i + 1 ))
#   echo "Processing $f file..."
# #   create_file_merge "${pwd}" "${1}" "${i}" "${working_dir}" "${2}" "${f}"
#   ./merge.exe ${pwd}/MergeTrees/run_2025/${1}/${1}_${i}.config 
# done

# # hadd files
# echo 'Hadding merged files..'
# hadd -f ${working_dir}${1}_MERGED.root ${working_dir}${1}_MERGED_*.root

# # matching
# echo 'Matching..'

# cd ${pwd}/MatchAndCompress
# root -l -b <<EOF
# .L MakeTreeForCalibration.C+
# MakeTreeForCalibration("${working_dir}${1}_MERGED.root", "${working_dir}${1}_MATCHED.root", "ZeroBias")
# .q
# EOF

# # Responses
# echo 'Making responses..'
# cd ${pwd}/PlotCalibrationResolution
# root -l -b <<EOF
# .L MakeResolutions_Data_reEMulated.C+
# MakeResolutions("${working_dir}${1}.root", -1, "${1}")
# .L MakeResolutions_Data_unpacked.C+
# MakeResolutions("${working_dir}${2}", "Ntuplizer/TagAndProbe", -1, "${1}")
# .q
# EOF

# Responses
echo 'Making responses..'
cd ${pwd}/PlotCalibrationResolution
root -l -b <<EOF
.L MakeResolutions_Data_reEMulated.C+
MakeResolutions("${working_dir}${1}.root", -1, "${1}", true)
MakeResolutions("${working_dir}${2}", -1, "${1}")
.q
EOF
  
# # TurnOns
# echo 'Making TurnOns..'
# cd ${pwd}/PlotTurnOns
# root -l -b <<EOF
# .L MakeEfficiencies_Data_reEmulated.C+
# MakeEfficiencies("${working_dir}${1}.root", -1, "${1}")
# .L MakeEfficiencies_Data_unpacked.C+
# MakeEfficiencies("${working_dir}${2}", "Ntuplizer/TagAndProbe", -1, "${1}")
# .q
# EOF

# # Rates
# echo 'Making rates..'

# cd ${pwd}/MakeRates

# # root -l -b <<EOF
# # .L Rate_ZeroBias_unpacked.C++
# # Rate("${working_dir}${3}", "histos_2025/histos_rate_ZeroBias_Run${4}_${1}_unpacked.root", ${4}, true)
# # .q
# # EOF

# root -l -b <<EOF
# .L Rate_ZeroBias_reEmu.C++
# Rate("${working_dir}${3}", "histos_2025/histos_rate_ZeroBias_Run${4}_${1}_reEmulated.root", ${4}, true)
# .q
# EOF

# uncomment this part to split responses and turnons into DecayModes

# tau_DM=(0 1 10)
# for decayMode in "${tau_DM[@]}"
#   do 
#   # Responses
#   echo 'Making responses..'
#   cd ${pwd}/PlotCalibrationResolution
#   root -l -b <<EOF
# .L MakeResolutions_Data_reEMulated.C+
# MakeResolutions("${working_dir}${1}_MATCHED.root", -1, "${1}_tauDM$decayMode", $decayMode)
# .L MakeResolutions_Data_unpacked.C+
# MakeResolutions("${working_dir}${2}", "Ntuplizer/TagAndProbe", -1, "${1}_tauDM$decayMode", $decayMode)
# .q
# EOF
# # .L MakeResolutions_Data_reEMulated_noCalib.C+
# # MakeResolutions_noCalib("${working_dir}${1}_MATCHED.root", -1, "${1}")
#   
# # TurnOns
# echo 'Making responses..'
# cd ${pwd}/PlotTurnOns
# root -l -b <<EOF
# .L MakeEfficiencies_Data_reEmulated.C+
# MakeEfficiencies("${working_dir}${1}_MATCHED.root", -1, "${1}_tauDM$decayMode", $decayMode)
# .L MakeEfficiencies_Data_unpacked.C+
# MakeEfficiencies("${working_dir}${2}", "Ntuplizer/TagAndProbe", -1, "${1}_tauDM$decayMode", $decayMode)
# .q
# EOF
# done

echo "Finish -- All good"

