#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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
	cout << "Multiply: " << position[0] * position[1] << endl << endl;
}

#pragma endregion AOC_02

#pragma region AOC_03

void ParseChar(int &position, string &line, int(&ones)[12], int(&zeros)[12]) {
	if (line.c_str()[position] == '0') {
		zeros[position] += 1;
	}
	else {
		ones[position] += 1;
	}
}

void ParseFileToGammaEpsilon(ifstream& file, int(&gamma)[12], int(&epsilon)[12]) {
	string line;
	int ones[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	int zeros[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	while (std::getline(file, line)) {
		for (int i = 0; i < 12; ++i) {
			ParseChar(i,line,ones,zeros);
		}
	}
	for (int i = 0; i < 12; ++i) {
		gamma[i] = (ones[i] > zeros[i] ? 1 : 0);
		epsilon[i] = (ones[i] > zeros[i] ? 0 : 1);
	}
}

void FindMatchingOnPosition(std::vector<string>& Lines, const int position, bool IsOxygen) {
	if (position == 12) {
		return;
	}

	int numOnes = 0;
	int numZeros = 0;
	for (string tmpLine : Lines) {
		if (tmpLine.c_str()[position] == '1') {
			numOnes += 1;
		}
		else {
			numZeros += 1;
		}
	}

	vector<string>::iterator it = Lines.begin();
	while (it != Lines.end() && Lines.size() > 1) {
		if (IsOxygen) {
			if ((numOnes >= numZeros && (*it).c_str()[position] == '1')) {
				++it;
			}
			else if ((numOnes < numZeros && (*it).c_str()[position] == '0')) {
				++it;
			}
			else it = Lines.erase(it);
		}
		else {
			if ((numOnes < numZeros && (*it).c_str()[position] == '1')) {
				++it;
			}
			else if ((numOnes >= numZeros && (*it).c_str()[position] == '0')) {
				++it;
			}
			else it = Lines.erase(it);
		}
	}

	FindMatchingOnPosition(Lines, position+1, IsOxygen);
}

void ParseFileToOxygen(ifstream& file, string &OxygenRating, string &CO2Rating) {
	std::vector<string> OxygenLines;
	std::vector<string> CO2Lines;
	string line;
	while (std::getline(file, line)) {
		OxygenLines.push_back(line);
		CO2Lines.push_back(line);
	}
	FindMatchingOnPosition(OxygenLines, 0, true);
	FindMatchingOnPosition(CO2Lines, 0, false);

	OxygenRating = OxygenLines[0];
	CO2Rating = CO2Lines[0];
}

int BinaryIntArrayToString(int(&arr)[12]) {
	std::ostringstream os;
	for (int i : arr) {
		os << i;
	}
	std::string str(os.str());
	return std::stoi(str, nullptr, 2);
}

void AoCThree() {
	int gamma[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	int epsilon[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	string OxygenRating, CO2Rating;
	std::ifstream infile("InputFiles/inputAoC_03.txt");
	if (infile.is_open()) {
		ParseFileToGammaEpsilon(infile, gamma, epsilon);
		infile.clear();
		infile.seekg(0);
		ParseFileToOxygen(infile, OxygenRating, CO2Rating);
		infile.close();
	}

	int gammaNumber = BinaryIntArrayToString(gamma);
	int epsilonNumber = BinaryIntArrayToString(epsilon);
	int oxygenRatingNumber = std::stoi(OxygenRating, nullptr, 2);
	int co2RatingNumber = std::stoi(CO2Rating, nullptr, 2);

	cout << "AOC 03" << endl;
	cout << "Gamma: " << gammaNumber << " Epsilon: " << epsilonNumber << endl;
	cout << "O2: " << oxygenRatingNumber << " CO2: " << co2RatingNumber << endl;
	cout << "Power Consumption: " << gammaNumber * epsilonNumber << endl;
	cout << "LifeSupport Rating: " << oxygenRatingNumber * co2RatingNumber << endl;

}

#pragma endregion AOC_03

int main() {
	AoCOne();
	AoCTwo();
	AoCThree();
	cin.get();
	return 0;
}