#ifndef OCTREE_COMPARABLE_H
#define OCTREE_COMPARABLE_H

namespace Octree
{
    enum NodeType {root, leave};

    struct Root
    {
        double xMin, xMax;
        double yMin, yMax;
        double zMin, zMax;

        Root() { }

        Root(const Root & root) : 
            xMin(root.xMin), xMax(root.xMax),
            yMin(root.yMin), yMax(root.yMax),
            zMin(root.zMin), zMax(root.zMax) { }

        Root(Root && root) :
            xMin(root.xMin), xMax(root.xMax),
            yMin(root.yMin), yMax(root.yMax),
            zMin(root.zMin), zMax(root.zMax) { }

        Root(               
            double xMin, double xMax,
            double yMin, double yMax,
            double zMin, double zMax
        ) :
            xMin(xMin), xMax(xMax),
            yMin(yMin), yMax(yMax),
            zMin(xMin), zMax(xMax) { }

        void operator=(const Root & root)
        {
            xMin = root.xMin;
            xMax = root.xMax;
            yMin = root.yMin;
            yMax = root.yMax;
            zMin = root.zMin;
            zMax = root.zMax;
        }

        void operator=(Root && root)
        {
            xMin = root.xMin;
            xMax = root.xMax;
            yMin = root.yMin;
            yMax = root.yMax;
            zMin = root.zMin;
            zMax = root.zMax;
        }
    };

    struct Leave
    {
        double x, y, z;

        Leave() { }
        
        Leave(const Leave & leave) : 
            x(leave.x), y(leave.y), z(leave.z) { }

        Leave(Leave && leave) :
            x(leave.x), y(leave.y), z(leave.z) { }

        Leave(const double x, const double y, const double z) :
            x(x), y(y), z(z) { }


        void operator=(const Leave & leave)
        {
            x = leave.x;
            y = leave.y;
            z = leave.z;
        }

        void operator=(Leave && leave)
        {
            x = leave.x;
            y = leave.y;
            z = leave.z;
        }
    };
    
    struct OctreeComparable
    {
        NodeType type;
        union Data
        {
            Root root;
            Leave leave;

            Data() {}
            Data(const Root & root) : root(root) { }
            Data(const Leave & leave) : leave(leave) { }
        } data;

        OctreeComparable() { }

        OctreeComparable(const OctreeComparable & c) : type(c.type) 
        {
            if (type == root)
                data.root = c.data.root;
            else 
                data.leave = c.data.leave;            
        }

        OctreeComparable(OctreeComparable && c) : type(c.type)
        {
            if (type == root)
                data.root = c.data.root;
            else 
                data.leave = c.data.leave;            
        }

        OctreeComparable(Root r, NodeType t = root) : type(t)
        {
            if (t == root)
                data.root = r;
        }

        OctreeComparable(Leave l, NodeType t = leave) : type(t)
        {
            if (t == leave)
                data.leave = l;
        }

        void operator=(const OctreeComparable & oc)
        {
            type = oc.type;
            if (oc.type == root)
                data.root = oc.data.root;
            else
                data.leave = oc.data.leave;
        }

        void operator=(OctreeComparable && oc)
        {
            type = oc.type;
            if (oc.type == root)
                data.root = oc.data.root;
            else
                data.leave = oc.data.leave;
        }
    };
} // namespace Octree

#endif // OCTREE_COMPARABLE_H