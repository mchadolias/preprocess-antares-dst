#!/bin/bash
#
# Script to create a list of files with a specific ending number
#
# Usage: ./quick_lists.sh DIR
#
# Arguments:
# DIR: Directory with the files to be listed

DIR=${1}

for ENDING_NUMBER in {0..9}
do 
    find ${DIR} -maxdepth 1 -type f -name "*end_${ENDING_NUMBER}.root"  -printf '%f\n' > ending_run_${ENDING_NUMBER}.txt

done

echo "Done"