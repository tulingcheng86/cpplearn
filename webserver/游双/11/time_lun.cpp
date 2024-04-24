#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <memory>
#include <thread>
#include <chrono>

using namespace std;

// 定义定时任务结构体
struct TimerTask 
{
    function<void()> callback;  // 任务回调函数
    int expiration;             // 过期时间（轮数）
    int rotation;               // 还需转动多少轮触发
    int slot;                   // 时间轮上的槽位

    // 构造函数，初始化定时任务
    TimerTask(function<void()> cb, int exp, int rot, int sl)
        : callback(move(cb)), expiration(exp), rotation(rot), slot(sl) {}
};

// 时间轮类
class TimeWheel 
{
public:
    // 构造函数，初始化指定数量的槽
    TimeWheel(int num_slots) : slots(num_slots), current_slot(0) {
        for (int i = 0; i < num_slots; ++i) {
            slots[i] = list<shared_ptr<TimerTask>>();
        }
    }

    ~TimeWheel() {}

    // 添加定时任务
    void add_timer(function<void()> callback, int timeout) {
        if (timeout < 0) {
            return;
        }
        // 计算所需的ticks和完整的轮数
        int ticks = timeout / TICK_INTERVAL;
        int rotation = ticks / slots.size();
        int slot = (current_slot + (ticks % slots.size())) % slots.size();
        // 创建任务并添加到相应的槽中
        auto task = make_shared<TimerTask>(callback, ticks, rotation, slot);
        slots[slot].push_back(task);
    }

    // 推动时间轮向前移动
    void tick() {
        auto& bucket = slots[current_slot]; // 获取当前槽位的任务列表
        auto it = bucket.begin();
        while (it != bucket.end()) {
            auto task = *it;
            // 检查任务是否到期
            if (task->rotation > 0) {
                --task->rotation; // 减少旋转数
                ++it;
            } else {
                task->callback();  // 执行任务
                it = bucket.erase(it);  // 从槽中移除任务
            }
        }
        // 更新当前槽位，移到下一个槽位
        current_slot = (current_slot + 1) % slots.size();
    }

private:
    vector<list<shared_ptr<TimerTask>>> slots;  // 时间槽
    int current_slot;  // 当前槽位
    static const int TICK_INTERVAL = 1;  // 时间间隔单位，用于计算任务延时
};

int main() 
{
    TimeWheel tw(10);  // 创建一个有10个槽的时间轮

    // 添加一个简单的定时任务
    tw.add_timer([]() {
        cout << "Task 1 executed!" << endl;
    }, 5);  // 设置5个时间单位后执行

    // 模拟时间推进
    for (int i = 0; i < 15; i++) {
        tw.tick();  // 每次调用代表时间前进一个单位
        this_thread::sleep_for(chrono::seconds(1));  // 等待1秒
    }

    return 0;
}
