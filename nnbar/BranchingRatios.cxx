#ifndef LARLITE_BRANCHINGRATIOS_CXX
#define LARLITE_BRANCHINGRATIOS_CXX

#include "BranchingRatios.h"

namespace larlite {

  bool BranchingRatios::initialize() {

    _tree = new TTree("br_tree","");
    _tree->Branch("final_state",&final_state,"final_state/I");

    return true;
  }
  
  bool BranchingRatios::analyze(storage_manager* storage) {
  
    final_state = -1;

    mctruth truth = (*(storage->get_data<event_mctruth>("generator")))[0];
    std::vector<mcpart> particles(truth.GetParticles());

    n_piplus  = 0;
    n_piminus = 0;
    n_pizero  = 0;
    n_omega   = 0;

    for (auto part : particles) {
      switch (part.Pdg()) {
        case 211:
          n_piplus++;
          break;
        case -211:
          n_piminus++;
          break;
        case 111:
          n_pizero++;
          break;
        case 322:
          n_omega++;
          break;
      }
    }
    
    final_state = topology();
    _tree->Fill();

    return true;
  }

  bool BranchingRatios::finalize() {

    if (_fout)
      _tree->Write();
  
    return true;
  }

}
#endif
