#!/bin/bash

# Specify the directory path
directory="/home/llr/cms/mchiusi/Run3preparation/Run3preparation_2023/CMSSW_11_0_2/src/TauObjectsOptimization/MergeTrees/run_2023/2023_07_27_olivier"

# Specify the string to search for and the string to replace with
search_string="2023_07_27_olivier/new_calo_params"
replace_string="2023_07_27_olivier/new_calo_params"

# Use find command to locate all files in the directory
find "$directory" -type f -exec sed -i "s|$search_string|$replace_string|g" {} +

