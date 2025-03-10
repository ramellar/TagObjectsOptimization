#!/usr/bin/bash
set -e

# sh produce_plots_unpacked.sh <tag> <miniaod_root_file>  
# sh produce_plots_unpacked.sh gen_gen_matched_MC25 gen_matched_MiniAOD_MC_2025_E.root

working_dir='/data_CMS/cms/amella/Run3_2025/MC25_Winter_optmization/'
pwd=$(pwd)

# Responses
echo 'Making responses..'
cd ${pwd}/PlotCalibrationResolution
echo "Using MakeResolutions_Data_gen_unpacked.C+"
root -l -b <<EOF
.L MakeResolutions_Data_gen_unpacked.C+
MakeResolutions("${working_dir}${2}", "Ntuplizer_noTagAndProbe/TagAndProbe", -1, "${1}")
.q
EOF
fi

echo "Finish -- All good, responses in PlotCalibrationResolution/ROOTs/ROOTs_2025/"
echo "TurnOns in PlotTurnOns/ROOTs/ROOTs_2025/"
