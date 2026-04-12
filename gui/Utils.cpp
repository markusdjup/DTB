#include "Utils.h"

long long parseKr(const std::string& text)
{
    // find separator
    size_t sepPos = text.find(',');
    if (sepPos == std::string::npos)
        sepPos = text.find('.');

    try {
        // no separator
        if (sepPos == std::string::npos) {
            size_t pos;
            long long result = std::stoll(text, &pos);
            if (pos != text.size()) return -1; // checks for trailing characters
            return result * 100;
        }

        // separator
        std::string krText = text.substr(0, sepPos);
        std::string oreText = text.substr(sepPos + 1);
        
        // øreText can't be empty or greater than 2 digits
        if (oreText.empty() || oreText.size() > 2) {
            return -1;
        }
        if (oreText.size() == 1) // "0,2" should become 20, not 2
            oreText += "0";
        
        size_t krPos, orePos;
        long long kr  = std::stoll(krText,  &krPos);
        long long ore = std::stoll(oreText, &orePos);

        // checks if entire string was consumed in both parts
        if (krPos != krText.size() || orePos != oreText.size()) 
            return -1;

        return kr * 100 + ore;
    } catch(...) {
        return -1;
    }
}

std::string formatAmount(long long amount) {
    long long kr = amount / 100;
    long long ore = std::abs(amount % 100); // if amount is negative, only kr should have minus sign
    std::string padding = (ore < 10 ? "0" : ""); // when ore is lower than 10, we need to add a zero in front
    return std::to_string(kr) + "," + padding + std::to_string(ore) + " kr";
}
