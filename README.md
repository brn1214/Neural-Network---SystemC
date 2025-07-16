# Neural-Network---SystemC
Implemented a NN in SystemC that identifies a circle, an square, a triangle or non-figure.

> ⚠️ **IMPORTANT:** These are just my quick notes, not a final report or something similar.

## Main.cpp

In this part of the code it is only defined each of the modules, we can see that we defined each module the same as when we instantiate a function. 

```cpp
#include "cpu.h"
#include "mem.h"
#include "neurona.h"
#include "IO.h"

enum figure
{
    Circle,
    Triangle,
    Square,
    Noisy
};

int sc_main(int argc, char* argv[])
{
    int figure = Square;

    CPU cpu_core("cpu");
    Mem memory("memory");
    neurona neuro("neuro", 1, figure);
    neurona neuro2("neuro2", 2, figure);
    neurona neuro3("neuro3", 3, figure);
    neurona neuro4("neuro4", 4, figure);
    neurona neuro5("neuro5", 5, figure);
    IO io("io", figure);
```

Then we define the signals (or we can see them as cables). This part is similar to systemverilog when we define the wires and so on. 

```cpp
    sc_clock clk("clk", sc_time(10, SC_NS));
    sc_signal<int, SC_MANY_WRITERS> addr;
    sc_signal<float, SC_MANY_WRITERS> data_cpu_bo;
    sc_signal<float, SC_MANY_WRITERS> data_cpu_bi;
    sc_signal<bool, SC_MANY_WRITERS> wr;
    sc_signal<bool, SC_MANY_WRITERS> rd;
    sc_signal<int> neu;
    sc_signal<int, SC_MANY_WRITERS> fin;

    cpu_core.clk_i(clk);
    cpu_core.addr_bo(addr);
    cpu_core.data_bi(data_cpu_bi);
    cpu_core.data_bo(data_cpu_bo);
    cpu_core.wr_o(wr);
    cpu_core.rd_o(rd);
    cpu_core.neuro(neu);
    cpu_core.fin(fin);

    memory.clk_i(clk);
    memory.addr_bi(addr);
    memory.data_bi(data_cpu_bo);
    memory.data_bo(data_cpu_bi);
    memory.wr_i(wr);
    memory.rd_i(rd);

    neuro.clk_i(clk);
    neuro.addr_bo(addr);
    neuro.data_bi(data_cpu_bi);
    neuro.data_bo(data_cpu_bo);
    neuro.wr_o(wr);
    neuro.rd_o(rd);
    neuro.n_i(neu);
    neuro.fin(fin);

    neuro2.clk_i(clk);
    neuro2.addr_bo(addr);
    neuro2.data_bi(data_cpu_bi);
    neuro2.data_bo(data_cpu_bo);
    neuro2.wr_o(wr);
    neuro2.rd_o(rd);
    neuro2.n_i(neu);
    neuro2.fin(fin);

    neuro3.clk_i(clk);
    neuro3.addr_bo(addr);
    neuro3.data_bi(data_cpu_bi);
    neuro3.data_bo(data_cpu_bo);
    neuro3.wr_o(wr);
    neuro3.rd_o(rd);
    neuro3.n_i(neu);
    neuro3.fin(fin);

    neuro4.clk_i(clk);
    neuro4.addr_bo(addr);
    neuro4.data_bi(data_cpu_bi);
    neuro4.data_bo(data_cpu_bo);
    neuro4.wr_o(wr);
    neuro4.rd_o(rd);
    neuro4.n_i(neu);
    neuro4.fin(fin);

    neuro5.clk_i(clk);
    neuro5.addr_bo(addr);
    neuro5.data_bi(data_cpu_bi);
    neuro5.data_bo(data_cpu_bo);
    neuro5.wr_o(wr);
    neuro5.rd_o(rd);
    neuro5.n_i(neu);
    neuro5.fin(fin);

    io.clk_i(clk);
    io.addr_bo(addr);
    io.data_bi(data_cpu_bi);
    io.data_bo(data_cpu_bo);
    io.wr_o(wr);
    io.rd_o(rd); 
    io.fin(fin);
    io.neuro(neu);

    sc_trace_file* wf = sc_create_vcd_trace_file("wave");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, addr, "addr_bo");
    sc_trace(wf, data_cpu_bi, "data_bi");
    sc_trace(wf, data_cpu_bo, "data_bo");
    sc_trace(wf, wr, "wr");
    sc_trace(wf, rd, "rd");

    sc_start();


    sc_close_vcd_trace_file(wf);

    // Cálculo del tiempo transcurrido
    sc_time elapsed_time = sc_time_stamp();

    // Imprimir el tiempo transcurrido en la consola
    cout << "Tiempo transcurrido: " << elapsed_time.to_seconds() * 1000000 << " microsegundos" << endl;


    return(0);

}
```

