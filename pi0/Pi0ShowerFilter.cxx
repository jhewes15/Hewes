#ifndef LARLITE_PI0SHOWERFILTER_CXX
#define LARLITE_PI0SHOWERFILTER_CXX

#include "Pi0ShowerFilter.h"

namespace larlite {

  bool Pi0ShowerFilter::initialize() {
        
    return true;

  }
  
  bool Pi0ShowerFilter::analyze(storage_manager* storage) {

    auto particles = storage->get_data<event_pfpart>("pandoraNu");
    if(!particles) {
      print(larlite::msg::kWARNING,__FUNCTION__,Form("Did not find specified data product, pfpart!"));
      return false;
    }

    int n_showers = 0;

    for (auto particle : *particles) {
      //if (particle.PdgCode() == 11) n_showers++;
      print(larlite::msg::kNORMAL,__FUNCTION__,Form("This particle's PDG code is %i",particle.PdgCode()));
    }

    //if (n_showers < 2) {
    //  print(larlite::msg::kWARNING,__FUNCTION__,Form("Fewer than 2 showers in this event, not continuing any further."));
    //  return false;
    //}

    return true;

  }

  bool Pi0ShowerFilter::finalize() {

    return true;
  }

}
#endif
