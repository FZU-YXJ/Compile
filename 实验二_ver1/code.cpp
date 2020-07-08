#include <map>
#include <set>
#include <list>
#include <ctime>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;
#define rep(i,a,n) for(int i=a;i<n;i++)
#define per(i,a,n) for(int i=n-1;i>=a;i--)
#define all(x) (x).begin(),(x).end()
typedef pair<string, vector<string>> Production;        //����ʽ
const int WIDTH = 16;    //setw(WIDTH)
// head

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

namespace project {    //������Ŀ����
    set<string> terminal;        //�ս��
    set<string> not_terminal;    //���ս��
    set<string> all_symbol;        //���еķ���
    vector<string> sss;            //���еķ���
    map<string, int> Hash;        //���Ź�ϣ
    vector<Production> vp;        //���еĲ���ʽ���ӡ�ǰ
    set<Production> sp;            //���е���Ŀ�����ӡ���

    void PROJECT() {
        ifstream fin("Grammar.txt");
        string a, b;
        vector<string> c;
        while (fin >> a >> b) {
            not_terminal.insert(a);
            string str;
            getline(fin, str);
            stringstream ss;
            ss.str(str);
            c.clear();
            while (ss >> str) {
                c.push_back(str);
                int fg = 0;
                rep(i, 0, str.size()) {
                    if (!(str[i] >= 'a' && str[i] <= 'z' || str[i] == '_')) {    //�ķ��з��ս������Сд��ĸ���»�����ɵ�
                        fg = 1;
                        break;
                    }
                }
                if (fg) terminal.insert(str);    //�������Сд��ĸ���»�������ķ��ž����ս��
                else not_terminal.insert(str);    //�����Ƿ��ս��
            }
            vp.push_back(Production(a, c));
        }
        terminal.insert("#");
        not_terminal.erase("program'");        //ɾ��program'
        all_symbol.insert(all(terminal));
        all_symbol.insert(all(not_terminal));
        for (auto it : terminal) sss.push_back(it);
        for (auto it : not_terminal) sss.push_back(it);    //sss��ʵ��Ϊ�����lr1��ı�ͷ
        rep(i, 0, sss.size()) Hash[sss[i]] = i;
        for (auto it : vp) {
            a = it.first, c = it.second;
            rep(i, 0, c.size() + 1) {
                vector<string> d = c;
                d.insert(d.begin() + i, ".");
                sp.insert(Production(a, d));
            }
        }
    }

    void main() {
        PROJECT();
    }
}

namespace lr1 {    //����lr��
    using namespace project;

    vector<set<Project>> vsp(1);    //��Ŀ����
    string lr1[1000][1000];        //lr1��
    int n, m;                    //n�У�m��

    set<string> FIRST(vector<string> X) {    //��FIRST����
        set<string> res;
        if (terminal.find(X[0]) != terminal.end()) {
            res.insert(X[0]);    //������ս����ֱ��insert����
            return res;
        }
        else {
            rep(j, 0, vp.size()) {    //�������еĲ���ʽ
                if (vp[j].first == X[0]) {
                    if (terminal.find(vp[j].second[0]) != terminal.end())    //�����һ�����ս��
                        res.insert(vp[j].second[0]);                            //���뵽res��
                    else {
                        set<string> t = FIRST(vp[j].second);                    //����ݹ���FIRST��
                        res.insert(all(t));
                    }
                }
            }
        }
        return res;
    }

    set<Project> GO(set<Project> I, string X) {    //GO����
        set<Project> J;
        for (auto it : I) {
            vector<string> vs = it.right;
            auto pos = find(all(vs), ".");
            if (pos == vs.end() - 1) continue;    //����������һ����continue
            if (*(pos + 1) == X) {
                swap(*pos, *(pos + 1));            //�������ͺ����һ���ַ���
                J.insert(Project{ it.left, vs, it.expect });
            }
        }
        return J;
    }

