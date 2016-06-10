/**
 * \file DataComparison.h
 *
 * \ingroup nnbar
 * 
 * \brief Class def header for a class DataComparison
 *
 * @author jhewes15
 */

/** \addtogroup nnbar

    @{*/

#ifndef LARLITE_DATACOMPARISON_H
#define LARLITE_DATACOMPARISON_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"
#include "TTree.h"
#include <vector>


namespace larlite {

  class DataComparison : public ana_base{
  
  public:

    /// Default constructor
    DataComparison(){ _name="DataComparison"; _fout=0;}

    /// Default destructor
    virtual ~DataComparison(){}

    /// Initialise function
    virtual bool initialize();

    /// Analyse function
    virtual bool analyze(storage_manager* storage);

    /// Finalise function
    virtual bool finalize();

  protected:
    
    // output analysis tree
    TTree * _tree;

    // multiplicity info
    int n_piplus;
    int n_piminus;
    int n_pizero;
    int n_omega;

    // data-mc comparison variables
    double dipion_mass_10 = 0;

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
      return -1;
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
