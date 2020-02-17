#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

// Create Matrix Unity
TGeoRotation *fGlobalRot = new TGeoRotation();

// Create a null translation
TGeoTranslation *fGlobalTrans = new TGeoTranslation();
TGeoRotation *fRefRot = NULL;

TGeoManager*  gGeoMan = NULL;

Double_t fThetaX = 0.;
Double_t fThetaY = 0.;
Double_t fThetaZ = 0.;
Double_t fPhi   = 0.;
Double_t fTheta = 0.;
Double_t fPsi   = 0.;
Double_t fX = 0.;
Double_t fY = 0.;
Double_t fZ = 1000.;
Bool_t fLocalTrans = kFALSE;
Bool_t fLabTrans = kTRUE;


TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans *fRef);


void VETO_Geometry()																	// NOTE: Change by C. A. Douma!
{
  // =================================================================================
  // NOTE: This section is added by C. A. Douma.
  
  // The function will no longer take inputs or give outputs. Inputs are
  // taken from the Inputs.root file. Hence in this section we will collect the 
  // inputs we need to build the geometry. The rest of the code will be
  // unchanged except for the adjustments we need to make to correctly process
  // the input parameters.
  
  // We begin with connecting to the Inputs.root file:
  TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");
  TH1D* Doubles = (TH1D*) inputs->Get("Doubles");
  TH1I* Integers = (TH1I*) inputs->Get("Integers");
  
  // Now we get our inputs:
  Double_t X_position = Doubles->GetBinContent(121);
  Double_t Y_position = Doubles->GetBinContent(122);
  Double_t Z_position = Doubles->GetBinContent(123);
  
  Double_t Rotation_X = Doubles->GetBinContent(124);
  Double_t Rotation_Y = Doubles->GetBinContent(125);
  Double_t Rotation_Z = Doubles->GetBinContent(126);
  
  Double_t Scintillator_Barlength = Doubles->GetBinContent(127);    // dimensions are including gabs, foil & wrapping tape!
  Double_t Scintillator_BarWidth = Doubles->GetBinContent(128);
  Double_t Scintillator_BarThickness = Doubles->GetBinContent(129);
  Double_t Gab_Size = Doubles->GetBinContent(130);
  Double_t Foil_Thickness = Doubles->GetBinContent(131);
  Double_t Tape_Thickness = Doubles->GetBinContent(132);
  
  Int_t N_Single_PLanes = Integers->GetBinContent(3);
  //Int_t N_Bars_VerticalPlanes = Integers->GetBinContent(4);
  //Int_t N_Bars_HorizontalPlanes = Integers->GetBinContent(5);
  Int_t N_Bars_OnePlane = Integers->GetBinContent(4);
  
  // That's it.
  // ================================================================================

  fGlobalTrans->SetTranslation(0.0,0.0,0.0);

  Double_t neuLAND_paddle_dimx   = Scintillator_Barlength/2.0;   // half of the length [cm]								// NOTE: Change by C. A. Douma!
  Double_t neuLAND_paddle_dimy   = Scintillator_BarWidth/2.0;    // half of the width [cm]								// NOTE: Change by C. A. Douma!
  Double_t neuLAND_paddle_dimz   = Scintillator_BarThickness/2.0;    // half of the depth [cm]								// NOTE: Change by C. A. Douma!
  Double_t neuLAND_depth_dim     = neuLAND_paddle_dimz*((Int_t) N_Single_PLanes);   // half detector depth [cm]						// NOTE: Change by C. A. Douma!
  Double_t neuLAND_gap_dim       = Gab_Size/2.0;   // total detector depth [cm]										// NOTE: Change by C. A. Douma!
  Double_t neuLAND_wrapping1_dim = Foil_Thickness;   // thickness of wrapping material [cm]								// NOTE: Change by C. A. Douma!
  Double_t neuLAND_wrapping2_dim = Tape_Thickness;   // thickness of wrapping material [cm]								// NOTE: Change by C. A. Douma!
  neuLAND_paddle_dimy = neuLAND_paddle_dimy - neuLAND_gap_dim - Foil_Thickness - Tape_Thickness;							// NOTE: Change by C. A. Douma!
  neuLAND_paddle_dimz = neuLAND_paddle_dimz - neuLAND_gap_dim - Foil_Thickness - Tape_Thickness;							// NOTE: Change by C. A. Douma!
  

  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media_r3b.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------


  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = "../../Data/Geometry/VETO.geo.root";												// NOTE: Change by C. A. Douma!
  // --------------------------------------------------------------------------


  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mBC408      = geoMedia->getMedium("BC408");
  if ( ! mBC408 ) Fatal("Main", "FairMedium BC408 not found");
  geoBuild->createMedium(mBC408);
  TGeoMedium* pMed37 = gGeoMan->GetMedium("BC408");
  if ( ! pMed37 ) Fatal("Main", "Medium BC408 not found");

  FairGeoMedium* mCH2      = geoMedia->getMedium("CH2");
  if ( ! mCH2 ) Fatal("Main", "FairMedium CH2 not found");
  geoBuild->createMedium(mCH2);
  TGeoMedium* pMed38 = gGeoMan->GetMedium("CH2");
  if ( ! pMed38 ) Fatal("Main", "Medium CH2 not found");

  FairGeoMedium* mAl      = geoMedia->getMedium("aluminium");
  if ( ! mAl ) Fatal("Main", "FairMedium aluminium not found");
  geoBuild->createMedium(mAl);
  TGeoMedium* pMedAl = gGeoMan->GetMedium("aluminium");
  if ( ! pMedAl ) Fatal("Main", "Medium aluminium not found");
  // --------------------------------------------------------------------------



  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("NVETO_SEGgeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);
  // --------------------------------------------------------------------------



  Double_t tx,ty,tz;
  //------------------ BC408 paddles -----------------------------------------
  TGeoVolume *paddle_h_box5_seg = gGeoManager->MakeBox("paddle_h_box5_seg", pMed37,
						  neuLAND_paddle_dimx,
						  neuLAND_paddle_dimy,
						  neuLAND_paddle_dimz);

  //------------------ wrapping Alu------------------------------------------
  TGeoShape* paddle_h_box1_seg = new TGeoBBox("paddle_h_box1_seg",
					 neuLAND_paddle_dimx, 
					 neuLAND_paddle_dimy + neuLAND_wrapping1_dim, 
					 neuLAND_paddle_dimz + neuLAND_wrapping1_dim);
  TGeoShape* paddle_h_box2_seg = new TGeoBBox("paddle_h_box2_seg",
					 neuLAND_paddle_dimx, 
					 neuLAND_paddle_dimy, 
					 neuLAND_paddle_dimz);
  
  // Create a composite shape
  TGeoCompositeShape *wrapping1 = new TGeoCompositeShape("diffbox", "paddle_h_box1_seg - paddle_h_box2_seg");
  TGeoVolume *bvol1 = new TGeoVolume("wrapping1", wrapping1, pMedAl);

  //------------------ wrapping Tape------------------------------------------
  TGeoShape* paddle_h_box3_seg = new TGeoBBox("paddle_h_box3_seg",
					 neuLAND_paddle_dimx,
					 neuLAND_paddle_dimy + neuLAND_wrapping1_dim + neuLAND_wrapping2_dim, 
					 neuLAND_paddle_dimz + neuLAND_wrapping1_dim + neuLAND_wrapping2_dim);
  TGeoShape* paddle_h_box4_seg = new TGeoBBox("paddle_h_box4_seg",
					 neuLAND_paddle_dimx, 
					 neuLAND_paddle_dimy + neuLAND_wrapping1_dim, 
					 neuLAND_paddle_dimz + neuLAND_wrapping1_dim);

  // Create a composite shape
  TGeoCompositeShape *wrapping2 = new TGeoCompositeShape("diffbox", "paddle_h_box3_seg - paddle_h_box4_seg");
  TGeoVolume *bvol2 = new TGeoVolume("wrapping2", wrapping2, pMed38);



  // Make the elementary assembly of the whole structure
  TGeoVolume *aLand = new TGeoVolumeAssembly("AVETO_SEG");

  Double_t total_dimx = neuLAND_paddle_dimx;
  Double_t total_dimy = neuLAND_paddle_dimy +
    neuLAND_wrapping1_dim + 
    neuLAND_wrapping2_dim + 
    neuLAND_gap_dim;
  Double_t total_dimz = neuLAND_paddle_dimz +
    neuLAND_wrapping1_dim +
    neuLAND_wrapping2_dim +
    neuLAND_gap_dim;
  
  //paddles
  TGeoRotation *zeroRotation = new TGeoRotation();
  zeroRotation->RotateX(0.);
  zeroRotation->RotateY(0.);
  zeroRotation->RotateZ(0.);

  TGeoRotation *rot1 = new TGeoRotation();
  rot1->RotateX(0.);
  rot1->RotateY(0.);
  rot1->RotateZ(90.);											                				// NOTE: Change by C. A. Douma!

  Double_t xx = 0.;
  Double_t yy = 0.;
  Double_t zz = 0.;
  
  aLand->AddNode(paddle_h_box5_seg, 1, new TGeoCombiTrans(xx, yy, zz, zeroRotation));
  aLand->AddNode(bvol1, 1, new TGeoCombiTrans(xx, yy, zz, zeroRotation));
  aLand->AddNode(bvol2, 1, new TGeoCombiTrans(xx, yy, zz, zeroRotation));

  TGeoVolume *cell = new TGeoVolumeAssembly("CEL_SEG");
  
  Double_t for_y = total_dimy*1.0*((Int_t) N_Bars_OnePlane);												// NOTE: Change by C. A. Douma!
  Double_t for_z = total_dimz*1.0*((Int_t) N_Single_PLanes);												// NOTE: Change by C. A. Douma!

  Int_t nindex = 0, i = 0;
  tx = 0.;
  tz = -for_z + total_dimz;
  for(tz = -for_z + total_dimz; tz < for_z; tz += total_dimz*2) {											// NOTE: Change by C. A. Douma!
    i += 1;
    for (ty = -for_y + total_dimy; ty < for_y; ty += total_dimy*2) {											// NOTE: Change by C. A. Douma!
      nindex++;
      if (i % 2 == 1) {
        cell->AddNode(aLand, nindex, new TGeoCombiTrans(tx, ty, tz, zeroRotation));
      } else {
        cell->AddNode(aLand, nindex, new TGeoCombiTrans(ty, tx, tz, rot1));
      }
    }
  }

  tx = X_position;																	// NOTE: Change by C. A. Douma!
  ty = Y_position;																	// NOTE: Change by C. A. Douma!
  tz = Z_position;																	// NOTE: Change by C. A. Douma!
  TGeoRotation *global = new TGeoRotation();				                                                		        	// NOTE: Change by C. A. Douma!
  global->RotateX(Rotation_X);														        	// NOTE: Change by C. A. Douma!
  global->RotateY(Rotation_Y);														        	// NOTE: Change by C. A. Douma!
  global->RotateZ(Rotation_Z);															        // NOTE: Change by C. A. Douma!
  TGeoCombiTrans *t0 = new TGeoCombiTrans(tx, ty, tz, global);

  top->AddNode(cell, 1, /*GetGlobalPosition(t0)*/t0);



  // ---------------   Finish   -----------------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  // --------------------------------------------------------------------------
}


TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans *fRef)
{
  if (fLocalTrans == kTRUE ) {
    
    if ( ( fThetaX == 0 )  && ( fThetaY==0 )  && ( fThetaZ == 0 )
	 &&
	 ( fX == 0 ) && ( fY == 0 ) && ( fZ == 0 )
	 )  return fRef;
    
    
    // X axis
    Double_t xAxis[3] = { 1. , 0. , 0. };
    Double_t yAxis[3] = { 0. , 1. , 0. };
    Double_t zAxis[3] = { 0. , 0. , 1. };
    // Reference Rotation
    fRefRot = fRef->GetRotation();
    
    if (fRefRot) {
      Double_t mX[3] = {0.,0.,0.};
      Double_t mY[3] = {0.,0.,0.};
      Double_t mZ[3] = {0.,0.,0.};
      
      fRefRot->LocalToMasterVect(xAxis,mX);
      fRefRot->LocalToMasterVect(yAxis,mY);
      fRefRot->LocalToMasterVect(zAxis,mZ);
      
      Double_t a[4]={ mX[0],mX[1],mX[2], fThetaX };
      Double_t b[4]={ mY[0],mY[1],mY[2], fThetaY };
      Double_t c[4]={ mZ[0],mZ[1],mZ[2], fThetaZ };
      
      ROOT::Math::AxisAngle aX(a,a+4);
      ROOT::Math::AxisAngle aY(b,b+4);
      ROOT::Math::AxisAngle aZ(c,c+4);
      
      ROOT::Math::Rotation3D fMatX( aX );
      ROOT::Math::Rotation3D fMatY( aY );
      ROOT::Math::Rotation3D fMatZ( aZ );
      
      ROOT::Math::Rotation3D  fRotXYZ = (fMatZ * (fMatY * fMatX));
      
      //cout << fRotXYZ << endl;
      
      Double_t fRotable[9]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
      fRotXYZ.GetComponents(
			    fRotable[0],fRotable[3],fRotable[6],
			    fRotable[1],fRotable[4],fRotable[7],
			    fRotable[2],fRotable[5],fRotable[8]
			    );
      TGeoRotation *pRot = new TGeoRotation();
      pRot->SetMatrix(fRotable);
      TGeoCombiTrans *pTmp = new TGeoCombiTrans(*fGlobalTrans,*pRot);
      
      // ne peut pas etre applique ici
      // il faut differencier trans et rot dans la multi.
      TGeoRotation rot_id;
      rot_id.SetAngles(0.0,0.0,0.0);
      
      TGeoCombiTrans c1;
      c1.SetRotation(rot_id);
      const Double_t *t = pTmp->GetTranslation();
      c1.SetTranslation(t[0],t[1],t[2]);
      
      TGeoCombiTrans c2;
      c2.SetRotation(rot_id);
      const Double_t *tt = fRefRot->GetTranslation();
      c2.SetTranslation(tt[0],tt[1],tt[2]);
      
      TGeoCombiTrans cc = c1 * c2 ;
      
      TGeoCombiTrans c3;
      c3.SetRotation(pTmp->GetRotation());
      TGeoCombiTrans c4;
      c4.SetRotation(fRefRot);
      
      TGeoCombiTrans ccc = c3 * c4;
      
      TGeoCombiTrans pGlobal;
      pGlobal.SetRotation(ccc.GetRotation());
      const Double_t *allt = cc.GetTranslation();
      pGlobal.SetTranslation(allt[0],allt[1],allt[2]);
      
      return  ( new TGeoCombiTrans( pGlobal ) );
      
    }else{
      
      cout << "-E- R3BDetector::GetGlobalPosition() \
	      No. Ref. Transformation defined ! " << endl;
      cout << "-E- R3BDetector::GetGlobalPosition() \
	      cannot create Local Transformation " << endl;
      return NULL;
    } //! fRefRot
    
  } else {
    // Lab Transf.
    if ( ( fPhi == 0 )  && ( fTheta==0 )  && ( fPsi == 0 )
	 &&
	 ( fX == 0 ) && ( fY == 0 ) && ( fZ == 0 )
	 )  return fRef;
    
    
    return ( new TGeoCombiTrans(*fGlobalTrans,*fGlobalRot) );
    
  }
}
