#include <iostream>
#include <fstream>
#include "print.h"
using namespace std;

static const string PATH_TO_ASCII = "bodyparts.txt";
static const string CSI = "\033[";
static const int HEIGHT = 15;

class BodyPart {

private:

    // Stores the image in a dynamic array of lines for the image
    string *image;

    // Name of the object
    string name;

    // Dimensions of the object
    int yDimension;
    int xDimension;

    // Where the object should be printed on the screen
    int yPosition;
    int xPosition;

public:

    // Initializes all member variables in the object
    BodyPart();

    // Frees all heap-allocated memory in the object
    ~BodyPart();

    /* Navigates to the position in the ifstream from which the object 
     * should be read in */
    void findBodyPart(ifstream &infile, int objectNumber);

    // Reads in a specific object from the file
    void readIn(string filename, int objectNumber);

    // Prints the object to the screen
    void print();

};

void screen_clear();
void screen_home();
void place_cursor(int row, int col);

void print_body_parts(int n)
{
    screen_clear();
    for (int i = 1; i <= n + 1; i++) {
        BodyPart body_part;
        body_part.readIn(PATH_TO_ASCII, i);
        body_part.print();
    }
    for (int i = 0; i < (HEIGHT-n); i++)
        cout << endl;
}

/* Constructor
 * Purpose: Initializes all member variables in the object
 * Parameters: None
 * Return Type: None
 * Errors Thrown: None */
BodyPart::BodyPart()
{
    image = NULL;
    name = "";

    yDimension = 0;
    xDimension = 0;

    yPosition = 0;
    xPosition = 0;
}

/* Destructor
 * Purpose: Frees all heap-allocated memory in the object
 * Parameters: None
 * Return Type: None
 * Errors Thrown: None */
BodyPart::~BodyPart()
{
    if (image) {
        delete []image;
        image = NULL;
    }
}

/* findBodyPart()
 * Purpose: Navigates to the position in the ifstream from which the object 
 * should be read in
 * Parameters: Reference to an ifstream (ifstream that will be updated when 
 * items are read-in), int (which object should be read-in)
 * Return Type: void
 * Errors Thrown: runtime_error if objects could not be read in correctly */
void BodyPart::findBodyPart(ifstream &infile, int objectNumber)
{
    int count = 0;
    string check;
    while (count != objectNumber) {
        // If it reaches end-of-file before finishing reading in
        if (!(infile >> check)) {
            place_cursor(0, 0);
            cerr << "Invalid BodyPart\n";
            return;
        }
        
        if (check == "object")
            count++;
    }
}
/* readIn()
 * Purpose: Reads in a specific object from the file
 * Parameters: string (file from which the object is read-in), int (which 
 * object to read in)
 * Return Type: void
 * Errors Thrown: runtime_error if the file could not be opened */
void BodyPart::readIn(string filename, int objectNumber)
{
    ifstream infile;
    infile.open(filename.c_str());
    if (!(infile.is_open())) 
    { 
        string errorMessage = "Error opening file " + filename;
        throw runtime_error(errorMessage);
    }

    findBodyPart(infile, objectNumber);
    infile >> yDimension >> xDimension >> yPosition >> xPosition >> name;
    image = new string[yDimension];

    /* getline() has to read in a throwaway string at the beginning because 
     * the newline character has not been reached yet */
    string throwaway;
    for (int i = 0; i < yDimension + 1; i++) 
    {
        if (i == 0)
            getline(infile, throwaway);
        else 
            getline(infile, image[i - 1]);
    }

    infile.close(); 
}

/* print()
 * Purpose: Prints the object to the screen
 * Parameters: None
 * Return Type: void
 * Errors Thrown: None */
void BodyPart::print()
{
    screen_home();
    for (int i = 0; i < yDimension; i++) 
    {
        for (int j = 0; j < xDimension; j++) 
        {
            place_cursor(yPosition + i, xPosition + j);
            // Does not print whitespace
            if (image[i][j] != ' ')
                if (image[i][j] != ' ')
                    cout << image[i][j];
        }
    }
}

void screen_clear()
{
    cout << (CSI + "H");
    cout << (CSI + "2J");
}

void screen_home()
{
    cout << (CSI + "H");
}

void place_cursor(int row, int col)
{
    cout << "\033[" << (row+1) << ";" << (col+1) << "H";
    flush(cout);
}
