#ifndef _CPU_i_H
#define _CPU_i_H

#include "systemc.h"


SC_MODULE(process)
{
    /////////////////////////memoria int/////////////
    sc_in<bool>  clk_i;
    sc_out<int>  add_e;
    sc_in<float>   data_i_e;
    sc_out<float>  data_o_e;
    sc_out<bool> wr_e;
    sc_out<bool> rd_e;

    ////////////////////////// ext //////////////////
    sc_in<float> data_bi;
    sc_out<float>  data_bo;
    sc_out<int> addr_bo;
    sc_in<int> p_i;
    sc_out<bool> wr_o;
    sc_out<bool> rd_o;
    sc_out<bool> fin;
    ///////////////////////////////////////////////////

    SC_HAS_PROCESS(process);

    process(sc_module_name nm);
    ~process();

    void mainThread_i();
private:
    
    void bus_write_e(int addr, float data);
    float  bus_read_e(int addr);
    void bus_write_i(int addr, float data);
    float  bus_read_i(int addr);

};


#endif