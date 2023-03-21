//Author Cem Bakisoglu 6888909, Kristiyana Tsenova
#include "neuron.h"

float neuron::transfer_function(std::vector<float> weights, std::vector<float> inputs)
{
	float sum = 0.0;
	int length = inputs.size();
	for (int i = 0; i < length; i++) {
		sum += (weights[i] * inputs[i]);
	}
	return sum;
}


float neuron::leaky_relu(float value)
{
	if (value > 0) {
		return value;
	}
	else {
		return 0.01 * value;
	}
}

float neuron::leaky_relu_derivate(float value)
{
	if (value < 0) {
		return 0.01;
	}
	else {
		return 1;
	}
}

std::vector<float> neuron::softmax(std::vector<float> layer)
{
	std::vector<float> temp, output;
	for (int i = 0; i < layer.size(); i++) {
		temp.push_back(exp(layer[i]));
	}

	float sum = std::accumulate(temp.begin(), temp.end(), 0);

	for (int i = 0; i < layer.size(); i++) {
		output.push_back(exp(layer[i]) / sum);
	}
	return output;
}

std::vector<float> neuron::softmax_derivate(std::vector<float> layer)
{
	std::vector<float> temp, output;
	temp = softmax(layer);
	for (int i = 0; i < layer.size(); i++) {
		output.push_back(temp[i] * (1 - temp[i]));
	}
	return output;
}

float neuron::init_weight(std::vector<float> layer)
{
	float random_double = (float)rand() / RAND_MAX;
	float result = random_double * sqrt((2.0/(float)layer.size()));
	return result;
}

std::vector<std::vector<float>> neuron::connectLayers(std::vector<float> layer_one, std::vector<float> layer_two)
{
	std::vector<std::vector<float>> matrix;

	for (int i = 0; i < layer_two.size(); i++) {
		matrix.push_back(std::vector<float>());

		for (int j = 0; j < layer_one.size(); j++) {
			matrix[i].push_back(init_weight(layer_one));
		}
	}

	return matrix;
}

std::vector<float> neuron::calc_gradient(std::vector<float> output, std::vector<float> target_value, std::vector<std::vector<float>> weights, std::vector<float> hidden_layer, std::string neuron)
{
	std::vector<float> output_gradient;
	for (int i = 0; i < output.size(); i++)
	{
		output_gradient.push_back((target_value[i] - leaky_relu(output[i])) * leaky_relu_derivate(output[i]));
	}
	if (neuron == "output")
	{
		return output_gradient;
	}

	else
	{
		std::vector<float> hiddenlayer_gradient;
		for (int i = 0; i < output.size(); i++)
		{
			hiddenlayer_gradient.push_back((output_gradient[i] * weights[i][0]) * leaky_relu_derivate(hidden_layer[i]));

		}
		return hiddenlayer_gradient;
	}
}

float neuron::get_weight(int index, std::vector<float> weights) const
{
	return weights[index];
}

float neuron::set_weight(int index, std::vector<float> weights, float set)
{
	return weights[index] = set;
}