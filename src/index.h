#pragma once

#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include "pagemanager.h"

template<template<typename U, int K> typename DS, typename T, int S>
class Index {
    private:
    DS<T, S> datastructure;

    protected:
    std::vector<std::string> datafiles;
    
    template<typename K, typename ...Args>
    void add_datafiles(K datafile) {
        datafiles.push_back(datafile);
    }

    template<typename K, typename ...Args>
    void add_datafiles(K datafile, Args... args) {
        datafiles.push_back(datafile);
        add_datafiles(args...);
    }

    public:
    template<typename K, typename ...Args>
    explicit Index(std::string indexfile, K datafile, Args ...args):
        datastructure{std::shared_ptr<pagemanager>{new pagemanager{indexfile}}} {
            add_datafiles(datafile, args...);
        }

    void insert(T k) {
        std::cout << "inserting " << k.value << std::endl;
        datastructure.insert(k);
    }

    void print(std::ofstream& out) {
        datastructure.print(out);
    }

    void execute() {
        struct Data {
            std::string value;
            unsigned long dir;
            unsigned offset;
        };

        std::vector<std::ifstream> streams;
        std::vector<Data> data(datafiles.size());

        auto consume_data_from_stream = [&] (int i) {
            std::string key, rest;
            if(std::getline(streams[i], key, '\t')) {
                std::getline(streams[i], rest);
                unsigned long pos = streams[i].tellg();
                data[i] = Data{ key, pos - rest.length(), static_cast<unsigned>(rest.length()) };
            }
            else
                data[i] = Data{};
        };

        auto get_min_key = [&] () {
            std::string min_value;
            for(auto d : data) {
                if(d.value.length()) 
                    min_value = d.value;
            }
            if(min_value == "")
                return min_value;
            for(auto d : data) {
                if(d.value.length())
                    min_value = d.value < min_value ? d.value : min_value;
            }
            return min_value;
        };

        for(int i = 0; i < datafiles.size(); ++i) {
            std::ifstream file(datafiles[i]);
            std::cout << datafiles[i] << std::endl;
            std::cout << (file.is_open() ? "si" : "no") << std::endl;
            streams.push_back(std::move(file));
            consume_data_from_stream(i);
        }
        
        for(std::string min_value = get_min_key(); min_value != ""; min_value = get_min_key()) {
            T t(min_value.c_str());
            for(int i = 0; i < data.size(); ++i) {
                if(data[i].value != "" && data[i].value == min_value) {
                    t.dirs[i] = data[i].dir;
                    t.offsets[i] = data[i].offset;
                    consume_data_from_stream(i);
                }
            }
            insert(t);
        }
    }
};