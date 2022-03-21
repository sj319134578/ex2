#include <iostream>

#include <queue>
#include <algorithm>
#include<fstream>
#include<iostream>


using namespace std;

#define MAXN 50

//问题表示
int n=3,W=30;
vector<int> w;//={0,16,15,15};		//重量，下标0不用
vector<int> v;//={0,45,25,25};  	//价值，下标0不用
//求解结果表示
int maxv=-9999;				//存放最大价值,初始为最小值
int bestx[MAXN];			//存放最优解,全局变量
int total=1;				//解空间中结点数累计,全局变量


/*  主干
 *  ->初始化根节点
 *  ->计算根节点上界及进队
 *  ->循环遍历队列，条件为非空：出一个节点,
       计算左孩子节点剪枝条件，满足的左孩子计算上界及进队；
       计算右孩子节点上界，符合上界条件的右孩子进队；
	   （根据容量剪去不满足要求的左孩子，根据上界条件剪去不满足要求的右孩子）
 *
*/
void bfs();
//  进队----不是叶子节点就直接进队，是叶子节点则判断是否更优解，是的话则更新最优解


// # 回溯法
//  使用n,W,w[],v[],maxv,bestv[]
//  main函数调用，用来初始化rw和op，以及dfs_back的入口
void bfs_back_main();

void dfs_back(int i,int tw,int tv,int rw,int op[]);

struct NodeType_Knap
{
	double w;
	double v;
	double p;					//p=v/w
	bool operator<(const NodeType_Knap &s) const
	{
		return p>s.p;			//按p递减排序
	}
};
vector<NodeType_Knap> A;		        //  含有输入的数据和排序后的数据
double V = 0;					//  价值，之前是int型，在这里为double
double x[MAXN];					//  最优解double类型，可以选择部分，即一定的比例
/*
 * 求单位重量的价值->按照自定义的格式排序->调用 Knap
*/
void knap_m();
/*
 * 排序后则贪心循环选择，如果剩余的容量还能容纳当前的，则放进去，不能的话跳出循环，选择部分放入
*/
void Knap();
// !# 贪心法

// # 斐波那契数列
int countf = 1;
int Fib(int n);
int dp_fib[MAXN];				//所有元素初始化为0
int Fib1(int n);
// !# 斐波那契数列


int dp[MAXN][MAXN];

void dp_Knap();
/*
 * 动态规划数组已经填充完毕，逆着推出最优解
   根据状态转移方程中的条件，判断每个物品是否选择
*/
void buildx();
// !# 动态规划法

int main()
{
	//  输入格式
	/*
		3      n个物品假设为3
		16 45  第一个物品的重量和价值
		15 25  第二个物品的重量和价值
		15 25  第三个物品的重量和价值
		30	   背包容量W
	*/
	using namespace std;

	fstream f;
	//文件读取，把原来的ios::out改成ios::in
	f.open("data.txt",ios::in);
	string s;
	//一直读到文件末尾
	while(f>>s)
	cout<<s<<endl; //显示读取内容
	f.close();


	//cin >> n;
	int m,l;
	//  下表0不用，填充0
	w.push_back(0);
	v.push_back(0);
	int j;
	for (j = 1; j <= n;j++)
	{
		cin >> m >> l;
		w.push_back(m);
		v.push_back(l);
	}
	cin >> W;


	dp_Knap();
	buildx();
	// !# 动态规划法

	cout << "最优解：";
	for (int i = 1;i <= n;i++)
	{
		if (V > 0)
		{// 贪心法   输出的是double类型
			cout << x[i] << " ";
		}else
		{//  回溯法输出的是int型
			cout << bestx[i] << " ";
		}

	}
	if (V > 0)
	{// 贪心法   输出的是double类型
		cout << endl << "最大价值为：" << V << endl;
	}else
	{//  回溯法输出的是int型
		cout << endl << "最大价值为：" << maxv << endl;
	}

	return 0;
}
//

