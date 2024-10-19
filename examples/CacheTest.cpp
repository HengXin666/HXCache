#ifdef CACHE_TEST_MAIN
#include <memory>

#include <HXCache/LRUCache.hpp>
#include <HXCache/LFUCache.hpp>

struct Node {
    int a;
    int b;
    mutable std::shared_ptr<int> _ptr;
 
    Node(int _a, int _b)
        : a(_a)
        , b(_b)
    {
        printf("new: %d, %d\n", a, b);
    }

    Node(const Node& node)
        : Node(node.a, node.b)
    {
        printf("copy: %d, %d\n", a, b);
    }

    void print() const {
        printf("print: %d %d\n", a, b);
    }

    Node& operator=(const Node& other) {
        if (this != &other) {
            // 拷贝成员变量
            this->a = std::move(other.a);
            this->b = std::move(other.b);
        }
        printf("operator=(const Node&)\n");
        return *this;
    }

    Node& operator=(Node&&) = delete;
    // Node& operator=(const Node&) = delete;

    ~Node() {
        printf("~Node: %d %d\n", a, b);
    }
};

void LRUCacheTest() {
    HX::LRUCache<int, Node> cache = []() -> HX::LRUCache<int, Node> {
        HX::LRUCache<int, Node> tmp(1);
        tmp.emplace(2233, 114514, 0721);
        return tmp; // 可以作为返回值 (编译器自动实现 && 转化)
    }();
    cache.emplace(1, 1, 2);
    auto&& item = cache.get(1);
    item._ptr = std::make_shared<int>(); // 使用智能指针
    item.print();
    cache.emplace(1, 4, 5); // 原地构造会释放指针
    cache.insert(2, {8, 9});
    printf("size = %lu\n", cache.size());

    // 线程安全的LRUCache
    HX::ThreadSafeLRUCache<std::string, std::string> ts(3);
    ts.emplace("abc", "char * -> string");
    printf("ts.get(\"abc\"): %s\n", ts.get("abc").c_str());

    // 透明查找的示例: 比如允许通过 const char* 查找 std::string 的键
    // 这使得用户可以在调用 contains 时, 不必显式地转换键的类型
    printf("ts中是否存在 [abc]: %d\n", ts.contains("abc"));

    // HX::ThreadSafeLRUCache<int, Node> tts(std::move(cache));
    // tts.get(2);
}

void LFUCacheTest() {
    HX::LFUCache<int, std::string> cache(1);
    cache.insert(2, "nb 666");
    printf("cache.get(2): %s\n", cache.get(2).c_str());
    cache.emplace(2, "nb 777");
    printf("cache.get(2): %s\n", cache.get(2).c_str());
    cache.emplace(2233, "nb Heng_Xin!!");
    printf("cache.get(2233): %s\n", cache.get(2233).c_str());

    // 支持从普通线程不安全的LFUCache移动构造出线程安全的ThreadSafeLFUCache
    HX::ThreadSafeLFUCache<int, std::string> ts(std::move(cache));
    printf("size = %lu\n", ts.size());
    printf("cache.get(2233): %s\n", ts.get(2233).c_str());
    ts.emplace(114514, "oh my god!");
    printf("cache.get(114514): %s\n", ts.get(114514).c_str());
    printf("cache.get(114514): %s\n", ts.get(114514).c_str());
    ts.clear();
    printf("ts.clear() => size = %lu\n", ts.size());
}

int main() {
    LRUCacheTest();
    printf("\n===============\n\n");
    LFUCacheTest();
    return 0;
}

#endif // CACHE_TEST_MAIN