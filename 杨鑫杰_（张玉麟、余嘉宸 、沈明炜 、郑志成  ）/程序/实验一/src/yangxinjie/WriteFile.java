package yangxinjie;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

/*д���ļ�yxj_output.txt
@yxj*/
public class WriteFile {
	public static void write(String str) {
		try {
			File file=new File("yxj_output.txt");
			if(!file.exists()) {
				file.createNewFile();
			}
			FileWriter fileWriter=new FileWriter(file.getName(),true);//ʹ��true��������append file
			fileWriter.write(str);
			fileWriter.close();
		}catch(IOException e) {
			e.printStackTrace();
		}		
	}
	public static void new_write(String str) {
		try {
			File file=new File("yxj_output.txt");
			if(!file.exists()) {
				file.createNewFile();
			}
			FileWriter fileWriter=new FileWriter(file.getName());//����ԭ�ļ��е�����
			fileWriter.write(str);
			fileWriter.close();
		}catch(IOException e) {
			e.printStackTrace();
		}		
	}
}
