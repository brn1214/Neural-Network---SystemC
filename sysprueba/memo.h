#include "systemc.h"
#include <iostream>
#include <vector>
using namespace std;

SC_MODULE(mem)
{
	sc_in<int> addr_bo;
	sc_in<float> data_bi;
	sc_out<float> data_bo;
	sc_in<bool> clk;
	
	sc_in<bool> wr_o;
	sc_in<bool> rd_o;
	//std::vector <float, 10> memo;
	float memo[10] ;
	
	void memory() {
		if (wr_o.read()) {
			 
			memo[addr_bo.read()] = data_bi.read();
			 
		}
		else if (rd_o.read()) {
			data_bo.write(memo[addr_bo.read()]);
		}
	}


	SC_CTOR(mem)
	{ 
		SC_METHOD(memory);
		sensitive <<clk.pos(); //addr_bo << wr_o << rd_o;// 
	}
};