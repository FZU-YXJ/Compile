#include<bits/stdc++.h>

#ifndef yjc_parsing_def
#define yjc_parsing_def
#include "yjc_parsing.h"
#endif

using namespace std;

/*�����Ƶ�����*/
namespace yjc_translate {
	using namespace yjc_parsing;
    ofstream fout("yjc_quaternion.txt");

    /*������ T1��T2*/
    int tempVariable = 0;
    /*��ʼ��ַ100*/
    int address = 100;
    /*����*/
    /*�����ķ�������*/
    vector<vector<string>> symbolTable;

    pair<int, vector<string>> genarateQuaternion(string a, string b, string c, string d);
    string searchVariable(string name);

    variable_definition_function(Trans tran,int u){
        symbolTable.back().push_back(attribution[GraphOfAdjacencyList[u][1]].name);
	}
	assignment_id_function(Trans tran,int u){

	}

	/*
	* returnType��void
	* function��id��ֵ������Ԫʽ
	* params��Trans int
	*/
	primary_num_function(Trans tran,int u){
		/*����*/
        tran.valiableAddress = "T" + to_string(++tempVariable);
        tran.value = attribution[GraphOfAdjacencyList[u][0]].value;
        tran.code.push_back(genarateQuaternion("=", tran.value, "-", tran.valiableAddress));
	}
	/*
	* returnType��void
	* function��ʶ��id����Ԫʽ
	* params��Trans int
	*/
	primary_id_function(Trans tran,int u){

	}
	/*
	* returnType��void
	* function���˷�����Ԫʽ
	* params��Trans int
	*/
	term_mul_function(Trans tran,int u){
		Trans e1 = attribution[GraphOfAdjacencyList[u][0]];
        Trans e2 = attribution[GraphOfAdjacencyList[u][2]];
        tran.valiableAddress = "T" + to_string(++tempVariable);
        tran.code.push_back(genarateQuaternion("*", e1.valiableAddress, e2.valiableAddress, tran.valiableAddress));
	}
	/*
	* returnType��void
	* function���ӷ���Ԫʽ
	* params��Trans int
	*/
	expr_add_function(Trans tran,int u){
		Trans e1 = attribution[GraphOfAdjacencyList[u][0]];
        Trans e2 = attribution[GraphOfAdjacencyList[u][2]];
        tran.valiableAddress = "T" + to_string(++tempVariable);
        tran.code.push_back(genarateQuaternion("+", e1.valiableAddress, e2.valiableAddress, tran.valiableAddress));
	}
	/*
	* returnType��void
	* function���Ƚϱ��ʽ��Ԫʽ
	* params��Trans int
	*/
	rel_function(Trans tran,int u){
		Trans id1 = attribution[GraphOfAdjacencyList[u][0]];
        Trans id2 = attribution[GraphOfAdjacencyList[u][2]];
        tran.code.push_back(genarateQuaternion("j" + symbol[GraphOfAdjacencyList[u][1]], id1.valiableAddress, id2.valiableAddress, "0"));
        tran.True = tran.code.back().first;
        tran.code.back().second[3] = to_string(tran.code.back().first + 2);
        tran.code.push_back(genarateQuaternion("j", "-", "-", "0"));
        tran.False = tran.code.back().first;
	}
	/*
	* returnType��void
	* function��while������Ԫʽ
	* params��Trans int
	*/
	while_function(Trans tran,int u){
		tran = attribution[GraphOfAdjacencyList[u][0]];
        attribution[u].code.clear();
        for (auto it : attribution[GraphOfAdjacencyList[u][2]].code)
            tran.code.push_back(it);
        for (auto it : attribution[GraphOfAdjacencyList[u][4]].code)
            tran.code.push_back(it);
        tran.code.push_back(genarateQuaternion("j", "-", "-", to_string(tran.code.front().first)));

        for (auto &it : tran.code) {
            if (it.first == attribution[GraphOfAdjacencyList[u][2]].False) {
                it.second[3] = to_string(tran.code.back().first + 1);
                break;
            }
        }
        tran.code.back().second[3] = to_string(tran.code.front().first);
	}
	/*
	* returnType��void
	* function��if�����Ԫʽ
	* params��Trans int
	*/
	if_function(Trans tran,int u){
		tran = attribution[GraphOfAdjacencyList[u][0]];
        attribution[u].code.clear();
        for (auto it : attribution[GraphOfAdjacencyList[u][2]].code)
            tran.code.push_back(it);
        for (auto it : attribution[GraphOfAdjacencyList[u][4]].code)
            tran.code.push_back(it);
        for (auto &it : tran.code) {
            if (it.first == attribution[GraphOfAdjacencyList[u][2]].False) {
                it.second[3] = to_string(tran.code.back().first + 1);
                break;
            }
        }
	}
	/*
	* returnType��void
	* function��if else�����Ԫʽ
	* params��Trans int
	*/
	if_else_function(Trans tran,int u){
		tran = attribution[GraphOfAdjacencyList[u][0]];
        attribution[u].code.clear();
        for (auto it : attribution[GraphOfAdjacencyList[u][2]].code)
            tran.code.push_back(it);
        for (auto it : attribution[GraphOfAdjacencyList[u][4]].code)
            tran.code.push_back(it);
        tran.code.push_back(genarateQuaternion("j", "-", "-", to_string(tran.code.back().first + attribution[GraphOfAdjacencyList[u][6]].code.size() + 2)));
        tran.code.back().first = tran.code.front().first + tran.code.size() - 1;
        for (auto &it : tran.code) {
            if (it.first == attribution[GraphOfAdjacencyList[u][2]].False) {
                it.second[3] = to_string(tran.code.back().first + 1);
                break;
            }
        }
        for (auto it : attribution[GraphOfAdjacencyList[u][6]].code) {
            it.first++;
            if (it.second[0][0] == 'j') {
                int num = atoi(it.second[3].c_str());
                it.second[3] = to_string(num + 1);
            }
            tran.code.push_back(it);
            }
	}
    /*
	* returnType��void
	* function��������ȱ�����Ԫʽ����
	* params��int
	*/
    void dfs(int u) {
    	Trans &tran = attribution[u];
    	/*���Ϊ�գ�������ǰѭ�� */
        if (GraphOfAdjacencyList[u].empty()) {
            return;
        }

        /*{�������Ŀ�ʼ�����½����ű�����*/
        if (symbol[GraphOfAdjacencyList[u].front()] == "{")
        {
        	symbolTable.push_back(vector<string>());
		}

		/*����*/
        for(int i=0;i<GraphOfAdjacencyList[u].size();i++){
			dfs(GraphOfAdjacencyList[u][i]);
		}

		/*�����Ԫʽ����*/
		attribution[u].code.clear();
        tran = attribution[GraphOfAdjacencyList[u][0]];

		/*��Ӷ��ӽڵ����Ԫʽ*/
		for(int i=0;i<GraphOfAdjacencyList[u].size();i++){
			for(auto quaternion : attribution[GraphOfAdjacencyList[u][i]].code){
				tran.code.push_back(quaternion);
			}
		}

        if (symbol[u] == "variable_definition") {//��������
        	variable_definition_function(tran,u);
        }
        else if (symbol[u] == "assignment"
                 && symbol[GraphOfAdjacencyList[u][0]] == "ID") {//��ֵ���
        	string p = searchVariable(attribution[GraphOfAdjacencyList[u][0]].name);    //����ű�
	        if (p == "unknown") {
	            cerr << "����δ����" << endl;
	            return;
	        }
	        tran.valiableAddress = p;
	        tran.code.push_back(genarateQuaternion("=", attribution[GraphOfAdjacencyList[u][2]].valiableAddress, "-", tran.valiableAddress));

        }
        else if (symbol[u] == "primary"
                 && symbol[GraphOfAdjacencyList[u][0]] == "ID") {    //��ԼID
        	string p = searchVariable(attribution[GraphOfAdjacencyList[u][0]].name);
	        if (p == "unknown") {
	            cerr << "����δ����" << endl;
	            return;
	        }
	        tran.valiableAddress = p;
	        tran.name = attribution[GraphOfAdjacencyList[u][0]].name;

        }
        else if (symbol[u] == "primary"
                 && symbol[GraphOfAdjacencyList[u][0]] == "NUM") {//��ԼNUM
        	primary_num_function(tran,u);
        }

        else if (symbol[u] == "term"
                 && GraphOfAdjacencyList[u].size() > 1
                 && symbol[GraphOfAdjacencyList[u][1]] == "*") {//�˷����ʽ
            term_mul_function(tran,u);
        }

        else if (symbol[u] == "rel"
                 && GraphOfAdjacencyList[u].size() > 1) {//��ϵ���ʽ
            rel_function(tran,u);
        }
         else if (symbol[u] == "expr"
                   && GraphOfAdjacencyList[u].size() > 1
                  && symbol[GraphOfAdjacencyList[u][1]] == "+") {//�ӷ����ʽ
            expr_add_function(tran,u);
        }
         else if (symbol[GraphOfAdjacencyList[u][0]] == "IF"
                  && GraphOfAdjacencyList[u].size() == 5) {//IF���
            if_function(tran,u);
        }

        else if (symbol[GraphOfAdjacencyList[u][0]] == "IF"
                  && GraphOfAdjacencyList[u].size() == 7) {//IF ELSE���
            if_else_function(tran,u);
        }

        else if (symbol[GraphOfAdjacencyList[u][0]] == "WHILE") {//WHILE���
            while_function(tran,u);
        }

        if (symbol[GraphOfAdjacencyList[u].back()] == "}")//������һ������Ϊ}��ɾ�����ű�
        {
        	symbolTable.pop_back();
		}
    }

    /*��ѯ���ű��еı���ֵ*/
    string searchVariable(string name) {
        per(i, 0, symbolTable.size()) rep(j, 0, symbolTable[i].size())
            if (symbolTable[i][j] == name) return name;
        return "unknown";
    }

    /*������Ԫʽ*/
    pair<int, vector<string>> genarateQuaternion(string a, string b, string c, string d) {
        vector<string> vec{ a,b,c,d };
        pair<int, vector<string>> p = make_pair(address++, vec);
        return p;
    }

    void main() {
        int root = cnt - 1;
        dfs(root);
        for (pair<int, vector<string>> it : attribution[root].code){
		    fout << "��ַ" << it.first << "��Ӧ����Ԫʽ:" << " (" << it.second[0] << ", " << it.second[1]<< ", " << it.second[2] << ", " << it.second[3] << ")" << endl;
		}

    }
}
