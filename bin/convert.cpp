/*
 * convert.cpp
 *
 *  Created on: 7 May 2019
 *      Author: jkiesele
 */

#include "TSystem.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TKey.h"
#include "TCanvas.h"
#include "TMath.h"

#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <typeinfo>
#include <iostream>


int main( int argc, char* argv[] ){

  // Check the number of parameters
  if (argc < 3) {
    // Tell the user how to run the program
    std::cerr << "Usage: " << argv[0] << " [input.root] [output.root] " << std::endl;
    return 1;
  }

  std::string inFileName = argv[1];
  std::string outFileName = argv[2];

  std::cout << "hello" <<std::endl;

  // total detector dimension
  double tot_x = 1000.;
  double tot_y = 1000.;
  int tot_l = 125;
  double tot_z = 250.;

  TFile fileIn(inFileName.c_str(),"read");
  TTree* tree = (TTree*)fileIn.Get("B4");
  double t_energy=0;
  double t_x=0;
  double t_y=0;
  double t_r=0;
  std::vector<double>* rec_energy=0;
  std::vector<double>* rec_x=0;
  std::vector<double>* rec_y=0;
  std::vector<double>* rec_layer=0;

  TBranch * b_energy =tree->GetBranch("true_energy");
  b_energy->SetAddress(&t_energy);
  TBranch * b_x =tree->GetBranch("true_x"); 
  b_x->SetAddress(&t_x);
  TBranch * b_y =tree->GetBranch("true_y");
  b_y->SetAddress(&t_y);
  TBranch * b_r =tree->GetBranch("true_r");
  b_r->SetAddress(&t_r);
  TBranch * b_rec_energy =tree->GetBranch("rechit_energy"); 
  b_rec_energy->SetAddress(&rec_energy);
  TBranch * b_rec_x =tree->GetBranch("rechit_x"); 
  b_rec_x->SetAddress(&rec_x);
  TBranch * b_rec_y =tree->GetBranch("rechit_y"); 
  b_rec_y->SetAddress(&rec_y);
  TBranch * b_rec_layer =tree->GetBranch("rechit_layer"); 
  b_rec_layer->SetAddress(&rec_layer);

  std::cout << outFileName <<"\n";
  
  TFile outfile(outFileName.c_str(),"recreate");
  TTree outtree("events","Events");
  
  std::vector<std::vector<std::vector<std::vector<float> > > > rechit(50, std::vector<std::vector<std::vector<float> > >(50, std::vector<std::vector<float> >(tot_l, std::vector<float>(4,0.) ) ) );
  double true_energy, true_x, true_y, true_r;

  outtree.Branch("true_energy", &true_energy, "true_energy/D");
  outtree.Branch("true_x",   &true_x, "true_x/D");
  outtree.Branch("true_y",   &true_y, "true_y/D");
  outtree.Branch("true_r",   &true_r, "true_r/D");
  outtree.Branch("rechit",   &rechit);

  int nevent = tree->GetEntries();
  std::cout << "Loop through " << nevent << " events. \n";

  for (Int_t ev=0;ev<nevent;ev++) {

    tree->GetEvent(ev);           //Read complete accepted event
    
    b_energy->GetEntry(ev);    
    b_x->GetEntry(ev);
    b_y->GetEntry(ev);
    b_r->GetEntry(ev);

    b_rec_energy->GetEntry(ev);
    b_rec_x->GetEntry(ev);
    b_rec_y->GetEntry(ev);
    b_rec_layer->GetEntry(ev);
    
    float E = 0.;
    if (ev<3)
      std::cout << "total numHits: " << rec_energy->size() << "\n";

    for (uint hit=0; hit<rec_energy->size(); hit++){
      float pos_x = rec_x->at(hit); // in mm
      float pos_y = rec_y->at(hit); // in mm
      float pos_layer = rec_layer->at(hit);
      float r_e = rec_energy->at(hit);
      
      int bin_x  = int(std::floor((pos_x+500)/20.));
      int bin_y  = int(std::floor((pos_y+500)/20.));
      int bin_l  = int(std::floor(pos_layer));
      if (bin_x<0)
	std::cout << "Negative bin: " << bin_x << "\n";
      if (r_e==0)
	continue;

      rechit[bin_x][bin_y][bin_l][0]+=r_e;
      rechit[bin_x][bin_y][bin_l][1]=pos_x;
      rechit[bin_x][bin_y][bin_l][2]=pos_y;
      rechit[bin_x][bin_y][bin_l][3]=pos_layer;

      if (ev==1 && hit<10){
	std::cout << "     x=" << bin_x << ", y=" << bin_y <<", layer="<< bin_l << "\n";
	std::cout << "energy=" << r_e   << ", x=" << pos_x << ", y="   << pos_y <<", layer="<< pos_layer << "\n";
      }
      E += r_e;
    }
    
    true_energy = t_energy;
    true_x = t_x;
    true_y = t_y;
    true_r= t_r;

    outtree.Fill();

    if (ev<1){
      std::cout << "total size: " << rechit.size()*rechit[1].size()*rechit[1][1].size() <<"\n";
    }
    
    // clear 4D vector  
    for(uint iX=0;iX<rechit.size();iX++){
      for(uint iY=0;iY<rechit[iX].size();iY++){
	for(uint iL=0;iL<rechit[iX][iY].size();iL++){
	  if (rechit[iX][iY][iL].size()>4)
	    std::cout << "Double filling in: x=" << iX << ", y="<< iY <<", layer="<< iL << "\n";
	  rechit[iX][iY][iL].clear();
	}
      }
    }

  }
  
  outtree.Write();
  outfile.Write();
  outfile.Close();
  
}
