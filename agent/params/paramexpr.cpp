#include "paramexpr.h"
#include <QStringList>
#include "inspector.h"
#include "cache.h"

#include <QVariant>
#include <QDebug>
static const auto operand_and = "&";
static const auto operand_or = "|";

ParamExpr::ParamExpr(const std::string& exp)
{
//TODO fast sketch refactor!
    auto qstr = QString::fromStdString(exp);
    auto expressions = qstr.split('&');

    auto createExpr = [](Expr* e, QStringList& tokens) -> bool {
        if(!e) return false;

        if(tokens.size() != 2)
        {
            qWarning() << "invalid param - missing token";
            return false;
        }

        e->value = tokens[1].trimmed();
        e->operation = "=";
        e->operand = "&";

        return true;
    };

    Expr* prev = nullptr;

    int cnt = 0;
    for(auto const& expr : qAsConst(expressions))
    {
        auto tokens = expr.split('=');
        if(cnt == 0)
        {
            if(tokens[0].trimmed() == "objectName")
            {
                this->exp = new ExprObjectName();
            }
            else if(tokens[0].trimmed() == "className")
            {
                this->exp = new ExprClassName();
            }
            else {
                this->exp = new ExprProperty(tokens[0].trimmed());
            }

            if(createExpr(this->exp, tokens))
            {
                prev = this->exp;
            }
            else {
                throw ParamParsingException("parsing expression parameters error");
            }
        }
        else
        {
            if(tokens[0].trimmed() == "objectName")
            {
                prev->next = new ExprObjectName();
            }
            else if(tokens[0].trimmed() == "className")
            {
                prev->next = new ExprClassName();
            }
            else {
                prev->next = new ExprProperty(tokens[0].trimmed());
            }

            if(createExpr(prev->next, tokens))
            {
                prev = prev->next;
            }
            else {
                throw ParamParsingException("parsing expression parameters error");
            }
        }

        cnt++;
    }

}

ParamExpr::~ParamExpr()
{
    delete exp;
}

bool ParamExpr::matchParam(QObject *obj)
{
    return exp->matchParam(obj);
}

bool ParamExpr::matchParam(const Cache &cache, QObjectList& result)
{
    Cache filtered = exp->matchParam(cache);
    result = filtered.getObjects();
    return !filtered.getObjects().isEmpty();
}

ParamExpr::Expr::~Expr()
{
    delete next;
}

bool ParamExpr::Expr::matchNext(bool prev, QObject *obj)
{
    bool result = prev;

    if(!next || operand.isEmpty())
    {
        result = prev;
    }
    else if(prev && operand == operand_and)
    {
        result =  prev && next->matchParam(obj);
    }
    else if (!prev && operand == operand_or) {
        result =  prev || next->matchParam(obj);
    }
    else {
        result =  prev;
    }

    return result;
}

Cache ParamExpr::Expr::matchNext(const Cache &origCache, const Cache &filtered)
{
    Cache result = filtered;
    if(!next || operand.isEmpty())
    {
        ;
    }
    else if(!origCache.isEmpty() && operand == operand_and)
    {
        auto nextResult = next->matchParam(origCache);
        result = Cache(filtered).intersect(nextResult);
        return result;
    }
    else if (origCache.isEmpty() && operand == operand_or) {
        auto nextResult = next->matchParam(origCache);
        result = Cache(filtered).unite(nextResult);
    }

    return result;
}

//TODO use cache.byObjectName
bool ParamExpr::ExprObjectName::matchParam(QObject *obj)
{
    bool result = false;

    auto objectName = Inspector::getObjectName(obj);
    if(!value.isNull())
    {
        QRegExp rx(value);
        rx.setPatternSyntax(QRegExp::WildcardUnix);
        result = rx.exactMatch(objectName);
    }

    return matchNext(result, obj);
}

Cache ParamExpr::ExprObjectName::matchParam(const Cache &cache)
{
    auto filtered = cache.byObjectName(value).getObjects();
    return matchNext(cache, filtered);
}

//TODO for deleyed search try use matchParam from cache with assumption that just created object is in cache
bool ParamExpr::ExprProperty::matchParam(QObject *obj)
{
    bool result = false;
    auto propertyValue = Inspector::getPropertyValue(obj, property);


    if(propertyValue.canConvert(QMetaType::QString) && !value.isNull())
    {
        QRegExp rx(value);
        rx.setPatternSyntax(QRegExp::WildcardUnix);
        result = rx.exactMatch(propertyValue.toString());
    }

    return matchNext(result, obj);
}

//TODO if preceding expr has AND operand then cache should be filtered here
Cache ParamExpr::ExprProperty::matchParam(const Cache &cache)
{
    auto filtereCache = cache.byPropertyValue(property, QVariant(value));
    return matchNext(cache, filtereCache);
}

bool ParamExpr::ExprClassName::matchParam(QObject *obj)
{
    bool result = false;
    auto className = Inspector::getClassName(obj);

    if(!value.isNull())
    {
        QRegExp rx(value);
        rx.setPatternSyntax(QRegExp::WildcardUnix);
        result = rx.exactMatch(className);
    }

    return matchNext(result, obj);
}

Cache ParamExpr::ExprClassName::matchParam(const Cache &cache)
{
    auto filtered = cache.byClassName(value).getObjects();
    return matchNext(cache, filtered);
}
