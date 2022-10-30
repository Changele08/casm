#include "devfunctionlibrary/devfunctionlibrary.hpp"
#define FAL 0
#define OK 1
#define cansee(a) ((a)>31&&(a)<127)
using namespace std;
string oversign="!@#$%^&*+=-|/?,.;:~ ",cntinvar=":\" !@#$%^&*()-=_+[]{}\\|;',.<>/?`~",calcsg="=+-^&|~<>",cmpsign="<>=!";
string regs[27]={"ax","bx","cx","dx","eax","ebx","ecx","edx","al","ah","bl","bh","dl","dh","sp","bp","si","di","flags","ip","es","ss","ds","cs"};
bool isover(char ch)//判断当前语句是否结束。结束符为可打印字符中任意一个非字符且非数字及非括号的字符 
{
	bool flag=true;
	for(int i=0;i<oversign.size();i++)
	  if(ch==oversign[i])
	    return true;
	return false;
}
bool isntcalcsg(char a)//判断是不是计算字符（+-*/） 
{
	for(int i=0;i<calcsg.size();i++)
	  if(a==calcsg[i])
	    return false;
	return true;
}
bool iscmpsg(char a)//判断是不是比大小字符（<,=,>,!） 
{
	for(int i=0;i<cmpsign.size();i++)
	  if(a==cmpsign[i])
	  	return true;
	return false;
}
void erasespace(string &line)//擦除字符中不可见字符和空格 
{
	for(int k=0;k<line.size();k++)
	  if(line[k]==' '||!cansee(line[k]))
	  {
	  	  line.erase(k,1);
	  	  k--;
	  }
	return;
}
void loadfile(char *infile,vector<string> &file)//加载文件到vector<string> 
{
	ifstream fin;
	fin.open(infile);
	string line;
	while(getline(fin,line))
	  file.push_back(line);
	fin.close();
	return;
}
void writefile(char *outfile,vector<string> file)//将vector<string>中的文件内容输出 
{
	ofstream fout;
	fout.open(outfile);
	for(int i=0;i<file.size();i++)
	  fout<<file[i]<<endl;
	fout.close();
	return;
}
short preprocess(char *infile,vector<string> &file)//仅对include和define处理。 
{
	map<string,string> define;
	vector<string> defstr;
	ifstream fin;
	string line;
	bool noerror=true;
	for(int i=0;i<file.size();i++)
	{
		if(file[i][0]=='#')//编译预处理
		{
			if(file[i].find("include")!=string::npos)//include预处理指令
			{
				int j,p=0;
				for(j=0;j<file[i].size();j++)
				  if(file[i][j]=='"')
				    break;
				j++;//文件的第一个字符
				string includefilename="";
				for(;file[i][j]!='"';includefilename+=file[i][j++]);
				if(!checkfile((char *)includefilename.c_str()))
				{
					cout<<"错误：无法找到文件："<<includefilename<<endl;
					noerror=false;
				}
				fin.open(includefilename.c_str());
				while(getline(fin,line))//加载include文件到第i+1行 
				{
					file.insert(file.begin()+i+p+1,line);
					p+=1;
				}
				fin.close();
				file.erase(file.begin()+i);//擦掉第i行 
				i--;
			}
			else
			  if(file[i].find("define")!=string::npos)//define预处理指令
			  {
				  string src="",to="";//define ull,unsigned long logn int 
				  int j;
				  for(j=7;j<file[i].size()&&(!cansee(file[i][j])||file[i][j]==' ');j++); //目标操作数的第一个字符必须是可打印字符 
				  if(j==file[i].size())//找到file.size也没找到目标操作数 
				  {
					  cout<<"错误："<<"define命令未指定目标操作数"<<endl;
					  noerror=false;
				  } 
				  for(;file[i][j]!=','&&j<file[i].size();to+=file[i][j++]);
				  if(j==file[i].size())//找到file.size也没找到分割define的","
				  {
					  cout<<"错误："<<"define命令未指定原操作数"<<endl;
					  noerror=false; 
			 	  } 
				  j++;
				  for(;j<file[i].size()&&!cansee(file[i][j]);j++);
				  if(j==file[i].size())//找到file.size也没找到源操作数 "
				  {
					  cout<<"错误："<<"define命令未指定原操作数"<<endl;
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
		for(int j=0;j<(int)file[i].size()-1;j++)//处理注释
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
		for(int j=0;j<file[i].size();j++)//处理define 
		{
			for(int k=0;k<defstr.size();k++)//在 define表中查找 
			  if(j+defstr[k].size()<file[i].size())
			  {
			  	  if(defstr[k]==file[i].substr(j,defstr[k].size()))
			  	  {
			  	  	  bool f=false;
			  	  	  if(j!=0)//检查define串的前面
			  	  	  {
			  	  	  	  for(int l=0;l<oversign.size();l++)//防止当前语句是一个长长的变量名或是函数名 
			  	  	        if(file[i][j-1]==cntinvar[l])
			  	  	        {
			  	  	    	    f=true;
			  	  	    	    break;
						    }
					  }
					  else
					    f=true;
					  if(f)//检查串的后方 
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
bool dotvone(string &line,int i,int j,char a,char b,string com)//处理+=，-=等由两个操作符组成并且有两个操作数的 
{
	if(line[j]==a&&line[j+1]==b)//a+=b 
	{
		string ax="",bx="";
		U I k=0;
		skipspace(line,k);//跳过开头的空格 
		for(;k<line.size()&&!isover(line[k])&&cansee(line[k])&&line[k]!=' ';ax+=line[k++]);
		skipspace(line,k);//跳过等于前、第一寄存器后的空格 
		k+=2;//跳过等于；
		skipspace(line,k);//跳过等于后的空格 
		for(;k<line.size()&&cansee(line[k])&&line[k]!=' '&&!isover(line[k]);bx+=line[k++]);
		bool flag=false;
		for(int l=0;l<27;l++)
		  if(ax==regs[l])
		  {
		  	  flag=true;
		  	  break;
		  }
		if(ax.substr(0,7)=="memaddr")//后方数为地址 ，由[number]组成 
		{
			ax.erase(0,7);
			flag=true;
		}
		if(ax.substr(0,5)=="const")//目标操作数（a+=b中的a）不得为常量，例如不能写成123+=4 
		{
			cout<<"error: line:"<<i<<' '<<com<<" 的目标操作数不得为数字常量"<<endl;
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
		    if(bx.substr(0,7)=="memaddr")//后方数为地址 
		    {
		    	bx.erase(0,7);
				flag=true;
			}
			if(bx.substr(0,5)=="const")//后方数为常量
			{
				bx.erase(0,5);
				flag=true;
			} 
			if(isdigit(bx[0]))
			  flag=true;
		    if(flag)//是寄存器 
		      line=com+' '+ax+','+bx;
		}
		return 1;//找到并处理 
	}
	return 0;
}
bool doovone(string &line,int i,int j,char a,char b,string com)//处理++--这样两个字符组成但是只有一个操作数的计算 
{
	if(line[j]==a&&line[j+1]==b)//a++ 
	{
		string ax="";
		U I k=0;
		skipspace(line,k);//跳过开头的空格 
		for(;k<line.size()&&cansee(line[k])&&line[k]!=' '&&!isover(line[k])&&isntcalcsg(line[k]);ax+=line[k++]);
		bool flag=false;
		for(int l=0;l<27;l++)
		  if(ax==regs[l])
		  {
		  	  flag=true;
		  	  break;
		  }
		if(ax.substr(0,7)=="memaddr")//后方数为地址 ，由[number]组成 
		{
			ax.erase(0,7);
			flag=true;
		}
		if(ax.substr(0,5)=="const")//目标操作数（a++中的a）不得为常量，例如不能写成123++ 
		{
			cout<<"error: line:"<<i<<" ++的目标操作数不得为数字常量"<<endl;
			flag=true;
		} 
		if(flag)
		{
		    if(flag)//是寄存器 
		      line=com+' '+ax;
		}
		return true;
	}	
	return false;
}
bool dotvvnone(string &line,int i,int j,char a,char b,char c,string com)//处理类似<<=的由3个字符组成的运算 
{
	if(line[j]==a&&line[j+1]==b&&line[j+2]==c)//a+=b 
	{
		string ax="",bx="";
		U I k=0;
		skipspace(line,k);//跳过开头的空格 
		for(;k<line.size()&&!isover(line[k])&&cansee(line[k])&&line[k]!=' '&&isntcalcsg(line[k]);ax+=line[k++]);
		skipspace(line,k);//跳过等于前、第一寄存器后的空格 
		k+=3;//跳过等于和<<
		skipspace(line,k);//跳过等于后的空格 
		for(;k<line.size()&&cansee(line[k])&&line[k]!=' '&&!isover(line[k])&&isntcalcsg(line[k]);bx+=line[k++]);
		bool flag=false;
		for(int l=0;l<27;l++)
		  if(ax==regs[l])
		  {
		  	  flag=true;
		  	  break;
		  }
		if(ax.substr(0,7)=="memaddr")//后方数为地址 ，由[number]组成 
		{
			cout<<"error: line:"<<i<<' '<<com<<" 的目标操作数不得为内存地址"<<endl;
			ax.erase(0,7);
			flag=true;
		}
		if(ax.substr(0,5)=="const")//目标操作数（a+=b中的a）不得为常量，例如不能写成123+=4 
		{
			cout<<"error: line:"<<i<<' '<<com<<" 的目标操作数不得为数字常量"<<endl;
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
		    if(bx.substr(0,7)=="memaddr")//后方数为地址 
		    {
		    	cout<<"error: line:"<<i<<' '<<com<<" 的源操作数不得为内存地址"<<endl;
		    	bx.erase(0,7);
				flag=true;
			}
			if(bx.substr(0,5)=="const")//后方数为常量
			{
				bx.erase(0,5);
				flag=true;
			} 
			if(isdigit(bx[0]))
			  flag=true;
		    if(flag)//是寄存器 
		      line=com+' '+ax+','+bx;
		}
		return 1;//找到并处理 
	}
	return 0;
}
string casmcmp2nasmcmp(string cmpinfo,string failjmp,string format)//对比大小命令进行翻译 
{
	failjmp.erase(failjmp.size()-1,1);//删去最后一个字符，因为最后一个是冒号，而nasm中jmp命令不需要在lable后加冒号 
	string cmp="",val1="",val2="",ret="";
	for(int i=0;i<cmpinfo.size();i++)
	{ 
		if(iscmpsg(cmpinfo[i]))//是比较信息 
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
	ret+="cmp "+val1+","+val2+"\n";//使用汇编语言的cmp命令 
	if(cmp=="<")//小于
	  (format=="signed")?ret+="jge "+failjmp+"\n":ret+="jae "+failjmp;//高于等于或是大于等于，有符号和无符号分开判断 
	if(cmp=="<=")//小于等于
	  (format=="signed")?ret+="jg "+failjmp+"\n":ret+="ja "+failjmp;//大于或是高于则跳转，有符号和无符号分开判断 
	if(cmp==">")//大于
	  (format=="signed")?ret+="jle"+failjmp+"\n":ret+="jbe "+failjmp;//小于等于或是低于等于则跳转，有符号和无符号分开判断
	if(cmp==">=")//大于等于 
	  (format=="signed")?ret+="jl "+failjmp+"\n":ret+="jb "+failjmp;//小于或低于则跳转，有符号和无符号分开判断 
	if(cmp=="==")//等于等于 
	  ret+="jne "+failjmp;//不等于则跳转，注意，判断不等于时没有有无符号之分 
	if(cmp=="!=")//不等于
	  ret+="je "+failjmp;//等于等于则跳转，注意，判断不等于时没有有无符号之分 
	return ret;
}
short lablecompile(vector<string> &file)//将大括号替换为lable 
{
	int beginlab=1,endlab=1;
	for(int i=0;i<file.size();i++)//处理大括号 
	{
		for(int j=0;j<file[i].size();j++)
		  if(file[i][j]=='{')//上括号，限定：打括号的那一行只能写一条语句，即括号本身。 
		  {
		  	  file[i]="beginlable"+int2string(beginlab++)+":";
			  int n=1,k=i+1;
			  bool no=false;
			  for(;k<file.size();k++)//获取下括号 
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
void easycompile(vector<string> &file)//进行简易替换将eax=ebx等代替为mov eax,ebx
{
	for(int i=0;i<file.size();i++)
	{
		for(int j=0;j<file[i].size();j++)
		{
			if(file[i][j]=='='&&file[i][j+1]!='='&&isntcalcsg(file[i][j-1]))//a=b only
			{
				string ax="",bx="";
				for(int k=0;k<j;k++)//取ax
				  ax+=file[i][k];
				for(int k=j+1;k<file[i].size();k++)//取ax
				  bx+=file[i][k];
				erasespace(ax);
				erasespace(bx);
				if(ax.substr(0,4)=="byte"||ax.substr(0,4)=="word")//byte word dwrd qwrd…… 
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
				if(ax.substr(0,7)=="memaddr")//后方数为地址 ，由[number]组成 
				{
					ax.erase(0,7);
					flag=true;
				}
				if(ax.substr(0,5)=="const")//后方数为常量，由number组成 
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
				    if(bx.substr(0,7)=="memaddr")//后方数为地址 
				    {
				    	bx.erase(0,7);
						flag=true;
					}
					if(bx.substr(0,5)=="const")//后方数为常量
					{
						bx.erase(0,5);
						flag=true;
					} 
					if(isdigit(bx[0]))
					  flag=true;
				    if(flag)//是寄存器 
				      file[i]="mov "+ax+','+bx;
				}
				break;//限定：一行只能写一条语句 
			}
			if(file[i][j]=='g'&&file[i][j+1]=='o'&&file[i][j+2]=='t'&&file[i][j+3]=='o')//goto指令
			{
				file[i].erase(j,4);
				file[i]=file[i].substr(0,j+1)+"jmp "+file[i].substr(j,file[i].size()-j);
				if(file[i][file[i].size()-1]==':')
				  file[i].erase(file[i].size()-1,1);//jmp命令不需要冒号
				   
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
void difficultcompile(vector<string> &file)//处理for，if等
{
	int esp=0; 
	vector<pair<string,string> > funclist;
	for(U int i=0;i<file.size();i++)
	{
		for(U int j=0;j<file[i].size();j++)
		{
			if(j+5<file[i].size())
			{
				if(file[i].substr(j,5)=="while")//while语句循环,casm语言中唯一支持的循环语句 
				{
					string cmpformat="";//有符号或无符号比较
					int erssize=0;
					for(int l=6;l<file[i].size()-1&&file[i][l-1]!=')';l++,erssize++)
					  cmpformat+=file[i][l];
					file[i].erase(6,erssize);//擦除（unsigned）或（signed）的标志，化繁为简 
					erasespace(cmpformat);
					swap(file[i],file[i+1]);//将while和lable交换位置,方便循环 
					int lbbg=0;
					for(int l=10;l<file[i].size()-1;l++)//最后一个是冒号 
					  lbbg=lbbg*10+file[i][l]-'0';
					i++;
					int k=0;//从下一行开始找 
					for(;k<file.size();k++)
					  if(file[k].substr(0,8)=="endlable")//找到与当前beginlabel对应的endlabel 
					  {
					  	  int lbed=0;
					  	  for(int l=8;l<file[k].size()-1;l++)//同理，最后一个字符是冒号 
					  	    lbed=lbed*10+file[k][l]-'0';
					  	  if(lbed==lbbg)
					  	    break;
					  }
					string faillable=file[k];//退出while循环后的位置，此处file[k]为endlable$x$,下条语句结束后file[k]将变为jmp beginlable$x$\n endlable$x$ 
					file[k]="jmp "+file[i-1].substr(0,file[i-1].size()-1)+"\n"+file[k];//从头取到倒数第二个字符，因为最后一个字符是冒号，而汇编中jmp不需要在lable后加冒号 
					string cmpinfo="";
					j+=5;
					skipspace(file[i],j);//跳过while和(间的空格 
					j++;//跳过(
					skipspace(file[i],j);//跳过(与判断语句间的空格
					for(;file[i][j]!=')';j++)//限定：while当行只能写while及其判断条件 
					  cmpinfo+=file[i][j];
					erasespace(cmpinfo);//擦去比较信息中的空格 
					file[i]=casmcmp2nasmcmp(cmpinfo,faillable,cmpformat);//对比大小命令的翻译子程序，faillable表示对比条件失败后（返回0）的跳转位置 
				} 
			}
			if(j+2<file[i].size())
			{
				if(file[i].substr(j,2)=="if")//if命令，casm中唯一支持的判断语句（注意，casm中暂时不支持else 
				{
					string cmpformat="";//有符号或无符号比较
					int erssize=0;
					for(int l=4;l<file[i].size()-1&&file[i][l-1]!=')';l++,erssize++)
					  cmpformat+=file[i][l];
					file[i].erase(4,erssize);//擦除（unsigne）或（signed）的标志，化繁为简 
					int lbbg=0;
					for(int l=10;l<file[i+1].size()-1;l++)//获取当前if对应的标号最后一个是冒号 
					  lbbg=lbbg*10+file[i+1][l]-'0';
					int k=0;//从下一行开始找 
					for(;k<file.size();k++)
					  if(file[k].substr(0,8)=="endlable")//找到与当前beginlabel对应的endlabel 
					  {
					  	  int lbed=0;
					  	  for(int l=8;l<file[k].size()-1;l++)//同理，最后一个字符是冒号 
					  	    lbed=lbed*10+file[k][l]-'0';
					  	  if(lbed==lbbg)
					  	    break;
					  }
					string faillable=file[k];
					string cmpinfo="";
					j+=2;
					skipspace(file[i],j);//跳过if和(间的空格 
					j++;//跳过(
					skipspace(file[i],j);//跳过(与判断语句间的空格
					for(;file[i][j]!=')';j++)//限定：if当行只能写while及其判断条件 
					  cmpinfo+=file[i][j];
					erasespace(cmpinfo);//擦去比较信息中的空格 
					file[i]=casmcmp2nasmcmp(cmpinfo,faillable,cmpformat); //同理，不赘述 
				}
			}
			if(j+4<file[i].size())//func，对函数的定义 
			{
				if(file[i].substr(j,4)=="func") 
				{
					string funcname=file[i].substr(5,file[i].size()-1);
					erasespace(funcname);
					funclist.push_back(make_pair(funcname,file[i+1]));
					int lbbg=0;
					for(int l=10;l<file[i+1].size()-1;l++)//获取当前func对应的标号最后一个是冒号 
					  lbbg=lbbg*10+file[i+1][l]-'0';
					int k=i+1;//从下一行开始找 
					for(;k<file.size();k++)
					  if(file[k].substr(0,8)=="endlable")//找到与当前beginlabel对应的endlabel 
					  {
					  	  int lbed=0;
					  	  for(int l=8;l<file[k].size()-1;l++)//同理，最后一个字符是冒号 
					  	    lbed=lbed*10+file[k][l]-'0';
					  	  if(lbed==lbbg)
					  	    break;
					  }
					file[k]="ret \n"+file[k]; 
					file.erase(file.begin()+i);//擦除第i行，因为当前行已经没有用了。当前行存放的是函数名称，而函数名称则在map中重新映射为标号
					i--;//file已经erase过了一个，所以这是i不需要变动就已经指向了下一行。这里i--用于和i++对抗 
					break;//当前行被删除，直接跳过 
				}
			}
			for(int k=0;k<funclist.size();k++)
			  if(j+funclist[k].first.size()<file[i].size())
				if(file[i].substr(j,funclist[k].first.size())==funclist[k].first)//找到函数 
				  file[i]="call "+funclist[k].second.substr(0,funclist[k].second.size()-1);//从头取到倒数第二个字符，因为最后一个是冒号，而nasm汇编语言中call命令不需要冒号 
		}
	}
} 
int main(int argc,char *argv[])
{ 
	cout<<"进行预处理"<<endl;
	vector<string> file;
	loadfile(argv[1],file);//加载文件到vector 
	preprocess(argv[1],file);//处理编译预处理命令
	cout<<"开始翻译"<<endl;
	lablecompile(file);//将文件的大括号翻译为标签 
	easycompile(file);
	difficultcompile(file);
	//for(int i=0;i<file.size();i++)
	//  cout<<file[i]<<endl;
	writefile(argv[2],file);
} 
/*
有待改进：
1、对于函数名称中出现if，func等关键字的判断
2、对于分号的处理（将一行中有分号分割（例如a=b;b=c;)，并将“以分号作为语句结尾”替换为“以\n作为语句结尾”） 
3、对于预处理中语义分析和词法分析的完善 
4、对于变量的支持
5、对于for、dowhile的支持
6、对于函数传递参数支持（不让用户维护堆栈） 
7、对于定义的函数名称在查找时出现包含（例如func a ... func cab ..）的判断  
*/
