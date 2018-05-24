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
    BinNodePosi(T) _hot;//���нڵ�ĸ���
    BinNodePosi(T) connect34(
        BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
        BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
    BinNodePosi(T) rotateAt(BinNodePosi(T) x);//��x���丸�ף��游ͳһ��ת����
};

//����
template <typename T>//����vΪ����BST�����в��ҹؼ���e
static BinNodePosi(T)& searchIn(BinNodePosi(T)& v, const T& e,
    BinNodePosi(T)& hot) {
    if (!v || (e == v->data)) return v;
    hot = v;
    return searchIn(((e < v->data) ? v->lc : v->rc), e, hot);
}
template <typename T>
BinNodePosi(T)& BST<T>::search(const T& e) {
    return searchIn(_root, e, _hot = nullptr);//����Ŀ��ڵ㴦�����ã��Ա�������룬ɾ������
}
//����
template <typename T>
BinNodePosi(T) BST<T>::insert(const T& e) {
    BinNodePosi(T)& x = search(e);
    if (x) return x;//ȷ��Ŀ�겻���ڲŲ���
    x = new BinNode<T>(e, _hot);//xָ�������λ�ã�_hotָ��x�ĸ��ڵ�
    ++_size;
    updateHeightAbove(x);
    return x;
}
//ɾ��
template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T)& x, BinNodePosi(T)& hot) {
    BinNodePosi(T) w = x;//ʵ�ʱ�ժ���Ľڵ㣬 ��ֵͬx
    BinNodePosi(T) succ = nullptr;//ʵ�ʱ�ɾ���ڵ�Ľ�����
    if (!HasLChild(*x)) {
        succ = x = x->rc;//��û�����ӣ���x�ĺ��Ϊ���Һ���,x�滻Ϊ�Һ���
    } else if (!HasRChild(*x)) {
        succ = x = x->lc;//��û���Һ��ӣ���x�ĺ��Ϊ������,x�滻Ϊ����
    } else {//���������������ڣ���ѡ��x��ֱ�Ӻ����Ϊʵ�ʱ�ժ���ڵ�
        w = w->succ();//��ʱx�ĺ��w�ض���x���������У���û������
        std::swap(x->data, w->data);
        BinNodePosi(T) u = w->parent;
        ((u == x) ? u->rc : u->lc) = succ = w->rc;//����ڵ�w
    }
    hot = w->parent;//��¼ʵ�ʱ�ɾ���ڵ�ĸ���
    if (succ) succ->parent = hot;//����ɾ���ڵ�Ľ�������hot����
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
    return b;//���ظ��ڵ�
}

template <typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v) {//vΪ�ǿ����ӱ��ڵ�
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