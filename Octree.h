#ifndef OCTREE_H
#define OCTREE_H

#include <exception>
#include <stack>
#include <cstring>

namespace Octree
{
    /*
        OctreeExpection

        Function:
        OctreeExpection:    the init function, set up the expection.
            parameter:  - ERROR_CODE
        what:               return the error message.
            return:     const char *
    */
    class OctreeExpection : public std::exception
    {
    public:
        enum ERROR_CODE{
            INSERT_ERROR = 0,
            REMOVE_ERROR = 0x1,
            TYPE_ERROR = 0x100,
            TREE_EMPTY,
            OUT_RANGE
        };

        OctreeExpection(ERROR_CODE code);
        const char * what();
    private:
        static const char *error_str[5];
    private:
        ERROR_CODE code;
    };

    /*
        Octree:

        Field:
        treeRoot:   this Octree Root.

        Function:
        Octree:     the init function, set up the treeRoot.
            parameter:  - const T, const T,
                          const T, const T,
                          const T, const T 
                        - const Root &
        ~Octree:    free whole tree.
        find:       find the node, retunr the point point to the node.
            parameter:  cosnt Node *
            return:     Node * 
        insert:     insert the node into tree. If node which will be insert type not leave, throw expection. 
            parameter:  const Node *
        remove:     remove the node in the tree.
            parameter:  const Node *
    */
    template <typename T = double>
    class Octree
    {
    public:
        class Node;
        typedef Node * NodePoint;
        
    private:
        /*
            Nodes compare result:
            outside:                        outside the node.
            equal:                          nodes are all leave and have the same position.
            xPositive_yPositive_zPositive   on the x positive axis, y positive axis, z positive axis.
            ...
            undefine                        undefined case
        */
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
        /*
            Node Type: Root and Leave
        */
        enum NodeType {root, leave};

        /*
            Root is a struct 
            Its child in the scope of xMin~xMax, yMin~yMax, zMin~zMax.
        */
        struct Root
        {
            T xMin, xMax;
            T yMin, yMax;
            T zMin, zMax;

            Root() { }

            Root(const Root &root) : 
                xMin(root.xMin), xMax(root.xMax),
                yMin(root.yMin), yMax(root.yMax),
                zMin(root.zMin), zMax(root.zMax) { }

            Root(Root &&root) :
                xMin(root.xMin), xMax(root.xMax),
                yMin(root.yMin), yMax(root.yMax),
                zMin(root.zMin), zMax(root.zMax) { }

            Root
            (               
                T xMin, T xMax,
                T yMin, T yMax,
                T zMin, T zMax
            ) :
                xMin(xMin), xMax(xMax),
                yMin(yMin), yMax(yMax),
                zMin(zMin), zMax(zMax) { }

            inline Root &operator=(const Root &root) 
            {
                std::memcpy(this, &root, sizeof(Root));
                return *this;
            }

            inline bool operator==(const Root &root) const { return std::memcmp(this, &root, sizeof(Root)) == 0; }

            inline bool operator!=(const Root &root) const { return std::memcmp(this, &root, sizeof(Root)) != 0; }
        };

        /*
            Leave is a struct
            It is the final data in the tree.
            Its position is (x, y, z).
        */
        struct Leave
        {
            T x, y, z;

            Leave() { }
            
            Leave(const Leave &leave) : 
                x(leave.x), y(leave.y), z(leave.z) { }

            Leave(const T x, const T y, const T z) :
                x(x), y(y), z(z) { }

            inline Leave &operator=(const Leave &leave)
            {
                std::memcpy(this, &leave, sizeof(Leave));
                return *this;
            }

            inline bool operator==(const Leave &leave) const { return std::memcmp(this, &leave, sizeof(Leave)) == 0; }

            inline bool operator!=(const Leave &leave) const { return std::memcmp(this, &leave, sizeof(Leave)) != 0; }
        };

        /*
            Data is a union
            Contain Root and Leave
            It's the Node data field.
        */
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

        /*
            Octree Node:

            Field:
            type:                           this node type
            data:                           the scope of positive or the positive of data.
            xPositive_yPositive_zPositive:  pointer to the node which on the self x positive axis, y positive axis, z positive axis.
            ...

            Function:
            comparator:                     compare with the parameter, return value is enum Result.
                parameter:      const Node *
                return value:   cosnt Result
            getPositionPoint                return a pointer point to a pointer to the scope of the parameter.
                parameter:      const Node *
                return value:   Node **
            setPosition                     set the pointer to the scope of the parameter as parameter.
                parameter:      const Node *
            getNodeType                     return this node type.
                return value:   const NodeType
            getRoot                         if this type is root, return data.root, else throw Expection.
                return value:   const Root &
            setRoot                         if this type is root, set root as the parameter.
                parameter:      const Root &     
            getLeave                        if this type is leave, return data.root, else throw Expection.
                return value:   const Leave &
            setLeave                        if this type is leave, set root as the parameter.
                parameter:      const Leave & 
        */
        class Node
        {
        private:
            friend class Octree<T>;

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
            static NodePoint *getNodePositionPoint(NodePoint node, const Result point);

