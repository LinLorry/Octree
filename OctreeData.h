#ifndef OCTREE_DATA_H
#define OCTREE_DATA_H

#include <OctreeBase.h>

namespace Octree
{
    struct OctreeRoot
    {
        double xMin, xMax;
        double yMin, yMax;
        double zMin, zMax;

        OctreeRoot() { }

        OctreeRoot(const OctreeRoot & root) : 
            xMin(root.xMin), xMax(root.xMax),
            yMin(root.yMin), yMax(root.yMax),
            zMin(root.zMin), zMax(root.zMax) { }

        OctreeRoot(OctreeRoot && root) :
            xMin(root.xMin), xMax(root.xMax),
            yMin(root.yMin), yMax(root.yMax),
            zMin(root.zMin), zMax(root.zMax) { }

        OctreeRoot(               
            double xMin, double xMax,
            double yMin, double yMax,
            double zMin, double zMax
        ) :
            xMin(xMin), xMax(xMax),
            yMin(yMin), yMax(yMax),
            zMin(xMin), zMax(xMax) { }

        void operator=(const OctreeRoot & root)
        {
            xMin = root.xMin;
            xMax = root.xMax;
            yMin = root.yMin;
            yMax = root.yMax;
            zMin = root.zMin;
            zMax = root.zMax;
        }

        void operator=(OctreeRoot && root)
        {
            xMin = root.xMin;
            xMax = root.xMax;
            yMin = root.yMin;
            yMax = root.yMax;
            zMin = root.zMin;
            zMax = root.zMax;
        }
    };

    struct OctreeLeave
    {
        double x, y, z;

        OctreeLeave() { }
        
        OctreeLeave(const OctreeLeave & leave) : 
            x(leave.x), y(leave.y), z(leave.z) { }

        OctreeLeave(OctreeLeave && leave) :
            x(leave.x), y(leave.y), z(leave.z) { }

        OctreeLeave(const double x, const double y, const double z) :
            x(x), y(y), z(z) { }


        void operator=(const OctreeLeave & leave)
        {
            x = leave.x;
            y = leave.y;
            z = leave.z;
        }

        void operator=(OctreeLeave && leave)
        {
            x = leave.x;
            y = leave.y;
            z = leave.z;
        }
    };
} // namespace Octree


#endif // OCTREE_DATA_H