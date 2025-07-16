#pragma once
//file: driver.h
// this is the driver program for the OR gate
#include "systemc.h"

SC_MODULE(driver)
{
	
	sc_in<float> read;
	sc_out<int> dir;
	sc_out<float> write; //inputs to the OR gate are of type sc_bit
	sc_out<sc_bit> rs;
	sc_out<sc_bit> ws;

	void drivea();
	void driveb();// these are two processes to stimulate the OR gate

	SC_CTOR(driver)
	{
		SC_THREAD(drivea);
		SC_THREAD(driveb);//processes are called here
	}
};