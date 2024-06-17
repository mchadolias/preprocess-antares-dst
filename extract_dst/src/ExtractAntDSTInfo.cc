/********************************************************************************
                      ExtractAntDSTInfo

  source code that extracts variables (both mc true information and reconstruction
  from ANTARES summary files (AntDST) to root files ready for SWIM
*********************************************************************************/

#include "AntDST.h"
#include "AntDSTFile.h"
#include "FileInfo.h"
#include "AntEventInfo.h"
#include "MCEvent.h"
#include "AntDataQuality.h"
#include "RecParticle.h"

#include <TFile.h>
#include <TMath.h>
#include <TTimeStamp.h>
#include <TTree.h>
#include <TVector3.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

/*========================================================
   print some usage hints
==========================================================*/
void usage()
{
  cerr << "\n  Usage: analyze <Outputfilename> <AntDSTfileName> \n"
       << "         where <AntDSTfileName> is the name of an AntDST file, e.g. AntDST.root \n"
       << "         or a list of AntDST files, e.g. \"AntDST*.root\". \n " << endl;
}

/*========================================================
  AcceptEventSoft()

   Event selection soft selection (trigger events)
   return value determines if event is used (return true)
   or not (return false)
==========================================================*/
vector<int> counters (6,0);
bool AcceptEventSoft(AntDST &theAntDST, vector<int> &counters) // accept event if the AntDSTs file has at least one of the reconstructions (most inclusive!!!)
{

  bool selected = false;

  if (theAntDST.HasStrategy(eAAFit) && theAntDST.GetStrategy(eAAFit).HasRecParticle(eMuon)) {counters[0]++; selected = true;}
  if (theAntDST.HasStrategy(eShowerTantraFit) &&
           theAntDST.GetStrategy(eShowerTantraFit).HasRecParticle(eBrightPoint)) {counters[1]++; selected = true;}
  if ((theAntDST.HasStrategy(eBBFit) && 
            theAntDST.GetStrategy(eBBFit).HasRecParticle(eMuon))) {counters[2]++; selected = true;}
  if ((theAntDST.HasStrategy(eShowerDusjFit) &&
            theAntDST.GetStrategy(eShowerDusjFit).HasRecParticle(eBrightPoint))) {counters[3]++; selected = true;}
  if ((theAntDST.HasStrategy(eGridFit) && 
            theAntDST.GetStrategy(eGridFit).HasRecParticle(eMuon))) {counters[4]++; selected = true;}
  if ((theAntDST.HasStrategy(eBBFitBrightPoint) && 
            theAntDST.GetStrategy(eBBFitBrightPoint).HasRecParticle(eBrightPoint))) {counters[5]++; selected = true;}

  return selected;
}

int extract_run_id(string filename)
{

  std::string::size_type p = filename.find('_');
  std::string::size_type pp = filename.find('_', p + 6);

  int run = atoi(filename.substr(p + 1, pp - p - 1).c_str());
  return run;
}

bool read_muon_weights(std::map<int, int> &m)
{
  int run = 0, number = 0;
  bool status = true;

  std::ifstream myfile;
  myfile.open("muons/muon_weights.dat");

  if (myfile.is_open())
  {
    //    cout << "file open" <<endl;
    // myfile >> run >> number;
    while (myfile >> run >> number)
    {
      m.insert(pair<int, int>(run, number));
    }
  }
  else
  {
    cout << "Unable to open file: muon_weights.dat. \n ";
    status = false;
  }

  myfile.close();
  // cout << "muon weight is now: " << weight <<endl;
  return status;
}

double muon_weight(int run_id, const std::map<int, int> &m)
{

  int number = 0;
  double weight = 0;

  std::map<int, int>::const_iterator it = m.find(run_id);

  //  cout << "Iterator points to " << it->first <<
  //    " = " << it->second << endl;

  number = it->second;

  if (number == 0 || number == 3)
  {
    weight = 3.;
  }
  else if (number == 1)
  {
    weight = 9.;
  }
  else if (number == 2)
  {
    weight = 9. / 2.;
  }
  return weight;
}

