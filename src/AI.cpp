#include "AI.h"
#include <math.h>
#include <stdio.h>

constexpr int direcX[4] = { 1, 1, 1, 0 };
constexpr int direcY[4] = { -1, 1, 0, 1 };

constexpr int MAX_TIME = 3000000;//最大模拟次数
constexpr double constant = 1.00;//常数因子
constexpr double timeLimit = 2.80;//时限(ms)

static int M, N;//棋盘大小
NodePool AI::nodepool = NodePool();
Node* AI::root = nullptr;
int AI::mylasty = -1;

AI::AI(const int _m, const int _n, const int* top, int** _board,
	const int lastX, const int lastY, const int noX, const int noY, const int count)
	:m(_m), n(_n), ucb_multier(0.0) {
	M = m, N = n;
	timer.start();
	srand((unsigned)time(0));
	board = new int[m * n];
	init_board = new int[m * n];
	simulationTop = new int[n];
	setX = new int[n];
	setY = new int[n];
	int i, j, offset, temp;
	for (i = 0; i < m; i++) {
		offset = i * n;
		for (j = 0; j < n; j++) {
			temp = _board[i][j];
			init_board[offset + j] = temp;
			board[offset + j] = temp;
		}
	}
	board[noX * n + noY] = init_board[noX * n + noY] = BARRIER;
    if(count < 2)resetAll();
	else if(root && mylasty >= 0)changeRoot(lastY);
}


Node* AI::addNode(int player) {
	Node* anode = newNode();
	anode->init(player, n);
	for (int i = 0; i < n; i++) {
		int x = m - 1;
		while (x >= 0 && board[x * n + i] != NONE) x--;
		anode->top[i] = x;
	}
	return anode;
}

bool AI::judgeWin(const int x, const int y) const {
	if(!isValid(x, y))
		return false;
	int dx, dy, center = board[x * n + y], count, locx, locy;
	for (int i = 0; i < 4; i++) {
		dx = direcX[i]; dy = direcY[i]; count = 1;
		for (locx = x + dx, locy = y + dy; isValid(locx, locy); locx += dx, locy += dy) {
			if (center != board[locx * n + locy]) break;
			count++;
		}
		for (locx = x - dx, locy = y - dy; isValid(locx, locy); locx -= dx, locy -= dy) {
			if (center != board[locx * n + locy]) break;
			count++;
		}
		if (count >= 4)return true;
	}
	return false;
}

bool AI::judgeConnect(const int x, const int y) const {
	if(!isValid(x, y))
		return false;
	int dx, dy, center = board[x * n + y], count, locx, locy;
	bool flag = false;
	for (int i = 0; i < 4; i++) {
		dx = direcX[i]; dy = direcY[i]; count = 1; flag = false;
		for (locx = x + dx, locy = y + dy; isValid(locx, locy); locx += dx, locy += dy) {
			if (center != board[locx * n + locy]) {
				if(!flag)flag = true;
				else break;
			}
			count++;
		}
		for (locx = x - dx, locy = y - dy; isValid(locx, locy); locx -= dx, locy -= dy) {
			if (center != board[locx * n + locy]) {
				if(!flag)flag = true;
				else break;
			}
			count++;
		}
		if (count >= 4)return true;
	}
	return false;
}

bool AI::judgeOtherWin(const int x, const int y) const {
	if(!isValid(x, y))
		return false;
	int dx, dy, center = board[x * n + y], count, locx, locy, likelycount;
	for (int i = 0; i < 4; i++) {
		dx = direcX[i]; dy = direcY[i]; count = 1; likelycount = 1;
		for (locx = x + dx, locy = y + dy; isValid(locx, locy); locx += dx, locy += dy) {
			if(center == board[locx * n + locy]){ count++; likelycount++; continue; }
			if(locx != simulationTop[locy] || likelycount <= 2)break;
			//if (center != board[locx * n + locy]) break;
			likelycount++;
		}
		if(likelycount >= 4)return true;
		likelycount = 1;
		for (locx = x - dx, locy = y - dy; isValid(locx, locy); locx -= dx, locy -= dy) {
			if(center == board[locx * n + locy]){ count++; likelycount++; continue; }
			if(locx != simulationTop[locy] || likelycount <= 2)break;
			//if (center != board[locx * n + locy]) break;
			likelycount++;
		}
		if (count >= 4)return true;
		if(likelycount >= 4)return true;
	}
	return false;
}

bool AI::checkCurPlayerWin(int* setX, int* setY, const int& choice, const int& player) {
	for (int i = 0; i < choice; i++) {
		board[setX[i] * n + setY[i]] = player;
		if (judgeWin(setX[i], setY[i])/* || judgeConnect(setX[i], setY[i])*/) {
			return true;
		}
		board[setX[i] * n + setY[i]] = NONE;
	}
	return false;
}

void AI::checkNextPlayerWin(int* setX, int* setY, const int& choice, const int& player, int* x, int* y) {
	int nextplayer = nextPlayer(player);
	for (int i = 0; i < choice && (*x) == -1; i++) {
		board[setX[i] * n + setY[i]] = nextplayer;
		if (/*judgeOtherWin(setX[i], setY[i])*/ judgeWin(setX[i], setY[i])) {
			(*x) = setX[i];
			(*y) = setY[i];
		}
		board[setX[i] * n + setY[i]] = NONE;
	}
}


