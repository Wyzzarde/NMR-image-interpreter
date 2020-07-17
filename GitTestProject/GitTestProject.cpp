// GitTestProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include "test_header.h"


#include <fstream>
#include <string>
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


class Bitmap {
	private:
		int blah = 9;

	public:
		std::vector<std::vector<int>> bitmap_values;//cannot preset size
		int width = 0;
		int height = 0;
	
		Bitmap(std::string);
		
		


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
	std::string buff{ "" };

	int string_iterator = 0;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			buff = "";
			for (;;) {
				if ((imageString[string_iterator] != ',') && (imageString[string_iterator] != '\\')) {
					buff = buff + imageString[string_iterator];
					string_iterator++;
				}
				if (imageString[string_iterator] == ',') {
					string_iterator++;
					break;
				}
				if (imageString[string_iterator] == '\\') {
					string_iterator = string_iterator+2;
					break;
				}
				//imageString.erase(0, 1);
			}
			//vector(i,j) = string buff
			bitmap_values.at(i).at(j) = std::stoi(buff);
			stringIterator++;
		}
		std::cout << std::to_string(stringIterator) << " : ";
		std::cout << std::to_string(0) << "," << std::to_string(j) << " - " << std::to_string(width) << "\n";
	}

	std::cout << "vector shape - " << std::to_string(bitmap_values.size()) << "x" << std::to_string(bitmap_values.at(0).size());

	std::cout << "\n" << std::to_string(width) << "x" << std::to_string(height);
	
}



int main()
{	
	Bitmap newMap("test-csv.csv");
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
