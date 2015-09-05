#ifndef DEF_SHORTEST_PATH_TESTS_UTILS
#define DEF_SHORTEST_PATH_TESTS_UTILS

/*
TODO :
- BFS est-il plus rapide que Dijkstra pour les graphes dont les liens ont tous le même coût ?

- Séparer les tests de plus court chemin et les autres tests de TestUnit dans deux projets différents

- Créer des tests avec un graphe aux coûts flottants :
	Coûts très importants, coûts négatifs, test basique, cycles négatifs

- Créer une classe arbre héritant de Graphe : un graphe orienté sans cycles, chaque noeud ayant un lien vers son parent

- Dans Floyd-Warshall, détecter les circuits absorbants

- Créer une fonction pour chaque algorithme de calcul du chemin le plus court permettant d'obtenir la liste des liens empruntés

- Indiquer la complexité et les points forts/points faibles de chacun des algorithmes,
	et indiquer (et vérifier à l'exécution) les limites de ces algorithmes (graphes sans cycles, à coûts positifs...)

- Implémenter les sémantiques move de C++ ?

- Ecrire une fonction de conversion d'un graphe dynamique vers un graphe statique ?

- Eliminer la récursivité dans DFS ?

- Essayer de paralléliser Bruteforcer avec OpenMP ?
	-> Impossible actuellement car chaque boucle change les mêmes paramètres en mémoire
*/



// Si défini, les tests les plus lents seront aussi activés
// Ces tests ont un temps d'exécution de l'ordre de la minute.
//#define ENABLE_SLOW_TESTS

#ifdef _DEBUG
#define PATH_FINDERS_COMPUTE_LOOPS				170
#else
#define PATH_FINDERS_COMPUTE_LOOPS				100000
#endif



