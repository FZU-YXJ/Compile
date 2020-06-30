#include<bits/stdc++.h> 
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
#define rep(i,a,n) for(int i=a;i<n;i++)
#define per(i,a,n) for(int i=n-1;i>=a;i--)
#define all(x) (x).begin(),(x).end()
typedef pair<string, vector<string>> Production;

#ifndef project2
#define project2
struct Project {        //��Ŀ��
    string left;
    vector<string> right;
    set<string> expect;

    const bool operator<(const Project &p) const {
        if (left < p.left) return true;
        if (left > p.left) return false;
        if (right < p.right) return true;
        if (right > p.right) return false;
        if (expect < p.expect) return true;
        return false;
    }

    const bool operator==(const Project &p) const {
        if (left == p.left && right == p.right && expect == p.expect) return true;
        return false;
    }
};

/*������Ŀ����*/
namespace smw_project {
    /*�洢���еķ���*/
    set<string> all_symbols;    
	/*������еķ���*/
    vector<string> vec_symbols;
	/*���Ź�ϣ*/
    map<string, int> Hash;      
    /*�洢�ս��*/
    set<string> terminal_symbol;    
	/*�洢���ս��*/
    set<string> nonterminal_symbol;              
    /*��Ų���ʽ���ӡ�ǰ  */ 
    vector<Production> vec_production;    
	/*�洢��Ŀ�����ӡ��� */ 
    set<Production> set_production;

    void smw_project() {
	    string str1, str2;
        vector<string> vec;
        /*����Grammar.txt�ļ�*/ 
        ifstream fin("zzc_Language.txt");
        while (fin >> str1 >> str2) {
            nonterminal_symbol.insert(str1);
            string str3;
            getline(fin, str3);
            stringstream str_stream;
            str_stream.str(str3);
            vec.clear();
            while (str_stream >> str3) {
                vec.push_back(str3);
                int flag = 0;
                rep(i, 0, str3.size()) {
				    /*���ս������Сд��ĸ���»������*/ 
                    if (!(str3[i] >= 'a' && str3[i] <= 'z' || str3[i] == '_')) {
                        flag = 1;
                        break;
                    }
                }
				/*����Сд��ĸ���»�������ķ��ž�Ϊ�ս��*/ 
                if (flag) terminal_symbol.insert(str3);
                /*���ս��*/ 
                else nonterminal_symbol.insert(str3);    
            }
            vec_production.push_back(Production(str1, vec));
        }
        terminal_symbol.insert("#");
        /*ɾ��program*/
        nonterminal_symbol.erase("program'");        
        all_symbols.insert(all(terminal_symbol));
        all_symbols.insert(all(nonterminal_symbol));
        for (auto it : terminal_symbol) vec_symbols.push_back(it);
        /*ͨ��vec_symbols���lr1��ı�ͷ*/ 
        for (auto it : nonterminal_symbol) vec_symbols.push_back(it);
        rep(i, 0, vec_symbols.size()) Hash[vec_symbols[i]] = i;
        for (auto it : vec_production) {
            str1 = it.first, vec = it.second;
            rep(i, 0, vec.size() + 1) {
                vector<string> d = vec;
                d.insert(d.begin() + i, ".");
                set_production.insert(Production(str1, d));
            }
        }
    }
    

    void main() {
        smw_project();
    }
}
#endif
