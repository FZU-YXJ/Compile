package yxj;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.Reader;

/*ʵ���ı���Ԥ����
 * ����˫������
 * �ֶα���
 * ÿ�����벻����1K
 * @yxj*/
public class ReadFile {
	public static String fileName="test.txt";
	//��ȡ�ļ��еĳ���
	public static void readFile(char[] bf,int fwd) {
		File file=new File(fileName);
		Reader reader=null;
		try {
			reader=new InputStreamReader(new FileInputStream(file));
			int ch;
			while((ch=reader.read())!=-1) {				
				if((char)ch!='\r'&&(char)ch!='\n'&&(char)ch!=' ') {//Ԥ�����С��ո�				
					bf[fwd]=(char) ch;//���ַ����뻺����
					fwd++;										
					System.out.print((char)ch);//����					
				}
				if(bf[fwd]=='~')//ÿ���������1K���ַ�
					break;
			}
			bf[fwd]='~';//EOF���Ž�β
			reader.close();
		}catch(Exception e) {
			e.printStackTrace();  
		}
	}
}
