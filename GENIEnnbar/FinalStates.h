/**
 * \file FinalStates.h
 *
 * \ingroup GENIEnnbar
 * 
 * \brief Class def header for a class FinalStates
 *
 * @author jhewes15
 */

/** \addtogroup GENIEnnbar

    @{*/

#ifndef LARLITE_FINALSTATES_H
#define LARLITE_FINALSTATES_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcpart.h"
#include "FSLibrary.h"
#include "TH2.h"

namespace larlite {
  /**
     \class FinalStates
     User custom analysis class made by SHELL_USER_NAME
   */
  class FinalStates : public ana_base{
  
  public:

    /// Default constructor
    FinalStates(){ _name="FinalStates"; _fout=0;}

    /// Default destructor
    virtual ~FinalStates(){}

    /**
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /**
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /**
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
    void SetDebug(bool debug) {_debug = debug; return;};
    
    void AddToPDGHist(int pdg);
    
  private:
    
    // Final state library instance
    FSLibrary * _FSLibrary;
    
    // Debug option
    bool _debug = false;
    
    // Plots of event-wise information
    TH1 * _hTotalP;
    TH1 * _hTotalKE;
    TH1 * _hPDG;
    
    // Plots of pion info
    TH1 * _hPionP;
    TH1 * _hPionKE;
    
    // Plots of gamma info
    TH1 * _hGammaP;
    TH1 * _hGammaKE;
    
    // Plots of nucleon info
    TH1 * _hNucleonP;
    TH1 * _hNucleonKE;
    
    // 2D plot of KE vs P per event
    TH2 * _hTotalKEvsP;
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
