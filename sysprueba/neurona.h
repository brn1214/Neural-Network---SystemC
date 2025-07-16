
#include "systemc.h"

SC_MODULE(neurona)
{
	sc_in<bool>  clk_i;
	sc_out<int>  addr_bo;
	sc_in<float>   data_bi;
	sc_out<float>  data_bo;
	sc_out<bool> wr_o;
	sc_out<bool> rd_o;

	////////////////////////////////////////////////
	sc_in<int> n_i;
	sc_out<int> fin;

	///////////////////////////////////////////////
	float Input = 0.0f;
	float Weight = 0.0f;
	float bias = 0.0f;
	//bool fin = false;
	float acc = 0;
	int num_n = 0;
	int f = 0;

	SC_HAS_PROCESS(neurona);

	neurona(sc_module_name nm, int num_ne, int fig);
	~neurona();

	void main_m();

	/*void neur() {

		if ((dir < 4) && memoria) {
			rd_o = true;
			if (m) {
				addr_o.write(dir);
				m = false;
				next_trigger();
			}
			else {
				addr_o.write(dir + 2);
				dir++;
				m = true;
				next_trigger();
			} 
		}
		else if ((dir >= 4) && memoria) {
			fin = true;
			float sig = 1 / (1 + (exp(-acc)));
			Sigma.write(sig);
		} 
	}

	void sigma() {
		if ((dir < 4) && memoria) {
			if (!m) {
				Input = data_i.read();
			}
			else {
				Weight = data_i.read();
			}
			acc += Input * Weight;
			acco.write(acc);
		}
	}*/
	

private:
	float mem_i[512] = { 0 };
	int pos = 0;
	int me_p;
	void storage(int figure);
	float busn_read(int addr);
	void busn_write(int addr, float data);
	void sigma(int d);
	void storage_sig();
	void sal(int d, int fig);

};