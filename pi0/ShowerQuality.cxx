#ifndef LARLITE_SHOWERQUALITY_CXX
#define LARLITE_SHOWERQUALITY_CXX

#include "ShowerQuality.h"

namespace larlite {

  bool ShowerQuality::initialize() {

    //_tree = new TTree("ShowerQuality","ShowerQuality");
    
    return true;
  }
  
  bool ShowerQuality::analyze(storage_manager* storage) {
  
    // want to know shower quality, matching of showers
    auto showers_reco = storage->get_data<event_shower>("showerreco");
    auto showers_truth = storage->get_data<event_mcshower>("mcreco");
    
    // get simchannel info
    //auto simchannel = storage->get_data<event_simch>("largeant");
    
    //print(larlite::msg::kNORMAL,__FUNCTION__,Form("Hey here's some simchannel info! There are %i objects in SimChannel.",simchannel->size()));
    
    int n_reco = showers_reco->size();
    int n_truth = showers_truth->size();
    
    print(larlite::msg::kNORMAL,__FUNCTION__,Form("%i showers reconstructed out of %i real ones",n_reco,n_truth));
    
    // good signifiers of shower matching:
    //   1. start point in y
    //   2. start point in z
    //   3. direction
    //   4. energy
    
    // create a shower association object
    std::vector<int> shower_ass;
    
    // loop over all reconstructed showers
    
    for (auto reco_shower : * showers_reco) {
      
      // similarity between reco shower & all true showers
      double sigma_min = 99999;
      int best_shower = -1;
      
      double sigma_min_start_y = 99999;
      double sigma_min_start_z = 99999;
      double sigma_min_dir     = 99999;
      double sigma_min_energy  = 99999;
      
      for (int i = 0; i < showers_truth->size(); i++) {
        
        auto true_shower = showers_truth->at(i);
        
        // 1. start point in y
        double sigma_start_y = fabs(true_shower.Start().Y() - reco_shower.ShowerStart().Y());
        double sigma = sigma_start_y;
        
        // 2. start point in z
        double sigma_start_z = fabs(true_shower.Start().Z() - reco_shower.ShowerStart().Z());
        sigma += sigma_start_z;
        
        // 3. shower direction
        TVector3 true_dir(true_shower.Start().Px(),true_shower.Start().Py(),true_shower.Start().Pz());
        double sigma_dir = reco_shower.Direction().Angle(true_dir);
        //sigma += sigma_dir;
        
        // 4. shower energy
        double reco_energy = 0;
        for (auto plane_energy : reco_shower.Energy_v())
          reco_energy += plane_energy;
        double sigma_energy = fabs(reco_energy - true_shower.Start().E()) / true_shower.Start().E();
        //sigma += sigma_energy;
        
        // find best fit
        if (sigma < sigma_min) {
          sigma_min = sigma;
          best_shower = i;
          sigma_min_start_y = sigma_start_y;
          sigma_min_start_z = sigma_start_z;
          sigma_min_dir = sigma_dir;
          sigma_min_energy = reco_energy / true_shower.Start().E();
        }
      }
      
      //_tree->Fill();
      
      print(larlite::msg::kNORMAL,__FUNCTION__,Form("Best candidate is %i - Start point error in y %f, start point error in z %f, direction error %f, energy reco efficiency %f",best_shower,sigma_min_start_y,sigma_min_start_z,sigma_min_dir,sigma_min_energy));
      
      shower_ass.push_back(best_shower);
      
    }
  
    return true;
  }

  bool ShowerQuality::finalize() {

    //if (_fout)
    //  _tree->Write();
  
    return true;
  }

}
#endif
