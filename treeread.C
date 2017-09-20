#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <algorithm>


#include <TFile.h>
#include "TSystem.h"
#include <TTree.h>
#include <inttypes.h>
#include "TH1F.h"
#include "TH1I.h"

//tree variables
int channel;
ULong64_t clock_time;
ULong64_t frame_time;
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
TH1F* hTrackTimes;
TH1F* hTriggerTimes;

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

int64_t GetDifference(uint64_t first, uint64_t second) {
    uint64_t abs_diff = (first > second) ? (first - second): (second - first);
    assert(abs_diff<=INT64_MAX);
    return (first > second) ? (int64_t)abs_diff : -(int64_t)abs_diff;
}


void Init_histos()
{
	hTrigger4HitsTimeDiff = new TH1F("hTrigger4HitsTimeDiff","Time difference between a TPC trigger and a 4 hit track in the pixel telescope; Time difference (ns)",2000 ,-500000,500000); 
	/*hStats = new TH1I("hStats","Module statistics",10,0.5,10.5);
	hStats->GetXaxis()->SetBinLabel(1,"Trigger-Tracks multi 1");
	hStats->GetXaxis()->SetBinLabel(2,"Trigger-Tracks multi 2");
	hStats->GetXaxis()->SetBinLabel(3,"Trigger-Tracks multi 3");
	hStats->GetXaxis()->SetBinLabel(4,"Trigger-Tracks multi 4");
	hStats->GetXaxis()->SetBinLabel(5,"Trigger-Tracks multi 4+");*/
	hMultiplicity=new TH1I("hMultiplicity"," trigger - track multiplicity",15,0.5,15.5);
	
	double max = 80000000000;
	double min = 60000000000;
	hTrackTimes = new TH1F("hTrackTimes","track times; time (ns)", 1000000,min,max);
	hTriggerTimes = new TH1F("hTriggerTimes","trigger times; time (ns)", 1000000,min,max);
}

void Init()
{
	Init_histos();
}

void Process_file(int run_nr)
{
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
  
  t2->SetBranchAddress("frame_time",&frame_time);
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
  
  std::cout << "nentries1 : " << nentries1 << std::endl;
  std::cout << "nentries2 : " << nentries2 << std::endl;
  std::cout << "nentries3 : " << nentries3 << std::endl;

  //check some basics
  t1->GetEntry(0);
  std::cout << "first trigger time : " << clock_time*2.0 << std::endl;
  t1->GetEntry(nentries1-1);
  std::cout << "last trigger time : " << clock_time*2.0 << std::endl;
  t3->GetEntry(0);
  std::cout << "first 4 hits time : " << frame_clock_time*2.0+time0*16.0 << std::endl;
  t3->GetEntry(nentries3-1);
  std::cout << "last 4 hits time : " << frame_clock_time*2.0+time0*16.0 << std::endl;
   
  std::vector<double> trigger_times;

  for (Long64_t iTrigger = 0; iTrigger < nentries1 ; iTrigger++) 
  {
    t1->GetEntry(iTrigger);
    double time = 2.0*double(clock_time);
    if(channel == 4)
    {
      hTriggerTimes->Fill(time);
      trigger_times.push_back(time);
    }
  }
  
  std::sort(trigger_times.begin(),trigger_times.end());
  std::cout << " number of TPC triggers found " << trigger_times.size();
  if(trigger_times.size()>0)
  {  
    std::cout << " First t " << trigger_times.at(0) << " Last t " <<  trigger_times.at(trigger_times.size()-1) << std::endl;
  }
  
  Long64_t iLow = 0;
  double tLow = -200000; // look 150 us in the past
  double tHigh = 200000; //look 150 us ahead
  

  
  for (Long64_t iTrack = 0; iTrack < nentries2 ; iTrack++) 
  {
    t2->GetEntry(iTrack);
    double time = frame_time*2.0 + 16*time_stamp;
    hTrackTimes->Fill(time);
    for (unsigned int iTrigger = iLow; iTrigger < trigger_times.size() ; iTrigger++) 
    {
      double tDiff = trigger_times.at(iTrigger) - time; // positive for TPC trigger after track, negative for before
      //std::cout << "tDiff " << tDiff << std::endl;
      if(tDiff < tLow ) { iLow = iTrigger; continue; }
      if(tDiff > tHigh ) break;
      
      if(fabs(tDiff) < 200000 )
      {
	//std::cout << "tDiff " << tDiff << endl; 
	hTrigger4HitsTimeDiff->Fill(tDiff);
	//std::cout << "tDiff " << tDiff << std::endl;	
      }
    }
  }
  //    hMultiplicity->Fill(multiplicity);    
         


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
      std::cout << file_name << "    " << run_nr << std::endl;
      
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
