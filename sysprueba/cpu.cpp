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

        switch (state)
        {
        case 0:
            if (fin.read() == 5) {

                neuro.write(neuro_n);
                wait(clk_i.posedge_event());
                wait(clk_i.posedge_event());
                state = 1;
            }
            //cout << "sale 1 cpu";
            break;
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
