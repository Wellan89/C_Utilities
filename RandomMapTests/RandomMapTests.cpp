#include "ShortestPathTestsUtils.h"

#ifdef _DEBUG

#define RANDOM_MAP_SIZE							151
#define RANDOM_MAP_MAX_NB_LINKS					15
#define RANDOM_MAP_MAX_COST						1000000

#else

#define RANDOM_MAP_SIZE							20000
#define RANDOM_MAP_MAX_NB_LINKS					5000	// Graphe dense
#define RANDOM_MAP_MAX_COST						1000000

#endif

namespace TestUnit
{
	void RandomMapInit(unsigned int testRefsCount)
	{
		srand((unsigned int)time(NULL));

		randomMap = new ShortestPathTest<>(RANDOM_MAP_SIZE, testRefsCount);
		for (Graphe::IndexNoeud i = 0; i < RANDOM_MAP_SIZE; i++)
		{
			unsigned int nbLinks = (unsigned int)rand() % RANDOM_MAP_MAX_NB_LINKS;
			for (unsigned int j = 0; j < nbLinks; j++)
			{
				Graphe::IndexNoeud target = (Graphe::IndexNoeud)((unsigned int)rand() % RANDOM_MAP_SIZE);
				Graphe::Cout cost = (Graphe::Cout)((unsigned int)rand() % RANDOM_MAP_MAX_COST);
				randomMap->g.addLink(i, target, cost, true);
			}
		}

		randomMap->startNode = (Graphe::IndexNoeud)((unsigned int)rand() % RANDOM_MAP_SIZE);
		randomMap->closestFinalNode = (Graphe::IndexNoeud)((unsigned int)rand() % RANDOM_MAP_SIZE);
		randomMap->finishTest();
	}

	TEST_MODULE_INITIALIZE(RandomMapTestsInit)
	{
		RandomMapInit(1);
	}
	TEST_MODULE_CLEANUP(RandomMapTestsCleanup)
	{
		if (randomMap)
			delete randomMap;
	}

	TEST_CLASS(RandomPaths)
	{
		TEST_METHOD(RandomGraph)
		{
			typedef ShortestPathTest<>::Graphe Graphe;

			Dijkstra<Graphe> dj(randomMap->g);
			dj.computeShortestPathsFrom(randomMap->startNode);

			AStar<Graphe> as(randomMap->g);
			as.computeShortestPathFrom(randomMap->startNode);

			// Bellman-Ford-Yen est peu efficace en mémoire sur des graphes denses !
			//BellmanFordYen<Graphe> bfy(randomMap->g);
			//bfy.computeShortestPathsFrom(randomMap->startNode);

			// Floyd-Warshall est peu efficace en mémoire sur des grands graphes !
			//FloydWarshall<Graphe> fw(randomMap->g);
			//fw.computeShortestPaths();

			bool reachableNode = dj.canReachNode(randomMap->closestFinalNode);
			Assert::AreEqual(reachableNode, as.hasFoundPath());
			//Assert::AreEqual(reachableNode, bfy.canReachNode(randomMap->closestFinalNode));
			//Assert::AreEqual(reachableNode, fw.pathExists(randomMap->startNode, randomMap->closestFinalNode));
			if (reachableNode)
			{
				Assert::AreEqual(randomMap->closestFinalNode, as.getFinalNode());

				Graphe::Cout cost = dj.getCostTo(randomMap->closestFinalNode);
				Assert::AreEqual(cost, as.getPathCost());
				//Assert::AreEqual(cost, bfy.getCostTo(randomMap->closestFinalNode));
				//Assert::AreEqual(cost, fw.getPathCost(randomMap->startNode, randomMap->closestFinalNode));

				deque<Graphe::IndexNoeud> path = dj.getShortestPathTo(randomMap->closestFinalNode);
				Assert::AreEqual(path, as.getShortestPath());
				//Assert::AreEqual(path, bfy.getShortestPathTo(randomMap->closestFinalNode));
				//Assert::AreEqual(deque_to_vect(path), fw.getShortestPath(randomMap->startNode, randomMap->closestFinalNode));
			}

			CHECK_TEST_DELETE(randomMap);
		}
	};
}
