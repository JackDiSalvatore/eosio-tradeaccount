# eosio-tradeaccount (WIP)

This contract allows eosio users to buy and sell account their names


Fee's would be paied out in a profiting sharing manner to token holders

---
# Overview

This contract allows eosio users to buy and sell accounts

Any private keys you see in this repository are for demo purposes only. For a real DApp NEVER expose the private keys.**

# Prerequisites

Make sure Docker and Node.js are installed

* Install Docker: https://docs.docker.com/docker-for-mac/install/
* Install Node.js: https://nodejs.org/en/

The DApp and eosio will occupy the ports 3000, 8888 and 9876. Make sure nothing else is already running on these ports.

Clone the repository:
```sh
git clone https://github.com/JackDiSalvatore/eosio-tradeaccount.git
```

The following guide assumes you are using macOS.

# Quick start - Run the DApp

In this section we provide a single command script to run all the commands needed to start both the blockchain and UI. For more detail on each component see the `Detailed guide` below.

**To start first time**
```sh
./quick_start.sh
```

**To start every time**
```sh
./start_eosio_docker.sh
```

**To stop**, press `ctrl+c` on your keyboard, and execute:
```sh
./stop_eosio_docker.sh
```
