
#ifndef _AI_H_
#define _AI_H_

#include <cstdlib>
#include <cstring>
#include <vector>
#include <iostream>
#include <list>
#include "Node.h"
#include "NodePool.h"
#include "Timer.h"
#include "Point.h"
#include "utils.h"
using std::vector;
using std::list;

class AI {
	int* board;
	int* init_board;
	int* simulationTop;
	int* setX;
	int* setY;
	int m, n;
	double ucb_multier;
	Timer timer;
	//list<Node*> overflowNodes;
	static NodePool nodepool;
	static Node* root;
	static int mylasty;

	inline int nextPlayer(int curPlayer) {
		return (COMPUTER + COMPETITOR - curPlayer);
	}
	inline bool isValid(const int x, const int y) const {
		return (x >= 0 && x < m && y >= 0 && y < n);
	}
	inline void reset() {
		memcpy(board, init_board, m * n * sizeof(int));
	}
	inline Node* newNode(){
		return nodepool.popNode();
	}
	Node* addNode(int player);
	bool judgeWin(const int x, const int y) const ;
	bool judgeOtherWin(const int x, const int y) const ;
	bool judgeConnect(const int x, const int y) const; 
	bool checkCurPlayerWin(int* setX, int* setY, const int& choice, const int& player);
	void checkNextPlayerWin(int* setX, int* setY,
		const int& choice, const int& player, int* x, int* y);
	Node* selection(Node* root, vector<Node*>& path);
	int simulation(int player);
	void backPropagation(vector<Node*>& path, int result);
	void initSimulationTop();
	void resetAll(){
		if(root)freeNode(root);
		root = nullptr;
		mylasty = -1;
	}
	void changeRoot(int lasty){
		for(int i = 0; i < n; i++){
			if(root->childlist[i] && mylasty != i)freeNode(root->childlist[i]);
		}
		if(root->childlist[mylasty])root = root->childlist[mylasty];
		else {
			root = nullptr;
			return;
		}
		for(int i = 0; i < n; i++){
			if(root->childlist[i] && lasty != i)freeNode(root->childlist[i]);
		}
		if(root->childlist[lasty])root = root->childlist[lasty];
		else root = nullptr;
	}
	void freeNode(Node* anode){
		for(int i = 0; i < n; i++){
			if(anode->childlist[i])freeNode(anode->childlist[i]);
		}
		nodepool.pushNode(anode);
	}

public:
	AI(const int _m, const int _n, const int* top, int** _board,
		const int lastX, const int lastY, const int noX, const int noY, const int count);
	virtual ~AI() {
		delete[] board;
		delete[] init_board;
		delete[] simulationTop;
		delete[] setX;
		delete[] setY;
	}
	Point* MCST();
};

#endif