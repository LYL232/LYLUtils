#include<bits/stdc++.h>
using namespace std;
const int MAX_N = 70;
int n, cnt = 0, num[MAX_N], l_now = 0, tot_len = 0;

const int LINK_MAX_LENGTH = MAX_N;
struct LINK {
    int head, next[LINK_MAX_LENGTH + 1],
            last[LINK_MAX_LENGTH + 1],
            node[LINK_MAX_LENGTH + 1],
            num[LINK_MAX_LENGTH + 1];

    void init(const int* data, int data_len) {
        head = 1;
        int nodes = 0, last_data = -1, next_node = 1;
        last[0] = 0;
        for(int i = 1; i <= data_len; ++i) {
                if(data[i] == last_data) {
                    num[nodes]++;
                } else {
                    last[next_node] = nodes;
                    next[nodes] = next_node;
                    num[next_node] = 1;
                    next[next_node] = 0;
                    node[next_node] = data[i];
                    ++nodes;
                }
        }
    }

    // 删除id为ptr的node
    void delete_node(int ptr)
    {
        if(num[ptr] > 1) {
            int last_node = last[ptr],
                    next_node = next[ptr];
            next[last_node] = next_node;
            last[next_node] = last_node;
        } else {
            num[ptr]--;
        }
    }

    // 在pos的节点插入值为data的节点,如果值
    void reinsert_node_after_node(int pos, int data) {
        int ori_next = next[pos];
        node[id] = data;
        last[id] = pos;
        next[id]  = next[pos];
        next[pos] = id;
        last[ori_next] = id;
    }

}link_list;

int continue_count_1 = 0, continue_count_2 = 0;
bool search(int ptr) {
    int last_len = -1;
    for(; ptr; ptr = link_list.next[ptr]) {
        int ptr_data = link_list.node[ptr];
        if(ptr_data == l_now) {
            if(!(link_list.next[ptr])) {
                return true;
            }
            continue_count_1++;
            continue;
        }

        for(int next_ptr = link_list.next[ptr]; next_ptr; next_ptr = link_list.next[next_ptr]) {
            if(last_len == link_list.node[next_ptr]) {
                continue_count_2++;
                continue;
            }
            last_len = link_list.node[next_ptr];
            if(link_list.node[ptr] + link_list.node[next_ptr] <= l_now) {
                int next_ptr_data = link_list.node[next_ptr],
                        last_node = link_list.last[next_ptr],
                                ptr_len = link_list.node[ptr] + link_list.node[next_ptr];
                link_list.delete_node(next_ptr);
                link_list.node[ptr] = ptr_len;
                if(search(ptr)) {
                    return true;
                }
                // 到达这个分支说明这一搜索失败了
                link_list.reinsert_node_after_node(next_ptr, last_node, next_ptr_data);
                link_list.node[ptr] = ptr_data;
                if(ptr_len == l_now) {
                    //如果当前ptr和next_ptr的长度加起来正好等于当前的枚举长度,由于是从大搜索到小的,
                    // 由端木棍灵活性可知,当前用这根木棍是最长的满足条件的最优木棍,如果到达这里,
                    // 说明所用木根最优仍失败,则是之前的木棍用法有问题,回溯即可
                    return false;
                }
            }
        }
        if(link_list.node[ptr] != l_now) {
            return false;
        }
    }
    return false;
}

bool cmp(int a, int b) {
    return a > b;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie();
    cout.tie();
    cin >> n;
    for(int i = 0; i < n; ++i) {
        cin >> num[cnt];
        if (num[cnt] <= 50) {
            // 由题意,大于50的过滤掉
            l_now = max(l_now, num[cnt]);
            tot_len += num[cnt];
            cnt++;
        }
    }
    //排序:由大到小遍历,因为更长的木根灵活性更差
   // 从灵活性最差的开始,可以减少不必要的搜索
    sort(num, num + cnt, cmp);
    link_list.init(num, cnt);
    while(true) {
        while(l_now < tot_len && tot_len % l_now != 0) {
            l_now++;
            if(l_now > tot_len) {
                cout << tot_len << endl;
                return 0;
            }
        }
        link_list.init(num, cnt);
        if(search(link_list.head)) {
            break;
        }
        l_now++;
    }
    cout << l_now << endl;
    cout << continue_count_1 << endl;
    cout << continue_count_2 << endl;
    return 0;
}