#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include<math.h>
using namespace std;


class Annealing_Placer{

public:
    int cells, conections, rows, columns;
    int net_list[240][60];
    int cell_location[260][2];
    int chip[25][25];
    int TWL;
    int rand_cell_1, rand_cell_2;
    int moves;
    double initial_temperature, final_temperature, temperature;
    
    Annealing_Placer(int x, int y, int z, int r){
        cells = x;
        conections = y;
        rows = z;
        columns = r;
        for (int i=0; i<rows; i++) {
            for (int j=0; j<columns; j++) {
                chip[i][j]= -1;
            }
        }
    }
    
    void initialize_random(){
        int x,y;
        for (int i=0; i<cells; i++) {
            do{
            y = rand() % rows;
            x = rand() % columns;
            }while (chip[y][x] != -1);
            chip[y][x] = i;
            cell_location[i][0]=y;
            cell_location[i][1]=x;
        }
        TWL = total_wirelegth();
    };
    
    int total_wirelegth (){
        int WL=0;
        int x_min, x_max, y_min, y_max;
        for (int i=0; i<conections; i++) {
            x_min = cell_location[(net_list[i][1])][1];
            x_max = cell_location[(net_list[i][1])][1];
            y_min = cell_location[(net_list[i][1])][0];
            y_max = cell_location[(net_list[i][1])][0];
            for (int j=2; j<=net_list[i][0] ; j++) {
                if (cell_location[(net_list[i][j])][1] < x_min )
                    x_min = cell_location[(net_list[i][j])][1];
                if (cell_location[(net_list[i][j])][1] > x_max )
                    x_max = cell_location[(net_list[i][j])][1];
                if (cell_location[(net_list[i][j])][0] < y_min )
                    y_min = cell_location[(net_list[i][j])][0];
                if (cell_location[(net_list[i][j])][0] > y_max )
                    y_max = cell_location[(net_list[i][j])][0];
            }
            WL = WL + (y_max-y_min) + (x_max-x_min);
        }
        return WL;
    };
    
    void swap_random_cells(){
        rand_cell_1 = rand() % cells;
        do{
        rand_cell_2 = rand() % cells;
        }while(rand_cell_1 == rand_cell_2);
        
        chip[cell_location[rand_cell_1][0]][cell_location[rand_cell_1][1]] = rand_cell_2;
        chip[cell_location[rand_cell_2][0]][cell_location[rand_cell_2][1]] = rand_cell_1;
        int temp_cell_location[2] = {cell_location[rand_cell_1][0],cell_location[rand_cell_1][1]};
        cell_location[rand_cell_1][0] = cell_location[rand_cell_2][0];
        cell_location[rand_cell_1][1] = cell_location[rand_cell_2][1];
        cell_location[rand_cell_2][0] = temp_cell_location[0];
        cell_location[rand_cell_2][1] = temp_cell_location[1];
    };
    
    void reverse_swap(){
        chip[cell_location[rand_cell_1][0]][cell_location[rand_cell_1][1]] = rand_cell_2;
        chip[cell_location[rand_cell_2][0]][cell_location[rand_cell_2][1]] = rand_cell_1;
        int temp_cell_location[2] = {cell_location[rand_cell_1][0],cell_location[rand_cell_1][1]};
        cell_location[rand_cell_1][0] = cell_location[rand_cell_2][0];
        cell_location[rand_cell_1][1] = cell_location[rand_cell_2][1];
        cell_location[rand_cell_2][0] = temp_cell_location[0];
        cell_location[rand_cell_2][1] = temp_cell_location[1];
    };
    
    void simulated_anealing(){
        initialize_random();
        initial_temperature = 500 * TWL;
        final_temperature = 5 * pow(10,-6) * TWL / conections;
        temperature = initial_temperature;
        moves = 10 * cells;
        int new_TWL;
        while (temperature > final_temperature) {
            for (int i=0; i<moves; i++) {
                swap_random_cells();
                new_TWL = total_wirelegth();
                if (new_TWL < TWL) {
                    TWL = new_TWL;
                } else {
                    
                    double probability = 1 - exp( (TWL - new_TWL) / temperature );
                    double result = (double)rand() / RAND_MAX;
                    if (result > probability) {
                        TWL = new_TWL;
                    } else {
                        reverse_swap();
                    }
                }
            }
            cool();
        }
        display();
    };
    
    void cool(){
        temperature = 0.95 * temperature;
       
    };
    
    void display(){
        for (int i = 0; i<rows; i++) {
            for (int j=0; j<columns; j++) {
                if (chip[i][j] == -1) {
                    cout<<"--"<<"\t";
                } else {
                    cout<<chip[i][j]<<"\t";
                }
            }
            cout<<endl;
        }
        cout<<"total wire length = "<<TWL<<endl;
    };

};


int main(){
    srand(time(NULL));
    fstream txt_file;
    txt_file.open("/Users/fadymohsen/Desktop/AUC/Semesters/Fall22/DD2/project/d0.txt",ios::in);
    if ( !(txt_file.is_open()) ) {
        cout<<"can't open text file"<<endl;
        return 0;
    }
    int cells, conections, rows, columns;
    txt_file>>cells>>conections>>rows>>columns;
    
    Annealing_Placer d0(cells, conections, rows, columns);
    
        for ( int i=0 ; i<conections ; i++ ) {
                txt_file >> d0.net_list[i][0];
            for (int j=1; j<=d0.net_list[i][0]; j++) {
                txt_file >> d0.net_list[i][j];
            }
        }
    txt_file.close();
    
    d0.simulated_anealing();
    
    
}
