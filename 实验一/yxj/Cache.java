package yxj;

/*
 * ��������ֹ��EOFʱ�Ĵ���
 * ���ǽ��ַ�'~'����EOF
 * @yxj*/
public class Cache {
	public static boolean cache(char[] bf,int forword) {
		boolean flag=true;//�ж��Ƿ񵽴����ַ���ĩβ
		switch (bf[forword]) {
		case '~'://����������
			if(forword==1023) {
				forword++;
				ReadFile.readFile(bf, forword);					
			}
			else if (forword==2047) {
				forword=0;
				ReadFile.readFile(bf, forword);
			}
			else{//�ַ���ĩβ��EOF
				flag=false;
				break;
			}
		default:
			break;
		}
		return flag;
	}
}
