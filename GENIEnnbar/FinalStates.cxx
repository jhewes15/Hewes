#ifndef LARLITE_FINALSTATES_CXX
#define LARLITE_FINALSTATES_CXX

#include "FinalStates.h"

namespace larlite {

  bool FinalStates::initialize() {

    _FSLibrary = new FSLibrary();

    return true;
  }
  
  bool FinalStates::analyze(storage_manager* storage) {
    
    // for now, this function just lists all the final state particles in each event
    
    // once I know what to expect, can implement the library function instead
    
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "                  EVENT SUMMARY                  " << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
  
    auto particles = storage->get_data<event_mcpart>("generator");
    int nPiPlus, nPiMinus, nPiZero;
    
    for (auto p : *particles)
    {
      if (p.StatusCode() == 1)
      {
        if (p.PdgCode() == 211)       nPiPlus++;
        else if (p.PdgCode() == -211) nPiMinus++;
        else if (p.PdgCode() == 111)  nPiZero++;
        else
        {
          std::cout << "PDG code not a pion but something else! We have " << p.PdgCode() ", whatever that is." << std::endl;
        }
      }
    }
    std::cout << nPiPlus << " pi+, " << nPiMinus << " pi-, " << nPiZero << " pi0." << std::endl;
    std::cout << "-------------------------------------------------" << std::endl << std::endl;
  
    return true;
  }

  bool FinalStates::finalize() {
  
    return true;
  }

}
#endif
