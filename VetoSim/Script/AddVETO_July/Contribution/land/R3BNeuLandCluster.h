// -----------------------------------------------------------------------------
// -----                       R3BNeuLandCluster                           -----
// -----                 Created 12-07-2011 by D.Kresan                    -----
// -----                     Updated by C. A. Douma                        -----
// -----------------------------------------------------------------------------

#ifndef R3BNEULANDCLUSTER
#define R3BNEULANDCLUSTER

#include "TObject.h"
#include "TVector3.h"


class R3BNeuLandCluster : public TObject
{
 public:
  R3BNeuLandCluster();
  R3BNeuLandCluster(Double_t startX, Double_t startY, Double_t startZ, Double_t startT,
	     Double_t stopX, Double_t stopY, Double_t stopZ, Double_t stopT,
	     Double_t e, Double_t size);
  virtual ~R3BNeuLandCluster();

  void SetStartPosition(Double_t startX, Double_t startY, Double_t startZ);
  void SetStopPosition(Double_t stopX, Double_t stopY, Double_t stopZ);
  void SetStartT(Double_t startT);
  void SetStopT(Double_t stopT);
  void SetE(Double_t e);
  void SetSize(Double_t size);
  void Mark();

  void StartPosition(TVector3 &pos) const;
  void StopPosition(TVector3 &pos) const;
  Double_t GetStartT() const;
  Double_t GetStartX() const;
  Double_t GetStartY() const;
  Double_t GetStartZ() const;
  Double_t GetStopT() const;
  Double_t GetStopX() const;
  Double_t GetStopY() const;
  Double_t GetStopZ() const;
  Double_t GetE() const;
  Double_t GetSize() const;
  Bool_t  IsMarked() const;

 private:
  Double32_t fStartX;
  Double32_t fStartY;
  Double32_t fStartZ;
  Double32_t fStartT;
  Double32_t fStopX;
  Double32_t fStopY;
  Double32_t fStopZ;
  Double32_t fStopT;
  Double32_t fE;
  Double32_t fSize;
  Bool_t     fMarked;

 public:
  ClassDef(R3BNeuLandCluster, 1)
};


#endif


