#ifndef BST_H_
#define BST_H_

#include "BinTree.h"

template <typename T>
class BST : public BinTree<T> {
public:
    virtual BinNodePosi(T)& search(const T& e);
    virtual BinNodePosi(T) insert(const T& e);
    virtual bool remove(const T& e);
protected:
    BinNodePosi(T) _hot;//命中节点的父亲
    BinNodePosi(T) connect34(
        BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
        BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
    BinNodePosi(T) rotateAt(BinNodePosi(T) x);//对x及其父亲，祖父统一旋转调整
};

//查找
template <typename T>//在以v为根的BST子树中查找关键吗e
static BinNodePosi(T)& searchIn(BinNodePosi(T)& v, const T& e,
    BinNodePosi(T)& hot) {
    if (!v || (e == v->data)) return v;
    hot = v;
    return searchIn(((e < v->data) ? v->lc : v->rc), e, hot);
}
template <typename T>
BinNodePosi(T)& BST<T>::search(const T& e) {
    return searchIn(_root, e, _hot = nullptr);//返回目标节点处的引用，以便后续插入，删除操作
}
//插入
template <typename T>
BinNodePosi(T) BST<T>::insert(const T& e) {
    BinNodePosi(T)& x = search(e);
    if (x) return x;//确认目标不存在才插入
    x = new BinNode<T>(e, _hot);//x指向待插入位置，_hot指向x的父节点
    ++_size;
    updateHeightAbove(x);
    return x;
}
//删除
template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T)& x, BinNodePosi(T)& hot) {
    BinNodePosi(T) w = x;//实际被摘除的节点， 初值同x
    BinNodePosi(T) succ = nullptr;//实际被删除节点的接替者
    if (!HasLChild(*x)) {
        succ = x = x->rc;//若没有左孩子，则x的后继为其右孩子,x替换为右孩子
    } else if (!HasRChild(*x)) {
        succ = x = x->lc;//若没有右孩子，则x的后继为其左孩子,x替换为左孩子
    } else {//若左右子树均存在，则选择x的直接后继作为实际被摘除节点
        w = w->succ();//此时x的后继w必定在x的右子树中，且没有左孩子
        std::swap(x->data, w->data);
        BinNodePosi(T) u = w->parent;
        ((u == x) ? u->rc : u->lc) = succ = w->rc;//隔离节点w
    }
    hot = w->parent;//记录实际被删除节点的父亲
    if (succ) succ->parent = hot;//将被删除节点的接替者与hot相连
    delete w->data;
    delete w;
    return succ;
}
template <typename T>
bool BST<T>::remove(const T& e) {
    BinNodePosi(T)& x = search(e);
    if (!x) return false;
    removeAt(x, _hot);
    --_size;
    updateHeightAbove(_hot);
    return true;
}

template <typename T>
BinNodePosi(T) BST<T>::connect34(
    BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c, 
    BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3) {
    a->lc = T0;
    if (T0) T0->parent = a;
    a->rc = T1;
    if (T1) T1->parent = a;
    updateHeight(a);
    c->lc = T2;
    if (T2) T2->parent = c;
    c->rc = T3;
    if (T3) T3->parent = c;
    updateHeight(c);
    b->lc = a;
    a->parent = b;
    b->rc = c;
    c->parent = b;
    updateHeight(a);
    return b;//返回根节点
}

template <typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v) {//v为非空孙子辈节点
    BinNodePosi(T) p = v->parent;
    BinNodePosi(T) g = p->parent;
    if (IsLChild(*p)) {//zig
        if (IsLChild(*v)) {//zig-zig
            p->parent = g->parent;
            return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
        } else {//zig-zag
            v->parent = g->parent;
            return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
        }
    } else {//zag
        if (IsRChild(*v)) {//zag-zig
            p->parent = g->parent;
            return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
        } else {//zag-zag
            v->parent = g->parent;
            return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
        }
    }
}

#endif//BST_H_