//  回溯法
void bfs_back_main()
{
	int *op  = new int[n];
	for (int j = 0;j < n;j++)
	{//  初始化为全0
		op[j] = 0;
	}
	//  所有物品的总容量
	int rw = 0;
	for (int j = 0;j < n;j++)
	{
		rw += w[j];
	}
	dfs_back(1,0,0,rw,op);
}
//求解0/1背包问题
void dfs_back(int i,int tw,int tv,int rw,int op[])
{  //初始调用时rw为所有物品重量和
	int j;
	if (i>n)				        //找到一个叶子结点
	{
		if (tw==W && tv>maxv) 		        //找到一个满足条件的更优解,保存
		{
			maxv=tv;
			for (j=1;j<=n;j++)		//复制最优解
				bestx[j]=op[j];
		}
	}else
	{				                //尚未找完所有物品
		if (tw+w[i]<=W)			        //左孩子结点剪枝
		{
			op[i]=1;			//选取第i个物品
			dfs_back(i+1,tw+w[i],tv+v[i],rw-w[i],op);
		}
		op[i]=0;				//不选取第i个物品,回溯
		if (tw+rw>W)			        //右孩子结点剪枝
			dfs_back(i+1,tw,tv,rw-w[i],op);
	}
}
//
//  贪心法
void knap_m()
{

	for (int i=0;i<=n;i++)
	{
		NodeType_Knap k;
		k.w = w[i];
		k.v = v[i];
		A.push_back(k);
	}

	for (int i=1;i<=n;i++)		//求v/w
		A[i].p=A[i].v/A[i].w;

	sort(++A.begin(),A.end());			//A[1..n]排序

	Knap();

}
//  求解背包问题并返回总价值
void Knap()
{
	V=0;						//V初始化为0
	double weight=W;				//背包中能装入的余下重量

	int i=1;
	while (A[i].w < weight)			        //物品i能够全部装入时循环
	{
		x[i]=1;					//装入物品i
		weight -= A[i].w;			//减少背包中能装入的余下重量
		V += A[i].v;				//累计总价值
		i++;					//继续循环
	}
	if (weight > 0)					//当余下重量大于0
	{
		x[i] = weight / A[i].w;		        //将物品i的一部分装入
		V += x[i] * A[i].v;			//累计总价值
	}

}
//
//  斐波那契数列
int Fib(int n)
{
	printf("(%d)求解Fib(%d)\n",countf++,n);
	if (n==1 || n==2)
	{
		printf("   计算出Fib(%d)=%d\n",n,1);
		return 1;
	}
	else
	{
		int x = Fib(n-1);
		int y = Fib(n-2);
		printf("   计算出Fib(%d)=Fib(%d)+Fib(%d)=%d\n",
		n,n-1,n-2,x+y);
		return x+y;
	}

}
//  动态规划后的斐波那契数列
int Fib1(int n)			//算法1
{
	dp_fib[1]=dp_fib[2]=1;
	printf("(%d)计算出Fib(1)=1\n",countf++);
	printf("(%d)计算出Fib(2)=1\n",countf++);
	for (int i=3;i<=n;i++)
	{
		dp_fib[i]=dp_fib[i-1]+dp_fib[i-2];
		printf("(%d)计算出Fib(%d)=%d\n",countf++,i,dp_fib[i]);
	}
	return dp_fib[n];
}

//
//  动态规划法
void dp_Knap()
{
	int i,r;
	for(i = 0;i <= n;i++)		//置边界条件dp[i][0]=0
		dp[i][0] = 0;
	for (r = 0;r <= W;r++)		//置边界条件dp[0][r]=0
		dp[0][r] = 0;
	for (i = 1;i <= n;i++)
	{
		for (r = 1;r <= W;r++)
			if (r < w[i])
				dp[i][r] = dp[i-1][r];
			else
				dp[i][r] = max(dp[i-1][r],dp[i-1][r-w[i]]+v[i]);
	}

}
void buildx()
{
	int i=n,r=W;
	maxv=0;
	while (i>=0)				//判断每个物品
	{
		if (dp[i][r] != dp[i-1][r])
		{
			bestx[i] = 1;		//选取物品i
			maxv += v[i];		//累计总价值
			r = r - w[i];
		}
		else
			bestx[i]=0;		//不选取物品i
		i--;
	}

}
