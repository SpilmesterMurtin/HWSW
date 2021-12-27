#define HelloExample
#include <systemc.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace sc_core;

SC_MODULE(m1) {
	sc_out<int> sensor_out;
	SC_CTOR(m1) {
		//empty constructor
	}

	void sensor()
	{
		int val;
		srand(time(NULL));
		val = rand() % 2;
		sensor_out.write(val);
	}

};

SC_MODULE(m3) {
	sc_out<int> camera_out;
	SC_CTOR(m3) {
		//empty constructor
	}

	void camera()
	{
		srand(time(NULL));
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
		for (int i = 0; i < 16; i++)
		{
			sum += val[i];
		}
		//write sum to port
		camera_out.write(sum);
	}

};

SC_MODULE(m2) {
	sc_in<int> port1, port2, port3;
	SC_CTOR(m2) {
		//empty constructor
	}
bool pin()
	{
		int pin;
		std::cin >> pin;
		if (pin == 1234)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
void dataanalysis()
	{
	int sen1, sen2, cam1;
	int itr = 0, pin_fail = 0;
		if (pin())
		{
			//read values from sensors and cameras
			if (itr % 2 == 0)
			{
				//wait(20, SC_MS);
				cam1 = port3.read();
				sen1 = port1.read();
				sen2 = port2.read();
				//wait(30, SC_MS);
			}
			else
			{
				//wait(10, SC_MS);
				sen1 = port1.read();
				sen2 = port2.read();
				//wait(40, SC_MS);
			}
			itr++;

			std::cout << cam1 << std::endl;
			std::cout << cam1 * (sen1 + sen2) << std::endl;

			//Check for intrution
			int i_m = cam1 * (sen1 + sen2);
			if (i_m > 1)
			{
				std::cout << "intrution detected!" << std::endl;
				//wait(10, SC_SEC);
				std::cout << "10 sec passed" << std::endl;
			}
			else
			{
				std::cout << "all is well" << std::endl;
			}
		}
		else
		{
			pin_fail++;
		}
	}
};