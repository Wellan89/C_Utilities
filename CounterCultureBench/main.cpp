#include "CounterCulture.h"
#include "AStarDynamic.cpp"
#include "AStar.cpp"
#include "Dijkstra.cpp"
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
		asd.computeShortestPathFrom(0);

		time_t diffTime = clock() - startTime;
		cout << "Result = " << asd.getPathCost() << " ; Time = " << diffTime << " ms" << endl
			<< "                                     ; " << asd.getNbExploredNodes() << " nodes explored " << endl;
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
		as.computeShortestPathFrom(0);

		time_t diffTime = clock() - startTime;
		cout << "Result = " << as.getPathCost() << " ; Time = " << diffTime << " ms" << endl;
	}
	catch (...)
	{
		cout << "Error !" << endl;
	}
}
void printResult_Dijkstra(CCDynGraph::IndexNoeud finalNode, CCNodeHeuristicGenerator& heuristicGen)
{
	try
	{
		cout << "Dijkstra                             : ";
		clock_t startTime = clock();

		CCGraph ccg = generateCCGraph(finalNode, &heuristicGen);

		Dijkstra<CCGraph> dj(ccg);
		dj.computeShortestPathsFrom(0);

		time_t diffTime = clock() - startTime;
		cout << "Result = " << dj.getCostTo(finalNode) << " ; Time = " << diffTime << " ms" << endl;

		// Se souviens de l'heuristique d'après les résultats pré-calculés
		heuristicGen.precomputeHeuristic(dj);
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
		cout << "Entrez le nombre a atteindre (ou 0 pour quitter) :" << endl;

		CCDynGraph::IndexNoeud finalNode;
		cin >> finalNode;
		if (finalNode == 0)
			break;

		CCNodeHeuristicGenerator heuristicGen(finalNode);

		cout << endl;
		printResult_AStarDynamic(finalNode, heuristicGen);
		printResult_AStar(finalNode, heuristicGen);
		printResult_Dijkstra(finalNode, heuristicGen);
		if (heuristicGen.isHeuristicPrecomputed())
		{
			printResult_AStarDynamic(finalNode, heuristicGen);
			printResult_AStar(finalNode, heuristicGen);
		}
		cout << endl << endl << endl;
	}

	return EXIT_SUCCESS;
}
