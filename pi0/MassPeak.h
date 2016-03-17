/**
 * \file MassPeak.h
 *
 * \ingroup pi0
 * 
 * \brief Class def header for a class MassPeak
 *
 * @author jhewes15
 */

/** \addtogroup pi0

    @{*/

#ifndef LARLITE_MASSPEAK_H
#define LARLITE_MASSPEAK_H

#include "Analysis/ana_base.h"
#include "MCComp/MCMatchAlg.h"
#include "DataFormat/shower.h"
#include "DataFormat/mcshower.h"
#include "TTree.h"


namespace larlite {
  /**
     \class MassPeak
     User custom analysis class made by SHELL_USER_NAME
   */
  class MassPeak : public ana_base{
  
  public:

    /// Default constructor
    MassPeak(){ _name="MassPeak"; _fout=0;}

    /// Default destructor
    virtual ~MassPeak(){}

    /** IMPLEMENT in MassPeak.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in MassPeak.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in MassPeak.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
    /// Function to calculate invariant mass for a pair of showers
    double CalculateMass(shower shower1, shower shower1);
    
    void SetShowerProducer(string producer) { shower_producer = producer; };
    
    void SetEnergyCut    (double cut) { energy_cut     = cut; };
    void SetAngleCut     (double cut) { angle_cut      = cut; };
    void SetEnergySumCut (double cut) { energy_sum_cut = cut; };
    void SetVertexCut    (double cut) { vertex_cut     = cut; };

  protected:
    
    string shower_producer;
    
    double energy_cut     = 40.;
    double angle_cut      = 0.00873;
    double energy_sum_cut = 70.;
    double vertex_cut     = 100.;
    
    TTree * mass_peak_tree;
    
    int n_showers; /// number of reconstructed showers in event
    
    std::vector<double> mass_inclusive /// invariant mass of all shower pairs
    std::vector<double> mass_exclusive /// invariant mass of shower pairs that pass all cuts
    
    std::vector<double> start_point_distance /// distance between shower start points
    
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
