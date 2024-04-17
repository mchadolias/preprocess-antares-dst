#!/bin/sh
#

### SLURM

#SBATCH --ntasks=1                    # Run a single task (by default tasks == CPU)
#SBATCH --mem=1G                      # GB
#SBATCH --time=01-00:00:00               #
#SBATCH --mail-user=mchadolias@km3net.de   # Where to send mail
#SBATCH --mail-type=FAIL,TIME_LIMIT              # Mail events (NONE, BEGIN, END, FAIL, ALL)

cd /home/saturn/capn/mppi133h/master_thesis/antares_dst/cut_selection
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
	 	echo -e "\nStarting analyze script with input file: ${rootfiles}"
	    INFILE=${DIRFILES}/extracted_cor/${rootfiles}
		OUTFILES=${DIRFILES}/extracted_merged/low_energy/${rootfiles%.root}_sample.root
		echo -e "Output file: ${OUTFILES} \n"
		/home/saturn/capn/mppi133h/master_thesis/antares_dst/cut_selection/bin/CutTrueSample ${INFILE} ${OUTFILES} 
done
