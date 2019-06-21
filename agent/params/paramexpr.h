#ifndef PARAMEXPR_H
#define PARAMEXPR_H
#include <QObject>
#include <vector>
#include <exception>
#include "param.h"

//TODO better remove exceptions
struct ParamParsingException : public std::runtime_error
{
    explicit ParamParsingException(const std::string& arg) : std::runtime_error(arg){}
};

class Cache;
class ParamExpr : public Param
{
public:
    ParamExpr(const std::string& exp);
    ~ParamExpr() override;

    struct Expr
    {
        virtual ~Expr();
        QString operation;
        QString value;

        QString operand;
        Expr* next = nullptr;

        bool matchNext(bool prev, QObject* obj);
        Cache matchNext(const Cache& cache, const Cache &filtered);

        virtual bool matchParam(QObject* obj) = 0;
        virtual Cache matchParam(const Cache& cache) = 0;
    };

    struct ExprObjectName : public Expr
    {
        ExprObjectName() = default;
        bool matchParam(QObject* obj) override;
        Cache matchParam(const Cache& cache) override;
    };

    struct ExprClassName : public Expr
    {
        ExprClassName() = default;
        bool matchParam(QObject* obj) override;
        Cache matchParam(const Cache& cache) override;
    };

    struct ExprProperty : public Expr
    {
        ExprProperty(const QString& property) : property(property){}
        bool matchParam(QObject* obj) override;
        Cache matchParam(const Cache& cache) override;

        QString property;
    };

    Expr* exp = nullptr;

    bool matchParam(QObject* obj);
    bool matchParam(const Cache& cache, QObjectList& result);
};

#endif // PARAMEXPR_H