## IO.h

In this code we just define the inputs and the outputs to this module and the functions.

```cpp
#ifndef _IO_H
#define _IO_H

#include "systemc.h"

SC_MODULE(IO)
{  
    sc_in<bool>  clk_i;
    sc_out<int>  addr_bo;
    sc_in<float>   data_bi;
    sc_out<float>  data_bo;
    sc_out<bool> wr_o;
    sc_out<bool> rd_o;
    sc_in<int> neuro;

    sc_out<int> fin;
    int f = 0;
    

    SC_HAS_PROCESS(IO);

    IO(sc_module_name nm, int fig);
    ~IO();

    void mainThreadio();
    void read_weights();
    void read_circle();
    void read_sqr();
    void read_tr();
    void read_noisy();
    void print(int f);
    void bus_write(int add, float data);
    float bus_read(int i);


private:
    float mem_io[2048] = { 0 };

};


#endif
```

## IO.cpp

This is the file that reads the data for the weights and the information for the pictures (square, triangle, etc). the weights and the information for the pictures are saved in .txt files, so this code read that information and load it to the memory of the program. It also prints the data in the screen.

```cpp
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
```
in this part of the code we read all that information from the different txt files. Each function read a file from a different picture, as the name of the files shows it. After reading the information from the txt, this program saves that data in a local memory located in IO.

```cpp
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


in this part of the code we read all that information that we saved in the local memory from IO, and send it to the general memory of the system (Mem.h) 

    for (int i = 0; i <= 1799; i++) {
        float dat = mem_io[i];
        bus_write(i, dat);
        
    }
    print(f);
    fin.write(5);
    
    wait(clk_i.posedge_event());
```

in this part of the code we enter an infinite loop and wait for a signal that we will receive from the module CPU, this signal is sent by the line calle “neuro” and the value we want is a 9, as we can see in the code. After we receive this, we read the final prediction for each figure in the general memory MEM and then print the results in the screen. 

```cpp
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
```
The following codes just define each of the functions we used in the previous part of the code, for example, reading the weights from the .txt file, reading each of the figures from the txt, sending data to the memory MEM and so on.

```cpp
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
```

## Mem.h

this is the module for the general memory. in this code we just define the inputs and outputs. We also define the size of the memory. it has 2048 slots, and we can see also that is defined as a float, it  means that in this memory we have 2048 floats, we know that a float size is 4 bytes, so the size is 2048*4 = 8192 bytes, or 8kB. 

```cpp
#ifndef _MEM_H
#define _MEM_H

#include "systemc.h"

SC_MODULE(Mem)
{

    sc_in<bool>  clk_i;
    sc_in<int>   addr_bi;
    sc_in<float>   data_bi;
    sc_out<float>  data_bo;
    sc_in<bool>  wr_i;
    sc_in<bool>  rd_i;

    SC_HAS_PROCESS(Mem);

    Mem(sc_module_name nm);
    ~Mem();

    void bus_write();
    void bus_read();

    float mem[2048] = {0};

};


#endif
```
## Mem.cpp

Here we define the 2 functions of the memory, read and write data. The read waits for a signal in the line “wr_i” and then it reads the addres of the memory and writes the data that is in the “data_bi” line. The write waits for a signal in “rd_i” and then the addres in “addr_bi” and sends the data that is saved in that addres of the memory.

```cpp
#include "mem.h"

Mem::Mem(sc_module_name nm)
    :sc_module(nm),
    clk_i("clk_i"),
    addr_bi("addr_bi"),
    data_bi("data_bi"),
    data_bo("data_bo"),
    wr_i("wr_i"),
    rd_i("rd_i")
{
    data_bo.initialize(0);

    SC_METHOD(bus_write);
    sensitive << clk_i.pos() << rd_i;

    SC_METHOD(bus_read);
    sensitive << clk_i.pos() << wr_i;

}

Mem::~Mem()
{
}

void Mem::bus_read()
{
    if (wr_i.read())
        mem[addr_bi.read()] = data_bi.read();
}

void Mem::bus_write()
{
    if (rd_i.read())
        data_bo.write(mem[addr_bi.read()]);
}
```

