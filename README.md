# Deprecated #

This repository is for a previous version of the Helium platform and is no longer being maintained. Please see  [Helium](https://helium.com) for information and updates on the latest product and related SDKs. 


# The Helium Command Line Interface (CLI)

The Helium CLI enables USB-connected [Helium Atoms](https://www.helium.com/dev/architecture/overview#atom-module) to connect, send data to, and receive data from the Helium network via the command line. This document covers the setup of the `helium-cli` tool, its basic funtionality, and demonstrated use.

* If you don't have any Helium hardware yet to use with this guide, head to the [Helium Store](https://helium.com/store) and pick up a [Helium Starter Kit with USB Adapter](https://www.helium.com/products/helium-starter-kit-eth). 
* Need support or have questions about this? Find us in the [Helium Developer Slack](http://chat.helium.com) or post a message to the [Helium Developer Forum](https://forum.helium.com).

--- 

* [Building the CLI](#setup)
* [CLI Commands](#commands)
* [CLI Examples](#cli-examples)
* [Further Reading](#further-reading)


## <h2 id="setup">Building the Helium CLI</h2>

Downloading and building the CLI is quick and painless. 

1. Clone this repo (and don't forget `--recursive`). 

``` 
$ git clone --recursive https://github.com/helium/helium-cli.git
```

2. Navigate into this directory and run `make`.

``` 
$ cd path/to/helium-cli
$ make 
```

3. Once this is complete, and your Atom is connected to your workstation via the USB Adapter, you can the test the connection by using the `info ` command and specifying the port you're connected. For example:
 
```
$ ./helium -p /dev/tty.usbserial-DN01IQCC info
```


## <h2 id="commands">CLI Commands</h2>

Once the CLI has been successfully built, running `./helium --help` will provide a list of options, commands, and descriptions which are also shown below.  

The format for a CLI command is:

```
$ ./helium [OPTIONS] [cmd [arg [arg ...]]]
```

### Top Level Commands

**connect** - Connect the Helium Atom to the Helium Network. Use `-f` to specify quick connect data from file. 
``` 
$ ./helium -p <port> connect
```

**connected** - Check if the Atom is connected. This will return `0` if connected.
``` 
$ ./helium -p <port> connected
```

**sleep** - Disconnect from the netork, saving quick connect data. 
``` 
$ ./helium -p <port> sleep
```

**info** - Print the Atom's information. 
``` 
$ ./helium -p <port> info
```

**reset** - Reset the Atom.
```
$ ./helium -p <port> reset
```

**create channel** - Create a channel with a given name. If the Channel was created successfully, it prints the ID - a number between `1` and `255`.
``` 
$ ./helium -p <port> channel create "Helium MQTT"" 
$ Channel created: 1
``` 

**channel send** - Sends the supplied text to the channel specified in the `id`. 
``` 
$ ./helium -p <port> channel send <id> [text]
```

**channel poll** - Poll the specified channel of a given `id` for all downlink data.
```
$ ./helium -p <port> channel poll <id>
```

**channel ping** - Ping the specified channel.
```
$ ./helium -p <port> channel ping <id>
``` 

**channel config** 

There are three variations for the `channel config` command: `get`, `set`, and `poll`. 

* `channel config get` - Get a channel configuration value for the given `id` and `key`.

```
$ ./helium -p <port> channel config get <id> <key>
``` 

* `channel config set` - Set a configuration value for a given key. Values must be expressed as JSON null, numbers (such as int and float), booleans (true or false), and strings.

```
$ ./helium -p <port> channel config set <id> <key> <value>
```

* `channel config poll` - Poll a channel configuration for a stale config indicator every 5 seconds. 

``` 
$ ./helium -p <port> channel config poll <id>
```

### CLI Command Options:

The following options (or "flags") can all be passed with applicable commands from above:

* `-h/--help` - This help message.
* `-p/--port` - The (USB) device to talk to.
* `-b/--baud` - The baud rate to use on the port (default 9600).
* `-f/--file` - The file to use for connect/sleep data or channel send.


## <h2 id="cli-examples">CLI Examples</h2>

Here are a series of commands and responses for standard CLI interface use. This example will connect, check connection, create channel, and send a string.

```
// Connect your Helium Atom to the Network via the "tty.usbserial-DN01IQCC" port 

$ ./helium -p /dev/tty.usbserial-DN01IQCC connect
```

```
// Check if you Helium Atom is connected to the Network while plugged into port "tty.usbserial-DN01IQCC" 

$ ./helium -p /dev/tty.usbserial-DN01IQCC connected
```

```
// Create a channel named "Helium MQTT" 

$ ./helium -p /dev/tty.usbserial-DN01IQCC channel create "Helium MQTT"
$ Channel created: 1
```

```
// Send the string "hello world" to the Helium MQTT Channel (which was given the ID "1" in the previous command).

$ ./helium -p /dev/tty.usbserial-DN01IQCC channel send 1 "hello world"
```

---

## <h2 id="further-reading">Further Reading and Support</h2>

* [Helium Developer Documenation](https://helium.com/dev)
* [Helium Developer Forum](https://forum.helium.com)
* [Heilum Developer Slack](http://chat.helium.com)
* [Buy Helium Hardware](http://helium.com/store)
