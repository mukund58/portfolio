#include <bits/stdc++.h>
using namespace std;
void solve() {
  int x, k;
  cin >> x >> k;
  if (x < k) {
    cout << 1 << endl;
    cout << x << endl;
    return;
  } else {
    int temp = x;
    while (x != 0) {
      if (x % k != 0) {
	if((temp-x)==0){
	cout << 1 << endl;
	cout<<x<<endl;
	break;
	}
        if ((temp - x) % k != 0) {

          cout << 2 << endl;
          cout << x<<" ";
          cout << temp - x << endl;
          break;
        }
      }
      x--;
    }
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
