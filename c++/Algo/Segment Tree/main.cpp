#include <bits/stdc++.h>

class SegmetTree
{
private:
    int len;
    std::vector<int> t;

public:
    SegmetTree(size_t l) : len(l)
    {
        t.resize(4 * len);
    }
    SegmetTree() {}

    void update(int v, int tl, int tr, int id, int val)
    {
        // if leaf node reached;

        if (tl == tr)
        {
            t[v] = val;
            return;
        }
        // out of bound ;

        if (id > tr || id < tl)
        {
            return; // identity transformation;
        }

        // partial overlap;

        int tm = (tl + tr) / 2;
        update(2 * v, tl, tm, id, val);
        update(2 * v + 1, tm + 1, tr, id, val);

        // update the effected node on changing one value
        t[v] = t[2 * v] + t[2 * v + 1];
    }

    // void build(veector<int>&inputvector,int index/vertices,int starting index (0), int ending index ( len-1))
    // time complexity O(n) dfs;
    // spcae complexity O(4n);
    void build(std::vector<int> &a, int v, int tl, int tr)
    {
        if (tl == tr)
        {
            // Fill the leaf node in the treeArray
            t[v] = a[tl];
            return;
        }
        int tm = (tl + tr) / 2;
        // Go for first and second child dfs
        build(a, 2 * v, tl, tm);
        build(a, 2 * v + 1, tm + 1, tr);
        // Fill the non leaf node;
        t[v] = t[2 * v] + t[2 * v + 1];
    }

    int query(int v, int tl, int tr, int l, int r)
    {
        // we will have 3 condition

        // No over lap return identity value;
        if (tl > r || tr < l)
            return 0;
        // complete overlap return t[v];
        if (tl >= l && tr <= r)
            return t[v];
        // partial overlap call the itself (recursion) on first and second half
        int tm = (tl + tr) / 2;
        int leftAnswer = query(2 * v, tl, tm, l, r);
        int rightAnswer = query(2 * v + 1, tm + 1, tr, l, r);

        return rightAnswer + leftAnswer; // apply the operation here what we want range sum , max, min etc ...
    }


    void build(std::vector<int>& a){
        build(a,1,0,len-1);
    }

    int query(int l, int r){
        return query(1,0,len-1,l,r);
    }

    void update(int id, int val){
        update(1,0,len-1,id,val);
    }
};
int main()
{
    // Update any thing and or xor gcd
    // Query any thing
    // here any thing men associative operation
    // max(a,max(b,c)) == max(max(a,b),b)
    // if any c++ fun is associative we can apply it

    std::vector<int> inputvector = {72, 23, 45, 54, 25, 764, 31, 64, 32, 64, 12, 65, 43, 56, 43, 9};
    int n = inputvector.size();
    // arrry must be size of 2^n
    // we will store the array in the leaf node of tree and the parent have the required operation
    // and the tree will aslo store in the array (treeArray) formate 4n n is size of the input array
    // 1 base indexing
    // v is the 1 for the root and index on treeArray
    // left child  = 2*v
    // right child =  2*v+1
    // _,root,left1,right1,left21,right21

    // -----------------bUILD THE Segment tree

    SegmetTree segmentTree(n);
    segmentTree.build(inputvector);

    // Point Query each element 
    for(int i=0; i<n;i++){
        std::cout<<segmentTree.query(i,i)<<" ";
    }

    std::cout<<std::endl;


    //------------------qUERY the Segment tree
    // Sumquery(int v,int 0,int lr,int l, int r)

    int sum = segmentTree.query(1,1);
    std::cout<<"sum query 1 5 "<<sum<<std::endl;

    segmentTree.update(8,5);

    for(int i=0; i<n;i++){
        std::cout<<segmentTree.query(i,i)<<" ";
    }

    std::cout<<std::endl;

    return 0;
}