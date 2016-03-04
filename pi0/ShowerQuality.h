/**
 * \file ShowerQuality.h
 *
 * \ingroup pi0
 * 
 * \brief Class def header for a class ShowerQuality
 *
 * @author jhewes15
 */

/** \addtogroup pi0

    @{*/

#ifndef LARLITE_SHOWERQUALITY_H
#define LARLITE_SHOWERQUALITY_H

#include "Analysis/ana_base.h"
#include "DataFormat/shower.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/simch.h"
#include "TTree.h"

namespace larlite {
  /**
     \class ShowerQuality
     User custom analysis class made by SHELL_USER_NAME
   */
  class ShowerQuality : public ana_base{
  
  public:

    /// Default constructor
    ShowerQuality(){ _name="ShowerQuality"; _fout=0;}

    /// Default destructor
    virtual ~ShowerQuality(){}

    /** IMPLEMENT in ShowerQuality.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ShowerQuality.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ShowerQuality.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:
    
    //TTree * _tree;
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
