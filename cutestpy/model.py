from cutestpy.actions import do_action
from cutestpy.object import QQ
from cutestpy.object_factory import create_object
import logging

logger = logging.getLogger(__name__)

class QModel(QQ):

    def __init__(self):
        super(QModel, self).__init__()

    def role_names(self):
        logger.info('get model''s value ')
        message = {}
        message.update({"cmd": "role_names"})
        message.update({"context": self.context})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)
        logger.debug('ok')
        return create_object(result)

    def value(self, role, idx):
        logger.info('get model''s value ' + role + ' ' + str(idx))
        message = {}
        message.update({"cmd": "model_value"})
        message.update({"context": self.context})
        message.update({"params": {"role": role, "index": idx}})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)
        logger.debug('ok')
        return create_object(result)

    def values(self, role):
        logger.info('get model''s values ' + role)
        message = {}
        message.update({"cmd": "model_value"})
        message.update({"context": self.context})
        message.update({"params": {"role": role}})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)
        logger.debug('ok')
        return create_object(result)

    def set_value(self, idx, value, role = ''):
        logger.info('get model''s values ' + role)
        message = {}
        message.update({"cmd": "model_set"})
        message.update({"context": self.context})
        message.update({"params": {"role": role, "index": idx, "value": value}})
        new_ctx, result = do_action(self._conn, message, fail_no_context=False)
        logger.debug('ok')

