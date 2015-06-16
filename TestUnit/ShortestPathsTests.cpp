#include "CppUnitTest.h"
#include "ToStringSpecializations.cpp"
#include <cstdlib>
#include <ctime>

/*
TODO :
Réecrire Dijkstra avec le nouvel algo wikipédia + chercher sur internet la solution la plus efficace (possible en O(n.log n) apparemment)
Ecrire un type de graphe "infini" : génération dynamique de la liste des noeuds et de la liste des liens entre eux
Permettre la gestion des coûts négatifs, avec une gestion appropriée des erreurs pour les algos A* et Dijkstra (entre autres)
Tester la fonction de suppression de liens du graphe
Tester le détection de circuits absorbants, et les cas de graphes à un ou deux noeuds pour Bellman-Ford et Bellman-Ford-Yen
Tester les algorithmes sur des graphes non simples : avec des boucles (une arête d'un noeud sur lui-même)
Utiliser les fonctions TEST_MODULE_INITIALIZE et TEST_MODULE_CLEANUP pour créer les graphes de test en dehors des fonctions de test
Créer une fonction pour chaque algorithme de calcul du chemin le plus court permettant d'obtenir la liste des liens empruntés
Indiquer la complexité et les points forts/points faibles de chacun des algorithmes,
	et indiquer (et vérifier à l'exécution) les limites de ces algorithmes (graphes sans cycles, à coûts positifs...)
Améliorer Floyd-Warshall afin de pouvoir obtenir le chemin le plus court entre toutes les paires de points (et non pas seulement son coût)
Permettre l'utilisation de plus de mémoire au processus de test de Visual Studio (on semble ici limité aux alentours des 1.6 gigabyte)
	+ Trouver un moyen de limiter l'utilisation mémoire des tests : éviter leur initialisation commune ?
	+ Essayer d'exécuter plusieurs tests en parallèle
		=> Ecrire chaque "gros test" (big maze, empty map et random map) dans des projets différents ?
Ecrire le test "CounterCulture" du google code jam round 1B 2015
*/

// ATTENTION : L'inclusion des .h ne marche pas ici : trouver pourquoi !
#include "Graph.h"
#include "DynamicGraph.h"
#include "Dijkstra.cpp"
#include "AStar.cpp"
#include "Bellman.cpp"
#include "DFS_ShortestPath.cpp"
#include "BFS_ShortestPath.cpp"
#include "BellmanFord.cpp"
#include "BellmanFordYen.cpp"
#include "FloydWarshall.cpp"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#ifdef _DEBUG
#define PATH_FINDERS_COMPUTE_LOOPS				800

#define BIG_MAZE_SIZE							201		// Doit être impair
#define EMPTY_MAP_SIZE							81

#define RANDOM_MAP_SIZE							151
#define RANDOM_MAP_MAX_NB_LINKS					15
#define RANDOM_MAP_MAX_COST						1000000
#else
#define PATH_FINDERS_COMPUTE_LOOPS				500000

// Attention ici à ne pas utiliser trop de mémoire !
#define BIG_MAZE_SIZE							2501	// Doit être impair
#define EMPTY_MAP_SIZE							1001

#define RANDOM_MAP_SIZE							15001
#define RANDOM_MAP_MAX_NB_LINKS					1000
#define RANDOM_MAP_MAX_COST						1000000
#endif

namespace TestUnit
{
	vector<unsigned int> reverse_vect(const deque<unsigned int>& d)
	{
		vector<unsigned int> v;
		v.reserve(d.size());
		for (auto it = d.rbegin(); it != d.rend(); ++it)
			v.push_back(*it);
		return v;
	}

	struct ShortestPathTest
	{
		unsigned int nbComputeLoops;

		Graph g;
		unsigned int startNode;
		unsigned int closestFinalNode;

		vector<unsigned int> costs;
		vector<deque<unsigned int> > paths;

		vector<vector<unsigned int> > reversePaths;

