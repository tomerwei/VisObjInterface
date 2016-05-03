/*
  ==============================================================================

    OrangeBlueAttribute.cpp
    Created: 26 Apr 2016 4:55:57pm
    Author:  falindrith

  ==============================================================================
*/

#include "OrangeBlueAttribute.h"

OrangeBlueAttribute::OrangeBlueAttribute(int w, int h) : HistogramAttribute("Orange Blue", w, h)
{
}

OrangeBlueAttribute::~OrangeBlueAttribute()
{
}

double OrangeBlueAttribute::evaluateScene(Snapshot * s)
{
  generateImage(s);

  // works on acceptable hue ranges:
  // blue: 170-245
  // orange: (350) -10-65
  Histogram1D hue = getHueHist(360);
  Histogram1D hue2 = hue.consolidate(2);

  int c1 = hue2.getNthBin(0);
  int c2 = hue2.getNthBin(1);

  // determine which one is closer to blue/orange
  int blue = closestToRange(c1, c2, 170, 245);
  int orange = (blue == c1) ? c2 : c1;

  float score = 0;

  // calculate objective scores
  if (170 <= blue && blue <= 245)
    score += 100;
  else {
    int diff = abs(blue - 170) < abs(blue - 245) ? abs(blue - 170) : abs(blue - 245);
    score += (100 - diff * 2);
  }

  if (orange >= 350)
    orange -= 360;

  if (-10 <= orange && orange <= 65)
    score += 100;
  else {
    int diff = abs(orange - 170) < abs(orange - 245) ? abs(orange - 170) : abs(orange - 245);
    score += (100 - diff * 2);
  }
  
  // color contrast score
  float cdiff = abs(blue - orange);

  // make sure sat and value are highish
  Histogram1D sat = getSatHist(20);
  Histogram1D val = getGrayscaleHist(20);

  score += sat.avg() * 50 + val.avg() * 50 + cdiff;
  score /= 4;

  return score;
}

unsigned int OrangeBlueAttribute::closestToRange(int x, int y, int min, int max)
{
  int x0 = x;
  int y0 = y;

  // check ranges
  if (min < 0) {
    if (x > (360 + min))
      x -= 360;

    if (y > (360 + min))
      y -= 360;
  }

  bool xinrange = (min <= x) && (x <= max);
  bool yinrange = (min <= y) && (y <= max);

  if (xinrange && yinrange) {
    int center = (max - min) / 2;
    
    return abs(x - center) < (abs(y - center)) ? x0 : y0;
  }

  if (xinrange)
    return x0;
  
  if (yinrange)
    return y0;

  int xminDist = abs(x - min) < (x - max) ? abs(x - min) : abs(x - max);
  int yminDist = abs(y - min) < (y - max) ? abs(y - min) : abs(y - max);

  return (xminDist < yminDist) ? x0 : y0;
}
