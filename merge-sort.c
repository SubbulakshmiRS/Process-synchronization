#include<stdio.h>

long int a[100001];

void merge1(long int a[],long int st1,long int end1,long int st2,long int end2)
{
	long int i=st1,j=st2,k=0;
	long int n1=end1+1;
	long int n2=end2+1;
	long int c[end2-st1+5];

	while(i<n1&&j<n2)
	{
		if(a[i]<a[j])
			c[k++]=a[i++];
		else if(a[j]<=a[i]) 
			c[k++]=a[j++];
	}
	if(i<n1)
		while(i<n1)
			c[k++]=a[i++];
	else if(j<n2)
		while(j<n2)
			c[k++]=a[j++];
	i=0;
	while(k--)
	{
		a[st1+i]=c[i];
		i++;
//		printf("%ld %ld\n",k+1,a[k+1]);
	
	}

//	printf("\n\n");

}

void merge_sort1(long int a[],long int st,long int end)
{
//	long int call=1;
	long int mid ;
	if(st<end)
	{
		mid=(st+end)/2;
		merge_sort1(a,st,mid);
		merge_sort1(a,mid+1,end);
		merge1(a,st,mid,mid+1,end);
	
	}

}

/*
int main()
{
	long int n;
	scanf("%ld",&n);
	for(long int i=0;i<n;i++)
		scanf("%ld",&a[i]);
	merge_sort1(a,0,n-1);
	
//	printf("SORTED ARRAY\n");
	for(long int i=0;i<n;i++)
		printf("%ld ",a[i]);
	return 0;
}
*/