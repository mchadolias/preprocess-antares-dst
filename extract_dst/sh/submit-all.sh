#!/bin/sh
FOLDER=${1} #applicable for both data or mc
FILEDIR=/home/saturn/capn/mppi133h/master_thesis/antares_dst/extract_dst/lists/mc/

echo "-------------------------"
echo "Starting script: $(basename $BASH_SOURCE)"

arr=$(find ${FILEDIR} -maxdepth 1 -type f -name "*mupage*")

for file in ${arr[@]};
do  
    echo "---------------------"
    echo $file
    echo sh/submit.sh 0 ${FOLDER} $(basename $file)
    sh/submit.sh 0 ${FOLDER} $(basename $file)
done
