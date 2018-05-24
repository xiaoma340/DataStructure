#ifndef BIN_NODE_H_
#define BIN_NODE_H_

#include <queue>
#include <stack>

#define BinNodePosi(T) BinNode<T>* /*�ڵ�λ��*/
#define stature(p) ((p) ? (p)->height : -1)//�ڵ�߶�
//BinNode״̬�������ж�
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))
//��BinNode���ض���ϵ�Ľڵ㼰ָ��
#define sibling(p) (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->lc)
#define uncle(x) (IsLChild(*((x)->parent)) ? (x)->parent->parent->rc : \
                  (x)->parent->parent->lc)
#define FromParentTo(x) \
    (IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lc : (x).parent->rc))


typedef enum { RB_RED, RB_BLACK } RBColor;//�ڵ���ɫ

template <typename T>
struct BinNode {
    //��Ա
    T data;
    BinNodePosi(T) parent;
    BinNodePosi(T) lc;
    BinNodePosi(T) rc;
    int height;
    int npl;
    RBColor color;
    //���캯��
    BinNode() :parent(nullptr), rc(nullptr), lc(nullptr), height(0), npl(1),
        color(RB_RED){ }
    BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lcc = nullptr, 
        BinNodePosi(T) rcc = nullptr, int h = 0, int l = 1, RBColor c = RB_RED):
        data(e), parent(p), lc(lcc), rc(rcc), height(h), npl(l), color(c) { }
    //�����ӿ�
    int size();//ͳ�Ƶ�ǰ�ڵ�������
    BinNodePosi(T) insertAsLC(const T& e);
    BinNodePosi(T) insertAsRC(const T& e);
    BinNodePosi(T) succ();//��ǰ�ڵ��ֱ�Ӻ��
    template <typename VST>
    void travLevel(VST&);//������α���
    template <typename VST>
    void travPre(VST&);//�����������
    template <typename VST>
    void travIn(VST&);//�����������
    template <typename VST>
    void travPost(VST&);//�����������
    //�Ƚ������е���
    bool operator<(const BinNode& bn) {
        return data < bn.data;
    }
    bool operator=(const BinNode& bn) {
        return data == bn.data;
    }
};
template <typename T>
int BinNode<T>::size() {
    int s = 1;
    if (lc) s += lc->size();
    if (rc) s += rc->size();
}

template <typename T>
BinNode<T>::succ() {
    BinNodePosi(T) s = this;
    if (rc) {
        s = rc;
        while (HasLChild(*s))s = s->lc;
    }
    else {
        while (IsRChild(*s)) s = s->parent;
        s = s->parent;
    }
    return s;
}

template <typename T>
BinNode<T>::insertAsRC(const T& e) {
    return rc = new BinNode(e, this);
}

