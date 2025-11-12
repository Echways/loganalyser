#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <time.h>
#include <set>
#include <variant>
#include <unordered_map>
#include <deque>


struct StateTrue {};
struct StateFalse {};


class InputData{
    private:
       inline static std::ifstream _input = std::ifstream();
    
    public:
        void SetInput(std::string str){
            std::ifstream in;
            in.open(str);

            if(!in.is_open()){
                std::cerr << "Wrong input! Please restart the app with a correct input file." << std::endl;
                return;
            }
            _input = std::ifstream(str);

        }

        std::ifstream& GetInput(){
            return _input; 
        }
};


class OutputData{
    private:
        inline static std::ofstream _output = std::ofstream();
    
    public:
        void SetOutput(std::string str){
            _output = std::ofstream(str);
        }

        std::ofstream& GetOutput(){
            return _output;
        }
};


class PrintParams{
    private:
        inline static bool _is_print = 0;

    public:
        void SetPrint(){
            _is_print = 1;
        }

        bool GetPrint() {
            return _is_print;
        }
};


class TimeParams{
    private:
        inline static long long _start_time = -1;
        inline static long long _end_time = INT64_MAX;
        inline static long long _first_appearence = -1;

        int GetMonth(std::string str){
            std::map<std::string, int> mp = {{"Jan", 0}, {"Feb", 1}, {"Mar", 2}, {"Apr", 3}, {"May", 4}, {"Jun", 5}, {"Jul", 6}, {"Aug", 7}, {"Sep", 8}, {"Oct", 9}, {"Nov", 10}, {"Dec", 11}};
            return mp[str];
        }

    public:
        void SetStartTime(long long arg){
            _start_time = arg;
        }

        void SetEndTime(long long arg){
            _end_time = arg;
        }

        void SetFirstAppearance(long long arg){
            _first_appearence = arg;
        }

        long long GetStartTime() {
            return _start_time;
        }

        long long GetEndTime() {
            return _end_time;
        }

        long long GetFirstAppearance() {
            return _first_appearence;
        }

        long long GetTimestamp(std::string time){
            struct tm t;
            time_t t_of_day;
            std::string str="";
            str.append(1, time[4]);
            str.append(1, time[5]);
            str.append(1, time[6]);
            t.tm_year = ((time[8]-'0')*1000+(time[9]-'0')*100+(time[10]-'0')*10+(time[11]-'0'))-1900;
            t.tm_mon = GetMonth(str);
            t.tm_mday = ((time[1]-'0')*10+time[2]-'0');
            t.tm_hour = ((time[13]-'0')*10+(time[14]-'0'));
            t.tm_min = ((time[16]-'0')*10+(time[17]-'0'));
            t.tm_sec = (time[19]-'0')*10+time[20]-'0';
            t.tm_isdst = -1;
            t_of_day = timegm(&t);

            return static_cast<long long>(t_of_day);
        }
};


class StatsParams{
    private:
        inline static int _stats_size = 0;
        inline static std::unordered_map<std::string, int> _freqMap;
        inline static std::multiset<std::pair<int, std::string>> _freqSet;
    
    public:
        void SetStats(int arg){
            _stats_size = arg;
        }

        int GetStats(){
            return _stats_size;
        }

        void FillStats(std::vector<std::vector<std::string>> vec){
            for(auto elem: vec){
                std::string str = elem[elem.size()-5]+elem[elem.size()-4]+elem[elem.size()-3];
                if (_freqMap.contains(str)) {
                    _freqMap.erase(str);
                }

                _freqMap[str]++;

                _freqSet.insert({_freqMap[str], str});
            }
        }

        void PrintStats(){
            int count = 0;

            for (auto it = _freqSet.rbegin(); it != _freqSet.rend() && count < _stats_size; ++it, ++count) {
                std::cout << it->second << "\n";
            }
        }
};

class ArgOperations{
    public:
        std::string GetArgFromString(std::string argfull){
            argfull.erase(0, argfull.find('=') + 1);
            return argfull;
        }
};

class WindowParams{
    public:
        struct TimeWindow{
            long long start;
            long long end;
            long long count;
        };

    private:
        inline static long long _window = 0;
        inline static long long _iter = 0;
        inline static long long _num = 0;
        inline static TimeWindow _ans = {-1,-1,-1};

    public:
        void SetWindow(long long arg){
            _window = arg;
        }

        long long GetWindow(){
            return _window;
        }

        void WindowPrint(){
            std::cout << _ans.start << " to " << _ans.end << "\n";
        }

        void ProcessWindow(long long timestamp){
            TimeParams time;

            if(timestamp <= time.GetFirstAppearance()+_window+_iter){
                ++_num;
            }else if(_num > _ans.count){
                WindowParams::TimeWindow tmp;
                tmp.start = time.GetFirstAppearance()+_iter;
                tmp.end = time.GetFirstAppearance()+_window+_iter;
                tmp.count = _num;

                _ans = tmp;
                _num = 0;
                ++_iter;
            }else{
                _num = 0;
                ++_iter;
            }
        }
};


