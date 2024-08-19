#!/bin/sh
#

### SLURM

#SBATCH --ntasks=1                    # Run a single task (by default tasks == CPU)
#SBATCH --mem=3G                      # GB
#SBATCH --time=00-04:30:00               #
#SBATCH --mail-user=mchadolias@km3net.de   # Where to send mail
#SBATCH --mail-type=FAIL,TIME_LIMIT              # Mail events (NONE, BEGIN, END, FAIL, ALL)

cd $WORK/master_thesis/antares_dst/corrections
DIRFILES=/home/wecapstor3/capn/mppi133h/ANTARES/mc
TREE_NAME="sel"
IS_WEIGHTED=0

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
	 	echo -e "\nStarting analyze script with input file: ${rootfiles}"
	    INFILE=${DIRFILES}/merged/extracted/${rootfiles}
		OUTFILES=${DIRFILES}/merged/corrected/${rootfiles%.root}_cor.root
		echo -e "Output file: ${OUTFILES} \n"
		./bin/CorrectTree ${INFILE} ${TREE_NAME} ${OUTFILES} ${IS_WEIGHTED}
done
