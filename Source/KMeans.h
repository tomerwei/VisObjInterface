/*
  ==============================================================================

    KMeans.h
    Created: 14 Jun 2016 11:27:12am
    Author:  eshimizu

  ==============================================================================
*/

#ifndef KMEANS_H_INCLUDED
#define KMEANS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "globals.h"
#include "SearchResultContainer.h"
#include "TopLevelCluster.h"

typedef function<double(SearchResultContainer*, SearchResultContainer*)> distFuncType;
typedef function<double(Eigen::VectorXd, Eigen::VectorXd)> gdistFuncType;

enum InitMode {
  FORGY,    // Forgy init method: randomly select k points as mean
  RND_PART  // Randomly assign each point to a center k
};

// An implementation of K-means for clustering search results
class KMeans {
public:
  KMeans();
  KMeans(distFuncType distFunc);

  // cluster, let alg determine starting center points
  Array<shared_ptr<TopLevelCluster> > cluster(int k, Array<shared_ptr<SearchResultContainer> >& points, InitMode init, bool addToCenters = true);
  Array<shared_ptr<SearchResultContainer> > clusterBestCenters(int k, Array<shared_ptr<SearchResultContainer> >& points);
  Array<shared_ptr<SearchResultContainer> > clusterBestRepCenters(int k, Array<shared_ptr<SearchResultContainer> >& points, function<double(shared_ptr<SearchResultContainer>)> repTerm);

  // cluster with a preset set of center points. k = centers.size()
  Array<shared_ptr<TopLevelCluster> > cluster(Array<shared_ptr<SearchResultContainer> >& points, Array<shared_ptr<TopLevelCluster> >& centers, bool addToCenters = true);

  Array<shared_ptr<SearchResultContainer> > clusterBestCenters(Array<shared_ptr<SearchResultContainer> >& points, Array<shared_ptr<SearchResultContainer> >& centers);
  Array<shared_ptr<SearchResultContainer> > clusterBestRepCenters(Array<shared_ptr<SearchResultContainer> >& points, Array<shared_ptr<SearchResultContainer> >& centers, function<double(shared_ptr<SearchResultContainer>)> repTerm);

  // top-down hierarchical clustering
  Array<shared_ptr<TopLevelCluster> > divisive(int maxK, Array<shared_ptr<SearchResultContainer> >& points);

  // top-down hierarchical clustering with a threshold as the stop criteria
  Array<shared_ptr<TopLevelCluster> > divisive(double t, Array<shared_ptr<SearchResultContainer> >& points);

private:
  Array<shared_ptr<TopLevelCluster> > forgy(int k, Array<shared_ptr<SearchResultContainer> >& points);
  Array<shared_ptr<TopLevelCluster> > rndpart(int k, Array<shared_ptr<SearchResultContainer> >& points);

  Array<shared_ptr<SearchResultContainer> > forgy2(int k, Array<shared_ptr<SearchResultContainer> >& points);

  // Returns the closest center to the specified point
  int closestCenter(shared_ptr<SearchResultContainer> point, Array<shared_ptr<TopLevelCluster> >& centers);
  int closestCenter2(shared_ptr<SearchResultContainer> point, Array<shared_ptr<SearchResultContainer> >& centers);

  distFuncType _distFunc;
};

// Operates on generic vectors instead of attribute search results
// It's almost the same, except the vectors don't get assigned as children to the centers
class GenericKMeans {
public:
  GenericKMeans();
  GenericKMeans(gdistFuncType distFunc);
  
  vector<Eigen::VectorXd> cluster(int k, vector<pair<Eigen::VectorXd, int> >& points, InitMode init);

private:
  vector<Eigen::VectorXd> forgy(int k, vector<pair<Eigen::VectorXd, int> >& points);
  vector<Eigen::VectorXd> rndpart(int k, vector<pair<Eigen::VectorXd, int> >& points);

  // Returns the closest center to the specified point
  int closestCenter(Eigen::VectorXd point, vector<Eigen::VectorXd>& centers);

  gdistFuncType _distFunc;
};

#endif  // KMEANS_H_INCLUDED
