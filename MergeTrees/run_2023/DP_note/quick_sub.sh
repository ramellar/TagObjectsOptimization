#!/bin/bash

# Specify the directory path
directory=""

# Specify the string to search for and the string to replace with
search_string="old_string"
replace_string="new_string"

# Use find command to locate all files in the directory
find "$directory" -type f -exec sed -i "s/$search_string/$replace_string/g" {} +

