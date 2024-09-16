//Dustin Gehm

#pragma once
#ifndef _AI_H
#define _AI_H

#include <functional>
#include <map>
#include <set>
#include <vector>

#include "Exception.h"
#include "Matrix.h"
#include "MinHeap.h"
#include "RandomManager.h"
#include "Utils.h"
#include "VecN.h"

using std::make_pair;
using std::map;
using std::reference_wrapper;
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

			/**
			 * How quickly the NeuralNetwork will learn during training
			 */
			void setLearningRate(double newRate);
			inline double getLearningRate() const {
				return learningRate;
			}

			void resetTrainingCount();

			/* 
			 * Controls after how many calls to train(...) that the learningRate starts to decrease
			 * Set to 0 to disable
			 */
			void setTrainingThreshold(size_t threshold);
			inline size_t getTrainingThreshold() const {
				return trainingThreshold;
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
			size_t trainingCount = 0;
			size_t trainingThreshold = 100;
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
			//a function to crossover entities
			typedef T(*CrossoverFunc)(const T& val1, const T& val2);
			//a function to change the entity randomly
			typedef T(*MutateFunc)(const T& value);
			//a function to calculate the 'fitness' score of an entity
			typedef int(*ScoreFunc)(const T& value);

			GeneticAlgorithm(int genSize, CreateFunc cFunc, CrossoverFunc crFunc, MutateFunc mFunc, ScoreFunc sFunc)
				: generationSize(genSize) {
				createFunc = cFunc;
				crossoverFunc = crFunc;
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

			void setEvolveRatio(double ratio) {
				evolveRatio = ratio;
			}

			inline double getEvolveRatio() const {
				return evolveRatio;
			}

			void setCrossoverRatio(double ratio) {
				crossoverRatio = ratio;
			}

			inline double getCrossoverRatio() const {
				return crossoverRatio;
			}

			void setMutateRatio(double ratio) {
				mutateRatio = ratio;
			}

			inline double getMutateRatio() const {
				return mutateRatio;
			}

			void setCreateFunc(CreateFunc func) {
				createFunc = func;
			}

			inline CreateFunc getCreateFunc() const {
				return createFunc;
			}

			void setCrossoverFunc(CrossoverFunc func) {
				crossoverFunc = func;
			}

			inline CrossoverFunc getCrossoverFunc() const {
				return crossoverFunc;
			}

			void setMutateFunc(MutateFunc func) {
				mutateFunc = func;
			}

			inline MutateFunc getMutateFunc() const {
				return mutateFunc;
			}

			void setScoreFunc(ScoreFunc func) {
				scoreFunc = func;
			}

			inline ScoreFunc getScoreFunc() const {
				return scoreFunc;
			}

			void insertEntity(T entity) {
				generation.push_back(entity);

				scores.push_back(make_pair(scoreFunc(entity), generationSize));

				//increment size last to get correct index for above
				generationSize += 1;
			}

			//use the genetic algorithm to advance the current generation into a new generation
			void advanceGeneration(uint num = 1) {
				RandomManager& randMan = RandomManager::getSingleton();
				uint evolving = static_cast<uint>(generationSize * evolveRatio);
				uint crossover = static_cast<uint>(generationSize * crossoverRatio);

				for (uint c = 0; c < num; c++) {
					vector<T> best = getBest(evolving);

					//copy all entities from best
					generation.assign(best.begin(), best.end());

					//crossover the best entities
					for (uint d = evolving; d < evolving + crossover; d++) {
						uint i1 = 0;
						uint i2 = 0;

						do {
							i1 = randMan.randInt(0, evolving - 1);
							i2 = randMan.randInt(0, evolving - 1);
						} while (i1 == i2);

						generation.push_back(crossoverFunc(generation[i1], generation[i2]));
					}

					//fill the rest in with new entities
					for (uint d = evolving + crossover; d < generationSize; d++) {
						generation.push_back(createFunc());
					}

					//mutate some of the generation
					uint mutating = static_cast<uint>(generationSize * mutateRatio);
					set<uint> mutated;

					while (mutating) {
						uint index = randMan.randInt(0, generationSize - 1);

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
			uint generationSize;
			double evolveRatio = 0.20;
			double crossoverRatio = 0.6;
			double mutateRatio = 0.75;
			vector<T> generation;
			vector<pair<int, uint>> scores;
			CreateFunc createFunc;
			CrossoverFunc crossoverFunc;
			MutateFunc mutateFunc;
			ScoreFunc scoreFunc;
		};

		namespace InternalUse {
			class SearchNode;
		}

		class A_StarNode {
		public:
			A_StarNode(int value);
			~A_StarNode();

			//adds node to the neighbor list
			void addNeighbor(int cost, A_StarNode* node);

			bool operator==(const A_StarNode& other) const;

		public:
			int id;

		private:
			bool visited = false;
			map<int, A_StarNode*> neighs;
			InternalUse::SearchNode* searchNode = NULL;

			friend class A_StarSolver;
		};

		namespace InternalUse {
			class SearchNode {
			public:
				SearchNode* parent;
				A_StarNode* graphNode;
				int cost = 0;

				bool operator<(const SearchNode& node) {
					return cost < node.cost;
				}

				bool operator>=(const SearchNode& node) {
					return cost >= node.cost;
				}

				bool operator==(const SearchNode& node) {
					return cost == node.cost;
				}
			};

			typedef pair<int*, reference_wrapper<A_StarNode>> OpenType;
		}

		class A_StarSolver {
		public:
			A_StarSolver(vector<A_StarNode>& nodes);
			~A_StarSolver();

			vector<A_StarNode*> solve(uint start, uint end);

		private:
			vector<A_StarNode> graph;

			MinHeap<InternalUse::OpenType> openList;

			static int dummyCost;
			static A_StarNode dummyNode;
		};

		bool operator< (InternalUse::OpenType val1, InternalUse::OpenType val2);
		bool operator>=(InternalUse::OpenType val1, InternalUse::OpenType val2);
		bool operator==(InternalUse::OpenType val1, InternalUse::OpenType val2);

	}

}

#endif