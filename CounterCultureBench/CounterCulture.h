#ifndef DEF_COUNTER_CULTURE
#define DEF_COUNTER_CULTURE

#include "Graph.h"
#include "DynamicGraph.h"

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

public:
	CCNodeHeuristicGenerator(CCDynGraph::IndexNoeud _finalNode) : finalNode(_finalNode)	{ }

	CCDynGraph::Cout operator()(CCDynGraph::IndexNoeud index) const
	{
		// Attention : on doit toujours vérifier : h(x) <= d(x, finalNode),
		// ce qui est difficile à garantir ici !
		return 0;
	}
};

CCGraph generateCCGraph(CCGraph::IndexNoeud finalNode)
{
	CCGraph g(finalNode + 1);

	g.setNodeFinal(finalNode);
	CCNodeHeuristicGenerator heuristicGen(finalNode);
	for (CCGraph::IndexNoeud i = 0; i < finalNode + 1; i++)
	{
		if (i < finalNode)
			g.addLink(i, i + 1, 1, true);

		CCGraph::IndexNoeud sw = swap(i);
		if (sw <= finalNode)
			g.addLink(i, sw, 1, true);

		g.setNodeHeuristic(i, heuristicGen(i));
	}
	return g;
}

#endif
