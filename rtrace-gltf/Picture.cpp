#include "Picture.h"

Picture::Picture(int width, int heigth)
{
	this->width = width;
	this->height = height;
	this->pixelBuffer = vector<vec<float, 3>>(width * height, { 0, 0, 0 });
}

vec<float, 3> Picture::getPixel(int x, int y)
{
	return pixelBuffer[y * width + x];
}

void Picture::setPixel(int x, int y, vec<float, 3> color)
{
	pixelBuffer[y * width + x] = color;
}