## Cpu.h

Here we define the inputs and outputs

```cpp
#ifndef _CPU_H
#define _CPU_H

#include "systemc.h"

SC_MODULE(CPU)
{
    sc_in<bool>  clk_i;
    sc_out<int>  addr_bo;
    sc_in<float>   data_bi;
    sc_out<float>  data_bo;
    sc_out<bool> wr_o;
    sc_out<bool> rd_o;

    sc_out<int> neuro;
    sc_in<int> fin;

    SC_HAS_PROCESS(CPU);

    CPU(sc_module_name nm);
    ~CPU();

    void mainThread();
private:

    void bus_write(int addr, float data);
    int  bus_read(int addr);

};


#endif
```
## Cpu.cpp

This module Works like a control for the system, it sends the instructions to the neural network module to start doing their calculations and so on.

```cpp
#include "cpu.h"

using namespace std;

CPU::CPU(sc_module_name nm)
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
    neuro.initialize(0);

    SC_CTHREAD(mainThread, clk_i.pos());

}

CPU::~CPU()
{

}

void CPU::mainThread()
{
    bool f = true;
    int m = 1;
    int neuro_n = 1;
    int state = 0;
    while (true) {
        int data_size = 10;
```
we can see that we have a switch case statement, like in a state machine, the cpu will control what the neuron will do next.

```cpp
        switch (state)
        {
```
First in the case 0, it waits for a signal in the “fin” line that is sent by the module IO, when it finishes loading the data from the txt files to the memory. then it sends a signal by the line “neuro”, this signal is given by the variable “neuro_n” which value is 1 as we can see above, this means that it says to the neuron number 1 that it can start working.
```cpp
        case 0:
            if (fin.read() == 5) {

                neuro.write(neuro_n);
                wait(clk_i.posedge_event());
                wait(clk_i.posedge_event());
                state = 1;
            }
            //cout << "sale 1 cpu";
            break;
```
when a neuron finish its processing, they send a signal by the line “fin”, so in this case 1, we wait for a number 1 in the line “fin”, when we read it, the counter “neuro_n” increases by 1, and then send this number to the line “neuro”, this means that it says to the neuron 2 that it can star working, and then it waits for this neuron 2 to finish, then it says to the neuron 3 and so on. When we get a signal from the neuron 5, we continue to the next case and reset the neuron counter to 1, and tell the neuron 1 that it can continue working.
```cpp
        case 1:   // lectura de datos
            if (fin.read() == neuro_n) {
                //cout << " At " << sc_time_stamp() << "enta 1 cpu************" << endl;
                neuro_n = neuro_n + 1;
                neuro.write(neuro_n);
                //cout << " At " << sc_time_stamp() << "escribe num 1 cpu************" << endl;
            }
            if (neuro_n > 5 && fin.read() == 5) {
                wait(clk_i.posedge_event());
                //cout << "sale 1 cpu********" << endl;
                state = 2;
                neuro_n = 1;
                neuro.write(1);
            }
            break;
```
in the case 2, we wait for a signal from the neuron 1, and then send a signal to the neuron 2 and so on, until we get to the neuron 5. When we get the response from the neuron 5, we continue to the case 3 and tell the neuron 1 that it can continue working.
```cpp
        case 2:     // escritura de pesos
            if (fin.read() == neuro_n) {
                //cout << "entra  2 cpu *****" << endl;
                neuro_n = neuro_n + 1;
                if (neuro_n > 5) neuro_n = 5;
                neuro.write(neuro_n);
                wait(clk_i.posedge_event());
            }

            if (neuro_n == 5 && fin.read() == 5) {
                state = 3;
                neuro.write(0);
                wait(clk_i.posedge_event());
                //cout << "sale 2 cpu*******" << endl;
                //sc_stop();
                state = 3;
                neuro_n = 1;
                neuro.write(1);
            }
            break;
```
in the case 3, we again wait for a signal from the neuron 1, increase the counter and tell the neuron 2 and so on, but this time we increase the counter only up to the neuron 3. When we get the response from the neuron 3 that it had finished, we reset the counter to 1 and continue to the case 4.
```cpp
        case 3:
            if (fin.read() == neuro_n) {
                //cout << " At " << sc_time_stamp() << "enta 3 cpu************" << endl;
                neuro_n = neuro_n + 1;
                
                if (neuro_n > 3) neuro_n = 3;
                //neuro_n = neuro_n + 1;
                wait(clk_i.posedge_event());

                neuro.write(neuro_n);
                //cout << " At " << sc_time_stamp() << "escribe n 3 cpu************" << endl;
            }
            if (fin.read() == 3 && neuro_n == 3) {
                //(neuro_n > 3 && fin.read() == 3) {
                wait(clk_i.posedge_event());
                /////cout << "termina cpu*******" << endl;
                state = 4; 
            }
            break;
```
here, it is supposed to have finished all the calculations from the neural network, so we send a signal with the number 9 by the line “neuro”.
```cpp
        case 4:
            wait(clk_i.posedge_event());
            neuro.write(9);
            //sc_stop();
            break;
        default:
            break;
            
        }

        wait(SC_ZERO_TIME);
    }
}
```
Here we just defined the read and write to memory, but I think in this code it isn’t used it at all so maybe I will delete it.

