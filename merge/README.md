job_merge_list.sh: This script imports a text file containg the filenames of the files to be merged. It parses this information to _job_merge.sh_
job_merge.sh: This script merges files into a new one based on the imported list
submit-all.sh: A script to parse all necessary information to the submitter script
submit.sh: This is a submitter script to SLURM