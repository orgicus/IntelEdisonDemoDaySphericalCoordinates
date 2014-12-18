import tornado.ioloop
import tornado.web
import tornado.websocket
import socket
#import commands
from tornado.options import define, options, parse_command_line
from datetime import timedelta

# UDP settings to receive data from arduino
UDP_IP = "127.0.0.1"
UDP_PORT = 12345
# UDP socket
sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
# list of WS clients
WS_TIME = 0.04 # time between UDP->WS updates (in seconds)
clients = []

# WS defaults
define("port", default=8888, help="", type=int)
class WebSocketHandler(tornado.websocket.WebSocketHandler):
  def open(self, *args): #WS open
    print "Connected"
    clients.append(self) #append client to list
  def on_message(self, message):
    print (message)
  def on_close(self): #client disconnected
    print "Disconnected"
    clients.remove(self) #remove client from list
  def check_origin(self,origin):
    return True
app = tornado.web.Application([
    (r'/', WebSocketHandler),
])
# callback to check UDP data and forward it to the clients
def send_message_to_clients():
  try:
    udpdata,addr = sock.recvfrom(1024) #read udp data
    for client in clients:             #for each client
      client.write_message(udpdata)    #forward the udp data via WS
  finally:
    tornado.ioloop.IOLoop.instance().add_timeout(timedelta(seconds=WS_TIME),send_message_to_clients) #repeat the callback

if __name__ == '__main__':
    parse_command_line()
    app.listen(options.port)
    io_loop = tornado.ioloop.IOLoop.instance()
    io_loop.add_timeout(timedelta(seconds=WS_TIME),send_message_to_clients)
    print "starting WS server"
    io_loop.start()

