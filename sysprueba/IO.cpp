#include "IO.h"
#include<iostream>
#include<fstream>
 
using namespace std;

IO::IO(sc_module_name nm, int figure)
    :sc_module(nm),
    clk_i("clk_i"),
    addr_bo("addr_bo"),
    data_bi("data_bi"),
    data_bo("data_bo"),
    wr_o("wr_o"),
    rd_o("rd_o"),
    neuro("neuro"),
    fin("fin")
{
    addr_bo.initialize(0);
    data_bo.initialize(0);
    wr_o.initialize(0);
    rd_o.initialize(0);
    //fin.initialize(0); 
    f = figure;

    SC_CTHREAD(mainThreadio, clk_i.pos());

}

IO::~IO()
{

}

void IO::mainThreadio()
{
    //cout << "\ncirculo \n";
    read_circle();
   // cout << "\ntr \n";
    read_tr();
  //  cout << "\nsqr \n";
    read_sqr();
  //  cout << "\ndat \n";
    read_weights();
  //  cout << "\ndat \n";
    read_noisy();



    for (int i = 0; i <= 1799; i++) {
        float dat = mem_io[i];
        bus_write(i, dat);
        
    }
    print(f);
    fin.write(5);
    
    wait(clk_i.posedge_event());

    while (true) {

        if (neuro.read() == 9) {
            //cout << "finish";
            wait(clk_i.posedge_event());
            wait(clk_i.posedge_event());
            wait(clk_i.posedge_event());
            wait(clk_i.posedge_event());
            wait(clk_i.posedge_event());
            float v1 = bus_read(500);
            wait(clk_i.posedge_event());
            float v2 = bus_read(501);
            wait(clk_i.posedge_event());
            float v3 = bus_read(502);
            cout << "\n\n" << "////////////////////////////////////////" << endl;
            cout << "******* CIRCLE  : " << v1 << endl;
            cout << "////////////////////////////////////////" << endl;
            cout << "******* TRIANGLE  : " << v2 << endl;
            cout << "////////////////////////////////////////" << endl;
            cout << "******* SQUARE : " << v3 << endl;

            sc_stop();
        }

        wait(SC_ZERO_TIME);
    }
    

}

void IO::read_weights() {

       //ifstream inputfile("C:/Users/pret_/OneDrive/Documentos/ITMO/Organizacion/Neural/NeuralNetwork/NeuralNetwork/pesos.txt");
    ifstream inputfile("pesos.txt");

    if (!inputfile.is_open())
        cout << "Error opening file";

    //Defining the loop for getting input from the file

    for (int r = 0; r < 1600; r++) //Outer loop for rows
    {
        inputfile >> mem_io[r];  //Take input from file and put into myArray
    }

    /*for (int r = 0; r < 1600; r++)
    {
        cout << mem_io[r] << " ";
    }*/
    

    inputfile.close();
}

void IO::read_circle() {
   
    //ifstream inputfile("C:/Users/pret_/OneDrive/Documentos/ITMO/Organizacion/Neural/NeuralNetwork/NeuralNetwork/pesos.txt");
    ifstream inputfile("circle.txt");

    if (!inputfile.is_open())
        cout << "Error opening file";

    //Defining the loop for getting input from the file

    for (int r = 1600; r < 1649; r++) //Outer loop for rows
    {
        inputfile >> mem_io[r];  //Take input from file and put into myArray
    }
    /*for (int r = 0; r < 49; r++)
    {
        cout << mem_io[r] << " ";
    }*/
    

    inputfile.close();
}

void IO::read_sqr() {
    
    //ifstream inputfile("C:/Users/pret_/OneDrive/Documentos/ITMO/Organizacion/Neural/NeuralNetwork/NeuralNetwork/pesos.txt");
    ifstream inputfile( "square.txt");

    if (!inputfile.is_open())
        cout << "Error opening file";

    //Defining the loop for getting input from the file

    for (int r = 1650; r < 1699; r++) //Outer loop for rows
    {
        inputfile >> mem_io[r];  //Take input from file and put into myArray
    }
    /*for (int r = 0; r < 49; r++)
    {
        cout << mem_io[r] << " ";
    }*/
    

    inputfile.close();
}

void IO::read_tr() {
    
    //ifstream inputfile("C:/Users/pret_/OneDrive/Documentos/ITMO/Organizacion/Neural/NeuralNetwork/NeuralNetwork/pesos.txt");
    ifstream inputfile("triangle.txt");

    if (!inputfile.is_open())
        cout << "Error opening file";

    //Defining the loop for getting input from the file

    for (int r = 1700; r < 1749; r++) //Outer loop for rows
    {
        inputfile >> mem_io[r];  //Take input from file and put into myArray
    }
    /*for (int r = 0; r < 49; r++)
    {
        cout << mem_io[r] << " ";
    }*/
    

    inputfile.close();
}


void IO::read_noisy() {

    //ifstream inputfile("C:/Users/pret_/OneDrive/Documentos/ITMO/Organizacion/Neural/NeuralNetwork/NeuralNetwork/pesos.txt");
    ifstream inputfile("bad.txt");

    if (!inputfile.is_open())
        cout << "Error opening file";

    //Defining the loop for getting input from the file

    for (int r = 1750; r < 1799; r++) //Outer loop for rows
    {
        inputfile >> mem_io[r];  //Take input from file and put into myArray
    }
    /*for (int r = 0; r < 49; r++)
    {
        cout << mem_io[r] << " ";
    }*/


    inputfile.close();
}


void IO::print(int f) {

    if (f == 0) { f = 1600; }
    else if (f == 1) { f = 1700; }
    else if (f == 2) { f = 1650; }
    else { f = 1750; }
    
    int r = f;
    cout << "\n"; cout << "\n";
    for (int c = 0; c < 7; c++ )
    {
        
        for ( r; r < f + 7; r++) {
            cout << mem_io[r] << " ";
        } 
        f += 7;
        cout <<  "\n";
    }


}

void IO::bus_write(int addr, float data)
{
    wait();
    addr_bo.write(addr);
    data_bo.write(data);
    wr_o.write(1);

    wait();
    wr_o.write(0);
    /*cout << "IO: WRITE " << endl;
    cout << "  -> addr: " << addr << endl;
    cout << "  -> data: " << data << endl;*/
    
    
}


float IO::bus_read(int addr)
{
    float data;

    wait();
    addr_bo.write(addr);
    rd_o.write(1);

    wait();
    rd_o.write(0);

    wait();
    data = data_bi.read();

    /*cout << "CPU: READ " << endl;
    cout << "  -> addr: " << addr << endl;
    cout << "  -> data: " << data << endl;*/


    return data;

}