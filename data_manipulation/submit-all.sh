#!/bin/sh
FILEDIR=/home/saturn/capn/mppi133h/master_thesis/antares_dst/data_manipulation/lists/

echo "-------------------------"
echo "Starting script: $(basename $BASH_SOURCE)"

arr=$(find ${FILEDIR} -maxdepth 1 -type f -name "*list*")

for file in ${arr[@]};
do  
    echo "---------------------"
    echo $file
    echo submit.sh 0 ${FOLDER} $(basename $file)
    ./submit.sh 0 $(basename $file)
done
