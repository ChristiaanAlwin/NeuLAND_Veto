#include "R3BBackTrackingStorageState.h"
#include <iostream>

using namespace std;


R3BBackTrackingStorageState::R3BBackTrackingStorageState()
  : p0(1101.),      //central value of the theoretical momentum calculation
    pcent(1101.),
    pmax(1250.),    //maximum value of the theoretical momentum calculation
    pmin(950.),     //minimum value of the theoretical momentum calculation
    pmaxabs(1250.), //maximum value of the theoretical momentum calculation
    pminabs(950.),  //minimum value of the theoretical momentum calculation
    pfinal(0.),     //calculated value for the momentum module
    piter(0.),      //Momentum in each iteration
    niter(0),       //iteration number
    converg(1),
    x0tar(0.), y0tar(0.), z0tar(0.), pPrim(0.),
    xSim(0.), ySim(0.), zSim(0.)
{
  //
  // Constructor
  //
}

 
R3BBackTrackingStorageState::~R3BBackTrackingStorageState()
{
  //
  // Destructor
  // 
}


void R3BBackTrackingStorageState::ReadNewLine()
{
  cout << "-I- R3BBackTrackingStorageState::ReadNewLine() not implemented " << endl;
}


ClassImp(R3BBackTrackingStorageState)
