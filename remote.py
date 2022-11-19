import http.client, urllib.parse
import keyboard as k

def httpCommand():
    params = urllib.parse.urlencode({'State': "L"})
    headers = {"Content-type": "application/x-www-form-urlencoded",
            "Accept": "text/plain"}
    conn = http.client.HTTPConnection("192.168.4.1")
    conn.request("POST", "", params, headers)
    response = conn.getresponse()
    print(response.status, response.reason)
    data = response.read()

    params = urllib.parse.urlencode({'State': "S"})
    conn = http.client.HTTPConnection("192.168.4.1")
    conn.request("POST", "", params, headers)
    response = conn.getresponse()
    print(response.status, response.reason)
    data = response.read()
    conn.close()

def main():
    pass

if __name__ == "__main__":
    main()