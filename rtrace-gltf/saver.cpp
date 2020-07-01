#include "saver.h"

#include <boost/qvm/vec.hpp>
#include <SDL.h>

using namespace boost::qvm;
using namespace std;

void savePicture(Picture* picture, string path)
{
	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, picture->getWidth(), picture->getHeight(), 32, SDL_PIXELFORMAT_RGBA8888);

	SDL_LockSurface(surface);
	Uint32* sdlPixels = (Uint32 *) surface->pixels;
	for (int y = 0; y < picture->getHeight(); y++) {
		for (int x = 0; x < picture->getWidth(); x++) {
			vec<float, 3> pixel = picture->getPixel(x, y);
			sdlPixels[y * picture->getWidth() + x] = SDL_MapRGBA(surface->format, pixel.a[0], pixel.a[1], pixel.a[2], 255);
		}
	}
	SDL_UnlockSurface(surface);

	SDL_SaveBMP(surface, path.c_str());
	SDL_FreeSurface(surface);
}
