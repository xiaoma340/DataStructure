#ifndef RED_BLACK_H_
#define RED_BLACK_H_

#include "BST.h"
/*
红黑树是满足下列四个条件的二叉搜索树：
1. 树根始终为黑色
2. 外部节点均为黑色
3. 其余节点若为红色，则其孩子节点比为黑色
4. 从任一外部节点到根节点的沿途，黑节点的数目相等
*/

#define IsBlack(p) (!(p) || (RB_BLACK == (p)->color))//外部节点也视为黑节点
#define IsRed(p) (!IsBlack(p))//非黑即红
#define BlackHeightUpdated(x) ( /*红黑树高度更新条件*/\
    (stature((x).lc) == stature((x).rc)) && \
    ((x).height == (IsRed(&x) ? stature((x).lc) : stature((x).lc) + 1))\
)

template <typename T>
class RedBlack : public BST<T> {
public:
    BinNodePosi(T) insert(const T& e) override;
    bool remove(const T& e) override;
    //search()接口沿用
protected:
    void solveDoubleRed(BinNodePosi(T) x);//双红修正
    void solveDoubleBlack(BinNodePosi(T) x);//双黑修正
    int updateHeight(BinNodePosi(T) x);//更新节点x的高度
};
//
template <typename T>
BinNodePosi(T) RedBlack<T>::insert(const T& e) {
    BinNodePosi(T)& x = search(e);
    if (x) return x;//确认目标不存在，_hot变为待插入节点的父亲
    x = new BinNode<T>(e, _hot, nullptr, nullptr, -1);//创建红节点，黑高度为-1
    ++_size;
    solveDoubleRed(x);
    return x ? x : _hot->parent;
}
template <typename T>
bool RedBlack<T>::remove(const T& e) {
    BinNodePosi(T)& x = search(e);
    if (!x) return false;//确认目标存在
    BinNodePosi(T) r = removeAt(x, _hot);//r为被删除节点的后继，_hot为r的父亲
    if (!(--_size)) return false;//实施删除，树变空则返回
    if (!_hot) {//若刚删除的是根节点，则将根节点置黑，并更新高度
        _root->color = RB_BLACK;
        updateHeight(_root);
        return true;
    }
    if (BlackHeightUpdated(*_hot)) return true;//若所有祖先黑深度依然平衡，则无需调整
    if (IsRed(r)) {//若r为红，则只需要令其转黑
        r->color = RB_BLACK;
        r->height++;
        return true;
    }
    solveDoubleBlack(r);
    return true;
}

template <typename T>
void RedBlack<T>::solveDoubleRed(BinNodePosi(T) x) {
    if (IsRoot(*x)) {//若已经递归至树根节点，则将其转黑
        _root->color = RB_BLACK;
        _root->height++;
        return;
    }
    BinNodePosi(T) p = x->parent;
    if (IsBlack(p)) return;//若x的父亲为黑色，则停止递归
    BinNodePosi(T) g = p->parent;//若p为红则p的父亲必然存在，且必为黑色
    BinNodePosi(T) u = uncle(x);//以下，视x的叔叔u的颜色分别处理
    if (IsBlack(u)) {
        if (IsLChild(*x) == IsLChild(*p)) {//若x与p同侧
            p->color = RB_BLACK;//p由红转黑，x保持红
        } else {
            x->color = RB_BLACK;
        }
        g->color = RB_RED;//g必定由黑转红
        BinNodePosi(T) gg = g->parent;
        BinNodePosi(T) r = FromParentTo(*g) = rotateAt(x);//旋转，r为调整后的子树根
        r->parent = gg;
    } else {
        p->color = RB_BLACK;
        p->height++;
        u->color = RB_BLACK;
        u->height++;
        if (!IsRoot(*g)) g->color = RB_RED;//若g非根，则转红
        solveDoubleRed(g);
    }
}

template <typename T>
void RedBlack<T>::solveDoubleBlack(BinNodePosi(T) x) {
    BinNodePosi(T) p = r ? r->parent : _hot;
    if (!p) return;
    BinNodePosi(T) s = (r == p->lc) ? p->rc : p->lc;//r的兄弟
    if (IsBlack(s)) {
        BinNodePosi(T) t = nullptr;
        if (IsRed(s->rc)) t = s->rc;
        if (IsRed(s->lc)) t = s->lc;
        if (t) {
            RBColor oldColor = p->color;
            BinNodePosi(T) b = FromParentTo(*p) = rotateAt(t);
            if (HasLChild(*b)) {
                b->lc->color = RB_BLACK;
                updateHeight(b->lc);
            }
            if (HasRChild(*b)) {
                b->rc->color = RB_BLACK;
                updateHeight(b->rc);
            }
            b->color = oldColor;
            updateHeight(b);
        } else {
            s->color = RB_RED;
            s->height--;
            if (IsRed(p)) {
                p->color = RB_BLACK;
            } else {
                p->height--;
                solveDoubleBlack(p);
            }
        }
    } else {
        s->color = RB_BLACK;
        p->color = RB_RED;
        BinNodePosi(T) t = IsLChild(*s) ? s->lc : s->rc;
        _hot = p;
        FromParentTo(*p) = rotateAt(t);
        solveDoubleBlack(r);
    }
}

template <typename T>
int RedBlack<T>::updateHeight(BinNodePosi(T) x) {
    x->height = std::max(stature(x->lc), stature(x->rc));
    return IsBlack(x) ? x->height++ : x->height;//若当前节点为黑则计入黑高度
}

#endif//RED_BLACK_H_


