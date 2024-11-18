#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <cmath>

struct Data {
    int id;
    std::string line;
    pthread_mutex_t *mutex1;
    pthread_mutex_t *mutex2;
    pthread_cond_t *condition;
    int *counter;

};

void inOrder(std::vector<char>& orderedL, std::vector<int>& orderedF) {
    for (int i = 0; i < orderedF.size(); i++) {
        for (int j = i + 1; j < orderedF.size(); j++) {
            if (orderedF[j] > orderedF[i] || (orderedF[j] == orderedF[i] && orderedL[j] > orderedL[i])) {
                std::swap(orderedF[i], orderedF[j]);
                std::swap(orderedL[i], orderedL[j]);
            }
        }
    }
}

void dec2bin(std::vector<float>& len, std::vector<float>& acc, std::vector<std::string>& cod) {
    for (int i = 0; i < acc.size(); i++) {
        float fraction = acc[i];
        std::string code = "";
        int originalLength = len[i];
        while (len[i] > 0) {
            fraction *= 2;
            int integer = static_cast<int>(fraction);

            code += std::to_string(integer);

            fraction -= integer;
            len[i] -= 1;
        }
        while (code.length() < originalLength) {
            code = "0" + code;
        }
        cod.push_back(code);
    }
}



void *output (void *void_ptr) {
    Data *data = (Data*) void_ptr;

    //Critical Section - retrieves data
    pthread_mutex_lock(data->mutex1);
    int localId = data->id;
    std::string localLine = data->line;

    std::vector<char>letters;
    std::vector<int> frequency;
    std::vector<int> shannon;
    std::vector<char> orderedLetters; //stores each letter in message, ordered by frequency 
    std::vector<int> orderedFrequency; //stores frequency of each letter in message, ordered by frequency 
    std::vector<float> probabilities;
    std::vector<float> accumulation;
    std::vector<float> length;
    std::string encoded;
    std::vector<std::string> code;


    //stores each letter in string into its own vector
    for( int i = 0; i < localLine.size(); i++) {
        letters.push_back(localLine[i]);
    }

    frequency.resize(letters.size(),0);

    //count frequency of each letter in string
    for( int i = 0; i < letters.size(); i++ ) {
        char current = letters[i];
        bool isDupe = false;

        for( int j = 0; j < i; j++ ) {
            if(letters[j] == current) {
                isDupe = true;
                break;
            }
        }

        if(!isDupe) {
            int count = 0;
            for( int j = 0; j < letters.size(); j++) {
                if(letters[j] == current) {
                    count++;
                }
            }
            frequency[i] = count;
        }
    }

    //places letters and its frequencies in the ordered vectors, ordered by frequency
    for( int i = 0; i < letters.size(); i++) {
        if(frequency[i] > 0 ) {
            orderedFrequency.push_back(frequency[i]);
            orderedLetters.push_back(letters[i]);
        }
    }

    inOrder(orderedLetters, orderedFrequency);

    //find probabilities
    int sum = 0;
    for( int i = 0; i < orderedFrequency.size(); i++) {
        sum = sum + orderedFrequency[i];
    }

    probabilities.resize(orderedFrequency.size());

    for( int i = 0; i < orderedFrequency.size(); i++ ){
        probabilities[i] = static_cast<float>(orderedFrequency[i]) / sum;
    }

//accumulation
    accumulation.resize(probabilities.size());

    accumulation[0] = 0.0; //first symbol is always 0
    for( int i = 1; i < probabilities.size(); i++) {
        accumulation[i] = accumulation[i-1] + probabilities[i-1]; 
    }

    length.resize(probabilities.size());

//length of shannon code
    for( int i = 0; i < probabilities.size(); i++ ) {
        length[i] = std::ceil(log2(1/probabilities[i]));
    }

//actual shannon code for each symbol
    dec2bin(length, accumulation, code);

//generate full shannon code for message
    //string line = contains message 
    for( int i = 0; i < localLine.size(); i++ ) {
        char current = localLine[i];

        //find code for each character
        for( int j = 0; j < orderedLetters.size(); j++ ) {
            if(orderedLetters[j] == current) {
                encoded = encoded + code[j];
                break; //break loop after appending code for symbol
            }
        }
    }
    pthread_mutex_unlock(data->mutex1);

    //Critical Section - checks turn and waits 
    pthread_mutex_lock(data->mutex2);
    while (*data->counter != localId) {
        pthread_cond_wait(data->condition, data->mutex2);
    }

    //ALGORITHM GOES HERE
    //std::cout << "Thread " << localId << ": " << localLine << std::endl;
    std::cout << "Message: " << localLine << std::endl;
    std::cout << std::endl;
    std::cout << "Alphabet: " << std::endl;

    for( int i = 0; i < orderedLetters.size(); i++) {
        std::cout << "Symbol: " << orderedLetters[i];
        std::cout << ", Frequency: " << orderedFrequency[i];
        std::cout << ", Shannon code: " << code[i] << std::endl;
    }

    std::cout<<std::endl;
    std::cout << "Encoded message: " << encoded << std::endl;

    //Critical Section - increment and signals 
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