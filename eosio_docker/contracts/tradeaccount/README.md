# tradeaccount

https://eosio.stackexchange.com/questions/1661/how-to-call-action-of-another-contract-within-a-contract

Accounts

```bash
cleos get account tradeaccount

permissions:
     owner     1:    1 EOS8BCgapgYA2L4LJfCzekzeSr3rzgSTUXRXwNi8bNRoz31D14en9
        active     1:    1 EOS8BCgapgYA2L4LJfCzekzeSr3rzgSTUXRXwNi8bNRoz31D14en9, 1 tradeaccount@eosio.code
```

```bash
cleos get account account4sale

permissions:
     owner     1:    1 EOS78RuuHNgtmDv9jwAzhxZ9LmC6F295snyQ9eUDQ5YtVHJ1udE6p
        active     1:    1 EOS78RuuHNgtmDv9jwAzhxZ9LmC6F295snyQ9eUDQ5YtVHJ1udE6p, 1 tradeaccount@eosio.code
```

```bash
cleos get account daniel

permissions:
     owner     1:    1 EOS6kYgMTCh1iqpq9XGNQbEi8Q6k5GujefN9DSs55dcjVyFAq7B6b
        active     1:    1 EOS6kYgMTCh1iqpq9XGNQbEi8Q6k5GujefN9DSs55dcjVyFAq7B6b
```

```bash
cleos get account sarah
created: 2018-12-25T18:40:59.000
permissions:
     owner     1:    1 EOS5yd9aufDv7MqMquGcQdD6Bfmv6umqSuh9ru3kheDBqbi6vtJ58
        active     1:    1 EOS5yd9aufDv7MqMquGcQdD6Bfmv6umqSuh9ru3kheDBqbi6vtJ58
           acntrader     1:    1 EOS5yd9aufDv7MqMquGcQdD6Bfmv6umqSuh9ru3kheDBqbi6vtJ58, 1 tradeaccount@eosio.code
```

User Story:

1. A person has created an account on the eosio blockchain that they wish to sell.
In this exampled a users owns the account *daniel* and is selling the account
*account4sale*.

```bash
cleos get account account4sale
created: 2018-12-21T20:03:43.000
permissions:
     owner     1:    1 EOS8BCgapgYA2L4LJfCzekzeSr3rzgSTUXRXwNi8bNRoz31D14en9
        active     1:    1 EOS8BCgapgYA2L4LJfCzekzeSr3rzgSTUXRXwNi8bNRoz31D14en9
```

The owner of this account should fist transfer out all the tokens and assets that they want to keep before
selling the account.

`cleos push action eosio.token transfer '["account4sale","daniel","55.0000 EOS","emptying balance"] -p account4sale`

Now the owner of the *account4sale* adds `tradeaccount@eosio.code` to their active permission.
This is a special permission defined by the EOSIO software to specify that only the
contract code of the contract account (tradeaccount) will be able to act under the
permission (role) ruled by that authority. This means that the account4sale@active
authority would contain the public key that the owner of that account controls, as
well as the tradeaccount@eosio.code permission.

The account looks like this:
```bash
cleos get account account4sale
permissions:
     owner     1:    1 EOS6kYgMTCh1iqpq9XGNQbEi8Q6k5GujefN9DSs55dcjVyFAq7B6b
        active     1:    1 EOS6kYgMTCh1iqpq9XGNQbEi8Q6k5GujefN9DSs55dcjVyFAq7B6b, 1 tradeaccount@eosio.code
```

This effectively implements what you were looking for: Authorizing a contract's code to act as another account,
 but not letting the contract's account act as the other account.


2. The user can now put their account up for sale using a third account beloning to the user called
the 'seller'.  This account will receive the funds that the account sells for

ex: *daniel* will sell their account called *account4sale* for *100.0000 EOS*

`cleos push action tradeaccount sellaccount '["daniel","account4sale","100.0000 EOS"]' -p account4sale`

3. Finally, another user, *sarah*, purchases the account

`cleos push action tradeaccount buyaccount '["sarah","account4sale","NEW_PUBLIC_KEY"]' -p sarah`

---


```bash
./start_eosio_docker.sh
```

To shut down press Ctrl+C and run

```bash
./stop_eosio_docker.sh
```



# Prerequisites

Make sure Docker and Node.js are installed

-   Install Docker: https://docs.docker.com/docker-for-mac/install/
-   Install Node.js: https://nodejs.org/en/

The DApp and eosio will occupy the ports 3000, 8888 and 9876. Make sure nothing else is already running on these ports.

Clone the repository:

```sh
git clone https://github.com/liebeskind/chestnut-eos
```

The following guide assumes you are using macOS.

# Quick start - Run the DApp

**To start**

```sh
./quick_start.sh
```

The above command will execute the following in sequence:

1. `first_time_setup.sh`
2. `start_eosio_docker.sh`
3. `start_frontend.sh`

**To stop**, press `ctrl+c` on your keyboard, and execute:

```sh
docker stop eosio_securitylogic_container
```

# Detailed guide

In this section we will describe in detail each script used to run the Chestnut environment in details.

## Initial setup

```sh
./first_time_setup.sh
```

Executing the above shell script verifies that docker and node.js are installed. It then downloads the `eosio/eos-dev` docker image (which contains a full version of the eosio blockchain), removes any previous instances of this docker container and installs node packages for the frontend react app.

## Initialise and start blockchain and DApp

After the initialisation, two terminal windows are required, both opened in the repository directory

-   The **first terminal window** is for **blockchain** process.
-   The **second terminal window** is for **frontend** react app.

**running the blockchain**

For the first (blockchain) terminal window, run

```sh
./start_eosio_docker.sh
```

The log of blockchain will be displayed on your screen. eosio is now running and starts producing blocks.

**running the DApp**

For the second (frontend) terminal window, run

```sh
./start_frontend.sh
```

## Stopping blockchain or DApp

**stopping the blockchain**

In the first (blockchain) terminal window, press `ctrl+c` on your keyboard, the log will stop printing. And then execute:

```sh
docker stop eosio_securitylogic_container
```

This action will take a few seconds. The blockchain will be stopped.

**stopping the DApp**

In the second (frontend) terminal window, press `ctrl+c` on your keyboard. The frontend react app will be stopped.

## Restarting blockchain or DApp

**restarting the blockchain**

```sh
./restart.sh
```

## Reset blockchain data

First, you need to stop the blockchain (as above) and then execute:

```sh
./first_time_setup.sh
```

This removes all data on the blockchain, including accounts, deployed smart contracts, etc... The block count will be reset when you start the blockchain again.
