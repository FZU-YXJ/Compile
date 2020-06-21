package ʵ���;

import java.util.HashMap;
import java.util.Map;
import java.util.Stack;
/*
 * function:grammatical analysis * 
 */
public class yjc_parsing {
	private Stack<Integer> stateStack = new Stack<>();
	private Stack<String> symbolStack = new Stack<>();
	private Stack<String> inputStringStack = new Stack<>();
	private Map<Integer,Map<String,String>> actionTable = new HashMap<>();//action��
	private Map<Integer,Map<String,String>> gotoTable = new HashMap<>();//goto��
	private Map<Integer,type> rulesMap=new HashMap<>();
	/*�������*/
	private int step=1;
	/*
	 * function:whether is Terminator
	 * params:
	 */
	public boolean isTerminator() {
		return true;
	}
	/*
	 * function:search Action Table
	 * params:
	 */
	public void searchAction(String inputStringStackTop,int stateStackTop) {		
		/*��ȡaction��������*/
		Map<String,String> actionMapValue= new HashMap<>();
		actionMapValue = actionTable.get(stateStackTop);
		String actionString = actionMapValue.get(inputStringStackTop);
		/*��������֫��*/
		char actionStringHead = actionString.charAt(0);
		String actionStringLeft = actionString.substring(1);
		
		/*��Լ*/
		if(actionStringHead=='r') {
			int ruleNum=Integer.parseInt(actionStringLeft);
			type type=new type();
			type=rulesMap.get(ruleNum);
			/*Ҫ��������*/
			int popnum=type.pop;
			System.out.println(step,stateStack.toString(), symbolStack.toString(),"����"+type.left+"->"+type.right+"�Ĺ�����й�Լ");
			for(int i=0;i<popnum;i++)
			{
				stateStack.pop();//״̬ջ����
				symbolStack.pop();//����ջ����
			}
			stateStackTop=stateStack.peek();//����
			symbolStack.push(type.right);
			/*���ս����goto��*/
			Map<String,String> gotoMap=new HashMap<>();
			gotoMap=gotoTable.get(stateStackTop);//���ս����goto
		    if(gotoMap.containsKey(type.right))
		    {
		    	symbolStack.push(Integer.parseInt(gotoMap.get(type.right)));
		    	stateStackTop=Integer.parseInt(gotoMap.get(type.right));		    	
		    }
		    else
		    {
		    	System.out.println("error sen!");
		    	System.out.println(("�﷨����!��ǰ״̬ ��"+stateStackTop+"�޷���������:"+actionString));
		    	break;
		    }
		}
		else if(actionStringHead=='s')/*����*/
		{
			System.out.println(step+ stateStack.toString()+symbolStack.toString()+actionString);
			stateStack.push(Integer.parseInt(actionStringLeft));
			stateStackTop=Integer.parseInt(actionStringLeft);
			symbolStack.push(actionString);
		}
		else if(actionString.equals("acc"))
		{
			//��Լ�ɹ�
			System.out.println(step+stateStack.toString()+symbolStack.toString());
			System.out.println("success!");
			break;
		}
		else
		{
			System.out.println("error");
			System.out.println(("\r\n�﷨����!\r\n��ǰ״̬ : "+stateStackTop+"\r\n�޷��������� ��"+actionString+"\r\n").getBytes());
			break;
		}
	}
	/*
	 * function:search Goto Table
	 * params:
	 */
	public void searchGoto(String inputStringStackTop,int stateStackTop) {
		/*��ȡgoto��������gotoValue*/
		Map<String,String> gotoMapValue= new HashMap<>();
		gotoMapValue = gotoTable.get(stateStackTop);
		String gotoValue=gotoMapValue.get(inputStringStackTop);
		/*stateStackTop:�ڼ�������*/
		stateStackTop = Integer.parseInt(gotoValue);
		stateStack.push(stateStackTop);
		
	}
	/*
	 * function:working starter
	 * params:useless params
	 */
	public void main(String[] args) {
		// TODO Auto-generated method stub
		/*��ȡԤ����õ�����*/
		while(true) {
			if(isTerminator()) {/*�ս����action*/
				searchAction(inputStringStack.peek(),stateStack.peek());
			}else {/*���ս����goto*/
				searchGoto(inputStringStack.peek(),stateStack.peek());
			}
			step+=1;//�����һ
		}
		System.out.println("���е���main");
	}

}
