#ifndef ERTOOL_ERANAMASSPEAK_CXX
#define ERTOOL_ERANAMASSPEAK_CXX

#include "ERAnaMassPeak.h"

namespace ertool {

  ERAnaMassPeak::ERAnaMassPeak(const std::string& name) : AnaBase(name)
  {}

  void ERAnaMassPeak::Reset()
  {}

  void ERAnaMassPeak::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaMassPeak::ProcessBegin()
  {
    // set up output tree
    MassPeak = new TTree("MassPeak","");
    
    MassPeak->Branch("n_showers_1",&n_showers_1,"n_showers_1/I");
    MassPeak->Branch("n_showers_2",&n_showers_2,"n_showers_2/I");
    MassPeak->Branch("n_showers_3",&n_showers_3,"n_showers_3/I");
    
    MassPeak->Branch("mass_inclusive","std::vector<double>",&mass_inclusive);
    MassPeak->Branch("mass_exclusive","std::vector<double>",&mass_exclusive);
    
    MassPeak->Branch("start_point_distance","std::vector<double>",&start_point_distance);
  }

  bool ERAnaMassPeak::Analyze(const EventData &data, const ParticleGraph &ps)
  {
    
    // get all showers from particle graph
    auto shower_nodes = ps.GetParticleNodes(kShower);
    
    // count number of showers in event
    n_showers_1 = shower_nodes.size();
    
    // calculate invariant mass for EVERY pair of showers ("inclusive")
    // and calculate distance between shower start points
    for (int i = 0; i < n_showers_1; i++) {
      for (int j = i + 1; j < n_showers_1; j++) {
        auto shower1 = data.Shower(shower_nodes.at(i));
        auto shower2 = data.Shower(shower_nodes.at(j));
        mass_inclusive.push_back(CalculateMass(shower1,shower2));
        start_point_distance.push_back(shower1.Start().Dist(shower2.Start()));
      }
    }
    
    // add showers with >10MeV energy to "good_showers"
    std::vector<Shower> good_showers;
    
    for (int shower_node : shower_nodes) {
      auto reco_id = ps.GetParticle(shower_node).RecoID();
      if (data.Shower(reco_id)._energy > energy_cut)
        good_showers.push_back(data.Shower(reco_id));
    }
    
    // count number of "good" showers
    n_showers_2 = good_showers.size();
    
    // count number of shower pairs w/ matching vertices
    // & calculate invariant mass ("exclusive")
    n_showers_3 = 0;
    
    for (int i = 0; i < n_showers_2; i++) {
      for (int j = i + 1; j < n_showers_2; j++) {
        
        auto shower1 = good_showers.at(i);
        auto shower2 = good_showers.at(j);
        
        double angle = shower1.Dir().Angle(shower2.Dir());
        double dist = shower1.Start().Dist(shower2.Start());
        
        // make our cuts
        if (angle < angle_cut) continue; // minimum angle
        else if (shower1._energy + shower2._energy < energy_sum_cut) continue;
        else if (dist > vtx_cut) continue;
        
        if (dist < vtx_cut) {
          n_showers_3++;
          mass_exclusive.push_back(CalculateMass(shower1,shower2));
        }
      }
    }
    
    // fill tree
    MassPeak->Fill();
    
    // clear vectors
    mass_inclusive.clear();
    mass_exclusive.clear();
    start_point_distance.clear();
    
    return true;
  }

  void ERAnaMassPeak::ProcessEnd(TFile* fout)
  {
    // write output tree to file
    if (fout)
      MassPeak->Write();
  }
  
  // function to calculate invariant mass of shower pair
  double ERAnaMassPeak::CalculateMass(Shower shower1, Shower shower2)
  {
    double energy1 = shower1._energy;
    double energy2 = shower2._energy;
    double angle   = shower1.Dir().Angle(shower2.Dir());
    
    double mass = sqrt(2 * energy1 * energy2 * (1 - cos(angle)));
    return mass;
  }

}

#endif
