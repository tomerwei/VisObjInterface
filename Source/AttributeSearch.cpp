/*
  ==============================================================================

    AttributeSearch.cpp
    Created: 6 Jan 2016 11:25:42am
    Author:  falindrith

  ==============================================================================
*/

#include "AttributeSearch.h"

map<EditType, vector<EditConstraint> > editConstraints = {
  { KEY_HUE, { EditConstraint(L_KEY, HUE) } },
  { KEY_INTENS, { EditConstraint(L_KEY, INTENSITY) } }
};

Array<SearchResult> attributeSearch(map<string, AttributeControllerBase*> active, int editDepth)
{
  // If there's no active attribute, just leave
  if (active.size() == 0)
    return Array<SearchResult>();

  // Save current state of rig.
  Rig* rig = getRig();
  Snapshot* original = new Snapshot(rig, nullptr);
  Array<SearchResult> scenes;

  // objective function for combined set of active attributes.
  attrObjFunc f = [active, original](Snapshot* s) {
    double sum = 0;
    for (const auto& kvp : active) {
      if (kvp.second->getStatus() == A_LESS)
        sum += kvp.second->evaluateScene(s->getRigData());
      else if (kvp.second->getStatus() == A_MORE)
        sum -= kvp.second->evaluateScene(s->getRigData());
      else if (kvp.second->getStatus() == A_EQUAL)
        sum += pow(kvp.second->evaluateScene(s->getRigData()) - kvp.second->evaluateScene(original->getRigData()), 2);
    }

    return sum;
  };

  // For each edit, get a list of scenes returned and just add it to the overall list.
  for (const auto& edits : editConstraints) {
    Array<Snapshot*> editScenes = performEdit(edits.first, original, f);

    for (const auto& s : editScenes) {
      SearchResult r;
      r._scene = s;
      r._editHistory.add(edits.first);
      for (const auto& kvp : active) {
        r._attrVals[kvp.first] = kvp.second->evaluateScene(s->getRigData());
      }
      scenes.add(r);
    }
  }

  // filter results
  // recurse if necessary

  return scenes;
}

Array<Snapshot*> performEdit(EditType t, Snapshot * orig, attrObjFunc f)
{
  // note of interest: which light is they key may vary though the course of this function,
  // potentially causing discontinuitous jumps in the objective function.
  // whether or not this is fatal remains to be seen.

  // duplicate initial state for internal use
  Snapshot* s = new Snapshot(*orig);

  // load settings
  double minDist = getGlobalSettings()->_minEditDist;   // may want to set min dist based on how large deriv changes are at start point
  int numScenes = getGlobalSettings()->_numEditScenes;
  double gamma = getGlobalSettings()->_searchGDGamma;
  double thresh = getGlobalSettings()->_searchGDTol;

  // Save start value
  double startAttrVal = f(s);

  int vecSize = editConstraints[t].size();
  Eigen::VectorXd oldX;
  Eigen::VectorXd newX;
  Array<Snapshot*> scenes;

  oldX.resize(vecSize);
  newX.resize(vecSize);

  // Initalize the x (variable) vector
  int i = 0;
  for (const auto& c : editConstraints[t]) {
    newX[i] = getDeviceValue(c, s);
    i++;
  }

  // run gradient descent until number of scenes to return
  // meets minimum, or the optimization is done.
  do {
    oldX = newX;
    
    // Get derivative
    Eigen::VectorXd dX;
    dX.resize(vecSize);
    i = 0;
    for (const auto& c : editConstraints[t]) {
      dX[i] = numericDeriv(c, s, f);
      i++;
    }

    // Descent
    newX = oldX - gamma * dX;

    // Update scene
    i = 0;
    for (const auto& c : editConstraints[t]) {
      setDeviceValue(c, newX[i], s);
      i++;
    }

    // Determine if scene should go in the list of scenes to return
    double attrVal = f(s);
    if (abs(attrVal - startAttrVal) > minDist && scenes.size() < numScenes) {
      scenes.insert(-1, new Snapshot(*s));
    }
    if (scenes.size() >= numScenes) {
      // scenes full, stop
      break;      
    }
  } while ((oldX - newX).norm() > thresh);
  
  return scenes;
}

