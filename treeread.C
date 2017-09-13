#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>

#include <TFile.h>
#include "TSystem.h"
#include <TTree.h>
#include <inttypes.h>
#include "TH1F.h"
#include "TH1I.h"

//tree variables
int channel;
ULong64_t clock_time;
double delta_t;   
double time_stamp;
double slope_x;
double slope_y;
double intersect_x;
double intersect_y;
double chi2;
ULong64_t frame_clock_time;
UChar_t time0;
int tdiff01;
int tdiff02;
int tdiff03;
UChar_t X0;
UChar_t X1;
UChar_t X2;
UChar_t X3;
UChar_t Y0;
UChar_t Y1;
UChar_t Y2;
UChar_t Y3;

//global vars
const char* path="/home/frederik/data/Hindenburg";
const char* out_name="ProcessedTree.root";
const char* dataset_name="dataset.dat";
const char* filename_root = "out_";

//Histograms
TH1F* hTrigger4HitsTimeDiff;
TH1I* hMultiplicity;

void Print_Track()
{
  std::cout << " ******* Track ******** " << std::endl;
  std::cout << " time_stamp: " << time_stamp << std::endl;
  std::cout << " slope_x: " << slope_x << std::endl;
  std::cout << " slope_y: " << slope_y << std::endl;
  std::cout << " intersect_x: " << intersect_x << std::endl;
  std::cout << " intersect_y: " << intersect_y << std::endl;
  std::cout << " chi2: " << chi2 << std::endl << std::endl;
}

void Init_histos()
{
	hTrigger4HitsTimeDiff = new TH1F("hTrigger4HitsTimeDiff","Time difference between a TPC trigger and a 4 hit track in the pixel telescope; Time difference (ns)",2000 ,-100000,100000); 
	/*hStats = new TH1I("hStats","Module statistics",10,0.5,10.5);
	hStats->GetXaxis()->SetBinLabel(1,"Trigger-Tracks multi 1");
	hStats->GetXaxis()->SetBinLabel(2,"Trigger-Tracks multi 2");
	hStats->GetXaxis()->SetBinLabel(3,"Trigger-Tracks multi 3");
	hStats->GetXaxis()->SetBinLabel(4,"Trigger-Tracks multi 4");
	hStats->GetXaxis()->SetBinLabel(5,"Trigger-Tracks multi 4+");*/
	hMultiplicity=new TH1I("hMultiplicity"," trigger - track multiplicity",15,0.5,15.5);
}

void Init()
{
	Init_histos();
}

