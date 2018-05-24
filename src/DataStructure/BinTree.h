#include "BinNode.h"

template <typename T>
class BinTree {
public:
    BinTree() : _size(0), _root(nullptr) { }
    virtual ~BinTree() {
        if (0 < _size) remove(_root);
    }
    int size() const {
        return _size;
    }
    bool empty() const {
        return !_root;
    }
    BinNodePosi(T) root() const {
        return _root;
    }
    BinNodePosi(T) insertAsRoot(const T& e);
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x, const T& e);
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x, const T& e);
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>*& t);
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>*& t);
    int remove(BinNodePosi(T) x);//删除节点x处为根的子树，返回该子树原先的规模
    BinTree<T>* secede(BinNodePosi(T) x);//将子树x从当前树中摘除，并转换成一棵独立的子树
    template <typename VST>
    void travLevel(VST& visit) {
        if (_root) _root->travLevel(visit);
    }
    template <typename VST>
    void travPre(VST& visit) {
        if (_root) _root->travPre(visit);
    }
    template <typename VST>
    void travIn(VST& visit) {
        if (_root) _root->travIn(visit);
    }
    template <typename VST>
    void travPost(VST& visit) {
        if (_root) _root->travPost(visit);
    }
    bool operator<(const BinTree<T>& t) {
        return _root && t._root && *_root < *(t._root);
    }
    bool operator==(const BinTree<T>& t) {
        return _root && t._root && _root == t._root;
    }
protected:
    int _size;
    BinNodePosi(T) _root;
    virtual int updateHeight(BinNodePosi(T) x);//更新节点x的高度
    void updateHeightAbove(BinNodePosi(T) x);//更新节点x及其祖先的高度
};

template <typename T>
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) {
    FromParentTo(*x) = nullptr;
    updateHeightAbove(x->parent);
    BinTree<T>* s = new BinTree<T>;
    s->_root = x;
    x->parent = nullptr;
    s->_size = x->size();
    _size -= s->_size;
    return s;
}

template <typename T>
static int removeAt(BinNodePosi(T) x) {
    if (!x) return 0;
    int n = 1 + removeAt(x->lc) + removeAt(x->rc);
    delete x->data;
    delete x;
    return n;
}
template <typename T>
int BinTree<T>::remove(BinNodePosi(T) x) {
    FromParentTo(*x) = nullptr;
    updateHeightAbove(x->parent);
    int n = removeAt(x);
    _size -= n;
    return n;
}

template <typename T>
BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>*& t) {
    if (x->rc = t->_root) x->rc->parent = x;
    _size += t->_size;
    updateHeightAbove(x);
    t->_root = nullptr;
    t->_size = 0;
    delete t;
    t = nullptr;
    return x;
}

template <typename T>
BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>*& t) {
    if (x->lc = t->_root) x->lc->parent = x;//接入
    _size += t->_size;
    updateHeightAbove(x);
    t->_root = nullptr;
    t->_size = 0;
    delete t;
    t = nullptr;
    return x;
}

template <typename T>
BinTree<T>::insertAsRC(BinNodePosi(T) x, const T& e) {
    ++_size;
    x->insertAsRC(e);
    updateHeightAbove(x);
    return x->rc;
}

template <typename T>
BinTree<T>::insertAsLC(BinNodePosi(T) x, const T& e) {
    ++_size;
    x->insertAsLC(e);
    updateHeightAbove(x);
    return x->lc;
}

template <typename T>
BinTree<T>::insertAsRoot(const T& e) {
    _size = 1;
    return _root = new BinNode<T>(e);
}

template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) {
    while (x) {
        updateHeight(x);
        x = x->parent;
    }
}

template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) {
    return x->height = 1 + max(stature(x->lc), stature(x->rc));
}

