//file: monitor.h
//this the monitor module
#include "systemc.h"

SC_MODULE(monitor)
{
	sc_in<bool> ws, rs;
	sc_in<int> dir;
	sc_in<float> write, read, sigma;
	sc_in<bool> clk;

	sc_in<float> acc;
	sc_in<bool> memoria;

	void prc_monitor()
	{
		cout << " At " << sc_time_stamp() << " dir is : ";
		cout << dir << " , write " << write << " read is : " << read;
		cout << " acc " << acc << " sigma is " << sigma;
		cout << " la memo " << memoria << endl;
	}

	SC_CTOR(monitor)
	{

		SC_METHOD(prc_monitor);
		sensitive << clk.pos();
		//whenever the i/p to the gate changes,
		//or the o/p changes, the process prc_monitor triggers
	}
};