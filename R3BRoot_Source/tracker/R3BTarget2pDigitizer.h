#ifndef R3BTARGET2PDIGITIZER_H
#define R3BTARGET2PDDIGITISER_H 1


#include "FairTask.h"
#include <map>
#include <string>
#include "R3BTarget2pDigiPar.h"
#include "R3BTarget2pDigi.h"

class TClonesArray;
class TObjectArray;
class TH1F;
class TH2F;



class R3BTarget2pDigitizer : public FairTask
{

 public:

  /** Default constructor **/  
  R3BTarget2pDigitizer();


  /** Destructor **/
  ~R3BTarget2pDigitizer();


  /** Virtual method Init **/
  virtual InitStatus Init();


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  virtual void Finish();
  virtual void Reset();


  R3BTarget2pDigi* AddHit(Double_t ss03_smul,Double_t ss03_kmul,Double_t ss06_smul,Double_t ss06_kmul,
  Double_t x0,Double_t y0,Double_t t0,Double_t estar,Double_t Pxf,Double_t Pyf,Double_t Pzf,Double_t Pf_tot,Double_t f_beta,
Double_t Pxp1,Double_t Pyp1,Double_t Pzp1,Double_t Pp1_tot,Double_t p1_beta,Double_t Pxp2,Double_t Pyp2,Double_t Pzp2,
//Double_t Pp2_tot,Double_t p2_beta);
Double_t Pp2_tot,Double_t p2_beta,Double_t alpha,Int_t ppmul);


  protected:
  TClonesArray* fTarget2pPoints;
  TClonesArray* fTarget2pMCTrack; 
  TClonesArray* fTarget2pDigi;
  TClonesArray* fMCTrack;

  // Parameter class
  R3BTarget2pDigiPar* fTarget2pDigiPar;

  //- Control Hitograms
  
  TH1F * ExEnIn_his;
  
  Int_t eventNoTra;
  
  
  
  private:
  virtual void SetParContainers();

 
  ClassDef(R3BTarget2pDigitizer,1);
  
};

#endif