    set<Project> CLOSURE(set<Project> I) {        //��closure
        while (1) {
            bool update = false;                    //�жϴ˴�ѭ���Ƿ��и���
            for (auto it : I) {
                vector<string> B = it.right;
                auto pos = find(all(B), ".");        //�ҵ�����λ��
                if (pos == B.end() - 1) continue;    //����������һ����continue
                string c = *(pos + 1);                //c���ڡ�������ַ�
                if (terminal.find(c) != terminal.end()) continue;    //���c���ս����continue
                B.erase(B.begin(), pos + 2);            //ɾ���������һ���ַ�֮ǰ�������ַ����������Լ�
                string last;                            //Ϊ�˼�֦����¼��һ����FIRST���ĵ�һ���ַ���
                for (auto ite : it.expect) {
                    B.push_back(ite);                //��expect���뵽B�ĺ���
                    if (last == B[0]) continue;        //���B[0]���ϴε�lastһ�����Ͳ����ˣ���Ϊ�ķ���û�пղ���ʽ
                    else last = B[0];
                    set<string> First = FIRST(B);    //��B��FIRST��
                    B.pop_back();
                    for (auto it1 : sp) {
                        if (it1.first == c && it1.second[0] == ".") {    //�ҵ�����ͷ��
                            set<string> ss;                                //ssΪ�գ�Ϊ�˶��ֲ���
                            Project p{ it1.first,it1.second,ss };
                            auto pos = I.lower_bound(p);                    //���ҵ�һ�����ڵ���p��λ��
                            if (pos != I.end() && (*pos).left == it1.first && (*pos).right == it1.second) {    //���posָ���first��second����
                                p = *pos;
                                I.erase(p);                        //��Ϊ����ֱ�Ӳ��룬������Ҫ��ɾ���ڲ���
                                p.expect.insert(all(First));
                                I.insert(p);
                            }
                            else {
                                update = true;        //�������µ�һ�����updateΪtrue���������Է���ֻ����expext���Բ�������updateΪtrue
                                I.insert(Project{ it1.first, it1.second, First });
                            }
                        }
                    }
                }
            }
            if (!update) break;    //���û���£�����ѭ��
        }
        return I;
    }

    void LR1() {
        for (auto it : sp) {        //�Ȳ����һ��program' -> program
            if (it.first == "program'" && it.second[0] == ".") {
                set<string> ss;
                ss.insert("#");
                vsp[0].insert(Project{ it.first,it.second, ss });
                break;
            }
        }
        vsp[0] = CLOSURE(vsp[0]);    //Ȼ�����һ����Ŀ���ıհ�
        rep(i, 0, vsp.size()) {
            time_t t = time(0);        //Ϊ���ڿ���̨��ʾÿ����Ŀ�������ʱ�䣬debug
            char tmp[64];
            strftime(tmp, sizeof(tmp), ": %X", localtime(&t));
            cout << i;
            puts(tmp);
            //��Լ
            for (auto it : vsp[i]) {
                int len = it.right.size();
                if (it.right[len - 1] == ".") {        //������һ���ַ����ǡ����Ϳ��Թ�Լ��
                    it.right.erase(it.right.end() - 1);        //ɾ���Ǹ���
                    Production p(it.left, it.right);            //�������ʽp
                    rep(j, 0, vp.size()) {
                        if (vp[j] == p) {                    //�˴�ѭ����Ϊ���ҵ�����ʽp�ı��Ϊj
                            string t = "r" + to_string(j);    //lr1��
                            if (j == 0) t = "acc";            //����ǵ�0����Ӧ����acc
                            for (auto its : it.expect)
                                lr1[i][Hash[its]] = t;        //��lr1��
                        }
                    }
                }
            }

            for (auto X : all_symbol) {                //�������еķ���
                set<Project> J = GO(vsp[i], X);        //J = GO(I, X)
                if (!J.empty()) {                    //���J��Ϊ��
                    J = CLOSURE(J);                    //��J�ıհ�
                    int k;
                    auto pos = find(all(vsp), J);
                    if (pos != vsp.end()) {            //���vsp���Ѿ������������Ŀ����
                        k = pos - vsp.begin();        //���´��ڵ�λ��
                    }
                    else {
                        k = vsp.size();                //�����ھͲ��룬���k����push֮ǰ��size
                        vsp.push_back(J);
                    }

                    //�ƽ���GOTO
                    int j = Hash[X];
                    if (terminal.find(X) != terminal.end())    //���XΪ�ս��
                        lr1[i][j] = "s" + to_string(k);        //���ƽ�
                    else lr1[i][j] = to_string(k);            //����goto
                }
            }
        }
    }

