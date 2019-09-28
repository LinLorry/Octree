#ifndef OCTREE_COMPARATOR_H
#define OCTREE_COMPARATOR_H

#include <OctreeComparable.h>

namespace Octree
{
    const Result OctreeComparator(
        const OctreeComparable<> & first, const OctreeComparable<> & second)
    {

        if (first.getType() == leave)
        {
            if (second.getType() == root)
                return OctreeComparator(second, first);
            
            if (
                first.getLeaveX() == second.getLeaveX() &&
                first.getLeaveY() == second.getLeaveY() &&
                first.getLeaveZ() == second.getLeaveZ()
            ) return equal;

            return undefine;
        }
        
        if (second.getType() == root) return undefine;

        if (
            first.getRootXMax() < second.getLeave().x || 
            first.getRootXMin() > second.getLeave().x ||
            first.getRootYMax() < second.getLeave().y || 
            first.getRootYMin() > second.getLeave().y ||
            first.getRootZMax() < second.getLeave().z ||
            first.getRootZMin() > second.getLeave().z
        ) return outside;
        else
        {
            auto xTmp = (first.getRootXMax() + first.getRootXMin()) / 2;
            auto yTmp = (first.getRootYMax() + first.getRootYMin()) / 2;
            auto zTmp = (first.getRootZMax() + first.getRootZMin()) / 2;

            if (second.getLeaveX() > xTmp)
            {
                if (second.getLeaveY() > yTmp)
                {
                    if (second.getLeaveZ() > zTmp)
                        return xPositive_yPositive_zPositive;
                    return xPositive_yPositive_zNegative;
                }
                if (second.getLeaveZ() > zTmp)
                    return xPositive_yNegative_zPositive;
                return xPositive_yNegative_zNegative;
            }
            if (second.getLeaveY() > yTmp)
            {
                if (second.getLeaveZ() > zTmp)
                    return xNegative_yPositive_zPositive;
                return xNegative_yPositive_zNegative;
            }
            if (second.getLeaveZ() > zTmp)
                return xNegative_yNegative_zPositive;
            return xNegative_yNegative_zNegative;
        } 
    }
} // namespace Octree


#endif // OCTREE_COMPARATOR_H