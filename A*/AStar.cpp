#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <array>
#include <climits>
#include <math.h>
#include <cassert>
#include <fstream>

using namespace std;

#define CHAR_FILLED_BOX "\u2588"
#define CHAR_DOT "\u2022"

typedef array<int, 2> point;

enum mazeChar {
    wall,
    space,
    startMark,
    endMark,
    pathMark
};

double heuristic(point current, point goal) {
    //Our heuristic is distance-squared
    //It's really distance, but there's no reason to compute the square root, as
    //sqrt is bijective and strictly increasing
    return pow(goal[0]-current[0], 2) + pow(goal[1]-current[1], 2);
}
point getCurrentPoint(vector<point> openPoints, vector<vector<double> > f, int &index) {
    int bestIndex = 0;
    int fBest = f[openPoints[0][0]][openPoints[0][1]];
    int fCurrent;
    for(int i=1; i<int(openPoints.size()); ++i) {
        fCurrent = f[openPoints[i][0]][openPoints[i][1]];
        if(fCurrent < fBest) {
            bestIndex = i;
        }
    }
    index = bestIndex;
    return openPoints[bestIndex];
}
void constructPath(vector<point> &path, vector<vector<point> > prev, point current, point start) {
    vector<point> backPath;
    backPath.push_back(current);
    point lastPoint;

    //Trace back to start
    while(backPath[int(backPath.size())-1] != start) {
        lastPoint = prev[current[0]][current[1]];
        current = lastPoint;
        backPath.push_back(current);
    }

    //Flip it!
    for(int i=int(backPath.size()-1); i>=0; --i) {
        path.push_back(backPath[i]);
    }
}
vector<point> getNeighbors(point loc, vector<vector<bool> > maze) {
    vector<point> nbrs;
    point foo;
    array<int, 2> dim;
    dim[0] = int(maze.size());
    dim[1] = int(maze[0].size());

    //Load within boundaries
    if(loc[0] > 0) {
        foo[0] = loc[0]-1;
        foo[1] = loc[1];
        nbrs.push_back(foo);
    }
    if(loc[0] < dim[0]-1) {
        foo[0] = loc[0]+1;
        foo[1] = loc[1];
        nbrs.push_back(foo);
    }
    if(loc[1] > 0) {
        foo[0] = loc[0];
        foo[1] = loc[1]-1;
        nbrs.push_back(foo);
    }
    if(loc[1] < dim[1]-1) {
        foo[0] = loc[0];
        foo[1] = loc[1]+1;
        nbrs.push_back(foo);
    }

    //Remove walls
    for(int i=0; i<int(nbrs.size()); ++i) {
        if(maze[nbrs[i][0]][nbrs[i][1]] == true) {
            nbrs.erase(nbrs.begin() + i);
            --i;
        }
    }

    return nbrs;
}
bool aStar(vector<point> &path, point start, point end, vector<vector<bool> > maze) {
    vector<point> closed; //Points already searched
    vector<point> open {start}; //Points to be searched
    vector<vector<point> > prev; //For each point, the best point to reach it from
    vector<vector<int> > gScore; //g(n) values
    vector<vector<double> > fScore; //f(n) values

    point emptyPoint;

    //Populate gScore, fScore, and prev
    for(int i=0; i<int(maze.size()); ++i) {
        gScore.push_back(vector<int>()); //Empty vector
        fScore.push_back(vector<double>());
        prev.push_back(vector<point>());
        for(int j=0; j<int(maze[i].size()); ++j) {
            gScore[i].push_back(INT_MAX);
            fScore[i].push_back(INT_MAX);
            prev[i].push_back(emptyPoint);
        }
    }
    gScore[start[0]][start[1]] = 0; //The g(n) value for the start is 0.
    fScore[start[0]][start[1]] = heuristic(start, end);

    //Variables used in the loop (so memory doesn't have to be allocated with each loop)
    int currentPointIndex;
    point currentPoint;
    vector<point> neighbors;
    bool checked;
    bool newNeighbor;
    double tentativeGScore;
    bool pathFound = false;

    //Main A* Loop
    int iters = 0;
    while(open.size() != 0u) {
        ++iters;
        currentPoint = getCurrentPoint(open, fScore, currentPointIndex); //Open point with lowest fScore value.
        if(currentPoint == end) {
            pathFound = true;
            constructPath(path, prev, currentPoint, start);
            break;
        }
        open.erase(open.begin() + currentPointIndex);
        closed.push_back(currentPoint);

        neighbors = getNeighbors(currentPoint, maze);
        for(int i=0; i<int(neighbors.size()); ++i) {
            checked = false;
            for(int j=0; j<int(closed.size()); ++j) {
                if(neighbors[i] == closed[j]) {
                    checked = true;
                    break;
                }
            }
            if(checked) {
                continue;
            }

            newNeighbor = true;
            for(int j=0; j<int(open.size()); ++j) {
                if(neighbors[i] == open[j]) {
                    newNeighbor = false;
                    break;
                }
            }
            if(newNeighbor) {
                open.push_back(neighbors[i]);
            }

            tentativeGScore = gScore[currentPoint[0]][currentPoint[1]] + 1;
            if(tentativeGScore >= gScore[neighbors[i][0]][neighbors[i][1]]) {
                continue;
            }

            prev[neighbors[i][0]][neighbors[i][1]] = currentPoint;
            gScore[neighbors[i][0]][neighbors[i][1]] = tentativeGScore;
            fScore[neighbors[i][0]][neighbors[i][1]] = tentativeGScore + heuristic(neighbors[i], end);
        }
    }

    cout << "Total Iterations: " << iters << endl;

    return pathFound;
}
vector<string> split(string str, string del) {
    vector<string> res;
    size_t pos = str.find(del);
    while(pos != string::npos) {
        string val = str.substr(0, pos);
        res.push_back(val);
        str.erase(0, pos + del.size());

        pos = str.find(del);
    }
    res.push_back(str);
    return res;
}
void printPath(vector<point> solution) {
    for(int i=0; i<int(solution.size()); ++i) {
        cout << "(" << solution[i][0] << "," << solution[i][1] << ")" << endl;
    }
}
void printMazeCharMatrix(vector<vector<mazeChar> > mat) {
    for(int i=-1; i<int(mat[0].size())+1; ++i) {
        cout << CHAR_FILLED_BOX;
    }
    cout << endl;
    for(int i=0; i<int(mat.size()); ++i) {
        cout << CHAR_FILLED_BOX;
        for(int j=0; j<int(mat[i].size()); ++j) {
            switch(mat[i][j]) {
                case wall:
                    cout << CHAR_FILLED_BOX;
                    break;
                case space:
                    cout << " ";
                    break;
                case startMark:
                    cout << "+";
                    break;
                case endMark:
                    cout << "x";
                    break;
                case pathMark:
                    cout << CHAR_DOT;
                    break;
                default:
                    cout << "?";
                    break;
            }
        }
        cout << CHAR_FILLED_BOX << endl;
    }
    for(int i=-1; i<int(mat[0].size())+1; ++i) {
        cout << CHAR_FILLED_BOX;
    }
    cout << endl;
}
void printMaze(vector<vector<bool> > maze, point start, point end) {
    vector<vector<mazeChar> > output;
    for(int i=0; i<int(maze.size()); ++i) {
        output.push_back(vector<mazeChar>());
        for(int j=0; j<int(maze[i].size()); ++j) {
            output[i].push_back(maze[i][j] ? wall : space);
        }
    }

    output[start[0]][start[1]] = startMark;
    output[end[0]][end[1]] = endMark;

    printMazeCharMatrix(output);
}
void printSolvedMaze(vector<vector<bool> > maze, vector<point> solution) {
    vector<vector<mazeChar> > output;
    for(int i=0; i<int(maze.size()); ++i) {
        output.push_back(vector<mazeChar>());
        for(int j=0; j<int(maze[i].size()); ++j) {
            output[i].push_back(maze[i][j] ? wall : space);
        }
    }

    int m,n;
    m = solution[0][0];
    n = solution[0][1];
    output[m][n] = startMark;

    m = solution[int(solution.size())-1][0];
    n = solution[int(solution.size())-1][1];
    output[m][n] = endMark;

    for(int i=1; i<int(solution.size())-1; ++i) {
        m = solution[i][0];
        n = solution[i][1];
        output[m][n] = pathMark;
    }

    printMazeCharMatrix(output);
}
void loadMaze(string filename, vector<vector<bool> > &maze, point &start, point &end) {
    ifstream in(filename.c_str());
    string line;
    string delimiter = ",";
    
    //Start Point
    getline(in, line);
    vector<string> startPointRaw = split(line, delimiter);
    start[0] = stoi(startPointRaw[0]);
    start[1] = stoi(startPointRaw[1]);

    getline(in, line);
    vector<string> endPointRaw = split(line, delimiter);
    end[0] = stoi(endPointRaw[0]);
    end[1] = stoi(endPointRaw[1]);

    getline(in, line);
    while(!in.fail()) {
        vector<string> lineRaw = split(line, delimiter);
        maze.push_back(vector<bool>());
        for(int i=0; i<int(lineRaw.size()); ++i) {
            maze[int(maze.size())-1].push_back(lineRaw[i] == "1");
        }
        getline(in, line);
    }

    in.close();
}
int main() {
    ifstream getFName("whichMaze.txt");
    string filename;
    getFName >> filename;
    getFName.close();

    vector<vector<bool> > maze;
    point start;
    point end;
    loadMaze(filename, maze, start, end);

    cout << endl;
    cout << "Maze:" << endl;
    printMaze(maze, start, end);
    cout << "+ is start, x is end." << endl;
    cout << endl;

    vector<point> solution;

    bool success = aStar(solution, start, end, maze);
    cout << endl;

    if(success) {
        cout << "Path:" << endl;
        printPath(solution);
        cout << endl;
        cout << "Solved maze:" << endl;
        printSolvedMaze(maze, solution);
        cout << "+ is start, x is end." << endl;
    }
    else {
        cout << "Not solvable!" << endl;
    }
    cout << endl;

    return 0;
}