        public:
            Node(const NodePoint node);
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

            const Result comparator(const NodePoint node) const;

            NodePoint *getPositionPoint(const NodePoint node);

            void setPosition(const NodePoint node);

            const NodeType getNodeType() const;

            const Root &getRoot() const;

            void setRoot(const Root &root);

            const Leave &getLeave() const;

            void setLeave(const Leave &leave);
        };

        class Iterator
        {
        public:
            const bool haveNext() const;

            const Leave &operator*() const;

            Iterator &operator++();

            Iterator &operator--();

            bool operator==(const Iterator &iter) const;

            bool operator!=(const Iterator &iter) const;

        private:
            Iterator(const NodePoint node);

            std::stack<NodePoint> node_stack;
            std::stack<Result> point_stack;

            NodePoint node;
            Result point;
        };
    
    private:
        NodePoint treeRoot;
        unsigned long long size;

    private:
        static void release(NodePoint node);

    public:
        Octree(
            const T xMin, const T xMax,
            const T yMin, const T yMax,
            const T zMin, const T zMax
        );

        Octree(const Octree<T>::Root &r);

        ~Octree();
        
        NodePoint find(const NodePoint data);

        void insert(const NodePoint data);

        void remove(const NodePoint data);

    private:
        NodePoint find(NodePoint node, const NodePoint data);
        
        void insert(NodePoint node, const NodePoint data);

        void remove(NodePoint *node, const NodePoint data);
    };

    typedef OctreeExpection::ERROR_CODE ERROR_TYPE;

    OctreeExpection::OctreeExpection(ERROR_CODE code) : code(code){ }

    const char *OctreeExpection::what()  { return error_str[code]; }

    const char *OctreeExpection::error_str[5] = 
    {
        "OCTREE::ERROR::INSERT_ERROR",
        "OCTREE::ERROR::REMOVE_ERROR",
        "OCTREE::ERROE::TYPE_ERROR",
        "OCTREE::ERROR::TREE_EMPTY",
        "OCTREE::ERROR::OUT_RANGE"
    };

    template <typename T>
    typename Octree<T>::NodePoint *Octree<T>::Node::getNodePositionPoint(NodePoint node, const Result point)
    {
        switch (point)
        {
        case Result::xPositive_yPositive_zPositive:
            return &(node->xPositive_yPositive_zPositive);
        case Result::xPositive_yPositive_zNegative:
            return &(node->xPositive_yPositive_zNegative);
        case Result::xPositive_yNegative_zNegative:
            return &(node->xPositive_yNegative_zNegative);
        case Result::xPositive_yNegative_zPositive:
            return &(node->xPositive_yNegative_zPositive);
        case Result::xNegative_yPositive_zPositive:
            return &(node->xNegative_yPositive_zPositive);
        case Result::xNegative_yPositive_zNegative:
            return &(node->xNegative_yPositive_zNegative);
        case Result::xNegative_yNegative_zNegative:
            return &(node->xNegative_yNegative_zNegative);
        case Result::xNegative_yNegative_zPositive:
            return &(node->xNegative_yNegative_zPositive);
        default:
            throw new OctreeExpection(ERROR_TYPE::OUT_RANGE);
        }
    }

    template <typename T>
    Octree<T>::Node::Node(const NodePoint node) :
        type(node->type),
        xPositive_yPositive_zPositive(node->xPositive_yPositive_zPositive),
        xPositive_yPositive_zNegative(node->xPositive_yPositive_zNegative),
        xPositive_yNegative_zNegative(node->xPositive_yNegative_zNegative),
        xPositive_yNegative_zPositive(node->xPositive_yNegative_zPositive),
        xNegative_yPositive_zPositive(node->xNegative_yPositive_zPositive),
        xNegative_yPositive_zNegative(node->xNegative_yPositive_zNegative),
        xNegative_yNegative_zNegative(node->xNegative_yNegative_zNegative),
        xNegative_yNegative_zPositive(node->xNegative_yNegative_zPositive) 
    {
        if (node->type == NodeType::root) data.root = node->data.root;
        else data.leave = node->data.leave; 
    }

