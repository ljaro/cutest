from cutestpy.actions import ObjectActions


class QQ(ObjectActions):

    object_name = None
    class_name = None

    def __init__(self):
        super(QQ, self).__init__()

    def __repr__(self):
        return 'QQ(' + str(self.object_name) + '/' + str(self.class_name) + ')'
