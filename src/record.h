#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include <iterator>
#include <stdio.h>
#include <string.h>

const int C = 300;

template<const unsigned N = 10, const unsigned int L = 1>
struct Record{
    char value[N];
    unsigned long dirs[L];
    unsigned offsets[L];

    Record() = default;

    Record(const char* inValue){
        int charSize = sizeof(value);
        strcpy(value, inValue);
        memset(dirs, 0, sizeof(unsigned long) * L);
        memset(offsets, 0, sizeof(unsigned) * L);
    }

    Record& operator=(const Record& record) {
        strcpy(value, record.value);
        memset(dirs, 0, sizeof(unsigned long) * L);
        memset(offsets, 0, sizeof(unsigned) * L);
        std::copy(record.dirs, std::end(record.dirs), dirs);
        std::copy(record.offsets, std::end(record.offsets), offsets);
        return *this;
    }

    friend std::ostream& operator<<(std::ostream &out, const Record& record) {
        std::ifstream file1("data/processed_French.txt");
        std::ifstream file2("data/processed_German.txt");
        std::ifstream file3("data/processed_Italian.txt");
        std::ifstream file4("data/processed_Portuguese.txt");
        std::ifstream file5("data/processed_Spanish.txt");
        std::ifstream file6("data/processed_Latin.txt");
        file1.seekg(record.dirs[0]);
        file2.seekg(record.dirs[1]);
        file3.seekg(record.dirs[2]);
        file4.seekg(record.dirs[3]);
        file5.seekg(record.dirs[4]);
        file6.seekg(record.dirs[5]);
        out << record.value << ":\t";
        char content1[C], content2[C], content3[C], content4[C], content5[C], content6[C];
        memset(content1, 0, C);
        memset(content2, 0, C);
        memset(content3, 0, C);
        memset(content4, 0, C);
        memset(content5, 0, C);
        memset(content6, 0, C);
        file1.read(content1, record.offsets[0]);
        file2.read(content2, record.offsets[1]);
        file3.read(content3, record.offsets[2]);
        file4.read(content4, record.offsets[3]);
        file5.read(content5, record.offsets[4]);
        file6.read(content6, record.offsets[5]);
        out << "F: ";
        if(record.dirs[0])
            out << content1;
        out << "\t G: ";
        if(record.dirs[1])
            out << content2;
        out << "\t I: ";
        if(record.dirs[2])
            out << content3;
        out << "\t P: ";
        if(record.dirs[3])
            out << content4;
        out << "\t S: ";
        if(record.dirs[4])
            out << content5;
        out << "\t L: ";
        if(record.dirs[5])
            out << content6;     
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

