#include "SketchPad.h"

#include <iostream>

#define COLOR_R 0
#define COLOR_G 0
#define COLOR_B 0
#define HEIGHT 600
#define WIDTH 600
#define FONT_LEFT_POS 10
#define FONT_BOTTOM_POS (HEIGHT-FONT_LEFT_POS)
/**
*	This function provides operations for EVENT_LBUTTONDOWN and EVENT_MOUSEMOVE.
* @Note: setMouseCallback() only accepts static functions.
**/
void SketchPad::mouseClick(int event, int x, int y, int flags, void *param) {
	SketchPad *skp = (SketchPad*) param;
	cv::Mat &mt = (skp->_sketchPad);
	char *txt = skp->_pos;
	switch (event) {
		// link lines together
		case cv::EVENT_LBUTTONDOWN:
			{
#ifdef DEBUG
				std::cout << "Clicked: " << x << " " << y << std::endl;
#endif
				if (skp -> painted() == true) {
					DDA(mt, skp->_last_x, skp->_last_y, x, y, cv::Vec3b(COLOR_R, COLOR_G, COLOR_B));
				} else {
					drawPixel(mt, x, y, cv::Vec3b(COLOR_R, COLOR_G, COLOR_B));
				}
				skp -> setLastPoint(x, y);
				skp -> setPainted(true);
				cv::imshow(skp -> _window, mt);
			}
			break;
		// show position of mouse
		case cv::EVENT_MOUSEMOVE:
			{
				sprintf(txt, "(%d, %d)", x, y);
				cv::string text(txt);
				for (int i = 0; i < 100; i++) {
					for (int j = 1; j <= 20; j++){
						drawPixel(mt, i + FONT_LEFT_POS, FONT_BOTTOM_POS + 5 - j, cv::Vec3b(255, 255, 255));
					}
				}
				cv::putText(mt, text, cv::Point(FONT_LEFT_POS, FONT_BOTTOM_POS), cv::FONT_HERSHEY_SIMPLEX, .5, cv::Scalar(255, 0, 0));
				cv::imshow(skp -> _window, mt);
			}
		default:
			break;
	};
}

// @Note: I think the original DDA is incorrect, so I modified it.
void SketchPad::DDA(cv::Mat &mt, int x0, int y0, int x1, int y1, cv::Vec3b pixel) {
	int distX = x1 - x0;
	int distY = y1 - y0;
	int maxStep = std::max(std::abs(distX), std::abs(distY));
	float stepX = (float)distX / (float)maxStep;
	float stepY = (float)distY / (float)maxStep;
	float x = x0, y = y0;
	for (int i = 0; i <= maxStep; i++) {
		drawPixel(mt, (int)x, (int)y, pixel);

#ifdef DEBUG
		std::cout << "\t" << x << "  " << y << std::endl;
#endif

		x += stepX;
		y += stepY;
	}
}

// @Note: the x and y of is reversed in cv::Mat
// @Note: the type of elements of cv::Mat
void SketchPad::drawPixel(cv::Mat &mt, int x, int y, cv::Vec3b pixel) {
	mt.at<cv::Vec3b>(y, x) = pixel; 
}

// @Note: CV_8BUC3 is compatible with cv::Vec3b
void SketchPad::init() {
	_painted = false;
	cv::imshow(_window, _sketchPad);
	cv::moveWindow(_window, 200, 200);
	cv::setMouseCallback(_window, mouseClick, this);
	// cv::waitKey(0);
	while (1) {
		int key = cv::waitKey(0);
		switch (key) {
		case 'c': 
			{
				for (int i = 0; i < WIDTH; i++) {
					for (int j = 0; j < HEIGHT; j++){
						drawPixel(_sketchPad, i, j, cv::Vec3b(255, 255, 255));
					}
				}
				this->_painted = false;
				cv::imshow(this -> _window, _sketchPad);
			}
			break;
		default:
			break;
		}
	}
}

SketchPad::SketchPad() : 
	_sketchPad(WIDTH, HEIGHT, CV_8UC3, cv::Scalar(255, 255, 255)) {
	_window = "SkectchPad: DDA";
	this->init();
}

SketchPad::SketchPad(char *windowName) : 
	_sketchPad(WIDTH, HEIGHT, CV_8UC3, cv::Scalar(255, 255, 255)) {
	_window = windowName;
	this->init();
}