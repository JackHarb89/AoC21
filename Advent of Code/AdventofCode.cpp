#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <math.h>

using namespace std;

struct Line {
	int x1,x2,y1,y2;

	Line ()
	: x1(-1)
	, x2(-1)
	, y1(-1)
	, y2(-1)
	{}

	int DistanceDiagonal45() {
		return abs(x1 - x2);
	}

	bool IsValid() {
		return x1 != -1 && x2 != -1 && y1 != -1 && y2 != -1;
	}

	bool IsHorizontal() {
		return y1 == y2;
	}

	bool IsDiagonal45() {
		return IsDiagonal() && abs(x1-x2) == abs(y1-y2);
	}

	bool IsVertical() {
		return x1 == x2;
	}

	bool IsDiagonal() {
		return !(IsHorizontal() || IsVertical());
	}

	void SetPoint(int in_x, int in_y, bool isStart) {
		if (isStart) {
			x1 = in_x;
			y1 = in_y;
		}
		else {
			x2 = in_x;
			y2 = in_y;
		}
	}

	Line operator=(const Line rhs) {
		x1 = rhs.x1;
		x2 = rhs.x2;
		y1 = rhs.y1;
		y2 = rhs.y2;
	}
};

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
	cout << "Power Consumption: " << gammaNumber * epsilonNumber << endl;
	cout << "O2: " << oxygenRatingNumber << " CO2: " << co2RatingNumber << endl;
	cout << "LifeSupport Rating: " << oxygenRatingNumber * co2RatingNumber << endl << endl;

}

#pragma endregion AOC_03

#pragma region AOC_04

void ParseDrawNumbers(ifstream& file, std::vector<int> &drawNumbers) {
	string line;
	int index = 0;
	while (std::getline(file, line, ',')) {
		drawNumbers.push_back(std::stoi(line));
		++index;
		if (index == 99) {
			std::getline(file, line);
			drawNumbers.push_back(std::stoi(line));
			return;
		}
	}
}

void SplitBoardRow(string& line, const string& delimiter, int(&boardRow)[5]) {
	size_t pos = 0;
	std::string rowNumber;
	int columnIndex = 0;
	while (columnIndex < 5 && (pos = line.find(delimiter)) != std::string::npos) {
		if (line.substr(0, pos) == "") {
			line.erase(0, pos + delimiter.length());
			continue;
		}
		boardRow[columnIndex] = std::stoi(line.substr(0, pos));
		line.erase(0, pos + delimiter.length());
		++columnIndex;
	}
	boardRow[4] = std::stoi(line);
}

void ParseBingoBoards(ifstream& file, vector < array<array<int, 5>, 5>> &boards) {
	string line;
	int indexRow = 0;
	array<array<int, 5>, 5> board;

	while (std::getline(file, line)) {
		if (line == "") {
			continue;
		}
		else {
			int boardRow[5];
			SplitBoardRow(line, " ", boardRow);
			board[indexRow][0] = boardRow[0];
			board[indexRow][1] = boardRow[1];
			board[indexRow][2] = boardRow[2];
			board[indexRow][3] = boardRow[3];
			board[indexRow][4] = boardRow[4];
			++indexRow;

			if (indexRow == 5) {
				indexRow = 0;
				boards.push_back(board);
			}
		}
	}
}

bool HasBingo(std::vector<int> drawNumbers, int(&line)[5]) {
	int matchCount = 0;
	for (int i = 0; i < 5 ; ++i) {
		for (int value : drawNumbers) {
			if (line[i] == value) {
				++matchCount;
				break;
			}
		}
	}
	return matchCount == 5;
}

bool ArrayContainsNumber(std::vector<int> arr, int number) {
	for (auto &value : arr) {
		if (value == number) {
			return true;
		}
	}
	return false;
}

int CalculateScore(array<array<int, 5>, 5> &board, std::vector<int> currentDrawNumbers, int lastNumberDrawn) {
	int SumUnmarked = 0;

	for (auto &row : board) {
		for (int value : row) {
			if (!ArrayContainsNumber(currentDrawNumbers, value)) {
				SumUnmarked += value;
			}
		}
	}

	return SumUnmarked * lastNumberDrawn;
}

