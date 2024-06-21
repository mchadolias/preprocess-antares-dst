/**
 * @brief Calculate neutrino oscillation weights for atmospheric neutrinos. 
 * For each event in the input file, the code calculates the oscillation probability
 * for the neutrino to undergo oscillation to a final state. The resulting weight is
 * from the product of the normalized weight, the oscillation probability, and the
 * flux of atmospheric neutrinos. The weights are then stored in the output file.
 *  
 * @author M. Chadolias
 * @version 1.1
 * @date 2024-06-20
 */

// C++
using namespace std;
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <iostream>
#include <math.h>       /* cos */

// ROOT
#include "TTree.h"
#include "TFile.h"
#include "TSystem.h"
#include "TMath.h"
#include "TH2D.h"

// OscProb
#include "PremModel.h"
#include "PMNS_Fast.h"
#include "PMNS_Base.h"

// Constants
const int scm_to_sm = 1e+4;

// declare functions
double GetFlux(TH2D** FluxHist_copy, int flavour , double E , double cos_zen);
pair<double, double> Get_Osc_Prob( OscProb::PMNS_Fast pmns , OscProb::PremModel prem , map<int,int> flavour_cor, int flv_f , double E , double cos_zen ); 
void TestOscProb( OscProb::PMNS_Fast pmns , OscProb::PremModel prem , map<int,int> flavour_cor, int type_i , double E , double cos_zen );
int sgn( double x );
void usage();


