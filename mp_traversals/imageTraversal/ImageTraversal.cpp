#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

#include <cassert>

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator() {
  /** @todo [Part 1] */
  traversal = NULL;
  png = NULL;
}

ImageTraversal::Iterator::Iterator(ImageTraversal* traversal_, const Point & start_, double tolerance_, PNG* png_) {
  traversal = traversal_;
  start = start_;
  curr = start;
  tolerance = tolerance_;
  png = png_;
  visited.resize(png->width());
  for (unsigned i = 0; i < png->width(); ++i) {
    visited.at(i).resize(png->height());
  }
  for (unsigned i = 0; i < png->width(); i++) {
    for (unsigned j = 0; j < png->height(); j++) {
      visited.at(i).at(j) = false;
    }
  }
}

/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  /** @todo [Part 1] */
  Point point;
  curr = traversal->pop();
  visited.at(curr.x).at(curr.y) = true;
  if (curr.x != png->width()-1) {
    Point curr_right = curr;
    curr_right.x += 1;
    traversal->add(curr_right);
  }
  if (curr.y != png->height()-1) {
    Point curr_down = curr;
    curr_down.y += 1;
    traversal->add(curr_down);
  }
  if (curr.x != 0) {
    Point curr_left = curr;
    curr_left.x -= 1;
    traversal->add(curr_left);
  }
  if (curr.y != 0) {
    Point curr_up = curr;
    curr_up.y -= 1;
    traversal->add(curr_up);
  }
  while (!(traversal->empty())) {
    point = traversal->peek();
    if (!(visited.at(point.x).at(point.y)) && (calculateDelta(png->getPixel(start.x, start.y), png->getPixel(point.x, point.y)) < tolerance)) {
      if (!(traversal->empty())) {
        curr = point;
        visited.at(curr.x).at(curr.y) = true;
        break;
      }
    }
    traversal->pop();
  }
  return *this;
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return curr;
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
  if (traversal != NULL && traversal->empty() == false) {
    return true;
  }
  return false;
}

