/**
 * \file FSLibrary.h
 *
 * \ingroup GENIEnnbar
 * 
 * \brief Class def header for a class FSLibrary
 *
 * @author jhewes15
 */

/** \addtogroup GENIEnnbar

    @{*/
#ifndef FSLIBRARY_H
#define FSLIBRARY_H

#include <iostream>
#include <vector>

/**
   \struct FinalState
   A simple struct to hold information on an nnbar final state.
   It just holds a few integers, keeping track of a few pieces of information.
   It keeps track of how many events have this final state, and how many of
   each type of particle are contained in it.
 */
struct FinalState{
  
  int _nEvents;
  int _nPiPlus;
  int _nPiMinus;
  int _nPiZero;
  int _nGamma;
  int _nProton;
  int _nNeutron;
  
};

/**
   \class FSLibrary
   The FSLibrary is designed as a simple library to keep track of all the final states.
   The only data it contains is a vector of FinalState structs, and it has a single function
   which takes in a final state, compares it to the states it has on record, and either adds
   to the tally for an existing final state, or creates a new entry for a new final state.
 */

class FSLibrary{

public:

  /// Default constructor
  FSLibrary(){}

  /// Default destructor
  ~FSLibrary(){}
  
  /// Add new event to library
  void AddEvent(FinalState s);
  
  /// Print summary of library
  void Summary();
  
private:
  
  /// Vector of final state objects
  std::vector<FinalState> FinalStates;

};

#endif
/** @} */ // end of doxygen group 

