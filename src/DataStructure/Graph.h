#ifndef GRAPH_H_
#define GRAPH_H_

#include <climits>
#include <stack>

typedef enum {UNDISCOVERED, DISCOVERED, VISITED} VStatus;//顶点状态
typedef enum {UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD} EType;//边在遍历树中的类型

template <typename Tv, typename Te>//顶点类型，边类型
class Graph {
public:
    //顶点
    int n;//顶点总数
    virtual int insert(const Tv&) = 0;//插入顶点，返回编号
    virtual Tv remove(int) = 0;//删除顶点及其关联边，返回该顶点信息
    virtual Tv& vertex(int) = 0;//顶点v的数据
    virtual int inDegree(int) = 0;//顶点v的入度
    virtual int outDegree(int) = 0;//顶点v的出度
    virtual int firstNbr(int) = 0;//顶点v的首个邻接顶点
    virtual int nextNbr(int, int) = 0;//顶点v相对于顶点j的下一个邻接顶点
    virtual VStatus& status(int) = 0;//顶点v的状态
    virtual int& dTime(int) = 0;//v的时间标签：发现时间
    virtual int& fTime(int) = 0;//v的时间标签：完成时间
    virtual int& parent(int) = 0;//v在遍历树中的父亲
    virtual int& priority(int) = 0;//v在遍历树中的优先级数
    //边
    int e;//边的总数
    virtual bool exists(int, int) = 0;//边(v, u)是否存在
    virtual void insert(const Te&, int, int, int) = 0;//在顶点v和u之间插入权重为w的边e
    virtual Te remove(int, int) = 0;//删除v和u之间的边e，返回该信息
    virtual EType& type(int, int) = 0;//边(v, u)的类型
    virtual Te& edge(int, int) = 0;//边(v, u)的数据
    virtual int& weight(int, int) = 0;//边(v, u)的权重
    //算法
    void bfs(int);
    void dfs(int);
    void bcc(int);
    std::stack<Tv>* tsort(int);
    void prim(int);//最小支撑树prim算法
    void dijkstra(int);//最短路径算法
    template <typename PU>
    void pfs(int, PU);//优先级搜索框架
private:
    void reset() {
        for (int i = 0; i < n; ++i) {
            status(i) = UNDISCOVERED;
            dTime(i) = fTime(i) = -1;
            parent(i) = -1;
            priority(i) = INT_MAX;
            for (int j = 0; j < n; ++j) {
                if (exists(i, j)) type(i, j) = UNDETERMINED;
            }
        }
    }
    void BFS(int, int&);//广度优先搜索
    void DFS(int, int&);//深度优先搜索
    void BCC(int, int&, std::stack<int>&);//基于DFS的双连通分量分解
    bool TSort(int, int&, std::stack<Tv>*);//基于DFS的拓扑排序算法
    template <typename PU>
    void PFS(int, PU);//优先级搜索框架
};

#endif//GRAPH_H_