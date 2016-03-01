#ifndef LARLITE_PI0SHOWERFILTER_CXX
#define LARLITE_PI0SHOWERFILTER_CXX

#include "Pi0ShowerFilter.h"

namespace larlite {

  bool Pi0ShowerFilter::initialize() {
        
    return true;

  }
  
  bool Pi0ShowerFilter::analyze(storage_manager* storage) {

    bool keep = false;

    auto particles = storage->get_data<event_pfpart>("pandoraNu");
    if(!particles) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, pfpart!"));
      return false;
    }

    for (auto particle : *particles) {
      std::cout << "Particle's pdg code is " << particle.PdgCode() << "." << std::endl;
    }

    keep = true;

    return keep;

  }

  bool Pi0ShowerFilter::finalize() {

    return true;
  }

}
#endif
