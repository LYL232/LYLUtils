// 离散化函数, 将src数组中的数字离散化至res中,要求src != res
// 最小值映射到 1 最大值映射到序列值的不同值的种数
// 返回序列中不相等的值的种数
const int MAX_DISCRETIZE_LEN = MAX_N;
int _discretize_temp[MAX_DISCRETIZE_LEN];

int discretize(const int *_src, int *_res, int _len) {
    memcpy(_discretize_temp, _src, _len * sizeof(int));
    sort(_discretize_temp, _discretize_temp + _len);
    auto _cnt = (int) (unique(_discretize_temp, _discretize_temp + _len) - _discretize_temp);
    for (int i = 0; i < _len; ++i) {
        _res[i] = (int) (lower_bound(_discretize_temp, _discretize_temp + _cnt, _src[i]) - _discretize_temp) + 1;
    }
    return _cnt;
}