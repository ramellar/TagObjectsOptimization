import os
import subprocess
import argparse

# Set variables
working_dir = '/data_CMS/cms/mchiusi/Run3preparation/ruth_MC24_Winter_optimization/'
pwd = os.getcwd()

# Function to create the config file
def create_config_file(output_dir, tag, input_file):
    config_content = f"""
Trainer: GBRTrain
Trainer: GBRLikelihoodTrain
NumberOfRegressions: 1
OutputDirectory: {output_dir}/Calibrate/forests_2024

Regression.1.Name: BDT_training_optimization_{tag}
Regression.1.InputFiles: {input_file}{tag}_COMPRESSED.root
Regression.1.Tree: outTreeForCalibration
Regression.1.Method: BDT
Regression.1.Options: MinEvents=200:Shrinkage=0.1:NTrees=1000:MinSignificance=5.0:RandomSeed=EventNumber
Regression.1.DoErrors: False
Regression.1.DoCombine: False
Regression.1.VariablesEB: abs(compressedieta):compressedE:L1Tau_hasEM:L1Tau_isMerged
Regression.1.VariablesEB: abs(compressedieta):compressedE:compressedsortedshape
Regression.1.VariablesEE: abs(compressedieta):compressedE:L1Tau_hasEM:L1Tau_isMerged
Regression.1.VariablesEE: abs(compressedieta):compressedE:compressedsortedshape
Regression.1.Target: Target
Regression.1.CutBase: OfflineTau_pt>18
Regression.1.CutEB: OfflineTau_pt>18
Regression.1.CutEE: OfflineTau_pt>18
    """
    os.makedirs(f"{output_dir}/Calibrate/RegressionTraining/run_2024", exist_ok=True)
    with open(f"{output_dir}/Calibrate/RegressionTraining/run_2024/{tag}.config", 'w') as config_file:
        config_file.write(config_content)

# Function to create the merge config file
def create_file_merge(output_dir, tag, prefix, working_dir, aod_file, raw_file):
    merge_content = f"""
TreeClass: TauStage2Trees

MainFiles: {aod_file}
MainTree: Ntuplizer_noTagAndProbe/TagAndProbe

SecondaryTrees.N: 1
SecondaryTrees.0.Tree: ZeroBias/ZeroBias
SecondaryTrees.0.Files: {raw_file}

OutputFile:  {working_dir}{tag}_MERGED_{prefix}.root
    """
    os.makedirs(f"{output_dir}/MergeTrees/run_2024/{tag}", exist_ok=True)
    with open(f"{output_dir}/MergeTrees/run_2024/{tag}/{tag}_{prefix}.config", 'w') as config_file:
        config_file.write(merge_content)

# Function to run subprocess commands
def run_subprocess(command):
    try:
        subprocess.run(command, check=True, shell=True)
    except subprocess.CalledProcessError as e:
        print(f"Error running command: {command}\n{e}")
        exit(1)

# Merging and matching function
def merge_and_match(tag_raw):
    print('Merging..')
    os.chdir(f"{pwd}/MergeTrees")
    run_subprocess("make clean &> /dev/null; make &> /dev/null")

    if not os.path.exists(f"{pwd}/MergeTrees/run_2024/{tag_raw}"):
        os.makedirs(f"{pwd}/MergeTrees/run_2024/{tag_raw}")
        print(f"Directory MergeTrees/run_2024/{tag_raw} created.")

    prefixes = ["VBF_"] # ["VBF_" "GluGlu" "VBFp"]
    for prefix in prefixes:
        raw_file = os.popen(f"ls {working_dir}{prefix}*RAW*.root 2>/dev/null").read().strip()
        aod_file = os.popen(f"ls {working_dir}{prefix}*MINIAOD*.root 2>/dev/null").read().strip()
        if raw_file and aod_file:
            print(f"Processing RAW file: {raw_file} and MINIAOD file: {aod_file}...")
            create_file_merge(pwd, tag_raw, prefix, working_dir, aod_file, raw_file)
            run_subprocess(f"./merge.exe {pwd}/MergeTrees/run_2024/{tag_raw}/{tag_raw}_{prefix}.config")
        else:
            print(f"No matching RAW and MINIAOD files found for prefix {prefix}")
            continue

    # Hadding merged files
    print('Hadding merged files..')
    run_subprocess(f"hadd {working_dir}{tag_raw}_MERGED.root {working_dir}{tag_raw}_MERGED_*.root")

    print('Matching..')
    os.chdir(f"{pwd}/MatchAndCompress")

    subprocess.run(f"""
    root -l -b <<EOF
    .L MakeTreeForCalibration.C+
    MakeTreeForCalibration("{working_dir}{tag_raw}_MERGED.root", "{working_dir}{tag_raw}_MATCHED.root", "Ntuplizer_noTagAndProbe_TagAndProbe")
    .q
    EOF
    """, shell=True)

    # Compressed
    run_subprocess(f"python3 produceTreeWithCompressedVars.py -i {working_dir}{tag_raw}_MATCHED.root \
                   -o {working_dir}{tag_raw}_COMPRESSED.root")

