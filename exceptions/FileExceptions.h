#pragma once
#include <stdexcept>
#include <string>

class FileError : public std::runtime_error
{
public:
    FileError(const std::string& path) : std::runtime_error("Failed to access file: " + path) {}
};
class CorruptedFileError : public std::runtime_error
{
public:
    CorruptedFileError(const std::string& path, const std::string& details)
    : std::runtime_error("Corrupted file: " + path + " | Details: " + details) {}
};
