// -------------------------------------------------------------------------
// -----                    R3Bveto_seg source file                        -----
// -----               Created 26/03/09  by D.Bertini                  -----
// -------------------------------------------------------------------------
#include "R3Bveto_seg.h"

#include "R3BGeoveto_seg.h"
#include "R3Bveto_segPoint.h"
#include "R3BGeoveto_segPar.h"
#include "R3Bveto_segDigiPar.h"
#include "R3BMCStack.h"

#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairRun.h"
#include "FairVolume.h"

#include "TClonesArray.h"
#include "TGeoMCGeometry.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TObjArray.h"

// includes for modeling
#include "TGeoManager.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TGeoMatrix.h"
#include "TGeoMaterial.h"
#include "TGeoBBox.h"
#include "TGeoPara.h"
#include "TGeoPgon.h"
#include "TGeoSphere.h"
#include "TGeoArb8.h"
#include "TGeoCone.h"
#include "TGeoBoolNode.h"
#include "TGeoCompositeShape.h"
#include "TGeoShapeAssembly.h"
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;



// -----  Default constructor  -------------------------------------------
R3Bveto_seg::R3Bveto_seg() : R3BDetector("R3Bveto_seg", kTRUE, kVETO_SEG)
{
  ResetParameters();
  fveto_segCollection = new TClonesArray("R3Bveto_segPoint");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fveto_segFirstHits = new TClonesArray("R3Bveto_segFirstHits");
}
// -------------------------------------------------------------------------



// -----  Standard constructor  ------------------------------------------
R3Bveto_seg::R3Bveto_seg(const char* name, Bool_t active) 
    : R3BDetector(name, active, kVETO_SEG)
{
  ResetParameters();
  fveto_segCollection = new TClonesArray("R3Bveto_segPoint");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fveto_segFirstHits = new TClonesArray("R3Bveto_segFirstHits");
}
// -------------------------------------------------------------------------



// -----  Destructor  ----------------------------------------------------
R3Bveto_seg::~R3Bveto_seg()
{
  if ( flGeoPar ) delete flGeoPar;
  if (fveto_segCollection) {
   fveto_segCollection->Delete();
   delete fveto_segCollection;
  }
  if (fveto_segFirstHits) {
   fveto_segFirstHits->Delete();
   delete fveto_segFirstHits;
  }
}
// -------------------------------------------------------------------------



