#ifndef __cplusplus
	#error This library is only for C++
#endif  
using namespace std;
template<typename que>
class Queue
{
	private:
		ull sizes;
		ull allsize;
		que *mai;
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
			mai=new que[1];
		}
		void reserver(ull newsize)
		{
			que *nw=new que[newsize];
			for(ull i=0;i<sizes;i++)
			  nw[i]=mai[i];
			delete[] mai;
			mai=new que[newsize];
			for(ull i=0;i<sizes;i++)
			  mai[i]=nw[i];
			delete[] nw;
			allsize=newsize;
		}
		void push(que ps)
		{
			mai[sizes++]=ps;
			if(sizes>=allsize)
			  reserver(Ceil(allsize*1.5));
		}
		que front()
		{
			return mai[0];
		}
		que back()
		{
			return mai[sizes-1];
		}
		bool empty()
		{
			return ((bool)sizes==0);
		} 
		void pop()
		{
			que *tmp=new que[allsize];
			for(ull i=1;i<sizes;i++)
			  tmp[i]=mai[i];
			delete[] mai;
			mai=new que[allsize];
			mai=tmp;
			delete[] tmp;
			sizes--;
		}
		ull size()
		{
			return sizes;
		}
		ull capacity()
		{
			return allsize;
		}
		que &operator[](ull i)
		{
			return mai[i];
		}
};
template<typename que,bool cmp(que a,que b)>
class priorityqueue
{
	private:
		ull sizes;
		ull allsize;
		que *mai;
		bool so,del;
	public:
		void clear()
		{
			sizes=0;
			allsize=1;
			if(del)
			  delete[] mai;
			else
			  del=true;
			mai=new que[1];
		}
		void reserver(ull newsize)
		{
			que *nw=new que[newsize];
			for(ull i=0;i<sizes;i++)
			  nw[i]=mai[i];
			delete[] mai;
			mai=new que[newsize];
			for(ull i=0;i<sizes;i++)
			  mai[i]=nw[i];
			delete[] nw;
			allsize=newsize;
		}
		priorityqueue()
		{
			del=false;
			clear();
		}
		void push(que ps)
		{
			mai[sizes++]=ps;
			so=false;
			if(sizes>=allsize)
			  reserver(Ceil(allsize*1.5));
		}
		que top()
		{
			if(!so)
			{
				sort(mai,mai+sizes,cmp);
				so=true;
			}
			return mai[0];
		}
		ull size()
		{
			return sizes;
		}
		bool empty()
		{
			return sizes==0;
		}
		ull capacity()
		{
			return allsize;
		}
		void pop()
		{
			if(!so)
			{
				sort(mai,mai+sizes,cmp);
				so=true;
			}
			que *tmp=new que[allsize];
			for(ull i=1;i<sizes;i++)
			  tmp[i-1]=mai[i];
			sizes--;
			for(ull i=0;i<sizes;i++)
			  mai[i]=tmp[i];
			delete[] tmp;
		}
		que *begin()
		{
			return mai;
		}
		que *end()
		{
			return &mai[sizes-1];
		}
};
template<typename que>
class dequeue
{
	private:
		ull sizes;
		ull allsize;
		que *mai;
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
			mai=new que[1];
		}
		void reserver(ull newsize)
		{
			que *nw=new que[newsize];
			for(ull i=0;i<sizes;i++)
			  nw[i]=mai[i];
			delete[] mai;
			mai=new que[newsize];
			for(ull i=0;i<sizes;i++)
			  mai[i]=nw[i];
			delete[] nw;
			allsize=newsize;
		}
		dequeue()
		{
			del=false;
			clear();
		}
		void push_back(que pb)
		{
			mai[sizes++]=pb;
			if(sizes>=allsize)
			  reserver(Ceil(allsize*1.5));
		}
		void pop_back()
		{
			sizes--;
		}
		void push_front(que pf)
		{
			que *nw=new que[allsize];
			sizes++;
			nw[0]=pf;
			for(ull i=0;i<sizes-1;i++)
			  nw[i+1]=mai[i];
			delete[] mai;
			mai=new que[allsize];
			mai=nw;
			if(sizes>=allsize)
			  reserver(Ceil(allsize*1.5));
		}
		que back()
		{
			return mai[sizes-1];
		}
		que front()
		{
			return mai[0];
		}
		ull size()
		{
			return sizes; 
		}
		bool empty()
		{
			return sizes==0; 
		}
		ull capacity()
		{
			return allsize;
		}
		que *begin()
		{
			return mai;
		}
		que *end()
		{
			return &mai[sizes-1];
		}
};
