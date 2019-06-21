#include "hook.h"
#include "spyspawner.h"
#include "spy.h"
#include "remotehandler.h"
#include "tcpserver.h"
#include "dispatcher.h"
#include "client.h"
#include <private/qhooks_p.h>
#include <iostream>
#include <unordered_set>
#include <QCoreApplication>
#include <QThread>

bool Hook::installed = false;

namespace {
std::unordered_set<QObject*> objectsBeforeHooks;
Spy* spy = nullptr;
}

void processBeforeHook(Spy* spy)
{
    std::cout << "before count " << objectsBeforeHooks.size() << std::endl;
    for(auto const& obj : objectsBeforeHooks)
    {
        spy->onObjectCreated(obj);
    }
    objectsBeforeHooks.clear();
}

extern "C" Q_DECL_EXPORT void addObjectHook(QObject *obj)
{
    if(spy)
    {
        spy->onObjectCreated(obj);
    }
    else
    {
        objectsBeforeHooks.insert(obj);
    }
}

extern "C" Q_DECL_EXPORT void deleteObjectHook(QObject *obj)
{
    if(spy)
    {
        spy->onObjectDeleted(obj);
    }
    else
    {
        objectsBeforeHooks.erase(obj);
    }
}

extern "C" Q_DECL_EXPORT void startupHook()
{

    if(!qApp) {
        std::cout << "app not ready!" << std::endl;
    }

    Q_ASSERT(QThread::currentThread() == qApp->thread());

    std::cout << "main thread " << static_cast<void*>(qApp->thread()) << std::endl;


    QTimer::singleShot(2000, [](){
        std::cout << "hooked..." << std::endl;
        qtHookData[QHooks::AddQObject] = reinterpret_cast<quintptr>(&addObjectHook);
        qtHookData[QHooks::RemoveQObject] = reinterpret_cast<quintptr>(&deleteObjectHook);

        Spy::create();
        spy = Spy::instance();
        QMutexLocker locker(spy->objectLock());

        processBeforeHook(spy);

        auto conn = new TcpServer(nullptr);

        QObject::connect(conn, &TcpServer::clientConnected, [=](Client* client){
            auto dispatcher = new Dispatcher(client);
            QObject::connect(spy, &Spy::objectDeleted, dispatcher, &Dispatcher::removeContext, Qt::DirectConnection);
        });

        conn->start();
    });

}

void Hook::install()
{
    std::cout << "install..." << std::endl;
    if(Hook::installed) {
        return;
    }


    Hook::installed = true;
    qtHookData[QHooks::Startup] = reinterpret_cast<quintptr>(&startupHook);
}

Hook::Hook()
{

}

