//Name: Syed Muhammad Ziean, ERP: 35505, Section: Akhrahmah Rashid Friday-Saturday Batch.

/*Think about it: 

1.Roughly what fraction of the cover values do you expect to change? Does the provided
20×40 example agree? 
Ans. I would expect roughly half of the cover values to change.
The example though, only changes 387 of the values,and not 400, so it doesnot agree with my expectations.

2. Could someone who does not know the trick tell that stego.txt holds a secret? What
might give it away?

Ans.They could not tell that the stego.txt holds a secret if they only looked at the image,and not the odd and even bit pattern, 
as it will look like an original file, because each value in stego differs by at most 1 from the cover values. 
What might give it away is that when the odd and even bits of a gray scaled image is extracted from its numbers (0 or 1) 
they are highly random, so the image generated from using the odd and even patterns of an image would generate an
unstructured image, while the odd and even bits from,stego.txt, forms a pattern, IBA, and the last row with all hashtages,
which is highly unlikely, so if the on-looker/user were to check the odd and even bits pattern in stego, would get to know that
it does indeed holds a secret.

*/

#include <iostream> 
#include <fstream>

using namespace std;

int main() {

    int covers;
    int secrets; 
    string cover_line;
    string secrets_line;
    int cover_values = 0; 
    int secret_values = 0;
    int cover_rows = 0;
    int secrets_rows = 0;
    int Row_num_cover, Col_num_cover; 
    int Row_num_secret, Col_num_secret;
    const int MAX_ROWS = 50;
    const int MAX_COLS = 50;

    ifstream File_read1("Text_Files_Used/cover.txt"); 
    ifstream File_read2("Text_Files_Used/secret.txt"); 

    if (!File_read1) {
        cout << "cover.txt cannot be opened"; 
        return 11;
    }
    if (!File_read2) {
        cout << "secret.txt cannot be opened";
        return 12;
    }
    File_read1 >> Row_num_cover; 
    File_read1 >> Col_num_cover; 

    File_read2 >> Row_num_secret; 
    File_read2 >> Col_num_secret; 

    //Grids that have rows or cols greater than 50 

    if(Row_num_cover > MAX_ROWS || Row_num_secret > MAX_ROWS  || Col_num_cover > MAX_COLS || Col_num_secret > MAX_COLS) {

        cout << "Rows and Columns should be kept under 51" << "\n";
        File_read2.close(); 
        return 8;
    }

    //Invalid Grid size, i.e less than 1

    if(Row_num_cover < 1 || Col_num_cover < 1 || Row_num_secret < 1 || Col_num_secret < 1) {

        cout << "Rows and Columns must be greater than 0"  << "\n";
        File_read2.close();
        return 9;
    }

    //Grid of different sizes

    if (Row_num_secret != Row_num_cover || Col_num_secret != Col_num_cover) {
        cout << "Cover and Secret, both files have different sizes of their respective grid." << "\n";
        File_read2.close();
        return 1;
    }
    
    //Counting actual number of row in the file .
    while(getline(File_read1, cover_line)) {
        cover_rows++; 

    }
    cover_rows--; //Exclude the first line read as it contains grid size
 
    if(cover_rows != Row_num_cover) { //If the actual number of rows are not equal to the grid size, reject the grid.
        cout << "Amount of rows in cover, dont match the amount of rows promised in the file." << "\n";
        File_read2.close();
        return 2;
    }
    File_read1.close(); 

    //Similarly for secret.txt
    while(getline(File_read2, secrets_line)) {
        secrets_rows++; 

    }
    secrets_rows--;

    if(secrets_rows != Row_num_secret) {
        cout << "Amount of rows in secret, dont match the amount of rows promised in the file." << "\n";
        File_read2.close();
        return 3;
    }
    File_read2.close();

    File_read1.open("Text_Files_Used/cover.txt");
    File_read2.open("Text_Files_Used/secret.txt");

    if (!File_read1) {
        cout << "cover.txt cannot be opened"; 
        File_read2.close();
        return 11;
    }
    if (!File_read2) {
        cout << "secret.txt cannot be opened";
        File_read1.close(); 
        return 12;
    }

    File_read1 >> Row_num_cover; 
    File_read1 >> Col_num_cover; 

    File_read2 >> Row_num_secret; 
    File_read2 >> Col_num_secret; 

    //Checking of cover values are less than 0 or greater than 255

    while(File_read1 >> covers) {

        cover_values++; 
        if (covers < 0 || covers > 255) {
            cout << "Cover values must be in the range 0-255 [inclusive]."  << "\n"; 
            File_read2.close();
            return 4; 
        }

    }
    File_read1.close();

    //Checking if the values in a row donot match the promised ones at the first line
    if(cover_values-((Row_num_cover-1) * Col_num_cover) != Col_num_cover) {
        cout << "Number of columns in the grid of cover, donot match the number of columns promised in the file." << "\n";
        File_read2.close();
        return 5;
    }

    while(File_read2 >> secrets) {
        secret_values++;
        if(secrets != 0 && secrets != 1){
            cout << "Secreat values can only be 1 or 0." << "\n";
            File_read2.close();
            return 6;
        }
    }
    if(secret_values-((Row_num_secret-1) * Col_num_secret) != Col_num_secret) {
        cout << "Number of columns in the grid of secrets, donot match the number of columns promised in the file." << "\n";
        File_read2.close();
        return 7;
    }
    File_read2.close();

    File_read1.open("Text_Files_Used/cover.txt");
    File_read2.open("Text_Files_Used/secret.txt");

    if (!File_read1) {
        cout << "cover.txt cannot be opened"; 
        File_read2.close();
        return 11;
    }
    if (!File_read2) {
        cout << "secret.txt cannot be opened";
        File_read1.close();
        return 12;
    }

    File_read1 >> Row_num_cover; 
    File_read1 >> Col_num_cover; 

    File_read2 >> Row_num_secret; 
    File_read2 >> Col_num_secret; 

    int Cover_array[MAX_ROWS][MAX_COLS], Secret_array[MAX_ROWS][MAX_COLS]; //Cover array stores values from cover.txt. 
                                                                           //Secret array stores values from secret.txt

    for(int x = 0; x < Row_num_cover; x++) {

        for(int y = 0; y < Col_num_secret; y++) {

            File_read1 >> Cover_array[x][y]; 
            File_read2 >> Secret_array[x][y];

        }
    }

    File_read1.close();
    File_read2.close();

    ofstream File_write("stego.txt");

    if(!File_write) {
        cout << "stego.txt cannot be opened for writing."; 
        return 13; 
    }


    File_write << Row_num_cover << " " << Col_num_secret << "\n";


    int total_values_changed = 0;
    
    //The first loop cycles throw rows in secret and cover array, and the second one loops through columns in each.
    //Inner loop checks if the value, in secret array, at a specific row and column is either odd or even. 
    //If the secret array value is even then check if corresponding cover array value is even or odd.
    //If its even write it as is to the file "Stego.txt".
    //If the cover value is odd then convert it into even by either adding 1 (make sure it is less than 255).
    //Or by subtracting one (make sure it is greater than or equals to 0)
    //Then just write the value.

    //Do the oppoiste  when the secret array value is even or "0".

    //First add 1 into the Cover_array 
    for(int x = 0; x < Row_num_cover; x++) {   

        for(int y = 0; y < Col_num_secret; y++) {

            if(Secret_array[x][y] % 2 == 0) {

                if(Cover_array[x][y] % 2 == 1) {
                    total_values_changed++;
                    if((Cover_array[x][y] + 1)%2 == 0 && (Cover_array[x][y]+1) < 255){

                        File_write << Cover_array[x][y]+1 << " ";
                    } else if ((Cover_array[x][y] - 1) % 2 == 0 && (Cover_array[x][y]-1) >= 0) {
                        File_write << Cover_array[x][y]-1 << " ";
                    }
                } else {
                    File_write << Cover_array[x][y] << " ";
                }

            } else {
                if(Cover_array[x][y] % 2 == 0) {
                    total_values_changed++;
                    if((Cover_array[x][y] + 1)%2 == 1 && (Cover_array[x][y]+1) <= 255){

                        File_write << Cover_array[x][y]+1 << " ";

                    } else if ((Cover_array[x][y] - 1) % 2 == 1 && (Cover_array[x][y]-1) > 0) {
                        File_write << Cover_array[x][y]-1 << " ";

                    }
                } else {
                    File_write << Cover_array[x][y] << " ";
                }
            }

        }
        File_write << "\n"; 

    }

    File_write.close();
    cout << total_values_changed << " of " << Row_num_cover * Col_num_cover << " values changed." << "\n";

    
    



    

    




    
}