#pragma once
#include <iostream>
#include <stdio.h>
#include <string.h>
template<const int ORDER = 10>
class Record{
    char value[ORDER];

public:
    Record(){}

    Record(const char* inValue){
        int charSize = sizeof(value);
        int i = 0;
        for (; i < ORDER-1 && i < charSize; i++) {
            value[i] = inValue[i];
        }
        value[i] = 0;
    }

    Record& operator=(const Record& record) {
        int i = 0;
        for (; i < ORDER-1; i++) {
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
        int result = strcmp(r1.value, r2.value);
        return result==0;
    }

    friend bool operator!= (const Record &r1, const Record &r2){
        return !(r1 == r2);
    }

    friend bool operator< (const Record &r1, const Record &r2){
        int i = 0;
        while(r1.value[i] == r2.value[i])i++;
        return r1.value[i]< r2.value[i];
    }

    friend bool operator<= (const Record &r1, const Record &r2){
        int i = 0;
        while(r1.value[i] == r2.value[i])i++;
        return r1.value[i] <= r2.value[i];
    }

    friend bool operator> (const Record &r1, const Record &r2){
        int i = 0;
        while(r1.value[i] == r2.value[i])i++;
        return r1.value[i] > r2.value[i];
    }

    friend bool operator>= (const Record &r1, const Record &r2){
        int i = 0;
        while(r1.value[i] == r2.value[i])i++;
        return r1.value[i] >= r2.value[i];
    }


};

