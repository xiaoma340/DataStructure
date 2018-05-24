#ifndef AVL_H_
#define AVL_H_

#include "BST.h"

#define Balanced(x) (stature((x).lc) == stature((x).rc))//理想平衡条件
#define BalFac(x) (stature((x).lc) - stature((x).rc))//平衡因子
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2))// AVL平衡条件
#define tallerChild(x) (\
    stature((x)->lc) > stature((x)->rc) ? (x)->lc : (/*左高*/ \
    stature((x)->lc) < stature((x)->rc) ? (x)->rc : (/*右高*/ \
    IsLChild(*(x)) ? (x)->lc : (x)->rc /*等高，选择与父亲同侧者*/ \
) ) )

template <typename T>
class AVL : public BST<T> {
public :
    BinNodePosi(T) insert(const T& e) override;
    bool remove(const T& e) override;
    //BST::search()可直接沿用
};

template <typename T>
BinNodePosi(T) AVL<T>::insert(const T& e) {
    BinNodePosi(T)& x = search(e);
    if (x) return x;//确认目标节点不存在
    BinNodePosi(T) xx = x = new BinNode<T>(e, _hot);//创建新节点x
    ++_size;
    //此时若x的父亲_hot高度增加，则其祖父有可能失衡
    for (BinNodePosi(T) g = _hot; g; g = g->parent) {//从x之父出发向上，检查各代祖先
        if (!AvlBalanced(*g)) {//一旦发现g失衡，则使之复衡，并将子树重新接入原树
            FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
            break;
        } else {//否则，g依然平衡，只需要更新其高度
            updateHeight(g);
        }
    }
    return xx;
}

template <typename T>
bool AVL<T>::remove(const T& e) {
    BinNodePosi(T)& x = search(e);
    if (!x) return false;//确认目标存在，_hot设置为命中节点的父亲
    removeAt(x, _hot); --_size;//删除节点x
    for (BinNodePosi(T) g = _hot; g; g = g->parent) {
        if (!AvlBalanced(*g)) {
            g = FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
        }
        updateHeight(g);//即便g未失衡，其高度也有可能降低
    }
    return true;
}

#endif//AVL_H_