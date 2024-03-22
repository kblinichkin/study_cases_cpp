#include <iostream>
#include <memory>

template <typename T, typename C = std::less<T>>
class BinarySearchTree {
    struct Node {
        // FIXME: to simplify algorhithms, make it std::unique_ptr<Node>* instead
        //        of Node*.
        // Node* parent { nullptr };
        std::unique_ptr<Node>* parent { nullptr };
        std::unique_ptr<Node> left { nullptr };
        std::unique_ptr<Node> right { nullptr };
        T value;

        Node(const T& value) : value (value) {}
        ~Node() {
            std::cout << "Node::~Node(): value - " << value << std::endl;
        }
    };
    std::unique_ptr<Node> m_root { nullptr };

    class iterator {
        // std::unique_ptr<Node>* m_root;
        // BinarySearchTree<T>* m_tree;
        std::unique_ptr<Node>* m_ptr;

        iterator(/*BinarySearchTree<T>* tree, */std::unique_ptr<Node>* curr_elm) :
            // m_tree(tree),
            m_ptr(curr_elm)
        {}
        friend class BinarySearchTree<T>;
    public:
        iterator(const iterator&) = default;
        ~iterator() = default;
        iterator& operator=(const iterator&) = default;

        T const& operator*() const {
            return (*m_ptr)->value;
        }
        iterator& operator++() {
            if ((*m_ptr)->right != nullptr) {
                m_ptr = BinarySearchTree<T>::treeMinimum(&(*m_ptr)->right);
                return *this;
            }

            std::unique_ptr<Node>* parent = (*m_ptr)->parent;
            while (parent != nullptr && *m_ptr == (*parent)->right) {
                m_ptr = parent;
                parent = (*parent)->parent;
            }
            m_ptr = parent;

            // FIXME: no case to return std::end

            return *this;
        }
        iterator& operator--() {
            if ((*m_ptr)->left != nullptr) {
                m_ptr = BinarySearchTree<T>::treeMaximum(&(*m_ptr)->left);
                return *this;
            }

            std::unique_ptr<Node>* parent = (*m_ptr)->parent;
            while (parent != nullptr && *m_ptr == (*parent)->left) {
                m_ptr = parent;
                parent = (*parent)->parent;
            }
            m_ptr = parent;

            // FIXME: no case to return std::end

            return *this;
        }
        bool operator!=(iterator const & rhs) {
            return m_ptr != rhs.m_ptr;
        }
    };
    friend class iterator;

public:
    BinarySearchTree() = default;

    // FIXME: return an iterator once it's implemented
    std::unique_ptr<Node>* find(const T& value) {
        C comp;
        auto* currNode = &m_root;
        while (*currNode != nullptr && (*currNode)->value != value) {
            currNode = comp(value, (*currNode)->value)
                        ? &(*currNode)->left
                        : &(*currNode)->right;
        }

        return currNode;
    }

    // FIXME: return an iterator to the inserted element once it's implemented
    void insert(const T& value) {
        C comp;

        // Despite the fact that this code below looks strange, operating by
        // pointers to std::unique_ptr makes this algorithm short and efficient.
        // Using std::shared_ptr here would add en excessive memory consumption
        // at least.
        auto* currNode = &m_root;
        auto* parentNode = static_cast<std::unique_ptr<Node>*>(nullptr);
        while (*currNode != nullptr) {
            parentNode = currNode;
            currNode = comp(value, (*currNode)->value)
                        ? &(*currNode)->left
                        : &(*currNode)->right;
        }
        (*currNode).reset(new Node{ value });
        // if (parentNode != nullptr) (*currNode)->parent = (*parentNode).get();
        if (parentNode != nullptr) (*currNode)->parent = parentNode;
    }

