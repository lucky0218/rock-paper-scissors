#include "functions.h"

using namespace std;
using namespace cv;

typedef pair<int, int> P;

vector <P> rec;
int x[] = { 1,0,-1,0 };
int y[] = { 0,1,0,-1 };
int mp[120][120],vis[120][120], threshold1=25;

void dfs_1(int i,int j) {
	vis[i][j] = 1;
	rec.push_back(make_pair(i, j));
	
	for (int k = 0; k < 3; k++) {
		if (i + y[k] >= 0 && j + x[k] >= 0 && mp[i + y[k]][j + x[k]]&&!vis[i + y[k]][j + x[k]]) dfs_1(i + y[k], j + x[k]);
	}
	return;
}
void noise_process(Mat& img) {
	memset(mp, 0, sizeof(mp));
	memset(vis, 0, sizeof(vis));
	int height = img.rows, width = img.cols;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			mp[i][j] = img.at<uchar>(i,j) > 0;

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) 
			if (!vis[i][j] && mp[i][j]) {
				rec.clear();
				dfs_1(i, j);
				if (rec.size() <= threshold1) {
					for (auto k : rec) mp[k.first][k.second]=0,img.at<uchar>(k.first, k.second) = 0;
				}
			}
	memset(vis, 0, sizeof(vis));
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			mp[i][j] ^= 1;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			if (!vis[i][j] && mp[i][j]) {
				rec.clear();
				dfs_1(i, j);
				if (rec.size() <= threshold1) {
					for (auto k : rec) img.at<uchar>(k.first, k.second) =255;
				}
			}
		}
	return;

}