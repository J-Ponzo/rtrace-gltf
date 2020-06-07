// rtrace-gltf.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>

#include "loader.h"
#include "renderer.h"
#include "saver.h"
#include "rtrace-gltf.h"

const string INPUT_FILE_OPTION = "inFile";
const string OUTPUT_FILE_OPTION = "outFile";
const string WIDTH_OPTION = "w";
const string HEIGHT_OPTION = "h";
const string BOUNCES_OPTION = "b";
const string SAMPLES_OPTION = "s";

const int DEFAULT_WIDTH = 256;
const int DEFAULT_HEIGHT = 256;
const int DEFAULT_BOUNCES = 5;
const int DEFAULT_SAMPLES = 5;

map<string, string> parseOptions(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	map<string, string> options = parseOptions(argc, argv);

	string inFile = options[INPUT_FILE_OPTION];
	cout << "------------------------------" << endl;
	cout << "-----     LOAD SCENE     -----" << endl;
	cout << "------------------------------" << endl;
	cout << INPUT_FILE_OPTION << "=" << inFile << endl;
	Scene* scene = loadScene(inFile);
	cout << "------------------------------" << endl << endl;

	int width = options[WIDTH_OPTION] != "" ? stoi(options[WIDTH_OPTION]) : DEFAULT_WIDTH;
	int height = options[HEIGHT_OPTION] != "" ? stoi(options[HEIGHT_OPTION]) : DEFAULT_HEIGHT;
	int bounces = options[BOUNCES_OPTION] != "" ? stoi(options[BOUNCES_OPTION]) : DEFAULT_BOUNCES;
	int samples = options[SAMPLES_OPTION] != "" ? stoi(options[SAMPLES_OPTION]) : DEFAULT_SAMPLES;
	cout << "------------------------------" << endl;
	cout << "-----     RENDERING      -----" << endl;
	cout << "------------------------------" << endl;
	cout << WIDTH_OPTION << "=" << width << endl;
	cout << HEIGHT_OPTION << "=" << height << endl;
	cout << BOUNCES_OPTION << "=" << bounces << endl;
	cout << SAMPLES_OPTION << "=" << samples << endl;
	Picture* picture = render(scene, width, height, bounces, samples);
	cout << "------------------------------" << endl << endl;

	string outFile = options[OUTPUT_FILE_OPTION];
	cout << "------------------------------" << endl;
	cout << "-----    SAVE PICTURE    -----" << endl;
	cout << "------------------------------" << endl;
	cout << OUTPUT_FILE_OPTION << "=" << outFile << endl;
	savePicture(picture, outFile);

	cout << "------------------------------" << endl << endl;

	system("pause");
}

map<string, string> parseOptions(int argc, char* argv[]) {
	map<string, string> options;

	for (int i = 1; i < argc; i++) {
		string arg = argv[i];
		int pos = arg.find('=');
		if (pos != string::npos) {
			options[arg.substr(0, pos)] = arg.substr(pos + 1, arg.length());
		}
	}

	return options;
}