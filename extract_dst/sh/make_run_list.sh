#!/bin/bash
# Path: manage_filelist/fetch_run_files.sh
# Usage: ./fetch_run_files.sh
# 
## Select run id to fetch mc production files. (e.g. 34567)

# This script is used to fetch the mc production files based on the run id
OUTFILE=test_run.txt
DIR=$(pwd)

rm $OUTFILE
printf "\nSelect run id to fetch mc production files.\n"

# Get the run id as an input
read -p 'Enter the document ID:' run_id

if ! [[ "$run_id" =~ [1-9][0-9]{4}$ ]]; then
    echo "Error: Wrong run id number."
    #exit 1
fi

declare -a pattern_array=("_numu_a_CC" "_numu_a_NC" "_anumu_a_CC" "_anumu_a_NC" \
                          "_nutau_a_CCmu" "_nutau_a_CCshow"  "_nutau_a_NC" \
				      "_anutau_a_CCmu" "_anutau_a_CCshow" "_anutau_a_NC" \
                          "_nue_a_CC" "_nue_a_NC" "_anue_a_CC" "_anue_a_NC" \
                          "_mupage")                        

for pat in "${pattern_array[@]}"
do
    file="MC_0$run_id${pat}_reco.root"
	echo "$file" >> $OUTFILE
done

printf "\n======== Script complete ======== \n\n"
cat $OUTFILE