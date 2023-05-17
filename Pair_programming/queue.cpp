#include <iostream>
#include <deque>

using namespace std;

template <typename T> // type 자유롭게 쓸 수 있는 템플릿
class Queue
{
    public:
        auto push(const T& element) -> bool // 템플릿의 요소 주솟값으로 받아옴
        {   
                elements.push_back(element);
            return 1;
        }

    private:
        deque<T> elements; // 템플릿을 이용한 객체
};


int main(int argc, char**argv)
{
    Queue<int> int_queue;; // 템플릿을 이용한 객체 선언

    assert(int_queue.push(1),1);


}