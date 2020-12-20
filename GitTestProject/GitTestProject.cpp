// GitTestProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include "test_header.h"
#include <cerrno>


//--------------------------------------------------------------------------------------------------
//function and class declarations
void printVectorsToCSV(std::string filename, std::vector<std::vector<int>> vectors);
class Bitmap;
std::vector<int> first_pixel_detect(Bitmap inputMap);

//--------------------------------------------------------------------------------------------------
//global variables - yes they are evil. They are here so that they can be set by the user.
// I am aware they should be values, but wth.

int inkThreshold = 120;

//top height functions
bool checkIntegerVectorForValue(std::vector<int> vector_check, int valueToCheck) {
	for (int i = 0; i < vector_check.size(); i++) {
		if (vector_check.at(i) == valueToCheck) {
			return (true);
		}
	}
	return (false);
}

bool checkForCoordinatesRepeat(std::vector<int> x_coords, std::vector<int> y_coords, int x, int y) {
	if (checkIntegerVectorForValue(x_coords, x) == true && checkIntegerVectorForValue(y_coords, y) == true) {
		return (true);
	}
	else
	{	
		return (false);
	}
}


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

//////////--------------------------------------------------------------------------------------------------
//CLASS:Bitmap Input buffer

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
//CLASS:BITMAP

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
		void printMapToCSV (std::string filename);

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

void Bitmap::printMapToCSV (std::string filename) {
	
	int arraySize = (width * height * 4) + width;
	int arrayIterator = 0;
	
	char * preStringPoint;
	preStringPoint = new char[arraySize];

	std::string cellBuffer;
	cellBuffer.reserve(10);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
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
	outFile.open(filename);
	outFile << outputString;
	outFile.close();
}

////---------------------------------------------------------------------------------------------------------
//CLASS:LINE

//This class will intially only contain the coordinate vectors, but will hopefully later also contain
//lines

class Linemap {
	public:
		//Variables
		std::vector<std::vector<int>> linemapX;
		std::vector<std::vector<int>> linemapY;

		//Functions
		Linemap(std::vector<std::vector<int>> linemapx, std::vector<std::vector<int>> linemapy);

};

Linemap::Linemap(std::vector<std::vector<int>> linemapx, std::vector<std::vector<int>> linemapy) {
	linemapX = linemapx;
	linemapY = linemapY;
}


///---------------------------------------------------------------------------------------------------------
//MoLS CODE

//Detect contiguous black pixels. Output is 2 matrices (vector^2) contained in a vector. 

Linemap test_line_detection (int initial_x, int initial_y, Bitmap inputMap) {
	//These vectors contain the x and y coordinates 
	std::vector<std::vector<int>> line_vectors_x;
	std::vector<std::vector<int>> line_vectors_y;

	double line_r_value = 0;
	double line_b = 0;
	double line_a = 0;

	std::vector<int> initial_vector_x = { initial_x };
	std::vector<int> initial_vector_y = { initial_y };

	line_vectors_x.push_back(initial_vector_x);
	line_vectors_y.push_back(initial_vector_y);

	//<testcode>
	int loopOne = 0;
	//</testcode>

	for (int loop_number = 0;;loop_number++) {//temp placeholder in for loop

		//find the last vector in the vector list
		int lastIndex = line_vectors_x.size() - 1;
		
		//spread_vector
		
		std::vector<int> spreadVectorX = line_vectors_x.at(lastIndex);
		std::vector<int> spreadVectorY = line_vectors_y.at(lastIndex);
		
		if (spreadVectorX.size() == 0) {
			line_vectors_x.pop_back();
			line_vectors_y.pop_back();
			break;
		}

		//previous vector - include if first round

		int prevVectorIndex = 0;

		if (loop_number != 0) {
			prevVectorIndex = lastIndex-1;
		}
		else {
			prevVectorIndex = lastIndex;
		}

		std::vector<int> previousVectorX = line_vectors_x.at(prevVectorIndex);
		std::vector<int> previousVectorY = line_vectors_y.at(prevVectorIndex);

		//vector buffer
		std::vector<int> bufferVectorX;
		std::vector<int> bufferVectorY;

		//<testcode>
		int loopTwo = 0;
		//</testcode>

		for (int n = 0; n < spreadVectorX.size(); n++) {
		
			//get values of x and y at the point being evaluated
			int x = spreadVectorX.at(n);
			int y = spreadVectorY.at(n);

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					int testXCoordinate = x + i;
					int testYCoordinate = y + j;

					int testCoordinateValue = inputMap.getValueAt(testXCoordinate, testYCoordinate);
					//check that coordinates that we are checking has not already been evaluated, check coord buffer, spread_vector (list we are currently eval on) and previous coords
					//note that nested ifs could be replaced by && statements, but are done like this to improve readability
					if (testCoordinateValue <= inkThreshold) {
						// check that coordinates that we are checking has not already been evaluated, check coord buffer, spread_vector(list we are currently eval on) and previous coords
						//note that nested ifs could be replaced by && statements, but are done like this to improve readability
						if (checkForCoordinatesRepeat(spreadVectorX, spreadVectorY, testXCoordinate, testYCoordinate) == false) {
							if (checkForCoordinatesRepeat(previousVectorX, previousVectorY, testXCoordinate, testYCoordinate) == false) {
								if (checkForCoordinatesRepeat(bufferVectorX, bufferVectorY, testXCoordinate, testYCoordinate) == false) {
									bufferVectorX.push_back(testXCoordinate);
									bufferVectorY.push_back(testYCoordinate);
								}
							}
						}
					}

					//after this, the new coordinate has been added to the latest vector
					
				}
			}

		}

		line_vectors_x.push_back(bufferVectorX);
		line_vectors_y.push_back(bufferVectorY);
		
	}

	Linemap returnLinemap(line_vectors_x, line_vectors_y);


	return (returnLinemap);
}

