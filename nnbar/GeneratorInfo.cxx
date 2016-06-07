#ifndef LARLITE_GENERATORINFO_CXX
#define LARLITE_GENERATORINFO_CXX

#include "GeneratorInfo.h"

namespace larlite {

  bool GeneratorInfo::initialize() {
    
    _tree = new TTree("genie_tree","");
    
    _tree->Branch("mult_n",&mult_n,"mult_n/I");
    _tree->Branch("mult_p",&mult_p,"mult_p/I");
    _tree->Branch("mult_pip",&mult_pip,"mult_pip/I");
    _tree->Branch("mult_pim",&mult_pim,"mult_pim/I");
    _tree->Branch("mult_pi0",&mult_pi0,"mult_pi0/I");
    
    _tree->Branch("mom_n","std::vector<double>",&mom_n);
    _tree->Branch("mom_p","std::vector<double>",&mom_p);
    _tree->Branch("mom_pip","std::vector<double>",&mom_pip);
    _tree->Branch("mom_pim","std::vector<double>",&mom_pim);
    _tree->Branch("mom_pi0","std::vector<double>",&mom_pi0);
    
    _tree->Branch("tot_mom_w_nuc",&tot_mom_w_nuc,"tot_mom_w_nuc/D");
    _tree->Branch("tot_mom_wo_nuc",&tot_mom_wo_nuc,"tot_mom_wo_nuc/D");
    _tree->Branch("tot_im_w_nuc",&tot_im_w_nuc,"tot_im_w_nuc/D");
    _tree->Branch("tot_im_wo_nuc",&tot_im_wo_nuc,"tot_im_wo_nuc/D");

    return true;
  }
  
  bool GeneratorInfo::analyze(storage_manager* storage) {
    
    tot_mom_w_nuc = 0;
    tot_mom_wo_nuc = 0;
    tot_im_w_nuc = 0;
    tot_im_wo_nuc = 0;
  
    // get mctruth info
    mctruth truth = (*(storage->get_data<event_mctruth>("generator")))[0];
    std::vector<mcpart> particles(truth.GetParticles());
    
    // mctruth information
    mult_n = 0;
    mult_p = 0;
    mult_pip = 0;
    mult_pim = 0;
    mult_pi0 = 0;
    mult_om = 0;
    
    TLorentzVector w_nuc, wo_nuc;
                                  
    for (mcpart part : particles) {
      TLorentzVector fourvec = part.Trajectory().at(0).Momentum();
      double momentum = fourvec.P();
      switch (part.PdgCode()) {
        case 2112:
          mult_n++;
          mom_n.push_back(momentum);
          wo_nuc += fourvec;
          break;
        case 2212:
          mult_p++;
          mom_p.push_back(momentum);
          wo_nuc += fourvec;
          break;
        case 211:
          mult_pip++;
          mom_pip.push_back(momentum);
          w_nuc += fourvec;
          wo_nuc += fourvec;
          break;
        case -211:
          mult_pim++;
          mom_pim.push_back(momentum);
          w_nuc += fourvec;
          wo_nuc += fourvec;
          break;
        case 111:
          mult_pi0++;
          mom_pi0.push_back(momentum);
          w_nuc += fourvec;
          wo_nuc += fourvec;
          break;
        case 332:
          mult_om++;
          w_nuc += fourvec;
          wo_nuc += fourvec;
          break;
      }
    }
    
    // net momentum
    tot_mom_w_nuc = w_nuc.Vect().Mag();
    tot_mom_wo_nuc = wo_nuc.Vect().Mag();
    
    // invariant mass
    tot_im_w_nuc = w_nuc.Mag();
    tot_im_wo_nuc = wo_nuc.Mag();
    
    _tree->Fill();
    
    mom_n.clear();
    mom_p.clear();
    mom_pip.clear();
    mom_pim.clear();
    mom_pi0.clear();
        
    return true;
  }

  bool GeneratorInfo::finalize() {

    if(_fout)
      _tree->Write();
  
    return true;
  }

}
#endif