    // FIXME: return an iterator to the successor once it's implemented
    void remove(std::unique_ptr<Node>* it) {
        if ((*it)->right == nullptr && (*it)->left == nullptr) {
            (*it).reset();
        // 1. cases a) and b)
        } else if ((*it)->right == nullptr) {
            transplant(&(*it)->left, it);
        } else if ((*it)->left == nullptr) {
            transplant(&(*it)->right, it);
        } else {
            // 2. cases c) and d)
            auto* min = treeMinimum(&(*it)->right);
            if ((*min)->parent != &(*it)) {
                auto tmpX = std::unique_ptr<Node>();
                if ((*min)->right != nullptr) {
                    (*min)->right->parent = (*min)->parent;
                    tmpX = std::move((*min)->right);
                }
                auto tmpY = std::unique_ptr<Node>();
                (*min).swap(tmpY);
                (*it)->right->parent = &tmpY;
                tmpY->right = std::move((*it)->right);
                (*it)->left->parent = &tmpY;
                tmpY->left = std::move((*it)->left);
                tmpY->parent = (*it)->parent;
                (*it) = std::move(tmpY);
                if (tmpX != nullptr) {
                    tmpX.swap(*min);
                }
            } else {
                (*min)->left.swap((*it)->left);
                transplant(&(*it)->right, it);
            }
        }
    }

    iterator begin() {
        return iterator(/*this, */treeMinimum(&m_root));
    }

    iterator end() {
        return iterator(/*this, */nullptr);
    }

    void printAll() {
        // Traverse a sorted tree structure
        std::cout << "[ ";
        inOrderTreeWalk(m_root.get(), [](auto* node) {
            std::cout << node->value << " ";
        });
        std::cout << "]" << std::endl;
    }
private:
    // FIXME: use iterators instead
    static std::unique_ptr<Node>* treeMinimum(std::unique_ptr<Node>* currNode) {
        // auto* currNode = &m_root;
        while (*currNode != nullptr && (*currNode)->left != nullptr)
            currNode = &(*currNode)->left;

        return currNode;
    }
    static std::unique_ptr<Node>* treeMaximum(std::unique_ptr<Node>* currNode) {
        // auto* currNode = &m_root;
        while (*currNode != nullptr && (*currNode)->right != nullptr)
            currNode = &(*currNode)->right;

        return currNode;
    }

    void transplant(std::unique_ptr<Node>* u, std::unique_ptr<Node>* v) {
        (*u)->parent = (*v)->parent;
        auto tmp = std::move(*u); // FIXME: Do we need this?..
        (*v) = std::move(tmp);
    }

    // TODO: rewrite it using cycle instead of recursion
    template <typename F>
    void inOrderTreeWalk(Node* node, F func) {
        if (node != nullptr) {
            inOrderTreeWalk(node->left.get(), func);
            func(node);
            inOrderTreeWalk(node->right.get(), func);
        }
    }
};
// PLAN for tomorrow:
// +1. Finish remove() function
// 2. Implement tasks from the book
//      b) Move to the theory
// 3. Implement iterator
// 4. Move functions to iterator if possible

int main() {
    BinarySearchTree<int> tree;
    tree.insert(11);
    tree.insert(12);
    tree.insert(4);
    tree.insert(2);
    tree.insert(1);
    tree.insert(3);
    tree.insert(9);
    tree.insert(10);
    tree.insert(7);
    tree.insert(8);
    tree.insert(5);
    tree.insert(6);


    tree.printAll();

    // auto* it_temp = tree.find(4);
    // tree.remove(it_temp);
    //
    // tree.printAll();

    auto it_a = tree.begin();
    auto it_b(it_a);
    std::cout << "begin: " << *it_b << std::endl;
    std::cout << "++begin: " << *(++it_b) << std::endl;
    std::cout << "--begin: " << *(--it_b) << std::endl;

    std::cout << "{";
    std::for_each(tree.begin(), tree.end(), [](const int val){
        std::cout << " " << val;
    });
    std::cout << " }" << std::endl;

    // while(1);
    return 0;
}
