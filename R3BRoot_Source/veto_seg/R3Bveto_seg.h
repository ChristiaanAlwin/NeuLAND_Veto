// -------------------------------------------------------------------------
// -----                        R3Bveto_seg header file                     -----
// -----                  Created 26/03/09  by D.Bertini               -----
// -------------------------------------------------------------------------

/**  R3Bveto_seg.h
 **/


#ifndef R3BVETO_SEG_H
#define R3BVETO_SEG_H

#include "R3BDetector.h"
#include "TLorentzVector.h"
#include "TGeoMedium.h"
#include <string>
#include <iostream>
#include <map>
#include "R3Bveto_segFirstHits.h"

using namespace std;

class TClonesArray;
class R3Bveto_segPoint;
class FairVolume;



class R3Bveto_seg : public R3BDetector
{
public:
  /** Default constructor **/
  R3Bveto_seg();
  
  /** Standard constructor.
   *@param name    detetcor name
   *@param active  sensitivity flag
   **/
  R3Bveto_seg(const char* name, Bool_t active);
  
  /** Destructor **/
  virtual ~R3Bveto_seg();
  
  /** Virtual method ProcessHits
   **
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a R3Bveto_segPoint and adds it to the
   ** collection.
   *@param vol  Pointer to the active volume
   **/
  virtual Bool_t ProcessHits(FairVolume* vol = 0);
  
  /** Virtual method BeginEvent
   **
   ** If verbosity level is set, print hit collection at the
   ** end of the event and resets it afterwards.
   **/
  virtual void BeginEvent();
  
  /** Virtual method EndOfEvent
   **
   ** If verbosity level is set, print hit collection at the
   ** end of the event and resets it afterwards.
   **/
  virtual void EndOfEvent();
  
  virtual void FinishEvent();
  
  /** Virtual method Register
   **
   ** Registers the hit collection in the ROOT manager.
   **/
  virtual void Register();
  
  virtual void FinishRun();
  
  /** Accessor to the hit collection **/
  virtual TClonesArray* GetCollection(Int_t iColl) const;
  
  /** Virtual method Print
   **
   ** Screen output of hit collection.
   **/
  virtual void Print(Option_t *option = "") const;
  
  /** Virtual method Reset
   **
   ** Clears the hit collection
   **/
  virtual void Reset();
  
  /** Virtual method CopyClones
   **
   ** Copies the hit collection with a given track index offset
   *@param cl1     Origin
   *@param cl2     Target
   *@param offset  Index offset
   **/
  virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2,
                          Int_t offset);
  
  /** Virtaul method Construct geometry
   **
   ** Constructs the STS geometry
   **/
  virtual void ConstructGeometry();
  
  virtual void Initialize();
  
  virtual void SetSpecialPhysicsCuts() {}
  
  virtual Bool_t CheckIfSensitive(std::string name);
  
  
private:
  /** Track information to be stored until the track leaves the
   active volume. **/
  Int_t          fTrackID;           //!  track index
  Int_t          fVolumeID;          //!  volume id
  Int_t          fCopyNo;            //!  copy no
  Int_t          fPaddleTyp;         //!  box number
  TLorentzVector fPosIn, fPosOut;    //!  position
  TLorentzVector fMomIn, fMomOut;    //!  momentum
  Double32_t     fTime;              //!  time
  Double32_t     fLength;            //!  length
  Double32_t     fELoss;             //!  energy loss
  Double32_t     fLightYield;        //!  light yield
  
  Int_t          fPosIndex;          //!
  TClonesArray*  fveto_segCollection;    //!  The hit collection
  TClonesArray*  fveto_segFirstHits;     //!  First Hits
  Bool_t         kGeoSaved;          //!
  TList *flGeoPar;                   //!
  Double_t fBirkC0,fBirkC1,fBirkC2;  //!
  
  Int_t          fMotherID;          //!  Mother ID
  Int_t          fNprim;             //!  Number of primaries
  
  Double_t firstHitX[6];             //! position and time of first our hits
  Double_t firstHitY[6];             //!
  Double_t firstHitZ[6];             //!
  Double_t firstT[6];                //!
  
  /** Private method AddHit
   **
   ** Adds a veto_segPoint to the HitCollection
   **/
  R3Bveto_segPoint* AddHit(Int_t trackID, Int_t detID, Int_t box, Int_t id1, Int_t id2,
                       TVector3 posIn,
                       TVector3 pos_out, TVector3 momIn,
                       TVector3 momOut, Double_t time,
                       Double_t length, Double_t eLoss,Double_t lightYield);
  
  R3Bveto_segFirstHits* AddHit1(Double_t x0, Double_t y0, Double_t z0, Double_t T0,
                            Double_t x1, Double_t y1, Double_t z1, Double_t T1,
                            Double_t x2, Double_t y2, Double_t z2, Double_t T2,
                            Double_t x3, Double_t y3, Double_t z3, Double_t T3,
                            Double_t x4, Double_t y4, Double_t z4, Double_t T4,
                            Double_t x5, Double_t y5, Double_t z5, Double_t T5);
  
  /** Private method ResetParameters
   **
   ** Resets the private members for the track parameter_seg
   **/
  void ResetParameters();
  
  /** Map of MCis with fixed VolID */
  map <Int_t,Int_t> fMapMcId;    //!
  
  void StepHistory();
  
  ClassDef(R3Bveto_seg,3);
};


inline void R3Bveto_seg::ResetParameters()
{
  fTrackID = fVolumeID = 0;
  fPosIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fPosOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fTime = fLength = fELoss = fLightYield = 0;
  fPosIndex = 0;
};


#endif 
