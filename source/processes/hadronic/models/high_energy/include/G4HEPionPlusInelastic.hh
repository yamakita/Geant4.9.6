//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
//
// G4 Gheisha High Energy model class -- header file
// H. Fesefeldt, RWTH Aachen 23-October-1996
// A prototype of the Gheisha High Energy collision model.

#ifndef G4HEPionPlusInelastic_h
#define G4HEPionPlusInelastic_h 1

// Class description:
// High energy parameterized model for pi+ inelastic scattering.  This
// class is responsible for producing the final state of the interaction and
// is typically valid for incident pi+ energies above 45 GeV.  This
// physics may be invoked by registering an instance of the class with
// G4PionPlusInelasticProcess in the user's physics list.
//
// This class is derived from G4HEInelastic which in turn is derived from
// G4HadronicInteraction.

// Class Description - End

#include <CLHEP/Units/SystemOfUnits.h>

#include "G4HEInelastic.hh"

class G4HEPionPlusInelastic : public G4HEInelastic  
{
  public:  // with description 
    G4HEPionPlusInelastic() : G4HEInelastic("G4HEPionPlusInelastic")
    {
      vecLength = 0;
      theMinEnergy = 45*CLHEP::GeV;
      theMaxEnergy = 10*CLHEP::TeV;
      MAXPART      = 2048;
      verboseLevel = 0; 
      G4cout << "WARNING: model G4HEPionPlusInelastic is being deprecated and will\n"
             << "disappear in Geant4 version 10.0"  << G4endl;  
    }

    ~G4HEPionPlusInelastic() {};

    virtual void ModelDescription(std::ostream&) const;
         
    G4int vecLength;
        
    G4HadFinalState* ApplyYourself(const G4HadProjectile& aTrack,
                                   G4Nucleus& targetNucleus);

    G4int GetNumberOfSecondaries() {return vecLength;}

    void FirstIntInCasPionPlus(G4bool& inElastic,
                               const G4double availableEnergy,
                               G4HEVector pv[],
                               G4int& vecLen, 
                               const G4HEVector& incidentParticle,
                               const G4HEVector& targetParticle,
                               const G4double atomicWeight);
};
#endif

