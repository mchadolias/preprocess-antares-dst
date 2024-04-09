#!/bin/bash

# executed in the container
source /home/master_thesis/antares_dst/extract_dst/sh/init
cd /home/master_thesis/antares_dst/extract_dst/

# plain test
TEST_INPUT=/mnt/MC_066730_mupage_reco.root
TEST_OUTPUT=out_part4.root

echo $TEST_INPUT
echo $TEST_OUTPUT

bin/ExtractAntDSTInfo ${TEST_OUTPUT} ${TEST_INPUT}

# in case the env variables are given from outside 

echo $TEST_INPUT_ENV
echo $TEST_OUTPUT_ENV

bin/ExtractAntDSTInfo $TEST_OUTPUT_ENV $TEST_INPUT_ENV
