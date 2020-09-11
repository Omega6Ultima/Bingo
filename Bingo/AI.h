//Dustin Gehm

#pragma once
#ifndef _AI_H
#define _AI_H

#include <vector>

#include "Exception.h"
#include "Matrix.h"
#include "RandomManager.h"
#include "Utils.h"
#include "VecN.h"

using std::vector;

typedef double(*NeuralNetActivationFunc)(double value);
typedef double(*NeuralNetActivationDerivFunc)(double value);
typedef int(*CrispFunc)(double value);

template<uint ins, uint outs>
class OldNeuralNetwork {
	class Perceptron {
	public:
		Perceptron(uint ins, uint outs)
			:weights(outs, ins, 0), error(outs, 0),
			lastInput(ins, 0), lastOutput(outs, 0) {
			for (uint c = 0; c < outs; c++) {
				for (uint d = 0; d < ins; d++) {
					weights.get(c, d) = RandomManager::randDouble(-1.0, 1.0);
				}
			}
		}

		~Perceptron() {
			//
		}

		inline uint getIns() const {
			return weights.getW();
		}

		inline uint getOuts() const {
			return weights.getH();
		}

		void adjustWeights(uint row, DynVecN<double> adjust) {
#if _DEBUG
			if (adjust.getSize() != getIns()) {
				throw Exception("Adjustment isnt the same size as the weights.");
			}
#endif
			for (uint c = 0; c < adjust.getSize(); c++) {
				weights.get(row, c) = weights.get(row, c) + adjust[c];
			}
		}

		DynVecN<double> run(DynVecN<double> inputs, bool training) {
			DynVecN<double> result(getOuts(), 0);
#if _DEBUG
			if (inputs.getSize() != getIns()) {
				throw Exception("Inputs doesnt match number of inputs for this Perceptron.");
			}
#endif
			uint inputSize = inputs.getSize();

			if (training) {
				lastInput = inputs;
			}

			DynMatrix<double> inMatrix(getIns(), 1, inputs);

			DynMatrix<double> outMatrix = weights.matrixMultiply(inMatrix);

			//for (uint c = 0; c < weights.getH(); c++){
			//	result[c] = outMatrix.get(c, 0);
			//}

			result = outMatrix.getCol(0);

			if (training) {
				lastOutput = result;
			}

			return result;
		}

	private:
		DynMatrix<double> weights;
		DynVecN<double> error;
		DynVecN<double> lastInput;
		DynVecN<double> lastOutput;

		friend class OldNeuralNetwork;
	};
public:
	OldNeuralNetwork(int numberHiddenLayers = -1) {
		for (uint c = ins; c >= outs; c--) {
			layers.push_back(Perceptron(c + 1, c - 1));
		}
	}

	~OldNeuralNetwork() {
		//TODO dynamically allocate Perceptrons??
	}

	void setActivation(NeuralNetActivationFunc func) {
		activate = func;
	}

	void setDeactivate(NeuralNetActivationDerivFunc func) {
		deactivate = func;
	}

	void setCrisper(CrispFunc func) {
		crisper = func;
	}

	inline double getLearningRate() const {
		return learningRate;
	}

	void setLearningRate(double rate) {
#if _DEBUG
		if (rate < 0 || rate >= 1.0) {
			throw Exception("Invalid learning rate, must be between 0 and 1.");
		}
#endif
		learningRate = rate;
	}

