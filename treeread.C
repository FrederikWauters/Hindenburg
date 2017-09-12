#include <stdlib.h>
#include <TFile.h>
#include <iostream>
#include <TTree.h>
#include <inttypes.h>

int channel;
ULong64_t clock_time;
double delta_t;
   
double time_stamp;
double slope_x;
double slope_y;
double intersect_x;
double intersect_y;
double chi2;

//double Diff = 


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


void Process()
{
   //input file
   TFile *f = new TFile("out_011500_tracks_triggers.root","read");
   TTree *t1 = (TTree*)f->Get("triggers");
   TTree *t2 = (TTree*)f->Get("tracks");

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
   
  //we do not close the file. We want to keep the generated histograms
  //we open a browser and the TreeViewer
  f->Close();
  // in the browser, click on "ROOT Files", then on "tree1.root".
  //     you can click on the histogram icons in the right panel to draw them.
  // in the TreeViewer, follow the instructions in the Help button.
}
