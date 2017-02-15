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

#include "cut_value.h"
#include "make_histogram.C"
#include "supportHeader.h"
#include "predict_ZH.C"

double Xmin,Xmax,Ymin,Ymax;
TString XTitle,YTitle;


void validate_data( std::string sample_name ){



  // -----------------------------------------------------
  // Part-I: make histogram
  cout<<endl<<" Part-I: make histogram "<<endl<<endl; 
  

//  std::string sample_name =  "SingleElectron-Run2016_combine_BCDEFG";
//  std::string sample_name =  "SingleMuonRun2016B";
//  std::string sample_name =  "SingleMuonRun2016BCDEFG_combine";


  const int NXbins4 = 7;
  double xbins4[NXbins4+1]={ 50, 65, 80, 100,120,135 ,160,200 };


//  const int NXbins4 = 8;
//  double xbins4[NXbins4+1]={ 50, 65, 80, 100,120,135 , 147,160,200 };

//  const int NXbins4 = 6;
//  double xbins4[NXbins4+1]={ 50, 65, 75,115,135 ,160,200 };

//  const int NXbins4 = 14;
//  double xbins4[NXbins4+1]={ 50,57, 65, 72,80,90, 100,110, 120,127,135,147,160,180,200 };



  TH1F* h_passDoubleBTagger_Jmass_wideBinning_diff = new TH1F("h_passDoubleBTagger_Jmass_wideBinning_diff" ,"passDoubleBTagger Jmass wideBinning ",NXbins4, xbins4);

  TH1F* h_failDoubleBTagger_Jmass_wideBinning_diff = (TH1F*) h_passDoubleBTagger_Jmass_wideBinning_diff ->Clone("h_failDoubleBTagger_Jmass_wideBinning_diff");
  h_failDoubleBTagger_Jmass_wideBinning_diff->Reset();

  TH1F* h_Ratio_wideBinning = (TH1F*) h_passDoubleBTagger_Jmass_wideBinning_diff ->Clone("h_Ratio_wideBinning");
  h_Ratio_wideBinning->Reset();

  make_histogram( sample_name , 
		  h_passDoubleBTagger_Jmass_wideBinning_diff,
		  h_failDoubleBTagger_Jmass_wideBinning_diff,
		  h_Ratio_wideBinning
  		);



  // ----------------------------------------------------------
  // part-II: fit pass/fail ratio
  cout<<endl<<" Part-II: fit pass/fail ratio "<<endl<<endl;



  Xmin= 30; Xmax=200; Ymin= -1; Ymax= 1;
  XTitle= "Corrected Soft Drop Mass [GeV]";

  YTitle= "Pass / Fail Ratio";
  Set_h1_Style( h_Ratio_wideBinning , Xmin,Xmax,Ymin,Ymax, XTitle,YTitle );

  h_Ratio_wideBinning ->GetYaxis ()->SetRangeUser(0 ,1 );

  // Draw
  TCanvas* c3 = new TCanvas("c3","c3",10,10,800,600);
  c3->cd(); h_Ratio_wideBinning -> Draw();

  // Fit
  gStyle->SetOptFit(11111111);

  TF1* fn_1 = new TF1("fn_1", fline_Quadric ,50,200,3);

  double parameters[10];
  parameters[0] = 0.00002; parameters[1] = -0.0032; parameters[2] = 0.15;

  Fit_TwoRange( h_Ratio_wideBinning , fn_1 , "Quadric", parameters );

  double mychi2 = GetMyChisquare ( h_Ratio_wideBinning , fn_1 );
  cout<<"mychi2: "<< mychi2 << endl;

/*
  // estimate area in validation region

  double Area_histo = GetMyHisto_Area(h_Ratio_wideBinning , Vmin , Vmax);
  double Area_fit   = fn_1->Integral(Vmin,Vmax);

  cout<<" Area in Validate of h_Ratio["<<Vmin<<","<<Vmax<<"]: "<< Area_histo<< endl ;
  cout<<" Area in Validate of fit ["<<Vmin<<","<<Vmax<<"]: "<< Area_fit<< endl ;
  cout<<"diff: "<< (Area_fit - Area_histo)/Area_histo << endl;
*/




  // ----------------------------------------------------------
  // Part-III: predict ZH mass
  cout<<endl<<" Part-III: predict ZH mass "<<endl<<endl;


  // define histogram
  
  TH1F* h_ZH_mass_pass = new TH1F("h_ZH_mass_pass" ,"h_ZH_mass_pass",40,0,2000);

  TH1F* h_ZH_mass_fail = (TH1F*) h_ZH_mass_pass -> Clone("h_ZH_mass_fail");
  h_ZH_mass_fail->Reset();

  TH1F* h_ZH_mass_pass_predict = (TH1F*) h_ZH_mass_pass -> Clone("h_ZH_mass_pass_predict");
  h_ZH_mass_pass_predict->Reset();


  h_ZH_mass_pass ->Sumw2();
  h_ZH_mass_fail ->Sumw2();
  h_ZH_mass_pass_predict ->Sumw2();


  predict_ZH(    h_ZH_mass_pass,
                 h_ZH_mass_fail,
                 h_ZH_mass_pass_predict,
                 fn_1,
		 sample_name
               );


  // estimate difference in ZH mass

/*
  double pass_area = h_ZH_mass_pass->Integral(); 
  double pass_area_predict = h_ZH_mass_pass_predict->Integral();

  cout<<" Area in h_ZH_mass_pass: "<< pass_area << endl ;
  cout<<" Area in h_ZH_mass_pass_predict:: "<< pass_area_predict << endl ;
  cout<<"relative difference in ZH mass : "<< (pass_area_predict - pass_area)/pass_area << endl;
*/


  // ----------------------------------------
  // part-IV: plot
  cout<<endl<<" Part-IV: plot "<<endl<<endl;


  // Pass and fail Soft Drop mass
  YTitle= "Number of Events / GeV";
  Set_h1_Style( h_passDoubleBTagger_Jmass_wideBinning_diff , Xmin,Xmax,Ymin,Ymax, XTitle,YTitle );
  Set_h1_Style( h_failDoubleBTagger_Jmass_wideBinning_diff , Xmin,Xmax,Ymin,Ymax, XTitle,YTitle );


  // fail ZH mass 
  h_ZH_mass_fail ->SetTitle("");
  h_ZH_mass_fail ->SetStats(0);

  h_ZH_mass_fail ->GetXaxis ()->SetTitle("ZH invariant mass [GeV]");
  h_ZH_mass_fail ->GetYaxis ()->SetTitle("Events");

  h_ZH_mass_fail ->GetYaxis()->SetTitleSize(0.04);
  h_ZH_mass_fail ->GetYaxis()->SetTitleOffset(0.9);

  h_ZH_mass_fail ->SetLineColor(4); // blue
  h_ZH_mass_pass ->SetLineColor(3); // green
  h_ZH_mass_pass_predict->SetLineColor(2); //red

/*
  // predicted/pass ratio 
  TString ratio_name = "h_Ratio";
  TH1F* h_ratio  = (TH1F*) h_ZH_mass_pass_predict ->Clone( ratio_name );
  h_ratio  ->Sumw2();
  h_ratio  ->Divide( h_ZH_mass_pass );

  h_ratio ->SetStats(0);
  h_ratio ->SetTitle("");
  h_ratio ->GetYaxis()->SetRangeUser(-1, 3);
  h_ratio ->GetYaxis()->SetNdivisions(505);
  h_ratio ->GetXaxis()->SetLabelSize(0.10);
  h_ratio ->GetYaxis()->SetLabelSize(0.10);
  h_ratio ->GetXaxis()->SetTitle("ZH invariant mass");
  h_ratio ->GetYaxis()->SetTitle("Predict / Pass");
  h_ratio ->GetYaxis()->SetTitleOffset(0.33);
  h_ratio ->GetXaxis()->SetTitleSize(0.11);
  h_ratio ->GetYaxis()->SetTitleSize(0.11);
*/

  // draw  

  TCanvas* c1 = new TCanvas("c1","c1",10,10,800,600);
  c1->cd(); h_passDoubleBTagger_Jmass_wideBinning_diff -> Draw();


  TCanvas* c2 = new TCanvas("c2","c2",10,10,800,600);
  c2->cd(); h_failDoubleBTagger_Jmass_wideBinning_diff -> Draw();


  TCanvas* c4 = new TCanvas("c4","c4",10,10,800,600);

//  TPad* pad1   = new TPad("pad1","pad1",0.0, 0.3, 1.0, 1.0);
//  TPad* pad2   = new TPad("pad2","pad2",0.0, 0.0, 1.0, 0.3);

  c4->cd();

//  pad1->SetBottomMargin(0.01);
//  pad1->Draw();
//  pad1->cd();

  TLegend* leg = new TLegend(0.5, 0.6, 0.9, 0.9);

  h_ZH_mass_fail -> Draw();  
//  h_ZH_mass_pass -> Draw("same");
  h_ZH_mass_pass_predict -> Draw("same");

  leg->AddEntry(h_ZH_mass_fail,"h_ZH_mass_fail","l");
//  leg->AddEntry(h_ZH_mass_pass,"h_ZH_mass_pass","l");
  leg->AddEntry(h_ZH_mass_pass_predict,"h_ZH_mass_pass_predict","l");

  leg->Draw();

/*
  c4->cd();

    pad2  ->SetTopMargin(0.9);
    pad2  ->SetBottomMargin(0.3);
    pad2  ->Draw();
    pad2  ->cd();

    h_ratio->Draw("E0PX0");
    TLine* OLine = new TLine( 0.0, 1. , 2000 , 1.);
    OLine ->SetLineColor(801);
    OLine ->SetLineWidth(2);
    OLine ->Draw();
*/
 
  // Save plot

  TString save_path ="plot/validate_data1.pdf";
  c1->Print( save_path );

  save_path ="plot/validate_data2.pdf";
  c2->Print( save_path      );

  save_path ="plot/validate_data3.pdf";
  c3->Print( save_path      );

  save_path ="plot/validate_data4.pdf";
  c4->Print( save_path );

  save_path ="plot/validate_data.pdf";
  c1->Print( save_path +"(");
  c2->Print( save_path );
  c3->Print( save_path );
  c4->Print( save_path +")");

  // Save TH1F in ROOT file

  save_path = "output_mu/Background_predicted_from_"+ sample_name +".root";
  TFile* outFile = new TFile( save_path , "recreate");

  h_ZH_mass_pass_predict->Write();

  outFile->Write();
  delete outFile;

  return;


}