```cpp
int CPU::bus_read(int addr)
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

void CPU::bus_write(int addr, float data)
{
    wait();
    addr_bo.write(addr);
    data_bo.write(data);
    wr_o.write(1);

    wait();
    wr_o.write(0);

    /*cout << "CPU: WRITE " << endl;
    cout << "  -> addr: " << addr << endl;
    cout << "  -> data: " << data << endl;*/
    

}

Neuron.h
Here we define the inputs and outputs and the constructors for the functions.
#include "systemc.h"

SC_MODULE(neurona)
{
	sc_in<bool>  clk_i;
	sc_out<int>  addr_bo;
	sc_in<float>   data_bi;
	sc_out<float>  data_bo;
	sc_out<bool> wr_o;
	sc_out<bool> rd_o;

	////////////////////////////////////////////////
	sc_in<int> n_i;
	sc_out<int> fin;

	///////////////////////////////////////////////
	float Input = 0.0f;
	float Weight = 0.0f;
	float bias = 0.0f;
	//bool fin = false;
	float acc = 0;
	int num_n = 0;
	int f = 0;

	SC_HAS_PROCESS(neurona);

	neurona(sc_module_name nm, int num_ne, int fig);
	~neurona();

	void main_m();

	
private:
	float mem_i[512] = { 0 };
	int pos = 0;
	int me_p;
	void storage(int figure);
	float busn_read(int addr);
	void busn_write(int addr, float data);
	void sigma(int d);
	void storage_sig();
	void sal(int d, int fig);

};
```

## Neuron.cpp
```cpp
#include "neurona.h"
#include <string>
```
In this code, we have some arguments in the fuction like the num_ne, and fig. it is because we assigned a number to each neuron, so they can know to which neuron we are trying to communicate. This number is given in the code “main”. The variable for this number is then assigned to the variable num_n (“number neuron”). Also the argument fig, it means which of the figures we are analyzing.

```cpp
neurona::neurona(sc_module_name nm, int num_ne, int fig)
    :sc_module(nm),
    clk_i("clk_i"),
    addr_bo("addr_bo"),
    data_bi("data_bi"),
    data_bo("data_bo"),
    wr_o("wr_o"),
    rd_o("rd_o"),
    n_i("ni"),
    fin("fin")
{
    addr_bo.initialize(0);
    data_bo.initialize(0);
    wr_o.initialize(0);
    rd_o.initialize(0);
    //fin.initialize(0);

    //SC_METHOD(main_m);
    //sensitive << clk_i.pos();
    num_n = num_ne;
    f = fig;

    SC_CTHREAD(main_m, clk_i.pos());
    
}

neurona::~neurona()
{
}
```
This code works similar as the cpu, we have different cases that will control the instructions step by step.

