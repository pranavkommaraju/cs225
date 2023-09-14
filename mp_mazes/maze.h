/* Your code here! */
#pragma once

#include "dsets.h"
#include "cs225/PNG.h"

using namespace cs225;

class SquareMaze {
    public:
        SquareMaze();	
        bool canTravel(int x, int y, int dir) const;
        void setWall(int x,int y, int dir, bool exists);
        PNG* drawMaze()	const;
        PNG* drawMazeWithSolution();	
        void makeMaze(int width_, int height_);
        std::vector<int> solveMaze();
        int determinedirection(std::pair<int, int> a, std::pair<int, int> b);
    private:
        struct walls {
            bool rightwall;
            bool downwall;
        };
        std::vector<walls> maze;
        int width;
        int height;
};
