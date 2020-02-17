#include "R3BEventManager.h"
#include "R3BIonName.h"
#include "TEveManager.h"
#include "TEveGeoNode.h"
#include "TGeoManager.h"
#include "TDatabasePDG.h"


#include <iostream>
using namespace std;


R3BEventManager *R3BEventManager::fgRinstance= 0;

R3BEventManager * R3BEventManager::Instance(){
        return fgRinstance;
}


R3BEventManager::R3BEventManager()
  :FairEventManager()
{
  cout << " calling ctor Event Manager" << endl;
   fgRinstance=this;

}

void R3BEventManager::AddParticlesToPdgDataBase(Int_t pdgCode){

  TDatabasePDG *pdgDB = TDatabasePDG::Instance();
 
 if ( !pdgDB->GetParticle(pdgCode))
  {
    int temp=pdgCode/1E8;
    temp = pdgCode - temp*1E8;
    int element = temp/1E4;
    int mass = (temp - element*1E4)/10;

    bool particleRecognised=true;
    char name[20];

    particleRecognised = ((R3BIonName*) fIonName)->GetIonName(element,name);
    
    if (particleRecognised)
    {
      char title[20];
      sprintf(title, "%s%i",name, mass);
      pdgDB->AddParticle(name, title, mass, kTRUE,
                         0, 0, "Ion", pdgCode);
    }
  }  
}


ClassImp(R3BEventManager)
