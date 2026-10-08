#include <iostream> 
#include <fstream> 
#include <cstdlib>
#include <string> 
#include <cmath> 
#include <iomanip>



using namespace std; 

int main() {
    
    string Num_readings;
    
    

    int total_readings = 0;

    //Validating if file exist
   
    ifstream file_read("trip.txt"); 

    if (!file_read.is_open()) {
        cout << "Error encountered!. File is not opening"; 

    } else {
        //Reading the number of values in the file 

        getline(file_read,Num_readings); 

        const int N = stoi(Num_readings); //Converting the Number of readings from string to integer and storing it.

        if (N > 1000) {
            cout << "Readings more than 1000 are not accepted by the program!";

        } else {
             while (!file_read.eof()) {
                getline(file_read,Num_readings); 
            
                total_readings++;  //Counting total number of readings in the file . 
        }
        file_read.close();

        if (N == total_readings){

            ifstream file_read("trip.txt"); 

            if (!file_read.is_open()) {
                cout << "Error encountered!. File is not opening"; 
                return 1;

            } else {

                getline(file_read , Num_readings); 

                const int first_read = stoi(Num_readings); //To read the first line "N" before the loop

                //Arrays are used for storing the corresponding speeds, times, and distance in order to wrote them in a correct order to the output file.
                const int MAX_Index = first_read; 

                double Prev_time = 0.00; //To track if time changes or not.
                double Prev_speed = 0.00; //To record the complete speed and time reading, at which time doesnt change.
                double speed;
                double time;
                double current_distance = 0.00; 
                double single_distance = 0.00; //Distance covered at each speed and time
                double Prev_distance = 0.00; //Distance convered at the previous speed and time.
                double total_time = 0.00;//Time for which car readings were recorded
                double total_distance; //The distance car travelled
                double average_speed = 0.00; 
                double Accel, Deccel; //Acceleration and decceleration values to compare with the Hardest acc and Hardest break
                double Hardest_acceleration = -1.00, Hardest_brake = 0.00; //Most acc and Most deceleration during the journey.
                double accel_time, accel_time_prev; //To store the times for hardest acceleration
                double deccel_time, deccel_time_prev; //To store the times for hardest break.
                double Time_Stop = 0.00; //The total time the car was stopped for.

                int Reading_num = 1; //To track the file line we are on, intialized to 1, as we have read the first line at the start.
                int Index_finder; //To record the Index of the space , and the number of digits in speed and time.
                int Index_extract; //Used for extracting the number of digits from the file string.
                int Array_element = 0; //Used for indexing all three arrays; 

                double speed_array[MAX_Index]; //To store the speeds at different point in time 
                double time_array[MAX_Index]; //To store the corresponding time
                double distance_array[MAX_Index]; //To store the corresponding distance 

                string speed_and_time; //To store the read data from trip.txt

                while (!file_read.eof()){
                    

                    getline(file_read, speed_and_time);
                    Reading_num++;
                    Index_finder = speed_and_time.find(" ");
                    Index_extract = speed_and_time.length() - Index_finder;  

                    time = stod(speed_and_time.substr(0,Index_extract+1));  
                    speed = stod(speed_and_time.substr(Index_finder+1,Index_extract));
                    
                    

                    if (time == Prev_time && time != 0){
                        
                        
                        cout << "The file is rejected! because time doesnt change between readings." << "\n"; 
                        cout << "Reading #" << Reading_num-1 << fixed << setprecision(2) << " states speed as: " << Prev_speed << " and time as: " << Prev_time << "\n"; 
                        cout << " While Reading #" << Reading_num-1 << fixed << setprecision(2) << " states speed as: " << speed << " and time as: " << time;
                        return 1;
                    } else if (speed < 0) {
                        cout << "The file is rejected! because speed readin is stated as negative." << "\n"; 
                        cout << "Reading #" << Reading_num << fixed  << setprecision(2) <<" states speed as: " << speed << " and time as: " << time << "\n"; 
                        return 1;
                    } else {
                        if (Array_element == 0) {
                        total_time = time; 
                        Prev_time = time; 

                    }
                        speed_array[Array_element] = speed; 
                        time_array[Array_element] = time; 
                        single_distance = (0.5*(Prev_speed + speed)*(time-Prev_time) ) + Prev_distance; 
                        distance_array[Array_element] = single_distance; 

                        if (speed == 0.00 && Prev_speed == 0.00 && time != 0) {
                            Time_Stop += time-Prev_time; 
                        }
                        if (time != 0 && Array_element != 0) {
                            Accel = (speed-Prev_speed)/(time-Prev_time) ;
                            Deccel = (speed-Prev_speed)/(time-Prev_time);  

                        } else {
                            Accel = 0.00;
                            Deccel = 0.00;
                        }
                        

                        if (Accel > Hardest_acceleration) {
                            Hardest_acceleration = Accel; 
                            accel_time = time;
                            accel_time_prev = Prev_time;

                        } if (Deccel < Hardest_brake && Deccel != 0) {
                            Hardest_brake = Deccel; 
                            deccel_time = time;
                            deccel_time_prev = Prev_time;
                        }

                        Array_element++;
                        Prev_distance = single_distance; 
                        Prev_speed = speed;
                        Prev_time = time; 

                    }


                }
                file_read.close();

                total_distance = single_distance; 
                total_time = time-total_time;
                if (total_time != 0) {
                    average_speed += single_distance/total_time;

                }
                    
                    

                    
                cout << "Trip duration:" << setw(to_string(total_time).length()+4) << fixed << setprecision(2) << total_time << " s";
                cout << "\n";
                cout << "Distance travelled:" <<  setw(to_string(total_distance).length()-1) <<fixed << setprecision(2) << total_distance << " m";
                cout << "\n";

                if (average_speed != 0.00) {
                    cout << "Average speed:" <<  setw(to_string(average_speed).length()+4) << fixed << setprecision(2) << average_speed << " m/s";
                        
                } else {
                    cout << "Average speed:" <<  setw(to_string(average_speed).length()+5) << "None.";
                }
                cout << "\n";
                    
                if (Hardest_acceleration != 0.00) {
                    cout << "Hardest acceleration:" <<  setw(to_string(Hardest_acceleration).length()-3) <<fixed << setprecision(2) << Hardest_acceleration << " m/s^2";
                    cout << " (between t = " << fixed << setprecision(2) << accel_time_prev << " s and t = " << accel_time << " s)";

                } else {
                    cout << "Hardest acceleration:" << setw(to_string(Hardest_acceleration).length()-3) << fixed << setprecision(2) << Hardest_acceleration << " m/s^2"; 
                }
                   
                cout << "\n";

                if (Hardest_brake != 0.00) {
                    cout << "Hardest braking:" <<  setw(to_string(Hardest_brake).length()+2) <<fixed << setprecision(2) << Hardest_brake << " m/s^2";
                    cout << " (between t = " << fixed << setprecision(2) << deccel_time_prev << " s and t = " << deccel_time << " s)";

                } else {
                    cout << "Hardest braking:" <<  setw(to_string(Hardest_brake).length()+2) <<fixed << setprecision(2) << Hardest_brake << " m/s^2";
                }
                    
                cout << "\n";
                cout << "Time fully stopped:" <<  setw(to_string(Time_Stop).length()-1) << fixed << setprecision(2) << Time_Stop << " s";
                cout << "\n";
                cout << "Full distance table written to trip_report.txt"; 

                

                ofstream file_write("trip_report.txt"); 
                if (!file_write.is_open()) {
                    cout << "Error! file trip_report.txt , cannot be opened"; 
                } else {
                    
                    file_write << "time(s)" << "\t"; 
                    file_write << " speed(m/s)" << "\t"; 
                    file_write << " distance(m)" << "\t"; 
                    file_write << "\n" ;

                    for(int i = 0; i < N; i++) {
                        file_write << fixed << setprecision(2) << "\t" << time_array[i] << "\t\t" << speed_array[i] << "\t\t" << distance_array[i] << "\t";
                        file_write << "\n" ;
                    }

                    cout << "\n";

                    file_write << "Trip duration:" << setw(to_string(total_time).length()+4) << fixed << setprecision(2) << total_time << " s";
                    file_write << "\n";
                    file_write << "Distance travelled:" <<  setw(to_string(total_distance).length()-1) <<fixed << setprecision(2) << total_distance << " m";
                    file_write << "\n";

                    if (average_speed != 0.00) {
                        file_write << "Average speed:" <<  setw(to_string(average_speed).length()+4) << fixed << setprecision(2) << average_speed << " m/s";
                        
                    } else {
                         file_write << "Average speed:" <<  setw(to_string(average_speed).length()+5) << "None.";
                    }
                    file_write << "\n";
                    
                    if (Hardest_acceleration != 0.00) {
                         file_write << "Hardest acceleration:" <<  setw(to_string(Hardest_acceleration).length()-3) <<fixed << setprecision(2) << Hardest_acceleration << " m/s^2";
                         file_write << " (between t = " << fixed << setprecision(2) << accel_time_prev << " s and t = " << accel_time << " s)";

                    } else {
                        file_write << "Hardest acceleration:" << setw(to_string(Hardest_acceleration).length()-3) << fixed << setprecision(2) << Hardest_acceleration << " m/s^2"; 
                    }
                   
                    file_write << "\n";

                    if (Hardest_brake != 0.00) {
                        file_write << "Hardest braking:" <<  setw(to_string(Hardest_brake).length()+2) <<fixed << setprecision(2) << Hardest_brake << " m/s^2";
                        file_write << " (between t = " << fixed << setprecision(2) << deccel_time_prev << " s and t = " << deccel_time << " s)";

                    } else {
                        file_write << "Hardest braking:" <<  setw(to_string(Hardest_brake).length()+2) <<fixed << setprecision(2) << Hardest_brake << " m/s^2";
                    }
                    
                    file_write << "\n";
                    file_write << "Time fully stopped:" <<  setw(to_string(Time_Stop).length()-1) << fixed << setprecision(2) << Time_Stop << " s";
                    file_write << "\n";

                    file_write.close();
                }
            }

        } else {
            cout << "The readings provided in the file dont match the readings mentioned at the first line of the file!." << "\n";
            cout << N << " mentioned. " << total_readings<< " available."; 
            
        }
    }

 }



       


        
}