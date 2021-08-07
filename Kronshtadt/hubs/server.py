
from http.server import BaseHTTPRequestHandler, HTTPServer
import ast
import json
import requests

hubId = []
supply = {}


class ScheduleSlot:
    def __init__(self, t):
        self.time = t
        self.dirs = {}

    def add_product(self, product):
        try:
            self.dirs[product.dst_id].append({product.oid: product.track})
            #print("append product!!!!!!!!!!!!!!")
            #print("len!!!    ",len(self.dirs[product.dst_id]))
        except KeyError:
            self.dirs[product.dst_id] = [{product.oid: product.track}]


class Product:
    def __init__(self, order_id, track):
        self.oid = order_id
        self.track = track
        path = self.track["Product_path"]
        self.dst_id = None
        self.dep_time = None
        flag = 0
        for node in path:
            if flag == 1:
                self.dst_id = int(node["HubID"])
                break
            if int(node["HubID"]) == hubId[0]:
                self.dep_time = int(node["Dep_time"])
                flag = 1
        #print("Флаг: ", flag, " self.dst_id: ", self.dst_id, " self.dep_time: ", self.dep_time, " hubId[0]: ", hubId[0])


def parse(post_data):  # {'order_id': order_id, 'order_track': order_track}
    d = dict(ast.literal_eval(json.loads(post_data)))
    print(d)
    oid = int(d['order_id'])
    tmp = d['order_track']
    track = dict(ast.literal_eval(tmp))
    return Product(oid, track)


class S(BaseHTTPRequestHandler):
    def __init__(self, request, client_address, server):
        super().__init__(request, client_address, server)

    def _set_response(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_POST(self):
        #print("\nPOST\n")
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        #print("DATA: ", post_data)
        new_product = parse(post_data)
        if new_product.dst_id != None:

            try:
                supply[new_product.dep_time].add_product(new_product)
                #print(" supl add ", len(supply[new_product.dep_time]))
            except KeyError:
                supply[new_product.dep_time] = ScheduleSlot(new_product.dep_time)
                supply[new_product.dep_time].add_product(new_product)
                #print(" supl new", len(supply[new_product.dep_time]))
            self._set_response()

            self.wfile.write("Order received".encode('utf-8'))
            #print("send NOT finish, oid: ", new_product.oid)
        else:
            self._set_response()
            self.wfile.write("Order finished".encode('utf-8'))    
            #print("before send")
            r = requests.post('http://192.168.1.78:8080/api/orders/' + str(new_product.oid) +  '/finish', data = '')
            #print("send finish, oid: ", new_product.oid)




def run(ahub_id, server_class=HTTPServer, handler_class=S, addr='localhost', port=8080):
    hubId.append(ahub_id)
    server_address = (addr, port)
    httpd = server_class(server_address, handler_class)
    print(' Starting httpd...  Port: ', port, 'Id_hub:', hubId[0])
    try:
        print(" Started!\n")
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    print('Stopping httpd...\n')

