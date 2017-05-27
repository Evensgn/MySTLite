#include <iostream>
#include <map>
#include <ctime>
#include <queue>
#include <cmath>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "deque.hpp"

using namespace std;

bool check1(){ // test push_back push_front pop_back pop_front
	sjtu::deque<int> Q;
	std::deque<int> stdQ;	
	for(int i = 1; i <= 50000; i++){
		int x = rand() % 2, t = rand();
		if(!x) {
			Q.push_back(t); stdQ.push_back(t);
		}
		else {
			Q.push_front(t); stdQ.push_front(t);
		}
		if(Q.front() != stdQ.front()) return 0;
		if(Q.back() != stdQ.back()) return 0;		
	}		
	for(int i = 0; i < 50000; i++) if(Q[i] != stdQ[i]) return 0;
	for(int i = 1; i <= 10000; i++){
		int x = rand() % 2;
		if(!x) {
			x = rand() % 2;
			if(!x) {
				Q.pop_back(); stdQ.pop_back();
			}
			else {
				Q.pop_front(); stdQ.pop_front();
			}
		}
		else {
			x = rand() % 2; int t = rand();
			if(!x){
				Q.push_back(t); stdQ.push_back(t);
			}
			else{
				Q.push_front(t); stdQ.push_front(t);
			}
		}
		if(Q.front() != stdQ.front()) return 0;
		if(Q.back() != stdQ.back()) return 0;		
	}	
	sjtu::deque<int>::iterator it;
	std::deque<int>::iterator stdit;	
	stdit = stdQ.begin();
	for(it = Q.begin(); it != Q.end(); it++){
		if(*it != *stdit) return 0;
		stdit++;
	}
	it = Q.end() - 1;
	for(stdit = stdQ.end() - 1; stdit != stdQ.begin(); stdit--){
		if(*stdit != *it) return 0;
		it--;
	}
	return 1;
}

bool check2(){ // test insert erase
	sjtu::deque<int> Q;
	std::deque<int> stdQ;	
	for(int i = 1; i <= 42000; i++){
		int x = rand() % 2, t = rand();
		if(!x) {
			Q.push_back(t); stdQ.push_back(t);
		}
		else {
			Q.push_front(t); stdQ.push_front(t);
		}		
	}		
	int n = 42000;
	sjtu::deque<int>::iterator it;
	std::deque<int>::iterator stdit;				
	for(int i = 1; i <= 6666; i++){
		it = Q.begin(); stdit = stdQ.begin();
		int x = rand() % n;
		it += x; stdit += x;
		it = Q.erase(it); stdit = stdQ.erase(stdit);
		n--;
		if(it == Q.end()){
			if(stdit != stdQ.end()) return 0;
		}
		else if(*it != *stdit) return 0;
	}
	for(int i = 1; i <= 6666; i++){
		it = Q.begin(); stdit = stdQ.begin();
		int x = rand() % n;
		it += x; stdit += x;
		it = Q.insert(it, x); stdit = stdQ.insert(stdit, x);
		n++;
		if(*it != *stdit) return 0;
	}
	stdit = stdQ.begin();
	for(it = Q.begin(); it != Q.end(); ++it){
		if(*it != *stdit) return 0;
		stdit++;
	}
	stdit = stdQ.end() - 1;
	for(it = Q.end() - 1; it != Q.begin(); --it){
		if(*stdit != *it) return 0;
		stdit--;
	}
	return 1;
}

bool check3(){ // copy && "=" && clear()
	sjtu::deque<int> Q;
	std::deque<int> stdQ;	
	sjtu::deque<int>::iterator it;
	std::deque<int>::iterator stdit;
	for(int i = 1; i <= 50000; i++){
		int x = rand() % 2, t = rand();
		if(!x) {
			Q.push_back(t); stdQ.push_back(t);
		}
		else {
			Q.push_front(t); stdQ.push_front(t);
		}		
	}		
	sjtu::deque<int> Q1(Q);
	for(int i = 49999; i >= 0; i--) if(Q1[i] != stdQ[i]) return 0; 
	it = Q1.end() - 1;
	for(stdit = stdQ.end() - 1; stdit != stdQ.begin(); stdit--){
		if(*stdit != *it) return 0;
		it--;
	}
	while(!Q.empty()) Q.pop_front();
	stdit = stdQ.begin();
	for(it = Q1.begin(); it != Q1.end(); it++){
		if(*it != *stdit) return 0;
		stdit++;
	}
	sjtu::deque<int> P;
	P = Q1; P = Q1;
	for(int i = 49999; i >= 0; i--) if(P[i] != stdQ[i]) return 0; 
	
	it = P.end() - 1;
	for(stdit = stdQ.end() - 1; stdit != stdQ.begin(); stdit--){
		if(*stdit != *it) return 0;
		it--;
	}
	Q1.clear();
	if(Q1.size() || !Q1.empty()) return 0;
	it = Q1.begin();
	if(it != Q1.end()) return 0;
	stdit = stdQ.begin();
	for(it = P.begin(); it != P.end(); it++){
		if(*it != *stdit) return 0;
		stdit++;
	}
	return 1;
}

