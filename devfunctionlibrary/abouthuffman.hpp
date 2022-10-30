//abouthuffman.h
//�й�huffman�Ͷ����Ƶ� 
#ifndef __cplusplus
	#error This library is only for C++
#endif 
#define hufshort unsigned short
#define hufint unsigned int
#define hufll  unsigned long long
#define hufchar unsigned char
struct myhuffcode
{
	unsigned char ch;//����ʾ�ַ� 
	ull num;		 //�ַ����ָ��� 
	int sd;			 //��Чλ 
	bool code[500];  //���� 
};
struct huffcode
{
	ull num;   //�ַ����� 
	U char ch; //����ʾ�ַ� 
	bool isleaf,root;//��Ҷ�ڵ㣬�Ǹ��ڵ� 
	string code; //���� 
	huffcode *left,*right;//�������� 
};
struct binaryshort
{
	bool binary[16];
};
struct binaryint
{
	bool binary[32];
};
struct binarychar
{
	bool binary[8];
};
struct binaryll
{
	bool binary[64];
};
bool huffmangradedowncmp(myhuffcode a,myhuffcode b)//huffman�������� 
{
	return a.num>b.num;
}
bool huffmangradeupcmp(myhuffcode a,myhuffcode b)//Huffman�������� 
{
	return a.num<b.num;
}
bool huffmanSRH(huffcode *a,huffcode *b)//huffmanС���ѽ��� 
{
	return a->num<b->num;
}
bool huffmanchup(huffcode a,huffcode b)
{
	return a.ch<b.ch;
}
bool sizecmp(huffcode a,huffcode b)
{
	return a.code.size()<b.code.size();
}
hufshort binaryshort2hufshort(binaryshort bin)
{
	hufshort ret=0,p=1;
	for(int i=15;i>=0;i--)
	  ret+=bin.binary[i]<<(15-i);
	return ret;
} 
hufint binaryint2hufint(binaryint bin)
{
	hufint ret=0,p=1;
	for(int i=31;i>=0;i--)
	  ret+=bin.binary[i]<<(31-i);
	return ret;
}
hufint binaryll2hufll(binaryll bin)
{
	hufint ret=0,p=1;
	for(int i=63;i>=0;i--)
	  ret+=bin.binary[i]<<(63-i);
	return ret;
}
hufchar binarychar2hufchar(binarychar bin)
{
	hufchar ret=0,p=1;
	for(int i=7;i>=0;i--)
	  ret+=bin.binary[i]<<(7-i);
	return ret;
}
binaryshort hufshort2binaryshort(hufshort hs)
{
	binaryshort ret;
	memset(ret.binary,0,sizeof(ret.binary));
	int p=15;
	if(hs==0)
	  return ret;
	while(hs!=0)
	{
		ret.binary[p--]=(hs%2); 
		hs/=2;
	}
	return ret;
}
binaryint hufint2binaryint(hufint hs)
{
	binaryint ret;
	memset(ret.binary,0,sizeof(ret.binary));
	int p=31;
	if(hs==0)
	  return ret;
	while(hs!=0)
	{
		ret.binary[p--]=(hs%2);
		hs/=2;
	}
	return ret;
}
binaryll hufll2binaryll(hufll hs)
{
	binaryll ret;
	memset(ret.binary,0,sizeof(ret.binary));
	int p=63;
	if(hs==0)
	  return ret;
	while(hs!=0)
	{
		ret.binary[p--]=(hs%2);
		hs/=2;
	}
	return ret;
}
binarychar hufchar2binarychar(hufchar hs)
{
	binarychar ret;
	memset(ret.binary,0,sizeof(ret.binary));
	int p=7;
	if(hs==0)
	  return ret;
	while(hs!=0)
	{
		ret.binary[p--]=(hs%2);
		hs/=2;
	}
	return ret;
} 
myhuffcode huffcode2myhuffcode(huffcode to)
{
	myhuffcode ret;
	ret.ch=to.ch;
	ret.num=to.num;
	ret.sd=to.code.size();
	for(int i=500-to.code.size();i<500;i++)
	  ret.code[i]=(to.code[i-(500-to.code.size())]=='1');
	return ret;
}
huffcode myhuffcode2huffcode(myhuffcode to)
{
	huffcode ret;
	ret.ch=to.ch;
	ret.num=to.num;
	for(int i=500-to.sd;i<500;i++)
	  ret.code+=(to.code[i]+'0');
	return ret;
} 
void clearmyhuffcode(myhuffcode *clean,ull size,U char ch,ull num,int sd,bool cod)
{
	for(ull i=0;i<size;i++)
	{
		clean[i].ch=ch;
		clean[i].num=num;
		clean[i].sd=sd;
		for(ull j=0;j<500;j++)
		  clean[i].code[j]=cod;
	}
	return;
}
void clearhuffcode(huffcode *clean,U char ch=0,ull num=0,huffcode *child=NULL,string cd="",bool isleaf=false,bool root=false)
{
	clean->ch=ch;
	clean->num=num;
	clean->left=clean->right=child;
	clean->isleaf=isleaf;
	clean->root=root;
	clean->code=cd;
	return;
}
bool onlyonetrue(bool a[256])
{
	int t=0;
	for(int i=0;i<256;i++)
	  t+=a[i];
	return (t==1); 
}
void mymakehuffmancode(myhuffcode huffman[])
{
	bool tmp[500]={0};
	ull sumofcode=0,p=0,minv;
	sort(huffman,huffman+256,huffmangradedowncmp);
	if(huffman[1].num==0||huffman[0].num==0)//ֻ��һ������Huffman�����ȡ||û�в�������ȡ��  
	{
		if(huffman[0].num!=0)
		  huffman[0].sd=1;
		return;
	}//��β����Ϊ0�����Բ��û���û�в�������ȡ�ģ�ֱ���˳� 
	for(int i=0;i<256;i++)//�����ܺ� 
	  sumofcode+=huffman[i].num;
	sumofcode-=huffman[p].num;
	minv=256;
	for(;minv>=0&&huffman[minv].num==0;minv--);
	minv=huffman[minv].num;
	if(huffman[p].num<=sumofcode)//��������С�ڵ��� 
	{
		huffman[p].code[499]=0;
		tmp[499]=1;
	}
	else//������
	{
		huffman[p].code[499]=1;
		tmp[499]=0;
	} 
	huffman[p].sd=1;
	if(sumofcode==huffman[p+1].num)//������ϣ��ܹ���ֻ������ 
	{
		huffman[p+1].code[499]=!huffman[p].code[499]; 
		return;
	}
	p++;//����λ����һλ
	while(true)//huffman code kernel
	{
		sumofcode-=huffman[p].num;
		for(int i=1;i<500;i++)//001111 -> 01111
		  huffman[p].code[i-1]=tmp[i];
		huffman[p].sd=huffman[p-1].sd+1;
		if(huffman[p].num<=sumofcode)//��������С�ڵ��� 
		{
			huffman[p].code[499]=0;
			for(int i=0;i<500;i++)
			  tmp[i]=huffman[p].code[i];
			tmp[499]=1;
		}
		else//������
		{
			huffman[p].code[499]=1;
			for(int i=0;i<500;i++)
			  tmp[i]=huffman[p].code[i];
			tmp[499]=0;
		} 
		if(huffman[p+1].num==sumofcode)//�������
		{
			for(int i=0;i<500;i++)
			  huffman[p+1].code[i]=huffman[p].code[i];
			huffman[p+1].code[499]=!huffman[p].code[499];
			huffman[p+1].sd=huffman[p].sd;
			break;
		} 
		p++; 
	}
	return;
}
void fillcode(huffcode *fill,string thiscode)
{
	fill->code=thiscode;
	if(fill->left!=NULL&&fill->right!=NULL)
	{
		fillcode(fill->left,thiscode+'0');
	    fillcode(fill->right,thiscode+'1');
	}
	return;
}
void fillcod(huffcode *huffman)
{
	sort(huffman,huffman+256,huffmanchup);
	sort(huffman,huffman+256,sizecmp);
	if(huffman[255].code.size()>=64)
	  return;
	int t=0;
	for(;huffman[t].code.size()==0;t++);
	ull icode=1;
	for(int i=0;i<huffman[t].code.size();i++)
	  icode=icode<<1;
	for(int i=t;huffman[i].code.size();i++)
	{
		ull tmp=icode++;
		huffman[i].code="";
		while(tmp!=1)
		{
			huffman[i].code+=((tmp%2)+'0');
			tmp>>=1; 
		}
		reverse(huffman[i].code.begin(),huffman[i].code.end());
		if(i==255)
		  break;
		if(huffman[i].code.size()!=huffman[i+1].code.size())
		  icode=icode<<(huffman[i+1].code.size()-huffman[i].code.size());
	}
}
bool huffmandown(huffcode a,huffcode b)
{
	return a.num>b.num; 
} 
void makehuffmancode(huffcode huffman[256])
{
	for(int i=0;i<256;i++)//�����벿������,�����ڵ�ȡ�� 
	{
		huffman[i].root=false;
	    huffman[i].code="";
	}
	priorityqueue<huffcode *,huffmanSRH> pq;//����С���� 
	huffcode sv[512];
	{
		int flag=0;
		for(int i=0;i<256;i++)
		  if(huffman[i].num!=0)
		    flag++;
		if(flag==0)//û�в���� 
		  return;
		if(flag==1)//ֻ��һ������� 
		  for(int i=0;i<256;i++)
		    if(huffman[i].num!=0)
		    {
		  	    huffman[i].code="0";
				return; 
			}
		if(flag==2)//ֻ��2������� 
		{
			int b=0;
			for(int i=0;i<256;i++)
		      if(huffman[i].num!=0)
		      {
		  	      huffman[i].code=(b==0?"0":"1");
		  	      b++; 
			  }
			return;
		}
	}
	int p=0,t=0;//p:sv����ѡ��,t:���ƻصı���ѡ��
	for(int i=0;i<256;i++)//���������� 
	  if(huffman[i].num!=0)
	  {
	  	  sv[p]=huffman[i];
	  	  sv[p].isleaf=true;
	  	  pq.push(&sv[p++]);
	  } 
	while(!pq.empty())//����huffman�� 
	{
		huffcode *leftchild,*rightchild;
		sv[p].left=pq.top();
		pq.pop();
		if(pq.empty())//root�������� 
		  break;
		sv[p].right=pq.top();
		pq.pop();
		sv[p].num=sv[p].left->num+sv[p].right->num;
		sv[p].isleaf=sv[p].root=false;
		pq.push(&sv[p++]);
	}  
	sv[p-1].root=true;//���һ�����Ǹ��ڵ� 
	fillcode(&sv[p-1],"");//fillcode(�ڼ����ڵ㣬��һ������),�ݹ�һ¥�Ǹ��ڵ㣬����codeΪ��  
	for(int i=0;i<256;i++)//�ص�ǰ��� 
	{
		huffman[i].ch=i;
		huffman[i].num=0;
		huffman[i].left=huffman[i].right=NULL;
		huffman[i].isleaf=huffman[i].root=false; 
		huffman[i].code=""; 
	}
	for(int i=0;i<p;i++)
	  if(sv[i].isleaf)
	  	huffman[sv[i].ch]=sv[i];
	return; 
}
