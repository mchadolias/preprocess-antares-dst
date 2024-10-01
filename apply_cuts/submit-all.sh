#!/bin/sh
FILEDIR=$WORK/master_thesis/antares_dst/apply_cuts/lists/
CUT=$1

if [ -z "$CUT" ]; then
    echo "No cut specified. Exiting..."
    exit 1
fi

echo "-------------------------"
echo "Starting script: $(basename $BASH_SOURCE)"

arr=$(find ${FILEDIR} -maxdepth 1 -type f -name "*part*")

for file in ${arr[@]};
do  
    echo "---------------------"
    echo $file
    echo submit.sh 0 $(basename $file) $CUT
    ./submit.sh 0 $(basename $file) $CUT
done