bool check4(){ // test const_iterator at()
	sjtu::deque<int> Q;
	std::deque<int> stdQ;	
	for(int i = 1; i <= 50000; i++){
		int x = rand() % 2, t = rand();
		if(!x) {
			Q.push_back(t); stdQ.push_back(t);
		}
		else {
			Q.push_front(t); stdQ.push_front(t);
		}		
	}	
	sjtu::deque<int>::const_iterator it;
	std::deque<int>::iterator stdit;
	stdit = stdQ.begin();
	for(it = Q.cbegin(); it != Q.cend(); it++){
		if(*it != *stdit) return 0;
		stdit++;
	}
	it = Q.cend() - 1;
	for(stdit = stdQ.end() - 1; stdit != stdQ.begin(); stdit--){
		if(*stdit != *it) return 0;
		it--;
	}
	for(int i = 0; i < 50000; i++) if(stdQ[i] != Q.at(i)) return 0;
	return 1;
}

struct node{
	int num;
	node(int Num) : num(Num) {}
};

bool check5(){ // test class writen by users
	sjtu::deque<node> Q;
	std::deque<int> stdQ;
	for(int i = 1; i <= 50000; i++){
		int x = rand() % 2, t = rand();
		if(!x) {
			Q.push_back(node(t)); stdQ.push_back(t);
		}
		else {
			Q.push_front(node(t)); stdQ.push_front(t);
		}		
	}
	for(int i = 0; i < 50000; i++) if(Q[i].num != stdQ[i]) return 0;
	sjtu::deque<node>::iterator it;
	std::deque<int>::iterator stdit;
	stdit = stdQ.begin();
	for(it = Q.begin(); it != Q.end(); it++){
		if(it -> num != *stdit) return 0;
		stdit++;
	}
	it = Q.end() - 1;
	for(stdit = stdQ.end() - 1; stdit != stdQ.begin(); stdit--){
		if(*stdit != it -> num) return 0;
		it--;
	}
	return 1;	
}

bool check6(){ // test throw
	int res = 0;
	sjtu::deque<int> Q;
	for(int i = 1; i <= 50; i++){
		int x = rand() % 2, t = rand();
		if(!x) Q.push_back(t); 
		else Q.push_front(t); 		
	}
	sjtu::deque<int> P(Q);
	sjtu::deque<int>::iterator it, pit;
	it = Q.begin() + 5;
	pit = Q.begin() + 30;
	if(it - pit != -25 || pit - it != 25) return 0;
	pit = P.begin() + 30;
	
	try{ int x = it - pit; } catch(...) {res++;}
	try{ Q.erase(pit); } catch(...) {res++;}
	try{ Q[-1]; } catch(...) {res++;}
	try{ Q[60]; } catch(...) {res++;}
	try{ Q.at(-3); } catch(...) {res++;}
	try{ Q.at(66); } catch(...) {res++;}
	try{ Q.erase(Q.end()); } catch(...) {res++;}
	Q.clear();
	
	try{ Q.pop_front(); } catch(...) {res++;}
	try{ Q.pop_back(); } catch(...) {res++;}
	try{ Q.front(); } catch(...) {res++;}
	try{ Q.back(); } catch(...) {res++;}
	
	return res == 11;
}

void Speed_test(){
	sjtu::deque<int> Q;
	for(int i = 1; i <= 800000; i++){
		int x = rand() % 2, t = rand();
		if(!x) Q.push_back(t); 
		else Q.push_front(t); 		
	}		
	int n = 800000;
	sjtu::deque<int>::iterator it;			
	for(int i = 1; i <= 400000; i++){
		it = Q.begin(); 
		int x = rand() % n;
		it += x; 
		it = Q.erase(it); 
		n--;
	}
	for(int i = 1; i <= 400000; i++){
		it = Q.begin(); 
		int x = rand() % n;
		it += x; 
		it = Q.insert(it, x); 
		n++;
	}
	it = Q.begin();
	for(int i = 1; i <= 2000000; i++) *it;
	Q.clear();
	for(int i = 1; i <= 800000; i++){
		int x = rand() % 2, t = rand();
		if(!x) Q.push_back(t); 
		else Q.push_front(t); 		
	}		
	n = 800000;
	for(int i = 1; i <= 64000; i++){
		Q[1]; Q[n - 1];
	}
}

int main(){
	srand(time(NULL));
	//int t1 = clock();
	if(check1()) puts("Test 1 Passed!!!!!!"); else puts("Test 1 Failed............");
	if(check2()) puts("Test 2 Passed!!!!!!"); else puts("Test 2 Failed............");
	if(check3()) puts("Test 3 Passed!!!!!!"); else puts("Test 3 Failed............");
	if(check4()) puts("Test 4 Passed!!!!!!"); else puts("Test 4 Failed............");
	if(check5()) puts("Test 5 Passed!!!!!!"); else puts("Test 5 Failed............");
	if(check6()) puts("Test 6 Passed!!!!!!"); else puts("Test 6 Failed............");
	Speed_test();
	//cout << clock() - t1 << endl;
	return 0;
}

