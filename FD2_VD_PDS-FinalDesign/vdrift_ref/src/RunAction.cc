// Arapuca simulation
// Authors: L. Paulucci & F. Marinho
// Date: 20th September 2016
//
// Added modifications should be reported in arapuca.cc header comments

#include "G4UImanager.hh"
#include "Randomize.hh"

#include "RunAction.hh"
#include "g4root.hh"

RunAction::RunAction(DetectorConstruction* det) 
:fDetector(det)
{   
  fSaveRndm = 0;  
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run*)
{  
  
  // Get/create analysis manager
  G4cout << "##### Create analysis manager " << "  " << this << G4endl;
  
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  
  G4cout << "Using " << man->GetType() << " analysis manager" << G4endl;

  // Open an output file
  man->OpenFile("arapuca");
  man->SetFirstNtupleId(1);

  //Declare ntuples
  //
  // Create 1st ntuple (id = 1)
  //
  man->CreateNtuple("ntuple", "data");
  man->CreateNtupleIColumn("evt");
  man->CreateNtupleIColumn("pdg");
  man->CreateNtupleIColumn("id");
  man->CreateNtupleIColumn("mid");
  man->CreateNtupleDColumn("x");
  man->CreateNtupleDColumn("y");
  man->CreateNtupleDColumn("z");
  man->CreateNtupleDColumn("de");
  man->CreateNtupleDColumn("e");
  man->CreateNtupleIColumn("vol");
  man->CreateNtupleIColumn("volrep");
  man->CreateNtupleIColumn("mvol");
  man->CreateNtupleIColumn("mvolrep");
  man->CreateNtupleIColumn("inter");
  man->CreateNtupleDColumn("time");
  man->FinishNtuple();

  G4int nvols = 710;
  // Histogram to get light yield maps
  G4int hv_id = man->CreateH1("hv","",nvols+1,-0.5,nvols+0.5);
  // Uniformity histograms
  G4int hdX_id = man->CreateH1("hdX","",40,-0.16875,0.16875);
  G4int hdY_id = man->CreateH1("hdY","",40,-0.1625,0.1625);
  G4int hdZ_id = man->CreateH1("hdZ","",40,-0.125,0.125);

  // save Rndm status
  if (fSaveRndm > 0)
    { 
      CLHEP::HepRandom::showEngineStatus();
      CLHEP::HepRandom::saveEngineStatus("beginOfRun.rndm");
    }
 
  fNumEvent = 0;
  
}

void RunAction::EndOfRunAction(const G4Run* /*aRun*/)
{     
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  
  // save Rndm status
  if (fSaveRndm == 1)
  { 
    CLHEP::HepRandom::showEngineStatus();
    CLHEP::HepRandom::saveEngineStatus("endOfRun.rndm");
  }   
  
  G4cout << G4endl;    
        
  man->Write();
  man->CloseFile();
  
  delete G4AnalysisManager::Instance();

}
