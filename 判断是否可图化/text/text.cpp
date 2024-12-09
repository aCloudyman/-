#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>

using namespace std;

//��������Ƿ�Ϊż��
bool isValidSequence(const vector<int>& sequence) {
    int sum = accumulate(sequence.begin(), sequence.end(), 0);
    return sum % 2 == 0;  // �ͱ���Ϊż��
}

//��֤�Ƿ�����Havel����
bool isGraphicalHavel(vector<int> sequence) {
    while (!sequence.empty()) {
        sort(sequence.rbegin(), sequence.rend());  // ����������
        if (sequence[0] < 0) return false;  // ������ֵ
        int maxDegree = sequence[0];
        sequence.erase(sequence.begin());  // �Ƴ�����
        if (maxDegree > sequence.size()) return false;  // �����Է���
        for (int i = 0; i < maxDegree; ++i) {
            sequence[i]--;  // �������
            if (sequence[i] < 0) return false;  // ������ֵ
        }
    }
    return true;
}


//��֤�Ƿ����㰣����ʲ����
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
            return false;  // �����㲻��ʽ
    }
    return true;
}


//�����ڽӾ���
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


//�ж�ͼ����ͨ��
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


// ������
int main() {
    vector<int> sequence;
    int n;
    // �û���������
    cout << "���������еĳ��ȣ�";
    cin >> n;
    if (n <= 0) {
        cout << "���г��ȱ���Ϊ��������" << endl;
        return 0;
    }
    cout << "���������е�Ԫ�أ��Կո�ָ�����" << endl;
    for (int i = 0; i < n; ++i) {
        int num;
        cin >> num;
        if (num < 0) {
            cout << "����Ԫ�ر���Ϊ�Ǹ�������" << endl;
            return 0;
        }
        sequence.push_back(num);
    }
    // ��֤�����Ƿ���ϻ�������
    if (!isValidSequence(sequence)) {
        cout << "���в���ͼ�����Ͳ���ż������" << endl;
        return 0;
    }
    // Havel �����ж�
    if (isGraphicalHavel(sequence)) {
        cout << "���� Havel ���������ǿ�ͼ���ġ�" << endl;
    }
    else {
        cout << "���� Havel �������в���ͼ����" << endl;
        return 0;
    }
    // ������ʲ�����ж�
    if (isGraphicalErdos(sequence)) {
        cout << "���ݰ�����ʲ���������ǿɼ�ͼ���ġ�" << endl;
    }
    else {
        cout << "���ݰ�����ʲ�������в��ɼ�ͼ����" << endl;
        return 0;
    }
    // �ڽӾ�������
    auto adjacencyMatrix = generateAdjacencyMatrix(sequence);
    cout << "�ڽӾ���" << endl;
    for (const auto& row : adjacencyMatrix) {
        for (int val : row) cout << val << " ";
        cout << endl;
    }
    // ��ͨ���ж�
    if (isConnected(adjacencyMatrix)) {
        cout << "ͼ����ͨ�ġ�" << endl;
    }
    else {
        cout << "ͼ������ͨ�ġ�" << endl;
    }
    return 0;
}