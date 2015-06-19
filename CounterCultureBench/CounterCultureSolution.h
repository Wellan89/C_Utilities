#include <cstdio>
#include <cstdlib>
#include <string>
#include <algorithm>

// Solution du problème Counter Culture donné par Google

bool is_1_followed_by_0s(std::string S)
{
	std::reverse(S.begin(), S.end());
	return (atoi(S.c_str()) == 1);
}
unsigned long long cc_solve(unsigned long long N)
{
	if (N < 10)			// Trivial case.
		return N;

	char X[20];
	sprintf_s(X, "%lld", N);
	std::string S = X;
	size_t M = S.length();	// Number of digits of N.

	// Starts from 1.
	unsigned long long ans = 1;

	// Part 1: from 1, get to the M digits as fast as possible.
	unsigned long long p10[10];	// p10[i] == 10^i
	p10[0] = 1;
	for (int i = 1; i < 10; i++)
		p10[i] = p10[i - 1] * 10;
	for (size_t d = 1; d < M; d++)
	{
		// For digits = 7, it starts from 7 digits:    1000000
		ans += p10[(d + 1) / 2] - 1;	// Count up 9999: 1009999
		if (d > 1)						// Flip once:     9999001
			ans++;
		ans += p10[d / 2] - 1;			// Count up 999: 10000000
	}

	// Part 2:

	// Split N into two halves. For example N = "1234567"
	std::string L = S.substr(0, M / 2);	// L = "123"
	std::string R = S.substr(M / 2);	// R = "4567"

	// Handles the case where the right half is all zeroes.
	if (atoi(R.c_str()) == 0)
		return cc_solve(N - 1) + 1;

	// Special case: Count directly to the answer.
	if (is_1_followed_by_0s(L))
		return ans + atoi(R.c_str());

	// Count until the right half looks like the left half of N
	// in reverse. In this case, count from 1000000 to 1000321.
	std::reverse(L.begin(), L.end());
	ans += atoi(L.c_str());

	// Flip 1000321 to 1230001.
	ans++;

	// Count up 4566 to the target from 1230001 to 1234567.
	ans += atoi(R.c_str()) - 1;
	return ans;
}
