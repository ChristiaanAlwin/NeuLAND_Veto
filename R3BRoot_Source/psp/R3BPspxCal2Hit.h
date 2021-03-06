// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           R3BPspxCal2Hit                          -----
// -----		   created 09-03-2016 by I. Syndikus		   -----
// -----                                                                   -----
// -----------------------------------------------------------------------------
/* Convert Cal data to Hit data
 */

#ifndef R3BPSPXCAL2HIT_H
#define R3BPSPXCAL2HIT_H

#include "FairTask.h"

const static UInt_t ndetectors = 5;
const static UInt_t nstrips = 16;
const static UInt_t length_array_detector = ndetectors+1;
const static UInt_t length_array_strip= nstrips+1;

const static Double_t detector_length=5.; // in cm
const static Double_t strip_width=detector_length/nstrips; // in cm

class TClonesArray;
class R3BEventHeader;
class R3BPspxHitPar;

class R3BPspxCal2Hit : public FairTask
{
  public:
    R3BPspxCal2Hit();
    R3BPspxCal2Hit(const char* name, Int_t iVerbose);
    virtual ~R3BPspxCal2Hit();

    virtual InitStatus Init();    
    InitStatus ReInit();
    void SetParContainers();

    virtual void Exec(Option_t* option);

    virtual void FinishEvent();
    virtual void FinishTask();

  private:

    R3BEventHeader *fHeader;    // do we need that?
    TClonesArray* fCalItems; // holding input data
    TClonesArray* fHitItems;    // holding output data
    
    R3BPspxHitPar* fHitPar; // holding slopes and offsets
    std::vector<vector<Float_t>> offset;
    std::vector<vector<Float_t>> slope;
    std::vector<Int_t> sign_x;
    std::vector<Int_t> sign_y;

    // void CreateHistos();
    // void WriteHistos();

  public:
    ClassDef(R3BPspxCal2Hit, 1)
};

#endif
