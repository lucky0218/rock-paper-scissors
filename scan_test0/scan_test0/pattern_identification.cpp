#include "functions.h"

using namespace std;
using namespace cv;

const int N = 130;


vector <P> rec[N*N],rec_1,skin;
P begin_point, end_point;
int begin_x, begin_y, end_x, end_y;
int x[] = { 1,0,-1,0 };
int y[] = { 0,1,0,-1 };
int mp[N][N],vis[N][N], threshold1=25;

void dfs_0(int i,int j,int cnt) {
	vis[i][j] = 1;
	rec[cnt].push_back(make_pair(i, j));
	for (int k = 0; k < 4; k++) {
		if (i + y[k] >= 0 && j + x[k] >= 0 && mp[i + y[k]][j + x[k]]&&!vis[i + y[k]][j + x[k]]) dfs_0(i + y[k], j + x[k],cnt);
	}
	return;
}
//dfs_0找寻连通块并记录
void dfs_1(int i, int j) {
	vis[i][j] = 1;
	rec_1.push_back(make_pair(i, j));

	for (int k = 0; k < 4; k++) {
		if (i + y[k] >= 0 && j + x[k] >= 0 && mp[i + y[k]][j + x[k]] && !vis[i + y[k]][j + x[k]]) dfs_1(i + y[k], j + x[k]);
	}
	return;
}
vector<P> noise_process(Mat& img) {
	int cnt = 0,maxn=0,pos=0;
	memset(mp, 0, sizeof(mp));
	memset(vis, 0, sizeof(vis));
	for (int i = 0; !rec[i].empty(); i++) rec[i].clear();
	int height = img.rows, width = img.cols;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			mp[i][j] = img.at<uchar>(i,j) > 0;

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) 
			if (!vis[i][j] && mp[i][j]) {
				rec[cnt].clear();
				dfs_0(i, j,cnt);
				cnt++;
			}
	memset(mp, 0, sizeof(mp));
	img=Mat::zeros(img.size(), img.type());
	for (int i = 0; !rec[i].empty(); i++)
		if (rec[i].size() > maxn) maxn = rec[i].size(), pos = i;
	for (auto k : rec[pos]) {
		skin.push_back(k);
		//printf("%d %d\n", k.first, k.second);
		mp[k.first][k.second] = 1, img.at<uchar>(k.first, k.second) = 255;
	}
/*	memset(vis, 0, sizeof(vis));
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			mp[i][j] ^= 1;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			if (!vis[i][j] && mp[i][j]) {
				rec_1.clear();
				dfs_1(i, j);
				if (rec_1.size() <= threshold1) {
					for (auto k : rec_1) img.at<uchar>(k.first, k.second) =255;
				}
			}
		}*/
	return skin;
}

double add(double a, double b) {
	if (fabs(a + b) < eps * (fabs(a) + fabs(b))) return 0;
	return a + b;
}
bool cmp_x(const point& p, const point& q) {
	if (p.x != q.x) return p.x < q.x;
	return p.y < q.y;
}
vector<point> convex_hull(point* ps, int n) {
	sort(ps, ps + n, cmp_x);
	int k = 0;
	vector<point> qs(2 * n);
	for (int i = 0; i < n; i++) {
		while (k > 1 && (qs[k - 1] - qs[k - 2]).det(ps[i] - qs[k - 1]) <= 0) k--;
		qs[k++] = ps[i];
	}
	for (int i = n - 2, t = k; i >= 0; i--) {
		while (k > t && (qs[k - 1] - qs[k - 2]).det(ps[i] - qs[k - 1]) <= 0) k--;
		qs[k++] = ps[i];
	}
	qs.resize(k - 1);
	return qs;
}

int max_distance() {
	int n = skin.size();
	point* ps = new point[n];
	for (int i = 0; i < n; i++) ps[i].x=skin[i].second, ps[i].y=skin[i].first;
	vector<point>qs = convex_hull(ps, n);
	double res = 0;
	for (int i=0;i<qs.size();i++)
		for (int j = 0; j < i; j++) {
			double dis = (qs[i] - qs[j]).dot(qs[i] - qs[j]);
			if (res < dis) {
				res = dis; 
				begin_x = qs[i].x, begin_y = qs[i].y;
				end_x = qs[j].x, end_y = qs[j].y;
			}
		}
	return 	res;
}

double ave_distance_begin(int y,int x) {
	y = begin_y, x = begin_x;
	double ave = 0;
	for (auto k : skin) ave += abs(k.first - y) * abs(k.first - y) + abs(k.second - x) * abs(k.second - x);
	ave /=(double) skin.size();
	return ave;
}
double ave_distance_end(int y, int x) {
	y = end_y, x = end_x;
	double ave = 0;
	for (auto k : skin) ave += abs(k.first - y) * abs(k.first - y) + abs(k.second - x) * abs(k.second - x);
	ave /= (double)skin.size();
	return ave;
}



