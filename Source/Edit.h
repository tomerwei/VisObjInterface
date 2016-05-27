/*
  ==============================================================================

    AttributeEdit.h
    Created: 12 May 2016 10:57:22am
    Author:  falindrith

  ==============================================================================
*/

#ifndef ATTRIBUTEEDIT_H_INCLUDED
#define ATTRIBUTEEDIT_H_INCLUDED

#include "globals.h"
#include <random>

using namespace Lumiverse;
using namespace Lumiverse::ShowControl;

enum EditParam;

/*
An edit contains logic for manipulating one scene according to particular rules.
The base edit class does basic operations requiring no knowledge of how other
lights should be edited in relation to others.
*/
class Edit
{
public:
  Edit(set<string> lockedParams);
  ~Edit();

  void initWithArea(string area, bool joint, bool uniform);
  void initWithSystem(string system, bool joint, bool uniform);
  void initArbitrary(string query, bool joint, bool uniform);

  void setParams(set<EditParam> affectedParams);

  // Takes a lighting configuration and modifies it according to the edit rules
  virtual void performEdit(Snapshot* s, double stepSize);

  // Gets the next edit given all the prior edits that have happened.
  virtual Edit* getNextEdit(vector<Edit*>& editHistory, vector<Edit*>& availableEdits);

  // returns true if the edit can actually take effect
  virtual bool canDoEdit();

  string _name;

private:
  void applyParamEdit(Device* d, EditParam p, double delta = -1);
  void setParam(Device* d, EditParam p, double val);
  bool isParamLocked(Device* d, EditParam c);

  set<string> _affectedAreas;
  set<string> _affectedSystems;
  set<EditParam> _affectedParams;

  bool _joint;      // If true, adjusts all lights in the edit by the same delta
  bool _uniform;    // If true, sets all params in the edit to the same value
  
  DeviceSet _affectedDevices;

  default_random_engine _gen;
  uniform_real_distribution<double> _udist;
  normal_distribution<double> _gdist;

  set<string> _lockedParams;
};



#endif  // ATTRIBUTEEDIT_H_INCLUDED