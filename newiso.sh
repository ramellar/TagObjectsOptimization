#!/usr/bin/bash
set -e
# sh newiso.sh sc_binning5_c_original Zerobias_Run386604 MC25_MiniAOD 386604
# sh newiso.sh sc_binning1p1 Zerobias_Run386604 MC25_MiniAOD 386604

pwd=$(pwd)
working_dir='/data_CMS/cms/amella/Run3_2025/MC2025_caloparams-2025/'

# Defining new super compression scheme
echo "Calibrating..."


# cd ${pwd}/Calibrate/

root -l -b <<EOF
.L ApplyCalibration.C+
ApplyCalibration("/data_CMS/cms/amella/Run3_2025/MC25_Winter_optmization/MC2025_caloparams-2025/", \
                "/data_CMS/cms/amella/Run3_2025/${1}_CALIBRATED.root", \
                "corrections_2025/corrections_BDT_training_HCALcFeb-caloParams_2025_conservative-ZS-MC25W_corrected.root")
.q
EOF

root -l <<EOF
.L ApplyCalibration_ZeroBias.C+
ApplyCalibrationZeroBias("${working_dir}${2}.root", \
                         "/data_CMS/cms/amella/Run3_2025/${2}_${1}_CALIBRATED.root", \
                         "corrections_2025/corrections_BDT_training_HCALcFeb-caloParams_2025_conservative-ZS-MC25W_corrected.root")
.q
EOF


#Isolation
cd ../Isolate/

echo "Isolation.."

root -l -b <<EOF
.L Build_Isolation.C+
Build_Isolation("/data_CMS/cms/amella/Run3_2025/${1}_CALIBRATED.root" , "LUTisolation_2025Rederivation_${1}.root", 2 , 8)
.q
EOF

echo "Relaxed Isolation.."

root -l -b <<EOF
.L Fill_RelaxedIsolation_gridsearch.C+
Fill_RelaxedIsolation_TH3("LUTisolation_2025Rederivation_${1}.root", "ROOTs4LUTs_2025/LUTrelaxation_${1}.root")
.q
EOF

# Rates
echo 'Making rates..'

cd ${pwd}/MakeRates

if [ ! -d ${pwd}/MakeRates/histos_2025 ]; then
    mkdir ${pwd}/MakeRates/histos_2025
    echo "Directory MakeRates/histos_2025 created."
fi

root -l -b <<EOF
.L Rate_ZeroBias_unpacked.C+
Rate("${working_dir}${2}.root", "histos_2025/histos_rate_ZeroBias_Run${4}_${1}_unpacked.root", ${4})
.q
EOF

root -l -b <<EOF
.L Rate_ZeroBias_gridSearch.C+
Rate("/data_CMS/cms/amella/Run3_2025/${2}_${1}_CALIBRATED.root", "histos_2025/histos_rate_ZeroBias_Run${4}_${1}_optimisation.root", \
     "../Isolate/ROOTs4LUTs_2025/LUTrelaxation_${1}.root", ${4})
.q
EOF

# # Threshold at Fixed rate 14kHz
# echo 'Computing Thresholds..'

# cd ${pwd}/CompareRates
# root -l -b <<EOF
# .L CompareRates_ZeroBias_gridSearch_withUnpacked.C+
# compare("../MakeRates/histos_2025/histos_rate_ZeroBias_Run${4}_${1}_unpacked.root", \
#         "../MakeRates/histos_2025/histos_rate_ZeroBias_Run${4}_${1}_optimisation.root", \
#         "../MakeRates/histos_2025/thresholds_fixedrate_ZeroBias_Run${4}_${1}unpacked_optimization.root")
# .q
# EOF

# # TunrOns
# echo 'Making TunrOns..'

# cd ${pwd}/MakeTurnOns
# root -l -b <<EOF
# .L ApplyIsolationForTurnOns_gridSearch.C+
# ApplyIsolationForTurnOns("/data_CMS/cms/amella/Run3_2025/${1}_CALIBRATED.root", \
#                          "/data_CMS/cms/amella/Run3_2025/Tau_MC_TURNONS_FIXEDRATE_14kHz_${1}.root", \
#                          "../Isolate/ROOTs4LUTs_2025/LUTrelaxation_${1}.root", \
#                          "../MakeRates/histos_2025/thresholds_fixedrate_ZeroBias_Run${4}_${1}unpacked_optimization.root", \
#                          "${working_dir}${3}.root", 14, 0)
# .q
# EOF

# cd ${pwd}/CompareGridSearchTrunons
# if [ ! -d ${pwd}/CompareGridSearchTrunons/FMs/FMs_2025 ]; then
#     mkdir ${pwd}/CompareGridSearchTrunons/FMs/FMs_2025
#     echo "Directory CompareGridSearchTrunons/FMs/FMs_2025 created."
# fi

# root -l -b <<EOF
# .L BestFMturnOns_gridSearch.C+
# compare("/data_CMS/cms/amella/Run3_2025/Tau_MC_TURNONS_FIXEDRATE_14kHz_${1}.root", \
#         "FMs/FMs_2025/FM_orderd_turnons_FIXEDRATE_14kHz_Run${4}_${1}.txt", \
#         "../MakeRates/histos_2025/thresholds_fixedrate_ZeroBias_Run${4}_${1}unpacked_optimization.root", \
#         "../MakeRates/histos_2025/histos_rate_ZeroBias_Run${4}_${1}_unpacked.root", "FM")
# .q
# EOF

echo "Making LUT with 0.7,13,46"

cd ${pwd}/Isolate/

root -l -b <<EOF
.L MakeTauIsoLUT.C+
MakeTauIsoLUT_Options("ROOTs4LUTs_2025/LUTrelaxation_${1}.root","LUTs/LUTs_2025/NewLUTrelaxation_${1}.txt", 0.7, 13, 46)
.q
EOF

echo "LUT created!"
echo "NewLUTrelaxation_${1}.txt"
