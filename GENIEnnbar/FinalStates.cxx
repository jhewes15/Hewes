#ifndef LARLITE_FINALSTATES_CXX
#define LARLITE_FINALSTATES_CXX

#include "FinalStates.h"

namespace larlite {

  bool FinalStates::initialize() {

    _FSLibrary = new FSLibrary();

    return true;
  }
  
  bool FinalStates::analyze(storage_manager* storage) {
    
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "                  EVENT SUMMARY                  " << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
  
    auto particles = storage->get_data<event_mcpart>("generator");
    int nPiPlus, nPiMinus, nPiZero;
    
    for (auto p : *particles)
    {
      if (p.StatusCode() == 1)
      {
        std::cout << "Final state particle w/ PDG code " << p.PdgCode() << "." << std::endl;
      }
    }
    
    std::cout << "-------------------------------------------------" << std::endl << std::endl;
  
    return true;
  }

  bool FinalStates::finalize() {
  
    return true;
  }

}
#endif
