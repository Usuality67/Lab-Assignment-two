//Name: Syed Muhammad Ziean, ERP: 35505, Section: Akhrahmah Rashid Friday-Saturday Batch.

#include <iostream> 
#include <cmath> 
#include <fstream> 

using namespace std; 

int main() {
    const int Max_row = 20; 
    const int Max_col = 80; 
    const int y_min = -1, y_max = 1;
    double x_max, x_min; 
    char Grid[Max_row][Max_col];

    cout << "Enter xmin and xmax (radians): "; 
    cin >> x_min; 
    cin >> x_max; 

    //Initialize the Grid with spaces
    for (int x = 0; x < Max_row; x++) {
        for (int y = 0; y < Max_col; y++) {

            Grid[x][y] = ' '; 
            
 
        }
    }

    //Finding Middle column, and middle row for making an axis.
   int Mid_Col = (Max_col)/2 ; 
   int Mid_Row =  (Max_row/2);
    
   //Building the x-axis

    for (int y = 0; y < Max_col; y++) {
        Grid[Mid_Row][y] = '-';
    }
    
    //Building the y-axis
    for (int x = 0; x < Max_row; x++) {
        Grid[x][Mid_Col] = '|';
    }

    
    //Using formulas to make the sine wave, and representing them with "*" in the Grid array
    for (int y = 0; y < Max_col;y++) {
        double current_val = (double) (x_min) + (y * ((x_max-x_min)/(Max_col-1)));
        double sin_val =  sin(current_val);
        int Row_num = round(((sin_val-y_min)/(y_max-y_min)) * (Max_row-1)); //Gives the corresponding Row number
                                                                            //At which the sin_val is to be stored

        Grid[(Row_num-19)*-1][y] = '*'; //As Small y value should go in large Row number e.g y value of 0 will go at
                                        // (0-19)*-1 = 19th row i.e the last row. Similarly a bigger y value
                                        //will go in a smaller row.
    }
    
    ofstream file_write("plot.txt"); 
    if(!file_write) {

        cout << "Plot.txt cannot be opened for writing" << "\n";
        return 1;

    }
    //Print , and write the grid to the file plot.txt.
    
    for (int x = 0; x < Max_row;x++) {
        for(int y = 0; y< Max_col;y++) {
            cout << Grid[x][y];
            file_write << Grid[x][y];   
        }
        cout << "\n"; 
        file_write << "\n"; 
    }
    cout << "Grid drawn in plot.txt."; 
    cout << "\n"; 
    file_write.close();

    


   


}