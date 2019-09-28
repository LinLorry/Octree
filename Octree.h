#ifndef OCTREE_H
#define OCTREE_H

#include <OctreeComparator.h>

namespace Octree
{
    template <
        template<typename Root = OctreeRoot, typename Leave = OctreeLeave> 
        typename Comparable = OctreeComparable
    >
    class Octree
    {
    public:
        typedef Comparable<> T;
        typedef const Result (*Comparator)(const T &, const T &);
    private:
        struct Node
        {
            typedef Node * NodePoint;

            T data;
            NodePoint xPositive_yPositive_zPositive,
                      xPositive_yPositive_zNegative,
                      xPositive_yNegative_zNegative,
                      xPositive_yNegative_zPositive,
                      xNegative_yPositive_zPositive,
                      xNegative_yPositive_zNegative,
                      xNegative_yNegative_zNegative,
                      xNegative_yNegative_zPositive;

            Node(
                const T & data, 
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
        NodePoint root;
        Comparator comparator;
    public:

        Octree(Comparator comparator = OctreeComparator) :
            comparator(comparator) { init(); }

        ~Octree() { release(root); };

        void insert(const T & data)
        {
            return insert(root, data);
        }

        // void insert(Comparable && data)
        // {
        //     return ;
        // }

        // void remove(const Comparable & data);
        // void remove(Comparable && data);

        // bool find(const Comparable & data) const;
        // bool find(Comparable && data) const;

        NodePoint find(const T & data) const
        {
            return find(root, data);
        }

    private:
        void insert(NodePoint node, const T & data)
        {
            NodePoint *p = getPositionPoint(node, data);
            if (p == nullptr)
                return ;
            else if (*p == nullptr)
            {
                *p = new Node(data);
                return ;
            }
            else if (((*p)->data).getType() == NodeType::leave)
            {
                if (comparator((*p)->data, data) == Result::equal) return ;                
                
                T *c = new T();
                c->setType(NodeType::root);

                auto xMin = (node->data).getRootXMin(), xMax = (node->data).getRootXMax(),
                       yMin = (node->data).getRootYMin(), yMax = (node->data).getRootYMax(),
                       zMin = (node->data).getRootZMin(), zMax = (node->data).getRootZMax(),
                       xTmp = ((node->data).getRootXMin() + (node->data).getRootXMax()) / 2,
                       yTmp = ((node->data).getRootYMin() + (node->data).getRootYMax()) / 2,
                       zTmp = ((node->data).getRootZMin() + (node->data).getRootZMax()) / 2;

                switch (comparator(node->data, data))
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
                
                c->setRootXMin(xMin);
                c->setRootXMax(xMax);
                c->setRootYMin(yMin);
                c->setRootYMax(yMax);
                c->setRootZMin(zMin);
                c->setRootZMax(zMax);

                NodePoint n = new Node(*c);
                setPositionPoint(n, *p);
                *p = n;
            }
            
            return insert(*p, data);
        }

        NodePoint find(const NodePoint node, const T & data) const
        {
            NodePoint *p = getPositionPoint(node, data);
            if (p == nullptr) 
                return nullptr;
            else if (*p != nullptr && ((*p)->data).type == NodeType::root)
                return find(*p, data);
            return node;
        } 

        void init()
        {
            T *c = new T();
            c->setType(NodeType::root);

            root = new Node(*c);
        }

        NodePoint * getPositionPoint(const NodePoint node, const T & data) const
        {
            switch (comparator(node->data, data))
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

        void setPositionPoint(const NodePoint node, const NodePoint p)
        {
                switch (comparator(node->data, p->data))
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

        void release(NodePoint node)
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
    };
} // namespace Octree


#endif // OCTREE_H