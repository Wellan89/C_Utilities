#include "ShortestPathTestsUtils.h"

#ifdef _DEBUG
#define BIG_MAZE_SIZE							201		// Doit �tre impair
#else
#define BIG_MAZE_SIZE							3001	// Doit �tre impair
#endif

namespace TestUnit
{
	void BigMazeInit(unsigned int testRefsCount)
	{
#define ID(i, j)	(Graphe::IndexNoeud)((i) * width + (j))
		Graphe::IndexNoeud width = BIG_MAZE_SIZE;
		Graphe::IndexNoeud height = BIG_MAZE_SIZE;

		Graphe::Cout cost = (BIG_MAZE_SIZE / 2) + BIG_MAZE_SIZE - 4;
		deque<Graphe::IndexNoeud> path;
		for (Graphe::IndexNoeud i = (BIG_MAZE_SIZE / 2); i < BIG_MAZE_SIZE - 2; i++)
			path.push_back(ID(i, 1));
		for (Graphe::IndexNoeud j = 1; j < BIG_MAZE_SIZE - 1; j++)
			path.push_back(ID(BIG_MAZE_SIZE - 2, j));

		bigMaze = new ShortestPathTest<>(width * height, testRefsCount, 1);
		bigMaze->startNode = path[0];
		bigMaze->closestFinalNode = path[path.size() - 1];

		for (Graphe::IndexNoeud i = 1; i < height - 1; i++)
		{
			for (Graphe::IndexNoeud j = 1; j < width - 1; j++)
			{
				Graphe::IndexNoeud id = ID(i, j);
				if (j == 1 && i != height - 1)
					bigMaze->g.addLink(id, ID(i + 1, j), 1);
				if (i % 2 == 1 && j != width - 1)
					bigMaze->g.addLink(id, ID(i, j + 1), 1);
			}
		}
		for (Graphe::IndexNoeud i = 0; i < height; i++)
			for (Graphe::IndexNoeud j = 0; j < width; j++)
				bigMaze->g.setNodeHeuristic(ID(i, j),
					(Graphe::Cout)(abs((long)(bigMaze->closestFinalNode / width) - (long)i)
					+ abs((long)(bigMaze->closestFinalNode % width) - (long)j)));

		bigMaze->costs[bigMaze->closestFinalNode] = cost;
		bigMaze->paths[bigMaze->closestFinalNode] = path;
		bigMaze->finishTest();
#undef ID
	}

	TEST_MODULE_INITIALIZE(BigMazeTestsInit)
	{
		BigMazeInit(5);
	}
	TEST_MODULE_CLEANUP(BigMazeTestsCleanup)
	{
		SAFE_TEST_DELETE(bigMaze);
	}

	TEST_CLASS(DijkstraTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(bigMaze, Dijkstra, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
	};

	TEST_CLASS(AStarTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(bigMaze, AStar, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
	};

	TEST_CLASS(DFS_Tests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(bigMaze, DFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE);
	};

	TEST_CLASS(BFS_Tests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(bigMaze, BFS_ShortestPath, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
	};

	TEST_CLASS(BellmanFordTests)
	{
	public:
		// Bellman-Ford est extr�mement lent pour les gros tests !
		//SHORTEST_PATH_TEST_METHOD(bigMaze, BellmanFord, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
	};

	TEST_CLASS(BellmanFordYenTests)
	{
	public:
		SHORTEST_PATH_TEST_METHOD(bigMaze, BellmanFordYen, RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES);
	};

	TEST_CLASS(FloydWarshallTests)
	{
		// Floyd-Warshall est extr�mement lent pour les gros tests !
		//SHORTEST_PATH_TEST_METHOD(bigMaze, FloydWarshall, RUN_SHORTEST_PATH_FINDER_TEST_ALL_PAIRS_OF_NODES);
	};
}
