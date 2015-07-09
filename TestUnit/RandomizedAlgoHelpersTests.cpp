#include "CppUnitTest.h"
#include "ToStringSpecializations.cpp"
#include <cstdlib>

#include "RandomizedAlgoEvaluator.h"
#include "RandomizedAlgoOptimizer.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestUnit
{
	class RandomMax
	{
	protected:
		int maxVal;

	public:
		static const int BEST_RESULT_PROB_ID = -10;

		void nextRandomProbs()
		{
			maxVal++;
		}
		bool allProbsTested()
		{
			return (maxVal > 10);
		}
		int getRandomProbsId()
		{
			return -maxVal;
		}
		float solve()
		{
			return (float)(rand() % (maxVal * 10 + 1));
		}

		RandomMax() : maxVal(0)
		{ }
	};
	class RandomMaxMultipleVars
	{
	protected:
		int val1;
		int val2;
		int val3;
		int val4;

	public:
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
			int maxRand = max(5 * val1 - 2 * val2 + 3 * val3 - 4 * val4, 1);
			return (float)(rand() % maxRand);
		}

		RandomMaxMultipleVars() : val1(0), val2(0), val3(0), val4(0)
		{ }
	};

	TEST_CLASS(RandomizedAlgoEvaluatorTests)
	{
		TEST_METHOD(RandEval_RandomMaxTest)
		{
			RandomMax randMax;
			RandomizedAlgoEvaluator<RandomMax> evaluator(randMax);
			evaluator.run();
			Assert::AreEqual(RandomMax::BEST_RESULT_PROB_ID, evaluator.getBestResultsProbsId());
		}

		TEST_METHOD(RandEval_RandomMaxMultipleVarsTest)
		{
			RandomMaxMultipleVars randMaxMultVars;
			RandomizedAlgoEvaluator<RandomMaxMultipleVars> evaluator(randMaxMultVars, 3000);
			evaluator.run();
			Assert::AreEqual(RandomMaxMultipleVars::BEST_RESULT_PROB_ID, evaluator.getBestResultsProbsId());
		}
	};

	TEST_CLASS(RandomizedAlgoOptimizerTests)
	{
	};
}
