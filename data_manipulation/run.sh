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


cd ${WORK}/master_thesis/antares_dst/data_manipulation

INPUT_FILE=$1
INPUT_TREE=$2
OUTPUT_FILE=$3

bin/RemoveDuplicateEvents $INPUT_FILE $INPUT_TREE $OUTPUT_FILE
