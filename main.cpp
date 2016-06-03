#include "opencv2/opencv.hpp"
#include <iostream>


using namespace cv;
using namespace std;
Mat hough2d(Mat img, Mat msk);
Mat morphOps(Mat img, int x);
Mat rescale_image(Mat img, double factor);

int main(int argc, char** argv)
{
	Mat im1, im2;
	vector<Mat> bgr(3);

	//im1 = imread("images/image07.jpg");


	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
	
	for (;;)
	{
		cap >> im1;
		//im1 = rescale_image(im1, .5);
		im1.copyTo(im2);
		cvtColor(im1, im1, CV_BGR2HSV);
		split(im1, bgr);
		

		vector<Mat> bgr_thresh(3);
		threshold(bgr[0], bgr_thresh[0], 26, 255, THRESH_BINARY);
		threshold(bgr[1], bgr_thresh[1], 60, 255, THRESH_BINARY);
		threshold(bgr[2], bgr_thresh[2], 140, 255, THRESH_BINARY);

		Mat bitwised;
		bitwise_and(bgr_thresh[0], bgr_thresh[1], bitwised);
		bitwise_and(bgr_thresh[2], bitwised, bitwised);
		

		Mat morphed = morphOps(bitwised, 5);
		
		Mat blurred;
		GaussianBlur(morphed, blurred, { 9, 9 }, 1, 1);
	


		Mat hough;
		hough = hough2d(im2, blurred);
		imshow("my program", hough);
		if (waitKey(30) >= 0) break;
	}
	
}


Mat hough2d(Mat img, Mat msk)
{

	Mat hough_in = msk;
	Mat img2 = img;
	//img2 = img;
	//hough_in = msk;

	vector<Vec3f> circles;
	HoughCircles(hough_in, circles, CV_HOUGH_GRADIENT, 40, 10, 100, 40);

	int thresh = 100;


	RNG rng(12345);
	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	threshold(hough_in, threshold_output, thresh, 255, THRESH_BINARY);

	findContours(threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());


//	cout << "possible circles " << contours.size() << endl;
	if (contours.size() == 0) return(img2);
	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		minEnclosingCircle(contours_poly[i], center[i], radius[i]);

		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

		//circle(img2, center[i], (int)radius[i], color, 5, 8, 0);
	}
	auto tempo = max_element(radius.begin(), radius.end());
	float maxrad = *max_element(radius.begin(), radius.end());
	//int indexmaxradius = 
	int dis = distance(radius.begin(), tempo);

	circle(img2, center[dis], (int)maxrad, { 0, 0, 255 }, 2, 8, 0);
	//cout << "max radius is " << endl;

	return(img2);
}

Mat morphOps(Mat img, int x)
{

	Mat dst;
	Mat element1 = getStructuringElement(MORPH_ELLIPSE, Size(x, x), Point((x - 1) / 2, (x - 1) / 2));
	Mat element2 = getStructuringElement(MORPH_ELLIPSE, Size(x - 2, x - 2), Point((x - 3) / 2, (x - 3) / 2));

	dilate(img, dst, element1);
	erode(img, dst, element1);

	erode(img, dst, element2);
	dilate(img, dst, element2);

	return(dst);
}
Mat rescale_image(Mat img, double factor)
{
	const int new_width = (int)((float)img.cols * factor);
	const int new_height = (int)((float)img.rows * factor);
	Size s(new_width, new_height);
	Mat img2 = Mat(s, CV_8UC3);
	resize(img, img2, s);
	return(img2);

}