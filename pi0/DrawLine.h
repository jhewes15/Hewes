/**
 * \file DrawLine.h
 *
 * \ingroup pi0
 * 
 * \brief Class def header for a class DrawLine
 *
 * @author jhewes15
 */

/** \addtogroup pi0

    @{*/

#ifndef LARLITE_DRAWLINE_H
#define LARLITE_DRAWLINE_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class DrawLine
     User custom analysis class made by SHELL_USER_NAME
   */
  class DrawLine : public ana_base{
  
  public:

    /// Default constructor
    DrawLine(){ _name="DrawLine"; _fout=0;}

    /// Default destructor
    virtual ~DrawLine(){}

    /** IMPLEMENT in DrawLine.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in DrawLine.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in DrawLine.cc! 
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
