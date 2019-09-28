#include <iostream>

#ifndef OCTREE_H
#define OCTREE_H

namespace Octree
{
    template <typename T = double>
    class Octree
    {
    public:
        enum NodeType {root, leave};

        struct Root
        {
            T xMin, xMax;
            T yMin, yMax;
            T zMin, zMax;

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
                T xMin, T xMax,
                T yMin, T yMax,
                T zMin, T zMax
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
            T x, y, z;

            Leave() { }
            
            Leave(const Leave & leave) : 
                x(leave.x), y(leave.y), z(leave.z) { }

            Leave(Leave && leave) :
                x(leave.x), y(leave.y), z(leave.z) { }

            Leave(const T x, const T y, const T z) :
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

        union Data
        {
            Root root;
            Leave leave;

            Data() {}
            Data(const Root & root) : root(root) { }
            Data(Root && root) : root(root) { }

            Data(const Leave & leave) : leave(leave) { }
            Data(Leave && leave) : leave(leave) { }
        };

        struct Comparable
        {
            NodeType type;
            Data data;

            Comparable() { }

            Comparable(const Comparable & c) : type(c.type) 
            {
                if (type == root)
                    data.root = c.data.root;
                else 
                    data.leave = c.data.leave;            
            }

            Comparable(Comparable && c) : type(c.type)
            {
                if (type == root)
                    data.root = c.data.root;
                else 
                    data.leave = c.data.leave;            
            }

            Comparable(Root root) : type(NodeType::root) { data.root = root; }

            Comparable(Leave leave) : type(NodeType::leave) { data.leave = leave; }

            void operator=(const Comparable & oc)
            {
                type = oc.type;
                if (oc.type == NodeType::root)
                    data.root = oc.data.root;
                else
                    data.leave = oc.data.leave;
            }

            void operator=(Comparable && oc)
            {
                type = oc.type;
                if (oc.type == NodeType::root)
                    data.root = oc.data.root;
                else
                    data.leave = oc.data.leave;
            }
        };
    
    private:
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

        struct Node
        {
            typedef Node * NodePoint;

            Comparable data;
            NodePoint xPositive_yPositive_zPositive,
                      xPositive_yPositive_zNegative,
                      xPositive_yNegative_zNegative,
                      xPositive_yNegative_zPositive,
                      xNegative_yPositive_zPositive,
                      xNegative_yPositive_zNegative,
                      xNegative_yNegative_zNegative,
                      xNegative_yNegative_zPositive;

            Node(
                const Comparable & data, 
                const NodePoint xPositive_yPositive_zPositive = nullptr,
                const NodePoint xPositive_yPositive_zNegative = nullptr,
                const NodePoint xPositive_yNegative_zNegative = nullptr,
                const NodePoint xPositive_yNegative_zPositive = nullptr,
                const NodePoint xNegative_yPositive_zPositive = nullptr,
                const NodePoint xNegative_yPositive_zNegative = nullptr,
                const NodePoint xNegative_yNegative_zNegative = nullptr,
                const NodePoint xNegative_yNegative_zPositive = nullptr
            ) :
            data(data),
            xPositive_yPositive_zPositive(xPositive_yPositive_zPositive),
            xPositive_yPositive_zNegative(xPositive_yPositive_zNegative),
            xPositive_yNegative_zNegative(xPositive_yNegative_zNegative),
            xPositive_yNegative_zPositive(xPositive_yNegative_zPositive),
            xNegative_yPositive_zPositive(xNegative_yPositive_zPositive),
            xNegative_yPositive_zNegative(xNegative_yPositive_zNegative),
            xNegative_yNegative_zNegative(xNegative_yNegative_zNegative),
            xNegative_yNegative_zPositive(xNegative_yNegative_zPositive) {}
        };

    public:
        typedef Node * NodePoint;
    
    private:
        NodePoint treeRoot;

    private:
        static const Result DataComparator(const Comparable & first, const Comparable & second);

        static NodePoint * getPositionPoint(const NodePoint node, const Comparable & data);

        static void setPositionPoint(const NodePoint node, const NodePoint p);

        static void release(NodePoint node);

    public:
        Octree(
            const T xMin, const T xMax,
            const T yMin, const T yMax,
            const T zMin, const T zMax
        );

        Octree(const Octree<T>::Root & r);

        ~Octree();
        
