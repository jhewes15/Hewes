/**
 * \file TrackQuality.h
 *
 * \ingroup nnbar
 * 
 * \brief Class def header for a class TrackQuality
 *
 * @author jhewes15
 */

/** \addtogroup nnbar

    @{*/

#ifndef LARLITE_TRACKQUALITY_H
#define LARLITE_TRACKQUALITY_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctrack.h"
#include "TTree.h"

namespace larlite {
  /**
     \class TrackQuality
     User custom analysis class made by SHELL_USER_NAME
   */
  class TrackQuality : public ana_base{
  
  public:

    /// Default constructor
    TrackQuality(){ _name="TrackQuality"; _fout=0;}

    /// Default destructor
    virtual ~TrackQuality(){}

    /** IMPLEMENT in TrackQuality.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in TrackQuality.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in TrackQuality.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
    
    TTree * _tree;
    
    std::vector<double> track_length;
    std::vector<double> pdg_code;
    std::vector<double> track_energy;
    
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
