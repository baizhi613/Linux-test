#pragma once

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include "Log.hpp"

namespace dict_ns
{
    const std::string defaultpath = "./Dict.txt";
    const std::string sep = ": ";

    class Dict
    {
    private:
        bool Load()
        {
            std::ifstream in(_dict_conf_filepath);
            if (!in.is_open())
            {
                LOG(FATAL, "open %s error\n", _dict_conf_filepath.c_str());
                return false;
            }
            std::string line;
            while(std::getline(in, line))
            {
                if(line.empty()) continue;
                auto pos = line.find(sep); // [happy): 快乐的    [abcde)fg
                if(pos == std::string::npos) continue;
                std::string word = line.substr(0, pos);
                if(word.empty()) continue;
                std::string han = line.substr(pos+sep.size());
                if(han.empty()) continue;
                LOG(DEBUG, "load info, %s: %s\n", word.c_str(), han.c_str());

                _dict.insert(std::make_pair(word, han));
            }
            in.close();
            LOG(DEBUG, "load %s success\n", _dict_conf_filepath.c_str());
            return true;
        }

    public:
        Dict(const std::string &path = defaultpath) : _dict_conf_filepath(path)
        {
            Load();
        }
        std::string Translate(const std::string &word, bool &ok)
        {
            ok = true;
            auto iter = _dict.find(word);
            if(iter == _dict.end())
            {
                ok = false;
                return "未找到";
            }
            // return _dict[word];
            return iter->second;
        }
        ~Dict()
        {
        }

    private:
        std::unordered_map<std::string, std::string> _dict;
        std::string _dict_conf_filepath;
    };
}