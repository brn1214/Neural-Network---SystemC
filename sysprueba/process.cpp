
#include "process.h"

using namespace std;

process::process(sc_module_name nm)
    :sc_module(nm),
    clk_i("clk_i"),
    add_e("add_e"),
    data_i_e("data_i_e"),
    data_o_e("data_oe"),
    wr_e("wr_e"),
    rd_e("rd_e"),
    data_bi("data_bi"),
    data_bo("data_bo"),
    addr_bo("addr_bo"),
    p_i("p_i"),    
    wr_o("wr_o"),
    rd_o("rd_o"),
    fin("fin")
{
    add_e.initialize(0);
    addr_bo.initialize(0);
    data_bo.initialize(0);
    wr_o.initialize(0);
    rd_o.initialize(0);
    data_o_e.initialize(0);
    fin.initialize(0);
    wr_e.initialize(0);
    rd_e.initialize(0);

    SC_METHOD(mainThread_i);
    sensitive << clk_i.pos();
    /*SC_CTHREAD(mainThread_i, clk_i.pos());
    sensitive << p_i << clk_i.pos();*/
    

}

process::~process()
{

}

void process::mainThread_i()
{
    float tem = 0;
    fin.write(0);
    if (p_i.read() == 1) {
        for (int i = 0; i < 8; i++)
        {
            tem = bus_read_e(i);
            cout << "leido:  " << endl;
            cout << "  -> lec: " << tem << endl;
            
            bus_write_i(i, tem);
        }
        for (int i = 0; i < 8; i++)
        {
            float inp = bus_read_i(i);
            cout << "red2:  " << endl;
            cout << "  -> red2: " << inp << endl;
        }
        fin.write(1);
        sc_stop();
    }
    
}

float process::bus_read_i(int addr)
{
    float data;

    next_trigger(10, SC_NS);
    add_e.write(addr);
    rd_e.write(1);

    next_trigger(10, SC_NS);
    rd_e.write(0);

    next_trigger(10, SC_NS);
    data = data_i_e.read();

    cout << "CPU: READ " << endl;
    cout << "  -> addr: " << hex << addr << endl;
    cout << "  -> data: " << data << endl;

    return data;

}

void process::bus_write_i(int addr, float data)
{
    next_trigger(10, SC_NS);
    add_e.write(addr);
    data_o_e.write(data);
    wr_e.write(1);

    next_trigger(10, SC_NS);
    wr_e.write(0);

    cout << "CPU: WRITE " << endl;
    cout << "  -> addr: " << hex << addr << endl;
    cout << "  -> data: " << data << endl;

}

float process::bus_read_e(int addr)
{
    float data;

    next_trigger(20, SC_NS);
    addr_bo.write(addr);
    rd_o.write(1);

    next_trigger(20, SC_NS);
    rd_o.write(0);

    next_trigger(20, SC_NS);
    data = data_bi.read();

    cout << "CPU: READ ex" << endl;
    cout << "  -> addr: " << hex << addr << endl;
    cout << "  -> data: " << data << endl;

    return data;

}

void process::bus_write_e(int addr, float data)
{
    next_trigger(10, SC_NS);
    addr_bo.write(addr);
    data_bo.write(data);
    wr_o.write(1);

    next_trigger(10, SC_NS);
    wr_o.write(0);

    cout << "lectura: WRITE " << endl;
    cout << "  -> addr: " << hex << addr << endl;
    cout << "  -> data: " << data << endl;

}