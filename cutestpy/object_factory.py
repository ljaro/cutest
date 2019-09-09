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
            from cutestpy.model import QModel
            new_obj = QModel()
        else:
            from cutestpy.object import QQ
            new_obj = QQ()

        new_obj.context = action_result['context']
        new_obj.type = action_result['type']
        new_obj.properties = action_result['properties']
        new_obj.object_name = action_result['objectName']
        new_obj.class_name = action_result['className']

        return new_obj

    return action_result
