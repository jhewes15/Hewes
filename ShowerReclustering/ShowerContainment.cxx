#ifndef LARLITE_SHOWERCONTAINMENT_CXX
#define LARLITE_SHOWERCONTAINMENT_CXX

#include "ShowerContainment.h"

namespace larlite {
  
  
  ////////////////////////////////////
  //                                //
  //           INITIALIZE           //
  //                                //
  ////////////////////////////////////
  
  
  bool ShowerContainment::initialize() {
    
    // set up output ana tree
    _tree = new TTree("clusterquality","");
    _tree->Branch("_pxhits","std::vector<std::vector<std::vector<larutil::PxHit>>>",&_pxhits);
    _tree->Branch("_pxhits_in_cone","std::vector<std::vector<std::vector<larutil::PxHit>>>",&_pxhits_in_cone);
    _tree->Branch("_pxhits_in_new_cone","std::vector<std::vector<std::vector<larutil::PxHit>>>",&_pxhits_in_new_cone);
    _tree->Branch("_all_pxhits","std::vector<std::vector<std::vector<larutil::PxHit>>>",&_all_pxhits);
    _tree->Branch("_all_pxhits_in_cone","std::vector<std::vector<std::vector<larutil::PxHit>>>",&_all_pxhits_in_cone);
    _tree->Branch("_sp_in_cone","std::vector<std::vector<spacepoint>>",&_sp_in_cone);
    _tree->Branch("_sp_not_in_cone","std::vector<std::vector<spacepoint>>",&_sp_not_in_cone);
    _tree->Branch("_sp_in_cone_2d","std::vector<std::vector<std::vector<larutil::PxPoint>>>",&_sp_in_cone_2d);
    _tree->Branch("_sp_not_in_cone_2d","std::vector<std::vector<std::vector<larutil::PxPoint>>>",&_sp_not_in_cone_2d);
    _tree->Branch("_cone_projection","std::vector<std::vector<std::vector<larutil::PxPoint>>>",&_cone_projection);
    _tree->Branch("_new_cone_projection","std::vector<std::vector<std::vector<larutil::PxPoint>>>",&_new_cone_projection);
    _tree->Branch("_new_cone","std::vector<bool>",&_new_cone);
    
    return true;
  }
  
  
  ////////////////////////////////////
  //                                //
  //            ANALYZE             //
  //                                //
  ////////////////////////////////////
  
  
  bool ShowerContainment::analyze(storage_manager* storage) {
    
    // get showerreco showers
    event_shower * ev_shower = storage->get_data<event_shower>("showerreco");
    
    // create new output pfparts
    if (_output_producer.empty())
      _output_producer = "ConeRecluster";
    //event_pfpart * ev_new_pfpart = storage->get_data<event_pfpart>(_output_producer);
    //event_cluster * ev_new_cluster = storage->get_data<event_cluster>(_output_producer);
    
    // check the showers are good
    if (!ev_shower) {
      print(msg::kWARNING,__FUNCTION__,Form("No showerreco shower data object found in this event."));
      return false;
    }
    else if (ev_shower->size() < 1) {
      print(msg::kWARNING,__FUNCTION__,Form("Showerreco shower data object in this event is empty."));
      return false;
    }
    
    // get clusters and cluster associations
    event_cluster * ev_cluster = nullptr;
    auto cluster_ass = storage->find_one_ass(ev_shower->id(), ev_cluster, ev_shower->name());
    
    if(!ev_cluster)
      print(msg::kWARNING,__FUNCTION__,Form("No cluster data object found associated with these showers."));
    else if (ev_cluster->size() < 1)
      print(msg::kWARNING,__FUNCTION__,Form("Cluster data object associated with these showers is empty."));
    
    // get hits and hit associations
    event_hit * ev_hit = nullptr;
    auto hit_ass = storage->find_one_ass(ev_cluster->id(), ev_hit, ev_cluster->name());
    
    if (!ev_hit)
      print(msg::kWARNING,__FUNCTION__,Form("No hit data object found associated with these clusters."));
    else if (ev_hit->size() < 1)
      print(msg::kWARNING,__FUNCTION__,Form("Hit data object associated with these clusters is empty."));
    
    // get association from hit to cluster
    event_cluster * throwaway = nullptr;
    auto hit_cluster_ass = storage->find_one_ass(ev_hit->id(), throwaway, ev_hit->name());
    
    // get pfparts and associations from shower to pfpart
    event_pfpart * ev_pfpart = nullptr;
    auto shower_pfpart_assoc = storage->find_one_ass(ev_shower->id(), ev_pfpart, ev_shower->name());
    
    if (!ev_pfpart)
      print(msg::kWARNING,__FUNCTION__,Form("No pfpart data object found in this event."));
    else if (ev_pfpart->size() < 1)
      print(msg::kWARNING,__FUNCTION__,Form("pfpart data object in this event is empty."));
    
    // get spacepoints and associations from pfpart to spacepoint
    event_spacepoint * ev_sps = nullptr;
    auto pfpart_sps_assoc = storage->find_one_ass(ev_pfpart->id(), ev_sps, ev_pfpart->name());
    
    if (!ev_sps)
      print(msg::kWARNING,__FUNCTION__,Form("No spacepoint data object found in this event."));
    else if (ev_sps->size() < 1)
      print(msg::kWARNING,__FUNCTION__,Form("Spacepoint data object in this event is empty."));
    
    // loop over each shower in the event
    for (int shower_id = 0; shower_id < ev_shower->size(); shower_id++) {
      
      // check there's a 1-to-1 correlation between shower & pfpart
      if (shower_pfpart_assoc[shower_id].size() != 1) {
        print(msg::kWARNING,__FUNCTION__,Form("More than one PFPart associated with this shower. This should not be the case."));
        return false;
      }
      
      // get id of pfpart associated with this shower
      unsigned int pfpart_id = shower_pfpart_assoc[shower_id][0];
      
      std::vector<spacepoint> sp_in_cone;
      std::vector<spacepoint> sp_not_in_cone;
      
      std::vector<std::vector<larutil::PxPoint>> sp_in_cone_2d;
      sp_in_cone_2d.resize(_n_planes);
      std::vector<std::vector<larutil::PxPoint>> sp_not_in_cone_2d;
      sp_not_in_cone_2d.resize(_n_planes);
      
      // get spacepoints associated with this pfpart, and project them down to 2d
      for (auto const& sps_id : pfpart_sps_assoc[pfpart_id]) {
        if(InsideCone(ev_shower->at(shower_id),ev_sps->at(sps_id))) {
          sp_in_cone.push_back(ev_sps->at(sps_id));
          for (int plane = 0; plane < _n_planes; plane++)
            sp_in_cone_2d[plane].push_back(MakePxPoint(ev_sps->at(sps_id),plane));
        }
        else {
          sp_not_in_cone.push_back(ev_sps->at(sps_id));
          for (int plane = 0; plane < _n_planes; plane++)
            sp_not_in_cone_2d[plane].push_back(MakePxPoint(ev_sps->at(sps_id),plane));
        }
      }
      
      _sp_in_cone.push_back(sp_in_cone);
      _sp_not_in_cone.push_back(sp_not_in_cone);
      _sp_in_cone_2d.push_back(sp_in_cone_2d);
      _sp_not_in_cone_2d.push_back(sp_not_in_cone_2d);
    }
    
    // create a pxhit object for every hit
    std::vector<std::vector<larutil::PxHit>> all_pxhits;
    all_pxhits.resize(_n_planes);
    
    for (auto const& hit : * ev_hit)
      all_pxhits[hit.View()].push_back(MakePxHit(hit));
    
    // loop over all showers
    for (unsigned int shower_id = 0; shower_id < ev_shower->size(); shower_id++) {
      
      bool good_shower = true;
      
      auto const& shower = ev_shower->at(shower_id);
      
      unsigned int n_clusters = cluster_ass[shower_id].size();
      
      std::vector<std::vector<larutil::PxHit>> pxhits;
      pxhits.resize(_n_planes);
      std::vector<std::vector<larutil::PxHit>> pxhits_in_cone;
      pxhits_in_cone.resize(_n_planes);
      std::vector<std::vector<larutil::PxHit>> all_pxhits_in_cone;
      all_pxhits_in_cone.resize(_n_planes);
      std::vector<std::vector<larutil::PxPoint>> cone_projection;
      cone_projection.resize(_n_planes);
      
      if (n_clusters > _n_planes) {
        print(msg::kERROR,__FUNCTION__,Form("Error! More matched clusters than planes for this shower!"));
        return false;
      }
      
      // loop over associated clusters and create pxhit objects
      for (unsigned int cluster_id : cluster_ass[shower_id]) {
        
        // loop over all hits in this cluster and create pxhit objects
        for (unsigned int hit_id : hit_ass[cluster_id])
          pxhits[ev_hit->at(hit_id).View()].push_back(MakePxHit(ev_hit->at(hit_id)));
      }
      
      // loop over each cluster
      for (unsigned int cluster_id : cluster_ass[shower_id]) {
        
        int view = ev_cluster->at(cluster_id).View();
        
        // try catch statement to avoid a segfault
        try {
          // get 2d polygon representation of the 3d shower cone
          std::vector<larutil::PxPoint> cone_2d = fgeoconic.ConicalFeatures(shower.ShowerStart(),shower.Direction(),shower.Length(),shower.OpeningAngle()*(180/M_PI),view,_smoothness);
          
          // get vector of associated hits inside the cone
          pxhits_in_cone[view] = fgeoconic.PolyContain(pxhits[view],cone_2d);
          
          // get vector of all hits inside the cone
          all_pxhits_in_cone[view] = fgeoconic.PolyContain(all_pxhits[view],cone_2d);
          
          // save projection of cone
          cone_projection[view] = cone_2d;
        }
        catch (const larutil::LArUtilException& e) {
          good_shower = false;
          print(msg::kWARNING,__FUNCTION__,Form("We had a segfault in the 2D projection of the cone."));
          continue;
        }
      }
      
      // calculate total hit containment for this cone, weighted in each plane
      double original_containment = CalculateContainment(pxhits_in_cone,pxhits);
      double best_containment = original_containment;
      bool found_better_containment = false;
      
      TVector3 new_direction;
      
      std::vector<std::vector<larutil::PxPoint>> best_cone_projection = cone_projection;
      std::vector<std::vector<larutil::PxHit>> pxhits_in_best_cone = pxhits_in_cone;
      
      for (unsigned int i = 0; i < 72; i++) {
        
        // get a new vector of hits in cone
        std::vector<std::vector<larutil::PxHit>> pxhits_in_cone_new;
        pxhits_in_cone_new.resize(_n_planes);
        std::vector<std::vector<larutil::PxPoint>> new_cone_projection;
        new_cone_projection.resize(_n_planes);
        
        // try a new shower direction
        if (!i) {
          new_direction = shower.Direction();
          new_direction.RotateZ(M_PI/36.);
        }
        else
          new_direction.Rotate(M_PI/36.,shower.Direction());
        
        // loop over each cluster
        for (unsigned int cluster_id : cluster_ass[shower_id]) {
          
          int view = ev_cluster->at(cluster_id).View();
          
          // try catch statement to avoid a segfault
          try {
            // get 2d polygon representation of the 3d shower cone
            std::vector<larutil::PxPoint> cone_2d = fgeoconic.ConicalFeatures(shower.ShowerStart(),new_direction,shower.Length(),shower.OpeningAngle()*(180/M_PI),view,_smoothness);
            
            // get vector of associated hits inside the cone
            pxhits_in_cone_new[view] = fgeoconic.PolyContain(pxhits[view],cone_2d);
            
            // save projection of cone
            new_cone_projection[view] = cone_2d;
          }
          catch (const larutil::LArUtilException& e) {
            good_shower = false;
            print(msg::kWARNING,__FUNCTION__,Form("We had a segfault in the 2D projection of the cone."));
            continue;
          }
          
          double containment = CalculateContainment(pxhits_in_cone_new,pxhits);
          if (containment > best_containment) {
            found_better_containment = true;
            best_containment = containment;
            best_cone_projection = new_cone_projection;
            pxhits_in_best_cone = pxhits_in_cone_new;
          }
        }
      }
      
      if (found_better_containment)
        print(msg::kNORMAL,__FUNCTION__,Form("Found better shower candidate! Old containment of %f, new containment of %f.",original_containment,best_containment));
      else
        print(msg::kNORMAL,__FUNCTION__,Form("Could not improve on containment of %f.",original_containment));
      print(msg::kNORMAL,__FUNCTION__,Form(""));
    
      // if there were no segfaults, save this information to the event vectors
      if (good_shower) {
        _pxhits.push_back(pxhits);
        _pxhits_in_cone.push_back(pxhits_in_cone);
        _pxhits_in_new_cone.push_back(pxhits_in_best_cone);
        _all_pxhits.push_back(all_pxhits);
        _all_pxhits_in_cone.push_back(all_pxhits_in_cone);
        _cone_projection.push_back(cone_projection);
        _new_cone_projection.push_back(best_cone_projection);
        _new_cone.push_back(found_better_containment);
      }
    }
    
    _tree->Fill();
    Clear();
    
    return true;
  }
  
  
  // you know the deal by now
  ///////////////////////////////////////////////////////////////////////////
  //                                                                       //
  //                                 EMBERS                                //
  //                                                                       //
  //                  tomorrow is here | dvs                               //
  //                 hornets! hornets! | the hold steady                   //
  // admitting the endorphin addiction | open mike eagle                   //
  //                      tears of joy | slow club                         //
  //         you can't fire me, i quit | tacocat                           //
  //        i like that you can see it | girlpool                          //
  //                       fall asleep | rodrigo amarante                  //
  //                      clay pigeons | michael cera                      //
  //                   sincerely yours | llll                              //
  //             myself when i am real | charles mingus                    //
  //                       new friends | pinegrove                         //
  //             song for dennis brown | the mountain goats                //
  //                                                                       //
  ///////////////////////////////////////////////////////////////////////////
  
  
  
  ////////////////////////////////////
  //                                //
  //            FINALIZE            //
  //                                //
  ////////////////////////////////////
  
  
  bool ShowerContainment::finalize() {
    
    if (_fout)
      _tree->Write();
    
    return true;
  }
  
}
#endif
