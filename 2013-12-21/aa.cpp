#include <cstdio>
#include <cstring>
#include <stack>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;
typedef unsigned long long ull;
const int N=200010;
char s[N];
int n,len,next[N],a[N],size[N],position[N],to[N],p[N];
ull power[N],hash[N];
void init()
{
    memset(next,-1,sizeof(next));
    stack <int> S;
    for (int i=1;i<=len;i++)
        if (s[i]=='0')
            S.push(i);
        else if (!S.empty())
        {
            next[S.top()]=i;
            S.pop();
        }
    power[0]=1;
    for (int i=1;i<=len;i++)
        power[i]=power[i-1]*3;
    for (int i=len;i;i--)
    {
        if (next[i]==-1)
            continue;
        hash[i]=power[next[i]-i]+2;
        if (next[i]!=i+1)
            hash[i]+=hash[i+1]*3;
    }
    vector <ull> lq;
    for (int i=1;i<=len;i++)
    {
        if (next[i]==-1)
            continue;
        lq.push_back(hash[i]);
        if (next[next[i]+1]!=-1)
            to[i]=to[next[i]+1];
        else
            to[i]=next[i];
    }
    sort(lq.begin(),lq.end());
    lq.erase(unique(lq.begin(),lq.end()),lq.end());
    int cnt=0;
    for (int i=1;i<=len;i++)
        if (next[i]!=-1)
        {
            p[i]=lower_bound(lq.begin(),lq.end(),hash[i])-lq.begin()+1;
            cnt++;
        }
    queue <int> Q;
    for (int i=1;i<=len;i++)
        if (next[i]!=-1)
        {
            Q.push(i);
            i=to[i];
        }
    while (!Q.empty())
    {
        int l=Q.front(),r=to[l];
        Q.pop();
        for (int i=l;i<=r;i++)
        {
            if (next[i]!=i+1)
                Q.push(i+1);
            a[++n]=p[i];
            size[n]=next[i]-i+1;
            position[n]=i;
            i=next[i];
        }
        a[++n]=++cnt;
        size[n]=0;
    }
}
int sa[N],rank[N],height[N],f[N],x[N];
void sort(int *a)
{
    int t=0;
    memset(f,0,sizeof(f));
    for (int i=1;i<=n;i++)
    {
        f[a[i]]++;
        t=max(t,a[i]);
    }
    for (int i=1;i<=t;i++)
        f[i]+=f[i-1];
    for (int i=n;i;i--)
        x[f[a[sa[i]]]--]=sa[i];
    for (int i=1;i<=n;i++)
        sa[i]=x[i];
}
int count(int *a,int *b)
{
    rank[sa[1]]=1;
    int t=1;
    for (int i=2;i<=n;i++)
    {
        if (a[sa[i]]!=a[sa[i-1]] || b[sa[i]]!=b[sa[i-1]])
            t++;
        rank[sa[i]]=t;
    }
    return(t);
}
int b[N],c[N];
void build()
{
    a[n+1]=-1;
    for (int i=1;i<=n;i++)
    {
        c[i]=a[i];
        b[i]=-1;
        sa[i]=i;
    }
    sort(c);
    count(b,c);
    for (int k=1;;k<<=1)
    {
        for (int i=1;i<=n;i++)
        {
            c[i]=rank[i];
            b[i]=i+k<=n?rank[i+k]:0;
        }
        sort(b),sort(c);
        if (count(b,c)>=n)
            break;
    }
    int k=0;
    for (int i=1;i<=n;i++)
    {
        k=k?k-1:0;
        if (rank[i]==1)
        {
            height[rank[i]]=0;
            continue;
        }
        int p=sa[rank[i]-1],q=sa[rank[i]];
        while (a[p+k]==a[q+k])
            k++;
        height[rank[i]]=k;
    }
}
int ansl,ansr,log[N],st[18][N];
ull ans,sum[N];
void calc(int l,int r)
{
    if (l==r)
        return;
    int t=log[r-l],u=st[t][l+1],v=st[t][r-(1<<t)+1];
    int pos=height[u]<height[v]?u:v,value=height[pos];
    if (size[sa[l]]!=0)
    {
        int now=sum[sa[pos]+value-1]-sum[sa[pos]-1];
        if (ull(now)*(r-l+1)>ans)
        {
            ans=ull(now)*(r-l+1);
            ansl=position[sa[pos]];
            ansr=position[sa[pos]+value-1];
        }
    }
}
void getans()
{
    sum[0]=0;
    for (int i=1;i<=n;i++)
        sum[i]=sum[i-1]+size[i];
    log[1]=0;
    for (int i=2;i<=n;i++)
    {
        log[i]=log[i-1];
        if (i&i-1)
            continue;
        log[i]++;
    }
    for (int i=1;i<=n;i++)
        st[0][i]=i;
    for (int i=1;i<=log[n];i++)
        for (int j=1;j<=n;j++)
        {
            if (j+(1<<i)-1>n)
                break;
            int p=height[st[i-1][j]],q=height[st[i-1][j+(1<<i-1)]];
            st[i][j]=p<q?st[i-1][j]:st[i-1][j+(1<<i-1)];
        }
    calc(1,n);
    for (int i=1;i<=len;i++)
    {
        if (next[i]==-1)
            continue;
        int now=to[i]-i+1;
        if (now>ans)
        {
            ans=now;
            ansl=ansr=i;
        }
    }
}
int main()
{
    scanf("%s",s+1);
    len=strlen(s+1);
    init();
    build();
    getans();
    printf("%llu\n",ans);
    for (int i=ansl;i<=ansr;i++)
        for (int j=i;j<=next[i];j++)
            printf("%c",s[j]);
    printf("\n");
    return(0);
}

