#include "filelinescalculator.h"

#include <iostream>
#include <filesystem>
#include <fstream>


using namespace std::filesystem;

std::mutex FileLinesCalculator::m_mutex;

FileLinesCalculator::FileLinesCalculator(const std::string &fileDirectory):
    m_fileDirectory(fileDirectory)
    ,m_countOfLines(0)
	,m_countRunThread(0)
{
	m_threadCount = std::thread::hardware_concurrency();
	path filePath(fileDirectory);
	if (!is_directory(filePath)) {
		//calculateLine(fileDirectory);
		m_vecOfFiles.push_back(fileDirectory);
	}
	else {
		for (auto const& dir_entry : recursive_directory_iterator(filePath))
		{
			if (!is_directory(dir_entry.path())) {
				//calculateLine(dir_entry.path().filename().u8string());
				m_vecOfFiles.push_back(dir_entry.path().filename().u8string());
			}
		}

	}
	start();

}

void FileLinesCalculator::printResult()
{
	std::cout << "m_countOfLines = " << m_countOfLines;
}

void FileLinesCalculator::start()
{
	std::vector<std::thread> threads;
	for (auto i = 0; i < m_threadCount - 2; i++) {
		threads.push_back(std::thread(&FileLinesCalculator::startCalculate, this, i));
		m_countRunThread++;
		//thread.join();
	}

	for (auto& thr : threads)
	{
		thr.join();
	}

	startCalculate(m_threadCount);
}

void FileLinesCalculator::calculateLine(const std::string &fileName)
{
	path filePath(fileName);
	int counter = 0;
	std::ifstream fstream(filePath);
	std::string str(static_cast<size_t>(file_size(filePath)),'0');
	while (!fstream.eof()) {
		fstream.getline(str.data(), static_cast<size_t>(file_size(filePath)), '\n');
		counter++;
	}
	m_countOfLines += counter;
	
}

void FileLinesCalculator::startCalculate(int thrNUm)
{
	
	std::string filePath;
	bool flag = true;
	while (flag) {
		m_mutex.lock();
		if(!m_vecOfFiles.empty()) {
			filePath = m_vecOfFiles.back();
			m_vecOfFiles.pop_back();
			m_mutex.unlock();
			calculateLine(filePath);
		} else {
			m_mutex.unlock();
			flag = false;
			m_countRunThread--;
		}
	}
	if (m_vecOfFiles.empty() && m_countRunThread == 0) {
		printResult();
	}
}
