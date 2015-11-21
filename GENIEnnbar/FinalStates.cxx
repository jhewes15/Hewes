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
    
    std::cout << "|-----------------------------------------------|" << std::endl;
    std::cout << "|                 EVENT SUMMARY                 |" << std::endl;
    std::cout << "|-----------------------------------------------|" << std::endl;
    
    event_mctruth * ev_truth = storage->get_data<event_mctruth>("generator");
    
    int nPiPlus  = 0;
    int nPiMinus = 0;
    int nPiZero  = 0;
    int nGamma   = 0;
    int nProton  = 0;
    int nNeutron = 0;
    
    for (mctruth a : * ev_truth) {
      std::vector<mcpart> particles(a.GetParticles());
      
      for (mcpart p : particles) {
        if (p.StatusCode() == 1) {
          if (p.PdgCode() == 211)       nPiPlus++;
          else if (p.PdgCode() == -211) nPiMinus++;
          else if (p.PdgCode() == 111)  nPiZero++;
          else if (p.PdgCode() == 22)   nGamma++;
          else if (p.PdgCode() == 2112) nProton++;
          else if (p.PdgCode() == 2212) nNeutron++;
          else std::cout << "PDG code not a pion but something else! We have " << p.PdgCode() << ", whatever that is." << std::endl;
        }
      }
    }
    
    std::cout << "| " << nPiPlus << " pi+, " << nPiMinus << " pi-, " << nPiZero << " pi0                           |" << std::endl;
    std::cout << "| " << nGamma << " gamma, " << nProton << " proton, " << nNeutron << " neutron                  |" << std::endl;
    std::cout << "|-----------------------------------------------|" << std::endl << std::endl;
  
    return true;
  }

  bool FinalStates::finalize() {
  
    return true;
  }

}
#endif
