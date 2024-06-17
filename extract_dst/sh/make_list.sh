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


find ${DIR} -maxdepth 1 -type f ! -size 0c -name "*.root" -printf '%f\n' > ${OUTFILE}

declare -a pattern_array=("_numu_${1}_CC" "_numu_${1}_NC" "_anumu_${1}_CC" "_anumu_${1}_NC" \
                          "_nutau_${1}_CCmu" "_nutau_${1}_CCshow"  "_nutau_${1}_NC" \
				      "_anutau_${1}_CCmu" "_anutau_${1}_CCshow" "_anutau_${1}_NC" \
                          "_nue_${1}_CC" "_nue_${1}_NC" "_anue_${1}_CC" "_anue_${1}_NC" "_mupage")


# split the list
for i in $(seq 0 9);
do
     grep -E "[0-9]{5}${i}" ${OUTFILE} > end_${i}.txt
    
     for pat in "${pattern_array[@]}"
     do
	     grep $pat end_${i}.txt > mc${pat}_end_${i}.txt
     done

done

rm -rfv end_{0..9}.txt
find . -size 0c -exec rm -if {} \;

printf "\n======== Script complete ======== \n\n"