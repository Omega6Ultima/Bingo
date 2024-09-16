#pragma once
#include "pch.h"
#include "AI.h"

TEST(TestNN, TestSigmoid) {
	EXPECT_EQ(Bingo::AI::sigmoid(0.0), 0.5);
	EXPECT_NEAR(Bingo::AI::sigmoid(2.0), 0.982013, .001);
}

TEST(TestNN, TestSigmoidDeriv) {
	EXPECT_EQ(Bingo::AI::sigmoidDeriv(0.0), 0.25);
	EXPECT_NEAR(Bingo::AI::sigmoidDeriv(2.0), 0.017662, .001);
}

TEST(TestNN, TestCrispify) {
	EXPECT_EQ(Bingo::AI::crispify(0.0), 0);
	EXPECT_EQ(Bingo::AI::crispify(0.3), 0);
	EXPECT_EQ(Bingo::AI::crispify(0.5), 0);
	EXPECT_EQ(Bingo::AI::crispify(0.51), 1);
	EXPECT_EQ(Bingo::AI::crispify(0.75), 1);
	EXPECT_EQ(Bingo::AI::crispify(1.0), 1);
}

TEST(TestNN, TestConstruct) {
	auto nn = Bingo::AI::NeuralNetwork(5, 2);
	auto nn2 = Bingo::AI::NeuralNetwork(50, 10);

	EXPECT_EQ(nn.getLearningRate(), nn2.getLearningRate());
}

TEST(TestNN, TestConstruct2) {
	auto nn = new Bingo::AI::NeuralNetwork(3, 1);
	auto nn2 = new Bingo::AI::NeuralNetwork(10, 3);

	EXPECT_EQ(nn->getLearningRate(), nn2->getLearningRate());

	delete nn;
	delete nn2;
}
 
TEST(TestNN, TestSetGetLearningRate) {
	auto nn = Bingo::AI::NeuralNetwork(2, 1);

	EXPECT_NO_THROW(nn.setLearningRate(1.0));

	EXPECT_EQ(nn.getLearningRate(), 1.0);
}

TEST(TestNN, TestResetTrainingCount) {
	auto nn = Bingo::AI::NeuralNetwork(2, 1);

	EXPECT_NO_THROW(nn.resetTrainingCount());
}

TEST(TestNN, TestResetTrainingCount2) {
	auto nn = Bingo::AI::NeuralNetwork(2, 1);

	auto in = Bingo::Math::DynVecN<uint>(2, { 0, 0, });
	auto out = Bingo::Math::DynVecN<uint>(1, { 1, });

	for (size_t i = 0; i < 100; i++) {
		nn.train(in, out);
	}

	EXPECT_NO_THROW(nn.resetTrainingCount());
}

TEST(TestNN, TestSetGetTrainingThreshold) {
	auto nn = Bingo::AI::NeuralNetwork(2, 1);

	EXPECT_NO_THROW(nn.setTrainingThreshold(150));

	EXPECT_EQ(nn.getTrainingThreshold(), 150);
}

TEST(TestNN, TestTrain) {
	auto nn = Bingo::AI::NeuralNetwork(3, 3);
	auto in = Bingo::Math::DynVecN<uint>({ 0, 1, 0 });
	auto out = Bingo::Math::DynVecN<uint>({ 1, 0, 1 });

	EXPECT_NO_THROW(nn.train(in, out));
}

