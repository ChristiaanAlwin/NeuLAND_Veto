// ------------------------------------------------------------
// -----               R3BNveto_segMapped2Cal               -----
// -----          Created 22-04-2014 by D.Kresan          -----
// ------------------------------------------------------------

#include "R3BNveto_segMapped2Cal.h"

#include "R3BTCalEngine.h"
#include "R3BNveto_segMappedData.h"
#include "R3BNveto_segCalData.h"
#include "R3BTCalPar.h"
#include "R3BNveto_segQCalPar.h"
#include "R3BEventHeader.h"

#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairRootManager.h"
#include "FairLogger.h"

#include "TClonesArray.h"
#include "TMath.h"
#include "TH1F.h"

Double_t wlk(Double_t x)
{
    Double_t y = 0;

    Double_t par1 = 1500.;                                                // +-0.2238
    Double_t par2 = 0.00075;                                              //+-2.355e-05
    y = par1 * TMath::Power(x, par2) - (par1 * TMath::Power(400., par2)); // Michael's

    // y=2.29083*log(x)-0.0870157*log(x)*log(x)-4.57824;  // mine

    return y;
    // return 0.;
}

R3BNveto_segMapped2Cal::R3BNveto_segMapped2Cal()
    : FairTask("Nveto_segMapped2Cal", 1)
    , fNEvents(0)
    , fPulserMode(kFALSE)
    , fWalkEnabled(kTRUE)
    , fRawHit(NULL)
    , fPmt(new TClonesArray("R3BNveto_segCalData"))
    , fNPmt(0)
    , fTcalPar(NULL)
    , fQCalPar(NULL)
    , fTrigger(-1)
    , fMap17Seen()
    , fMapStopTime()
    , fMapStopClock()
    , fMapQdcOffset()
    , fClockFreq(1. / TACQUILA_CLOCK_MHZ * 1000.)
{
}

