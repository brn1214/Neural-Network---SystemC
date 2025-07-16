
//file: driver.cpp
// this file contains the process definitions
#include "driver.h"

void driver::drivea()
{ 
	ws.write((sc_bit)true);
	dir.write(1);//(ba)=00
	write.write((float)1.1);//(ba)=00
	ws.write((sc_bit)false);
	wait(5, SC_NS);

	ws.write((sc_bit)true);
	dir.write(2);//(ba)=00
	write.write((float)2.2);//(ba)=00
	ws.write((sc_bit)false);
	wait(5, SC_NS);

	ws.write((sc_bit)true);
	dir.write(3);//(ba)=00
	write.write((float)3.3);//(ba)=00
	ws.write((sc_bit)false);
	wait(5, SC_NS);

	ws.write((sc_bit)true);
	dir.write(4);//(ba)=00
	write.write((float)4.4);//(ba)=00
	ws.write((sc_bit)false);
	wait(5, SC_NS);

	/*sig.write((sc_bit)false);
	d_a.write((float)1.1);//(ba)=00
	wait(5, SC_NS);
	d_a.write((float)2.1);//(ba)=01
	wait(5, SC_NS);
	d_a.write((float)3.1);//(ba)=10, false=0
	wait(5, SC_NS);
	
	d_a.write((float)4.1);//(ba)=11, true=1
	sig.write((sc_bit)true);
	wait(10, SC_NS);*/
}

void driver::driveb()
{
	rs.write((sc_bit)true);
	dir.write(1);//(ba)=00
	read.read();//(ba)=00
	rs.write((sc_bit)false);
	wait(5, SC_NS);

	ws.write((sc_bit)true);
	dir.write(2);//(ba)=00
	write.write((float)2.2);//(ba)=00
	ws.write((sc_bit)false);
	wait(5, SC_NS);

	ws.write((sc_bit)true);
	dir.write(3);//(ba)=00
	write.write((float)3.3);//(ba)=00
	ws.write((sc_bit)false);
	wait(5, SC_NS);

	ws.write((sc_bit)true);
	dir.write(4);//(ba)=00
	write.write((float)4.4);//(ba)=00
	ws.write((sc_bit)false);
	wait(5, SC_NS);
	/*d_b.write((float)1.3);
	wait(10, SC_NS);
	d_b.write((float)1.2);
	wait(5, SC_NS);*/
}