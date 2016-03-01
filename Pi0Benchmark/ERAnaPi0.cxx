#ifndef ERTOOL_ERANAPI0_CXX
#define ERTOOL_ERANAPI0_CXX

#include "ERAnaPi0.h"

namespace ertool {

  ERAnaPi0::ERAnaPi0(const std::string& name) : AnaBase(name)
  {}

  void ERAnaPi0::Reset()
  {}

  void ERAnaPi0::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaPi0::ProcessBegin()
  {
    // set up an output tree
    delete _Pi0Ana_tree;
    _Pi0Ana_tree = new TTree("_Pi0Ana_tree", "Pi0 ana tree");
    _Pi0Ana_tree->Branch("_n_shwr_mc", &_n_shwr_mc, "_n_shwr_mc/I");
    _Pi0Ana_tree->Branch("_n_shwr_pr", &_n_shwr_mc, "_n_shwr_pr/I");
    _Pi0Ana_tree->Branch("_n_pi0_mc" , &_n_pi0_mc , "_n_pi0_mc/I" );
    _Pi0Ana_tree->Branch("_n_pi0_pr" , &_n_pi0_pr , "_n_pi0_pr/I" );
    _Pi0Ana_tree->Branch("_energy_res", &_energy_res, "_energy_res/D");
    _Pi0Ana_tree->Branch("_angle_res", &_angle_res, "_angle_res/D");
  }

  bool ERAnaPi0::Analyze(const EventData &data, const ParticleGraph &ps)
  {
    try {
    
      // get us an mctruth particlegraph
      auto ps_mc = MCParticleGraph();
    
      // get showers & pi0s
      auto shwr_mc = ps_mc.GetParticleNodes(kShower);
      auto shwr_pr = ps.GetParticleNodes(kShower);
      auto pi0_mc  = ps_mc.GetParticleNodes(kInvisible, false, 111);
      auto pi0_pr  = ps.GetParticleNodes(kInvisible, false, 111);
    
      // count number of each
      _n_shwr_mc = shwr_mc.size();
      _n_shwr_pr = shwr_pr.size();
      _n_pi0_mc  = pi0_mc.size();
      _n_pi0_pr  = pi0_pr.size();
    
      // did we reconstruct a pi0 in this event?
      if (_n_pi0_pr > 0) {
      
        // so... we know for a fact that there's only one pi0 at truth level
        // because this code (right now) is being run on a single-pi0 sample
      
        // but it's best to check
      
        if (_n_pi0_mc != 1 || _n_pi0_pr != 1) {
          throw ertool::ERException(Form("expecting 1 pion in both mctruth & reco, but there are %i in mctruth and %i in reco", _n_pi0_mc, _n_pi0_pr));
        }
        
        // first energy resolution
        _energy_res = ps_mc.GetParticle(shwr_mc[0]).Energy() / ps.GetParticle(shwr_pr[0]).Energy();
        
        // now angular resolution
        _angle_res = ps_mc.GetParticle(shwr_mc[0]).Momentum().Angle(ps.GetParticle(shwr_pr[0]).Momentum());
        
      }
      // if there's no pi0, make sure the energy / angle resolution is set to zero
      else {
        _energy_res = 0;
        _angle_res = 0;
      }
      
      // fill up the tree!
      _Pi0Ana_tree->Fill();
    }
    catch (ertool::ERException & e) {
      std::cout << Form("expecting 1 pion in both mctruth & reco, but there are %i in mctruth and %i in reco", _n_pi0_mc, _n_pi0_pr) << std::endl;
      _energy_res = 0;
      _angle_res = 0;
    }
    
    
    return true;
  }

  void ERAnaPi0::ProcessEnd(TFile* fout)
  {
    if (fout)
      _Pi0Ana_tree->Write();
  }

}

#endif
