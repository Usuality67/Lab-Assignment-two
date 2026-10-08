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

    for (int x = 0; x < Max_row; x++) {
        for (int y = 0; y < Max_col; y++) {

            Grid[x][y] = ' '; 
            
 
        }
    }
   int Mid_Col = (Max_col)/2 ; 
   int Mid_Row =  (Max_row/2);

    for (int y = 0; y < Max_col; y++) {
        Grid[Mid_Row][y] = '-';
    }

    for (int x = 0; x < Max_row; x++) {
        Grid[x][Mid_Col] = '|';
    }

    
    
    for (int y = 0; y < Max_col;y++) {
        double current_val = (double) (x_min) + (y * ((x_max-x_min)/(Max_col-1)));
        double sin_val =  sin(current_val);
        int Row_num = round(((sin_val-y_min)/(y_max-y_min)) * (Max_row-1)); 

        Grid[(Row_num-19)*-1][y] = '*'; 
    }
    
    ofstream file_write("plot.txt"); 
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