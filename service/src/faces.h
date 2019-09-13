#pragma once
#include <cstdio>
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Faces
{
public:
	Faces();
	int find(std::string in, std::string out);
	~Faces();

private:

};
