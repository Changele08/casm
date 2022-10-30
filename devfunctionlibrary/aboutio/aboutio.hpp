//�й�IO 
#ifndef __cplusplus
	#error This library is only for C++
#endif 
#include <fstream>
#include <io.h>
#include <stdio.h>
#include <string>
#define Byte 1
#define KB   1000
#define MB   1000000
#define GB   1000000000
#define TB   1000000000000
#define eoi -1
#define null NULL
#define EOI -1
#define KEY_DOWN(VK_NONAME) (bool)(GetAsyncKeyState(VK_NONAME)&0x8000)
using namespace std;
void getFiles( string path, vector<string>& files)  
{
    intptr_t hFile = 0;	        //�ļ����
    struct _finddata_t fileinfo;  //�ļ���Ϣ��ȡ�ṹ
    string p;
    if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)  
    {  
        do  
        {   
	     	if(fileinfo.attrib & _A_SUBDIR)    //�Ƚ��ļ������Ƿ����ļ���
	     	{
	     		if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)  
	         	{
	         		files.push_back(p.assign(path).append("\\").append(fileinfo.name)+"\\") ;
	 	     		getFiles( p.assign(path).append("\\").append(fileinfo.name), files );     //������ļ��У��������±���
	         	}
		 	}
	     	else    //���ļ�
	          files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  //���ļ�������·�����뵽files������
        }while(_findnext(hFile, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
        _findclose(hFile); 
    } 
}
long long fastread()//��� 
{
	char ch;
	int ans=0;
	bool num=true;
	ch=getchar();
	while(!isdigit(ch))
	{
		if(ch=='-')
	      num=!num;
	    ch=getchar();
	}
	while(isdigit(ch))
	{
		ans*=10;
		ans+=ch-'0';
		ch=getchar(); 
	}
	return num?ans:-ans;
}
bool checkfile(char *filename)
{
	fstream _file;
    _file.open(filename,ios::in);
    return (bool)_file;
}
ull getfilesize(char *file)
{
	ull ret=0,n;
	FILE *rd=fopen(file,"rb");
	if(!checkfile(file))
	  return 0;
	char *ch=new char[1000000];
	while(n)
	{
		n=fread(ch,1,1000000,rd);
		ret+=n;
	}
	fclose(rd);
	delete[] ch;
	return ret;
}
bool mergerfiles(char *infile,char *infile1,char *outfile,bool cover)
{
	char ch;
	FILE *rd=fopen(infile,"rb"),*wt=fopen(outfile,"wb");
	if(!checkfile(infile)||!checkfile(infile1)||(checkfile(outfile)&&!cover))
	  return false;
	for(;fread(&ch,1,1,rd);fwrite(&ch,1,1,wt));
	fclose(rd);
	rd=fopen(infile1,"rb");
	for(;fread(&ch,1,1,rd);fwrite(&ch,1,1,wt));
	fclose(rd);
	fclose(wt);
	return true;
}
void printprogress(ull ok,ull all)
{
	cout<<"----------------------"<<endl;
	cout<<'|';
	for(int i=0;i<ok*10/all;i++)
	  cout<<"��";
	for(int i=ok*10/all*2;i<20;i++)
	  cout<<' '; 
	cout<<'|'<<endl;
	cout<<"----------------------"<<endl;
}
bool copyfile(char *from,char *to,bool cover)
{
	if(!cover&&checkfile(to))//cover==true������copy 
	  return false;
	else
	{
		FILE *rd=fopen(from,"rb"),*wt=fopen(to,"wb");
		char ch;
		for(;fread(&ch,1,1,rd);fwrite(&ch,1,1,wt));
		fclose(rd);
		fclose(wt);
	} 
	return true;
}
bool filecmp(char *file,char *file1)
{
	FILE *rd=fopen(file,"rb"),*rb=fopen(file1,"rb");
	char ch,ch1;
	if(getfilesize(file)!=getfilesize(file1))
	  return false;
	for(;fread(&ch,1,1,rd)&&fread(&ch1,1,1,rb);)
	  if(ch!=ch1)
	    return false;
	return true;
}
ull infilefind(char *file,char find)
{
	ull wz;
	FILE *rd=fopen(file,"rb");
	char ch;
	for(wz=0;fread(&ch,1,1,rd);wz++)
	  if(ch==find)
	    return wz;
	return 0;
} 
bool loadfont(char* file,unsigned char *loadto)
{
	FILE* rd=fopen(file,"rb");
	if(rd==NULL)
		return false;
	I i;
	fread(&i,4,1,rd);
	fread(&i,4,1,rd);
	fread(loadto,1,getfilesize(file)-8,rd);
	return true;
}
ull readvlqnumber(FILE *rd)//���ض�ȡ���Ĳ��� 
{
	U char buf[8],p=0;
	memset(buf,0,8); 
	U char ch;
	if(!fread(&ch,1,1,rd))
	  return -1;
	buf[p++]=ch;
	while(gethighest(ch))
	{
		if(!fread(&ch,1,1,rd))
		  return -1;
		buf[p++]=ch;
	} 
	return vlq2int(buf);
}
ull writenumberinvlq(ull num,FILE *wt)//��������˼�λ 
{
	U char buf[8];
	int sz=int2vlq(num,buf);
	fwrite(buf,1,sz,wt);
	return sz;
}
void copyFILE(FILE *rd,char *out,ull sz)
{
	FILE *wt=fopen(out,"wb");
	for(ull i=0;i<sz;i++)
	{
		U char ch;
		fread(&ch,1,1,rd);
		fwrite(&ch,1,1,wt);
	}
	fclose(wt);
	return;
}
#include "graphicsio.hpp" 
#include "ops.hpp"
#include "bitfile.hpp" 
