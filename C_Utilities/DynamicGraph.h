#ifndef DEF_DYNAMIC_GRAPH
#define DEF_DYNAMIC_GRAPH

#include <deque>
#include <vector>

// Un graphe dynamique est capable de d�terminer la liste des liens d'un noeud pour tout identificateur de noeud donn�.
// Un lien contient un noeud (le voisin) et son co�t de trajet.

// Fonctions n�cessaires de la classe Graphe :
//		std::vector<Lien> getNodeLinks(unsigned int index) const;
//			Note :	std::vector peut �tre remplac� par tout autre conteneur de Lien* pouvant �tre it�r�.
// Fonction suppl�mentaire n�cessaire pour les algorithmes dynamiques A* et DFS :
//		bool isNodeFinal(unsigned int node) const;
// Fonction suppl�mentaire n�cessaire pour l'algorithme dynamique A* :
//		unsigned int getNodeHeuristic(unsigned int node) const;
//			Note :	Cette fonction doit renvoyer la distance minimale estim�e du noeud jusqu'� un noeud final.
//					Afin que l'algorithme fourni ici fonctionne, cette fonction doit v�rifier :
//						Pour tous noeuds x et y : h(x) <= h(y) + d(x, y).
//					La classe fournie par d�faut ici impl�mente une heuristique nulle :
//					son utilisation revient � effectuer l'algorithme de Dijkstra sur le graphe fourni.

// Fonctions n�cessaires de la classe Lien :
//		unsigned int getCost() const;
//		unsigned int getTargetIndex() const;



// Impl�mentation minimale et efficace en m�moire de ces classes
class DynamicLink;
class EmptyDynamicLinkVectorGenerator
{
public:
	std::vector<DynamicLink> operator()(unsigned int index) const
	{	return std::vector<DynamicLink>();	}
};
class FalseGenerator
{
public:
	bool operator()(unsigned int index) const
	{	return false;						}
};
class NullGenerator
{
public:
	unsigned int operator()(unsigned int index) const
	{	return 0;							}
};
class DynamicLink
{
protected:
	unsigned int targetIndex;
	unsigned int cost;

public:
	unsigned int getTargetIndex() const	{ return targetIndex; }
	unsigned int getCost() const		{ return cost; }

	DynamicLink(unsigned int _targetIndex, unsigned int _cost)
		: targetIndex(_targetIndex), cost(_cost) { }
};
template<class NodeLinksGenerator = EmptyDynamicLinkVectorGenerator,
	class NodeFinalGenerator = FalseGenerator,
	class NodeHeuristicGenerator = NullGenerator>
class DynamicGraph
{
	const NodeLinksGenerator& linksGen;
	const NodeFinalGenerator& finalGen;
	const NodeHeuristicGenerator& heuristicGen;

public:
	DynamicGraph(const NodeLinksGenerator& linksGenerator,
		const NodeFinalGenerator& finalGenerator,
		const NodeHeuristicGenerator& heuristicGenerator)
		: linksGen(linksGenerator), finalGen(finalGenerator), heuristicGen(heuristicGenerator)
	{ }

	std::vector<DynamicLink> getNodeLinks(unsigned int index) const
	{
		return linksGen(index);
	}
	bool isNodeFinal(unsigned int node) const
	{
		return finalGen(node);
	}
	unsigned int getNodeHeuristic(unsigned int node) const
	{
		return heuristicGen(node);
	}
};

#endif
