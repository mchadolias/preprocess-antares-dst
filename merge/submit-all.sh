#!/bin/sh
#
## Usage:
#   ./submit-all.sh <DRY_RUN> <FILE_FOLDER> <END_FOLDER>
#
## Example:
#   ./submit-all.sh 0 extracted merged/merged_before_nnfit
#
## Arguments:
#   DRY_RUN: 0 for submission to SLURM, 1 for frontend execution, 2 for test run
#   FILE_FOLDER: folder where the files are located
#   END_FOLDER: folder where the merged files will be stored


echo "--------------------"
echo "Starting script:" $(basename $BASH_SOURCE)
echo "Parameters: \
     - dry run flag: ${1}  \
     - input folder: ${2} \
     - output folder: ${3}"

# Check the value of DRY_RUN and set the WORK accordingly
DRY_RUN=${1}

# Specify the folder where the antares extracted dst files are stored
FILE_FOLDER=${2} # e.g. extracted (current folder structure)
END_FOLDER=${3} # e.g. merged/merged_before_nnfit
JOB_SCRIPT="job_merge_list.sh"

# Check the value of JOB_SCRIPT and set the FILEDIR accordingly
if [[ "$JOB_SCRIPT" == "job_merge_list.sh" ]]; then
    FILEDIR=${WORK}/master_thesis/antares_dst/merge/lists/filename
elif [[ "$JOB_SCRIPT" == "job_merge.sh" ]]; then
    FILEDIR=${WORK}/master_thesis/antares_dst/merge/lists/files
fi

arr=$(find ${FILEDIR} -maxdepth 1 -type f -name "*end_all_runs*" ! -name "*.root" ! -size 0c)

for file in ${arr[@]};
do  
    echo "---------------------"
    echo $file
    echo submit.sh ${DRY_RUN} ${FILE_FOLDER} ${END_FOLDER} ${JOB_SCRIPT} $(basename $file)
    ./submit.sh ${DRY_RUN} ${FILE_FOLDER} ${END_FOLDER} ${JOB_SCRIPT} $(basename $file)
done
