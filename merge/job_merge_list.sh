#!/bin/sh
#

### SLURM

#SBATCH --ntasks=1                    # Run a single task (by default tasks == CPU)
#SBATCH --mem=3G                      # GB
#SBATCH --time=01-00:00:00               #
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
echo -e "Output folder: ${END_FOLDER} \n"

# Read each line in the list and merge the files
while IFS= read -r line
do
    FILE=${WORK}/master_thesis/antares_dst/merge/lists/files/${line}
    
    echo "Merging file: ${FILE}"
    hadd -f ${FILE%.txt}.root @${FILE}
    mv ${FILE%.txt}.root ${ANTARES}/mc/${END_FOLDER}/

    echo -e "Done merging file: ${FILE} \n"
    sleep 30
done < ${INPUT_LIST}

