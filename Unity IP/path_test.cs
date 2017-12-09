using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class path_test : MonoBehaviour {

	// Use this for initialization
	void Start () {
		//pathfinding alogithm test (not technically working)
		Texture2D fMaze = pmaze; //pmaze will be the original image texture
		//this class also requires access to the vector of 0 and 1

		int xPos = 124;
		int yPos = 15;
		int lastmove = -1;
		int move = -2; //0 right 1 left 2 up 3 down
		int inc = 0;
		int or = -1;
		int ol = -1;
		int ou = -1;
		int od = -1;
		int movement = -1; //for which way we moved last (0 for x 1 for y)

		print (fMaze.width);
		print (fMaze.height);

		bool searching = true;
		while (searching) {
			//find closest wall
			int right = -1;
			int left = -1;
			int down = -1;
			int up = -1;

			//right
			for (int i = xPos; i < fMaze.width; i++) {
				if (vector [i, yPos] == true) {
					right = i-xPos;
					break;
				}
			}
			//left
			for (int i = xPos; i > 0; i--) {
				if (vector [i, yPos] == true) {
					left = -i+xPos;
					break;
				}
			}
			//down
			for (int i = yPos; i > 0; i--) {
				if (vector [xPos, i] == true) {
					down = -i+yPos;
					break;
				}
			}
			//up
			for (int i = yPos; i < fMaze.height; i++) {
				if (vector [xPos, i] == true) {
					up = i-yPos;
					break;
				}
			}

			print ("Right " + right + " Left " + left + " Up " + up + " Down " + down);

			if (right > or && right > left && movement != 0) {
				print (left);
				movement = 0;
				while (vector [xPos + 1, yPos] != true) {
					movement = 0;
					xPos += 1;
					fMaze.SetPixel (xPos, yPos, Color.blue);
				}
			} else if (left > ol && movement != 0) {
				movement = 0;
				while (vector [xPos - 1, yPos] != true) {
					movement = 0;
					xPos -= 1;
					fMaze.SetPixel (xPos, yPos, Color.red);
				}
			} else if (up > ou && up > down && movement != 1) {
				movement = 1;
				while (vector [xPos, yPos + 1] != true) {
					movement = 1;
					yPos += 1;
					fMaze.SetPixel (xPos, yPos, Color.green);
				}
			} else if (down > od && movement != 1) {
				movement = 1;
				while (vector [xPos, yPos - 1] != true) {
					movement = 1;
					yPos -= 1;
					fMaze.SetPixel (xPos, yPos, Color.cyan);
				}
			} else {

				if (right > left && movement != 0) {
					movement = 0;
					while (vector [xPos + 1, yPos] != true) {
						xPos += 1;
						fMaze.SetPixel (xPos, yPos, Color.blue);
					}
				} else if (left > right && movement != 0) {
					movement = 0;
					while (vector [xPos - 1, yPos] != true) {
						xPos -= 1;
						fMaze.SetPixel (xPos, yPos, Color.red);
					}
				} else if (up > down && movement != 1) {
					movement = 1;
					while (vector [xPos, yPos + 1] != true) {
						yPos += 1;
						fMaze.SetPixel (xPos, yPos, Color.green);
					}
				} else {
					movement = 1;
					while (vector [xPos, yPos - 1] != true) {
						yPos -= 1;
						fMaze.SetPixel (xPos, yPos, Color.cyan);
					}
				}
			}


			lastmove = move;

			if (inc > 1000) { //# of steps
				searching = false;
			}
			or = right;
			ol = left;
			ou = up;
			od = down;

			inc++;
		}

		byte[] bytess = fMaze.EncodeToPNG();
		File.WriteAllBytes(Application.dataPath + "/" + "path.png", bytess); //writing new image to asset folder
	}
}
