#ifndef LARLITE_DATACOMPARISON_CXX
#define LARLITE_DATACOMPARISON_CXX

#include "DataComparison.h"

namespace larlite {

  bool DataComparison::initialize() {

    _tree = new TTree("DataComparison.root","");
    _tree->Branch("dipion_mass_10",&dipion_mass_10,"dipion_mass_10/D");

    return true;
  }
  
  bool DataComparison::analyze(storage_manager* storage) {
  
    // get mctruth info
    mctruth truth = (*(storage->get_data<event_mctruth>("generator")))[0];
    std::vector<mcpart> particles(truth.GetParticles());

    n_piplus  = 0;
    n_piminus = 0;
    n_pizero  = 0;
    n_omega   = 0;

    std::vector<TLorentzVector> fourvec_piplus;
    std::vector<TLorentzVector> fourvec_piminus;
    std::vector<TLorentzVector> fourvec_pizero;
    std::vector<TLorentzVector> fourvec_omega;
  
    for (auto part : particles) {
      TLorentzVector fourvec = part.Trajectory().at(0).Momentum();
      switch (part.PdgCode()) {
        case 211:
          fourvec_piplus.push_back(fourvec);
          n_piplus++;
          break;
        case -211:
          fourvec_piminus.push_back(fourvec);
          n_piminus++;
          break;
        case 111:
          fourvec_pizero.push_back(fourvec);
          n_pizero++;
          break;
        case 322:
          fourvec_omega.push_back(fourvec);
          n_omega++;
          break;
      }
    }

    switch (topology()) {
      case 10:
        TLorentzVector combination;
        combination = fourvec_piplus[0] + fourvec_piminus[0];
        dipion_mass_10 = combination.Mag();
        _tree->Fill();
        combination = fourvec_piplus[0] + fourvec_pizero[0];
        dipion_mass_10 = combination.Mag();
        _tree->Fill();
        combination = fourvec_piminus[0] + fourvec_pizero[0];
        dipion_mass_10 = combination.Mag();
        _tree->Fill();
    }

    return true;
  }

  bool DataComparison::finalize() {

    if (_fout)
      _tree->Write();
  
    return true;
  }

}
#endif
