//Author Cem Bakisoglu 6888909, Kristiyana Tsenova
#pragma once
#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <ctime>
#include <math.h>

class neuron {
	std::vector<float> weights;
	std::vector<float> inputs;
public:

	float transfer_function(std::vector<float> weights, std::vector<float> inputs);
		/**/

	float leaky_relu(float value);
		/*
		Leaky Relu activation function
		returns given value if value > 0
		returns 0.01 * value else
		*/

	float leaky_relu_derivate(float value);
		/*
		*/

	std::vector<float> softmax(std::vector<float> layer);
		/*softmax activation function
		*/

	std::vector<float> softmax_derivate(std::vector<float> layer);
		/*derivate of softmax activation function
		*/

	float init_weight(std::vector<float> layer);
		/*returns a random double for weights
		*/

	std::vector<std::vector<float>> connectLayers(std::vector<float> layer_one, std::vector<float> layer_two);
		/*creates a matrix with weigths between layer i and layer i + 1
		*/

	std::vector<float> calc_gradient(std::vector<float> output, std::vector<float> target_value, std::vector<std::vector<float>> matrix, std::vector<float> hidden_layer, std::string neuron);
		/*
		*/

	float get_weight(int index, std::vector<float> weights) const;
		/*returns the weight of the given index
		*/

	float set_weight(int index, std::vector<float> weights, float set);
		/*sets given weight to given index
		*/


private:

protected:

};