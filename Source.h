#define HelloExample
#include <systemc.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace sc_core;

SC_MODULE(m1) {
	sc_out<int> sensor_out;
	SC_CTOR(m1) {
		SC_METHOD(sensor)
	}

	void sensor()
	{
		int val;
		srand(sc_simulation_time());
		val = rand() % 2;
		sensor_out.write(val);
		next_trigger(50, SC_MS);
	}

};

SC_MODULE(m3) {
	sc_out<int> camera_out;
	SC_CTOR(m3) {
		SC_METHOD(camera)
	}

	void camera()
	{
		srand(sc_simulation_time());
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
		next_trigger(100, SC_MS);

	}

};

SC_MODULE(m2) {
	sc_in<int> port1, port2, port3;
	sc_clock clk;
	SC_CTOR(m2): clk("clk",10,SC_MS) {
		SC_CTHREAD(dataanalysis ,clk);
		sensitive << port1 << port2 << port3;
	}

	bool valid()
	{
		int val;
		srand(time(NULL)+sc_simulation_time());
		val = rand() % 10;
		return val < 6;
	}

	void dataanalysis()
	{
		double t0 = 0;
		int sen1, sen2, cam1;
		int itr = 0, pin_fail = 0;
		while (true)
		{
			if (valid())
			{
				pin_fail = 0;
				//read values from sensors and cameras
				if (itr % 2 == 0)
				{
					wait(20, SC_MS);
					cam1 = port3.read();
					sen1 = port1.read();
					sen2 = port2.read();
					wait(30, SC_MS);
				}
				else
				{
					wait(10, SC_MS);
					sen1 = port1.read();
					sen2 = port2.read();
					wait(40, SC_MS);
				}
				itr++;

				std::cout << cam1 << std::endl;
				std::cout << cam1 * (sen1 + sen2) << std::endl;

				//Check for intrution
				int i_m = cam1 * (sen1 + sen2);
				if (i_m > 1)
				{
					t0 = sc_time_stamp().to_seconds();
					std::cout << "intrution detected!" << std::endl;
					int random;
					bool vlid = false;
					
					for (int i = 0; i < 3; i++)
					{
						srand(time(NULL) + sc_simulation_time());
						random = rand() % 5;
						wait(random, SC_SEC);
						if (valid())
						{
							vlid = true;
							std::cout << "Correct pin entered. Welcome home" << std::endl;
							i = 3;
						}
					}
					if (vlid!=true || sc_time_stamp().to_seconds() - t0 >= 10)
					{
						wait(10, SC_SEC);
						std::cout << "10 secounds has passed with no correct pin entered" << std::endl;
					}
				}
				else
				{
					std::cout << "all is well" << std::endl;
				}
			}
			else
			{
				if (pin_fail == 0)
				{
					t0 = sc_time_stamp().to_seconds();
				}
				pin_fail++;
				std::cout << "Wrong pin code. You have " << 3 - pin_fail << " attempts before the alarm sounds" << std::endl;
				srand(time(NULL) + sc_simulation_time());
				int random = rand() % 5;
				wait(random, SC_SEC);
			}
			std::cout << sc_time_stamp().to_seconds() - t0 << "S" << std::endl;
			if (pin_fail > 2 || sc_time_stamp().to_seconds() - t0 >= 10)
			{
				pin_fail = 0;
				std::cout << "intrution detected Alarm is sounding!" << std::endl;
				wait(10, SC_SEC);
				std::cout << "10 sec passed" << std::endl;
			}
			wait();
		}
	}



};