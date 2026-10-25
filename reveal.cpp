//Name: Syed Muhammad Ziean, ERP: 35505, Section: Akhrahmah Rashid Friday-Saturday Batch.



#include <iostream>
#include <fstream> 
using namespace std;

int main() {
    const int MAX_ROWS = 50;
    const int MAX_COLS = 50;
    int Rows;
    int differ_cells = 0;
    int Cols;
    int Number;
    int Secret;
    int Col_tracker = -1; //Starts at -1, so that we can index the comparision array correctly.
    int Row_tracker = 0;
    int Comparision_Array[MAX_ROWS][MAX_COLS]; //Used to compare the odd and even pattern in stego and secret.txt.

    ifstream File_read("stego.txt"); 
    ofstream File_write("recovered.txt"); //Holds the odd and even bit pattern produced  by stego.txt

    if(!File_read) {
        cout << "File stego.txt cannot be opened for reading" << "\n"; 
        File_write.close(); 
        return 1;

    }

    if(!File_write) {
        cout << "File recovered.txt cannot be written to." << "\n";
        File_read.close();
        return 3;
    }

    File_read >> Rows; 
    File_read >> Cols; 
    File_write << Rows;
    File_write << " ";
    File_write << Cols;
    File_write <<"\n"; 

    //If the number read from Stego is even print out a space, if its odd then print out a #.
    while(File_read >> Number) {
        if (Number % 2 == 0) {
            
            cout << " ";
        } else {
            cout << "#";
        }

       
        Col_tracker++; 
        Comparision_Array[Row_tracker][Col_tracker] = Number % 2; //Store whether number os odd or even i.e 0 or 1.
        File_write << Number % 2 << " ";  //Write 0 or 1 into recovered.txt

        if (Col_tracker == Cols-1) {
            cout << "\n"; 
            File_write << "\n"; 
            Row_tracker++;
            Col_tracker = -1; 
        }

    }

    File_read.close();
    File_write.close();
    

    ifstream File_read2("Text_Files_Used/secret.txt") ;
    if(!File_read2) {cout << "File secret.txt cannot be opened for reading." << "\n"; return 2;}

    File_read2 >> Rows;
    File_read2 >> Cols;

    //The outer loop changes rows, and inner changes columns.
    //Read the first character from secret.txt, and compare it to the corresponding character in Comparision array.
    //If they differ then add 1 to differ_Cells which represent the number of cells that are different in revealed and secret.txt.

    for (int x = 0; x < Rows; x++) {

        for(int y = 0; y < Cols; y++) {
            File_read2 >> Secret; 
        
            if (Secret != Comparision_Array[x][y]) {
                differ_cells++; 
            }
            
        }
    }

    File_read2.close();

    if(differ_cells == 0) {

        cout << "Secret recovered perfectly (" << differ_cells << " of " << Rows*Cols << " cells differ).";
        cout << "\n"; 
    } else {
        cout << "Secret was not recovered perfectly (" << differ_cells << " of " << Rows*Cols << " cells differ).";
        cout << "\n";
    }








}