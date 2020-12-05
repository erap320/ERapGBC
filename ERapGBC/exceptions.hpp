#pragma once
#include "utils.hpp"
#include <exception>
using std::exception;

class WrongExecutionException : public exception
{
public:
    virtual const char* what() const throw () {
        return "Execution error";
    }
};

class ReadWriteException : public exception
{
public:
    virtual const char* what() const throw () {
        return "Read/Write size mismatch";
    }
};