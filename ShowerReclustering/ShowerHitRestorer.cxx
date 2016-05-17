#ifndef LARLITE_SHOWERHITRESTORER_CXX
#define LARLITE_SHOWERHITRESTORER_CXX

#include "ShowerHitRestorer.h"

namespace larlite {
  
  
  ////////////////////////////////////
  //                                //
  //           INITIALIZE           //
  //                                //
  ////////////////////////////////////
  
  
  bool ShowerHitRestorer::initialize() {
    
    // set input shower producer
    if (_input_producer.empty())
      _input_producer = "showerreco";
    
    if (_output_producer.empty())
      _output_producer = "ShowerHitRestorer";
      
      // Set up output ana tree
      if (_tree_name.empty())
        _tree_name = "ShowerHitRestorer";
    _tree = new TTree(_tree_name.c_str(),"");
    _tree->Branch("_n_showers",&_n_showers,"_n_showers/I");
    _tree->Branch("_missing_charge","std::vector<double>",&_missing_charge);
    _tree->Branch("_hit_candidates","std::vector<std::vector<std::vector<larutil::PxHit>>>",&_hit_candidates);
    _tree->Branch("_associated_hits","std::vector<std::vector<std::vector<larutil::PxHit>>>",&_associated_hits);
    _tree->Branch("_cone_projection","std::vector<std::vector<std::vector<larutil::PxPoint>>>",&_cone_projection);
    
    return true;
  }
  
  
  ////////////////////////////////////
  //                                //
  //            ANALYZE             //
  //                                //
  ////////////////////////////////////
  
  
  bool ShowerHitRestorer::analyze(storage_manager* storage) {
    
    // clear an eventwise vector
    _missing_charge.clear();
    _missing_charge.resize(_n_planes);
    
    // get showerreco showers
    event_shower * ev_shower = storage->get_data<event_shower>(_input_producer);
    
    // check the showers are good
    if (!ev_shower) {
      print(msg::kWARNING,__FUNCTION__,Form("No showerreco shower data object found in this event."));
      return false;
    }
    else if (ev_shower->size() < 1) {
      print(msg::kWARNING,__FUNCTION__,Form("Showerreco shower data object in this event is empty."));
      return false;
    }
    
    // count showers
    _n_showers = ev_shower->size();
    
    // get clusters and shower-cluster associations
    event_cluster * ev_cluster = nullptr;
    auto shower_cluster_assoc = storage->find_one_ass(ev_shower->id(), ev_cluster, ev_shower->name());
    
    if(!ev_cluster)
      print(msg::kWARNING,__FUNCTION__,Form("No cluster data object found associated with these showers."));
    else if (ev_cluster->size() < 1)
      print(msg::kWARNING,__FUNCTION__,Form("Cluster data object associated with these showers is empty."));
    
    // get hits and cluster-hit associations
    event_hit * ev_hit = nullptr;
    auto cluster_hit_assoc = storage->find_one_ass(ev_cluster->id(), ev_hit, ev_cluster->name());
    
    if (!ev_hit)
      print(msg::kWARNING,__FUNCTION__,Form("No hit data object found associated with these clusters."));
    else if (ev_hit->size() < 1)
      print(msg::kWARNING,__FUNCTION__,Form("Hit data object associated with these clusters is empty."));
    
    //for (auto const& hit : * ev_hit)
      //print(msg::kNORMAL,__FUNCTION__,Form("The plane of this hit is %c",MakePxHit(hit).plane));
    
    // get hit-cluster associations
    event_cluster * throwaway = nullptr;
    auto hit_cluster_assoc = storage->find_one_ass(ev_hit->id(), throwaway, ev_cluster->name());
    hit_cluster_assoc.resize(ev_hit->size());
    
    // create a vector to hold unclustered hits for this event
    std::vector<larutil::PxHit> unclustered_hits;
    std::vector<unsigned int> unclustered_hits_id;
    
    // search for unclustered hits and fill vector
    for (unsigned int hit_id = 0; hit_id < ev_hit->size(); hit_id++) {
      auto const& hit = ev_hit->at(hit_id);
      if (!hit_cluster_assoc[hit_id].size()) {
        unclustered_hits.push_back(MakePxHit(hit));
        unclustered_hits_id.push_back(hit_id);
      }
    }
    
    // vector to count how many clusters a currently unclustered hit is matched with
    std::vector<std::vector<unsigned int>> hit_cluster_match;
    hit_cluster_match.resize(unclustered_hits.size());
    
    // loop over each shower in the event
    for (int shower_id = 0; shower_id < ev_shower->size(); shower_id++) {
      
      // initialise bool that we can set to false later if something gets screwed up
      bool good_shower = true;
      
      // get the shower object associated with this shower id
      auto const& shower = ev_shower->at(shower_id);
      
      std::vector<unsigned int> hit_candidates;
      std::vector<larutil::PxHit> associated_hits;
      std::vector<larutil::PxHit> new_pxhits_in_cone;
      std::vector<larutil::PxPoint> cone_projection;
      
      // loop over clusters associated with this shower
      for (unsigned int cluster_id : shower_cluster_assoc[shower_id]) {
        
        // loop over every hit associated with this cluster & create pxhit objects
        for (unsigned int hit_id : cluster_hit_assoc[cluster_id])
          associated_hits.push_back(MakePxHit(ev_hit->at(hit_id)));
      }
      
      // loop over clusters again
      for (unsigned int cluster_id : shower_cluster_assoc[shower_id]) {

        // get the cluster object
        auto const& cluster = ev_cluster->at(cluster_id);
        
        // get the plane this cluster is in
        int plane = cluster.View();
        
        // get all the unclustered hits in this plane
        std::vector<larutil::PxHit> unclustered_hits_in_plane;
        for (auto const& pxhit : unclustered_hits)
          if (pxhit.plane == plane)
            unclustered_hits_in_plane.push_back(pxhit);
        
        // try-catch to avoid segfaults
        try {
          
          // get 2D polygon representation of 3D shower cone
          std::vector<larutil::PxPoint> cone_2d = fgeoconic.ConicalFeatures(shower.ShowerStart(),shower.Direction(),shower.Length(),
                                                                            shower.OpeningAngle()*(180/M_PI),plane,_smoothness);
          
          for (auto const& point : cone_2d)
            print(msg::kNORMAL,__FUNCTION__,Form("The plane of this point is %i",point.plane));
          
          // get vector of unassociated hits inside cone
          std::vector<larutil::PxHit> hit_candidates_in_plane = fgeoconic.PolyContain(unclustered_hits_in_plane,cone_2d);
          
          for (auto const& hit : hit_candidates_in_plane)
            print(msg::kNORMAL,__FUNCTION__,Form("The plane of this hit is %i",hit.plane));
          
          // trace back to get which unclustered hits are inside cone
          
          // loop over all the unclustered hits inside the cone in this plane
          for (auto const& pxhit : hit_candidates_in_plane) {
            
            // add to vector of all hits
            new_pxhits_in_cone.push_back(pxhit);
            
            // loop over the integer for each unclustered hit
            for (unsigned int unclustered_hit_id = 0; unclustered_hit_id < unclustered_hits.size(); unclustered_hit_id++) {
              
              // get this unclustered hit & check if it matches with current unclustered hit in cone
              larutil::PxHit test_hit = unclustered_hits[unclustered_hit_id];
              if (pxhit.w == test_hit.w && pxhit.t == test_hit.w && pxhit.charge == test_hit.charge && pxhit.plane == test_hit.plane) {
                
                // store the reference back to this hit's position in ev_hit
                hit_candidates.push_back(unclustered_hits_id[unclustered_hit_id]);
                
                // also note the reference back from unclustered hit -> shower
                hit_cluster_match[unclustered_hit_id].push_back(cluster_id);
                
                // then quit out
                break;
              }
            }
          }
          
          // save projection of cone
          for (auto const& pxhit : cone_2d)
            cone_projection.push_back(pxhit);
        }
        catch (const larutil::LArUtilException& e) {
          
          // if there's a segfault, flag this shower as bad & ignore it
          good_shower = false;
          print(msg::kWARNING,__FUNCTION__,Form("We had a segfault in 2D projection of cone. Skipping this shower."));
          continue;
        }
      }
      
      if (good_shower) {
        _hit_candidates.push_back(new_pxhits_in_cone);
        _associated_hits.push_back(associated_hits);
        _cone_projection.push_back(cone_projection);
      }
    }
    
    // create new hit data product (actually identical to existing)
    event_hit * new_ev_hit = storage->get_data<event_hit>(_output_producer);
    
    // copy new hit data product from old one
    for (unsigned int hit_id = 0; hit_id < ev_hit->size(); hit_id++)
      new_ev_hit->push_back(ev_hit->at(hit_id));
    
    // now create new associations from cluster to hit, including previously unclustered hits
    std::vector<std::vector<unsigned int>> new_cluster_hit_assoc_v;
    
    // first of all, just copy over all existing associations
    new_cluster_hit_assoc_v = cluster_hit_assoc;
    
    // loop over all unclustered hits
    for (unsigned int unclustered_hit_id = 0; unclustered_hit_id < unclustered_hits.size(); unclustered_hit_id++) {
      
      // if this unclustered hit was matched w/ no clusters, skip
      if (!hit_cluster_match[unclustered_hit_id].size())
        continue;
      
      // if this unclustered hit was matched w/ exactly one cluster, add it to that cluster!
      else if (hit_cluster_match[unclustered_hit_id].size() == 1) {
        
        // get the id of this hit & cluster, then add it to association
        unsigned int hit_id = unclustered_hits_id[unclustered_hit_id];
        unsigned int cluster_id = hit_cluster_match[hit_id][0];
        new_cluster_hit_assoc_v[cluster_id].push_back(hit_id);
      }
      
      // if this clustered hit was matched w/ more than one cluster, ignore it but keep track of its charge
      else {
        larutil::PxHit pxhit = unclustered_hits[unclustered_hit_id];
        _missing_charge[pxhit.plane] += pxhit.charge;
      }
    }
    
    // now save that new association to output file
    event_ass * new_cluster_hit_assoc = storage->get_data<event_ass>(_output_producer);
    new_cluster_hit_assoc->set_association(ev_cluster->id(), product_id(data::kHit, new_ev_hit->name()), new_cluster_hit_assoc_v);
    
    _tree->Fill();
    Clear();
    
    return true;
  }
  
  
  ////////////////////////////////////
  //                                //
  //            FINALIZE            //
  //                                //
  ////////////////////////////////////
  
  
  bool ShowerHitRestorer::finalize() {
    
    if (_fout)
      _tree->Write();
    
    return true;
  }
  
}
#endif
