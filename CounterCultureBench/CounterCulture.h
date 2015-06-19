#ifndef DEF_COUNTER_CULTURE
#define DEF_COUNTER_CULTURE

#include "Graph.h"
#include "DynamicGraph.h"
#include "Dijkstra.h"

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

		if (index < finalNode)
			links.push_back(DynamicLink(index + 1, 1));

		CCDynGraph::IndexNoeud sw = swap(index);
		if (sw <= finalNode)
			links.push_back(DynamicLink(sw, 1));

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
		return (index == finalNode);
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

	// Afin de déterminer l'importance de l'heuristique, on permet un calcul d'une heuristique optimale
	// sur tous les noeuds grâce à l'application préalable d'un Dijkstra sur le graphe.
	// On notera qu'après l'application du Dijkstra, tous les chemins ont été trouvés, en particulier celui
	// menant jusqu'au noeud final, mais on ne cherche ici qu'à mesurer l'importance de l'heuristique.
	void precomputeHeuristic(const Dijkstra<CCGraph>& dj)
	{
		precomputedHeuristic.resize(finalNode + 1);
		for (CCDynGraph::IndexNoeud i = 0; i <= finalNode; i++)
			precomputedHeuristic[i] = dj.getCostTo(i);
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
	g.setNodeFinal(finalNode);

	for (CCGraph::IndexNoeud i = 0; i < finalNode + 1; i++)
	{
		if (i < finalNode)
			g.addLink(i, i + 1, 1, true);

		CCGraph::IndexNoeud sw = swap(i);
		if (sw <= finalNode)
			g.addLink(i, sw, 1, true);

		if (heuristicGen)
			g.setNodeHeuristic(i, (*heuristicGen)(i));
	}
	return g;
}

#endif
