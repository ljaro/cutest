#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QObject>
#include <vector>
#include <unordered_map>
#include <memory>
#include "param.h"
#include "testobject.h"
#include "context.h"
#include "testactionstatus.h"
#include "response.h"

class RemoteHandler;
class Dispatcher : public QObject, public Response
{
public:
    Dispatcher(RemoteHandler* client);
private:
    enum class State
    {
        IDLE,
        CMD_PROCESSING,
    };

    State state = State::IDLE;
    RemoteHandler* client;
    std::unordered_map<ContextStr, TestObject> context2object;
    std::unordered_map<QObject*, ContextStr> object2context;
    QByteArray createResponse(QString cmdId, TestObject obj, Status status);
    bool checkAndRememberContext(TestObject obj);
    TestObject getContext(ContextStr uuid) const;
    void parseCommand(const QString &cmd);

public slots:
    void commandArrived(const QString&);
    void removeContext(QObject* obj);
};

#endif // DISPATCHER_H