    void PRINT() {    //�˺���Ϊ�������Ŀ�����lr���ļ���
        ofstream fout("output.out");
        cout<<"���е�������"<<endl; 
        fout << "      CLOSURE" << endl;
        rep(i, 0, 20) fout << '-';
        fout << endl;
        rep(i, 0, vsp.size()) {
            fout << i << ":" << endl;
            set<Project> sp = vsp[i];
            for (auto it : sp) {
                string b;
                rep(i, 0, it.right.size()) b += ' ' + it.right[i];
                b.erase(0, 1);
                string c;
                for (auto it2 : it.expect) c += it2 + '/';
                c.pop_back();
                fout << "    " << it.left << " -> "
                    << b << ", " << c << endl;
            }
            rep(i, 0, 20) fout << '-';
            fout << endl;
        }
        fout << endl << endl << endl;

        rep(i, 0, 3) fout << setw(WIDTH) << ' ';
        fout << "LR(1)������" << endl;
        fout << '|' << endl << '|';
        fout << setw(WIDTH) << "״̬";
        fout << '|';
        n = vsp.size();
        m = sss.size();
        rep(i, 0, m) {
            fout << setw(WIDTH) << sss[i];
            if (i == terminal.size() - 1) fout << '|';
        }
        fout << '|' << endl << '|';
        rep(i, 0, 200) fout << '-';
        fout << '|' << endl << '|';
        rep(i, 0, n) {
            fout << setw(WIDTH) << i;
            fout << '|';
            rep(j, 0, m) {
                fout << setw(WIDTH) << lr1[i][j];
                if (j == terminal.size() - 1) fout << '|';
            }
            fout << '|' << endl << '|';
        }
        rep(i, 0, 200) fout << '-';
        fout << '|';
        fout << endl << endl << endl;
    }

    void OUT_LR1() {        //���lr1���ļ���
        ofstream fout("LR(1).txt");
        fout << n << " " << m << endl;
        rep(i, 0, n) {
            rep(j, 0, m) {
                if (lr1[i][j] == "") fout << "err ";        //���Ϊ�գ����err
                else fout << lr1[i][j] << ' ';
            }
            fout << endl;
        }
    }

    void main() {
        project::main();
        LR1();
        PRINT();
        OUT_LR1();
    }
}

namespace grammar {
    using namespace project;
    struct E {            //E����¼һ����Ϣ
        string place;    //λ��
        vector<pair<int, vector<string>>> code;    //��Ԫʽ
        string name;        //������name
        string value;    //������value
        int Begin;
        int Next;
        int True;
        int False;;
    };

    string lr1[1000][1000];
    int n, m;
    int cnt = 0;                //Tree�Ľڵ���
    vector<int> G[1000];        //���ڽӱ��Tree
    vector<string> symbol;    //ÿ���ڵ��Ӧ�ķ�������
    vector<E> attr;            //ÿ���ڵ���õ����ԣ�������E

    void IN_LR1() {    //����lr��
        ifstream fin("LR(1).txt");
        fin >> n >> m;
        rep(i, 0, n) rep(j, 0, m) fin >> lr1[i][j];
    }