	void train(VecN<double, ins> inputs, VecN<double, outs> answers) {
		//error10 = answers - inputs;

		//layers[-1].weights = layer[-1].weights + lRate * lastIn * activate(lastOut) * error10;

		//error00 = sum(k) { layers[-1][k] * error10[k] };

		//layers[-2].weights = layers[-2].weights + lRate * activate(lastOut) * lastIn * error00;

		//error01 = sum(k) { layers[-1][k] * error10[k] };

		//layers[-2].weights = layers[-2].weights + lRate * activate(lastOut) * lastIn * error01;

			//VecN<double, outs> result = runTrain(inputs, true);
			//DynVecN<double> error(answers - result);

			//Perceptron* lastLayer = &layers[layers.size() - 1];

			//for (uint c = 0; c < lastLayer->getOuts(); c++){
			//	lastLayer->adjustWeights(c, lastLayer->lastInput[c] * learningRate * deactivate(lastLayer->lastOutput[c]) * error[c]);
			//}

			//vector<Perceptron>::iterator iter;
			//for (iter = layers.end() - 1; iter != layers.begin() - 1; iter--){
			//	iter->adjustWeights(iter->lastInput * learningRate * deactivate(iter->lastOutput) * error);
			//}

		//VecN<double, outs> result = runTrain(inputs, true);
		//DynVecN<double> error(answers - result);

		//TODO redo this
		//for (vector<Perceptron>::iterator iter = layers.begin(); iter != layers.end(); iter++){
		//	VecN<DynVecN<double>, outs> adjustment;

		//	for (uint c = 0; c < outs; c++){
		//		adjustment[c] = (*iter).lastInput * (learningRate * error[c] * activate((*iter).lastOutput[c]));
		//	}

		//	for (uint c = 0; c < outs; c++){
		//		(*iter).adjustWeights(c, adjustment[c]);
		//	}

		//	DynVecN<double> newError((*iter).getIns() - 1);

		//	for (uint c = 0; c < newError.getSize(); c++){
		//		newError[c] = error.dot((*iter).weights.getCol(c));
		//	}

		//	error = newError;
		//}
	}

	inline VecN<double, outs> run(VecN<double, ins> inputs) {
		return runTrain(inputs);
	}

private:
	VecN<double, outs> runTrain(VecN<double, ins> inputs, bool training = false) {
		VecN<double, outs> result;
		DynVecN<double> working(ins + 1);

		//do the augmented input
		for (uint c = 0; c < ins; c++) {
			working[c] = inputs[c];
		}

		working[working.getSize() - 1] = 1;

		for (auto iter = layers.begin(); iter != layers.end(); iter++) {
			DynVecN<double> temp = (*iter).run(working, training);

			working.setSize(temp.getSize() + 1);

			for (uint c = 0; c < temp.getSize(); c++) {
				working[c] = activate(temp[c]);
			}

			working[working.getSize() - 1] = 1;
		}

		if (!training) {
			for (uint c = 0; c < outs; c++) {
				result[c] = crisper(working[c]);
			}
		}
		else {
			for (uint c = 0; c < outs; c++) {
				result[c] = working[c];
			}
		}

		return result;
	}

private:
	vector<Perceptron> layers;
	NeuralNetActivationFunc activate = sigmoid;
	NeuralNetActivationDerivFunc deactivate = sigmoidDeriv;
	CrispFunc crisper = crispify;
	double learningRate = 0.1;
};

double sigmoid(double value);
double sigmoidDeriv(double value);
int crispify(double value);

class NeuralNetwork {
public:
	NeuralNetwork(DynVecN<uint> layer_sizes) {
		numLayers = layer_sizes.getSize();

		layerSizes.resize(numLayers + 1);

		layerSizes[0] = numLayers + 1;

		for (uint c = 1; c < numLayers + 1; c++) {
			layerSizes[c] = layer_sizes[c - 1] + 1;
		}

		layerSizes[numLayers] -= 1;

		//layerBiases.resize(numLayers - 1);
		layerWeights.resize(numLayers);
		layerLastInputs.resize(numLayers);
		layerLastOutputs.resize(numLayers);

		for (uint c = 0; c < numLayers - 1; c++) {
			//layerBiases[c] = DynMatrix<double>(1, layerSizes[c] - 1);

			//for (uint d = 0; d < layerBiases[c].getH(); d++) {
			//	layerBiases[c].get(0, d) = RandomManager::randDouble(-1, 1);
			//}

			layerWeights[c] = DynMatrix<double>(layerSizes[c + 1] - 1, layerSizes[c]);

			for (uint d = 0; d < layerWeights[c].getH(); d++) {
				for (uint e = 0; e < layerWeights[c].getW(); e++) {
					layerWeights[c].get(d, e) = RandomManager::randDouble(-1, 1);
				}
			}
		}

		layerWeights[numLayers - 1] = DynMatrix<double>(layerSizes[numLayers], layerSizes[numLayers - 1]);

		for (uint d = 0; d < layerWeights[numLayers - 1].getH(); d++) {
			for (uint e = 0; e < layerWeights[numLayers - 1].getW(); e++) {
				layerWeights[numLayers - 1].get(d, e) = RandomManager::randDouble(-1, 1);
			}
		}
	}

