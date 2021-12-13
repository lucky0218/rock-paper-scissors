#include "functions.h"

using namespace std;
using namespace cv;

const int N = 130;
typedef pair<int, int> P;

vector <P> rec[N*N],rec_1;
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
void dfs_1(int i, int j) {
	vis[i][j] = 1;
	rec_1.push_back(make_pair(i, j));

	for (int k = 0; k < 4; k++) {
		if (i + y[k] >= 0 && j + x[k] >= 0 && mp[i + y[k]][j + x[k]] && !vis[i + y[k]][j + x[k]]) dfs_1(i + y[k], j + x[k]);
	}
	return;
}
void noise_process(Mat& img) {
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
		printf("%d %d\n", k.first, k.second);
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
	return;

}