void R3Bveto_seg::Initialize()
{
  FairDetector::Initialize();

  LOG(INFO) << "R3Bveto_seg initialisation " << FairLogger::endl;
  
  Int_t id1 = 0, id2 = 0, id3 = 0;
  
  TString fileName = GetGeometryFileName();
  if(fileName.Contains("Nveto_seg")) {
    LOG(DEBUG) << "R3Bveto_seg: Paddle B5 (McId): " << gMC->VolId("paddle_h_box5_seg") << FairLogger::endl;
    id3 = gMC->VolId("paddle_h_box5_seg");
    fMapMcId[id3]=3;
  } else {
    LOG(DEBUG) << "R3Bveto_seg: Paddle B3 (McId): " << gMC->VolId("paddle_h_box3_seg") << FairLogger::endl;
    LOG(DEBUG) << "R3Bveto_seg: Paddle B4 (McId): " << gMC->VolId("paddle_h_box4_seg") << FairLogger::endl;
    LOG(DEBUG) << "R3Bveto_seg: Paddle B5 (McId): " << gMC->VolId("paddle_h_box5_seg") << FairLogger::endl;
    id1 = gMC->VolId("paddle_h_box3_seg");
    id2 = gMC->VolId("paddle_h_box4_seg");
    id3 = gMC->VolId("paddle_h_box5_seg");
    fMapMcId[id1]=1;
    fMapMcId[id2]=2;
    fMapMcId[id3]=3;
  }
    
    TGeoBBox *box = (TGeoBBox*) gGeoManager->GetVolume(gMC->VolId("paddle_h_box5_seg"))->GetShape();
    TGeoBBox *bbox = (TGeoBBox*) gGeoManager->GetVolume(gMC->VolId("AVETO_SEG"))->GetShape();
    TGeoBBox *bbbox = (TGeoBBox*) gGeoManager->GetVolume("CELL")->GetShape();
    Int_t maxPlane;
    Int_t maxPaddle;
    maxPlane = (Int_t) (bbbox->GetDZ() / bbox->GetDZ());
    if(fileName.Contains("Nveto_seg_proto")) {
        maxPaddle = (Int_t) ( maxPlane * ((Int_t)(bbbox->GetDX()/bbox->GetDY())) );
    } else if(fileName.Contains("Nveto_seg")) {
        maxPaddle = (Int_t) ( maxPlane * ((Int_t)(box->GetDX()/bbox->GetDY())) );
    } else {
        maxPaddle = (Int_t) ( maxPlane * ((Int_t)(box->GetDX()/box->GetDY())) );
    }
    FairRuntimeDb *rtdb= FairRun::Instance()->GetRuntimeDb();
    R3Bveto_segDigiPar* par=(R3Bveto_segDigiPar*)(rtdb->getContainer("R3Bveto_segDigiPar"));
    par->SetGeometryFileName(GetGeometryFileName());
    par->SetMaxPaddle(maxPaddle);
    par->SetMaxPlane(maxPlane);
    par->SetPaddleLength(box->GetDX());
    par->setChanged();


  // Sensitive Volumes :: Unique  Id
  //  paddle_h_box3       1
  //  paddle_h_box4       2
  //  paddle_h_box5       3

  //<D.B> Check me
  // Initialise variables from Birk law
  Double_t dP = 1.032 ;
  // Set constants for Birk's Law implentation
  fBirkC0 =  1.;
  fBirkC1 =  0.013/dP;
  fBirkC2 =  9.6e-6/(dP * dP);
}



