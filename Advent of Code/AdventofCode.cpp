#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#pragma region AOC_01
void AoCOne() {
	int numberOfIncreases = 0;
	std::ifstream infile("InputFiles/inputAoC_01.txt");
	if (infile.is_open()) {
		std::string line;
		std::getline(infile, line);

		std::getline(infile, line);
		int fLine = atoi(line.c_str());
		std::getline(infile, line);
		int sLine = atoi(line.c_str());
		std::getline(infile, line);
		int tLine = atoi(line.c_str());

		int lastDetphSum = fLine + sLine + tLine;

		while (std::getline(infile, line)) {
			fLine = sLine;
			sLine = tLine;
			tLine = atoi(line.c_str());
			int currentDepthSum = fLine + sLine + tLine;
			if (currentDepthSum > lastDetphSum) numberOfIncreases++;
			lastDetphSum = currentDepthSum;
		}
		infile.close();
	}
	cout << "AOC 01" << endl;
	cout << "Number of Sonar increases: " << numberOfIncreases << endl << endl;
}

#pragma endregion AOC_01

#pragma region AOC_02
bool ParseLine(ifstream& file, string& command, int& parameter) {
	string line;
	if (std::getline(file, line)) {
		istringstream ss(line);
		ss >> command;
		ss >> parameter;
		return true;
	}
	return false;
}

bool ExecuteCommand(const string& command, const int& parameter, int(&position)[3]) {
	if (command == "forward") {
		position[0] += parameter;
		position[1] += parameter * position[2];
		return true;
	}
	else if (command == "down") {
		position[2] += parameter;
		return true;
	}
	else if (command == "up") {
		position[2] -= parameter;
		return true;
	}

	return false;
}

void AoCTwo() {
	int position[3] = { 0,0,0 };
	std::ifstream infile("InputFiles/inputAoC_02.txt");
	if (infile.is_open()) {
		string command;
		int parameter;
		while (ParseLine(infile, command, parameter)) {
			ExecuteCommand(command, parameter, position);
		}
		infile.close();
	}
	cout << "AOC 02" << endl;
	cout << "Positions: " << position[0] << " " << position[1] << " " << position[2] << endl;
	cout << "Multiply: " << position[0] * position[1] << endl;
}

#pragma endregion AOC_02

int main() {
	AoCOne();
	AoCTwo();
	cin.get();
	return 0;
}