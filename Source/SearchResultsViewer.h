/*
  ==============================================================================

    SearchResultsViewer.h
    Created: 15 Dec 2015 5:07:58pm
    Author:  falindrith

  ==============================================================================
*/

#ifndef SEARCHRESULTSVIEWER_H_INCLUDED
#define SEARCHRESULTSVIEWER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "globals.h"
#include "AttributeSearch.h"
#include "AttributeSearchResult.h"
#include "HistoryPanel.h"
#include "AttributeSearchCluster.h"
#include <mutex>

class SearchResultsRenderer : public ThreadWithProgressWindow
{
public:
  SearchResultsRenderer(Array<AttributeSearchResult*> results);
  ~SearchResultsRenderer();

  void run() override;
  void threadComplete(bool userPressedCancel) override;

private:
  Array<AttributeSearchResult*> _results;
};

//===============================================================================

class SearchResultsContainer : public Component
{
public:
  SearchResultsContainer();
  ~SearchResultsContainer();

  void paint(Graphics& g);
  void resized();

  // Recluster results
  void recluster();

  // Update the width of the component
  void setHeight(int height);

  void markDisplayedCluster(AttributeSearchResult* c);

  // Return the results for some other use
  Array<AttributeSearchResult*> getResults();

  // Add a new search result to the display area. Is thread safe.
  bool addNewResult(SearchResult* r);

  // integrate new results and display
  void showNewResults();

  // Deletes all scenes currently contained in the viewer.
  void clear();

  // Returns true when the container can't support any more elements
  bool isFull();

private:
  int _width;
  int _height;

  bool _isRoot;

  mutex _resultsLock;
  int _numResults;

  // queue of results to add to the search.
  Array<AttributeSearchResult*> _newResults;

  // results components
  Array<AttributeSearchResult*> _results;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SearchResultsContainer)
};

//==============================================================================
/*
*/
class SearchResultsViewer : public Component
{
public:
  SearchResultsViewer();
  ~SearchResultsViewer();

  void paint (Graphics&);
  void resized();

  void redisplay();
  void sortDisplayedCluster();

  void setBotComponent(Component* c, Component* source);

  Array<AttributeSearchResult*> getResults();

  HistoryPanel* getHistory() { return _history; }

  // adds a new search result to the display area. Thread safe.
  bool addNewResult(SearchResult* r);

  // tells the search result container to update itself
  void showNewResults();

  // Deletes all scenes currently displayed and contained by the viewer.
  void clearContainer();

  // Returns true when the viewer can't display any more scenes
  bool isFull();

private:
  Viewport* _viewer;
  Viewport* _detailViewer;
  Viewport* _historyViewer;
  SearchResultsContainer* _container;
  AttributeSearchCluster* _displayedCluster;
  HistoryPanel* _history;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SearchResultsViewer)
};


#endif  // SEARCHRESULTSVIEWER_H_INCLUDED
