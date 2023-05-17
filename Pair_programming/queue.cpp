#include <iostream>
#include <deque>

using namespace std;

template <typename T> // type 자유롭게 쓸 수 있는 템플릿
class Queue
{
    public:
        auto push(const T& element) -> bool // 템플릿의 요소 주솟값(주소 -> 참조)으로 받아옴
        {   
            elements.push_back(element); // 참조로 받아왔으나 push_back에서 결국 복사됨
            return true;
        }

        auto pop() -> bool
        {   
            if (elements.size() == 0)
                throw out_of_range("Error in pop : size is 0");
            
            elements.pop_front();
            return true;
        }

        auto front() -> T
        {   
            if (elements.size() != 0)
                return elements.front();
           
            throw out_of_range("Error in top : size is 0");
        }

        auto empty() -> bool
        {   
            return elements.size() == 0;
        }

    private:
        deque<T> elements; // 템플릿을 이용한 객체
};


int main(int argc, char**argv)
{
    Queue<int> int_queue;; // 템플릿을 이용한 객체 선언

    assert(int_queue.push(3) == true);
    assert(int_queue.front() == 3);
    assert(int_queue.pop() == true);
    assert(int_queue.empty() == true);
}