Float_t correctForLivetimeRatio( double type, double interaction_type, double run){

  // NB: no nu-tau file for runs < 34348 was generated -> weight files in 2009 by
  // an extra factor of 2 to account for missing files in 2007

  Float_t DataMCRatio = 0;
  
  if(type == 14 && interaction_type == 1){ // numuCC
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/185.206; //1.10986
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/176.05; //1.21145
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/205.652; //1.11195
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/213.281; //1.13012
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/273.041; //1.0318
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/225.306; //1.11021
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/275.059; //1.0222
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/334.233; //1.01152
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/348.871; //1.01167
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/347.604; //1.02581
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/310.459; //1.14512
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/282.929; //1.20043
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/316.377; //1.10973
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/264.507; //1.34224
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/317.415; //1.10302
    else if(run>=89982)                 DataMCRatio = 40.824/41.2675; //0.989253
  } else if(type == -14 && interaction_type == 1){  // anumuCC
    
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/184.076; //1.11667
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/175.543; //1.21495
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/205.782; //1.11124
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/213.227; //1.13041
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/271.578; //1.03736
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/224.735; //1.11303
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/273.856; //1.02669
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/332.875; //1.01565
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/351.604; //1.00381
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/345.554; //1.03189
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/310.959; //1.14328
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/282.113; //1.2039
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/315.858; //1.11155
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/263.664; //1.34653
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/316.934; //1.1047
    else if(run>=89982)                 DataMCRatio = 40.824/41.2675; //0.989253
  } else if(type == 14 && interaction_type == 0){  // numuNC
    
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/185.088; //1.11056
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/176.211; //1.21034
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/205.918; //1.11051
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/213.04; //1.1314
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/257.072; //1.0959
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/225.065; //1.1114
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/275.101; //1.02205
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/334.199; //1.01163
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/352.294; //1.00184
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/347.298; //1.02671
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/309.459; //1.14882
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/283.902; //1.19632
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/317.166; //1.10697
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/264.991; //1.33979
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/317.915; //1.10129
    else if(run>=89982)                 DataMCRatio = 40.824/41.2675; //0.989253
  } else if(type == -14 && interaction_type == 0){  // anumuNC
    
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/184.586; //1.11358
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/175.783; //1.21329
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/205.391; //1.11336
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/212.722; //1.13309
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/221.754; //1.27043
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/224.36; //1.11489
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/275.101; //1.02205
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/332.732; //1.01609
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/351.421; //1.00433
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/346.278; //1.02973
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/309.64; //1.14815
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/283.13; //1.19958
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/317.166; //1.10697
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/264.991; //1.33979
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/317.032; //1.10436
    else if(run>=89982)                 DataMCRatio = 40.824/41.2675; //0.989253
  } else if(type == 12 && interaction_type == 1){  // nueCC
    
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/185.256; //1.10956
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/176.411; //1.20897
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/205.918; //1.11051
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/213.407; //1.12945
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/232.246; //1.21304
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/225.428; //1.10961
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/275.101; //1.02205
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/334.233; //1.01152
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/352.294; //1.00184
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/347.87; //1.02502
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/310.959; //1.14328
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/283.902; //1.19632
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/316.666; //1.10872
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/264.991; //1.33979
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/318.415; //1.09956
    else if(run>=89982)                 DataMCRatio = 40.824/41.2675; //0.989253
  } else if(type == -12 && interaction_type == 1){  // anueCC
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/185.256; //1.10956     
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/176.411; //1.20897      
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/205.918; //1.11051     
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/213.412; //1.12943     
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/259.408; //1.08603     
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/224.841; //1.11251     
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/273.904; //1.02651     
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/332.88; //1.01564     
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/345.79; //1.02068     
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/345.945; //1.03072     
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/310.102; //1.14643     
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/283.902; //1.19632      
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/316.666; //1.10872      
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/264.991; //1.33979
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/317.915; //1.10129      
    else if(run>=89982)                 DataMCRatio = 40.824/41.2675; //0.989253
    
  } else if(type == 12 && interaction_type == 0){  // nueNC
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/185.101; //1.11049
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/176.405; //1.20901
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/205.494; //1.1128
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/213.168; //1.13073
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/225.914; //1.24704
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/222.781; //1.12279
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/267.152; //1.05246
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/330.301; //1.02357
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/349.532; //1.00976
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/346.216; //1.02992
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/309.622; //1.14821
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/283.902; //1.19632
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/317.166; //1.10697
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/264.991; //1.33979
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/317.915; //1.10129
    else if(run>=89982)                 DataMCRatio = 40.824/41.2675; //0.989253                                                                                                                        
  } else if(type == -12 && interaction_type == 0){  // anueNC
    
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/183.845; //1.11807
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/176.276; //1.2099
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/205.43; //1.11315
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/212.645; //1.1335
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/268.974; //1.0474
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/223.009; //1.12164
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/273.823; //1.02682
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/332.817; //1.01583
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/351.642; //1.0037
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/347.324; //1.02663
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/309.997; //1.14682
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/281.925; //1.20471
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/308.994; //1.13625
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/264.174; //1.34393
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/318.415; //1.09956
    else if(run>=89982)                 DataMCRatio = 40.824/41.2675; //0.989253
  } else if(type == 16 && interaction_type == 2) { //nutauCCmu      
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/0; //inf
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/99.2344; //2.14921
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/207.129; //1.10402
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/237.026; //1.01691
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/274.346; //1.02689
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/225.349; //1.11
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/276.423; //1.01716
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/334.84; //1.00969
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/352.639; //1.00086
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/354.659; //1.0054
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/193.458; //1.83767
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/125.467; //2.70698
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/144.941; //2.42231
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/171.686; //2.06792
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/316.504; //1.1062
    else if(run>=89982)                 DataMCRatio = 40.824/41.2675; //0.989253
  } else if(type == -16 && interaction_type == 2) { //anutauCCmu
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/0; //inf
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/99.0051; //2.15419
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/207.025; //1.10457
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/238.264; //1.01163
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/274.445; //1.02652
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/224.96; //1.11192
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/276.157; //1.01814
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/335.841; //1.00668
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/352.039; //1.00256
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/354.954; //1.00456
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/193.264; //1.83952
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/125.467; //2.70698
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/144.941; //2.42231
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/172.186; //2.06191
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/317.368; //1.10319
    else if(run>=89982)                 DataMCRatio = 40.824/40.7674; //1.00139
  } else if(type == 16 && interaction_type == 3) { //nutauCCshow
    
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/0; //inf
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/92.3397; //2.30969
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/207.019; //1.1046
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/236.514; //1.01911
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/274.935; //1.02469
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/225.694; //1.1083
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/276.194; //1.018
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/335.841; //1.00668
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/352.942; //0.999999
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/354.954; //1.00456
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/193.764; //1.83477
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/125.467; //2.70698      
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/144.604; //2.42796      
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/172.186; //2.06191      
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/317.434; //1.10296      
    else if(run>=89982)                 DataMCRatio = 40.824/41.2675; //0.989253
    
  } else if(type == -16 && interaction_type == 3) { //anutauCCshow
    
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/0; //inf
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/92.4961; //2.30578
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/207.253; //1.10336
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/238.541; //1.01045
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/274.502; //1.02631
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/225.682; //1.10836
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/276.928; //1.0153
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/335.841; //1.00668
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/352.942; //0.999999
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/354.779; //1.00506
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/193.764; //1.83477
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/125.467; //2.70698
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/144.941; //2.42231
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/172.186; //2.06191
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/317.434; //1.10296
    else if(run>=89982)                 DataMCRatio = 40.824/41.2675; //0.989253
    
  } else if(type == 16 && interaction_type == 0) { //nutauNC
    
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/0; //inf
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/99.3108; //2.14756
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/207.212; //1.10358
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/238.147; //1.01212
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/275; //1.02445
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/225.048; //1.11148
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/276.928; //1.0153
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/335.841; //1.00668
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/352.942; //0.999999
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/354.954; //1.00456
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/193.764; //1.83477
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/125.467; //2.70698
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/144.941; //2.42231
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/172.186; //2.06191
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/317.434; //1.10296    
    else if(run>=89982)                 DataMCRatio = 40.824/41.2675; //0.989253
    
  } else if(type == -16 && interaction_type == 0) { //anutauNC
    
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/0; //inf
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/98.8201; //2.15823
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/207.253; //1.10336
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/238.323; //1.01137
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/274.384; //1.02675
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/225.694; //1.1083
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/276.928; //1.0153
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/335.841; //1.00668
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/352.942; //0.999999   
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/354.954; //1.00456     
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/193.764; //1.83477    
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/125.467; //2.70698     
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/144.941; //2.42231      
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/172.186; //2.06191     
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/317.434; //1.10296      
    else if(run>=89982)                 DataMCRatio = 40.824/41.2675; //0.989253
  }
  else if (interaction_type == -1){ // muons
    
    if(run >= 25416 && run < 31051)     DataMCRatio = 205.552/189.22; //1.08631
    else if(run>=31051 && run < 38234)  DataMCRatio = 213.276/190.573; //1.11913
    else if(run>=38234 && run < 45639)  DataMCRatio = 228.674/226.374; //1.01016
    else if(run>=45639 && run < 54250)  DataMCRatio = 241.034/241.417; //0.998415
    else if(run>=54250 && run < 61907)  DataMCRatio = 281.724/254.565; //1.10669
    else if(run>=61907 && run < 68690)  DataMCRatio = 250.137/198.276; //1.26156
    else if(run>=68690 && run < 74353)  DataMCRatio = 281.166/266.569; //1.05476
    else if(run>=74353 && run < 79222)  DataMCRatio = 338.085/330.884; //1.02176
    else if(run>=79222 && run < 83078)  DataMCRatio = 352.942/319.608; //1.1043
    else if(run>=83078 && run < 84663)  DataMCRatio = 356.574/224.545; //1.58798
    else if(run>=84663 && run < 85741)  DataMCRatio = 355.512/352.794; //1.0077
    else if(run>=85741 && run < 86878)  DataMCRatio = 339.637/303.736; //1.1182
    else if(run>=86878 && run < 87886)  DataMCRatio = 351.093/314.68; //1.11571
    else if(run>=87886 && run < 88931)  DataMCRatio = 355.032/322.866; //1.09962
    else if(run>=88931 && run < 89982)  DataMCRatio = 350.116/318.415; //1.09956
    else if(run>=89982)                 DataMCRatio = 40.824/40.8242; //0.999995
  }

  return DataMCRatio;
  }

