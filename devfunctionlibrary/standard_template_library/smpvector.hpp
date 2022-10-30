#ifndef __cplusplus
	#error This library is only for C++
#endif
using namespace std;
template<typename vec>
class smpvector
{
	private:
		ull sizes;
		ull allsize;
		vec *mai;
		bool del;
	public:
		void clear()
		{
			sizes=0;
			allsize=1;
			if(del)
			  delete[] mai;
			else
			  del=true;
			mai=new vec[1];
		}
		~smpvector()
		{
			delete[] mai;
		} 
		void reserve(ull newsize)
		{
			vec *nw=new vec[newsize];
			for(ull i=0;i<sizes;i++)
			  nw[i]=mai[i];
			delete[] mai;
			mai=new vec[newsize];
			for(ull i=0;i<sizes;i++)
			  mai[i]=nw[i];
			delete[] nw;
			allsize=newsize;
		}
		smpvector()
		{
			del=false;
			clear();
		}
		void push(vec ps)
		{
			mai[sizes++]=ps;
			if(sizes>=allsize)
			  reserve(Ceil(allsize*1.5));
		}
		ull size()
		{
			return sizes;
		}
		ull capacity()
		{
			return allsize;
		}
		void pop()
		{
			size--;
		}
		bool empty(void)
		{
			return ((bool)sizes==0);
		}
		vec *begin()
		{
			return mai;
		}
		vec *end()
		{
			return &mai[sizes-1];
		}
		vec &operator[](ull i)
		{
			if(i<sizes)
			  return mai[i];
		}
		void operator=(smpvector yic)
		{
			if(sizeof(yic[0])==sizeof(vec))
			{
				clear();
				for(ull i=0;i<yic.size();i++)
				  push(yic[i]);
			}
		}
		smpvector operator+(smpvector plus)
		{
			smpvector ret;
			if(sizeof(plus[0])==sizeof(vec))
			{
				for(ull i=0;i<size();i++)
				  ret.push(mai[i]); 
				for(ull i=0;i<plus.size();i++)
				  ret.push(plus[i]);
				return ret;
			}
		} 
};