    void MORPHOLOGY() {    //�ʷ�������
        ifstream fin("Program.txt");
        ofstream fout("Token.txt");
        string keyword[27] = { "main","if","else","int","return","void","while",
            "=","+","-","*","/","<","<=",">",">=","!=","==",",",";",":","{","}","[","]","(",")" };
        map<string, int> H;
        rep(i, 0, 27) H[keyword[i]] = i + 1;
        string s;
        while (fin >> s) {
            rep(i, 0, s.length()) {
                if (isalpha(s[i])) {
                    if (s[i] == 'm' && s[i + 1] == 'a' && s[i + 2] == 'i'
                        && s[i + 3] == 'n' && !isalnum(s[i + 4])) {
                        fout << "ID" << endl;
                        i += 3;
                    }
                    else if (s[i] == 'i' && s[i + 1] == 'f' && !isalnum(s[i + 2])) {
                        fout << "IF" << endl;
                        i += 1;
                    }
                    else if (s[i] == 'e' && s[i + 1] == 'l' && s[i + 2] == 's'
                        && s[i + 3] == 'e' && !isalnum(s[i + 4])) {
                        fout << "ELSE" << endl;
                        i += 3;
                    }
                    else if (s[i] == 'i' && s[i + 1] == 'n' && s[i + 2] == 't'
                        && !isalnum(s[i + 3])) {
                        fout << "INT" << endl;
                        i += 2;
                    }
                    else if (s[i] == 'r' && s[i + 1] == 'e' && s[i + 2] == 't' && s[i + 3] == 'u'
                        && s[i + 4] == 'r' && s[i + 5] == 'n' && !isalnum(s[i + 6])) {
                        fout << "RETURN" << endl;
                        i += 5;
                    }
                    else if (s[i] == 'v' && s[i + 1] == 'o' && s[i + 2] == 'i'
                        && s[i + 3] == 'd' && !isalnum(s[i + 4])) {
                        fout << "VOID" << endl;
                        i += 3;
                    }
                    else if (s[i] == 'w' && s[i + 1] == 'h' && s[i + 2] == 'i'
                        && s[i + 3] == 'l' && s[i + 4] == 'e' && !isalnum(s[i + 5])) {
                        fout << "WHILE" << endl;
                        i += 4;
                    }
                    else {
                        string token;
                        while (isalnum(s[i])) token += s[i++];
                        i--;
                        fout << "ID" << ' ' << token << endl;
                    }
                }
                else if (isdigit(s[i])) {
                    int num = 0;
                    while (isdigit(s[i])) num = num * 10 + s[i++] - '0';
                    i--;
                    fout << "NUM" << ' ' << num << endl;
                }
                else {
                    if (s[i] == '!') {
                        fout << "!=" << endl;
                        i++;
                    }
                    else if (s[i] == '=' && s[i + 1] == '=') {
                        fout << "==" << endl;
                        i++;
                    }
                    else if (s[i] == '<' && s[i + 1] == '=') {
                        fout << "<=" << endl;
                        i++;
                    }
                    else if (s[i] == '>' && s[i + 1] == '=') {
                        fout << ">=" << endl;
                        i++;
                    }
                    else {
                        string t;
                        t += s[i];
                        fout << t << endl;
                    }
                }
            }
        }
    }

