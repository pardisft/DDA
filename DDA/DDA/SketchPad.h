#ifndef SKETCHPAD_H
#define SKETCHPAD_H

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class SketchPad {
public:
	SketchPad();
	SketchPad(char *);

	inline bool painted() { return _painted; }
	inline void setPainted(bool painted) { _painted = painted; }
	inline void setLastPoint(int x, int y) { _last_x = x, _last_y = y; }

	cv::Mat _sketchPad;

private:
	void init();
	static void mouseClick(int, int, int, int, void*);
	static void drawPixel(cv::Mat&, int, int, cv::Vec3b);
	static void DDA(cv::Mat&, int, int, int, int, cv::Vec3b);

	char *_window;
	char _pos[20];
	bool _painted;
	int _last_x, _last_y;
};

#endif