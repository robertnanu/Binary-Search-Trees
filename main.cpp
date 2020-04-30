#include <bits/stdc++.h>
using namespace std;
 
using namespace std;
 
ifstream f ("abce.in");
ofstream g ("abce.out");
 
class Node
{
private:
    void splitChild(int, Node*);
 
    void takePrev(int);
    void takeNext(int);
 
    void combine(int);
    void stuff(int);
 
    int getPred(int);
    int getSucc(int);
 
    void removeLeaf(int);
    void removeNonLeaf(int);
 
protected:
    vector <int> keys;
    vector <Node*> boys;
 
    const int t;
    int n;
    bool isLeaf;
 
public:
 
    explicit Node(int = 3, bool = true);
 
    void traverse(vector<int>&);
    void traverse(vector<int>&, int, int);
    int search(int);
    int find_low(int, int = 0);
    int find_upper(int, int = 0);
    bool exist(int);
 
    bool isFull();
 
    void appendChild(int);
    void removeBoy(int);
 
    friend class BTree;
};
 
class BTree
{
    Node *root;
    const int t;
 
public:
    BTree(int = 3);
 
    int search(int, bool);
    int search(int);
 
    bool exist(int);
 
    vector <int> traverse();
    vector <int> traverse(int, int);
 
    void print(const string& = " ");
    void print(int, int, const string& = " ");
 
    void insert(int);
    void insert(vector <int>);
    void remove(int);
};
 
Node::Node(int t, bool isLeaf): t(t), isLeaf(isLeaf), n(0)
{
    keys.resize(2*t+1);
    boys.resize(2*t);
}
 
void Node::removeBoy(int x)
{
 
    int idx=0;
    while(idx < n && keys[idx] < x) ++idx;
 
    if(idx < n && keys[idx] == x)
    {
        if(isLeaf) removeLeaf(idx);
        else removeNonLeaf(idx);
    }
 
    else
    {
        if(isLeaf) return;
 
        bool flag = (idx==n);
 
        if(boys[idx]->n < t) stuff(idx);
 
        if(flag && idx > n) --idx;
        boys[idx]->removeBoy(x);
    }
}
 
bool Node::isFull()
{
    return n == t * 2 - 1;
}
 
void Node::removeLeaf(int idx)
{
    for (int i = idx + 1; i < n; ++i) keys[i-1] = keys[i];
    --n;
}
 
void Node::removeNonLeaf(int idx)
{
    int x = keys[idx];
 
    if(boys[idx]->n >= t)
    {
        int pred = getPred(idx);
        keys[idx] = pred;
        boys[idx]->removeBoy(pred);
    }
 
    else if(boys[idx + 1]->n >= t)
    {
        int succ = getSucc(idx);
        keys[idx] = succ;
        boys[idx + 1]->removeBoy(succ);
    }
 
    else
    {
        combine(idx);
        boys[idx]->removeBoy(x);
    }
}
 
int Node::getPred(int idx)
{
    Node *cur = boys[idx];
    while(!cur->isLeaf) cur = cur->boys[cur->n];
 
    return cur->keys[cur->n-1];
}
 
int Node::getSucc(int idx)
{
    Node *cur = boys[idx + 1];
    while(!cur->isLeaf) cur = cur->boys[0];
    return cur->keys[0];
}
 
void Node::stuff(int idx)
{
    if (idx != 0 && boys[idx - 1]->n >= t) takePrev(idx);
    else if (idx != n && boys[idx + 1]->n >= t) takeNext(idx);
    else
    {
        if(idx != n) combine(idx);
        else combine(idx - 1);
    }
}
 
void Node::takePrev(int idx)
{
    Node *child = boys[idx];
    Node *bro = boys[idx-1];
 
    for(int i = child->n-1; i >= 0; --i) child->keys[i+1] = child->keys[i];
 
    if(!child->isLeaf)
        for(int i=child->n; i>=0; --i) child->boys[i + 1] = child->boys[i];
 
    child->keys[0] = keys[idx-1];
 
    if(!child->isLeaf) child->boys[0] = bro->boys[bro->n];
 
    keys[idx-1] = bro->keys[bro->n-1];
 
    child->n++;
    bro->n--;
}
 
void Node::takeNext(int idx)
{
    Node *child=boys[idx];
    Node *bro=boys[idx+1];
 
    child->keys[(child->n)] = keys[idx];
 
    if(!(child->isLeaf)) child->boys[(child->n) + 1] = bro->boys[0];
 
    keys[idx] = bro->keys[0];
 
    for(int i = 1; i < bro->n; ++i) bro->keys[i-1] = bro->keys[i];
 
    if(!bro->isLeaf)
        for(int i = 1; i <= bro->n; ++i) bro->boys[i-1] = bro->boys[i];
 
    child->n++;
    bro->n--;
}
 
void Node::combine(int idx)
{
    Node *child = boys[idx];
    Node *bro = boys[idx+1];
 
    child->keys[t - 1] = keys[idx];
 
    for(int i = 0; i < bro->n; ++i) child->keys[i+t] = bro->keys[i];
 
    if (!child->isLeaf)
        for(int i = 0; i <= bro->n; ++i) child->boys[i + t] = bro->boys[i];
 
    for (int i = idx + 1; i < n; ++i) keys[i - 1] = keys[i];
 
    for (int i = idx + 2; i <= n; ++i) boys[i - 1] = boys[i];
 
    child->n += bro->n+1;
    n--;
 
    delete(bro);
}
 
