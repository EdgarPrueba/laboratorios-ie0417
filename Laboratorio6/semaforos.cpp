#include <semaphore>

std::counting_semaphore<1> sem(1);

void accessResource(){
    sem.acquire();
    sem.release();
}
int main() {
    accessResource();
    return 0;
}