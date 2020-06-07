#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <filesystem>
#include "Scene.h"

using namespace std;

Scene* loadScene(filesystem::path path);