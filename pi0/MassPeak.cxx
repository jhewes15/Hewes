#ifndef LARLITE_MASSPEAK_CXX
#define LARLITE_MASSPEAK_CXX

#include "MassPeak.h"

namespace larlite {

  bool MassPeak::initialize() {
    
    _tree = new TTree("MassPeak","MassPeak");
    
    _tree->Branch("mass","std::vector<double>",&mass);
    _tree->Branch("n_showers_1",&n_showers_1,"n_showers_1/D");
    _tree->Branch("n_showers_2",&n_showers_2,"n_showers_2/D");
    _tree->Branch("n_showers_3",&n_showers_3,"n_showers_3/D");

    return true;
  }
  
  bool MassPeak::analyze(storage_manager* storage) {
  
    // want to calculate pi0 mass peak
    
    auto showers = storage->get_data<event_shower>("showerreco");
    
    // get our two showers
    
    n_showers_1 = showers->size(); // no. of reconstructed showers
    
    print(larlite::msg::kINFO,__FUNCTION__,Form("Number of showerreco showers here is %lu",showers->size()));
    
    n_showers_2 = 0; // no. of showers w/ over 10MeV energy
    std::vector<shower> good_showers;
    
    for (auto shower : * showers) {
      double energy = 0;
      for (double plane_energy : shower.Energy_v())
        energy += plane_energy;
      if (energy > 10) {
        good_showers.push_back(shower);
        n_showers_2++;
      }
    }
    
    n_showers_3 = 0; // no. of good showers with a shared vertex
    
    for (int i = 0; i < n_showers_2; i++){
      for (int j = i + 1; j < n_showers_2; j++){
        TVector3 vtx_diff = good_showers.at(i).ShowerStart() - good_showers.at(j).ShowerStart();
        double vtx_cut = 10;
        if (fabs(vtx_diff.X()) < vtx_cut && fabs(vtx_diff.Y()) < vtx_cut && fabs(vtx_diff.Z()) < vtx_cut)
          n_showers_3++;
      }
    }
    
    for (int i = 0; i < n_showers_1; i++) {
      for (int j = i+1; j < n_showers_1; j++) {
        
        shower shower1 = showers->at(i);
        shower shower2 = showers->at(j);
        
        double energy1 = 0;
        for (double plane_energy : shower1.Energy_v())
          energy1 += plane_energy;
        
        double energy2 = 0;
        for (double plane_energy : shower2.Energy_v())
          energy2 += plane_energy;
        
        double angle = shower1.Direction().Angle(shower2.Direction());
        
        mass.push_back(sqrt(4 * energy1 * energy2 * pow(sin(angle/2.),2)));
      }
    }
    
    _tree->Fill();
    
    return true;
    
  }

  bool MassPeak::finalize() {
    
    if (_fout)
      _tree->Write();
  
    return true;
  }

}
#endif
