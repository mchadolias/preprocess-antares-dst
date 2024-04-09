#!/bin/sh
#
### executed in the container

## init the environment for the AntDSTs in the container
source /home/master_thesis/antares_dst/extract_dst/sh/init
cd /home/master_thesis/antares_dst/extract_dst/

echo "-----------------------------"
echo "Starting script:" $(basename $BASH_SOURCE)
echo "----------------------------- 0"
echo ${INLIST}
echo ${FOLDER}

### extract only filenames from provided filelist
awk '{print $2}' ${INLIST} > ${INLIST%.txt}_tmp.txt

### fill array with filenames
readarray -t myarr < ${INLIST%.txt}.txt
### clean up temporary file
rm -f ${INLIST%.txt}_tmp.txt

echo "----------------------------- 1"
echo ${myarr[@]}

### loop over array, run executable on file
for antdstfile in ${myarr[@]};
	 do
	 	echo "Starting analyze script with input file: ${antdstfile}"
	    INFILE=/mc/${antdstfile}
	    OUTFILE=/sps/${FOLDER}/${antdstfile%_reco.root}_ext.root
		echo -e "Output file: ${OUTFILE} \n"
		/home/master_thesis/antares_dst/extract_dst/bin/ExtractAntDSTInfo ${OUTFILE} $INFILE
done

