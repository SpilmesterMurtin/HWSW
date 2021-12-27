#define HelloExample
#include <systemc.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace sc_core;

SC_MODULE(m1) {
	sc_out<int> port1;
	sc_in<bool> rdy;
	SC_CTOR(m1) {
		SC_THREAD(sensor);
		sensitive << rdy;
	}

	void sensor()
	{
		int val;
		srand(time(NULL));

		while (true)
		{
			if (rdy.read() == true)
			{
				val = rand() % 2;
				port1.write(val);
				wait(10, SC_MS);
			}
			else
			{
				wait();
			}
		}
	}

};

SC_MODULE(m3) {
	sc_out<int> port3;
	sc_in<bool> rdy;
	SC_CTOR(m3) {
		SC_THREAD(camera);
		sensitive << rdy;
	}

	void camera()
	{
		srand(time(NULL));
		while (true)
		{
			if (rdy.read() == true)
			{
				//sum of matrix
				int sum = 0;
				//Array containing 4x4 matrix
				int val[16];

				//mock values for the camera
				for (int i = 0; i < 16; i++)
				{
					val[i] = rand() % 10;
				}

				//Calculate sum of matrix
				if (true)
				{
					for (int i = 0; i < 16; i++)
					{
						sum += val[i];
					}
				}
				//write sum to port
				
				wait(20, SC_MS);
				port3.write(sum);
			}
			else
			{
				wait();
			}
			
			

			/*
			
			for (int i = 0; i < 16; i++)
				{
					port3.write(val[i]);
					vld.write(true);
					wait(1, SC_MS);
					vld.write(false);
				}

			*/

			//wait(10, SC_SEC);
		}
	}

};

SC_MODULE(m4) {
	sc_out<bool> vld1;


	SC_CTOR(m4) {
		SC_THREAD(pin);
		//dont_initialize();
	}

	void pin()
	{
		vld1.write(false);

			int pin;
			std::cin >> pin;
			if (pin ==1234)
			{
				vld1.write(true);
			}
			else
			{
				vld1.write(false);
			}
	}
};

SC_MODULE(m2) {
	sc_in<int> port1, port2, port3;
	sc_out<bool> vld1, vld2, vld3;
	sc_in<bool> rdy;

	SC_CTOR(m2) {
		SC_THREAD(dataanalysis);
		//dont_initialize();
		sensitive << port1;
		sensitive << port2;
		sensitive << port3;
		sensitive << rdy;
	}
void dataanalysis()
{
	int sen1, sen2, cam1, in_wait;
	int itr = 0;
	while (true)
	{
		if (rdy.read() == true)
		{
			vld1.write(false);
			vld2.write(false);
			vld3.write(false);

			//read values from sensors and cameras
			if (itr % 2 == 0)
			{
				vld3.write(true);
				vld1.write(true);
				vld2.write(true);
				wait(21, SC_MS);
				vld3.write(false);
				vld1.write(false);
				vld2.write(false);
				cam1 = port3.read();
				sen1 = port1.read();
				sen2 = port2.read();
				wait(29, SC_MS);
			}
			else
			{
				vld1.write(true);
				vld2.write(true);
				wait(11, SC_MS);
				vld1.write(false);
				vld2.write(false);
				sen1 = port1.read();
				sen2 = port2.read();
				wait(39, SC_MS);
			}
			itr++;

			std::cout << cam1 << std::endl;
			std::cout << cam1 * (sen1 + sen2) << std::endl;

			//Check for intrution
			int i_m = cam1 * (sen1 + sen2);
			if (i_m > 1)
			{
				std::cout << "intrution detected!" << std::endl;
				wait(10, SC_SEC);
				std::cout << "10 sec passed" << std::endl;
			}
			else
			{
				std::cout << "all is well" << std::endl;
			}
		}
		else
		{
			wait();
		}
	}
}

};