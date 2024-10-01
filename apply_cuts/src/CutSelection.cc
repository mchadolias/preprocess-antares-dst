#include <TFile.h>
#include <TTree.h>

#include <iostream>
#include <string>
#include <cmath>    

using namespace std;

// Global variables
TFile *file;

bool ApplyCuts(string cut_selection, double energy_true, double nnfit_track_cos_zenith, double nnfit_shower_cos_zenith, double nnfit_track_sigma_z_closest, double nnfit_track_sigma_r_closest, double nnfit_shower_sigma_z_vertex, double nnfit_shower_sigma_r_vertex, string topology)
{
    bool selected = false;

    if (cut_selection == "muon_free"){
        if (energy_true < 1e2 && energy_true > 1e1)
            selected = true;
    }
    else if (cut_selection == "nnfit_loose_cuts"){
        if ((topology == "shower" && nnfit_shower_cos_zenith < 0) || (topology == "track" && nnfit_track_cos_zenith < 0))
            selected = true;
    }
    else if (cut_selection == "nnfit_hard_cuts"){
        if ((topology == "shower" && nnfit_shower_cos_zenith < 0 && nnfit_shower_sigma_z_vertex < 15 && nnfit_shower_sigma_r_vertex < 15) || (topology == "track" && nnfit_track_cos_zenith < 0 && nnfit_track_sigma_z_closest < 10 && nnfit_track_sigma_r_closest < 7))
            selected = true;
    }
    else
    {
        cerr << "Error: cut selection not found" << endl;
        exit(1);
    }

    return selected;
}

string GetTopology(double type, double interaction_type)
{
    string topology;

    if ( (abs(type) == 14 && interaction_type ==  1) || (abs(type) == 16 && interaction_type ==  2) || (abs(type) == 13) )
    {
        topology = "track";
    }
    else if ( (abs(type) == 12) || (abs(type) == 14 && interaction_type ==  0) || (abs(type) == 16 && interaction_type !=  2) )
    {
        topology = "shower";
    }
    else
    {
        cerr << "Error: topology not found" << endl;
        exit(1);
    }
    return topology;
}    

// Open the tree
TTree *OpenTree(string input, string tree_name, string option)
{
    file = TFile::Open(input.c_str(), option.c_str());
    if (!file || file->IsZombie())
    {
        std::cerr << "Error: file " << input << " not found" << std::endl;
        exit(1);
    }

    // TTree
    TTree *input_tree = dynamic_cast<TTree *>(file->Get(tree_name.c_str()));
    if (!input_tree)
    {
        std::cerr << "Error: tree not found" << std::endl;
        exit(1);
    }

    return input_tree;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cerr << "Usage: " << argv[0] << " <input> <output> <cut_selection>" << endl;
        return 1;
    }

    string input_file = argv[1];
    string output_file = argv[2];
    string cut_selection = argv[3];

    // Open the tree
    TTree *input_tree = OpenTree(input_file, "sel", "READ");

    int type, interaction_type;
    double energy_true, cos_zenith_true;
    double nnfit_track_cos_zenith, nnfit_shower_cos_zenith;
    float nnfit_track_sigma_theta, nnfit_track_sigma_r_closest, nnfit_track_sigma_z_closest;
    float nnfit_shower_sigma_theta, nnfit_shower_sigma_r_vertex, nnfit_shower_sigma_z_vertex;

    input_tree->SetBranchAddress("energy_true", &energy_true);
    input_tree->SetBranchAddress("cos_zenith_true", &cos_zenith_true);
    input_tree->SetBranchAddress("type", &type);
    input_tree->SetBranchAddress("interaction_type", &interaction_type);
    input_tree->SetBranchAddress("NNFitTrack_cos_zenith", &nnfit_track_cos_zenith);
    input_tree->SetBranchAddress("NNFitShower_cos_zenith", &nnfit_shower_cos_zenith);
    input_tree->SetBranchAddress("NNFitTrack_SigmaTheta", &nnfit_track_sigma_theta);
    input_tree->SetBranchAddress("NNFitShower_SigmaTheta", &nnfit_shower_sigma_theta);
    input_tree->SetBranchAddress("NNFitTrack_SigmaRClosest", &nnfit_track_sigma_r_closest);
    input_tree->SetBranchAddress("NNFitTrack_SigmaZClosest", &nnfit_track_sigma_z_closest);
    input_tree->SetBranchAddress("NNFitShower_SigmaRVertex", &nnfit_shower_sigma_r_vertex);
    input_tree->SetBranchAddress("NNFitShower_SigmaZVertex", &nnfit_shower_sigma_z_vertex);

    // Create a new file
    TFile *output = TFile::Open(output_file.c_str(), "RECREATE");
    TTree *output_tree = input_tree->CloneTree(0);

    // Get the number of events
    int ntot = input_tree->GetEntries();
    int nsel = 0;

    // Define 
    string topology;

    // Loop over the events
    for (int i = 0; i < ntot; i++)
    {
        input_tree->GetEntry(i);
    

        // Get the topology
        topology = GetTopology(type, interaction_type);

        // Apply the cuts
        if (ApplyCuts(cut_selection, energy_true, nnfit_track_cos_zenith, nnfit_shower_cos_zenith, nnfit_track_sigma_z_closest, nnfit_track_sigma_r_closest, nnfit_shower_sigma_z_vertex, nnfit_shower_sigma_r_vertex, topology))
        {
            nsel++;
            output_tree->Fill();
        }

        // Print the progress
        if (i % (ntot / 50) == 0)
            cout << "Processed " << i << " events out of " << ntot << endl;
    }

    cout << "\nSummary:" << endl
         << "Processed " << ntot << " events out of " << ntot << endl
         << "Selected " << nsel << " events out of " << ntot << endl;

    // Write the tree
    file->Close();
    output_tree->Write();
    output->Close();
}