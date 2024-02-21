#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cstring>
#include <sstream>
using namespace std;



/*typedef struct{
	int node_num;
	int source;
}Node;
*/
vector<int> *adj;
vector<int> *prime_path;
int node_cnt;

int make_adjlist(string filename){
	ifstream fin(filename);//command argument: 입력파일 명
	if(!fin.is_open()) {
		cerr << "Can't file open" << endl;
		return 1;
	}
	//node 개수 파악
	node_cnt = 0;
	string buf;
	while(getline(fin,buf)){
		node_cnt++;
	}
	fin.clear();
	cout << "Node Count : " << node_cnt << endl;
	fin.seekg(0, fin.beg);

	adj = new vector<int>[node_cnt+1];
	int idx = 1;
	while(getline(fin, buf)){
		istringstream iss(buf);
		string token;
		getline(iss, token, ' '); //첫node 값은 날리기
		while(getline(iss, token, ' ')){
			adj[idx].push_back(stoi(token));
		}
		idx++;
	}
	for(int x=1; x<=node_cnt; x++){
		int y = 0;
		cout << x << ' ';
		if(adj[x].empty()){
		       cout << endl;
			continue;
		}
		for(vector<int>::iterator it = adj[x].begin(); it != adj[x].end(); ++it){
			cout << *it << ' ';
		}
		cout << endl;
		/*
		while( (adj[x][y].empyt()) ){
			cout << adj[x][y];
			cout << " ";
			y++;
		}
		cout << endl;
		*/
	}
	prime_path = new vector<int>[node_cnt+1]; // prime_path 틀 만들어 놓기
	fin.close();
}

vector<int> pathOrder; //TODO: pathOrder을 adjancency list로 바꿔서 push_back시키면 prime_path랑 비교하기 쉬워지겠지?
stack<int> deep;

void find_primePath(){
	ofstream resultfile("resultfile.txt", ios::out | ios::app);
	if(!resultfile.is_open()){
		cerr << "Can't open resultfile" << endl;
		exit(1);
	}
	for(int i = 1; i <= node_cnt; i++){
		deep.push(i); //시작node는 number순서대로 방문
		int node;
		if(adj[i].empty()) continue;//시작 node가 Jump할 수 있는 edge가 없을 때
		while(!deep.empty()){ 
			node = deep.top();
			if(!pathOrder.empty() && node == pathOrder.back()){//방문할 곳 다 방문한 parent node로 왔을 때
				deep.pop();
				pathOrder.pop_back();
				continue;
			}
			if (adj[node].empty()){//더 이상 연결된 edge가 없을 때
			//	if(pathOrder.size() == 1) break; 
				pathOrder.push_back(node);	
				for(vector<int>::iterator it = pathOrder.begin(); it != pathOrder.end(); ++it){
					int next = *(it+1);
					if( find(prime_path[*it].begin(), prime_path[*it].end(),next) == prime_path[*it].end()){
						for(vector<int>::iterator it = pathOrder.begin(); it != pathOrder.end(); ++it){
							int now = *(it);
							if( (it+1) == pathOrder.end()){
								resultfile << now;
								if(i == pathOrder.back()) continue;
								prime_path[*it].push_back(now);
								continue;
							}
							resultfile << now << "-";
							prime_path[*it].push_back(now);
							//prime_path[*it].push_back(next);
						}
						resultfile << endl;
						break;
					}	
				}
				pathOrder.pop_back();
				deep.pop();
			}else if((find(pathOrder.begin(), pathOrder.end(), node) != pathOrder.end())){//이미 방문했던 노드를 만났을 경우
				if(node == i) pathOrder.push_back(node); //현재 방문한 노드가 source일 경우
				for(vector<int>::iterator it = pathOrder.begin(); it != pathOrder.end(); ++it){
					int next = *(it+1);
					if( !(find(prime_path[*it].begin(), prime_path[*it].end(),next) != prime_path[*it].end())){
						for(vector<int>::iterator it = pathOrder.begin(); it != pathOrder.end(); ++it){
							int now = *(it);
							if(it == pathOrder.end()){
								resultfile << now;
							 	if(i == pathOrder.back()) continue;
								prime_path[*it].push_back(now);
								continue;
							}
							resultfile << now << "-";
							prime_path[*it].push_back(now);
							//prime_path[*it].push_back(next);
						}//prime_path에 없는 node를 발견했으면 pathOrder는 prime path니까 처음부터 노드추가
						resultfile << endl;
						break;
					}	
				}
				pathOrder.pop_back();
				deep.pop();
				
			}else{ 	
				for(vector<int>::iterator it = adj[node].begin(); it != adj[node].end(); ++it){
					if(node == *it){
						deep.push(*it);
				     		continue;	  
					}
					if( !(find(pathOrder.begin(),pathOrder.end(), *it) != pathOrder.end())){
						deep.push(*it);
					}
				}
				pathOrder.push_back(node);
			}
		}	

		pathOrder.erase(pathOrder.begin(), pathOrder.end());
	}
	resultfile.close();
}

void print_primePath(){
	for(int i= 1; i <= node_cnt; i++){
		
	}	
}

int main(int argc, char *args[])
{
	make_adjlist(args[1]);
	find_primePath();
	
	cout << "--------------Prime Path----------------" << endl;
	
	/*print_primePath();
	for(int x=1; x<=node_cnt; x++){
		int y = 0;
		cout << x << ' ';
		if(prime_path[x].empty()){
			continue;
		}
		for(vector<int>::iterator it = prime_path[x].begin(); it != prime_path[x].end(); ++it){
			cout << *it << ' ';
		}
		cout << endl;
	}*/

	delete[] prime_path;	
	delete[] adj;
	return 0;
}
