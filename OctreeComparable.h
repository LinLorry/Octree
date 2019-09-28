#ifndef OCTREE_COMPARABLE_H
#define OCTREE_COMPARABLE_H

#include <OctreeData.h>

namespace Octree
{
    template <typename Root = OctreeRoot, typename Leave = OctreeLeave>
    class OctreeComparable
    {
        private:
            NodeType type;
            union Data
            {
                Root root;
                Leave leave;

                Data() {}
                Data(const Root & root) : root(root) { }
                Data(const Leave & leave) : leave(leave) { }
            } data;
        
        public:
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

            OctreeComparable(Root root) : type(NodeType::root) { data.root = root; }

            OctreeComparable(Leave leave) : type(NodeType::leave) { data.leave = leave; }

            void operator=(const OctreeComparable & oc)
            {
                type = oc.type;
                if (oc.type == NodeType::root)
                    data.root = oc.data.root;
                else
                    data.leave = oc.data.leave;
            }

            void operator=(OctreeComparable && oc)
            {
                type = oc.type;
                if (oc.type == NodeType::root)
                    data.root = oc.data.root;
                else
                    data.leave = oc.data.leave;
            }

        public:
            const NodeType & getType() const { return type; }

            const Root & getRoot() const { return data.root; }

            const Leave & getLeave() const { return data.leave; }

            const double getRootXMin() const { return data.root.xMin; }
            const double getRootXMax() const { return data.root.xMax; }
            const double getRootYMin() const { return data.root.yMin; }
            const double getRootYMax() const { return data.root.yMax; }
            const double getRootZMin() const { return data.root.zMin; }
            const double getRootZMax() const { return data.root.zMax; }

            const double getLeaveX() const { return data.leave.x; }
            const double getLeaveY() const { return data.leave.y; }
            const double getLeaveZ() const { return data.leave.z; }

            void setType(const NodeType & type) { this->type = type; }

            void setRoot(const Root & root) { (this->data).root = root; }

            void setLeave(const Leave & leave) { (this->data).leave = leave; }

            void setRootXMin(const double value) { data.root.xMin = value; }
            void setRootXMax(const double value) { data.root.xMax = value; }
            void setRootYMin(const double value) { data.root.yMin = value; }
            void setRootYMax(const double value) { data.root.yMax = value; }
            void setRootZMin(const double value) { data.root.zMin = value; }
            void setRootZMax(const double value) { data.root.zMax = value; }

            void setLeaveX(const double value) { data.leave.x = value; }
            void setLeaveY(const double value) { data.leave.y = value; }
            void setLeaveZ(const double value) { data.leave.z = value; }


    };
} // namespace Octree

#endif // OCTREE_COMPARABLE_H