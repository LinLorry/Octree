#ifndef OCTREE_BASE_H
#define OCTREE_BASE_H

namespace Octree
{
    enum NodeType {root, leave};

    enum Result 
    {
        outside = 0,
        equal,
        xPositive_yPositive_zPositive,
        xPositive_yPositive_zNegative,
        xPositive_yNegative_zNegative,
        xPositive_yNegative_zPositive,
        xNegative_yPositive_zPositive,
        xNegative_yPositive_zNegative,
        xNegative_yNegative_zNegative,
        xNegative_yNegative_zPositive,
        undefine
    };
} // namespace Octree
#endif // OCTREE_BASE_H