#include<bits/stdc++.h>
using namespace std;
struct Edge{
    int from,to,cap,flow,cost;
    Edge(int f,int t,int c,int fl,int co):from(f),to(t),cap(c),flow(fl),cost(co){}
};
const int INF=0x3fffffff,MAXV=2005;//无穷大,结点最大个数
vector<Edge>edges;
vector<vector<int>>graph(MAXV);
int v,e;
void insertEdge(int id1,int id2,int cap,int cost){//插入边
    graph[id1].push_back(edges.size());
    edges.push_back(Edge(id1,id2,cap,0,cost));
    graph[id2].push_back(edges.size());
    edges.push_back(Edge(id2,id1,0,0,-cost));
}
bool BellmanFord(int s,int t,int&flow,long long&cost){//最大流算法,s为源点,t为汇点
    int a[MAXV]={0},p[MAXV]={0},dis[MAXV]={0};//a数组表示源点到结点a[i]的残量,p数组表示最短路树上到达结点p[i]的边在edges数组中的序号
    fill(dis,dis+MAXV,INF);
    bool inQueue[MAXV]={false};
    dis[s]=0;
    inQueue[s]=true;
    a[s]=INF;//起点的残量置为无穷大
    queue<int>q;
    q.push(s);
    while(!q.empty()){//广度优先遍历查找从源点到达汇点的增广路
        int u=q.front();
        q.pop();
        inQueue[u]=false;
        for(int i:graph[u]){//遍历以x为起点的边
            Edge&e=edges[i];
            if(e.cap>e.flow&&dis[e.to]>dis[u]+e.cost){//当前边的终点的残量为0且容量大于流量
                dis[e.to]=dis[u]+e.cost;
                p[e.to]=i;//更新到达该终点的边的编号
                a[e.to]=min(a[u],e.cap-e.flow);//更新源点到该终点的残量
                if(!inQueue[e.to]){
                    inQueue[e.to]=true;
                    q.push(e.to);//压入队列
                }
            }
        }
    }
    if(dis[t]==INF)
        return false;
    flow+=a[t];
    cost+=dis[t]*1ll*a[t];
    for(int u=t;u!=s;u=edges[p[u]].from){//从汇点向前遍历增广路经，更新每条增广路的流量
        edges[p[u]].flow+=a[t];
        edges[p[u]^1].flow-=a[t];
    }
    return true;
}
//需要保证初始网络中没有负权环
pair<int,long long> MinCostMaxFlow(int s,int t){
    int flow=0;
    long long cost=0;
    while(BellmanFord(s,t,flow,cost));
    return {flow,cost};
}
int main(){
    while(~scanf("%d%d",&v,&e)&&v!=0){
        edges.clear();
        fill(graph.begin(),graph.end(),vector<int>());
        insertEdge(1,1+v,2,0);
        for(int i=2;i<v;++i)
            insertEdge(i,i+v,1,0);
        insertEdge(v,v+v,2,0);
        for(int i=0;i<e;++i){
            int a,b,c;
            scanf("%d%d%d",&a,&b,&c);
            insertEdge(a+v,b,1,c);
        }
        auto i=MinCostMaxFlow(1,v+v);
        printf("%lld\n",i.second);
    }
    return 0;
}
