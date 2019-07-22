#pragma once
#include "Tensor.h"

/*
LSTM Algorithm 

To Do 
- Map all vector sizes in forward pass, need to make sure C++ implementation can handle the sizes needed in the array - done
- Implement LSTM looping algorithm
- Test forward pass for shape consistency

*/
class LSTM
{

private:

	int input_size; 
	int sequence_size; 
	int hidden_size; 


	/* Private Tensor objects for Weight/Bias data */

	//Weights and Biases for Input Gate
	Tensor W_ii; 
	Tensor W_hi; 
	Tensor b_i; 

	//Weights and biases for Forget Gate
	Tensor  W_if; 
	Tensor  W_hf; 
	Tensor  b_f; 

	//Weights and biases for Cell Gate
	Tensor W_ig; 
	Tensor W_hg; 
	Tensor b_g;

	//Weights and biases for Output Gate
	Tensor W_io; 
	Tensor W_ho; 
	Tensor b_o; 

public: 

	Tensor h_t; //hidden state
	Tensor c_t; //cell state


	//Default Constructor
	LSTM(int input_size, int hidden_size, int sequence_size); 

	Tensor forward(Tensor x_t);


};

