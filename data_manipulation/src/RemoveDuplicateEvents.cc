#include <TTree.h>
#include <TBranch.h>
#include <TFile.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Define each function
void OpenFile(TFile *&file, string input_file);
void RemoveDuplicateEvents(TFile *file, string input_tree, string output_file);

int main(int argc, char *argv[])
{

    // Check the number of parameters
    if (argc != 4)
    {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
    }

    // Define the input parameters
    string input_file = argv[1];
    string input_tree = argv[2];
    string output_file = argv[3];

    // Open the input file
    TFile *file;
    OpenFile(file, input_file);

    // Remove the duplicate events
    RemoveDuplicateEvents(file, input_tree, output_file);

    // Close the input file
    file->Close();

    cout << "\n============= End of the program =============" << endl;
}

void OpenFile(TFile *&file, string filename)
{
    file = TFile::Open(filename.c_str(), "READ");
    if (!file || file->IsZombie())
    {
        cerr << "Error: file " << filename << " not found" << endl;
        exit(1);
    }
}

void RemoveDuplicateEvents(TFile *file, string tree, string output_file)
{
    // Open the input tree
    TTree *input_tree = (TTree *)file->Get(tree.c_str());
    if (!input_tree)
    {
        cerr << "Error: tree " << input_tree << " not found" << endl;
        exit(1);
    }

    // Create the output file
    TFile *output = TFile::Open(output_file.c_str(), "RECREATE");
    if (!output || output->IsZombie())
    {
        cerr << "Error: file " << output_file << " not found" << endl;
        exit(1);
    }

    // Create the output tree
    TTree *output_tree = input_tree->CloneTree(0);

    // Define the variables of each branch
    //  Reconstruction parameters
    bool aafit_flag, bbfit_flag, gridfit_flag, bbfit_shower_flag, showerdusj_flag, showertantra_flag;

    double aafit_lambda, aafit_bjy, aafit_pos_x, aafit_pos_y, aafit_pos_z, angerrordeg_aafit, zenithdeg_aafit, cos_zenith_aafit, azimuthdeg_aafit, nusedlines_aafit, nusedhits_aafit, energy_aafit_ANN_ECAP, energy_aafit_dEdX_CEA, aafit_totalamp, aafit_zmin, aafit_zmax, aafit_tracklength, aafit_nhits;
    double bbfit_quality, bbfit_bjy, bbfit_pos_x, bbfit_pos_y, bbfit_pos_z, angerrordeg_bbfit, zenithdeg_bbfit, cos_zenith_bbfit, azimuthdeg_bbfit, nusedlines_bbfit, nusedhits_bbfit, bbfit_totalamp, bbfit_zmin, bbfit_zmax, bbfit_nhits;
    double gridfit_quality, gridfit_bjy, gridfit_pos_x, gridfit_pos_y, gridfit_pos_z, angerrordeg_gridfit, zenithdeg_gridfit, cos_zenith_gridfit, azimuthdeg_gridfit, nusedlines_gridfit, nusedhits_gridfit, gridfit_totalamp, gridfit_zmin, gridfit_zmax, gridfit_nhits;
    double bbfit_shower_quality, bbfit_shower_bjy, bbfit_shower_pos_x, bbfit_shower_pos_y, bbfit_shower_pos_z, angerrordeg_bbfit_shower, zenithdeg_bbfit_shower, cos_zenith_bbfit_shower, azimuthdeg_bbfit_shower, nusedlines_bbfit_shower, nusedhits_bbfit_shower, bbfit_shower_totalamp, bbfit_shower_zmin, bbfit_shower_zmax, bbfit_shower_nhits;
    double showerdusj_quality, showerdusj_bjy, showerdusj_pos_x, showerdusj_pos_y, showerdusj_pos_z, angerrordeg_showerdusj, zenithdeg_showerdusj, cos_zenith_showerdusj, azimuthdeg_showerdusj, nusedlines_showerdusj, nusedhits_showerdusj, showerdusj_totalamp, showerdusj_energy, showerdusj_nhits, showerdusj_radius, showerdusj_height;
    double showertantra_quality, showertantra_bjy, showertantra_pos_x, showertantra_pos_y, showertantra_pos_z, angerrordeg_showertantra, zenithdeg_showertantra, cos_zenith_showertantra, azimuthdeg_showertantra, nusedlines_showertantra, nusedhits_showertantra, showertantra_totalamp, showertantra_energy, showertantra_radius, showertantra_height, showertantra_nhits;

    // Set the branches for the modified tree
    // AAFIT
    // Reconstruction parameters
    input_tree->SetBranchAddress("aafit_lambda", &aafit_lambda);
    input_tree->SetBranchAddress("aafit_bjy", &aafit_bjy);
    input_tree->SetBranchAddress("aafit_pos_x", &aafit_pos_x);
    input_tree->SetBranchAddress("aafit_pos_y", &aafit_pos_y);
    input_tree->SetBranchAddress("aafit_pos_z", &aafit_pos_z);
    input_tree->SetBranchAddress("aafit_angerr_deg", &angerrordeg_aafit);
    input_tree->SetBranchAddress("aafit_zenith_deg", &zenithdeg_aafit);
    input_tree->SetBranchAddress("aafit_cos_zenith", &cos_zenith_aafit);
    input_tree->SetBranchAddress("aafit_azimuth_deg", &azimuthdeg_aafit);
    input_tree->SetBranchAddress("aafit_nusedlines", &nusedlines_aafit);
    input_tree->SetBranchAddress("aafit_nusedhits", &nusedhits_aafit);
    input_tree->SetBranchAddress("aafit_totalamp", &aafit_totalamp);
    input_tree->SetBranchAddress("aafit_zmin", &aafit_zmin);
    input_tree->SetBranchAddress("aafit_zmax", &aafit_zmax);
    input_tree->SetBranchAddress("energy_aafit_dEdX_CEA", &energy_aafit_dEdX_CEA);
    input_tree->SetBranchAddress("energy_aafit_ANN_ECAP", &energy_aafit_ANN_ECAP);
    input_tree->SetBranchAddress("aafit_tracklength", &aafit_tracklength);
    input_tree->SetBranchAddress("aafit_nhits", &aafit_nhits);
    input_tree->SetBranchAddress("aafit_flag", &aafit_flag);

    input_tree->SetBranchAddress("bbfit_quality", &bbfit_quality);
    input_tree->SetBranchAddress("bbfit_bjy", &bbfit_bjy);
    input_tree->SetBranchAddress("bbfit_pos_x", &bbfit_pos_x);
    input_tree->SetBranchAddress("bbfit_pos_y", &bbfit_pos_y);
    input_tree->SetBranchAddress("bbfit_pos_z", &bbfit_pos_z);
    input_tree->SetBranchAddress("bbfit_angerr_deg", &angerrordeg_bbfit);
    input_tree->SetBranchAddress("bbfit_zenith_deg", &zenithdeg_bbfit);
    input_tree->SetBranchAddress("bbfit_cos_zenith", &cos_zenith_bbfit);
    input_tree->SetBranchAddress("bbfit_azimuth_deg", &azimuthdeg_bbfit);
    input_tree->SetBranchAddress("bbfit_nusedlines", &nusedlines_bbfit);
    input_tree->SetBranchAddress("bbfit_nusedhits", &nusedhits_bbfit);
    input_tree->SetBranchAddress("bbfit_totalamp", &bbfit_totalamp);
    input_tree->SetBranchAddress("bbfit_zmin", &bbfit_zmin);
    input_tree->SetBranchAddress("bbfit_zmax", &bbfit_zmax);
    input_tree->SetBranchAddress("bbfit_nhits", &bbfit_nhits);
    input_tree->SetBranchAddress("bbfit_flag", &bbfit_flag);

    input_tree->SetBranchAddress("gridfit_quality", &gridfit_quality);
    input_tree->SetBranchAddress("gridfit_bjy", &gridfit_bjy);
    input_tree->SetBranchAddress("gridfit_pos_x", &gridfit_pos_x);
    input_tree->SetBranchAddress("gridfit_pos_y", &gridfit_pos_y);
    input_tree->SetBranchAddress("gridfit_pos_z", &gridfit_pos_z);
    input_tree->SetBranchAddress("gridfit_angerr_deg", &angerrordeg_gridfit);
    input_tree->SetBranchAddress("gridfit_zenith_deg", &zenithdeg_gridfit);
    input_tree->SetBranchAddress("gridfit_cos_zenith", &cos_zenith_gridfit);
    input_tree->SetBranchAddress("gridfit_azimuth_deg", &azimuthdeg_gridfit);
    input_tree->SetBranchAddress("gridfit_nusedlines", &nusedlines_gridfit);
    input_tree->SetBranchAddress("gridfit_nusedhits", &nusedhits_gridfit);
    input_tree->SetBranchAddress("gridfit_totalamp", &gridfit_totalamp);
    input_tree->SetBranchAddress("gridfit_zmin", &gridfit_zmin);
    input_tree->SetBranchAddress("gridfit_zmax", &gridfit_zmax);
    input_tree->SetBranchAddress("gridfit_nhits", &gridfit_nhits);
    input_tree->SetBranchAddress("gridfit_flag", &gridfit_flag);

    input_tree->SetBranchAddress("bbfit_shower_quality", &bbfit_shower_quality);
    input_tree->SetBranchAddress("bbfit_shower_bjy", &bbfit_shower_bjy);
    input_tree->SetBranchAddress("bbfit_shower_pos_x", &bbfit_shower_pos_x);
    input_tree->SetBranchAddress("bbfit_shower_pos_y", &bbfit_shower_pos_y);
    input_tree->SetBranchAddress("bbfit_shower_pos_z", &bbfit_shower_pos_z);
    input_tree->SetBranchAddress("bbfit_shower_angerr_deg", &angerrordeg_bbfit);
    input_tree->SetBranchAddress("bbfit_shower_zenith_deg", &zenithdeg_bbfit);
    input_tree->SetBranchAddress("bbfit_shower_cos_zenith", &cos_zenith_bbfit);
    input_tree->SetBranchAddress("bbfit_shower_azimuth_deg", &azimuthdeg_bbfit);
    input_tree->SetBranchAddress("bbfit_shower_nusedlines", &nusedlines_bbfit);
    input_tree->SetBranchAddress("bbfit_shower_nusedhits", &nusedhits_bbfit);
    input_tree->SetBranchAddress("bbfit_shower_totalamp", &bbfit_shower_totalamp);
    input_tree->SetBranchAddress("bbfit_shower_zmin", &bbfit_shower_zmin);
    input_tree->SetBranchAddress("bbfit_shower_zmax", &bbfit_shower_zmax);
    input_tree->SetBranchAddress("bbfit_shower_nhits", &bbfit_shower_nhits);
    input_tree->SetBranchAddress("bbfit_shower_flag", &bbfit_shower_flag);

    input_tree->SetBranchAddress("showerdusj_quality", &showerdusj_quality);
    input_tree->SetBranchAddress("showerdusj_bjy", &showerdusj_bjy);
    input_tree->SetBranchAddress("showerdusj_pos_x", &showerdusj_pos_x);
    input_tree->SetBranchAddress("showerdusj_pos_y", &showerdusj_pos_y);
    input_tree->SetBranchAddress("showerdusj_pos_z", &showerdusj_pos_z);
    input_tree->SetBranchAddress("showerdusj_angerr_deg", &angerrordeg_showerdusj);
    input_tree->SetBranchAddress("showerdusj_zenith_deg", &zenithdeg_showerdusj);
    input_tree->SetBranchAddress("showerdusj_cos_zenith", &cos_zenith_showerdusj);
    input_tree->SetBranchAddress("showerdusj_azimuth_deg", &azimuthdeg_showerdusj);
    input_tree->SetBranchAddress("showerdusj_nusedlines", &nusedlines_showerdusj);
    input_tree->SetBranchAddress("showerdusj_nusedhits", &nusedhits_showerdusj);
    input_tree->SetBranchAddress("showerdusj_totalamp", &showerdusj_totalamp);
    input_tree->SetBranchAddress("showerdusj_energy", &showerdusj_energy);
    input_tree->SetBranchAddress("showerdusj_nhits", &showerdusj_nhits);
    input_tree->SetBranchAddress("showerdusj_radius", &showerdusj_radius);
    input_tree->SetBranchAddress("showerdusj_height", &showerdusj_height);
    input_tree->SetBranchAddress("showerdusj_flag", &showerdusj_flag);

    input_tree->SetBranchAddress("showertantra_quality", &showertantra_quality);
    input_tree->SetBranchAddress("showertantra_bjy", &showertantra_bjy);
    input_tree->SetBranchAddress("showertantra_pos_x", &showertantra_pos_x);
    input_tree->SetBranchAddress("showertantra_pos_y", &showertantra_pos_y);
    input_tree->SetBranchAddress("showertantra_pos_z", &showertantra_pos_z);
    input_tree->SetBranchAddress("showertantra_angerr_deg", &angerrordeg_showertantra);
    input_tree->SetBranchAddress("showertantra_zenith_deg", &zenithdeg_showertantra);
    input_tree->SetBranchAddress("showertantra_cos_zenith", &cos_zenith_showertantra);
    input_tree->SetBranchAddress("showertantra_azimuth_deg", &azimuthdeg_showertantra);
    input_tree->SetBranchAddress("showertantra_nusedlines", &nusedlines_showertantra);
    input_tree->SetBranchAddress("showertantra_nusedhits", &nusedhits_showertantra);
    input_tree->SetBranchAddress("showertantra_totalamp", &showertantra_totalamp);
    input_tree->SetBranchAddress("showertantra_energy", &showertantra_energy);
    input_tree->SetBranchAddress("showertantra_radius", &showertantra_radius);
    input_tree->SetBranchAddress("showertantra_height", &showertantra_height);
    input_tree->SetBranchAddress("showertantra_nhits", &showertantra_nhits);
    input_tree->SetBranchAddress("showertantra_flag", &showertantra_flag);

    // Define the number of events
    Int_t ntot = (Int_t)input_tree->GetEntries();

    // Loop over the events
    for (Int_t i; i < ntot; i++)
    {
        // Get the entry
        input_tree->GetEntry(i);

        // Check if the event is duplicated or not by the reconstruction flags
        if (aafit_flag == false)
        {
            // Set reco parameters to NAN
            aafit_lambda = NAN;
            aafit_bjy = NAN;
            aafit_pos_x = NAN;
            aafit_pos_y = NAN;
            aafit_pos_z = NAN;
            angerrordeg_aafit = NAN;
            zenithdeg_aafit = NAN;
            cos_zenith_aafit = NAN;
            azimuthdeg_aafit = NAN;
            nusedlines_aafit = NAN;
            nusedhits_aafit = NAN;
            energy_aafit_ANN_ECAP = NAN;
            energy_aafit_dEdX_CEA = NAN;
            aafit_totalamp = NAN;
            aafit_zmin = NAN;
            aafit_zmax = NAN;
            aafit_tracklength = NAN;
            aafit_nhits = NAN;
        }
        if (bbfit_flag == false)
        {
            // Set reco parameters to NAN
            bbfit_quality = NAN;
            bbfit_bjy = NAN;
            bbfit_pos_x = NAN;
            bbfit_pos_y = NAN;
            bbfit_pos_z = NAN;
            angerrordeg_bbfit = NAN;
            zenithdeg_bbfit = NAN;
            cos_zenith_bbfit = NAN;
            azimuthdeg_bbfit = NAN;
            nusedlines_bbfit = NAN;
            nusedhits_bbfit = NAN;
            bbfit_totalamp = NAN;
            bbfit_zmin = NAN;
            bbfit_zmax = NAN;
            bbfit_nhits = NAN;
        }
        if (gridfit_flag == false)
        {
            // Set reco parameters to NAN
            gridfit_quality = NAN;
            gridfit_bjy = NAN;
            gridfit_pos_x = NAN;
            gridfit_pos_y = NAN;
            gridfit_pos_z = NAN;
            angerrordeg_gridfit = NAN;
            zenithdeg_gridfit = NAN;
            cos_zenith_gridfit = NAN;
            azimuthdeg_gridfit = NAN;
            nusedlines_gridfit = NAN;
            nusedhits_gridfit = NAN;
            gridfit_totalamp = NAN;
            gridfit_zmin = NAN;
            gridfit_zmax = NAN;
            gridfit_nhits = NAN;
        }
        if (bbfit_shower_flag == false)
        {
            // Set reco parameters to NAN
            bbfit_shower_quality = NAN;
            bbfit_shower_bjy = NAN;
            bbfit_shower_pos_x = NAN;
            bbfit_shower_pos_y = NAN;
            bbfit_shower_pos_z = NAN;
            angerrordeg_bbfit_shower = NAN;
            zenithdeg_bbfit_shower = NAN;
            cos_zenith_bbfit_shower = NAN;
            azimuthdeg_bbfit_shower = NAN;
            nusedlines_bbfit_shower = NAN;
            nusedhits_bbfit_shower = NAN;
            bbfit_shower_totalamp = NAN;
            bbfit_shower_zmin = NAN;
            bbfit_shower_zmax = NAN;
            bbfit_shower_nhits = NAN;
        }
        if (showerdusj_flag == false)
        {
            // Set reco parameters to NAN
            showerdusj_quality = NAN;
            showerdusj_bjy = NAN;
            showerdusj_pos_x = NAN;
            showerdusj_pos_y = NAN;
            showerdusj_pos_z = NAN;
            angerrordeg_showerdusj = NAN;
            zenithdeg_showerdusj = NAN;
            cos_zenith_showerdusj = NAN;
            azimuthdeg_showerdusj = NAN;
            nusedlines_showerdusj = NAN;
            nusedhits_showerdusj = NAN;
            showerdusj_totalamp = NAN;
            showerdusj_energy = NAN;
            showerdusj_nhits = NAN;
            showerdusj_radius = NAN;
            showerdusj_height = NAN;
        }
        if (showertantra_flag == false)
        {
            // Set reco parameters to NAN
            showertantra_quality = NAN;
            showertantra_bjy = NAN;
            showertantra_pos_x = NAN;
            showertantra_pos_y = NAN;
            showertantra_pos_z = NAN;
            angerrordeg_showertantra = NAN;
            zenithdeg_showertantra = NAN;
            cos_zenith_showertantra = NAN;
            azimuthdeg_showertantra = NAN;
            nusedlines_showertantra = NAN;
            nusedhits_showertantra = NAN;
            showertantra_totalamp = NAN;
            showertantra_energy = NAN;
            showertantra_radius = NAN;
            showertantra_height = NAN;
            showertantra_nhits = NAN;
        }

        // Fill the output tree
        output_tree->Fill();

        // Print the progress every 5% of the events
        if (i % (ntot / 20) == 0)
        {
            cout << "Processed " << i << " events out of " << ntot << endl;
        }
    }

    // Write the output tree
    output_tree->Write();
    output->Close();
}
