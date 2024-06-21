#!/bin/sh
#

### SLURM

#SBATCH --ntasks=1                    # Run a single task (by default tasks == CPU)
#SBATCH --mem=60G                      # GB
#SBATCH --time=01-00:00:00               #
#SBATCH --mail-user=mchadolias@km3net.de   # Where to send mail
#SBATCH --mail-type=FAIL,TIME_LIMIT              # Mail events (NONE, BEGIN, END, FAIL, ALL)

cd /home/saturn/capn/mppi133h/master_thesis/antares_dst/oscillation_weights/
source $HOME/bash_init/root_env.sh
source $HOME/bash_init/oscprob_setenv.sh

make clean && make

cd ./lists/
DIRFILES=/home/wecapstor3/capn/mppi133h/ANTARES/mc

echo "-----------------------------"
echo "Starting script:" $(basename $BASH_SOURCE)
echo "----------------------------- 0"
echo ${INPUT_LIST}

### fill array with filenames
readarray -t myarr < ${INPUT_LIST%.txt}.txt

echo "----------------------------- 1"
echo ${myarr[@]}

### loop over array, run executable on file
for rootfiles in ${myarr[@]};
	 do
	 	echo "Starting analyze script with input file: ${rootfiles}"
	    INFILE=${DIRFILES}/cut_selection/${FOLDER}/${rootfiles}
		OUTFILES=${DIRFILES}/cut_selection/${FOLDER}/${rootfiles%.root}_w.root
		echo -e "Output file: ${OUTFILES} \n"
		/home/saturn/capn/mppi133h/master_thesis/antares_dst/oscillation_weights/bin/OscillationWeights ${INFILE} ${OUTFILES} ${CLUSTER}
done