    template <typename T>
    Octree<T>::Node::Node(const Leave & leave) : 
        type(NodeType::leave), data(leave),
        xPositive_yPositive_zPositive(nullptr),
        xPositive_yPositive_zNegative(nullptr),
        xPositive_yNegative_zNegative(nullptr),
        xPositive_yNegative_zPositive(nullptr),
        xNegative_yPositive_zPositive(nullptr),
        xNegative_yPositive_zNegative(nullptr),
        xNegative_yNegative_zNegative(nullptr),
        xNegative_yNegative_zPositive(nullptr) { }

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
    type(NodeType::root), data(root),
    xPositive_yPositive_zPositive(xPositive_yPositive_zPositive),
    xPositive_yPositive_zNegative(xPositive_yPositive_zNegative),
    xPositive_yNegative_zNegative(xPositive_yNegative_zNegative),
    xPositive_yNegative_zPositive(xPositive_yNegative_zPositive),
    xNegative_yPositive_zPositive(xNegative_yPositive_zPositive),
    xNegative_yPositive_zNegative(xNegative_yPositive_zNegative),
    xNegative_yNegative_zNegative(xNegative_yNegative_zNegative),
    xNegative_yNegative_zPositive(xNegative_yNegative_zPositive) { }

    template <typename T>
    const typename Octree<T>::Result Octree<T>::Node::comparator(const NodePoint node) const
    {
        if (type == NodeType::leave)
            if (
                node->type == NodeType::leave && 
                this->data.leave.x == node->data.leave.x &&
                this->data.leave.y == node->data.leave.y &&
                this->data.leave.z == node->data.leave.z
            ) return Result::equal;
            else return Result::undefine;
        
        if (node->type == NodeType::root) return Result::undefine;

        if (
            this->data.root.xMax < node->data.leave.x || 
            this->data.root.xMin > node->data.leave.x ||
            this->data.root.yMax < node->data.leave.y || 
            this->data.root.yMin > node->data.leave.y ||
            this->data.root.zMax < node->data.leave.z ||
            this->data.root.zMin > node->data.leave.z
        ) return Result::outside;
        else
        {
            T xTmp = (this->data.root.xMax + this->data.root.xMin) / 2;
            T yTmp = (this->data.root.yMax + this->data.root.yMin) / 2;
            T zTmp = (this->data.root.zMax + this->data.root.zMin) / 2;

            if (node->data.leave.x > xTmp)
                if (node->data.leave.y > yTmp)
                    if (node->data.leave.z > zTmp)
                        return Result::xPositive_yPositive_zPositive;
                    else return Result::xPositive_yPositive_zNegative;
                else if (node->data.leave.z > zTmp)
                    return Result::xPositive_yNegative_zPositive;
                else return Result::xPositive_yNegative_zNegative;
            else if (node->data.leave.y > yTmp)
                if (node->data.leave.z > zTmp)
                    return Result::xNegative_yPositive_zPositive;
                else return Result::xNegative_yPositive_zNegative;
            else if (node->data.leave.z > zTmp)
                return Result::xNegative_yNegative_zPositive;
            return Result::xNegative_yNegative_zNegative;
        } 
    }

    template <typename T>
    typename Octree<T>::NodePoint *Octree<T>::Node::getPositionPoint(const NodePoint node)
    {
        return Node::getNodePositionPoint(this, this->comparator(node));
    }

    template <typename T>
    void Octree<T>::Node::setPosition(const NodePoint node)
    {
        (*Node::getNodePositionPoint(this, this->comparator(node))) = node;
    }

    template <typename T>
    const typename Octree<T>::NodeType Octree<T>::Node::getNodeType() const
    {
        return type;
    }

    template <typename T> 
    const typename Octree<T>::Root &Octree<T>::Node::getRoot() const
    {
        if (type == NodeType::root) return data.root;
        else throw OctreeExpection(ERROR_TYPE::TYPE_ERROR);
    }

    template <typename T> 
    void Octree<T>::Node::setRoot(const Root &root)
    {
        if (type == NodeType::root) data.root = root;
        else throw OctreeExpection(ERROR_TYPE::TYPE_ERROR);
    }

    template <typename T> 
    const typename Octree<T>::Leave &Octree<T>::Node::getLeave() const
    {
        if (type == NodeType::leave) return data.leave;
        else throw OctreeExpection(ERROR_TYPE::TYPE_ERROR);
    }

    template <typename T> 
    void Octree<T>::Node::setLeave(const Leave &leave)
    {
        if (type == NodeType::leave) data.leave = leave;
        else throw OctreeExpection(ERROR_TYPE::TYPE_ERROR);
    }

