// Test Counter Culture sur le graphe invers�, pour d�terminer les performances des diff�rents algorithmes
#define CC_USE_INVERTED_GRAPH
#define CC_PRECOMPUTATION_HEURISTIC_MAX_SIZE	10000000

#include "CounterCulture.h"
#include "AStarDynamic.h"
#include "AStar.h"
#include "Dijkstra.h"
#include <iostream>
#include <ctime>

using namespace std;

void printResult_AStarDynamic(CCDynGraph::IndexNoeud finalNode, const CCNodeHeuristicGenerator& heuristicGen)
{
	try
	{
		cout << "AStarDynamic";
		if (heuristicGen.isHeuristicPrecomputed())
			cout << " (precomputed heuristic)";
		else
			cout << "                        ";
		cout << " : ";
		clock_t startTime = clock();

		CCNodeLinksGenerator linksGen(finalNode);
		CCNodeFinalGenerator finalGen(finalNode);
		CCDynGraph ccdg(linksGen, finalGen, heuristicGen);

		AStarDynamic<CCDynGraph> asd(ccdg);
#ifndef CC_USE_INVERTED_GRAPH
		asd.computeShortestPathFrom(0);
#else
		asd.computeShortestPathFrom(finalNode);
#endif

		time_t diffTime = clock() - startTime;
		cout << "Result = " << asd.getPathCost() << " ; Time = " << diffTime << " ms" << endl;
#ifdef _DEBUG
		cout << "                                     ; " << asd.getNbExploredNodes() << " nodes explored " << endl;
#endif
	}
	catch (...)
	{
		cout << "Error !" << endl;
	}
}
void printResult_AStar(CCDynGraph::IndexNoeud finalNode, const CCNodeHeuristicGenerator& heuristicGen)
{
	try
	{
		cout << "AStar";
		if (heuristicGen.isHeuristicPrecomputed())
			cout << " (precomputed heuristic)       ";
		else
			cout << "                               ";
		cout << " : ";
		clock_t startTime = clock();

		CCGraph ccg = generateCCGraph(finalNode, &heuristicGen);

		AStar<CCGraph> as(ccg);
#ifndef CC_USE_INVERTED_GRAPH
		as.computeShortestPathFrom(0);
#else
		as.computeShortestPathFrom(finalNode);
#endif

		time_t diffTime = clock() - startTime;
		cout << "Result = " << as.getPathCost() << " ; Time = " << diffTime << " ms" << endl;
#ifdef _DEBUG
		cout << "                                     ; " << as.getNbExploredNodes() << " nodes explored " << endl;
#endif
	}
	catch (...)
	{
		cout << "Error !" << endl;
	}
}
void printResult_Dijkstra(CCDynGraph::IndexNoeud finalNode)
{
	try
	{
		cout << "Dijkstra                             : ";
		clock_t startTime = clock();

		CCGraph ccg = generateCCGraph(finalNode, NULL);

		Dijkstra<CCGraph> dj(ccg);
#ifndef CC_USE_INVERTED_GRAPH
		dj.computeShortestPathsFrom(0);
#else
		dj.computeShortestPathsFrom(finalNode);
#endif

		time_t diffTime = clock() - startTime;

		cout << "Result = "
#ifndef CC_USE_INVERTED_GRAPH
			<< dj.getCostTo(finalNode)
#else
			<<  dj.getCostTo(0)
#endif
			<< " ; Time = " << diffTime << " ms" << endl;
	}
	catch (...)
	{
		cout << "Error !" << endl;
	}
}
int main()
{
	while (true)
	{
		cout << "Type the number to be reached (or 0 to quit) :" << endl;

		CCDynGraph::IndexNoeud finalNode;
		cin >> finalNode;
		if (finalNode == 0)
			break;

		CCNodeHeuristicGenerator heuristicGen(finalNode);

		cout << "Real result = " << cc_solve(finalNode) << endl;
		if (finalNode > CC_PRECOMPUTATION_HEURISTIC_MAX_SIZE)
			cout << "Warning : heuristic precomputation size limit is " << CC_PRECOMPUTATION_HEURISTIC_MAX_SIZE << "." << endl;
		cout << endl;
		printResult_AStarDynamic(finalNode, heuristicGen);
		printResult_AStar(finalNode, heuristicGen);
		printResult_Dijkstra(finalNode);

		try
		{
			heuristicGen.precomputeHeuristic();
		}
		catch (...)
		{
			cout << "Cannot precompute heuristic !" << endl;
		}
		if (heuristicGen.isHeuristicPrecomputed())
		{
			printResult_AStarDynamic(finalNode, heuristicGen);
			printResult_AStar(finalNode, heuristicGen);
		}
		cout << endl << endl << endl;
	}

	return EXIT_SUCCESS;
}