const double sec_to_year = 1./(3.6*1.e3*24*365.25);

// ##############################################################
int main(int argc, char **argv)
{

  if (argc != 3)
  {
    usage();
    exit(1);
  }

  //  OUTPUT-Definitions
  TFile *outFile = new TFile(argv[1], "RECREATE");
  TTree *outTree = new TTree("sel", "TreeSelectedEvents");
  AntDSTFile *dataFile = AntDSTFile::SmartOpen(argv[2]);
  AntDST *theAntDST = new AntDST();
  dataFile->SetBuffers(&(theAntDST));

  unsigned int nsel = 0;
  unsigned int ntot = dataFile->GetNEvents();
  int fivePercent = int(0.05 * ntot);
  if (fivePercent < 1)
    fivePercent = 1;
  cout << " reading file(s) " << argv[2] << " with " << ntot << " events\n"
       << endl;

  FileInfo theInfo;
  dataFile->ReadFileInfo(theInfo);
  cout << " file reconstructed by: " << theInfo.GetUser() << endl;
  cout << " framework " << theInfo.GetReconstructionFramework() << endl;

  // get filename
  string currentFileName = dataFile->GetActiveFileName();
  int run = extract_run_id(currentFileName);

  AntEventInfo evtInfo; // = new AntEventInfo();
  AntDataQuality currentDataQuality;

  // General Info parameters
  double MJD = 0.;
  unsigned long long event_id = 0, trigg_counter = 0;

  // data quality parameters
  double baseline_rate = 0, burst_fraction = 0, RunDurationYear = 0, runDurationSeconds = 0, runDuration = 0, run_quality = 0, frame = 0;
  double detcenter_x = 0., detcenter_y = 0., detcenter_z = 0.; // detector center

  // true parameters
  bool is_neutrino = true, is_cc = true, t3N_active = false, tT2_active = false, tT3_active = false, tTQ_active = false;
  bool aafit_flag = false, bbfit_flag = false, gridfit_flag = false, bbfit_shower_flag = false, showerdusj_flag = false, showertantra_flag = false;
  
  int run_id = 0, type = 0, interaction_type = 0, Date = 0, scan;
  double E_min_gen = -1, E_max_gen = -1;
  Float_t w_muon, ratio_active, DataMCRatio; // weight for muons
  double energy_true = 0, w2= 0, w3= 0, w_honda= 0, ngen = 0, pos_x_true = 0, pos_y_true = 0, pos_z_true = 0, bjorken_y_true = 0, cos_zenith_true = 0, azimuthdeg_true = 0;

  // reconstruction parameters
  double aafit_lambda = -20., aafit_bjy = 0, aafit_pos_x = 0, aafit_pos_y = 0, aafit_pos_z = 0, angerrordeg_aafit = 0, zenithdeg_aafit = 0, cos_zenith_aafit = 0, azimuthdeg_aafit = 0, nusedlines_aafit = 0, nusedhits_aafit = 0., energy_aafit_ANN_ECAP = 0, energy_aafit_dEdX_CEA = 0, aafit_totalamp = 0, aafit_zmin = 0, aafit_zmax = 0, aafit_tracklength = 0., aafit_nhits = 0.;
  double bbfit_quality = 0., bbfit_bjy = 0, bbfit_pos_x = 0, bbfit_pos_y = 0, bbfit_pos_z = 0, angerrordeg_bbfit = 0., zenithdeg_bbfit = 0., cos_zenith_bbfit = 0, azimuthdeg_bbfit = 0., nusedlines_bbfit = 0., nusedhits_bbfit = 0., bbfit_totalamp = 0., bbfit_zmin = 0, bbfit_zmax = 0, bbfit_nhits = 0.;
  double gridfit_quality = 0., gridfit_bjy = 0, gridfit_pos_x = 0, gridfit_pos_y = 0, gridfit_pos_z = 0, angerrordeg_gridfit = 0., zenithdeg_gridfit = 0., cos_zenith_gridfit = 0, azimuthdeg_gridfit = 0., nusedlines_gridfit = 0., nusedhits_gridfit = 0., gridfit_totalamp = 0., gridfit_zmin = 0, gridfit_zmax = 0, gridfit_nhits = 0.;
  double bbfit_shower_quality = 0., bbfit_shower_bjy = 0, bbfit_shower_pos_x = 0, bbfit_shower_pos_y = 0, bbfit_shower_pos_z = 0, angerrordeg_bbfit_shower = 0., zenithdeg_bbfit_shower = 0., cos_zenith_bbfit_shower = 0, azimuthdeg_bbfit_shower = 0., nusedlines_bbfit_shower = 0., nusedhits_bbfit_shower = 0., bbfit_shower_totalamp = 0., bbfit_shower_zmin = 0, bbfit_shower_zmax = 0, bbfit_shower_nhits = 0.;
  double showerdusj_quality = 0., showerdusj_bjy = 0, showerdusj_pos_x = 0, showerdusj_pos_y = 0, showerdusj_pos_z = 0, angerrordeg_showerdusj = 0., zenithdeg_showerdusj = 0., cos_zenith_showerdusj = 0, azimuthdeg_showerdusj = 0., nusedlines_showerdusj = 0., nusedhits_showerdusj = 0., showerdusj_totalamp = 0., showerdusj_energy = 0., showerdusj_nhits = 0., showerdusj_radius = 0., showerdusj_height = 0.;
  double showertantra_quality = 0., showertantra_bjy = 0, showertantra_pos_x = 0, showertantra_pos_y = 0, showertantra_pos_z = 0, angerrordeg_showertantra = 0., zenithdeg_showertantra = 0., cos_zenith_showertantra = 0, azimuthdeg_showertantra = 0., nusedlines_showertantra = 0., nusedhits_showertantra = 0., showertantra_totalamp = 0., showertantra_energy = 0., showertantra_radius = 0., showertantra_height = 0., showertantra_nhits = 0.;

  // check if file is Mupage
  bool isMupage = (currentFileName.find("mupage") != string::npos);
  std::map<int, int> m;
  if (isMupage)
  {
    bool status = read_muon_weights(m);
    if (false == status)
    {
      cerr << "Error in reading the muon weights data";
      exit(1);
    }
  }

  if (!isMupage)
  {
    E_min_gen = 5;    // GeV
    E_max_gen = 20e3; // GeV

    if (currentFileName.find("_CC_") != string::npos)
      interaction_type = 1;
    else if (currentFileName.find("_CCmu") != string::npos)
      interaction_type = 2;
    else if (currentFileName.find("_CCshow") != string::npos)
      interaction_type = 3;
    else
    {
      is_cc = 0;
      interaction_type = 0;
    }
  }
  else
  {
    interaction_type = -1;
    E_min_gen = 4;   // GeV
    E_max_gen = 1e6; // GeV
  }

  /// Set the branches for the output tree
  // General Info branches
  outTree->Branch("MJD", &MJD);
  outTree->Branch("Date", &Date);
  outTree->Branch("run", &run);
  outTree->Branch("event_id", &event_id);
  outTree->Branch("event_counter_trigger", &trigg_counter);
  outTree->Branch("frame_index", &frame);
  outTree->Branch("BaselineRate", &baseline_rate);
  outTree->Branch("RatioActive", &ratio_active);
  outTree->Branch("BurstFraction", &burst_fraction);
  outTree->Branch("run_duration", &runDurationSeconds);
  outTree->Branch("run_duration_dq", &runDuration);
  outTree->Branch("RunDurationYear", &RunDurationYear);
  outTree->Branch("RunQuality", &run_quality);
  outTree->Branch("Scan", &scan);

  // Selected events true values branches
  outTree->Branch("run_id", &run_id);
  outTree->Branch("type", &type);
  outTree->Branch("is_neutrino", &is_neutrino);
  outTree->Branch("is_cc", &is_cc);
  outTree->Branch("interaction_type", &interaction_type);
  outTree->Branch("w2", &w2);
  outTree->Branch("w3", &w3);
  outTree->Branch("w_muon", &w_muon);
  outTree->Branch("w_honda", &w_honda);
  outTree->Branch("ngen", &ngen);
  outTree->Branch("DataMCRatio", &DataMCRatio);
  outTree->Branch("pos_x_true", &pos_x_true);
  outTree->Branch("pos_y_true", &pos_y_true);
  outTree->Branch("pos_z_true", &pos_z_true);
  outTree->Branch("energy_true", &energy_true);
  outTree->Branch("cos_zenith_true", &cos_zenith_true);
  outTree->Branch("azimuthdeg_true", &azimuthdeg_true);
  outTree->Branch("bjorken_y_true", &bjorken_y_true);
  outTree->Branch("E_min_gen", &E_min_gen);
  outTree->Branch("E_max_gen", &E_max_gen);

  outTree->Branch("t3N_active", &t3N_active);
  outTree->Branch("tT2_active", &tT2_active);
  outTree->Branch("tT3_active", &tT3_active);
  outTree->Branch("tTQ_active", &tTQ_active);

  // Reconstruction parameters
  outTree->Branch("aafit_lambda", &aafit_lambda);
  outTree->Branch("aafit_bjy", &aafit_bjy);
  outTree->Branch("aafit_pos_x", &aafit_pos_x);
  outTree->Branch("aafit_pos_y", &aafit_pos_y);
  outTree->Branch("aafit_pos_z", &aafit_pos_z);
  outTree->Branch("aafit_angerr_deg", &angerrordeg_aafit);
  outTree->Branch("aafit_zenith_deg", &zenithdeg_aafit);
  outTree->Branch("aafit_cos_zenith", &cos_zenith_aafit);
  outTree->Branch("aafit_azimuth_deg", &azimuthdeg_aafit);
  outTree->Branch("aafit_nusedlines", &nusedlines_aafit);
  outTree->Branch("aafit_nusedhits", &nusedhits_aafit);
  outTree->Branch("aafit_totalamp", &aafit_totalamp);
  outTree->Branch("aafit_zmin", &aafit_zmin);
  outTree->Branch("aafit_zmax", &aafit_zmax);
  outTree->Branch("energy_aafit_dEdX_CEA", &energy_aafit_dEdX_CEA);
  outTree->Branch("energy_aafit_ANN_ECAP", &energy_aafit_ANN_ECAP);
  outTree->Branch("aafit_tracklength", &aafit_tracklength);
  outTree->Branch("aafit_nhits", &aafit_nhits);
  outTree->Branch("aafit_flag", &aafit_flag);

  outTree->Branch("bbfit_quality", &bbfit_quality);
  outTree->Branch("bbfit_bjy", &bbfit_bjy);
  outTree->Branch("bbfit_pos_x", &bbfit_pos_x);
  outTree->Branch("bbfit_pos_y", &bbfit_pos_y);
  outTree->Branch("bbfit_pos_z", &bbfit_pos_z);
  outTree->Branch("bbfit_angerr_deg", &angerrordeg_bbfit);
  outTree->Branch("bbfit_zenith_deg", &zenithdeg_bbfit);
  outTree->Branch("bbfit_cos_zenith", &cos_zenith_bbfit);
  outTree->Branch("bbfit_azimuth_deg", &azimuthdeg_bbfit);
  outTree->Branch("bbfit_nusedlines", &nusedlines_bbfit);
  outTree->Branch("bbfit_nusedhits", &nusedhits_bbfit);
  outTree->Branch("bbfit_totalamp", &bbfit_totalamp);
  outTree->Branch("bbfit_zmin", &bbfit_zmin);
  outTree->Branch("bbfit_zmax", &bbfit_zmax);
  outTree->Branch("bbfit_nhits", &bbfit_nhits);
  outTree->Branch("bbfit_flag", &bbfit_flag);

  outTree->Branch("gridfit_quality", &gridfit_quality);
  outTree->Branch("gridfit_bjy", &gridfit_bjy);
  outTree->Branch("gridfit_pos_x", &gridfit_pos_x);
  outTree->Branch("gridfit_pos_y", &gridfit_pos_y);
  outTree->Branch("gridfit_pos_z", &gridfit_pos_z);
  outTree->Branch("gridfit_angerr_deg", &angerrordeg_gridfit);
  outTree->Branch("gridfit_zenith_deg", &zenithdeg_gridfit);
  outTree->Branch("gridfit_cos_zenith", &cos_zenith_gridfit);
  outTree->Branch("gridfit_azimuth_deg", &azimuthdeg_gridfit);
  outTree->Branch("gridfit_nusedlines", &nusedlines_gridfit);
  outTree->Branch("gridfit_nusedhits", &nusedhits_gridfit);
  outTree->Branch("gridfit_totalamp", &gridfit_totalamp);
  outTree->Branch("gridfit_zmin", &gridfit_zmin);
  outTree->Branch("gridfit_zmax", &gridfit_zmax);
  outTree->Branch("gridfit_nhits", &gridfit_nhits);
  outTree->Branch("gridfit_flag", &gridfit_flag);

  outTree->Branch("bbfit_shower_quality", &bbfit_shower_quality);
  outTree->Branch("bbfit_shower_bjy", &bbfit_shower_bjy);
  outTree->Branch("bbfit_shower_pos_x", &bbfit_shower_pos_x);
  outTree->Branch("bbfit_shower_pos_y", &bbfit_shower_pos_y);
  outTree->Branch("bbfit_shower_pos_z", &bbfit_shower_pos_z);
  outTree->Branch("bbfit_shower_angerr_deg", &angerrordeg_bbfit);
  outTree->Branch("bbfit_shower_zenith_deg", &zenithdeg_bbfit);
  outTree->Branch("bbfit_shower_cos_zenith", &cos_zenith_bbfit);
  outTree->Branch("bbfit_shower_azimuth_deg", &azimuthdeg_bbfit);
  outTree->Branch("bbfit_shower_nusedlines", &nusedlines_bbfit);
  outTree->Branch("bbfit_shower_nusedhits", &nusedhits_bbfit);
  outTree->Branch("bbfit_shower_totalamp", &bbfit_shower_totalamp);
  outTree->Branch("bbfit_shower_zmin", &bbfit_shower_zmin);
  outTree->Branch("bbfit_shower_zmax", &bbfit_shower_zmax);
  outTree->Branch("bbfit_shower_nhits", &bbfit_shower_nhits);
  outTree->Branch("bbfit_shower_flag", &bbfit_shower_flag);  

  outTree->Branch("showerdusj_quality", &showerdusj_quality);
  outTree->Branch("showerdusj_bjy", &showerdusj_bjy);
  outTree->Branch("showerdusj_pos_x", &showerdusj_pos_x);
  outTree->Branch("showerdusj_pos_y", &showerdusj_pos_y);
  outTree->Branch("showerdusj_pos_z", &showerdusj_pos_z);
  outTree->Branch("showerdusj_angerr_deg", &angerrordeg_showerdusj);
  outTree->Branch("showerdusj_zenith_deg", &zenithdeg_showerdusj);
  outTree->Branch("showerdusj_cos_zenith", &cos_zenith_showerdusj);
  outTree->Branch("showerdusj_azimuth_deg", &azimuthdeg_showerdusj);
  outTree->Branch("showerdusj_nusedlines", &nusedlines_showerdusj);
  outTree->Branch("showerdusj_nusedhits", &nusedhits_showerdusj);
  outTree->Branch("showerdusj_totalamp", &showerdusj_totalamp);
  outTree->Branch("showerdusj_energy", &showerdusj_energy);
  outTree->Branch("showerdusj_nhits", &showerdusj_nhits);
  outTree->Branch("showerdusj_radius", &showerdusj_radius);
  outTree->Branch("showerdusj_height", &showerdusj_height);
  outTree->Branch("showerdusj_flag", &showerdusj_flag);

  outTree->Branch("showertantra_quality", &showertantra_quality);
  outTree->Branch("showertantra_bjy", &showertantra_bjy);
  outTree->Branch("showertantra_pos_x", &showertantra_pos_x);
  outTree->Branch("showertantra_pos_y", &showertantra_pos_y);
  outTree->Branch("showertantra_pos_z", &showertantra_pos_z);
  outTree->Branch("showertantra_angerr_deg", &angerrordeg_showertantra);
  outTree->Branch("showertantra_zenith_deg", &zenithdeg_showertantra);
  outTree->Branch("showertantra_cos_zenith", &cos_zenith_showertantra);
  outTree->Branch("showertantra_azimuth_deg", &azimuthdeg_showertantra);
  outTree->Branch("showertantra_nusedlines", &nusedlines_showertantra);
  outTree->Branch("showertantra_nusedhits", &nusedhits_showertantra);
  outTree->Branch("showertantra_totalamp", &showertantra_totalamp);
  outTree->Branch("showertantra_energy", &showertantra_energy);
  outTree->Branch("showertantra_radius", &showertantra_radius);
  outTree->Branch("showertantra_height", &showertantra_height);
  outTree->Branch("showertantra_nhits", &showertantra_nhits);
  outTree->Branch("showertantra_flag", &showertantra_flag);

  int currentRun = -1, previousRun = -1;
  bool isSameRun = false; 
  bool IsMC = false;
  if (dataFile->IsMC()) IsMC = true;

  // Set counter for number of reconstructed events per algorithm
  vector<int> counters(6,0);

  // loop over events
  for (unsigned int i = 0; i < ntot; i++)
  {
    if (!dataFile->ReadEvent(i) == AntDSTFile::eSuccess) continue;

    // Reset flag status
    aafit_flag = false, bbfit_flag = false, gridfit_flag = false, showerdusj_flag = false, showertantra_flag = false, bbfit_shower_flag = false;

    previousRun = currentRun;
    currentRun = (int)theAntDST->GetRunId();
    
    isSameRun = (currentRun == previousRun);

    if(!isSameRun) // get dq quality
    {
      if(dataFile ->HasDataQuality(currentRun))
      {
        cout << "\nThere is data quality for run "<<currentRun<<"!"<<endl;
	      
	      dataFile->GetDataQuality((uint) currentRun, currentDataQuality);
	      runDuration = currentDataQuality.GetRunDuration();
	      runDurationSeconds = (currentDataQuality.GetFrameTime() * currentDataQuality.GetNSlices() / 1e3);
        RunDurationYear = runDurationSeconds * sec_to_year;
	      
        baseline_rate = currentDataQuality.GetBaseline();
        burst_fraction = currentDataQuality.GetBurstFraction();

        ratio_active = currentDataQuality.GetNActiveOM() / currentDataQuality.GetNTheoOM(); 
	      run_quality = currentDataQuality.GetQualityBasic();
	      scan = currentDataQuality.GetScanFlag();
      }
      else
       {
	      cout << "There is data NO quality for run "<<currentRun<<"!"<<endl;
	      runDurationSeconds = -1;
	      run_quality = -1;
	      scan = -1;
	    }
	  cout << "Data quality info: " << currentRun << ' ' << RunDurationYear << ' ' << run_quality << ' ' << scan  << "\n" << endl;
    }

    // apply quality / selection cuts
    if (AcceptEventSoft(*theAntDST, counters))
    {
      nsel++;

      // get time of the event
      const TTimeStamp time = theAntDST->GetEventTime();

      // get event info
      dataFile->GetAntEventInfo(i, &evtInfo);

      run_id = evtInfo.GetRunId();
      event_id = theAntDST->GetEventId();
      trigg_counter = theAntDST->GetTriggerCounter();
      frame = theAntDST->GetFrameIndex();
      MJD = theAntDST->GetMJD();
      Date = theAntDST->GetYYMMDD();

      if (IsMC)
      {
        MCEvent MCevt = theAntDST->GetMCEvent();
        dataFile->GetAntEventInfo(i, &evtInfo);

        run_id = evtInfo.GetRunId();
        type = MCevt.GetPrimaryParticle().GetParticleType();
        energy_true = MCevt.GetPrimaryParticle().GetEnergy(); // GeV

        if (isMupage) w_muon = muon_weight(run_id, m);
        else w_muon = 0;

        w2= MCevt.GetWeight_W2();
        w3= MCevt.GetWeight_W3();
        w_honda= MCevt.GetWeight_Honda();
        ngen = MCevt.GetNGenEvents();

        // Data/MC ratio correction
        DataMCRatio = correctForLivetimeRatio(type, interaction_type, run_id);

        if (type == -13) is_neutrino = false;

        TVector3 pos = MCevt.GetPrimaryParticle().GetPosition();
        pos_x_true = pos.X();
        pos_y_true = pos.Y();
        pos_z_true = pos.Z();
        bjorken_y_true = 0.5;
        cos_zenith_true = TMath::Cos(MCevt.GetPrimaryParticle().GetZenith());
        azimuthdeg_true = MCevt.GetPrimaryParticle().GetAzimuth();
      }
        // triggers
        vector<ETriggerType> vectTriggers = theAntDST->GetTriggerType();

        t3N_active = false;
	      tT2_active = false;
	      tT3_active = false;
	      tTQ_active = false;
	  
     	  for(unsigned int i=0;i<vectTriggers.size();i++){
    	    if(vectTriggers[i]==e3D_SCAN) t3N_active = true;
    	    if(vectTriggers[i]==eT2)      tT2_active = true;
    	    if(vectTriggers[i]==eT3)      tT3_active = true;
          if(vectTriggers[i]==eTQ)      tTQ_active = true;
        }

      // reconstruction - AAFit
      if (theAntDST->HasStrategy(eAAFit) &&
          theAntDST->GetStrategy(eAAFit).HasRecParticle(eMuon))
      {
        RecEvent &aafit = theAntDST->GetStrategy(eAAFit);
        RecParticle aafit_muon = aafit.GetRecParticle(eMuon);

        aafit_lambda = aafit.GetRecQuality();
        angerrordeg_aafit = aafit_muon.GetAngularError() * TMath::RadToDeg();
        zenithdeg_aafit = aafit_muon.GetZenith() * TMath::RadToDeg();
        cos_zenith_aafit = TMath::Cos(aafit_muon.GetZenith());
        azimuthdeg_aafit = aafit_muon.GetAzimuth() * TMath::RadToDeg();
        nusedlines_aafit = aafit_muon.GetNUsedLines();
        nusedhits_aafit = aafit_muon.GetNUsedHits();
        aafit_totalamp = aafit_muon.GetTotalUsedAmplitude();
        aafit_zmin = aafit_muon.GetZmin();
        aafit_zmax = aafit_muon.GetZmax();
        energy_aafit_dEdX_CEA = aafit_muon.GetEnergy(edEdX_CEA);
        energy_aafit_ANN_ECAP = aafit_muon.GetEnergy(eANN_ECAP);
        aafit_tracklength = aafit_muon.GetAntEnergy(edEdX_CEA).GetParameter("TrackLength");
        aafit_nhits = aafit.GetNTriggeredHits();

        TVector3 pos = aafit_muon.GetPosition();
        aafit_pos_x = pos.X();
        aafit_pos_y = pos.Y();
        aafit_pos_z = pos.Z();

        aafit_flag = true;
      }

      // reconstruction - BBFit
      if (theAntDST->HasStrategy(eBBFit) &&
          theAntDST->GetStrategy(eBBFit).HasRecParticle(eMuon))
      {
        RecEvent &bbfit = theAntDST->GetStrategy(eBBFit);
        RecParticle bbfit_muon = bbfit.GetRecParticle(eMuon);
        bbfit_quality = bbfit.GetRecQuality();
        angerrordeg_bbfit = bbfit_muon.GetAngularError() * TMath::RadToDeg();
        zenithdeg_bbfit = bbfit_muon.GetZenith() * TMath::RadToDeg();
        cos_zenith_bbfit = TMath::Cos(bbfit_muon.GetZenith());
        azimuthdeg_bbfit = bbfit_muon.GetAzimuth() * TMath::RadToDeg();
        nusedlines_bbfit = bbfit_muon.GetNUsedLines();
        nusedhits_bbfit = bbfit_muon.GetNUsedHits();
        bbfit_totalamp = bbfit_muon.GetTotalUsedAmplitude();
        bbfit_zmin = bbfit_muon.GetZmin();
        bbfit_zmax = bbfit_muon.GetZmax();
        bbfit_nhits = bbfit.GetNTriggeredHits();

        TVector3 pos = bbfit_muon.GetPosition();
        bbfit_pos_x = pos.X();
        bbfit_pos_y = pos.Y();
        bbfit_pos_z = pos.Z();

        bbfit_flag = true;
      }

      // reconstruction - Gridfit
      if (theAntDST->HasStrategy(eGridFit) &&
          theAntDST->GetStrategy(eGridFit).HasRecParticle(eMuon))
      {
        RecEvent &gridfit = theAntDST->GetStrategy(eGridFit);
        RecParticle gridfit_muon = gridfit.GetRecParticle(eMuon);
        gridfit_quality = gridfit.GetRecQuality();
        angerrordeg_gridfit = gridfit_muon.GetAngularError() * TMath::RadToDeg();
        zenithdeg_gridfit = gridfit_muon.GetZenith() * TMath::RadToDeg();
        cos_zenith_gridfit = TMath::Cos(gridfit_muon.GetZenith());
        azimuthdeg_gridfit = gridfit_muon.GetAzimuth() * TMath::RadToDeg();
        nusedlines_gridfit = gridfit_muon.GetNUsedLines();
        nusedhits_gridfit = gridfit_muon.GetNUsedHits();
        gridfit_zmin = gridfit_muon.GetZmin();
        gridfit_zmax = gridfit_muon.GetZmax();
        gridfit_nhits = gridfit.GetNTriggeredHits();

        TVector3 pos = gridfit_muon.GetPosition();
        gridfit_pos_x = pos.X();
        gridfit_pos_y = pos.Y();
        gridfit_pos_z = pos.Z();

        gridfit_flag = true;
      }

      // reconstruction - BBFit_shower
      if (theAntDST->HasStrategy(eBBFitBrightPoint) &&
          theAntDST->GetStrategy(eBBFitBrightPoint).HasRecParticle(eBrightPoint))
      {
        RecEvent &bbfit_shower = theAntDST->GetStrategy(eBBFitBrightPoint);
        RecParticle bbfit_elec = bbfit_shower.GetRecParticle(eBrightPoint);
        bbfit_shower_quality = bbfit_shower.GetRecQuality();
        angerrordeg_bbfit_shower = bbfit_elec.GetAngularError() * TMath::RadToDeg();
        zenithdeg_bbfit_shower = bbfit_elec.GetZenith() * TMath::RadToDeg();
        cos_zenith_bbfit_shower = TMath::Cos(bbfit_elec.GetZenith());
        azimuthdeg_bbfit_shower = bbfit_elec.GetAzimuth() * TMath::RadToDeg();
        nusedlines_bbfit_shower = bbfit_elec.GetNUsedLines();
        nusedhits_bbfit_shower = bbfit_elec.GetNUsedHits();
        bbfit_shower_totalamp = bbfit_elec.GetTotalUsedAmplitude();
        bbfit_shower_zmin = bbfit_elec.GetZmin();
        bbfit_shower_zmax = bbfit_elec.GetZmax();
        bbfit_shower_nhits = bbfit_shower.GetNTriggeredHits();

        TVector3 pos = bbfit_elec.GetPosition();
        bbfit_shower_pos_x = pos.X();
        bbfit_shower_pos_y = pos.Y();
        bbfit_shower_pos_z = pos.Z();

        bbfit_shower_flag = true;
      }

      // reconstruction - ShowerDusj
      if (theAntDST->HasStrategy(eShowerDusjFit) &&
          theAntDST->GetStrategy(eShowerDusjFit).HasRecParticle(eBrightPoint))
      {
        RecEvent &showerdusj = theAntDST->GetStrategy(eShowerDusjFit);
        RecParticle showerdusj_elec = showerdusj.GetRecParticle(eBrightPoint);
        showerdusj_quality = showerdusj.GetRecQuality();
        angerrordeg_showerdusj = showerdusj_elec.GetAngularError() * TMath::RadToDeg();
        zenithdeg_showerdusj = showerdusj_elec.GetZenith() * TMath::RadToDeg();
        cos_zenith_showerdusj = TMath::Cos(showerdusj_elec.GetZenith());
        azimuthdeg_showerdusj = showerdusj_elec.GetAzimuth() * TMath::RadToDeg();
        nusedlines_showerdusj = showerdusj_elec.GetNUsedLines();
        nusedhits_showerdusj = showerdusj_elec.GetNUsedHits();
        showerdusj_energy = showerdusj_elec.GetEnergy(eDusjShowerEnergy);
        showerdusj_nhits = showerdusj.GetNTriggeredHits();

        TVector3 pos = showerdusj_elec.GetPosition();
        showerdusj_pos_x = pos.X();
        showerdusj_pos_y = pos.Y();
        showerdusj_pos_z = pos.Z();

        showerdusj_radius = sqrt((showerdusj_pos_x - detcenter_x) * (showerdusj_pos_x - detcenter_x) + (showerdusj_pos_y - detcenter_y) * (showerdusj_pos_y - detcenter_y));
        showerdusj_height = showerdusj_pos_z - detcenter_z;

        showerdusj_flag = true;
      }

      // reconstruction - ShowerTantraFit
      if (theAntDST->HasStrategy(eShowerTantraFit) &&
          theAntDST->GetStrategy(eShowerTantraFit).HasRecParticle(eBrightPoint))
      {
        RecEvent &showertantra = theAntDST->GetStrategy(eShowerTantraFit);
        RecParticle showertantra_elec = showertantra.GetRecParticle(eBrightPoint);
        showertantra_quality = showertantra.GetRecQuality();
        angerrordeg_showertantra = showertantra_elec.GetAngularError() * TMath::RadToDeg();
        zenithdeg_showertantra = showertantra_elec.GetZenith() * TMath::RadToDeg();
        cos_zenith_showertantra = TMath::Cos(showertantra_elec.GetZenith());
        azimuthdeg_showertantra = showertantra_elec.GetAzimuth() * TMath::RadToDeg();
        nusedlines_showertantra = showertantra_elec.GetNUsedLines();
        nusedhits_showertantra = showertantra_elec.GetNUsedHits();
        showertantra_energy = showertantra_elec.GetEnergy(eTantraShowerEnergy);
        showertantra_nhits = showertantra.GetNTriggeredHits();

        TVector3 pos = showertantra_elec.GetPosition();
        showertantra_pos_x = pos.X();
        showertantra_pos_y = pos.Y();
        showertantra_pos_z = pos.Z();

        showertantra_radius = sqrt((showertantra_pos_x - detcenter_x) * (showertantra_pos_x - detcenter_x) + (showertantra_pos_y - detcenter_y) * (showertantra_pos_y - detcenter_y));
        showertantra_height = showertantra_pos_z - detcenter_z;

        showertantra_flag = true;
      }

      if (type != 0 || IsMC == false) outTree->Fill();

    } // end selection

    const int currentEventNumber = dataFile->GetCurrentEvent();
    if (currentEventNumber && currentEventNumber % fivePercent == 0)
      cout << " " << currentEventNumber << " out of " << ntot << " events processed" << endl;
  }

  outFile->cd();
  outFile->Write();
  outFile->Close();

  cout << "\n total number of events: " << ntot << "\n"
       << " total selected events: " << nsel  << "\n"
       << " events reconstructed by AAfit: " << counters[0]  << "\n"
       << " events reconstructed by ShowerTantra: " << counters[1] << "\n"
       << " events reconstructed by BBfit: " << counters[2] << "\n"
       << " events reconstructed by ShowerDusj: " << counters[3] << "\n"
       << " events reconstructed by Gridfit: " << counters[4] << "\n"
       << " events reconstructed by BBfitShower: " << counters[5] << "\n"
       << endl;
}
