#ifndef R3BGEOMTOF_H
#define R3BGEOMTOF_H

#include "FairGeoSet.h"

class  R3BGeomTof : public FairGeoSet {
protected:
  char modName[20];  // name of module
  char eleName[20];  // substring for elements in module
public:
  R3BGeomTof();
  ~R3BGeomTof() {}
  const char* getModuleName(Int_t);
  const char* getEleName(Int_t);
  inline Int_t getModNumInMod(const TString&);
  ClassDef(R3BGeomTof,0) // Class for STS
};

inline Int_t R3BGeomTof::getModNumInMod(const TString& name) {
  // returns the module index from module name
  return (Int_t)(name[3]-'0')-1;
}

#endif  /* !R3BGEOMTOF_H */