int main(int argc, char* argv[]){
  cout << "\nStarting OscillationWeights.cc" << endl;
  cout << "Running code:" << "\nYour input parameters are the following:"<< endl;
  for(int i = 1; i < argc; i++) { cout << i << " \t " << argv[i] << endl; }
  string input_file = argv[1];
  string out_file = argv[2];
  string cluster = argv[3];

  string hist_names[4] = {"h_nue_logElogF", "h_anue_logElogF", "h_numu_logElogF", "h_anumu_logElogF"};
  string hist_names_clone[4] = {"clone_h_nue_logElogF", "clone_h_anue_logElogF", "clone_h_numu_logElogF", "clone_h_anumu_logElogF"};
  string hist_path, flux_file;

  if (cluster == "woody"){
    cout << "Running on Woody" << endl;
    hist_path = "/home/saturn/capn/mppi133h/master_thesis/antares_dst/oscillation_weights/models/";
  }
  else if (cluster == "in2p3"){
    cout << "Running on IN2P3" << endl;
    hist_path = "/sps/km3net/users/mchadoli/masters_thesis/antares_dst/oscillation_weights/models/";
  }
  flux_file = hist_path + "Honda2014_frj-solmin-aa_ORCA6_hist.root";

  if( argc != 4){
    usage();
    exit(1);
  }
  //===========================================================
  // input root file
  //===========================================================
  cout << "\nReading Filr= " << input_file.c_str() << endl;
  TFile *f = new TFile(input_file.c_str());
  TTree *oldtree = (TTree *)f->Get("sel"); // imported Selected events tree

  if (!f || f->IsZombie()){
    cerr << "Error: Could not open the input ROOT file." << std::endl;
    exit(1);;
  }

  // get the histogram that contain the flux values
  TFile* FluxInput =  TFile::Open(flux_file.c_str());
  TH2D* FluxHist[4];

  if (!FluxInput || FluxInput->IsZombie()){
    cerr << "Error: Could not open the input ROOT file." << std::endl;
    exit(1);
  }
  
  cout  << "Retrieving Histograms" << endl;
  for (int i=0; i<4; i++){
    FluxInput -> GetObject(hist_names[i].c_str(), FluxHist[i]);
  }

  //===========================================================
  // Output root file
  //===========================================================
  TFile *f_out = new TFile(out_file.c_str(), "RECREATE");
  TTree *event_tree = oldtree->CloneTree(0, "fast");
  TH2D* FluxHist_copy[4];

  cout  << "Copying Histograms" << endl;
  for (int i=0; i<4; i++){
    FluxHist_copy[i] =  dynamic_cast<TH2D*>(FluxHist[i] -> Clone(hist_names_clone[i].c_str()));
  }  
  FluxInput->Close();

  if (!f_out || f_out->IsZombie()){
    cerr << "Error: Could not open the output ROOT file." << std::endl;
    exit(1);
  }

  //===========================================================
  // analysis
  //===========================================================
  // Variables
  int run_id, type;
  double run_duration, w2, w3, energy_true, cos_zenith_true, w_non_osc, ngen;

  // New variables
  double w_osc, prob_nue, prob_numu, w2_norm, flux_nue, flux_numu;

  // Branches from old tree
  oldtree->SetBranchAddress("run_id", &run_id);
  oldtree->SetBranchAddress("type", &type);
  oldtree->SetBranchAddress("RunDurationYear", &run_duration);
  oldtree->SetBranchAddress("ngen", &ngen);
  oldtree->SetBranchAddress("w2", &w2);
  oldtree->SetBranchAddress("w3", &w3);
  oldtree->SetBranchAddress("energy_true", &energy_true); // check the units of the energy 
  oldtree->SetBranchAddress("cos_zenith_true", &cos_zenith_true);
  oldtree->SetBranchAddress("w_non_osc", &w_non_osc);

  // New Branches added in tree
  event_tree->Branch("w_osc", &w_osc, "w_osc/D");
  event_tree->Branch("prob_nue", &prob_nue, "prob_nue/D");
  event_tree->Branch("prob_numu", &prob_numu, "prob_numuu/D");

  Int_t ntot = (Int_t)oldtree->GetEntries();
  Int_t nsel = 0; Int_t nsample = 0;

  // weight calculation - oscillation parameters
  OscProb::PMNS_Fast pmns;
  OscProb::PremModel prem;

  // input neutrino flavour corresponding to OscProb arguments
  map<int, int> flavour_cor;
  flavour_cor.insert(pair<int, int>(12, 0));
  flavour_cor.insert(pair<int, int>(-12, 0));
  flavour_cor.insert(pair<int, int>(14, 1));
  flavour_cor.insert(pair<int, int>(-14, 1));
  flavour_cor.insert(pair<int, int>(16, 2));
  flavour_cor.insert(pair<int, int>(-16, 2));

  cout << "\nStarting loop" << endl;
  for (unsigned int i = 0; i < ntot; i++)
  {
    oldtree->GetEntry(i);

    if (cos_zenith_true < 0 &&  energy_true < TMath::Power(10, 4))
    {

      // Data according to  JHEP 09 (2020) 178 ---> assuming Normal Ordering without SK atmospheric data
      double dm_21 = 7.42e-5;
      double dm_31 = 2.514e-3;
      double theta_12 = 0.5836;
      double theta_23 = 0.8552;
      double theta_13 = 0.1496;
      double dcp = (195 * TMath::Pi()) / 180;

      // Apply data to OscProb objects
      pmns.SetDm(2, dm_21);
      pmns.SetDm(3, dm_31);
      pmns.SetAngle(1, 2, theta_12);
      pmns.SetAngle(1, 3, theta_13);
      pmns.SetAngle(2, 3, theta_23);
      pmns.SetDelta(1, 3, dcp);

      // oscillation weight calculation
      w2_norm = (w2 * scm_to_sm) / ngen; // normalized weight

      pair<double, double> prob = Get_Osc_Prob(pmns, prem, flavour_cor, type, energy_true, cos_zenith_true);
      prob_nue = prob.first;  // oscillation probability for nu_e to transition to nu_final
      prob_numu = prob.second; // oscillation probability for nu_mu to transition to nu_final

      flux_nue = GetFlux(FluxHist_copy, sgn(type) * 12, energy_true, cos_zenith_true);  // flux for initial nu_e
      flux_numu = GetFlux(FluxHist_copy, sgn(type) * 14, energy_true, cos_zenith_true); // flux for initial nu_mu

      w_osc = w2_norm * (flux_nue * prob_nue + flux_numu * prob_numu) * run_duration; // oscillated atmospheric weight for nu_final
    }
    else{ w_osc = w_non_osc;}

    event_tree->Fill();
    nsel++;

    if (i % (ntot / 50) == 0)
    {
      cout << "Current Event: \t" << i << "\t\t Total Number of Events: \t" << ntot << endl;
    }
  }

  cout << "\nFinal countdown:\t" << nsel << "\t out of " << ntot << " events." << endl;

  // WRITE OUTPUT
  f_out->cd();
  event_tree->Write();
  f_out->Close();
  f->Close();
  cout << endl;
  cout << "||========= Successful execution! =========||" << endl;

} // end of main