// -----  Public method ProcessHits  --------------------------------------
Bool_t R3Bveto_seg::ProcessHits(FairVolume* vol)
{
  // <DB>  Before filling MC info for current track
	//       print step info if verbose > 1
  //    cout<<"Test verbose: "<<fVerboseLevel<<endl;
  //if (fVerboseLevel>1) StepHistory();
  
  StepHistory(); // FIXME: StepHistory also responsible for filling veto_segFirstHits 

  // --- get Geometry hiearchical Information
  Int_t cp1=-1;
  Int_t cp2=-1;
  Int_t volId1=-1;
  Int_t volId2=-1;

  // Crystals Ids
  volId1 =  gMC->CurrentVolID(cp1);
  // Lead - Crystal numbering scheme
  fPaddleTyp = fMapMcId[volId1];
  // Mother Assembly def
  volId2 =  gMC->CurrentVolOffID(1, cp2);

  if ( gMC->IsTrackEntering() ) {
   fELoss  = 0.;
   fLightYield = 0.;
   fTime  = gMC->TrackTime() * 1.0e09;
   fLength = gMC->TrackLength();
   gMC->TrackPosition(fPosIn);
   gMC->TrackMomentum(fMomIn);
  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();
//  cout<<"fELoss: "<<fELoss<<endl;

  // Apply Birk's law ( Adapted from G3BIRK/Geant3)
  Double_t lightYield =  gMC->Edep() ;
/*  
  const Double_t* cpos;
  cpos = gGeoManager->GetCurrentPoint();
  cout<<"current pos: "<<cpos[0]<<"  "<<cpos[1]<<"  "<<cpos[2]<<endl;
  cout<<"particle: "<<gMC->TrackPid()<<endl;
  cout<<"step: "<<gMC->TrackStep()<<endl;
  cout<<"fELoss: "<<fELoss<<endl;
*/  
  // Correction for all charge states
  if (gMC->TrackCharge()!=0) {
    Double_t birkC1Mod = 0;
     // Apply correction for higher charge states
    if (fBirkC0==1){
      if (TMath::Abs(gMC->TrackCharge())>=2)
        birkC1Mod=fBirkC1*7.2/12.6;
      else
        birkC1Mod=fBirkC1;
    }

    Double_t dedxcm=0.;
    if (gMC->TrackStep()>0)
    {
      dedxcm=1000.*gMC->Edep()/gMC->TrackStep();
//      cout<<"Edep: "<<gMC->Edep()<<" Trackstep: "<<gMC->TrackStep()<<endl;
      lightYield=lightYield/(1.+birkC1Mod*dedxcm+fBirkC2*dedxcm*dedxcm);
//      cout<<"dedxcm "<<dedxcm<<" factor "<<1./(1.+birkC1Mod*dedxcm+fBirkC2*dedxcm*dedxcm)
//      <<" light yield "<<lightYield<<endl;
      fLightYield=fLightYield+lightYield;
//      cout<<"fLightYield: "<<fLightYield<<endl;
    }
  }


  // Set additional parameter_seg at exit of active volume. Create R3Bveto_segPoint.
  if ( gMC->IsTrackExiting()   ||
     gMC->IsTrackStop()        ||
     gMC->IsTrackDisappeared()  )
  {
    fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);
    if (fELoss == 0. ) return kFALSE;
     
    if (gMC->IsTrackExiting()) {
      const Double_t* oldpos;
      const Double_t* olddirection;
      Double_t newpos[3];
      Double_t newdirection[3];
      Double_t safety;
    
      gGeoManager->FindNode(fPosOut.X(),fPosOut.Y(),fPosOut.Z());
      oldpos = gGeoManager->GetCurrentPoint();
      olddirection = gGeoManager->GetCurrentDirection();
    
  //     cout << "1st direction: " << olddirection[0] << "," << olddirection[1] << "," << olddirection[2] << endl;

      for (Int_t i=0; i<3; i++){
        newdirection[i] = -1*olddirection[i];
      }
    
      gGeoManager->SetCurrentDirection(newdirection);
     //  TGeoNode *bla = gGeoManager->FindNextBoundary(2);
      safety = gGeoManager->GetSafeDistance();


      gGeoManager->SetCurrentDirection(-newdirection[0],-newdirection[1],-newdirection[2]);
    
      for (Int_t i=0; i<3; i++){
        newpos[i] = oldpos[i] - (3*safety*olddirection[i]);
      }

      fPosOut.SetX(newpos[0]);
      fPosOut.SetY(newpos[1]);
      fPosOut.SetZ(newpos[2]);
    }



    AddHit(fTrackID, fVolumeID,  fPaddleTyp,  cp2, cp1,
      TVector3(fPosIn.X(),  fPosIn.Y(),  fPosIn.Z()),
      TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
      TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
      TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
      fTime, fLength, fELoss, fLightYield);
     

    // Increment number of veto_segPoints for this track
    R3BStack* stack = (R3BStack*) gMC->GetStack();
    stack->AddPoint(kVETO_SEG);
     
    ResetParameters();
  }

  return kTRUE;
}




// -----  Public method BeginOfEvent  -----------------------------------------
void R3Bveto_seg::BeginEvent()
{
  //cout << "-I- begin event called ############################# " << endl;

//   if (gGeoManager)
//   TGeoVolume * vol = gGeoManager->FindVolumeFast("paddle_h_box3_seg");
   for(Int_t l=0;l<6;l++){
      firstHitX[l]=0.;
      firstHitY[l]=0.;
      firstHitZ[l]=0.;
      firstT[l]=0.;      
   }
}



