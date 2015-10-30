#ifndef DEF_MODULUS_NUMBER
#define DEF_MODULUS_NUMBER

#include <istream>
#include <ostream>
#include <limits>

// Un nombre modulo mod (un élément de Z/mod.Z).
// Le type de donné interne IntContainer doit pouvoir contenir tous les entiers entre 0 et mod*mod.
// Ce type peut être non signé à condition de n'effectuer aucune opération impliquant des nombres négatifs :
// dans ce cas ceux-ci seraient alors implicitement convertis en leur équivalent non signé, donc positif !
template<class IntContainer, IntContainer mod>
class ModulusNumber
{
protected:
	static_assert((unsigned long long)std::numeric_limits<IntContainer>::max() > (unsigned long long)mod * (unsigned long long)mod, "IntContainer is too small.");

	// Version optimisée de distance(0, a)
	static IntContainer distanceFrom0(IntContainer a)
	{
		IntContainer distIncr = a;
		IntContainer distDecr = mod - a;
		return min(distIncr, distDecr);
	}
#if 0
	// Versions simples
	static IntContainer distance(IntContainer a, IntContainer b)
	{
		IntContainer distIncr = (b + mod - a) % mod;
		IntContainer distDecr = (a + mod - b) % mod;
		return min(distIncr, distDecr);
	}
	static IntContainer less(IntContainer a, IntContainer b)
	{
		IntContainer distIncr = (b + mod - a) % mod;
		IntContainer distDecr = (a + mod - b) % mod;
		return (distIncr < distDecr);
	}
#else
	// Versions optimisées
	static IntContainer distance(IntContainer a, IntContainer b)
	{
		return distanceFrom0(a < b ? (b - a) : (a - b));
	}
	static bool less(IntContainer a, IntContainer b)
	{
		if (b > a)
			return (2 * b < 2 * a + mod);
		else
			return (2 * b + mod < 2 * a);
	}
#endif

	IntContainer val;

	// Constructeur rapide pour 0 <= initVal < mod
	static ModulusNumber<IntContainer, mod> forceToModNum(IntContainer initVal)
	{
		ModulusNumber<IntContainer, mod> nb;
		nb.val = initVal;
		return nb;
	}

public:
	ModulusNumber<IntContainer, mod>() : val(0)
	{
	}
	ModulusNumber<IntContainer, mod>(IntContainer initVal) : val(initVal % mod)
	{
		if (val < 0)
			val += mod;
	}
	ModulusNumber<IntContainer, mod>(const ModulusNumber<IntContainer, mod>& other) : val(other.val)
	{
	}

	// Un nombre invalide
	static ModulusNumber<IntContainer, mod> INVALID()
	{
		// On force la valeur du nombre à -1
		return forceToModNum(-1);
	}

	const ModulusNumber<IntContainer, mod>& operator=(const ModulusNumber<IntContainer, mod>& other)
	{
		val = other.val;
		return (*this);
	}
	void operator+=(const ModulusNumber<IntContainer, mod>& add)
	{
		// Opérateur optimisé grâce aux garanties de cette classe
		val += add.val;
		if (val >= mod)
			val -= mod;
	}
	void operator-=(const ModulusNumber<IntContainer, mod>& add)
	{
		(*this) += forceToModNum(mod - add.val);
	}
	ModulusNumber<IntContainer, mod> operator++()
	{
		(*this) += forceToModNum(1);
		return (*this);
	}
	ModulusNumber<IntContainer, mod> operator--()
	{
		(*this) += forceToModNum(mod - 1);
		return (*this);
	}
	ModulusNumber<IntContainer, mod> operator++(int)
	{
		ModulusNumber<IntContainer, mod> tmp(*this);
		(*this) += forceToModNum(1);
		return tmp;
	}
	ModulusNumber<IntContainer, mod> operator--(int)
	{
		ModulusNumber<IntContainer, mod> tmp(*this);
		(*this) += forceToModNum(mod - 1);
		return tmp;
	}
	ModulusNumber<IntContainer, mod> operator+(const ModulusNumber<IntContainer, mod>& other) const
	{
		ModulusNumber<IntContainer, mod> tmp(*this);
		tmp += forceToModNum(other.val);
		return tmp;
	}
	ModulusNumber<IntContainer, mod> operator-(const ModulusNumber<IntContainer, mod>& other) const
	{
		ModulusNumber<IntContainer, mod> tmp(*this);
		tmp += forceToModNum(mod - other.val);
		return tmp;
	}
	ModulusNumber<IntContainer, mod> operator+() const
	{
		return (*this);
	}
	ModulusNumber<IntContainer, mod> operator-() const
	{
		return forceToModNum(mod - val);
	}

