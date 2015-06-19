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

class CCNodeLinksGenerator
{
protected:
	CCDynGraph::IndexNoeud finalNode;

public:
	CCNodeLinksGenerator(CCDynGraph::IndexNoeud _finalNode) : finalNode(_finalNode)	{ }

	std::vector<DynamicLink> operator()(CCDynGraph::IndexNoeud index) const
	{
		std::vector<DynamicLink> links;
		links.reserve(2);

#ifndef CC_USE_INVERTED_GRAPH
		if (index < finalNode)
			links.push_back(DynamicLink(index + 1, 1));

		CCDynGraph::IndexNoeud sw = swap(index);
		if (sw <= finalNode)
			links.push_back(DynamicLink(sw, 1));
#else
		if (index > 0)
			links.push_back(DynamicLink(index - 1, 1));

		if (index % 10 != 0)
		{
			CCDynGraph::IndexNoeud sw = swap(index);
			if (sw <= finalNode)
				links.push_back(DynamicLink(sw, 1));
		}
#endif

		return links;
	}
};
class CCNodeFinalGenerator
{
protected:
	CCDynGraph::IndexNoeud finalNode;

public:
	CCNodeFinalGenerator(CCDynGraph::IndexNoeud _finalNode) : finalNode(_finalNode)	{ }

	bool operator()(CCDynGraph::IndexNoeud index) const
	{
#ifndef CC_USE_INVERTED_GRAPH
		return (index == finalNode);
#else
		return (index == 0);
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

	// Attention : on doit toujours vérifier : h(x) <= d(x, finalNode),
	// ce qui est difficile à garantir ici !

	// Afin de déterminer l'importance de l'heuristique, on permet le calcul d'une heuristique
	// en utilisant la solution donnée par Google.
	void precomputeHeuristic()
	{
		precomputedHeuristic.resize(finalNode + 1);

#ifndef CC_USE_INVERTED_GRAPH
		unsigned long long finalDist = cc_solve(finalNode);
		for (CCDynGraph::IndexNoeud i = 0; i <= finalNode; i++)
			precomputedHeuristic[i] = (CCDynGraph::IndexNoeud)(finalDist - cc_solve(i));	// Minorant peu précis de d(i, f)
#else
		// Le graphe CC inversé permet ici d'obtenir une heuristique exacte !
		for (CCDynGraph::IndexNoeud i = 0; i <= finalNode; i++)
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
