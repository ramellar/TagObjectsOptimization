#!/usr/bin/bash
set -e

# sh produce_plots_unpacked.sh <tag> <miniaod_root_file> <gen_cut-for_pt> <cut_pt_eta_barrel_endacap> 
# sh produce_plots_unpacked.sh gen_gen_matched_MC25 gen_matched_MiniAOD_MC_2025_E.root gen_matched_MiniAOD_MC_2025_E.root 2025

working_dir='/data_CMS/cms/amella/Run3_2025/MC25_Winter_optmization/'
pwd=$(pwd)

if [ -n "$5" ]; then  
    echo "Using MakeResolutions_Data_unpacked_with_gen_cut.C+"
    root -l -b <<EOF
.L MakeResolutions_Data_unpacked_with_gen_cut.C+
MakeResolutions("${working_dir}${2}", "Ntuplizer_noTagAndProbe/TagAndProbe", -1, "${3}", "${4}", "${1}")
.q
EOF
else
    echo "Using MakeResolutions_Data_gen_unpacked.C+"
    root -l -b <<EOF
.L MakeResolutions_Data_gen_unpacked.C+
MakeResolutions("${working_dir}${2}", "Ntuplizer_noTagAndProbe/TagAndProbe", -1, "${1}")
.q
EOF
fi

echo "Finish -- All good, responses in PlotCalibrationResolution/ROOTs/ROOTs_2025/"
echo "TurnOns in PlotTurnOns/ROOTs/ROOTs_2025/"
