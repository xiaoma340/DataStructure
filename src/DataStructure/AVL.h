#ifndef AVL_H_
#define AVL_H_

#include "BST.h"

#define Balanced(x) (stature((x).lc) == stature((x).rc))//����ƽ������
#define BalFac(x) (stature((x).lc) - stature((x).rc))//ƽ������
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2))// AVLƽ������
#define tallerChild(x) (\
    stature((x)->lc) > stature((x)->rc) ? (x)->lc : (/*���*/ \
    stature((x)->lc) < stature((x)->rc) ? (x)->rc : (/*�Ҹ�*/ \
    IsLChild(*(x)) ? (x)->lc : (x)->rc /*�ȸߣ�ѡ���븸��ͬ����*/ \
) ) )

template <typename T>
class AVL : public BST<T> {
public :
    BinNodePosi(T) insert(const T& e) override;
    bool remove(const T& e) override;
    //BST::search()��ֱ������
};

template <typename T>
BinNodePosi(T) AVL<T>::insert(const T& e) {
    BinNodePosi(T)& x = search(e);
    if (x) return x;//ȷ��Ŀ��ڵ㲻����
    BinNodePosi(T) xx = x = new BinNode<T>(e, _hot);//�����½ڵ�x
    ++_size;
    //��ʱ��x�ĸ���_hot�߶����ӣ������游�п���ʧ��
    for (BinNodePosi(T) g = _hot; g; g = g->parent) {//��x֮���������ϣ�����������
        if (!AvlBalanced(*g)) {//һ������gʧ�⣬��ʹ֮���⣬�����������½���ԭ��
            FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
            break;
        } else {//����g��Ȼƽ�⣬ֻ��Ҫ������߶�
            updateHeight(g);
        }
    }
    return xx;
}

template <typename T>
bool AVL<T>::remove(const T& e) {
    BinNodePosi(T)& x = search(e);
    if (!x) return false;//ȷ��Ŀ����ڣ�_hot����Ϊ���нڵ�ĸ���
    removeAt(x, _hot); --_size;//ɾ���ڵ�x
    for (BinNodePosi(T) g = _hot; g; g = g->parent) {
        if (!AvlBalanced(*g)) {
            g = FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
        }
        updateHeight(g);//����gδʧ�⣬��߶�Ҳ�п��ܽ���
    }
    return true;
}

#endif//AVL_H_