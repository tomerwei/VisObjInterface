/*
  ==============================================================================

    AttributeSorting.h
    Created: 16 Feb 2016 1:41:37pm
    Author:  falindrith

  ==============================================================================
*/

#ifndef ATTRIBUTESORTING_H_INCLUDED
#define ATTRIBUTESORTING_H_INCLUDED

#include "SearchResultContainer.h"

class SearchResultContainer;

class AttributeSorter
{
public:
  AttributeSorter() { }
  ~AttributeSorter() { }
   
  virtual int compareElements(shared_ptr<SearchResultContainer> first, shared_ptr<SearchResultContainer> second) = 0;
};

class DefaultSorter : public AttributeSorter
{
public:
  DefaultSorter() { }
  ~DefaultSorter() { }

  virtual int compareElements(shared_ptr<SearchResultContainer> first, shared_ptr<SearchResultContainer> second);
};

class AvgHueSorter : public AttributeSorter
{
public:
  AvgHueSorter() { }
  ~AvgHueSorter() { }

  virtual int compareElements(shared_ptr<SearchResultContainer> first, shared_ptr<SearchResultContainer> second);
};

class AvgBrightSorter : public AttributeSorter
{
public:
  AvgBrightSorter() { }
  ~AvgBrightSorter() { }

  virtual int compareElements(shared_ptr<SearchResultContainer> first, shared_ptr<SearchResultContainer> second);
};

class StyleSorter : public AttributeSorter
{
public:
  StyleSorter(Style s) : _s(s) { }
  ~StyleSorter() {}

  virtual int compareElements(shared_ptr<SearchResultContainer> first, shared_ptr<SearchResultContainer> second);

  Style _s;
};

class CacheSorter : public AttributeSorter
{
public:
  CacheSorter(string s) : _s(s) { }

  virtual int compareElements(shared_ptr<SearchResultContainer> first, shared_ptr<SearchResultContainer> second);
 
  string _s;
};

class ExtraSorter : public AttributeSorter
{
public:
  ExtraSorter(String s) : _key(s) {}

  virtual int compareElements(shared_ptr<SearchResultContainer> first, shared_ptr<SearchResultContainer> second);

  String _key;
};

#endif  // ATTRIBUTESORTING_H_INCLUDED