TEST(TestNN, TestRun) {
	// Create RandomManager to seed randomness
	Bingo::RandomManager randMan;

	size_t numCorrect = 0;
	const size_t maxRuns = 10;

	for (size_t i = 0; i < maxRuns; i++) {
		auto nn = Bingo::AI::NeuralNetwork(3, 3);
		const size_t trainRuns = 100;

		nn.setTrainingThreshold(trainRuns / 2);

		// Training behavior of flipping 0's and 1's
		{
			auto in = Bingo::Math::DynVecN<uint>(3, { 0, 0, 0 });
			auto out = Bingo::Math::DynVecN<uint>(3, { 1, 1, 1 });

			if (i == 0) {
				std::cout << "Training (" << in << ") -> (" << out << ")" << std::endl;
			}

			for (size_t _ = 0; _ < trainRuns; _++) {
				nn.train(in, out);
			}
		}

		nn.resetTrainingCount();

		for (size_t j = 0; j < 3; j++) {
			auto in = Bingo::Math::DynVecN<uint>(3);
			auto out = Bingo::Math::DynVecN<uint>(3);

			in.fill(0);
			out.fill(1);

			in.set(j, 1);
			out.set(j, 0);

			if (i == 0) {
				std::cout << "Training (" << in << ") -> (" << out << ")" << std::endl;
			}

			for (size_t _ = 0; _ < trainRuns; _++) {
				nn.train(in, out);
			}
		}

		nn.resetTrainingCount();

		for (size_t j = 0; j < 3; j++) {
			auto in = Bingo::Math::DynVecN<uint>(3);
			auto out = Bingo::Math::DynVecN<uint>(3);

			in.fill(1);
			out.fill(0);

			in.set(j, 0);
			out.set(j, 1);

			if (i == 0) {
				std::cout << "Training (" << in << ") -> (" << out << ")" << std::endl;
			}

			for (size_t _ = 0; _ < trainRuns; _++) {
				nn.train(in, out);
			}
		}

		nn.resetTrainingCount();

		auto testIn = Bingo::Math::DynVecN<uint>({ 1, 1, 1 });
		auto testOut = Bingo::Math::DynVecN<uint>({ 0, 0, 0 });

		//EXPECT_EQ(nn.run(testIn), testOut);
		if (nn.run(testIn) == testOut) {
			numCorrect++;
		}
	}

	std::cout << "Got " << numCorrect << " tries out of " << maxRuns << std::endl;

	EXPECT_GT(numCorrect, maxRuns / 2);
}

size_t createNumber() {
	return Bingo::RandomManager::randInt(0, 50);
}

size_t crossoverNumber(const size_t& val1, const size_t& val2) {
	return (val1 + val2) / 2;
}

size_t mutateNumber(const size_t& val) {
	auto mutation = Bingo::RandomManager::randPosNeg() +
		Bingo::RandomManager::randInt(1, 3);
	return val + mutation;
}

int scoreNumber(const size_t& val) {
	auto score = 0;

	if ((val % 5) == 0) {
		score += 5;
	}

	if ((val % 2) == 0) {
		score += 10;
	}

	if ((val % 6) == 0) {
		score += 20;
	}

	return score;
}

#define GA_FUNCS createNumber, crossoverNumber, mutateNumber, scoreNumber

TEST(TestGA, TestConstruct) {
	auto ga = Bingo::AI::GeneticAlgorithm<size_t>(50, GA_FUNCS);
	auto ga2 = new Bingo::AI::GeneticAlgorithm<size_t>(50, GA_FUNCS);

	EXPECT_EQ(ga.getGenerationSize(), 50);
	EXPECT_EQ(ga2->getGenerationSize(), 50);

	delete ga2;
}

TEST(TestGA, TestConstruct2) {
	auto ga = Bingo::AI::GeneticAlgorithm<size_t>(500, GA_FUNCS);

	EXPECT_EQ(ga.getGenerationSize(), 500);
}

TEST(TestGA, TestGetGenerationSize) {
	auto ga = Bingo::AI::GeneticAlgorithm<size_t>(10, GA_FUNCS);

	EXPECT_NO_THROW(ga.getGenerationSize());
}

TEST(TestGA, TestSetGetEvolveRatio) {
	auto ga = Bingo::AI::GeneticAlgorithm<size_t>(10, GA_FUNCS);

	EXPECT_NO_THROW(ga.setEvolveRatio(0.65));

	EXPECT_EQ(ga.getEvolveRatio(), 0.65);
}

