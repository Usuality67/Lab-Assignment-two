//Name: Syed Muhammad Ziean, ERP: 35505, Section: Akhrahmah Rashid Friday-Saturday Batch.

/*Think about it:   

  1. Change the wind to W and run again. Does the river still protect the east side? Why?
  Ans. East side is till protected, as the water cannot burn , no matter the direction the wind flows in, and hence 
  water cannot spread fire to the east side, and east side will not burn.
  
  2.Make a map where the river has a gap one cell wide. How often does the fire get across? 
  Ans. Fire never gets across before the broken cell, but at the broken cell, fire gets across to the eastern side more often.
  Since I choose the gap cell to be a tree, the first cell to catch fire is this gap cell, after which it spreads fire to its eastern neighbour.
  Which then spreads it to each of its 4 neighbours, and fire starts to ignite on the eastern side as well.

  3.Find roughly the smallest p at which the fire usually burns most of the forest.
  Ans. 0.53, at this probablity all the 185 trees, that are west to river, are burned. 
       Percentage being 41.11% trees burned, as the remaining 58.89 are on the east side, 
       where the river is blocking the fire, and hence they cannot be burned.
*/


#include <iostream> 
#include <cstdlib> 
#include <algorithm> 
#include <fstream>
#include <iomanip>

using namespace std; 

