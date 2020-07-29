// GitTestProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include "test_header.h"
#include <cerrno>

//This function takes a filename (string) and returns the contents of the file in the form of a string.
std::string get_file_contents(std::string filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

class BmpInputBuffer {
	//extremely important for this class is that memory does not need to be re-assigned. As such, the length of the string must not be modified.
	private:
		std::string buff = "aaa"; // note, stoi will return values ignoring non-number
		int stringIndex = 0;
	public:
		int getValues();
		void addValues(char stringValue);
		void bufferReset();
};
 
int BmpInputBuffer::getValues() {
	return std::stoi(buff);
}

void BmpInputBuffer::addValues(char stringValue) {
	buff[stringIndex] = stringValue;
	stringIndex++;
}

void BmpInputBuffer::bufferReset() {
	buff = "aaa";
	stringIndex = 0;
}

//////////-------------------------------------------------------------------------------------------------

class Bitmap {
	private:
		//blah is a placeholder variable, affects sod all
		int blah = 9;

	public:
		std::vector<std::vector<int>> bitmap_values;//cannot preset size
		int width = 0;
		int height = 0;
	
		Bitmap(std::string);
		int getValueAt (int x, int y);
		void changeValueAt(int x, int y, int to_value);
		void printMapToCSV ();

		//need function to grab and edit cells
		


	//constructor()
	//destructor()
};

Bitmap::Bitmap(std::string filename) {//constructor
	//load file in the form of a string
	std::string imageString = get_file_contents(filename);

	//csv comprehension

	//counts the number of commas and \n
	int numberOfCommas = std::count(imageString.begin(), imageString.end(), ',');
	int numberOfLines = std::count(imageString.begin(), imageString.end(), '\n');

	width = (numberOfCommas / numberOfLines)+ 1;
	height = numberOfLines;
	std::cout << std::to_string(width) << " - " << std::to_string(height) << "\n";
	
	//sets vector size
	bitmap_values = std::vector<std::vector<int>> (width, std::vector<int>(height));
	
	//iterates through the vector array
	int stringIterator = 0;
	char buff [3];

	BmpInputBuffer pixelBuffer;

	std::cout << "vector shape - " << std::to_string(bitmap_values.size()) << "x" << std::to_string(bitmap_values.at(0).size()) << "\n";

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			pixelBuffer.bufferReset();
			for (;;) {
				if ((imageString[stringIterator] != ',') && (imageString[stringIterator] != '\n')) {
					pixelBuffer.addValues(imageString[stringIterator]);
					stringIterator++;
					continue;
				}
				if (imageString[stringIterator] == ',') {
					stringIterator++;
					break;
				}
				if (imageString[stringIterator] == '\n') {
					stringIterator++;
					break;
				}
			}
			//std::cout << std::to_string(stringIterator) << " : ";
			//std::cout << std::to_string(i) << "," << std::to_string(j) << " - " << std::to_string(width) << "\n";
			bitmap_values.at(i).at(j) = pixelBuffer.getValues();
			pixelBuffer.bufferReset();
		}
		//std::cout << std::to_string(stringIterator) << " : ";
		//std::cout << std::to_string(0) << "," << std::to_string(j) << " - " << std::to_string(width) << "\n";
	}

	std::cout << "vector shape - " << std::to_string(bitmap_values.size()) << "x" << std::to_string(bitmap_values.at(0).size());

	std::cout << "\n" << std::to_string(width) << "x" << std::to_string(height) << "\n";
	
}

int Bitmap::getValueAt(int x, int y) {
	return bitmap_values.at(x).at(y);
}

void Bitmap::changeValueAt(int x, int y, int to_value) {
	bitmap_values.at(x).at(y) = to_value;
}

void Bitmap::printMapToCSV () {
	
	int arraySize = (width * height * 4) + width;
	int arrayIterator = 0;
	
	char * preStringPoint;
	preStringPoint = new char[arraySize];

	std::string cellBuffer;
	cellBuffer.reserve(10);

	int iterator_number = 0;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			iterator_number++;
			//std::cout << std::to_string(x) << "," << std::to_string(y) << "\n";
			cellBuffer = std::to_string(bitmap_values.at(x).at(y));
			for (int i = 0; i < cellBuffer.size(); i++) {
				preStringPoint[arrayIterator] = cellBuffer[i];
				arrayIterator++;
			}
			if (x == width - 1) {
				preStringPoint[arrayIterator] = '\n';
				arrayIterator++;
				if (y == height - 1) {
					preStringPoint[arrayIterator] = NULL;
					break;
				}
			}
			else {
				preStringPoint[arrayIterator] = ',';
				arrayIterator++;
			}
		}
	}

	std::cout << "done!";
	std::string outputString = std::string(preStringPoint);
		
	std::ofstream outFile;
	outFile.open("csv_edges.csv");
	outFile << outputString;
	outFile.close();
}

////---------------------------------------------------------------------------------------------------------

int inkThreshold = 120;
//int bufferSize = 5;

void highlightEdges(Bitmap inputMap) {
	for (int x = 0; x < inputMap.width; x++) {
		for (int y = 0; y < inputMap.height; y++) {
			
			int pointValue = inputMap.getValueAt(x, y);
			bool edge = false;
			
			if (pointValue <= inkThreshold) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; i++) {
						
						if (inputMap.getValueAt(x+i, y+j) >= 120) {
							edge = true;
							break;
						}
					}
					break;
				}
			}

			if (edge != true) {
				inputMap.changeValueAt(x, y, 300);
			}
		}
	}
	for (int x = 0; x < inputMap.width; x++) {
		for (int y = 0; y < inputMap.height; y++) {
			
			int pointValue = inputMap.getValueAt(x, y);
			
			if (pointValue >= 300) {
				inputMap.changeValueAt(x, y, 200);
			}
		}
	}
}

void testAlgorithms(Bitmap map) {
	map.changeValueAt(0, 0, 1234);
	std::cout << std::to_string(map.getValueAt(0, 0)) << "-2\n";
}

int main()
{
	Bitmap newMap("test-csv.csv");
	highlightEdges(newMap);
	newMap.changeValueAt(0, 0, 201);
	std::cout << std::to_string(newMap.getValueAt(0,0)) << "-1\n";
	testAlgorithms(newMap);
	std::cout << std::to_string(newMap.getValueAt(0, 0)) << "-36\n";
	//newMap.printMapToCSV();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
