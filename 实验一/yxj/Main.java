package yxj;

public class Main {	
	public static void main(String[] args) {
		char[] bf=new char[2048];//����˫������
		bf[1023]='~';//EOF���Ž�β
		bf[2047]='~';//EOF���Ž�β
		Lexer lexer=new Lexer();
		lexer.analyse(bf);			
	}
}