        NodePoint find(const Comparable & data) const;

        void insert(const Comparable & data);

    private:
        NodePoint find(const NodePoint node, const Comparable & data) const;

        void insert(NodePoint node, const Comparable & data);
    };

    template <typename T>
    const typename Octree<T>::Result Octree<T>::DataComparator(const Comparable & first, const Comparable & second)
    {

        if (first.type == NodeType::leave)
        {
            if (second.type == NodeType::root)
                return DataComparator(second, first);
            
            if (
                first.data.leave.x == second.data.leave.x &&
                first.data.leave.y == second.data.leave.y &&
                first.data.leave.z == second.data.leave.z
            ) return Result::equal;

            return Result::undefine;
        }
        
        if (second.type == NodeType::root) return Result::undefine;

        if (
            first.data.root.xMax < second.data.leave.x || 
            first.data.root.xMin > second.data.leave.x ||
            first.data.root.yMax < second.data.leave.y || 
            first.data.root.yMin > second.data.leave.y ||
            first.data.root.zMax < second.data.leave.z ||
            first.data.root.zMin > second.data.leave.z
        ) return Result::outside;
        else
        {
            auto xTmp = (first.data.root.xMax + first.data.root.xMin) / 2;
            auto yTmp = (first.data.root.yMax + first.data.root.yMin) / 2;
            auto zTmp = (first.data.root.zMax + first.data.root.zMin) / 2;

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

    template <typename T>
    typename Octree<T>::NodePoint *Octree<T>::getPositionPoint(const Octree<T>::NodePoint node, const Comparable & data)
    {
        switch (DataComparator(node->data, data))
        {
            case xPositive_yPositive_zPositive:
                return &(node->xPositive_yPositive_zPositive);
            case xPositive_yPositive_zNegative:
                return &(node->xPositive_yPositive_zNegative);
            case xPositive_yNegative_zNegative:
                return &(node->xPositive_yNegative_zNegative);
            case xPositive_yNegative_zPositive:
                return &(node->xPositive_yNegative_zPositive);
            case xNegative_yPositive_zPositive:
                return &(node->xNegative_yPositive_zPositive);
            case xNegative_yPositive_zNegative:
                return &(node->xNegative_yPositive_zNegative);
            case xNegative_yNegative_zNegative:
                return &(node->xNegative_yNegative_zNegative);
            case xNegative_yNegative_zPositive:
                return &(node->xNegative_yNegative_zPositive);
            case outside:
            case undefine:
            default:
                return nullptr;
        }
    }

    template <typename T>
    void Octree<T>::setPositionPoint(const Octree<T>::NodePoint node, const Octree<T>::NodePoint p)
    {
        switch (DataComparator(node->data, p->data))
        {
            case xPositive_yPositive_zPositive:
                node->xPositive_yPositive_zPositive = p;
                break;
            case xPositive_yPositive_zNegative:
                node->xPositive_yPositive_zNegative = p;
                break;
            case xPositive_yNegative_zNegative:
                node->xPositive_yNegative_zNegative = p;
                break;
            case xPositive_yNegative_zPositive:
                node->xPositive_yNegative_zPositive = p;
                break;
            case xNegative_yPositive_zPositive:
                node->xNegative_yPositive_zPositive = p;
                break;
            case xNegative_yPositive_zNegative:
                node->xNegative_yPositive_zNegative = p;
                break;
            case xNegative_yNegative_zNegative:
                node->xNegative_yNegative_zNegative = p;
                break;
            case xNegative_yNegative_zPositive:
                node->xNegative_yNegative_zPositive = p;
                break;
        }
    }

    template <typename T>
    void Octree<T>::release(Octree<T>::NodePoint node)
    {
        if (node->xPositive_yPositive_zPositive != nullptr)
            release(node->xPositive_yPositive_zPositive);

        if (node->xPositive_yPositive_zNegative != nullptr)
            release(node->xPositive_yPositive_zNegative);

        if (node->xPositive_yNegative_zNegative != nullptr)
            release(node->xPositive_yNegative_zNegative);

        if (node->xPositive_yNegative_zPositive != nullptr)
            release(node->xPositive_yNegative_zPositive);
        
        if (node->xNegative_yPositive_zPositive != nullptr)
            release(node->xNegative_yPositive_zPositive);
        
        if (node->xNegative_yPositive_zNegative != nullptr)
            release(node->xNegative_yPositive_zNegative);

        if (node->xNegative_yNegative_zNegative != nullptr)
            release(node->xNegative_yNegative_zNegative);

        if (node->xNegative_yNegative_zPositive != nullptr)
            release(node->xNegative_yNegative_zPositive);
        
        delete node;
    }

    template <typename T>
    Octree<T>::Octree(
            const T xMin, const T xMax,
        const T yMin, const T yMax,
        const T zMin, const T zMax
    ) { treeRoot = new Node(Comparable(Root(xMin, xMax, yMin, yMax, zMin, zMax))); }

    template <typename T>
    Octree<T>::Octree(const Octree<T>::Root & r) { treeRoot = new Node(Comparable(r)); }

    template <typename T>
    Octree<T>::~Octree() { release(treeRoot); }

    template <typename T>
    inline typename Octree<T>::NodePoint Octree<T>::find(const Octree<T>::Comparable & data) const 
    { return find(treeRoot, data); }

    template <typename T>
    inline void Octree<T>::insert(const Octree<T>::Comparable & data)
    { insert(treeRoot, data); }

    template <typename T>
    typename Octree<T>::NodePoint Octree<T>::find(const Octree<T>::NodePoint node, const Comparable & data) const
    {
        NodePoint *p = getPositionPoint(node, data);
        if (p == nullptr) 
            return nullptr;
        else if (*p != nullptr && ((*p)->data).type == NodeType::root)
            return find(*p, data);
        return node;
    }

    template <typename T>
    void Octree<T>::insert(Octree<T>::NodePoint node, const Octree<T>::Comparable & data)
    {
        NodePoint *p = getPositionPoint(node, data);
        if (p == nullptr)
            return ;
        else if (*p == nullptr)
        {
            *p = new Node(data);
            return ;
        }
        else if (((*p)->data).type == NodeType::leave)
        {
            if (DataComparator((*p)->data, data) == Result::equal) return ; 
            
            Comparable *c = new Comparable();
            c->type = NodeType::root;

            auto xMin = (node->data).data.root.xMin, xMax = (node->data).data.root.xMax,
                 yMin = (node->data).data.root.yMin, yMax = (node->data).data.root.yMax,
                 zMin = (node->data).data.root.zMin, zMax = (node->data).data.root.zMax,
                 
                 xTmp = ((node->data).data.root.xMin + (node->data).data.root.xMax) / 2,
                 yTmp = ((node->data).data.root.yMin + (node->data).data.root.yMax) / 2,
                 zTmp = ((node->data).data.root.zMin + (node->data).data.root.zMax) / 2;

            switch (DataComparator(node->data, data))
            {
                case xPositive_yPositive_zPositive:
                    xMin = xTmp;
                    yMin = yTmp;
                    zMin = zTmp;
                    break;
                case xPositive_yPositive_zNegative:
                    xMin = xTmp;
                    yMin = yTmp;
                    zMax = zTmp;
                    break;
                case xPositive_yNegative_zNegative:
                    xMin = xTmp;
                    yMax = yTmp;
                    zMax = zTmp;
                    break;
                case xPositive_yNegative_zPositive:
                    xMin = xTmp;
                    yMax = yTmp;
                    zMin = zTmp;
                    break;
                case xNegative_yPositive_zPositive:
                    xMax = xTmp;
                    yMin = yTmp;
                    zMin = zTmp;
                    break;
                case xNegative_yPositive_zNegative:
                    xMax = xTmp;
                    yMin = yTmp;
                    zMax = zTmp;
                    break;
                case xNegative_yNegative_zNegative:
                    xMax = xTmp;
                    yMax = yTmp;
                    zMax = zTmp;
                    break;
                case xNegative_yNegative_zPositive:
                    xMin = xTmp;
                    yMin = yTmp;
                    zMin = zTmp;
                    break;
            }
            
            c->data.root.xMin = xMin;
            c->data.root.xMax = xMax;
            c->data.root.yMin = yMin;
            c->data.root.yMax = yMax;
            c->data.root.zMin = zMin;
            c->data.root.zMax = zMax;

            NodePoint n = new Node(*c);
            setPositionPoint(n, *p);
            *p = n;
        }
        
        return insert(*p, data);
    }

} // namespace Octree


#endif // OCTREE_H