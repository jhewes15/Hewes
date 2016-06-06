/**
 * \file GeneratorInfo.h
 *
 * \ingroup nnbar
 * 
 * \brief Class def header for a class GeneratorInfo
 *
 * @author jhewes15
 */

/** \addtogroup nnbar

    @{*/

#ifndef LARLITE_GENERATORINFO_H
#define LARLITE_GENERATORINFO_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mctrack.h"
#include "TTree.h"
#include <vector>

namespace larlite {
  /**
     \class GeneratorInfo
     User custom analysis class made by SHELL_USER_NAME
   */
  class GeneratorInfo : public ana_base{
  
  public:

    /// Default constructor
    GeneratorInfo(){ _name="GeneratorInfo"; _fout=0;}

    /// Default destructor
    virtual ~GeneratorInfo(){}

    /** IMPLEMENT in GeneratorInfo.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in GeneratorInfo.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in GeneratorInfo.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    // set FSI switched on / off
    void FsiEnabled(bool is) { fsi_enabled = is; }

    // cherrypick a specific final state
    void SetFinalState(int fs) { final_state = fs; }

  protected:
    
    // output tree
    TTree * _tree;

    // data comparison tree
    TTree * _datacomparison_tree;
    
    // do we have any FSI enabled here?
    bool fsi_enabled = true;
    int final_state = 0;

    // multiplicities
    int mult_n;
    int mult_p;
    int mult_pip;
    int mult_pim;
    int mult_pi0;
    int mult_om;
    
    // momentum
    std::vector<double> mom_n;
    std::vector<double> mom_p;
    std::vector<double> mom_pip;
    std::vector<double> mom_pim;
    std::vector<double> mom_pi0;
        
    // event info
    double tot_mom_w_nuc;
    double tot_mom_wo_nuc;
    double tot_im_w_nuc;
    double tot_im_wo_nuc;

    // data comparison variables
    double dipion_mass_10;

    // select the event topology
    int topology() {
      if (mult_pip == 1 && mult_pim == 0 && mult_pi0 == 1 && mult_om == 0)
        return 1;
      else if (mult_pip == 1 && mult_pim == 0 && mult_pi0 == 2 && mult_om == 0)
        return 2;
      else if (mult_pip == 1 && mult_pim == 0 && mult_pi0 == 3 && mult_om == 0)
        return 3;
      else if (mult_pip == 2 && mult_pim == 1 && mult_pi0 == 1 && mult_om == 0)
        return 4;
      else if (mult_pip == 2 && mult_pim == 1 && mult_pi0 == 2 && mult_om == 0)
        return 5;
      else if (mult_pip == 2 && mult_pim == 1 && mult_pi0 == 0 && mult_om == 2)
        return 6;
      else if (mult_pip == 3 && mult_pim == 2 && mult_pi0 == 1 && mult_om == 0)
        return 7;
      else if (mult_pip == 1 && mult_pim == 1 && mult_pi0 == 0 && mult_om == 0)
        return 8;
      else if (mult_pip == 0 && mult_pim == 0 && mult_pi0 == 2 && mult_om == 0)
        return 9;
      else if (mult_pip == 1 && mult_pim == 1 && mult_pi0 == 1 && mult_om == 0)
        return 10;
      else if (mult_pip == 1 && mult_pim == 1 && mult_pi0 == 2 && mult_om == 0)
        return 11;
      else if (mult_pip == 1 && mult_pim == 1 && mult_pi0 == 3 && mult_om == 0)
        return 12;
      else if (mult_pip == 2 && mult_pim == 2 && mult_pi0 == 0 && mult_om == 0)
        return 13;
      else if (mult_pip == 2 && mult_pim == 2 && mult_pi0 == 1 && mult_om == 0)
        return 14;
      else if (mult_pip == 1 && mult_pim == 1 && mult_pi0 == 0 && mult_om == 1)
        return 15;
      else if (mult_pip == 2 && mult_pim == 2 && mult_pi0 == 2 && mult_om == 0)
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
