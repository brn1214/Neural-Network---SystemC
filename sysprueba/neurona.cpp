#include "neurona.h"
#include <string>

neurona::neurona(sc_module_name nm, int num_ne, int fig)
    :sc_module(nm),
    clk_i("clk_i"),
    addr_bo("addr_bo"),
    data_bi("data_bi"),
    data_bo("data_bo"),
    wr_o("wr_o"),
    rd_o("rd_o"),
    n_i("ni"),
    fin("fin")
{
    addr_bo.initialize(0);
    data_bo.initialize(0);
    wr_o.initialize(0);
    rd_o.initialize(0);
    //fin.initialize(0);

    //SC_METHOD(main_m);
    //sensitive << clk_i.pos();
    num_n = num_ne;
    f = fig;

    SC_CTHREAD(main_m, clk_i.pos());
    
}

neurona::~neurona()
{
}

void neurona::main_m() {

    int state = 1;
    bool flag = false;

    while (true) {

            switch (state)
            {
            case 1:
                if (n_i.read() == num_n) {
                    //cout << "enta 1 neu **********" << num_n << endl;
                    storage(f);
                    fin.write(num_n);
                    state = 2;
                    //cout << " At " << sc_time_stamp() << "sale 1 neu **********" << num_n << endl;
                }
                break;
            case 2:
                wait(clk_i.posedge_event());
                //cout << "entra a 2 n " << num_n << endl;
                for (int h = 0; h < 6; h++) {
                    sigma(h);
                    wait(clk_i.posedge_event());
                }
                fin.write(0);
                //cout << " At " << sc_time_stamp() << "sale 2 neu **********" << num_n << endl;
                state = 3;
                //fin.write(num_n);
                break;
            case 3:
                wait(clk_i.posedge_event());
                
                if (n_i.read() == num_n && (num_n <= 6)) {
                    //cout << " At " << sc_time_stamp() << "enta 3 neu **********" << num_n << endl;
                    for (int i = 0; i < 6; i++) {
                        //cout << " At " << sc_time_stamp() << " enta a cicli 3 neu ********** " << num_n << endl;

                        float s = mem_i[500 + i];
                        busn_write(2000 + i + (6 * (num_n - 1)), s);
                        pos++;
                        //sc_stop();
                        wait();
                    }
                    //cout << "fin neuro " << num_n << endl;
                    fin.write(num_n);
                    state = 4;
                    //bool flag = false;
                }
                break;
            case 4:
                wait(clk_i.posedge_event());
                if (n_i.read() == 1) flag = true;
                if (n_i.read() == num_n && flag) {
                    //cout << "enta 4 neu **********" << num_n << endl;

                    storage_sig();
                    fin.write(num_n);
                    state = 5;
                    //cout << " At " << sc_time_stamp() << "sale 4 neu **********" << num_n << endl;
                }
                break;
            case 5:
                //cout << "entra 5 neu **********" << num_n << endl;
                
                wait(clk_i.posedge_event());
                sal(num_n-1, num_n);

                wait(clk_i.posedge_event());
                
                fin.write(0);
                //cout << " At " << sc_time_stamp() << "sale 5 neu **********" << num_n << endl;
                state = 6;
                //fin.write(num_n);
                break;
            default:
                wait();
            }

        wait(SC_ZERO_TIME);
    }

}

void neurona::storage(int figure) {

    int fig = 0;
    switch (figure)
    {
    case 0:
        fig = 1600;
        break;
    case 1:
        fig = 1650;
        break;
    case 2:
        fig = 1700;
        break;
    case 3:
        fig = 1750;
        break;
    default:
        break;
    }

    fin.write(0);
    wait(clk_i.posedge_event());
    //bias
    for (int i = 0; i < 30; i++) {

        float val = busn_read(i);
        mem_i[i] = val;
    }
    //weights
    for (int i = 30+((324-30)*(num_n -1)); i < 324+(49*6*(num_n -1)); i++) {

        float val = busn_read(i);
        mem_i[30+i- (30 + ((324 - 30) * (num_n - 1)))] = val;
    }
    for (int i = fig; i < fig+49; i++) {

        float val = busn_read(i);
        mem_i[400 + (i-fig)] = val;
    }
}

