#!/bin/sh

# Path: manage_filelist/make_list.sh
# Usage: ./make_list.sh a /home/wecapstor3/capn/mppi19/ANTARES/MC/v4.2/antdst full_antares_dst_list.txt

# This script is used to create a list of files in the directory
# The list is then split into smaller lists based on the file name

ENERGY_RANGE=$1 # a for energies 5 GeV - 20 TeV / b for 20 TeV to 100 PeV 
DIR=$2
OUTFILE=$3

echo "-------------------------------------"
echo "Running bash scipt: $(basename $BASH_SOURCE)"
echo  "Energy range: ${ENERGY_RANGE}"
echo  "Directory: ${DIR}"
echo  "Output file: ${OUTFILE}"


find ${DIR} -maxdepth 1 -type f ! -size 0c -name "*.txt" -printf '%f\n' > ${OUTFILE}

declare -a pattern_array=("numu_${1}_CC" "numu_${1}_NC" "anumu_${1}_CC" "anumu_${1}_NC" \
                          "nutau_${1}_CCmu" "nutau_${1}_CCshow"  "nutau_${1}_NC" \
				      "anutau_${1}_CCmu" "anutau_${1}_CCshow" "anutau_${1}_NC" \
                          "nue_${1}_CC" "nue_${1}_NC" "anue_${1}_CC" "anue_${1}_NC" "mupage")


# split the list    
 for pat in "${pattern_array[@]}"
 do
     grep $pat ${OUTFILE} > list_${pat}_end_all_runs.txt
 done


printf "\n======== Script complete ======== \n\n"