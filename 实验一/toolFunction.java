package yxj;

public class toolFunction {
	public static boolean isLetter(char c)
	{
		if(c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z')
		{
			return true;
		}
		else return false;
	}
	public static boolean isDigit(char ch)
	{
		
		if(ch >= '0' && ch <= '9')
		{
			return true;
		}
		else return false;
	}
	public static boolean isRelop(char c) {//�ж��Ƿ��ǱȽ������
		if(c=='<'||c=='='||c=='>')
			return true;
		else return false;
	}	
}
