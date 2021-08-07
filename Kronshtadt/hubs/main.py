from server import *
import time
import threading
import requests
import multiprocessing


def start_uav(slot):
    # {'orders': [{id: track}, {id: track}, {id: track}, ...], 'departure_hub_id': id, 'destination_hub_id': id}
    
    i = 0
    for next_hub, products in slot.dirs.items():
        #print("iter ", i)
        print("len products ", len(products))
        i = i + 1
        d = {"orders": [products], 'departure_hub_id': hubId[0], 'destination_hub_id': next_hub}
        print("Send")
        #js = json.dumps(d)
        #print("JJJJJJJJJJSSSSSSSON", js)
        r = requests.post("http://192.168.1.78:8000/api/orders", data=d)
        #r = requests.post("http://26.68.12.236:9999/api/orders", data=d)
        #print("status: ", r.status_code, r.reason)


def work():
    while True:
        time.sleep(0.01)
        t = time.time()
        key = None
        try:
            key = min(supply.keys())
            #if key != 0:
            #    print("key NOT 0 ", key, "; t: ", t)
        except BaseException:
            key = None
            continue
        try:
            if t >= key and key != None and key != 0:
                #print("-------")
                #print(" key: ", key)
                start_uav(supply[key])
                supply.pop(key)
                #print("-------")
        except BaseException:
            continue
            #print("err: t>=key")


def run_proc(port):
    thread = threading.Thread(target=work)
    thread.start()
    run(port - 11000, addr="localhost", port=port)


if __name__ == '__main__':
    proces = []
    for i in range(212):
        time.sleep(0.1)
        proces.append(multiprocessing.Process(target=run_proc, args=(11000 + i,)))
        proces[i].start()
    proces[0].join()

