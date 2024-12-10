#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "CLHEP/Random/Random.h"
#include "gesimPrimaryGeneratorAction.hh"
#include "G4HEPEvtInterface.hh"
#include "gesimDetectorConstruction.hh"
#include "G4Tubs.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4HEPEvtInterface.hh"
#include "G4SystemOfUnits.hh"
#include "G4Neutron.hh"
#include "G4Gamma.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>

G4VPrimaryGenerator* HEPEvt;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

gesimPrimaryGeneratorAction::gesimPrimaryGeneratorAction(gesimDetectorConstruction* gesimDC):gesimDetector(gesimDC),fParticleGun(0){
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);   
}

gesimPrimaryGeneratorAction::~gesimPrimaryGeneratorAction(){
  delete fParticleGun;
}

std::vector<double> loadArrayFromFile(const std::string& filename) {
    std::vector<double> array;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return array;
    }

    double value;
    while (file >> value) {
        array.push_back(value);
    }

    file.close();
    return array;
}






gesimPrimaryGeneratorAction::BaccGeneratorAmBeSOURCES4A()
    : neutron(G4Neutron::Definition())
    , gamma(G4Gamma::Definition())
    , neutronCDFState0()
    , neutronEnergyState0()
    , neutronCDFState1()
    , neutronEnergyState1()
    , neutronCDFState2()
    , neutronEnergyState2()
    , gammaCDF()
    , gammaAngle()
{
    name = "AmBeSOURCES4A";
    activityMultiplier = 1;

    //std::string BACC_ROOT(std::getenv("BACC_ROOT"));
    // Neutron group n0
    // ******************************************************************************************************
    // Neutron group n0
    std::string neutronCDF0File = "/mnt/raid5/users/amslivar/AmBe_work/AmBe_v2/AmBe_neutron_gamma_spec/AmBe_SOURCES4A_n0_neutronCDF.dat";
    std::string neutronEnergy0File = "/mnt/raid5/users/amslivar/AmBe_work/AmBe_v2/AmBe_neutron_gamma_spec/AmBe_SOURCES4A_n0_neutronEnergy.dat";

    neutronCDFState0 = loadArrayFromFile(neutronCDF0File);
    neutronEnergyState0 = loadArrayFromFile(neutronEnergy0File);

    // Neutron group n1
    std::string neutronCDF1File = "/mnt/raid5/users/amslivar/AmBe_work/AmBe_v2/AmBe_neutron_gamma_spec/AmBe_SOURCES4A_n1_neutronCDF.dat";
    std::string neutronEnergy1File = "/mnt/raid5/users/amslivar/AmBe_work/AmBe_v2/AmBe_neutron_gamma_spec/AmBe_SOURCES4A_n1_neutronEnergy.dat";

    neutronCDFState1 = loadArrayFromFile(neutronCDF1File);
    neutronEnergyState1 = loadArrayFromFile(neutronEnergy1File);

    // Neutron group n2
    std::string neutronCDF2File =  "/mnt/raid5/users/amslivar/AmBe_work/AmBe_v2/AmBe_neutron_gamma_spec/AmBe_SOURCES4A_n2_neutronCDF.dat";
    std::string neutronEnergy2File =  "/mnt/raid5/users/amslivar/AmBe_work/AmBe_v2/AmBe_neutron_gamma_spec/AmBe_SOURCES4A_n2_neutronEnergy.dat";

    neutronCDFState2 = loadArrayFromFile(neutronCDF2File);
    neutronEnergyState2 = loadArrayFromFile(neutronEnergy2File);

    // Gamma angle for the neutron group n2
    std::string gammaCDFFile = "/mnt/raid5/users/amslivar/AmBe_work/AmBe_v2/AmBe_neutron_gamma_spec/AmBe_SOURCES4A_gammaCDF.dat";
    std::string gammaAngleFile = "/mnt/raid5/users/amslivar/AmBe_work/AmBe_v2/AmBe_neutron_gamma_spec/AmBe_SOURCES4A_gammaAngle.dat";

    gammaCDF =loadArrayFromFile(gammaCDFFile);
    gammaAngle = loadArrayFromFile(gammaAngleFile);
}


void gesimPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){

  float unit=1*CLHEP::mm;
  float ssShieldTopVoidHeight = 0.89*unit;
  float ssShieldBottomVoidHeight = 0.25267*unit; // ssShieldInnerDia/2*tg(22.5deg) if the bore was made with 135deg point angle drill bit
  float ssShieldCentralVoidHeight = 5.11*unit-ssShieldBottomVoidHeight;
  float ssShieldHeight = 6.50*unit;

  float wShieldHeight = 4.19*unit;
  float wShieldBoreHeight = 4*unit;

  float wWireHeight = 1.8*unit;
  float wWireTop = 0.3*unit;

  float srcCoreHeight = wShieldBoreHeight-(wWireHeight-wWireTop);
  float srcCoreOffset = 0.5*wShieldHeight - (wWireHeight-wWireTop)-0.5*srcCoreHeight;

  // W Capsule dimensions 
  float wCapsCapHeight = 21*unit;
  float wCapsBaseOuterDia = 18*unit;
  float wCapsBaseHeight = 16*unit;
  float wCapsBaseBottomHeight = 9*unit;
  float wCapsBaseBoreHeight = 7.62*unit;
  float wCapsBaseBoreOffset = 0*unit;

  static const G4double inch = 2.54*cm;
  float fEndCapLength = 159.0*mm;
  float fHeadLength = (81.5-13.0)*mm; ; 
  float fDetectorEnclosureLength = fEndCapLength + fHeadLength;
  float fNeckOffset = 30.0*mm; // distance from neck center to "back" of head
  float fNeckLocation = 8.125*inch; //distance from inner cavity center to neck center.

  double endCapFace = fDetectorEnclosureLength - fNeckOffset - fNeckLocation;

  auto srcGenOffsetZ = wCapsBaseHeight-0.5*wCapsBaseBottomHeight-wCapsBaseBoreHeight+0.5*ssShieldHeight+0.5*ssShieldHeight-ssShieldTopVoidHeight-0.5*ssShieldCentralVoidHeight+srcCoreOffset;

  auto srcGenOffsetY = wCapsBaseBoreOffset;//when source is in the outer W capsule
  
  auto theta = 90.*deg;//angle of the outer W capsule rotation wrt the outer W capsule z axis. 270 deg for "near", 90 deg for "far" orientation
  // for symmetric outer W capsule, this does not matter 
  /*
  G4ParticleDefinition *gammaParticle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");

  CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
  CLHEP::HepRandom::setTheSeed((unsigned)clock());

  fParticleGun->SetParticleDefinition(gammaParticle);

  auto sourcePV  = gesimDetector->GetSrcCorePV();
  auto sourceLV = sourcePV->GetLogicalVolume();
  auto sourceS = sourceLV->GetSolid();
  */
  auto srcCtrInCvtyCord =  G4ThreeVector(endCapFace+0.5*wCapsBaseOuterDia,0*CLHEP::mm,-0.5*wCapsCapHeight);;
  

  auto source = (G4Tubs*)sourceS; //source is a cylinder 
  auto rSource = source->GetOuterRadius();
  auto hSource=source->GetZHalfLength();
  auto rSourceRnd = rSource*G4UniformRand();
  auto alpha = CLHEP::twopi*G4UniformRand();  // generating a random angle between 0 and 2pi  
  
  auto xRnd = rSourceRnd*std::cos(alpha)+srcGenOffsetY*std::sin(theta); //srcGenOffsetY is 0 for symmetric outer W capsule
  auto yRnd = rSourceRnd*std::sin(alpha)+srcGenOffsetY*std::cos(theta);
  auto zRnd = hSource*(2*G4UniformRand()-1)+srcGenOffsetZ;

  G4ThreeVector position(srcCtrInCvtyCord.getX()+xRnd, srcCtrInCvtyCord.getY()+yRnd, srcCtrInCvtyCord.getZ()+zRnd);

  //fParticleGun->SetParticlePosition(position);
  auto beta = CLHEP::twopi*G4UniformRand(); 
  float dirZ = -1.0 + 2*G4UniformRand();
  auto dirR = sqrt(1.-dirZ*dirZ);
  auto dirX = dirR*std::cos(beta);
  auto dirY = dirR*std::sin(beta); 
  float aEnergy = 4438; //keV 
  //fParticleGun->SetParticleEnergy(aEnergy*keV);
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(dirX,dirY,dirZ));
  //fParticleGun->GeneratePrimaryVertex(anEvent);



    G4double probability_state;
    //4double time = (firstNode->timeOfEvent) / ns;
    G4ThreeVector pos = srcCtrInCvtyCord;
    G4double singleNeutronEnergy = 0.0;

    //n0->27%, n1->62.7% and n2->10.3% (individual probabilities)
    // Down below are the cumulative probabilities
    G4double AmBe_n0 = 0.270;
    G4double AmBe_n1 = 0.897;

    // First determine the neutron group
    probability_state = G4UniformRand();
    G4int state = 0;

    // when state =0 or 2, neutron only 
    // when state =1, neutron and gamma

    if (probability_state < AmBe_n0)
    {
        state = 0;
    }
    else if (probability_state < AmBe_n1)
    {
        state = 1;
    }
    else
    {
        state = 2;
    }

    // Choose the neutron energy for the particular state
    singleNeutronEnergy = GetNeutronEnergy(state);

    // now generate a neutron (Adopted from AmBe generator)
    particleGun->GetCurrentSource()->SetParticleDefinition(neutron);
    particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
    particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(GetRandomDirection());
    particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(singleNeutronEnergy * MeV);
    // Set the time (adopted from Sally's AlphaN generator)
    /*
    SetParentTime(time);
    if (baccManager->GetGenTimeZero())
    {
        particleGun->GetCurrentSource()->SetParticleTime(0 * ns);
    }
    else
    {
        particleGun->GetCurrentSource()->SetParticleTime(time * ns);
    }
    */
    particleGun->GeneratePrimaryVertex(event);
    //baccManager->AddPrimaryParticle(GetParticleInfo(particleGun));

    //Generate correlated gamma only for 1st excited state (two gamma emission for the 2nd state is very low, and thus ignored)
    if (state == 1)
    {

        particleGun->GetCurrentSource()->SetParticleDefinition(gamma);
        particleGun->GetCurrentSource()->GetPosDist()->SetCentreCoords(pos);
        particleGun->GetCurrentSource()->GetEneDist()->SetMonoEnergy(4439. * keV);
        particleGun->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(GetRandomDirection());

        particleGun->GeneratePrimaryVertex(event);
        //baccManager->AddPrimaryParticle(GetParticleInfo(particleGun));
    }



}


