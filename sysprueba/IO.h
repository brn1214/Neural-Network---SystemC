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