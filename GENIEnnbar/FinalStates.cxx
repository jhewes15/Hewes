#ifndef LARLITE_FINALSTATES_CXX
#define LARLITE_FINALSTATES_CXX

#include "FinalStates.h"

namespace larlite {

  bool FinalStates::initialize() {

    // Instantiate FSLibrary object
    _FSLibrary = new FSLibrary();
    
    // Instantiate histograms
    _hTotalP     = new TH1D("_hTotalP", "Total net momentum in event", 50, 0, 2);
    _hTotalKE    = new TH1D("_hTotalKE", "Total kinetic energy in event", 50, 0, 3);
    _hPDG        = new TH1I("_hPDG", "PDG breakdown in event", 6, 0, 6);
    _hPionP      = new TH1D("_hPionP", "Pion momentum", 50, 0, 2);
    _hPionKE     = new TH1D("_hPionKE", "Pion kinetic energy", 50, 0, 2);
    _hGammaP     = new TH1D("_hGammaP", "Gamma momentum", 50, 0, 2);
    _hGammaKE    = new TH1D("_hGammaKE", "Gamma kinetic energy", 50, 0, 2);
    _hNucleonP   = new TH1D("_hNucleonP","Nucleon momentum", 50, 0, 2);
    _hNucleonKE  = new TH1D("_hNucleonKE", "Nucleon kinetic energy", 50, 0, 2);
    _hTotalKEvsP = new TH2D("_hTotalKEvsP", "Total net momentum vs kinetic energy in event", 50, 0, 2, 50, 0, 2);

    return true;
  }
  
  bool FinalStates::analyze(storage_manager* storage) {
    
    // get mctruth info out of input info holder
    event_mctruth * ev_truth = storage->get_data<event_mctruth>("generator");
    
    // create a final state object for this event
    FinalState fs;
    fs._nEvents  = 1;
    fs._nPiPlus  = 0;
    fs._nPiMinus = 0;
    fs._nPiZero  = 0;
    fs._nGamma   = 0;
    fs._nProton  = 0;
    fs._nNeutron = 0;
    
    // Define some useful variables
    double totalPx = 0;
    double totalPy = 0;
    double totalPz = 0;
    double totalKE = 0;
    
    // Get the mcparticles for this event
    for (mctruth a : * ev_truth) {
      std::vector<mcpart> particles(a.GetParticles());
      
      // Loop over each particle, and add it to the tally
      for (mcpart part : particles) {
        
        // Make sure it's a particle in the final state
        if (part.StatusCode() == 1) {
          
          // Add it to the PDG histogram
          AddToPDGHist(part.PdgCode());
          
          // Check particle type, and add to the tally
          if (part.PdgCode() == 211)       fs._nPiPlus++;
          else if (part.PdgCode() == -211) fs._nPiMinus++;
          else if (part.PdgCode() == 111)  fs._nPiZero++;
          else if (part.PdgCode() == 22)   fs._nGamma++;
          else if (part.PdgCode() == 2112) fs._nProton++;
          else if (part.PdgCode() == 2212) fs._nNeutron++;
          else std::cout << "PDG code not a pion but something else! We have " << part.PdgCode() << ", whatever that is." << std::endl;
          
          // Get particle momentum & kinetic energy
          double px = part.Trajectory().back().Px();
          double py = part.Trajectory().back().Py();
          double pz = part.Trajectory().back().Pz();
          double p  = sqrt(pow(px,2) + pow(py,2) + pow(pz,2));
          double e  = part.Trajectory().back().E();
          
          // Add momentum & KE to relevant histogram
          if (part.PdgCode() == 211 || part.PdgCode() == -211 || part.PdgCode() == 111) {
            _hPionKE->Fill(e);
            _hPionP->Fill(p);
          } else if (part.PdgCode() == 2112 || part.PdgCode() == 2212) {
            _hNucleonKE->Fill(e);
            _hNucleonP->Fill(p);
          } else if (part.PdgCode() == 22) {
            _hGammaKE->Fill(e);
            _hGammaP->Fill(p);
          } else {
            std::cout << "Unrecognised particle of type " << part.PdgCode() << ". Exiting..." << std::endl;
            exit(1);
          }
          
          // Add energy & momentum to total
          totalPx += px;
          totalPy += py;
          totalPz += pz;
          totalKE += e;
        }
      }
    }
    
    // Add eventwise information to histograms
    double totalP = sqrt(pow(totalPx, 2) + pow(totalPy, 2) + pow(totalPz, 2));
    _hTotalP->Fill(totalP);
    _hTotalKE->Fill(totalKE);
    _hTotalKEvsP->Fill(totalKE, totalP);
    
    // pass this event on to the final state library
    _FSLibrary -> AddEvent(fs);
    
    // throw some debug output out, if that's your kind of thing
    if (_debug) {
      std::cout << "|-----------------------------------------------|" << std::endl;
      std::cout << "|                 EVENT SUMMARY                 |" << std::endl;
      std::cout << "|-----------------------------------------------|" << std::endl;
      std::cout << "| " << fs._nPiPlus << " pi+, " << fs._nPiMinus << " pi-, "
      << fs._nPiZero << " pi0                           |" << std::endl;
      std::cout << "| " << fs._nGamma << " gamma, " << fs._nProton << " proton, "
      << fs._nNeutron << " neutron                  |" << std::endl;
      std::cout << "|-----------------------------------------------|" << std::endl << std::endl;
    }
    
    return true;
  }

  // larlite ana module finalize function
  bool FinalStates::finalize() {
    
    _hTotalKEvsP->Draw("colz");
    
    _fout->Write();
    
    if (_debug)
      _FSLibrary->Summary();
  
    return true;
  }
  
  // Utility function for adding PDG codes to histogram
  void FinalStates::AddToPDGHist(int pdg) {
    
    if (pdg == 211)
      _hPDG->AddBinContent(1);
    else if (pdg == 111)
      _hPDG->AddBinContent(2);
    else if (pdg == -211)
      _hPDG->AddBinContent(3);
    else if (pdg == 22)
      _hPDG->AddBinContent(4);
    else if (pdg == 2112)
      _hPDG->AddBinContent(5);
    else if (pdg == 2212)
      _hPDG->AddBinContent(6);
    else
      std::cout << "Didn't recognise pdg code: " << pdg << std::endl;
  }

}
#endif
