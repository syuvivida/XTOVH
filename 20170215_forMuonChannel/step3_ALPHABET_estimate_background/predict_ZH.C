#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <deque>
#include <list>

#include <TPaveStats.h>
#include <TH1D.h>
#include <TFile.h>
#include <TF1.h>
#include <TPad.h>
#include <TFile.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TTree.h>
#include "TObject.h"
#include "TColor.h"
#include "TFile.h"
#include "TMarker.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TString.h"
#include "TMath.h"


void predict_ZH( TH1F* &h_ZH_mass_pass,    
		 TH1F* &h_ZH_mass_fail,
 		 TH1F* &h_ZH_mass_pass_predict,
		 TF1*   fn_1,
                 std::string sample_name
	       ){


  // open TTree to fill histogram

//  std::string sample_path = "/Users/index0192/alphabet_plot/skimTTree_sample/output_ele_8_0_20puppi_remove_HLT_Ele105/SingleElectron-Run2016_combine_BCDEFG.root";
  std::string sample_path ="output_mu/" + sample_name + ".root";

  bool isData = false; // false for MC
  if( sample_path.find("SingleElectron") != std::string::npos ) {cout<<"find data" << endl; isData = true;}// true for Data

  TFile* file   = TFile::Open( sample_path.c_str() );
  TTree* tree = (TTree*) file->Get("new_tree");

  double Event_Weight;
  double Jet_SDmass_corrected;
  double Double_bTagger;
  double Jet_Tau1, Jet_Tau2;
  double ZHmass;

  tree->SetBranchAddress("Event_Weight",         &Event_Weight);
  tree->SetBranchAddress("Jet_SDmass_corrected", &Jet_SDmass_corrected);
  tree->SetBranchAddress("Double_bTagger",       &Double_bTagger);
  tree->SetBranchAddress("Jet_Tau1",             &Jet_Tau1);
  tree->SetBranchAddress("Jet_Tau2",             &Jet_Tau2);

  tree->SetBranchAddress("ZHmass",               &ZHmass);

  int Number_to_print    =  10000;

  // event loop 
  for( Long64_t ev = 0; ev < tree->GetEntriesFast(); ev++ ){

    if( ev %   Number_to_print == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, tree->GetEntriesFast());

    tree->GetEntry(ev);

    double Jet_mass = Jet_SDmass_corrected;
    double Jet_Tau21 = Jet_Tau2/Jet_Tau1;

//    if ( Jet_Tau21 < Tau21Cut ) continue;

    // blind data in the signal region (no blind in MC)
//    if ( isData && Jet_mass > 65 && Jet_mass < 135 )continue;

    // require in validation region
    if ( !(Jet_mass > Vmin && Jet_mass < Vmax) ) continue;


    // pass
//    if ( Double_bTagger > DoubleBTaggerCut ){ h_ZH_mass_pass ->Fill(ZHmass, Event_Weight ) ; }

    // fail and predicted pass
    else{
      h_ZH_mass_fail         ->Fill(ZHmass, Event_Weight ) ;

      double pass_fail_ratio = fn_1->Eval( Jet_mass );
      double new_weight = Event_Weight * pass_fail_ratio;
      h_ZH_mass_pass_predict ->Fill(ZHmass, new_weight ) ;

    }

  }// end event loop



}
