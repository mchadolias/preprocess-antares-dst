#!/bin/sh
#
#
# Usage: ./submit-all.sh
#
## set this to 1 for a DRY RUN, i.e. without submission to SLURM

FILEDIR=$WORK/master_thesis/antares_dst/add_SWIM_Branches/lists/

echo "-------------------------"
echo "Starting script: $(basename $BASH_SOURCE)"

arr=$(find ${FILEDIR} -maxdepth 1 -type f -name "*rest*")

for file in ${arr[@]};
do  
    echo "---------------------"
    echo $file
    echo submit.sh 0 $(basename $file)
    ./submit.sh 0 $(basename $file) 
done
