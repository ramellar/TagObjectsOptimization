#!/usr/bin/bash
set -e

# sh produce_plots_unpacked.sh <tag> <miniaod_root_file> <rate_file> <run_number>

# working_dir='/data_CMS/cms/amella/Run3_2024/Run3_2024/unpacked_2024/Muon_Run2024I_MINIAOD/'
working_dir='/data_CMS/cms/amella/Run3_2024/Run3_2024/'
pwd=$(pwd)

# # Responses
# echo 'Making responses..'
# cd ${pwd}/PlotCalibrationResolution
# root -l -b <<EOF
# .L MakeResolutions_Data_unpacked.C+
# MakeResolutions("${working_dir}${2}", "Ntuplizer/TagAndProbe", -1, "${1}")
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
Rate("${working_dir}${3}", "histos_2024/histos_rate_ZeroBias_Run${4}_${1}_unpacked.root", ${4})
.q
EOF

echo "Finish -- All good, responses in PlotCalibrationResolution/ROOTs/ROOTs_2024/"
echo "TurnOns in PlotTurnOns/ROOTs/ROOTs_2024/"
