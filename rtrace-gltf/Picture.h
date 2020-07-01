#pragma once

#include <vector>
#include <boost/qvm/vec.hpp>

class Picture {
private:
	int width;
	int height;
	std::vector<boost::qvm::vec<float, 3>> pixelBuffer;
public:
	Picture(int width, int heigth);
	int getWidth();
	int getHeight();
	std::vector<boost::qvm::vec<float, 3>> getPixelBuffer();
	boost::qvm::vec<float, 3> getPixel(int x, int y);
	void setPixel(int x, int y, boost::qvm::vec<float, 3> color);
};