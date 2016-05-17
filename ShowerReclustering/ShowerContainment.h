/**
 * \file ShowerContainment.h
 *
 * \ingroup ShowerReclustering
 *
 * \brief Class def header for a class ShowerContainment
 *
 * @author jeremy
 */

/** \addtogroup ShowerReclustering
 
 @{*/

#ifndef LARLITE_SHOWERCONTAINMENT_H
#define LARLITE_SHOWERCONTAINMENT_H

#include "Analysis/ana_base.h"

#include "LArUtil/LArUtilBase.h"
#include "LArUtil/TimeService.h"
#include "LArUtil/PxUtils.h"

#include "DataFormat/shower.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/pfpart.h"
#include "DataFormat/spacepoint.h"

#include "TTree.h"

#include "ConeProjection/geoconic.h"

#include <cmath>

namespace larlite {
  /**
   \class ShowerContainment
   User custom analysis class made by SHELL_USER_NAME
   */
  class ShowerContainment : public ana_base{
    
  public:
    
    /// Default constructor
    ShowerContainment(){
      _name="ShowerContainment";
      _fout=0;
      _geom = larutil::GeometryHelper::GetME();
      _tservice = larutil::TimeService::GetME();
      _tpc_clock = &(_tservice->TPCClock());
      _tick_offset = _tservice->TriggerOffsetTPC() * _tpc_clock->Frequency();
      _n_planes = larutil::Geometry::GetME()->Nplanes();
    }
    
    /// Default destructor
    virtual ~ShowerContainment(){}
    
    /** IMPLEMENT in ShowerContainment.cc!
     Initialization method to be called before the analysis event loop.
     */
    virtual bool initialize();
    
    /** IMPLEMENT in ShowerContainment.cc!
     Analyze a data event-by-event
     */
    virtual bool analyze(storage_manager* storage);
    
    /** IMPLEMENT in ShowerContainment.cc!
     Finalize method to be called after all events processed.
     */
    virtual bool finalize();
    
    /**
     * Function to decide if spacepoint falls inside volume of cone
     */
    bool InsideCone(shower sh, spacepoint sp) {
      
      // get the spacepoint position as a TVector3
      TVector3 point(sp.XYZ()[0],sp.XYZ()[1],sp.XYZ()[2]);
      
      // get shower start point as a TVector3
      TVector3 shower_start(sh.ShowerStart()[0],sh.ShowerStart()[1],sh.ShowerStart()[2]);
      
      // transform the spacepoint so shower start is at origin
      point -= shower_start;
      
      // get shower direction
      TVector3 shower_dir(sh.Direction()[0],sh.Direction()[1],sh.Direction()[2]);
      
      // now get direction of shower
      double phi = shower_dir.Phi();
      double theta = shower_dir.Theta();
      
      // now rotate point around these frames
      point.RotateZ(-phi);
      point.RotateY(-theta);
      
      // calculate cone radius
      double cone_radius = point.Z() * tan(sh.OpeningAngle());
      
      // check to see if point is inside cone or not
      if (point.Z() < 0 || point.Z() > sh.Length() || point.Perp() > cone_radius)
        return false;
      else
        return true;
    }
    
    /**
     * Function to calculate percentage of hits contained in cone
     */
    double CalculateContainment(std::vector<std::vector<larutil::PxHit>> hits_inside, std::vector<std::vector<larutil::PxHit>> hits_all) {
      int n_hits_inside = 0;
      int n_hits_total = 0;
      for (int plane = 0; plane < _n_planes; plane++) {
        n_hits_inside += hits_inside[plane].size();
        n_hits_total += hits_all[plane].size();
      }
      double containment = double(n_hits_inside) / double(n_hits_total);
      return containment;
    }
    
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
    
    /**
     * Function to make 2D larutil::PxPoint out of 3D spacepoint
     */
    larutil::PxPoint MakePxPoint(spacepoint sp, int plane) {
      TVector3 tv3(sp.XYZ()[0],sp.XYZ()[1],sp.XYZ()[2]);
      larutil::PxPoint pxp = _geom->Point_3Dto2D(tv3,plane);
      return pxp;
    }
    
    void Clear() {
      _pxhits.clear();
      _pxhits_in_cone.clear();
      _all_pxhits.clear();
      _all_pxhits_in_cone.clear();
      _sp_in_cone.clear();
      _sp_not_in_cone.clear();
      _sp_in_cone_2d.clear();
      _sp_not_in_cone_2d.clear();
      _cone_projection.clear();
      _new_cone_projection.clear();
    }
    
    void SetVerbose(bool yn) { _verbose = yn; };
    
    void SetSmoothness(int s) { _smoothness = s; };
    
    
  protected:
    
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
    TTree * _tree;                          /**< output analysis tree */
    
    std::vector<std::vector<std::vector<larutil::PxHit>>> _pxhits;                  /**< pxhits associated with shower */
    std::vector<std::vector<std::vector<larutil::PxHit>>> _pxhits_in_cone;          /**< pxhits associated with shower that fall inside cone */
    std::vector<std::vector<std::vector<larutil::PxHit>>> _pxhits_in_new_cone;       /**< pxhits associated with showerin event that fall inside corrected cone */
    std::vector<std::vector<std::vector<larutil::PxHit>>> _all_pxhits;              /**< all pxhits in event */
    std::vector<std::vector<std::vector<larutil::PxHit>>> _all_pxhits_in_cone;      /**< all pxhits in event that fall inside cone */
    std::vector<std::vector<std::vector<larutil::PxPoint>>> _cone_projection;       /**< pxpoints describing projection of cone in 2d planes */
    std::vector<std::vector<std::vector<larutil::PxPoint>>> _new_cone_projection;   /**< pxpoints describing projection of cone in 2d planes */
    std::vector<std::vector<std::vector<larutil::PxPoint>>> _sp_in_cone_2d;         /**< 2d projections of spacepoint within cone */
    std::vector<std::vector<std::vector<larutil::PxPoint>>> _sp_not_in_cone_2d;     /**< 2d projections of spacepoint outside cone */
    std::vector<std::vector<spacepoint>> _sp_in_cone;                               /**< spacepoints inside each cone */
    std::vector<std::vector<spacepoint>> _sp_not_in_cone;                           /**< spacepoints not inside each cone */
    std::vector<bool> _new_cone;
    
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
