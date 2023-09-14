/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    int rows = (&theSource)->getRows();
    int cols = (&theSource)->getColumns();
    MosaicCanvas* mosaic = new MosaicCanvas(rows, cols);
    vector<Point<3>> tiles;
    for (auto it = theTiles.begin(); it != theTiles.end(); it++) {
        tiles.push_back(convertToXYZ(it->getAverageColor()));
    }
    KDTree<3> tree(tiles);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Point<3> p = tree.findNearestNeighbor(convertToXYZ((&theSource)->getRegionColor(i, j)));
            TileImage* t;
            for (int k = 0; k < (int) theTiles.size(); ++k) {
                if (((theTiles.at(k)).getAverageColor()).l == p[0] && ((theTiles.at(k)).getAverageColor()).u == p[1] && ((theTiles.at(k)).getAverageColor()).v == p[2]) {
                    t = &(theTiles.at(k));
                }
            }
            mosaic->setTile(i, j, t);
        }
    }
    return mosaic;
}

