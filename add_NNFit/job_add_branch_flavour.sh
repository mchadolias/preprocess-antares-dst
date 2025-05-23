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
#SBATCH --mem=140G                      # GB
#SBATCH --time=01-00:00:00               #
#SBATCH --mail-user=mchadolias@km3net.de   # Where to send mail
#SBATCH --mail-type=FAIL,TIME_LIMIT              # Mail events (NONE, BEGIN, END, FAIL, ALL)

echo "Running add_column_branch.py"
echo "ROOT_FILE: ${ROOT_FILE}"
echo "H5_FILE: ${H5_FILE}"
echo "TASK: ${TASK}"

# Define the name of the Anaconda environment to activate
ENVNAME="master_thesis"

# Define the path to the Anaconda installation's 'bin' directory2
ANACONDAPATH="$WORK/software/private/conda/envs/master_thesis/bin"
WORKDIR="$WORK/master_thesis/antares_dst/add_NNFit"
ROOT_FILE=("antares_nue.root" "antares_numu_a_CC.root" "antares_numu_a_NC.root" "antares_nutau.root")


# Add the Anaconda installation's 'bin' directory to the PATH variable
export PATH="$ANACONDAPATH:$PATH"

# Activate the conda environment
source activate "$ENVNAME"
which python

cd ${WORKDIR}
COMMAND="python3 scripts/add_new_branches.py --r ${ROOT_FILE} --h5 ${H5_FILE} --task ${TASK}"

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

echo -e "\n DONE! \n"