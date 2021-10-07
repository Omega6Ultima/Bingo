//Dustin Gehm

#include "AI.h"

#include <math.h>

#include "FileManager.h"
#include "NBT.h"
#include "Utils.h"

using std::make_pair;
using std::reverse;
using std::pow;
using std::to_string;

using Bingo::AI::GeneticAlgorithm;
using Bingo::AI::NeuralNetwork;
using Bingo::AI::A_StarNode;
using Bingo::AI::InternalUse::OpenType;
using Bingo::AI::InternalUse::SearchNode;
using Bingo::AI::A_StarSolver;
using Bingo::Math::DynVecN;
using Bingo::Utils::Warn;

double Bingo::AI::sigmoid(double value) {
	const static int k = 2;

	return 1.0 / (1.0 + pow(EULER, (-k * value)));
}

double Bingo::AI::sigmoidDeriv(double value) {
	double result = sigmoid(value);

	return result * (1 - result);
}

int Bingo::AI::crispify(double value) {
	//return lround(value);
	if (value > 0.5) {
		return 1;
	}
	else {
		return 0;
	}
}

NeuralNetwork::NeuralNetwork(uint numInputs, uint numOutputs)
	: numLayers(numInputs - numOutputs + 2) {
	layerSizes.resize(numLayers + 1);

	layerSizes[0] = numInputs + 1;

	for (uint c = 1; c < numLayers + 1; c++) {
		layerSizes[c] = numInputs - (c - 1) + 1;
	}

	layerWeights.resize(numLayers);
	layerLastInputs.resize(numLayers);
	layerLastOutputs.resize(numLayers);

	for (uint c = 0; c < numLayers - 1; c++) {
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

DynVecN<uint> NeuralNetwork::train(DynVecN<uint>& inputs, DynVecN<uint>& expectedOutput) {
	DynVecN<double> output = runInput(inputs, true);

	DynVecN<double> error = static_cast<DynVecN<double>>(expectedOutput) - output;

	for (int c = numLayers - 1; c >= 0; c--) {
		DynVecN<double> rowAdj(1);

		for (uint d = 0; d < error.getSize(); d++) {
			rowAdj = layerLastInputs[c].getRow(d) * (learningRate * error[d] * activationDerivFunc(layerLastOutputs[c].get(d, 0)));

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

	for (uint c = 0; c < output.getSize(); c++) {
		output[c] = crispFunc(output[c]);
	}

	return output;
}

DynVecN<uint> NeuralNetwork::run(DynVecN<uint>& inputs) {
	return static_cast<DynVecN<uint>>(runInput(inputs, false));
}

void NeuralNetwork::setLearningRate(double newRate) {
	if (newRate < 0.0) {
		newRate = 0.0;
	}
	else if (newRate > 1.0) {
		newRate = 1.0;
	}

	learningRate = newRate;
}

void NeuralNetwork::setActivationFunction(NeuralNetActivationFunc func) {
	activationFunc = func;
}

void NeuralNetwork::setActivationDerivFunction(NeuralNetActivationDerivFunc func) {
	activationDerivFunc = func;
}

void NeuralNetwork::setCrispFunction(NeuralNetCrispFunc func) {
	crispFunc = func;
}

void NeuralNetwork::save(string filename) {
	NBT_Compound root("NeuralNet");

	root.setTag(new NBT_Tag<uint>("NumIns", layerSizes[0] - 1));
	root.setTag(new NBT_Tag<uint>("NumOuts", layerSizes[layerSizes.size() - 1]));
	root.setTag(new NBT_Tag<double>("LearnRate", learningRate));

	NBT_Compound* layers = new NBT_Compound("Layers");

	for (uint c = 0; c < layerWeights.size(); c++) {
		NBT_Compound* matrix = new NBT_Compound("Matrix" + to_string(c));

		for (uint d = 0; d < layerWeights[c].getH(); d++) {
			matrix->setTag(new NBT_Tag<DynVecN<double>>("Row" + to_string(d), layerWeights[c].getRow(d)));
		}

		layers->setTag(matrix);
	}

	root.setTag(layers);

	FileManager* fileMan = FileManager::getSingletonPtr();

	fileMan->createFile(filename);
	fileMan->openFile(filename, FileManager::WRITE);
	fileMan->writeNBT(filename, &root);
	fileMan->closeFile(filename);
}

void NeuralNetwork::load(string filename) {
	FileManager* fileMan = FileManager::getSingletonPtr();

	if (!fileMan->checkFile(filename)) {
		return;
	}

	NBT_Compound* root = NULL;

	fileMan->openFile(filename, FileManager::READ);
	root = fileMan->readNBT(filename);
	fileMan->closeFile(filename);

	uint numIns = static_cast<NBT_Tag<uint>*>(root->getTag("NumIns"))->getData();
	uint numOuts = static_cast<NBT_Tag<uint>*>(root->getTag("NumOuts"))->getData();

	if ((numIns != layerSizes[0] - 1) || (numOuts != layerSizes[layerSizes.size() - 1])) {
		Warn("Loading NeuralNetwork from file with different dimensions\n");

		return;
	}

	learningRate = static_cast<NBT_Tag<double>*>(root->getTag("LearnRate"))->getData();

	NBT_Compound* layers = static_cast<NBT_Compound*>(root->getTag("Layers"));

	for (uint c = 0; c < layers->getTagCount(); c++) {
		NBT_Compound* matrix = static_cast<NBT_Compound*>(layers->getTag("Matrix" + to_string(c)));

		for (uint d = 0; d < matrix->getTagCount(); d++) {
			vector<double> row = static_cast<NBT_Tag<vector<double>>*>(matrix->getTag("Row" + to_string(d)))->getData();

			layerWeights[c].setRow(d, row);
		}
	}

	std::cout << "0: " << layerWeights[0].get(0, 0) << '\n';
	std::cout << "1: " << layerWeights[1].get(1, 1) << '\n';
	std::cout << "2: " << layerWeights[2].get(2, 2) << '\n';
	std::cout << "3: " << layerWeights[3].get(3, 3) << '\n';

	delete root;
}

DynVecN<double> NeuralNetwork::runInput(DynVecN<uint>& inputs, bool training) {
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
		inputMatrix = layerWeights[c].matrixMultiply(inputMatrix);

		if (training) {
			layerLastOutputs[c] = inputMatrix;
		}

		//copy values into new matrix with augmented input
		DynMatrix<double> tempMat(inputMatrix.getH() + 1, 1, 1.0);

		for (uint d = 0; d < inputMatrix.getH(); d++) {
			tempMat.get(d, 0) = activationFunc(inputMatrix.get(d, 0));
		}

		inputMatrix = tempMat;
	}

	//run values through final output layer
	if (training) {
		layerLastInputs[numLayers - 1] = inputMatrix;
	}

	inputMatrix = layerWeights[numLayers - 1].matrixMultiply(inputMatrix);

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
			output[c] = crispFunc(inputMatrix.get(c, 0));
		}
	}

	return output;
}

A_StarNode::A_StarNode(int value) {
	id = value;
}

A_StarNode::~A_StarNode() {
	if (searchNode) {
		delete searchNode;
	}
}

void A_StarNode::addNeighbor(int cost, A_StarNode* node) {
	neighs[cost] = node;
}

bool A_StarNode::operator==(const A_StarNode& other) const {
	return id == other.id;
}

A_StarSolver::A_StarSolver(vector<A_StarNode>& nodes)
	: openList(pair<int*, A_StarNode&>(&dummyCost, dummyNode)) {
	graph = nodes;
}

A_StarSolver::~A_StarSolver() {
	//
}

vector<A_StarNode*> A_StarSolver::solve(uint start, uint end) {
	vector<A_StarNode*> result;

	//add starting node to Open as cost 0
	graph[start].searchNode = new SearchNode();
	graph[start].searchNode->parent = NULL;
	graph[start].searchNode->graphNode = &graph[start];
	graph[start].searchNode->cost = 0;

	openList.push(make_pair(&graph[start].searchNode->cost, reference_wrapper<A_StarNode>(graph[start])));

	while (!openList.empty()) {
		auto curPair = openList.pop();
		int* curCost = curPair.first;
		A_StarNode& curNode = curPair.second.get();

		curNode.visited = true;

		if (curNode == graph[end]) {
			result.push_back(&curNode);

			SearchNode* parent = curNode.searchNode->parent;

			while (parent) {
				result.push_back(parent->graphNode);

				parent = parent->parent;
			}

			reverse(result.begin(), result.end());

			return result;
		}

		for (auto iter = curNode.neighs.begin(); iter != curNode.neighs.end(); iter++) {
			if (!iter->second->visited) {
				iter->second->searchNode = new SearchNode();
				iter->second->searchNode->parent = curNode.searchNode;
				iter->second->searchNode->graphNode = iter->second;
				iter->second->searchNode->cost = *curCost + curNode.searchNode->cost;

				openList.push(make_pair(const_cast<int*>(&iter->first), reference_wrapper<A_StarNode>(*iter->second)));
			}
			else if (openList.exist(make_pair(const_cast<int*>(&iter->first), reference_wrapper<A_StarNode>(*iter->second)))) {
				if (iter->second->searchNode->parent && curNode.searchNode->parent) {
					if (iter->second->searchNode->parent->cost > curNode.searchNode->cost) {
						iter->second->searchNode->parent = curNode.searchNode;
						iter->second->searchNode->cost = *curCost + curNode.searchNode->cost;
					}
				}
			}
			else if (iter->second->visited) {
				if (iter->second->searchNode->parent && curNode.searchNode->parent) {
					if (iter->second->searchNode->parent->cost > curNode.searchNode->cost) {
						iter->second->visited = false;
						iter->second->searchNode->parent = curNode.searchNode;
						iter->second->searchNode->cost = *curCost + curNode.searchNode->cost;

						openList.push(make_pair(const_cast<int*>(&iter->first), reference_wrapper<A_StarNode>(*iter->second)));
					}
				}
			}
		}
	}

	return result;
}

int A_StarSolver::dummyCost(INT_MIN);
A_StarNode A_StarSolver::dummyNode(INT_MIN);

bool Bingo::AI::operator<(OpenType val1, OpenType val2) {
	return val1.first < val2.first;
}

bool Bingo::AI::operator>=(OpenType val1, OpenType val2) {
	return val1.first >= val2.first;
}

bool Bingo::AI::operator==(OpenType val1, OpenType val2) {
	return val1.first == val2.first;
}
