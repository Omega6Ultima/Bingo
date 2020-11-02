//Dustin Gehm

#pragma once
#ifndef _AI_H
#define _AI_H

#include <vector>
#include <set>

#include "Exception.h"
#include "Matrix.h"
#include "RandomManager.h"
#include "Utils.h"
#include "VecN.h"

using std::make_pair;
using std::pair;
using std::set;
using std::vector;

namespace Bingo {

	namespace AI {

		using Math::DynMatrix;
		using Math::DynVecN;

		typedef double(*NeuralNetActivationFunc)(double value);
		typedef double(*NeuralNetActivationDerivFunc)(double value);
		typedef int(*NeuralNetCrispFunc)(double value);

		double sigmoid(double value);
		double sigmoidDeriv(double value);
		int crispify(double value);

		class NeuralNetwork {
		public:
			NeuralNetwork(uint numInputs, uint numOutputs);

			DynVecN<uint> train(DynVecN<uint>& inputs, DynVecN<uint>& expectedOutput);
			DynVecN<uint> run(DynVecN<uint>& inputs);

			void setLearningRate(double newRate);
			inline double getLearningRate() const {
				return learningRate;
			}

			void setActivationFunction(NeuralNetActivationFunc func);
			inline NeuralNetActivationFunc getActivationFunc() const {
				return activationFunc;
			}

			void setActivationDerivFunction(NeuralNetActivationDerivFunc func);
			inline NeuralNetActivationDerivFunc getActivationDerivFunction() const {
				return activationDerivFunc;
			}

			void setCrispFunction(NeuralNetCrispFunc func);
			inline NeuralNetCrispFunc getCrispFunction() const {
				return crispFunc;
			}

			void save(string filename);
			void load(string filename);

		private:
			DynVecN<double> runInput(DynVecN<uint>& inputs, bool training);

		private:
			double learningRate = 0.1;
			NeuralNetActivationFunc activationFunc = sigmoid;
			NeuralNetActivationDerivFunc activationDerivFunc = sigmoidDeriv;
			NeuralNetCrispFunc crispFunc = crispify;
			const uint numLayers;
			vector<uint> layerSizes;
			vector<DynMatrix<double>> layerWeights;
			vector<DynMatrix<double>> layerLastInputs;
			vector<DynMatrix<double>> layerLastOutputs;
		};

		template<class T>
		class GeneticAlgorithm {
		public:
			//a function to create entities
			typedef T(*CreateFunc)();
			//a function to change the entity randomly
			typedef T(*MutateFunc)(const T& value);
			//a function to calculate the 'fitness' score of an entity
			typedef int(*ScoreFunc)(const T& value);

			GeneticAlgorithm(int genSize, CreateFunc cFunc, MutateFunc mFunc, ScoreFunc sFunc)
				: generationSize(genSize) {
				createFunc = cFunc;
				mutateFunc = mFunc;
				scoreFunc = sFunc;

				for (uint c = 0; c < generationSize; c++) {
					T entity = createFunc();

					scores.push_back(make_pair(scoreFunc(entity), c));
					generation.push_back(entity);
				}
			}

			~GeneticAlgorithm() {
				//
			}

			inline uint getGenerationSize() const {
				return generationSize;
			}

			inline double getEvolveRatio() const {
				return evolveRatio;
			}

			void setEvolveRation(double ratio) {
				evolveRatio = ratio;
			}

			inline double getMutateRatio() const {
				return mutateRatio;
			}

			void setMutateRatio(double ratio) {
				mutateRatio = ratio;
			}

			void setCreateFunc(CreateFunc func) {
				createFunc = func;
			}

			void setScoreFunc(ScoreFunc func) {
				scoreFunc = func;
			}

			void setMutateFunc(MutateFunc func) {
				mutateFunc = func;
			}

			//use the genetic algorithm to advance the current generation into a new generation
			void advanceGeneration(uint num = 1) {
				RandomManager* randMan = RandomManager::getSingletonPtr();
				uint evolving = static_cast<uint>(generationSize * evolveRatio);

				for (uint c = 0; c < num; c++) {
					vector<T> best = getBest(evolving);

					//copy all entities from best
					generation.assign(best.begin(), best.end());

					//fill the rest in with new entities
					for (uint d = evolving; d < generationSize; d++) {
						generation.push_back(createFunc());
					}

					//mutate some of the generation
					uint mutating = static_cast<uint>(generationSize * mutateRatio);
					set<uint> mutated;

					while (mutating) {
						uint index = randMan->randInt(0, generationSize - 1);

						if (mutated.find(index) == mutated.end()) {
							mutated.insert(index);

							mutating--;

							generation[index] = mutateFunc(generation[index]);
						}
					}

					//calculate scores
					for (uint d = 0; d < generationSize; d++) {
						scores[d] = make_pair(scoreFunc(generation[d]), d);
					}
				}
			}

			//retrieve the num most fit entities from the current generation
			vector<T> getBest(uint num = 1) {
				vector<T> best;
				vector<pair<int, uint>> scoresCopy(scores.begin(), scores.end());

				//use reverse iterators to copy in descending order
				sort(scoresCopy.rbegin(), scoresCopy.rend());

				for (uint c = 0; c < num; c++) {
					best.push_back(generation[scoresCopy[c].second]);
				}

				return best;
			}
		private:
			const uint generationSize;
			double evolveRatio = 0.5;
			double mutateRatio = 0.75;
			vector<T> generation;
			vector<pair<int, uint>> scores;
			CreateFunc createFunc;
			MutateFunc mutateFunc;
			ScoreFunc scoreFunc;
		};

	}

}

#endif