    void JUDGE() {    //�����﷨��
        ifstream fin("Token.txt");
        ofstream fout("Analysis.txt");

        vector<pair<string, string>> vs;        //��Token
        pair<string, string> str;
        while (fin >> str.first) {
            if (str.first == "NUM" || str.first == "ID")
                fin >> str.second;    //�����NUM��ID��Ҫ����name��value
            vs.push_back(str);
        }

        int a[1000], top = 1;    //aλ״̬ջ��topΪջ��ָ��
        a[0] = 0;                                        //��ʼ״̬
        E e;                                                //��ʼ״̬
        vector<pair<string, E>> b{ make_pair("#",e) };    //��ʼ״̬
        vector<pair<string, string>> c = vs;                //��ʼ״̬
        c.push_back(make_pair("#", ""));                    //��ʼ״̬

        fout << left << setw(WIDTH / 2) << "";
        fout << left << setw(WIDTH * 4) << "״̬";
        fout << left << setw(WIDTH * 10) << "����";
        fout << right << setw(WIDTH * 7) << "���봮" << endl;

        int id = 1;    //idΪ����
        stack<int> S;    //Sͬ��ÿһ�β�����Ϊ������Tree
        while (1) {
            string temp;
            rep(i, 0, top) temp += to_string(a[i]) + ' ';
            fout << left << setw(WIDTH / 2) << id++;
            fout << left << setw(WIDTH * 4) << temp;
            string sb, sc;
            for (auto it : b) sb += ' ' + it.first;
            sb.erase(0, 1);
            for (auto it : c) sc += ' ' + it.first;
            sc.erase(0, 1);
            fout << left << setw(WIDTH * 10) << sb;
            fout << right << setw(WIDTH * 7) << sc << endl;    //���϶���Ϊ�����
			cout<<"a[top-1]"<<a[top - 1]<<endl;
            string action = lr1[a[top - 1]][Hash[c[0].first]];    //��lr1��
            cout<<"actionֵ��"<<action<<endl; 
            if (action == "acc") break;                            //�����acc��ʾ������break
            if (action == "err") {                                //�����err��ʾ����ֱ��return
                cerr << "error" << endl;
                return;
            }
            if (action[0] == 's') {                                //���action��һ���ַ�Ϊs����ʾ�ƽ�
                action.erase(0, 1);                                //ɾ����һ���ַ�
                int num = atoi(action.c_str());                    //ת������
                a[top++] = num;                                    //push��ջa
                E e;
                if (c[0].first == "ID") e.name = c[0].second;    //�����ID������name
                else if (c[0].first == "NUM") e.value = c[0].second;    //�����NUM������value
                b.push_back(make_pair(c[0].first, e));            //push��ջb
                symbol.push_back(c[0].first);                    //push��ջsymbol
                attr.push_back(e);                                //push��ջattr
                S.push(cnt++);                                    //S��push�ĵ�ǰ�Ľڵ��
                c.erase(c.begin());                                //pop��ջc
            }
            else {
                action.erase(0, 1);
                int num = atoi(action.c_str());
                cout<<"��Լ������Ϊ��"<<num<<endl;
                Production p = vp[num];                    //�õ�num������ʽ��Լ
                int len = p.second.size();                //second�ĳ���
                queue<int> son;                            //son��ʱ��¼ջS��pop�Ľڵ�
                while (len--) {
                    top--;                                //popջa
                    b.pop_back();                        //popջb
                    son.push(S.top());                    //son�ȼ�¼��S��Ҫpop�Ľڵ�
                    S.pop();                                //popջS
                }
                b.push_back(make_pair(p.first, e));        //pushջb
                symbol.push_back(p.first);                //pushջsymbol
                cout<<"p.first:"<<p.first<<endl;
                attr.push_back(e);                        //pushջattr
                S.push(cnt++);                            //pushջS
                while (!son.empty()) {
                    G[cnt - 1].push_back(son.front());    //����Tree
                    son.pop();
                }
                //yjc
                //a[top++] = atoi(lr1[a[top-1]][Hash[p.first]].c_str());    //pushջa
                cout<<"a[top-1]:"<<a[top-1]<<endl;
                cout<<"Hash[p.first]:"<<Hash[p.first]<<endl;
                cout<<"lr1[a[top-1]][Hash[p.first]]:"<<lr1[a[top-1]][Hash[p.first]]<<endl;
                cout<<"atoi(lr1[a[top-1]][Hash[p.first]].c_str()):"<<atoi(lr1[a[top-1]][Hash[p.first]].c_str())<<endl;
                int preAtop = atoi(lr1[a[top-1]][Hash[p.first]].c_str());    //pushջa
                //��Ȼ���Ҹĳ����ˣ��������� 
                a[top++] = preAtop;
                cout<<"a[top-1]:"<<a[top-1]<<endl;
            }
        }
        fout << endl << endl;

        fout << "�﷨��" << endl;    //����﷨��
        per(i, 1, cnt) if (!G[i].empty()) {
            fout << '(' << i << ',' << symbol[i] << ')' << "  ---->  ";
            sort(all(G[i]));
            rep(j, 0, G[i].size()) fout << '(' << G[i][j] << ',' << symbol[G[i][j]] << ')' << ' ';
            fout << endl;
        }
        fout << endl << endl;
    }

