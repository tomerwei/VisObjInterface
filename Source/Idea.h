/*
  ==============================================================================

    Idea.h
    Created: 23 Nov 2016 2:19:45pm
    Author:  falindrith

  ==============================================================================
*/

#ifndef IDEA_H_INCLUDED
#define IDEA_H_INCLUDED

#include "globals.h"
#include "../JuceLibraryCode/JuceHeader.h"

enum IdeaType {
  COLOR_PALETTE = 1
};

class IdeaList;

// the Idea class contains information to apply an idea to a particular
// set of devices on the stage
class Idea : public Component, public ComboBoxListener {
public:
  Idea(Image src, IdeaType type = COLOR_PALETTE);
  ~Idea();

  void paint(Graphics& g) override;
  void resized() override;
 
  void mouseDown(const MouseEvent& e) override;
  void mouseDrag(const MouseEvent& e) override;
  void mouseUp(const MouseEvent& e) override;

  void comboBoxChanged(ComboBox* b) override;

  // indicates if this element is selected
  bool _selected;

private:
  // source image
  Image _src;

  // idea type
  IdeaType _type;

  // graphical elements
  ComboBox _typeSelector;

  // bbox representing the area of the image the idea comes from
  Rectangle<float> _focusArea;

  // variables for rectangle area selection
  // the active points are stored in absolute coordinates for ease of drawing.
  Point<float> _firstPt;
  Point<float> _secondPt;
  bool _isBeingDragged;

  // takes the current type and updates necesssary data for the Idea to function
  void updateType();

  // takes a point within this element and returns a location within the source image.
  // If out of bounds, clamps to [0,1]
  Point<float> localToRelativeImageCoords(Point<float> pt);
  Point<float> relativeImageCoordsToLocal(Point<float> pt);
};

class IdeaList : public Component {
public:
  IdeaList();
  ~IdeaList();

  void paint(Graphics& g) override;
  void resized() override;

  shared_ptr<Idea> getActiveIdea();

  void clearActiveIdea();
  void updateActiveIdea();

  // adds an idea to the list.
  void addIdea(Image i);

private:
  // list of ideas contained
  vector<shared_ptr<Idea> > _ideas;

  // current active idea
  int _activeIdea;
};


#endif  // IDEA_H_INCLUDED