/**
 * \file EvSel.h
 *
 * \ingroup nnbarTruthEvSel
 * 
 * \brief Class def header for a class EvSel
 *
 * @author jhewes15
 */

/** \addtogroup nnbarTruthEvSel

    @{*/

#ifndef LARLITE_EVSEL_H
#define LARLITE_EVSEL_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"
#include "TH2.h"
#include "TVector3.h"
#include "TVector.h"

namespace larlite {
  /**
     \class EvSel
     User custom analysis class made by SHELL_USER_NAME
   */
  class EvSel : public ana_base{
  
  public:

    /// Default constructor
    EvSel(){ _name="EvSel"; _fout=0;}

    /// Default destructor
    virtual ~EvSel(){}

    virtual bool initialize();
    virtual bool analyze(storage_manager* storage);
    virtual bool finalize();

  protected:
    
    TH2 * _h;
    
    std::vector<double> _energy;
    std::vector<double> _momentum;
    std::vector<double> _invMass;
    
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
