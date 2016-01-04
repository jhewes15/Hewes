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
    
    // variables & functions used for parameterised reconstruction
    // stolen shamelessly from dan & rik's dune fast mc code
    
    TVector3 SmearAngle(TVector3 input, double theta);
    
    double pipm_Ep_lowwidth   = 5.0;
    double pipm_Ep_lowbias    = 1.0;
    double pipm_Ep_highwidth  = 30.0;
    double pipm_Ep_highbias   = 5.0;
    double pipm_Ep_highorlow  = 0.5;
    double pipm_q_width       = 1.0;
    double pipm_q_bias        = 0.0;
    double pipm_E_thresh      = 0.1;
    
    double pi0_Ep_lowwidth    = 28.0;
    double pi0_Ep_lowbias     = 2.8;
    double pi0_Ep_highwidth   = 28.0;
    double pi0_Ep_highbias    = 2.8;
    double pi0_Ep_highorlow   = 0.0;
    double pi0_q_width        = 3.0;
    double pi0_q_bias         = 0.0;
    double pi0_E_thresh       = 0.042;
    
    double p_Ep_lowwidth      = 10.0;
    double p_Ep_lowbias       = 1.0;
    double p_Ep_highwidth     = 30.0;
    double p_Ep_highbias      = 5.0;
    double p_Ep_highorlow     = 0.4;
    double p_q_width          = 5.0;
    double p_q_bias           = 0.0;
    double p_E_thresh         = 0.050;
    
    double n_Ep_lowwidth      = 50.0;
    double n_Ep_lowbias       = 0.0;
    double n_Ep_highwidth     = 40.0;
    double n_Ep_highbias      = 0.0;
    double n_Ep_highorlow     = 1.0;
    double n_q_width          = 5.0;
    double n_q_bias           = 0.0;
    double n_E_thresh         = 0.050;
    double n_escape_prob      = 0.10;
    double n_KE_frac          = 0.60;
    
    double gamma_Ep_lowwidth  = 15.0;
    double gamma_Ep_lowbias   = 2.0;
    double gamma_Ep_highwidth = 15.0;
    double gamma_Ep_highbias  = 2.0;
    double gamma_Ep_highorlow = 0.0;
    double gamma_q_width      = 1.0;
    double gamma_q_bias       = 0.0;
    double gamma_E_thresh     = 0.030;

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
