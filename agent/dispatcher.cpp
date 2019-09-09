#include "dispatcher.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <vector>
#include <iostream>

#include "remotehandler.h"
#include "testaction.h"
#include "actionresult.h"
#include "spy.h"
#include "params/paramexpr.h"
#include "params/paramprops.h"

using namespace std;

Dispatcher::Dispatcher(RemoteHandler* client) : client(client)
{
    QObject::connect(client, &RemoteHandler::commandReceived,
                     this, &Dispatcher::commandArrived);

}

void Dispatcher::commandArrived(const QString& cmd)
{
    switch(state)
    {
    case State::IDLE:
        parseCommand(cmd);
        break;

    case State::CMD_PROCESSING:
        qDebug() << "Another command is processing";
//        createResponse()
        break;
    }
}

void Dispatcher::parseCommand(const QString& cmd)
{
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(cmd.toUtf8(), &error);

    if(error.error)
    {
        qDebug() << "Command parsing error";
        qDebug() << cmd.toUtf8();
        state = State::IDLE;
        return;
    }

    QJsonObject obj = jsonDoc.object();

    if(!jsonDoc.isObject())
    {
        qDebug() << "Received command is not json object";
        state = State::IDLE;
        return;
    }

    auto requestId = obj.value("id").toVariant().toString();
    if(requestId.isEmpty())
    {
        qDebug() << "Received command without request id";
        state = State::IDLE;
        return;
    }

    auto cmdName = obj.value("cmd").toString();
    if(cmdName.isEmpty())
    {
        qDebug() << "Received command without command name";
        state = State::IDLE;
        return;
    }

    ContextStr cmdCtx = obj.value("context").toString().toStdString();

    //TODO should valid context here or in each action?
    auto ctx = getContext(cmdCtx);
    auto params = obj.value("params").isObject() ? obj.value("params").toObject() : QJsonObject();

    try
    {
        auto& action = TestActionBuilder::getAction(cmdName, ctx, params);

        state = State::CMD_PROCESSING;
        action.exec(ctx, [this, requestId](ActionResult result, Status status){

            if(result.type == ActionResult::ValueType::ObjectQt)
            {
                auto& obj = result.objectValue;
                if(checkAndRememberContext(obj))
                {
                    auto response = result.createResponse(requestId, status);
                    client->sendResponse(response);
                }
                else
                {
                    //TODO refactor
                    auto response = ActionResult().createResponse(requestId, ActionStatus::create(this).contextNotFound());
                    client->sendResponse(response);
                }
            }
            else if(result.type == ActionResult::ValueType::ObjectQtList)
            {
                //auto lst = result.objectValueList;
                for(auto const & testObject : qAsConst(result.objectValueList))
                {
                    auto obj = testObject.getQObject();
                    auto exists = false;

                    if(obj != nullptr)
                    {
                        exists = checkAndRememberContext(testObject);
                    }

                    if(!exists)
                    {
                        auto response = ActionResult().createResponse(requestId, ActionStatus::create(this).contextNotFound());
                        client->sendResponse(response);
                        return;
                    }
                }
                auto response = result.createResponse(requestId, status);
                client->sendResponse(response);
            }
            else if(result.type == ActionResult::ValueType::Simple)
            {
                auto response = result.createResponse(requestId, status);
                client->sendResponse(response);
            }
            else {
                auto response = createResponse(requestId, TestObject(), ActionStatus::create(this).contextNotFound());
                client->sendResponse(response);
            }

            state = State::IDLE;
        });
    }
    catch (const ParamParsingException& e)
    {
        auto response = createResponse(requestId, TestObject(), ActionStatus::create(this).parsingError(e.what()));
        client->sendResponse(response);
    }
    catch (...)
    {
        auto response = createResponse(requestId, TestObject(), ActionStatus::create(this).parsingError(""));
        client->sendResponse(response);
    }

}

QByteArray Dispatcher::createResponse(QString requestId,
                                      TestObject obj,
                                      Status status)
{
    QJsonDocument doc;
    QJsonObject resp;
    resp.insert("id", requestId);
    resp.insert("context", QString::fromStdString(obj.getUuid()));
    resp.insert("status", QJsonObject::fromVariantHash(status.getStatuses()));
    resp.insert("qobject", obj.serialize());
    doc.setObject(resp);
    return doc.toJson();
}

/*
 * {
 *      id: 100
 *      cmd: click
 *      context: 3c82303df3
 *      params: {
 *          exp: 'objectName=bleble',
 *          timeout: 100
 *          delay: 1
 *          etc...
 *      }
 * }
 *
 */

bool Dispatcher::checkAndRememberContext(TestObject obj)
{//TODO spy lock?
    if(!obj.getQObject())
    {
        return false;
    }

    bool hasObj = Spy::instance()->cache.hasObject(obj.getQObject());
    if(!hasObj)
    {
        return false;
    }

    auto uuid = obj.getUuid();
    auto qobj = obj.getQObject();

    std::cout << "context created " << uuid << std::endl;
    context2object.insert({uuid, obj});
    object2context.insert({qobj, uuid});

    return true;
}

TestObject Dispatcher::getContext(ContextStr uuid) const
{
    if(!uuid.empty() && context2object.find(uuid) != context2object.end())
    {
        auto& p = context2object.at(uuid);
        std::cout << "CONTEXT EXEC " << p.getQObject() << std::endl;
        return context2object.at(uuid);
    }
    else
    {
        // return empty object and decision about response is posponed to coresponding action
        return TestObject();
    }
}

void Dispatcher::removeContext(QObject *obj)
{
    auto ctxIt = object2context.find(obj);
    if(ctxIt != object2context.end())
    {
        std::cout << "ERASE " << obj << std::endl;
        context2object.erase(ctxIt->second);
        object2context.erase(obj);
    }
}
