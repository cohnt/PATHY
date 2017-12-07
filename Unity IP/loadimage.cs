using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class loadimage : MonoBehaviour {

	Texture2D rawmaze; //texture of raw maze image
	bool[,] vector; //2d vector to store processed image results (1,0)

	void Start () {
		rawmaze = (Texture2D)Resources.Load("maze"); //reading image from resource folder called maze
		vector = new bool[rawmaze.width, rawmaze.height];
		Texture2D pmaze = rawmaze; //new texture to output processed image
		int tolerance = 200;

		//image processing algorithm
		for(int i = 0; i < rawmaze.width; i++) {
			for(int j = 0; j < rawmaze.height;j++) {
				if (rawmaze.GetPixel (i, j).grayscale > tolerance/255) {
					pmaze.SetPixel(i, j, Color.white);
					vector [i,j] = false;
				} else {
					pmaze.SetPixel (i, j, Color.black);
					vector [i,j] = true;
				}
			}
		}


		byte[] bytes = pmaze.EncodeToPNG();
		File.WriteAllBytes(Application.dataPath + "/" + "swag.png", bytes); //writing new image to asset folder
	}
}
