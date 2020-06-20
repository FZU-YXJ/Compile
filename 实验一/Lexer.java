package yxj;

public class Lexer {	
	public int begin=0;//��ʼָ��
	public int forward=0;//����ָ��
	public int state=0;	//״̬
	public String Key[]= {"if","else"};//�ؼ���
	

	public String word;//���ʷ���
	public String type;//���

	
	public void analyse(char[] bf) {
		ReadFile.readFile(bf, forward);//������һ��
		while(Cache.cache(bf, forward)){
			int fwd=forward;
			while(fwd==forward) {//����ͬһ���ַ���ѭ��
				switch (state) {
				case 0:
					state=relop(bf);
					break;
				case 9:
					state=id(bf);
					break;
				case 12:
					state=fail();
					break;
				case 20:
					state=fail();
					break;
				case 25:
					state=fail();
					break;
				default:
					break;
				}
			}
			state=0;
		}
	}

	//�Ƚ������DFA
	public int relop(char[] bf) {
		int state=0;
		char c;
		while(toolFunction.isRelop(c=bf[forward])) {
			switch (c) {
			case '<':
				state=1;
				c=bf[++forward];
				switch (c) {
				case '=':
					state=2;
					word="<=";
					type="relop";
					break;
				case '>':
					state=3;
					word="<>";
					type="relop";
					break;
				default:
					state=4;
					retract(1);//����
					word="<";
					type="relop";				
					break;
				}
				break;
			case '=':
				state=5;
				word="=";
				type="��ֵ";
				break;
			case '>':
				state=6;
				c=bf[++forward];
				switch (c) {
				case '=':
					state=7;
					word=">=";
					type="relop";
					break;
				default:
					state=8;
					retract(1);//����
					word=">";
					type="relop";
					break;
				}
				break;
			default:
				state=fail();
				break;
			}
			forward++;
			System.out.print("\n<"+word+","+type+">");		
		}
		if(state==0)
			state=fail();
		return state;
	}
	
	//��ʶ�����ؼ��ֵ�DFA
	public int id(char[] bf) {
		int state=9;
		char c;
		String str="";
		if(toolFunction.isLetter(c=bf[forward])) {
			state=10;
			str+=c;
			forward++;
			while(toolFunction.isLetter(c=bf[forward])||toolFunction.isDigit(c=bf[forward])) {
				state=10;
				str+=c;
				forward++;
			}
			state=11;
			retract(1);//����
			word=str;
			type="id";
			for (String s : Key) {//�ж�����Ǳ�ʶ�����ǹؼ���
				if(word.equals(s)){
					type="key";
				}
				//System.out.print("\nword:  "+word+"  s:  "+s+"  �����"+(word.equals(s)));
			}
			forward++;
			System.out.print("\n<"+word+","+type+">");	
		}
		else 
			state=fail();
		return state;
	}
	
	//��*���ս�״̬���ˣ���ɨ����õ�
	public void retract(int n) {
		forward-=n;
	}
	
	
	//���Ӽ���DFA
	public int fail() {
		int start=state;
		switch (start) {
        case 0:   start = 9;  break;
        case 9:   start = 12; break;
        case 12:  start = 20; break;
        case 20:  start = 25; break;
        case 25:  forward++;/*recover();*/  break;
        default:  /* compiler error */break;			
		}
		return start;
	}
	
}
