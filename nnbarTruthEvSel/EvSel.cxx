#ifndef LARLITE_EVSEL_CXX
#define LARLITE_EVSEL_CXX

#include "EvSel.h"

namespace larlite {

  bool EvSel::initialize() {
    
    _h = new TH2D("_h", "", 10, 0, 10, 10, 0, 10);
    
    return true;
  }
  
  bool EvSel::analyze(storage_manager* storage) {
  
    // hey what's up, im in a chatty mood so have fun wading through all these comments \o/
    
    // right now im working in an extremely chill 24hr coffee shop in austin tx
    // if u want to recreate the vibe, im listening to the vladimir horowitz recording of
    // schumann's kinderszenen (https://www.youtube.com/watch?v=yibf6QNjgGU) and drinking
    // a maine root mexicane cola
    
    // ok so here's the deal
    
    // im gonna just run an event selection using only truth-level information on both our
    // signal mc (from the genie nnbar event generator) and the hacked-together atmospherics
    // sample we have - it's using a SNO atmospheric flux for energies < 10GeV (because SNO
    // is the closest geographical approximation to DUNE), and a Kamiokande flux file for
    // energies > 10GeV - it's a poorer approximation, but there are no SNO fluxes for that
    // energy range.
    
    // for each event, we're gonna look at:
    //   - total net momentum
    //   - total visible energy
    //   - total invariant mass
    
    // i'll let cuts on these variables float and optimise a figure of merit: eff / sqrt(bkg)
    // this will let us benchmark a maximum efficiency & background
    
    // if we do this for truth info, we know we can't do better than this
    // (and moving to reconstruction means we'll definitely do worse)
    
    // anyway i should probably put some god damn code here at some point
    
    
    // creating holders for eventwise information
    TVector3 evtMomentum(0,0,0);
    double   evtEnergy = 0;
    
    // get this event's mctruth information
    mctruth truth = (*(storage->get_data<event_mctruth>("generator")))[0];
    std::vector<mcpart> particles(truth.GetParticles());
    
    // loop over each particle
    for (mcpart part : particles) {
      
      if (part.StatusCode() != 1) continue;
      
      //if ( ! (abs(part.PdgCode()) == 211 || part.PdgCode() == 111 || part.PdgCode() == 22 || abs(part.PdgCode()) == 2212 || abs(part.PdgCode()) == 2112))
        //std::cout << "PDG code not recognised: " << part.PdgCode() << std::endl;
      
      // get momentum & create a vector
      double px = part.Trajectory().back().Px();
      double py = part.Trajectory().back().Py();
      double pz = part.Trajectory().back().Pz();
      TVector3 partMomentum(px,py,pz);
      
      // get energy
      double partEnergy = part.Trajectory().back().E();
      
      // add the energy & momentum to the grand total
      evtMomentum += partMomentum;
      evtEnergy   += partEnergy;
    }

    // calculate event invariant mass
    double evtInvMass = sqrt(pow(evtEnergy,2) - pow(evtMomentum.Mag(),2));
    
    // add event information to vectors
    _energy.push_back(evtEnergy);
    _momentum.push_back(evtMomentum.Mag());
    _invMass.push_back(evtInvMass);
    
    std::cout << "Event summary: momentum is " << evtMomentum.Mag() << " GeV, energy is " << evtEnergy << " GeV" << std::endl;
    
    return true;
  }

  bool EvSel::finalize() {
    
    TTree _t("_t", "energy/momentum tree");
    _t.Branch("_energy",   "std::vector<double>", &_energy  );
    _t.Branch("_momentum", "std::vector<double>", &_momentum);
    _t.Branch("_invMass",  "std::vector<double>", &_invMass );
    
    _t.Fill();

    if(_fout) {
      _fout->cd();
      _t.Write();
    }
  
    return true;
  }

}
#endif
