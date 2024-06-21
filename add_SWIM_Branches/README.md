# add_SWIM_branches

The purpose of this folder, is to add some new branches on the ROOT file in order to accomplish the required task in SWIM. The actions taken in this part of the pipeline are two, first the addition of a 3D vector necessary for the building of the ResponceMatrix during the first iteration in SWIM. Secondly, is the addition of some branches to be easily accesible in SWIM. The required branches are the energy, the cosine of the zenith direction and the bjorken y. Here, these branches are added for the NNFit reconstruction and for the Monte Carlo true information. The later is required if someone wants to use MC information in SWIM, since there seems to be a conflict, when accessing the MC information during the building of the ResponceMatrix for true and the reconstructed information. Thus duplicate branches are created.

## ATTENTION  

For this process it is prerequisite that also NNFit branches exist. In case only the MC information is required some slight modifications are required.
