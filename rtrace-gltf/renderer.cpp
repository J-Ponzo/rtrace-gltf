#include "renderer.h"

Picture* render(Scene* scene, int width, int height, int bounces, int samples)
{
    Picture* picture = new Picture(width, height);

    //TODO Remove this stub
    for (int x = 0; x < picture->getWidth(); x++) {
        for (int y = 0; y < picture->getHeight(); y++) {
            vec<float, 3> color = { x, y, x + y };
            picture->setPixel(x, y, color);
        }
    }
	return picture;
}
