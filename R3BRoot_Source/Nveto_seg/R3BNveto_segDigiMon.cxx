#include "R3BNveto_segDigiMon.h"

#include <iostream>
#include <algorithm>
#include <numeric>

#include "TClonesArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"

#include "FairRootManager.h"
#include "FairLogger.h"

#include "R3BNveto_segDigi.h"

R3BNveto_segDigiMon::R3BNveto_segDigiMon(const Option_t* option)
    : FairTask("R3B Nveto_seg Nveto_segDigi Monitor")
{
    LOG(INFO) << "Using R3B Nveto_seg Nveto_segDigi Monitor" << FairLogger::endl;

    TString opt = option;
    opt.ToUpper();

    if (opt.Contains("3DTRACK"))
    {
        fIs3DTrackEnabled = true;
        LOG(INFO) << "... with 3D track visualization" << FairLogger::endl;
    }
    else
    {
        fIs3DTrackEnabled = false;
    }
}

R3BNveto_segDigiMon::~R3BNveto_segDigiMon() {}

InitStatus R3BNveto_segDigiMon::Init()
{
    FairRootManager* rm = FairRootManager::Instance();

    fDigis = (TClonesArray*)rm->GetObject("Nveto_segDigis");
    if (fDigis == nullptr)
    {
        LOG(FATAL) << "R3BNveto_segDigiMon: No Nveto_segDigis!" << FairLogger::endl;
        return kFATAL;
    }

    if (fIs3DTrackEnabled)
    {
        // XYZ -> ZXY (side view)
        fh3 = new TH3D("hDigis", "hDigis", 60, 1400, 1700, 50, -125, 125, 50, -125, 125);
        fh3->SetTitle("Nveto_seg Digis");
        fh3->GetXaxis()->SetTitle("Z");
        fh3->GetYaxis()->SetTitle("X");
        fh3->GetZaxis()->SetTitle("Y");

        rm->Register("Nveto_segDigiMon", "Digis in Nveto_seg", fh3, kTRUE);
    }

    hDepth = new TH1D("hDepth", "Maxial penetration depth", 60, 1400, 1700);
    hForemostEnergy = new TH1D("hForemostEnergy", "Foremost energy deposition", 100, 0, 100);
    hSternmostEnergy = new TH1D("hSternmostEnergy", "Sternmost energy deposition", 100, 0, 100);
    hDepthVSForemostEnergy = new TH2D("hDepthVSFrontEnergy", "Depth vs Foremost Energy", 60, 1400, 1700, 100, 0, 100);
    hDepthVSSternmostEnergy =
        new TH2D("hDepthVSSternmostEnergy", "Depth vs Sternmost Energy", 60, 1400, 1700, 100, 0, 100);
    hEtot = new TH1D("hEtot", "Total Energy", 10000, 0, 10000);
    hDepthVSEtot = new TH2D("hDepthVSEtot", "Depth vs Total Energy", 60, 1400, 1700, 1000, 0, 1000);
    hPosVSEnergy = new TH2D("hPosVSEnergy", "Position vs Energy deposition", 60, 1400, 1700, 1000, 0, 1000);

    return kSUCCESS;
}

void R3BNveto_segDigiMon::Exec(Option_t*)
{
    const unsigned int nDigis = fDigis->GetEntries();

    if (fIs3DTrackEnabled)
    {
        fh3->Reset("ICES");
        R3BNveto_segDigi* digi;
        for (unsigned int i = 0; i < nDigis; i++)
        {
            digi = (R3BNveto_segDigi*)fDigis->At(i);
            // XYZ -> ZXY (side view)
            fh3->Fill(digi->GetPosition().Z(), digi->GetPosition().X(), digi->GetPosition().Y(), digi->GetE());
        }
    }

    std::vector<R3BNveto_segDigi*> digis;
    for (unsigned int i = 0; i < nDigis; i++)
    {
        digis.push_back((R3BNveto_segDigi*)fDigis->At(i));
    }

    for (auto digi : digis)
    {
        hPosVSEnergy->Fill(digi->GetPosition().Z(), digi->GetE());
    }

    auto maxDepthDigi = std::max_element(digis.begin(),
                                         digis.end(),
                                         [](R3BNveto_segDigi* a, R3BNveto_segDigi* b)
                                         {
                                             return a->GetPosition().Z() < b->GetPosition().Z();
                                         });
    if (maxDepthDigi != digis.end())
    {
        hDepth->Fill((*maxDepthDigi)->GetPosition().Z());
        hSternmostEnergy->Fill((*maxDepthDigi)->GetE());
        hDepthVSSternmostEnergy->Fill((*maxDepthDigi)->GetPosition().Z(), (*maxDepthDigi)->GetE());
    }

    auto minDepthDigi = std::min_element(digis.begin(),
                                         digis.end(),
                                         [](R3BNveto_segDigi* a, R3BNveto_segDigi* b)
                                         {
                                             return a->GetPosition().Z() < b->GetPosition().Z();
                                         });
    if (minDepthDigi != digis.end())
    {
        hForemostEnergy->Fill((*minDepthDigi)->GetE());
        hDepthVSForemostEnergy->Fill((*maxDepthDigi)->GetPosition().Z(), (*minDepthDigi)->GetE());
    }

    auto Etot = std::accumulate(digis.begin(),
                                digis.end(),
                                Double_t(0.),
                                [](const Double_t a, R3BNveto_segDigi* b)
                                {
                                    return a + b->GetE();
                                });
    hEtot->Fill(Etot);
    if (maxDepthDigi != digis.end())
    {
        hDepthVSEtot->Fill((*maxDepthDigi)->GetPosition().Z(), Etot);
    }
}

void R3BNveto_segDigiMon::Finish()
{
    hDepth->Write();
    hForemostEnergy->Write();
    hSternmostEnergy->Write();
    hDepthVSForemostEnergy->Write();
    hDepthVSSternmostEnergy->Write();
    hEtot->Write();
    hDepthVSEtot->Write();
    hPosVSEnergy->Write();
}

ClassImp(R3BNveto_segDigiMon)
