#include "utils.h"
#include "graph.h"

#include <sstream>
#ifdef CONVERT
namespace graph{

template <typename nodeData>
void Graph<nodeData>::convert_edgelist2adj(const std::string& infilename, const std::string& outfilename)
{
    std::vector<Uint32Pair> fileData = read_edgelist(infilename);
    value.assign(numNodes, __MAX_DIST__);
    utils::print_vector<u32>("value ", value);
    dstOffset.resize(numNodes);
    // phase 2: traversal the vector O(2n)
    /* reorder */
    // std::set<u32> srcSet;
    // for(u32 i=0; i< numEdges; i++)
    // {
    //     srcSet.insert(fileData[i].first); // set会自动有序排列
    //     srcSet.insert(fileData[i].second);
    // }
    // u32 cnt=0;
    // for(auto it=srcSet.begin(); it!=srcSet.end(); it++)
    // {
    //     verticeMap.insert(std::make_pair(*it,cnt));
    //     cnt++;
    // }
    std::vector<std::vector<u32> > oedges(numNodes);
    outDegree.resize(numNodes+1,0);
    for(auto d : fileData)
    {
        oedges[d.first].emplace_back(d.second);
        outDegree[d.first] ++; //不用.at 每次访问都检查是否越界，没必要
    }
    // phase 3: 计算offset
    dstOffset[0] = 0;
    for(u32 i=1; i< numNodes; i++)
    {
        dstOffset[i] = dstOffset[i-1] + outDegree[i-1];
    }
    utils::print_vector<u32>("out_off", dstOffset);

    for(auto e : oedges)
    {
        dst.insert(dst.end(), e.begin(), e.end());
    }
    utils::print_vector<u32>("dst   ", dst);
    oedges.clear();
    oedges.shrink_to_fit();

    std::ofstream outfile(outfilename.c_str());
    outfile << "AdjacencyGraph" << std::endl;
    outfile << numNodes << std::endl;
    outfile << numEdges << std::endl;
    for(auto a : dstOffset)
    {
        outfile << a << std::endl;
    }
    for(auto d : dst)
    {
        outfile << d << std::endl;
    }
    outfile.close();
}

}

int main()
{
    graph::Graph<u32> csrGraph;
    csrGraph.convert_edgelist2adj("../example.edgelist", "../outfile");
    return 0;
}

#endif