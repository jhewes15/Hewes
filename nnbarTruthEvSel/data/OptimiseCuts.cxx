#include <vector>

// figure of merit
double CutQuality(std::vector<double> nnbarEnergy, std::vector<double> nnbarMomentum, std::vector<double> atmosEnergy,
                  std::vector<double> atmosMomentum, std::vector<double> cuts) {
  
  // get the number of nnbar & atmospheric events
  int n_nnbar = nnbarEnergy.size();
  int n_atmos = atmosEnergy.size();
  
  int n_nnbar_pass = 0;
  int n_atmos_pass = 0;
  
  // count nnbar events that pass cut
  for (int i = 0; i < n_nnbar; i++)
    if (nnbarEnergy[i] > cuts[0] && nnbarEnergy[i] < cuts[1] && nnbarMomentum[i] > cuts[2] && nnbarMomentum[i] < cuts[3])
      n_nnbar_pass ++;
  
  for (int i = 0; i < n_atmos; i++)
    if (atmosEnergy[i] > cuts[0] && atmosEnergy[i] < cuts[1] && atmosMomentum[i] > cuts[2] && atmosMomentum[i] < cuts[3])
      n_atmos_pass ++;
  
  std::cout << n_nnbar_pass << " events out of " << n_nnbar << " nnbar events passed the cut." << std::endl;
  std::cout << n_atmos_pass << " events out of " << n_atmos << " atmospheric events passed the cut." << std::endl;
  
  double quality;
  if (n_atmos_pass == 0) quality = 0;
  else {
    double efficiency = (double)n_nnbar_pass / (double)n_nnbar;
    std::cout << "Efficiency " << efficiency << ", background " << n_atmos_pass << std::endl;
    quality = efficiency / sqrt((double)n_atmos);
  }

  return quality;
}

// root macro to make an nnbar event selection
void OptimiseCuts() {
  
  // open nnbar & atmospheric files and get trees
  TFile _nnbarFile("./EvSel_nnbar.root", "READ");
  TFile _atmosFile("./EvSel_atmos.root", "READ");
  
  TTree * _nnbarTree = (TTree *) _nnbarFile.Get("_t");
  TTree * _atmosTree = (TTree *) _atmosFile.Get("_t");
  
  // get the energy & momentum vectors
  std::vector<double> * _nnbarEnergyTemp;
  std::vector<double> * _nnbarMomentumTemp;
  _nnbarTree->SetBranchAddress("_energy",   & _nnbarEnergyTemp  );
  _nnbarTree->SetBranchAddress("_momentum", & _nnbarMomentumTemp);
  _nnbarTree->GetEntry(0);
  std::vector<double> _nnbarEnergy   = * _nnbarEnergyTemp;
  std::vector<double> _nnbarMomentum = * _nnbarMomentumTemp;
  
  std::vector<double> * _atmosEnergyTemp;
  std::vector<double> * _atmosMomentumTemp;
  _atmosTree->SetBranchAddress("_energy",   & _atmosEnergyTemp  );
  _atmosTree->SetBranchAddress("_momentum", & _atmosMomentumTemp);
  _atmosTree->GetEntry(0);
  std::vector<double> _atmosEnergy   = * _atmosEnergyTemp;
  std::vector<double> _atmosMomentum = * _atmosMomentumTemp;
  
  // scan for an initial set of cuts
  
  // what we really want here is a function that takes a value for each of the four parameters
  // as well as some variable to give us an idea of how tight the constraints are
  // and then reach out over some region defined by that variable
  // and find the best cuts in this region
  // and do this iteratively
  
  // we don't actually need to vary them simultaneously
  // which makes life way way easier
  
  std::vector<double> cuts;
  cuts.resize(4);
  cuts[0] = 1;
  cuts[1] = 2;
  cuts[2] = 0;
  cuts[3] = 0.5;
  
  double constraint = 0.5;
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 4; j++) {
      std::vector<double> temp_cuts = cuts;
      std::vector<double> candidate;
      candidate.resize(11);
      std::vector<double> candidate_quality;
      candidate_quality.resize(11);
      
      int    best_index = 5;
      double best_value = 0;
      for (int k = 0; k < 11; k++) {
        temp_cuts[j] = cuts[j] + (k - 5) * constraint;
        if (temp_cuts[j] < 0) temp_cuts[j] = 0;
        candidate[k] = temp_cuts[j];
        double quality = CutQuality(_nnbarEnergy, _nnbarMomentum, _atmosEnergy, _atmosMomentum, temp_cuts);
        std::cout << "For cuts " << temp_cuts[0] << ", " << temp_cuts[1] << ", " << temp_cuts[2] << ", " << temp_cuts[3] << " quality is " << quality << std::endl;
        if (quality > best_value) {
          best_value = quality;
          best_index = k;
        }
      }
      std::cout << "Best candidate is " << candidate[best_index] << " with quality " << best_value << std::endl;
      std::cout << std::endl;
      cuts[j] = candidate[best_index];
    }
    constraint *= 0.5;
  }
  
  std::cout << "FINAL SUMMARY: best cut values are " << cuts[0] << ", " << cuts[1] << ", " << cuts[2] << ", " << cuts[3] << std::endl;
  CutQuality(_nnbarEnergy, _nnbarMomentum, _atmosEnergy, _atmosMomentum, cuts);
  // figure out the figure of merit
  
  // vary all four of the cuts at once, try to zone in on the area of best selection
  
  
}