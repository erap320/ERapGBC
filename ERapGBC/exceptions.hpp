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

class UninmplementedException : public exception
{
public:
    virtual const char* what() const throw () {
        return "Uninmplemented feature";
    }
};

class ArchitectureException : public exception
{
public:
    virtual const char* what() const throw () {
        return "An operation that is inconsistent with the architecture was encounteres";
    }
};