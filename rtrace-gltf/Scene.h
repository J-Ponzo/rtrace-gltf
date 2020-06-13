#pragma once
#include <vector>
#include <boost/qvm/all.hpp>

using namespace std;
using namespace boost::qvm;

struct Node;
struct Mesh;
struct Primitive;
struct Light;
struct Camera;

struct Scene
{
	Node* root;
};

struct Node
{
	vec<float, 3> translation = {0, 0, 0};
	quat<float> rotation = {0, 0, 0, 1};
	vec<float, 3> scale = {1, 1, 1};

	Mesh* mesh;
	Camera* camera;
	Light* light;

	string name;
	vector<Node*> childs;
};

struct Mesh {
	vector<Primitive*> primitives;
};

struct Primitive {
	vector<vec<float, 3>> positions;
	vector<vec<int, 3>> triangles;
};

struct Light {
	vec<float, 3> color;
	float intensity;
};

struct Camera {
	float ratio;
	float fov;		//expressed in radians
};