    void main() {
        project::main();
        IN_LR1();
        MORPHOLOGY();
        JUDGE();
    }
}

namespace translate {    //����
    using namespace grammar;
    ofstream fout("Code.txt");

    vector<vector<string>> table;    //���ű�
    int temp_cnt = 0;                //��������
    int nextquad = 100;                //��Ԫʽ���

    void mktable() {                    //�½����ű�
        table.push_back(vector<string>());
    }

    void rmtable() {                    //ɾ�����ű�
        table.pop_back();
    }

    void enter(string name) {        //��������
        table.back().push_back(name);
    }

    string lookup(string name) {        //�鿴�����Ƿ����
        per(i, 0, table.size()) rep(j, 0, table[i].size())
            if (table[i][j] == name) return name;
        return "nil";
    }

    string newtemp() {                //�½�һ������
        return "T" + to_string(++temp_cnt);
    }

    pair<int, vector<string>> gen(string a, string b, string c, string d) {    //������Ԫʽ
        vector<string> vs{ a,b,c,d };
        return make_pair(nextquad++, vs);
    }

    void dfs(int u) {    //�����﷨��
        if (G[u].empty()) return;                        //���Ϊ��ֱ��return
        if (symbol[G[u].front()] == "{") mktable();        //�����һ������Ϊ{���½����ű�
        rep(i, 0, G[u].size()) dfs(G[u][i]);                //�������еĶ��ӽڵ�

        E &e = attr[u];                                    //eΪattr[u]�����ã�����ֱ���޸�
        e = attr[G[u][0]];                                //��д���ѵ�һ�����ӵ�ֱ�Ӹ�ֵ��attr[u]
        attr[u].code.clear();                            //�����code��Ԫʽ
        rep(i, 0, G[u].size()) {                            //�����ж��ӵ���Ԫʽ�ӽ���
            for (auto it : attr[G[u][i]].code) {
                e.code.push_back(it);
            }
        }

        if (symbol[u] == "variable_definition") {//��������
            string name = attr[G[u][1]].name;
            enter(name);
        }
        else if (symbol[u] == "assignment_expression" && symbol[G[u][0]] == "ID") {//��ֵ���
            string p = lookup(attr[G[u][0]].name);    //����ű�
            if (p == "nil") {
                cerr << "����δ����" << endl;
                return;
            }
            e.place = p;
            e.code.push_back(gen("=", attr[G[u][2]].place, "-", e.place));
        }
        else if (symbol[u] == "primary_expression" && symbol[G[u][0]] == "NUM") {//��ԼNUM
            e.place = newtemp();
            e.value = attr[G[u][0]].value;
            e.code.push_back(gen("=", e.value, "-", e.place));
        }
        else if (symbol[u] == "primary_expression" && symbol[G[u][0]] == "ID") {    //��ԼID
            string p = lookup(attr[G[u][0]].name);
            if (p == "nil") {
                cerr << "����δ����" << endl;
                return;
            }
            e.place = p;
            e.name = attr[G[u][0]].name;
        }
        else if (symbol[u] == "multiplicative_expression" && G[u].size() > 1 && symbol[G[u][1]] == "*") {//�˷����ʽ
            E e1 = attr[G[u][0]];
            E e2 = attr[G[u][2]];
            e.place = newtemp();
            e.code.push_back(gen("*", e1.place, e2.place, e.place));
        }
        else if (symbol[u] == "additive_expression" && G[u].size() > 1 && symbol[G[u][1]] == "+") {//�ӷ����ʽ
            E e1 = attr[G[u][0]];
            E e2 = attr[G[u][2]];
            e.place = newtemp();
            e.code.push_back(gen("+", e1.place, e2.place, e.place));
        }
        else if (symbol[u] == "relational_expression" && G[u].size() > 1) {//��ϵ���ʽ
            E id1 = attr[G[u][0]];
            E id2 = attr[G[u][2]];
            e.code.push_back(gen("j" + symbol[G[u][1]], id1.place, id2.place, "0"));
            e.True = e.code.back().first;
            e.code.back().second[3] = to_string(e.code.back().first + 2);
            e.code.push_back(gen("j", "-", "-", "0"));
            e.False = e.code.back().first;
        }
        else if (symbol[G[u][0]] == "WHILE") {//WHILE���
            e = attr[G[u][0]];
            attr[u].code.clear();
            for (auto it : attr[G[u][2]].code)
                e.code.push_back(it);
            for (auto it : attr[G[u][4]].code)
                e.code.push_back(it);
            e.code.push_back(gen("j", "-", "-", to_string(e.code.front().first)));

            for (auto &it : e.code) {
                if (it.first == attr[G[u][2]].False) {
                    it.second[3] = to_string(e.code.back().first + 1);
                    break;
                }
            }
            e.code.back().second[3] = to_string(e.code.front().first);
        }
        else if (symbol[G[u][0]] == "IF" && G[u].size() == 5) {//IF���
            e = attr[G[u][0]];
            attr[u].code.clear();
            for (auto it : attr[G[u][2]].code)
                e.code.push_back(it);
            for (auto it : attr[G[u][4]].code)
                e.code.push_back(it);
            for (auto &it : e.code) {
                if (it.first == attr[G[u][2]].False) {
                    it.second[3] = to_string(e.code.back().first + 1);
                    break;
                }
            }
        }
        else if (symbol[G[u][0]] == "IF" && G[u].size() == 7) {//IF ELSE���
            e = attr[G[u][0]];
            attr[u].code.clear();
            for (auto it : attr[G[u][2]].code)
                e.code.push_back(it);
            for (auto it : attr[G[u][4]].code)
                e.code.push_back(it);
            e.code.push_back(gen("j", "-", "-", to_string(e.code.back().first + attr[G[u][6]].code.size() + 2)));
            e.code.back().first = e.code.front().first + e.code.size() - 1;
            for (auto &it : e.code) {
                if (it.first == attr[G[u][2]].False) {
                    it.second[3] = to_string(e.code.back().first + 1);
                    break;
                }
            }
            for (auto it : attr[G[u][6]].code) {
                it.first++;
                if (it.second[0][0] == 'j') {
                    int num = atoi(it.second[3].c_str());
                    it.second[3] = to_string(num + 1);
                }
                e.code.push_back(it);
            }
        }

        if (symbol[G[u].back()] == "}") rmtable();        //������һ������Ϊ}��ɾ�����ű�
    }

    void main() {
        int rt = cnt - 1;    //rtΪ���ڵ�
        dfs(rt);
        for (auto it : attr[rt].code)
            fout << it.first << " (" << it.second[0] << ", " << it.second[1]
            << ", " << it.second[2] << ", " << it.second[3] << ")" << endl;
        fout << nextquad << endl;    //�����һ��
    }
}

int main() {
    //lr1::main();
    grammar::main();
    translate::main();
    return 0;
}
