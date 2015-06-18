#ifndef DEF_DYNAMIC_GRAPH
#define DEF_DYNAMIC_GRAPH

#include <deque>
#include <vector>

// Un graphe dynamique est capable de déterminer la liste des liens d'un noeud pour tout identificateur de noeud donné.
// Un lien contient un noeud (le voisin) et son coût de trajet.

// Types nécessaire de la classe Graphe :
//		typedef <> IndexNoeud;
//		typedef <> Cout;
//			Note :	Ces types doivent être des types entiers.
// Constantes nécessaires de la classe Graphe :
//		static const IndexNoeud INVALID_NODE_INDEX;
//		static const Cout INFINITE_COST;
//			Note :	Cette dernière constante doit être supérieure stricte à tout autre coût possible.
// Fonctions nécessaires de la classe Graphe :
//		std::vector<Lien> getNodeLinks(IndexNoeud index) const;
//			Note :	std::vector peut être remplacé par tout autre conteneur de Lien* pouvant être itéré.
// Fonction supplémentaire nécessaire pour les algorithmes dynamiques A* et DFS :
//		bool isNodeFinal(IndexNoeud node) const;
// Fonction supplémentaire nécessaire pour l'algorithme dynamique A* :
//		Cout getNodeHeuristic(IndexNoeud node) const;
//			Note :	Cette fonction doit renvoyer la distance minimale estimée du noeud jusqu'à un noeud final.
//					Afin que l'algorithme fourni ici fonctionne, cette fonction doit vérifier :
//						Pour tous noeuds x et y : h(x) <= h(y) + d(x, y).
//					Plus simplement, l'heuristique d'un noeud x doit toujours être inférieure
//					à la distance réelle entre x et le point final le plus proche. Plus cette heuristique est proche
//					de cette distance réelle, meilleure est l'heuristique, et l'algorithme est alors plus efficace.
//					La classe fournie par défaut ici implémente une heuristique nulle :
//					son utilisation revient à effectuer l'algorithme de Dijkstra sur le graphe fourni.

// Fonctions nécessaires de la classe Lien :
//		Cout getCost() const;
//		IndexNoeud getTargetIndex() const;



// Implémentation minimale et efficace en mémoire de ces classes
class NullGenerator
{
public:
	long long int operator()(long long int index) const
	{	return 0;	}
};
template<class NodeLinksGenerator, class NodeFinalGenerator,
	class NodeHeuristicGenerator = NullGenerator >
class DynamicGraph
{
public:
	class DynamicLink;
	typedef DynamicLink Lien;
	typedef long long int IndexNoeud;
	static const IndexNoeud INVALID_NODE_INDEX = (long long int)(-1);
	typedef long long int Cout;
	static const Cout INFINITE_COST = LLONG_MAX;

	class DynamicLink
	{
	protected:
		IndexNoeud targetIndex;
		Cout cost;

	public:
		IndexNoeud getTargetIndex() const	{ return targetIndex; }
		Cout getCost() const				{ return cost; }

		DynamicLink(IndexNoeud _targetIndex, Cout _cost)
			: targetIndex(_targetIndex), cost(_cost) { }
	};

protected:
	const NodeLinksGenerator& linksGen;
	const NodeFinalGenerator& finalGen;
	const NodeHeuristicGenerator& heuristicGen;

public:
	DynamicGraph(const NodeLinksGenerator& linksGenerator,
		const NodeFinalGenerator& finalGenerator,
		const NodeHeuristicGenerator& heuristicGenerator)
		: linksGen(linksGenerator), finalGen(finalGenerator), heuristicGen(heuristicGenerator)
	{ }

	std::vector<DynamicLink> getNodeLinks(IndexNoeud index) const
	{
		return linksGen(index);
	}
	bool isNodeFinal(IndexNoeud node) const
	{
		return finalGen(node);
	}
	Cout getNodeHeuristic(IndexNoeud node) const
	{
		return heuristicGen(node);
	}
};

#endif