TEST(TestGA, TestSetGetCrossoverRatio) {
	auto ga = Bingo::AI::GeneticAlgorithm<size_t>(10, GA_FUNCS);

	EXPECT_NO_THROW(ga.setCrossoverRatio(0.42));

	EXPECT_EQ(ga.getCrossoverRatio(), 0.42);
}

TEST(TestGA, TestSetGetMutateRatio) {
	auto ga = Bingo::AI::GeneticAlgorithm<size_t>(10, GA_FUNCS);

	EXPECT_NO_THROW(ga.setMutateRatio(0.86));

	EXPECT_EQ(ga.getMutateRatio(), 0.86);
}

size_t createNumberAlt() {
	return Bingo::RandomManager::randInt(0, 50);
}

size_t crossoverNumberAlt(const size_t& val1, const size_t& val2) {
	return (val1 + val2) / 3;
}

size_t mutateNumberAlt(const size_t& val) {
	auto mutation = Bingo::RandomManager::randPosNeg() +
		Bingo::RandomManager::randInt(4, 6);
	return val + mutation;
}

int scoreNumberAlt(const size_t& val) {
	auto score = 0;

	if ((val % 5) == 1) {
		score += 5;
	}

	if ((val % 2) == 1) {
		score += 10;
	}

	if ((val % 6) == 1) {
		score += 20;
	}

	return score;
}

TEST(TestGA, TestSetGetCreateFunc) {
	auto ga = Bingo::AI::GeneticAlgorithm<size_t>(10, GA_FUNCS);

	EXPECT_NO_THROW(ga.setCreateFunc(createNumberAlt));

	EXPECT_TRUE(ga.getCreateFunc() == createNumberAlt);
}

TEST(TestGA, TestSetGetCrossoverFunc) {
	auto ga = Bingo::AI::GeneticAlgorithm<size_t>(10, GA_FUNCS);

	EXPECT_NO_THROW(ga.setCrossoverFunc(crossoverNumberAlt));

	EXPECT_TRUE(ga.getCrossoverFunc() == crossoverNumberAlt);
}

TEST(TestGA, TestSetGetMutateFunc) {
	auto ga = Bingo::AI::GeneticAlgorithm<size_t>(10, GA_FUNCS);

	EXPECT_NO_THROW(ga.setMutateFunc(mutateNumberAlt));

	EXPECT_TRUE(ga.getMutateFunc() == mutateNumberAlt);
}

TEST(TestGA, TestSetGetScoreFunc) {
	auto ga = Bingo::AI::GeneticAlgorithm<size_t>(10, GA_FUNCS);

	EXPECT_NO_THROW(ga.setScoreFunc(scoreNumberAlt));

	EXPECT_TRUE(ga.getScoreFunc() == scoreNumberAlt);
}

TEST(TestGA, TestInsertEntity) {
	auto ga = Bingo::AI::GeneticAlgorithm<size_t>(10, GA_FUNCS);

	EXPECT_NO_THROW(ga.insertEntity(42));
}

TEST(TestGA, TestAdvanceGeneration) {
	auto ga = Bingo::AI::GeneticAlgorithm<size_t>(10, GA_FUNCS);

	EXPECT_NO_THROW(ga.advanceGeneration(20));

	auto best = ga.getBest();

	std::cout << "Got best of " << best[0] << ", score(" << scoreNumber(best[0]) << ")" << std::endl;

	EXPECT_GE(scoreNumber(best[0]), 30);
}

TEST(TestGA, TestGetBest) {
	auto ga = Bingo::AI::GeneticAlgorithm<size_t>(10, GA_FUNCS);

	ga.insertEntity(60);

	auto best = ga.getBest();

	EXPECT_EQ(best.size(), 1);
	EXPECT_EQ(best[0], 60);
}