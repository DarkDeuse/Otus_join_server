#include "joinServer.h"

JoinServer::JoinServer():m_words{4}
{
    m_thread = std::thread(&JoinServer::process, std::ref(*this));
    
    m_trigers.emplace(std::make_pair(messType::INSERT,                  "INSERT"));
    m_trigers.emplace(std::make_pair(messType::TRUNCATE,                "TRUNCATE"));
    m_trigers.emplace(std::make_pair(messType::INTERSECTION,            "INTERSECTION"));
    m_trigers.emplace(std::make_pair(messType::SYMMETRIC_DIFFERENCE,    "SYMMETRIC_DIFFERENCE"));
}

JoinServer::~JoinServer()
{
    if (m_thread.joinable())
        m_thread.join();
}

void JoinServer::insert(std::map<int, std::string>& tab)
{
    int index = std::stoi(m_words[2]);
    auto find = tab.find(index);
        if (find==tab.end())
        {
            try
            {
                tab.emplace(std::make_pair(index, m_words[3]));
                std::cout << "OK. Insertion complete" << std::endl;
            }
            catch(const std::exception& e){
                std::cerr << e.what() << '\n';}
        }
        else
            std::cerr << "ERR duplicate " << m_words[2] << std::endl;
}

// chain of responsibility
void JoinServer::processINSERT(){
    if (m_words[0] != m_trigers[messType::INSERT])
    {
        processTRUNCATE();
        return;
    };

    if (m_words.size() != 4)
    {
        std::cerr << "wrong number of parametrs : " << " INSERT tabName index value " << std::endl;
        return;
    }

    if (m_words[1] == "A")
        insert(m_tabA);
    else if (m_words[1] == "B")
        insert(m_tabB);
    else
        std::cerr << "unknown tabName parametr : " << m_words[1] << std::endl;
}



void JoinServer::processTRUNCATE(){
    if (m_words[0] != m_trigers[messType::TRUNCATE]){
        processINTERSECTION();
        return;
    };

    if (m_words[1] == "A")
        m_tabA.clear();
    else if (m_words[1] == "B")
        m_tabB.clear();
    else
        std::cerr << "unknown tabName parametr : " << m_words[1] << std::endl;
    std::cout << "process TRUNCATE\n"; 
}

void JoinServer::processINTERSECTION(){
    if (m_words[0] != m_trigers[messType::INTERSECTION])
    {
        processSYMMETRIC_DIFFERENCE();
        return;
    };

    m_results.clear();
    for (auto const&  iA : m_tabA)
        for (auto const&  jB : m_tabB)
            if (iA.first == jB.first)
                m_results.try_emplace(iA.first, 
                                        std::make_pair(iA.second,jB.second));
    
    std::cout << "ID,FirstValue,SecondValue\n";
    for (auto const&  i : m_results)
        std::cout << i.first << "," << i.second.first << "," << i.second.second << "\n";
    std::cout << std::endl;
}

void JoinServer::processSYMMETRIC_DIFFERENCE(){
    if (m_words[0] != m_trigers[messType::SYMMETRIC_DIFFERENCE]){
        if (m_words[0]!="")
            std::cout << "unknown command : " << m_words[0] << std::endl;
        return;
    };

    m_results.clear();
    for (auto const&  iA : m_tabA){
        auto found = m_tabB.find(iA.first);
            if (found == m_tabB.end())
                m_results.emplace(iA.first, 
                                        std::make_pair(iA.second,""));
    }
    for (auto const&  jB : m_tabB){
        auto found = m_tabA.find(jB.first);
            if (found == m_tabA.end())
                m_results.try_emplace(jB.first, 
                                        std::make_pair("",jB.second));
    }

    std::cout << "ID,FirstValue,SecondValue\n";
    for (auto const&  i : m_results)
        std::cout << i.first << "," << i.second.first << "," << i.second.second << "\n";
    std::cout << std::endl;
}


void JoinServer::process()
{
    std::string stringToProcess;
    
    // pop 1 comand, and split into words
    while (m_queue.pop(stringToProcess))
    {   
        boost::split(m_words, stringToProcess, [](char c){return c == ' ';});
        processINSERT();
    }
}

void JoinServer::pushBack(std::string& buf)
{
    // push one command contain more than 1 word
    m_queue.push(buf);
}