//--------------------------------------------------------------------------------------------------------

std::vector<Linemap> detectLinemaps(Bitmap& inputMap) {
	
	Bitmap inputMapCopy = inputMap;

	std::vector<Linemap> linemapVector;

	for (;;) {//this loop will loop through a series of black pixel blocks, and will attempt to detect linemaps
		// a linemap is defined as a matrix that will 
		
		std::vector<int> initial_pixels = first_pixel_detect(inputMapCopy);//returns a vector with x,y coordinates of first detected black pixel.

		if (initial_pixels.size() == 0) {//check that there were black pixels detected
			break;
		}
		
		//initial pixels of block
		int initialXCoord = initial_pixels.at(0);
		int initialYCoord = initial_pixels.at(1);

		Linemap sq_vectors = test_line_detection(initialXCoord, initialYCoord, inputMap);//get all the x and y coordinates of this block of black pixels. Input - initial x and y coordinates, input map-
		//- is the map to explore on.

		std::vector<std::vector<int>> lineMapX = sq_vectors.linemapX;
		std::vector<std::vector<int>> lineMapY = sq_vectors.linemapY;
	
		for (int i = 0; i < lineMapX.size(); i++) {
			for (int j = 0; j < lineMapX.at(i).size(); j++) {
				int x = lineMapX.at(i).at(j);
				int y = lineMapY.at(i).at(j);

				inputMapCopy.bitmap_values.at(x).at(y) = 254;
			}
		}
		
		linemapVector.push_back(sq_vectors);
	}

	return (linemapVector);
}


//--------------------------------------------------------------------------------------------------------
//Utility functions

void printVectorsToCSV(std::string filename, std::vector<std::vector<int>> vectors) {

	std::string outputString;
	int stringPointRef = 0;
	int allVectorsSize = 0;

	for (int i = 0; i < vectors.size(); i++) {
		for (int j = 0; j < vectors.at(i).size(); j++) {
			allVectorsSize++;
		}
	}



	char* preStringChar;
	int preStringCharLength = (allVectorsSize + 1) * 4;
	preStringChar = new char[preStringCharLength];
	int arrayIterator = 0;


	std::string cellBuffer;
	cellBuffer.reserve(10);


	for (int i = 0; i < vectors.size(); i++) {
		for (int j = 0; j < vectors.at(i).size(); j++) {
			cellBuffer = std::to_string(vectors.at(i).at(j));
			for (int n = 0; n < cellBuffer.size(); n++) {
				preStringChar[arrayIterator] = cellBuffer[n];
				arrayIterator++;
			}
			if (j < vectors.at(i).size() - 1) {
				preStringChar[arrayIterator] = ',';
				arrayIterator++;
			}
			if (j == vectors.at(i).size() - 1) {
				preStringChar[arrayIterator] = '\n';
				arrayIterator++;
			}
		}
	}

	preStringChar[arrayIterator] = NULL;

	std::cout << "done char_array";
	std::string VectorOutputString = std::string(preStringChar);

	std::ofstream outFile;
	outFile.open(filename);
	outFile << VectorOutputString;
	outFile.close();
}

std::vector<int> first_pixel_detect(Bitmap inputMap) {
	for (int x = 0; x < inputMap.width; x++) {
		//std::cout << std::to_string(x) << "\n";
		for (int y = 0; y < inputMap.height; y++) {
			if (inputMap.getValueAt(x, y) <= inkThreshold) {

				std::vector<int> returnVector = { x,y };

				return (returnVector);
			}
		}
	}

	std::vector<int> empty = {};

	return (empty);
}

//--------------------------------------------------------------------------------------------------------





void testAlgorithms(Bitmap &map) {

}

void method_of_least_squares(Bitmap inputMap) {//need to add to bitmap class line containers. 

}


 //detect first pixel, complete


int main()
{
	Bitmap newMap("test-csv.csv");
	//highlightEdges(newMap);
	//std::cout << std::to_string(newMap.getValueAt(0,0)) << "-1\n";
	//newMap.printMapToCSV("csv_edges.csv");
	
	int firstXCoord = 0;
	int firstYCoord = 0;

	std::vector<int> firstPixelCoord  = first_pixel_detect(newMap);

	firstXCoord = firstPixelCoord.at(0);
	firstYCoord = firstPixelCoord.at(1);

	Linemap sq_vectors = test_line_detection(firstXCoord, firstYCoord, newMap);
	
	std::vector<std::vector<int>> test_sqvectorX = sq_vectors.linemapX;
	std::vector<std::vector<int>> test_sqvectorY = sq_vectors.linemapY;

	for (int i = 0; i < test_sqvectorX.size(); i++) {
		int temp_int = test_sqvectorX.at(i);
		std::cout << std::to_string(temp_int);
	}
	

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
