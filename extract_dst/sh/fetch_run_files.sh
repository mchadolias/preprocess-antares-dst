#!/bin/bash

# Path: manage_filelist/fetch_run_files.sh  CLUSTER_DIR FOLDER_DI
# Specify which cluster to use
# 1. wecapstor3
# 2. sps-km3net

# Usage: ./fetch_run_files.sh /home/wecapstor3/capn/mppi133h/ANTARES/ ext   # for ECAP
# Usage: ./fetch_run_files.sh /sps/km3net/mchadoli/ANTARES/ ext   # for CC-IN2P3

echo "-------------------------"
echo "Starting script: $(basename $BASH_SOURCE)"
ecjo "Cluster directory: ${1}"
echo "Folder directory: ${2}"

# This script is used to fetch the mc production files from the cluster to the local machine
CLUSTER_DIR=${1} # Get the directory where the files are located in the cluster (e.g. /home/wecapstor3/capn/mppi133h/ANTARES for ECAP and /sps/km3net/mchadoli/ANTARES for CC-IN2P3
FOLDER_DIR=${2} # Get the subfolder when the root files are located
REPDIR=${CLUSTER_DIR}/ANTARES/mc/${FOLDER_DIR}
DIR=$(pwd)

cd $REPDIR

printf "\nSelect run id to fetch mc production files.\n"

# Get the run id as an input
read -p "Enter the document ID: " run_id

files_to_transfer=$(find . -maxdepth 1 -type f -name "MC_0${run_id}*")

# Check if any files were found
if [ -z "$files_to_transfer" ]; then
    echo "No documents found with ID $run_id in the source directory."
    exit 1
fi

# Create folder to keep transfered files
if [ ! -d "$DIR/events" ]; then
    mkdir -p "$DIR/events"
fi

echo -e "\n ------- Transfering files -------"
echo "Files to transfer:"
echo ${run_id}
echo $files_to_transfer

# Transfer files to the destination directory
for file in $files_to_transfer; do
    cp "$file" "$DIR/events/$file"  # Copy files
    echo "Transferred: $file"
done


cd $DIR
printf "\n======= Transfer complete ========\n"
