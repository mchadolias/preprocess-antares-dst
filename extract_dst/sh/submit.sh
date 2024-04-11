#!/bin/sh
### usage:
###       ./submit.sh <dry run flag> <output folder> <sub folder> <input list>
###
###       e.g. ./submit.sh 0 extracted split_files list_mupage_end_1_part00.txt
###       e.g. ./submit.sh 0 extracted full_ending_runlist list_nue_a_NC_end_1.txt
#

echo "--------------------"
echo "Starting script:" $(basename $BASH_SOURCE)
echo "Parameters: \
     - dry run flag: ${1}  \
     - output folder: ${2} \
     - submission folder: ${3} \
     - input list: ${4}"

### set this to 1 for a DRY RUN, i.e. without submission to SLURM
DRY_RUN=$1

### FOLDER where the antares extracted dst files are stored
FOLDER=$2 # e.g. extracted

### FOLDER for submission
SUB_FOLDER=$3

### FILENAME of a list of files in subdir filelists
THIS_INPUT_LIST=$4

echo "--------------------"
echo "Starting script:" $(basename $BASH_SOURCE)
echo "Parameters: \
     - input list: ${THIS_INPUT_LIST}  \
     - path extension: ${FOLDER}"

### PROJECT DIR for logs and the worker script
if [ -z "$THIS_INPUT_PROJ_DIR" ]; then
    THIS_PROJ_DIR=/home/saturn/capn/$USER/master_thesis/antares_dst/extract_dst
else
    THIS_PROJ_DIR=$THIS_INPUT_PROJ_DIR
fi

### SET INPUT FILELIST
PROJ_DIR_CONTAINER=/home/master_thesis/antares_dst/extract_dst
INPUT_LIST_DIR=${PROJ_DIR_CONTAINER}/lists/mc/${SUB_FOLDER}
## VARIABLE to pass into the container
MY_INPUT_LIST=${INPUT_LIST_DIR}/${THIS_INPUT_LIST}

### JOBNAME
JOBNAME_TMP=${THIS_INPUT_LIST%.txt}
JOBNAME=${JOBNAME_TMP#list_}

### LOGs
if [ ! -d ${THIS_PROJ_DIR}/logs ]; then
    mkdir ${THIS_PROJ_DIR}/logs
fi

### SCRIPT TO EXECUTE
WORKER_SCRIPT=${THIS_PROJ_DIR}/sh/sub_conversion.sh


if [[ "$DRY_RUN" -eq 0 ]]; then
### SUBMISSION TO SLURM
echo "sbatch \
--job-name=${JOBNAME} \
--output=logs/conv_${JOBNAME}_%j.log \
--export=ALL,\
INPUT_LIST=$MY_INPUT_LIST \
FOLDER=$FOLDER \
         ${WORKER_SCRIPT}"

sbatch \
--job-name=${JOBNAME} \
--output=logs/conv_${JOBNAME}_%j.log \
--mail-user=mchadolias@km3net.de \
--export=ALL,\
INPUT_LIST=$MY_INPUT_LIST,\
FOLDER=$FOLDER,\
         ${WORKER_SCRIPT}


else
### FRONTEND EXECUTION

export INPUT_LIST=$MY_INPUT_LIST
export FOLDER=$FOLDER 

${WORKER_SCRIPT}


fi
