#include <stdlib.h>
#include <TFile.h>
#include <iostream>
#include <TTree.h>
#include <inttypes.h>

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
UChar_t tdiff01;
UChar_t tdiff02;
UChar_t tdiff03;

//global vars
char path[256]="/data/mu3e/IKAR201708/analysis";

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


void ProcessFile(int run_nr)
{
  //input file
  char filename[256];
  sprintf(filename,"%s/out_%d_tracks_triggers.root",path,run_nr);
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
  //

  //read all entries and fill the histograms
  Long64_t nentries1 = t1->GetEntries();
  Long64_t nentries2 = t2->GetEntries();


  std::vector<double> trigger_times;
   
  for (Long64_t i=0;i<nentries1;i++) 
  {
    t1->GetEntry(i);
    if(channel == 4) trigger_times.push_back(2.0*clock_time);
  }

         
  unsigned int first = 0;
  for (Long64_t i_track=0;i_track<nentries2;i_track++) 
  {
    t2->GetEntry(i_track);
    for(Long64_t i_trigger = first; i_trigger < trigger_times.size(); i_trigger++)
    {
    }
  }

  t1->GetEntry(0);
  std::cout << "first trigger time : " << clock_time << std::endl;
  t1->GetEntry(nentries1-1);
  std::cout << "last trigger time : " << clock_time << std::endl;
  t2->GetEntry(0);
  std::cout << "first track time : " << time_stamp << std::endl;
  t2->GetEntry(nentries2-1);
  std::cout << "last track time : " << time_stamp << std::endl;
   
  f->Close();
  // in the browser, click on "ROOT Files", then on "tree1.root".
  //     you can click on the histogram icons in the right panel to draw them.
  // in the TreeViewer, follow the instructions in the Help button.
}

void Process()
{
	int run_nr = 011500;
	ProcessFile(run_nr);
	
}
