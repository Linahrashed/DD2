#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include<math.h>
#include<vector>
#include <algorithm>
using namespace std;


class Annealing_Placer{

public:
    int cells, conections, rows, columns;
    int net_list[1290][210];
    int cell_location[1300][2];
    vector< int > cell_netlist[1300]; //each cell mawgooda f anhe netlist
    int chip[70][70];
    int TWL;
    int net_wl[1290]; //kol net eh el wire length beta3ha
    int new_net_wl[1290];//
    int rand_chip_1[2], rand_chip_2[2];
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
            x_min = cell_location[net_list[i][1]][1];
            x_max = cell_location[(net_list[i][1])][1];
            y_min = cell_location[(net_list[i][1])][0];
            y_max = cell_location[(net_list[i][1])][0];
            cell_netlist[net_list[i][1]].push_back(i);
            for (int j=2; j<=net_list[i][0] ; j++) {
                cell_netlist[net_list[i][j]].push_back(i);
                if (cell_location[(net_list[i][j])][1] < x_min )
                    x_min = cell_location[(net_list[i][j])][1];
                if (cell_location[(net_list[i][j])][1] > x_max )
                    x_max = cell_location[(net_list[i][j])][1];
                if (cell_location[(net_list[i][j])][0] < y_min )
                    y_min = cell_location[(net_list[i][j])][0];
                if (cell_location[(net_list[i][j])][0] > y_max )
                    y_max = cell_location[(net_list[i][j])][0];
            }
            net_wl[i] =(y_max-y_min) + (x_max-x_min);

        }
        
        for (int i = 0; i < conections; i++) {
            WL = WL + net_wl[i];
        }
        copy(net_wl, net_wl+1290, new_net_wl);
        return WL;
    };
    
    
    
    
    //tesdt
    int new_wirelegth (){
        int WL=0;
        int x_min, x_max, y_min, y_max;
        
        if (rand_cell_1 != -1) {
        for (auto& i : cell_netlist[rand_cell_1]) {
            x_min = cell_location[net_list[i][1]][1];
            x_max = cell_location[(net_list[i][1])][1];
            y_min = cell_location[(net_list[i][1])][0];
            y_max = cell_location[(net_list[i][1])][0];
          //  cell_netlist[net_list[i][1]].push_back(i);
            for (int j=2; j<=net_list[i][0] ; j++) {
               // cell_netlist[net_list[i][j]].push_back(i);
                if (cell_location[(net_list[i][j])][1] < x_min )
                    x_min = cell_location[(net_list[i][j])][1];
                if (cell_location[(net_list[i][j])][1] > x_max )
                    x_max = cell_location[(net_list[i][j])][1];
                if (cell_location[(net_list[i][j])][0] < y_min )
                    y_min = cell_location[(net_list[i][j])][0];
                if (cell_location[(net_list[i][j])][0] > y_max )
                    y_max = cell_location[(net_list[i][j])][0];
            }
            new_net_wl[i] =(y_max-y_min) + (x_max-x_min);

        }
        }
        if (rand_cell_2 != -1) {
        for (auto& i : cell_netlist[rand_cell_2]) {
            x_min = cell_location[net_list[i][1]][1];
            x_max = cell_location[(net_list[i][1])][1];
            y_min = cell_location[(net_list[i][1])][0];
            y_max = cell_location[(net_list[i][1])][0];
          //  cell_netlist[net_list[i][1]].push_back(i);
            for (int j=2; j<=net_list[i][0] ; j++) {
               // cell_netlist[net_list[i][j]].push_back(i);
                if (cell_location[(net_list[i][j])][1] < x_min )
                    x_min = cell_location[(net_list[i][j])][1];
                if (cell_location[(net_list[i][j])][1] > x_max )
                    x_max = cell_location[(net_list[i][j])][1];
                if (cell_location[(net_list[i][j])][0] < y_min )
                    y_min = cell_location[(net_list[i][j])][0];
                if (cell_location[(net_list[i][j])][0] > y_max )
                    y_max = cell_location[(net_list[i][j])][0];
            }
            new_net_wl[i] =(y_max-y_min) + (x_max-x_min);

        }
        }
        
        
        for (int i = 0; i < conections; i++) {
            WL = WL + new_net_wl[i];
        }
        return WL;
    };
    
    void swap_random_cells(){
        rand_chip_1[0] = rand() % rows;
        rand_chip_1[1] = rand() % columns;
        do{
            rand_chip_2[0] = rand() % rows;
            rand_chip_2[1] = rand() % columns;
        }while(rand_chip_1[0] == rand_chip_2[0] && rand_chip_1[1] == rand_chip_2[1]);
        
        rand_cell_1 =  chip[rand_chip_1[0]][rand_chip_1[1]];
        rand_cell_2 =  chip[rand_chip_2[0]][rand_chip_2[1]];
        
        
        chip[rand_chip_1[0]][rand_chip_1[1]] = rand_cell_2;
        chip[rand_chip_2[0]][rand_chip_2[1]] = rand_cell_1;
        
        
        if (rand_cell_1 != -1) {
            cell_location[rand_cell_1][0] = rand_chip_2[0];
            cell_location[rand_cell_1][1] = rand_chip_2[1];
        }
        
        if (rand_cell_2 != -1) {
            cell_location[rand_cell_2][0] = rand_chip_1[0];
            cell_location[rand_cell_2][1] = rand_chip_1[1];
        }

    };
    
    void reverse_swap(){
        chip[rand_chip_1[0]][rand_chip_1[1]] = rand_cell_1;
        chip[rand_chip_2[0]][rand_chip_2[1]] = rand_cell_2;
        
        if (rand_cell_1 != -1) {
            cell_location[rand_cell_1][0] = rand_chip_1[0];
            cell_location[rand_cell_1][1] = rand_chip_1[1];
        }
        
        if (rand_cell_2 != -1) {
            cell_location[rand_cell_2][0] = rand_chip_2[0];
            cell_location[rand_cell_2][1] = rand_chip_2[1];
        }
        
    };
    
    void simulated_anealing(){
        initialize_random();
        display();
        initial_temperature = 500 * TWL;
        final_temperature = 5 * pow(10,-6) * TWL / conections;
        temperature = initial_temperature;
        moves = 10 * cells;
        int new_TWL;
        while (temperature > final_temperature) {
            for (int i=0; i<moves; i++) {
                swap_random_cells();
                new_TWL = new_wirelegth();
                if (new_TWL < TWL) {
                    TWL = new_TWL;
                    copy(new_net_wl, new_net_wl+1290, net_wl);
                } else {
                    
                    double probability = 1 - exp( (TWL - new_TWL) / temperature );
                    double result = (double)rand() / RAND_MAX;
                    if (result > probability) {
                        TWL = new_TWL;
                        copy(new_net_wl, new_net_wl+1290, net_wl);
                    } else {
                        reverse_swap();
                        copy(net_wl, net_wl+1290, new_net_wl);
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
                    cout<<"1 ";
                } else {
                    cout<<"0 ";
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
    txt_file.open("/Users/linah/Downloads/t3.txt",ios::in);
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
