#ifndef LARLITE_TRACKQUALITY_CXX
#define LARLITE_TRACKQUALITY_CXX

#include "TrackQuality.h"

namespace larlite {

  bool TrackQuality::initialize() {
    
    _tree= new TTree("TrackQualityTree","");
    _tree->Branch("track_length","std::vector<double>",&track_length);
    _tree->Branch("pdg_code"    ,"std::vector<double>",&pdg_code    );
    _tree->Branch("track_energy","std::vector<double>",&track_energy);

    return true;
  }
  
  bool TrackQuality::analyze(storage_manager* storage) {
  
    // loop over all mctracks
    auto ev_mct = storage->get_data<event_mctrack>("mcreco");
    if(!ev_mct || !ev_mct->size()) {
      //print(larlite::msg::kWARNING,__FUNCTION__,Form("No mctrack information found in event!"));
      return false;
    }
    
    for (auto track : * ev_mct) {
      track_length.push_back(track.size());
      pdg_code.push_back(track.PdgCode());
      track_energy.push_back(track.Start().E());
    }
      
    _tree->Fill();
    track_length.clear();
    pdg_code.clear();
    track_energy.clear();
  
    return true;
  }

  bool TrackQuality::finalize() {
  
    if(_fout)
      _tree->Write();
    
    return true;
  }

}
#endif
