// include the required C++ headers:
#include <iostream>

// include the required root headers:

// include own headers:
#include "../Analysis/Inputs_Load.h"
#include "AnaLeafs_Connect.h"

// use the standard C++ variables:
using namespace std;

void VETOedParticles()
{
  // This function counts the number of VETOed, Detected and Kept particles
  // within one run. It then prints a table on the screen with results.
    
  // Written by C. A. Douma.
    
  // ===================================================================================
  // Before we do anything, we will obtain the Counter and the Series from sed:
  Int_t Series = 0;
Series = 0;

  Int_t Counter = 0;
Counter = 1;

  cout << "We now read out a single analysis tree.\n\n"
       << "=========================================================================\n\n";
  
  // We begin with connecting to the Inputs.root-file:
  Inputs_Structure TheInputs;
  Inputs_Structure* Inputs = &TheInputs;
  Bool_t Load = Inputs_Load(Inputs,Counter,Series);
  
  // Then connect to the analysis tree:
  AnaLeafs_Structure TheLeafs;
  AnaLeafs_Structure* AnaLeafs = &TheLeafs;
  Bool_t Connect = AnaLeafs_Connect(AnaLeafs,Counter);
  
  // Then check if everything worked:
  if (!(Connect&&Load))
  {
      cout << "### ERROR: Some initializers could not be created!\n"
           << "### ERROR: Without these initializers this task could not be performed!\n"
           << "### ERROR: Therefore the program is now terminated!\n\n";
  }
  else
  {
      cout << "All initializers are succesfully created.\n\n"
           << "======================================================================\n\n"
           << "We will now loop over the tree.\n\n";

  // ====================================================================================
  // Now get everything that we need for the event loop:
  Long64_t nEvents = Inputs->nEvents;
  Long64_t nBytes = 0;
  
  // We first declare what we need:
  Int_t Total_Particles[11];
  Int_t VETOed_Particles[11];
  Int_t Kept_Particles[11];
  Int_t Resolved_Particles[11];
  
  Int_t GoodEvents = 0;
  Int_t NeutronCounter = 0;
  Int_t VETOedCounter = 0;
  Bool_t ThisEventIsGood = kFALSE;
  
  for (Int_t k = 0; k<11; ++k)
  {
      Total_Particles[k] = 0;
      VETOed_Particles[k] = 0;
      Kept_Particles[k] = 0;
      Resolved_Particles[k] = 0;
  }
  Int_t Nprims = 0;
  
  // Now perform the event loop over the tree:
  for (Long64_t Event = 0; Event<nEvents; ++Event)
  {
      // read out an event:
      nBytes = AnaLeafs->TheTree->GetEntry(Event);
      
      // Reset the boolian for good events:
      ThisEventIsGood = kTRUE;
      NeutronCounter = 0; // number of neutrons that passed the tests.
      VETOedCounter = 0; // number of other particles that passed the tests AND are detected!
      
      // now we simply start counting particles:
      Nprims = AnaLeafs->Nprims_NeuLAND->GetValue(0);
      
      if (Nprims>0)
      {
          for (Int_t k = 0; k<Nprims; ++k)
          {
              if (AnaLeafs->Prim_PDG->GetValue(k)==2212) // protons
              {
                  Total_Particles[0] = Total_Particles[0] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[0] = VETOed_Particles[0] + 1;}
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Kept_Particles[0] = Kept_Particles[0] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[0] = Resolved_Particles[0] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {ThisEventIsGood = kFALSE;}
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOedCounter = VETOedCounter + 1;}
              }
              
              else if (AnaLeafs->Prim_PDG->GetValue(k)==2112) // neutrons
              {
                  Total_Particles[1] = Total_Particles[1] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[1] = VETOed_Particles[1] + 1;}
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Kept_Particles[1] = Kept_Particles[1] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[1] = Resolved_Particles[1] + 1;}
                  
                  // For neutrons: each neutron should be detected, resolved and NOT VETOed:
                  if (!((AnaLeafs->Prim_Detected->GetValue(k))&&(AnaLeafs->Prim_Resolved->GetValue(k))&&(!(AnaLeafs->Prim_IsVETOed->GetValue(k))))) {ThisEventIsGood = kFALSE;} else {NeutronCounter = NeutronCounter + 1;}
              }
              
              else if (AnaLeafs->Prim_PDG->GetValue(k)==11) // Electrons
              {
                  Total_Particles[2] = Total_Particles[2] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[2] = VETOed_Particles[2] + 1;}
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Kept_Particles[2] = Kept_Particles[2] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[2] = Resolved_Particles[2] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {ThisEventIsGood = kFALSE;}
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOedCounter = VETOedCounter + 1;}
              }
              
              else if (AnaLeafs->Prim_PDG->GetValue(k)==22) // Gammas
              {
                  Total_Particles[3] = Total_Particles[3] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[3] = VETOed_Particles[3] + 1;}
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Kept_Particles[3] = Kept_Particles[3] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[3] = Resolved_Particles[3] + 1;}
                  
                  // these should simply not be detected:
                  if (AnaLeafs->Prim_Detected->GetValue(k)) {ThisEventIsGood = kFALSE;} // NOTE: no gamma should be detected. hence no counter increase here!
              }
              
              else if (TMath::Abs(AnaLeafs->Prim_PDG->GetValue(k))==211) // pions
              {
                  Total_Particles[4] = Total_Particles[4] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[4] = VETOed_Particles[4] + 1;}
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Kept_Particles[4] = Kept_Particles[4] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[4] = Resolved_Particles[4] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {ThisEventIsGood = kFALSE;}
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOedCounter = VETOedCounter + 1;}
              }
              
              else if (TMath::Abs(AnaLeafs->Prim_PDG->GetValue(k))==13) // muons
              {
                  Total_Particles[5] = Total_Particles[5] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[5] = VETOed_Particles[5] + 1;}
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Kept_Particles[5] = Kept_Particles[5] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[5] = Resolved_Particles[5] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {ThisEventIsGood = kFALSE;} else {VETOedCounter = VETOedCounter + 1;}
              }
              
              else if (AnaLeafs->Prim_PDG->GetValue(k)==1000020040) // Alphas
              {
                  Total_Particles[6] = Total_Particles[6] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[6] = VETOed_Particles[6] + 1;}
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Kept_Particles[6] = Kept_Particles[6] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[6] = Resolved_Particles[6] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {ThisEventIsGood = kFALSE;}
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOedCounter = VETOedCounter + 1;}
              }
              
              else if ((AnaLeafs->Prim_PDG->GetValue(k)>1000060000)&&(AnaLeafs->Prim_PDG->GetValue(k)<1000060190)) // Cions
              {
                  Total_Particles[7] = Total_Particles[7] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[7] = VETOed_Particles[7] + 1;}
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Kept_Particles[7] = Kept_Particles[7] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[7] = Resolved_Particles[7] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {ThisEventIsGood = kFALSE;}
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOedCounter = VETOedCounter + 1;}
              }
              
              else if (AnaLeafs->Prim_PDG->GetValue(k)>1000000000) // Fragments
              {
                  Total_Particles[8] = Total_Particles[8] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[8] = VETOed_Particles[8] + 1;}
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Kept_Particles[8] = Kept_Particles[8] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[8] = Resolved_Particles[8] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {ThisEventIsGood = kFALSE;}
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOedCounter = VETOedCounter + 1;}
              }
              
              else if (AnaLeafs->Prim_PDG->GetValue(k)==-11) // Positrons
              {
                  Total_Particles[9] = Total_Particles[9] + 1;
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[9] = VETOed_Particles[9] + 1;}
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Kept_Particles[9] = Kept_Particles[9] + 1;}
                  if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[9] = Resolved_Particles[9] + 1;}
                  
                  // For each charged particle it is the case: if it is detected but not VETOed, we are in trouble!
                  if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {ThisEventIsGood = kFALSE;}
                  if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOedCounter = VETOedCounter + 1;}
              }
              
              else {cout << "### ERROR: We have an unidentified primary particle! | Event = " << Event << " | PDG = " << AnaLeafs->Prim_PDG->GetValue(k) << " | Index = " << k << "\n";}
              
              Total_Particles[10] = Total_Particles[10] + 1;
              if ((AnaLeafs->Prim_IsVETOed->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {VETOed_Particles[10] = VETOed_Particles[10] + 1;}
              if ((!(AnaLeafs->Prim_IsVETOed->GetValue(k)))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Kept_Particles[10] = Kept_Particles[10] + 1;}
              if ((AnaLeafs->Prim_Resolved->GetValue(k))&&(AnaLeafs->Prim_Detected->GetValue(k))) {Resolved_Particles[10] = Resolved_Particles[10] + 1;}
              
          }
      }
      
      // Now after we did all the particles: increase the good event counter. 
      // NOTE: in addition, the number of detected & VETOed particles + the number of good neutrons should equal the number of reconstructed points:
      if ((ThisEventIsGood)&&(Nprims>0)&&((NeutronCounter+VETOedCounter)==(AnaLeafs->N_Reconstructed_PrimPoints->GetValue(0)))) {GoodEvents = GoodEvents + 1;}

      // Give a sign of life:
      if ((Event+1)%1000==0) {cout << "We processed " << Event+1 << " Events\n";}
  }
  
  cout << "The AnalysisTree is read out succesfully.\n\n"
       << "==========================================================================\n\n";
  
  // ====================================================================================
  // Now write down our results:
       
  cout << "Identified primary particles:\n\n";
  cout << "Species:    %VETOed:     %Kept:     %Resolved:    Total:\n";
       
  for (Int_t k = 0; k<11; ++k)
  {
      if (k==0) {cout << "Protons:    ";}
      if (k==1) {cout << "Neutrons:   ";}
      if (k==2) {cout << "Electrons:  ";}
      if (k==3) {cout << "Gammas:     ";}
      if (k==4) {cout << "Pions:      ";}
      if (k==5) {cout << "Muons:      ";}
      if (k==6) {cout << "Alphas:     ";}
      if (k==7) {cout << "C-ions:     ";}
      if (k==8) {cout << "Fragments:  ";}
      if (k==9) {cout << "Positrons:  ";}
      if (k==10) {cout << "Total:      ";}
      
      if (Total_Particles[k]>0) {
      cout << 100.0*((Int_t) VETOed_Particles[k])/((Int_t) Total_Particles[k]) << "           "
           << 100.0*((Int_t) Kept_Particles[k])/((Int_t) Total_Particles[k]) << "           "
           << 100.0*((Int_t) Resolved_Particles[k])/((Int_t) Total_Particles[k]) << "           "
           << Total_Particles[k] << "\n";}
      else
      {
      cout << "0           0           0\n";
      }
  }
  
  // Print the number of events:
  cout << "\n\nTotal Number of Events = " << nEvents << "\n"
           << "Number of fully solved Events = " << GoodEvents << "\n\n\n";
  
  // Finish:
  cout << "Program is finished now.\n\n\n";
  // Done!
}
}

// Now define the main for stand-alone compilation:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("VETOedParticles",&argc,argv);
  VETOedParticles();
  // TheApp->Run();
  return 0;
}
#endif