void BTree::insert(int x)
{
    if(root == nullptr)
    {
        root = new Node(t, true);
        root->keys[0] = x;
        root->n = 1;
        return;
    }
    if(root->isFull())
    {
        Node *s = new Node(t, false);
 
        s->boys[0] = root;
        s->splitChild(0, root);
 
        int i = 0;
        if(s->keys[0] < x) ++i;
 
        s->boys[i]->appendChild(x);
 
        root = s;
        return;
    }
    root->appendChild(x);
}
 
void BTree::insert(vector <int> values)
{
    for(auto x: values) insert(x);
}
 
void Node::appendChild(int x)
{
    int i = n - 1;
 
    if (isLeaf)
    {
        while (i >= 0 && keys[i] > x)
        {
            keys[i+1] = keys[i];
            --i;
        }
        keys[i + 1] = x;
        n++;
        return;
    }
    while(i >= 0 && keys[i] > x) --i;
    ++i;
    if(boys[i]->isFull())
    {
        splitChild(i, boys[i]);
        if (keys[i] < x) ++i;
    }
    boys[i]->appendChild(x);
}
 
void Node::splitChild(int i, Node *y)
{
    Node *z = new Node(y->t, y->isLeaf);
    z->n = t - 1;
 
    for (int j = 0; j < t - 1; ++j) z->keys[j] = y->keys[j + t];
 
    if (!y->isLeaf)
        for (int j = 0; j < t; ++j) z->boys[j] = y->boys[j + t];
 
    y->n = t - 1;
 
    for (int j = n; j >= i + 1; --j) boys[j + 1] = boys[j];
 
    boys[i + 1] = z;
 
    for (int j = n-1; j >= i; j--) keys[j + 1] = keys[j];
    keys[i] = y->keys[t - 1];
 
    n++;
}
 
void Node::traverse(vector <int> &values)
{
    int i;
    for (i = 0; i < n; ++i)
    {
        if (!isLeaf) boys[i]->traverse(values);
        values.push_back(keys[i]);
    }
    if (!isLeaf) boys[i]->traverse(values);
}
 
void Node::traverse(vector<int> &values, int x, int y)
{
    int i;
    for(i = 0; i < n; ++i)
    {
        if (!isLeaf) boys[i]->traverse(values, x, y);
        if(keys[i] >= x && keys[i] <= y) values.push_back(keys[i]);
    }
    if(!isLeaf) boys[i]->traverse(values, x, y);
}
 
int Node::search(int x)
{
    int i = 0;
    while(i < n && x > keys[i]) ++i;
    if(keys[i] == x) return keys[i];
    if(isLeaf) return 0;
    return boys[i]->search(x);
}
 
BTree::BTree(int t): root(nullptr), t(t){}
 
int BTree::search(int x)
{
    if(root== nullptr) return 0;
    return root->search(x);
}
 
bool BTree::exist(int x)
{
    if(root == nullptr) return false;
    return root->exist(x);
}
 
int BTree::search(int x, bool lower)
{
    if(root == nullptr) return 0;
    if(lower) return root->find_low(x);
    return root->find_upper(x);
}
 
void BTree::remove(int x)
{
    root->removeBoy(x);
 
    if(root->n==0)
    {
        Node *tmp = root;
        if(root->isLeaf) root = NULL;
        else root = root->boys[0];
        delete tmp;
    }
}
 
vector <int> BTree::traverse()
{
    vector<int> values;
    if(root!= nullptr) root->traverse(values);
    return values;
}
 
vector <int> BTree::traverse(int x, int y)
{
    vector <int> values;
    if(root!= nullptr) root->traverse(values, x, y);
    return values;
}
 
void BTree::print(const string& sep)
{
    vector<int> values = traverse();
    bool alpha = false;
    for(auto x: values)
    {
        if(alpha) g<<sep;
        alpha = true;
        g<<x;
    }
    g<<'\n';
}
 
void BTree::print(int x, int y, const string& sep)
{
    vector <int> values = traverse(x, y);
    bool alpha = false;
    for(auto x: values)
    {
        if(alpha) g<<sep;
        alpha = true;
        g<<x;
    }
    g<<'\n';
}
 
bool Node::exist(int x)
{
    int i = 0;
    while(i < n && x > keys[i]) ++i;
    if(i < n && keys[i]==x) return true;
    if(isLeaf) return false;
    return boys[i]->exist(x);
}
 
int Node::find_upper(int x, int last)
{
    int i = 0;
    while (i < n && x > keys[i]) ++i;
    if(i<n)
    {
        if(keys[i] == x) return keys[i];
        last = keys[i];
    }
    if (isLeaf) return last;
 
    return boys[i]->find_upper(x, last);
}
 
int Node::find_low(int x, int last)
{
    int i = 0;
    while (i < n && x > keys[i]) ++i;
 
    if(i<n)
        if (keys[i] == x) return keys[i];
 
    if(i>0) last = keys[i - 1];
    if(isLeaf)
        return last;
 
    return boys[i]->find_low(x, last);
}
 
void execute(BTree *b, int op, int x, int y = 0)
{
    if(op == 1) b->insert(x);
 
    if(op == 2) b->remove(x);
 
    if(op == 3) g<<(int)b->exist(x)<<'\n';
 
    if(op == 4) g<<b->search(x, true)<<'\n';
 
    if(op == 5) g<<b->search(x, false)<<'\n';
 
    if(op == 6) b->print(x, y);
}
 
int main()
{
    int n, op, x, y;
    BTree b(3);
    f>>n;
    while(n--)
    {
        f>>op>>x;
        if(op == 6) f>>y;
        execute(&b, op, x, y);
    }
    return 0;
}