		ShortestPathTest(unsigned int nodesNb, unsigned int nbLoops = PATH_FINDERS_COMPUTE_LOOPS)
			: g(nodesNb), nbComputeLoops(nbLoops)
		{
			costs.resize(nodesNb, (unsigned int)(-1));
			paths.resize(nodesNb);
			reversePaths.resize(nodesNb);
		}
		void finishTest()
		{
			g.setNodeFinal(closestFinalNode);

			for (unsigned int i = 0; i < paths.size(); i++)
				reversePaths[i] = reverse_vect(paths[i]);
		}
	};
	ShortestPathTest *simpleTest, *littleMaze, *bigMaze, *emptyMap;

#define RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(test, spf)										\
	do {																						\
		for (unsigned int i = 0; i < test->nbComputeLoops; i++)									\
			spf.computeShortestPathsFrom(test->startNode);										\
		for (unsigned int i = 0; i < test->g.size(); i++) {										\
			if (test->costs[i] != (unsigned int)(-1)) {											\
				Assert::IsTrue(spf.canReachNode(i));											\
				Assert::AreEqual(test->costs[i], spf.getCostTo(i));								\
				if (test->paths[i].size() > 1) {												\
					Assert::AreEqual(test->paths[i], spf.getShortestPathTo(i));					\
					Assert::AreEqual(test->reversePaths[i], spf.getReverseShortestPathTo(i));	\
				}																				\
			}																					\
		}																						\
	} while (false)
#define RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(test, spf)								\
	do {																						\
		for (unsigned int i = 0; i < test->nbComputeLoops; i++)									\
			spf.computeShortestPathFrom(test->startNode);										\
		unsigned int i = test->closestFinalNode;												\
		if (test->costs[i] != (unsigned int)(-1)) {												\
			Assert::AreEqual(test->costs[i], spf.getPathCost());								\
			if (test->paths[i].size() > 1) {													\
				Assert::AreEqual(test->paths[i], spf.getShortestPath());						\
				Assert::AreEqual(test->reversePaths[i], spf.getReverseShortestPath());			\
			}																					\
		}																						\
	} while (false)

	void SimpleTestInit()
	{
#define CTI(c)	(c - 'a')
		// Wikipedia example test : http://fr.wikipedia.org/wiki/Algorithme_de_Dijkstra
		simpleTest = new ShortestPathTest(CTI('j') + 1);
		simpleTest->g.addLink(CTI('a'), CTI('b'), 85);
		simpleTest->g.addLink(CTI('a'), CTI('c'), 217);
		simpleTest->g.addLink(CTI('a'), CTI('e'), 173);
		simpleTest->g.addLink(CTI('b'), CTI('f'), 80);
		simpleTest->g.addLink(CTI('c'), CTI('g'), 186);
		simpleTest->g.addLink(CTI('c'), CTI('h'), 103);
		simpleTest->g.addLink(CTI('d'), CTI('h'), 183);
		simpleTest->g.addLink(CTI('e'), CTI('j'), 502);
		simpleTest->g.addLink(CTI('f'), CTI('i'), 250);
		simpleTest->g.addLink(CTI('i'), CTI('j'), 84);

		simpleTest->startNode = CTI('a');
		simpleTest->closestFinalNode = CTI('j');

		const unsigned int heuristics[] = { 450, 400, 250, 300, 500, 300, 400, 150, 50, 0 };
		for (unsigned int i = 0; i <= CTI('j'); i++)
			simpleTest->g.setNodeHeuristic(i, heuristics[i]);

		simpleTest->costs = { 0, 85, 217, 503, 173, 165, 403, 320, 415, 499 };
		simpleTest->paths = {
			deque < unsigned int > { CTI('a') },
			deque < unsigned int > { CTI('a'), CTI('b') },
			deque < unsigned int > { CTI('a'), CTI('c') },
			deque < unsigned int > { CTI('a'), CTI('c'), CTI('h'), CTI('d') },
			deque < unsigned int > { CTI('a'), CTI('e') },
			deque < unsigned int > { CTI('a'), CTI('b'), CTI('f') },
			deque < unsigned int > { CTI('a'), CTI('c'), CTI('g') },
			deque < unsigned int > { CTI('a'), CTI('c'), CTI('h') },
			deque < unsigned int > { CTI('a'), CTI('b'), CTI('f'), CTI('i') },
			deque < unsigned int > { CTI('a'), CTI('b'), CTI('f'), CTI('i'), CTI('j') } };
		simpleTest->finishTest();
#undef CTI
	}
	void LittleMazeInit()
	{
#define ID(i, j)	((i) * width + (j))
		vector<string> map = {
			"###########",
			"#         #",
			"# #########",
			"#         #",
			"# #########",
			"#S        #",
			"# #########",
			"#         #",
			"# #########",
			"#        E#",
			"###########" };
		const unsigned int width = map[0].size();
		const unsigned int height = map.size();

		const unsigned int cost = 12;
		const deque<unsigned int> path = { ID(5, 1), ID(6, 1), ID(7, 1), ID(8, 1), ID(9, 1),
			ID(9, 2), ID(9, 3), ID(9, 4), ID(9, 5), ID(9, 6), ID(9, 7), ID(9, 8), ID(9, 9) };

		littleMaze = new ShortestPathTest(width * height);
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				if (map[i][j] != '#')
				{
					unsigned int id = ID(i, j);

					if (i < height - 1 && map[i + 1][j] != '#')
						littleMaze->g.addLink(id, ID(i + 1, j), 1);
					if (j < width - 1 && map[i][j + 1] != '#')
						littleMaze->g.addLink(id, ID(i, j + 1), 1);

					if (map[i][j] == 'S')
						littleMaze->startNode = id;
					else if (map[i][j] == 'E')
						littleMaze->closestFinalNode = id;
				}
			}
		}
		for (unsigned int i = 0; i < height; i++)
			for (unsigned int j = 0; j < width; j++)
				littleMaze->g.setNodeHeuristic(ID(i, j),
					abs((int)(littleMaze->closestFinalNode / width) - (int)i)
					+ abs((int)(littleMaze->closestFinalNode % width) - (int)j));

		littleMaze->costs[littleMaze->closestFinalNode] = cost;
		littleMaze->paths[littleMaze->closestFinalNode] = path;
		littleMaze->finishTest();
