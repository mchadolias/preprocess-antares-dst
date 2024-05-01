#!/bin/sh
#

### SLURM

#SBATCH --ntasks=1                    # Run a single task (by default tasks == CPU)
#SBATCH --mem=1G                      # GB
#SBATCH --time=00-01:00:00               #
#SBATCH --mail-user=mchadolias@km3net.de   # Where to send mail
#SBATCH --mail-type=FAIL,TIME_LIMIT              # Mail events (NONE, BEGIN, END, FAIL, ALL)

cd ${ANTARES}/mc/${FILE_FOLDER}/

# Load ROOT 
source $HOME/bash_init/root_env.sh

echo "-----------------------------"
echo "Starting script:" $(basename $BASH_SOURCE)
echo "----------------------------- 0"
echo "Input list: ${INPUT_LIST}"
echo "Input folder: ${FILE_FOLDER}"
echo "Output folder: ${END_FOLDER}"

hadd -f ${INPUT_LIST%.txt}.root @${INPUT_LIST}
mv ${INPUT_LIST%.txt}.root ${ANTARES}/mc/${END_FOLDER}/