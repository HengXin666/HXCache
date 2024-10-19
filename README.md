<h1 align="center" style="color:yellow">HXCache</h1>

实现了线程安全的LRU、LFU，只需要单头文件; 只依赖于STL, 支持原地构造和透明查找.

本项目是从 [HXNet](https://github.com/HengXin666/HXNet) 中分出来的(*并且特化支持C++20以下的内容*), 专门用于提供LRU、LFU等Cache.

## 构建要求

- 最低 C++11 (因为STL没有读写锁, 所以使用`std::mutex`)

- C++14 此时支持`透明查找`(`contains`) (因为STL没有纯的读写锁, 所以使用`std::shared_timed_mutex`, 不过我相信在不使用定时的情况下, 性能损耗为 0)

- C++17 (性能完全体, 使用`std::shared_mutex`)

## 示例

- [LRUCache/LFUCache的示例](examples/CacheTest.cpp)

下面是一个简单的示例, LFU和LRU的的API是一样的.

```C++
#include <HXCache/LFUCache.hpp>

int main() {
    HX::ThreadSafeLFUCache<int, std::string> cache(2); // capacity = 2

    cache.insert(123, "This 123~");
    cache.emplace(456, "This 456~"); // 原地构造

    const std::string& str = cache.get(123);

    std::cout << str << '\n'; // This 123~

    std::cout << "cache size = " << cache.size() << '\n'; // 2
    cache.clear();

    std::cout << "cache size = " << cache.size() << '\n'; // 0
    return 0;
}
```