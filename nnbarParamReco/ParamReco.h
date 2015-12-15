/**
 * \file ParamReco.h
 *
 * \ingroup nnbarParamReco
 * 
 * \brief Class def header for a class ParamReco
 *
 * @author jhewes15
 */

/** \addtogroup nnbarParamReco

    @{*/

#ifndef LARLITE_PARAMRECO_H
#define LARLITE_PARAMRECO_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class ParamReco
     User custom analysis class made by SHELL_USER_NAME
   */
  class ParamReco : public ana_base{
  
  public:

    /// Default constructor
    ParamReco(){ _name="ParamReco"; _fout=0;}

    /// Default destructor
    virtual ~ParamReco(){}

    /** IMPLEMENT in ParamReco.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ParamReco.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ParamReco.cc! 
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
