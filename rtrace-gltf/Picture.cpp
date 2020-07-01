#include "Picture.h"

using namespace boost::qvm;
using namespace std;

Picture::Picture(int width, int height)
{
	this->width = width;
	this->height = height;
	this->pixelBuffer = vector<vec<float, 3>>(width * height, { 0, 0, 0 });
}

int Picture::getWidth()
{
	return width;
}

int Picture::getHeight()
{
	return height;
}

vector<vec<float, 3>> Picture::getPixelBuffer()
{
	return pixelBuffer;
}

vec<float, 3> Picture::getPixel(int x, int y)
{
	return pixelBuffer[y * width + x];
}

void Picture::setPixel(int x, int y, vec<float, 3> color)
{
	pixelBuffer[y * width + x] = color;
}
