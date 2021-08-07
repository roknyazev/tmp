from django.http import HttpResponse
import json
from .models import ORDER, BPLA, HUB, STATISTICS
import ast
from . import microservice_messages as messages
from django.views.decorators.csrf import csrf_exempt
from . import db_interactions as database
import requests
from ast import literal_eval

@csrf_exempt
def manage_orders(request):
    # Добавление нового заказа
    if request.method == "POST":
        print("AAAAAAAAAAAAAAAAAAAA: ", request.body)
        order_params = eval(json.loads(request.body))
        print(order_params)
        # Запись заказа в БД
        new_order = database.create_order(order_params)
        print("заказ создан", new_order.order_id)
        # Передача данных о заказе для дальнейшей обработки другими модулями
        messages.start_shipping_message(new_order.cur_departure, new_order.order_id, new_order.track)
        print("заказ создан")
        return HttpResponse(json.dumps({'result': 'success', 'id': new_order.order_id}), content_type="application/json")


@csrf_exempt
def manage_single_order(request, order_id):
    # Обновление данных по конкретному заказу
    if request.method == "PUT":
        order_data = ast.literal_eval(json.loads(request.read().decode('utf-8')))
        order = ORDER.objects.get(order_id=order_id)
        order.bpla = BPLA.objects.get(board_number=str(order_data['bpla']))
        order.cur_departure = HUB.objects.get(hub_id=(int(order_data['dep_hub_id'])))
        order.cur_destination = HUB.objects.get(hub_id=(int(order_data['dest_hub_id'])))
        print("прошлый айди", order.order_id)
        order.save()
        return HttpResponse("<h1>Successfully added new order data to DB.</h1>")

    # Отправка данных по конкретному заказу
    elif request.method == "GET":
        order = ORDER.objects.get(order_id=order_id)
        order_json = {"weight": order.weight,
                      "cur_departure": order.cur_departure,
                      "cur_destination": order.cur_destination,
                      "bpla": order.bpla,
                      "track": order.track,
                      "start_time": order.start_time}
        order_json = json.dumps(order_json)
        return HttpResponse(order_json, content_type="application/json")


@csrf_exempt
def manage_drone_fleet(request):
    # Передача данных о текущих параметрах всех беспилотных аппаратов, находящихся в воздухе в данный момент
    if request.method == "GET":
        drone_fleet = BPLA.objects.all()
        drone_fleet_params = {'drones': []}
        for uav in drone_fleet:
            dest_hub = HUB.objects.get(hub_id=uav.destination_hub)
            drone_fleet_params['drones'].append({'id': uav.board_number,
                                                 'type': uav.type,
                                                 'lat': uav.latitude,
                                                 'lon': uav.longitude,
                                                 'speed': uav.speed,
                                                 'azimuth': uav.azimuth,
                                                 'lat_delta': uav.lat_delta,
                                                 'long_delta': uav.long_delta,
                                                 'destination_hub_lat': float(dest_hub.latitude),
                                                 'destination_hub_long': float(dest_hub.longitude)})
        return HttpResponse(json.dumps(drone_fleet_params), content_type="application/json")

    # Добавление нового беспилотного аппарата в БД
    elif request.method == "POST":
        uav_params = request.POST
        database.create_uav(uav_params)
        return HttpResponse("<h1>Successfully added new UAV</h1>")

    # Удаление указанных беспилотных аппаратов
    elif request.method == "DELETE":
        uav_deleting_ids = request.body.decode('utf-8')
        uav_deleting_ids = ast.literal_eval(uav_deleting_ids)
        try:
            uav_deleting_ids = ast.literal_eval(uav_deleting_ids)
            print(uav_deleting_ids, type(uav_deleting_ids))
        except ValueError:
            for i in range(100):
                print('удалены все дроны')
            BPLA.objects.all().delete()
            return HttpResponse("<h1>Successfully erased all active UAVs.</h1>")
        response_str = ""
        for uav_id in uav_deleting_ids['id']:
            response_str += str(uav_id)
            response_str += "  "
            BPLA.objects.filter(board_number=uav_id).delete()
            print("отвечаю базе", uav_id)
            print()
        if len(uav_deleting_ids['id']) == 0:
            return HttpResponse("<h1>No uavs to delete.</h1>")
        return HttpResponse("<h1>Successfully erased all mentioned UAVs.</h1>" + str(response_str))

    # Обновление параметров всех находящихся в работе в данный момент времени беспилотных аппаратов
    elif request.method == "PUT":
        uav_updated_params = request.body.decode('utf-8')
        uav_updated_params = ast.literal_eval(ast.literal_eval(uav_updated_params))
        statistics = STATISTICS.objects.all()[0]
        for uav in BPLA.objects.all():
            try:
                statistics.total_distance_travelled += uav_updated_params[str(uav.board_number)]['distance']
                uav.speed = uav_updated_params[str(uav.board_number)]['speed']
                uav.latitude = uav_updated_params[str(uav.board_number)]['latitude']
                uav.longitude = uav_updated_params[str(uav.board_number)]['longitude']
                uav.azimuth = uav_updated_params[str(uav.board_number)]['azimuth']
                uav.lat_delta = uav_updated_params[str(uav.board_number)]['lat_delta']
                uav.long_delta = uav_updated_params[str(uav.board_number)]['long_delta']
                uav.save()
            except KeyError:
                pass
        statistics.save()
        return HttpResponse("<h1>Successfully updated all uav data.</h1>")


