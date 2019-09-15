import uuid
import time
import json
from cutestpy.connection import Connection
from cutestpy.object_factory import create_object
import logging

logger = logging.getLogger(__name__)


class ResponseFormatError(Exception):
    pass


class TestFailed(Exception):
    pass


class WrongContext(Exception):
    pass


class ResponseMissing(Exception):
    pass


def do_action(conn, message, timeout=None, fail_no_context=True):
    message.update({"id": str(uuid.uuid1())})
    if timeout:
        message['timeout'] = timeout

    conn.send_message(message)
    response = conn.read_message()

    context, result = handle_response(response)

    if fail_no_context:
        if not context:
            raise WrongContext(context)

    return context, result


def handle_response(response):
    """
    1. checks response and raise exception if something missing
    2. checks status inside respone and raise exception if its not 'ok'
    3. checks context. must have value otherwise raise exception
    4. if all above ok then return context and result

    :param response:
    :return: context + result(if exists)
    """

    # print(json.dumps(response, indent=4, sort_keys=True))

    if not response:
        raise ResponseMissing('no response')
    if 'id' not in response:
        raise ResponseFormatError('missing id')
    if 'status' not in response:
        raise ResponseFormatError('missing status')
    if 'status' not in response['status']:
        raise ResponseFormatError('missing status detail')

    if response['status']['status'] != 'ok':
        status = response['status']['status']
        details = response['status']['details'] if 'details' in response['status'] else ''
        raise TestFailed(status, details)
    else:
        value = response['qobject'] if 'qobject' in response else response['simple'] if 'simple' in response else None
        ctx = response['context'] if 'context' in response else ''
        return ctx, value


