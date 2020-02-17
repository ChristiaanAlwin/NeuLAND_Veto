// -------------------------------------------------------------------------
// -----                    R3BFieldConst source file                  -----
// -------------------------------------------------------------------------
#include "R3BFieldConst.h"

#include "R3BFieldPar.h"

#include <iomanip>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::setw;


// -----   Default constructor   -------------------------------------------
R3BFieldConst::R3BFieldConst()
: fXmin(0.)
, fXmax(0.)
, fYmin(0.)
, fYmax(0.)
, fZmin(0.)
, fZmax(0.)
, fBx(0.)
, fBy(0.)
, fBz(0.)
{
    fType = 0;
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
R3BFieldConst::R3BFieldConst(const char* name, Double_t xMin,
                             Double_t xMax, Double_t yMin,
                             Double_t yMax, Double_t zMin,
                             Double_t zMax, Double_t bX,
                             Double_t bY, Double_t bZ) 
: FairField(name)
, fXmin(xMin)
, fXmax(xMax)
, fYmin(yMin)
, fYmax(yMax)
, fZmin(zMin)
, fZmax(zMax)
, fBx(bX)
, fBy(bY)
, fBz(bZ)
{
  fType = 0;
}
// -------------------------------------------------------------------------



// --------   Constructor from R3BFieldPar   -------------------------------
R3BFieldConst::R3BFieldConst(R3BFieldPar* fieldPar)
: fXmin(0.)
, fXmax(0.)
, fYmin(0.)
, fYmax(0.)
, fZmin(0.)
, fZmax(0.)
, fBx(0.)
, fBy(0.)
, fBz(0.)
{
    if ( ! fieldPar )
    {
        cerr << "-W- R3BFieldConst::R3BFieldConst: empty parameter container!"
        << endl;
        fType = 0;
    }
    else
    {
        fXmin = fieldPar->GetXmin();
        fXmax = fieldPar->GetXmax();
        fYmin = fieldPar->GetYmin();
        fYmax = fieldPar->GetYmax();
        fZmin = fieldPar->GetZmin();
        fZmax = fieldPar->GetZmax();
        fBx   = fieldPar->GetBx();
        fBy   = fieldPar->GetBy();
        fBz   = fieldPar->GetBz();
        fType = fieldPar->GetType();
    }
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
R3BFieldConst::~R3BFieldConst() { }
// -------------------------------------------------------------------------



// -----   Set field region   ----------------------------------------------
void R3BFieldConst::SetFieldRegion(Double_t xMin, Double_t xMax, 
				   Double_t yMin, Double_t yMax, 
				   Double_t zMin, Double_t zMax) {
  fXmin = xMin;
  fXmax = xMax;
  fYmin = yMin;
  fYmax = yMax;
  fZmin = zMin;
  fZmax = zMax;
}
// -------------------------------------------------------------------------



// -----   Set field values   ----------------------------------------------
void R3BFieldConst::SetField(Double_t bX, Double_t bY, Double_t bZ) {
  fBx   = bX;
  fBy   = bY;
  fBz   = bZ;
}
// -------------------------------------------------------------------------



// -----   Get x component of field   --------------------------------------
Double_t R3BFieldConst::GetBx(Double_t x, Double_t y, Double_t z) {
  if ( x < fXmin  ||  x > fXmax  ||
       y < fYmin  ||  y > fYmax  ||
       z < fZmin  ||  z > fZmax ) return 0.;
  return fBx;
}
// -------------------------------------------------------------------------



// -----   Get y component of field   --------------------------------------
Double_t R3BFieldConst::GetBy(Double_t x, Double_t y, Double_t z) {
  if ( x < fXmin  ||  x > fXmax  ||
       y < fYmin  ||  y > fYmax  ||
       z < fZmin  ||  z > fZmax ) return 0.;
  return fBy;
}
// -------------------------------------------------------------------------



// -----   Get z component of field   --------------------------------------
Double_t R3BFieldConst::GetBz(Double_t x, Double_t y, Double_t z) {
  if ( x < fXmin  ||  x > fXmax  ||
       y < fYmin  ||  y > fYmax  ||
       z < fZmin  ||  z > fZmax ) return 0.;
  return fBz;
}
// -------------------------------------------------------------------------



// -----   Screen output   -------------------------------------------------
void R3BFieldConst::Print(Option_t*) const
{
  cout << "======================================================" << endl;
  cout << "----  " << fTitle << " : " << fName << endl;
  cout << "----" << endl;
  cout << "----  Field type    : constant" << endl;
  cout << "----" << endl;
  cout << "----  Field regions : " << endl;
  cout << "----        x = " << setw(4) << fXmin << " to " << setw(4) 
       << fXmax << " cm" << endl;
  cout << "----        y = " << setw(4) << fYmin << " to " << setw(4) 
       << fYmax << " cm" << endl;
  cout << "----        z = " << setw(4) << fZmin << " to " << setw(4)
       << fZmax << " cm" << endl;
  cout.precision(4);
  cout << "----  B = ( " << fBx << ", " << fBy << ", " << fBz << " ) kG"
       << endl;
  cout << "======================================================" << endl;
}
// -------------------------------------------------------------------------



ClassImp(R3BFieldConst)
