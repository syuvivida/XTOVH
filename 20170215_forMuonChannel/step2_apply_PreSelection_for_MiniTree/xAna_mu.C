#include <vector>
#include <string>
#include <iostream>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <TF1.h>

#include "interface/untuplizer.h"
//#include "interface/isPassZee.h"
#include "interface/isPassZmumu.h"
#include "interface/readSample.h"
#include "interface/ele_Cert_271036-282092-MinBiasXsec_69200/standalone_LumiReWeighting.cc"
#include "interface/dataFilter.h"
#include "interface/PuppiSDmassCorr/getPUPPIweight.h"

//void xAna_ele(std::string inputFile, std::string outputFolder, std::string outputFile){
void xAna_mu(std::string inputFile, std::string outputFolder, std::string outputFile){

  cout<<"finish compiling and start to run macro"<< endl;


  // ------  TF1 for Thea's SD mass correction -----------


  TFile *f3;
  f3=TFile::Open("interface/PuppiSDmassCorr/puppiCorr.root");
  TF1* tf1[3];
  tf1[0]=(TF1 *) f3->FindObjectAny("puppiJECcorr_gen");
  tf1[1]=(TF1 *) f3->FindObjectAny("puppiJECcorr_reco_0eta1v3");
  tf1[2]=(TF1 *) f3->FindObjectAny("puppiJECcorr_reco_1v3eta2v5");


  // --------  open  ntuples ------------ 

  // get paths of every root files
  std::vector<string> infiles; 
  readSample(inputFile, infiles);

  cout<<"infiles.size():"<< infiles.size() << endl<<endl;

  // combine TTree
//  TChain* tree = new TChain("tree/treeMaker");
  TChain* tree = new TChain("treeMaker");


  for(unsigned int i=0 ; i<infiles.size() ; i++ ){
    std::string open_root="root://" + infiles[i] ;
    tree->Add( open_root.c_str()  );
  }

  cout<<"tree->GetEntries(): "<< tree->GetEntries() << endl<<endl;

  // read the TTree

  TreeReader data( tree );
  cout<<"finish TreeReader data( tree );"<< endl<<endl; 

  // -------- Add new TTree ----------

  // root file to save
  std::string root_name = Form("%s.root",outputFile.c_str() ) ;
  TString save_path = outputFolder +"/"+  root_name;

  TFile* outFile = new TFile( save_path , "recreate");

  // Mini TTree
  TTree* new_tree = new TTree("new_tree", "Skiming MiniTTree");

  double Event_Weight;
  double PU_nTrueInt;
  int    NVtx;

//  double LeadingElePt   , LeadingEleEta   , LeadingElePhi;
//  double SubLeadingElePt, SubLeadingEleEta, SubLeadingElePhi;  

  double LeadingMuPt   , LeadingMuEta   , LeadingMuPhi;
  double SubLeadingMuPt, SubLeadingMuEta, SubLeadingMuPhi;

  double Z_Pt  , Z_Eta  , Z_Phi  , Z_M  , Z_Rapidity;

  double Jet_Pt, Jet_Eta, Jet_Phi, Jet_M;
  double Jet_CISVV2, Jet_SDmass, Jet_SDmass_corrected, Jet_PRmass, Jet_PRmass_corrected, Jet_Tau1, Jet_Tau2;

  int    nSubJet;
  double LeadingSubJet_Pt   , LeadingSubJet_Eta   , LeadingSubJet_Phi   , LeadingSubJet_E;
  double SubLeadingSubJet_Pt, SubLeadingSubJet_Eta, SubLeadingSubJet_Phi, SubLeadingSubJet_E;

  double LeadingSubJet_SDCSV   , SubLeadingSubJet_SDCSV;

  double Double_bTagger, Double_bTagger_ADDjet; 

  double ZHmass;

  new_tree->Branch("Event_Weight",         &Event_Weight,          "Event_Weight/D");
  new_tree->Branch("PU_nTrueInt",          &PU_nTrueInt,           "PU_nTrueInt/D");
  new_tree->Branch("NVtx",        	   &NVtx,        	   "NVtx/I");

//  new_tree->Branch("LeadingElePt",         &LeadingElePt,          "LeadingElePt/D");
//  new_tree->Branch("LeadingEleEta",        &LeadingEleEta,         "LeadingEleEta/D");
//  new_tree->Branch("LeadingElePhi",        &LeadingElePhi,         "LeadingElePhi/D");

//  new_tree->Branch("SubLeadingElePt",      &SubLeadingElePt,       "SubLeadingElePt/D");
//  new_tree->Branch("SubLeadingEleEta",     &SubLeadingEleEta,      "SubLeadingEleEta/D");
//  new_tree->Branch("SubLeadingElePhi",     &SubLeadingElePhi,      "SubLeadingElePhi/D");

  new_tree->Branch("LeadingMuPt",         &LeadingMuPt,          "LeadingMuPt/D");
  new_tree->Branch("LeadingMuEta",        &LeadingMuEta,         "LeadingMuEta/D");
  new_tree->Branch("LeadingMuPhi",        &LeadingMuPhi,         "LeadingMuPhi/D");

  new_tree->Branch("SubLeadingMuPt",      &SubLeadingMuPt,       "SubLeadingMuPt/D");
  new_tree->Branch("SubLeadingMuEta",     &SubLeadingMuEta,      "SubLeadingMuEta/D");
  new_tree->Branch("SubLeadingMuPhi",     &SubLeadingMuPhi,      "SubLeadingMuPhi/D");

  new_tree->Branch("Z_Pt",        	   &Z_Pt,        	   "Z_Pt/D");
  new_tree->Branch("Z_Eta",        	   &Z_Eta,        	   "Z_Eta/D");
  new_tree->Branch("Z_Phi",         	   &Z_Phi,        	   "Z_Phi/D");
  new_tree->Branch("Z_M",        	   &Z_M,        	   "Z_M/D");
  new_tree->Branch("Z_Rapidity",           &Z_Rapidity,            "Z_Rapidity/D");

  new_tree->Branch("Jet_Pt",               &Jet_Pt,        	   "Jet_Pt/D");
  new_tree->Branch("Jet_Eta",              &Jet_Eta,        	   "Jet_Eta/D");
  new_tree->Branch("Jet_Phi",              &Jet_Phi,        	   "Jet_Phi/D");
  new_tree->Branch("Jet_M",                &Jet_M,        	   "Jet_M/D");

  new_tree->Branch("Jet_CISVV2",           &Jet_CISVV2,            "Jet_CISVV2/D");
  new_tree->Branch("Jet_SDmass",           &Jet_SDmass,            "Jet_SDmass/D");
  new_tree->Branch("Jet_SDmass_corrected", &Jet_SDmass_corrected,  "Jet_SDmass_corrected/D");
  new_tree->Branch("Jet_PRmass",           &Jet_PRmass,            "Jet_PRmass/D");
  new_tree->Branch("Jet_PRmass_corrected", &Jet_PRmass_corrected,  "Jet_PRmass_corrected/D");
  new_tree->Branch("Jet_Tau1",             &Jet_Tau1,              "Jet_Tau1/D");
  new_tree->Branch("Jet_Tau2",             &Jet_Tau2,              "Jet_Tau2/D");

  new_tree->Branch("nSubJet",              &nSubJet,               "nSubJet/I");

  new_tree->Branch("LeadingSubJet_Pt",     &LeadingSubJet_Pt,      "LeadingSubJet_Pt/D");
  new_tree->Branch("LeadingSubJet_Eta",    &LeadingSubJet_Eta,     "LeadingSubJet_Eta/D");
  new_tree->Branch("LeadingSubJet_Phi",    &LeadingSubJet_Phi,     "LeadingSubJet_Phi/D");
  new_tree->Branch("LeadingSubJet_E",      &LeadingSubJet_E,       "LeadingSubJet_E/D");


  new_tree->Branch("SubLeadingSubJet_Pt",  &SubLeadingSubJet_Pt,   "SubLeadingSubJet_Pt/D");
  new_tree->Branch("SubLeadingSubJet_Eta", &SubLeadingSubJet_Eta,  "SubLeadingSubJet_Eta/D");
  new_tree->Branch("SubLeadingSubJet_Phi", &SubLeadingSubJet_Phi,  "SubLeadingSubJet_Phi/D");
  new_tree->Branch("SubLeadingSubJet_E",   &SubLeadingSubJet_E,    "SubLeadingSubJet_E/D");

  new_tree->Branch("LeadingSubJet_SDCSV",    &LeadingSubJet_SDCSV,    "LeadingSubJet_SDCSV/D");
  new_tree->Branch("SubLeadingSubJet_SDCSV", &SubLeadingSubJet_SDCSV, "SubLeadingSubJet_SDCSV/D");

  new_tree->Branch("Double_bTagger",      &Double_bTagger,      "Double_bTagger/D");
  new_tree->Branch("Double_bTagger_ADDjet", &Double_bTagger_ADDjet, "Double_bTagger_ADDjet/D");

  new_tree->Branch("ZHmass",              &ZHmass,              "ZHmass/D");

  // ------- Declare the histogram ----------------

  TH1D* h_totalEvents      = new TH1D("h_totalEv",          "totalEvents",       10,     0,   10);


  // -----------------------------------------

  int counter_0=0;
  int counter_1=0;
  int counter_2=0;    
  int counter_3=0;

  cout<<"finish define new TTree"<< endl<<endl;

  // ------ begin of event loop ---------------

  bool UseFullStat    = true;    bool PU_weight_flag    = true; 
  int Number_to_print =  100000; int Max_number_to_read =  300000;

  if( UseFullStat ) cout<<"use Full Statistics: "   << data.GetEntriesFast() <<endl<<endl;
  else              cout<<"use Partial Statistics: "<< Max_number_to_read    <<endl<<endl;

  if( PU_weight_flag ) cout<<"Will use Pile Up weight"    <<endl;
  else                 cout<<"Will not use Pile Up weight"<<endl;

  cout<<"starting loop events"<< endl;

  for( Long64_t ev = 0; ev < data.GetEntriesFast(); ev++ ){

    if( ev %   Number_to_print == 0 )
      fprintf(stderr, "Processing event %lli of %lli\n", ev + 1, data.GetEntriesFast());

    if( !UseFullStat && ev > Max_number_to_read ) break;


    data.GetEntry(ev);


    // get Branch

    // variable of event
    Bool_t   isData                  = data.GetBool("isData");
    Float_t  ntrue                   = data.GetFloat("pu_nTrueInt");
    Int_t    nVtx                    = data.GetInt("nVtx");

    // variable of electron
//    const TClonesArray* eleP4 = (TClonesArray*) data.GetPtrTObject("eleP4");
    const TClonesArray* muP4  = (TClonesArray*) data.GetPtrTObject("muP4");

    // variable of FatJet
    Int_t          FATnJet           = data.GetInt("FATnJet");    
    TClonesArray*  FATjetP4          = (TClonesArray*) data.GetPtrTObject("FATjetP4");

    Float_t*       FATjetCISVV2      = data.GetPtrFloat("FATjetCISVV2");
    Float_t*       FATjetSDmass      = data.GetPtrFloat("FATjetSDmass");
    Float_t*       FATjetPRmass      = data.GetPtrFloat("FATjetPRmass");
    Float_t*       FATjetPRmassCorr  = data.GetPtrFloat("FATjetPRmassL2L3Corr");
    Float_t*       FATjetTau1        = data.GetPtrFloat("FATjetTau1");
    Float_t*       FATjetTau2        = data.GetPtrFloat("FATjetTau2");
    vector<bool>&  FATjetPassIDLoose = *((vector<bool>*) data.GetPtr("FATjetPassIDLoose"));

    // variable of SubJet
    Int_t*         FATnSubSDJet      = data.GetPtrInt("FATnSubSDJet");
    vector<float>* FATsubjetSDPx     = data.GetPtrVectorFloat("FATsubjetSDPx", FATnJet);
    vector<float>* FATsubjetSDPy     = data.GetPtrVectorFloat("FATsubjetSDPy", FATnJet);
    vector<float>* FATsubjetSDPz     = data.GetPtrVectorFloat("FATsubjetSDPz", FATnJet);
    vector<float>* FATsubjetSDE      = data.GetPtrVectorFloat("FATsubjetSDE", FATnJet);
    vector<float>* FATsubjetSDCSV    = data.GetPtrVectorFloat("FATsubjetSDCSV", FATnJet);


    // variable of ADDJet
    Int_t ADDnJet              = data.GetInt("ADDnJet");
    TClonesArray* ADDjetP4     = (TClonesArray*) data.GetPtrTObject("ADDjetP4");
    Float_t*  ADDjet_DoubleSV  = data.GetPtrFloat("ADDjet_DoubleSV");


    // variable of AK8PuppiJet
    Int_t          AK8PuppinJet           = data.GetInt("AK8PuppinJet");
    TClonesArray*  AK8PuppijetP4          = (TClonesArray*) data.GetPtrTObject("AK8PuppijetP4");

//    Float_t*       AK8PuppijetCSV         = data.GetPtrFloat("AK8PuppijetCSV");
    Float_t*       AK8PuppijetCISVV2         = data.GetPtrFloat("AK8PuppijetCISVV2");
    Float_t*       AK8PuppijetSDmass      = data.GetPtrFloat("AK8PuppijetSDmass");
    Float_t*       AK8PuppijetTau1        = data.GetPtrFloat("AK8PuppijetTau1");
    Float_t*       AK8PuppijetTau2        = data.GetPtrFloat("AK8PuppijetTau2");
    Float_t*       AK8Puppijet_DoubleSV   = data.GetPtrFloat("AK8Puppijet_DoubleSV");
    vector<bool>&  AK8PuppijetPassIDLoose = *((vector<bool>*) data.GetPtr("AK8PuppijetPassIDLoose"));

    // variable of AK8PuppiSubJet
    Int_t*         AK8PuppinSubSDJet      = data.GetPtrInt("AK8PuppinSubSDJet");
    vector<float>* AK8PuppisubjetSDPx     = data.GetPtrVectorFloat("AK8PuppisubjetSDPx",  AK8PuppinJet);
    vector<float>* AK8PuppisubjetSDPy     = data.GetPtrVectorFloat("AK8PuppisubjetSDPy",  AK8PuppinJet);
    vector<float>* AK8PuppisubjetSDPz     = data.GetPtrVectorFloat("AK8PuppisubjetSDPz",  AK8PuppinJet);
    vector<float>* AK8PuppisubjetSDE      = data.GetPtrVectorFloat("AK8PuppisubjetSDE",   AK8PuppinJet);
    vector<float>* AK8PuppisubjetSDCSV    = data.GetPtrVectorFloat("AK8PuppisubjetSDCSV", AK8PuppinJet);


    // get Pile-Up weight 


    double PU_weight_central =1;// weight=1 for data

    if(!isData && PU_weight_flag){// for MC

      standalone_LumiReWeighting LumiWeights_central(0);
      PU_weight_central = LumiWeights_central.weight(ntrue);

    }
   
//    double eventWeight = PU_weight_central;   
    double eventWeight = 1; // for a moment   

    h_totalEvents->Fill(1,eventWeight);
 
    // data filter 

    bool CSCT       = FilterStatus(data, "Flag_CSCTightHaloFilter");
    bool eeBadSc    = FilterStatus(data, "Flag_eeBadScFilter");
    bool Noise      = FilterStatus(data, "Flag_HBHENoiseFilter");
    bool NoiseIso   = FilterStatus(data, "Flag_HBHENoiseIsoFilter");

    bool filter_pass = false; 
    if( isData && CSCT && eeBadSc && Noise && NoiseIso ) filter_pass = true;// Data 
    else if (!isData) filter_pass = true; // MC, doesn't apply data filter

    if( !filter_pass ) continue;



    //  apply HLT 

    bool eleTrigger1 = TriggerStatus(data, "HLT_Ele105_CaloIdVT_GsfTrkIdT_v");

    bool HLT_pass = false;
    if( isData && eleTrigger1 ) HLT_pass= true;// Data
    else if (!isData) HLT_pass = true; // MC, doesn't apply HLT

//    if( !HLT_pass ) continue; // Don't apply HLT for a moment

    counter_0++;



    //  nVtx>=1 

    if( nVtx < 1 ) continue;

    counter_1++;



    // select good Electrons and Z boson 

//    vector<Int_t> goodEleID;
//    if( !isPassZee(data, goodEleID) ) continue;

//    TLorentzVector* thisEle = (TLorentzVector*)eleP4->At(goodEleID[0]);
//    TLorentzVector* thatEle = (TLorentzVector*)eleP4->At(goodEleID[1]);

    // select good Muons and Z boson 

    vector<Int_t> goodMuID;
    if( !isPassZmumu(data, goodMuID) ) continue;

    TLorentzVector* thisMu = (TLorentzVector*)muP4->At(goodMuID[0]);
    TLorentzVector* thatMu = (TLorentzVector*)muP4->At(goodMuID[1]);

    counter_2++;
 
    // select Fat jet for Higgs 

    Int_t goodFATJetID = -1;
    TLorentzVector* thisJet = NULL;

    // (use FatJet or AK8PuppiJet and comment one of them)
/*  
    for(Int_t ij = 0; ij < FATnJet; ++ij){

      thisJet = (TLorentzVector*)FATjetP4->At(ij);

      if( thisJet->Pt() < 200 ) continue;
      if( fabs(thisJet->Eta()) > 2.4 ) continue;
      if( !FATjetPassIDLoose[ij] ) continue;
      if( thisJet->DeltaR(*thisEle) < 0.8 || thisJet->DeltaR(*thatEle) < 0.8 ) continue;
//      if( !( FATjetPRmassCorr[ij] < 65 || FATjetPRmassCorr[ij] > 135) ) continue;// side band region

      goodFATJetID = ij;
      break;

    }
*/

    for(Int_t ij = 0; ij < AK8PuppinJet; ++ij){

      thisJet = (TLorentzVector*)AK8PuppijetP4->At(ij);

      if( thisJet->Pt() < 200 ) continue;
      if( fabs(thisJet->Eta()) > 2.4 ) continue;
      if( !AK8PuppijetPassIDLoose[ij] ) continue;
//      if( thisJet->DeltaR(*thisEle) < 0.8 || thisJet->DeltaR(*thatEle) < 0.8 ) continue;
      if( thisJet->DeltaR(*thisMu) < 0.8 || thisJet->DeltaR(*thatMu) < 0.8 ) continue;
//      if( !( AK8PuppijetSDmass[ij] < 65 || AK8PuppijetSDmass[ij] > 135) ) continue;// side band region

      goodFATJetID = ij;
      break;

    }


    if( goodFATJetID < 0 ) continue;

    counter_3++;


    // ---------------------------------------
    // save variable in TTree in Physical object level

    // nVtx

    NVtx         = nVtx;
    PU_nTrueInt  = ntrue;
    Event_Weight = eventWeight;

    // Leading and SubLeading Electron 
/*
    if( thisEle->Pt() > thatEle->Pt() ){

      LeadingElePt    = thisEle->Pt(); LeadingEleEta    = thisEle->Eta(); LeadingElePhi    = thisEle->Phi();
      SubLeadingElePt = thatEle->Pt(); SubLeadingEleEta = thatEle->Eta(); SubLeadingElePhi = thatEle->Phi();

    }else{

      SubLeadingElePt = thisEle->Pt(); SubLeadingEleEta = thisEle->Eta(); SubLeadingElePhi = thisEle->Phi();
      LeadingElePt    = thatEle->Pt(); LeadingEleEta    = thatEle->Eta(); LeadingElePhi    = thatEle->Phi();
    }
*/

    // Leading and SubLeading Muon

    if( thisMu->Pt() > thatMu->Pt() ){

      LeadingMuPt    = thisMu->Pt(); LeadingMuEta    = thisMu->Eta(); LeadingMuPhi    = thisMu->Phi();
      SubLeadingMuPt = thatMu->Pt(); SubLeadingMuEta = thatMu->Eta(); SubLeadingMuPhi = thatMu->Phi();

    }else{

      SubLeadingMuPt = thisMu->Pt(); SubLeadingMuEta = thisMu->Eta(); SubLeadingMuPhi = thisMu->Phi();
      LeadingMuPt    = thatMu->Pt(); LeadingMuEta    = thatMu->Eta(); LeadingMuPhi    = thatMu->Phi();
    }

    // Z boson

//    TLorentzVector l4_Z = (*thisEle+*thatEle); 
    TLorentzVector l4_Z = (*thisMu+*thatMu);

    Z_Pt       = l4_Z.Pt();
    Z_Eta      = l4_Z.Eta();
    Z_Phi      = l4_Z.Phi();
    Z_M        = l4_Z.M();
    Z_Rapidity = l4_Z.Rapidity();

    // Fat Jet for Higgs

    double FATjetPuppiSDmassThea = -99;    
//    FATjetPuppiSDmassThea = ( FATjetSDmass[goodFATJetID] )*getPUPPIweight(thisJet->Pt(),thisJet->Eta(),tf1);
    FATjetPuppiSDmassThea = ( AK8PuppijetSDmass[goodFATJetID] )*getPUPPIweight(thisJet->Pt(),thisJet->Eta(),tf1);

    Jet_Pt  = thisJet->Pt();  
    Jet_Eta = thisJet->Eta();
    Jet_Phi = thisJet->Phi();
    Jet_M   = thisJet->M();

/*
    Jet_SDmass           = FATjetSDmass[goodFATJetID];
    Jet_SDmass_corrected = FATjetPuppiSDmassThea;
    Jet_PRmass           = FATjetPRmass[goodFATJetID];
    Jet_PRmass_corrected = FATjetPRmassCorr[goodFATJetID];
    Jet_CISVV2           = FATjetCISVV2[goodFATJetID];
    Jet_Tau1             = FATjetTau1[goodFATJetID];
    Jet_Tau2             = FATjetTau2[goodFATJetID];
*/

    Jet_SDmass           = AK8PuppijetSDmass[goodFATJetID];
    Jet_SDmass_corrected = FATjetPuppiSDmassThea;
//    Jet_CISVV2           = AK8PuppijetCSV[goodFATJetID];
    Jet_CISVV2           = AK8PuppijetCISVV2[goodFATJetID];
    Jet_Tau1             = AK8PuppijetTau1[goodFATJetID];
    Jet_Tau2             = AK8PuppijetTau2[goodFATJetID];

    Double_bTagger       = AK8Puppijet_DoubleSV[goodFATJetID]; 

    // SubJet 

    // set dummy for case nSubJet = 0 or 1
    LeadingSubJet_Pt  = -99; 
    LeadingSubJet_Eta = -99;
    LeadingSubJet_Phi = -99;
    LeadingSubJet_E   = -99;
    LeadingSubJet_SDCSV = -99;

    SubLeadingSubJet_Pt  = -99;
    SubLeadingSubJet_Eta = -99;
    SubLeadingSubJet_Phi = -99;
    SubLeadingSubJet_E   = -99;
    SubLeadingSubJet_SDCSV = -99;

//    nSubJet = FATnSubSDJet[goodFATJetID];
    nSubJet = AK8PuppinSubSDJet[goodFATJetID];

    std::vector<double> subjetPt;  subjetPt .clear();
    std::vector<double> subjetEta; subjetEta.clear();
    std::vector<double> subjetPhi; subjetPhi.clear();
    std::vector<double> subjetE;   subjetE  .clear();

//    for(Int_t is = 0; is < FATnSubSDJet[goodFATJetID]; ++is){
    for(Int_t is = 0; is < AK8PuppinSubSDJet[goodFATJetID]; ++is){


      TLorentzVector l4_subjet(0,0,0,0);

/*
      l4_subjet.SetPxPyPzE(FATsubjetSDPx[goodFATJetID][is],
                           FATsubjetSDPy[goodFATJetID][is],
                           FATsubjetSDPz[goodFATJetID][is],
                           FATsubjetSDE [goodFATJetID][is]);
*/

      l4_subjet.SetPxPyPzE(AK8PuppisubjetSDPx[goodFATJetID][is],
			   AK8PuppisubjetSDPy[goodFATJetID][is],
			   AK8PuppisubjetSDPz[goodFATJetID][is],
			   AK8PuppisubjetSDE [goodFATJetID][is]);

      subjetPt .push_back( l4_subjet.Pt()  );
      subjetEta.push_back( l4_subjet.Eta() );
      subjetPhi.push_back( l4_subjet.Phi() );
      subjetE  .push_back( l4_subjet.E()   );

    }


    if( nSubJet == 1 ){    

        LeadingSubJet_Pt    = subjetPt [0];
        LeadingSubJet_Eta   = subjetEta[0];
        LeadingSubJet_Phi   = subjetPhi[0];
        LeadingSubJet_E     = subjetE  [0];

//        LeadingSubJet_SDCSV = FATsubjetSDCSV[goodFATJetID][0];
        LeadingSubJet_SDCSV = AK8PuppisubjetSDCSV[goodFATJetID][0];

    }


    if( nSubJet > 1 ){

      double leading_pt_temp =-99; double subleading_pt_temp =-99;
      unsigned int leading_index =-1;     unsigned  int subleading_index =-1;

      // loop to find leading index
      for(unsigned int is=0; is<subjetPt.size() ; is++){        
        if( subjetPt[is] > leading_pt_temp ){leading_pt_temp = subjetPt[is] ; leading_index=is; } 
      }
      
      // loop to find subleading index
      for(unsigned int is=0; is<subjetPt.size() ; is++){
        if( is == leading_index) continue;
        if( subjetPt[is] > subleading_pt_temp ){subleading_pt_temp = subjetPt[is] ; subleading_index=is; }
      }


        LeadingSubJet_Pt    = subjetPt [leading_index];
        LeadingSubJet_Eta   = subjetEta[leading_index];
        LeadingSubJet_Phi   = subjetPhi[leading_index];
        LeadingSubJet_E     = subjetE  [leading_index];
//        LeadingSubJet_SDCSV = FATsubjetSDCSV[goodFATJetID][leading_index];
        LeadingSubJet_SDCSV = AK8PuppisubjetSDCSV[goodFATJetID][leading_index];

        SubLeadingSubJet_Pt    = subjetPt [subleading_index];
        SubLeadingSubJet_Eta   = subjetEta[subleading_index];
        SubLeadingSubJet_Phi   = subjetPhi[subleading_index];
        SubLeadingSubJet_E     = subjetE  [subleading_index];
//        SubLeadingSubJet_SDCSV = FATsubjetSDCSV[goodFATJetID][subleading_index];
        SubLeadingSubJet_SDCSV = AK8PuppisubjetSDCSV[goodFATJetID][subleading_index];

    }


    // ADDjet

    Int_t goodADDjetID = -1;

    for(int iADD=0 ; iADD<ADDnJet ; iADD++){

      TLorentzVector* thisADDjet = (TLorentzVector*) ADDjetP4 ->At(iADD);
      double dR_ADDFat = thisADDjet->DeltaR(*thisJet);

      if(dR_ADDFat<0.3){
        goodADDjetID = iADD; break;
      }
    }

//    Double_bTagger = ADDjet_DoubleSV[goodADDjetID];
    Double_bTagger_ADDjet = ADDjet_DoubleSV[goodADDjetID];

    // FatJet 

    Int_t good_FATjet_ID = -1;

    for(int iFAT=0 ; iFAT<FATnJet ; iFAT++){

      TLorentzVector* thisFATjet = (TLorentzVector*) FATjetP4 ->At(iFAT);
      double dR_FatJet = thisFATjet->DeltaR(*thisJet);

      if(dR_FatJet<0.3){
        good_FATjet_ID = iFAT; break;
      }
    }

    Jet_PRmass           = FATjetPRmass[good_FATjet_ID];
    Jet_PRmass_corrected = FATjetPRmassCorr[good_FATjet_ID];


    // ZH invariant mass

    TLorentzVector l4_ZH = l4_Z + ( *thisJet );     
    ZHmass = l4_ZH.M() ;


    // -----------------------------------------------
    new_tree->Fill();


  } // ------------------ end of event loop ------------------

  cout<<"counter_0: "<< counter_0 << endl;
  cout<<"counter_1: "<< counter_1 << endl;
  cout<<"counter_2: "<< counter_2 << endl;
  cout<<"counter_3: "<< counter_3 << endl;


  fprintf(stderr, "Processed all events\n");


  h_totalEvents     ->Write("totalEvents");
  new_tree->Write();  

  outFile->Write();  
  delete outFile;


}




