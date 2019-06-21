

def create_object(action_result):

    if 'type' in action_result:
        if action_result['type'] == 'model':
            from model import QModel
            obj = QModel()
            obj.context = action_result['context']
            return obj

    from object import QQ
    new_obj = QQ()
    new_obj.context = action_result['context']
    return new_obj

