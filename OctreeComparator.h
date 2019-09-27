#ifndef OCTREE_COMPARATOR_H
#define OCTREE_COMPARATOR_H

#include <OctreeComparable.h>

namespace Octree
{
    enum Location 
    {
        outside = 0,
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

    const Location OctreeComparator(
        const OctreeComparable & first, const OctreeComparable & second)
    {
        if (first.type == leave && second.type == root)
            return OctreeComparator(second, first);
        else if (
            (first.type == leave && second.type == leave) || 
            (first.type == root && second.type == root)
        ) return undefine;
        else
        {
            if (
                first.data.root.xMax < second.data.leave.x || 
                first.data.root.xMin > second.data.leave.x ||
                first.data.root.yMax < second.data.leave.y || 
                first.data.root.yMin > second.data.leave.y ||
                first.data.root.zMax < second.data.leave.z ||
                first.data.root.zMin > second.data.leave.z
            ) return outside;
            else
            {
                auto xTmp = (first.data.root.xMax + first.data.root.xMax) / 2;
                auto yTmp = (first.data.root.yMax + first.data.root.yMax) / 2;
                auto zTmp = (first.data.root.zMax + first.data.root.zMax) / 2;

                if (second.data.leave.x > xTmp)
                {
                    if (second.data.leave.y > yTmp)
                    {
                        if (second.data.leave.z > zTmp)
                            return xPositive_yPositive_zPositive;
                        return xPositive_yPositive_zNegative;
                    }
                    if (second.data.leave.z > zTmp)
                        return xPositive_yNegative_zPositive;
                    return xPositive_yNegative_zNegative;
                }
                if (second.data.leave.y > yTmp)
                {
                    if (second.data.leave.z > zTmp)
                        return xNegative_yPositive_zPositive;
                    return xNegative_yPositive_zNegative;
                }
                if (second.data.leave.z > zTmp)
                    return xNegative_yNegative_zPositive;
                return xNegative_yNegative_zNegative;
            }
        }        
    }
} // namespace Octree


#endif // OCTREE_COMPARATOR_H