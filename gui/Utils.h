#pragma once
#include <string>

// parses kr amount (for example "10,50", "10.50" or "10") into øre
long long parseKr(const std::string& text);

// formats øre into a readable kr string
std::string formatAmount(long long amount);
