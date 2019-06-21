#ifndef PARAMPROPS_H
#define PARAMPROPS_H
#include <QVariant>
#include <unordered_map>
#include "param.h"

//TODO it no make sense to keep this wrapper
class ParamProps : public Param
{
public:
    ParamProps() = default;
    std::unordered_map<std::string, QVariant> props;
};

#endif // PARAMPROPS_H
