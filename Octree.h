#ifndef OCTREE_H
#define OCTREE_H

namespace Octree
{
template <typename T = double>
    class Octree
    {
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
                zMin(zMin), zMax(zMax) { }

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

        class Node
        {
            private:
                friend class Octree<T>;
                typedef Node * NodePoint;

                NodeType type;
                Data data;

                NodePoint xPositive_yPositive_zPositive,
                        xPositive_yPositive_zNegative,
                        xPositive_yNegative_zNegative,
                        xPositive_yNegative_zPositive,
                        xNegative_yPositive_zPositive,
                        xNegative_yPositive_zNegative,
                        xNegative_yNegative_zNegative,
                        xNegative_yNegative_zPositive;

            public:
                Node(const Node & node);
                Node(const Leave & leave);

                Node(
                    const Root & root, 
                    const NodePoint xPositive_yPositive_zPositive = nullptr,
                    const NodePoint xPositive_yPositive_zNegative = nullptr,
                    const NodePoint xPositive_yNegative_zNegative = nullptr,
                    const NodePoint xPositive_yNegative_zPositive = nullptr,
                    const NodePoint xNegative_yPositive_zPositive = nullptr,
                    const NodePoint xNegative_yPositive_zNegative = nullptr,
                    const NodePoint xNegative_yNegative_zNegative = nullptr,
                    const NodePoint xNegative_yNegative_zPositive = nullptr
                );

                const Result comparator(const Node & node) const;

                NodePoint *getPositionPoint(const Node & node);

                void setPosition(const NodePoint node);
        };
    
    public:
        typedef Node * NodePoint;
    
    private:
        NodePoint treeRoot;

    private:
        static void release(NodePoint node);

    public:
        Octree(
            const T xMin, const T xMax,
            const T yMin, const T yMax,
            const T zMin, const T zMax
        );

        Octree(const Octree<T>::Root & r);

        ~Octree();
        
        NodePoint find(const Node & data) const;

        void insert(const Node & data);

        void insert(Node & node, const Node & data);

    private:
        NodePoint find(const Node & node, const Node & data) const;
    };

    template <typename T>
    Octree<T>::Node::Node(const Node & node) :
        type(node.type),
        xPositive_yPositive_zPositive(node.xPositive_yPositive_zPositive),
        xPositive_yPositive_zNegative(node.xPositive_yPositive_zNegative),
        xPositive_yNegative_zNegative(node.xPositive_yNegative_zNegative),
        xPositive_yNegative_zPositive(node.xPositive_yNegative_zPositive),
        xNegative_yPositive_zPositive(node.xNegative_yPositive_zPositive),
        xNegative_yPositive_zNegative(node.xNegative_yPositive_zNegative),
        xNegative_yNegative_zNegative(node.xNegative_yNegative_zNegative),
        xNegative_yNegative_zPositive(node.xNegative_yNegative_zPositive) 
        {
            if (node.type == Octree<T>::NodeType::root) data.root = node.data.root;
            else data.leave = node.data.leave; 
        }

    template <typename T>
    Octree<T>::Node::Node(const Leave & leave) : 
    type(Octree<T>::NodeType::leave), data(leave),
    xPositive_yPositive_zPositive(nullptr),
    xPositive_yPositive_zNegative(nullptr),
    xPositive_yNegative_zNegative(nullptr),
    xPositive_yNegative_zPositive(nullptr),
    xNegative_yPositive_zPositive(nullptr),
    xNegative_yPositive_zNegative(nullptr),
    xNegative_yNegative_zNegative(nullptr),
    xNegative_yNegative_zPositive(nullptr) 
     { }

    template <typename T>
    Octree<T>::Node::Node(
        const Root & root, 
        const NodePoint xPositive_yPositive_zPositive,
        const NodePoint xPositive_yPositive_zNegative,
        const NodePoint xPositive_yNegative_zNegative,
        const NodePoint xPositive_yNegative_zPositive,
        const NodePoint xNegative_yPositive_zPositive,
        const NodePoint xNegative_yPositive_zNegative,
        const NodePoint xNegative_yNegative_zNegative,
        const NodePoint xNegative_yNegative_zPositive
    ) :
    type(Octree::NodeType::root), data(root),
    xPositive_yPositive_zPositive(xPositive_yPositive_zPositive),
    xPositive_yPositive_zNegative(xPositive_yPositive_zNegative),
    xPositive_yNegative_zNegative(xPositive_yNegative_zNegative),
    xPositive_yNegative_zPositive(xPositive_yNegative_zPositive),
    xNegative_yPositive_zPositive(xNegative_yPositive_zPositive),
    xNegative_yPositive_zNegative(xNegative_yPositive_zNegative),
    xNegative_yNegative_zNegative(xNegative_yNegative_zNegative),
    xNegative_yNegative_zPositive(xNegative_yNegative_zPositive) { }