void R3Bveto_seg::FinishEvent()
{
  AddHit1(firstHitX[0], firstHitY[0], firstHitZ[0], firstT[0],
          firstHitX[1], firstHitY[1], firstHitZ[1], firstT[1],
          firstHitX[2], firstHitY[2], firstHitZ[2], firstT[2],
          firstHitX[3], firstHitY[3], firstHitZ[3], firstT[3],
          firstHitX[4], firstHitY[4], firstHitZ[4], firstT[4],
          firstHitX[5], firstHitY[5], firstHitZ[5], firstT[5]);
}



// -----  Public method EndOfEvent  -----------------------------------------
void R3Bveto_seg::EndOfEvent()
{
  if (fVerboseLevel) {
    Print();
  }
  Reset();
}
// ----------------------------------------------------------------------------



// -----  Public method Register  -------------------------------------------
void R3Bveto_seg::Register()
{
  FairRootManager::Instance()->Register("veto_segPoint", GetName(), fveto_segCollection, kTRUE);
  FairRootManager::Instance()->Register("veto_segFirstHits", GetName(), fveto_segFirstHits, kTRUE);
}
// ----------------------------------------------------------------------------



// -----  Public method GetCollection  --------------------------------------
TClonesArray* R3Bveto_seg::GetCollection(Int_t iColl) const
{
  if (iColl == 0) return fveto_segCollection;
  else return NULL;
}
// ----------------------------------------------------------------------------



// -----  Public method Print  ----------------------------------------------
void R3Bveto_seg::Print(Option_t *option) const
{
  Int_t nHits = fveto_segCollection->GetEntriesFast();
  LOG(INFO) << "R3Bveto_seg: " << nHits << " points registered in this event" << FairLogger::endl;
  nHits = fveto_segFirstHits->GetEntriesFast();
  LOG(INFO) << "R3Bveto_segFirstHits: " << nHits << " points registered in this event" << FairLogger::endl;
}
// ----------------------------------------------------------------------------



// -----  Public method Reset  ----------------------------------------------
void R3Bveto_seg::Reset()
{
  fveto_segCollection->Clear();
  ResetParameters();
  fveto_segFirstHits->Clear();
  ResetParameters();
}
// ----------------------------------------------------------------------------



// -----  Public method CopyClones  -----------------------------------------
void R3Bveto_seg::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "R3Bveto_seg: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  R3Bveto_segPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (R3Bveto_segPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[fPosIndex]) R3Bveto_segPoint(*oldpoint);
   fPosIndex++;
  }
  LOG(INFO) << "R3Bveto_seg: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}



// -----  Private method AddHit  --------------------------------------------
R3Bveto_segPoint* R3Bveto_seg::AddHit(Int_t trackID, Int_t detID, Int_t box, Int_t id1, Int_t id2,
			      TVector3 posIn,
			      TVector3 posOut, TVector3 momIn,
			      TVector3 momOut, Double_t time,
			      Double_t length, Double_t eLoss, Double_t lightYield)
{
  TClonesArray& clref = *fveto_segCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel>1) {
    LOG(INFO) << "R3Bveto_seg: Adding Point at (" << posIn.X() << ", " << posIn.Y()
    << ", " << posIn.Z() << ") cm,  detector " << detID << ", track "
    << trackID << ", energy loss " << eLoss  << " GeV" << FairLogger::endl;
  }
  return new(clref[size]) R3Bveto_segPoint(trackID, detID, box, id1, id2,  posIn, posOut,
                                       momIn, momOut, time, length, eLoss, lightYield);
}



R3Bveto_segFirstHits* R3Bveto_seg::AddHit1(Double_t x0, Double_t y0, Double_t z0, Double_t t0,
                                   Double_t x1, Double_t y1, Double_t z1, Double_t t1,
                                   Double_t x2, Double_t y2, Double_t z2, Double_t t2,
                                   Double_t x3, Double_t y3, Double_t z3, Double_t t3,
                                   Double_t x4, Double_t y4, Double_t z4, Double_t t4,
                                   Double_t x5, Double_t y5, Double_t z5, Double_t t5)
{
  TClonesArray& clref = *fveto_segFirstHits;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) R3Bveto_segFirstHits(x0,y0,z0,t0,x1,y1,z1,t1,x2,y2,z2,t2,x3,y3,z3,t3,x4,y4,z4,t4,x5,y5,z5,t5);
}



