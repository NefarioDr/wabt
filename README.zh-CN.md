# TOPIC
本项目在wabt的基础上，封装了eos的contract运行时环境。

## 编译及使用
1. 按[README.md](README.md)描述编译wabt，编译成功之后，会在build目录下生成诸多工具，其中wasm-runner是我们的目标工具。
2. 安装[eosio.cdt](https://github.com/EOSIO/eosio.cdt).
3. 编写如下c++的源文件test.cpp.
```cpp
extern "C" {
    int apply(int x) {
        return x*x ;
    }
}
```
4. 使用eosio-cpp工具(步骤2中编译产生)，编译test.cpp文件：
```bash
> eosio-cpp test.cpp -o test.wasm
```
编译成功之后，会在test.cpp的同一个路径下，生成test.wasm文件。

5. 使用wasm-runner工具(步骤1中编译产生)，运行test.wasm中的apply方法：
```bash
> wasm-runner test.wasm apply -p 9
```
运行成功的话，会产生如下输出：
```bash
> apply(i32:9) => i32:81
```
