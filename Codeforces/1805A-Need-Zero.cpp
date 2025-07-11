#include <bits/stdc++.h>
using namespace std;
void solve() {
  int n;
  cin >> n;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  int z = 0;
  for (int i = 0; i < n; i++) {
    z ^= a[i];
  }
  if (n % 2 == 1) {
    cout << z << endl;
  } else {
    if (z == 0) {
      cout << z << endl;
    } else {
      cout << -1 << endl;
    }
  }
}
int main() {
  int tc;
  cin >> tc;
  while (tc--) {
    solve();
  }
}
