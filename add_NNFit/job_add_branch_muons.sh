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
#SBATCH --mem=200G                      # GB
#SBATCH --time=01-00:00:00               #
#SBATCH --mail-user=mchadolias@km3net.de   # Where to send mail
#SBATCH --mail-type=FAIL,TIME_LIMIT              # Mail events (NONE, BEGIN, END, FAIL, ALL)

echo "Running add_column_test.py"
echo "ROOT_FILE: ${ROOT_FILE}"
echo "H5_FILE: ${H5_FILE}"
echo "TASK: ${TASK}"

# Define the name of the Anaconda environment to activate
ENVNAME="km3net-bootcamp"

# Define the path to the Anaconda installation's 'bin' directory2
ANACONDAPATH="/home/saturn/capn/mppi133h/software/private/conda/envs/km3net-bootcamp/bin"
WORKDIR="/home/saturn/capn/mppi133h/master_thesis/antares_dst/add_NNFit"

# Add the Anaconda installation's 'bin' directory to the PATH variable
export PATH="$ANACONDAPATH:$PATH"

# Activate the conda environment
which python

cd ${WORKDIR}

for i in $(seq 0 9)
do
    ROOT_FILE="mc_mupage_end_${i}_cor_weighted.root"
    COMMAND="python3 scripts/add_new_branches.py --r ${ROOT_FILE} --h5 ${H5_FILE} --task ${TASK} --particle muons"

    echo " Executing command: "
    echo -e "\n ========================================================="
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