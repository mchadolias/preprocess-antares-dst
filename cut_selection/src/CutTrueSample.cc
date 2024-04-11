#include <TFile.h>
#include <TTree.h>

#include <iostream>
#include <string>

using namespace std;

// Open the tree
TTree *OpenTree(string file, string input, string tree_name, string option)
{
    TFile *file = TFile::Open(input.c_str(), option.c_str());
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
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <input> <output>" << endl;
        return 1;
    }

    string input_file = argv[1];
    string output_file = argv[2];

    TTree *input_tree = OpenTree(input_file, input_file, "sel", "READ");

    double energy_true, cos_zenith_true;
    input_tree->SetBranchAddress("energy_true", &energy_true);
    input_tree->SetBranchAddress("cos_zenith_true", &cos_zenith_true);

    // Create a new file
    TFile *output = TFile::Open(output_file.c_str(), "RECREATE");
    TTree *output_tree = input_tree->CloneTree(0);

    // Get the number of events
    int ntot = input_tree->GetEntries();
    int nsel = 0;

    // Loop over the events
    for (int i = 0; i < ntot; i++)
    {
        input_tree->GetEntry(i);

        // Apply the cuts
        if (energy_true < 1e2 && energy_true > 1e1 && cos_zenith_true < 0)
        {
            nsel++;
            output_tree->Fill();
        }

        // Print the progress
        if (i % (ntot / 20) == 0)
            cout << "Processed " << i << " events out of " << ntot << endl;
    }

    cout << "\nSummary:" << endl
         << "Processed " << ntot << " events out of " << ntot << endl
         << "Selected " << nsel << " events out of " << ntot << endl;

    // Write the tree
    output_tree->Write();
    output->Close();
}