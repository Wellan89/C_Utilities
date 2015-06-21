#ifndef DEF_SHORTEST_PATH_TESTS_UTILS
#define DEF_SHORTEST_PATH_TESTS_UTILS

/*
TODO :
- Eliminer la récursivité dans DFS

- Créer une fonction pour chaque algorithme de calcul du chemin le plus court permettant d'obtenir la liste des liens empruntés

- Indiquer la complexité et les points forts/points faibles de chacun des algorithmes,
	et indiquer (et vérifier à l'exécution) les limites de ces algorithmes (graphes sans cycles, à coûts positifs...)

- Ecrire une fonction de conversion d'un graphe dynamique vers un graphe statique

- Dans les fonctions Graph::removeLinks, parcourir les liens de la fin vers le début, pour obtenir une meilleure complexité en moyenne

- Détecter les conditions d'application des algorithmes lorsqu'ils s'appliquent sur un graphe : reste encore DFS et FloydWarshall à traiter
- Dans DFS, vérifier les cycles négatifs (et permettre de ne pas le faire) + permettre de ne pas vérifier les cycles du tout (en enlevant visitedNodes)
	+ Dans Dijkstra, BFS et A*, permettre de savoir si un lien négatif a été détecté
	+ Dans Bellman, permettre de savoir si un cycle a été détecté
	+ idem pour les autres algos

- Tester la fonction de suppression de liens du graphe
- Tester les algos de plus court chemin avec des coûts très proches du coût "infini"

- Créer un test avec de nombreux liens au coût nul

- Déplacer toutes les fonctions des fichiers sources dépendant de templates dans les headers
*/



#ifdef _DEBUG
#define PATH_FINDERS_COMPUTE_LOOPS				800
#else
#define PATH_FINDERS_COMPUTE_LOOPS				500000
#endif



// ATTENTION : L'inclusion des .h ne marche pas ici : trouver pourquoi !
#include <cstdlib>
#include <ctime>
#include "CppUnitTest.h"
#include "ToStringSpecializations.cpp"
#include "Graph.h"
#include "DynamicGraph.h"
#include "Dijkstra.cpp"
#include "AStar.cpp"
#include "AStarDynamic.cpp"
#include "Bellman.cpp"
#include "DFS_ShortestPath.cpp"
#include "BFS_ShortestPath.cpp"
#include "BellmanFord.cpp"
#include "BellmanFordYen.cpp"
#include "FloydWarshall.cpp"

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

	template<class Cost = int, class NodeIndex = Graph<>::IndexNoeud>
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
			costs.resize(nodesNb, Graphe::INFINITE_COST());
			paths.resize(nodesNb);
		}
		void finishTest()
		{
			if (closestFinalNode < g.size())
				g.setNodeFinal(closestFinalNode);

			reversePaths.resize(paths.size());
			for (unsigned int i = 0; i < paths.size(); i++)
				reversePaths[i] = reverse_vect(paths[i]);
		}
	};
	typedef ShortestPathTest<>::Graphe Graphe;

	ShortestPathTest<> *simpleTest, *littleMaze,
		*negativeLinksGraph, *negativeCycleGraph, *negativeLoopGraph,
		*notSimpleGraph, *algo2Graph, *roGraph,
		*emptyGraph, *noFinalNodeGraph, *oneNodeGraph, *twoNodesGraph,	// Little graphs
		*bigMaze, *emptyMap, *randomMap, *fullGraph;

	void dropTest(ShortestPathTest<>*& test)
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

	void safeTestDelete(ShortestPathTest<>*& ptr)
	{
		if (ptr)
			delete ptr;
		ptr = NULL;
	}

	template<class PathFinder>
	void runSpfTest_AllNodes(ShortestPathTest<>* test, PathFinder spf)
	{
		if (test->startNode < test->g.size())
			for (unsigned int i = 0; i < test->nbComputeLoops; i++)
				spf.computeShortestPathsFrom(test->startNode);

		for (Graphe::IndexNoeud i = 0; i < test->g.size(); i++)
		{
			if (test->costs[i] != Graphe::INFINITE_COST())
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
				Assert::AreEqual(Graphe::INFINITE_COST(), spf.getCostTo(i), L"Finite cost.");
			}
		}
	}

	template<class PathFinder>
	void runSpfTest_ClosestFinalNode(ShortestPathTest<>* test, PathFinder spf)
	{
		if (test->startNode < test->g.size())
			for (unsigned int i = 0; i < test->nbComputeLoops; i++)
				spf.computeShortestPathFrom(test->startNode);

		Graphe::IndexNoeud i = test->closestFinalNode;
		if (test->costs[i] != Graphe::INFINITE_COST())
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
				Assert::AreEqual(Graphe::INFINITE_COST(), spf.getPathCost(), L"Finite cost.");
		}
	}

	template<class PathFinder>
	void runSpfTest_AllPairsOfNodes(ShortestPathTest<>* test, PathFinder spf)
	{
		if (test->startNode < test->g.size())
			for (unsigned int i = 0; i < test->nbComputeLoops; i++)
				spf.computeShortestPaths();

		Graphe::IndexNoeud j = test->startNode;
		for (Graphe::IndexNoeud i = 0; i < test->g.size(); i++)
		{
			if (test->costs[i] != Graphe::INFINITE_COST())
			{
				Assert::IsTrue(spf.pathExists(j, i), L"Path not found.");
				Assert::AreEqual(test->costs[i], spf.getPathCost(j, i), L"Different path cost.");
				if (test->paths[i].size() > 1)
					Assert::AreEqual(deque_to_vect(test->paths[i]), spf.getShortestPath(j, i), L"Different path.");
			}
			else if (test->infiniteCostCheck)
			{
				Assert::IsFalse(spf.pathExists(j, i), L"Path found.");
				Assert::AreEqual(Graphe::INFINITE_COST(), spf.getPathCost(j, i), L"Finite cost.");
			}
		}
	}

#define SHORTEST_PATH_TEST_METHOD_SEP(test, SPClass, run_func, sep)	\
	TEST_METHOD(##SPClass ##sep ##test) {							\
		if (!test)													\
			Assert::Fail(L"Invalid test references count !");		\
		SPClass<Graphe> sp(test->g);								\
		run_func(test, sp);											\
		dropTest(test);												\
	}

#define SHORTEST_PATH_TEST_METHOD(test, SPClass, run_func)	SHORTEST_PATH_TEST_METHOD_SEP(test, SPClass, run_func, _)
}

#endif
