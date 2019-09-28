#include <Octree.h>

int main()
{
    Octree::Octree<> tree(0,1, 0, 1, 0,1);
    Octree::Octree<double>::Comparable c;
    c.type = Octree::Octree<>::NodeType::leave;
    c.data.leave.x = 0.1;
    c.data.leave.y = 0.1;
    c.data.leave.z = 0.1;

    tree.insert(c);

    tree.insert(c);

    Octree::Octree<>::NodePoint p;
    p = tree.find(c);
    if (p == nullptr)
        std::cout << "NO" << std::endl;
    std::cout << p->data.data.leave.x << std::endl;
}