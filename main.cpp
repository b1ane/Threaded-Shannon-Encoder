#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>

struct Data {
    int id;
    std::string line;
    //std::vector<std::string> lines; //all input lines
    pthread_mutex_t *mutex1;
    pthread_mutex_t *mutex2;
    pthread_cond_t *condition;
    int *counter;

};

void *output (void *void_ptr) {
    Data *data = (Data*) void_ptr;
    pthread_mutex_lock(data->mutex1);

    int localId = data->id;
    std::string localLine = data->line;

    pthread_mutex_unlock(data->mutex1);

    pthread_mutex_lock(data->mutex2);

    while (*data->counter != localId) {
        pthread_cond_wait(data->condition, data->mutex2);
    }

    std::cout << "Thread " << localId << ": " << localLine << std::endl;

    (*data->counter)++;
    pthread_cond_broadcast(data->condition);

    pthread_mutex_unlock(data->mutex2);  

    return nullptr;
}

int main() {
    std::string line;
    std::vector<std::string> lines;

    while(std::getline(std::cin, line)) {
        if(line.empty()) {
            break;
        }
        lines.push_back(line);
    }

// Create n POSIX threads (where n is the number of lines from the input
    int nthreads = lines.size();
    pthread_t* tid = new pthread_t[nthreads];
    Data data[nthreads];

    pthread_mutex_t mutex1, mutex2;
    pthread_cond_t condition;
    int counter = 0;

    // Initialize mutexes and condition variable
    pthread_mutex_init(&mutex1, nullptr);
    pthread_mutex_init(&mutex2, nullptr);
    pthread_cond_init(&condition, nullptr);

    for( int i = 0; i < nthreads; i++ ) {
        data[i].id = i;
        data[i].line = lines[i];
        data[i].mutex1 = &mutex1;
        data[i].mutex2 = &mutex2;
        data[i].condition = &condition;
        data[i].counter = &counter;
        if(pthread_create(&tid[i], nullptr, output, &data[i])) {
            std::cerr << "Error creating thread" << std::endl;
            return 1;
        }
    }

    for( int i = 0; i < nthreads; i++ ) {
        pthread_join(tid[i], nullptr);
    }

    if(tid != nullptr) {
        delete[] tid;
    }
    
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    pthread_cond_destroy(&condition);


    return 0;
}