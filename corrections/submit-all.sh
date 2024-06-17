#!/bin/sh
#
#
# Usage: ./submit-all.sh <sub_folder>
#
## set this to 1 for a DRY RUN, i.e. without submission to SLURM

SUB_FOLDER=$1
FILEDIR=$WORK/master_thesis/antares_dst/corrections/lists/${SUB_FOLDER}

echo "-------------------------"
echo "Starting script: $(basename $BASH_SOURCE)"

arr=$(find ${FILEDIR} -maxdepth 1 -type f -name "*ending_run_*")

for file in ${arr[@]};
do  
    echo "---------------------"
    echo $file
    echo submit.sh 0 ${FOLDER} $(basename $file)
    ./submit.sh 0 $(basename $file) ${SUB_FOLDER}
done
