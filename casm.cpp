#include "devfunctionlibrary/devfunctionlibrary.hpp"
#define FAL 0
#define OK 1
#define cansee(a) ((a)>31&&(a)<127)
using namespace std;
string oversign="!@#$%^&*+=-|/?,.;:~ ",cntinvar=":\" !@#$%^&*()-=_+[]{}\\|;',.<>/?`~",calcsg="=+-^&|~<>",cmpsign="<>=!";
string regs[27]={"ax","bx","cx","dx","eax","ebx","ecx","edx","al","ah","bl","bh","dl","dh","sp","bp","si","di","flags","ip","es","ss","ds","cs"};
bool isover(char ch)//�жϵ�ǰ����Ƿ������������Ϊ�ɴ�ӡ�ַ�������һ�����ַ��ҷ����ּ������ŵ��ַ� 
{
	bool flag=true;
	for(int i=0;i<oversign.size();i++)
	  if(ch==oversign[i])
	    return true;
	return false;
}
bool isntcalcsg(char a)//�ж��ǲ��Ǽ����ַ���+-*/�� 
{
	for(int i=0;i<calcsg.size();i++)
	  if(a==calcsg[i])
	    return false;
	return true;
}
bool iscmpsg(char a)//�ж��ǲ��Ǳȴ�С�ַ���<,=,>,!�� 
{
	for(int i=0;i<cmpsign.size();i++)
	  if(a==cmpsign[i])
	  	return true;
	return false;
}
void erasespace(string &line)//�����ַ��в��ɼ��ַ��Ϳո� 
{
	for(int k=0;k<line.size();k++)
	  if(line[k]==' '||!cansee(line[k]))
	  {
	  	  line.erase(k,1);
	  	  k--;
	  }
	return;
}
void loadfile(char *infile,vector<string> &file)//�����ļ���vector<string> 
{
	ifstream fin;
	fin.open(infile);
	string line;
	while(getline(fin,line))
	  file.push_back(line);
	fin.close();
	return;
}
void writefile(char *outfile,vector<string> file)//��vector<string>�е��ļ�������� 
{
	ofstream fout;
	fout.open(outfile);
	for(int i=0;i<file.size();i++)
	  fout<<file[i]<<endl;
	fout.close();
	return;
}
short preprocess(char *infile,vector<string> &file)//����include��define���� 
{
	map<string,string> define;
	vector<string> defstr;
	ifstream fin;
	string line;
	bool noerror=true;
	for(int i=0;i<file.size();i++)
	{
		if(file[i][0]=='#')//����Ԥ����
		{
			if(file[i].find("include")!=string::npos)//includeԤ����ָ��
			{
				int j,p=0;
				for(j=0;j<file[i].size();j++)
				  if(file[i][j]=='"')
				    break;
				j++;//�ļ��ĵ�һ���ַ�
				string includefilename="";
				for(;file[i][j]!='"';includefilename+=file[i][j++]);
				if(!checkfile((char *)includefilename.c_str()))
				{
					cout<<"�����޷��ҵ��ļ���"<<includefilename<<endl;
					noerror=false;
				}
				fin.open(includefilename.c_str());
				while(getline(fin,line))//����include�ļ�����i+1�� 
				{
					file.insert(file.begin()+i+p+1,line);
					p+=1;
				}
				fin.close();
				file.erase(file.begin()+i);//������i�� 
				i--;
			}
			else
			  if(file[i].find("define")!=string::npos)//defineԤ����ָ��
			  {
				  string src="",to="";//define ull,unsigned long logn int 
				  int j;
				  for(j=7;j<file[i].size()&&(!cansee(file[i][j])||file[i][j]==' ');j++); //Ŀ��������ĵ�һ���ַ������ǿɴ�ӡ�ַ� 
				  if(j==file[i].size())//�ҵ�file.sizeҲû�ҵ�Ŀ������� 
				  {
					  cout<<"����"<<"define����δָ��Ŀ�������"<<endl;
					  noerror=false;
				  } 
				  for(;file[i][j]!=','&&j<file[i].size();to+=file[i][j++]);
				  if(j==file[i].size())//�ҵ�file.sizeҲû�ҵ��ָ�define��","
				  {
					  cout<<"����"<<"define����δָ��ԭ������"<<endl;
					  noerror=false; 
			 	  } 
				  j++;
				  for(;j<file[i].size()&&!cansee(file[i][j]);j++);
				  if(j==file[i].size())//�ҵ�file.sizeҲû�ҵ�Դ������ "
				  {
					  cout<<"����"<<"define����δָ��ԭ������"<<endl;
					  noerror=false;
				  } 
				  for(;j<file[i].size();src+=file[i][j++]);
				  define[to]=src;
				  defstr.push_back(to);
				  file.erase(file.begin()+i);
			  }
		} 
	}
	bool rem=false;
	for(int i=0;i<file.size();i++)
	{
		for(int j=0;j<(int)file[i].size()-1;j++)//����ע��
		{
			if(file[i][j]=='/'&&file[i][j+1]=='/')
			{
				file[i].erase(j,file[i].size()-j);
				break;
			}
			if(file[i][j]=='/'&&file[i][j+1]=='*') 
			{
				rem=true;
				file[i].erase(j,2);
			}
			while(j<file[i].size()-1&&rem)
			{
				if(file[i][j]=='*'&&file[i][j+1]=='/') 
				{
					file[i].erase(j,2);
					rem=false;
					break;
				}
				file[i].erase(j,1);
			}
		}
	}
	for(int i=0;i<file.size();i++)
	{
		for(int j=0;j<file[i].size();j++)//����define 
		{
			for(int k=0;k<defstr.size();k++)//�� define���в��� 
			  if(j+defstr[k].size()<file[i].size())
			  {
			  	  if(defstr[k]==file[i].substr(j,defstr[k].size()))
			  	  {
			  	  	  bool f=false;
			  	  	  if(j!=0)//���define����ǰ��
			  	  	  {
			  	  	  	  for(int l=0;l<oversign.size();l++)//��ֹ��ǰ�����һ�������ı��������Ǻ����� 
			  	  	        if(file[i][j-1]==cntinvar[l])
			  	  	        {
			  	  	    	    f=true;
			  	  	    	    break;
						    }
					  }
					  else
					    f=true;
					  if(f)//��鴮�ĺ� 
					  {
					  	  f=false;
					  	  if(j+defstr[k].size()!=file[i].size()-1)
					  	  {
					  	  	  for(int l=0;l<cntinvar.size();l++)
			  	  	            if(file[i][j+defstr[k].size()+1]==cntinvar[l])
			  	  	            {
			  	  	    	        f=true;
			  	  	    	        break;
						        }
						  }
						  else
						    f=true;
					  }
					  if(f)
					  {
				  	      string src=file[i].substr(j,defstr[k].size());
				  	      file[i].erase(j,defstr[k].size());
				  	      string nw=file[i].substr(j,file[i].size()-j);
				  	      file[i].erase(j,file[i].size()-j);
				  	      file[i]+=define[src]+nw;
					  }
				  }
			  }
		}
		if(file[i][file[i].size()-1]=='\\')
		{
			file[i].erase(file[i].size()-1,1);
			file[i]+=file[i+1];
			file.erase(file.begin()+i+1);
		}
	}
	return noerror?OK:FAL;
}
bool dotvone(string &line,int i,int j,char a,char b,string com)//����+=��-=����������������ɲ����������������� 
{
	if(line[j]==a&&line[j+1]==b)//a+=b 
	{
		string ax="",bx="";
		U I k=0;
		skipspace(line,k);//������ͷ�Ŀո� 
		for(;k<line.size()&&!isover(line[k])&&cansee(line[k])&&line[k]!=' ';ax+=line[k++]);
		skipspace(line,k);//��������ǰ����һ�Ĵ�����Ŀո� 
		k+=2;//�������ڣ�
		skipspace(line,k);//�������ں�Ŀո� 
		for(;k<line.size()&&cansee(line[k])&&line[k]!=' '&&!isover(line[k]);bx+=line[k++]);
		bool flag=false;
		for(int l=0;l<27;l++)
		  if(ax==regs[l])
		  {
		  	  flag=true;
		  	  break;
		  }
		if(ax.substr(0,7)=="memaddr")//����Ϊ��ַ ����[number]��� 
		{
			ax.erase(0,7);
			flag=true;
		}
		if(ax.substr(0,5)=="const")//Ŀ���������a+=b�е�a������Ϊ���������粻��д��123+=4 
		{
			cout<<"error: line:"<<i<<' '<<com<<" ��Ŀ�����������Ϊ���ֳ���"<<endl;
			flag=true;
		} 
		if(flag)
		{
			flag=false;
			for(int l=0;l<27;l++)
		  	  if(bx==regs[l])
		      {
		  	      flag=true;
		  	      break;
		      }
		    if(bx.substr(0,7)=="memaddr")//����Ϊ��ַ 
		    {
		    	bx.erase(0,7);
				flag=true;
			}
			if(bx.substr(0,5)=="const")//����Ϊ����
			{
				bx.erase(0,5);
				flag=true;
			} 
			if(isdigit(bx[0]))
			  flag=true;
		    if(flag)//�ǼĴ��� 
		      line=com+' '+ax+','+bx;
		}
		return 1;//�ҵ������� 
	}
	return 0;
}
bool doovone(string &line,int i,int j,char a,char b,string com)//����++--���������ַ���ɵ���ֻ��һ���������ļ��� 
{
	if(line[j]==a&&line[j+1]==b)//a++ 
	{
		string ax="";
		U I k=0;
		skipspace(line,k);//������ͷ�Ŀո� 
		for(;k<line.size()&&cansee(line[k])&&line[k]!=' '&&!isover(line[k])&&isntcalcsg(line[k]);ax+=line[k++]);
		bool flag=false;
		for(int l=0;l<27;l++)
		  if(ax==regs[l])
		  {
		  	  flag=true;
		  	  break;
		  }
		if(ax.substr(0,7)=="memaddr")//����Ϊ��ַ ����[number]��� 
		{
			ax.erase(0,7);
			flag=true;
		}
		if(ax.substr(0,5)=="const")//Ŀ���������a++�е�a������Ϊ���������粻��д��123++ 
		{
			cout<<"error: line:"<<i<<" ++��Ŀ�����������Ϊ���ֳ���"<<endl;
			flag=true;
		} 
		if(flag)
		{
		    if(flag)//�ǼĴ��� 
		      line=com+' '+ax;
		}
		return true;
	}	
	return false;
}
bool dotvvnone(string &line,int i,int j,char a,char b,char c,string com)//��������<<=����3���ַ���ɵ����� 
{
	if(line[j]==a&&line[j+1]==b&&line[j+2]==c)//a+=b 
	{
		string ax="",bx="";
		U I k=0;
		skipspace(line,k);//������ͷ�Ŀո� 
		for(;k<line.size()&&!isover(line[k])&&cansee(line[k])&&line[k]!=' '&&isntcalcsg(line[k]);ax+=line[k++]);
		skipspace(line,k);//��������ǰ����һ�Ĵ�����Ŀո� 
		k+=3;//�������ں�<<
		skipspace(line,k);//�������ں�Ŀո� 
		for(;k<line.size()&&cansee(line[k])&&line[k]!=' '&&!isover(line[k])&&isntcalcsg(line[k]);bx+=line[k++]);
		bool flag=false;
		for(int l=0;l<27;l++)
		  if(ax==regs[l])
		  {
		  	  flag=true;
		  	  break;
		  }
		if(ax.substr(0,7)=="memaddr")//����Ϊ��ַ ����[number]��� 
		{
			cout<<"error: line:"<<i<<' '<<com<<" ��Ŀ�����������Ϊ�ڴ��ַ"<<endl;
			ax.erase(0,7);
			flag=true;
		}
		if(ax.substr(0,5)=="const")//Ŀ���������a+=b�е�a������Ϊ���������粻��д��123+=4 
		{
			cout<<"error: line:"<<i<<' '<<com<<" ��Ŀ�����������Ϊ���ֳ���"<<endl;
			flag=true;
		} 
		if(flag)
		{
			flag=false;
			for(int l=0;l<27;l++)
		  	  if(bx==regs[l])
		      {
		  	      flag=true;
		  	      break;
		      }
		    if(bx.substr(0,7)=="memaddr")//����Ϊ��ַ 
		    {
		    	cout<<"error: line:"<<i<<' '<<com<<" ��Դ����������Ϊ�ڴ��ַ"<<endl;
		    	bx.erase(0,7);
				flag=true;
			}
			if(bx.substr(0,5)=="const")//����Ϊ����
			{
				bx.erase(0,5);
				flag=true;
			} 
			if(isdigit(bx[0]))
			  flag=true;
		    if(flag)//�ǼĴ��� 
		      line=com+' '+ax+','+bx;
		}
		return 1;//�ҵ������� 
	}
	return 0;
}
string casmcmp2nasmcmp(string cmpinfo,string failjmp,string format)//�Աȴ�С������з��� 
{
	failjmp.erase(failjmp.size()-1,1);//ɾȥ���һ���ַ�����Ϊ���һ����ð�ţ���nasm��jmp�����Ҫ��lable���ð�� 
	string cmp="",val1="",val2="",ret="";
	for(int i=0;i<cmpinfo.size();i++)
	{ 
		if(iscmpsg(cmpinfo[i]))//�ǱȽ���Ϣ 
		{
			for(int j=0;j<i;j++)
			  val1+=cmpinfo[j];
			cmp+=cmpinfo[i];//<,>
			if(iscmpsg(cmpinfo[i+1]))//<=,>=,==,!=
			  cmp+=cmpinfo[++i];
			for(int j=i+1;j<cmpinfo.size();j++)
			  val2+=cmpinfo[j];
			break;
		}
	}
	ret+="cmp "+val1+","+val2+"\n";//ʹ�û�����Ե�cmp���� 
	if(cmp=="<")//С��
	  (format=="signed")?ret+="jge "+failjmp+"\n":ret+="jae "+failjmp;//���ڵ��ڻ��Ǵ��ڵ��ڣ��з��ź��޷��ŷֿ��ж� 
	if(cmp=="<=")//С�ڵ���
	  (format=="signed")?ret+="jg "+failjmp+"\n":ret+="ja "+failjmp;//���ڻ��Ǹ�������ת���з��ź��޷��ŷֿ��ж� 
	if(cmp==">")//����
	  (format=="signed")?ret+="jle"+failjmp+"\n":ret+="jbe "+failjmp;//С�ڵ��ڻ��ǵ��ڵ�������ת���з��ź��޷��ŷֿ��ж�
	if(cmp==">=")//���ڵ��� 
	  (format=="signed")?ret+="jl "+failjmp+"\n":ret+="jb "+failjmp;//С�ڻ��������ת���з��ź��޷��ŷֿ��ж� 
	if(cmp=="==")//���ڵ��� 
	  ret+="jne "+failjmp;//����������ת��ע�⣬�жϲ�����ʱû�����޷���֮�� 
	if(cmp=="!=")//������
	  ret+="je "+failjmp;//���ڵ�������ת��ע�⣬�жϲ�����ʱû�����޷���֮�� 
	return ret;
}
short lablecompile(vector<string> &file)//���������滻Ϊlable 
{
	int beginlab=1,endlab=1;
	for(int i=0;i<file.size();i++)//��������� 
	{
		for(int j=0;j<file[i].size();j++)
		  if(file[i][j]=='{')//�����ţ��޶��������ŵ���һ��ֻ��дһ����䣬�����ű��� 
		  {
		  	  file[i]="beginlable"+int2string(beginlab++)+":";
			  int n=1,k=i+1;
			  bool no=false;
			  for(;k<file.size();k++)//��ȡ������ 
			    for(int l=0;l<file[k].size();l++)
				{
					if(file[k][l]=='{')
				      n++;
				    if(file[k][l]=='}')
				      n--;
				    if(n==0)
					  goto nxt; 
				} 
			  nxt:
			  file[k]="endlable"+int2string(endlab++)+":";
		  } 
	} 
}
void easycompile(vector<string> &file)//���м����滻��eax=ebx�ȴ���Ϊmov eax,ebx
{
	for(int i=0;i<file.size();i++)
	{
		for(int j=0;j<file[i].size();j++)
		{
			if(file[i][j]=='='&&file[i][j+1]!='='&&isntcalcsg(file[i][j-1]))//a=b only
			{
				string ax="",bx="";
				for(int k=0;k<j;k++)//ȡax
				  ax+=file[i][k];
				for(int k=j+1;k<file[i].size();k++)//ȡax
				  bx+=file[i][k];
				erasespace(ax);
				erasespace(bx);
				if(ax.substr(0,4)=="byte"||ax.substr(0,4)=="word")//byte word dwrd qwrd���� 
				  ax=ax.substr(0,4)+" "+ax.substr(4,ax.size()-4);
				if(ax.substr(0,5)=="dword")
				  ax=ax.substr(0,5)+" "+ax.substr(5,ax.size()-5);
				bool flag=false;
				for(int l=0;l<26;l++)
				  if(ax==regs[l])
				  {
				  	  flag=true;
				  	  break;
				  }
				if(ax.substr(0,7)=="memaddr")//����Ϊ��ַ ����[number]��� 
				{
					ax.erase(0,7);
					flag=true;
				}
				if(ax.substr(0,5)=="const")//����Ϊ��������number��� 
				{
					ax.erase(0,5);
					flag=true;
				} 
				if(flag)
				{
					flag=false;
					for(int l=0;l<26;l++)
				  	  if(bx==regs[l])
				      {
				  	      flag=true;
				  	      break;
				      }
				    if(bx.substr(0,7)=="memaddr")//����Ϊ��ַ 
				    {
				    	bx.erase(0,7);
						flag=true;
					}
					if(bx.substr(0,5)=="const")//����Ϊ����
					{
						bx.erase(0,5);
						flag=true;
					} 
					if(isdigit(bx[0]))
					  flag=true;
				    if(flag)//�ǼĴ��� 
				      file[i]="mov "+ax+','+bx;
				}
				break;//�޶���һ��ֻ��дһ����� 
			}
			if(file[i][j]=='g'&&file[i][j+1]=='o'&&file[i][j+2]=='t'&&file[i][j+3]=='o')//gotoָ��
			{
				file[i].erase(j,4);
				file[i]=file[i].substr(0,j+1)+"jmp "+file[i].substr(j,file[i].size()-j);
				if(file[i][file[i].size()-1]==':')
				  file[i].erase(file[i].size()-1,1);//jmp�����Ҫð��
				   
			} 
			dotvone(file[i],i,j,'+','=',"add");
			dotvone(file[i],i,j,'-','=',"sub");
			dotvone(file[i],i,j,'^','=',"xor");
			dotvone(file[i],i,j,'|','=',"or");
			dotvone(file[i],i,j,'&','=',"and");	 
			dotvone(file[i],i,j,'~','=',"not");
			doovone(file[i],i,j,'+','+',"inc");
			doovone(file[i],i,j,'-','-',"dec");
			dotvvnone(file[i],i,j,'<','<','=',"shl"); 
			dotvvnone(file[i],i,j,'>','>','=',"shr"); 
		}
	}
} 
void difficultcompile(vector<string> &file)//����for��if��
{
	int esp=0; 
	vector<pair<string,string> > funclist;
	for(U int i=0;i<file.size();i++)
	{
		for(U int j=0;j<file[i].size();j++)
		{
			if(j+5<file[i].size())
			{
				if(file[i].substr(j,5)=="while")//while���ѭ��,casm������Ψһ֧�ֵ�ѭ����� 
				{
					string cmpformat="";//�з��Ż��޷��űȽ�
					int erssize=0;
					for(int l=6;l<file[i].size()-1&&file[i][l-1]!=')';l++,erssize++)
					  cmpformat+=file[i][l];
					file[i].erase(6,erssize);//������unsigned����signed���ı�־������Ϊ�� 
					erasespace(cmpformat);
					swap(file[i],file[i+1]);//��while��lable����λ��,����ѭ�� 
					int lbbg=0;
					for(int l=10;l<file[i].size()-1;l++)//���һ����ð�� 
					  lbbg=lbbg*10+file[i][l]-'0';
					i++;
					int k=0;//����һ�п�ʼ�� 
					for(;k<file.size();k++)
					  if(file[k].substr(0,8)=="endlable")//�ҵ��뵱ǰbeginlabel��Ӧ��endlabel 
					  {
					  	  int lbed=0;
					  	  for(int l=8;l<file[k].size()-1;l++)//ͬ�����һ���ַ���ð�� 
					  	    lbed=lbed*10+file[k][l]-'0';
					  	  if(lbed==lbbg)
					  	    break;
					  }
					string faillable=file[k];//�˳�whileѭ�����λ�ã��˴�file[k]Ϊendlable$x$,������������file[k]����Ϊjmp beginlable$x$\n endlable$x$ 
					file[k]="jmp "+file[i-1].substr(0,file[i-1].size()-1)+"\n"+file[k];//��ͷȡ�������ڶ����ַ�����Ϊ���һ���ַ���ð�ţ��������jmp����Ҫ��lable���ð�� 
					string cmpinfo="";
					j+=5;
					skipspace(file[i],j);//����while��(��Ŀո� 
					j++;//����(
					skipspace(file[i],j);//����(���ж�����Ŀո�
					for(;file[i][j]!=')';j++)//�޶���while����ֻ��дwhile�����ж����� 
					  cmpinfo+=file[i][j];
					erasespace(cmpinfo);//��ȥ�Ƚ���Ϣ�еĿո� 
					file[i]=casmcmp2nasmcmp(cmpinfo,faillable,cmpformat);//�Աȴ�С����ķ����ӳ���faillable��ʾ�Ա�����ʧ�ܺ󣨷���0������תλ�� 
				} 
			}
			if(j+2<file[i].size())
			{
				if(file[i].substr(j,2)=="if")//if���casm��Ψһ֧�ֵ��ж���䣨ע�⣬casm����ʱ��֧��else 
				{
					string cmpformat="";//�з��Ż��޷��űȽ�
					int erssize=0;
					for(int l=4;l<file[i].size()-1&&file[i][l-1]!=')';l++,erssize++)
					  cmpformat+=file[i][l];
					file[i].erase(4,erssize);//������unsigne����signed���ı�־������Ϊ�� 
					int lbbg=0;
					for(int l=10;l<file[i+1].size()-1;l++)//��ȡ��ǰif��Ӧ�ı�����һ����ð�� 
					  lbbg=lbbg*10+file[i+1][l]-'0';
					int k=0;//����һ�п�ʼ�� 
					for(;k<file.size();k++)
					  if(file[k].substr(0,8)=="endlable")//�ҵ��뵱ǰbeginlabel��Ӧ��endlabel 
					  {
					  	  int lbed=0;
					  	  for(int l=8;l<file[k].size()-1;l++)//ͬ�����һ���ַ���ð�� 
					  	    lbed=lbed*10+file[k][l]-'0';
					  	  if(lbed==lbbg)
					  	    break;
					  }
					string faillable=file[k];
					string cmpinfo="";
					j+=2;
					skipspace(file[i],j);//����if��(��Ŀո� 
					j++;//����(
					skipspace(file[i],j);//����(���ж�����Ŀո�
					for(;file[i][j]!=')';j++)//�޶���if����ֻ��дwhile�����ж����� 
					  cmpinfo+=file[i][j];
					erasespace(cmpinfo);//��ȥ�Ƚ���Ϣ�еĿո� 
					file[i]=casmcmp2nasmcmp(cmpinfo,faillable,cmpformat); //ͬ����׸�� 
				}
			}
			if(j+4<file[i].size())//func���Ժ����Ķ��� 
			{
				if(file[i].substr(j,4)=="func") 
				{
					string funcname=file[i].substr(5,file[i].size()-1);
					erasespace(funcname);
					funclist.push_back(make_pair(funcname,file[i+1]));
					int lbbg=0;
					for(int l=10;l<file[i+1].size()-1;l++)//��ȡ��ǰfunc��Ӧ�ı�����һ����ð�� 
					  lbbg=lbbg*10+file[i+1][l]-'0';
					int k=i+1;//����һ�п�ʼ�� 
					for(;k<file.size();k++)
					  if(file[k].substr(0,8)=="endlable")//�ҵ��뵱ǰbeginlabel��Ӧ��endlabel 
					  {
					  	  int lbed=0;
					  	  for(int l=8;l<file[k].size()-1;l++)//ͬ�����һ���ַ���ð�� 
					  	    lbed=lbed*10+file[k][l]-'0';
					  	  if(lbed==lbbg)
					  	    break;
					  }
					file[k]="ret \n"+file[k]; 
					file.erase(file.begin()+i);//������i�У���Ϊ��ǰ���Ѿ�û�����ˡ���ǰ�д�ŵ��Ǻ������ƣ���������������map������ӳ��Ϊ���
					i--;//file�Ѿ�erase����һ������������i����Ҫ�䶯���Ѿ�ָ������һ�С�����i--���ں�i++�Կ� 
					break;//��ǰ�б�ɾ����ֱ������ 
				}
			}
			for(int k=0;k<funclist.size();k++)
			  if(j+funclist[k].first.size()<file[i].size())
				if(file[i].substr(j,funclist[k].first.size())==funclist[k].first)//�ҵ����� 
				  file[i]="call "+funclist[k].second.substr(0,funclist[k].second.size()-1);//��ͷȡ�������ڶ����ַ�����Ϊ���һ����ð�ţ���nasm���������call�����Ҫð�� 
		}
	}
} 
int main(int argc,char *argv[])
{ 
	cout<<"����Ԥ����"<<endl;
	vector<string> file;
	loadfile(argv[1],file);//�����ļ���vector 
	preprocess(argv[1],file);//�������Ԥ��������
	cout<<"��ʼ����"<<endl;
	lablecompile(file);//���ļ��Ĵ����ŷ���Ϊ��ǩ 
	easycompile(file);
	difficultcompile(file);
	//for(int i=0;i<file.size();i++)
	//  cout<<file[i]<<endl;
	writefile(argv[2],file);
} 
/*
�д��Ľ���
1�����ں��������г���if��func�ȹؼ��ֵ��ж�
2�����ڷֺŵĴ�����һ�����зֺŷָ����a=b;b=c;)���������Էֺ���Ϊ����β���滻Ϊ����\n��Ϊ����β���� 
3������Ԥ��������������ʹʷ����������� 
4�����ڱ�����֧��
5������for��dowhile��֧��
6�����ں������ݲ���֧�֣������û�ά����ջ�� 
7�����ڶ���ĺ��������ڲ���ʱ���ְ���������func a ... func cab ..�����ж�  
*/
