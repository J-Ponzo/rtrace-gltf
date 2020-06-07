#pragma once

#include "Scene.h"
#include "Picture.h"

Picture* render(Scene* scene, int width, int height, int bounces, int samples);