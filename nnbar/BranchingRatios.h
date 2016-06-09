/**
 * \file BranchingRatios.h
 *
 * \ingroup nnbar
 * 
 * \brief Class def header for a class BranchingRatios
 *
 * @author jhewes15
 */

/** \addtogroup nnbar

    @{*/

#ifndef LARLITE_BRANCHINGRATIOS_H
#define LARLITE_BRANCHINGRATIOS_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class BranchingRatios
     User custom analysis class made by SHELL_USER_NAME
   */
  class BranchingRatios : public ana_base{
  
  public:

    /// Default constructor
    BranchingRatios(){ _name="BranchingRatios"; _fout=0;}

    /// Default destructor
    virtual ~BranchingRatios(){}

    /** IMPLEMENT in BranchingRatios.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in BranchingRatios.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in BranchingRatios.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

  protected:

    // output tree
    TTree * _tree;
    
    // multiplicities
    int n_piplus;
    int n_piminus;
    int n_pizero;
    int n_omega;

    // final state
    int final_state;
    
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
