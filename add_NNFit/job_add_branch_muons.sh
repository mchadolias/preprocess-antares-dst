#!/bin/bash
# 
# Usage:
#       ./job_check_num_files.sh <root_file> <path_to_file>
#
# Description:
#       This script is used to submit a job to the cluster. It is used to run the merged_nnfit.py script
#       for a given list of files. The list of files is given as an argument to the script.


### SLURM

#SBATCH --ntasks=1                    # Run a single task (by default tasks == CPU)
#SBATCH --mem=180G                      # GB
#SBATCH --time=00-02:30:00               #
#SBATCH --mail-user=mchadolias@km3net.de   # Where to send mail
#SBATCH --mail-type=FAIL,TIME_LIMIT              # Mail events (NONE, BEGIN, END, FAIL, ALL)

echo "Running add_column_branch.py"
echo "ROOT_FILE: ${ROOT_FILE}"
echo "H5_FILE: ${H5_FILE}"
echo "TASK: ${TASK}"

WORKDIR="$WORK/master_thesis/antares_dst/add_NNFit"

cd ${WORKDIR}

if [ DRY_RUN -eq 0 ]; then
    module load python/3.7
fi

for i in {1..29};
do
    ROOT_FILE="Antares_mupage_${i}_cor_weighted.root"
    if [[ i -lt 10 ]]; then
        H5_FILE="Mupage_000${i}"
    else
        H5_FILE="Mupage_00${i}"
    fi

    COMMAND="python3 scripts/add_new_branches.py --r ${ROOT_FILE} --h5 ${H5_FILE} --task ${TASK} --particle muons"

    echo -e "\n Executing command: "
    echo -e " ========================================================="
    echo " ${COMMAND}"
    echo -e " =========================================================\n"
    eval "${COMMAND}"

    if [[ $? == 0 ]]; then
        echo " Code executed successfully"
    else
        echo " Unexpected error! See error log!"
    fi

    sleep 30
done

echo -e "\n DONE! \n"