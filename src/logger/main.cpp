


#include <Logger.hpp>
#include <string>

#include <iostream>

#include "LogData.hpp"



int main(int args, char argch[])
{
	logger::debug << "well, this is exciting, ppl : " << 55454 << std::endl;
	logger::error << "well, this is exciting, ppl : " << 55454 << std::endl;
	logger::warning << "well, this is exciting, ppl : " << 55454 << std::endl;
	logger::fatal << "well, this is exciting, ppl : " << 55454 << std::endl;




	std::cin.get();
	return 0;
}