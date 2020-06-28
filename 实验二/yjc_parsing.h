#include<bits/stdc++.h> 

#ifndef project
#define project
#include "smw_project.h"
#endif

using namespace std;

namespace yjc_parsing{ 
	using namespace smw_project;
    /*cnt��¼����*/
	int cnt = 0;
    vector<int> GraphOfAdjacencyList[1005];
    
	/*����LR(1)����������*/
	string lr1[1005][1005];
	/*��¼LR(1)����������������*/
	int m, n;	
	/*��ȡ�ʷ������Ľ���������﷨���������ݴ�����һ���ļ���*/
	ifstream fin("yjc_resultOfWordAnalysis.txt");
	ofstream fout("yjc_parsingProcess.txt");
	/*״̬ջ�Լ�״̬ջ��ʼ��,ջ��ָ��*/
	int stateStack[1000]; 	
	int top = 1; 
	struct Trans{
		vector<pair<int, vector<string>>> code;    //��Ԫʽ
		string valiableAddress;
		int True;
        int False;
	    string name;     //������name
	    string value;    //������value
	};
	
	/*�ڵ�ṹ����*/
	vector<Trans> attribution;
	vector<string> symbol;   	
	/*����������ͷ�ڵ�*/
    stack<int> S;
    queue<int> DyingAttr; 
   
	/*
	* returnType��void
	* function����ȡ�������е�����
	* params��null 
	*/
    void yjc_inputTableLR1() {
        ifstream fin("yxj_LR(1)Table.txt");
        fin >> n >> m;
        for(int i=0;i<n;i++){
        	for(int j=0;j<m;j++){
        		 fin >> lr1[i][j];
			}
		} 
    }   
	
