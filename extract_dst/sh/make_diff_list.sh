#!/bin/bash
#
## Usage: make_diff_list.sh antdst_files extracted_files missing_files
#
# This script compares two lists of files and writes the difference to a third file.
# The script was made to add the file list that had been re-processed and now uncorrupted. 
# The first list is the list of files in with the new version of the antares-dst files.
# The second list is the list of files that had already been extracted.
# The third list is the list of files that are in the first list but not in the second list.
#
## Arguments:
# antdst_files: The list of files in the new version of the antares-dst files.
# extracted_files: The list of files that had already been extracted.
# missing_files: The list of files that are in the first list but not in the second list.
#
## Example:
# ./make_diff_list.sh full_antdst_muons.txt  full_extracted_muons.txt  missing_muons.txt


# Check if the number of arguments is correct
if [ "$#" -ne 3 ]; then
  echo "Usage: $0 antdst_files extracted_files missing_files"
  exit 1
fi

# Assigning arguments to variables
antdst_files="$1"
extracted_files="$2"
missing_files="$3"

# Check if the input files exist
if [ ! -f "$antdst_files" ] || [ ! -f "$extracted_files" ]; then
  echo "One or both input files do not exist."
  exit 1
fi

# Sort the input files (comm requires sorted input)
sort "$antdst_files" -o sorted_antdst_files.txt
sort "$extracted_files" -o sorted_extracted_files.txt

comm -23 sorted_antdst_files.txt sorted_extracted_files.txt > "$missing_files"

# Clean up sorted temporary files
rm sorted_antdst_files.txt sorted_extracted_files.txt

echo "Difference written to $missing_files"
