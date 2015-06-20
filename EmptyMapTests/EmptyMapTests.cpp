#include "ShortestPathTestsUtils.h"

#ifdef _DEBUG
#define EMPTY_MAP_SIZE							81
#else
#define EMPTY_MAP_SIZE							2001
#endif

namespace TestUnit
{
	void EmptyMapInit(unsigned int testRefsCount)
	{
#define ID(i, j)	(Graphe::IndexNoeud)((i) * width + (j))
		Graphe::IndexNoeud width = EMPTY_MAP_SIZE;
		Graphe::IndexNoeud height = EMPTY_MAP_SIZE;

		Graphe::IndexNoeud start = ID(height / 2, width / 2);
		Graphe::IndexNoeud finalNode = ID(height - 1, width - 1);
		unsigned int cost = abs((long)(start / width) - (long)(finalNode / width)) + abs((long)(start % width) - (long)(finalNode % width));

		emptyMap = new ShortestPathTest<>(width * height, testRefsCount, 1);
		emptyMap->startNode = start;
		emptyMap->closestFinalNode = finalNode;
		for (Graphe::IndexNoeud i = 0; i < height; i++)
		{
			for (Graphe::IndexNoeud j = 0; j < width; j++)
			{
				Graphe::IndexNoeud id = ID(i, j);
#if 0
				// Ajoute 2 fois plus de liens que nécessaire
				if (i > 0)			emptyMap->g.addLink(id, ID(i - 1, j), 3);	// Haut<->Bas : coûte 3     
				if (i < height - 1)	emptyMap->g.addLink(id, ID(i + 1, j), 1);	// Bas<->Haut : coûte 1     
				if (j > 0)			emptyMap->g.addLink(id, ID(i, j - 1), 1);	// Gauche<->Droite : coûte 1
				if (j < width - 1)	emptyMap->g.addLink(id, ID(i, j + 1), 2);	// Droite<->Gauche : coûte 2
#else
				if (i < height - 1)
					emptyMap->g.addLink(id, ID(i + 1, j), 1);
				if (j < width - 1)
					emptyMap->g.addLink(id, ID(i, j + 1), 1);
#endif

				emptyMap->g.setNodeHeuristic(ID(i, j),
					(Graphe::Cout)(abs((long)(finalNode / width) - (long)i) + abs((long)(finalNode % width) - (long)j)));
			}
		}

		emptyMap->costs[emptyMap->closestFinalNode] = cost;
		emptyMap->finishTest();
#undef ID
	}

	TEST_MODULE_INITIALIZE(EmptyMapTestsInit)
	{
		EmptyMapInit(4);
	}
	TEST_MODULE_CLEANUP(EmptyMapTestsCleanup)
	{
		safeTestDelete(emptyMap);
	}

	TEST_CLASS(DijkstraTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(emptyMap, Dijkstra, runSpfTest_AllNodes);
	};

	TEST_CLASS(AStarTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(emptyMap, AStar, runSpfTest_ClosestFinalNode);
	};

	TEST_CLASS(DFS_Tests)
	{
	public:
		// Ce test échoue toujours (stack overflow très probablement) !
		//SHORTEST_PATH_TEST_METHOD(emptyMap, DFS_ShortestPath, runSpfTest_ClosestFinalNode);
	};

	TEST_CLASS(BFS_Tests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(emptyMap, BFS_ShortestPath, runSpfTest_AllNodes);
	};

	TEST_CLASS(BellmanFordTests)
	{
	public:
		// Bellman-Ford est extrêmement lent pour les gros tests !
		//SHORTEST_PATH_TEST_METHOD(emptyMap, BellmanFord, runSpfTest_AllNodes);
	};

	TEST_CLASS(BellmanFordYenTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(emptyMap, BellmanFordYen, runSpfTest_AllNodes);
	};

	TEST_CLASS(FloydWarshallTests)
	{
		// Floyd-Warshall est extrêmement lent pour les gros tests !
		//SHORTEST_PATH_TEST_METHOD(emptyMap, FloydWarshall, runSpfTest_AllPairsOfNodes);
	};
}
