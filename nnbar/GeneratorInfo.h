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

  protected:
    
    // output tree
    TTree * _tree;

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
