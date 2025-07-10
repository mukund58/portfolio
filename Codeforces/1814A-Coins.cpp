#include <bits/stdc++.h>
using namespace std;
void solve() {
  long long n, k;
  cin >> n >> k;
  if (n % 2 != 0) {
    if ((n - k) % 2 == 0 ){
		cout<<"Yes"<<endl;
	}
    else
      cout << "NO" << endl;
  } else {
    cout << "Yes" << endl;
  }
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

