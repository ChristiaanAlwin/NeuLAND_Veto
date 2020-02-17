#include "R3BNeutronTrak_seg.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;



// -----   Default constructor   -------------------------------------------
R3BNeutronTrak_seg::R3BNeutronTrak_seg()
  : fX(0.), fY(0.), fZ(0.),
    fX_in(0.), fY_in(0.), fZ_in(0.),
    fX_out(0.), fY_out(0.), fZ_out(0.),
    fPx_out(0.), fPy_out(0.), fPz_out(0.),
    fPaddleNb(-1)
{
}



R3BNeutronTrak_seg::R3BNeutronTrak_seg(TVector3 posIn, TVector3 posOut,
				 TVector3 momOut, Double_t tof)
  : fX(posIn.X()), fY(posIn.Y()), fZ(posIn.Z()),
    fX_in(0.), fY_in(0.), fZ_in(0.),
    fX_out(posOut.X()), fY_out(posOut.Y()), fZ_out(posOut.Z()),
    fPx_out(momOut.X()), fPy_out(momOut.Y()), fPz_out(momOut.Z()),
    fPaddleNb(-1)
{
}



R3BNeutronTrak_seg::R3BNeutronTrak_seg(const R3BNeutronTrak_seg& right)
  : fX(right.fX), fY(right.fY), fZ(right.fZ),
    fX_in(right.fX_in), fY_in(right.fY_in), fZ_in(right.fZ_in),
    fX_out(right.fX_out), fY_out(right.fY_out), fZ_out(right.fZ_out),
    fPx_out(right.fPx_out), fPy_out(right.fPy_out), fPz_out(right.fPz_out),
    fPaddleNb(right.fPaddleNb)
{
}



// -----   Destructor   ----------------------------------------------------
R3BNeutronTrak_seg::~R3BNeutronTrak_seg()
{
}



// -----   Public method Print   -------------------------------------------
void R3BNeutronTrak_seg::Print(const Option_t* opt) const
{
  cout << "-I- R3BNeutronTrak_seg: VETO_SEG Digi in detector " << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  cout << "    Momentum (" << fPx_out << ", " << fPy_out << ", " << fPz_out
       << ") GeV" << endl; 
}
// -------------------------------------------------------------------------



ClassImp(R3BNeutronTrak_seg)