	/*
	* returnType��void
	* function��ͨ���������������봮���﷨�������� 
	* params��null 
	*/
	void yjc_parsing(){
		/*�������*/
		int lineNum = 1; //��ǰ�� 
		stateStack[0] = 0; //״̬ջ��ʼֵ 
		/*�����ͷ*/
		fout << left << setw(5) << "";
	    fout << left << setw(80) << "״̬ջ";
	    fout << left << setw(150) << "����ջ";
	    fout << left << setw(80) << "���봮" << endl;
		/*�ʷ������Ľ����num������ֵ*/
	    pair<string, string> word;
	    /*��vector��ʷ������Ľ��*/
	    vector<pair<string, string>> words;
	    while (fin >> word.first) {
	        if (word.first == "NUM" || word.first == "ID")fin >> word.second;
	        words.push_back(word);
	    }
	    /*������־#*/
	    words.push_back(make_pair("#", ""));
	    
	    Trans trans;                                            //��ʼ״̬
	    vector<pair<string, Trans>> symbolStack{ make_pair("#",trans) };    //��ʼ״̬
	    
	    /*�﷨��������*/
	    while (1) {
	    	/*����к�*/    	
	        fout << left << setw(5) << lineNum++;
	        
	    	/*���״̬ջ������*/
	        string stateStackString;
	        for(int i=0;i<top;i++){
	        	stateStackString += to_string(stateStack[i]) + ' ';
			}
	        fout << left << setw(50) << stateStackString;
	        
	        /*�������ջ������*/
	        string symbolStackString;
	        for (pair<string, Trans> str : symbolStack){
	        	symbolStackString += str.first + ' ';
			} 
			fout << left << setw(150) << symbolStackString;
	        
	        /*������봮��ʣ������*/
	        string inputStackString;
	        for (pair<string, string> word : words) inputStackString += word.first + ' ';
	        fout << left << setw(80) << inputStackString << endl;
	        
	        
			cout<<"��ǰ״̬ջջ��Ԫ�أ�"<<stateStack[top - 1]<<endl;
			/*��LR��1����*/
	        string tableValue = lr1[stateStack[top - 1]][Hash[words[0].first]];
	        
	        cout<<"��ѯLR��1����Ķ�Ӧֵ��"<<tableValue<<endl; 
	        
	        /*������accҲ����err��˵����s�ƽ�����r��Լ*/
	        if (tableValue[0] == 's') { 
	        	cout<<"--------------------------�ֽ���-----------------------------"<<endl;
	        	cout<<"���ڽ����ƽ�����..."<<endl;
	            /*ɾȥ��ͷ��s*/
	            tableValue.erase(0, 1);
	            /*�ַ���תΪ����*/
	            int num = atoi(tableValue.c_str());            
	                
				/*�½�һ��ת����ϵ*/
	            Trans trans;
	            if (words[0].first == "ID") trans.name = words[0].second;
	            else if (words[0].first == "NUM") trans.value = words[0].second;
	            
	            /*��ջ*/
	            stateStack[top++] = num; 
	            cout<<"״̬ջ��ջ��"<<num<<endl;
	            symbolStack.push_back(make_pair(words[0].first, trans));
	            cout<<"����ջ��ջ��"<<words[0].first<<endl;
	            
	            symbol.push_back(words[0].first);                    //push��ջsymbol
                attribution.push_back(trans);                                //push��ջattribution
                S.push(cnt++);                                    //S��push�ĵ�ǰ�Ľڵ��
	            
	            /*��ջ*/
	            words.erase(words.begin());
	            cout<<"���봮���ݼ���Ϊ��"<<inputStackString<<endl;
	        }
	        else if(tableValue[0] == 'r'){
	        	cout<<"--------------------------�ֽ���-----------------------------"<<endl;
	        	cout<<"���ڽ��й�Լ����..."<<endl;
	        	
	        	DyingAttr = queue<int>();

	        	
	            /*ɾȥ��ͷ��s*/
	            tableValue.erase(0, 1);
	            /*�ַ���תΪ����*/
	            int num = atoi(tableValue.c_str());  
	            
	            cout<<"��Լ���﷨����Ϊ��"<<num<<endl;
	            
	            /*�õ�num������ʽ��Լ*/
	            Production production = vec_production[num];
	            /*��¼second�ĳ���*/
	            int len = production.second.size();
	            for(int i=0;i<len;i++){
	            	/*״̬ջ��ջ*/
	                top--;
	                cout<<"״̬ջ��ջ��"<<stateStack[top]<<endl;
	                /*����ջ��ջ*/
	                cout<<"����ջ��ջ��"<<symbolStack.back().first<<endl;
	                symbolStack.pop_back();
	                
	                DyingAttr.push(S.top());                    //DyingAttr�ȼ�¼��S��Ҫpop�Ľڵ�
                    S.pop();                                //popջS
	            }
	            /*��ջ*/
	            symbolStack.push_back(make_pair(production.first, trans)); 
	            cout<<"����ջ��ջ��"<<symbolStack[0].first<<endl;
	            
	            symbol.push_back(production.first);                //pushջsymbol
                cout<<"production.first:"<<production.first<<endl;
                attribution.push_back(trans);                        //pushջattribution
                S.push(cnt++);                            //pushջS
                while (!DyingAttr.empty()) {
                    GraphOfAdjacencyList[cnt - 1].push_back(DyingAttr.front());    //����Tree
                    DyingAttr.pop();
                }
	            
	            //cout<<"production.first:"<<production.first<<endl;
	            //yjc
	            //stateStack[top++] = atoi(lr1[stateStack[top-1]][Hash[p.first]].c_str());    //pushջa
	            //cout<<"stateStack[top-1]:"<<stateStack[top-1]<<endl;
	            //cout<<"Hash[p.first]:"<<Hash[production.first]<<endl;
	            //cout<<"lr1[stateStack[top-1]][Hash[p.first]]:"<<lr1[stateStack[top-1]][Hash[production.first]]<<endl;
	            //cout<<"atoi(lr1[stateStack[top-1]][Hash[p.first]].c_str()):"<<atoi(lr1[stateStack[top-1]][Hash[production.first]].c_str())<<endl;
	            int preAtop = atoi(lr1[stateStack[top-1]][Hash[production.first]].c_str());    //pushջa
	            //��Ȼ���Ҹĳ����ˣ��������� 
	            stateStack[top++] = preAtop;
	            cout<<"״̬ջ��ջ��"<<stateStack[top]<<endl;
	            //cout<<"stateStack[top-1]:"<<stateStack[top-1]<<endl;
	        }
	        if (tableValue == "acc"){
	        	cout<<"--------------------------�ֽ���-----------------------------"<<endl;
			    cout<<"�﷨���������ҳɹ�!���û���﷨����"<<endl; 
				break; 
			} 
	        if (tableValue == "err") {
	        	cout<<"--------------------------�ֽ���-----------------------------"<<endl;
	        	string pre;
				pre =  symbolStackString;
	            cout<<"�﷨����������ʧ��!����ڴ���"+pre+"���ʱ�����˴���"<<endl; 
	            return;
	        }	        
	    }
	}
	void main() {
        smw_project::main();
        yjc_inputTableLR1();
        yjc_parsing();
    }
    
} 
