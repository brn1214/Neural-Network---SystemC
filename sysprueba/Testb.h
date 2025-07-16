#include "systemc.h"

SC_MODULE(Testb)
{
    sc_out<bool> ws, rs;
    sc_out<int> dir;
    sc_out<float> write;
    sc_in<bool> clk;

    sc_out<bool> memoria;

    void StimGen()
    {
        memoria.write(false);
        ws.write(true);
        dir.write(0);//(ba)=00
        write.write((float)0.111);//(ba)=00
        //wait(5, SC_NS);
        wait();
        ws.write(false);
        
        ws.write(true);
        dir.write(1);//(ba)=00
        write.write((float)0.2222);//(ba)=00
        wait();
        ws.write(false);

        ws.write(true);
        dir.write(2);//(ba)=00
        write.write((float)0.33331);//(ba)=00
        wait();
        ws.write(false);
        
        
        ws.write(true);
        dir.write(3);//(ba)=00
        write.write((float)0.44431);//(ba)=00
        wait();
        ws.write(false);


        rs.write(true);
        dir.write(0);//(ba)=00
        wait();
        rs.write(false);



        rs.write(true);
        dir.write(1);//(ba)=00
        wait();
        rs.write(false);


        rs.write(true);
        dir.write(2);//(ba)=00
        wait();
        rs.write(false);

        rs.write(true);
        dir.write(3);//(ba)=00
        wait();
        rs.write(false);
        wait();
        wait();
        wait();
        wait();
        wait();
        wait();
        memoria.write(true);
        

    }
    SC_CTOR(Testb)
    {
        SC_THREAD(StimGen);
        sensitive << clk.pos();
    }
};