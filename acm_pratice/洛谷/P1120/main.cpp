#include<bits/stdc++.h>
using namespace std;
const int MAX_N = 70;
int n, cnt = 0, num[MAX_N], l_now = 0, tot_len = 0;

const int LINK_MAX_LENGTH = MAX_N;
struct LINK {
    int head, next[LINK_MAX_LENGTH + 1],
            last[LINK_MAX_LENGTH + 1],
            node[LINK_MAX_LENGTH + 1];

    void init(int* data, int data_len) {
        head = 1;
        next[data_len]  = 0;
        for(int i = 1; i <= data_len; ++i) {
            if (i < data_len) {
                next[i] = i + 1;
            }
            last[i] = i - 1;
            node[i] = num[i - 1];
        }
    }

    // 删除id为ptr的node
    void delete_node(int ptr)
    {
        int last_node = last[ptr],
                next_node = next[ptr];
        next[last_node] = next_node;
        last[next_node] = last_node;
    }

    // 重新将指定id的节点加入id为pos的节点后
    void reinsert_node_after_node(int id, int pos, int data) {
        int ori_next = next[pos];
        node[id] = data;
        last[id] = pos;
        next[id]  = next[pos];
        next[pos] = id;
        last[ori_next] = id;
    }

    void print() {
        for(int i = head; ; i = next[i]) {
            cout << "[" << i << ":" << node[i] << "]";
            if(next[i]) {
                cout << "->";
            } else {
                break;
            }
        }
        cout << endl;
    }
}link_list;

int depth = 0;
bool search(int ptr) {
    cout << "search:(" << ptr << ")" << "l_now=" << l_now << " depth = "<< ++depth << endl;
    link_list.print();
    // getchar();
    bool res = false;
    for(; ptr && !res; ptr = link_list.next[ptr]) {
        int ptr_data = link_list.node[ptr];
        //cout << "ptr=" << ptr <<endl;
        if(link_list.node[ptr] == l_now) {
            if(!(link_list.next[ptr])) {
                --depth;
                return true;
            }
            continue;
        }

        for(int next_ptr = link_list.next[ptr]; next_ptr; next_ptr = link_list.next[next_ptr]) {
            //cout << "next_ptr=" << next_ptr <<endl;
            if(link_list.node[ptr] + link_list.node[next_ptr] <= l_now) {
                cout << "merge (" << ptr << " , " << next_ptr << ")" << endl;
                int next_ptr_data = link_list.node[next_ptr],
                        last_node = link_list.last[next_ptr];
                link_list.delete_node(next_ptr);
                link_list.node[ptr] = link_list.node[ptr] + link_list.node[next_ptr];
                res = search(ptr);
                if(res) {
                    break;
                }
                cout << "reinsert (" << ptr << " , " << next_ptr << ")" << endl;
                link_list.reinsert_node_after_node(next_ptr, last_node, next_ptr_data);
                link_list.node[ptr] = ptr_data;
                link_list.print();
            }
        }
        if(link_list.node[ptr] != l_now) {
            --depth;
            return false;
        }
    }
    --depth;
    return res;
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
            l_now = max(l_now, num[cnt]);
            tot_len += num[cnt];
            cnt++;
        }
    }
    link_list.init(num, cnt);
    while(!search(link_list.head)) {
        while(l_now < tot_len && tot_len % l_now != 0) {
            l_now++;
        }
        link_list.init(num, cnt);
    }
    cout << l_now << endl;
    return 0;
}