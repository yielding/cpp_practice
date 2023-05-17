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
            return true;
        }

        auto pop() -> bool
        {   
            if(elements.size() != 0)
            {
                elements.pop_front();
                return true;
            }
            else
                throw out_of_range("Error in pop : size is 0");
            
            return false;
        }

        auto front() -> T
        {   
            if(elements.size() != 0)
                return elements.front();
            else
                throw out_of_range("Error in top : size is 0");
        }

        auto empty() -> bool
        {   
            if(elements.size() == 0)
                return true;
            else
                return false;
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