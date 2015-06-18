#ifndef DEF_DYNAMIC_GRAPH
#define DEF_DYNAMIC_GRAPH

#include <deque>
#include <vector>

// Un graphe dynamique est capable de d�terminer la liste des liens d'un noeud pour tout identificateur de noeud donn�.
// Un lien contient un noeud (le voisin) et son co�t de trajet.

// Types n�cessaire de la classe Graphe :
//		typedef <> IndexNoeud;
//		typedef <> Cout;
//			Note :	Ces types doivent �tre des types entiers.
// Fonctions statiques constantes n�cessaires de la classe Graphe :
//		static IndexNoeud INVALID_NODE_INDEX();
//		static Cout INFINITE_COST();
//			Note :	Cette derni�re fonction constante doit �tre sup�rieure stricte � tout autre co�t possible.
// Fonctions n�cessaires de la classe Graphe :
//		std::vector<Lien> getNodeLinks(IndexNoeud index) const;
//			Note :	std::vector peut �tre remplac� par tout autre conteneur de Lien* pouvant �tre it�r�.
// Fonction suppl�mentaire n�cessaire pour les algorithmes dynamiques A* et DFS :
//		bool isNodeFinal(IndexNoeud node) const;
// Fonction suppl�mentaire n�cessaire pour l'algorithme dynamique A* :
//		Cout getNodeHeuristic(IndexNoeud node) const;
//			Note :	Cette fonction doit renvoyer la distance minimale estim�e du noeud jusqu'� un noeud final.
//					Afin que l'algorithme fourni ici fonctionne, cette fonction doit v�rifier :
//						Pour tous noeuds x et y : h(x) <= h(y) + d(x, y).
//					Plus simplement, l'heuristique d'un noeud x doit toujours �tre inf�rieure
//					� la distance r�elle entre x et le point final le plus proche. Plus cette heuristique est proche
//					de cette distance r�elle, meilleure est l'heuristique, et l'algorithme est alors plus efficace.
//					La classe fournie par d�faut ici impl�mente une heuristique nulle :
//					son utilisation revient � effectuer l'algorithme de Dijkstra sur le graphe fourni.

// Fonctions n�cessaires de la classe Lien :
//		Cout getCost() const;
//		IndexNoeud getTargetIndex() const;



// Impl�mentation minimale et efficace en m�moire de ces classes
class NullGenerator
{
public:
	long long int operator()(unsigned int index) const
	{	return 0;	}
};
template<class NodeLinksGenerator, class NodeFinalGenerator,
	class NodeHeuristicGenerator = NullGenerator,
	class NodeIndex = unsigned int, class Cost = long>
class DynamicGraph
{
public:
	class DynamicLink;
	typedef DynamicLink Lien;
	typedef NodeIndex IndexNoeud;
	typedef Cost Cout;

	// On est ici oblig� de d�finir des fonctions pour acc�der � ces constantes,
	// � cause du standard C++ qui d�finit la valeur max d'un type en tant que fonction.
	static IndexNoeud INVALID_NODE_INDEX()
	{
		return std::numeric_limits<IndexNoeud>::max();
	}
	static Cout INFINITE_COST()
	{
		return std::numeric_limits<Cout>::max();
	}

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
