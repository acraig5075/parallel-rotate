#pragma once
#include <string>

struct Settings
{
	enum EPointInPolyWhichLoop { ParalleliseInnerLoop, ParalleliseOuterLoop };

	Settings();
	~Settings();

	void EverythingOn();
	void EverythingOff();
	void SetEverything(bool on);

	bool HasFile();
	void Read();
	void Write();

	std::string kFilename = "settings.txt";

	// Independent checks for correctness
	bool Verify = false;

	// Experiments to perform
	bool DoRotation = true;
	bool DoMultiplication = true;
	bool DoPointInPoly = true;
	bool DoDuplicates = true;

	// Use following implementations
	bool UseSerial = true;
	bool UsePpl = true;
	bool UseOmp = true;
	bool UseAmp = true;

	// Rotation
	size_t RotationNumPoints = 10000;
	float RotationDegreeStep = 1.f;

	// Multiplication
	size_t MultiplicationMatSize = 400;

	// Point-in-poly
	EPointInPolyWhichLoop PointInPolyWhichLoop;

	// Duplicates
	size_t DuplicatesGridSize = 100;
};

