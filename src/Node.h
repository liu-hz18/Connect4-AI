
#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "Point.h"
#include "utils.h"
using std::vector;

constexpr int NONE = 0;
constexpr int COMPETITOR = 1;
constexpr int COMPUTER = 2;
constexpr int BARRIER = 3;

class AI;

class Node {
	friend class AI;
	int winner, player;
	int totaltime, wintime;
	double ucb_a, ucb_b;
	Node** childlist;
	int* top;

public:
	Node(): winner(0), player(0), totaltime(0), wintime(0), ucb_a(0.0), ucb_b(0.0) {
		top = new int[13];
		childlist = new Node*[13];
	}
	~Node(){
		if(top) delete[] top;
		if(childlist) delete[] childlist;
	}
	void reset(){
		winner = player = totaltime = wintime = 0;
		ucb_a = ucb_b = 0.0;
	}
	void init(int p, const int n) {
		player = p;
		for(size_t i = 0; i < 13; i++){
			childlist[i] = nullptr;
		}
	}
	void update() {
		ucb_a = double(wintime) / double(totaltime);
		//ucb_b = 1 / sqrtf(totaltime * 1.0f);
		ucb_b = fastInvSqrt(totaltime*1.0f);
		if (player == COMPUTER) {
			ucb_a = 1.00 - ucb_a;
		}
	}
	Point* giveSelection(const double multier, const int n, double& max) const {
		Point* p = new Point(0, 0);
		//double max = -1.0;
		double temp = 0.0, max_temp = -1.0;
		for(int i = 0; i < n; i++){
			if(childlist[i] != nullptr && max_temp < (temp = (childlist[i]->ucb_a * (1.05 - 0.05 * childlist[i]->minChild(n))))){
				max = childlist[i]->ucb_a;
				max_temp = temp;
				p->x = top[i];
				p->y = i;
			}
		}
		return p;
	}
	void print(int n) const {
		for(int i = 0; i < n; i++){
			if(childlist[i]){
				fprintf(stderr, "child: %d, ucb_a: %.3f, ucb_b:%.3f, x = %d, y = %d, wintime:%d, total:%d\n", i, childlist[i]->ucb_a, childlist[i]->ucb_b, top[i], i, childlist[i]->wintime, childlist[i]->totaltime);
			}
		}
	}
	double calculate(const double multier)const {
		return (ucb_a + ucb_b * multier);
	}
	Node* findWinChild(const int n) {
		for(int i = 0; i < n; i++){
			if(childlist[i] != nullptr && childlist[i]->winner == player){
				this->winner = player;
				return childlist[i];
			}
		}
		return nullptr;
	}
	int expand(int n) const {
		vector<int> candidate;
		for (int i = 0; i < n; i++) {
			if (top[i] >= 0 && childlist[i] == nullptr) {
				candidate.push_back(i);
			}
		}
		if (!candidate.empty()) {
			return candidate[rand() % candidate.size()];
		}
		return -1;
	}
	int findRoad(const double multier, Node*& next, const int n) const {
		double max = -1.0;
		int road_rank = -1;
		for(int i = 0; i < n; i++){
			if(childlist[i] && max < childlist[i]->calculate(multier)){
				max = childlist[i]->calculate(multier);
				next = childlist[i];
				road_rank = i;
			}
		}
		return road_rank;
	}
	double minChild(int n) const {
		double max = -1.00;
		for(int i = 0; i < n; i++){
			if(childlist[i] != nullptr && max < childlist[i]->ucb_a){
				max = childlist[i]->ucb_a;
			}
		}
		return max;
	}
};

#endif