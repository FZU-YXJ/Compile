#include "smw_PJSet.h"

//����LR(1)�ķ�����Ĳ���
namespace yxj_LRtable {
    using namespace smw_project;

    int row, col;                      //row�������������col�����������
    string LRtalbe[1005][1005];        //�洢LR(1)������Ľ��
    vector<set<Project>> pset(1);      //��Ŀ�ļ���

	//��FIRST��
	set<string> FirstSet(vector<string> X) {
        set<string> First;
        if (terminal_symbol.find(X[0])!=terminal_symbol.end()){ //������ս����ֱ��insert����
            First.insert(X[0]);
            return First;
        }
        else {
            for(int i=0;i<vec_production.size();i++){
                if (vec_production[i].first==X[0]){
                    if (terminal_symbol.find(vec_production[i].second[0])==terminal_symbol.end()){     //�����һ�������ս���Ļ�
                        set<string> temp = FirstSet(vec_production[i].second);                         //�͵ݹ���FIRST��
                        First.insert(temp.begin(),temp.end());
					}
                    else																			   //�����һ�����ս���Ļ�
						First.insert(vec_production[i].second[0]);									   //�Ͳ��뵽First��
                }
            }
        }
        return First;
    }

    set<Project> yxj_Go(set<Project> SP, string S){
        set<Project> G;
        for (auto it : SP){
            vector<string> p=it.right;
            auto positon=find(p.begin(), p.end(), ".");

			if(positon==p.end()-1)
				continue;
            if(*(positon+1)==S){
                swap(*positon, *(positon+1));
                G.insert(Project{it.left,p,it.expect});
            }
        }
        return G;
    }

    //�������ܣ�������closure
    set<Project> zyl_Closure(set<Project> I) {
        while (1){
            //is_update�����жϴ˴ε�ѭ���Ƿ��и���
            bool is_update=false;

            for (auto it : I) {
                vector<string> B=it.right;

                //position���ڼ�¼�ҵ��ġ���λ��
                auto position=find((B).begin(),(B).end(), ".");
                if (position==B.end()-1){
                    continue;
                }

                string the_char=*(position+1);
                if (terminal_symbol.find(the_char)!=terminal_symbol.end()){
                    continue;
                }

                B.erase(B.begin(), position+2);
                string last;//last���ڼ�¼��һ����FIRST���ĵ�һ���ַ���
                for (auto ite : it.expect) {
                    B.push_back(ite);//��expect���뵽B�ĺ���
                    if (last==B[0]){
                        continue;//�ķ���û�пղ���ʽ,���Բ�����
                    }

                    else last=B[0];

                    //��B��FIRST��
                    set<string> First=FirstSet(B);
                    B.pop_back();

                    for (auto it1 : set_production) {
                        if (it1.first==the_char&&it1.second[0]==".") {
                            set<string> a_set;
                            Project p{it1.first,it1.second,a_set};
                            auto position=I.lower_bound(p);//���ҵ�һ�����ڵ���p��λ��
                            if (position!=I.end()&&(*position).left==it1.first&&(*position).right==it1.second){
                                p=*position;
                                I.erase(p);//��ɾ���ٲ���
                                p.expect.insert((First).begin(),(First).end());
                                I.insert(p);
                            }
                            else {
                                is_update=true;
                                I.insert(Project{it1.first,it1.second,First});
                            }
                        }
                    }
                }
            }
            //û����������ѭ��
            if (!is_update) {
                break;
            }
        }
        return I;
    }

