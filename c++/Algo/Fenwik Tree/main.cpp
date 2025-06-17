#include <bits/stdc++.h>
using namespace std;

class FenwickTree {
private:
    vector<int> bit;
    size_t n;

public:
    FenwickTree(size_t size) : n(size), bit(size + 2, 0) {}


    // Add 'delta' to index 'idx' (1-based)
    void point_update(int idx, int delta) {
        while (idx <= n) {
            bit[idx] += delta;
            idx += idx & -idx;
        }
    }

    // Prefix sum [1..idx]
    int prefix_query(int idx) {
        int sum = 0;
        while (idx > 0) {
            sum += bit[idx];
            idx -= idx & -idx;
        }
        return sum;
    }

    // Range sum [l..r] (1-based)
    int range_query(int l, int r) {
        return prefix_query(r) - prefix_query(l - 1);
    }

    // Range Update + Point Query Version -----------------------

    // Add 'delta' to range [l..r] (1-based)
    void range_update(int l, int r, int delta) {
        point_update(l, delta);
        point_update(r + 1, -delta);
    }

    // Get value at index 'idx' (1-based)
    int point_query(int idx) {
        return prefix_query(idx);
    }

    // Utility function to print the tree (for debugging)
    void print() {
        cout << "Fenwick Tree: [";
        for (int i = 1; i <= n; i++) {
            if (i > 1) cout << ", ";
            cout << point_query(i);
        }
        cout << "]" << endl;
    }
};

int main() {
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int n = arr.size();
    FenwickTree ft(n);
    // for (int i = 0; i < n; i++) {
    //     ft.point_update(i + 1, arr[i]);
    // }
    ft.print();  
    // ft.point_update(5, 10);
    // ft.print();
    // cout << "Sum of elements [3..7]: " << ft.range_query(3, 7) << endl;
    // cout << "Value at index 4: " << ft.point_query(4) << endl;

    ft.range_update(4, 8, 5);
    ft.print();
    // cout << "Sum of elements [3..7]: " << ft.range_query(3, 7) << endl;
    // cout << "Value at index 4: " << ft.point_query(4) << endl;
    return 0;
}