import socket as S
import json
import logging
logger = logging.getLogger(__name__)


class Connection:
    def __init__(self, socket):
        self._socket = socket
        pass

    @staticmethod
    def create(host='localhost', port=10010):
        socket = S.socket(S.AF_INET, S.SOCK_STREAM)
        socket.setsockopt(S.SOL_SOCKET, S.SO_REUSEADDR, 1)
        server_address = (host, port)
        socket.connect(server_address)
        instance = Connection(socket)
        return instance

    def send_message(self, message):
        t1 = json.dumps(message).encode('utf-8')
        t0 = bytes(len(t1).to_bytes(2, byteorder='little'))
        data = t0 + t1
        # logger.debug(data)
        self._socket.sendall(data)

    def read_message(self):
        try:
            data = self.receive()
            msg = json.loads(data.decode('utf-8'))
            return msg
        except:
            logger.critical('Decode error')
            raise

    def receive(self):
        chunks = []
        size_chunks = []
        bytes_recd = 0
        msg_len_size = 0
        msg_len_bytes = 3

        while msg_len_size < msg_len_bytes:
            chunk = self._socket.recv(min(msg_len_bytes - msg_len_size, 2048))
            if chunk == b'':
                raise RuntimeError("socket connection broken")
            size_chunks.append(chunk)
            msg_len_size = msg_len_size + len(chunk)

        msg_len = int.from_bytes(b''.join(size_chunks), byteorder='big')
        # logger.debug(msg_len)

        while bytes_recd < msg_len:
            chunk = self._socket.recv(min(msg_len - bytes_recd, 2048))
            if chunk == b'':
                raise RuntimeError("socket connection broken")
            chunks.append(chunk)
            bytes_recd = bytes_recd + len(chunk)
        return b''.join(chunks)


