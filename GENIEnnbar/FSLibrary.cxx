#ifndef FSLIBRARY_CXX
#define FSLIBRARY_CXX

#include "FSLibrary.h"

 /**
  * AddEvent is used to add a new event to the vector.
  * The code will check to see if a corresponding FinalState object already exists.
  * If so, it just adds one to the tally for that final state.
  * If not, it will create one and start a new tally.
  * The input arguments are fairly self-explanatory, but I'll run through them as well.
  *     int nPiPlus  - number of pi+ particles in final state
  *     int nPiMinus - number of pi- particles in final state
  *     int nPiZero  - number of pi0 particles in final state
  */
void FSLibrary::AddEvent(int nPiPlus, int nPiMinus, int nPiZero) {
  
  // Loop over all existing final states, up the tally & return if we get a match
  for(auto fs : FinalStates)
    if (nPiPlus == fs._nPiPlus && nPiMinus == fs._nPiMinus && nPiZero == fs._nPiZero)
    {
      fs._nEvents++;
      return;
    }
  
  // If we don't get a match, instead create a new final state.
  FinalState s;
  s._nEvents  = 1;
  s._nPiPlus  = nPiPlus;
  s._nPiMinus = nPiMinus;
  s._nPiZero  = nPiZero;
  FinalStates.push_back(s);
  return;
}

#endif
