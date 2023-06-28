#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "TF1.h"
#include "TMath.h"
#include "TFormula.h"
#include "TH1D.h"
#include "TRandom3.h"
#include "g4root.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  fParticleGun  = new G4ParticleGun(1);

}

PrimaryGeneratorAction::PrimaryGeneratorAction(double x, double y, double z)
{
  fParticleGun  = new G4ParticleGun(1);
  x0 = x;
  y0 = y;
  z0 = z;
    
}// Voxel placed at (x, y, z). Those coordinates are the input given to run the standalone simulation

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4int i=0;
  G4double theta,phi;
  G4double test;
  double Xlimit, Ylimit, Zlimit; //Voxel half size, in meters
  
  Xlimit = 6.75;
  Ylimit = 3.25;
  Zlimit = 5.00;
    
  //Generating random photons throughout the voxel placed at (x0, y0, z0).
  xi = CLHEP::RandFlat::shoot(x0-Xlimit,x0+Xlimit);
  yi = CLHEP::RandFlat::shoot(y0-Ylimit,y0+Ylimit);
  zi = CLHEP::RandFlat::shoot(z0-Zlimit,z0+Zlimit);
  //zi = CLHEP::RandFlat::shoot(z0,z0+Zlimit);
  
  phi = CLHEP::twopi*CLHEP::RandFlat::shoot(0.0,1.0)*rad; 

  while(i==0){
     theta = CLHEP::RandFlat::shoot(0.0,CLHEP::pi)*rad;
     test = CLHEP::RandFlat::shoot(0.0,1.0);
     if(sin(theta)>test)i++;
  }

  //Particle direction
  G4double kx, ky, kz; //components
  
  kx=cos(phi)*sin(theta);
  ky=sin(phi)*sin(theta);
  kz=cos(theta);
  G4ThreeVector dir_vec (kx,ky,kz);

  //Polarization 
  G4ThreeVector polar = Polarisation(dir_vec);
  
  //fParticleGun->SetParticleEnergy(9.68*eV); //Ar light
  fParticleGun->SetParticleEnergy(7.08*eV); //Xe light
  fParticleGun->SetParticleMomentumDirection(dir_vec);
  fParticleGun->SetParticlePosition(G4ThreeVector(xi*m,yi*m,zi*m)); //Random photons
  //fParticleGun->SetParticlePosition(G4ThreeVector(x0*cm,y0*cm,z0*cm)); //All photons at voxel center
  fParticleGun->SetParticlePolarization(polar); 
  
  G4ParticleDefinition* particle=
    //G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    G4ParticleTable::GetParticleTable()->FindParticle("opticalphoton");
  
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->GeneratePrimaryVertex(anEvent);
  
  //Analysis manager                                                          
  G4AnalysisManager* man = G4AnalysisManager::Instance();                 
                                                                              
  //Histograms to check the uniformity in X, Y and Z directions                
  G4int hdX_id = man->GetH1Id("hdX");                                          
  man->FillH1(hdX_id,xi-x0);                                                   
  G4int hdY_id = man->GetH1Id("hdY");                                        
  man->FillH1(hdY_id,yi-y0);                                                 
  G4int hdZ_id = man->GetH1Id("hdZ");                                        
  man->FillH1(hdZ_id,zi-z0); 
}

G4ThreeVector PrimaryGeneratorAction::Polarisation(G4ThreeVector d){

  if(d.mag()!=1.0) d = d.unit();

  G4ThreeVector a = d.orthogonal();
  G4ThreeVector b = a.cross(d); 
  a = a.unit();
  b = b.unit();
  G4double theta = CLHEP::twopi*G4UniformRand();
  return cos(theta)*a + sin(theta)*b;

}

G4ThreeVector PrimaryGeneratorAction::TransversePosition(G4ThreeVector d, double r){

  if(d.mag()!=1.0) d = d.unit();

  G4ThreeVector a = d.orthogonal();
  G4ThreeVector b = a.cross(d); 
  a = a.unit();
  b = b.unit();
  G4double theta = CLHEP::twopi*G4UniformRand();
  return r*(cos(theta)*a + sin(theta)*b);

}


void PrimaryGeneratorAction::DepthSampling(){


  

}

void PrimaryGeneratorAction::GetEMShowerParameters(){

  
}