int main() {
    const int MAX_ROWS = 50;
    const int MAX_COLS = 100;
    int Symbols[MAX_ROWS][MAX_COLS]; 
    int Steps[MAX_ROWS][MAX_COLS]; //Stores the step at which a tree was ignited.
    int Symbols_Temp[3][MAX_COLS]; //Stores the forest map temporarly .
    int Line_num = 0; 
    int Row_length = 0;
    int Rows,Cols; //Grid size (Row and Columns) from the file
    int total_steps; //S or total number of steps from the file
    int Burned_steps; //stores b ,the number of steps after which a tree will turn in to ash , from the file.
    int random_seed; //The random number , to be placed inside srand() , given in the file
    double probablity_burning; //p ,probablity of a tree to successfully burn,from the file
    double probablity_intensity, temp_prob_inten; //g, probablity of a tree intensity to increase by 1 , from the file
    char symbol; //T or "." or # or intensity (1-9)
    char wind_direction; //The direction of wind (E,W,N,S) given in the file
    char symbol_check[12] {'T','.','#','1','2','3','4','5','6','7','8','9'}; //Array storing all the possible characters stored in the file
    string var_check_row;
    int Total_Trees = 0;
    int Total_Burning; 
    int Total_Ash = 0;
    
    ofstream File_write("fire_report.txt"); 
    ifstream File_read("Text_Files_Used/Text_Files_Used/forest.txt"); 

    if(!File_read) {

        cout << "File cannot be opened for reading.";
        File_write.close(); 
        return 1; 
    }
    if(!File_write) {
        cout << "File cannot be written to.";
        File_read.close();
        return 10;
    }

    for(int u = 0; u < MAX_ROWS; u++) {

        for(int o = 0; o < MAX_COLS; o++) {
            Steps[u][o] = -1; 
        }
    }

    
    
    

    while(getline(File_read,var_check_row)) {
        Line_num++; 
    }
    Line_num -= 2; 

    File_read.close(); 

    File_read.open("Text_Files_Used/forest.txt") ;
    if(!File_read) {

        cout << "File cannot be opened for reading."; 
        File_write.close(); 
        return 1; 
    }


    File_read >> Rows; 
    File_read >> Cols; 

    if(Rows > 50 || Cols > 100) {
        cout << "The rows or columns are greater than the maximum limit (50 x 100) the program could handel.";
        cout << "\n"; 
        File_read.close();
        File_write.close(); 
        return 2;
    }

    if (Rows != Line_num) {
        cout << "Rows mentioned donot match the number of actual rows in the file." << "\n"; 
        File_read.close();
        File_write.close(); 
        return 3; 
    }
    

    File_read >> total_steps; 
    File_read >> probablity_burning; 
    File_read >> probablity_intensity; 
    File_read >> Burned_steps; 
    File_read >> wind_direction; 
    File_read >> random_seed; 

    srand(random_seed); 
    
    //Check for an invalid symbol i.e symbols outside of the ones given in symbol_check array.

    while(File_read >> symbol) {
        bool char_checker = false; 
        for (int x = 0; x < 12 && !char_checker; x++) {

            if (symbol_check[x] == symbol) {
                char_checker = true; 
            }
            
        }

        if (!char_checker) {
            cout << "The symbol " << symbol << " is not a standard symbol.";
            cout << "\n";
            File_read.close();
            File_write.close(); 
            return 4; 
        } 

        }
    File_read.close();

    File_read.open("Text_Files_Used/forest.txt"); 

    if(!File_read) {
        cout << "The file forest cannot be open for reading" << "\n";
        File_write.close(); 
        return 5;

    }

    getline(File_read,var_check_row); //To flush the grid size on line 1
    getline(File_read,var_check_row); //To flush the values given on line 2

    int x = 0; // Holds Row_number
    
    
    while(getline(File_read,var_check_row)) {

        //Checking the number of columns in a line are the same as the promised columns in the first line of the file
        if(var_check_row.length() != Cols) {
            cout << "Rows " << x+1 << " has " << var_check_row.length() << " columns only. Expected " << Cols << " columns." << "\n"; 
            File_read.close();
            File_write.close(); 
            return 6;
        }
        //printing this string at the start when row read are 0
       if (x == 0) {
        cout << "Initial map:" << "\n"; 
        File_write << "Initial map:" << "\n"; 

       }
        cout << var_check_row << "\n";  //Printing the current symbol of the initial map
        
        File_write << var_check_row << "\n"; //Writing the initial symbol of the initial map

         
        for (int y = 0; y < Cols; y++){
                char temp = var_check_row[y];  //holds the current character being read from the file.

                //Assigning Numbers to each characters, and storing them in the Symbols array. 

                switch(temp) {

                case 'T': //In case the character is a Tree, mark it as 10 in the symbols array
                    Symbols[x][y] = 10; 
                    Total_Trees += 1; //Keeping track of total number of trees in the file.
                    break;
                case '#' : //In case the character is water (#) , mark it as -2 in the Symbols Array.
                    Symbols[x][y] = -2;
                    break;
                case 'A' : //In case the character is A (Ashed tree), mark it as -1 in the Symbols Array
                   Symbols[x][y] = -1;
                   break;
                case '.' : //In case there the character is ground (.), mark it as -3 in the symbols Array.
                    Symbols[x][y] = -3;
                    break;
                default: //Lastly, if the character is a burning tree (represented by an intenisty number) store it, in the symbols array, in its original form.
                    Symbols[x][y] = temp-48;  //To convert char into an integer.
                    Steps[x][y] = 0;
                    Total_Trees += 1; 
                    break;
            }
        }
        x++;
    }

    //Writing things that are being printed into a file called "fire_report.txt"
  
   int steps = 0; //Keeps track of number of steps the forest is being burning for. Must be less than total steps given in the file.
   while(steps < total_steps) {

    Total_Burning = 0; //Total number of trees currently burning
    Total_Ash = 0; //Total number of trees that have been turned into Ash

    steps++;  

    for(int x = 0; x < Rows; x++) {

        for(int y = 0; y < Cols; y++) {
            //If the tree is already burning
            if(Symbols[x][y] > 0 && Symbols[x][y] < 10) {
                if (Steps[x][y] == steps) {   //Check to see if its burning at the same step it was ignited, if yes so then move on to the next character.
                    continue; 
                } 
                    for (int r = 0; r < 3; r++) {
                        for (int c = 0; c < 100; c++) {
                            Symbols_Temp[r][c] = -9;       //Intialize all the indexes in the Symbols_temp array to -9.

                        }
                    }
                    for(int m = 0; m < 3; m++) {    //There are 3 rows in the Symbols_Temp array, so to store immediate neighbours of the burning cell

                        if(x-1 >= 0 && x+1 < Rows) { //Check if the row above and the row below the burning cell row exist

                        for(int z = 0; z < Cols; z++) {
                            if(m == 0) { //Stores the row above the burning cell row into the first row of Symbols_temp

                            Symbols_Temp[m][z] = Symbols[x-1][z];


                            }else if (m == 1) {
                            Symbols_Temp[m][z] = Symbols[x][z];  //Stores the row that has the burning cell into the second row of Symbols_Temp
 
                            }else {
                            Symbols_Temp[m][z] = Symbols[x+1][z]; //Stores the row that is below the row that has the burning cell
                        }

                        }
                    } else if (x-1 >= 0) { //if only row above the burning cell row exist then just store the burning cell row, and the row above it to the Symbol_temp array

                        for(int z = 0; z < Cols;z++) {
                            if(m == 0) {
                            Symbols_Temp[m][z] = Symbols[x-1][z]; 


                            }else if (m == 1) {
                            Symbols_Temp[m][z] = Symbols[x][z]; 

                            } else { //Donot fill if m = 2, as it already contains -9 which is the defualt value of the Symbol_temp array
                            continue;
                        }

                        }
                    } else { // if the row above the burning cell row doesnot exist only store the burning cell row, and the row below it
                        for(int z = 0; z < Cols;z++) {
                            if(m == 0) {
                            continue; 


                            } else if (m == 1) {
                            Symbols_Temp[m][z] = Symbols[x][z]; 

                            } else if (m == 2 && x+1 < Rows ){ //check to see if indeed x+1 is less than total number of rows, if not then skip this. Making the symbol_Temp array 1xColumns.
                            Symbols_Temp[m][z] = Symbols[x+1][z]; 
                        }

                    }
                }
            }
                bool zero = false;  //If the first row exist (i.e have values other than -9) then this turns to true
                bool two = false; // if third row exist, this turns to true
                bool nine = false; //if the burning cell , i nthe burning cell row, has an intensity of 9 then this turns to true

                temp_prob_inten = rand()/(RAND_MAX+1.0); 

                if(Symbols_Temp[0][y] != -9){
                    zero = true; 
                }

                if(Symbols_Temp[2][y] != -9) {
                    two = true;
                }

                if(Symbols_Temp[1][y] == 9) {
                    nine = true;
                }

                /* For all: 
                x = Row, y = col 
                Symbol_Temp[x][y] = Burning cell
                Symbol_Temp[x-1][y] = Neighbour at north
                Symbol_Temp[x+1][y] = Neighbour at south
                Symbol_Temp[x][y-1] = Neighbour at west
                Symbol_Temp[x][y+1] = Neighbout at east
                */

                if(zero && two && !nine) {  //If the burning cell intensity isnt 9.

                    switch (wind_direction) {

                        case 'E': //If wind direction is East

                            //rand()/(RAND_MAX+1.0) used to calculate the probablity for the burning of a burning cell's each neighbour
                            if (rand() / (RAND_MAX + 1.0) < probablity_burning) { //If the random probablity generated is within the passing range
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1; //Intensity for north neighbour
                            int Temp2 = (rand() % Symbols_Temp[1][y]) + 1; //Intensity for south neighbour 

                            if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) { //Check if the north neighbour has an already existing intensity that is less than the calculated intensity
                                Symbols_Temp[0][y] = Temp1; //if yes then assign the newly calculated intensity to the already burning tree
                            } else if (Symbols_Temp[0][y] == 10) { //If it doesnt have an already existing intensity but it is an unburned tree.
                                Symbols_Temp[0][y] = Temp1; //if true then assign the Temp1 intensity to the tree, as it will burn now.
                                Steps[x-1][y] = steps; //The step at which the tree was ignited is stored at its resepctive index
                            }

                            if (Symbols_Temp[2][y] < Temp2 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) { //Done similarly for the Southern neighbour
                                Symbols_Temp[2][y] = Temp2;
                            } else if (Symbols_Temp[2][y] == 10) {
                                Symbols_Temp[2][y] = Temp2;
                                Steps[x+1][y] = steps;
                            }
                        }
                        
                        if (rand() / (RAND_MAX + 1.0) < min(1.0,2.0*probablity_burning) && y+1 < Cols) { //y+1 is the eastern neighbout, and since wind direction is to the east , the probablity of ignition will succeed by 2p
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                                Symbols_Temp[1][y+1] = Temp1;
                            } else if (Symbols_Temp[1][y+1] == 10) {
                                Symbols_Temp[1][y+1] = Temp1;
                                Steps[x][y+1] = steps;
                            } 
                        }
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y-1 >= 0) { //y-1 is the western neighbour,so ignition will succeced by p/2.
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                                Symbols_Temp[1][y-1] = Temp1;
                            } else if (Symbols_Temp[1][y-1] == 10) {
                                Symbols_Temp[1][y-1] = Temp1;
                                Steps[x][y-1] = steps;
                            }
                        }
                        break;

                    case 'W':
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                                Symbols_Temp[0][y] = Temp1;
                            } else if (Symbols_Temp[0][y] == 10) {
                                Symbols_Temp[0][y] = Temp1;
                                Steps[x-1][y] = steps;
                            }
                            if (Symbols_Temp[2][y] < Temp2 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                                Symbols_Temp[2][y] = Temp2;
                            } else if (Symbols_Temp[2][y] == 10) {
                                Symbols_Temp[2][y] = Temp2;
                                Steps[x+1][y] = steps;
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning && y-1 >= 0) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                                Symbols_Temp[1][y-1] = Temp1;
                            } else if (Symbols_Temp[1][y-1] == 10) {
                                Symbols_Temp[1][y-1] = Temp1;
                                Steps[x][y-1] = steps;
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y+1 < Cols) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                                Symbols_Temp[1][y+1] = Temp1;
                            } else if (Symbols_Temp[1][y+1] == 10) {
                                Symbols_Temp[1][y+1] = Temp1;
                                Steps[x][y+1] = steps;
                            }
                        }
                        break;

                    case 'S':
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                            if (y-1 >= 0) {
                                int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                                if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                                    Symbols_Temp[1][y-1] = Temp1;
                                } else if (Symbols_Temp[1][y-1] == 10) {
                                    Symbols_Temp[1][y-1] = Temp1;
                                    Steps[x][y-1] = steps;
                                }
                            }
                            if (y+1 < Cols) {
                                int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                                if (Symbols_Temp[1][y+1] < Temp2 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                                    Symbols_Temp[1][y+1] = Temp2;
                                } else if (Symbols_Temp[1][y+1] == 10) {
                                    Symbols_Temp[1][y+1] = Temp2;
                                    Steps[x][y+1] = steps;
                                }
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[2][y] < Temp1 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                                Symbols_Temp[2][y] = Temp1;
                            } else if (Symbols_Temp[2][y] == 10) {
                                Symbols_Temp[2][y] = Temp1;
                                Steps[x+1][y] = steps;
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning/2) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                                Symbols_Temp[0][y] = Temp1;
                            } else if (Symbols_Temp[0][y] == 10) {
                                Symbols_Temp[0][y] = Temp1;
                                Steps[x-1][y] = steps;
                            }
                        }
                        break;

                    case 'N':
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                            if (y-1 >= 0) {
                                int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                                if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                                    Symbols_Temp[1][y-1] = Temp1;
                                } else if (Symbols_Temp[1][y-1] == 10) {
                                    Symbols_Temp[1][y-1] = Temp1;
                                    Steps[x][y-1] = steps;
                                }
                            }
                            if (y+1 < Cols) {
                                int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                                if (Symbols_Temp[1][y+1] < Temp2 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                                    Symbols_Temp[1][y+1] = Temp2;
                                } else if (Symbols_Temp[1][y+1] == 10) {
                                    Symbols_Temp[1][y+1] = Temp2;
                                    Steps[x][y+1] = steps;
                                }
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                                Symbols_Temp[0][y] = Temp1;
                            } else if (Symbols_Temp[0][y] == 10) {
                                Symbols_Temp[0][y] = Temp1;
                                Steps[x-1][y] = steps;
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning/2) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[2][y] < Temp1 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                                Symbols_Temp[2][y] = Temp1;
                            } else if (Symbols_Temp[2][y] == 10) {
                                Symbols_Temp[2][y] = Temp1;
                                Steps[x+1][y] = steps;
                            }
                        }
                        break;
                    }
                } else if (zero && two && nine) { //If up and down neighbours exist , and the intensity of the burning cell is nine i.e crown fire.

                    switch (wind_direction) {

                        //Handling fire spread to the Up,down,Left, and right neighbours

                        case 'E':
                            if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                                Symbols_Temp[0][y] = Temp1;
                            } else if (Symbols_Temp[0][y] == 10) {
                                Symbols_Temp[0][y] = Temp1;
                                Steps[x-1][y] = steps;
                            }
                            if (Symbols_Temp[2][y] < Temp2 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                                Symbols_Temp[2][y] = Temp2;
                            } else if (Symbols_Temp[2][y] == 10) {
                                Symbols_Temp[2][y] = Temp2;
                                Steps[x+1][y] = steps;
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning && y+1 < Cols) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                                Symbols_Temp[1][y+1] = Temp1;
                            } else if (Symbols_Temp[1][y+1] == 10) {
                                Symbols_Temp[1][y+1] = Temp1;
                                Steps[x][y+1] = steps;
                            } 
                        }
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y-1 >= 0) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                                Symbols_Temp[1][y-1] = Temp1;
                            } else if (Symbols_Temp[1][y-1] == 10) {
                                Symbols_Temp[1][y-1] = Temp1;
                                Steps[x][y-1] = steps;
                            }
                        }
                        break;

                    case 'W':
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                                Symbols_Temp[0][y] = Temp1;
                            } else if (Symbols_Temp[0][y] == 10) {
                                Symbols_Temp[0][y] = Temp1;
                                Steps[x-1][y] = steps;
                            }
                            if (Symbols_Temp[2][y] < Temp2 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                                Symbols_Temp[2][y] = Temp2;
                            } else if (Symbols_Temp[2][y] == 10) {
                                Symbols_Temp[2][y] = Temp2;
                                Steps[x+1][y] = steps;
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning && y-1 >= 0) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                                Symbols_Temp[1][y-1] = Temp1;
                            } else if (Symbols_Temp[1][y-1] == 10) {
                                Symbols_Temp[1][y-1] = Temp1;
                                Steps[x][y-1] = steps;
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y+1 < Cols) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                                Symbols_Temp[1][y+1] = Temp1;
                            } else if (Symbols_Temp[1][y+1] == 10) {
                                Symbols_Temp[1][y+1] = Temp1;
                                Steps[x][y+1] = steps;
                            }
                        }
                        break;

                    case 'S':
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                            if (y-1 >= 0) {
                                int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                                if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                                    Symbols_Temp[1][y-1] = Temp1;
                                } else if (Symbols_Temp[1][y-1] == 10) {
                                    Symbols_Temp[1][y-1] = Temp1;
                                    Steps[x][y-1] = steps;
                                }
                            }
                            if (y+1 < Cols) {
                                int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                                if (Symbols_Temp[1][y+1] < Temp2 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                                    Symbols_Temp[1][y+1] = Temp2;
                                } else if (Symbols_Temp[1][y+1] == 10) {
                                    Symbols_Temp[1][y+1] = Temp2;
                                    Steps[x][y+1] = steps;
                                }
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[2][y] < Temp1 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                                Symbols_Temp[2][y] = Temp1;
                            } else if (Symbols_Temp[2][y] == 10) {
                                Symbols_Temp[2][y] = Temp1;
                                Steps[x+1][y] = steps;
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning/2) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                                Symbols_Temp[0][y] = Temp1;
                            } else if (Symbols_Temp[0][y] == 10) {
                                Symbols_Temp[0][y] = Temp1;
                                Steps[x-1][y] = steps;
                            }
                        }
                        break;

                    case 'N':
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                            if (y-1 >= 0) {
                                int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                                if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                                    Symbols_Temp[1][y-1] = Temp1;
                                } else if (Symbols_Temp[1][y-1] == 10) {
                                    Symbols_Temp[1][y-1] = Temp1;
                                    Steps[x][y-1] = steps;
                                }
                            }
                            if (y+1 < Cols) {
                                int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                                if (Symbols_Temp[1][y+1] < Temp2 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                                    Symbols_Temp[1][y+1] = Temp2;
                                } else if (Symbols_Temp[1][y+1] == 10) {
                                    Symbols_Temp[1][y+1] = Temp2;
                                    Steps[x][y+1] = steps;
                                }
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                                Symbols_Temp[0][y] = Temp1;
                            } else if (Symbols_Temp[0][y] == 10) {
                                Symbols_Temp[0][y] = Temp1;
                                Steps[x-1][y] = steps;
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning/2) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[2][y] < Temp1 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                                Symbols_Temp[2][y] = Temp1;
                            } else if (Symbols_Temp[2][y] == 10) {
                                Symbols_Temp[2][y] = Temp1;
                                Steps[x+1][y] = steps;
                            }
                        }
                        break;
                    }

                    //Handling fire spread to the diagonal neighbours 

                    if(rand() / (RAND_MAX + 1.0) < probablity_burning && y+1 < Cols) {
                        int Temp1 = (rand() % Symbols_Temp[1][y]) + 1; 
                        if(Symbols_Temp[0][y+1] < Temp1 && Symbols_Temp[0][y+1] > 0 && Steps[x-1][y+1] == steps) {
                            Symbols_Temp[0][y+1] = Temp1; 
                        } else if (Symbols_Temp[0][y+1] == 10) {
                            Symbols_Temp[0][y+1] = Temp1;
                            Steps[x-1][y+1] = steps; 
                        }
                    }
                    if (rand() / (RAND_MAX + 1.0) < probablity_burning && y-1 >= 0 ) {
                        int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[0][y-1] < Temp1 && Symbols_Temp[0][y-1] > 0 && Steps[x-1][y-1] == steps) {
                            Symbols_Temp[0][y-1] = Temp1;

                        } else if (Symbols_Temp[0][y-1] == 10) {
                            Symbols_Temp[0][y-1] = Temp1;
                            Steps[x-1][y-1] = steps;
                        }
                }
                if (rand() / (RAND_MAX + 1.0) < probablity_burning && y-1 >= 0) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[2][y-1] < Temp1 && Symbols_Temp[2][y-1] > 0 && Steps[x+1][y-1] == steps) {
                        Symbols_Temp[2][y-1] = Temp1;
                    } else if (Symbols_Temp[2][y-1] == 10) {
                        Symbols_Temp[2][y-1] = Temp1;
                        Steps[x+1][y-1] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < probablity_burning && y+1 < Cols) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[2][y+1] < Temp1 && Symbols_Temp[2][y+1] > 0 && Steps[x+1][y+1] == steps) {
                        Symbols_Temp[2][y+1] = Temp1;
                    } else if (Symbols_Temp[2][y+1] == 10) {
                        Symbols_Temp[2][y+1] = Temp1;
                        Steps[x+1][y+1] = steps;
                    }
                }


                } else if (zero && nine) { //If only first row and the second row exist, with second row having a crown fire intensity

                    switch (wind_direction) {

                        case 'E':
                            if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                                Symbols_Temp[0][y] = Temp1;
                            } else if (Symbols_Temp[0][y] == 10) {
                                Symbols_Temp[0][y] = Temp1;
                                Steps[x-1][y] = steps;
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning && y+1 < Cols) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                                Symbols_Temp[1][y+1] = Temp1;
                            } else if (Symbols_Temp[1][y+1] == 10) {
                                Symbols_Temp[1][y+1] = Temp1;
                                Steps[x][y+1] = steps;
                            } 
                        }
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y-1 >= 0) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                                Symbols_Temp[1][y-1] = Temp1;
                            } else if (Symbols_Temp[1][y-1] == 10) {
                                Symbols_Temp[1][y-1] = Temp1;
                                Steps[x][y-1] = steps;
                            }
                        }
                        break;

                    case 'W':
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                                Symbols_Temp[0][y] = Temp1;
                            } else if (Symbols_Temp[0][y] == 10) {
                                Symbols_Temp[0][y] = Temp1;
                                Steps[x-1][y] = steps;
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning && y-1 >= 0) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                                Symbols_Temp[1][y-1] = Temp1;
                            } else if (Symbols_Temp[1][y-1] == 10) {
                                Symbols_Temp[1][y-1] = Temp1;
                                Steps[x][y-1] = steps;
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y+1 < Cols) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                                Symbols_Temp[1][y+1] = Temp1;
                            } else if (Symbols_Temp[1][y+1] == 10) {
                                Symbols_Temp[1][y+1] = Temp1;
                                Steps[x][y+1] = steps;
                            }
                        }
                        break;

                    case 'S':
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                            if (y-1 >= 0) {
                                int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                                if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                                    Symbols_Temp[1][y-1] = Temp1;
                                } else if (Symbols_Temp[1][y-1] == 10) {
                                    Symbols_Temp[1][y-1] = Temp1;
                                    Steps[x][y-1] = steps;
                                }
                            }
                            if (y+1 < Cols) {
                                int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                                if (Symbols_Temp[1][y+1] < Temp2 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                                    Symbols_Temp[1][y+1] = Temp2;
                                } else if (Symbols_Temp[1][y+1] == 10) {
                                    Symbols_Temp[1][y+1] = Temp2;
                                    Steps[x][y+1] = steps;
                                }
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning/2) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                                Symbols_Temp[0][y] = Temp1;
                            } else if (Symbols_Temp[0][y] == 10) {
                                Symbols_Temp[0][y] = Temp1;
                                Steps[x-1][y] = steps;
                            }
                        }
                        break;

                    case 'N':
                        if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                            if (y-1 >= 0) {
                                int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                                if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                                    Symbols_Temp[1][y-1] = Temp1;
                                } else if (Symbols_Temp[1][y-1] == 10) {
                                    Symbols_Temp[1][y-1] = Temp1;
                                    Steps[x][y-1] = steps;
                                }
                            }
                            if (y+1 < Cols) {
                                int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                                if (Symbols_Temp[1][y+1] < Temp2 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                                    Symbols_Temp[1][y+1] = Temp2;
                                } else if (Symbols_Temp[1][y+1] == 10) {
                                    Symbols_Temp[1][y+1] = Temp2;
                                    Steps[x][y+1] = steps;
                                }
                            }
                        }
                        if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning) {
                            int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                            if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                                Symbols_Temp[0][y] = Temp1;
                            } else if (Symbols_Temp[0][y] == 10) {
                                Symbols_Temp[0][y] = Temp1;
                                Steps[x-1][y] = steps;
                            }
                        }
                        break;
                    }

                    
                    if(rand() / (RAND_MAX + 1.0) < probablity_burning && y+1 < Cols) {
                        int Temp1 = (rand() % Symbols_Temp[1][y]) + 1; 
                        if(Symbols_Temp[0][y+1] < Temp1 && Symbols_Temp[0][y+1] > 0 && Steps[x-1][y+1] == steps) {
                            Symbols_Temp[0][y+1] = Temp1; 
                        } else if (Symbols_Temp[0][y+1] == 10) {
                            Symbols_Temp[0][y+1] = Temp1;
                            Steps[x-1][y+1] = steps; 
                        }
                    }
                    if (rand() / (RAND_MAX + 1.0) < probablity_burning && y-1 >= 0 ) {
                        int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[0][y-1] < Temp1 && Symbols_Temp[0][y-1] > 0 && Steps[x-1][y-1] == steps) {
                            Symbols_Temp[0][y-1] = Temp1;

                        } else if (Symbols_Temp[0][y-1] == 10) {
                            Symbols_Temp[0][y-1] = Temp1;
                            Steps[x-1][y-1] = steps;
                        }
                }
                } else if (zero && !two && !nine) { //If only row 1 and row 2 exist, and no crown fire

            switch (wind_direction) {

            case 'E':
                if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                        Symbols_Temp[0][y] = Temp1;
                    } else if (Symbols_Temp[0][y] == 10) {
                        Symbols_Temp[0][y] = Temp1;
                        Steps[x-1][y] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning && y+1 < Cols) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                        Symbols_Temp[1][y+1] = Temp1;
                    } else if (Symbols_Temp[1][y+1] == 10) {
                        Symbols_Temp[1][y+1] = Temp1;
                        Steps[x][y+1] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y-1 >= 0) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                        Symbols_Temp[1][y-1] = Temp1;
                    } else if (Symbols_Temp[1][y-1] == 10) {
                        Symbols_Temp[1][y-1] = Temp1;
                        Steps[x][y-1] = steps;
                    }
                }
                break;

            case 'W':
                if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                        Symbols_Temp[0][y] = Temp1;
                    } else if (Symbols_Temp[0][y] == 10) {
                        Symbols_Temp[0][y] = Temp1;
                        Steps[x-1][y] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning && y-1 >= 0) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                        Symbols_Temp[1][y-1] = Temp1;
                    } else if (Symbols_Temp[1][y-1] == 10) {
                        Symbols_Temp[1][y-1] = Temp1;
                        Steps[x][y-1] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y+1 < Cols) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                        Symbols_Temp[1][y+1] = Temp1;
                    } else if (Symbols_Temp[1][y+1] == 10) {
                        Symbols_Temp[1][y+1] = Temp1;
                        Steps[x][y+1] = steps;
                    }
                }
                break;

            case 'S':
                if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                    if (y-1 >= 0) {
                        int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                            Symbols_Temp[1][y-1] = Temp1;
                        } else if (Symbols_Temp[1][y-1] == 10) {
                            Symbols_Temp[1][y-1] = Temp1;
                            Steps[x][y-1] = steps;
                        }
                    }
                    if (y+1 < Cols) {
                        int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y+1] < Temp2 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                            Symbols_Temp[1][y+1] = Temp2;
                        } else if (Symbols_Temp[1][y+1] == 10) {
                            Symbols_Temp[1][y+1] = Temp2;
                            Steps[x][y+1] = steps;
                        }
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < probablity_burning/2) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                        Symbols_Temp[0][y] = Temp1;
                    } else if (Symbols_Temp[0][y] == 10) {
                        Symbols_Temp[0][y] = Temp1;
                        Steps[x-1][y] = steps;
                    }
                }
                break;

            case 'N':
                if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                    if (y-1 >= 0) {
                        int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                            Symbols_Temp[1][y-1] = Temp1;
                        } else if (Symbols_Temp[1][y-1] == 10) {
                            Symbols_Temp[1][y-1] = Temp1;
                            Steps[x][y-1] = steps;
                        }
                    }
                    if (y+1 < Cols) {
                        int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y+1] < Temp2 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                            Symbols_Temp[1][y+1] = Temp2;
                        } else if (Symbols_Temp[1][y+1] == 10) {
                            Symbols_Temp[1][y+1] = Temp2;
                            Steps[x][y+1] = steps;
                        }
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[0][y] < Temp1 && Symbols_Temp[0][y] > 0 && Steps[x-1][y] == steps) {
                        Symbols_Temp[0][y] = Temp1;
                    } else if (Symbols_Temp[0][y] == 10) {
                        Symbols_Temp[0][y] = Temp1;
                        Steps[x-1][y] = steps;
                    }
                }
                break;
            }

        } else if (!zero && two && !nine) { //if only Row2 and Row 3 exist, and no crown fire

            switch (wind_direction) {

            case 'E':
                if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[2][y] < Temp1 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                        Symbols_Temp[2][y] = Temp1;
                    } else if (Symbols_Temp[2][y] == 10) {
                        Symbols_Temp[2][y] = Temp1;
                        Steps[x+1][y] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning && y+1 < Cols) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                        Symbols_Temp[1][y+1] = Temp1;
                    } else if (Symbols_Temp[1][y+1] == 10) {
                        Symbols_Temp[1][y+1] = Temp1;
                        Steps[x][y+1] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y-1 >= 0) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                        Symbols_Temp[1][y-1] = Temp1;
                    } else if (Symbols_Temp[1][y-1] == 10) {
                        Symbols_Temp[1][y-1] = Temp1;
                        Steps[x][y-1] = steps;
                    }
                }
                break;

            case 'W':
                if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[2][y] < Temp1 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                        Symbols_Temp[2][y] = Temp1;
                    } else if (Symbols_Temp[2][y] == 10) {
                        Symbols_Temp[2][y] = Temp1;
                        Steps[x+1][y] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning && y-1 >= 0) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                        Symbols_Temp[1][y-1] = Temp1;
                    } else if (Symbols_Temp[1][y-1] == 10) {
                        Symbols_Temp[1][y-1] = Temp1;
                        Steps[x][y-1] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y+1 < Cols) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                        Symbols_Temp[1][y+1] = Temp1;
                    } else if (Symbols_Temp[1][y+1] == 10) {
                        Symbols_Temp[1][y+1] = Temp1;
                        Steps[x][y+1] = steps;
                    }
                }
                break;

            case 'S':
                if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                    if (y-1 >= 0) {
                        int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                            Symbols_Temp[1][y-1] = Temp1;
                        } else if (Symbols_Temp[1][y-1] == 10) {
                            Symbols_Temp[1][y-1] = Temp1;
                            Steps[x][y-1] = steps;
                        }
                    }
                    if (y+1 < Cols) {
                        int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y+1] < Temp2 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                            Symbols_Temp[1][y+1] = Temp2;
                        } else if (Symbols_Temp[1][y+1] == 10) {
                            Symbols_Temp[1][y+1] = Temp2;
                            Steps[x][y+1] = steps;
                        }
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[2][y] < Temp1 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                        Symbols_Temp[2][y] = Temp1;
                    } else if (Symbols_Temp[2][y] == 10) {
                        Symbols_Temp[2][y] = Temp1;
                        Steps[x+1][y] = steps;
                    }
                }
                break;

            case 'N':
                if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                    if (y-1 >= 0) {
                        int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                            Symbols_Temp[1][y-1] = Temp1;
                        } else if (Symbols_Temp[1][y-1] == 10) {
                            Symbols_Temp[1][y-1] = Temp1;
                            Steps[x][y-1] = steps;
                        }
                    }
                    if (y+1 < Cols) {
                        int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y+1] < Temp2 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                            Symbols_Temp[1][y+1] = Temp2;
                        } else if (Symbols_Temp[1][y+1] == 10) {
                            Symbols_Temp[1][y+1] = Temp2;
                            Steps[x][y+1] = steps;
                        }
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < probablity_burning/2) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[2][y] < Temp1 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                        Symbols_Temp[2][y] = Temp1;
                    } else if (Symbols_Temp[2][y] == 10) {
                        Symbols_Temp[2][y] = Temp1;
                        Steps[x+1][y] = steps;
                    }
                }
                break;
            }

        } else if (!zero && two && nine) { // if only Row 2 and Row 3 exist with crown fire

            switch (wind_direction) {

            case 'E':
                if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[2][y] < Temp1 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                        Symbols_Temp[2][y] = Temp1;
                    } else if (Symbols_Temp[2][y] == 10) {
                        Symbols_Temp[2][y] = Temp1;
                        Steps[x+1][y] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning && y+1 < Cols) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                        Symbols_Temp[1][y+1] = Temp1;
                    } else if (Symbols_Temp[1][y+1] == 10) {
                        Symbols_Temp[1][y+1] = Temp1;
                        Steps[x][y+1] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y-1 >= 0) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                        Symbols_Temp[1][y-1] = Temp1;
                    } else if (Symbols_Temp[1][y-1] == 10) {
                        Symbols_Temp[1][y-1] = Temp1;
                        Steps[x][y-1] = steps;
                    }
                }
                break;

            case 'W':
                if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[2][y] < Temp1 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                        Symbols_Temp[2][y] = Temp1;
                    } else if (Symbols_Temp[2][y] == 10) {
                        Symbols_Temp[2][y] = Temp1;
                        Steps[x+1][y] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning && y-1 >= 0) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                        Symbols_Temp[1][y-1] = Temp1;
                    } else if (Symbols_Temp[1][y-1] == 10) {
                        Symbols_Temp[1][y-1] = Temp1;
                        Steps[x][y-1] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y+1 < Cols) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                        Symbols_Temp[1][y+1] = Temp1;
                    } else if (Symbols_Temp[1][y+1] == 10) {
                        Symbols_Temp[1][y+1] = Temp1;
                        Steps[x][y+1] = steps;
                    }
                }
                break;

            case 'S':
                if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                    if (y-1 >= 0) {
                        int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                            Symbols_Temp[1][y-1] = Temp1;
                        } else if (Symbols_Temp[1][y-1] == 10) {
                            Symbols_Temp[1][y-1] = Temp1;
                            Steps[x][y-1] = steps;
                        }
                    }
                    if (y+1 < Cols) {
                        int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y+1] < Temp2 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                            Symbols_Temp[1][y+1] = Temp2;
                        } else if (Symbols_Temp[1][y+1] == 10) {
                            Symbols_Temp[1][y+1] = Temp2;
                            Steps[x][y+1] = steps;
                        }
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[2][y] < Temp1 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                        Symbols_Temp[2][y] = Temp1;
                    } else if (Symbols_Temp[2][y] == 10) {
                        Symbols_Temp[2][y] = Temp1;
                        Steps[x+1][y] = steps;
                    }
                }
                break;

            case 'N':
                if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                    if (y-1 >= 0) {
                        int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                            Symbols_Temp[1][y-1] = Temp1;
                        } else if (Symbols_Temp[1][y-1] == 10) {
                            Symbols_Temp[1][y-1] = Temp1;
                            Steps[x][y-1] = steps;
                        }
                    }
                    if (y+1 < Cols) {
                        int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y+1] < Temp2 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                            Symbols_Temp[1][y+1] = Temp2;
                        } else if (Symbols_Temp[1][y+1] == 10) {
                            Symbols_Temp[1][y+1] = Temp2;
                            Steps[x][y+1] = steps;
                        }
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < probablity_burning/2) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[2][y] < Temp1 && Symbols_Temp[2][y] > 0 && Steps[x+1][y] == steps) {
                        Symbols_Temp[2][y] = Temp1;
                    } else if (Symbols_Temp[2][y] == 10) {
                        Symbols_Temp[2][y] = Temp1;
                        Steps[x+1][y] = steps;
                    }
                }
                break;
            }

            if (rand() / (RAND_MAX + 1.0) < probablity_burning && y-1 >= 0) {
                int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                if (Symbols_Temp[2][y-1] < Temp1 && Symbols_Temp[2][y-1] > 0 && Steps[x+1][y-1] == steps) {
                    Symbols_Temp[2][y-1] = Temp1;
                } else if (Symbols_Temp[2][y-1] == 10) {
                    Symbols_Temp[2][y-1] = Temp1;
                    Steps[x+1][y-1] = steps;
                }
            }
            if (rand() / (RAND_MAX + 1.0) < probablity_burning && y+1 < Cols) {
                int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                if (Symbols_Temp[2][y+1] < Temp1 && Symbols_Temp[2][y+1] > 0 && Steps[x+1][y+1] == steps) {
                    Symbols_Temp[2][y+1] = Temp1;
                } else if (Symbols_Temp[2][y+1] == 10) {
                    Symbols_Temp[2][y+1] = Temp1;
                    Steps[x+1][y+1] = steps;
                }
            }

        } else if (!zero && !two) { //If only the East and West neighbours exist i.e no row up and down, could be crown fire and could not be as well

            switch (wind_direction) {

            case 'E':
                if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning && y+1 < Cols) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                        Symbols_Temp[1][y+1] = Temp1;
                    } else if (Symbols_Temp[1][y+1] == 10) {
                        Symbols_Temp[1][y+1] = Temp1;
                        Steps[x][y+1] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y-1 >= 0) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                        Symbols_Temp[1][y-1] = Temp1;
                    } else if (Symbols_Temp[1][y-1] == 10) {
                        Symbols_Temp[1][y-1] = Temp1;
                        Steps[x][y-1] = steps;
                    }
                }
                break;

            case 'W':
                if (rand() / (RAND_MAX + 1.0) < 2*probablity_burning && y-1 >= 0) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                        Symbols_Temp[1][y-1] = Temp1;
                    } else if (Symbols_Temp[1][y-1] == 10) {
                        Symbols_Temp[1][y-1] = Temp1;
                        Steps[x][y-1] = steps;
                    }
                }
                if (rand() / (RAND_MAX + 1.0) < probablity_burning/2 && y+1 < Cols) {
                    int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                    if (Symbols_Temp[1][y+1] < Temp1 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                        Symbols_Temp[1][y+1] = Temp1;
                    } else if (Symbols_Temp[1][y+1] == 10) {
                        Symbols_Temp[1][y+1] = Temp1;
                        Steps[x][y+1] = steps;
                    }
                }
                break;
            //Reciprocate the same case with Up, and down wind directions, as Up and down neighbours dont exist
            case 'S':
            case 'N': 
                if (rand() / (RAND_MAX + 1.0) < probablity_burning) {
                    if (y-1 >= 0) {
                        int Temp1 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y-1] < Temp1 && Symbols_Temp[1][y-1] > 0 && Steps[x][y-1] == steps) {
                            Symbols_Temp[1][y-1] = Temp1;
                        } else if (Symbols_Temp[1][y-1] == 10) {
                            Symbols_Temp[1][y-1] = Temp1;
                            Steps[x][y-1] = steps;
                        }
                    }
                    if (y+1 < Cols) {
                        int Temp2 = (rand() % Symbols_Temp[1][y]) + 1;
                        if (Symbols_Temp[1][y+1] < Temp2 && Symbols_Temp[1][y+1] > 0 && Steps[x][y+1] == steps) {
                            Symbols_Temp[1][y+1] = Temp2;
                        } else if (Symbols_Temp[1][y+1] == 10) {
                            Symbols_Temp[1][y+1] = Temp2;
                            Steps[x][y+1] = steps;
                        }
                    }
                }
                break;
            }
        }

        if (steps - Steps[x][y] >= Burned_steps) { //Checking if tree has burned till Burned steps or b in the file
             Symbols_Temp[1][y] = -1; //If yes then turn the burning tree into Ash or -1.

        } else if (temp_prob_inten <= probablity_intensity) {
            Symbols_Temp[1][y] = min(Symbols_Temp[1][y] + 1, 9); //if the  calculated probablity is within g, increase the intensity by 1, if intensity = 10, convert to 9.

        } else {
            Symbols_Temp[1][y] = max(0,Symbols_Temp[1][y] - 1); //if the calculated probablity is not within g, decrease the intensity by 1, if intensity gets negative then pick 0.

            if(Symbols_Temp[1][y] == 0) { // if intensity of a tree is 0, then convert to Ash or -1
                Symbols_Temp[1][y] = -1; 
            }
           
        }
        //Assigning the Symbol_Temp map back to symbols for the next iterations.

        if (zero && two) {

            for(int z = 0; z < Cols; z++) {
                Symbols[x-1][z] = Symbols_Temp[0][z];
                Symbols[x][z] = Symbols_Temp[1][z]; 
                Symbols[x+1][z] = Symbols_Temp[2][z]; 
            }
        } else if (zero && !two) {
            for(int z = 0; z < Cols; z++) {
                Symbols[x-1][z] = Symbols_Temp[0][z];
                Symbols[x][z] = Symbols_Temp[1][z]; 

            }
        } else if (!zero && two) {
            for(int z = 0; z < Cols; z++) {
                
                Symbols[x][z] = Symbols_Temp[1][z]; 
                Symbols[x+1][z] = Symbols_Temp[2][z]; 

            }

        } else if (!zero && !two) {
             for(int z = 0; z < Cols; z++) {
                
                Symbols[x][z] = Symbols_Temp[1][z]; 

            }

        }
            
        } 
        }
    } 

    //At the end of each step, calculate the currently burning trees, and the trees that have been turned into Ash
    
    for(int y = 0; y < Rows; y++) {

        for(int z = 0; z < Cols; z++) {

            if (Symbols[y][z] > 0 && Symbols[y][z] < 10) {
                Total_Burning++; 
            } else if (Symbols[y][z] == -1) { 
                Total_Ash++;
            }
        }
    }

    //Print the number of steps, and the total trees remaining i.e those that arent burning or ashes. 
    //print the number of tree burned, and Ashed as well.


    cout << "Step " << steps << ": Trees: " << Total_Trees-Total_Burning-Total_Ash << ", Burning: " << Total_Burning << ", Ash: " << Total_Ash << "\n"; 
    File_write << "Step " << steps << ": Trees: " << Total_Trees-Total_Burning-Total_Ash << ", Burning: " << Total_Burning << ", Ash: " << Total_Ash << "\n"; 

    if (Total_Burning == 0) {
    break; 
    }



}
//Print the Final map,and the amount of trees destroyed i.e turned to ashes.

   cout << "\n"; 
   cout << "Final map:" << "\n"; 

   File_write << "\n";
   File_write << "Final map:" << "\n";
   for(int y = 0; y < Rows; y++) {

        for(int z = 0; z < Cols; z++) {
            if (Symbols[y][z] == -1) {
                cout << 'A';
                File_write << 'A'; 
            } else if (Symbols[y][z] == 10) {cout << 'T'; File_write << 'T';} 

            else if (Symbols[y][z] == -2) {cout << '#'; File_write << '#';} 
            else if (Symbols[y][z] == -3) {cout << '.' ; File_write << '.';} 
            else {cout << Symbols[y][z]; File_write << Symbols[y][z];}
        }
    cout << "\n"; 
    File_write << "\n"; 

    }

    cout << "Forest destroyed: " << Total_Ash << " of " << Total_Trees << fixed << setprecision(2) << " (" << ((double)Total_Ash/Total_Trees)*100 << "%)";
    File_write << "Forest destroyed: " << Total_Ash << " of " << Total_Trees << fixed << setprecision(2) << " (" << ((double)Total_Ash/Total_Trees)*100 << "%)" << "\n";
   
   File_write.close(); 
   cout << "\n";
   
}