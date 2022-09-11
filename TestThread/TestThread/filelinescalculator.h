#ifndef FILELINESCALCULATOR_H
#define FILELINESCALCULATOR_H

#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <vector>

class FileLinesCalculator
{
public:
    FileLinesCalculator(const std::string& fileDirectory);


private:
    std::string m_fileDirectory;
    std::atomic_int m_countOfLines;
	unsigned int m_threadCount;
	std::vector<std::string> m_vecOfFiles;
	static std::mutex m_mutex;
	std::atomic_int m_countRunThread;


    void printResult();
	void start();
    void calculateLine(const std::string& fileName);
	void startCalculate(int thrNUm);

};

#endif // FILELINESCALCULATOR_H