Node* AI::selection(Node* root, vector<Node*>& path) {
	Node* cur = root, *temp;
	//fprintf(stderr, "root winner: %d\n", root->winner);
	while (cur != nullptr && timer.duration() < timeLimit) {
		path.push_back(cur);//加入路径
		if (cur->winner > 0) {
			break;
		}
		if ((temp = cur->findWinChild(n)) != nullptr) {
			cur = temp;
			path.push_back(cur);
			break;
		}
		int expand_rank = cur->expand(n);//扩展
		if (expand_rank >= 0 && expand_rank < n) {//可以扩展
			int x = cur->top[expand_rank], y = expand_rank;
			board[x * n + y] = cur->player;//落子
			cur->childlist[expand_rank] = addNode(nextPlayer(cur->player));
			cur->childlist[expand_rank]->totaltime = 2;
			if (judgeWin(x, y)) {//选择过程中遇到胜利，立刻返回
				cur->childlist[expand_rank]->winner = cur->player;
				cur->childlist[expand_rank]->wintime = int(cur->player == COMPUTER)*2;
			}
			cur = cur->childlist[expand_rank];
			path.push_back(cur);
			break;
		} else { //不可被扩展, 找一条向下的道路
			Node* next = nullptr;
			int road_rank = cur->findRoad(this->ucb_multier, next, n);
			if (road_rank != -1 && next != nullptr) {
				int x = cur->top[road_rank];
				int y = road_rank;
				board[x * n + y] = cur->player;
				cur = next;
			} else {
				return nullptr;
			}
		}
	}
	return cur;
}

void AI::initSimulationTop() {
	for (int j = 0; j < n; j++) {
		int i = m - 1;
		while (i >= 0 && board[i * n + j] != NONE) i--;
		simulationTop[j] = i;
	}
}

int AI::simulation(int player) {
	int result = 1;
	initSimulationTop();
	int lasty = -1;
	while (timer.duration() < timeLimit) {
		int choice = 0;
		for (int i = 0; i < n; i++) {
			if (simulationTop[i] >= 0) {
				setX[choice] = simulationTop[i];
				setY[choice] = i;
				choice++;
			}
		}
		if (choice == 0) {
			return result;//tie
		}
		if (checkCurPlayerWin(setX, setY, choice, player)) {
			return int(player == COMPUTER)*2;
		}
		int x = -1, y = -1;
		//模拟过程中, 如果对方有必胜策略，我方就占据x, y的位置
		checkNextPlayerWin(setX, setY, choice, player, &x, &y);
		if (x == -1 && y == -1) {
			int r = rand() % choice;
			x = setX[r], y = setY[r];
			//if(rand() % 2 && lasty >= 0 && simulationTop[lasty] >= 0){
			//	x = setX[lasty], y = setY[lasty];
			//} else {
			//	
			//}
		}
		if(!isValid(x, y)){
			int r = rand() % choice;
			x = setX[r], y = setY[r];
			//fprintf(stderr, "error! x, y not set!\n");
			//return 0;
		}
		board[x * n + y] = player;
		lasty = y;
		player = nextPlayer(player);
		x = m - 1;
		while (x >= 0 && board[x * n + y] != NONE)x--;
		simulationTop[y] = x;
	}
	return result;
}

void AI::backPropagation(vector<Node*>& path, int result) {
	for(Node* node: path){
		if(node == nullptr)continue;
		node->wintime += result;
		node->totaltime += 2;
		node->update();
	}
}

Point* AI::MCST() {
	//首先检查自己或对方是否有必胜策略
	initSimulationTop();
	int choice = 0;
	for (int i = 0; i < n; i++) {//初始化set
		if (simulationTop[i] >= 0) {
			setX[choice] = simulationTop[i];
			setY[choice] = i;
			choice++;
		}
	}
	//检查本方是否必胜
	for (int i = 0; i < choice; i++) {
		board[setX[i] * n + setY[i]] = COMPUTER;
		if (judgeWin(setX[i], setY[i])) {
			fprintf(stderr, "mast win: %d, %d\n", setX[i], setY[i]);
			mylasty = setY[i];
			return new Point(setX[i], setY[i]);
		}
		board[setX[i] * n + setY[i]] = NONE;
	}
	//检查对方是否必胜
	int t, result = 0;
	for (int i = 0; i < choice; i++) {
		board[setX[i] * n + setY[i]] = COMPETITOR;
		if (judgeWin(setX[i], setY[i])){
			fprintf(stderr, "mast lose: %d, %d\n", setX[i], setY[i]);
			mylasty = setY[i];
			return new Point(setX[i], setY[i]);
		}
		board[setX[i] * n + setY[i]] = NONE;
	}
	//开始UCT
	if(root == nullptr)root = addNode(COMPUTER);
	for (t = 0; t < MAX_TIME && timer.duration() < timeLimit; t++) {
		this->ucb_multier = (root->totaltime <= 1) ? 0.0 : constant * sqrt(fastLog(root->totaltime*1.0f));
		vector<Node*> path;
		Node* pos = selection(root, path);
		if (pos != nullptr) {//可以扩展
			result = (pos->winner > 0) ? (int(pos->winner == COMPUTER)*2) : simulation(pos->player);
		} else {//无路可走
			result = 0;
		}
		backPropagation(path, result);
		reset();
	}
	double max = -1.0;
	Point* p = root->giveSelection(this->ucb_multier, n, max);
	mylasty = p->y;
	fprintf(stderr, "rate: %.3f, x: %d, y: %d, times:%d, time:%.2lf, poolLeft: %lld\n", max, p->x, p->y, t, timer.duration(), nodepool.size());
	return p;
}
