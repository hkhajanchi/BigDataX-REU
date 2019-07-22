#include "LSTM.h"
#include "Tensor.h"
#include "assert.h"

using namespace std; 


LSTM::LSTM(int input_size, int hidden_size, int sequence_size)
	:

	W_ii(Tensor(input_size, hidden_size)),
	W_hi(Tensor(hidden_size, hidden_size)),
	b_i(Tensor(1, hidden_size, 'z')),

	W_if(Tensor(input_size, hidden_size)),
	W_hf(Tensor(hidden_size, hidden_size)),
	b_f(Tensor(1, hidden_size, 'z')),

	W_ig(Tensor(input_size, hidden_size)),
	W_hg(Tensor(hidden_size, hidden_size)),
	b_g(Tensor(1, hidden_size, 'z')),

	W_io(Tensor(input_size, hidden_size)),
	W_ho(Tensor(hidden_size, hidden_size)),
	b_o(Tensor(1, hidden_size, 'z')),

	h_t(Tensor(1, hidden_size)),
	c_t(Tensor(1, hidden_size))

{
	this->input_size = input_size; 
	this->hidden_size = hidden_size; 
	this->sequence_size = sequence_size; 
}

Tensor LSTM::forward(Tensor x_t)
{
	assert(x_t.getRows() == sequence_size);


	auto curr_xt = Tensor(1, input_size);

	auto output_Tensor = Tensor(sequence_size, hidden_size);

	for (int i = 0; i < sequence_size; i++)
	{
		
		//Update to use at current timestep
		curr_xt.tensor[0] = x_t.tensor[i];
		
		Tensor i_t = (curr_xt * W_ii + h_t * W_hi) + b_i;
		i_t.sigmoid(); 

		Tensor f_t = (curr_xt * W_if + h_t * W_hf) + b_f;
		f_t.sigmoid();

		Tensor g_t = (curr_xt * W_ig + h_t * W_hg) + b_g;
		g_t.tanh();

		Tensor o_t = (curr_xt * W_io + h_t * W_ho) + b_o;
		o_t.sigmoid();

		//Update Cell State
		c_t = c_t.hadamard(f_t) + i_t.hadamard(g_t);

		c_t.tanh();

		h_t = o_t.hadamard(c_t);

		output_Tensor.tensor[i] = h_t.tensor[0]; 


		
	}

	return output_Tensor; 
}