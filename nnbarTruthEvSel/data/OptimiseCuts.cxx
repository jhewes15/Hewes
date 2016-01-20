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
  std::cout << std::endl;
  
  if (n_atmos_pass == 0) {
    std::cerr << "Background rate is zero. You're gonna have to deal with this problem." << std::endl;
    exit(1);
  }
  
  double efficiency = (double)n_nnbar_pass / (double)n_nnbar;

  double quality = efficiency / sqrt((double)n_atmos);
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
  cuts.resize[4];
  cuts = { 1, 2, 0, 0.5 };
  
  double constraint = 0.5;
  
  for (int j = 0; j < 4; j++) {
    std::vector<double> cuts_temp = cuts;
    std::vector<double> candidate;
    candidate.resize(11);
    std::vector<double> candidate_quality;
    candidate_quality.resize(11);
    for (int k = 0; k < 11; k++) {
      temp_cuts[j] += (k - 5) * constraint;
      int    best_index = 5;
      double best_value = 0;
      double quality = CutQuality(_nnbarEnergy, _nnbarMomentum, _atmosEnergy, _atmosMomentum, temp_cuts);
      if (quality > best_value) {
        best_value = quality;
        best_index = k;
      }
    }
    cuts[j] = candidate[best_index];
  }
  
  double constraint = 0.2;
  double _energyMin = 1;
  double _energyMax = 2;

  double _momentumMin = 0;
  double _momentumMax = 0.5;
  
  std::vector<double> _candidate;
  std::vector<double> _candidateQuality;
  
  double _energyMinTemp   = _energyMin;
  double _energyMaxTemp   = _energyMax;
  double _momentumMinTemp = _momentumMin;
  double _momentumMaxTemp = _momentumMax;
  
  _candidate.clear();
  _candidate.resize(11);
  _quality.clear();
  _quality.resize(11);
  
  for (int i = 0; i < 11; i++) {
    _candidate[i] = _energyMin + ((i - 5) * constraint);
    _quality[i]   = CutQuality(
    
  }
  
  double _quality = CutQuality(_nnbarEnergy, _nnbarMomentum, _atmosEnergy, _atmosMomentum, _energyMin, _energyMax, _momentumMin, _momentumMax);
  
  std::cout << "Quality is " << _quality << std::endl;
  
  // figure out the figure of merit
  
  // vary all four of the cuts at once, try to zone in on the area of best selection
  
  
}