#include "Settings.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

const int kSize = 10000; // no. of points for rotation
const bool kVerify = false; // whether to perform (slow) tests on correctness of results
const float kStep = 1.f; // degree increment for rotation
const int kSquare = 400; // matrix dimension for multiplication
const int kGrid = 100; // square grid size for duplicates


Settings::Settings()
{
	RotationNumPoints = kSize;
	RotationDegreeStep = kStep;
	MultiplicationMatSize = kSquare;
	DuplicatesGridSize = kGrid;
}


Settings::~Settings()
{
}

void Settings::EverythingOn()
{
	SetEverything(true);
}

void Settings::EverythingOff()
{
	SetEverything(false);
}

void Settings::SetEverything(bool on)
{
	DoRotation = on;
	DoMultiplication = on;
	DoPointInPoly = on;
	DoDuplicates = on;

	// Implementations
	UseSerial = on;
	UsePpl = on;
	UseOmp = on;
	UseAmp = on;
}

bool Settings::HasFile()
{
	ifstream f(kFilename.c_str());
	if (f.good())
	{
		f.close();
		return true;
	}
	else
	{
		f.close();
		return false;
	}
}

namespace
{
	bool BooleanValue(const string &val)
	{
		string value = val;

		if (value.empty())
			return false;

		std::transform(value.begin(), value.end(), value.begin(), ::tolower);

		if (value == "no" || value == "off" || value == "false" || value == "0")
			return false;
		else
			return true;
	}

	int IntegerValue(const string &val)
	{
		if (val.empty())
			return 0;
		else
			return std::atoi(val.c_str());
	}

	float FloatValue(const string &val)
	{
		if (val.empty())
			return 0.f;
		else
			return static_cast<float>(std::atof(val.c_str()));
	}
}

void Settings::Read()
{
	string line, name, value;
	char equals;

	ifstream in(kFilename);
	while (in.good())
	{
		std::getline(in, line);
		if (line.empty() || line[0] == '#')
			continue;

		stringstream iss(line);
		if (iss >> name >> equals >> value)
		{
			if (name == "VERIFY")
				Verify = BooleanValue(value);

			else if (name == "DO-ROTATION")
				DoRotation = BooleanValue(value);
			else if (name == "DO-MULTIPLICATION")
				DoMultiplication = BooleanValue(value);
			else if (name == "DO-POINT-IN-POLY")
				DoPointInPoly = BooleanValue(value);
			else if (name == "DO-DUPLICATES")
				DoDuplicates = BooleanValue(value);

			else if (name == "USE-SERIAL")
				UseSerial = BooleanValue(value);
			else if (name == "USE-PPL")
				UsePpl = BooleanValue(value);
			else if (name == "USE-OMP")
				UseOmp = BooleanValue(value);
			else if (name == "USE-AMP")
				UseAmp = BooleanValue(value);

			else if (name == "ROTATION-NUM-POINTS")
				RotationNumPoints = IntegerValue(value);
			else if (name == "ROTATION-DEGREE-STEP")
				RotationDegreeStep = FloatValue(value);
			else if (name == "MULTIPLICATION-MAT-SIZE")
				MultiplicationMatSize = IntegerValue(value);
			else if (name == "DUPLICATES-GRID-SIZE")
				DuplicatesGridSize = IntegerValue(value);

			else
				std::cerr << "Warning: Unknown setting " << name << '\n';
		}
		else
			std::cerr << "Warning: Couldn't parse setting line \"" << line << "\"\n";
	}

	in.close();
}

void Settings::Write()
{
	ofstream out(kFilename);
	out << "#######################################\n"
		<< "# Settings file for parallel-rotate.exe\n"
		<< "#######################################\n"
		<< '\n';

	out << "# Independent checks for correctness\n"
		<< "VERIFY = " << Verify << '\n'
		<< '\n';

	out << "# Experiments to perform\n"
		<< setw(30) << std::left << "DO-ROTATION"       << "= " << DoRotation << '\n'
		<< setw(30) << std::left << "DO-MULTIPLICATION" << "= " << DoMultiplication << '\n'
		<< setw(30) << std::left << "DO-POINT-IN-POLY"  << "= " << DoPointInPoly << '\n'
		<< setw(30) << std::left << "DO-DUPLICATES"     << "= " << DoDuplicates << '\n'
		<< '\n';

	out << "# Use following implementations\n"
		<< setw(30) << std::left << "USE-SERIAL" << "= " << UseSerial << '\n'
		<< setw(30) << std::left << "USE-PPL"    << "= " << UsePpl << '\n'
		<< setw(30) << std::left << "USE-OMP"    << "= " << UseOmp << '\n'
		<< setw(30) << std::left << "USE-AMP"    << "= " << UseAmp << '\n'
		<< '\n';

	out << "# Rotation\n"
		<< setw(30) << std::left << "ROTATION-NUM-POINTS"  << "= " << RotationNumPoints << '\n'
		<< setw(30) << std::left << "ROTATION-DEGREE-STEP" << "= " << RotationDegreeStep << '\n'
		<< '\n';

	out << "# Multiplication\n"
		<< setw(30) << std::left << "MULTIPLICATION-MAT-SIZE" << "= " << MultiplicationMatSize << '\n'
		<< '\n';

	out << "# Point-in-poly\n"
		<< '\n';

	out << "# Duplicates\n"
		<< setw(30) << std::left << "DUPLICATES-GRID-SIZE" << "= " << DuplicatesGridSize << '\n'
		<< '\n';

	out.close();
}
