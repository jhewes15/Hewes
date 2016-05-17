/**
 * \file ShowerHitRestorer.h
 *
 * \ingroup ShowerReclustering
 * 
 * \brief Class def header for a class ShowerHitRestorer
 *
 * @author jeremy
 */

/** \addtogroup ShowerReclustering

    @{*/

#ifndef LARLITE_SHOWERHITRESTORER_H
#define LARLITE_SHOWERHITRESTORER_H

#include "Analysis/ana_base.h"

#include "LArUtil/LArUtilBase.h"
#include "LArUtil/TimeService.h"
#include "LArUtil/PxUtils.h"

#include "DataFormat/shower.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"

#include "TTree.h"

#include "ConeProjection/geoconic.h"

#include <cmath>

namespace larlite {
  /**
     \class ShowerHitRestorer
     User custom analysis class made by SHELL_USER_NAME
   */
  class ShowerHitRestorer : public ana_base{
  
  public:

    /// Default constructor
    ShowerHitRestorer(){
      _name="ShowerHitRestorer";
      _fout=0;
      _geom = larutil::GeometryHelper::GetME();
      _tservice = larutil::TimeService::GetME();
      _tpc_clock = &(_tservice->TPCClock());
      _tick_offset = _tservice->TriggerOffsetTPC() * _tpc_clock->Frequency();
      _n_planes = larutil::Geometry::GetME()->Nplanes();
    }

    /// Default destructor
    virtual ~ShowerHitRestorer(){}

    /** IMPLEMENT in ShowerHitRestorer.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ShowerHitRestorer.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ShowerHitRestorer.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();
    
    /**
     * Function to make larutil::PxHit out of larlite::hit
     */
    larutil::PxHit MakePxHit(hit h) {
      larutil::PxHit pxh;
      pxh.t = (h.PeakTime() + _tick_offset) * _geom->TimeToCm() ;
      pxh.w = h.WireID().Wire  * _geom->WireToCm();
      pxh.charge = h.Integral();
      pxh.plane = h.View();
      return pxh;
    }
    
    void Clear() {
      _hit_candidates.clear();
      _associated_hits.clear();
      _cone_projection.clear();
    }
    
    void SetTreeName(std::string name) { _tree_name = name; };
    
    void SetInputProducer(std::string producer) { _input_producer = producer; };
    
    void SetOutputProducer(std::string producer) { _output_producer = producer; };

  protected:
    
    std::string _tree_name;                 /**< name of output tree */
    bool _verbose = false;                  /**< verbosity bool */
    int _smoothness = 20;                   /**< number of points on 2d cone polygon */
    std::string _input_producer;            /**< shower input producer */
    std::string _output_producer;           /**< pfpart output producer */
    
    
    const larutil::GeometryHelper * _geom;  /**< geometry helper */
    const larutil::TimeService * _tservice; /**< time service */
    const larutil::ElecClock * _tpc_clock;  /**< tpc clock */
    double _tick_offset;                    /**< tick offset */
    unsigned int _n_planes;                 /**< number of planes */
    ::larlite::geoconic fgeoconic;          /**< geoconic object for 2d cone projections. found in RecoTool/ConeProjection */
    TTree * _tree;                          /**< output tree */
    
    int _n_showers;                                                 /**< number of showers in event */
    std::vector<double> _missing_charge;                            /**< Charge from unclustered hits that aren't merged */
    std::vector<std::vector<larutil::PxHit>> _hit_candidates;       /**< Hits inside cone associated with no clusters */
    std::vector<std::vector<larutil::PxHit>> _associated_hits;      /**< Hits associated with shower */
    std::vector<std::vector<larutil::PxPoint>> _cone_projection;    /**< Projections of 3D cone in 2D planes */
    
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
