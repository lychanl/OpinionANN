#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>
#include "../include/MiddleLayer.h"

MiddleLayer::MiddleLayer(int neurons, int previousLayerNeurons) : NeuronLayer(neurons) {
    this->connections = new Eigen::MatrixXf(neurons, previousLayerNeurons);
    this->bias = new Eigen::MatrixXf(neurons, 1);
    this->connections->setOnes();
	this->output = new Eigen::MatrixXf(neurons, 1);
}

MiddleLayer::MiddleLayer(const MiddleLayer &middleLayer) : NeuronLayer(middleLayer.getNeuronNumber()) {
    this->connections = new Eigen::MatrixXf(*(middleLayer.connections));
    this->bias = new Eigen::MatrixXf(*(middleLayer.bias));
    this->output = new Eigen::MatrixXf(*(middleLayer.output));
}

void MiddleLayer::computeOutput(Eigen::MatrixXf* previousOutput) {
	if (output != nullptr) delete output;
	if (weitghtedInput != nullptr) delete weitghtedInput;
    //multiply output of previous neuron layer by connections' weights
    Eigen::Product<Eigen::MatrixXf, Eigen::MatrixXf> product =  (*connections) * (*previousOutput);

	weitghtedInput = new Eigen::MatrixXf(product);
	output = new Eigen::MatrixXf(product);

    //update output by summing up the multiplication results and biases
    // and calculating value of activation function (arctan)
    for (int i = 0; i < output->cols(); i++){
        for (int j = 0; j < output->rows(); j++){
            (*output)(j, i) = activationFunction((*output)(j, i) + (*bias)(j, i));
        }
    }
}

float MiddleLayer::activationFunction(float x) {
    return atan(x)/M_PI_2;
}

Eigen::MatrixXf* MiddleLayer::getOutput() {
	return output;
}

Eigen::MatrixXf* MiddleLayer::getWeightedInput() {
	return weitghtedInput;
}

void MiddleLayer::initRandomConnections() {
    connections->setRandom();
    bias->setRandom();
}

Eigen::MatrixXf* MiddleLayer::getWeights()
{
	return connections;
}

MiddleLayer::~MiddleLayer() {
    delete connections;
	delete weitghtedInput;
	delete bias;
}


void MiddleLayer::adjustConnections(Eigen::MatrixXf* diff)
{
	*this->connections += *diff;
}

void MiddleLayer::adjustBiases(Eigen::MatrixXf* diff)
{
	*this->bias += *diff;
}

void MiddleLayer::initKnownConnections(const std::pair<Eigen::MatrixXf, Eigen::MatrixXf>& connections)
{
	*this->connections = connections.first;
	*this->bias = connections.second;
}

std::pair<Eigen::MatrixXf, Eigen::MatrixXf> MiddleLayer::getKnownConnections()
{
	return{ *this->connections, *this->bias };
}