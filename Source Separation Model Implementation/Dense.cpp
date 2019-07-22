#include "Dense.h"
#include "assert.h"

using namespace std; 

//Implementation of default constructor, instantiates two Tensor objects of shape [input size, output size] (bias is assumed to be automatically broadcasted, Tensor broadcasting will be implemented at some point)
Dense::Dense(int input_size, int output_size, int sequence_length)
	:
	fc_Weights(Tensor(input_size, output_size)),
	fc_bias(Tensor(sequence_length, output_size))
{
	this->input_size = input_size; 
	this->output_size = output_size; 
}

//Standard Dense layer implementation, nothing fancy
Tensor Dense::forward(Tensor x_t)
{

	assert(x_t.getCols() == input_size);

	auto result = Tensor(input_size, output_size);

	result = (x_t * fc_Weights) + fc_bias;

	return result; 

}