void neurona::storage_sig() {
     
    fin.write(0);
    wait(clk_i.posedge_event());
    //bias
    for (int i = 1500; i < 1503; i++) {

        float val = busn_read(i);
        mem_i[i-1500] = val;
    }
    //weights
    for (int i = 1503 + ( 30 * (num_n - 1)); i < 1503 + ( 30 * (num_n - 1))+30; i++) {

        float val = busn_read(i);
        mem_i[i+50 - (1503 + (30 * (num_n - 1)))] = val;
    }
    for (int i = 2000; i < 2029; i++) {

        float val = busn_read(i);
        mem_i[10 + (i - 2000)] = val;
    }
}

void neurona::sigma(int d) { 

    // [0:29] input bias
    // [30:79] weights ......
    acc = 0;

        for (int j = 30+(49*d); j < 30 + (49 * d) +49; j++) {

            Input = mem_i[400+(j-(30 + (49 * d)))];
            Weight = mem_i[j];
            acc += Input * Weight;
            /*cout << "**************************************************************" << endl;
            cout << " neuro accumulado " << num_n << " :  " << endl;
            cout << "  -> int: " << Input << endl;
            cout << "  -> wei: " << Weight << endl;
            cout << "  -> acc: " << acc << endl;*/
            
        }
        bias = mem_i[d + (6 * (num_n - 1))];
        acc += bias;

        /*cout << "**************************************************************" << endl;
        cout << " At " << sc_time_stamp() << " neuro accumulado " << num_n << ":  " << endl;
        cout << "  -> bias: " << bias << endl;
         
        cout << "  -> acc: " << acc << endl;*/
        
        

        float sig = 1 / (1 + (exp(-acc)));
        /*cout << "****************************************************" << endl;
        cout << " At " << sc_time_stamp() << " neuro" << num_n << ":  " << endl;
        cout << "  -> sig: " << sig << endl;*/
        
        
        //wait(clk_i.posedge_event());
        mem_i[d + 500] = sig;
        
     
}

void neurona::sal(int d, int fig) {

    acc = 0;
    std::string fi;
    if (fig == 1) fi = "Circle";
    else if (fig == 2) fi = "Triangle";
    else if (fig == 3) fi = "Square";

   
    for (int j = 10 ; j < 40; j++) {

        Input = mem_i[j];
        Weight = mem_i[50+(j-10)];
        acc += Input * Weight;
    }
    bias = mem_i[d];
    acc += bias;


    /*cout << "****************************************************" << endl;
    cout << " At " << sc_time_stamp() << " neuro accumulado " << num_n << ":  " << endl;
    cout << "  -> int: " << Input << endl;
    cout << "  -> wei: " << Weight << endl;
    cout << "  -> acc: " << acc << endl;*/
    

    float sig = 1 / (1 + (exp(-acc)));
    /*cout << "\n\n********************* " << fi << " ***************************" << fig << endl;
    cout << " At " << sc_time_stamp() << " neuro " << num_n << ":  " << endl;
    cout << "  -> Percentage: " << sig*100 << "%" << endl;*/

    mem_i[d + 500] = sig;
    busn_write(d + 500, sig);
}


float neurona::busn_read(int addr) {
    float data;

    //next_trigger(10, SC_NS);
    //wait();
    addr_bo.write(addr);
    rd_o.write(1);

    wait();
    rd_o.write(0);

    wait();
    data = data_bi.read();

    /*cout << "neuro  " << num_n << " write " << " At " << sc_time_stamp()<< endl;
    cout << "  -> addr: " << addr << endl;
    cout << "  -> data: " <<  data << endl;*/
    

    return data;
}

void neurona::busn_write(int addr, float data)
{
    wait();
    addr_bo.write(addr);
    data_bo.write(data);
    wr_o.write(1);

    wait();
    wr_o.write(0);

    /*cout << "neuro: " << num_n << " WRITE " << endl;
    cout << "  -> addr: " << addr << endl;
    cout << "  -> data: " << data << endl;*/
    
    

}