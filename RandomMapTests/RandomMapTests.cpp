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

			Assert::AreEqual(dj.canReachNode(randomMap->closestFinalNode), as.hasFoundPath());
			if (as.hasFoundPath())
			{
				Assert::AreEqual(randomMap->closestFinalNode, as.getFinalNode());
				Assert::AreEqual(dj.getCostTo(randomMap->closestFinalNode), as.getPathCost());
				Assert::AreEqual(dj.getShortestPathTo(randomMap->closestFinalNode), as.getShortestPath());
			}
		}
	};
}
