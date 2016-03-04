#ifndef LARLITE_DRAWLINE_CXX
#define LARLITE_DRAWLINE_CXX

#include "DrawLine.h"

namespace larlite {

  bool DrawLine::initialize() {

    return true;
  }
  
  bool DrawLine::analyze(storage_manager* storage) {
  
    // this anaunit draws a line. that's literally all it does. check this motherfucker out:
    print(larlite::msg::kNORMAL,__FUNCTION__,Form("______________________________________________________________________________"));
  
    return true;
  }

  bool DrawLine::finalize() {
  
    return true;
  }

}
#endif
