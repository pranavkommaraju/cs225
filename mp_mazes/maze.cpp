/* Your code here! */
#include "maze.h"
#include <random>
#include <queue>
#include <algorithm>


SquareMaze::SquareMaze() {}	

bool SquareMaze::canTravel(int x, int y, int dir) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    if (dir == 0 && x + 1 < width) {
        if (!maze.at(y * height + x).rightwall) {
            return true;
        }
    } else if (dir == 1 && y + 1 < height) {
        if (!maze.at(y * height + x).downwall) {
            return true;
        }
    } else if (dir == 2 && x - 1 >= 0) {
        if (!maze.at(y * height + (x-1)).rightwall) {
            return true;
        }
    } else if (dir == 3 && y - 1 >= 0) {
        if (!maze.at((y-1) * height + x).downwall) {
            return true;
        }
    }
    return false;
}

void SquareMaze::setWall(int x,int y, int dir, bool exists) {
    if (dir == 0) {
        maze.at(y * height + x).rightwall = exists;
    } else {
        maze.at(y * height + x).downwall = exists;
    }
}

PNG* SquareMaze::drawMaze()	const {
    int pngwidth = width * 10 + 1;
    int pngheight = height * 10 + 1;
    PNG* mazepic = new PNG(pngwidth, pngheight);
    for (int x = 10; x < pngwidth; ++x) {
        mazepic->getPixel(x, 0).l = 0;
    }
    for (int y = 0; y < pngheight; ++y) {
        mazepic->getPixel(0, y).l = 0;
    }
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            if (maze.at(y * height + x).rightwall) {
                for (int k = 0; k < 11; ++k) {
                    mazepic->getPixel((x + 1) * 10, y * 10 + k).l = 0;
                }
            }
            if (maze.at(y * height + x).downwall) {
                for (int k = 0; k < 11; ++k) {
                    mazepic->getPixel(x * 10 + k, (y + 1) * 10).l = 0;
                }
            }
        }
    }
    return mazepic;
}

PNG* SquareMaze::drawMazeWithSolution() {
    PNG* solmazepic = drawMaze();
    std::vector<int> steps = solveMaze();
    int x = 5; 
    int y = 5;
    for (unsigned i = 0; i < steps.size(); ++i) {
        if (steps.at(i) == 0) {
            for (int j = 0; j < 11; ++j) {
                solmazepic->getPixel(x, y).s = 1;
                solmazepic->getPixel(x, y).l = 0.5;
                x++;
            }
            x--;
        } else if (steps.at(i) == 1) {
            for (int j = 0; j < 11; ++j) {
                solmazepic->getPixel(x, y).s = 1;
                solmazepic->getPixel(x, y).l = 0.5;
                y++;
            }
            y--;
        } else if (steps.at(i) == 2) {
            for (int j = 0; j < 11; ++j) {
                solmazepic->getPixel(x, y).s = 1;
                solmazepic->getPixel(x, y).l = 0.5;
                x--;
            }
            x++;
        } else if (steps.at(i) == 3) {
            for (int j = 0; j < 11; ++j) {
                solmazepic->getPixel(x, y).s = 1;
                solmazepic->getPixel(x, y).l = 0.5;
                y--;
            }
            y++;
        }
    }
    x -= 5;
    y += 5;
    for (int k = 1; k < 10; ++k) {
        solmazepic->getPixel(x + k, y).l = 1;
    }
    return solmazepic;
}

void SquareMaze::makeMaze(int width_, int height_) {
    width = width_;
    height = height_;
    walls* w = new walls();
    w->rightwall = true;
    w->downwall = true;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            walls w_copy = *w;
            maze.push_back(w_copy);
        }
    }
    DisjointSets* sets = new DisjointSets();
    sets->addelements(width * height);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> distw(0, width - 1);
    std::uniform_int_distribution<int> disth(0, height - 1);
    std::uniform_int_distribution<int> distdir(0, 1);
    while (sets->size(0) != width * height) {
        int a = disth(mt);
        int b = distw(mt);
        int dir = distdir(mt);
        if ((a == height - 1 && dir == 1) || (b == width - 1 && dir == 0)) {
            continue;
        }
        int idx = a * height + b;
        if (dir == 0) {
            if (sets->find(idx) == sets->find(idx + 1)) {
                continue;
            }
            sets->setunion(idx, idx + 1);
            maze.at(idx).rightwall = false;
        } else {
            if (sets->find(idx) == sets->find(idx + height)) {
                continue;
            }
            sets->setunion(idx, idx + height);
            maze.at(idx).downwall = false;
        }
    }
}

std::vector<int> SquareMaze::solveMaze() {
    std::vector<std::vector<bool>> visited(width, std::vector<bool> (height, false));
    std::queue<std::pair<int, int>> q;
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> paths;
    q.push(std::make_pair(0, 0));
    while (!q.empty()) {
        std::pair<int, int> v = q.front();
        q.pop();
        if (visited.at(v.first).at(v.second)) {
            continue;
        }
        visited.at(v.first).at(v.second) = true;
        if (canTravel(v.first, v.second, 0)) {  // right
            if (!visited.at(v.first + 1).at(v.second)) {
                q.push(std::make_pair(v.first + 1, v.second));
                paths.push_back(std::make_pair(std::make_pair(v.first, v.second), std::make_pair(v.first + 1, v.second)));
            }
        }
        if (canTravel(v.first, v.second, 1)) {  // down
            if (!visited.at(v.first).at(v.second + 1)) {
                q.push(std::make_pair(v.first, v.second + 1));
                paths.push_back(std::make_pair(std::make_pair(v.first, v.second), std::make_pair(v.first, v.second + 1)));
            }
        }
        if (canTravel(v.first, v.second, 2)) {  // left
            if (!visited.at(v.first - 1).at(v.second)) {
                q.push(std::make_pair(v.first - 1, v.second));
                paths.push_back(std::make_pair(std::make_pair(v.first, v.second), std::make_pair(v.first - 1, v.second)));
            }
        }
        if (canTravel(v.first, v.second, 3)) {  // up
            if (!visited.at(v.first).at(v.second - 1)) {
                q.push(std::make_pair(v.first, v.second - 1));
                paths.push_back(std::make_pair(std::make_pair(v.first, v.second), std::make_pair(v.first, v.second - 1)));
            }
        }
    }
    std::vector<int> steps;
    std::pair<int, int> laststep;
    for (int i = paths.size()-1; i >= 0; i--) {
        if (paths.at(i).second.second == height - 1) {
            steps.push_back(determinedirection(paths.at(i).first, paths.at(i).second));
            laststep = paths.at(i).first;
            break;
        }
    }
    for (int i = paths.size()-1; i >= 0; i--) {
        if (paths.at(i).second == laststep) {
            steps.push_back(determinedirection(paths.at(i).first, paths.at(i).second));
            laststep = paths.at(i).first;
        }
    }
    std::reverse(steps.begin(), steps.end());
    return steps;
}

int SquareMaze::determinedirection(std::pair<int, int> a, std::pair<int, int> b) {
    if (a.first == b.first) {
        if (a.second + 1 == b.second) {
            return 1;
        } else {
            return 3;
        }
    } else {
        if (a.first + 1 == b.first) {
            return 0;
        } else {
            return 2;
        }
    }
}

