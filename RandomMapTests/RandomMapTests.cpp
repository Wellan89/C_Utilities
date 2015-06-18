#include "ShortestPathTestsUtils.h"

namespace TestUnit
{
	void RandomMapInit(unsigned int testRefsCount)
	{
		srand((unsigned int)time(NULL));

		randomMap = new ShortestPathTest(RANDOM_MAP_SIZE, testRefsCount);
		for (Graph::IndexNoeud i = 0; i < RANDOM_MAP_SIZE; i++)
		{
			unsigned int nbLinks = (unsigned int)rand() % RANDOM_MAP_MAX_NB_LINKS;
			for (unsigned int j = 0; j < nbLinks; j++)
			{
				Graph::IndexNoeud target = (Graph::IndexNoeud)((unsigned int)rand() % RANDOM_MAP_SIZE);
				Graph::Cout cost = (Graph::Cout)((unsigned int)rand() % RANDOM_MAP_MAX_COST);
				randomMap->g.addLink(i, target, cost, true);
			}
		}

		randomMap->startNode = (Graph::IndexNoeud)((unsigned int)rand() % RANDOM_MAP_SIZE);
		randomMap->closestFinalNode = (Graph::IndexNoeud)((unsigned int)rand() % RANDOM_MAP_SIZE);
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
			Dijkstra<> dj(randomMap->g);
			dj.computeShortestPathsFrom(randomMap->startNode);

			AStar<> as(randomMap->g);
			as.computeShortestPathFrom(randomMap->startNode);

			// Bellman-Ford-Yen est peu efficace en mémoire sur des graphes denses !
			//BellmanFordYen<> bfy(randomMap->g);
			//bfy.computeShortestPathsFrom(randomMap->startNode);

			// Floyd-Warshall est peu efficace en mémoire sur des grands graphes !
			//FloydWarshall<> fw(randomMap->g);
			//fw.computeShortestPaths();

			bool reachableNode = dj.canReachNode(randomMap->closestFinalNode);
			Assert::AreEqual(reachableNode, as.hasFoundPath());
			//Assert::AreEqual(reachableNode, bfy.canReachNode(randomMap->closestFinalNode));
			//Assert::AreEqual(reachableNode, fw.pathExists(randomMap->startNode, randomMap->closestFinalNode));
			if (reachableNode)
			{
				Assert::AreEqual(randomMap->closestFinalNode, as.getFinalNode());

				Graph::Cout cost = dj.getCostTo(randomMap->closestFinalNode);
				Assert::AreEqual(cost, as.getPathCost());
				//Assert::AreEqual(cost, bfy.getCostTo(randomMap->closestFinalNode));
				//Assert::AreEqual(cost, fw.getPathCost(randomMap->startNode, randomMap->closestFinalNode));

				deque<Graph::IndexNoeud> path = dj.getShortestPathTo(randomMap->closestFinalNode);
				Assert::AreEqual(path, as.getShortestPath());
				//Assert::AreEqual(path, bfy.getShortestPathTo(randomMap->closestFinalNode));
				//Assert::AreEqual(deque_to_vect(path), fw.getShortestPath(randomMap->startNode, randomMap->closestFinalNode));
			}

			CHECK_TEST_DELETE(randomMap);
		}
	};
}
