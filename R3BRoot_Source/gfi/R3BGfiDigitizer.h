#ifndef R3BGFIDIGITIZER_H
#define R3BGFIDDIGITISER_H 1


#include "FairTask.h"
#include <map>
#include <string>
#include "R3BGfiDigiPar.h"
#include "R3BGfiDigi.h"

class TClonesArray;
class TObjectArray;
class TH1F;
class TH2F;



class R3BGfiDigitizer : public FairTask
{

 public:

  /** Default constructor **/  
  R3BGfiDigitizer();


  /** Destructor **/
  ~R3BGfiDigitizer();


  /** Virtual method Init **/
  virtual InitStatus Init();


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  virtual void Finish();
  virtual void Reset();


  R3BGfiDigi* AddHit(Int_t gf1mul,Double_t gf1_01x,Int_t gf2mul,Double_t gf2_01x);


  protected:
  TClonesArray* fGfiPoints;
  TClonesArray* fGfiMCTrack; 
  TClonesArray* fGfiDigi;

  // Parameter class
  R3BGfiDigiPar* fGfiDigiPar;

  //- Control Hitograms

  
  Int_t eventNoGfi;
  
  
  private:
  virtual void SetParContainers();

 
  ClassDef(R3BGfiDigitizer,1);
  
};

#endif
