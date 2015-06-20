#include "CppUnitTest.h"
#include "ToStringSpecializations.cpp"
#include <cstdlib>
#include <ctime>

#include "List.c"
#include "cost_priority_queue.h"
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
			list_delete(l);
		}

	};

	TEST_CLASS(CostPriorityQueueTest)
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
