#include "agent.h"
#include "hook.h"
#include "cachelookup.h"
#include <memory>
#include "testaction.h"
#include "actions/waitfor.h"
#include "actions/waitforcheck.h"
#include "actions/waitforcheckchild.h"
#include "actions/waitforchild.h"
#include "actions/click.h"
#include "actions/nullaction.h"
#include "actions/mousepress.h"
#include "actions/mouserelease.h"
#include "actions/mousemove.h"
#include "actions/highlight.h"
#include "actions/write.h"
#include "actions/findsibling.h"
#include "actions/hitkey.h"
#include "actions/set.h"
#include "actions/get.h"
#include "actions/nextaction.h"
#include "actions/invoke.h"
#include "actions/modelvalue.h"
#include "actions/rolenames.h"
#include "actions/modelset.h"
#include "actions/tree.h"
using namespace std;

Agent::Agent()
{
    //TODO make auto registration per class
    using namespace TestRobot::Action;
    TestActionBuilder::registerAction(make_shared<WaitFor>());
    TestActionBuilder::registerAction(make_shared<WaitForCheck>());
    TestActionBuilder::registerAction(make_shared<WaitForCheckChild>());
    TestActionBuilder::registerAction(make_shared<WaitForChild>());
    TestActionBuilder::registerAction(make_shared<FindSibling>());
    TestActionBuilder::registerAction(make_shared<Click>());
    TestActionBuilder::registerAction(make_shared<MousePress>());
    TestActionBuilder::registerAction(make_shared<Highlight>());
    TestActionBuilder::registerAction(make_shared<Write>());
    TestActionBuilder::registerAction(make_shared<HitKey>());
    TestActionBuilder::registerAction(make_shared<Set>());
    TestActionBuilder::registerAction(make_shared<Get>());
    TestActionBuilder::registerAction(make_shared<Next>());
    TestActionBuilder::registerAction(make_shared<Invoke>());
    TestActionBuilder::registerAction(make_shared<ModelValue>());
    TestActionBuilder::registerAction(make_shared<RoleNames>());
    TestActionBuilder::registerAction(make_shared<ModelSet>());
    TestActionBuilder::registerAction(make_shared<Tree>());
    TestActionBuilder::registerAction(make_shared<Null>());

    CacheLookup::instance();
    Hook::install();
}

static Agent agent;
