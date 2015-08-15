#include "CppUnitTest.h"
#include "ToStringSpecializations.h"
#include <cstdlib>
#include <cmath>
#include <array>
#include <sstream>

//#define ALGORITHM_OPTIMIZATION_DEBUG_LOG(msg)	do { ostringstream ss; ss << msg; Logger::WriteMessage(ss.str().c_str()); } while (false)

#include "RandomizedAlgoEvaluator.h"
#include "RandomizedAlgoOptimizer.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestUnit
{
	class RandomMax
	{
	public:
		float maxVal;

		static const int BEST_RESULT_PROB_ID = -10;

		void nextRandomProbs()
		{
			maxVal += 0.1f;
		}
		bool allProbsTested()
		{
			return (maxVal >= 1.01f);
		}
		int getRandomProbsId()
		{
			return (int)(maxVal * -10.0f);
		}
		float solve()
		{
			return (float)(rand() % ((int)(maxVal * 100.0f) + 1));
		}

		RandomMax() : maxVal(0.0f)
		{ }
	};
	class RandomMaxMultipleVars
	{
	public:
		int val1;
		int val2;
		int val3;
		int val4;

		static const int BEST_RESULT_PROB_ID = (((10 * 11) + 0) * 11 + 10) * 11 + 0;

		void nextRandomProbs()
		{
			if (val4 < 10)
			{
				val4++;
				return;
			}
			val4 = 0;

			if (val3 < 10)
			{
				val3++;
				return;
			}
			val3 = 0;

			if (val2 < 10)
			{
				val2++;
				return;
			}
			val2 = 0;

			val1++;
		}
		bool allProbsTested()
		{
			return (val1 > 10);
		}
		int getRandomProbsId()
		{
			return (((val1 * 11) + val2) * 11 + val3) * 11 + val4;
		}
		float solve()
		{
			int maxRand = max(5 * val1 - 2 * val2 + val3 - 4 * val4, 1);
			return (float)(rand() % maxRand);
		}

		RandomMaxMultipleVars() : val1(0), val2(0), val3(0), val4(0)
		{ }
	};
	class RandomMaxMiddleVal
	{
	public:
		// val doit être compris entre 0 et 2.
		// L'espérance de solve() est maximale lorsque val vaut 1.
		float val;

		static const int BEST_RESULT_PROB_ID = 10;

		void nextRandomProbs()
		{
			val += 0.2f;
		}
		bool allProbsTested()
		{
			return (val >= 2.01f);
		}
		int getRandomProbsId()
		{
			return (int)(val * 10.0f);
		}
		float solve()
		{
			int randMod = (int)(100.0f * (1.0f - (val - 1.0f) * (val - 1.0f)));
			return (float)(rand() % (randMod + 1));
		}

		RandomMaxMiddleVal() : val(0.0f)
		{ }
	};

	TEST_CLASS(RandomizedAlgoEvaluatorTests)
	{
		TEST_METHOD(RandEval_RandMaxTest)
		{
			RandomMax randMax;
			RandomizedAlgoEvaluator<RandomMax> evaluator(randMax);
			evaluator.run();
			Assert::AreEqual(RandomMax::BEST_RESULT_PROB_ID, evaluator.getBestResultsProbsId());
		}

		TEST_METHOD(RandEval_RandMaxMultipleVarsTest)
		{
			RandomMaxMultipleVars randMaxMultVars;
			RandomizedAlgoEvaluator<RandomMaxMultipleVars> evaluator(randMaxMultVars, 5000);
			evaluator.run();
			Assert::AreEqual(RandomMaxMultipleVars::BEST_RESULT_PROB_ID, evaluator.getBestResultsProbsId());
		}

		TEST_METHOD(RandEval_RandMaxMiddleVal)
		{
			RandomMaxMiddleVal randMaxMiddleVal;
			RandomizedAlgoEvaluator<RandomMaxMiddleVal> evaluator(randMaxMiddleVal, 2000);
			evaluator.run();
			Assert::AreEqual(RandomMaxMiddleVal::BEST_RESULT_PROB_ID, evaluator.getBestResultsProbsId());
		}
	};

	TEST_CLASS(RandomizedAlgoOptimizerTests)
	{
		TEST_METHOD(RandOpt_RandMaxTest)
		{
			// Test basique : faire monter la valeur d'un paramètre jusqu'à son maximum.

			RandomMax randMax;

			vector<ParamRange<float> > params;
			params.push_back(ParamRange<float>(&randMax.maxVal, 0.0f, 1.001f, 0.1f));

			RandomizedAlgoOptimizer<RandomMax, float> optimizer(randMax, params);
			optimizer.run();
			Assert::AreEqual(*(optimizer.getBestParams()[0].value), 1.0f, 0.01f);
		}

		TEST_METHOD(RandOpt_RandMaxMultipleVarsTest)
		{
			// Teste le cas de plusieurs variables, dont certaines ont une influence très faible.

			RandomMaxMultipleVars randMaxMultVars;

			vector<ParamRange<int> > params;
			params.push_back(ParamRange<int>(&randMaxMultVars.val1, 0, 10, 1));
			params.push_back(ParamRange<int>(&randMaxMultVars.val2, 0, 10, 1));
			params.push_back(ParamRange<int>(&randMaxMultVars.val3, 0, 10, 1));
			params.push_back(ParamRange<int>(&randMaxMultVars.val4, 0, 10, 1));

			RandomizedAlgoOptimizer<RandomMaxMultipleVars, int> optimizer(randMaxMultVars, params);
			optimizer.run();
			Assert::AreEqual(10, *(optimizer.getBestParams()[0].value));
			Assert::AreEqual(0, *(optimizer.getBestParams()[1].value));
			Assert::AreEqual(10, *(optimizer.getBestParams()[2].value));
			Assert::AreEqual(0, *(optimizer.getBestParams()[3].value));
		}

		TEST_METHOD(RandOpt_RandMaxMiddleValFewLoops)
		{
			// Teste la capacité de l'algorithme à s'arrêter sur une valeur intermédiaire,
			// en lui fournissant très peu de boucles d'évaluation !

			RandomMaxMiddleVal randMaxMiddleVal;

			vector<ParamRange<float> > params;
			params.push_back(ParamRange<float>(&randMaxMiddleVal.val, 0.0f, 2.001f, 0.2f));

			RandomizedAlgoOptimizer<RandomMaxMiddleVal, float> optimizer(randMaxMiddleVal, params, 250);
			optimizer.run();
			Assert::AreEqual(*(optimizer.getBestParams()[0].value), 1.0f, 0.01f);
		}

		TEST_METHOD(RandOpt_PlaceboParameter)
		{
			// On teste ici qu'avec un pas suffisamment petit, en cas de variable placebo (sans effet),
			// l'algorithme ne s'écartera pas beaucoup de la valeur initiale de cette variable
			// (écart inférieur à 5 fois la valeur du pas).

			RandomMax randMax;
			randMax.maxVal = 0.7f;

			array<float, 10> startVals = { -0.73f, 0.14f, 0.0f, 0.24f, 0.33f, 0.54f, 0.62f, 0.71f, 1.0f, 1.42f };
			for (float startVal : startVals)
			{
				float placebo;
				vector<ParamRange<float> > params;
				params.push_back(ParamRange<float>(&placebo, startVal, 2.001f, 0.001f));

				RandomizedAlgoOptimizer<RandomMax, float> optimizer(randMax, params);
				optimizer.run();
				Assert::AreEqual(*(optimizer.getBestParams()[0].value), startVal, 0.05f);
			}
		}
	};
}
