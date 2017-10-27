# Helium USB/CLI Interaface
## A command line interface for the Helium Atom module over USB

### Installation:
 
#### Clone and make this repository by opening a terminal and entering the following commands in order: 
1. git clone --recursive https://github.com/helium/helium-cli.git
2. make

### Usage

#### Confirm you are successfully connected and operating with the following terminal command:
	- "./helium -p < the-comm-port-you-connected-to > info" (e.g. ./helium -p /dev/tty.usbserial-DN01IQCC info)

#### Display command options:
	- "./helium -p < the-comm-port-you-connected-to > --help"

#### Connect, send, and poll data examples:
	- Connect "./helium -p <port> connect"
	- Check connection "./helium -p <port> connected" (returns 0 if connected)
	- Create channel "./helium -p <port> channel create "Helium MQTT"" (prints channel ID if successful)
	- Channel send "./helium -p <port> channel send <id> "hello world""
	- Channel poll "./helium -p <port> channel poll <id>"

### Further documentation and examples: 
