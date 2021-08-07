from http.server import BaseHTTPRequestHandler, HTTPServer
import socket
import json
import struct
import ast
import sys
import requests

sock = socket.socket()
sock.connect(('localhost', 12345))


def bytes_to_json(data):
    out = []
    while data:
        out.append(data[:4])
        data = data[4:]
    gg = []
    for aa in out:
        gg.append(struct.unpack('i', aa)[0])
    data = []
    while gg:
        data.append(gg[:3])
        gg = gg[3:]

    gg = {"Product_path":[]}

    for hub_it in data:
        d = {"HubID": hub_it[0], "Dst_time": hub_it[1], "Dep_time": hub_it[2]}
        gg["Product_path"].append(d)
    return gg


class S(BaseHTTPRequestHandler):
    def __init__(self, request, client_address, server):
        super().__init__(request, client_address, server)

    def _set_response(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        print("Body:\n%s\n", post_data.decode('ascii'))
        arr = dict(json.loads(post_data))
        for key in arr:
            try:
                d = arr[key]
                id1 = int(d["first_hub"])
                id2 = int(d["last_hub"])
                weight = float(d["weight"])
                print(weight, "  ", id1, "  ", id2, '\n')
                res = struct.pack("dii", weight, id1, id2)
            except KeyError:
                return
            self._set_response()

            sock.send(res)
            data = sock.recv(1024)
            data = bytes_to_json(data)
            self.wfile.write(json.dumps(data).encode('utf-8'))

            result = json.dumps({"weight": weight, "first_hub": id1, "last_hub": id2, "track": data})
            requests.post('http://192.168.1.78:8080/api/orders', json=result)


def run(server_class=HTTPServer, handler_class=S, ip='localhost'):
    print(ip)
    server_address = (ip, 10000)
    httpd = server_class(server_address, handler_class)
    print(' Starting httpd...\n')
    try:
        print(" Started!\n")
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    print('Stopping httpd...\n')


if __name__ == '__main__':
    print('Start logging\n')
    run()
