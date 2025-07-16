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