void Process_file(int run_nr)
{
	return;
  //input file
  char filename[256];
  sprintf(filename,"%s/out_%06d_tracks_triggers.root",path,run_nr);
  TFile *f = new TFile(filename,"read");
  TTree *t1 = (TTree*)f->Get("triggers");
  TTree *t2 = (TTree*)f->Get("tracks");
  TTree *t3 = (TTree*)f->Get("hits");

  //set branches

  t1->SetBranchAddress("channel",&channel);
  t1->SetBranchAddress("clock_time",&clock_time);
  t1->SetBranchAddress("delta_t",&delta_t);
  
  t2->SetBranchAddress("time_stamp",&time_stamp);
  t2->SetBranchAddress("slope_x",&slope_x);
  t2->SetBranchAddress("slope_y",&slope_y);
  t2->SetBranchAddress("intersect_x",&intersect_x);
  t2->SetBranchAddress("intersect_y",&intersect_y);
  t2->SetBranchAddress("chi2",&chi2);
  
  t3->SetBranchAddress("frame_clock_time",&frame_clock_time);
  t3->SetBranchAddress("time0",&time0);
  t3->SetBranchAddress("tdiff01",&tdiff01);
  t3->SetBranchAddress("tdiff02",&tdiff02);
  t3->SetBranchAddress("tdiff03",&tdiff03);
  t3->SetBranchAddress("X0",&X0);
  t3->SetBranchAddress("X1",&X1);
  t3->SetBranchAddress("X2",&X2);
  t3->SetBranchAddress("X3",&X3);
  t3->SetBranchAddress("Y0",&Y0);
  t3->SetBranchAddress("Y1",&Y1);
  t3->SetBranchAddress("Y2",&Y2);
  t3->SetBranchAddress("Y3",&Y3);
  //

  //read all entries and fill the histograms
  Long64_t nentries1 = t1->GetEntries();
  Long64_t nentries2 = t2->GetEntries();
  Long64_t nentries3 = t3->GetEntries();

  
  Long64_t iLow = 0;
  Long64_t tLow = -50000; // look 100 us in the past
  Long64_t tHigh = 50000; //look 100 us ahead
  
  for (Long64_t iTrigger = 0; iTrigger < nentries1 ; iTrigger++) 
  {
	unsigned int multiplicity = 0;
    t1->GetEntry(iTrigger);
    if(channel == 4)
    {
      ULong64_t trigger_time_clk = clock_time;
      for(Long64_t iHit = iLow; iHit < nentries3; iHit++)
      {
		t3->GetEntry(iHit);
        Long64_t tDiff =  Long64_t(trigger_time_clk) - Long64_t(frame_clock_time);
        if( tDiff > tHigh) continue; //trigger to far ahead
        if( tDiff < tLow) { iLow = iHit; break; }//hits ahead of this trigger
        //in ROI
        double tDiff_fine = 2.0*tDiff - time0*16;
        hTrigger4HitsTimeDiff->Fill(tDiff_fine);
        multiplicity++;
	  }
	}
	hMultiplicity->Fill(multiplicity);
     
  }

         

  t1->GetEntry(0);
  std::cout << "first trigger time : " << clock_time*2.0 << std::endl;
  t1->GetEntry(nentries1-1);
  std::cout << "last trigger time : " << clock_time*2.0 << std::endl;
  t3->GetEntry(0);
  std::cout << "first 4 hits time : " << frame_clock_time*2.0+time0*16.0 << std::endl;
  t3->GetEntry(nentries3-1);
  std::cout << "last 4 hits time : " << frame_clock_time*2.0+time0*16.0 << std::endl;
   
  f->Close();
  // in the browser, click on "ROOT Files", then on "tree1.root".
  //     you can click on the histogram icons in the right panel to draw them.
  // in the TreeViewer, follow the instructions in the Help button.
}

void Process(int run_nr = 11000)
{
  // *** open output file file ***
  printf("Creating output file \n");
  
  TFile *fout = new TFile ( out_name, "RECREATE");
  if ( fout->IsZombie() )
  {
    std::cerr << "**ERROR! Cannot open file [" << out_name << "]" << std::endl;
    return;
  }	  
  fout->cd();
  
  // ** init *** 
  Init();
  
  //  *** create dataset / file list ***
  printf("Creating RunList\n");
  char cmd[256];
  sprintf(cmd,"ls %s > runs.dat",path);
  gSystem->Exec(cmd);
  printf("Creating ifstream\n");
  
  // read files 
  char file_name[1024];
  ifstream *is = new ifstream("runs.dat");
  ifstream *isdataset = new ifstream(dataset_name);
  
  // read in dataset
  std::vector<int> run_numbers;
  int number;
  while ( isdataset->good() )
  {
    *isdataset >> number;
    run_numbers.push_back(number);
    printf("run_number %d\n",number);
  }
  
  int files_processed = 0;
  
  while ( is->good() )
  {
    *is >> file_name;
    if ( strncmp(file_name, filename_root, strlen(filename_root)) == 0 )
    {	
      // *** extract file number ***
      int run_nr = atoi(file_name+strlen(filename_root));
      cout << file_name << "    " << run_nr << endl;
      
      bool good = false;
      for(unsigned int i = 0; i < run_numbers.size()-1; i++)
      {
         if( run_numbers.at(i) == run_nr) {good = true; cout << run_numbers.at(i) << "  " << run_nr << endl;}    
      }

      if (good == true) {Process_file(run_nr); files_processed++;}
      else {printf("run %d not in dataset\n",run_nr);}
    }
  }
  delete is;

  printf("Files processed: %d\n", files_processed); 

  fout->Write();
  delete fout; 
}
