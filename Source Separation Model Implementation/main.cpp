#include <cstdio>
#include "Tensor.h"
#include <math.h>
#include "LSTM.h"
#include "Dense.h"
#include "assert.h"
#include <chrono>

using namespace std; 

/* Scratch file for testing class functions and random C++ constructs */

	int main()
	{
		try {


			Tensor test(678, 300);
	

			
			std::vector<std::chrono::duration<double>> run_times(10);

			for (int i = 100; i < 1100; i = i+100)
			{
				auto lstm_layer_1 = LSTM(129, 300, i);
				auto lstm_layer_2 = LSTM(300, 300, i);
				auto lstm_layer_3 = LSTM(300, 300, i);
				auto lstm_layer_4 = LSTM(300, 300, i);
				auto dense_layer = Dense(300, 2580, i);

				auto input = Tensor(i, 129);

				auto start = std::chrono::high_resolution_clock::now(); 

				Tensor out_1 = lstm_layer_1.forward(input);
				Tensor out_2 = lstm_layer_1.forward(out_1);
				Tensor out_3 = lstm_layer_1.forward(out_2);
				Tensor out_4 = lstm_layer_1.forward(out_3);

				Tensor model_out = dense_layer.forward(out_4);

				auto finish = std::chrono::high_resolution_clock::now();

				assert(model_out.getRows() == i && model_out.getCols() == 2580);
				std::chrono::duration<double> time = finish - start; 

				cout << "Current Sequence Length: " << i << endl; 
				cout << "Runtime latency: " << time.count() << endl; 



				run_times.push_back(time);


				assert(model_out.getRows() == i && model_out.getCols() == 2580);

			}
			
			printf("done");
		}

		catch (int e)
		{
			cout << "Matrix Dimensions do not match. Unable to compute product." << endl;
		}
	}

