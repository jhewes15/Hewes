#ifndef LARLITE_PARAMRECO_CXX
#define LARLITE_PARAMRECO_CXX

#include "ParamReco.h"

namespace larlite {

  bool ParamReco::initialize() {
    
    rand_var = new TRandom3(17);
    _h_nnbar = new TH2D("_h_nnbar", "", 20, 0, 4, 10, 0, 2);
    
    n_evts = 0;
    
    return true;
  }
  
  bool ParamReco::analyze(storage_manager* storage) {
    
    auto ev_truth = storage->get_data<event_mctruth>("generator");
    
    // eventwise information - momentum & energy
    TVector3 momentum(0,0,0);
    double energy(0);
    
    TVector3 true_momentum(0,0,0);
    double true_energy(0);
    
    // loop over all particles in event
    
    for (mctruth a : * ev_truth) {
      std::vector<mcpart> particles(a.GetParticles());
      for (mcpart part : particles) {
        if (part.StatusCode() == 1) {
          
          double px = part.Trajectory().back().Px();
          double py = part.Trajectory().back().Py();
          double pz = part.Trajectory().back().Pz();
          
          // do parameterised reco for each particle
          
          double width, q_width, e_thresh;

          // first let's deal with charged pions          
          if (TMath::Abs(part.PdgCode()) == 211)  { width = pipm_Ep_highwidth;  q_width = pipm_q_width;  e_thresh = pipm_E_thresh;  }
          // now look at neutral pions
          else if        (part.PdgCode() == 111)  { width = pi0_Ep_highwidth;   q_width = pi0_q_width;   e_thresh = pi0_E_thresh;   }
          // protons!
          else if        (part.PdgCode() == 2112) { width = p_Ep_highwidth;     q_width = p_q_width;     e_thresh = p_E_thresh;     }
          // neutrons!
          else if        (part.PdgCode() == 2212) { width = n_Ep_highwidth;     q_width = n_q_width;     e_thresh = n_E_thresh;     }
          // need to deal with gammas now
          else if        (part.PdgCode() == 22)   { width = gamma_Ep_highwidth; q_width = gamma_q_width; e_thresh = gamma_E_thresh; }
          // electrons & positrons!
          else if (TMath::Abs(part.PdgCode()) == 11) continue;
          // muons & posimuons (because antimuons need a better name)
          else if (TMath::Abs(part.PdgCode()) == 13) continue;
          // ignore all the neutrinos lol
          else if (TMath::Abs(part.PdgCode()) == 12 || TMath::Abs(part.PdgCode()) == 14 || TMath::Abs(part.PdgCode()) == 16) continue;
          // deuterons can go away
          else if (part.PdgCode() == 2000000101) continue;
          else {
            std::cerr << "An unexpected particle showed up, with PDG code " << part.PdgCode() << ". Exiting..." << std::endl;
            return false;
          }
          
          // smear out the momentum angle
          TVector3 old_direction(px, py, pz);
          double p = old_direction.Mag();
          TVector3 new_direction = SmearAngle(old_direction, q_width * TMath::DegToRad());
          
          // check the width
          width = width/100.0;
          if(width < 0.01)width = 0.01;
          if(width > 0.50)width = 0.50;
          
          // now smear out the momentum!
          double p_smeared = -1;
          while(p_smeared < 0.0)
            p_smeared = rand_var->Gaus(p, p * width);
          
          //use newdir unit vector to form p_reco into components.
          TVector3 p_reco = p_smeared * new_direction;
          double mass = GetMass(part.PdgCode());
          double e_reco = sqrt(pow(p_reco.Mag(),2) + pow(mass,2));
          bool above_threshold = (e_reco - mass > e_thresh) ? true : false;
          
          TVector3 temp_momentum(px,py,pz);
          true_momentum += temp_momentum;
          true_energy += part.Trajectory().back().E();
          
          if (part.PdgCode() == 2112) above_threshold = false;
          
          if (above_threshold) {
            momentum += p_reco;
            if (part.PdgCode() != 2212)
              energy   += e_reco;
          }
        }
      }
    }
    
    double invariant_mass = sqrt(pow(energy,2) - pow(momentum.Mag(),2));
    
    ev_momentum.push_back(momentum.Mag());
    ev_invariant_mass.push_back(invariant_mass);
    n_evts++;
    
    _h_nnbar->Fill(invariant_mass, momentum.Mag());
    
    //std::cout << "Truth info: momentum is " << true_momentum.Mag() << "GeV and energy is " << true_energy << "GeV" << std::endl;
    //std::cout << "Event info: momentum is " << momentum.Mag() << "GeV and energy is " << energy << "GeV" << std::endl;
    //std::cout << std::endl;
  
    return true;
  }

  bool ParamReco::finalize() {

    //TGraph * _g = new TGraph(n_evts, &ev_invariant_mass[0], &ev_momentum[0]);
    
    _h_nnbar->Draw();
    _h_nnbar->SetTitle("nnbar parameterised reconstruction");
    _h_nnbar->GetXaxis()->SetTitle("Invariant mass (GeV)");
    _h_nnbar->GetYaxis()->SetTitle("Momentum (GeV)");
    
     
    _fout->Write();
  
    return true;
  }
  
  // this function stolen from dune fast mc code
  TVector3 ParamReco::SmearAngle(TVector3 input, double theta) {
    
    // Unit vector smeared around Z axis by gaussian theta and pi
    double rtheta = rand_var->Gaus(0, theta);
    double rphi = rand_var->Uniform(2.0 * 3.141592654);
    
    // Need to have a safe copy of the input for later.
    TVector3 vsafe = input;
    
    // need to make sure the input isn't along the z axis.
    if(input.z() > 0.9999999){
      TVector3 xaxis(1.0,0.0,0.0);
      // get the vector normal to input and x axis with cross product
      TVector3 vnorm = input.Cross(xaxis);
      // rotate by theta (its +/-) around that vector
      input.Rotate(rtheta,vnorm);
    } else {
      TVector3 zaxis(0.0,0.0,1.0);
      // normal case get the vector normal to input and z axis with cross product
      TVector3 vnorm = input.Cross(zaxis);
      // rotate theta (its +/-) around that vector
      input.Rotate(rtheta,vnorm);
    }
    
    // now rotate by phi around original vector.
    input.Rotate(rphi,vsafe);
    
    // should be unnecessary unit vector
    TVector3 output = input.Unit();
    
    return output;
    
  }
  
  double ParamReco::GetMass(int pdg) {
    
    double mass;
    
    if (TMath::Abs(pdg) == 211) mass = 139.570;
    else if (pdg == 111)        mass = 134.976;
    else if (pdg == 2212)       mass = 938.272;
    else if (pdg == 2112)       mass = 939.565;
    else if (pdg == 22)         mass = 0;
    else {
      std::cerr << "PDG code not recognised: " << pdg << ". Exiting..." << std::endl;
      exit(1);
    }
    
    return (0.001 * mass);
  }

}
#endif
