#include "GlobalConfiguration.h"
#include <boost/property_tree/ptree.hpp>

namespace sequoia {

namespace core {

template <>
GlobalConfiguration* Singleton<GlobalConfiguration>::instance_ = nullptr;

GlobalConfiguration::GlobalConfiguration()
    : ptree_(std::make_shared<boost::property_tree::ptree>()) {}


} // namespace core

} // namespace sequoia
