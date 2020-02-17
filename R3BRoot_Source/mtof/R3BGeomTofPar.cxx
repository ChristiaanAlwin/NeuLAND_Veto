//*-- AUTHOR : Denis Bertini
//*-- Created : 21/06/2005

#include "R3BGeomTofPar.h"

#include "FairParamList.h"

#include "TObjArray.h"

#include <iostream>
#include <iomanip>

ClassImp(R3BGeomTofPar)

R3BGeomTofPar::R3BGeomTofPar(const char* name,const char* title,const char* context)
           : FairParGenericSet(name,title,context) {

               fGeoSensNodes = new TObjArray();
               fGeoPassNodes = new TObjArray();
}

R3BGeomTofPar::~R3BGeomTofPar(void) {
}

void R3BGeomTofPar::clear(void) {
    if(fGeoSensNodes) delete fGeoSensNodes;
    if(fGeoPassNodes) delete fGeoPassNodes;
}

void R3BGeomTofPar::putParams(FairParamList* l) {
  if (!l) return;
   l->addObject("FairGeoNodes Sensitive List", fGeoSensNodes);
   l->addObject("FairGeoNodes Passive List", fGeoPassNodes);
}

Bool_t R3BGeomTofPar::getParams(FairParamList* l) {
    if (!l) return kFALSE;
    if (!l->fillObject("FairGeoNodes Sensitive List", fGeoSensNodes)) return kFALSE;
    if (!l->fillObject("FairGeoNodes Passive List", fGeoPassNodes)) return kFALSE;

  return kTRUE;
}
