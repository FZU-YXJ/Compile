package yangxinjie;


//�ʷ�������
//@yxj
public class Lexer {	
	public int begin=0;//��ʼָ��
	public int forward=0;//����ָ��
	public int state=0;	//״̬
	public String Key[]= {"boolean","byte","char", "double","false","float",
			"int","long","new","short","true","void","instanceof","break",
			"case","catch","continue","default","else","for","if",
			"return","switch","try"," while","finally","throw","this", 
			"super","abstract","final","native","private", "protected",
			"public","static","synchronized","transient","volatile","class",
			"extends", "implements","interface", "package","import","throws",
			"true","false","null"};//�ؼ���
	

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
					state=number(bf);
					break;
				case 20:
					state=number(bf);
					break;
				case 25:
					state=number(bf);
					break;
				case 28:
					state=delim(bf);
					break;
				case 31:
					//System.out.print("\nhhh");
					state=Op(bf);
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
			WriteFile.write("\n<"+word+","+type+">");
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
			type="id";
			while(toolFunction.isLetter(c=bf[forward])||toolFunction.isDigit(c=bf[forward])) {
				state=10;
				str+=c;
				forward++;
				for (String s : Key) {//�ж�����Ǳ�ʶ�����ǹؼ���
					if(str.equals(s)){					
						type="key";
						break;
					}
					//System.out.print("\nword:  "+word+"  s:  "+s+"  �����"+(word.equals(s)));
				}
				if (type=="key") {
					break;
				}				
			}
			state=11;
			retract(1);//����
			word=str;			
			forward++;
			System.out.print("\n<"+word+","+type+">");
			WriteFile.write("\n<"+word+","+type+">");
		}
		else 
			state=fail();
		return state;
	}
	
	//������DFA
	public int number(char bf[]) 
	{
//		int pos = 1;
//		boolean flag = false;
		String str="";
//		Pair pair = new Pair(); 
//		Scanner in = new Scanner(System.in);
//		String s = in.nextLine();
//		s += "\n";
		char c = bf[forward]; 
		if(toolFunction.isDigit(c))
		{
			state=13;
//			if(!toolFunction.isDigit(c) && c!='.' && c!='E' && c!='+' && c!='-')
//			{
//				break;
//			}
			while(true)
			{
				str+=c;
				c=bf[++forward];				
				switch(state)
				{
//				case 12:								
//					if(toolFunction.isDigit(c))
//					{
//						state = 13;
//					}
//					break;
				case 13:
					if(toolFunction.isDigit(c))
					{
						state = 13;
					}
					else if(c=='E')
					{
						state = 16;
					}
					else if(c=='.')
					{
						state = 14;
					}
					else {
						state = 27;
					}
					break;
				case 14:
					if(toolFunction.isDigit(c))
					{
						state = 15;
					}
					break;
				case 15:
					if(toolFunction.isDigit(c))
					{
						state = 15;
					}
					else if(c=='E')
					{
						state = 16;
					}
					else {
						state = 24;
					}
					break;
				case 16:
					if(c=='+'||c=='-')
					{
						state = 17;
					}
					else if(toolFunction.isDigit(c))
					{
						state = 18;
					}
					break;
				case 17:
					if(toolFunction.isDigit(c))
					{
						state = 18;
					}
					break;
				case 18:
					if(toolFunction.isDigit(c))
					{
						state = 18;
					}
					else if(!toolFunction.isDigit(c)){
						state = 19;
					}
					break;
				}
				if(state == 19 || state == 24 || state == 27) {
					break;
				}
			}
			word=str;
			type="num";
			System.out.print("\n<"+word+","+type+">");
			WriteFile.write("\n<"+word+","+type+">");
		}
		else {
			state=fail();
		}
		return state;
	}
	
	//�����DFA
	public int delim(char bf[]) {
		state=28;
		if(toolFunction.isDelim(bf[forward])) {
			state=29;
			String str="";
			str+=bf[forward];
			word=str;
			type="���";
			forward++;			
			System.out.print("\n<"+word+","+type+">");
			WriteFile.write("\n<"+word+","+type+">");
		}
		else {
			state=fail();
		}
		return state;
	}
	
	//�����������DFA
	public int Op(char bf[]) {
		//System.out.print("\nhhh");
		if(toolFunction.isOp(bf[forward])) {
			String str="";
			str+=bf[forward];
			word=str;
			type="op";
			forward++;			
			System.out.print("\n<"+word+","+type+">");
			WriteFile.write("\n<"+word+","+type+">");
		}
		else {
			state=fail();
		}
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
        case 25:  start = 28; break;
        case 28:  start = 31; break;
        case 31:  
        	System.out.print("\n compiler error!");
        	WriteFile.write("\n compiler error!");/*recover();*/  
        	break;
        default: 
        	System.out.print("\n compiler error!!");
        	WriteFile.write("\n compiler error!"); 
        	break;			
		}
		return start;
	}
	
}
