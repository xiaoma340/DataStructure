#ifndef RED_BLACK_H_
#define RED_BLACK_H_

#include "BST.h"
/*
����������������ĸ������Ķ�����������
1. ����ʼ��Ϊ��ɫ
2. �ⲿ�ڵ��Ϊ��ɫ
3. ����ڵ���Ϊ��ɫ�����亢�ӽڵ��Ϊ��ɫ
4. ����һ�ⲿ�ڵ㵽���ڵ����;���ڽڵ����Ŀ���
*/

#define IsBlack(p) (!(p) || (RB_BLACK == (p)->color))//�ⲿ�ڵ�Ҳ��Ϊ�ڽڵ�
#define IsRed(p) (!IsBlack(p))//�Ǻڼ���
#define BlackHeightUpdated(x) ( /*������߶ȸ�������*/\
    (stature((x).lc) == stature((x).rc)) && \
    ((x).height == (IsRed(&x) ? stature((x).lc) : stature((x).lc) + 1))\
)

template <typename T>
class RedBlack : public BST<T> {
public:
    BinNodePosi(T) insert(const T& e) override;
    bool remove(const T& e) override;
    //search()�ӿ�����
protected:
    void solveDoubleRed(BinNodePosi(T) x);//˫������
    void solveDoubleBlack(BinNodePosi(T) x);//˫������
    int updateHeight(BinNodePosi(T) x);//���½ڵ�x�ĸ߶�
};
//
template <typename T>
BinNodePosi(T) RedBlack<T>::insert(const T& e) {
    BinNodePosi(T)& x = search(e);
    if (x) return x;//ȷ��Ŀ�겻���ڣ�_hot��Ϊ������ڵ�ĸ���
    x = new BinNode<T>(e, _hot, nullptr, nullptr, -1);//������ڵ㣬�ڸ߶�Ϊ-1
    ++_size;
    solveDoubleRed(x);
    return x ? x : _hot->parent;
}
template <typename T>
bool RedBlack<T>::remove(const T& e) {

}

template <typename T>
void RedBlack<T>::solveDoubleRed(BinNodePosi(T) x) {
    if (IsRoot(*x)) {//���Ѿ��ݹ��������ڵ㣬����ת��
        _root->color = RB_BLACK;
        _root->height++;
        return;
    }
    BinNodePosi(T) p = x->parent;
    if (IsBlack(p)) return;//��x�ĸ���Ϊ��ɫ����ֹͣ�ݹ�
    BinNodePosi(T) g = p->parent;//��pΪ����p�ĸ��ױ�Ȼ���ڣ��ұ�Ϊ��ɫ
    BinNodePosi(T) u = uncle(x);//���£���x������u����ɫ�ֱ���
    if (IsBlack(u)) {
        if (IsLChild(*x) == IsLChild(*p)) {//��x��pͬ��
            p->color = RB_BLACK;//p�ɺ�ת�ڣ�x���ֺ�
        } else {
            x->color = RB_BLACK;
        }
        g->color = RB_RED;//g�ض��ɺ�ת��
        BinNodePosi(T) gg = g->parent;
        BinNodePosi(T) r = FromParentTo(*g) = rotateAt(x);//��ת��rΪ�������������
        r->parent = gg;
    } else {
        p->color = RB_BLACK;
        p->height++;
        u->color = RB_BLACK;
        u->height++;
        if (!IsRoot(*g)) g->color = RB_RED;//��g�Ǹ�����ת��
        solveDoubleRed(g);
    }
}

template <typename T>
void RedBlack<T>::solveDoubleBlack(BinNodePosi(T) x)
{

}

template <typename T>
int RedBlack<T>::updateHeight(BinNodePosi(T) x) {
    x->height = std::max(stature(x->lc), stature(x->rc));
    return IsBlack(x) ? x->height++ : x->height;//����ǰ�ڵ�Ϊ�������ڸ߶�
}

#endif//RED_BLACK_H_