R3BNveto_segMapped2Cal::R3BNveto_segMapped2Cal(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNEvents(0)
    , fPulserMode(kFALSE)
    , fWalkEnabled(kTRUE)
    , fRawHit(NULL)
    , fPmt(new TClonesArray("R3BNveto_segCalData"))
    , fNPmt(0)
    , fTcalPar(NULL)
    , fQCalPar(NULL)
    , fTrigger(-1)
    , fMap17Seen()
    , fMapStopTime()
    , fMapStopClock()
    , fMapQdcOffset()
    , fClockFreq(1. / TACQUILA_CLOCK_MHZ * 1000.)
{
}

R3BNveto_segMapped2Cal::~R3BNveto_segMapped2Cal()
{
    if (fPmt)
    {
        delete fPmt;
        fPmt = NULL;
        fNPmt = 0;
    }
}

InitStatus R3BNveto_segMapped2Cal::Init()
{
    LOG(INFO) << "R3BNveto_segMapped2Cal::Init : read " << fTcalPar->GetNumModulePar() << " calibrated modules"
              << FairLogger::endl;
    // fTcalPar->printParams();

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
    {
        FairLogger::GetLogger()->Fatal(MESSAGE_ORIGIN, "FairRootManager not found");
    }

    header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");
    if (NULL == header)
    {
        FairLogger::GetLogger()->Fatal(MESSAGE_ORIGIN, "Branch R3BEventHeader not found");
    }

    fRawHit = (TClonesArray*)mgr->GetObject("Nveto_segMappedData");
    if (NULL == fRawHit)
    {
        FairLogger::GetLogger()->Fatal(MESSAGE_ORIGIN, "Branch veto_segRawHitMapped not found");
    }

    mgr->Register("Nveto_segCalData", "veto_seg", fPmt, kTRUE);

    fh_pulser_5_2 = new TH1F("h_pulser_5_2", "Single PMT resolution Bar 5 vs 2", 40000, -200., 200.);
    fh_pulser_105_2 = new TH1F("h_pulser_105_2", "Single PMT resolution Bar 105 vs 2", 40000, -200., 200.);

    SetParameter();
    
    return kSUCCESS;
}

void R3BNveto_segMapped2Cal::SetParContainers()
{
    FairRunAna* ana = FairRunAna::Instance();
    FairRuntimeDb* rtdb = ana->GetRuntimeDb();
    fTcalPar = (R3BTCalPar*)(rtdb->getContainer("veto_segTCalPar"));
    fQCalPar = (R3BNveto_segQCalPar*)(rtdb->getContainer("Nveto_segQCalPar"));
}

void R3BNveto_segMapped2Cal::SetParameter()
{
  
  std::map<Int_t, Double_t> tempMapQdcOffset;

  for (Int_t i = 0; i < fQCalPar->GetNumPar(); i++) 
    tempMapQdcOffset[i] = fQCalPar->GetParAt(i);
    
  LOG(INFO) << "R3BNveto_segMapped2Cal::SetParameter : Number of Parameters: " << fQCalPar->GetNumPar() << FairLogger::endl;
  
  fMapQdcOffset = tempMapQdcOffset;
}

InitStatus R3BNveto_segMapped2Cal::ReInit()
{
    SetParContainers();
    SetParameter();
    return kSUCCESS;
}

void R3BNveto_segMapped2Cal::Exec(Option_t* option)
{
    if (fTrigger >= 0)
    {
        if (header->GetTrigger() != fTrigger)
        {
            return;
        }
    }

    Int_t nHits = fRawHit->GetEntriesFast();
    if (fPulserMode)
    {
        if (nHits < fNofPMTs)
        {
            return;
        }
    }
    else
    {
        if (nHits > (fNofPMTs / 2))
        {
            return;
        }
    }

    if(nHits > 0)
    {
        MakeCal();
    }

    if (fPulserMode)
    {
        R3BNveto_segCalData* pmt1;
        Double_t time1;
        for (Int_t i = 0; i < fNPmt; i++)
        {
            pmt1 = (R3BNveto_segCalData*)fPmt->At(i);
            if (pmt1->GetBarId() == 2 && pmt1->GetSide() == 1)
            {
                time1 = pmt1->GetTime();
                break;
            }
        }
        for (Int_t i = 0; i < fNPmt; i++)
        {
            pmt1 = (R3BNveto_segCalData*)fPmt->At(i);
            if (pmt1->GetBarId() == 5 && pmt1->GetSide() == 1)
            {
                fh_pulser_5_2->Fill(pmt1->GetTime() - time1);
            }
            if (pmt1->GetBarId() == 105 && pmt1->GetSide() == 1)
            {
                fh_pulser_105_2->Fill(pmt1->GetTime() - time1);
            }
        }
    }
}

void R3BNveto_segMapped2Cal::MakeCal()
{
    Int_t nHits = fRawHit->GetEntriesFast();
    R3BNveto_segMappedData* hit;
    R3BNveto_segMappedData* hit2;
    Int_t iPlane;
    Int_t iPaddle;
    Int_t iSide;
    Int_t channel;
    Int_t tdc;
    R3BTCalModulePar* par;
    Double_t time;
    Double_t time2;
    Int_t qdc;

    for (Int_t khit = 0; khit < nHits; khit++)
    {
        hit2 = (R3BNveto_segMappedData*)fRawHit->At(khit);
        if (NULL == hit2)
        {
            continue;
        }

        iPlane = hit2->GetPlane();
        iPaddle = hit2->GetPaddle();
        iSide = hit2->GetSide();

        if (!(par = fTcalPar->GetModuleParAt(iPlane, iPaddle, iSide)))
        {
            LOG(DEBUG) << "R3BNveto_segMapped2Cal::Exec : Tcal par not found, channel: " << iPlane << " / " << iPaddle << " / " << iSide
            << FairLogger::endl;
            continue;
        }

        tdc = hit2->GetTacData();
        time = par->GetTimeTacquila(tdc);
        if (time < 0. || time > fClockFreq)
        {
            LOG(ERROR) << "R3BNveto_segMapped2Cal::Exec : error in time calibration: ch=" << channel << ", tdc=" << tdc
            << ", time=" << time << FairLogger::endl;
            continue;
        }

        if (!(par = fTcalPar->GetModuleParAt(iPlane, iPaddle, iSide + 2)))
        {
            LOG(DEBUG) << "R3BNveto_segMapped2Cal::Exec : Tcal par not found, channel: " << iPlane << " / " << iPaddle << " / " << (iSide+2)
            << FairLogger::endl;
            continue;
        }
        
        tdc = hit2->GetStopT();
        time2 = par->GetTimeTacquila(tdc);
        if (time2 < 0. || time2 > fClockFreq)
        {
            LOG(ERROR) << "R3BNveto_segMapped2Cal::Exec : error in time calibration: ch=" << channel << ", tdc=" << tdc
            << ", time=" << time2 << FairLogger::endl;
            continue;
        }
  
        qdc = hit2->GetQdcData()-fMapQdcOffset[2*((iPlane-1)*50 + iPaddle) + iSide - 1];        
        qdc = std::max(qdc,0);
	
        time = time - time2 + hit2->GetClock() * fClockFreq;
        if (fWalkEnabled)
        {
            time += wlk(qdc);
        }
        new ((*fPmt)[fNPmt]) R3BNveto_segCalData((iPlane-1)*50 + iPaddle, iSide, time, qdc);
        fNPmt += 1;
    }
}

void R3BNveto_segMapped2Cal::FinishEvent()
{
    if (fVerbose && 0 == (fNEvents % 1000))
    {
        LOG(INFO) << "R3BNveto_segMapped2Cal::Exec : event=" << fNEvents << " nPMTs=" << fNPmt << FairLogger::endl;
    }

    if (fPmt)
    {
        fPmt->Clear();
        fNPmt = 0;
    }
    fMap17Seen.clear();
    fMapStopTime.clear();
    fMapStopClock.clear();

    fNEvents += 1;
}

void R3BNveto_segMapped2Cal::FinishTask()
{
    fh_pulser_5_2->Write();
    fh_pulser_105_2->Write();
}

ClassImp(R3BNveto_segMapped2Cal)
