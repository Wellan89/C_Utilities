#ifndef DEF_DYNAMIC_GRAPH
#define DEF_DYNAMIC_GRAPH

#include <deque>
#include <vector>

// Un graphe dynamique est capable de déterminer la liste des liens d'un noeud pour tout identificateur de noeud donné.
// Un lien contient un noeud (le voisin) et son coût de trajet.

// Fonctions nécessaires de la classe Graphe :
//		std::vector<Lien> getNodeLinks(unsigned int index) const;
//			Note :	std::vector peut être remplacé par tout autre conteneur de Lien* pouvant être itéré.
// Fonction supplémentaire nécessaire pour les algorithmes dynamiques A* et DFS :
//		bool isNodeFinal(unsigned int node) const;
// Fonction supplémentaire nécessaire pour l'algorithme dynamique A* :
//		unsigned int getNodeHeuristic(unsigned int node) const;
//			Note :	Cette fonction doit renvoyer la distance minimale estimée du noeud jusqu'à un noeud final.
//					Afin que l'algorithme fourni ici fonctionne, cette fonction doit vérifier :
//						Pour tous noeuds x et y : h(x) <= h(y) + d(x, y).
//					La classe fournie par défaut ici implémente une heuristique nulle :
//					son utilisation revient à effectuer l'algorithme de Dijkstra sur le graphe fourni.

// Fonctions nécessaires de la classe Lien :
//		unsigned int getCost() const;
//		unsigned int getTargetIndex() const;



// Implémentation minimale et efficace en mémoire de ces classes
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
