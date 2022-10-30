/*
2Dgraphicsʵ��For C++!
����ʱ�䣺2021-11-16~2021-11-20
�汾��V22.2.7
Reaky_Software��˾����
�����Ͼ�Reaky�������������뼼�����޹�˾[����]��2020~2021��
��Ȩ���У�Reaky_Software
ͷ�ļ���Ϣ��
����һ���ڲ�ͷ�ļ�����dev_function_library������,graphics�ֿ��е�һ��ͷ�ļ���
��ͷ�ļ���Ҫ��graphicsһ��ʹ��
�벻Ҫ����ֱ�ӵ�������ͷ�ļ�����
��ͷ�ļ�������ʹ��Ȩ�����ǲ�û����������Ȩ�������ơ��޸ġ��ַ������۱�ͷ�ļ�
���ͷ�ļ�����������C++��
*/
/*
dflimage����ע���
dflimage()���캯��start
~dflimage()��������end
loadimage(�ļ�·����������)���ļ�����ͼƬ
initgraphics(������)��ʼͼ�λ�
draw(����xԭ��,����yԭ��)��������Ļ
pause()�����������
pause(ֹͣʱ�䣨���룩)ָֹͣ��ʱ��
getx()�õ���ǰͼƬ�ĳ�
gety()�õ���ǰͼƬ�Ŀ�
getr(��ȡx�㣬��ȡy��)�õ�(x,y)�ĺ�ɫֵ
getr(����)��ȡ���صĺ�ɫֵ
getg(��ȡx�㣬��ȡy��)�õ�(x,y)����ɫֵ
getg(����)��ȡ���ص���ɫֵ
getb(��ȡx�㣬��ȡy��)�õ�(x,y)����ɫֵ
getb(����)��ȡ���ص���ɫֵ
getpixel(��ȡ��x�㣬��ȡ��y��)�õ�x��y�����ɫ
setpixel(���õ�x�㣬���õ�y�㣬���õ�ֵ)���ã�x��y������ɫ
resize(�µĳ����µĿ�)�ı���Ƭ�ߴ�
clearscreen()����
clearmemory()����ڴ��е�ͼƬ
closegraphics()�ر�ͼ�λ�
getsourcex()��ȡԭʼ��Ƭ��
getsourcey()��ȡԭʼ��Ƭ��
getdrawingboardx()��ȡ��ͼ���ڳ�
getdrawingboardy()��ȡ��ͼ���ڿ�
getmemorybuf()��ȡ��ͼ�ڴ��ַ
saveimage(�ļ������Ƿ񸲸�)���浽�ļ�
dflimage(�ļ�����������) ���ļ�������Ϊ��ʼ��
dflimage(������) �Գ�����Ϊ��ʼ��
dflimage(COLORRFָ��) ���ڴ��ַ��Ϊ��ʼ��
dflimage(mimage) ��structmimage ����ʼ��
tomimage(mimage &to) ת����mimage
void operator=(dflimage) �����ض���
reinit () ��������ͼ�ν��棨��close+init�ȼۣ� 
*/
struct mimage
{
	COLORREF*memorys=new COLORREF[1];
	int x;
	int y;
};
class dflimage
{
private:
	COLORREF*memorys,*source;
	int x,y,sx,sy,lcx,lcy;
	bool isvdo;
	void init()
	{
		memorys=new COLORREF[1];
		source=new COLORREF[1];
		x=y=sx=sy=lcx=lcy=0;
	}
public:
	dflimage()
	{
		init();
	}
	dflimage(char*infile,int nx,int ny)
	{
		init();
		loadimage(infile,nx,ny);
	}
	dflimage(int nx,int ny)
	{
		init();
		delete[] memorys;
		delete[] source;
		memorys=new COLORREF[nx*ny];
		source=new COLORREF[nx*ny];
		x=nx;
		y=ny;
	}
	dflimage(COLORREF*memory,int nx,int ny)
	{
		init();
		delete[] memorys;
		delete[] source;
		memorys=new COLORREF[nx*ny];
		source=new COLORREF[nx*ny];
		for (int i=0;i<nx*ny;i++)
			memorys[i]=source[i]=memorys[i];
		x=nx;
		y=ny;
	}
	dflimage(mimage in)
	{
		init();
		delete[] memorys;
		delete[] source;
		x=in.x;
		y=in.y;
		memorys=new COLORREF[x*y];
		source=new COLORREF[x*y];
		for (int i=0;i<x*y;i++)
			memorys[i]=source[i]=in.memorys[i];
	}
	~dflimage(void)
	{
		delete[] memorys;
		delete[] source;
	}
	void tomimage(mimage& to)
	{
		to.x=x;
		to.y=y;
		delete[] to.memorys;
		to.memorys=new COLORREF[x*y];
		for (int i=0;i<x*y;i++)
			to.memorys[i]=memorys[i];
		return;
	}
	bool loadimage(char*infile,int nx=0,int ny=0)
	{
		isvdo=false;
		if (!checkfile(infile))
			return false;
		string format;
		format=infile;
		if (format.size()<=4)//û����չ��
			return false;
		format=format.substr(format.size()-3,4);
		delete[] memorys;
		delete[] source;
		if (format !="pho")
		{
			CImage oldimg,newimg;
			oldimg.Load(infile);
			delete[] wide;
			if (oldimg.IsNull())
				return false;
			int nWidth=oldimg.GetWidth(),nHeight=oldimg.GetHeight();
			if (nx==0 || ny==0)
			{
				nx=oldimg.GetWidth();
				ny=oldimg.GetHeight();
			}
			nHeight=nx;
			nWidth=ny;
			if (!newimg.CreateEx(nWidth,nHeight,24,BI_RGB))
			{
				oldimg.Destroy();
				return false;
			}
			int nPreMode=::SetStretchBltMode(newimg.GetDC(),HALFTONE);
			newimg.ReleaseDC();
			oldimg.Draw(newimg.GetDC(),0,0,nWidth,nHeight,0,0,oldimg.GetWidth(),oldimg.GetHeight());
			newimg.ReleaseDC();
			::SetBrushOrgEx(newimg.GetDC(),0,0,NULL);
			newimg.ReleaseDC();
			::SetStretchBltMode(newimg.GetDC(),nPreMode);
			newimg.ReleaseDC();
			source=new COLORREF[newimg.GetWidth()*newimg.GetHeight()];
			memorys=new COLORREF[newimg.GetWidth()*newimg.GetHeight()];
			x=newimg.GetWidth();
			y=newimg.GetHeight();
			lcx=x;
			lcy=y;
			int p=0;
			for (int i=0;i<newimg.GetWidth();i++)
				for (int j=0;j<newimg.GetHeight();j++)
				{
					source[p]=newimg.GetPixel(i,j);
					memorys[p++]=newimg.GetPixel(i,j);
				}
		}
		else
		{
			FILE*rd=fopen(infile,"rb");
			char*sign=new char[50];
			fread(sign,25,1,rd);
			sign[25]='\0';
			if (fastcharcmp((char*)"ThisisphofilecompressBK21\0",sign)==0)
			{
				FILE*rd=fopen(infile,"rb");
				fread(sign,25,1,rd);
				sign[25]='\0';
				ull sx,sy,cx=GetSystemMetrics(SM_CXFULLSCREEN),cy=GetSystemMetrics(SM_CYFULLSCREEN);;
				unsigned int ysxs;
				fread(&ysxs,4,1,rd);
				fread(&sx,8,1,rd);
				fread(&sy,8,1,rd);
				memorys=new COLORREF[sx*sy];
				source=new COLORREF[sx*sy];
				x=sx;
				y=sy;
				lcx=x;
				lcy=y;
				int p=0;
				for (int i=0;i<sx;i++)
					for (int j=0;j<sy;j+=ysxs)
					{
						COLORREF nowcolor;
						fread(&nowcolor,4,1,rd);
						for (ull l=0;l<ysxs;l++)
						{
							source[p]=nowcolor;
							memorys[p++]=nowcolor;
						}
					}
			}
			else
			{
				if (fastcharcmp((char*)"Thisisphofilenocomprenocm\0",sign)==0)
				{
					ull sx,sy,p=0;
					fread(&sx,8,1,rd);
					fread(&sy,8,1,rd);
					x=sx;
					y=sy;
					memorys=new COLORREF[x*y];
					source=new COLORREF[x*y];
					lcx=x;
					lcy=y;
					fread(memorys,4,x*y,rd);
					for(int i=0;i<x*y;i++)
					    source[i]=memorys[i];
				}
				else
				{
					if (fastcharcmp((char*)"ThisisphofilewithRLCcmprs\0",sign)==0)
					{
						ull sx,sy;
						fread(&sx,8,1,rd);
						fread(&sy,8,1,rd);
						x=sx;
						y=sy;
						memorys=new COLORREF[x*y];
						source=new COLORREF[x*y];
						lcx=x;
						lcy=y;
						for (int i=0;i<x*y;)
						{
							COLORREF in;
							fread(&in,4,1,rd);
							for (int k=0;k<gethead(in);k++)
								memorys[i+k]=source[i+k]=nothead(in);
							i+=gethead(in);
						}
					}
					else
					{
						if (fastcharcmp((char*)"Thisisphofileforstdformat\0",sign)==0)
						{
							ull sx,sy;
							fread(&sx,8,1,rd);
							fread(&sy,8,1,rd);
							x=sx;
							y=sy;
							memorys=new COLORREF[x*y];
							source=new COLORREF[x*y];
							lcx=x;
							lcy=y;
							for (int i=0;i<x*y;i++)
							{
								unsigned char in[3];
								fread(in,1,3,rd);
								COLORREF ins=rgb(in[0],in[1],in[2]);
								memorys[i]=source[i]=ins;
							}
						}
						else
						{
							if (fastcharcmp((char*)"Thisisphofileforvdoformat\0",sign)==0)
							{
								ull sx,sy;
								fread(&sx,8,1,rd);
								fread(&sy,8,1,rd);
								x=sx;
								y=sy;
								memorys=new COLORREF[x*y];
								source=new COLORREF[x*y];
								lcx=x;
								lcy=y;
								for (int i=0;i<y;i++)
								  for(int j=0;j<x;j++)
								  {
								  	  fread(&memorys[i*x+j],4,1,rd);
								  	  memorys[i*x+j]=bgr(memorys[i*x+j]);
								  }
								for(int i=0;i<x*y;i++)
								  source[i]=memorys[i];
								isvdo=true;
							}
							else
							{
								fclose(rd);
								FILE*rb=fopen(infile,"rb");
								ull sx,sy,p=0;
								fread(&sx,8,1,rd);
								fread(&sy,8,1,rd);
								x=sx;
								y=sy;
								memorys=new COLORREF[x*y];
								source=new COLORREF[x*y];
								lcx=x;
								lcy=y;
								fread(memorys,4,x*y,rd);
								for(int i=0;i<x*y;i++)
								  source[i]=memorys[i];
							} 
						}
					}
				}
			}
			if ((nx!=x||ny!=y)&&nx&&ny)
				resize(nx,ny);
			delete[] sign;
		}
	}
	void initgraphics(int nx,int ny)
	{
		initgraph(nx,ny);
		sx=nx;
		sy=ny;
	}
	bool draw(int nx,int ny)
	{
		if (x==0||y==0)
			return false;
		int p=0;
		DWORD*GPUbuf=GetImageBuffer(NULL);
		if(!isvdo)
			for (int i=0;i<x;i++)
				for (int j=0;j<y;j++)
				{
					absfastputpixel(i+nx,j+ny,sx,sy,memorys[p],GPUbuf);
					p++;
				}
		else
			for (int i=0;i<y;i++)
				for (int j=0;j<x;j++)
				{
					absfastputpixel(i+nx,j+ny,sx,sy,memorys[p],GPUbuf);
					p++;
				}
		return true;
	}
	DWORD*getmemorybuf()
	{
		DWORD*GPUbuf=GetImageBuffer(NULL);
		return GPUbuf;
	}
	void pause(int sleeps)
	{
		Sleep(sleeps);
	}
	void pause(void)
	{
		getchar();
	}
	int getx()
	{
		return x;
	}
	int gety()
	{
		return y;
	}
	int getsourcex(void)
	{
		return lcx;
	}
	int getsourcey(void)
	{
		return lcy;
	}
	int getdrawingboardx(void)
	{
		return sx;
	}
	int getdrawingboardy(void)
	{
		return sy;
	}
	short getr(int nx,int ny)
	{
		return getR(getpixel(nx,ny));
	}
	short getb(int nx,int ny)
	{
		return getB(getpixel(nx,ny));
	}
	short getg(int nx,int ny)
	{
		return getG(getpixel(nx,ny));
	}
	short getr(COLORREF color)
	{
		return getR(color);
	}
	short getb(COLORREF color)
	{
		return getB(color);
	}
	short getg(COLORREF color)
	{
		return getG(color);
	}
	COLORREF getpixel(int nx,int ny)
	{
		return x*y<nx*x+ny?0:memorys[nx*x+ny];
	}
	void setpixel(int nx,int ny,COLORREF color)
	{
		if (x*y<nx*x+ny)
			return;
		memorys[nx*x+ny]=color;
	}
	void resize(int nx,int ny)
	{
		CImage oldimg,newimg;
		oldimg.CreateEx(lcx,lcy,24,BI_RGB);
		int p=0;
		for (int i=0;i<lcx;i++)
			for (int j=0;j<lcy;j++)
				oldimg.SetPixel(i,j,source[p++]);
		int nWidth=0,nHeight=0;
		nHeight=ny;
		nWidth=nx;
		if (!newimg.CreateEx(nWidth,nHeight,24,BI_RGB))
		{
			oldimg.Destroy();
			return;
		}
		int nPreMode=::SetStretchBltMode(newimg.GetDC(),HALFTONE);
		newimg.ReleaseDC();
		oldimg.Draw(newimg.GetDC(),0,0,nWidth,nHeight,0,0,oldimg.GetWidth(),oldimg.GetHeight());
		newimg.ReleaseDC();
		::SetBrushOrgEx(newimg.GetDC(),0,0,NULL);
		newimg.ReleaseDC();
		::SetStretchBltMode(newimg.GetDC(),nPreMode);
		newimg.ReleaseDC();
		delete[] memorys;
		x=nx;
		y=ny;
		memorys=new COLORREF[x*y];
		p=0;
		for (int i=0;i<x;i++)
			for (int j=0;j<y;j++)
				memorys[p++]=newimg.GetPixel(i,j);
		newimg.Destroy();
		oldimg.Destroy();
	}
	bool saveimage(char*outfile,bool cover=true)
	{
		string outf=outfile;
		if (outf.size()<=4)
			outf+=".pho";
		if (checkfile((char*)outf.c_str()) && !cover)
			return false;
		if (outf.substr(outf.size()-3,4)=="pho")
		{
			FILE*wt=fopen(outf.c_str(),"wb");
			char*sign=(char*)"Thisisphofilenocomprenocm";
			fwrite(sign,25,1,wt);
			if(!isvdo)
			  fwrite(memorys,x*y,4,wt);
			else
			  for (int i=0;i<y;i++)
				for(int j=0;j<x;j++)
				  fwrite(&memorys[i*x+j],4,1,wt);
		}
		else
		{
			CImage oldimg;
			oldimg.CreateEx(x,y,24,BI_RGB);
			int p=0;
			for (int i=0;i<lcx;i++)
				for (int j=0;j<lcy;j++)
					oldimg.SetPixel(i,j,source[p++]);
			oldimg.Save(outfile);
			delete[] wide;
		}
	}
	void reinit()
	{
		closegraph();
		initgraph(x,y);
	}
	void cleanscreen()
	{
		cleardevice();
	}
	void clearmemory()
	{
		delete[] memorys;
		delete[] source;
		memorys=new COLORREF[1];
		source=new COLORREF[1];
	}
	void operator=(dflimage yic)
	{
		init();
		x=yic.getx();
		y=yic.gety();
		delete[] memorys;
		delete[] source;
		memorys=new COLORREF[x*y];
		source=new COLORREF[x*y];
		for (int i=0;i<x;i++)
			for (int j=0;j<y;j++)
				setpixel(i,j,yic.getpixel(i,j));
	}
	void closegraphics()
	{
		closegraph();
	}
};
void loadimage(mimage& to,char*infile,int x,int y)
{
	dflimage dflimg(infile,x,y);
	dflimg.tomimage(to);
	return;
}
void putimage(mimage& from,int x,int y)
{
	dflimage dflimg(from);
	dflimg.draw(x,y);
}
