/*
Weekend Contest

E. Extravagant Journey

link to the problem:
find problems.pdf
*/
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <deque>
#include <stack>
#include <utility>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <climits>
#include <cstdlib>
#include <fstream>
# define ull unsigned long long
# define ll long long
ll mod = 1e9 + 7;
using namespace std;
// void setIO(string s) {
// 	freopen((s + ".in").c_str(), "r", stdin);
// 	freopen((s + ".out").c_str(), "w", stdout);
// }
// ll power(ll x, ll p)
// {
// 	if (p == 0) return (1);
// 	ll res = power(x, p/2);
// 	if (p%2)
// 		return ((res * res) * x);
// 	return (res * res);
// }
// ll gcd(ll a, ll b)
// {
// 	if (b == 0) return (a);
// 	return (gcd(b, a%b));
// }
// ll lcm(ll a, ll b)
// {
// 	return ((a*b)/gcd(a, b));
// }

ll n;

map< pair<ll, ll>, ll > dp;

ll dfs(vector< pair<ll, ll> > &a, ll v, ll i = 0)
{
	if (i >= a.size())
		return (0);

	if (dp.find({v, i}) != dp.end())
		return dp[{v, i}];

	if (v - a[i].first < 0)
		return (dp[{v, i}] = dfs(a, v, i + 1));

	return dp[{v, i}] = max(a[i].second + dfs(a, v - a[i].first, i + 1), dfs(a, v, i + 1));
}

void solve()
{
	ll v;
	cin >> n >> v;

	vector< pair<ll, ll> > a(n);

	for (auto &[f, s] : a)
		cin >> f;
	for (auto &[f, s] : a)
		cin >> s;

	cout << dfs(a, v) << "\n";
}



int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	solve();
}