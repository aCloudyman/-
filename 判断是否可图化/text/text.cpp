#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>

using namespace std;

//检查序列是否为偶数
bool isValidSequence(const vector<int>& sequence) {
    int sum = accumulate(sequence.begin(), sequence.end(), 0);
    return sum % 2 == 0;  // 和必须为偶数
}

//验证是否满足Havel定理
bool isGraphicalHavel(vector<int> sequence) {
    while (!sequence.empty()) {
        sort(sequence.rbegin(), sequence.rend());  // 按降序排列
        if (sequence[0] < 0) return false;  // 不允许负值
        int maxDegree = sequence[0];
        sequence.erase(sequence.begin());  // 移除最大度
        if (maxDegree > sequence.size()) return false;  // 不足以分配
        for (int i = 0; i < maxDegree; ++i) {
            sequence[i]--;  // 分配度数
            if (sequence[i] < 0) return false;  // 不允许负值
        }
    }
    return true;
}


//验证是否满足埃尔德什定理
bool isGraphicalErdos(vector<int> sequence) {
    sort(sequence.rbegin(), sequence.rend());
    int n = sequence.size();
    vector<int> prefixSum(n + 1, 0);

    for (int i = 1; i <= n; ++i)
        prefixSum[i] = prefixSum[i - 1] + sequence[i - 1];

    for (int k = 1; k <= n; ++k) {
        int rightSum = k * (k - 1);
        for (int i = k; i < n; ++i)
            rightSum += min(k, sequence[i]);

        if (prefixSum[k] > rightSum)
            return false;  // 不满足不等式
    }
    return true;
}


//生成邻接矩阵
vector<vector<int>> generateAdjacencyMatrix(vector<int> sequence) {
    int n = sequence.size();
    vector<vector<int>> adjacencyMatrix(n, vector<int>(n, 0));

    while (!sequence.empty()) {
        sort(sequence.rbegin(), sequence.rend());
        int maxDegree = sequence[0];
        sequence.erase(sequence.begin());
        for (int i = 0; i < maxDegree; ++i) {
            adjacencyMatrix[0][i + 1] = 1;
            adjacencyMatrix[i + 1][0] = 1;
            sequence[i]--;
        }
        sequence.erase(remove(sequence.begin(), sequence.end(), 0), sequence.end());
    }
    return adjacencyMatrix;
}


//判断图的连通性
bool isConnected(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    vector<bool> visited(n, false);
    queue<int> q;

    q.push(0);
    visited[0] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int i = 0; i < n; ++i) {
            if (matrix[node][i] == 1 && !visited[i]) {
                visited[i] = true;
                q.push(i);
            }
        }
    }
    return all_of(visited.begin(), visited.end(), [](bool v) { return v; });
}


// 主程序
int main() {
    vector<int> sequence;
    int n;
    // 用户输入序列
    cout << "请输入序列的长度：";
    cin >> n;
    if (n <= 0) {
        cout << "序列长度必须为正整数。" << endl;
        return 0;
    }
    cout << "请输入序列的元素（以空格分隔）：" << endl;
    for (int i = 0; i < n; ++i) {
        int num;
        cin >> num;
        if (num < 0) {
            cout << "序列元素必须为非负整数。" << endl;
            return 0;
        }
        sequence.push_back(num);
    }
    // 验证序列是否符合基本条件
    if (!isValidSequence(sequence)) {
        cout << "序列不可图化（和不是偶数）。" << endl;
        return 0;
    }
    // Havel 定理判断
    if (isGraphicalHavel(sequence)) {
        cout << "根据 Havel 定理，序列是可图化的。" << endl;
    }
    else {
        cout << "根据 Havel 定理，序列不可图化。" << endl;
        return 0;
    }
    // 埃尔德什定理判断
    if (isGraphicalErdos(sequence)) {
        cout << "根据埃尔德什定理，序列是可简单图化的。" << endl;
    }
    else {
        cout << "根据埃尔德什定理，序列不可简单图化。" << endl;
        return 0;
    }
    // 邻接矩阵生成
    auto adjacencyMatrix = generateAdjacencyMatrix(sequence);
    cout << "邻接矩阵：" << endl;
    for (const auto& row : adjacencyMatrix) {
        for (int val : row) cout << val << " ";
        cout << endl;
    }
    // 连通性判断
    if (isConnected(adjacencyMatrix)) {
        cout << "图是连通的。" << endl;
    }
    else {
        cout << "图不是连通的。" << endl;
    }
    return 0;
}