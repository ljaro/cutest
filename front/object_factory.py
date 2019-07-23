

def create_object(action_result):
    """
    :param action_result: can be any of: [], int, {type: 'model'}, {type: 'qobject'}
    :return:
    """
    if isinstance(action_result, list):
        result = []
        for itm in action_result:
            result.append(create_object(itm))
        return result

    if type(action_result) is dict and 'type' in action_result:
        if action_result['type'] == 'model':
            from model import QModel
            obj = QModel()
            obj.context = action_result['context']
            return obj

        from object import QQ
        new_obj = QQ()
        new_obj.context = action_result['context']
        return new_obj

    return action_result

