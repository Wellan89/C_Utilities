#include "CppUnitTest.h"
#include "ToStringSpecializations.cpp"
#include <cstdlib>
#include <ctime>

#include "List.c"
#include "cost_priority_queue.h"
#include "ModulusNumber.h"
#include "Memoizator.h"
#include "Graph.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace std
{
	template<> class hash<pair<int, float>>
	{
	public:
		size_t operator()(const pair<int, float>& p) const
		{
			return (3 * hash<int>()(p.first) + hash<float>()(p.second));
		}
	};
}

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

		TEST_METHOD(Memoizator_SimpleFunction)
		{
			Memoizator<decltype(simpleFunc), int> simpleFunc_mem(simpleFunc);
			for (int i = -5; i <= 5; i++)
				Assert::AreEqual(simpleFunc(abs(i)), simpleFunc_mem(abs(i)));
		}
		TEST_METHOD(Memoizator_ClassFunction)
		{
			ClassFunc classFunc(10);
			Memoizator<ClassFunc, int> classFunc_mem(classFunc);
			for (int i = -5; i <= 5; i++)
				Assert::AreEqual(classFunc(abs(i)), classFunc_mem(abs(i)));
		}
		TEST_METHOD(Memoizator_LambdaFunction)
		{
			auto lambda = [](int x) { return x * x; };
			Memoizator<decltype(lambda), int> lambda_mem(lambda);
			for (int i = -5; i <= 5; i++)
				Assert::AreEqual(lambda(abs(i)), lambda_mem(abs(i)));
		}

		TEST_METHOD(Memoizator_ComplexArgs)
		{
			auto f = [](pair<int, float> p) { return p.second * (float)p.first; };
			Memoizator<decltype(f), pair<int, float>> f_mem(f);

			for (int i = -5; i <= 5; i++)
			{
				for (float j = -5.0f; j < 5.5f ; j += 1.0f)
				{
					auto arg = make_pair(abs(i), abs(j));
					Assert::AreEqual(f(arg), f_mem(arg));
				}
			}
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
