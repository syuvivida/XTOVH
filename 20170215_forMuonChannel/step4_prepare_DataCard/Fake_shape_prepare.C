#include <vector>
#include <string>
#include <iostream>
#include <fstream>
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

void Fake_shape_prepare(){


  // -----------------------------
  // Open signal and background TH1F
  
  std::string path_signal="old_files/text_and_ROOT_files/Zprime_shape_Ele_1st_pt_cut_435.root";

  const int nSig = 11;
  int mass_points[nSig] = {800,1000,1200,1400,1600,1800,2000,2500,3000,3500,4000};
  TString h_name[nSig];  for (int i=0;i<nSig;i++){ h_name[i] = Form("SIG_M%d",mass_points[i]);} 

  TFile* file_signal = TFile::Open( path_signal.c_str() );
  TH1D* h_signal[nSig]; for (int i=0;i<nSig;i++){ h_signal[i] = (TH1D*) file_signal->Get( h_name[i] );}
//  h_signal[3]->Draw();

  std::string path_background="Background_predicted_from_SingleMuonRun2016BCDEFG_combine.root";
  TFile* file_background = TFile::Open( path_background.c_str() );
  TH1D* h_background = (TH1D*) file_background ->Get("h_ZH_mass_pass_predict");
//  h_background->Draw();



  // -----------------------------
  // Clone TH1F for data

  double data_norm_fake = h_background->Integral() +1 ;
  TH1D* h_data = (TH1D*) h_background->Clone("data_obs");
  h_data->Scale(data_norm_fake/h_data->Integral()); 


  // -----------------------------
  // Print normalization 

  ofstream fout;
  fout.open("MuonCh_EvtNumber.txt");

  cout<<"data_obs: "<< h_data ->Integral()<<endl;  
  fout<<"data_obs: "<< h_data ->Integral()<<endl;

  for (int i=0;i<nSig;i++){
    cout<<h_name[i]<<": "<< h_signal[i]->Integral()<<endl;
    fout<<h_name[i]<<": "<< h_signal[i]->Integral()<<endl;
  }

  cout<<"BACKGROUND: "<< h_background ->Integral()<<endl;
  fout<<"BACKGROUND: "<< h_background ->Integral()<<endl;





  // ----------------------------
  // Save in ROOT file

  TFile* outFile = new TFile("DataCard_and_ROOT_MuonCh/Shape_test.root", "recreate");
  h_data ->Write("data_obs");
  for (int i=0;i<nSig;i++){ h_signal[i] ->Write( h_name[i] ); }
  h_background ->Write("BACKGROUND");

  outFile->Write();
  delete outFile; 


}
/*

void Differential(TH1F* &histo1D  );

void make_histogram(    std::string sample_name ,

			TH1F* &h_passDoubleBTagger_Jmass_wideBinning_diff_,
			TH1F* &h_failDoubleBTagger_Jmass_wideBinning_diff_,
			TH1F* &h_Ratio_wideBinning_

		   ){


  std::string sample_path ="output_mu/" + sample_name + ".root"; 
  cout<<"sample_path: "<< sample_path << endl;

  std::string save_name = sample_name;

  bool isData = false; // false for MC
//  if( sample_path.find("SingleElectron") != std::string::npos ) {cout<<"find data" << endl; isData = true;}// true for Data
//  if( sample_path.find("SingleMuon") != std::string::npos ) {cout<<"find data" << endl; isData = true;}// true for Data
  if( sample_path.find("Run2016") != std::string::npos ) {cout<<"find data" << endl; isData = true;}// true for Data

  // ------ define histogram --------

  TH1F* h_passDoubleBTagger_Jmass = new TH1F("h_passDoubleBTagger_Jmass" ,"passDoubleBTagger Jmass ",200,0,1000);
  TH1F* h_failDoubleBTagger_Jmass = new TH1F("h_failDoubleBTagger_Jmass" ,"failDoubleBTagger Jmass ",200,0,1000);

  TH1F* h_passDoubleBTagger_Jmass_wideBinning = (TH1F*) h_passDoubleBTagger_Jmass_wideBinning_diff_ ->Clone("h_passDoubleBTagger_Jmass_wideBinning");
  TH1F* h_failDoubleBTagger_Jmass_wideBinning = (TH1F*) h_failDoubleBTagger_Jmass_wideBinning_diff_ ->Clone("h_failDoubleBTagger_Jmass_wideBinning");

  TH2F* h2_Jmass_DoubleBTagger = new TH2F("h2_Jmass_DoubleBTagger" ,"h2_Jmass_DoubleBTagger",200,0,1000, 20, -1, 1); 

  TH1F* h_tau21 = new TH1F("h_tau21" ,"h_tau21 ",20,0,1);

  // Sumw2


  h_passDoubleBTagger_Jmass             ->Sumw2();
  h_failDoubleBTagger_Jmass             ->Sumw2();

  h_passDoubleBTagger_Jmass_wideBinning ->Sumw2();
  h_failDoubleBTagger_Jmass_wideBinning ->Sumw2();

  h2_Jmass_DoubleBTagger ->Sumw2();
  h_tau21                ->Sumw2();

  // ------ open root file ---------- 

  TFile* file   = TFile::Open( sample_path.c_str() );

  TH1F* h_totalEv = (TH1F*)file->FindObjectAny("totalEvents");

  TTree* tree = (TTree*) file->Get("new_tree");

  double Event_Weight;
  double Jet_SDmass_corrected;
  double Double_bTagger;
  double Jet_Tau1, Jet_Tau2;
  double ZHmass;

  tree->SetBranchAddress("Event_Weight",         &Event_Weight);
  tree->SetBranchAddress("Jet_SDmass_corrected", &Jet_SDmass_corrected);
  tree->SetBranchAddress("Double_bTagger",       &Double_bTagger);
  tree->SetBranchAddress("Jet_Tau1",       	 &Jet_Tau1);
  tree->SetBranchAddress("Jet_Tau2",       	 &Jet_Tau2);

  tree->SetBranchAddress("ZHmass",               &ZHmass);

  // ----- event loop on Mini TTree -----------


  int Number_to_print    =  10000;
  

  for( Long64_t ev = 0; ev < tree->GetEntriesFast(); ev++ ){

    if( ev %   Number_to_print == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, tree->GetEntriesFast());

    tree->GetEntry(ev);


    double Jet_mass = Jet_SDmass_corrected;

    double Jet_Tau21 = Jet_Tau2/Jet_Tau1;


    if ( Jet_mass < Xmin1 )  continue;// remove low mass peak in Jet Mass
    if ( Jet_mass > Xmax2 )  continue;// remove high mass spikes

//    if ( Jet_Tau21 < Tau21Cut ) continue; // since apply anti-tau21 cut so we don't blind SD mass [65,135]


    // blind data in the signal region (no blind in MC)
//    if ( isData && Jet_mass > Xmax1 && Jet_mass < Xmin2 && Double_bTagger > DoubleBTaggerCut )continue;
    if ( isData && Jet_mass > Xmax1 && Jet_mass < Xmin2 )continue;

    // 2D plot and tau21 plot
    h2_Jmass_DoubleBTagger ->Fill( Jet_mass , Double_bTagger , Event_Weight);
    h_tau21 		   ->Fill( Jet_Tau21 , Event_Weight);


    // Pass
    if( Double_bTagger > DoubleBTaggerCut){
 
      h_passDoubleBTagger_Jmass             ->Fill( Jet_mass , Event_Weight);
      h_passDoubleBTagger_Jmass_wideBinning ->Fill( Jet_mass , Event_Weight);
    }
    // Fail
    else{

      h_failDoubleBTagger_Jmass             ->Fill( Jet_mass , Event_Weight);
      h_failDoubleBTagger_Jmass_wideBinning ->Fill( Jet_mass , Event_Weight);

    }



  }// ------ end event loop -------------




  // Differential histogram
  TH1F* h_passDoubleBTagger_Jmass_wideBinning_diff = (TH1F*) h_passDoubleBTagger_Jmass_wideBinning->Clone("h_passDoubleBTagger_Jmass_wideBinning_diff");
  Differential (  h_passDoubleBTagger_Jmass_wideBinning_diff );

  TH1F* h_failDoubleBTagger_Jmass_wideBinning_diff = (TH1F*) h_failDoubleBTagger_Jmass_wideBinning->Clone("h_failDoubleBTagger_Jmass_wideBinning_diff");
  Differential (  h_failDoubleBTagger_Jmass_wideBinning_diff );


  // Add Ratio plot for data only

  TH1F* h_Ratio; TH1F* h_Ratio_wideBinning;
 
  if(isData){ 

    h_Ratio = (TH1F*) h_passDoubleBTagger_Jmass ->Clone("h_Ratio");
    h_Ratio ->Sumw2();
    h_Ratio ->Divide( h_failDoubleBTagger_Jmass );
    h_Ratio ->SetTitle("h_Ratio");

    h_Ratio_wideBinning = (TH1F*) h_passDoubleBTagger_Jmass_wideBinning ->Clone("h_Ratio_wideBinning");
    h_Ratio_wideBinning ->Sumw2();
    h_Ratio_wideBinning ->Divide( h_failDoubleBTagger_Jmass_wideBinning );
    h_Ratio_wideBinning ->SetTitle("h_Ratio_wideBinning");

  }


  // save histogram in root file
  TString save_path = "output_mu/"+ save_name +"_histogram.root";
  TFile* outFile = new TFile( save_path , "recreate");

  h_passDoubleBTagger_Jmass                  ->Write();
  h_passDoubleBTagger_Jmass_wideBinning      ->Write();
  h_passDoubleBTagger_Jmass_wideBinning_diff ->Write();

  h_failDoubleBTagger_Jmass                  ->Write();
  h_failDoubleBTagger_Jmass_wideBinning      ->Write();
  h_failDoubleBTagger_Jmass_wideBinning_diff ->Write();

  h2_Jmass_DoubleBTagger                     ->Write();
  h_tau21                                    ->Write();

  h_totalEv				     ->Write();

  if(isData) { // for data only
    h_Ratio                                  ->Write();
    h_Ratio_wideBinning                      ->Write();
  }

  outFile->Write();
  delete outFile;


  // transfer the 3 TH1F back to main functions 
  h_passDoubleBTagger_Jmass_wideBinning_diff_ = h_passDoubleBTagger_Jmass_wideBinning_diff;
  h_failDoubleBTagger_Jmass_wideBinning_diff_ = h_failDoubleBTagger_Jmass_wideBinning_diff;
  h_Ratio_wideBinning_                        = h_Ratio_wideBinning;

}




void Differential (TH1F* &histo1D ){


  histo1D ->Sumw2();

  int Nbins = histo1D ->GetNbinsX();

  for(int i=1;i<=Nbins; i++){
    double BinContent = histo1D -> GetBinContent(i);
    double BinWidth   = histo1D -> GetBinWidth(i);
    histo1D -> SetBinContent(i, BinContent/ BinWidth );
    histo1D -> SetBinError(i, (1/BinWidth) * sqrt(BinContent) );
  }

  TString title_name = histo1D ->GetTitle();
  title_name = title_name + "differential";
  histo1D ->SetTitle(title_name);

}

*/