	void operator*=(const ModulusNumber<IntContainer, mod>& other)
	{
		val = (val * other.val) % mod;
	}
	ModulusNumber<IntContainer, mod> operator*(const ModulusNumber<IntContainer, mod>& other) const
	{
		ModulusNumber<IntContainer, mod> tmp(*this);
		tmp *= other;
		return tmp;
	}
	void operator/=(const ModulusNumber<IntContainer, mod>& other)
	{
		val /= other.val;
	}
	ModulusNumber<IntContainer, mod> operator/(const ModulusNumber<IntContainer, mod>& other) const
	{
		ModulusNumber<IntContainer, mod> tmp(*this);
		tmp /= other;
		return tmp;
	}

	bool operator==(const ModulusNumber<IntContainer, mod>& other) const
	{
		// Opérateur optimisé grâce aux garanties de cette classe
		return (val == other.val);
	}
	bool operator!=(const ModulusNumber<IntContainer, mod>& other) const
	{
		return !((*this) == other);
	}

	// Détermine si cette valeur est l'opposée d'une autre
	bool isOpposite(const ModulusNumber<IntContainer, mod>& other) const
	{
		if (mod % 2 == 0)
		{
			IntContainer absDiff = (val > other.val ? val - other.val : other.val - val);
			return (absDiff == mod / 2);
		}
		else
			return false;
	}

	// Une valeur est inférieure à une autre si et seulement si le chemin le plus rapide 
	// depuis celle-ci pour arriver jusqu'à l'autre est d'incrémenter celle-ci.
	// Si les deux valeurs sont opposées, la plus petite des deux est la pus petite numériquement.
	bool operator<(const ModulusNumber<IntContainer, mod>& other) const
	{
		if (!isOpposite(other))
			return less(val, other.val);
		else
			return (val < other.val);
	}
	bool operator<=(const ModulusNumber<IntContainer, mod>& other) const
	{
		return (*this == other || *this < other);
	}
	bool operator>(const ModulusNumber<IntContainer, mod>& other) const
	{
		return !(*this <= other);
	}
	bool operator>=(const ModulusNumber<IntContainer, mod>& other) const
	{
		return !(*this < other);
	}

	explicit operator IntContainer() const
	{
		return val;
	}
	IntContainer v() const
	{
		return val;
	}

	// Distance entre deux valeurs, en nombre minimal d'incrémentations ou décrémentations sur l'une d'elles pour atteindre l'autre
	IntContainer distanceTo(const ModulusNumber<IntContainer, mod>& other) const
	{
		return distance(val, other.val);
	}
};

template<int mod>
using IntModulusNumber = ModulusNumber<int, mod>;

template<int mod, class IntContainer>
std::ostream& operator<<(std::ostream& os, ModulusNumber<IntContainer, mod> mnb)
{
	return (os << mnb.v());
}
template<int mod, class IntContainer>
std::istream& operator>>(std::istream& is, ModulusNumber<IntContainer, mod>& mnb)
{
	IntContainer val;
	is >> val;
	mnb = ModulusNumber<IntContainer, mod>(val);
	return is;
}

#endif
