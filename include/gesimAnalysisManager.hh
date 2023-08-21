#ifndef gesimAnalysisManager_h
#define gesimAnalysisManager_h 1

//---------------------------------------------------------------------------
//
// ClassName:   gesimAnalysisManager
//
// Description: Singleton class to hold analysis parameters and build histograms.
//              User cannot access to the constructor.
//              The pointer of the only existing object can be got via
//              gesimAnalysisManager::GetInstance() static method.
//              The first invokation of this static method makes
//              the singleton object.
//
//----------------------------------------------------------------------------
//

#include "globals.hh"
#include "gesimEnergyDeposition.hh"
#include "gesimAnalysisMessenger.hh"
#include "G4Event.hh"
#include <vector>
#include "CLHEP/Random/RandGauss.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class gesimHisto;

class gesimAnalysisManager
{

public:
  // With description

  static gesimAnalysisManager* GetInstance();
  static void Dispose();

private:

  gesimAnalysisManager();
  ~gesimAnalysisManager();

public: // Without description

  void BookHisto();

  void BeginOfRun();
  void EndOfRun(G4int);

  void BeginOfEvent();
  void EndOfEvent(const G4Event* evt);

  void AddParticle(G4int, G4int, G4int, G4int, G4double, G4double, G4double, G4double);
  //void AddIsotope(G4double, G4double, G4double);
  void AddEnergy(G4double, G4double, G4double, G4int);
  void AddDecayProduct(G4double pid,G4int Z, G4int A, G4double energy, G4double time,G4double weight);

  void SetVerbose(G4int val) {fVerbose = val;};
  G4int GetVerbose() const {return fVerbose;};

  void SetFirstEventToDebug(G4int val) {fNEvt1 = val;};
  G4int FirstEventToDebug() const {return fNEvt1;};
  void SetLastEventToDebug(G4int val) {fNEvt2 = val;};
  G4int LastEventToDebug() const {return fNEvt2;};

  void SetMaxEnergyforHisto(G4double val) {fHistEMax = val;};
  G4double  GetMaxEnergyforHisto() const {return fHistEMax;};
  void SetMinEnergyforHisto(G4double val) {fHistEMin = val;};
  G4double  GetMinEnergyforHisto() const {return fHistEMin;};
  void SetNumBinforHisto(G4int val) {fHistNBin = val;};
  G4int  GeNumBinforHisto() const {return fHistNBin;};

  void SetThresholdEnergyforTarget(G4double val) {fTargetThresE = val;};
  G4double GetThresholdEnergyforTarget () const {return fTargetThresE;};
  void SetThresholdEnergyforDetector(G4double val) {fDetectorThresE = val;};
  G4double GetThresholdEnergyforDetector () const {return fDetectorThresE;};
  void SetPulseWidth(G4double val) {fPulseWidth = val;};
  G4double GetPulseWidth () const {return fPulseWidth;};

  G4double SimulateEnergyResolution(G4double energy);
  void SelectDetector(G4String s);
  void SaveGen(G4String s);

private:

  // MEMBERS
  static gesimAnalysisManager* fManager;
  gesimAnalysisMessenger * fAMessenger;

  G4int fVerbose;
  G4int fNEvt1;
  G4int fNEvt2; 

  G4double fHistEMax;
  G4double fHistEMin;
  G4int fHistNBin;

  G4double fTargetThresE;
  G4double fDetectorThresE;
  G4double fPulseWidth;

  // energy depositions for an event
  std::vector <gesimEnergyDeposition> fEdepo;
  //
  gesimHisto* fHisto;
  CLHEP::RandGauss* randGauss;
  G4String fDetId;
  G4String fSaveGen;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
