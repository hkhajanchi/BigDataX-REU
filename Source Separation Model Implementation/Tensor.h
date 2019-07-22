#pragma once

#include <iostream>
#include <vector>
using namespace std; 

#define MIN 0 
#define MAX 0.5

#define COL_VECTOR std::vector<float> 

//Todo: 
/*
 - Implement full matrix multiplication operation - done
 - Implement Hadamard product multiplication - done
 - Implement matrix addition - done
 - Implement Tanh() and Softmax() mapping functions - done
 - Implement zero initialization as part of constructor - done 
 - Add 4th term to Tanh() approximation

*/

class Tensor
{

protected:
	vector<vector<float>> tensor;

	friend class LSTM; 

private:
  
	int rows;
	int cols;

	float randNum(); 

	//Taylor Series Approximation functions for Neural Network modulators
	float _sigmoid(float x);
	float _tanh(float x);

	Tensor _broadcast(Tensor x,int dimExpand); //needs to be fully implemented 

public:


	//Parameterized Constructor with random initialization data
	Tensor(int rows, int cols);

	//Regular constructor with zero initialization
	Tensor(int rows, int cols, char initArg);

	Tensor(int rows, int cols, COL_VECTOR); //FixMe: Delete this constructor, not needed


	//Accessor methods
	int getRows() const ; 
	int getCols() const ; 

	//General matrix multiplication function
	Tensor matMul(Tensor const &mat_b); 

	//Override native multiplication operator to make matmul nice and easy
	inline Tensor operator * (Tensor const& b)
	{
		return matMul(b);
	}

	//General hadamard product function
	Tensor hadamard(Tensor const& mat_b);

	//Tensor addition
	Tensor add(Tensor const &mat_b);

	inline Tensor operator + (Tensor const& b)
	{
		return add(b);
	}

	//Element wise sigmoid/tanh operators
	void sigmoid(); 

	void tanh(); 

};