	void train(DynVecN<uint>& inputs, DynVecN<uint>& expectedOutput) {
		DynVecN<double> output = runInput(inputs, true);

		DynVecN<double> error = static_cast<DynVecN<double>>(expectedOutput) - output;

		for (int c = numLayers - 1; c >= 0; c--) {
			DynVecN<double> rowAdj(1);

			for (uint d = 0; d < error.getSize(); d++) {
				rowAdj = layerLastInputs[c].getRow(d) * (learningRate * error[d] * sigmoidDeriv(layerLastOutputs[c].get(d, 0)));

				for (uint e = 0; e < rowAdj.getSize(); e++) {
					layerWeights[c].get(e, d) += rowAdj[e];
				}
			}

			DynVecN<double> newErr(layerSizes[c] - 1);

			for (uint d = 0; d < layerSizes[c] - 1; d++) {
				newErr[d] = error.dot(layerWeights[c].getCol(d));
			}

			error = newErr;
		}
	}

	DynVecN<uint> run(DynVecN<uint>& inputs) {
		return static_cast<DynVecN<uint>>(runInput(inputs, false));
	}

private:
	DynVecN<double> runInput(DynVecN<uint>& inputs, bool training) {
		//copy inputs into matrix with augmented input
		DynMatrix<double> inputMatrix(inputs.getSize() + 1, 1, 1.0);

		for (uint c = 0; c < inputs.getSize(); c++) {
			inputMatrix.get(c, 0) = inputs[c];
		}

		//run values through all the hidden layers
		for (uint c = 0; c < numLayers - 1; c++) {
			if (training) {
				layerLastInputs[c] = inputMatrix;
			}

			//run the values through this layer of 'perceptrons'
			//inputMatrix = inputMatrix.matrixMultiply(layerWeights[c]);
			inputMatrix = layerWeights[c].matrixMultiply(inputMatrix);
			//inputMatrix += layerBiases[c];

			if (training) {
				layerLastOutputs[c] = inputMatrix;
			}

			//copy values into new matrix with augmented input
			DynMatrix<double> tempMat(inputMatrix.getH() + 1, 1, 1.0);

			for (uint d = 0; d < inputMatrix.getH(); d++) {
				tempMat.get(d, 0) = inputMatrix.get(d, 0);
			}

			inputMatrix = tempMat;
		}

		//run values through final output layer
		if (training) {
			layerLastInputs[numLayers - 1] = inputMatrix;
		}

		//inputMatrix = inputMatrix.matrixMultiply(layerWeights[numLayers - 1]);
		inputMatrix = layerWeights[numLayers - 1].matrixMultiply(inputMatrix);
		//inputMatrix += layerBiases[numLayers - 1];

		if (training) {
			layerLastOutputs[numLayers - 1] = inputMatrix;
		}

		//copy final values and potentially crisp them
		DynVecN<double> output(inputMatrix.getH());

		if (training) {
			for (uint c = 0; c < output.getSize(); c++) {
				output[c] = inputMatrix.get(c, 0);
			}
		}
		else {
			for (uint c = 0; c < output.getSize(); c++) {
				output[c] = crispify(inputMatrix.get(c, 0));
			}
		}

		return output;
	}

private:
	double learningRate = 0.1;
	uint numLayers;
	vector<uint> layerSizes;
	//vector<DynMatrix<double>> layerBiases;
	vector<DynMatrix<double>> layerWeights;
	vector<DynMatrix<double>> layerLastInputs;
	vector<DynMatrix<double>> layerLastOutputs;
};

#endif