double numericDeriv(EditConstraint c, Snapshot* s, attrObjFunc f)
{
  // load the appropriate settings and get the proper device.
  double h = getGlobalSettings()->_searchDerivDelta;
  Device* d = getSpecifiedDevice(c._light, s);
  double f1 = f(s);
  double f2;

  switch (c._param) {
  case INTENSITY:
  {
    float o = d->getIntensity()->asPercent();
    d->getIntensity()->setValAsPercent(o + h);
    f2 = f(s);
    d->setIntensity(o);
    break;
  }
  case HUE:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    d->getColor()->setHSV(hsv[0] + h, hsv[1], hsv[2]);
    f2 = f(s);
    d->getColor()->setHSV(hsv[0], hsv[1], hsv[2]);
    break;
  }
  case SAT:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    d->getColor()->setHSV(hsv[0], hsv[1] + h, hsv[2]);
    f2 = f(s);
    d->getColor()->setHSV(hsv[0], hsv[1], hsv[2]);
    break;
  }
  case VALUE:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    d->getColor()->setHSV(hsv[0], hsv[1], hsv[2] + h);
    f2 = f(s);
    d->getColor()->setHSV(hsv[0], hsv[1], hsv[2]);
    break;
  }
  case RED:
  {
    double r = d->getColor()->getColorChannel("Red");
    d->getColor()->setColorChannel("Red", r + h);
    f2 = f(s);
    d->getColor()->setColorChannel("Red", r);
    break;
  }
  case BLUE:
  {
    double b = d->getColor()->getColorChannel("Blue");
    d->getColor()->setColorChannel("Blue", b + h);
    f2 = f(s);
    d->getColor()->setColorChannel("Blue", b);
    break;
  }
  case GREEN:
  {
    double g = d->getColor()->getColorChannel("Green");
    d->getColor()->setColorChannel("Green", g + h);
    f2 = f(s);
    d->getColor()->setColorChannel("Green", g);
    break;
  }
  case POLAR:
  {
    LumiverseOrientation* val = (LumiverseOrientation*)d->getParam("polar");
    float p = val->getVal();
    val->setVal(p + h);
    f2 = f(s);
    val->setVal(p);
    break;
  }
  case AZIMUTH:
  {
    LumiverseOrientation* val = (LumiverseOrientation*)d->getParam("azimuth");
    float a = val->getVal();
    val->setVal(a + h);
    f2 = f(s);
    val->setVal(a);
    break;
  }
  default:
    break;
  }

  return (f2 - f1) / h;
}

void setDeviceValue(EditConstraint c, double val, Snapshot * s)
{
  Device* d = getSpecifiedDevice(c._light, s);

  switch (c._param) {
  case INTENSITY:
    d->setIntensity(val);
    break;
  case HUE:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    d->getColor()->setHSV(val, hsv[1], hsv[2]);
    break;
  }
  case SAT:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    d->getColor()->setHSV(hsv[0], val, hsv[2]);
    break;
  }
  case VALUE:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    d->getColor()->setHSV(hsv[0], hsv[1], val);
    break;
  }
  case RED:
    d->getColor()->setColorChannel("Red", val);
    break;
  case BLUE:
    d->getColor()->setColorChannel("Blue", val);
    break;
  case GREEN:
    d->getColor()->setColorChannel("Green", val);
    break;
  case POLAR:
  {
    LumiverseOrientation* o = (LumiverseOrientation*)d->getParam("polar");
    o->setVal(val);
    break;
  }
  case AZIMUTH:
  {
    LumiverseOrientation* o = (LumiverseOrientation*)d->getParam("azimuth");
    o->setVal(val);
    break;
  }
  default:
    break;
  }

}

double getDeviceValue(EditConstraint c, Snapshot * s)
{
  Device* d = getSpecifiedDevice(c._light, s);

  switch (c._param) {
  case INTENSITY:
    return d->getIntensity()->getVal();
  case HUE:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    return hsv[0];
  }
  case SAT:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    return hsv[1];
  }
  case VALUE:
  {
    Eigen::Vector3d hsv = d->getColor()->getHSV();
    return hsv[2];
  }
  case RED:
    return d->getColor()->getColorChannel("Red");
  case BLUE:
    return d->getColor()->getColorChannel("Blue");
  case GREEN:
    return d->getColor()->getColorChannel("Green");
  case POLAR:
  {
    LumiverseOrientation* o = (LumiverseOrientation*)d->getParam("polar");
    return o->getVal();
  }
  case AZIMUTH:
  {
    LumiverseOrientation* o = (LumiverseOrientation*)d->getParam("azimuth");
    return o->getVal();
  }
  default:
    break;
  }
}

Device* getSpecifiedDevice(EditLightType l, Snapshot * s)
{
  auto devices = s->getRigData();
  
  // Rim is easy to identify
  if (l == L_RIM)
    return devices["rim"];
  
  Device* key;
  Device* fill;

  // Determine which light is key/fill
  if (devices["right"]->getIntensity()->getVal() > devices["left"]->getIntensity()->getVal()) {
    key = devices["right"];
    fill = devices["left"];
  }
  else {
    key = devices["left"];
    fill = devices["right"];
  }

  if (l == L_FILL)
    return fill;
  else if (l == L_KEY)
    return key;

  // If this ever happens, something's gone terribly wrong
  return nullptr;
}
