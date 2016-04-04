/**
 * \file ERAnaMassPeak.h
 *
 * \ingroup pi0
 * 
 * \brief Class def header for a class ERAnaMassPeak
 *
 * @author jhewes15
 */

/** \addtogroup pi0

    @{*/

#ifndef ERTOOL_ERANAMASSPEAK_H
#define ERTOOL_ERANAMASSPEAK_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"

namespace ertool {

  /**
     \class ERAnaMassPeak
     User custom Analysis class made by kazuhiro
   */
  class ERAnaMassPeak : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaMassPeak(const std::string& name="ERAnaMassPeak");

    /// Default destructor
    virtual ~ERAnaMassPeak(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);
    
    double CalculateMass(Shower shower1, Shower shower2);
    
    void SetEnergyCut(double cut)    { energy_cut    = cut; };
    void SetAngleCut(double cut)     { angle_cut     = cut; };
    void SetAsymmetryCut(double cut) { asymmetry_cut = cut; };
    void SetVtxCut(double cut)       { vtx_cut       = cut; };
    
  protected:
    
    double energy_cut    = 50.;
    double angle_cut     = 0.3;
    double asymmetry_cut = 0.8;
    double vtx_cut       = 10.;
    
    TTree * MassPeak;
    
    int n_showers_1; // no. of reco'd showers in event
    int n_showers_2; // no. of reco'd showers w/ >10MeV
    int n_showers_3; // no. of reco'd showers w/ shared vtx
    
    std::vector<double> mass_inclusive; // invariant mass of all shower pairs
    std::vector<double> mass_exclusive; // invariant mass of only good shower pairs
    
    std::vector<double> start_point_distance; // distance between shower start points

  };
}
#endif

/** @} */ // end of doxygen group 
