#pragma once

#include <exception>

class InvalidTableException : public std::exception
{};

class InvalidColumnException : public std::exception
{};

class InvalidOperationException : public std::exception
{};
