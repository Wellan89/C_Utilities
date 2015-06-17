#include "ShortestPathTestsUtils.h"

namespace TestUnit
{
	void RandomMapInit()
	{
		srand((unsigned int)time(NULL));

		randomMap = new ShortestPathTest(RANDOM_MAP_SIZE);
		for (unsigned int i = 0; i < RANDOM_MAP_SIZE; i++)
		{
			unsigned int nbLinks = (unsigned int)rand() % RANDOM_MAP_MAX_NB_LINKS;
			for (unsigned int j = 0; j < nbLinks; j++)
			{
				unsigned int target = (unsigned int)rand() % RANDOM_MAP_SIZE;
				unsigned int cost = (unsigned int)rand() % RANDOM_MAP_MAX_COST;
				randomMap->g.addLink(i, target, cost, true);
			}
		}

		randomMap->startNode = rand() % RANDOM_MAP_SIZE;
		randomMap->closestFinalNode = rand() % RANDOM_MAP_SIZE;
		randomMap->finishTest();
	}

	TEST_MODULE_INITIALIZE(RandomMapTestsInit)
	{
		RandomMapInit();
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

			bool reachableNode = dj.canReachNode(randomMap->closestFinalNode);

			Assert::AreEqual(reachableNode, as.hasFoundPath());
			//Assert::AreEqual(reachableNode, bfy.canReachNode(randomMap->closestFinalNode));
			if (reachableNode)
			{
				unsigned int cost = dj.getCostTo(randomMap->closestFinalNode);
				deque<unsigned int> path = dj.getShortestPathTo(randomMap->closestFinalNode);

				Assert::AreEqual(randomMap->closestFinalNode, as.getFinalNode());
				Assert::AreEqual(cost, as.getPathCost());
				//Assert::AreEqual(cost, bfy.getCostTo(randomMap->closestFinalNode));
				Assert::AreEqual(path, as.getShortestPath());
				//Assert::AreEqual(path, bfy.getShortestPathTo(randomMap->closestFinalNode));
			}
		}
	};
}
