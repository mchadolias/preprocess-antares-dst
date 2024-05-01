#!/bin/sh
## Usage:
#   ./submit.sh <DRY_RUN> <FILE_FOLDER> <END_FOLDER> <JOB_SCRIPT> <THIS_INPUT_LIST>
#
## Example:
#   ./submit.sh 0 extracted_cor merged/merged_before_nnfit job_merge_list.sh list_anue_a_CC_end_all_runs.txt
#
## Arguments:
#   DRY_RUN: 0 for submission to SLURM, 1 for frontend execution, 2 for test run
#   FILE_FOLDER: folder where the files are located
#   END_FOLDER: folder where the merged files will be stored
#   JOB_SCRIPT: the script that will be executed
#   THIS_INPUT_LIST: the list of files that will be merged

DRY_RUN=$1
FILE_FOLDER=$2
END_FOLDER=$3
JOB_SCRIPT=$4
THIS_INPUT_LIST=$5


echo -e "\n--------------------"
echo "Starting script:" $(basename $BASH_SOURCE)

# PROJECT DIR for logs and the worker script
if [ -z "$THIS_INPUT_PROJ_DIR" ]; then
    THIS_PROJ_DIR=${WORK}/master_thesis/antares_dst/merge
else
    THIS_PROJ_DIR=$THIS_INPUT_PROJ_DIR
fi

# Check the value of JOB_SCRIPT and set the INPUT_LIST_DIR accordingly
if [[ "$JOB_SCRIPT" == "job_merge_list.sh" ]]; then
    INPUT_LIST_DIR=${WORK}/master_thesis/antares_dst/merge/lists/filename
elif [[ "$JOB_SCRIPT" == "job_merge.sh" ]]; then
    INPUT_LIST_DIR=${WORK}/master_thesis/antares_dst/merge/lists/files
fi

MY_INPUT_LIST=${INPUT_LIST_DIR}/${THIS_INPUT_LIST}

# JOBNAME
JOBNAME_TMP=${THIS_INPUT_LIST%.txt}
JOBNAME=${JOBNAME_TMP#list_}

# LOGs
if [ ! -d ${THIS_PROJ_DIR}/logs ]; then
    mkdir ${THIS_PROJ_DIR}/logs
fi

WORKER_SCRIPT=${THIS_PROJ_DIR}/${JOB_SCRIPT}


if [[ "$DRY_RUN" -eq 0 ]]; then
# SUBMISSION TO SLURM
echo "sbatch \
--job-name=${JOBNAME} \
--output=logs/conv_${JOBNAME}_%j.log \
--export=ALL,\
INPUT_LIST=$MY_INPUT_LIST \
         ${WORKER_SCRIPT}"

sbatch \
--job-name=${JOBNAME} \
--output=logs/conv_${JOBNAME}_%j.log \
--mail-user=michael.chadolias@fau.de \
--export=ALL,\
INPUT_LIST=$MY_INPUT_LIST,\
FILE_FOLDER=$FILE_FOLDER,\
END_FOLDER=$END_FOLDER \
         ${WORKER_SCRIPT}

elif [[ "$DRY_RUN" -eq 1 ]]; then
# FRONTEND EXECUTION
    echo -e "FRONTEND EXECUTION \n ----------------------------------"
    export INPUT_LIST=$MY_INPUT_LIST 
    export FILE_FOLDER=$FILE_FOLDER
    export END_FOLDER=$END_FOLDER
    ${WORKER_SCRIPT}
else
    echo -e "Test run with following parameters: \n \
            WORKER_SCRIPT:${WORKER_SCRIPT} \n \
            FILE_FOLDER:${FILE_FOLDER} \n \
            END_FOLDER:${END_FOLDER} \n \
            LIST_NAME:$(basename ${MY_INPUT_LIST}) \n \
            DIR_LIST:${MY_INPUT_LIST} \n \
            SCRIPT:${JOB_SCRIPT} \n \
            "

fi
