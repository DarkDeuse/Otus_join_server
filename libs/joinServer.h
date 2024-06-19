#include <algorithm>
#include <iostream>
#include <map>
#include <string.h>
#include <sstream>
#include <thread>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "queue.h"

enum messType : uint8_t
{
    INSERT,
    TRUNCATE,
    INTERSECTION,
    SYMMETRIC_DIFFERENCE
};

class JoinServer
{
    public:
        JoinServer();
        ~JoinServer();
        void pushBack(std::string&);
        
    private:

        void process();
        void processINSERT();
        void processTRUNCATE();
        void processINTERSECTION();
        void processSYMMETRIC_DIFFERENCE();

        static bool compareNoCase( const std::string& s1, const std::string& s2 ); 
        void insert(std::map<int, std::string>&);

        std::map<int, std::string> m_tabA, m_tabB;
        std::map<int, std::pair<std::string, std::string>> m_results;

        std::map<messType, std::string> m_trigers;
        std::vector<std::string> m_words;
        queueLists<std::string> m_queue;
        std::thread m_thread;
};

