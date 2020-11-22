#include "mod_liste_adiacenta.cpp"
#include<fstream>
#include<vector>
#include<iostream>
#include<queue>
using namespace std;


void DFs(int n, vector<int> *la,int x, int *viz, vector<int> &parc_df,int *&d,int *&tata){
    viz[x]=1;
    parc_df.push_back(x+1);
    for (int i=0;i<la[x].size();i++){
		int y=la[x][i];
        if (viz[y]==0){
		     viz[y]=1;
             tata[y]=x;
             d[y]=d[x]+1;
             DFs(n,la,y,viz,parc_df,d,tata);
        }
    }
}

void DF(int n, vector<int> *la, vector<int> &parc_df,int *&d,int *&tata, int s=1){
    int *viz=new int[n];
    tata=new int[n+1];
	d=new int[n+1];
	parc_df.clear();
	for(int i=0;i<n;i++){
		tata[i]=d[i]=-1;
		viz[i]=0;
	}
	s--;
    d[s]=0;
    DFs(n,la,s,viz,parc_df,d,tata);
}

void BF(int n, vector<int> *la, vector<int> &parc_bf,int *&d,int *&tata, int s=1)
{
	parc_bf.clear();
	int i,*viz=new int[n];
	tata=new int[n+1];
	d=new int[n+1];
	for(i=0;i<n;i++){
		tata[i]=d[i]=-1;
		viz[i]=0;
	}
	s--;
	queue<int> c;
	c.push(s);
	viz[s]=1;
	d[s]=0;
	
	while(c.size()>0){
		int x=c.front();//x- nodul curent
		c.pop();
		parc_bf.push_back(x+1);
		for (i=0;i<la[x].size();i++){
			int y=la[x][i];//pentru toti vecinii y ai nodului curent x
			if(viz[y]==0)
				{ 
					viz[y]=1;
					tata[y]=x; 
					d[y]=d[x]+1;
					c.push(y);
				}
		}
	}
}

void afis(int n,vector<int> parc_bf,int *d,int *tata){
	int i;
	for(i=0;i<parc_bf.size();i++)
	    cout<<parc_bf[i]<<" ";
	cout<<endl;
	for(i=0;i<n;i++)
	    cout<<d[i]<<" ";
	cout<<endl;  
	for(i=0;i<n;i++)
	    cout<<tata[i]<<" ";
	cout<<endl;    
}
int main(){
	vector<int> *la,parc_bf,parc_df;
	int n,m,s;
	int i,j;
	int  *tata,*d;
    
    cout<<"neorientat"<<endl;
	citire(n,la );
    afisare_liste(n,la);
    s=1;
   	BF(n,la,parc_bf,d,tata,s);
   	afis(n,parc_bf,d,tata);
   	DF(n,la,parc_df,d,tata,s);
   	afis(n,parc_df,d,tata);
   	
   	cout<<"orientat"<<endl;
	citire(n,la,1,"grafor.in" );
    afisare_liste(n,la);
    s=1;
   	BF(n,la,parc_bf,d,tata,s);
	afis(n,parc_bf,d,tata);
	DF(n,la,parc_df,d,tata,s);
   	afis(n,parc_df,d,tata);
}