# Calibration function
def calibration(tag_raw, tag_zerobias):
    print('Calibrating..')
    os.makedirs(f"{pwd}/Calibrate/forests_2024", exist_ok=True)
    os.makedirs(f"{pwd}/Calibrate/corrections_2024", exist_ok=True)

    os.chdir(f"{pwd}/Calibrate/RegressionTraining")
    run_subprocess("make clean &> /dev/null || { echo 'make clean failed'; exit 1; }")
    run_subprocess("make")

    create_config_file(pwd, tag_raw, working_dir)
    run_subprocess(f"./regression.exe run_2024/{tag_raw}.config")

    # Run the makeTH4_LUT.py script for LUT generation
    os.chdir(f"{pwd}/Calibrate/")  # Navigate to the Calibrate directory
    run_subprocess(f"python3 makeTH4_LUT.py -i forests_2024/BDT_training_optimization_{tag_raw}_results.root \
                       -o corrections_2024/corrections_BDT_training_{tag_raw}.root")

    # Apply calibration
    subprocess.run(f"""
    root -l -b <<EOF
    .L ApplyCalibration.C+
    ApplyCalibration("{working_dir}{tag_raw}_COMPRESSED.root", "{working_dir}{tag_raw}_CALIBRATED.root", \
                     "corrections_2024/corrections_BDT_training_{tag_raw}.root")
    .q
    EOF
    """, shell=True)

    subprocess.run(f"""
    root -l -b <<EOF
    .L ApplyCalibration_ZeroBias.C+
    ApplyCalibrationZeroBias("{working_dir}{tag_zerobias}.root", "{working_dir}{tag_zerobias}_CALIBRATED.root", \
                             "corrections_2024/corrections_BDT_training_{tag_raw}.root")
    .q
    EOF
    """, shell=True)

    subprocess.run(f"""
    root -l -b <<EOF
    .L MakeTauCalibLUT.C+
    MakeTauCalibLUT("LUTs_2024/LUTcalibration_{tag_raw}.txt", "corrections_2024/corrections_BDT_training_{tag_raw}.root")
    .q
    EOF
    """, shell=True)

# Isolation function
def isolation(tag_raw):
    print('Isolation..')
    os.chdir(f"{pwd}/Isolate")
    os.makedirs(f"{pwd}/Isolate/ROOTs4LUTs_2024", exist_ok=True)

    subprocess.run(f"""
    root -l -b <<EOF
    .L Build_Isolation.C+
    Build_Isolation("{working_dir}{tag_raw}_CALIBRATED.root", "ROOTs4LUTs_2024/LUTisolation_{tag_raw}.root", 3 , 14)
    .q
    EOF
    """, shell=True)

    subprocess.run(f"""
    root -l -b <<EOF
    .L Fill_RelaxedIsolation_gridsearch.C+
    Fill_RelaxedIsolation_TH3("ROOTs4LUTs_2024/LUTisolation_{tag_raw}.root", "ROOTs4LUTs_2024/LUTrelaxation_{tag_raw}.root")
    .q
    EOF
    """, shell=True)

# Rate function
def rate(tag_raw, tag_zerobias, run):
    print('Making rates..')
    os.chdir(f"{pwd}/MakeRates")
    os.makedirs(f"{pwd}/MakeRates/histos_2024", exist_ok=True)

    subprocess.run(f"""
    root -l -b <<EOF
    .L Rate_ZeroBias_unpacked.C+
    Rate("{working_dir}{tag_zerobias}.root", "histos_2024/histos_rate_ZeroBias_Run{run}_{tag_raw}_unpacked.root", {run})
    .q
    EOF
    """, shell=True)

    subprocess.run(f"""
    root -l -b <<EOF
    .L Rate_ZeroBias_gridSearch.C+
    Rate("{working_dir}{tag_zerobias}_CALIBRATED.root", "histos_2024/histos_rate_ZeroBias_Run{run}_{tag_raw}_optimisation.root", \
         "../Isolate/ROOTs4LUTs_2024/LUTrelaxation_{tag_raw}.root", {run})
    .q
    EOF
    """, shell=True)

