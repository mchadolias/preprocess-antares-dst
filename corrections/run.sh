#!/bin/bash
#
# Script to run the RemoveDuplicateEvents program
#
# Usage: ./run.sh INPUT_FILE INPUT_TREE OUTPUT_FILE
#
# Arguments:
# INPUT_FILE: Input file with the dataset   
# INPUT_TREE: Name of the tree in the input file
# OUTPUT_FILE: Output file with the dataset without duplicated events
# IS_WEIGHTED: Flag to indicate if the input file is weighted (1) or not (0)

cd ${WORK}/master_thesis/antares_dst/corrections

INPUT_FILE=$1
INPUT_TREE=$2
OUTPUT_FILE=$3
IS_WEIGHTED=$4

echo "Running CorrectTree program..."
echo "INPUT_FILE: ${INPUT_FILE}"
echo "INPUT_TREE: ${INPUT_TREE}"
echo "OUTPUT_FILE: ${OUTPUT_FILE}"
echo "IS_WEIGHTED: ${IS_WEIGHTED}"

bin/CorrectTree $INPUT_FILE $INPUT_TREE $OUTPUT_FILE $IS_WEIGHTED
