#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
template<const unsigned N = 10, const unsigned int L = 1>
struct Record{
    char value[N];
    unsigned long dirs[L];
    unsigned offsets[L];

    Record() = default;

    Record(const char* inValue){
        int charSize = sizeof(value);
        strcpy(value, inValue);
        memset(dirs, 0, L);
        memset(offsets, 0, L);
    }

    Record& operator=(const Record& record) {
        int i = 0;
        for (; i < N-1; i++) {
            value[i] = record.value[i];
        }
        value[i] = 0;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream &out, const Record& record) {
        out << record.value;
        return out;
    }

    friend bool operator== (const Record &r1, const Record &r2){
        return std::string{r1.value} == std::string{r2.value};
    }

    friend bool operator!= (const Record &r1, const Record &r2){
        return !(r1 == r2);
    }

    friend bool operator< (const Record &r1, const Record &r2){
        return std::string{r1.value} < std::string{r2.value};
    }

    friend bool operator<= (const Record &r1, const Record &r2){
        return std::string{r1.value} <= std::string{r2.value};
    }

    friend bool operator> (const Record &r1, const Record &r2){
        return std::string{r1.value} > std::string{r2.value};
    }

    friend bool operator>= (const Record &r1, const Record &r2){
        return std::string{r1.value} >= std::string{r2.value};
    }


};

