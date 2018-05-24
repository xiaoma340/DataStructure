#ifndef GRAPH_H_
#define GRAPH_H_

#include <climits>
#include <stack>

typedef enum {UNDISCOVERED, DISCOVERED, VISITED} VStatus;//����״̬
typedef enum {UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD} EType;//���ڱ������е�����

template <typename Tv, typename Te>//�������ͣ�������
class Graph {
public:
    //����
    int n;//��������
    virtual int insert(const Tv&) = 0;//���붥�㣬���ر��
    virtual Tv remove(int) = 0;//ɾ�����㼰������ߣ����ظö�����Ϣ
    virtual Tv& vertex(int) = 0;//����v������
    virtual int inDegree(int) = 0;//����v�����
    virtual int outDegree(int) = 0;//����v�ĳ���
    virtual int firstNbr(int) = 0;//����v���׸��ڽӶ���
    virtual int nextNbr(int, int) = 0;//����v����ڶ���j����һ���ڽӶ���
    virtual VStatus& status(int) = 0;//����v��״̬
    virtual int& dTime(int) = 0;//v��ʱ���ǩ������ʱ��
    virtual int& fTime(int) = 0;//v��ʱ���ǩ�����ʱ��
    virtual int& parent(int) = 0;//v�ڱ������еĸ���
    virtual int& priority(int) = 0;//v�ڱ������е����ȼ���
    //��
    int e;//�ߵ�����
    virtual bool exists(int, int) = 0;//��(v, u)�Ƿ����
    virtual void insert(const Te&, int, int, int) = 0;//�ڶ���v��u֮�����Ȩ��Ϊw�ı�e
    virtual Te remove(int, int) = 0;//ɾ��v��u֮��ı�e�����ظ���Ϣ
    virtual EType& type(int, int) = 0;//��(v, u)������
    virtual Te& edge(int, int) = 0;//��(v, u)������
    virtual int& weight(int, int) = 0;//��(v, u)��Ȩ��
    //�㷨
    void bfs(int);
    void dfs(int);
    void bcc(int);
    std::stack<Tv>* tsort(int);
    void prim(int);//��С֧����prim�㷨
    void dijkstra(int);//���·���㷨
    template <typename PU>
    void pfs(int, PU);//���ȼ��������
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
    void BFS(int, int&);//�����������
    void DFS(int, int&);//�����������
    void BCC(int, int&, std::stack<int>&);//����DFS��˫��ͨ�����ֽ�
    bool TSort(int, int&, std::stack<Tv>*);//����DFS�����������㷨
    template <typename PU>
    void PFS(int, PU);//���ȼ��������
};

#endif//GRAPH_H_