    template <typename T>
    Octree<T>::Iterator::Iterator(const NodePoint node)
    {
        if (node->type == NodeType::root)
        {
            this->node = nullptr;
            node_stack.push(node);
            point = Result::xPositive_yPositive_zPositive;

            while (point <= Result::xNegative_yNegative_zPositive)
            {
                NodePoint tmp = *(Node::getNodePositionPoint(node_stack.top(), point));
                if (tmp != nullptr)
                {
                    if (tmp->type == NodeType::root)
                    {
                        node_stack.push(tmp);
                        point_stack.push(point);
                        point = Result::xPositive_yPositive_zPositive;
                        continue;
                    }
                    this->node = tmp;
                    break;
                }
                point++;
            }

            if (this->node == nullptr) throw new OctreeExpection(ERROR_TYPE::TREE_EMPTY); 
        }
        else
        {
            this->node = node; 
        }
        
    }

    template <typename T>
    const bool Octree<T>::Iterator::haveNext() const
    {
        if (this->empty())
            return false;

        Result p = point + 1;
        NodePoint n = node_stack.top();

        while (p <= Result::xNegative_yNegative_zPositive)
        {
            if (*(Node::getNodePositionPoint(n, p++)) != nullptr) return true;
        }

        p = point;
        point = point_stack.top();

        node_stack.pop();
        point_stack.pop();

        bool result = this->haveNext();

        node_stack.push(n);
        point_stack.push(point);
        point = p;

        return result; 
    }

    template <typename T>
    const typename Octree<T>::Leave &Octree<T>::Iterator::operator*() const
    {
        return node->data.leave;
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
    ) : treeRoot(new Node(Root(xMin, xMax, yMin, yMax, zMin, zMax))), size(0) { }

    template <typename T>
    Octree<T>::Octree(const Root &r) : treeRoot(new Node(r)), size(0) { }

    template <typename T>
    Octree<T>::~Octree() { release(treeRoot); }

    template <typename T>
    inline typename Octree<T>::NodePoint Octree<T>::find(const NodePoint data)
    { return this->find(treeRoot, data); }

    template <typename T>
    inline void Octree<T>::insert(const NodePoint data)
    { 
        if (data->getNodeType() == NodeType::leave)
            this->insert(treeRoot, data);
        else throw  OctreeExpection(ERROR_TYPE::INSERT_ERROR); // TODO Expection
    }

    template <typename T>
    void Octree<T>::insert(NodePoint node, const NodePoint data)
    {
        NodePoint *p = node->getPositionPoint(data);

        if (*p == nullptr)
        {
            *p = new Node(data);
            size++;
            return ;
        }
        else if ((*p)->type == NodeType::leave)
        {
            if ((*p)->comparator(data) == Result::equal) return ; 

            T   xMin = node->data.root.xMin, xMax = node->data.root.xMax,
                yMin = node->data.root.yMin, yMax = node->data.root.yMax,
                zMin = node->data.root.zMin, zMax = node->data.root.zMax,
                 
                xTmp = (node->data.root.xMin + node->data.root.xMax) / 2,
                yTmp = (node->data.root.yMin + node->data.root.yMax) / 2,
                zTmp = (node->data.root.zMin + node->data.root.zMax) / 2;

            NodePoint *q = &(node->xPositive_yPositive_zPositive);
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
        
        insert(*p, data);
    }

    template <typename T>
    inline void Octree<T>::remove(const NodePoint data)
    { return this->remove(&treeRoot, data); }

    template <typename T>
    void Octree<T>::remove(NodePoint *node, const NodePoint data)
    {
        NodePoint *p = (*node)->getPositionPoint(data);
        if (*p == nullptr) return;
        else if ((*p)->type == NodeType::root) remove(p, data);
        else 
        {
            delete *p;
            *p = nullptr;
            size--;
        }

        if 
        (
            *p == nullptr && 
            (*node)->xPositive_yPositive_zPositive == nullptr && 
            (*node)->xPositive_yPositive_zNegative == nullptr && 
            (*node)->xPositive_yNegative_zNegative == nullptr &&
            (*node)->xPositive_yNegative_zPositive == nullptr &&
            (*node)->xNegative_yPositive_zPositive == nullptr &&
            (*node)->xNegative_yPositive_zNegative == nullptr &&
            (*node)->xNegative_yNegative_zNegative == nullptr &&
            (*node)->xNegative_yNegative_zPositive == nullptr
        ) 
        {
            delete *node;
            *node = nullptr;
        }
    }

    template <typename T>
    typename Octree<T>::NodePoint Octree<T>::find(NodePoint node, const NodePoint data)
    {
        NodePoint *p = node->getPositionPoint(data);
        if (*p != nullptr && (*p)->type == NodeType::root)
            return find(*p, data);
        return node;
    }

} // namespace Octree


#endif // OCTREE_H