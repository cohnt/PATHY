/*
Author: Victor Hao & Noah Koorndyk & Roberto Jimenez
Date Created: 12/6/2017
Last Update: 12/10/17 12:57 PM
Lab Section: 103
Project #: Team Hack
Filename: dijkstra.cpp

Program Description: Implementation of Dijkstra's pathfinding algorithm for a maze.
A slight twist on classic dijkstra's since each pixel move has the same weight of 1.
First we crawl the entire maze and assign their respective cost values until the
end is found. Then, we can read the path back from the end by simply choosing an
adjacent move which has the lowest value of the four possible moves. Returned
finalPath corresponds exactly with pixel coords in image (does not start at 0 due to
nature of padding).

NOTES: This could probably be done much more efficiently with a 1D array. When this
code is converted to C# that is what I recommend be done if possible (although not
necessary).

MUST BE COMPILED IN C++ 11 or above.
*/

#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <climits>

using namespace std;

//Pads the maze boundary with zeros
void padMaze(vector<vector<int> > & input) {

	//Create padded as a vector of vectors of size [input rows+2][input cols+2]
	//filled with -1. This prevents accessing invalid positions.
	vector<vector<int> > padded(input.size() + 2, vector<int>(input.at(0).size() + 2, -1));

	//Create a new grid that has walls represented with -1 instead of 1
	vector<vector<int> > modGrid = input;
	for (unsigned int i = 0; i < modGrid.size(); i++) {
		for (unsigned int j = 0; j < modGrid.at(0).size(); j++) {
			if (input.at(i).at(j) == 1) {
				modGrid.at(i).at(j) = -1;
			}
		}
	}

	//Assign inside of padded to modGrid
	for (unsigned int i = 0; i < modGrid.size(); i++) {
		for (unsigned int j = 0; j < modGrid.at(0).size(); j++) {
			padded.at(i + 1).at(j + 1) = modGrid.at(i).at(j);
		}
	}

	//Set input to padded
	input = padded;
}

//Solves the maze
vector<vector<int> >  mazeSolve(vector<vector<int> > grid, vector<int> start, vector<int> end) {

	//Initialize Variables
	bool notSolved = true, changedVals = false, home = false;
	//curSweep should look for start in the first iteration
	int curSweep = 1;
	vector<vector<int> > crawled;
	vector<vector<int> > finalPath;

	//Pad the outside of the maze with a wall (stops index out of bounds)
	padMaze(grid);
	//Sets Start to 1 and end to -2
	grid.at(start.at(0)).at(start.at(1)) = 1;
	grid.at(end.at(0)).at(end.at(1)) = -2;

	//Loop until the maze is solved
	while (notSolved) {
		changedVals = false;
		//Loop through all positions in the image
		for (unsigned int i = 1; i < grid.size() - 1; i++) {
			for (unsigned int j = 1; j < grid.at(0).size() - 1; j++) {
				//Check if the value at the location is what we are sweeping for
				if (grid.at(i).at(j) == curSweep) {
					//If above value is open, change to curSweep + 1
					if (grid.at(i - 1).at(j) == 0) {
						grid.at(i - 1).at(j) = curSweep + 1;
						//Change changedVals to true
						changedVals = true;
					}

					//Same operation for below
					if (grid.at(i + 1).at(j) == 0) {
						grid.at(i + 1).at(j) = curSweep + 1;
						changedVals = true;
					}

					//Same operation for left
					if (grid.at(i).at(j - 1) == 0) {
						grid.at(i).at(j - 1) = curSweep + 1;
						changedVals = true;
					}

					//Same operation for right
					if (grid.at(i).at(j + 1) == 0) {
						grid.at(i).at(j + 1) = curSweep + 1;
						changedVals = true;
					}

					//Check if any of the adjacent values are the goal
					if (grid.at(i - 1).at(j) == -2 || grid.at(i + 1).at(j) == -2 || grid.at(i).at(j - 1) == -2 || grid.at(i).at(j + 1) == -2) {
						notSolved = false;
						break;
					}
				}
				//Break out of the outer loop if solved
				if (notSolved == false) {
					break;
				}
			}
		}
		//If we didn't change any values and didn't find the end, we failed (break)
		if (!changedVals) {
			cout << "Oh no.";
			return finalPath;
		}
		//Increment curSweep
		curSweep++;
	}

	//Initialize some variables
	finalPath.push_back(end);
	vector<int> curPos = end;
	vector<int> nextPos(2);
	int curVal = INT_MAX, temp;
	grid.at(end.at(0)).at(end.at(1)) = INT_MAX;
	//This loop traces the shortest path by choosing positions with the lowest value
	while (!home) {

		//Set temp to above value
		temp = grid.at(curPos.at(0) - 1).at(curPos.at(1));
		//If temp is larger than 0 and it is less than curVal, set nextPos to above
		//and set curVal to temp
		if (temp > 0 && temp < curVal) {
			nextPos.at(0) = curPos.at(0) - 1;
			nextPos.at(1) = curPos.at(1);
			curVal = temp;
		}

		//Same operations done below for bottom, left, and right

		temp = grid.at(curPos.at(0) + 1).at(curPos.at(1));
		if (temp > 0 && temp < curVal) {
			nextPos.at(0) = curPos.at(0) + 1;
			nextPos.at(1) = curPos.at(1);
			curVal = temp;
		}

		temp = grid.at(curPos.at(0)).at(curPos.at(1) - 1);
		if (temp > 0 && temp < curVal) {
			nextPos.at(0) = curPos.at(0);
			nextPos.at(1) = curPos.at(1) - 1;
			curVal = temp;
		}

		temp = grid.at(curPos.at(0)).at(curPos.at(1) + 1);
		if (temp > 0 && temp < curVal) {
			nextPos.at(0) = curPos.at(0);
			nextPos.at(1) = curPos.at(1) + 1;
			curVal = temp;
		}

		//If we have found the start, set home to true and add start to the path
		if (curVal == 1) {
			home = true;
			finalPath.push_back(start);
		}
		//Otherwise add nextPos to the path and set curPos to nextPos
		else {
			finalPath.push_back(nextPos);
			curPos = nextPos;
		}
	}

	//Returns the solved path
	return finalPath;
}

//Temporary test setup
//The following works with the basictestmaze.txt on github
int main() {
	//Initialize Vars
	ifstream mazeRead("insert file path here"); //careful with escape chars
	vector<vector<int> > grid;
	//Choose the start and end
	vector<int> start = { 2,2 };
	vector<int> end = { 4,13 };
	//Text File read is only meant for testing, so must specify exact size of maze
	int numRows = 7;
	int numCols = 14;

	//Initialize temporary vars
	int temp;
	vector<int> tempVec;
	//Reads in the maze
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			mazeRead >> temp;
			tempVec.push_back(temp);
		}
		grid.push_back(tempVec);
		tempVec.clear();
	}

	//Get the shortest path by calling mazeSolve()
	vector<vector<int> > path = mazeSolve(grid, start, end);

	//Prints Path
	for (unsigned int i = 0; i < path.size(); i++) {
		cout << path.at(i).at(0) << " " << path.at(i).at(1) << endl;
	}

	//Close the file
	mazeRead.close();
	return 0;
}