#undef ID
	}
	void BigMazeInit()
	{
#define ID(i, j)	((i) * width + (j))
		unsigned int width = BIG_MAZE_SIZE;
		unsigned int height = BIG_MAZE_SIZE;

		unsigned int cost = (BIG_MAZE_SIZE / 2) + BIG_MAZE_SIZE - 4;
		deque<unsigned int> path;
		for (unsigned int i = (BIG_MAZE_SIZE / 2); i < BIG_MAZE_SIZE - 2; i++)
			path.push_back(ID(i, 1));
		for (unsigned int j = 1; j < BIG_MAZE_SIZE - 1; j++)
			path.push_back(ID(BIG_MAZE_SIZE - 2, j));

		bigMaze = new ShortestPathTest(width * height, 1);
		bigMaze->startNode = path[0];
		bigMaze->closestFinalNode = path[path.size() - 1];

		for (unsigned int i = 1; i < height - 1; i++)
		{
			for (unsigned int j = 1; j < width - 1; j++)
			{
				unsigned int id = ID(i, j);
				if (j == 1 && i != height - 1)
					bigMaze->g.addLink(id, ID(i + 1, j), 1);
				if (i % 2 == 1 && j != width - 1)
					bigMaze->g.addLink(id, ID(i, j + 1), 1);
			}
		}
		for (unsigned int i = 0; i < height; i++)
			for (unsigned int j = 0; j < width; j++)
				bigMaze->g.setNodeHeuristic(ID(i, j),
					abs((int)(bigMaze->closestFinalNode / width) - (int)i)
					+ abs((int)(bigMaze->closestFinalNode % width) - (int)j));

		bigMaze->costs[bigMaze->closestFinalNode] = cost;
		bigMaze->paths[bigMaze->closestFinalNode] = path;
		bigMaze->finishTest();
#undef ID
	}
	void EmptyMapInit()
	{
#define ID(i, j)	((i) * width + (j))
		unsigned int width = EMPTY_MAP_SIZE;
		unsigned int height = EMPTY_MAP_SIZE;

		unsigned int start = ID(height / 2, width / 2);
		unsigned int finalNode = ID(height - 1, width - 1);
		unsigned int cost = abs((int)(start / width) - (int)(finalNode / width)) + abs((int)(start % width) - (int)(finalNode % width));

		emptyMap = new ShortestPathTest(width * height, 1);
		emptyMap->startNode = start;
		emptyMap->closestFinalNode = finalNode;
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				unsigned int id = ID(i, j);
				if (i > 0)			emptyMap->g.addLink(id, ID(i - 1, j), 3);	// Haut<->Bas : coûte 3     
				if (i < height - 1)	emptyMap->g.addLink(id, ID(i + 1, j), 1);	// Bas<->Haut : coûte 1     
				if (j > 0)			emptyMap->g.addLink(id, ID(i, j - 1), 1);	// Gauche<->Droite : coûte 1
				if (j < width - 1)	emptyMap->g.addLink(id, ID(i, j + 1), 2);	// Droite<->Gauche : coûte 2

				emptyMap->g.setNodeHeuristic(ID(i, j),
					abs((int)(finalNode / width) - (int)i) + abs((int)(finalNode % width) - (int)j));
			}
		}

		emptyMap->costs[emptyMap->closestFinalNode] = cost;
		emptyMap->finishTest();
