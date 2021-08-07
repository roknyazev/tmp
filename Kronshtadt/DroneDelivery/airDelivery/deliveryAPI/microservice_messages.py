import requests
import json

math_module_url = "http://localhost"
math_module_port = "10000"
hub_url = "http://localhost"
hub_base_port = "11000"


def get_order_track_from_math_module(order_weight, departure_point, destination_point):
    request_url = math_module_url + ":" + math_module_port
    request_body = {'weight': float(order_weight), 'first_hub': int(departure_point), 'last_hub': int(destination_point)}
    result = json.loads(requests.post(request_url, json=json.dumps(request_body)).content)
    print('ответ матмодуля', result)
    return result


def start_shipping_message(hub, order_id, track):
    print("shipping started")
    port = str(int(hub_base_port) + int(hub.hub_id))
    request_url = hub_url + ":" + port
    request_body = {'order_id': int(order_id), 'order_track': track}
    result = requests.post(request_url, json=json.dumps(request_body))
    print('ответ хаба', result)
