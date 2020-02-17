// -------------------------------------------------------------------------
// -----                      R3Bveto_segPoint source file                  -----
// -------------------------------------------------------------------------

#include "R3Bveto_segDigi.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;



// -----   Default constructor   -------------------------------------------
R3Bveto_segDigi::R3Bveto_segDigi()
  : fPaddleNb(-1),
    fTdcL(0.), fTdcR(0.), fTdc(0.), fQdcL(0.), fQdcR(0.), fQdc(0.),
    fxx(0.), fyy(0.), fzz(0.)
{
}



R3Bveto_segDigi::R3Bveto_segDigi(Int_t paddle, Double_t tdcL, Double_t tdcR,
			 Double_t tdc, Double_t qdcL, Double_t qdcR, Double_t qdc,
			 Double_t xx,Double_t yy, Double_t zz )
  : fPaddleNb(paddle),
    fTdcL(tdcL), fTdcR(tdcR), fTdc(tdc), fQdcL(qdcL), fQdcR(qdcR), fQdc(qdc),
    fxx(xx), fyy(yy), fzz(zz)
{
}



R3Bveto_segDigi::R3Bveto_segDigi(Int_t paddle, Double_t tdcR, Double_t tdcL,
			 Double_t qdcR, Double_t qdcL)
  : fPaddleNb(paddle),
    fTdcL(tdcL), fTdcR(tdcR), fTdc(0.), fQdcL(qdcL), fQdcR(qdcR), fQdc(0.),
    fxx(0.), fyy(0.), fzz(0.)
{
}



R3Bveto_segDigi::R3Bveto_segDigi(const R3Bveto_segDigi& right)
  : fPaddleNb(right.fPaddleNb),
    fTdcL(right.fTdcL), fTdcR(right.fTdcR), fTdc(right.fTdc),
    fQdcL(right.fQdcL), fQdcR(right.fQdcR), fQdc(right.fQdc),
    fxx(right.fxx), fyy(right.fyy), fzz(right.fzz)
{
}



// -----   Destructor   ----------------------------------------------------
R3Bveto_segDigi::~R3Bveto_segDigi()
{
}



// -----   Public method Print   -------------------------------------------
void R3Bveto_segDigi::Print(const Option_t* opt) const
{
  cout << "-I- R3Bveto_segDigi: VETO_SEG Digi in detector " << endl;
  cout << "    PaddleNr: " << fPaddleNb << endl;
  cout << "    TdcR: " << fTdcR << "    TdcL " << fTdcL << endl;
  cout << "    QdcR: " << fQdcR << "    QdcL " << fQdcL << endl;
}
// -------------------------------------------------------------------------



ClassImp(R3Bveto_segDigi)
