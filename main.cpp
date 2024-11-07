#include <iostream>
#include <opencv2/opencv.hpp>
#include "visionneuse.h"

using namespace cv;
using namespace std;

int main()
{
	Mat flux;
	visionneuse v = visionneuse(flux, "images");
	v.start();
	return 0;
}