//------++++++------++++++------++++++------++++++------++++++------++++++------
//               GetNeutronEnergy()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double gesimPrimaryGeneratorAction::GetNeutronEnergy(G4int state){
    G4double energy = 0.;
    if (state == 0)
    {
        energy = EnergySampler(neutronCDFState0.data(), neutronEnergyState0.data(), neutronCDFState0.size());
    }
    else if (state == 1)
    {
        energy = EnergySampler(neutronCDFState1.data(), neutronEnergyState1.data(), neutronCDFState1.size());
    }
    else
    {
        energy = EnergySampler(neutronCDFState2.data(), neutronEnergyState2.data(), neutronCDFState2.size());
    }
    return energy;
}



//------++++++------++++++------++++++------++++++------++++++------++++++------
//               EnergySampler()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double gesimPrimaryGeneratorAction::EnergySampler(const G4double* ptr_NeutronCDF, const G4double* ptr_NeutronEnergy, unsigned int size)
{

    G4double prob = G4UniformRand();
    G4int indexLow = 0;
    G4int indexHigh = size - 1;

    while (!(*(ptr_NeutronCDF + indexLow + 1) > prob && *(ptr_NeutronCDF + indexHigh - 1) < prob))
    {
        if (*(ptr_NeutronCDF + (indexLow + indexHigh) / 2) < prob)
        {
            indexLow = (indexLow + indexHigh) / 2;
        }
        else
        {
            indexHigh = (indexLow + indexHigh) / 2;
        }
    }

    G4double split = (prob - *(ptr_NeutronCDF + indexLow)) / (*(ptr_NeutronCDF + indexHigh) - *(ptr_NeutronCDF + indexLow));
    G4double energy = *(ptr_NeutronEnergy + indexLow) + split * (*(ptr_NeutronEnergy + indexHigh) - *(ptr_NeutronEnergy + indexLow));

    return (energy);
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               GetGammaAngle()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double gesimPrimaryGeneratorAction::GetGammaAngle()
{
    //theta is the polar angle between the two gammas, taking the direction of the first gamma to be z-axis
    // i.e., theta=0, same direction (theta=pi, back-to-back)
    G4double theta = 0.;
    theta = AngleSampler(gammaCDF.data(), gammaAngle.data(), gammaCDF.size());
    return theta;
}

//------++++++------++++++------++++++------++++++------++++++------++++++------
//               AngleSampler()
//------++++++------++++++------++++++------++++++------++++++------++++++------
G4double gesimPrimaryGeneratorAction::AngleSampler(const G4double* ptr_GammaCDF, const G4double* ptr_GammaAngle, unsigned int size)
{

    G4double prob = G4UniformRand();
    G4int indexLow = 0;
    G4int indexHigh = size - 1;

    while (!(*(ptr_GammaCDF + indexLow + 1) > prob && *(ptr_GammaCDF + indexHigh - 1) < prob))
    {
        if (*(ptr_GammaCDF + (indexLow + indexHigh) / 2) < prob)
        {
            indexLow = (indexLow + indexHigh) / 2;
        }
        else
        {
            indexHigh = (indexLow + indexHigh) / 2;
        }
    }

    G4double split = (prob - *(ptr_GammaCDF + indexLow)) / (*(ptr_GammaCDF + indexHigh) - *(ptr_GammaCDF + indexLow));
    G4double angle = *(ptr_GammaAngle + indexLow) + split * (*(ptr_GammaAngle + indexHigh) - *(ptr_GammaAngle + indexLow));

    return (angle);
}






