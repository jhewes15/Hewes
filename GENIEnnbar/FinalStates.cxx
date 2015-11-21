#ifndef LARLITE_FINALSTATES_CXX
#define LARLITE_FINALSTATES_CXX

#include "FinalStates.h"

namespace larlite {

  bool FinalStates::initialize() {

    _FSLibrary = new FSLibrary();

    return true;
  }
  
  bool FinalStates::analyze(storage_manager* storage) {
    
    // get mctruth info out of input info holder
    event_mctruth * ev_truth = storage->get_data<event_mctruth>("generator");
    
    // create a final state object for this event
    FinalState fs;
    fs._nEvents  = 1;
    fs._nPiPlus  = 0;
    fs._nPiMinus = 0;
    fs._nPiZero  = 0;
    fs._nGamma   = 0;
    fs._nProton  = 0;
    fs._nNeutron = 0;
    
    // get the mcparticles for this event
    for (mctruth a : * ev_truth) {
      std::vector<mcpart> particles(a.GetParticles());
      
      // loop over each particle, and add it to the tally
      for (mcpart p : particles) {
        if (p.StatusCode() == 1) {
          if (p.PdgCode() == 211)       fs._nPiPlus++;
          else if (p.PdgCode() == -211) fs._nPiMinus++;
          else if (p.PdgCode() == 111)  fs._nPiZero++;
          else if (p.PdgCode() == 22)   fs._nGamma++;
          else if (p.PdgCode() == 2112) fs._nProton++;
          else if (p.PdgCode() == 2212) fs._nNeutron++;
          else std::cout << "PDG code not a pion but something else! We have " << p.PdgCode() << ", whatever that is." << std::endl;
        }
      }
    }
    
    // pass this event on to the final state library
    _FSLibrary -> AddEvent(fs);
    
    // throw some debug output out, if that's your kind of thing
    if (_debug) {
      std::cout << "|-----------------------------------------------|" << std::endl;
      std::cout << "|                 EVENT SUMMARY                 |" << std::endl;
      std::cout << "|-----------------------------------------------|" << std::endl;
      std::cout << "| " << fs._nPiPlus << " pi+, " << fs._nPiMinus << " pi-, "
      << fs._nPiZero << " pi0                           |" << std::endl;
      std::cout << "| " << fs._nGamma << " gamma, " << fs._nProton << " proton, "
      << fs._nNeutron << " neutron                  |" << std::endl;
      std::cout << "|-----------------------------------------------|" << std::endl << std::endl;
    }
    
    return true;
  }

  bool FinalStates::finalize() {
    
    _FSLibrary -> Summary();
  
    return true;
  }

}
#endif
