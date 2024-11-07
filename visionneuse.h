#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <Windows.h>

using namespace cv;
using namespace std;

class visionneuse
{

public:

	visionneuse(Mat flux, String directory);
	

	double dist2color(Vec3b color_pixel, Vec3b spec_color);


	void start();

	void afficherEnsembleImage();
	void afficherRectBas();
	void afficherCurrentPic();
	void afficherBoutons();

	void nextImage();
	void precImage();



	void realMouse(int event, int x, int y, int flag) {
		if (event == cv::EVENT_LBUTTONDOWN) {

			if (x >= d_hautGauche_gauche.x && x <= d_basDroite_gauche.x &&
				y >= d_hautGauche_gauche.y && y <= d_basDroite_gauche.y)
			{
				std::cout << "Les coordonnées se situent dans le rectangle vert." << std::endl;
				precImage();

			}
			else if (x >= d_hautGauche_droite.x && x <= d_basDroite_droite.x &&
				y >= d_hautGauche_droite.y && y <= d_basDroite_droite.y)
			{
				std::cout << "Les coordonnées se situent dans le rectangle rouge." << std::endl;
				std::cout << d_hautGauche_droite << "," << d_basDroite_droite << "rouge " << std::endl;
				std::cout << d_hautGauche_gauche << "," << d_basDroite_gauche << "vert " << std::endl;
				nextImage();
			}

			else {
				d_color = d_flux.at<cv::Vec3b>(y, x);
				d_vec_color.push_back(d_color);
			}
			
		}
	}

	void zoomCurrentPic();

	// static wrapper (does NOT need "this")
	static void onMouse(int event, int x, int y, int flag, void* me) {
		// "me" is, what we fed into setMouseCallback() below:
		visionneuse* that = (visionneuse*)(me); // cast back to proper class
		that->realMouse(event, x, y, flag); // call class member
	}

	boolean checkLeftButton();
	boolean checkRightButton();

private:
	Mat d_flux, d_flux_red, d_flux_green;

	String d_directory;
	vector<cv::String> fn;

	int nb_images;

	bool isPressed = false;

	int d_width;
	int d_height;

	Mat redMask;
	Mat greenMask;

	int d_count;
	int d_red_count = 0;
	int d_green_count = 0;

	Point d_best;

	Point pGauche;
	Point pDroite;

	Point d_hautGauche_gauche;
	Point d_basDroite_gauche;

	Point d_basDroite_droite;
	Point d_hautGauche_droite;

	std::vector<Vec3b> d_vec_color;

	Vec3b d_color = (255, 255, 255);

	int rouge_nocolor = -1;
	int rouge_fullcolor = -2;

	int vert_nocolor = -1;
	int vert_fullcolor = -2;

    VideoCapture d_cap;

	vector<Point> d_point;
	int resize_w = 80;
	int resize_h = 60;

	int current_pic = 0;

	Point p1_fullscreen_gaucheHaut;
	Point p2_fullscreen_basDroit;

	bool last = false;
	bool lastr = false;

};



