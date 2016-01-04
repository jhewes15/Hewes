#ifndef LARLITE_PARAMRECO_CXX
#define LARLITE_PARAMRECO_CXX

#include "ParamReco.h"

namespace larlite {

  bool ParamReco::initialize() {

    std::vector<double> energy;
    std::vector<> momentum;

    return true;
  }
  
  bool ParamReco::analyze(storage_manager* storage) {
  
    auto ev_truth = storage->get_data<event_mctruth>("generator");
    
    // loop over all particles in event
    
    for (mctruth a : * ev_truth) {
      std::vector<mcpart> particles(a.GetParticles());
      for (mcpart part : particles) {
        if (part.StatusCode() == 1) {
          
          double px = part.Trajectory().back().Px();
          double py = part.Trajectory().back().Py();
          double pz = part.Trajectory().back().Pz();
          double e  = part.Trajectory().back().E();
          
          // do parameterised reco for each particle
          
          double width, q_width, e_thresh;
          
          if (TMath::Abs(part.PdgCode()) == 211)  { width = pipm_Ep_highwidth;  q_width = pipm_q_width;  e_thresh = pipm_e_thresh;  }
          else if        (part.PdgCode() == 111)  { width = pi0_Ep_highwidth;   q_width = pi0_q_width;   e_thresh = pi0_e_thresh;   }
          else if        (part.PdgCode() == 2112) { width = p_Ep_highwidth;     q_width = p_q_width;     e_thresh = p_e_thresh;     }
          else if        (part.PdgCode() == 2212) { width = n_Ep_highwidth;     q_width = n_q_width;     e_thresh = n_e_thresh;     }
          else if        (part.PdgCode() == 22)   { width = gamma_Ep_highwidth; q_width = gamma_q_width; e_thresh = gamma_e_thresh; }
          else {
            std::cerr << "An unexpected particle showed up, with PDG code " << part.PdgCode() << ". Exiting..." << std::endl;
            exit(1);
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
          double p_reco = -1;
          while(p_reco < 0.0)
            p_reco = rand_var->Gaus(p,p*width);
          
          //use newdir unit vector to form p_reco into components.
          new_direction *= p_reco;
          
          std::cout << "PDG code is " << part.PdgCode() << std::endl;
          std::cout << "Original momentum: ";
          old_direction.Print();
          std::cout << "New momentum:      ";
          new_direction.Print();
        }
      }
    }

    // for each particle, smear out energy and momentum
    
    // then add together all energy & momentum for event
  
    return true;
  }

  bool ParamReco::finalize() {

    // write ttree to output file
  
    return true;
  }
  
  // this function stolen from dune fast mc code
  TVector3 ParamReco::SmearAngle(TVector3 input, double theta) {
    
    // Unit vector smeared around Z axis by gaussian theta and pi
    double rtheta = rand_var->Gaus(0,theta);
    double rphi = rand_var->Uniform(2.0*3.141592654);
    
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

}
#endif
