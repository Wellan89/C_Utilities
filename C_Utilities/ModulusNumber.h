#ifndef DEF_MODULUS_NUMBER
#define DEF_MODULUS_NUMBER

// Un nombre modulo mod (un élément de Z/mod.Z)
template<int mod>
class ModulusNumber
{
protected:
	// Version optimisée de distance(0, a)
	static int distanceFrom0(int a)
	{
		int distIncr = a;
		int distDecr = mod - a;
		return min(distIncr, distDecr);
	}
#if 0
	// Versions simples
	static int distance(int a, int b)
	{
		int distIncr = (b + mod - a) % mod;
		int distDecr = (a + mod - b) % mod;
		return min(distIncr, distDecr);
	}
	static int less(int a, int b)
	{
		int distIncr = (b + mod - a) % mod;
		int distDecr = (a + mod - b) % mod;
		return (distIncr < distDecr);
	}
#else
	// Versions optimisées
	static int distance(int a, int b)
	{
		return distanceFrom0(a < b ? (b - a) : (a - b));
	}
	static bool less(int a, int b)
	{
		if (b > a)
			return (2 * b < 2 * a + mod);
		else
			return (2 * b + mod < 2 * a);
	}
#endif

	int val;

	// Constructeur rapide pour 0 <= initVal < mod
	static ModulusNumber<mod> forceToModNum(int initVal)
	{
		ModulusNumber<mod> nb;
		nb.val = initVal;
		return nb;
	}

public:
	ModulusNumber<mod>() : val(0)
	{
	}
	explicit ModulusNumber<mod>(int initVal) : val(initVal % mod)
	{
		if (val < 0)
			val += mod;
	}
	ModulusNumber<mod>(const ModulusNumber<mod>& other) : val(other.val)
	{
	}

	// Un nombre invalide
	static ModulusNumber<mod> INVALID()
	{
		// On force la valeur du nombre à -1
		return forceToModNum(-1);
	}

	const ModulusNumber<mod>& operator=(const ModulusNumber<mod>& other)
	{
		val = other.val;
		return (*this);
	}
	void operator+=(const ModulusNumber<mod>& add)
	{
		// Opérateur optimisé
		val += add.val;
		if (val >= mod)
			val -= mod;
	}
	void operator-=(const ModulusNumber<mod>& add)
	{
		(*this) += forceToModNum(mod - add.val);
	}
	void operator+=(int add)
	{
		(*this) += ModulusNumber<mod>(add);
	}
	void operator-=(int add)
	{
		(*this) += ModulusNumber<mod>(-add);
	}
	ModulusNumber<mod> operator++()
	{
		(*this) += forceToModNum(1);
		return (*this);
	}
	ModulusNumber<mod> operator--()
	{
		(*this) += forceToModNum(mod - 1);
		return (*this);
	}
	ModulusNumber<mod> operator++(int)
	{
		ModulusNumber<mod> tmp(*this);
		(*this) += forceToModNum(1);
		return tmp;
	}
	ModulusNumber<mod> operator--(int)
	{
		ModulusNumber<mod> tmp(*this);
		(*this) += forceToModNum(mod - 1);
		return tmp;
	}
	ModulusNumber<mod> operator+(const ModulusNumber<mod>& other) const
	{
		ModulusNumber<mod> tmp(*this);
		tmp += forceToModNum(other.val);
		return tmp;
	}
	ModulusNumber<mod> operator-(const ModulusNumber<mod>& other) const
	{
		ModulusNumber<mod> tmp(*this);
		tmp += forceToModNum(mod - other.val);
		return tmp;
	}
	ModulusNumber<mod> operator+(int add) const
	{
		ModulusNumber<mod> tmp(*this);
		tmp += ModulusNumber<mod>(add);
		return tmp;
	}
	ModulusNumber<mod> operator-(int add) const
	{
		ModulusNumber<mod> tmp(*this);
		tmp += ModulusNumber<mod>(-add);
		return tmp;
	}
	ModulusNumber<mod> operator+() const
	{
		return (*this);
	}
	ModulusNumber<mod> operator-() const
	{
		return forceToModNum(mod - val);
	}

	void operator*=(int mult)
	{
		val = (val * mult) % mod;
		if (val < 0)
			val += mod;
	}
	void operator*=(const ModulusNumber<mod>& other)
	{
		(*this) *= other.val;
	}
	ModulusNumber<mod> operator*(int mult) const
	{
		ModulusNumber<mod> tmp(*this);
		tmp *= mult;
		return tmp;
	}
	ModulusNumber<mod> operator*(const ModulusNumber<mod>& other) const
	{
		return ((*this) * other.val);
	}
	void operator/=(int div)
	{
		val = (val / div) % mod;
		if (val < 0)
			val += mod;
	}
	void operator/=(const ModulusNumber<mod>& other)
	{
		(*this) /= other.val;
	}
	ModulusNumber<mod> operator/(int div) const
	{
		ModulusNumber<mod> tmp(*this);
		tmp /= div;
		return tmp;
	}
	ModulusNumber<mod> operator/(const ModulusNumber<mod>& other) const
	{
		return ((*this) * other.val);
	}

	bool operator==(const ModulusNumber<mod>& other) const
	{
		// Opérateur optimisé
		return (val == other.val);
	}
	bool operator!=(const ModulusNumber<mod>& other) const
	{
		return !((*this) == other);
	}
	bool operator==(int c) const
	{
		return ((*this) == ModulusNumber<mod>(c));
	}
	bool operator!=(int c) const
	{
		return !((*this) == ModulusNumber<mod>(c));
	}

	// Détermine si cette valeur est l'opposée d'une autre
	bool isOpposite(const ModulusNumber<mod>& other) const
	{
		if (mod % 2 == 0)
			return (abs(val - other.val) == mod / 2);
		else
			return false;
	}

	// Une valeur est inférieure à une autre ssi le chemin le plus rapide pour arriver jusqu'à l'autre est d'incrémenter celle-ci.
	// Si les deux valeurs sont opposées, la plus petite des deux est la pus petite numériquement.
	bool operator<(const ModulusNumber<mod>& other) const
	{
		if (!isOpposite(other))
			return less(val, other.val);
		else
			return (val < other.val);
	}
	bool operator<=(const ModulusNumber<mod>& other) const
	{
		return (*this == other || *this < other);
	}
	bool operator>(const ModulusNumber<mod>& other) const
	{
		return !(*this <= other);
	}
	bool operator>=(const ModulusNumber<mod>& other) const
	{
		return !(*this < other);
	}

	explicit operator int() const
	{
		return val;
	}
	int v() const
	{
		return val;
	}

	// Distance entre deux valeurs, en nombre minimal d'incrémentations ou décrémentations sur l'une d'elles pour atteindre l'autre
	int distanceTo(const ModulusNumber<mod>& other) const
	{
		return distance(val, other.val);
	}
};

#endif
