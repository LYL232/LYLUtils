#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#ifndef LYL_UTILS_STRING_H
#define LYL_UTILS_STRING_H

#include <malloc.h>
#include <cstring>
#include <cstdio>
#include <vector>

/**
 * TODO:
 * append(const char* str, size_t len);
 */
class String {
public:
    String() {
        mem_apply();
        len = 0;
        str[0] = '\0';
    }

    /**
     * 复制直到遇到 \0才停止
     * @param c_str
     */
    explicit String(const char *c_str) {
        mem_apply();
        register size_t i = len = 0;
        char c;
        while ((c = c_str[i++]) != '\0') {
            _append(c);
        }
        _append('\0');
    }

    explicit String(char c) {
        mem_apply();
        len = 0;
        append(c);
    }

    String(const String &other) {
        mem_apply(mem_size = other.mem_size);
        memcpy(str, other.str, (other.len + 1) * sizeof(char));
        len = other.len;
    }

    /**
     * 不断从标准输入读字符,直到读到'\n', '\0' 为止,并追加 '\0'
     */
    void std_input() {
        char c;
        while ((c = (char) getchar()) != '\n' and c != '\0') {
            _append(c);
        }
        _append('\0');
    };

    std::vector<String> split(const String &deli) const {
        std::vector<String> res;
        if (empty()) {
            return res;
        }
        //先将要切割的字符串从string类型转换为char*类型
        auto *str_char = new char[len + 1]; //不要忘了
        strcpy(str_char, str);

        char *p = strtok(str_char, deli.c_str());
        while (p) {
            res.emplace_back(p); //存入结果数组
            p = strtok(nullptr, deli.c_str());
        }
        delete str_char;
        return res;
    }

    inline bool empty() const {
        return !len;
    }

    inline size_t length() const {
        return len;
    }

    /**
     *  追加一个字符c, 并自动再追加'\0'
     * @param c
     */
    inline void append(char c) {
        _append(c);
        _append('\0');
    }

    /**
     *  追加一个字符串str, 并自动再追加'\0'
     * @param str
     */
    void append(const char *str) {
        size_t i = 0;
        char c;
        while ((c = str[i++]) != '\0') {
            _append(c);
        }
        _append('\0');
    }

    /**
     *  追加一个字符串str, 并自动再追加'\0'
     * @param str
     */
    void append(const String &str) {
        const char *c_str = str.c_str();
        for(size_t i = 0; i < str.len; ++i) {
            _append(c_str[i]);
        }
        _append('\0');
    }

    /**
     * 预先申请内存, 原有内容不会保存
     * @param _size
     */
    inline void reserve(size_t _size) {
        free(str);
        // 最后附加一位 \0
        mem_apply(_size + 1);
    }

    /**
     * 返回C语言风格的字符串
     * @return
     */
    const char *c_str() const {
        return str;
    }

    bool operator==(const String &other) const {
        if (len == other.len) {
            return strcmp(str, other.str) == 0;
        }
        return false;
    }

    bool operator==(const char *c_str) const {
        return strcmp(str, c_str) == 0;
    }

    inline void clear() {
        len = 0;
    }

    /**
     * 链接字符串
     * @param other other字符串在后
     * @return String
     */
    String concat(const String &other) const {
        String res;
        res.reserve(len + other.len + 1);
        memcpy(res.str, str, len);
        memcpy(res.str + len, other.str, other.len);
        res.len = len + other.len;
        res._append('\0');
        return res;
    }

    /**
     * 链接字符串
     * @param other other字符串在后
     * @return String
     */
    String concat(const char *other) const {
        String res;
        register size_t i = 0;
        char c;
        while ((c = res.str[i++]) != '\0') {
            res._append(c);
        }
        res._append('\0');
        return res;
    }

    String strip() const {
        String res;
        if (len == 0) {
            return res;
        }
        res.reserve(len);
        register int ptr = 0;
        while (ptr < len && str[ptr] == ' ') {
            ptr++;
        }
        memcpy(res.str, str + ptr, res.len = len - ptr);
        while (res.len > 0 && res.str[res.len - 1] == ' ') {
            res.len--;
        }
        res.str[res.len] = '\0';
        return res;
    }

    /**
     * 删除字符串最后的几个字符，如果超出长度则会变为空字符串
     * @param rm_len
     */
    inline void erase_from_end(size_t rm_len) {
        if(len < rm_len) {
            len = 0;
        } else {
            len -= rm_len;
        }
        str[len] = '\0';
    }

    /**
     * 将字符串中存在的from字符换成 to字符
     * @param from
     * @param to
     */
    void replace(char from, char to) {
        for(register size_t i = 0; i < len; ++i) {
            if(str[i] == from) {
                str[i] = to;
            }
        }
    }

private:
    char *str;
    // len <= mem_size - 1 因为最后一位要有 /0
    size_t len, mem_size;

    /**
     * 申请内存, 并自动复制mem_size
     * @param _size
     */
    inline void mem_apply(size_t _size = default_size) {
        str = (char *) malloc((mem_size = _size) * sizeof(char));
    }


    /**
     * 内存膨胀: 新申请一块新的内存,大小是原来的inflate_factor倍, 复制原有内容到新内存中
     */
    inline void inflate() {
        auto new_str = (char *) malloc((mem_size *= inflate_factor) * sizeof(char));
        memcpy(new_str, str, len + 1);
        free(str);
        str = new_str;
    }

    /**
     * 往str后加入一个字符c, 当len将要大于mem_size - 1时,会自动膨胀,
     * 不会在后一位加上 /0
     * @param c 字符
     */
    inline void _append(char c) {
        if (len == mem_size - 1) {
            inflate();
        }
        str[len] = c;
        if (c != '\0') {
            len++;
        }
    }

    static size_t default_size;
    static float inflate_factor;
};

size_t String::default_size = 64;
float String::inflate_factor = 2.0f;
#endif //LYL_UTILS_STRING_H

#pragma clang diagnostic pop