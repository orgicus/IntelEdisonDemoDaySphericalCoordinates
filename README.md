Intel Edison Demo Day Spherical Coordinates
===========================================

![Spherical Coordinates](https://github.com/orgicus/IntelEdisonDemoDaySphericalCoordinates/raw/master/spherical_coords.png)

This project was started at the [Intel Edison Demo Day](http://designswarm.com/makingitwithedison/)
workshop held by [design swarm](http://designswarm.com).

It's a standalone demo: it can work without an internet connection since the Edison can also
act as a WiFi Access Point. This demo can also be coded using the [iotkit](https://github.com/enableiot/iotkit-agent).

The current implementation however has the following components:
 1. An Arduino sketch that reads sensor data and sends it to localhost via UDP 
 2. A Python script which receives the sensor data via UDP, but also acts as a websocket server thus forwarding the UDP data to the browser.
 3. An html page which is the websocket client, reading sensor data, then mapping it as parameters for a 3D sphere rendered in a 2D canvas
(which can be viewed on computers and smartphones as well)

Setup
=====

The Python script uses [Tornado](http://www.tornadoweb.org/en/stable/) to keep the websocket part minimal.
You may need to install the following depenencies for Tornado:
 - [setuptools](https://pypi.python.org/packages/source/s/setuptools/setuptools-8.0.4.tar.gz#md5=26cf2e8fef0c89bd3e5f62a6158023c3)
 - [backports ssl match hostname](https://pypi.python.org/packages/source/b/backports.ssl_match_hostname/backports.ssl_match_hostname-3.4.0.2.tar.gz)

An example how to set these up:
```
wget https://pypi.python.org/packages/source/s/setuptools/setuptools-8.0.4.tar.gz#md5=26cf2e8fef0c89bd3e5f62a6158023c3 --no-check-certificate
tar xvzf setuptools-8.0.4.tar.gz
cd setuptools-8.0.4
python setup.py install
cd ..
rm -R setuptools-8.0.4

wget https://pypi.python.org/packages/source/b/backports.ssl_match_hostname/backports.ssl_match_hostname-3.4.0.2.tar.gz --no-check-certificate
tar xvzf backports.ssl_match_hostname-3.4.0.2.tar.gz
cd backports.ssl_match_hostname-3.4.0.2
python setup.py install
cd ..
rm -R backports.ssl_match_hostname-3.4.0.2

wget https://pypi.python.org/packages/source/t/tornado/tornado-4.0.2.tar.gz --no-check-certificate
tar xvzf tornado-4.0.2.tar.gz
cd tornado-4.0.2
python setup.py install
cd ..
rm -R tornado-4.0.2

```

I've placed the files in ```/usr/lib/edison_config_tools/public``` which is probably not ideal, but a quick
public folder to drop some files because Edison's already running an HTTP server at that location.
Feel free to use other public http server location if you like.

In terms of transferind files I've found [FileZilla](https://filezilla-project.org/download.php?type=client) as a quick solution. 
Simply setup a new site pointing to your edison, using SFTP as the transfer protocol, "Ask for password as the logon type" with username ```root```.

One that's setup run the WS server:
```
python wsserver.py
```
Double check that the address to the server is correct in wsclient.html:
```
var ws = new WebSocket ("ws:edison.local:8888");
```
If the client support namespaces (IOS/Apple device, Windows with Bonjour Service running, Linux with avahi installed, etc.)
you should be able to use the hostname, which by default is edison.local, unles explicitly changed.
You can also ue the IP address which you can find by running
```
configure_edison --showWiFiIP
```
via ssh or Serial

Upload the Arduino sketch if you haven't done so already.

If all these are setup you should be able the client html page on your smartphone or computer's browser 
either by using [edison's host name](http://edison.local/wsclient.html) or it's IP (e.g. ```http://192.168.X.X/wsclient.html``` where ```X```s are the rest of the ip address to your Edison)
