#ifndef SPLAY_H_
#define SPLAY_H_

#include "BST.h"

template <typename T>
class Splay : public: BST<T>{
public:
    BinNodePosi(T)& search(const T& e) override;
    BinNodePosi(T) insert(const T& e) override;
    bool remove(const T& e) override;
protected:
    BinNodePosi(T) splay(BinNodePosi(T) v);//将节点v伸展至树根
};

template <typename T>
BinNodePosi(T)& Splay<T>::search(const T& e) {
    BinNodePosi(T) p = searchIn(_root, e, _hot = nullptr);
    _root = splay(p ? p : _hot);//最后一个被访问的节点伸展至树根
    return _root;
}

template <typename T>
BinNodePosi(T) Splay<T>::insert(const T& e) {
    if (!_root) {//处理原树为空的情况
        ++_size;
        return _root = new BinNode<T>(e);
    }
    if (e == search(e)->data) return _root;//确认目标不存在,_root为待插入父节点
    ++_size;
    BinNodePosi(T) t = _root;
    if (_root->data < e) {//插入新根，以t和t->rc为左右孩子
        t->parent = _root = new BinNode<T>(e, nullptr, t, t->lc);
        if (HasRChild(*t)) {
            t->rc->parent = _root;
            t->rc = nullptr;
        }
    } else {
        t->parent = _root = new BinNode<T>(e, nullptr, t->lc, t);
        if (HasLChild(*t)) {
            t->lc->parent = _root;
            t->lc = nullptr;
        }
    }
    updateHeightAbove(t);
    return _root;
}

template <typename T>
bool Splay<T>::remove(const T& e) {
    if (!_root || (e != search(e)->data)) return false;//若树空或目标不存在
    BinNodePosi(T) w = _root;//待删除节点已经在_root处
    if (!HasLChild(*_root)) {//若无左子树则直接删除
        _root = _root->rc;
        if (_root) _root->parent = nullptr;
    } else if(!HasRChild(*_root)) {//若无右子树也直接删除
        _root = _root->lc;
        if (_root) _root->parent = nullptr;
    } else {
        BinNodePosi(T) left_tree = _root->lc;
        left_tree->parent = nullptr;
        _root->lc = nullptr;//将左子树暂时切除
        _root = _root->rc;
        _root->parent = nullptr;//只保留右子树
        search(w->data);//以待删除数据为目标，在原右子树中做一次必定失败的查找，将
        //右子树中最小节点伸展至树根
        _root->lc = left_tree;
        left_tree->parent = _root;//只需要将原左子树接回原位
    }
    delete w->data;
    delete w;
    --_size;
    if (_root) updateHeight(_root);
    return true;
}

template <typename NodePosi>
inline void attachAsLChild(NodePosi p, NodePosi lc) {
    p->lc = lc;
    if (lc) lc->parent = p;
}
template <typename NodePosi>
inline void attachAsRChild(NodePosi p, NodePosi rc) {
    p->rc = rc;
    if (rc) rc->parent = p;
}
template <typename T>
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v) {
    if (!v) return nullptr;
    BinNodePosi(T) p;
    BinNodePosi(T) g;
    while ((p = v->parent) && (g = p->parent)) {
        BinNodePosi(T) gg = g->parent;
        if (IsLChild(*v)) {
            if (IsLChild(*p)) {//zig-zig
                attachAsLChild(g, p->rc);
                attachAsLChild(p, v->rc);
                attachAsRChild(p, g);
                attachAsRChild(v, p);
            } else {//zig-zag
                attachAsLChild(p, v->rc);
                attachAsRChild(g, v->lc);
                attachAsLChild(v, g);
                attachAsRChild(v, p);
            }
        } else {
            if (IsRChild(*p)) {//zag-zag
                attachAsRChild(g, p->lc);
                attachAsRChild(p, v->lc);
                attachAsLChild(p, g);
                attachAsLChild(v, p);
            } else {
                attachAsRChild(p, v->lc);
                attachAsLChild(g, v->lc);
                attachAsRChild(v, g);
                attachAsLChild(v, p);
            }
        }
        if (!gg) {
            v->parent = nullptr;//若原先v的曾祖父gg不存在，则v应为树根
        } else {
            (g == gg->lc) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);
        }
        updateHeight(g);
        updateHeight(p);
        updateHeight(v);
    }//双层伸展结束时必有g==nullptr，但p未必为空
    if (p = v->parent) {//若p非空，则再做一次单旋
        if (IsLChild(*v)) {
            attachAsLChild(p, v->rc);
            attachAsRChild(v, p);
        } else {
            attachAsRChild(p, v->lc);
            attachAsLChild(v, p);
        }
        updateHeight(p);
        updateHeight(v);
    }
    v->parent = nullptr;
    return v;
}

#endif//SPLAY_H_