#ifndef DEF_COUNTER_CULTURE
#define DEF_COUNTER_CULTURE

#include "Graph.h"
#include "DynamicGraph.h"
#include "Dijkstra.h"
#include "CounterCultureSolution.h"

template<class T>
T swap(T i)
{
	T k = 0;
	while (i > 0)
	{
		T d = i % 10;
		i /= 10;
		k = k * 10 + d;
	}
	return k;
}

class CCNodeLinksGenerator;
class CCNodeFinalGenerator;
class CCNodeHeuristicGenerator;
typedef DynamicGraph<CCNodeLinksGenerator, CCNodeFinalGenerator, CCNodeHeuristicGenerator> CCDynGraph;
typedef CCDynGraph::DynamicLink DynamicLink;

typedef Graph<CCDynGraph::Cout, CCDynGraph::IndexNoeud> CCGraph;

CCGraph generateCCGraph(CCGraph::IndexNoeud finalNode, const CCNodeHeuristicGenerator* heuristicGen = NULL);

#ifndef CC_PRECOMPUTATION_HEURISTIC_MAX_SIZE
#define CC_PRECOMPUTATION_HEURISTIC_MAX_SIZE	std::numeric_limits<CCDynGraph::IndexNoeud>::max()
#endif

class CCNodeLinksGenerator
{
protected:
	CCDynGraph::IndexNoeud finalNode;

public:
	CCNodeLinksGenerator(CCDynGraph::IndexNoeud _finalNode) : finalNode(_finalNode)	{ }

	CCDynGraph::IndexNoeud linksCount(CCDynGraph::IndexNoeud node) const
	{
		CCDynGraph::IndexNoeud count = 0;

#ifndef CC_USE_INVERTED_GRAPH
		if (node < finalNode)
			count++;

		if (swap(node) <= finalNode)
			count++;
#else
		if (node > 0)
			count++;

		if (node % 10 != 0)
			count++;
#endif

		return count;
	}
	CCDynGraph::Lien link(CCDynGraph::IndexNoeud node, CCDynGraph::IndexNoeud linkIndex) const
	{
#ifndef CC_USE_INVERTED_GRAPH
		if (node < finalNode && linkIndex == 0)
			return DynamicLink(node + 1, 1);
#else
		if (node > 0 && linkIndex == 0)
			return DynamicLink(node - 1, 1);
#endif

		return DynamicLink(swap(node), 1);
	}
};
class CCNodeFinalGenerator
{
protected:
	CCDynGraph::IndexNoeud finalNode;

public:
	CCNodeFinalGenerator(CCDynGraph::IndexNoeud _finalNode) : finalNode(_finalNode)	{ }

	bool operator()(CCDynGraph::IndexNoeud node) const
	{
#ifndef CC_USE_INVERTED_GRAPH
		return (node == finalNode);
#else
		return (node == 0);
#endif
	}
};
class CCNodeHeuristicGenerator
{
protected:
	CCDynGraph::IndexNoeud finalNode;
	std::vector<CCDynGraph::Cout> precomputedHeuristic;

public:
	CCNodeHeuristicGenerator(CCDynGraph::IndexNoeud _finalNode) : finalNode(_finalNode)	{ }

	// Attention : on doit toujours v�rifier : h(x) <= d(x, finalNode),
	// ce qui est difficile � garantir ici !

	// Afin de d�terminer l'importance de l'heuristique, on permet le calcul d'une heuristique
	// en utilisant la solution donn�e par Google.
	void precomputeHeuristic()
	{
#ifndef CC_USE_INVERTED_GRAPH
		unsigned long long finalDist = cc_solve(finalNode);
		precomputedHeuristic.resize(finalNode + 1);
		for (CCDynGraph::IndexNoeud i = 0; i <= finalNode; i++)
			precomputedHeuristic[i] = (CCDynGraph::IndexNoeud)(finalDist - cc_solve(i));	// Minorant peu pr�cis de d(i, f)
#else
		// Limite la taille du tableau de l'heuristique
		CCDynGraph::IndexNoeud maxHeuristicNode = std::min<CCDynGraph::IndexNoeud>(finalNode, CC_PRECOMPUTATION_HEURISTIC_MAX_SIZE);

		// Le graphe CC invers� permet ici d'obtenir une heuristique exacte !
		precomputedHeuristic.resize(maxHeuristicNode + 1);
		for (CCDynGraph::IndexNoeud i = 0; i <= maxHeuristicNode; i++)
			precomputedHeuristic[i] = (CCDynGraph::IndexNoeud)cc_solve(i);					// Vaut exactement d(i, f) car f = 0
#endif
	}

	bool isHeuristicPrecomputed() const
	{
		return (precomputedHeuristic.size() > 0);
	}
	CCDynGraph::Cout operator()(CCDynGraph::IndexNoeud index) const
	{
		if (index < precomputedHeuristic.size())
			return precomputedHeuristic[index];
		return 0;
	}
};

CCGraph generateCCGraph(CCGraph::IndexNoeud finalNode, const CCNodeHeuristicGenerator* heuristicGen)
{
	CCGraph g(finalNode + 1);
#ifndef CC_USE_INVERTED_GRAPH
	g.setNodeFinal(finalNode);
#else
	g.setNodeFinal(0);
#endif

	for (CCGraph::IndexNoeud i = 0; i < finalNode + 1; i++)
	{
#ifndef CC_USE_INVERTED_GRAPH
		if (i < finalNode)
			g.addLink(i, i + 1, 1, true);

		CCGraph::IndexNoeud sw = swap(i);
		if (sw <= finalNode)
			g.addLink(i, sw, 1, true);
#else
		if (i < finalNode)
			g.addLink(i + 1, i, 1, true);

		if (i % 10 != 0)
		{
			CCGraph::IndexNoeud sw = swap(i);
			if (sw <= finalNode)
				g.addLink(i, sw, 1, true);
		}
#endif

		if (heuristicGen)
			g.setNodeHeuristic(i, (*heuristicGen)(i));
	}
	return g;
}

#endif
