#ifndef BIN_NODE_H_
#define BIN_NODE_H_

#include <queue>
#include <stack>

#define BinNodePosi(T) BinNode<T>* /*节点位置*/
#define stature(p) ((p) ? (p)->height : -1)//节点高度
//BinNode状态与性质判断
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))
//与BinNode有特定关系的节点及指针
#define sibling(p) (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->lc)
#define uncle(x) (IsLChild(*((x)->parent)) ? (x)->parent->parent->rc : \
                  (x)->parent->parent->lc)
#define FromParentTo(x) \
    (IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lc : (x).parent->rc))


typedef enum { RB_RED, RB_BLACK } RBColor;//节点颜色

template <typename T>
struct BinNode {
    //成员
    T data;
    BinNodePosi(T) parent;
    BinNodePosi(T) lc;
    BinNodePosi(T) rc;
    int height;
    int npl;
    RBColor color;
    //构造函数
    BinNode() :parent(nullptr), rc(nullptr), lc(nullptr), height(0), npl(1),
        color(RB_RED){ }
    BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lcc = nullptr, 
        BinNodePosi(T) rcc = nullptr, int h = 0, int l = 1, RBColor c = RB_RED):
        data(e), parent(p), lc(lcc), rc(rcc), height(h), npl(l), color(c) { }
    //操作接口
    int size();//统计当前节点后代总数
    BinNodePosi(T) insertAsLC(const T& e);
    BinNodePosi(T) insertAsRC(const T& e);
    BinNodePosi(T) succ();//当前节点的直接后继
    template <typename VST>
    void travLevel(VST&);//子树层次遍历
    template <typename VST>
    void travPre(VST&);//子树先序遍历
    template <typename VST>
    void travIn(VST&);//子树中序遍历
    template <typename VST>
    void travPost(VST&);//子树后序遍历
    //比较器，判等器
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
//遍历
//二叉树先序遍历递归版
template <typename T, typename VST>
void travPre_R(BinNodePosi(T) x, VST& visit) {
    if (!x) return;
    visit(x->data);
    travPre_R(x->lc, visit);
    travPre_R(x->rc, visit);
}
//先序遍历迭代版
template <typename T, typename VST>
static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, 
                                std::stack<BinNodePosi(T)>& s) {
    while (x) {
        visit(x->data);
        s.push(x->rc);//右孩子入栈
        x = x->lc;//沿左侧分支深入一层
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
//二叉树后序遍历递归版
template <typename T, typename VST>
void travPost_R(BinNodePosi(T) x, VST& visit) {
    if (!x) return;
    travPost_R(x->lc, visit);
    travPost_R(x->rc, visit);
    visit(x->data);
}
//后序遍历迭代版
template <typename T>
static void gotoHLVFL(std::stack<BinNodePosi(T)>& s) {//在s的栈顶为根的子树中找到最高左侧可见叶节点
    while (BinNodePosi(T) x = s.top()) {//反复检查栈顶节点
        s.pop();
        if (HasLChild(*x)) {//尽可能向左
            if (HasRChild(*x)) s.push(x->rc);//若有右孩子优先入栈
            s.push(x->lc);//然后菜转至左孩子
        } else {//实不得已，才向右
            s.push(x->rc);
        }
    }
    s.pop();//弹出栈顶节点
}
template <typename T, typename VST>
void travPost_I(BinNodePosi(T) x, VST& visit) {
    std::stack<BinNodePosi(T)> s;
    if (x) s.push(x);
    while (!s.empty()) {
        if (s.top() != x->parent) {//若栈顶非当前节点之父（则必为其右兄），则需要
            gotoHLVFL(s);//在以其右兄为根的子树中，找到HLVFL
        }
        x = s.top();
        s.pop();
        visit(x->data);
    }
}
//二叉树中序遍历递归版
template <typename T, typename VST>
void travIn_R(BinNodePosi(T) x, VST& visit) {
    if (!x) return;
    travIn_R(x->lc, visit);
    visit(x->data);
    travIn_R(x->rc, visit);
}
//二叉树中序遍历迭代版
template <typename T, typename VST>
static void goAlongLeftBranch(BinNodePosi(T) x, std::stack<BinNodePosi(T)>& s) {
    while (x) {//当前节点入栈，然后向左侧分支深入，直到无左孩子
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
            s.push(x);//根节点入栈
            x = x->lc;//深入遍历左子树
        } else if (!s.empty()){
            x = s.top();//弹出尚未访问的最低祖先节点
            s.pop();
            visit(x->data);
            x = x->rc;//遍历祖先的右子树
        } else {
            break;
        }
    }
}
template <typename T, typename VST>
void travIn_I3(BinNodePosi(T) x, VST& visit) {
    bool backtrack = false;//前一步是否刚从右子树回溯
    while (true) {
        if (!backtrack && HasLChild(*x)) {
            x = x->lc;//深入遍历左子树
        } else {//无左子树或刚刚回溯
            visit(x->data);
            if (HasRChild(*x)) {//如果右子树非空，则深入右子树继续遍历并关闭回溯
                x = x->rc;
                backtrack = false;
            } else {//若右子树空，则回溯
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