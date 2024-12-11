#!/usr/bin/bash
set -e

# sh produce_plots.sh <tag_data_validation> <miniaod.root> <rate.root> <run_number>

working_dir='/data_CMS/cms/mchiusi/Run3preparation/Run3_2024/re-emu_Sept/'
pwd=$(pwd)

create_file_merge() {
    cat <<EOF >"${1}/MergeTrees/run_2024/${2}/${2}_${3}.config"
TreeClass: TauStage2Trees

MainFiles: ${4}${5}
MainTree: Ntuplizer/TagAndProbe

SecondaryTrees.N: 1
SecondaryTrees.0.Tree: ZeroBias/ZeroBias
SecondaryTrees.0.Files: ${6}

OutputFile:  ${4}${2}_MERGED_${3}.root
EOF
}

# merging
echo 'Merging..'

cd ${pwd}/MergeTrees
make clean &> /dev/null; make &> /dev/null
mkdir ${pwd}/MergeTrees/run_2024/${1}

i=0
for f in ${working_dir}${1}_*.root
do
  i=$(( i + 1 ))
  echo "Processing $f file..."
  create_file_merge "${pwd}" "${1}" "${i}" "${working_dir}" "${2}" "${f}"x
  ./merge.exe ${pwd}/MergeTrees/run_2024/${1}/${1}_${i}.config 
done

# hadd files
echo 'Hadding merged files..'
hadd -f ${working_dir}${1}_MERGED.root ${working_dir}${1}_MERGED_*.root

# matching
echo 'Matching..'

cd ${pwd}/MatchAndCompress
root -l -b <<EOF
.L MakeTreeForCalibration.C+
MakeTreeForCalibration("${working_dir}${1}_MERGED.root", "${working_dir}${1}_MATCHED.root", "Ntuplizer_TagAndProbe")
.q
EOF

# Responses
echo 'Making responses..'
cd ${pwd}/PlotCalibrationResolution
root -l -b <<EOF
.L MakeResolutions_Data_reEMulated.C+
MakeResolutions("${working_dir}${1}_MATCHED.root", -1, "${1}")
.L MakeResolutions_Data_reEMulated_noCalib.C+
MakeResolutions_noCalib("${working_dir}${1}_MATCHED.root", -1, "${1}")
.L MakeResolutions_Data_unpacked.C+
MakeResolutions("${working_dir}${2}", "Ntuplizer/TagAndProbe", -1, "${1}")
.q
EOF
  
# TurnOns
echo 'Making responses..'
cd ${pwd}/PlotTurnOns
root -l -b <<EOF
.L MakeEfficiencies_Data_reEmulated.C+
MakeEfficiencies("${working_dir}${1}_MATCHED.root", -1, "${1}")
.L MakeEfficiencies_Data_unpacked.C+
MakeEfficiencies("${working_dir}${2}", "Ntuplizer/TagAndProbe", -1, "${1}")
.q
EOF

# Rates
echo 'Making rates..'

cd ${pwd}/MakeRates
root -l -b <<EOF
.L Rate_ZeroBias_unpacked.C++
Rate("${working_dir}${3}", "histos_2024/histos_rate_ZeroBias_Run${4}_${1}_unpacked.root", ${4})
.L Rate_ZeroBias_reEmu.C++
Rate("${working_dir}${3}", "histos_2024/histos_rate_ZeroBias_Run${4}_${1}_reEmulated.root", ${4})
.q
EOF

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
#   # TurnOns
#   echo 'Making responses..'
#   cd ${pwd}/PlotTurnOns
#   root -l -b <<EOF
# .L MakeEfficiencies_Data_reEmulated.C+
# MakeEfficiencies("${working_dir}${1}_MATCHED.root", -1, "${1}_tauDM$decayMode", $decayMode)
# .L MakeEfficiencies_Data_unpacked.C+
# MakeEfficiencies("${working_dir}${2}", "Ntuplizer/TagAndProbe", -1, "${1}_tauDM$decayMode", $decayMode)
# .q
# EOF
#   done

echo "Finish -- All good, responses in PlotCalibrationResolution/ROOTs/ROOTs_2024/"
echo "TurnOns in PlotTurnOns/ROOTs/ROOTs_2024/"
