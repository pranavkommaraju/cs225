/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
    if (first[curDim] > second[curDim]) {
      return false;
    } else {
      if (first[curDim] == second[curDim]) {
        return first.operator<(second);
      }
      return true;
    }
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
    double currbestdist = 0;
    double potentialdist = 0;
    for (size_t i = 0; i < Dim; ++i) {
      currbestdist += pow((currentBest[i] - target[i]), 2);
      potentialdist += pow((potential[i] - target[i]), 2);
    }
    if (potentialdist > currbestdist) {
      return false;
    } else {
      if (potentialdist == currbestdist) {
        return potential.operator<(currentBest);
      }
      return true;
    }
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    root = new KDTreeNode();
    size = newPoints.size();
    vector<Point<Dim>> points_copy;
    std::copy(newPoints.begin(), newPoints.end(), back_inserter(points_copy));
    size = newPoints.size();
    buildtree(points_copy, 0, size - 1, root, 0);
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */


}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */

  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    return (findNearest(query, 0, root))->point;
}

template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& points, int left, int right, int pivotidx, int dim) {
  Point pivotval = points.at(pivotidx);
  std::swap(points.at(pivotidx), points.at(right));
  int storeidx = left;
  for (int i = left; i < right; ++i) {
    if (smallerDimVal(points.at(i), pivotval, dim)) {
      std::swap(points.at(storeidx), points.at(i));
      storeidx++;
    }
  }
  std::swap(points.at(right), points.at(storeidx));
  return storeidx;
}

template <int Dim>
int KDTree<Dim>::quickselect(vector<Point<Dim>>& points, int left, int right, int k, int dim) {
  if (left == right) {
    return left;
  }
  int pivotidx = left + (rand() % (right - left + 1));
  pivotidx = partition(points, left, right, pivotidx, dim);
  if (k == pivotidx) {
    return k;
  } else if (k < pivotidx) {
    return quickselect(points, left, pivotidx-1, k, dim);
  } else {
    return quickselect(points, pivotidx+1, right, k, dim);
  }
}

template <int Dim>
void KDTree<Dim>::buildtree(vector<Point<Dim>>& points, int left, int right, KDTreeNode* curr, int dim) {
  if (left <= right) {
    int middle = quickselect(points, left, right, (left + right) / 2, dim);
    curr->point = points.at(middle);
    if (middle != right) {
      KDTreeNode* currright = new KDTreeNode();
      curr->right = currright;
      buildtree(points, middle + 1, right, currright, (dim + 1) % Dim);
    }
    if (middle != left) {
      KDTreeNode* currleft = new KDTreeNode();
      curr->left = currleft;
      buildtree(points, left, middle - 1, currleft, (dim + 1) % Dim);
    }
  }
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::findNearest(const Point<Dim>& query, int dim, KDTreeNode* curr) const {
  KDTreeNode* nearest = root;
  KDTreeNode* tempNearest = root;
  if (curr->left == NULL && curr->right == NULL) {
    return curr;
  }
  if ((smallerDimVal(query, curr->point, dim) && curr->left != NULL) || curr->right == NULL) {
    nearest = findNearest(query, (dim + 1) % Dim, curr->left);
  } else if (curr->right != NULL) {
    nearest = findNearest(query, (dim + 1) % Dim, curr->right);
  }
  if (shouldReplace(query, nearest->point, curr->point)) {
    nearest = curr;
  }
  double radius = 0;
  for (int i = 0; i < Dim; ++i) {
    radius += pow((query[i] - (nearest->point)[i]), 2);
  }
  double splitdist = pow(((curr->point)[dim] - query[dim]), 2);
  if (radius >= splitdist) {
    if (smallerDimVal(query, curr->point, dim)) {
      if (curr->right != NULL) {
        tempNearest = findNearest(query, (dim + 1) % Dim, curr->right);
      }
    } else {
      if (curr->left != NULL) {
        tempNearest = findNearest(query, (dim + 1) % Dim, curr->left);
      }
    }
    if (tempNearest != NULL) {
      if (shouldReplace(query, nearest->point, tempNearest->point)) {
        nearest = tempNearest;
      }
    }
  }
  return nearest;
}
