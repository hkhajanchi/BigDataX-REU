#pragma once
#include "Tensor.h"
class Dense
{


private: 

	int input_size; 
	int output_size; 
	

	Tensor fc_Weights; 
	Tensor fc_bias; 


public: 

	//Default Constructor
	Dense(int input_size, int output_size, int sequence_length);

	Tensor forward(Tensor x_t);
};

