//
// C++ Interface: PndTutPar
//
#ifndef R3BVETO_SEGDIGIPAR_H
#define R3BVETO_SEGDIGIPAR_H

#include <TVector3.h>
#include <TObjString.h>

#include "FairParGenericSet.h"
#include "FairParamList.h"

class R3Bveto_segDigiPar : public FairParGenericSet
{
    public :
	R3Bveto_segDigiPar (const char* name="R3Bveto_segDigiParTest",
			const char* title="Tutorial  parameter",
			const char* context="TestDefaultContext");
	~R3Bveto_segDigiPar(void){};
	void clear(void){};
	void putParams(FairParamList* list);
	Bool_t getParams(FairParamList* list);

	void Print(Option_t* option="") const;
	/** Accessor functions **/
	const Int_t GetMaxPaddle() { return  nMaxPaddle;}
	const Int_t GetMaxPlane() { return  nMaxPlane;}
	const Double_t GetPaddleLength() { return  paddleLength;}
	const Double_t GetPaddleHeight() { return  paddleHeight;}
	const Double_t GetPaddleDepth() { return  paddleDepth;}
	const Double_t GetPaddleSpacing() { return  paddleSpacing; }
	const Double_t GetPaddleWrapping() { return  paddleWrapping; }
	const TString  GetGeometryFileName() { return fGeometryFileName; }
	void SetMaxPaddle(Int_t paddle ){nMaxPaddle=paddle;}
	void SetMaxPlane(Int_t pla){nMaxPlane=pla;}
	void SetPaddleLength(Double_t length){paddleLength=length;}
	void SetPaddleHeight(Double_t height){paddleHeight=height;}
	void SetPaddleDepth(Double_t depth){paddleDepth=depth;}
	void SetPaddleSpacing(Double_t space){paddleSpacing=space;}
	void SetPaddleWrapping(Double_t wrap){paddleWrapping=wrap;}
	void SetGeometryFileName(TString name) { fGeometryFileName = name; }

  private:
  // Digi. Parameters
  Int_t nMaxPaddle;
  Int_t nMaxPlane;
  Double_t paddleLength;
  Double_t paddleHeight;
  Double_t paddleDepth;
  Double_t paddleSpacing;
  Double_t paddleWrapping;
  TString  fGeometryFileName;

   ClassDef(R3Bveto_segDigiPar,2); //
};

#endif /* !R3BVETO_SEGDIGIPAR_H*/

