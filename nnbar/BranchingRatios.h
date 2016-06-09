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

    // select final state topology
    int topology() {
      if (n_piplus == 1 && n_piminus == 0 && n_pizero == 1 && n_omega == 0)
        return 1;
      else if (n_piplus == 1 && n_piminus == 0 && n_pizero == 2 && n_omega == 0)
        return 2;
      else if (n_piplus == 1 && n_piminus == 0 && n_pizero == 3 && n_omega == 0)
        return 3;
      else if (n_piplus == 2 && n_piminus == 1 && n_pizero == 1 && n_omega == 0)
        return 4;
      else if (n_piplus == 2 && n_piminus == 1 && n_pizero == 2 && n_omega == 0)
        return 5;
      else if (n_piplus == 2 && n_piminus == 1 && n_pizero == 0 && n_omega == 2)
        return 6;
      else if (n_piplus == 3 && n_piminus == 2 && n_pizero == 1 && n_omega == 0)
        return 7;
      else if (n_piplus == 1 && n_piminus == 1 && n_pizero == 0 && n_omega == 0)
        return 8;
      else if (n_piplus == 0 && n_piminus == 0 && n_pizero == 2 && n_omega == 0)
        return 9;
      else if (n_piplus == 1 && n_piminus == 1 && n_pizero == 1 && n_omega == 0)
        return 10;
      else if (n_piplus == 1 && n_piminus == 1 && n_pizero == 2 && n_omega == 0)
        return 11;
      else if (n_piplus == 1 && n_piminus == 1 && n_pizero == 3 && n_omega == 0)
        return 12;
      else if (n_piplus == 2 && n_piminus == 2 && n_pizero == 0 && n_omega == 0)
        return 13;
      else if (n_piplus == 2 && n_piminus == 2 && n_pizero == 1 && n_omega == 0)
        return 14;
      else if (n_piplus == 1 && n_piminus == 1 && n_pizero == 0 && n_omega == 1)
        return 15;
      else if (n_piplus == 2 && n_piminus == 2 && n_pizero == 2 && n_omega == 0)
        return 16;
      print(msg::kWARNING,__FUNCTION__,Form("Warning! Couldn't find a reasonable final state for this."));
      return 0;
    }
    
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