#include <cstdlib>
#include <ctime>
#include "CppUnitTest.h"
#include "ToStringSpecializations.h"
#include "Graph.h"
#include "DynamicGraph.h"
#include "Dijkstra.h"
#include "AStar.h"
#include "AStarDynamic.h"
#include "Bellman.h"
#include "DFS_ShortestPath.h"
#include "BFS_ShortestPath.h"
#include "BellmanFord.h"
#include "BellmanFordYen.h"
#include "FloydWarshall.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestUnit
{
	template<class T>
	vector<T> reverse_vect(const deque<T>& d)
	{
		vector<T> v;
		v.reserve(d.size());
		for (auto it = d.rbegin(); it != d.rend(); ++it)
			v.push_back(*it);
		return v;
	}
	template<class T>
	vector<T> deque_to_vect(const deque<T>& d)
	{
		vector<T> v;
		v.reserve(d.size());
		for (auto it = d.begin(); it != d.end(); ++it)
			v.push_back(*it);
		return v;
	}

	template<class Cost = int, class NodeIndex = unsigned int>
	struct ShortestPathTest
	{
		typedef Cost Cout;
		typedef NodeIndex IndexNoeud;
		typedef Graph<Cout, IndexNoeud> Graphe;

		unsigned int nbComputeLoops;
		unsigned int testsRefsCount;
		bool infiniteCostCheck;

		Graphe g;
		IndexNoeud startNode;
		IndexNoeud closestFinalNode;

		vector<Cout> costs;
		vector<deque<IndexNoeud> > paths;

		vector<vector<IndexNoeud> > reversePaths;

		ShortestPathTest(IndexNoeud nodesNb, unsigned int refsCount, unsigned int nbLoops = PATH_FINDERS_COMPUTE_LOOPS)
			: g(nodesNb), testsRefsCount(refsCount), nbComputeLoops(nbLoops),
				infiniteCostCheck(false), startNode(0), closestFinalNode(0)
		{
			costs.resize(nodesNb, Graphe::INFINITE_COST);
			paths.resize(nodesNb);
		}
		void finishTest()
		{
			if (closestFinalNode < g.size())
				g.setNodeFinal(closestFinalNode);

			reversePaths.resize(paths.size());
			for (size_t i = 0; i < paths.size(); i++)
				reversePaths[i] = reverse_vect(paths[i]);
		}
	};
	typedef ShortestPathTest<>::Graphe Graphe;

	ShortestPathTest<> *simpleTest, *littleMaze,
		*negativeLinksGraph, *negativeCycleGraph, *negativeLoopGraph,
		*notSimpleGraph, *algo2Graph, *roGraph, *manyNullCostLinks,
		*emptyGraph, *noFinalNodeGraph, *oneNodeGraph, *twoNodesGraph,	// Little graphs
		*bigMaze, *emptyMap, *randomMap, *fullGraph;
	ShortestPathTest<signed char, signed char> *bigCostPaths;

	template<class SPTest>
	void dropTest(SPTest*& test)
	{
		if (test && test->testsRefsCount > 0)
		{
			test->testsRefsCount--;
			if (test->testsRefsCount == 0)
			{
				delete test;
				test = NULL;
			}
		}
	}

	template<class SPTest>
	void safeTestDelete(SPTest*& ptr)
	{
		if (ptr)
			delete ptr;
		ptr = NULL;
	}

	template<class SPTest, class PathFinder>
	void runSpfTest_AllNodes(SPTest* test, PathFinder spf)
	{
		if (test->startNode < test->g.size())
			for (unsigned int i = 0; i < test->nbComputeLoops; i++)
				spf.computeShortestPathsFrom(test->startNode);

		for (SPTest::Graphe::IndexNoeud i = 0; i < test->g.size(); i++)
		{
			if (test->costs[i] != SPTest::Graphe::INFINITE_COST)
			{
				Assert::IsTrue(spf.canReachNode(i), L"Path not found.");
				Assert::AreEqual(test->costs[i], spf.getCostTo(i), L"Different path cost.");
				if (test->paths[i].size() > 1)
				{
					Assert::AreEqual(test->paths[i], spf.getShortestPathTo(i), L"Different path.");
					Assert::AreEqual(test->reversePaths[i], spf.getReverseShortestPathTo(i), L"Different reverse path.");
				}
			}
			else if (test->infiniteCostCheck)
			{
				Assert::IsFalse(spf.canReachNode(i), L"Path found.");
				Assert::AreEqual(SPTest::Graphe::INFINITE_COST, spf.getCostTo(i), L"Finite cost.");
			}
		}
	}

	template<class SPTest, class PathFinder>
	void runSpfTest_ClosestFinalNode(SPTest* test, PathFinder spf)
	{
		if (test->startNode < test->g.size())
			for (unsigned int i = 0; i < test->nbComputeLoops; i++)
				spf.computeShortestPathFrom(test->startNode);

		SPTest::Graphe::IndexNoeud i = test->closestFinalNode;
		if (test->costs[i] != SPTest::Graphe::INFINITE_COST)
		{
			Assert::IsTrue(spf.hasFoundPath(), L"Path not found.");
			Assert::AreEqual(test->costs[i], spf.getPathCost(), L"Different path cost.");
			if (test->paths[i].size() > 1)
			{
				Assert::AreEqual(test->paths[i], spf.getShortestPath(), L"Different path.");
				Assert::AreEqual(test->reversePaths[i], spf.getReverseShortestPath(), L"Different reverse path.");
			}
		}
		else if (test->infiniteCostCheck)
		{
			Assert::IsFalse(spf.hasFoundPath(), L"Path found.");
			Assert::AreEqual(SPTest::Graphe::INFINITE_COST, spf.getPathCost(), L"Finite cost.");
		}
	}

	template<class SPTest, class PathFinder>
	void runSpfTest_AllPairsOfNodes(SPTest* test, PathFinder spf)
	{
		if (test->startNode < test->g.size())
			for (unsigned int i = 0; i < test->nbComputeLoops; i++)
				spf.computeShortestPaths();

		SPTest::Graphe::IndexNoeud j = test->startNode;
		for (SPTest::Graphe::IndexNoeud i = 0; i < test->g.size(); i++)
		{
			if (test->costs[i] != SPTest::Graphe::INFINITE_COST)
			{
				Assert::IsTrue(spf.pathExists(j, i), L"Path not found.");
				Assert::AreEqual(test->costs[i], spf.getPathCost(j, i), L"Different path cost.");
				if (test->paths[i].size() > 1)
					Assert::AreEqual(deque_to_vect(test->paths[i]), spf.getShortestPath(j, i), L"Different path.");
			}
			else if (test->infiniteCostCheck)
			{
				Assert::IsFalse(spf.pathExists(j, i), L"Path found.");
				Assert::AreEqual(SPTest::Graphe::INFINITE_COST, spf.getPathCost(j, i), L"Finite cost.");
			}
		}
	}

#define SHORTEST_PATH_TEST_METHOD_SEP(test, SPClass, run_func, sep)	\
	TEST_METHOD(SPClass ## sep ## test) {							\
		if (!test)													\
			Assert::Fail(L"Invalid test references count !");		\
		SPClass<decltype(test->g)> sp(test->g);						\
		run_func(test, sp);											\
		dropTest(test);												\
	}

#define SHORTEST_PATH_TEST_METHOD(test, SPClass, run_func)	SHORTEST_PATH_TEST_METHOD_SEP(test, SPClass, run_func, _)
}

#endif
