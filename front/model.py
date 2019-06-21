from actions import do_action
from object import QQ


class QModel(QQ):

    def __init__(self):
        super(QModel, self).__init__()

    def role_names(self):
        print('get model''s value ', end='', flush=True)
        message = {}
        message.update({"cmd": "role_names"})
        message.update({"context": self.context})
        new_ctx, result = do_action(self._conn, message)
        print('ok')

    def value(self, role, idx):
        print('get model''s value ' + role + ' ' + str(idx), end='', flush=True)
        message = {}
        message.update({"cmd": "model_value"})
        message.update({"context": self.context})
        message.update({"params": {"role": role, "index": idx}})
        new_ctx, result = do_action(self._conn, message)
        print('ok')

    def values(self, role):
        pass

