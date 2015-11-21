#ifndef FSLIBRARY_CXX
#define FSLIBRARY_CXX

#include "FSLibrary.h"

 /**
  * AddEvent is used to add a new event to the vector.
  * The code will check to see if a corresponding FinalState object already exists.
  * If so, it just adds one to the tally for that final state.
  * If not, it will create one and start a new tally.
  */
void FSLibrary::AddEvent(FinalState s) {
  
  // Loop over all existing final states, up the tally & return if we get a match
  for(auto & fs : FinalStates)
    if (s._nPiPlus == fs._nPiPlus && s._nPiMinus == fs._nPiMinus && s._nPiZero == fs._nPiZero
        && s._nGamma == fs._nGamma && s._nProton == fs._nProton && s._nNeutron && fs._nNeutron)
    {
      fs._nEvents++;
      return;
    }
  
  // If we don't get a match, instead create a new final state.
  FinalStates.push_back(s);
  return;
}

void FSLibrary::Summary() {
  
  int total = FinalStates.size();
  int mean  = 0;

  // loop over all final states
  for (auto fs : FinalStates) {
    std::cout << "FINAL STATE TOPOLOGY:" << std::endl;
    std::cout << "  No. pi+s     :  " << fs._nPiPlus  << std::endl;
    std::cout << "  No. pi-s     :  " << fs._nPiMinus << std::endl;
    std::cout << "  No. pi0s     :  " << fs._nPiZero  << std::endl;
    std::cout << "  No. gammas   :  " << fs._nGamma   << std::endl;
    std::cout << "  No. protons  :  " << fs._nProton  << std::endl;
    std::cout << "  No. neutrons :  " << fs._nNeutron << std::endl;
    std::cout << fs._nEvents << " EVENTS WITH THIS TOPOLOGY" << std::endl << std::endl;
    
    mean += fs._nEvents;
  }
  
  mean /= total;
  
  std::cout << "There are " << total << " topologies, with an average of " << mean << " events per topology." << std::endl;
}

#endif
