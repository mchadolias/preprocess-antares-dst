#!/bin/sh
cd /home/saturn/capn/mppi133h/master_thesis/oscillation_weights

TEST_INPUT=/home/saturn/capn/mppi133h/master_thesis/antares_dst/data/$1
TEST_OUTPUT=/home/saturn/capn/mppi133h/master_thesis/antares_dst/data/output/test_2.root
CLUSTER=$2

bin/OscillationWeights $TEST_INPUT $TEST_OUTPUT $CLUSTER