class OutputOperations{
    public:
        void Output(std::vector<std::vector<std::string>> vec, StateTrue){
            OutputData output;
            for(auto elem: vec){
                for(auto elemm: elem){
                    output.GetOutput() << elemm << " ";
                    std::cout << elemm << " ";
                }
                output.GetOutput() << "\n";
                std::cout << "\n";
            }
        }

        void Output(std::vector<std::vector<std::string>> vec, StateFalse){
            OutputData output;
            for(auto elem: vec){
                for(auto elemm: elem){
                    output.GetOutput() << elemm << " ";
                }
                output.GetOutput() << "\n";
            }
        }
};


class VecOperations{
    public:
        void FilterVec(std::vector<std::vector<std::string>> vec){
            std::vector<std::vector<std::string>> vecc;
            TimeParams time;
            WindowParams window;
            long long timestamp;
            int z=0;


            for(int i = 0; i < vec.size(); i++){
                z=0;

                for(auto c: vec[i]){
                    if(c[0]=='['){
                        timestamp = time.GetTimestamp(c);
                    }

                    if(z==vec[i].size()-2 && c[0]=='5' && timestamp >= time.GetStartTime() && timestamp <= time.GetEndTime()){
                        vecc.push_back(vec[i]);
                    }
                    z++;
                }

                if(time.GetFirstAppearance() == -1){
                    time.SetFirstAppearance(timestamp);
                }

                window.ProcessWindow(timestamp);
            }

            PrintParams print;
            StatsParams stats;
            OutputOperations output;

            if(print.GetPrint()){
                output.Output(vecc, StateTrue());
            }else{
                output.Output(vecc, StateFalse());
            }

            stats.FillStats(vecc);
        }
};

class BufferParams{
    private:
        inline static int _buff_size = 10;

    public:
        void SetBuffSize(int arg){
            _buff_size = arg;
        }

        void FillBuff(){
            std::string str;
            std::vector<std::vector<std::string>> vec;
            std::vector<std::string> vecc;
            std::string full_str;
            std::stringstream sstream_str;

            BufferParams buff;
            InputData input;
            VecOperations vecop;


            for(int _ = 0; _ < _buff_size; _++){
                vecc.clear();
                full_str = "";
                std::getline(input.GetInput(), full_str);
                sstream_str.str("");
                sstream_str << full_str;

                while(std::getline(sstream_str, str, ' ')){
                    vecc.push_back(str);
                }
                vec.push_back(vecc);
            }
            vecop.FilterVec(vec);
        }

};


int main(int argc, char* argv[]) {
    for(int i=1;i<argc; i++){
        std::string currarg = argv[i];
        ArgOperations argsop;

        if(currarg[currarg.length()-1]=='g'){
                InputData data;
                data.SetInput(currarg);
            }

        if (argc > 1) {
            switch (currarg[1]) {
                case 'o':{
                    ++i;
                    OutputData data;
                    data.SetOutput(argv[i]);
                    break;}
                case 'p':{
                    PrintParams param;
                    param.SetPrint();
                    break;}
                case 's':{
                    ++i;
                    StatsParams stats;
                    stats.SetStats(std::stoi(argv[i]));
                    break;}
                case 'w':{
                    ++i;
                    WindowParams param;
                    param.SetWindow(std::stoi(argv[i]));
                    break;}
                case 'f':{
                    ++i;
                    TimeParams param;
                    param.SetStartTime(std::stoi(argv[i]));
                    break;}
                case 'e':{
                    ++i;
                    TimeParams param;
                    param.SetEndTime(std::stoi(argv[i]));
                    break;}
                case 'b':{
                    ++i;
                    BufferParams param;
                    param.SetBuffSize(std::stoi(argv[i]));
                    break;
                }
                default:{

                }
            }

            switch (currarg[2]) {
                case 'o':{
                    OutputData data;
                    data.SetOutput(argsop.GetArgFromString(currarg));
                    break;}
                case 'p':{
                    PrintParams param;
                    param.SetPrint();
                    break;}
                case 's':{
                    StatsParams stats;
                    stats.SetStats(std::stoi(argsop.GetArgFromString(currarg)));
                    break;}
                case 'w':{
                    WindowParams param;
                    param.SetWindow(std::stoi(argsop.GetArgFromString(currarg)));
                    break;}
                case 'f':{
                    TimeParams param;
                    param.SetStartTime(std::stoi(argsop.GetArgFromString(currarg)));
                    break;}
                case 't':{
                    TimeParams param;
                    param.SetEndTime(std::stoi(argsop.GetArgFromString(currarg)));
                    break;}
                case 'b':{
                    BufferParams param;
                    param.SetBuffSize(std::stoi(argsop.GetArgFromString(currarg)));
                    break;
                }
                default:{

                }
            }
        }
        else {
            std::cerr << "No options provided" << std::endl;
            return 0;
        }
    }

    InputData input;
    StatsParams stats;
    WindowParams window;
    BufferParams buff;


    while(!input.GetInput().eof()){
        buff.FillBuff();
    }

    stats.PrintStats();
    if(window.GetWindow()>0) window.WindowPrint();
    
}