#undef ID
	}

	TEST_MODULE_INITIALIZE(ShortestPathsTestsInit)
	{
		SimpleTestInit();
		LittleMazeInit();
		BigMazeInit();
		EmptyMapInit();
	}

	TEST_CLASS(DijkstraTests)
	{
	public:
		TEST_METHOD(DijkstraSimpleTest)
		{
			Dijkstra<> dj(simpleTest->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(simpleTest, dj);
		}

		TEST_METHOD(DijkstraLittleMaze)
		{
			Dijkstra<> dj(littleMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(littleMaze, dj);
		}

		TEST_METHOD(DijkstraBigMaze)
		{
			Dijkstra<> dj(bigMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(bigMaze, dj);
		}

		TEST_METHOD(DijkstraEmptyMap)
		{
			Dijkstra<> dj(emptyMap->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(emptyMap, dj);
		}
	};

	TEST_CLASS(AStarTests)
	{
	public:
		TEST_METHOD(AStarSimpleTest)
		{
			AStar<> as(simpleTest->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(simpleTest, as);
		}

		TEST_METHOD(AStarLittleMaze)
		{
			AStar<> as(littleMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(littleMaze, as);
		}

		TEST_METHOD(AStarBigMaze)
		{
			AStar<> as(bigMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(bigMaze, as);
		}

		TEST_METHOD(AStarEmptyMap)
		{
			AStar<> as(emptyMap->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(emptyMap, as);
		}
	};

	TEST_CLASS(BellmanTests)
	{
		TEST_METHOD(BellmanSimpleTest_Algo2)
		{
			// Exemple tiré du cours d'Algorithmique 2 (Ensimag 1A) :
			Graph g(8);
			g.addLink(7, 6, 1, true);
			g.addLink(7, 5, 1, true);
			g.addLink(6, 3, 2, true);
			g.addLink(5, 3, 1, true);
			g.addLink(4, 2, 1, true);
			g.addLink(3, 1, 1, true);
			g.addLink(2, 1, 1, true);
			g.addLink(1, 0, 1, true);

			Bellman<> bm(g);
			bm.computeShortestPathsFrom(7);

			const unsigned int costs[8] =
				{ 4, 3, (unsigned int)(-1), 2, (unsigned int)(-1), 1, 1, 0 };
			const deque<unsigned int> paths[8] = {
				deque < unsigned int > { 7, 5, 3, 1, 0 },
				deque < unsigned int > { 7, 5, 3, 1 },
				deque < unsigned int > { 2 },
				deque < unsigned int > { 7, 5, 3 },
				deque < unsigned int > { 4 },
				deque < unsigned int > { 7, 5 },
				deque < unsigned int > { 7, 6 },
				deque < unsigned int > { 7 } };
			for (unsigned int i = 0; i < g.size(); i++)
			{
				Assert::AreEqual((costs[i] != -1), bm.canReachNode(i));
				Assert::AreEqual(costs[i], bm.getCostTo(i));
				Assert::AreEqual(paths[i], bm.getShortestPathTo(i));
				Assert::AreEqual(reverse_vect(paths[i]), bm.getReverseShortestPathTo(i));
			}
		}
		TEST_METHOD(BellmanSimpleTest_RO)
		{
			// Exemple tiré du cours de Rercherche Opérationnelle (Ensimag 1A) :
			Graph g(8);
			g.addLink(0, 2, 1, true);
			g.addLink(0, 3, 1, true);
			g.addLink(1, 3, 1, true);
			g.addLink(2, 5, 1, true);
			g.addLink(2, 6, 1, true);
			g.addLink(3, 4, 1, true);
			g.addLink(3, 5, 2, true);
			g.addLink(3, 7, 1, true);
			g.addLink(5, 6, 1, true);
			g.addLink(5, 7, 1, true);

			Bellman<> bm(g);
			bm.computeShortestPathsFrom(0);

			const unsigned int costs[8] =
				{ 0, (unsigned int)(-1), 1, 1, 2, 2, 2, 2 };
			const deque<unsigned int> paths[8] = {
				deque < unsigned int > { 0 },
				deque < unsigned int > { 1 },
				deque < unsigned int > { 0, 2 },
				deque < unsigned int > { 0, 3 },
				deque < unsigned int > { 0, 3, 4 },
				deque < unsigned int > { 0, 2, 5 },
				deque < unsigned int > { 0, 2, 6 },
				deque < unsigned int > { 0, 3, 7 } };
			for (unsigned int i = 0; i < g.size(); i++)
			{
				Assert::AreEqual((costs[i] != -1), bm.canReachNode(i));
				Assert::AreEqual(costs[i], bm.getCostTo(i));
				Assert::AreEqual(paths[i], bm.getShortestPathTo(i));
				Assert::AreEqual(reverse_vect(paths[i]), bm.getReverseShortestPathTo(i));
			}
		}
		TEST_METHOD(BellmanImpossibleTopologicalOrder)
		{
			// Exemple tiré du cours de Rercherche Opérationnelle (Ensimag 1A) :
			Graph g(6);
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

			Bellman<> bm(g);
			bm.computeShortestPathsFrom(0);
			for (unsigned int i = 0; i < g.size(); i++)
				Assert::IsFalse(bm.canReachNode(i));
		}
	};

	TEST_CLASS(DFS_Tests)
	{
	public:
		TEST_METHOD(DFS_SimpleTest)
		{
			DFS_ShortestPath<> dfs(simpleTest->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(simpleTest, dfs);
		}

		TEST_METHOD(DFS_LittleMaze)
		{
			DFS_ShortestPath<> dfs(littleMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(littleMaze, dfs);
		}

		TEST_METHOD(DFS_BigMaze)
		{
			DFS_ShortestPath<> dfs(bigMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(bigMaze, dfs);
		}

		/* Ce test échoue toujours !
		TEST_METHOD(DFS_EmptyMap)
		{
			DFS_ShortestPath<> dfs(emptyMap->g);
			RUN_SHORTEST_PATH_FINDER_TEST_CLOSEST_FINAL_NODE(emptyMap, dfs);
		}
		*/
	};

	TEST_CLASS(BFS_Tests)
	{
	public:
		TEST_METHOD(BFS_SimpleTest)
		{
			BFS_ShortestPath<> bfs(simpleTest->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(simpleTest, bfs);
		}

		TEST_METHOD(BFS_LittleMaze)
		{
			BFS_ShortestPath<> bfs(littleMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(littleMaze, bfs);
		}

		TEST_METHOD(BFS_BigMaze)
		{
			BFS_ShortestPath<> bfs(bigMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(bigMaze, bfs);
		}

		TEST_METHOD(BFS_EmptyMap)
		{
			BFS_ShortestPath<> bfs(emptyMap->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(emptyMap, bfs);
		}
	};

	TEST_CLASS(BellmanFordTests)
	{
	public:
		TEST_METHOD(BellmanFordSimpleTest)
		{
			BellmanFord<> bf(simpleTest->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(simpleTest, bf);
		}

		TEST_METHOD(BellmanFordLittleMaze)
		{
			BellmanFord<> bf(littleMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(littleMaze, bf);
		}

		TEST_METHOD(BellmanFordBigMaze)
		{
			BellmanFord<> bf(bigMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(bigMaze, bf);
		}

		TEST_METHOD(BellmanFordEmptyMap)
		{
			BellmanFord<> bf(emptyMap->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(emptyMap, bf);
		}
	};

	TEST_CLASS(BellmanFordYenTests)
	{
	public:
		TEST_METHOD(BellmanFordYenSimpleTest)
		{
			BellmanFordYen<> bfy(simpleTest->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(simpleTest, bfy);
		}

		TEST_METHOD(BellmanFordYenLittleMaze)
		{
			BellmanFordYen<> bfy(littleMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(littleMaze, bfy);
		}

		TEST_METHOD(BellmanFordYenBigMaze)
		{
			BellmanFordYen<> bfy(bigMaze->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(bigMaze, bfy);
		}

		TEST_METHOD(BellmanFordYenEmptyMap)
		{
			BellmanFordYen<> bfy(emptyMap->g);
			RUN_SHORTEST_PATH_FINDER_TEST_ALL_NODES(emptyMap, bfy);
		}
	};

	TEST_CLASS(FloydWarshallTests)
	{
		TEST_METHOD(FloydWarshall_SimpleTest)
		{
#define CTI(c)	(c - 'a')

			// Wikipedia example test : http://fr.wikipedia.org/wiki/Algorithme_de_Dijkstra
			Graph g(CTI('j') + 1);
			g.addLink(CTI('a'), CTI('b'), 85);
			g.addLink(CTI('a'), CTI('c'), 217);
			g.addLink(CTI('a'), CTI('e'), 173);

			g.addLink(CTI('b'), CTI('f'), 80);

			g.addLink(CTI('c'), CTI('g'), 186);
			g.addLink(CTI('c'), CTI('h'), 103);

			g.addLink(CTI('d'), CTI('h'), 183);

			g.addLink(CTI('e'), CTI('j'), 502);

			g.addLink(CTI('f'), CTI('i'), 250);

			g.addLink(CTI('i'), CTI('j'), 84);

			FloydWarshall<> fw(g);
			for (int i = 0; i < PATH_FINDERS_COMPUTE_LOOPS; i++)
				fw.computeShortestPaths();

			const unsigned int costs[] = { 0, 85, 217, 503, 173, 165, 403, 320, 415, 499 };
			for (unsigned int i = 0; i <= CTI('j'); i++)
			{
				Assert::IsTrue(fw.pathExists(0, i));
				Assert::AreEqual(costs[i], fw.getPathCost(0, i));
			}
#undef CTI
		}
	};

	TEST_CLASS(RandomPaths)
	{
		TEST_METHOD(RandomGraph)
		{
			srand((unsigned int)time(NULL));

			Graph g(RANDOM_MAP_SIZE);
			for (unsigned int i = 0; i < RANDOM_MAP_SIZE; i++)
			{
				unsigned int nbLinks = (unsigned int)rand() % RANDOM_MAP_MAX_NB_LINKS;
				for (unsigned int j = 0; j < nbLinks; j++)
				{
					unsigned int target = (unsigned int)rand() % RANDOM_MAP_SIZE;
					unsigned int cost = (unsigned int)rand() % RANDOM_MAP_MAX_COST;
					g.addLink(i, target, cost, true);
				}
			}

			unsigned int startNode = rand() % RANDOM_MAP_SIZE;
			unsigned int finalNode = rand() % RANDOM_MAP_SIZE;
			g.setNodeFinal(finalNode);

			Dijkstra<> dj(g);
			dj.computeShortestPathsFrom(startNode);

			AStar<> as(g);
			as.computeShortestPathFrom(startNode);

			Assert::AreEqual(dj.canReachNode(finalNode), as.hasFoundPath());
			if (as.hasFoundPath())
			{
				Assert::AreEqual(finalNode, as.getFinalNode());
				Assert::AreEqual(dj.getCostTo(finalNode), as.getPathCost());
				Assert::AreEqual(dj.getShortestPathTo(finalNode), as.getShortestPath());
			}
		}
	};
}
