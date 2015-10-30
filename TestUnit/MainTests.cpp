#include "CppUnitTest.h"
#include "ToStringSpecializations.h"
#include <cstdlib>
#include <ctime>
#include <array>

#include "List.c"
#include "cost_priority_queue.h"
#include "ModulusNumber.h"
#include "Memoizator.h"
#include "Bruteforcer.h"
#include "Graph.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestUnit
{
	TEST_CLASS(ListTests)
	{
	public:
		TEST_METHOD(ListCreate)
		{
			List* l = list_create();
			Assert::AreNotEqual((List*)NULL, l);
			Assert::AreEqual(0u, list_getSize(l));
			Assert::IsTrue(list_isEmpty(l));
			list_delete(l);
		}

		TEST_METHOD(ListPushAndPop)
		{
			List* l = list_create();
			list_pushBack(l, 5);
			list_pushBack(l, 23);
			list_pushFront(l, 14);
			list_pushFront(l, 7);
			Assert::AreEqual(4u, list_getSize(l));
			Assert::IsFalse(list_isEmpty(l));
			Assert::AreEqual(7, list_front(l));
			Assert::AreEqual(23, list_back(l));
			
			list_popFront(l);
			list_popBack(l);
			Assert::AreEqual(14, list_front(l));
			Assert::AreEqual(5, list_back(l));

			list_popFront(l);
			list_popBack(l);
			Assert::AreEqual(0u, list_getSize(l));
			Assert::IsTrue(list_isEmpty(l));

			list_pushFront(l, 25);
			Assert::AreEqual(1u, list_getSize(l));
			Assert::IsFalse(list_isEmpty(l));
			list_popFront(l);
			Assert::AreEqual(0u, list_getSize(l));
			Assert::IsTrue(list_isEmpty(l));

			list_delete(l);
		}

		TEST_METHOD(ListIterations)
		{
			List* l = list_create();
			list_pushBack(l, 25);
			list_pushBack(l, 30);
			list_pushBack(l, 12);
			list_pushBack(l, -69);

			const int res[4] = { 25, 30, 12, -69 };

			int i = 0;
			for (Cell* it = list_getFrontIterator(l); list_isValidIt(it); it = list_incrIt(it))
			{
				Assert::AreEqual(res[i], list_getElt(it));

				if (i == 0)
				{
					Assert::IsTrue(list_isFirst(it));
					Assert::IsFalse(list_isLast(it));
				}
				else if (i == 3)
				{
					Assert::IsFalse(list_isFirst(it));
					Assert::IsTrue(list_isLast(it));
				}
				else
				{
					Assert::IsFalse(list_isFirst(it));
					Assert::IsFalse(list_isLast(it));
				}

				i++;
			}

			i = 3;
			for (Cell* it = list_getBackIterator(l); list_isValidIt(it); it = list_decrIt(it))
			{
				Assert::AreEqual(res[i], list_getElt(it));

				if (i == 0)
				{
					Assert::IsTrue(list_isFirst(it));
					Assert::IsFalse(list_isLast(it));
				}
				else if (i == 3)
				{
					Assert::IsFalse(list_isFirst(it));
					Assert::IsTrue(list_isLast(it));
				}
				else
				{
					Assert::IsFalse(list_isFirst(it));
					Assert::IsFalse(list_isLast(it));
				}

				i--;
			}

			list_delete(l);
		}
	};

	TEST_CLASS(CostPriorityQueueTests)
	{
		TEST_METHOD(CostPriorityQueueSimple)
		{
			cost_priority_queue<int, int> pq;
			Assert::IsTrue(pq.empty());
#define COST_PRIORITY_QUEUE_SIMPLE_ELTS_SIZE	11
			const pair<int, int> elts[COST_PRIORITY_QUEUE_SIMPLE_ELTS_SIZE] =
			{
				pair<int, int>(2, 65),
				pair<int, int>(2, 70),
				pair<int, int>(5, 65),	// Pour les cas d'égalité, les éléments les plus récents doivent être renvoyés en premier
				pair<int, int>(9, -32),
				pair<int, int>(10, -62),
				pair<int, int>(9, 32),
				pair<int, int>(415, 8697),
				pair<int, int>(-6590, 466),
				pair<int, int>(3, 65),
				pair<int, int>(983, 256),
				pair<int, int>(-489, -58)
			};
			const int ordered_elts[COST_PRIORITY_QUEUE_SIMPLE_ELTS_SIZE] =
				{ 10, -489, 9, 9, 3, 5, 2, 2, 983, -6590, 415 };
			for (int i = 0; i < COST_PRIORITY_QUEUE_SIMPLE_ELTS_SIZE; i++)
			{
				pq.push(elts[i].first, elts[i].second);
				Assert::IsFalse(pq.empty());
			}
			for (int i = 0; i < COST_PRIORITY_QUEUE_SIMPLE_ELTS_SIZE; i++)
			{
				int expected = ordered_elts[i];
				Assert::IsFalse(pq.empty());
				int actual = pq.top();
				pq.pop();
				Assert::AreEqual(expected, actual);
			}
			Assert::IsTrue(pq.empty());
		}
	};

	TEST_CLASS(ModulusNumberTests)
	{
		TEST_METHOD(ModulusNumber_BasicTests)
		{
			ModulusNumber<15> mnb;
			Assert::AreEqual(0, (int)mnb);

			mnb++;
			Assert::AreEqual(1, (int)mnb);

			mnb += 3;
			Assert::AreEqual(4, mnb.v());

			mnb += 26;
			Assert::AreEqual(0, (int)mnb);

			mnb += (-4);
			Assert::AreEqual(11, mnb.v());

			mnb -= (-6);
			Assert::AreEqual(2, (int)mnb);

			ModulusNumber<15> mnb2(35);
			Assert::AreEqual(5, (int)mnb2);

			Assert::AreEqual(6, (int)(mnb + mnb2 - ModulusNumber<15>(16)));

			Assert::IsTrue(mnb < mnb2);
			Assert::IsTrue(mnb <= mnb2);
			Assert::IsFalse(mnb > mnb2);
			Assert::IsFalse(mnb >= mnb2);
			Assert::IsFalse(mnb == mnb2);
			Assert::IsTrue(mnb != mnb2);
			mnb += 3;
			Assert::IsFalse(mnb < mnb2);
			Assert::IsTrue(mnb <= mnb2);
			Assert::IsFalse(mnb > mnb2);
			Assert::IsTrue(mnb >= mnb2);
			Assert::IsTrue(mnb == mnb2);
			Assert::IsFalse(mnb != mnb2);

			mnb2 += 16;
			mnb = mnb2;
			ModulusNumber<15> mnb3(mnb);
			Assert::AreEqual(6, (int)(mnb3));

			mnb3 = -mnb3;
			Assert::AreEqual(9, (int)(mnb3));

			mnb3 *= -2;
			Assert::AreEqual(12, mnb3.v());

			mnb3 /= mnb;
			Assert::AreEqual(2, (int)mnb3);

			Assert::AreEqual(4, mnb.distanceTo(mnb3));
			Assert::AreEqual(4, mnb3.distanceTo(mnb));
			Assert::AreEqual(0, mnb.distanceTo(mnb2));

			ModulusNumber<1000000007, long long int> llMnb(1);
			for (int i = 0; i < 30; i++)
				llMnb *= 2;
			Assert::AreEqual(73741817ll, llMnb.v());	// 2^30 % (10^9 + 7)
			llMnb *= llMnb;
			Assert::AreEqual(536396504ll, llMnb.v());	// 2^60 % (10^9 + 7)
			llMnb *= 164973264845;
			Assert::AreEqual(886528608ll, llMnb.v());
			llMnb *= (-5);
			Assert::AreEqual(567356995ll, llMnb.v());
			llMnb /= 5;
			Assert::AreEqual(113471399ll, llMnb.v());

			ostringstream oss;
			oss << llMnb;
			Assert::AreEqual(string("113471399"), oss.str());

			istringstream iss("1099511627776");
			iss >> llMnb;
			Assert::AreEqual(511620083ll, llMnb.v());
		}
	};

	TEST_CLASS(MemoizatorTests)
	{
		static int simpleFunc(int x)
		{
			return x + 3;
		}

		class ClassFunc
		{
		protected:
			int offset;

		public:
			ClassFunc(int addOffset) : offset(addOffset)
			{ }
			int operator()(int x)
			{
				return x + offset;
			}
		};

		template<class RecFunc>
		static int recFibonacci(RecFunc& recFibo, int n)
		{
			if (n <= 1)
				return n;

			return recFibo(n - 1) + recFibo(n - 2);
		}
		static int fibonacci(int n)
		{
			return recFibonacci(fibonacci, n);
		}

		TEST_METHOD(Memoizator_SimpleFunction)
		{
			Memoizator<int, int> simpleFunc_mem(simpleFunc);
			for (int i = -5; i <= 5; i++)
				Assert::AreEqual(simpleFunc(abs(i)), simpleFunc_mem(abs(i)));
			Assert::AreEqual((size_t)6, simpleFunc_mem.getTableSize());
		}
		TEST_METHOD(Memoizator_ClassFunction)
		{
			ClassFunc classFunc(10);
			Memoizator<int, int> classFunc_mem(classFunc);
			for (int i = -5; i <= 5; i++)
				Assert::AreEqual(classFunc(abs(i)), classFunc_mem(abs(i)));
			Assert::AreEqual((size_t)6, classFunc_mem.getTableSize());
		}
		TEST_METHOD(Memoizator_LambdaFunction)
		{
			Memoizator<int, int> lambda_mem(
				[](int x)
				{
					return x * x;
				});
			for (int i = -5; i <= 5; i++)
				Assert::AreEqual(i * i, lambda_mem(abs(i)));
			Assert::AreEqual((size_t)6, lambda_mem.getTableSize());
		}
		TEST_METHOD(Memoizator_ComplexArgs)
		{
			auto f = [](pair<int, float> p) { return p.second * (float)p.first; };
			Memoizator<float, pair<int, float> > f_mem(f);

			for (int i = -5; i <= 5; i++)
			{
				for (float j = -5.0f; j < 5.5f ; j += 1.0f)
				{
					auto arg = make_pair(abs(i), abs(j));
					Assert::AreEqual(f(arg), f_mem(arg));
				}
			}
			Assert::AreEqual((size_t)36, f_mem.getTableSize());
		}
		TEST_METHOD(Memoizator_MultipleArguments)
		{
			auto add = [](int x, int y) { return x + y; };
			MultipleArgsMemoizator<int, int, int> add_mem(add);

			for (int i = -5; i <= 5; i++)
				for (int j = 0; j <= 5; j++)
					Assert::AreEqual(add(abs(i), abs(j)), add_mem(abs(i), abs(j)));
			Assert::AreEqual((size_t)36, add_mem.getTableSize());
		}
		TEST_METHOD(Memoizator_RecursiveFunction)
		{
			typedef RecursiveMemoizator<int, int> RecMem;
			RecMem fibo_recMem(recFibonacci<RecMem>);
			int res = fibo_recMem(1);
			for (int i = -5; i <= 5; i++)
				Assert::AreEqual(fibonacci(abs(i)), fibo_recMem(abs(i)));
			Assert::AreEqual((size_t)6, fibo_recMem.getTableSize());
		}
		TEST_METHOD(Memoizator_RecursiveMultipleArguments)
		{
			auto recAdd = [](auto& recAddFunc, int x, int y)
				{
					if (y < 1)
						return x + y;
					else
						return 1 + recAddFunc(x, y - 1);
				};

			RecursiveMultipleArgsMemoizator<int, int, int> recAdd_mem(recAdd);

			for (int i = -5; i <= 5; i++)
				for (int j = 0; j <= 5; j++)
					Assert::AreEqual(abs(i) + abs(j), recAdd_mem(abs(i), abs(j)));
			Assert::AreEqual((size_t)36, recAdd_mem.getTableSize());
		}
		TEST_METHOD(Memoizator_TemporaryObjects)
		{
			function<int(int)> mem(Memoizator<int, int>([](int x)
				{ return 2 * x; }));
			Assert::AreEqual(64, mem(32));

			function<int(int)> recMem(RecursiveMemoizator<int, int>([](auto& rec, int x)
				{ return (x <= 0 ? x : rec(x - 1) + 1); }));
			Assert::AreEqual(16, recMem(16));

			function<int(int, int)> multMem(MultipleArgsMemoizator<int, int, int>([](int x, int y)
				{ return x + y; }));
			Assert::AreEqual(96, multMem(64, 32));

			function<int(int, int)> recMultMem(RecursiveMultipleArgsMemoizator<int, int, int>([](auto& rec, int x, int y)
				{ return (x <= 0 ? x + y : rec(x - 1, y) + 1); }));
			Assert::AreEqual(48, recMultMem(32, 16));
		}
	};

	TEST_CLASS(BruteforcerTests)
	{
		TEST_METHOD(Bruteforcer_RandomArrayRead)
		{
#ifdef _DEBUG
			constexpr int size = 4;
#else
			constexpr int size = 32;
#endif
			vector<int> arr(size * size * size * size);
			for (size_t i = 0; i < arr.size(); i++)
				arr[i] = (rand() % 1000000);

			int maxPos = max_element(arr.begin(), arr.end()) - arr.begin();
			int maxVal = arr[maxPos];

			int i, j, k, l;
			vector<ParamRange<int> > params;
			params.push_back(ParamRange<int>(&i, 0, size - 1, 1));
			params.push_back(ParamRange<int>(&j, 0, size - 1, 1));
			params.push_back(ParamRange<int>(&k, 0, size - 1, 1));
			params.push_back(ParamRange<int>(&l, 0, size - 1, 1));

			FunctionAlgorithm<int> evalAlgo(
				[&]()
				{
					return arr[((i * size + j) * size + k) * size + l];
				});

			Bruteforcer<decltype(evalAlgo), int> bf(evalAlgo, params);
			bf.run();

			Assert::AreEqual(maxVal, (int)bf.getBestScore());
			Assert::AreEqual(maxPos / (size * size * size), bf.getBestParams()[0]);
			Assert::AreEqual(((maxPos / (size * size)) % size), bf.getBestParams()[1]);
			Assert::AreEqual(((maxPos / size) % size), bf.getBestParams()[2]);
			Assert::AreEqual(maxPos % (size), bf.getBestParams()[3]);
		}
		TEST_METHOD(Bruteforcer_MemoizedFunction)
		{
#ifdef _DEBUG
			constexpr int maxVal = 4;
#else
			constexpr int maxVal = 15;
#endif

			int dummy1, dummy2;
			FunctionAlgorithm<int, int, int> memFuncAlgo(RecursiveMultipleArgsMemoizator<int, int, int>(
				[&](auto& recFunc, int x, int y)
				{
					if (x == 1)
						return y;
					return (x * y) + recFunc(1, y) - y;
				}));
			
			vector<ParamRange<int> > params;
			params.push_back(ParamRange<int>(&memFuncAlgo.getArg<0>(), 0, maxVal, 1));
			params.push_back(ParamRange<int>(&dummy1, 0, maxVal, 1));
			params.push_back(ParamRange<int>(&memFuncAlgo.getArg<1>(), 0, maxVal, 1));
			params.push_back(ParamRange<int>(&dummy2, 0, maxVal, 1));

			Bruteforcer<decltype(memFuncAlgo), int> bf(memFuncAlgo, params);
			bf.run();

			Assert::AreEqual(maxVal * maxVal, bf.getBestScore());
			Assert::AreEqual(maxVal, bf.getBestParams()[0]);
			Assert::AreEqual(maxVal, bf.getBestParams()[2]);
		}
		TEST_METHOD(Bruteforcer_ConditionalElementsPicking)
		{
			// But : maximiser la somme des éléments choisis dans le tableau ci-dessous,
			// sans jamais choisir deux éléments consécutifs.
#ifdef _DEBUG
			vector<int> arr = { 7, 3, 6, 2, 5, 9, 1, 3, 4, 9, 1, 3, 6, 2 };
			vector<int> sol = { 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0 };
#else
			vector<int> arr = { 7, 3, 6, 2, 5, 9, 1, 3, 4, 9, 1, 3, 6, 2, 8, 1, 3, 4, 6, 7 };
			vector<int> sol = { 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1 };
#endif

			auto eval = [&](vector<int> choices)
				{
					int sum = 0;
					for (size_t i = 0; i < arr.size(); i++)
					{
						if (choices[i])
						{
							if (i + 1 < arr.size() && choices[i + 1])
								return -1;
							sum += arr[i];
						}
					}
					return sum;
				};

			FunctionAlgorithm<int, vector<int> > evalAlgo(eval);
			get<0>(evalAlgo.getArgs()).resize(arr.size());

			vector<ParamRange<int> > params;
			for (size_t i = 0; i < arr.size(); i++)
				params.push_back(ParamRange<int>(&(get<0>(evalAlgo.getArgs())[i]), 0, 1, 1));

			Bruteforcer<decltype(evalAlgo), int> bf(evalAlgo, params);
			bf.run();

			Assert::AreEqual(eval(sol), bf.getBestScore());
			for (size_t i = 0; i < arr.size(); i++)
				Assert::AreEqual(sol[i], bf.getBestParams()[i]);
		}
	};

	template<class T>
	bool checkReverseTopologicalOrder(const Graph<>& g, const vector<T>& rto)
	{
		if (rto.size() != g.size())
			return false;

		// Vérifie que pour chaque noeud, le noeud cible de chacun de ses liens est plus proche que lui du début du vecteur
		for (auto it = rto.begin() + 1; it != rto.end(); ++it)
		{
			const auto& links = g[*it].getLinks();
			for (auto l = links.begin(); l != links.end(); ++l)
			{
				auto targetIt = find(rto.begin(), it, l->getTargetIndex());
				if (targetIt == it)
					return false;
			}
		}
		return true;
	}

	TEST_CLASS(GraphTests)
	{
		TEST_METHOD(GraphRemoveLink_NoCostCheck)
		{
			Graph<> g(5);
			g.addLink(0, 1, 2);
			g.addLink(0, 2, 5);
			g.addLink(3, 2, 8, true);
			g.addLink(2, 3, 10);
			g.addLink(1, 4, 4);
			g.addLink(4, 3, 24, true);

			g.removeLink(3, 2, true);
			Assert::AreEqual((size_t)2, g[2].getLinks().size());
			Assert::AreEqual((size_t)0, g[3].getLinks().size());

			g.removeLink(0, 4, false);
			Assert::AreEqual((size_t)2, g[4].getLinks().size());

			g.removeLink(4, 1, false);
			Assert::AreEqual((size_t)1, g[4].getLinks().size());
		}
		TEST_METHOD(GraphRemoveLink_WithCostCheck)
		{
			Graph<> g(5);
			g.addLink(0, 1, 2);
			g.addLink(0, 2, 5);
			g.addLink(3, 2, 8, true);
			g.addLink(2, 3, 10);
			g.addLink(1, 4, 4);
			g.addLink(4, 3, 24, true);

			g.removeLinkWithCostCheck(3, 2, 8, false);
			Assert::AreEqual((size_t)2, g[2].getLinks().size());
			Assert::AreEqual((size_t)1, g[3].getLinks().size());

			g.removeLinkWithCostCheck(0, 4, 4, false);
			Assert::AreEqual((size_t)2, g[4].getLinks().size());

			g.removeLinkWithCostCheck(1, 4, 4, true);
			Assert::AreEqual((size_t)2, g[4].getLinks().size());
		}
		TEST_METHOD(GraphSimpleTopologicalOrder_Algo2)
		{
			// Exemple tiré du cours d'Algorithmique 2 (Ensimag 1A) :
			Graph<> g(8);
			g.addLink(7, 6, 1, true);
			g.addLink(7, 5, 1, true);
			g.addLink(6, 3, 1, true);
			g.addLink(5, 3, 1, true);
			g.addLink(4, 2, 1, true);
			g.addLink(3, 1, 1, true);
			g.addLink(2, 1, 1, true);
			g.addLink(1, 0, 1, true);
			Assert::IsTrue(checkReverseTopologicalOrder(g, g.getReverseTopologicalyOrderedNodes()));
		}
		TEST_METHOD(GraphSimpleTopologicalOrder_RO)
		{
			// Exemple tiré du cours de Rercherche Opérationnelle (Ensimag 1A) :
			Graph<> g(10);
			g.addLink(0, 2, 1, true);
			g.addLink(0, 3, 1, true);
			g.addLink(1, 3, 1, true);
			g.addLink(2, 5, 1, true);
			g.addLink(2, 6, 1, true);
			g.addLink(3, 4, 1, true);
			g.addLink(3, 5, 1, true);
			g.addLink(3, 7, 1, true);
			g.addLink(5, 6, 1, true);
			g.addLink(5, 7, 1, true);

			// Ajoute deux noeuds supplémentaires non connectés au reste du graphe
			g.addLink(8, 9, -1, true);

			Assert::IsTrue(checkReverseTopologicalOrder(g, g.getReverseTopologicalyOrderedNodes()));
		}
		TEST_METHOD(GraphImpossibleTopologicalOrder)
		{
			// Exemple tiré du cours de Rercherche Opérationnelle (Ensimag 1A) :
			Graph<> g(6);
			g.addLink(0, 1, 2, true);
			g.addLink(0, 2, 8, true);
			g.addLink(1, 2, 5, true);
			g.addLink(1, 3, 1, true);
			g.addLink(2, 4, 1, true);
			g.addLink(3, 2, 2, true);
			g.addLink(3, 4, 4, true);
			g.addLink(3, 5, 1, true);
			g.addLink(4, 1, 3, true);
			g.addLink(4, 5, 1, true);
			Assert::IsTrue(g.getReverseTopologicalyOrderedNodes().empty());
		}
	};
}
