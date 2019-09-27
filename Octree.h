#ifndef OCTREE_H
#define OCTREE_H

#include <OctreeComparator.h>

namespace Octree
{
    template <typename Comparable = OctreeComparable>
    class Octree
    {
    public:
        typedef const Location (*Comparator)(const Comparable &, const Comparable &);

    private:
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
                Comparable data, 
                NodePoint xPositive_yPositive_zPositive = nullptr,
                NodePoint xPositive_yPositive_zNegative = nullptr,
                NodePoint xPositive_yNegative_zNegative = nullptr,
                NodePoint xPositive_yNegative_zPositive = nullptr,
                NodePoint xNegative_yPositive_zPositive = nullptr,
                NodePoint xNegative_yPositive_zNegative = nullptr,
                NodePoint xNegative_yNegative_zNegative = nullptr,
                NodePoint xNegative_yNegative_zPositive = nullptr
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
        typedef Node * NodePoint;

        NodePoint root;
        Comparator comparator;
        
    public:

        Octree(Comparator comparator = OctreeComparator) :
            comparator(comparator)
        {
            Comparable c;
            root = new Node(c);
        }

        ~Octree() { delete root; };

        void insert(const Comparable & data)
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

    private:
        void insert(NodePoint node, const Comparable & data)
        {
            NodePoint *p;
            switch (comparator(node->data, data))
            {
            case outside:
                break;
            case xPositive_yPositive_zPositive:
                p = &(node->xPositive_yPositive_zPositive);
                break;
            case xPositive_yPositive_zNegative:
                p = &(node->xPositive_yPositive_zNegative);
                break;
            case xPositive_yNegative_zNegative:
                p = &(node->xPositive_yNegative_zNegative);
                break;
            case xPositive_yNegative_zPositive:
                p = &(node->xPositive_yNegative_zPositive);
                break;
            case xNegative_yPositive_zPositive:
                p = &(node->xNegative_yPositive_zPositive);
                break;
            case xNegative_yPositive_zNegative:
                p = &(node->xNegative_yPositive_zNegative);
                break;
            case xNegative_yNegative_zNegative:
                p = &(node->xNegative_yNegative_zNegative);
                break;
            case xNegative_yNegative_zPositive:
                p = &(node->xNegative_yNegative_zPositive);
                break;
            default:
                return ;
            }

            if (p != nullptr && *p == nullptr)
            {
                *p = new Node(data);
                return ;
            }
            // return insert(*p, data);
        }
    };
} // namespace Octree


#endif // OCTREE_H