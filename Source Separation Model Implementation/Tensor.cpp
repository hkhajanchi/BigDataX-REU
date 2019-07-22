#include "Tensor.h"
#include "assert.h"
#include "math.h"

using namespace std; 

/* ---- Constructor Methods ----- */

//Implement a parameterized constructor 
//Creates a tensor object with randomized "float" data
Tensor::Tensor(int rows, int cols)
{
	this->rows = rows; 
	this->cols = cols; 

	this->tensor = vector<vector<float>>(this->rows, vector<float>(this->cols));

	vector<vector<float>>::iterator row_ptr;
	vector<float>::iterator col_ptr; 

	//Populate tensor object with quasi-random floats between 0-10
	for (row_ptr = tensor.begin(); row_ptr != tensor.end(); row_ptr++)
	{
		for (col_ptr = row_ptr->begin(); col_ptr != row_ptr->end(); col_ptr++)
		{
			*col_ptr = randNum();
		}
	}

}

//Constructor for a column vector ONLY
Tensor::Tensor(int rows, int cols, COL_VECTOR input_data)
{

	this->rows = rows;
	this->cols = cols;

	this->tensor = vector<vector<float>>(rows, vector<float>(cols));

	assert(rows == 1); 

	tensor[0] = input_data; 

}

//Constructor method that initializes the Tensor to zero, takes an additional charArg that is somewhat useless for now
Tensor::Tensor(int rows, int cols, char charArg)
{
	this->rows = rows; 
	this->cols = cols; 

	this->tensor = vector<vector<float>>(rows, vector<float>(cols));

	vector <vector <float>>::iterator row_ptr; 
	vector<float>::iterator col_ptr; 

	//Initializes all elements in the Tensor to be zero (used primiarily for Bias initialization)
	for (row_ptr = tensor.begin(); row_ptr != tensor.end(); row_ptr++)
	{
		for (col_ptr = row_ptr->begin(); col_ptr != row_ptr->end(); col_ptr++)
		{
			*col_ptr = 0; 
		}
	}
}

/* ----- Helper Methods ------------ */

//Generates a random float in between 0-10
float Tensor::randNum()
{
		float r = (float)rand() / (float)RAND_MAX;
		return MIN + r * (MAX - MIN);
}

//Row Accessor Method
int Tensor::getRows() const 
{
	return this->rows; 
}

//Column Accessor Method
int Tensor::getCols() const 
{
	return this->cols; 
}

//Private sigmoid helper function, uses a 4-term Taylor Series Approximation
// Standard sigmoid function implementation, not at all HW friendly but eventually this will be replaced with 
// an optimized polynomial approximation

float Tensor::_sigmoid(float x)
{
	return static_cast<float> (1 / (1 + exp(-x)));
}

//Private Tanh() helper function, uses a 3-term Taylor Series Approximation
// Added a rough Tanh() approximation with some domain bounds, might lose accuracy 
// Need to find a better, more-optimized approximation
float Tensor::_tanh(float x)
{
	if (x > 3)
	{
		return 1;
	}
	else if (x < -3)
	{
		return -1;
	}
	else
	{
		double ans1 = (exp(x) - exp(-x));
		double ans2 = (exp(x) + exp(-x));
		return static_cast<float> (ans1 / ans2);
	}

}


Tensor Tensor::_broadcast(Tensor x, int dimExpand)
{
	auto result = Tensor(dimExpand, x.getCols());

	//To Do: Implement broadcasting functionality

	return result; 

}

/* ------ Core Class Functions ----- */

//Naive Matrix Multiplication implementation
Tensor Tensor::matMul(Tensor const &mat_b)
{
	//Need to make sure that the matrix multiplication can work in the first place
	assert(this->cols == mat_b.getRows());

	//Define output Tensor
	auto result = Tensor(this->getRows(), mat_b.getCols());

	for (int i = 0; i < this->getRows(); i++)
	{
		for (int j = 0; j < mat_b.getCols(); j++)
		{
			result.tensor[i][j] = 0; 

			for (int k = 0; k < this->getCols(); k++)
			{
				result.tensor[i][j] += this->tensor[i][k] * mat_b.tensor[k][j];
			}
		}
	}

	return result; 

}

//Naive Hadamard Product implementation
Tensor Tensor::hadamard(Tensor const& mat_b)
{
	//Hadamard Product requires the input matricies to be the same dimensions

	if (getRows() != mat_b.getRows() || getCols() != mat_b.getCols())
	{		
		throw 20;
	}

	auto result = Tensor(getRows(), getCols());

	for (int i = 0; i < getRows(); i++)
	{
		for (int j = 0; j < getCols(); j++)
		{
			result.tensor[i][j] = tensor[i][j] * mat_b.tensor[i][j];
		}
	}

	return result; 
}

//Tensor addition method
Tensor Tensor::add(Tensor const& mat_b)
{

	//Checks if the matrix is not the same dimensions
	if (getRows() != mat_b.getRows() || getCols() != mat_b.getCols())
	{

		throw 20; 
	};

	auto result = Tensor(getRows(), getCols());

	for (int i = 0; i < getRows(); i++)
	{
		for (int j = 0; j < getCols(); j++)
		{
			result.tensor[i][j] = tensor[i][j] + mat_b.tensor[i][j];
		}
	}

	return result;
}

//Element wise NN modulators
void Tensor::sigmoid()
{

	for (int i = 0; i < getRows(); i++)
	{
		for (int j = 0; j < getCols(); j++)
		{
			tensor[i][j] = _sigmoid(tensor[i][j]);
		}
	}

}

void Tensor::tanh()
{

	for (int i = 0; i < getRows(); i++)
	{
		for (int j = 0; j < getCols(); j++)
		{
			tensor[i][j] = _tanh(tensor[i][j]);
		}
	}
}

