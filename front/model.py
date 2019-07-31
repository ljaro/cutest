from actions import do_action
from object import QQ
from object_factory import create_object


class QModel(QQ):

    def __init__(self):
        super(QModel, self).__init__()

    def role_names(self):
        print('get model''s value ', end='', flush=True)
        message = {}
        message.update({"cmd": "role_names"})
        message.update({"context": self.context})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)
        print('ok')
        return create_object(result)

    def value(self, role, idx):
        print('get model''s value ' + role + ' ' + str(idx), end='', flush=True)
        message = {}
        message.update({"cmd": "model_value"})
        message.update({"context": self.context})
        message.update({"params": {"role": role, "index": idx}})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)
        print('ok')
        return create_object(result)

    def values(self, role):
        print('get model''s values ' + role, end='', flush=True)
        message = {}
        message.update({"cmd": "model_value"})
        message.update({"context": self.context})
        message.update({"params": {"role": role}})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)
        print('ok')
        return create_object(result)

    def set_value(self, idx, value, role = ''):
        print('get model''s values ' + role, end='', flush=True)
        message = {}
        message.update({"cmd": "model_set"})
        message.update({"context": self.context})
        message.update({"params": {"role": role, "index": idx, "value": value}})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)
        print('ok')

