#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
mutex mtx; // 互斥锁用于保护对结果的访问

// 计算1到5的和
void calculateSum1to5(int& result) {
    int sum = 0;
    for (int i = 1; i <= 5; ++i) {
        sum += i;
    }
    // 使用互斥锁保护对结果的访问
    lock_guard<mutex> lock(mtx);
    result = sum;
}

// 计算6到10的和
void calculateSum6to10(int& result) {
    int sum = 0;
    for (int i = 6; i <= 10; ++i) {
        sum += i;
    }
    // 使用互斥锁保护对结果的访问
    lock_guard<mutex> lock(mtx);
    result = sum;
}

int main() {
    int result1 = 0;
    int result2 = 0;

    // 创建两个线程，分别计算1到5和6到10的和
    thread thread1(calculateSum1to5, ref(result1));
    thread thread2(calculateSum6to10, ref(result2));
    //执行calculateSum6to10函数，并且将result2的引用传递给这个线程，以便在线程中修改result2的值。

    // 等待线程执行完毕
    thread1.join();
    thread2.join();


    cout<<"sum:"<<result1+result2<<endl;

    return 0;
}
