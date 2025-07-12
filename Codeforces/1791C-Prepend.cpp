#include <bits/stdc++.h>
using namespace std;
void solve() {
  long long  n;
  cin >> n;

  string arr;
  cin >> arr;
  int left = 0;
  int right = n - 1;

  while (left < right) {
    if ((arr[left] == '1'  &&  arr[right] == '0' )|| ((arr[left] == '0' && arr[right] == '1'))) {
      n--;
      n--;
      left++;
      right--;
    } else break;
  }
  cout << n << endl;
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
