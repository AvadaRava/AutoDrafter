#include "Champion.h"
#include <utility>

Champion::Champion(std::string name, std::string imageURL) noexcept
  : name_{std::move(name)}, imageURL_{std::move(imageURL)}
{}

const std::string& Champion::name() const noexcept {
    return name_;
}
