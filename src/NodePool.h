#pragma once

#include <list>
#include <cstring>
#include "Node.h"
using std::list;

constexpr static size_t INIT_NUM = 100000;

class NodePool{
    list<Node*> unusedNodes;//空闲节点
    void expand(){
        for(size_t i = 0; i < 1000; i++){
            unusedNodes.push_back(new Node());
        }
    }

public:
    NodePool(){
        for(size_t i = 0; i < INIT_NUM; i++){
            unusedNodes.push_back(new Node());
        }
    }
    ~NodePool(){
        //fprintf(stderr, "free nodepool\n");
        for(Node* node_ptr: unusedNodes){
            if(node_ptr){ delete node_ptr; node_ptr = nullptr; }
        }
    }
    size_t size() const {
        return unusedNodes.size();
    }
    Node* popNode(){
        if(unusedNodes.size() < 2){
            expand();
        }
        Node* anode = unusedNodes.front();
        unusedNodes.pop_front();
        return anode;
    }
    void pushNode(Node* anode){
        anode->reset();
        unusedNodes.push_back(anode);
    }
};