    template <typename T>
    const typename Octree<T>::Result Octree<T>::Node::comparator(const Node & node) const
    {
        if (type == NodeType::leave)
            if (node.type == NodeType::root)
                return Result::undefine;
            else if (
                this->data.leave.x == node.data.leave.x &&
                this->data.leave.y == node.data.leave.y &&
                this->data.leave.z == node.data.leave.z
            ) return Result::equal;
            else return Result::undefine;
        
        if (node.type == NodeType::root) return Result::undefine;

        if (
            this->data.root.xMax < node.data.leave.x || 
            this->data.root.xMin > node.data.leave.x ||
            this->data.root.yMax < node.data.leave.y || 
            this->data.root.yMin > node.data.leave.y ||
            this->data.root.zMax < node.data.leave.z ||
            this->data.root.zMin > node.data.leave.z
        ) return Result::outside;
        else
        {
            T xTmp = (this->data.root.xMax + this->data.root.xMin) / 2;
            T yTmp = (this->data.root.yMax + this->data.root.yMin) / 2;
            T zTmp = (this->data.root.zMax + this->data.root.zMin) / 2;

            if (node.data.leave.x > xTmp)
                if (node.data.leave.y > yTmp)
                    if (node.data.leave.z > zTmp)
                        return Result::xPositive_yPositive_zPositive;
                    else return Result::xPositive_yPositive_zNegative;
                else if (node.data.leave.z > zTmp)
                    return Result::xPositive_yNegative_zPositive;
                else return Result::xPositive_yNegative_zNegative;
            else if (node.data.leave.y > yTmp)
                if (node.data.leave.z > zTmp)
                    return Result::xNegative_yPositive_zPositive;
                else return Result::xNegative_yPositive_zNegative;
            else if (node.data.leave.z > zTmp)
                return Result::xNegative_yNegative_zPositive;
            return Result::xNegative_yNegative_zNegative;
        } 
    }

    template <typename T>
    typename Octree<T>::Node::NodePoint *Octree<T>::Node::getPositionPoint(const Node & node)
    {
        switch (this->comparator(node))
        {
            case Result::xPositive_yPositive_zPositive:
                return &(this->xPositive_yPositive_zPositive);
            case Result::xPositive_yPositive_zNegative:
                return &(this->xPositive_yPositive_zNegative);
            case Result::xPositive_yNegative_zNegative:
                return &(this->xPositive_yNegative_zNegative);
            case Result::xPositive_yNegative_zPositive:
                return &(this->xPositive_yNegative_zPositive);
            case Result::xNegative_yPositive_zPositive:
                return &(this->xNegative_yPositive_zPositive);
            case Result::xNegative_yPositive_zNegative:
                return &(this->xNegative_yPositive_zNegative);
            case Result::xNegative_yNegative_zNegative:
                return &(this->xNegative_yNegative_zNegative);
            case Result::xNegative_yNegative_zPositive:
                return &(this->xNegative_yNegative_zPositive);
            case outside:
            case undefine:
            default:
                return nullptr;
        }
    }

    template <typename T>
    void Octree<T>::Node::setPosition(const NodePoint node)
    {
        switch (this->comparator(*node))
        {
            case Result::xPositive_yPositive_zPositive:
                this->xPositive_yPositive_zPositive = node;
                return;
            case Result::xPositive_yPositive_zNegative:
                this->xPositive_yPositive_zNegative = node;
                return;
            case Result::xPositive_yNegative_zNegative:
                this->xPositive_yNegative_zNegative = node;
                return;
            case Result::xPositive_yNegative_zPositive:
                this->xPositive_yNegative_zPositive = node;
                return;
            case Result::xNegative_yPositive_zPositive:
                this->xNegative_yPositive_zPositive = node;
                return;
            case Result::xNegative_yPositive_zNegative:
                this->xNegative_yPositive_zNegative = node;
                return;
            case Result::xNegative_yNegative_zNegative:
                this->xNegative_yNegative_zNegative = node;
                return;
            case Result::xNegative_yNegative_zPositive:
                this->xNegative_yNegative_zPositive = node;
                return;
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
    ) : treeRoot(new Node(Root(xMin, xMax, yMin, yMax, zMin, zMax))) { }

    template <typename T>
    Octree<T>::Octree(const Root & r) : treeRoot(new Node(r)) { }

    template <typename T>
    Octree<T>::~Octree() { release(treeRoot); }

    template <typename T>
    inline typename Octree<T>::NodePoint Octree<T>::find(const Node & data) const 
    { return this->find(treeRoot, data); }

    template <typename T>
    inline void Octree<T>::insert(const Node & data)
    { this->insert(*treeRoot, data); }

    template <typename T>
    void Octree<T>::insert(Octree<T>::Node & node, const Node & data)
    {
        NodePoint *p = node.getPositionPoint(data);
        if (p == nullptr)
            return ;
        else if (*p == nullptr)
        {
            *p = new Node(data);
            return ;
        }
        else if ((*p)->type == NodeType::leave)
        {
            if ((*p)->comparator(data) == Result::equal) return ; 

            T xMin = node.data.root.xMin, xMax = node.data.root.xMax,
                yMin = node.data.root.yMin, yMax = node.data.root.yMax,
                zMin = node.data.root.zMin, zMax = node.data.root.zMax,
                 
                xTmp = (node.data.root.xMin + node.data.root.xMax) / 2,
                yTmp = (node.data.root.yMin + node.data.root.yMax) / 2,
                zTmp = (node.data.root.zMin + node.data.root.zMax) / 2;

            NodePoint *q = &node.xPositive_yPositive_zPositive;
            short diff = 0;

            for (;p!=q; ++q, ++diff) ;
            
            if (diff < 4) xMin = xTmp;
            else xMax = xTmp;

            if (diff%4 < 2) yMin = yTmp;
            else yMax = yTmp;

            if (diff%4 == 0 || diff%4 == 3) zMin = zTmp;
            else zMax= zTmp;
            
            NodePoint n = new Node(Root(
                xMin, xMax,
                yMin, yMax,
                zMin, zMax
            ));
            
            n->setPosition(*p);
            *p = n;
        }
        
        return insert(**p, data);
    }

    template <typename T>
    typename Octree<T>::NodePoint Octree<T>::find(const Node & node, const Node & data) const
    {
        NodePoint *p = node.getPositionPoint(data);
        if (p == nullptr) 
            return nullptr;
        else if (*p != nullptr && (*p)->type == NodeType::root)
            return find(*p, data);
        return node;
    }

} // namespace Octree


#endif // OCTREE_H