# Final comparison function
def final_comparison(tag_raw, tag_aod, run):
    print('Computing Thresholds..')
    os.chdir(f"{pwd}/CompareRates")

    subprocess.run(f"""
    root -l -b <<EOF
    .L CompareRates_ZeroBias_gridSearch_withUnpacked.C+
    compare("../MakeRates/histos_2024/histos_rate_ZeroBias_Run{run}_{tag_raw}_unpacked.root", \
            "../MakeRates/histos_2024/histos_rate_ZeroBias_Run{run}_{tag_raw}_optimisation.root", \
            "../MakeRates/histos_2024/thresholds_fixedrate_ZeroBias_Run{run}_{tag_raw}_unpacked_optimization.root")
    .q
    EOF
    """, shell=True)

    print('Making TurnOns..')
    os.chdir(f"{pwd}/MakeTurnOns")

    subprocess.run(f"""
    root -l -b <<EOF
    .L ApplyIsolationForTurnOns_gridSearch.C+
    ApplyIsolationForTurnOns("{working_dir}{tag_raw}_CALIBRATED.root", \
                             "{working_dir}Tau_MC_TURNONS_FIXEDRATE_14kHz_{tag_raw}.root", \
                             "../Isolate/ROOTs4LUTs_2024/LUTrelaxation_{tag_raw}.root", \
                             "../MakeRates/histos_2024/thresholds_fixedrate_ZeroBias_Run{run}_{tag_raw}_unpacked_optimization.root", \
                             "{working_dir}{tag_aod}.root", 14, 0)
    .q
    EOF
    """, shell=True)

    print('Comparing turn-ons..')
    os.chdir(f"{pwd}/CompareGridSearchTrunons")
    os.makedirs(f"{pwd}/CompareGridSearchTrunons/FMs/FMs_2024", exist_ok=True)

    subprocess.run(f"""
    root -l -b <<EOF
    .L BestFMturnOns_gridSearch.C+
    compare("{working_dir}Tau_MC_TURNONS_FIXEDRATE_14kHz_{tag_raw}.root", \
            "FMs/FMs_2024/FM_orderd_turnons_FIXEDRATE_14kHz_Run{run}_{tag_raw}.txt", \
            "../MakeRates/histos_2024/thresholds_fixedrate_ZeroBias_Run{run}_{tag_raw}_unpacked_optimization.root", \
            "../MakeRates/histos_2024/histos_rate_ZeroBias_Run{run}_{tag_raw}_unpacked.root", "FM")
    .q
    EOF
    """, shell=True)

    print(f"Finish -- All good, best turnOns in CompareGridSearchTrunons/FMs/FMs_2024/FM_orderd_turnons_FIXEDRATE_14kHz_Run{run}_{tag_raw}.txt")

# Argument parsing
if __name__ == "__main__":
    ''' python3 run_optimisation.py --tag_raw MC24_version13_test_python --tag_zerobias raw_ntuples --tag_aod VBF_MINIAOD --run 386604 '''

    parser = argparse.ArgumentParser(description="Optimisation: Run the calibration and isolation workflow.")
    parser.add_argument('--tag_raw',      type=str, help="tag used to identify the raw in the working directory, it can be customised")
    parser.add_argument('--tag_aod',      type=str, help="tag used to identify the MINIAOD in the working directory")
    parser.add_argument('--tag_zerobias', type=str, help="tag used to identify the ZeroBias in the working directory")
    parser.add_argument('--run',          type=str, help="ZeroBias run number")
    args = parser.parse_args()

    # Run all the functions with user-provided arguments
    merge_and_match(args.tag_raw)
    calibration(args.tag_raw, args.tag_zerobias)
    isolation(args.tag_raw)
    rate(args.tag_raw, args.tag_zerobias, args.run)
    final_comparison(args.tag_raw, args.tag_aod, args.run)
