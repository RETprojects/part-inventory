// This program lets the user search for a part number within a file. If the part number is there, the program returns stats about the
// part. If not, the user may give those stats about the part and write them to the file.
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// fills vectors
bool get_data(vector <string>& part_number, vector <char>& part_class, vector <int>& part_ohb, vector <double>& part_cost);

// Does a binary search  
int bin_search(string key, const vector<string>& part_number);

// Asks user for a part number to search for
string get_target();

// gets remaining info to add a part number
void get_more_data(char& class_in, int& part_ohb_in, double& part_cost_in);

// Inserts part number data into vectors 
void insert_data(vector <string>& part_number, vector <char>& part_class, vector <int>& part_ohb, vector <double>& part_cost, string part_in, char class_in, int part_ohb_in, double part_cost_in);

// Displays info on part number
void display(const vector <string>& part_number, const vector <char>& part_class, const vector <int>& part_ohb, const vector <double>& part_cost, int finder);

// sorts vectors (Calls swapper) 
void sort(vector <string>& part_number, vector <char>& part_class, vector <int>& part_ohb, vector <double>& part_cost);

// prints search stats
void print_stats(int searches, int good, int bad);

// writes out file when program ends, so additions are saved to file
void put_data(const vector <string>& part_number, const vector <char>& part_class, const vector <int>& part_ohb,
	const vector <double>& part_cost);

// templated swap function – Swaps two items in a vector of any type
// Put this BEFORE main()  Called from sort function
template <class CType>
void swapper(CType& a, CType& b)
{
	CType temp;
	temp = a;
	a = b;
	b = temp;
}

