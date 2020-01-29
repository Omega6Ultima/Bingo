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
class OldNeuralNetwork{
	class Perceptron{
	public:
		Perceptron(uint ins, uint outs)
			:weights(outs, ins, 0), error(outs, 0),
			lastInput(ins, 0), lastOutput(outs, 0){
			for (uint c = 0; c < outs; c++){
				for (uint d = 0; d < ins; d++){
					weights.get(c, d) = RandomManager::randDouble(-1.0, 1.0);
				}
			}
		}

		~Perceptron(){
			//
		}

		inline uint getIns() const{
			return weights.getW();
		}

		inline uint getOuts() const{
			return weights.getH();
		}

		void adjustWeights(uint row, DynVecN<double> adjust){
#if _DEBUG
			if (adjust.getSize() != getIns()){
				throw Exception("Adjustment isnt the same size as the weights.");
			}
#endif
			for (uint c = 0; c < adjust.getSize(); c++){
				weights.get(row, c) = weights.get(row, c) + adjust[c];
			}
		}

		DynVecN<double> run(DynVecN<double> inputs, bool training){
			DynVecN<double> result(getOuts(), 0);
#if _DEBUG
			if (inputs.getSize() != getIns()){
				throw Exception("Inputs doesnt match number of inputs for this Perceptron.");
			}
#endif
			uint inputSize = inputs.getSize();

			if (training){
				lastInput = inputs;
			}

			DynMatrix<double> inMatrix(getIns(), 1, inputs);

			DynMatrix<double> outMatrix = weights.matrixMultiply(inMatrix);

			//for (uint c = 0; c < weights.getH(); c++){
			//	result[c] = outMatrix.get(c, 0);
			//}

			result = outMatrix.getCol(0);

			if (training){
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
	OldNeuralNetwork(int numberHiddenLayers = -1){
		for (uint c = ins; c >= outs; c--){
			layers.push_back(Perceptron(c + 1, c - 1));
		}
	}

	~OldNeuralNetwork(){
		//TODO dynamically allocate Perceptrons??
	}

	void setActivation(NeuralNetActivationFunc func){
		activate = func;
	}

	void setDeactivate(NeuralNetActivationDerivFunc func){
		deactivate = func;
	}

	void setCrisper(CrispFunc func){
		crisper = func;
	}

	inline double getLearningRate() const{
		return learningRate;
	}

	void setLearningRate(double rate){
#if _DEBUG
		if (rate < 0 || rate >= 1.0){
			throw Exception("Invalid learning rate, must be between 0 and 1.");
		}
#endif
		learningRate = rate;
	}

	void train(VecN<double, ins> inputs, VecN<double, outs> answers){
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

	inline VecN<double, outs> run(VecN<double, ins> inputs){
		return runTrain(inputs);
	}

private:
	VecN<double, outs> runTrain(VecN<double, ins> inputs, bool training = false){
		VecN<double, outs> result;
		DynVecN<double> working(ins + 1);

		//do the augmented input
		for (uint c = 0; c < ins; c++){
			working[c] = inputs[c];
		}

		working[working.getSize() - 1] = 1;

		for (auto iter = layers.begin(); iter != layers.end(); iter++){
			DynVecN<double> temp = (*iter).run(working, training);

			working.setSize(temp.getSize() + 1);

			for (uint c = 0; c < temp.getSize(); c++){
				working[c] = activate(temp[c]);
			}

			working[working.getSize() - 1] = 1;
		}

		if (!training){
			for (uint c = 0; c < outs; c++){
				result[c] = crisper(working[c]);
			}
		}
		else{
			for (uint c = 0; c < outs; c++){
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

class NeuralNetwork{
public:
	NeuralNetwork(DynVecN<uint> layerSizes){
		numLayers = layerSizes.getSize();

		sizes.resize(numLayers);

		for (uint c = 0; c < numLayers; c++){
			sizes[c] = layerSizes[c];
		}

		biases.resize(numLayers - 1);
		weights.resize(numLayers - 1);

		for (uint c = 0; c < numLayers - 1; c++){
			biases[c] = DynMatrix<double>(sizes[c], 1);

			for (uint d = 0; d < biases[c].getH(); d++){
				biases[c].get(d, 0) = RandomManager::randDouble(-1, 1);
			}

			weights[c] = DynMatrix<double>(sizes[c], sizes[c + 1]);

			for (uint d = 0; d < weights[c].getH(); d++){
				for (uint e = 0; e < weights[c].getW(); e++){
					weights[c].get(d, e) = RandomManager::randDouble(-1, 1);
				}
			}
		}
	}

private:
	uint numLayers;
	vector<uint> sizes;
	vector<DynMatrix<double>> biases;
	vector<DynMatrix<double>> weights;
};

double sigmoid(double value);
double sigmoidDeriv(double value);
int crispify(double value);

#endif