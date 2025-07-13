#include <bits/stdc++.h>
#include <numeric>
using namespace std;

void solve() {
  int n;
  cin >> n;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  int flag = 0;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (gcd(a[i], a[j]) <= 2) {
        flag = 1;
      }
    }
  }
  if (flag != 0) {
    cout << "YES\n";
  } else {
    cout << "NO\n";
  }
  return;
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    solve();
  }

  return 0;
}
