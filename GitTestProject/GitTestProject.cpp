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

//This section will contain temporary code that will prototype the code for the comprehension




std::vector<std::vector<int>> test_line_detection (int initial_x, int initial_y, Bitmap inputMap) {
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

	for (int loop_number = 0;;loop_number++) {//temp placeholder in for loop
		
		//find the last vector in the vector list
		int lastIndex = line_vectors_x.size() - 1;
		
		//spread_vector
		
		std::vector<int>& spreadVectorX = line_vectors_x.at(lastIndex);
		std::vector<int>& spreadVectorY = line_vectors_y.at(lastIndex);
		
		//previous vector - include if first round

		int prevVectorIndex = 0;

		if (loop_number != 0) {
			prevVectorIndex = lastIndex-1;
		}
		else {
			prevVectorIndex = lastIndex;
		}

		std::vector<int>& previousVectorX = line_vectors_x.at(prevVectorIndex);
		std::vector<int>& previousVectorY = line_vectors_y.at(prevVectorIndex);

		//create vector buffer
		line_vectors_x.push_back(std::vector<int>());
		line_vectors_y.push_back(std::vector<int>());

		//vector buffer
		std::vector<int>& bufferVectorX = line_vectors_x.at(lastIndex);
		std::vector<int>& bufferVectorY = line_vectors_y.at(lastIndex);

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
					if (testCoordinateValue >= inkThreshold) {
						// check that coordinates that we are checking has not already been evaluated, check coord buffer, spread_vector(list we are currently eval on) and previous coords
						//note that nested ifs could be replaced by && statements, but are done like this to improve readability
						if (checkForCoordinatesRepeat(spreadVectorX, spreadVectorY, testXCoordinate, testYCoordinate == false)) {
							if (checkForCoordinatesRepeat(previousVectorX, previousVectorY, testXCoordinate, testYCoordinate == false)) {
								if (checkForCoordinatesRepeat(bufferVectorX, bufferVectorY, testXCoordinate, testYCoordinate == false)) {
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
	}

	return (line_vectors_x, line_vectors_y);
}

int first_pixel_detect(Bitmap inputMap) {
	for (int x = 0; x < inputMap.width; x++) {
		//std::cout << std::to_string(x) << "\n";
		for (int y = 0; y < inputMap.height; y++) {
			if (inputMap.getValueAt(x, y) >= inkThreshold) {
				return (x, y);
			}
		}
	}
}

void printVectorsToCSV(std::string filename, std::vector<std::vector<int>> vectors) {
	
	std::string outputString;
	int stringPointRef = 0;
	int allVectorsSize = 0;

	for (int i = 0; i <= vectors.size(); i++) {
		for (int j = 0; j <= vectors.at(i).size(); j++) {
			allVectorsSize++;
		}
	}

	char* preStringChar;
	preStringChar = new char[(allVectorsSize + 1) * 4];

	std::string cellBuffer;
	cellBuffer.reserve(10);

	for (int i = 0; i <= vectors.size(); i++) {
		for (int j = 0; j <= vectors.at(i).size(); j++) {
			cellBuffer = std::to_string(vectors.at(i).at(j));

		}
	}
}

//---------------------------------------------------------------------------------------------------------
int inkThreshold = 120;
//int bufferSize = 5;

void highlightEdges(Bitmap &inputMap) {

	for (int x = 0; x < inputMap.width; x++) {
		//std::cout << std::to_string(x) << "\n";
		for (int y = 0; y < inputMap.height; y++) {
			int pointValue = inputMap.getValueAt(x, y);
			bool edge = false;
			if (pointValue <= inkThreshold) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {

						int testPointVal = inputMap.getValueAt(x + i, y + j);

						if (testPointVal >= 120 && testPointVal < 300) {
							if (!(i == 0 && j == 0)) {
								edge = true;
								break;
							}
						}
					
					}
				}

			}
			if (edge != true && pointValue <= inkThreshold) {
				inputMap.changeValueAt(x, y, 301);
			}
		}
	}

	for (int x = 0; x < inputMap.width; x++) {
		for (int y = 0; y < inputMap.height; y++) {
			
			int pointValue = inputMap.getValueAt(x, y);
			
			if (pointValue > 120) {
				inputMap.changeValueAt(x, y, 254);
			}
		}
	}
}


void testAlgorithms(Bitmap &map) {

}

bool checkIntegerVectorForValue(std::vector<int> vector_check, int valueToCheck) {
	for (int i = 0; i <= vector_check.size(); i++) {
		if (vector_check.at(i) == valueToCheck) {
			return (true);
		}
	}
	return (false);
}

bool checkForCoordinatesRepeat(std::vector<int> x_coords, std::vector<int> y_coords, int x, int y) {
	if (checkIntegerVectorForValue(x_coords, x) == true && checkIntegerVectorForValue(y_coords, y) == true) {
		return true;
	}
	else
	{
		return false;
	}
}

void insertVectorEraseSource(std::vector<int> &receiveVector, std::vector<int> &sourceVector) {
	receiveVector.insert(receiveVector.end(), sourceVector.begin(), sourceVector.end());
	sourceVector.erase(sourceVector.begin(), sourceVector.end());
}

void detect_line_pixels(int initial_x, int initial_y, Bitmap inputMap) {
	//spread vector contains pixels on which spread evaluations should be conducted, i.e. the pixels on the edge of the line that has been evaluated thus far.
	std::vector<int> spread_vector_x;
	std::vector<int> spread_vector_y;

	std::vector<int> line_coords_x;
	std::vector<int> line_coords_y;

	std::vector<int> previous_coords_x;
	std::vector<int> previous_coords_y;

	std::vector<int> coordinates_buffer_x;
	std::vector<int> coordinates_buffer_y;

	double line_r_value = 0;
	double line_b = 0;
	double line_a = 0;

	for (;;) { //at some point some kind of sanity check will have to be implemented, also need to have an end condition when spread coord is empty (while(spread_coords.size() > 0) loop replace for?)
		for (int n = 0; n <= spread_vector_x.size(); n++) {
			//look at surrounding pixels, evaluate whether black. If black, add to spread vector, else dont. previous_coords->line_coords, fhift all from this loop of (;;) into previous_coords

			int x = spread_vector_x.at(n);
			int y = spread_vector_y.at(n);

			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					int testXCoordinate = x + i;
					int testYCoordinate = y + j;
					
					int testCoordinateValue = inputMap.getValueAt(testXCoordinate, testYCoordinate);
					//check that coordinates that we are checking has not already been evaluated, check coord buffer, spread_vector (list we are currently eval on) and previous coords
					//note that nested ifs could be replaced by && statements, but are done like this to improve readability
					if (testCoordinateValue >= inkThreshold) {
						if (checkForCoordinatesRepeat(spread_vector_x, spread_vector_y, testXCoordinate, testYCoordinate) == false) {//evaluate checkIntegerVectorForValue
							if (checkForCoordinatesRepeat(previous_coords_x, previous_coords_y, testXCoordinate, testYCoordinate) == false) {
								if (checkForCoordinatesRepeat(coordinates_buffer_x, coordinates_buffer_y, testXCoordinate, testYCoordinate) == false) {
									coordinates_buffer_x.push_back(testXCoordinate);
									coordinates_buffer_y.push_back(testYCoordinate);
								}
							}
						}
					}
				}
			}

		} //here each round of evalution is complete, and below buffer moving takes place



		// after all spread coords have been evaluated, and coord buffers filled, move each buffer to the buffer above it. line_coords <- previous_coords <- spread_vector <- buffer
		line_coords_x.insert(line_coords_x.end(), previous_coords_x.begin(), previous_coords_x.end());
		
		//lineCoords <- previousCoords
		insertVectorEraseSource(line_coords_x, previous_coords_x);
		insertVectorEraseSource(line_coords_y, previous_coords_y);
		
		//previous_coords <- spread_vector
		insertVectorEraseSource(previous_coords_x, spread_vector_x);
		insertVectorEraseSource(previous_coords_y, spread_vector_y);

		//if source vector 
		if (coordinates_buffer_x.size() == 0) {
			break;
		}

		//spread_vector <- buffer
		insertVectorEraseSource(spread_vector_x, coordinates_buffer_x);
		insertVectorEraseSource(spread_vector_y, coordinates_buffer_y);

	}
	//end for (;;)
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

	firstXCoord, firstYCoord = first_pixel_detect(newMap);





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
