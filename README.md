This folder contains the first part of the work carried out during my Master' thesis. This is the pre-processing pipeline converting AntDST files from the ANTARES Collaboration into ROOT files ready to be imported in Swim. Every folder contains a step of this pipeline. The converted files are not stored in this folder, but outside a specified location.

Explanation of the work carried out in the folders is presented here for each one alphabeticaly.

## add_NNFit
This is the 4th step of the pipeline. This folder merges the NNFit output H5 files for better management and includes new branches to the processed dataset introducing NNFit information to the sample. 

## add_SWIM_Branches
This is the 6th step of the pipeline. In this folder, some minor modifications are applied to the dataset necessary to be converted to the format needs of Swim to some branches. 

## apply_cuts
This is the 5th step of the pipeline. In this folder, some cut selection criteria are applied to the events specific to my analysis.

## corrections
This is the 2nd step of the processing pipeline, during which some corrections are applied to the dataset due to the implementation of the previous step.

## extract_dst
This is the 1st part of the pipeline, where the AntDSTs are converted into new copies into a ROOT format based on standard TTree structure.

## merge
In this step, a merging startegy is included that can be applied in multiple steps.

## oscillation_weights
This is the 3rd step of the processing pipeline. In this step the oscillated atmospheric weights are calculated for neutrino events.

## libraries
This folder contains general Python-based functions used throught the pipeline.

standard_template.mk: A template Makefile used to compile C++ sourcecode in multiple steps of the pipeline. 