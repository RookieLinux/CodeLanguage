//
// Created by rookie on 23-12-11.
//
#include <iostream>
#include <boost/timer/timer.hpp>
#include <boost/thread.hpp>

using namespace  std;
using namespace  boost;
using namespace  boost::timer;

void wait(int seconds)
{
    boost::this_thread::sleep(boost::posix_time::seconds(seconds));
}

boost::mutex mex;

void thr()
{
    for (int i = 0; i < 5; ++i)
    {
        wait(1);
        boost::lock_guard<boost::mutex> lock(mex); //构造时加锁 析构时减锁 RAII封装
        std::cout << "Thread " << boost::this_thread::get_id() << ": " << i <<std::endl;
    }
}

boost::timed_mutex mex2;

void thr2()
{
    for (int i = 0; i < 5; ++i)
    {
        wait(1);
        boost::unique_lock<boost::timed_mutex> lock(mex2, boost::try_to_lock);
        if (!lock.owns_lock())
            lock.timed_lock(boost::get_system_time() + boost::posix_time::seconds(1));
        std::cout << "Thread " << boost::this_thread::get_id() << ": " << i << std::endl;
        boost::timed_mutex *m = lock.release();
        m->unlock();
    }
    // 就像 boost::lock_guard 一样， boost::unique_lock 的析构函数也会相应地释放互斥量。此外，可以手动地用 unlock() 释放互斥量。
    // 也可以通过调用 release() 解除boost::unique_lock 和互斥量之间的关联。然而在这种情况下，必须显式地调用 unlock() 方法来释放互斥量，
    // 因为 boost::unique_lock 的析构函数不再做这件事情。
}

int main()
{
    std::cout << "lock_guard" << std::endl;
    boost::thread t1(thr);
    boost::thread t2(thr);
    t1.join();
    t2.join();
    getchar();

    std::cout << "unique_lock" << std::endl;
    boost::thread t3(thr2);
    boost::thread t4(thr2);
    t3.join();
    t4.join();
    getchar();

    return 0;
}