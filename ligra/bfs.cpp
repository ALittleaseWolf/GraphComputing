#include "ligra.h"
#include "graph.h"

// 单机单线程 push
void bfs_push(graph::Graph<u32> &csrGraph, u32 s)
{
    csrGraph.value[s] = 0;
    // u32 numNodes = csrGraph.get_numNodes();
    std::queue<u32> activeVers;
    activeVers.push(s);
    while (!activeVers.empty())
    {
        u32 av = activeVers.front();
        // u32 offset = csrGraph.get_offset(av);
        for (auto n : csrGraph.get_outneighbor(av))
        {
            if(csrGraph.value[n] > csrGraph.value[av] + 1)
            {
                csrGraph.value[n] = csrGraph.value[av] + 1;
                activeVers.push(n);
            }
        }
        activeVers.pop();
    }
}

// 单机单线程 pull
void bfs_pull(graph::Graph<u32> &csrGraph, u32 s)
{
    csrGraph.value[s] = 0;
    u32 numNodes = csrGraph.get_numNodes();
    u32 iteration = 0;
    while(1)
    {
        char flag = 0;
        for(u32 v=0;v<numNodes;v++)
        {
            u32 min = __MAX_DIST__;
            for(auto n : csrGraph.get_inneighbor(v))
            {
                min = std::min(csrGraph.value[n], min);
            }
            if(csrGraph.value[v] > min + 1)
            {
                flag = 1;
                csrGraph.value[v] = min + 1;
            }
        }
        if(flag == 0 || iteration > 100)
        {
            break;
        }
        iteration ++;
    }
}

// 单机多线程
void BFS(graph::Graph<u32> &csrGraph, u32 s){
    csrGraph.value[s] = 0;
    
}


// test
#ifdef TEST
int main()
{
    graph::Graph<u32> csrGraph("../example.edgelist");
    // test neighbor函数
    std::cout << "out neigbor" << std::endl;
    for(u32 src=0;src<csrGraph.get_numNodes();src++)
        for(auto n : csrGraph.get_outneighbor(src))
        {
            std::cout<<n<<" ";
        }
    std::cout << std::endl;
    std::cout << "in neigbor" << std::endl;
    for(u32 src=0;src<csrGraph.get_numNodes();src++)
        for(auto n : csrGraph.get_inneighbor(src))
        {
            std::cout<<n<<" ";
        }
    std::cout << std::endl;

    // test bfs_push
    std::cout << "bfs_push" << std::endl;
    bfs_push(csrGraph, 0);
    for(u32 src=0;src<csrGraph.get_numNodes();src++)
    {
        std::cout << csrGraph.value[src] <<" ";
    }
    std::cout << std::endl;

    // // test bfs_pull
    std::cout << "bfs_pull" << std::endl;
    csrGraph.resize_value(__MAX_DIST__);
    bfs_pull(csrGraph, 0);
    for(u32 src=0;src<csrGraph.get_numNodes();src++)
    {
        std::cout << csrGraph.value[src] <<" ";
    }
    std::cout << std::endl;
    return 0;
}
#endif