class ObjectActions:
    _conn = None  # type: Connection
    _properties = {}

    def __init__(self):
        self.context = ""
        self._search_context = ""
        pass

    def wait_for(self, expr):
        """
        For given object_name wait for object until global test timeout expire.

        :param expr: correspond to QML component's id
        :return: context QQ or raise  TestFailed
        should return QQ with found context. If nothing found, timeout raise TestFailed exception
        """
        logger.info('wait_for {}'.format(expr))
        message = {}
        message.update({"cmd": "wait_for"})
        message.update({"context": 'frontend'})
        message.update({"params": {"exp": expr}})
        new_ctx, result = do_action(self._conn, message)

        logger.debug(' found (ok)')

        new_obj = create_object(result)
        return new_obj

    def wait_for_object(self, object_name, visible=True):
        return self.wait_for('objectName={0}&visible={1}'.format(object_name, str(visible).lower()))

    def wait_for_child(self, expr):
        """
        For given object_name wait for object until global test timeout expire.
        Searching scope is limited to children of context on which function was called.

        i.e wait_for('foo').wait_for_child('bar') will limit scope to children of foo

        Will return only one child

        :param expr: correspond to QML component's id
        :return: context QQ or raise  TestFailed
        should return QQ with found context. If nothing found, timeout raise TestFailed exception
        """
        logger.info('wait_for_child {}'.format(expr))
        message = {}
        message.update({"cmd": "wait_for_child"})
        message.update({"context": self.context})
        message.update({"params": {"exp": expr}})
        new_ctx, result = do_action(self._conn, message)

        logger.debug(' found (ok)')
        new_obj = create_object(result)
        return new_obj

    def wait_for_child_object(self, object_name, visible=True):
        return self.wait_for_child('objectName={0}&visible={1}'.format(object_name, str(visible).lower()))

    def wait_check(self, expr, timeout=5):
        """
        For given object_name wait for object until timeout expire.
        If timeout expire will return False as object was not found.
        If object found before timeout expire returns True.

        global test timeout is ignored for this action i.e timeout=10000 will make wait test until is finished
        regardless of test timeout which could be 5 seconds

        Can be used to check whether object exists or not without failing test

        :param object_name:
        :param visible:
        :param timeout:
        :return: True or False depending whether object was found
        """
        logger.info('wait_for_check {}'.format(expr))
        message = {}
        message.update({"cmd": "wait_check"})
        message.update({"context": "frontend"})
        message.update({"params": {"timeout": timeout, "exp": expr}})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)

        return create_object(result)

    def wait_check_child(self, expr, timeout=5):
        """
        For given object_name wait for object until timeout expire.
        If timeout expire will return False as object was not found.
        If object found before timeout expire returns True.

        global test timeout is ignored for this action i.e timeout=10000 will make wait test until is finished
        regardless of test timeout which could be 5 seconds

        Can be used to check whether object exists or not without failing test

        :param object_name:
        :param visible:
        :param timeout:
        :return: True or False depending whether object was found
        """
        logger.info('exists_child {}'.format(expr))
        message = {}
        message.update({"cmd": "wait_check_child"})
        message.update({"context": self.context})
        message.update({"params": {"timeout": timeout, "exp": expr}})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)

        return create_object(result)

    def click(self, hold_time=0):
        """
        Click left mouse button on context on which is invoked. After success returns same context which can be used for
        commands chaining. In case of clicking on non existing object/context, exception is raised.
        This keeps explicit feedback on miss-clicked objects, but is problematic on dynamic object i.e ListView delegate
        which is dynamically created. In such case its up to test to wait_for such newly created object and not keep
        old one.

        :param hold_time: mouse left button hold time
        :return: QQ with context on which was invoked. TestFailed may be raised if context on with clicked was not found
        """
        logger.info('click {}'.format(self._search_context))
        message = {}
        message.update({"cmd": "click"})
        message.update({"context": self.context})
        message.update({"params": {"hold_time": hold_time}})
        do_action(self._conn, message)
        return self

    def mouse_press(self):
        logger.info('mouse_press {}'.format(self._search_context))
        message = {}
        message.update({"cmd": "mouse_action"})
        message.update({"context": self.context})
        message.update({"params": {"type": "mouse_press"}})
        do_action(self._conn, message)
        return self

    def mouse_release(self):
        logger.info('mouse_release {}'.format(self._search_context))
        message = {}
        message.update({"cmd": "mouse_action"})
        message.update({"context": self.context})
        message.update({"params": {"type": "mouse_release"}})
        do_action(self._conn, message)
        return self

    def mouse_move(self, x, y):
        logger.info('mouse_move {}'.format(self._search_context))
        message = {}
        message.update({"cmd": "mouse_action"})
        message.update({"context": self.context})
        message.update({"params": {"type": "mouse_move", "x": x, "y": y}})
        do_action(self._conn, message)
        return self

    def wait(self, seconds):
        logger.info('wait {}'.format(seconds))
        time.sleep(seconds)
        logger.info(' done')
        return self

    def hi(self, msec):
        message = {}
        message.update({"cmd": "highlight"})
        message.update({"context": self.context})
        message.update({"params": {"msec": msec}})
        do_action(self._conn, message)
        return self

    def write(self, text):
        message = {}
        message.update({"cmd": "write"})
        message.update({"context": self.context})
        message.update({"params": {"text": text}})
        do_action(self._conn, message)
        return self

    def find_sibling(self, expr, ancestor_object_name):
        """
        Use to find item that has same ancestor as current object
        :param expr: i.e 'objectName=btn1&visible=true'
        :param ancestor_object_name: objectName of ancestor
        :return:
        """
        logger.info('find_sibling {} {}'.format(expr, ancestor_object_name))
        message = {}
        message.update({"cmd": "find_sibling"})
        message.update({"context": self.context})
        message.update({"params": {"exp": expr, "ancestor": ancestor_object_name}})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)

        new_obj = create_object(result)

        logger.debug('ok')

        return new_obj

    def hit_key(self, key_name):
        logger.info('find_sibling {}'.format(key_name))
        message = {}
        message.update({"cmd": "hit_key"})
        message.update({"context": self.context})
        message.update({"params": {"key_name": key_name}})
        do_action(self._conn, message)
        logger.debug('ok')
        return self

    def set(self, property_name, property_value):
        logger.info('set property {} to {}'.format(property_name, property_value))
        message = {}
        message.update({"cmd": "set"})
        message.update({"context": self.context})
        message.update({"params": {"property_name": property_name, "property_value": property_value}})
        do_action(self._conn, message)
        logger.debug('ok')
        return self

    def get(self, property_name):
        """
        Makes query to application to get value of property
        Values is returned from this method and it is stored inside context object
        Accessing properties from context object doesn't make query to application and can be out of date
        :param property_name:
        :return: property value
        """
        logger.info('get property {}'.format(property_name))
        message = {}
        message.update({"cmd": "get"})
        message.update({"context": self.context})
        message.update({"params": {"property_name": property_name}})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)
        logger.debug('ok')

        if new_ctx == self.context:
            self._properties = result['properties']
            return self._properties[property_name] if property_name in self._properties else None
        else:
            # if context changed it means that property was new object
            return create_object(result)

    def next(self):
        """
        Use to find item that has same ancestor as current object
        :param expr: i.e 'objectName=btn1&visible=true'
        :param ancestor_object_name: objectName of ancestor
        :return:
        """
        logger.info('next ')
        message = {}
        message.update({"cmd": "next"})
        message.update({"context": self.context})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)

        new_obj = create_object(result, '')

        logger.debug('ok')

        return new_obj

    def invoke(self, method_name, arg1=None, arg2=None, arg3=None, arg4=None, arg5=None):
        message = {}
        message.update({"cmd": "invoke"})
        message.update({"context": self.context})
        args = {"method": method_name}

        if arg1 is not None:
            args.update({"arg1": arg1})

        if arg2 is not None:
            args.update({"arg2": arg2})

        if arg3 is not None:
            args.update({"arg3": arg3})

        if arg4 is not None:
            args.update({"arg4": arg4})

        if arg5 is not None:
            args.update({"arg5": arg5})

        message.update({"params": args})

        new_ctx, result = do_action(self._conn, message, fail_no_context=True)

        new_obj = create_object(result)

        logger.debug('ok')

        return self

    def print_object(self, item, max_depth, depth=0):
        object_name = item['objectName'].strip() if item['objectName'].strip() else '<empty>'
        class_name = item['className'].strip() if item['className'].strip() else '<empty>'
        type_name = item['typeName'].strip() if item['typeName'].strip() else '<empty>'

        if depth == 0:
            print("+-- {} ({}) - {}".format(object_name, class_name, type_name))
        else:
            print("|{}+-- {} ({}) - {}".format(" " * ((depth * 3) + (depth - 1)), object_name, class_name, type_name))

        if len(item['children']) > 0 and depth > max_depth:
            print("|{}<...has children...>".format(" " * (((depth + 1) * 3) + (depth))))
            return

        for itm in item['children']:
            self.print_object(itm, max_depth, depth + 1)

    def tree(self, max_depth=3):
        message = {}
        message.update({"cmd": "tree"})
        message.update({"context": self.context})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)
        self.print_object(result, max_depth, 0)

