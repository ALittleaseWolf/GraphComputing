#pragma once
#include "utils.h"
#define __MAX_DIST__ (u32)(4294967296/4)

namespace graph{
// CSR
template <typename nodeData>
class Graph{
private:
    u32 numNodes;
    u64 numEdges;
    std::map<u32,u32> verticeMap; // 可能存在不与下标对应的图

    std::vector<u32> dstOffset;
    std::vector<u32> outDegree;
    std::vector<u32> dst;
    std::vector<nodeData> dstData;

    std::vector<u32> srcOffset;
    std::vector<u32> inDegree;
    std::vector<u32> src;
    std::vector<nodeData> srcData;
public:
    std::vector<u32> value;
    void resize_value(u32 initial);
    u32 get_numNodes();
    Graph();
    Graph(const std::string &edgelist);
    std::vector<Uint32Pair> read_edgelist(const std::string &edgelist); // vector做返回值，底层是数组，产生大量的复制操作
    u32 get_dstoffset(u32 &vertices);
    u32 get_srcoffset(u32 &vertices);
    std::vector<u32> get_outneighbor(u32 &vertices);
    std::vector<u32> get_inneighbor(u32 &vertices);
#ifdef CONVERT
    void convert_edgelist2adj(const std::string& infilename, const std::string& outfilename);
#endif
};

template <typename nodeData>
void Graph<nodeData>::resize_value(u32 initial)
{
    for(u32 i=0;i<numNodes;i++)
    {
        value[i] = initial;
    }
}

template <typename nodeData>
u32 Graph<nodeData>::get_numNodes()
{
    return numNodes;
}

template <typename nodeData>
Graph<nodeData>::Graph(){}

template <typename nodeData>
Graph<nodeData>::Graph(const std::string &edgelist)
{
    //* outdegree + csr *//
    // phase 1: read the file
    std::vector<Uint32Pair> fileData = read_edgelist(edgelist);
    value.assign(numNodes, __MAX_DIST__);
    utils::print_vector<u32>("value ", value);
    dstOffset.resize(numNodes);
    srcOffset.resize(numNodes);
    // phase 2: traversal the vector O(2n)
    /* reorder */
    std::set<u32> srcSet;
    for(u32 i=0; i< numEdges; i++)
    {
        srcSet.insert(fileData[i].first); // set会自动有序排列
        srcSet.insert(fileData[i].second);
    }
    u32 cnt=0;
    for(auto it=srcSet.begin(); it!=srcSet.end(); it++)
    {
        verticeMap.insert(std::make_pair(*it,cnt));
        cnt++;
    }
    std::vector<std::vector<u32> > oedges(numNodes);
    outDegree.resize(numNodes+1,0);
    for(auto d : fileData)
    {
        oedges[verticeMap[d.first]].emplace_back(verticeMap[d.second]);
        outDegree[verticeMap[d.first]] ++; //不用.at 每次访问都检查是否越界，没必要
    }
    utils::print_map<u32,u32>("reorder", verticeMap);
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

    //* indegree + csc *//
    std::vector<std::vector<u32> > iedges(numNodes);
    inDegree.resize(numNodes+1,0);
    for(auto d : fileData)
    {
        iedges[verticeMap[d.second]].emplace_back(verticeMap[d.first]);
        inDegree[verticeMap[d.second]] ++;
    }
    fileData.clear();
    fileData.shrink_to_fit();
    srcOffset[0] = 0;
    for(u32 i=1; i< numNodes; i++)
    {
        srcOffset[i] = srcOffset[i-1] + inDegree[i-1];
    }
    utils::print_vector<u32>("in_off", srcOffset);

    for(auto e : iedges)
    {
        src.insert(src.end(), e.begin(), e.end());
    }
    utils::print_vector<u32>("src   ", src);
    iedges.clear();
    iedges.shrink_to_fit();
}

template <typename nodeData>
std::vector<Uint32Pair> Graph<nodeData>::read_edgelist(const std::string &edgelist){
    std::ifstream file(edgelist.c_str(),std::ios::in);
    if(!file.is_open())
    {
        std::cout << "Error while reading file" << std::endl;
    }else
    {
        file >> numNodes >> numEdges;
        std::cout << "numNodes: " << numNodes << " numEdges: "<< numEdges << std::endl;
    }
    
    std::vector<Uint32Pair> fileData(numEdges);
    u64 edges=0;
    while(!file.eof())
    {
        u32 s,d;
        file >> s >> d;

        fileData[edges]= std::make_pair(s,d);

        edges ++;
    }
    file.close();

    assert(edges == numEdges);

    return fileData;
}

template <typename nodeData>
u32 Graph<nodeData>::get_dstoffset(u32 &vertices)
{
    return dstOffset[vertices];
}

template <typename nodeData>
u32 Graph<nodeData>::get_srcoffset(u32 &vertices)
{
    return srcOffset[vertices];
}

template <typename nodeData>
std::vector<u32> Graph<nodeData>::get_outneighbor(u32 &vertices)
{
    u32 offset = get_dstoffset(vertices);
    auto start = dst.begin()+offset;
    auto end = dst.begin()+offset+outDegree[vertices];
    std::vector<u32> neighbor(start, end);
    return neighbor;
}

template <typename nodeData>
std::vector<u32> Graph<nodeData>::get_inneighbor(u32 &vertices)
{
    u32 offset = get_srcoffset(vertices);
    auto start = src.begin()+offset;
    auto end = src.begin()+offset+inDegree[vertices];
    std::vector<u32> neighbor(start, end);
    return neighbor;
}

}