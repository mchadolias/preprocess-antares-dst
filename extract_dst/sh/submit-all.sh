#!/bin/sh
#
### usage:
###      ./submit-all.sh <dry run flag> <output folder> <sub folder> 
###
###       e.g. ./submit-all.sh 0 extracted split_files for muons specify also the names of the files *mupage*
###       e.g. ./submit-all.sh 0 extracted full_ending_runlist for neutrinos specify also the names of the files *nu*

echo "--------------------"
echo "Starting script:" $(basename $BASH_SOURCE)
echo "Parameters: \
     - dry run flag: ${1}  \
     - output folder: ${2} \
     - submission folder: ${3}"

## set this to 1 for a DRY RUN, i.e. without submission to SLURM
# Specify DRY_RUN flag
DRY_RUN=${1}

### Specify the folder where the antares extracted dst files are stored
FOLDER=${2} # e.g. extracted (current folder structure)

### Specify the subfolder for submission
SUB_FOLDER=${3} 

### for muons or neutrinos  
### - neutrinos saved on full_ending_runlist 
### - muons saved on split_files

FILEDIR=$WORK/master_thesis/antares_dst/extract_dst/lists/mc/${SUB_FOLDER}

echo "-------------------------"
echo "Starting script: $(basename $BASH_SOURCE)"

arr=$(find ${FILEDIR} -maxdepth 1 -type f -name "*mupage*")

for file in ${arr[@]};
do  
    echo "---------------------"
    echo $file
    echo sh/submit.sh ${DRY_RUN} ${FOLDER} ${SUB_FOLDER} $(basename $file)
    sh/submit.sh ${DRY_RUN} ${FOLDER} ${SUB_FOLDER} $(basename $file)
done
