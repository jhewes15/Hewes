#ifndef ERTOOL_ERANAFLASHEFFICIENCY_CXX
#define ERTOOL_ERANAFLASHEFFICIENCY_CXX

#include "ERAnaFlashEfficiency.h"

namespace ertool {

  ERAnaFlashEfficiency::ERAnaFlashEfficiency(const std::string& name) : AnaBase(name)
  {}

  void ERAnaFlashEfficiency::Reset()
  {}

  void ERAnaFlashEfficiency::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaFlashEfficiency::ProcessBegin()
  {}

  bool ERAnaFlashEfficiency::Analyze(const EventData &data, const ParticleGraph &ps)
  {
    // get flash information
    auto flashes = data.Flash();
    std::cout << "Number of flashes in this event: " << flashes.size() << std::endl;
    
    // loop over all shower particles
    auto shower_nodes = ps.GetParticleNodes(kShower);
    std::cout << "Number of showers in this event: " << shower_nodes.size() << std::endl;
    
    // print out flash id for each shower
    for (auto node : shower_nodes) {
      auto shower = ps.GetParticle(node);
      auto shower_flash = shower.FlashID();
      std::cout << "The flash ID associated with this shower is " << shower_flash << std::endl;

      // also check shower energy
      auto reco_id = shower.RecoID();
      auto original_shower = data.Shower(reco_id);
      std::cout << "As a quick check, the shower energy is " << original_shower._energy << std::endl;
    }
   
    
    return true;
  }

  void ERAnaFlashEfficiency::ProcessEnd(TFile* fout)
  {}

}

#endif
