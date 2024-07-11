#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TObject.h>
#include <cmath>
#include <string>
#include "addBranches.h"

using namespace std;

void addBranches(string old_root_file, string new_root_file)
{
    cout << "Starting the program" << endl;

    // Open the ROOT file
    cout << "Opening file: " << old_root_file << endl;

    TFile *oldfile = new TFile(old_root_file.c_str(), "READ");

    if (oldfile->IsZombie())
    {
        cout << "Error opening file" << endl;
        return;
    }

    // Get the TTree object
    cout << "Getting TTree object" << endl;
    TTree *oldtree = (TTree *)oldfile->Get("sel");

    if (oldtree == NULL)
    {
        cout << "Error opening tree" << endl;
        return;
    }

    // Define variables
    Float_t nnfit_shower_theta, nnfit_track_theta, nnfit_shower_logE, nnfit_track_logE;
    Double_t energy_true, cos_zenith_true, energy_recoTrue, cos_zenith_recoTrue, bjorken_y_recoTrue;20
    Double_t nnfit_shower_cos_zenith, nnfit_track_cos_zenith, nnfit_shower_energy, nnfit_track_energy, nnfit_bjorken_y;

    // Create a new branch
    cout << "Creating new branch" << endl;

    // Create the new branches
    oldtree->SetBranchAddress("NNFitShower_Theta", &nnfit_shower_theta);
    oldtree->SetBranchAddress("NNFitTrack_Theta", &nnfit_track_theta);
    oldtree->SetBranchAddress("NNFitShower_Log10Energy", &nnfit_shower_logE);
    oldtree->SetBranchAddress("NNFitTrack_Log10Energy", &nnfit_track_logE);
    oldtree->SetBranchAddress("energy_true", &energy_true);
    oldtree->SetBranchAddress("cos_zenith_true", &cos_zenith_true);

    TFile *newfile = new TFile(new_root_file.c_str(), "RECREATE");
    TTree *newtree = oldtree->CloneTree(0);

    newtree->Branch("NNFitShower_Energy", &nnfit_shower_energy, "NNFitShower_Energy/D");
    newtree->Branch("NNFitTrack_Energy", &nnfit_track_energy, "NNFitTrack_Energy/D");
    newtree->Branch("NNFitShower_cos_zenith", &nnfit_shower_cos_zenith, "NNFitShower_CosZenith/D");
    newtree->Branch("NNFitTrack_cos_zenith", &nnfit_track_cos_zenith, "NNFitTrack_CosZenith/D");
    newtree->Branch("NNFit_Bjorken_y", &nnfit_bjorken_y, "NNFit_Bjorken_y/D");
    newtree->Branch("energy_recoTrue", &energy_recoTrue, "energy_recoTrue/D");
    newtree->Branch("cos_zenith_recoTrue", &cos_zenith_recoTrue, "cos_zenith_recoTrue/D");
    newtree->Branch("bjorken_y_recoTrue", &bjorken_y_recoTrue, "bjorken_y_recoTrue/D");

    double pi = 3.14159265359;

    cout << "Setting new branches" << "\nStarting loop over the entries of the tree" << endl;

    Int_t numEntries = oldtree->GetEntries();
    // Set the new branches
    for (int i = 0; i < numEntries; i++)
    {
        oldtree->GetEntry(i);

        // Set the new branches
        energy_recoTrue = energy_true;
        cos_zenith_recoTrue = cos_zenith_true;
        bjorken_y_recoTrue = 0.5;
        nnfit_bjorken_y = 0.5;
        nnfit_shower_energy = pow(10, nnfit_shower_logE);
        nnfit_track_energy = pow(10, nnfit_track_logE);
        nnfit_shower_cos_zenith = - cos(nnfit_shower_theta * pi / 180);
        nnfit_track_cos_zenith = - cos(nnfit_track_theta * pi / 180);
        
        newtree->Fill();
        
        // Print the progress
        if(i % (numEntries/100) == 0){
            cout << "Progress: " << i << " / " << numEntries  << endl;
        }
    }

    // Write the new branches
    cout << "Writing new branches" << endl;
    newfile->cd();
    newfile->Write();
    newfile->Close();
}
