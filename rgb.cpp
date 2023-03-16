#include "rgb.h"

RGB::RGB() :
	r(0), g(0), b(0)
{ }

RGB::RGB(int red, int green, int blue) :
	r(red), g(green), b(blue)
{ }

RGB& RGB::operator=(RGB source) {
	r = source.r;
	g = source.g;
	b = source.b;

	return *this;
}
