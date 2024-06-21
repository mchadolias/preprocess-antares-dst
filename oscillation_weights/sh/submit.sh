#!/bin/sh
### usage:
###       ./submit.sh <dry run flag> <file_dir> <filelist> <dir_outfile> <CLUSTER_selection>
###
###       e.g. ./submit.sh 1 list_anutau_b_NC_end_2.txt swim/CLUSTER_a CLUSTER_a
#
### set this to 1 for a DRY RUN, i.e. without submission to SLURM
DRY_RUN=$1
THIS_INPUT_LIST=$2
FOLDER=$3
CLUSTER=$4

echo -e "\n--------------------"
echo "Starting script:" $(basename $BASH_SOURCE)

### PROJECT DIR for logs and the worker script
if [ -z "$THIS_INPUT_PROJ_DIR" ]; then
    THIS_PROJ_DIR=/home/saturn/capn/$USER/master_thesis/antares_dst/oscillation_weights
else
    THIS_PROJ_DIR=$THIS_INPUT_PROJ_DIR
fi

### SET INPUT FILELIST
INPUT_LIST_DIR=${THIS_PROJ_DIR}/lists
MY_INPUT_LIST=${INPUT_LIST_DIR}/${THIS_INPUT_LIST}

### JOBNAME
JOBNAME_TMP=${THIS_INPUT_LIST%.txt}
JOBNAME=${JOBNAME_TMP#list_}

### LOGs
if [ ! -d ${THIS_PROJ_DIR}/logs ]; then
    mkdir ${THIS_PROJ_DIR}/logs
fi

WORKER_SCRIPT=${THIS_PROJ_DIR}/sh/job.sh


if [[ "$DRY_RUN" -eq 0 ]]; then
### SUBMISSION TO SLURM
echo "sbatch \
--job-name=${JOBNAME} \
--output=logs/conv_${JOBNAME}_%j.log \
--export=ALL,\
INPUT_LIST=$MY_INPUT_LIST \
         ${WORKER_SCRIPT}"

sbatch \
--job-name=${JOBNAME} \
--output=logs/conv_${JOBNAME}_%j.log \
--mail-user=mchadolias@km3net.de \
--export=ALL,\
FOLDER=$FOLDER,\
CLUSTER=$CLUSTER,\
INPUT_LIST=$MY_INPUT_LIST, \
         ${WORKER_SCRIPT}

elif [[ "$DRY_RUN" -eq 1 ]]; then
### FRONTEND EXECLUSTERION
    echo -e "FRONTEND EXECLUSTERION \n ----------------------------------"

    source $HOME/bash_init/root_env.sh
    source $HOME/bash_init/oscprob_setenv.sh

    make clean && make

    export INPUT_LIST=$MY_INPUT_LIST CLUSTER=$CLUSTER FOLDER=$FOLDER 
    ${WORKER_SCRIPT}
else
    echo -e "Test run with following parameters: \n \
            WORKER_SCRIPT:${WORKER_SCRIPT} \n \
            INPUT_LIST:${MY_INPUT_LIST} \n \
            FOLDER:${FOLDER} \n \
            CLUSTER:${CLUSTER} \n"
fi
