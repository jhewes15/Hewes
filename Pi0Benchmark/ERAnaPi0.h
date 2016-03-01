/**
 * \file ERAnaPi0.h
 *
 * \ingroup Pi0Benchmark
 * 
 * \brief Class def header for a class ERAnaPi0
 *
 * @author jhewes15
 */

/** \addtogroup Pi0Benchmark

    @{*/

#ifndef ERTOOL_ERANAPI0_H
#define ERTOOL_ERANAPI0_H

#include "ERTool/Base/AnaBase.h"
#include "TTree.h"

namespace ertool {

  /**
     \class ERAnaPi0
     User custom Analysis class made by kazuhiro
   */
  class ERAnaPi0 : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaPi0(const std::string& name="ERAnaPi0");

    /// Default destructor
    virtual ~ERAnaPi0(){}

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

  protected:
    
    TTree * _Pi0Ana_tree = nullptr;
    
    int _n_shwr_mc;
    int _n_shwr_pr;
    
    int _n_pi0_mc;
    int _n_pi0_pr;
    
    double _energy_res;
    double _angle_res;
    
  };
}
#endif

/** @} */ // end of doxygen group 