```cpp
void neurona::main_m() {

    int state = 1;
    bool flag = false;

    while (true) {

            switch (state)
            {
```
In the case 1, we read the line n_i (neuron input) and if this number match the number assigned to this neuron, we can start working. First we run the function storage(f), it reads the data for the weights and the inputs of the figure from the main memory MEM and save that data in its local memory. when it finishes, it sends a signal by the line “fin”. This signal is the number assigned to the neuron, so the CPU can understand which of the neurons had finished its work.
```cpp
            case 1:
                if (n_i.read() == num_n) {
                    //cout << "enta 1 neu **********" << num_n << endl;
                    storage(f);
                    fin.write(num_n);
                    state = 2;
                    //cout << " At " << sc_time_stamp() << "sale 1 neu **********" << num_n << endl;
                }
                break;
```
in the case 2, we calculate only 6 of the hidden nodes and save the values. This means that each neuron module, calculate 6 and as they are 5 modules, it’s equal to 30 nodes in the hidden layer.
```cpp
            case 2:
                wait(clk_i.posedge_event());
                //cout << "entra a 2 n " << num_n << endl;
                for (int h = 0; h < 6; h++) {
                    sigma(h);
                    wait(clk_i.posedge_event());
                }
                fin.write(0);
                //cout << " At " << sc_time_stamp() << "sale 2 neu **********" << num_n << endl;
                state = 3;
                //fin.write(num_n);
                break;
```
in the case 3, we read the value of each node from the local memory and save it to the general memory MEM, each neuron will read its 6 that calculated before, so at the end, in the memory MEM there will be the 30 values of the nodes.
```cpp
            case 3:
                wait(clk_i.posedge_event());
                
                if (n_i.read() == num_n && (num_n <= 6)) {
                    //cout << " At " << sc_time_stamp() << "enta 3 neu **********" << num_n << endl;
                    for (int i = 0; i < 6; i++) {
                        //cout << " At " << sc_time_stamp() << " enta a cicli 3 neu ********** " << num_n << endl;

                        float s = mem_i[500 + i];
                        busn_write(2000 + i + (6 * (num_n - 1)), s);
                        pos++;
                        //sc_stop();
                        wait();
                    }
                    //cout << "fin neuro " << num_n << endl;
                    fin.write(num_n);
                    state = 4;
                    //bool flag = false;
                }
                break;
```
in the case 4, we read all the weights from the MEM and save them in the local memory. from here, it will only work 3 of the neuron modules.
```cpp
            case 4:
                wait(clk_i.posedge_event());
                if (n_i.read() == 1) flag = true;
                if (n_i.read() == num_n && flag) {
                    //cout << "enta 4 neu **********" << num_n << endl;

                    storage_sig();
                    fin.write(num_n);
                    state = 5;
                    //cout << " At " << sc_time_stamp() << "sale 4 neu **********" << num_n << endl;
                }
                break;
```
in the case 5, we calculate the final output. As we have 3 outputs (circle, triangle, square) we will have 3 neurons, each one calculating one of the outputs. When they finish, they save the value to the memory MEM.
```cpp
            case 5:
                //cout << "entra 5 neu **********" << num_n << endl;
                
                wait(clk_i.posedge_event());
                sal(num_n-1, num_n);

                wait(clk_i.posedge_event());
                
                fin.write(0);
                //cout << " At " << sc_time_stamp() << "sale 5 neu **********" << num_n << endl;
                state = 6;
                //fin.write(num_n);
                break;
            default:
                wait();
            }

        wait(SC_ZERO_TIME);
    }

}

void neurona::storage(int figure) {

    int fig = 0;
    switch (figure)
    {
    case 0:
        fig = 1600;
        break;
    case 1:
        fig = 1650;
        break;
    case 2:
        fig = 1700;
        break;
    case 3:
        fig = 1750;
        break;
    default:
        break;
    }

    fin.write(0);
    wait(clk_i.posedge_event());
    //bias
    for (int i = 0; i < 30; i++) {

        float val = busn_read(i);
        mem_i[i] = val;
    }
    //weights
    for (int i = 30+((324-30)*(num_n -1)); i < 324+(49*6*(num_n -1)); i++) {

        float val = busn_read(i);
        mem_i[30+i- (30 + ((324 - 30) * (num_n - 1)))] = val;
    }
    for (int i = fig; i < fig+49; i++) {

        float val = busn_read(i);
        mem_i[400 + (i-fig)] = val;
    }
}

void neurona::storage_sig() {
     
    fin.write(0);
    wait(clk_i.posedge_event());
    //bias
    for (int i = 1500; i < 1503; i++) {

        float val = busn_read(i);
        mem_i[i-1500] = val;
    }
    //weights
    for (int i = 1503 + ( 30 * (num_n - 1)); i < 1503 + ( 30 * (num_n - 1))+30; i++) {

        float val = busn_read(i);
        mem_i[i+50 - (1503 + (30 * (num_n - 1)))] = val;
    }
    for (int i = 2000; i < 2029; i++) {

        float val = busn_read(i);
        mem_i[10 + (i - 2000)] = val;
    }
}

void neurona::sigma(int d) { 

    // [0:29] input bias
    // [30:79] weights ......
    acc = 0;

        for (int j = 30+(49*d); j < 30 + (49 * d) +49; j++) {

            Input = mem_i[400+(j-(30 + (49 * d)))];
            Weight = mem_i[j];
            acc += Input * Weight;
            /*cout << "**************************************************************" << endl;
            cout << " neuro accumulado " << num_n << " :  " << endl;
            cout << "  -> int: " << Input << endl;
            cout << "  -> wei: " << Weight << endl;
            cout << "  -> acc: " << acc << endl;*/
            
        }
        bias = mem_i[d + (6 * (num_n - 1))];
        acc += bias;

        /*cout << "**************************************************************" << endl;
        cout << " At " << sc_time_stamp() << " neuro accumulado " << num_n << ":  " << endl;
        cout << "  -> bias: " << bias << endl;
         
        cout << "  -> acc: " << acc << endl;*/
        
        

        float sig = 1 / (1 + (exp(-acc)));
        /*cout << "****************************************************" << endl;
        cout << " At " << sc_time_stamp() << " neuro" << num_n << ":  " << endl;
        cout << "  -> sig: " << sig << endl;*/
        
        
        //wait(clk_i.posedge_event());
        mem_i[d + 500] = sig;
        
     
}

void neurona::sal(int d, int fig) {

    acc = 0;
    std::string fi;
    if (fig == 1) fi = "Circle";
    else if (fig == 2) fi = "Triangle";
    else if (fig == 3) fi = "Square";

   
    for (int j = 10 ; j < 40; j++) {

        Input = mem_i[j];
        Weight = mem_i[50+(j-10)];
        acc += Input * Weight;
    }
    bias = mem_i[d];
    acc += bias;


    /*cout << "****************************************************" << endl;
    cout << " At " << sc_time_stamp() << " neuro accumulado " << num_n << ":  " << endl;
    cout << "  -> int: " << Input << endl;
    cout << "  -> wei: " << Weight << endl;
    cout << "  -> acc: " << acc << endl;*/
    

    float sig = 1 / (1 + (exp(-acc)));
    /*cout << "\n\n********************* " << fi << " ***************************" << fig << endl;
    cout << " At " << sc_time_stamp() << " neuro " << num_n << ":  " << endl;
    cout << "  -> Percentage: " << sig*100 << "%" << endl;*/

    mem_i[d + 500] = sig;
    busn_write(d + 500, sig);
}


float neurona::busn_read(int addr) {
    float data;

    //next_trigger(10, SC_NS);
    //wait();
    addr_bo.write(addr);
    rd_o.write(1);

    wait();
    rd_o.write(0);

    wait();
    data = data_bi.read();

    /*cout << "neuro  " << num_n << " write " << " At " << sc_time_stamp()<< endl;
    cout << "  -> addr: " << addr << endl;
    cout << "  -> data: " <<  data << endl;*/
    

    return data;
}

void neurona::busn_write(int addr, float data)
{
    wait();
    addr_bo.write(addr);
    data_bo.write(data);
    wr_o.write(1);

    wait();
    wr_o.write(0);

    /*cout << "neuro: " << num_n << " WRITE " << endl;
    cout << "  -> addr: " << addr << endl;
    cout << "  -> data: " << data << endl;*/
    
    

}
```

## Results

<img width="442" height="231" alt="image" src="https://github.com/user-attachments/assets/ecb1160b-9372-456a-a6dc-228276bf0bad" />

<img width="442" height="231" alt="image" src="https://github.com/user-attachments/assets/c158fab9-b3ea-4c5d-b76e-47ea09b68d39" />

