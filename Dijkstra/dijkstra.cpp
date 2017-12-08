/*
Author: Victor Hao & Noah Koorndyk & Roberto Jimenez
Uniqname: vhao
Date Created: 12/6/2017
Lab Section: 103
Project #: Team Hack
Filename: dijkstra.cpp

Program Description: Implementation of Dijkstra's pathfinding algorithm for a maze.
NOTES: This could probably be done much more efficiently with a 1D array. When this
code is converted to C# that is what I recommend be done.
MUST BE COMPILED IN C++ 11
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
	//filled with -1.
	vector<vector<int> > padded(input.size() + 2, vector<int>(input.at(0).size(), -1));

	//Assign inside of padded to input
	for (unsigned int i = 0; i < input.size(); i++) {
		for (unsigned int j = 0; j < input.at(0).size(); j++) {
			padded.at(i + 1).at(j + 1) = input.at(i).at(j);
		}
	}

	//Set input to padded
	input = padded;
}

//Solves the maze
vector<vector<int> >  mazeSolve(vector<vector<int> > grid, vector<int> start, vector<int> end) {

	//Initialize Variables
	bool notSolved = false, changedVals = false, home = false;
	int curSweep = 1;
	vector<vector<int> > crawled;
	vector<vector<int> > finalPath;
	finalPath.push_back(end);

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
			}
		}
		//If we didn't change any values and didn't find the end, we failed (break)
		if (!changedVals) {
			cout << "Oh no.";
			break;
		}
		//Increment curSweep
		curSweep++;
	}

	//Initialize some variables
	vector<int> curPos = end;
	vector<int> nextPos(2);
	int curVal = INT_MAX, temp;
	grid.at(end.at(1)).at(end.at(2)) = INT_MAX;
	//This loop traces the shortest path by choosing positions with the lowest value
	while (!home) {

		//Set temp to above value
		temp = grid.at(curPos.at(1) - 1).at(curPos.at(2));
		//If temp is not 0 and it is less than curVal, set nextPos to above and set
		//curVal to temp
		if (temp != 0 && temp < curVal) {
			nextPos.at(1) = curPos.at(1) - 1;
			nextPos.at(2) = curPos.at(2);
			curVal = temp;
		}

		//Same operations done below for bottom, left, and right

		temp = grid.at(curPos.at(1) + 1).at(curPos.at(2));
		if (temp != 0 && temp < curVal) {
			nextPos.at(1) = curPos.at(1) + 1;
			nextPos.at(2) = curPos.at(2);
			curVal = temp;
		}

		temp = grid.at(curPos.at(1)).at(curPos.at(2) - 1);
		if (temp != 0 && temp < curVal) {
			nextPos.at(1) = curPos.at(1);
			nextPos.at(2) = curPos.at(2) - 1;
			curVal = temp;
		}

		temp = grid.at(curPos.at(1)).at(curPos.at(2) + 1);
		if (temp != 0 && temp < curVal) {
			nextPos.at(1) = curPos.at(1);
			nextPos.at(2) = curPos.at(2) + 1;
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

	//Outputs some text to console depending on whether the maze was solved or not
	if (notSolved == true) {
		cout << "REKT";
	}
	else {
		cout << "YITE!";
	}

	//Returns the solved path
	return finalPath;
}

int main() {
	return 0;
}
