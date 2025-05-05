#pragma once
#include <exception>
#include <string>

// Bază proprie de excepții
class LoLException : public std::exception {
    std::string msg_;
public:
    explicit LoLException(std::string msg) noexcept
      : msg_{std::move(msg)} {}
    const char* what() const noexcept override {
        return msg_.c_str();
    }
};

// Erori specifice distincte
class InvalidRankException : public LoLException {
public:
    InvalidRankException() noexcept
      : LoLException{"Invalid rank for operation"} {}
};

class NoPlayersException : public LoLException {
public:
    NoPlayersException() noexcept
      : LoLException{"No players available on server"} {}
};

class DrafterException : public LoLException {
public:
    DrafterException() noexcept
      : LoLException{"Drafter failed to draft"} {}
};
