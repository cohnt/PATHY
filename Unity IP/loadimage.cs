using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class loadimage : MonoBehaviour {

	Texture2D rawmaze; //texture of raw maze image
	bool[,] vector; //2d vector to store processed image results (1,0)
	double scale = 0.5; //this is the downscale factor that is being utilized. 0.5 would effectively "half" the res.
	double tolerance = 240; //tolerance of lowest value for pixel considered "white"

	void Start () {
		rawmaze = (Texture2D)Resources.Load ("maze"); //reading image from resource folder called maze
		rawmaze = downscale (rawmaze, scale);
		vector = new bool[rawmaze.width, rawmaze.height];
		Texture2D pmaze = new Texture2D (rawmaze.width, rawmaze.height, TextureFormat.ARGB32, false); //new texture to output processed image

		//image processing algorithm
		for (int i = 0; i < rawmaze.width; i++) {
			for (int j = 0; j < rawmaze.height; j++) {
				//print (rawmaze.GetPixel (i, j).grayscale);
				if (rawmaze.GetPixel (i, j).grayscale > tolerance/255.0) {
					pmaze.SetPixel (i, j, Color.white);
					vector [i, j] = false;
				} else {
					pmaze.SetPixel (i, j, Color.black);
					vector [i, j] = true;
				}
			}
		}


		byte[] bytes = pmaze.EncodeToPNG ();
		File.WriteAllBytes (Application.dataPath + "/" + "swag.png", bytes); //writing new image to asset folder
	}

	//function to downscale image by a given factor
	Texture2D downscale(Texture2D raw, double res) {
		int nwidth = (int)(raw.width * res);
		int nheight = (int)(raw.height * res);
		TextureScale.Bilinear (raw, nwidth, nheight);
		return raw;
	}
}
