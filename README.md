# Debugger Detection Bypass
By default, frida's gadget will attach a debugger to the target process.\
To bypass or detect this behavior, we can use the following script to bypass the debugger detection and inject it before the app has the time to detect for a debugger.
```js
Interceptor.attach(Module.findExportByName(null, "ptrace"), function (args) {
    console.log("ptrace called");
});
```

# Default Port Bypass
Frida Server default listening port is 27042. In some case, an app will check if the port 27042 is open.\
To bypass or detect this behavior, we can change the listen port to another port.
```sh
frida-server -l 0.0.0.0:27043
```

# Port Scanning / Fingerprinting Bypass
By default, Frida communicates with the client using WebSocket. In some case, an app will pretend to be a WebSocket and act like a client.\
To bypass or detect this behavior, we can set the ORIGIN to any string.
```
# On frida-server
frida-server -l 0.0.0.0:27043 --origin=AKANE

# On client
frida-ps -H device_ip:27043 --origin=AKANE
```

Alternatively, we can set CERTIFICATE to make sure the frida communication is only performed in TLS.
```
# On frida-server
frida-server -l 0.0.0.0:27043 --certificate=certificate.pem

# On client
frida-ps -H device_ip:27043 --certificate=certificate.pem
```

# Artifacts Detection Bypass
When frida gadget is injected to a process, it will leave some Artifacts especially in the memory.\
To bypass or detect this behavior, we can use a customized frida-server such as:
- https://github.com/hzzheyang/strongR-frida-android

## Notes
It is recommended that you build the frida yourself so that any string artifacts are changed incase the developer already flagged the binaries in the repo release.

# Gadget Detection Bypass
There are lots of way to detect frida gadgets which might be overwhelming, one of them is Debugger Detection.\
But let's say found a way to inject a library to the target process, then you can use the frida's gumjs.\
It injects the script directly through the memory without needing a gadget.\

Compile [AndroGum](AndroGum) using NDK and use the library to inject a script.
## Note
Don't forget to download [libfrida-gumjs.a](https://github.com/frida/frida/releases) and put it in the same dir as Android.mk.
