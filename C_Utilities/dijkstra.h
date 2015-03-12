#ifndef DEF_DIJKSTRA
#define DEF_DIJKSTRA

#include <list>

template<class Graphe, class Noeud>
class Dijkstra
{
protected:
	const Graphe& g;

public:
	Dijkstra(const Graphe& gr) : g(gr) { }

	void computeShortestPathsFrom(Noeud* from);

	std::list<Noeud*> getShortestPathTo(Noeud* to);
};

#endif
