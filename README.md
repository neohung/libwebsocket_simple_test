Websocket simple test
==================
  這是一個Websocket在Openshift上的測試程式

OpenShift
---------

### Usage

```
$ rhc app create your_app_name nodejs-0.10
Application Options
-------------------
Domain:     XXXXXXX
Cartridges: nodejs-0.10
Gear Size:  default
Scaling:    no

Creating application 'your_app_name' ... done

Your application 'your-app-name' is now available.

  URL:        http://your-app-name-XXXXXXXX.rhcloud.com/
  ...
  Git remote: ssh://54b5e30d4382ec020700010d@your-app-name-XXXXXXXX.rhcloud.com/~/git/your-app-name.git/
  ...
```

Add openshift remote name
```
$ git remote add openshift ssh://54b5e30d4382ec020700010d@your-appn-ame-XXXXXXXX.rhcloud.com/~/git/your-app-name.git
$ git push openshift -f
```

Restart App if needed
```
rhc app restart -a your-app-name
```

Check log by:
```
$ rhc tail -a your-app-name
```

Client
---------
### Modify src/client.html
change "127.0.0.1" to your openshift domain
```
var websocket = new WebSocket('ws://your-openshift-domain:8000', null);
```

### Modify src/client.c
change "127.0.0.1" to your openshift domain
```
wsi = libwebsocket_client_connect(context,
                                  "your-openshift-domain",
                                  8000,
                                  0,         
                                  "/",         
                                  "your-openshift-domain", 
                                  "your-openshift-domain", 
                                  NULL, -1);
```
