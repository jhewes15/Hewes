/**
 * \file ERAnaFlashEfficiency.h
 *
 * \ingroup pi0
 * 
 * \brief Class def header for a class ERAnaFlashEfficiency
 *
 * @author jhewes15
 */

/** \addtogroup pi0

    @{*/

#ifndef ERTOOL_ERANAFLASHEFFICIENCY_H
#define ERTOOL_ERANAFLASHEFFICIENCY_H

#include "ERTool/Base/AnaBase.h"

namespace ertool {

  /**
     \class ERAnaFlashEfficiency
     User custom Analysis class made by kazuhiro
   */
  class ERAnaFlashEfficiency : public AnaBase {
  
  public:

    /// Default constructor
    ERAnaFlashEfficiency(const std::string& name="ERAnaFlashEfficiency");

    /// Default destructor
    virtual ~ERAnaFlashEfficiency(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

  };
}
#endif

/** @} */ // end of doxygen group 