@csrf_exempt
def manage_single_uav(request, uav_id):
    # Обовление параметров конкретного беспилотного аппарата
    if request.method == "PUT":
        uav_updated_params = ast.literal_eval(request.body.decode('utf-8'))
        uav = BPLA.objects.get(id=uav_id)
        uav.speed = uav_updated_params['speed']
        uav.latitude = uav_updated_params['latitude']
        uav.longitude = uav_updated_params['longitude']
        uav.azimuth = uav_updated_params['azimuth']
        uav.save()
        return HttpResponse("<h1>Successfully updated uav data.</h1>")

    # Удаление конкретного беспилотного аппарата
    elif request.method == "DELETE":
        uav = BPLA.objects.get(id=uav_id)
        for i in range(40):
            print()
        print('дрон', uav_id, 'удален')
        for i in range(40):
            print()
        uav.delete()
        return HttpResponse("<h1>Successfully deleted uav from DB.</h1>")


@csrf_exempt
def manage_hubs(request):
    # Создание в БД хабов для кейса Якутска в режиме пресета
    if request.method == "POST":
        database.manual_hubs_creation()
        return HttpResponse("Successfully added hubs data to DB.")

    # Удаление всех данных хабов из БД
    elif request.method == "DELETE":
        HUB.objects.all().delete()
        return HttpResponse("Successfully erased all hub data.")

    # Отправка информации по всем хабам
    elif request.method == "GET":
        response_body = dict()
        for hub in HUB.objects.all():
            hub_data = {int(hub.hub_id): {'id': int(hub.hub_id),
                                          'type': hub.type,
                                          'workload': hub.workload,
                                          'latitude': hub.latitude,
                                          'longitude': hub.longitude}}
            response_body.update(hub_data)
        return HttpResponse(json.dumps(response_body), content_type="application/json")


@csrf_exempt
def manage_single_hub(request, hub_id):
    # Отправка данных по конкретному хабу
    if request.method == "GET":
        hub = HUB.objects.get(hub_id=hub_id)
        print(hub, type(hub))
        hub_data = {'id': int(hub.hub_id),
                    'type': hub.type,
                    'workload': hub.workload,
                    'latitude': hub.latitude,
                    'longitude': hub.longitude}
        return HttpResponse(json.dumps(hub_data), content_type="application/json")


@csrf_exempt
def manage_orders_carried_by_uav(request, drone_id):
    if request.method == "GET":
        uav = BPLA.objects.get(board_number=drone_id)
        orders_carried = ORDER.objects.filter(bpla=uav)
        response_body = {'id': []}
        total_weight = 0
        for order in orders_carried:
            response_body['id'].append(order.id)
            total_weight += order.weight
        response_body.update({'total_weight': total_weight})
        return HttpResponse(json.dumps(response_body), content_type="application/json")


@csrf_exempt
def manage_order_track(request, order_id):
    if request.method == "GET":
        order = ORDER.objects.get(order_id=order_id)
        track_dict = ast.literal_eval(order.track)
        print(track_dict)
        for point in track_dict['Product_path']:
            print(point['HubID'])
            hub = HUB.objects.get(hub_id=point['HubID'])
            point.update({'Latlong': [float(hub.latitude), float(hub.longitude)]})
        response_body = {'track': track_dict}
        return HttpResponse(json.dumps(response_body), content_type="application/json")


@csrf_exempt
def manage_uav_track(request, drone_id):
    if request.method == "GET":
        uav = BPLA.objects.get(board_number=drone_id)
        departure_hub = HUB.objects.get(hub_id=uav.departure_hub)
        destination_hub = HUB.objects.get(hub_id=uav.destination_hub)
        response_body = {'departure_hub': [float(departure_hub.latitude), float(departure_hub.longitude)],
                         'destination_hub': [float(destination_hub.latitude), float(destination_hub.longitude)]}
        return HttpResponse(json.dumps(response_body), content_type="application/json")


@csrf_exempt
def apply_acceleration(request):
    if request.method == "POST":
        acceleration = json.loads(request.body)
        request_url = "http://192.168.1.78:8000/api/acceleration"
        request_body = {'multiplier': acceleration['multiplier']}
        requests.post(request_url, json=json.dumps(request_body))
        return HttpResponse('success')


@csrf_exempt
def order_finishing(request, order_id):
    if request.method == "POST":
        order = ORDER.objects.get(order_id=order_id)
        statistics = STATISTICS.objects.all()[0]
        print(statistics.total_weight_delivered, order.weight)
        statistics.total_weight_delivered = statistics.total_weight_delivered + order.weight
        print(statistics.total_weight_delivered)
        statistics.save()
        return HttpResponse('success')


@csrf_exempt
def get_statistics(request):
    if request.method == "GET":
        statistics = STATISTICS.objects.all()[0]
        uavs = len(BPLA.objects.all())
        response_body = {'weight': statistics.total_weight_delivered, 'distance': statistics.total_distance_travelled,
                         'uavs': uavs}
        print(response_body)
        return HttpResponse(json.dumps(response_body), content_type="application/json")
    elif request.method == "DELETE":
        statistics = STATISTICS.objects.all()[0]
        statistics.total_weight_delivered = 0
        statistics.total_distance_travelled = 0
        statistics.save()
        return HttpResponse('success')


@csrf_exempt
def create_statistics(request):
    statistics = STATISTICS(total_weight_delivered=0,
                            current_uav_quantity=0,
                            total_distance_travelled=0)
    statistics.save()
    return HttpResponse('success')
