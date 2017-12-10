/*
Author: Victor Hao & Noah Koorndyk & Roberto Jimenez
Date Created: 12/10/2017
Last Update: 12/10/17 3:36 PM
Lab Section: 103
Project #: Team Hack
Filename: dijkstraV2.cpp

Program Description: Updated implementation of Dijkstra's pathfinding algorithm for
a maze. Avoids iterating over the whole image with every outer loop iteration by
adding altered points to a queue and only iterating over those points. Dramatic
runtime decrease with this new optimization.

A slight twist on classic dijkstra's since each pixel move has the same weight of 1.
First we crawl the entire maze and assign their respective cost values until the
end is found. Then, we can read the path back from the end by simply choosing an
adjacent move which has the lowest value of the four possible moves. Returned
finalPath corresponds exactly with pixel coords in image (does not start at 0 due to
nature of padding).

NOTES: Use 1D array if a small optimization is desired.

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
	vector<vector<int> > finalPath, toSweep, queue;
	vector<int> holding;

	//Begin the sweeps at start
	toSweep.push_back(start);

	//curSweep should look for start in the first iteration
	int curSweep = 1;

	//Pad the outside of the maze with a wall (stops index out of bounds)
	padMaze(grid);
	//Sets Start to 1 and end to -2
	grid.at(start.at(0)).at(start.at(1)) = 1;
	grid.at(end.at(0)).at(end.at(1)) = -2;

	//Loop until the maze is solved
	int i, j;
	while (notSolved) {

		changedVals = false;
		//Loop through all positions in the image
		for (unsigned int a = 0; a < toSweep.size(); a++) {

			//Set i,j to row and col coords of current point
			i = toSweep.at(a).at(0);
			j = toSweep.at(a).at(1);

			//If above value is open, change to curSweep + 1
			if (grid.at(i - 1).at(j) == 0) {
				grid.at(i - 1).at(j) = curSweep + 1;
				//Set changedVals to true
				changedVals = true;

				//Add the coords of changed point to holding
				holding.push_back(i - 1);
				holding.push_back(j);
				//Add point to queue
				queue.push_back(holding);
				//Clear holding
				holding.clear();
			}

			//Same operation for below
			if (grid.at(i + 1).at(j) == 0) {
				grid.at(i + 1).at(j) = curSweep + 1;
				changedVals = true;

				holding.push_back(i + 1);
				holding.push_back(j);
				queue.push_back(holding);
				holding.clear();
			}

			//Same operation for left
			if (grid.at(i).at(j - 1) == 0) {
				grid.at(i).at(j - 1) = curSweep + 1;
				changedVals = true;

				holding.push_back(i);
				holding.push_back(j - 1);
				queue.push_back(holding);
				holding.clear();
			}

			//Same operation for right
			if (grid.at(i).at(j + 1) == 0) {
				grid.at(i).at(j + 1) = curSweep + 1;
				changedVals = true;

				holding.push_back(i);
				holding.push_back(j + 1);
				queue.push_back(holding);
				holding.clear();
			}

			//Check if any of the adjacent values are the goal
			if (grid.at(i - 1).at(j) == -2 || grid.at(i + 1).at(j) == -2 || grid.at(i).at(j - 1) == -2 || grid.at(i).at(j + 1) == -2) {
				notSolved = false;
				break;
			}
		}
		//If we didn't change any values and didn't find the end, we failed (break)
		if (!changedVals) {
			cout << "Oh no." << endl;
			return finalPath;
		}

		//Increment curSweep
		curSweep++;
		//Clear toSweep
		toSweep.clear();

		//Add all of queue to toSweep
		for (unsigned int b = 0; b < queue.size(); b++) {
			toSweep.push_back(queue.at(b));
		}

		//Clear queue
		queue.clear();
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
	vector<int> start = { 20,20 };
	vector<int> end = { 580,345 };
	//Text File read is only meant for testing, so must specify exact size of maze
	int numRows = 601;
	int numCols = 366;

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