int FindBingo(std::vector<int> drawNumbers, vector<array<array<int, 5>, 5>> &boards, bool findLastBingo = false) {
	int currentBingoScore = 0;
	std::vector<int> currentDrawNumbers;
	for (int i = 0; i < 100; ++i) {							// Increase DrawNumber
		currentDrawNumbers.push_back(drawNumbers[i]);
		auto j = std::begin(boards);
		while (j != std::end(boards)) {
			bool BingoFound = false;
			auto board = *j;
			for (int k = 0; k < 5; k++) {
				int column[5] = { board[k][0], board[k][1], board[k][2], board[k][3], board[k][4] };
				int row[5] = { board[0][k], board[1][k], board[2][k], board[3][k], board[4][k] };
				if (HasBingo(currentDrawNumbers, column) || HasBingo(currentDrawNumbers, row)) {
					currentBingoScore = CalculateScore(board, currentDrawNumbers, drawNumbers[i]);

					if (!findLastBingo) {
						return currentBingoScore;
					}

					j = boards.erase(j);
					BingoFound = true;
					break;
				}
			}

			if (BingoFound) {
				continue;
			}
			++j;
		}
	}
	return currentBingoScore;
}


void AoCFour() {
	vector<int> drawNumbers; 
	vector<array<array<int,5>,5>> boards;
	int bingoScore = 0;


	std::ifstream infile("InputFiles/inputAoC_04.txt");
	if (infile.is_open()) {
		ParseDrawNumbers(infile, drawNumbers);
		ParseBingoBoards(infile, boards);
		bingoScore = FindBingo(drawNumbers, boards, false);
		infile.close();
	}

	cout << "AOC 04" << endl;
	cout << "Bingo Score: " << bingoScore << endl << endl;
}

#pragma endregion AOC_04

#pragma region AOC_05

void ParseLines(ifstream& file, array<array<int,1000>,1000> &pointCloud) {
	string fileLine;
	for (int i = 0; i < 1000; ++i) {
		for (int j = 0; j < 1000; ++j) {
			pointCloud[i][j] = 0;
		}
	}

	auto parseStringLineToLine = [](string &fileLine) {
		auto parseStringPoint = [](string pointLine, Line &line, bool isStart) {
			std::string delimiterPoints = ",";
			size_t posDelimPoint = pointLine.find(delimiterPoints);
			int x,y;

			x = std::stoi(pointLine.substr(0, posDelimPoint));
			y = std::stoi(pointLine.substr(posDelimPoint + delimiterPoints.length(), std::string::npos));

			line.SetPoint(x, y, isStart);
		};

		Line line;
		std::string delimiterLine = " -> ";
		size_t posDelimLine = posDelimLine = fileLine.find(delimiterLine);

		parseStringPoint(fileLine.substr(0, posDelimLine), line, true);
		parseStringPoint(fileLine.substr(posDelimLine + delimiterLine.length(), std::string::npos), line, false);

		return line;
	};

	auto addLineToPointCloud = [](Line &line, array<array<int, 1000>, 1000>& pointCloud) {
		if (!line.IsValid()) return;

		if (line.IsDiagonal45()) {		
			int iterations = line.DistanceDiagonal45();
			for (int i = 0; i <= iterations; ++i) {
				int xCoord = line.x1 + i * ((line.x2 - line.x1) / iterations);
				int yCoord = line.y1 + i * ((line.y2 - line.y1) / iterations);
				pointCloud[xCoord][yCoord] += 1;
			}
		}

		else if (line.IsHorizontal()) {
			int start = (line.x1 < line.x2 ? line.x1 : line.x2);
			int end = (line.x2 > line.x1 ? line.x2 : line.x1);;

			for (int i = start; i <= end; ++i) {
				pointCloud[i][line.y1] += 1;
			}
		}

		else if (line.IsVertical()) {
			int start = (line.y1 < line.y2 ? line.y1 : line.y2);
			int end = (line.y2 > line.y1 ? line.y2 : line.y1);;

			for (int j = start; j <= end; ++j) {
				pointCloud[line.x1][j] += 1;
			}
		}
	};

	while (std::getline(file, fileLine)) {
		Line line = parseStringLineToLine(fileLine);
		addLineToPointCloud(line, pointCloud);	
	}
}

int MultiLineOverlaps(array<array<int, 1000>, 1000>& pointCloud) {
	int overlaps = 0;
	for (int i = 0; i < 1000; ++i) {
		for (int j = 0; j < 1000; ++j) {
			if(pointCloud[i][j] >= 2) overlaps++;
		}
	}
	return overlaps;
}

void AoCFive() {
	std::ifstream infile("InputFiles/inputAoC_05.txt");
	array<array<int, 1000>, 1000>* pointCloud = new array<array<int, 1000>, 1000>;
	int overlaps = 0;
	if (infile.is_open()) {
		ParseLines(infile, *pointCloud);
		overlaps = MultiLineOverlaps(*pointCloud);
		infile.close();
	}
	delete pointCloud;
	cout << "AOC 05" << endl;
	cout << "Overlaps: " << overlaps << endl;
}

#pragma endregion AOC_05

int main() {
	AoCOne();
	AoCTwo();
	AoCThree();
	AoCFour();
	AoCFive();
	cin.get();
	return 0;
}