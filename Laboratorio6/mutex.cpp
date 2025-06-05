#include <mutex>
std::mutex mtx;

void safeIncrement(int &counter){
    std::lock_guard<std::mutex> lock(mtx);
    ++counter;
    //mtx se libera automáticamente cuando lock sale del ambito
}

int main (){
    int counter = 0;
    safeIncrement(counter);
    return 0;
}