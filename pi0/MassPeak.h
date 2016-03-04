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
#include "DataFormat/shower.h"
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

  protected:
    
    TTree * _tree;
    std::vector<double> mass;
    double n_showers_1; // no. of reconstructed showers in event
    double n_showers_2; // no. of reco'd showers w/ >10MeV
    double n_showers_3; // no. of showers w/ shared vtx
    
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
