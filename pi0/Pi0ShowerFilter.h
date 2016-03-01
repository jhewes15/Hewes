/**
 * \file Pi0ShowerFilter.h
 *
 * \ingroup pi0
 * 
 * \brief Class def header for a class Pi0ShowerFilter
 *
 * @author jeremy
 */

/** \addtogroup pi0

    @{*/

#ifndef LARLITE_PI0SHOWERFILTER_H
#define LARLITE_PI0SHOWERFILTER_H

#include "Analysis/ana_base.h"
#include "DataFormat/pfpart.h"

namespace larlite {
  /**
     \class Pi0ShowerFilter
     User custom analysis class made by jeremy
   */
  class Pi0ShowerFilter : public ana_base{
  
  public:

    /// Default constructor
    Pi0ShowerFilter(){ _name="Pi0ShowerFilter"; _fout=0; };

    /// Default destructor
    virtual ~Pi0ShowerFilter(){};

    /** IMPLEMENT in Pi0ShowerFilter.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in Pi0ShowerFilter.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in Pi0ShowerFilter.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:
    
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