void R3Bveto_seg::ConstructGeometry()
{
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing (Neu)VETO_SEG geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    LOG(FATAL) << "Geometry file name is not set" << FairLogger::endl;
  }
}



// -----   CheckIfSensitive   -------------------------------------------------
Bool_t R3Bveto_seg::CheckIfSensitive(std::string name)
{
  TString volName = name;
  if(volName.Contains("paddle_h_box5_seg")) {
    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------



void R3Bveto_seg::StepHistory()
{
  // <DB> dump of step info.
  static Int_t iStepN;

  // Particle being tracked
  const char *sParticle;
  switch(gMC->TrackPid()){
    case 2212:          sParticle="proton"    ;break;
    case 2112:          sParticle="neutron"   ;break;
    case 22:            sParticle="gamma"     ;break;
    case 50000050:      sParticle="ckov"      ;break;
    case 111:           sParticle="pi0"       ;break;  
    case 211:           sParticle="pi+"       ;break;  
    case -211:          sParticle="Pi-"       ;break;  
    case 1000010020:            sParticle="deuteron"        ;break;
    case 1000010030:            sParticle="triton"        ;break;
    case 1000020030:            sParticle="he3"        ;break;
    case 1000020040:            sParticle="alpha"        ;break;
    default:            sParticle="not known" ;break;
  }


  TString flag="-I- STEPINFO: tracking status: ";
  if(gMC->IsTrackAlive()) {
    if(gMC->IsTrackEntering())      flag="enters to";
    else if(gMC->IsTrackExiting())  flag="exits from";
    else if(gMC->IsTrackInside())   flag="inside";
  } else {
    if(gMC->IsTrackStop())          flag="stopped in";
  }
  
  //Where am i ?
  Int_t vid=0,copy=0;
  //current volume and his mother are always there
  TString path=gMC->CurrentVolName(); path.Prepend("-");path.Prepend(gMC->CurrentVolOffName(1));
  vid=gMC->CurrentVolOffID(2,copy);  if(vid) {path.Prepend("-");path.Prepend(gMC->VolName(vid));}
  vid=gMC->CurrentVolOffID(3,copy);  if(vid) {path.Prepend("-");path.Prepend(gMC->VolName(vid));}
  
//  printf("step  (nr=%i): %s (%i) %s %s m=%.6f GeV q=%.1f dEdX=%.4f Etot=%.4f \n",
//  iStepN,sParticle,gMC->TrackPid(),flag.Data(),path.Data(),gMC->TrackMass(),gMC->TrackCharge(),gMC->Edep()*1e9,gMC->Etot());
  
  Double_t gMcTrackPos[3]; gMC->TrackPosition(gMcTrackPos[0],gMcTrackPos[1],gMcTrackPos[2]);
  Double_t  gMcTrackPosLoc[3]; gMC->Gmtod(gMcTrackPos,gMcTrackPosLoc,1);
//  printf(" : track Position (MARS) x: %5.3lf, y: %5.3lf, z: %5.3lf (r: %5.3lf) ---> (LOC) x: %5.3f, y: %5.3f, z: %5.3f",
//          gMcTrackPos[0],gMcTrackPos[1],gMcTrackPos[2],
//          TMath::Sqrt(gMcTrackPos[0]*gMcTrackPos[0]+gMcTrackPos[1]*gMcTrackPos[1]+gMcTrackPos[2]*gMcTrackPos[2]),
//          gMcTrackPosLoc[0],gMcTrackPosLoc[1],gMcTrackPosLoc[2]);
  
//  printf("step (nr=%i): tid=%i flags alive=%i disap=%i enter=%i exit=%i inside=%i out=%i stop=%i new=%i",
//		 iStepN,  gMC->GetStack()->GetCurrentTrackNumber(),
//		 gMC->IsTrackAlive(), gMC->IsTrackDisappeared(),gMC->IsTrackEntering(), gMC->IsTrackExiting(),
//		 gMC->IsTrackInside(),gMC->IsTrackOut(),        gMC->IsTrackStop(),     gMC->IsNewTrack());
  
  Float_t a,z,den,rad,abs; a=z=den=rad=abs=-1;
  //Int_t mid=gMC->CurrentMaterial(a,z,den,rad,abs);
//  printf("step (nr=%i): mid=%i a=%7.2f z=%7.2f den=%9.4f rad=%9.2f abs=%9.2f\n\n",iStepN,mid,a,z,den,rad,abs);

/*  
  TArrayI proc;  gMC->StepProcesses(proc); 
  printf("-I- STEPINFO: Processes in this step:\n");
  for ( int i = 0 ; i < proc.GetSize(); i++)
  {
    printf("-I- Process involved --->   code: %i : name: %s\n", proc.At(i) , TMCProcessName[proc.At(i)]);
  }
  printf("-I- STEPINFO: end of process list\n");  
*/

  //Int_t Nprim=gMC->GetStack()->GetNprimary();

  Int_t trackNo=gMC->GetStack()->GetCurrentTrackNumber();
  Int_t motherNo=gMC->GetStack()->GetCurrentParentTrackNumber();
  Double_t time=gMC->TrackTime();

/*
  cout << "Prim: " << Nprim<<endl;
  cout << "TrackID: " << trackNo<<endl;
  cout << "MotherID: " << motherNo <<endl;
  cout << "time: " << time <<endl;
  cout << "PiD: "<<gMC->TrackPid()<<endl;
*/  

  if(gMC->TrackPid()==2112 && motherNo<0){
  // we have a primary neutron
     TArrayI proc;  gMC->StepProcesses(proc); 
     for ( int i = 0 ; i < proc.GetSize(); i++){
//        printf("-I- Process involved --->   code: %i : name: %s\n", proc.At(i) , TMCProcessName[proc.At(i)]);
     
        if(proc.At(i)!=22 && proc.At(i)!=23 && proc.At(i)!=31 && proc.At(i)!=43 &&  proc.At(i)!=13){
           LOG(INFO) << "new primary neutron interaction: " << proc.At(i) <<"  "<< TMCProcessName[proc.At(i)] << FairLogger::endl;
	
	}
         
// make histogram with first interaction
  
        if(proc.At(i)==22 && trackNo<6){
	   // elastic scattering
//           cout<<"primary neutron interaction elastic"<<endl;
           firstHitX[trackNo] = gMcTrackPos[0];
           firstHitY[trackNo] = gMcTrackPos[1];
           firstHitZ[trackNo] = gMcTrackPos[2];
           firstT[trackNo] = time;
  
        }
	
        if(proc.At(i)==13 && trackNo<6){
	   // elastic scattering
//           cout<<"primary neutron interaction hadronic"<<endl;
           firstHitX[trackNo] = gMcTrackPos[0];
           firstHitY[trackNo] = gMcTrackPos[1];
           firstHitZ[trackNo] = gMcTrackPos[2];
           firstT[trackNo] = time;
  
        }

        if(proc.At(i)==23 && trackNo<6){
	   // elastic scattering
//           cout<<"primary neutron interaction inelastic"<<endl;
           firstHitX[trackNo] = gMcTrackPos[0];
           firstHitY[trackNo] = gMcTrackPos[1];
           firstHitZ[trackNo] = gMcTrackPos[2];
           firstT[trackNo] = time;
  
        }
     }  
  }
  
  iStepN++;

}



void R3Bveto_seg::FinishRun()
{
}



ClassImp(R3Bveto_seg)