/**
 * @brief Function to calculate the neutrino flux 
 * 
 * @param flavour 
 * @param E 
 * @param cos_zen run_duration_calc
 * @return Honda flux for the current E and cos_zenith
 */
double GetFlux(TH2D** FluxHist_copy, int flavour , double E , double cos_zen ){
  int i; // option

  if( flavour==12 ){ i=0; }
  if( flavour==-12 ){ i=1; }
  if( flavour==14 ){ i=2; }
  if( flavour==-14 ){ i=3; }

  double flux = FluxHist_copy[i]-> Interpolate(TMath::Log10(E), cos_zen); // it's in logF form
  flux = TMath::Power(10, flux);
  return flux;
}

/**
 * @brief Function to calculate neutrino oscillation probability
 * 
 * @param pmns The PMNS matrix
 * @param prem  The PREM (Earth model)
 * @param flavour_cor The mapping of the neutrino flavours
 * @param type_i  The initial neutrino flavour
 * @param type_f  The final neutrino flavour
 * @param E       The neutrino energy
 * @param cos_zen  The cosine of the zenith angle 
 * @return Calculate oscillation probability from OscProb  
 */
pair<double, double> Get_Osc_Prob( OscProb::PMNS_Fast pmns , OscProb::PremModel prem , map<int,int> flavour_cor, int type_f , double E , double cos_zen ){

  bool is_nubar;
  int flv_f = flavour_cor[type_f];
  if( type_f < 0 ){ 
    is_nubar = 1;
    }

  prem.FillPath( cos_zen );
  pmns.SetPath( prem.GetNuPath() );
  pmns.SetIsNuBar( is_nubar ); // in case of antiparticle

  double prob_nue = pmns.Prob( 0, flv_f, E );
  double prob_numu = pmns.Prob( 1, flv_f, E );

  return make_pair(prob_nue, prob_numu);
}

/**
 * @brief Typical sgn function 
 * 
 * @param x 
 * @return -1 for negative values and +1 for positive ones 
 */
int sgn( double x ){
  int sign ;
  if( x > 0 ){ sign = 1; }
  if( x < 0 ){ sign = -1; }
  return sign;
}

/**
 * @brief Statement print in case something fails due to wrong input parameters
 * 
 */
void usage(){
  cerr << "A problem arose while running ExpectedEvents.CC" << endl;
}


void TestOscProb(OscProb::PMNS_Fast pmns , OscProb::PremModel prem , map<int,int> flavour_cor, int type_i , double E , double cos_zen ){

  bool is_nubar;
  int flv_i = flavour_cor[type_i];
  
  if (type_i < 0){is_nubar = 1;}

  prem.FillPath(cos_zen);
  pmns.SetPath(prem.GetNuPath());
  pmns.SetIsNuBar(is_nubar); // in case of antiparticle

  double prob_e_trans = pmns.Prob(flv_i, 0, E);
  double prob_m_trans = pmns.Prob(flv_i, 1, E);
  double prob_t_trans = pmns.Prob(flv_i, 2, E);
  double prob = prob_e_trans + prob_m_trans + prob_t_trans;
  cout << "\nThe transition probabilities from neutrino with flavour: " << type_i << endl
       << "Energy: "  << E << " CosZenith: " << cos_zen << endl 
       << "prob to e: " << prob_e_trans << endl
       << "prob to mu: " << prob_m_trans << endl
       << "prob to tau: " << prob_t_trans << endl
       << "Sum of prob: " << prob << "\n" << endl;

}