/**
 * @file addCanANTARES.cc
 * @author Michael Chadolias
 * @brief Simple function that adds a 3D vector with the dimensions of the ANTARES detector. Inspired by the AddCanDimensions function in the SWIM code.
 * @version 0.1
 * @date 2024-06-21
 * 
 */

#include "TFile.h"
#include "TTree.h"
#include "TMap.h"
#include "TVectorD.h"
#include <iostream>

using namespace std;

void addCanANTARES(const string & outpath){

  TFile* outfile = TFile::Open(outpath.c_str(),"update");

  if (!outfile){
    cerr << "ERROR: file " << outpath << " could not be opened. Exiting" << endl; exit(1);
  }


  // Store can dimensions in vector
  TVectorD vcd(3);
  double zmin=-271.42;
  double zmax=357.94;
  double radius = 279.45;

  vcd[0]=zmin;
  vcd[1]=zmax;
  vcd[2]=radius;
  vcd.Write("CanDimensions");

  outfile->Close();
}