int main()
{
	cout << "Loading..." << endl;
	// Define parallel vectors:
	vector<string> part_number;
	vector<char> part_class;
	vector<int> part_ohb;
	vector<double> part_cost;

	get_data(part_number, part_class, part_ohb, part_cost);
}
// This function fills the vectors.
bool get_data(vector <string>& part_number, vector <char>& part_class, vector <int>& part_ohb, vector <double>& part_cost)
{
	bool outcome;
	ifstream inputFile;
	inputFile.open("parts.txt");
	// The program reads the file's contents into parallel vectors.
	if (inputFile)
	{
		outcome = true;
		// This while loop reads every line of the file into parallel vectors.
		string line;
		while (getline(inputFile, line, '\n'))
		{
			// The part number is the first set of characters, ending before the first space.
			int space;
			space = line.find(' ', 0);
			part_number.push_back(line.substr(0, space));
			line = line.substr(space + 1, line.length() - space - 1);
			// The class is just the first character of this new string.
			part_class.push_back(line[0]);
			line = line.substr(2, line.length() - 2);
			// Then the program finds the on hand balance.
			space = line.find(' ', 0);
			part_ohb.push_back(stoi(line.substr(0, space)));
			line = line.substr(space + 1, line.length() - space - 1);
			// Finally, the rest of the line is the cost.
			part_cost.push_back(stod(line));
		}
	}
	else
		outcome = false;

	// The part numbers are then sorted:
	sort(part_number, part_class, part_ohb, part_cost);

	// Search stats:
	int searches = 0;
	int good = 0;
	int bad = 0;

	// The user can search for parts within this do-while loop:
	char answer = 'Y';
	do {
		string number = get_target();// asks user for part number to search for

		int pos = bin_search(number, part_number);
		searches++;
		if (pos != -1)// if the part number has been found in the file
		{
			good++;
			display(part_number, part_class, part_ohb, part_cost, pos);
		}
		else// if the part number was not found
		{
			bad++;
			cout << "Add this part?" << endl;
			char add_it;
			cin >> add_it;
			if (add_it == 'Y' || add_it == 'y')
			{
				char class_in;
				int part_ohb_in;
				double part_cost_in;
				// allows the user to enter the stats of the part:
				get_more_data(class_in, part_ohb_in, part_cost_in);
				// inserts the data into parallel vectors:
				insert_data(part_number, part_class, part_ohb, part_cost, number, class_in, part_ohb_in, part_cost_in);
			}
		}
		cout << "Would you like to do another search? (Y or N)" << endl;
		cin >> answer;
	} while (answer == 'Y' || answer == 'y');
	// The search stats are displayed:
	print_stats(searches, good, bad);
	// The additions are written to the file.
	inputFile.close();
	// updates the file and saves additions:
	put_data(part_number, part_class, part_ohb, part_cost);
	
	return outcome;
}
// This function performs a binary search.
// Note: this is a modified version of the code on page 467.
int bin_search(string key, const vector<string>& part_number)
{
	int first = 0;
	int last = part_number.size() - 1;
	int middle;
	int position = -1;
	bool found = false;

	while (!found && first <= last)
	{
		middle = (first + last) / 2;
		if (part_number[middle] == key)
		{
			found = true;
			position = middle;
		}
		else if (part_number[middle] > key)
			last = middle - 1;
		else
			first = middle + 1;
	}
	return position;
}
// This function asks the user for a part number to search for.
string get_target()
{
	string number;
	cout << "What part number would you like to look for?" << endl;
	cin >> number;
	return number;
}
// This function gets the remaining information to add a part number to the vectors.
void get_more_data(char& class_in, int& part_ohb_in, double& part_cost_in)
{
	cout << "What class is this part number in?" << endl;
	cin >> class_in;
	cout << "What is the on hand balance of this part?" << endl;
	cin >> part_ohb_in;
	cout << "What is the cost of this part?" << endl;
	cin >> part_cost_in;
}
// This function inserts the part number's data into the vectors.
void insert_data(vector <string>& part_number, vector <char>& part_class, vector <int>& part_ohb, vector <double>& part_cost, string part_in, char class_in, int part_ohb_in, double part_cost_in)
{
	part_number.push_back(part_in);
	part_class.push_back(class_in);
	part_ohb.push_back(part_ohb_in);
	part_cost.push_back(part_cost_in);
	
	// Then the data are inserted into their proper places:
	sort(part_number, part_class, part_ohb, part_cost);
}
// This function displays the information on a part number.
void display(const vector <string>& part_number, const vector <char>& part_class, const vector <int>& part_ohb, const vector <double>& part_cost, int finder)
{
	cout << "There are " << part_ohb[finder] << " of Part Number " << part_number[finder] << " in inventory." << endl;
	cout << "It is a class " << part_class[finder] << " part. The cost is $" << part_cost[finder] << "." << endl;
	cout << "The value of that inventory is $" << (part_ohb[finder] * part_cost[finder]) << "." << endl;
}
// This function sorts the vectors by calling the swapper() function.
// Note: this was modified from code from the Shell Sort page in Module 15.
void sort(vector <string>& part_number, vector <char>& part_class, vector <int>& part_ohb, vector <double>& part_cost)
{
	// Part numbers:
	bool flag = true;
	int i;
	int d = part_number.size();
	while (flag || (d > 1)) // bool flag
	{
		flag = false;  //set flag to false    
		d = (d + 1) / 2;
		for (i = 0; i < (part_number.size() - d); i++)
		{
			if (part_number[i + d] < part_number[i])
			{
				swapper(part_number[i + d], part_number[i]);
				// The other parallel vectors are sorted based on how the part_number vector was sorted:
				swapper(part_class[i + d], part_class[i]);
				swapper(part_ohb[i + d], part_ohb[i]);
				swapper(part_cost[i + d], part_cost[i]);
				flag = true; //tells swap has occurred
			}
		}
	}
}
// This function prints the search stats.
void print_stats(int searches, int good, int bad)
{
	cout << "There were " << searches << " searches performed today." << endl;
	cout << good << " of them were successful." << endl;
	cout << bad << " of them were not in the system." << endl;
}
// When the program ends, this function writes the additions to the file.
void put_data(const vector <string>& part_number, const vector <char>& part_class, const vector <int>& part_ohb,
	const vector <double>& part_cost)
{
	// The output file is the same file as the input file.
	ofstream outputFile;
	outputFile.open("parts.txt");
	// The parallel vectors are written to the file, line by line.
	for (int i = 0; i < part_number.size(); i++)
	{
		outputFile << part_number[i] << " " << part_class[i] << " " << part_ohb[i] << " " << part_cost[i] << endl;
	}
	cout << "The file is now updated." << endl;
}