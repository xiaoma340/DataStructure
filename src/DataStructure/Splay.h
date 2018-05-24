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
    BinNodePosi(T) splay(BinNodePosi(T) v);//���ڵ�v��չ������
};

template <typename T>
BinNodePosi(T)& Splay<T>::search(const T& e) {
    BinNodePosi(T) p = searchIn(_root, e, _hot = nullptr);
    _root = splay(p ? p : _hot);//���һ�������ʵĽڵ���չ������
    return _root;
}

template <typename T>
BinNodePosi(T) Splay<T>::insert(const T& e) {
    if (!_root) {//����ԭ��Ϊ�յ����
        ++_size;
        return _root = new BinNode<T>(e);
    }
    if (e == search(e)->data) return _root;//ȷ��Ŀ�겻����,_rootΪ�����븸�ڵ�
    ++_size;
    BinNodePosi(T) t = _root;
    if (_root->data < e) {//�����¸�����t��t->rcΪ���Һ���
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
    if (!_root || (e != search(e)->data)) return false;//�����ջ�Ŀ�겻����
    BinNodePosi(T) w = _root;//��ɾ���ڵ��Ѿ���_root��
    if (!HasLChild(*_root)) {//������������ֱ��ɾ��
        _root = _root->rc;
        if (_root) _root->parent = nullptr;
    } else if(!HasRChild(*_root)) {//����������Ҳֱ��ɾ��
        _root = _root->lc;
        if (_root) _root->parent = nullptr;
    } else {
        BinNodePosi(T) left_tree = _root->lc;
        left_tree->parent = nullptr;
        _root->lc = nullptr;//����������ʱ�г�
        _root = _root->rc;
        _root->parent = nullptr;//ֻ����������
        search(w->data);//�Դ�ɾ������ΪĿ�꣬��ԭ����������һ�αض�ʧ�ܵĲ��ң���
        //����������С�ڵ���չ������
        _root->lc = left_tree;
        left_tree->parent = _root;//ֻ��Ҫ��ԭ�������ӻ�ԭλ
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
            v->parent = nullptr;//��ԭ��v�����游gg�����ڣ���vӦΪ����
        } else {
            (g == gg->lc) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);
        }
        updateHeight(g);
        updateHeight(p);
        updateHeight(v);
    }//˫����չ����ʱ����g==nullptr����pδ��Ϊ��
    if (p = v->parent) {//��p�ǿգ�������һ�ε���
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