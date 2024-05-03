
from flask import Flask, jsonify, request
import random

app = Flask(__name__)
humi = 50
temp = 35

@app.route('/api/data', methods=['GET'])
def get_data():
    global humi,temp
    data = {
        'humi': humi,
        'temp': temp,
    }
    humi = random.randint(0, 100)
    temp = random.randint(-20, 50)
    return jsonify(data)

@app.route('/api/temp', methods=['GET'])
def get_temp():
    global humi,temp
    data = [1,2,5,8,10,9,0,-4]
    return jsonify(data)

@app.route('/api/change', methods=['POST'])
def post_change():
    text = request.data.decode('utf-8')
    print('receive text:', text)
    return "OK"

@app.route('/esp8266', methods=['POST'])
def upload_file():
    data = request.json
    global humi,temp
    temp = data['temp']
    humi = data['humi']
    print('recv: temp:', temp, 'humi:', humi)
    return "OK"

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=12345)
