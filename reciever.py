from flask import Flask, request
import os

app = Flask(__name__)

@app.route('/receive_image', methods=['POST'])
def receive_image():
    img_data = request.data  # Get the raw image data
    img_filename = "received_image.jpg"
    
    # Save the image to disk
    with open(img_filename, 'wb') as img_file:
        img_file.write(img_data)
    
    return "Image received successfully", 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080)  # Make sure port matches ESP32 server port
