#include <Octree.h>

#include <iostream>

int main()
{
    Octree::Octree<> tree(0,1, 0, 1, 0,1);

    Octree::Octree<>::Node a(Octree::Octree<>::Leave(0.1, 0.1, 0.1));
    Octree::Octree<>::Node b(Octree::Octree<>::Leave(0.1, 0.1, 0.2));
    Octree::Octree<>::Node c(Octree::Octree<>::Leave(0.6, 0.3, 0.1));
    Octree::Octree<>::Node d(Octree::Octree<>::Leave(0.6, 0.3, 0.2));
    Octree::Octree<>::Node e(Octree::Octree<>::Leave(0.1, 0.2, 0.1));
    Octree::Octree<>::Node f(Octree::Octree<>::Leave(0.1, 0.1, 0.11));
    Octree::Octree<>::Node g(Octree::Octree<>::Leave(0.1, 0.1, 0.111));
    Octree::Octree<>::Node h(Octree::Octree<>::Leave(0.1, 0.1, 0.1111));
    Octree::Octree<>::Node i(Octree::Octree<>::Leave(0.1, 0.1, 0.1));

    tree.insert(a);
    tree.insert(b);
    tree.insert(c);
    tree.insert(d);
    tree.insert(e);
    tree.insert(f);
    tree.insert(g);
    tree.insert(h);
    tree.insert(i);

    Octree::Octree<>::NodePoint j = tree.find(a);
    Octree::Octree<>::NodePoint k = tree.find(b);
    Octree::Octree<>::NodePoint l = tree.find(c);
    Octree::Octree<>::NodePoint m = tree.find(i);

    tree.remove(a);
    tree.remove(b);
    tree.remove(i);

}