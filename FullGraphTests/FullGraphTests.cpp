#include "ShortestPathTestsUtils.h"

#ifdef _DEBUG
#define FULL_GRAPH_COMPUTE_LOOPS				1
#define FULL_GRAPH_NODES_COUNT					350
#else
#define FULL_GRAPH_COMPUTE_LOOPS				2
#define FULL_GRAPH_NODES_COUNT					5000
#endif

namespace TestUnit
{
	void FullGraphInit(unsigned int testRefsCount)
	{
		fullGraph = new ShortestPathTest<>(FULL_GRAPH_NODES_COUNT + 1, testRefsCount, FULL_GRAPH_COMPUTE_LOOPS);

		fullGraph->startNode = 0;
		fullGraph->closestFinalNode = FULL_GRAPH_NODES_COUNT;

		for (Graphe::IndexNoeud i = 1; i < FULL_GRAPH_NODES_COUNT; i++)
		{
			for (Graphe::IndexNoeud j = 0; j < i; j++)
				fullGraph->g.addLink(i, j, 1);

			fullGraph->g.addLink(i, FULL_GRAPH_NODES_COUNT, 1);

			fullGraph->costs[i] = 1;
		}

		fullGraph->costs[FULL_GRAPH_NODES_COUNT] = 2;
		fullGraph->finishTest();
	}

	TEST_MODULE_INITIALIZE(FullGraphTestsInit)
	{
		FullGraphInit(6);
	}
	TEST_MODULE_CLEANUP(FullGraphTestsCleanup)
	{
		SAFE_TEST_DELETE(fullGraph);
	}

	TEST_CLASS(DijkstraTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(fullGraph, Dijkstra, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
	};

	TEST_CLASS(AStarTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(fullGraph, AStar, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
	};

	TEST_CLASS(DFS_Tests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(fullGraph, DFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
	};

	TEST_CLASS(BFS_Tests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(fullGraph, BFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
	};

	TEST_CLASS(BellmanFordTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(fullGraph, BellmanFord, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
	};

	TEST_CLASS(BellmanFordYenTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(fullGraph, BellmanFordYen, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
	};

	TEST_CLASS(FloydWarshallTests)
	{
		// Floyd-Warshall est extrêmement lent pour les gros tests !
		//SHORTEST_PATH_TEST_METHOD(fullGraph, FloydWarshall, RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES);
	};
}
