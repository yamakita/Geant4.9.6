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
// Authors: S. Guatelli and M. G. Pia, INFN Genova, Italy
// 
// Based on code developed by the undergraduate student G. Guerrieri 
// Note: this is a preliminary beta-version of the code; an improved 
// version will be distributed in the next Geant4 public release, compliant
// with the design in a forthcoming publication, and subject to a 
// design and code review.
//
#include "G4MIRDThyroid.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4HumanPhantomMaterial.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4VSolid.hh"
#include "G4SubtractionSolid.hh"

G4MIRDThyroid::G4MIRDThyroid()
{
}

G4MIRDThyroid::~G4MIRDThyroid()
{
}

G4VPhysicalVolume* G4MIRDThyroid::ConstructThyroid(G4VPhysicalVolume* mother, G4String sex, G4bool sensitivity)
{
 G4HumanPhantomMaterial* material = new G4HumanPhantomMaterial();
 G4Material* soft = material -> GetMaterial("soft_tissue");
 delete material;

 G4double z= 4.20*cm; //c thickness = c,  
 G4double rmin= 0. * cm;
 G4double rmax= 1.85 *cm; //Rmax
  G4double startphi = 0. * degree;
  G4double deltaphi= 180. * degree; // y< y0

  G4Tubs* LobOfThyroidOut = new G4Tubs("LobOfThyroidOut",
				       rmin, rmax,z/2., 
				       startphi, deltaphi);

  z= 4.50*cm; // c thickness + something
  rmax= 0.83 * cm; //r
  deltaphi= 360. * degree; 
  G4Tubs* LobOfThyroidIn = new G4Tubs("LobOfThyroidIn",
				       rmin, rmax,z/2., 
				      startphi, deltaphi);

  G4double xx = 3.72*cm;
  G4double yy= 3.72*cm;
  G4double zz= 20.00*cm;
  G4Box* SubtrThyroid = new G4Box("SubtrThyroid",
				  xx/2., yy/2., zz/2.);

  // subtraction of the two tubs
  G4SubtractionSolid* FirstThyroid = new G4SubtractionSolid("FirstThyroid",
							    LobOfThyroidOut,
							    LobOfThyroidIn);

  G4RotationMatrix* relative_matrix = new G4RotationMatrix();
  relative_matrix -> rotateX(-50.* degree);

  G4SubtractionSolid* SecondThyroid = new G4SubtractionSolid("SecondThyroid",
							    FirstThyroid,
							    SubtrThyroid,
							    relative_matrix,
							    G4ThreeVector(0.0 *cm,0.0 *cm, 4.20*cm));

 G4RotationMatrix* relative_matrix_2 = new G4RotationMatrix();
  relative_matrix_2 -> rotateX(50.* degree);
 
G4SubtractionSolid* thyroid = new G4SubtractionSolid("SecondThyroid",
						      SecondThyroid,
						      SubtrThyroid,
						      relative_matrix_2,
						      G4ThreeVector(0.0 *cm,0.0 *cm, -5.40*cm));



 G4LogicalVolume* logicThyroid = new G4LogicalVolume(thyroid, soft,
						     "ThyroidVolume",
						     0, 0, 0);

 G4RotationMatrix* rm = new G4RotationMatrix();
  
 rm -> rotateZ(180.*degree);
 
  G4VPhysicalVolume* physThyroid = new G4PVPlacement(rm,
						     G4ThreeVector(0.0*cm,-3.91*cm, -5.925*cm),//y0
      			       "physicalThyroid",
  			       logicThyroid,
			       mother,
			       false,
			       0);

  // Sensitive Body Part
  if (sensitivity==true)
  { 
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    logicThyroid->SetSensitiveDetector( SDman->FindSensitiveDetector("BodyPartSD") );
  }

  // Visualization Attributes
  G4VisAttributes* ThyroidVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  ThyroidVisAtt->SetForceSolid(true);
  logicThyroid->SetVisAttributes(ThyroidVisAtt);

  G4cout << "Thyroid created for "<<sex << " !!!!!!" << G4endl;

  // Testing Thyroid Volume
  G4double ThyroidVol = logicThyroid->GetSolid()->GetCubicVolume();
  G4cout << "Volume of Thyroid = " << ThyroidVol/cm3 << " cm^3" << G4endl;
  
  // Testing Thyroid Material
  G4String ThyroidMat = logicThyroid->GetMaterial()->GetName();
  G4cout << "Material of Thyroid = " << ThyroidMat << G4endl;
  
  // Testing Density
  G4double ThyroidDensity = logicThyroid->GetMaterial()->GetDensity();
  G4cout << "Density of Material = " << ThyroidDensity*cm3/g << " g/cm^3" << G4endl;

  // Testing Mass
  G4double ThyroidMass = (ThyroidVol)*ThyroidDensity;
  G4cout << "Mass of Thyroid = " << ThyroidMass/gram << " g" << G4endl;

  
  return physThyroid;
}
