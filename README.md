# Atom Command Line Interface (CLI)
***

The Atom CLI package enables USB connected devices to connect, send, and poll client transmissions through a terminal command line. 

This document covers the setup of the helium-cli tool, its basic funtionality, and demonstrated use.

* [Setting up Helium-CLI](#setup)
* [Commands via CLI](#commands)
* [Helium-CLI examples](#cli-examples)
* [Further Reading](#further-reading)


---

## <h2 id="setup">Setting up Helium-CLI</h2>

Downloading and building the Helium-CLI.

In Terminal:

1. Go to https://github.com/helium/helium-cli and clone the Helium-CLI repository.
  * "git clone --recursive https://github.com/helium/helium-cli.git" (don't forget --recursive)
2. Enter "cd helium-cli"
3. Enter "make"
4. Test the connection:
  * "./helium -p < the-comm-port-you-connected-to > info" (e.g. ./helium -p /dev/tty.usbserial-DN01IQCC info)


## <h2 id="commands">Commands via CLI</h2>

Once the client tool has been successfully built, running ./helium --help will provide a list of options, commands, and descriptions which are also shown below. The format for a CLI command is:
  * "./helium [OPTIONS] [cmd [arg [arg ...]]]"

  * Connect - Connect the Helium Atom to the network. Use -f to specify quick connect data from file. 
    - "./helium -p <port> connect"
  * Check connection - Check if the Atom is connected.
    -  "./helium -p <port> connected" (returns 0 if connected)
  * Sleep - Disconnect from the netork, saving quick connect data. 
   - "./helium -p <port> sleep"
  * Info - Print information of the Atom.
    - "./helium -p <port> info"
  * Reset - Reset the Atom.
    - "./helium -p <port> reset"
  * Create channel - Create a channel with a given name and prints the ID of the created channel.
    - "./helium -p <port> channel create "Helium MQTT"" (prints channel ID if successful)
  * Channel send - Sends the given text ("hello world!" format) to the channel specified in <id>. 
    - "./helium -p <port> channel send <id> [text]"
  * Channel poll - Poll the specified channel for all downlink data.
    - "./helium -p <port> channel poll <id>"
  * Channel ping - Ping the specified channel.
    - "./helium -p <port> channel ping <id>"
  * Channel config: 
    1. Get - Get a channel configuration value for the given key.
        - "./helium -p <port> channel config get <id> <key>"
    2. Set - Set a configuration value for a given key. Values must be expressed as JSON null, numbers (such as int and float), booleans (true or false), and strings.
        - "./helium -p <port> channel config set <id> <key> <value>"
    3. Poll - Poll a channel configuration for a stale config indicator every 5 seconds. 
        - "./helium -p <port> channel config poll <id>"

  * Options:
    1. -h/--help      This help message.
    2. -p/--port      The (USB) device to talk to.
    3. -b/--baud      The baud rate to use on the port (default 9600).
    4. -f/--file      The file to use for connect/sleep data or channel send.


## <h2 id="cli-examples">Helium-CLI example</h2>

Simply, the Helium-CLI is a basic direct interface to the Atom. It allows the user to alter low level parameters and monitor activity. 

### Simple Use Case Example

Here are a series of commands and responses for standard CLI interface use. This example will connect, check connection, create channel, and send a string.

```bash
./helium -p /dev/tty.usbserial-DN01IQCC connect
./helium -p /dev/tty.usbserial-DN01IQCC connected
$ ./helium -p /dev/tty.usbserial-DN01IQCC channel create "Helium MQTT"
Channel created: 1
./helium -p /dev/tty.usbserial-DN01IQCC channel send "hello world"
```

---

## <h2 id="further-reading">Further Reading</h2=>

**GitHub Source** 

* [Helium-CLI Repository](https://github.com/helium/helium-cli)

**USB Interface Hardware** 

* [Sparkfun XBee Dongle](https://www.sparkfun.com/products/11697) 


