#ifndef LARLITE_MASSPEAK_CXX
#define LARLITE_MASSPEAK_CXX

#include "MassPeak.h"

namespace larlite {

  bool MassPeak::initialize() {

    // set up output tree
    mass_peak_tree = new TTree("MassPeak","");
    
    mass_peak_tree->Branch("n_showers",&n_showers,"n_showers/D");
    
    mass_peak_tree->Branch("mass_inclusive","std::vector<double>",&mass_inclusive);
    mass_peak_tree->Branch("mass_exclusive","std::vector<double>",&mass_exclusive);
    
    mass_peak_tree->Branch("start_point_distance","std::vector<double>",&start_point_distance);

    return true;
  }
  
  bool MassPeak::analyze(storage_manager* storage) {
  
    // get all mcshowers
    auto ev_mcs = storage->get_data<event_mcshower>("mcreco");
    
    if (!ev_mcs || !(ev_mcs->size())) {
      print(msg::kERROR,__FUNCTION__,"MCShower data product not found!");
      return false;
    }
    
    // get event information
    int event  = ev_mcs->event_id();
    int run    = ev_mcs->run();
    int subrun = ev_mcs->subrun();
    
    // get simchannel information
    auto ev_simch = storage->get_data<event_simch>("largeant");
    if (!ev_simch || !(ev_simch->size())) {
      print(msg::kERROR,__FUNCTION__,"SimChannel data product not found!");
      return false;
    }
    
    // get reconstructed showers
    auto ev_shwr = storage->get_data<event_shower>(shower_producer);
    if (!ev_shwr || !(ev_shwr->size())) {
      print(msg::kERROR,__FUNCTION__,"Shower data product not found!");
      return false;
    }
    
    int num_reco_showers = ev_shower->size();
    
    // get associated clusters
    
    event_cluster * ev_cluster = nullptr;
    auto const& ass_cluster_v = storage->find_one_ass(ev_shower->id(), ev_cluster, ev_shower->name());
    
    // get all reconstructed showers
  
    return true;
  }

  bool MassPeak::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
  
    return true;
  }

}
#endif