template <typename T>
BinNode<T>::insertAsLC(const T& e) {
    return lc = new BinNode(e, this);
}
//����
//��������������ݹ��
template <typename T, typename VST>
void travPre_R(BinNodePosi(T) x, VST& visit) {
    if (!x) return;
    visit(x->data);
    travPre_R(x->lc, visit);
    travPre_R(x->rc, visit);
}
//�������������
template <typename T, typename VST>
static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, 
                                std::stack<BinNodePosi(T)>& s) {
    while (x) {
        visit(x->data);
        s.push(x->rc);//�Һ�����ջ
        x = x->lc;//������֧����һ��
    }
}
template <typename T, typename VST>
void travPre_I2(BinNodePosi(T) x, VST& visit) {
    std::stack<BinNodePosi(T)> s;
    while (true) {
        visitAlongLeftBranch(x, visit, s);
        if (s.empty()) break;
        x = s.top();
        s.pop();
    }
}
//��������������ݹ��
template <typename T, typename VST>
void travPost_R(BinNodePosi(T) x, VST& visit) {
    if (!x) return;
    travPost_R(x->lc, visit);
    travPost_R(x->rc, visit);
    visit(x->data);
}
//�������������
template <typename T>
static void gotoHLVFL(std::stack<BinNodePosi(T)>& s) {//��s��ջ��Ϊ�����������ҵ�������ɼ�Ҷ�ڵ�
    while (BinNodePosi(T) x = s.top()) {//�������ջ���ڵ�
        s.pop();
        if (HasLChild(*x)) {//����������
            if (HasRChild(*x)) s.push(x->rc);//�����Һ���������ջ
            s.push(x->lc);//Ȼ���ת������
        } else {//ʵ�����ѣ�������
            s.push(x->rc);
        }
    }
    s.pop();//����ջ���ڵ�
}
template <typename T, typename VST>
void travPost_I(BinNodePosi(T) x, VST& visit) {
    std::stack<BinNodePosi(T)> s;
    if (x) s.push(x);
    while (!s.empty()) {
        if (s.top() != x->parent) {//��ջ���ǵ�ǰ�ڵ�֮�������Ϊ�����֣�������Ҫ
            gotoHLVFL(s);//����������Ϊ���������У��ҵ�HLVFL
        }
        x = s.top();
        s.pop();
        visit(x->data);
    }
}
//��������������ݹ��
template <typename T, typename VST>
void travIn_R(BinNodePosi(T) x, VST& visit) {
    if (!x) return;
    travIn_R(x->lc, visit);
    visit(x->data);
    travIn_R(x->rc, visit);
}
//�������������������
template <typename T, typename VST>
static void goAlongLeftBranch(BinNodePosi(T) x, std::stack<BinNodePosi(T)>& s) {
    while (x) {//��ǰ�ڵ���ջ��Ȼ��������֧���룬ֱ��������
        s.push(x);
        x = x->lc;
    }
}
template <typename T, typename VST>
void travIn_I1(BinNodePosi(T) x, VST& visit) {
    std::stack<BinNodePosi(T)> s;
    while (true) {
        goAlongLeftBranch(x, s);
        if (s.empty()) break;
        x = s.top();
        s.pop();
        visit(x->data);
        x = x->rc;
    }
}
template <typename T, typename VST>
void travIn_I2(BinNodePosi(T) x, VST& visit) {
    std::stack<BinNodePosi(T)> s;
    while (true) {
        if (x) {
            s.push(x);//���ڵ���ջ
            x = x->lc;//�������������
        } else if (!s.empty()){
            x = s.top();//������δ���ʵ�������Ƚڵ�
            s.pop();
            visit(x->data);
            x = x->rc;//�������ȵ�������
        } else {
            break;
        }
    }
}
template <typename T, typename VST>
void travIn_I3(BinNodePosi(T) x, VST& visit) {
    bool backtrack = false;//ǰһ���Ƿ�մ�����������
    while (true) {
        if (!backtrack && HasLChild(*x)) {
            x = x->lc;//�������������
        } else {//����������ոջ���
            visit(x->data);
            if (HasRChild(*x)) {//����������ǿգ������������������������رջ���
                x = x->rc;
                backtrack = false;
            } else {//���������գ������
                if (!(x = x->succ())) break;
                backtrack = true;
            }
        }
    }
}
template <typename T, typename VST>
void BinNode<T>::travLevel(VST& visit) {
    std::queue<BinNodePosi(T)> q;
    q.push(this);
    while (!q.empty()) {
        BinNodePosi(T) x = q.front();
        q.pop();
        visit(x->data);
        if (HasLChild(*x)) q.push(x->lc);
        if (HasRChild(*x)) q.push(x->rc);
    }
   
}

template <typename T, typename VST>
void BinNode<T>::travPre(VST& visit) {
    travPre_R(this, visit);
    //travPre_I2(this, visit);
}

template <typename T, typename VST>
void BinNode<T>::travIn(VST& visit) {
    travIn_R(this, visit);
    //travIn_I1(this, visit);
    //travIn_I2(this, visit);
    //travIn_I3(this, visit);
}

template <typename VST>
void BinNode<T>::travPost(VST& visit){
    travPost_R(this, visit);
    //travPost_I(this, visit);
}

#endif//BIN_NODE_H_