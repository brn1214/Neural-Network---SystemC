
#include "cpu.h"
#include "mem.h"
#include "neurona.h"
#include "IO.h"

enum figure
{
    Circle,
    Triangle,
    Square,
    Noisy
};

int sc_main(int argc, char* argv[])
{
    int figure = Circle;

    CPU cpu_core("cpu");
    Mem memory("memory");
    neurona neuro("neuro", 1, figure);
    neurona neuro2("neuro2", 2, figure);
    neurona neuro3("neuro3", 3, figure);
    neurona neuro4("neuro4", 4, figure);
    neurona neuro5("neuro5", 5, figure);
    IO io("io", figure);

    sc_clock clk("clk", sc_time(10, SC_NS));
    sc_signal<int, SC_MANY_WRITERS> addr;
    sc_signal<float, SC_MANY_WRITERS> data_cpu_bo;
    sc_signal<float, SC_MANY_WRITERS> data_cpu_bi;
    sc_signal<bool, SC_MANY_WRITERS> wr;
    sc_signal<bool, SC_MANY_WRITERS> rd;
    sc_signal<int> neu;
    sc_signal<int, SC_MANY_WRITERS> fin;

    cpu_core.clk_i(clk);
    cpu_core.addr_bo(addr);
    cpu_core.data_bi(data_cpu_bi);
    cpu_core.data_bo(data_cpu_bo);
    cpu_core.wr_o(wr);
    cpu_core.rd_o(rd);
    cpu_core.neuro(neu);
    cpu_core.fin(fin);

    memory.clk_i(clk);
    memory.addr_bi(addr);
    memory.data_bi(data_cpu_bo);
    memory.data_bo(data_cpu_bi);
    memory.wr_i(wr);
    memory.rd_i(rd);

    neuro.clk_i(clk);
    neuro.addr_bo(addr);
    neuro.data_bi(data_cpu_bi);
    neuro.data_bo(data_cpu_bo);
    neuro.wr_o(wr);
    neuro.rd_o(rd);
    neuro.n_i(neu);
    neuro.fin(fin);

    neuro2.clk_i(clk);
    neuro2.addr_bo(addr);
    neuro2.data_bi(data_cpu_bi);
    neuro2.data_bo(data_cpu_bo);
    neuro2.wr_o(wr);
    neuro2.rd_o(rd);
    neuro2.n_i(neu);
    neuro2.fin(fin);

    neuro3.clk_i(clk);
    neuro3.addr_bo(addr);
    neuro3.data_bi(data_cpu_bi);
    neuro3.data_bo(data_cpu_bo);
    neuro3.wr_o(wr);
    neuro3.rd_o(rd);
    neuro3.n_i(neu);
    neuro3.fin(fin);

    neuro4.clk_i(clk);
    neuro4.addr_bo(addr);
    neuro4.data_bi(data_cpu_bi);
    neuro4.data_bo(data_cpu_bo);
    neuro4.wr_o(wr);
    neuro4.rd_o(rd);
    neuro4.n_i(neu);
    neuro4.fin(fin);

    neuro5.clk_i(clk);
    neuro5.addr_bo(addr);
    neuro5.data_bi(data_cpu_bi);
    neuro5.data_bo(data_cpu_bo);
    neuro5.wr_o(wr);
    neuro5.rd_o(rd);
    neuro5.n_i(neu);
    neuro5.fin(fin);

    io.clk_i(clk);
    io.addr_bo(addr);
    io.data_bi(data_cpu_bi);
    io.data_bo(data_cpu_bo);
    io.wr_o(wr);
    io.rd_o(rd); 
    io.fin(fin);
    io.neuro(neu);

    sc_trace_file* wf = sc_create_vcd_trace_file("wave");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, addr, "addr_bo");
    sc_trace(wf, data_cpu_bi, "data_bi");
    sc_trace(wf, data_cpu_bo, "data_bo");
    sc_trace(wf, wr, "wr");
    sc_trace(wf, rd, "rd");

    sc_start();


    sc_close_vcd_trace_file(wf);

    // Cálculo del tiempo transcurrido
    sc_time elapsed_time = sc_time_stamp();

    // Imprimir el tiempo transcurrido en la consola
    cout << "Time elapsed: " << elapsed_time.to_seconds() * 1000000 << " microseconds" << endl;


    return(0);

}