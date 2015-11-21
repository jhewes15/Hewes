/**
 * \file FinalStates.h
 *
 * \ingroup GENIEnnbar
 * 
 * \brief Class def header for a class FinalStates
 *
 * @author jhewes15
 */

/** \addtogroup GENIEnnbar

    @{*/

#ifndef LARLITE_FINALSTATES_H
#define LARLITE_FINALSTATES_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcpart.h"
#include "FSLibrary.h"

namespace larlite {
  /**
     \class FinalStates
     User custom analysis class made by SHELL_USER_NAME
   */
  class FinalStates : public ana_base{
  
  public:

    /// Default constructor
    FinalStates(){ _name="FinalStates"; _fout=0;}

    /// Default destructor
    virtual ~FinalStates(){}

    /**
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /**
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /**
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  private:
    
    FSLibrary * _FSLibrary;
    
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