	//���LR1(1)������Ĺ���
    void yxj_LR1(){
        for (auto it : set_production){
            if (it.first=="program'"&&it.second[0]=="."){
                set<string> setString;
                setString.insert("#");
                pset[0].insert(Project{it.first,it.second,setString});
                break;
            }
        }

        pset[0]=zyl_Closure(pset[0]);
        for(int i=0;i<pset.size();i++){

			//���й�Լ
            for (auto it : pset[i]){
                int length=it.right.size();

                if (it.right[length-1]=="."){
                    it.right.erase(it.right.end() - 1);
                    Production p(it.left, it.right);
					for(int j=0;j<vec_production.size();j++){
                        if (vec_production[j]==p){
                            string res="r" + to_string(j);
							if (j==0)
								res="acc";
                            for (auto its : it.expect)
                                LRtalbe[i][Hash[its]]=res;
                        }
                    }
                }
            }

            for (auto S : all_symbols){               //�������еķ���
                set<Project> P=yxj_Go(pset[i], S);    //P = GO(I, X)
                if (!P.empty()){                      //���P��Ϊ��
                    P=zyl_Closure(P);                 //��P�ıհ�
                    int t;
                    auto positon=find(pset.begin(),pset.end(),P);
                    if(positon==pset.end()){
                    	t=pset.size();
                        pset.push_back(P);
                    }
                    else{
                        t=positon-pset.begin();
                    }

                    //GOTO���ƽ�
                    int j;
					j=Hash[S];
                    if (terminal_symbol.find(S)==terminal_symbol.end())
  						LRtalbe[i][j]=to_string(t);
                    else
                    	LRtalbe[i][j]="s"+to_string(t);
                }
            }
        }
    }

    //�����Ŀ����LR(1)�ķ������ļ���
    void Print_Pj() {
        ofstream fout("yxj_result.txt");//������ļ�yxj_result.txt

        fout<<"      CLOSURE"<<endl;
        for(int i=0;i<40;i++)
			fout<<'*';
        fout<<endl;

        //�����Ŀ��
		for(int i=0;i<pset.size();i++){
            fout<<i<<":"<<endl;
            set<Project> set_production=pset[i];
            for (auto it1 : set_production){
                string b;
                for(int i=0;i<it1.right.size();i++)
					b+=' '+it1.right[i];
                b.erase(0, 1);

				string c;
                for(auto it2 : it1.expect)
					c+=it2+'/';
                c.pop_back();
                fout<<"    "<<it1.left<<" -> "<<b<<", "<<c<<endl;
            }
            for(int i=0;i<40;i++)
				fout<<'*';
            fout<<endl;
        }
        fout<<endl<<endl<<endl<<endl;

		//���LR(1)�ķ�����
        for(int i=0;i<3;i++)
			fout<<setw(16)<<' ';
        fout<<"LR(1)�ķ�����"<< endl;
        fout<<'|'<<endl<<'|';
        fout<<setw(16)<<"״̬";
        fout<<'|';
        row=pset.size();
        col=vec_symbols.size();
        for(int i=0;i<col;i++){
            fout<<setw(16)<<vec_symbols[i];
            if(i==terminal_symbol.size()-1)
				fout<<'|';
        }
        fout<<'|'<<endl<<'|';
       	for(int i=0;i<200;i++)
		   fout<<'-';
        fout<<'|'<<endl<<'|';
        for(int i=0;i<row;i++){
            fout<<setw(16)<<i;
            fout<<'|';
            for(int j=0;j<col;j++){
                fout<<setw(16)<<LRtalbe[i][j];
                if(j==terminal_symbol.size()-1)
					fout<<'|';
            }
            fout<<'|'<<endl<<'|';
        }
        for(int i=0;i<200;i++)
			fout<<'-';
        fout<<'|';
        fout<<endl<<endl<<endl<<endl;
    }

	//��LR(1)������������ļ�yxj_LR(1)Table.txt��
    void Print_LR1(){
        cout<<"we do it "<<endl;
        ofstream fout("yxj_LR(1)Table.txt");
        fout<<row<<" "<<col<<endl;;
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++) {
                if(LRtalbe[i][j]!="") //�ж��Ƿ�Ϊ��
					fout<<LRtalbe[i][j]<<' ';
                else //���Ϊ�գ����err
					fout<<"err ";
            }
            fout<<endl;
        }
    }

    //������ļ�
    void yxj_Print(){
    	Print_Pj();
    	Print_LR1();
	}

    void main() {
        smw_project::main();
        yxj_LR1();
        yxj_Print();
    }
}
