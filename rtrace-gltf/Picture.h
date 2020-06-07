#pragma once

#include <vector>
#include <boost/qvm/vec.hpp>

using namespace std;
using namespace boost::qvm;

class Picture {
private:
	int width;
	int height;
	vector<vec<float, 3>> pixelBuffer;
public:
	Picture(int width, int heigth);
	vec<float, 3> getPixel(int x, int y);
	void setPixel(int x, int y, vec<float, 3> color);
};