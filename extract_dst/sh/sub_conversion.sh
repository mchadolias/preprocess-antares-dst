#!/bin/sh
### this script is submitted to SLURM
### it runs on a worker node
### it calls the singularity container
### and executes a script in the container

### SLURM

#SBATCH --ntasks=1                    # Run a single task (by default tasks == CPU)
#SBATCH --mem=3G                      # GB
#SBATCH --time=01-00:00:00               #
#SBATCH --mail-user=mchadolias@km3net.de   # Where to send mail
#SBATCH --mail-type=FAIL,TIME_LIMIT           # Mail events (NONE, BEGIN, END, FAIL, ALL)

### input is a filelist file, about 1700 lines
### filename is passed as an env variable to the container
### script running in the container can access this variable

echo "0 ---------------------------------------------------------F--------------"
echo "  this is the filelist for this process:"
echo ${INPUT_LIST}
echo ${FOLDER}


### start conversion in container
### parameters are passed bv env variables
echo "1 -----------------------------------------------------------------------"


## SINGULARITY CONTAINER
#CONTAINER=/home/saturn/capn/mppi104h/singularity/antaresSeatray_18-07-00_flex.sif
CONTAINER=/home/saturn/capn/mppi104h/singularity/antares_seatray_18-07-00_km3pipe_8.23.4_lp_2.1.2_bash.simg

## SCRIPT to be run inside the container
CONTAINER_SCRIPT=/home/master_thesis/antares_dst/extract_dst/sh/conversion.sh

echo "... entering singularity container @" $(date)
echo "... executing script: " ${CONTAINER_SCRIPT}

## input will be in /mc or /data depending on the type of antdst used
## output will go to /sps

echo "singularity exec \
    --env INLIST=${INPUT_LIST} \
    --env FOLDER=${FOLDER} \
    --bind /home/saturn/capn/${USER}:/home \
    --bind /home/wecapstor3/capn/mppi19/ANTARES/MC/v4.2/antdst:/mc \
    --bind /home/wecapstor3/capn/mppi19/ANTARES/data/antdst:/data \
    --bind /home/wecapstor3/capn/mppi133h/ANTARES/:/sps \
    ${CONTAINER} \
    bash --noprofile -c '${CONTAINER_SCRIPT}'"

## container is called and script is run via a bash inside the container
singularity exec \
    --env INLIST=${INPUT_LIST} \
    --env FOLDER=${FOLDER} \
    --bind /home/saturn/capn/${USER}:/home \
    --bind /home/wecapstor3/capn/mppi19/ANTARES/MC/v4.2/antdst:/mc \
    --bind /home/wecapstor3/capn/mppi19/ANTARES/data/antdst:/data \
    --bind /home/wecapstor3/capn/${USER}/ANTARES/mc:/sps \
    ${CONTAINER} \
    bash --noprofile -c ${CONTAINER_SCRIPT}


