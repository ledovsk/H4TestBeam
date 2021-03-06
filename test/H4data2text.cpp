#include <string>
#include "TTree.h"
#include "TFile.h"

#include <iostream>
#include <fstream>


//---- from TBtree Shashlik ----
#include "include/TBEvent.h"

//---- boost
// #include "boost/program_options.hpp"
// #include "boost/program_options/options_description.hpp"

// namespace po = boost::program_options;


int main(int argc, char**argv){

 std::string input_file;
 std::string output_file;

 //---- configuration
 
 //---- configuration
 
 int c;
 while ((c = getopt (argc, argv, "i:o:m:")) != -1)
  switch (c)
  {
   case 'i':
    input_file = string(optarg);
    break;
   case 'o':
    output_file =  string(optarg);
    break;
    
   case '?':
    if (optopt == 'i'|| optopt == 'o')
     fprintf (stderr, "Option -%c requires an argument.\n", optopt);
    else if (isprint (optopt))
     fprintf (stderr, "Unknown option `-%c'.\n", optopt);
    else
     fprintf (stderr,
              "Unknown option character `\\x%x'.\n",
              optopt);
     return 1;
   default:
    exit (-1);
  }
  
  
  
  
//  // Declare the supported options.
//  po::options_description desc("Allowed options");
//  desc.add_options()
//     ("help", "produce help message")
//     ("input_file",  po::value<std::string>(), "input file")
//     ("output_file", po::value<std::string>(), "output file")
//  ;
//  
//  po::variables_map vm;
//  po::store(po::parse_command_line(argc, argv, desc), vm);
//  po::notify(vm);
//  
//  if (vm.count("help")) {
//   std::cout << desc << "\n";
//   return 1;
//  }
//  
//  if (vm.count("input_file")) {
//   input_file = vm["input_file"].as<std::string>();
//  } else {
//   std::cout << "defult?.\n";
//  }
//  
//  if (vm.count("output_file")) {
//   output_file = vm["output_file"].as<std::string>();
//  } else {
//   std::cout << "defult?.\n";
//  }
 
 
 //---- configuration (end)
 
 //--- it works, but if I activate I need to link to libTB library, and to keep it simple I don't
 TBSpill tbspill;
 TBEvent tbevent;
 
 
 //---- read file
 TFile* fileIn = new TFile (input_file.c_str(), "READ");
 TTree* H4tree = (TTree*) fileIn->Get("H4tree");
 
 
//  int nEntries = H4tree->Draw("evtTime:evtNumber:evtTimeDist","","para goff");
 int nEntries = H4tree->Draw("runNumber:spillNumber:evtNumber:evtTimeDist","","para goff");
 Double_t *v_runNumber = H4tree->GetV1();
 Double_t *v_spillNumber = H4tree->GetV2();
 Double_t *v_evtNumber = H4tree->GetV3();
 
 std::cout << " nEntries = " << nEntries << std::endl;
 for (int i=0; i<nEntries; i++) {
  std::cout << " v_runNumber  [" << i << "] = " << v_runNumber[i] << " --- ";
  std::cout << " v_spillNumber[" << i << "] = " << v_spillNumber[i] << " --- ";
  std::cout << " v_evtNumber  [" << i << "] = " << v_evtNumber[i] << std::endl;
 }
 
  
 ofstream myfile;
 myfile.open (output_file);
 myfile << "Writing this to a file.\n";
 for (int i=0; i<nEntries; i++) {
  myfile << " v_runNumber  [" << i << "] = " << v_runNumber[i] << " --- ";
  myfile << " v_spillNumber[" << i << "] = " << v_spillNumber[i] << " --- ";
  myfile << " v_evtNumber  [" << i << "] = " << v_evtNumber[i] << std::endl;
 }
 myfile.close(); 
 
 
 TFile* output_file_root = new TFile ("out.root","RECREATE");
 TTree* outtree = new TTree("t1041","t1041");
 outtree->Branch("tbspill", "TBSpill", &tbspill, 64000, 0);
 outtree->Branch("tbevent", "TBEvent", &tbevent, 64000, 0);
 
 for (int i=0; i<nEntries; i++) {
  tbspill.SetSpillData(v_spillNumber[i], 0, 0, 0);
  outtree->Fill();
 }
 outtree->Write();
 output_file_root->Close();
 
 
}



