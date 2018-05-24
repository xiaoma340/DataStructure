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
    BinNodePosi(T)& x = search(e);
    if (!x) return false;//ȷ��Ŀ�����
    BinNodePosi(T) r = removeAt(x, _hot);//rΪ��ɾ���ڵ�ĺ�̣�_hotΪr�ĸ���
    if (!(--_size)) return false;//ʵʩɾ����������򷵻�
    if (!_hot) {//����ɾ�����Ǹ��ڵ㣬�򽫸��ڵ��úڣ������¸߶�
        _root->color = RB_BLACK;
        updateHeight(_root);
        return true;
    }
    if (BlackHeightUpdated(*_hot)) return true;//���������Ⱥ������Ȼƽ�⣬���������
    if (IsRed(r)) {//��rΪ�죬��ֻ��Ҫ����ת��
        r->color = RB_BLACK;
        r->height++;
        return true;
    }
    solveDoubleBlack(r);
    return true;
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
void RedBlack<T>::solveDoubleBlack(BinNodePosi(T) x) {
    BinNodePosi(T) p = r ? r->parent : _hot;
    if (!p) return;
    BinNodePosi(T) s = (r == p->lc) ? p->rc : p->lc;//r���ֵ�
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
    return IsBlack(x) ? x->height++ : x->height;//����ǰ�ڵ�Ϊ�������ڸ߶�
}

#endif//RED_BLACK_H_


