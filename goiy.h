#include <string.h>
#include "btree.h"

typedef struct mangxaugoiyRec
{
  char tu[100];
} mangxaugoiy;


int ss2xau(char s1[],char s2[])
{
  int i,d=1;
  for (i=1;i<=strlen(s2);i++)
    {
      if (s2[i-1]!=s1[i-1])
	{
	  d=0;
	  return;
	}
    }
  return d;
}


int goiy(char s[],BTA*tree,char u[])
{
  int j=0;
  mangxaugoiy a[50];
  int value,d,i=0;
  char s1[100],s2[100];
  btpos(tree, 1);
 while(btseln(tree,s1,s2, 101,&value)==0&&i<50)
   {
     if (ss2xau(s1,s)==1)
       {
	 i++;
	 strcpy(a[i-1].tu,s1);
	 printf("%d\t%s\n",i,s1);
       }
   }
 if(i!=0)
   do{
     printf("Moi ban nhap 1 so de chon tu ma ban muon:");
     scanf("%d",&d);
     strcpy(u,a[d-1].tu);
   }while(d<0||d>i);
 else
   j=1;
 return j;
}
