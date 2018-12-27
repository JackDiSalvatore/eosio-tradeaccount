# echo '=== hello world ==='
# cleos push action tradeaccount hello '[""]' -p tradeaccount

echo '==================================================================='
echo '===                    T R A D E  A C C O U N T                 ==='
echo '==================================================================='

sleep 1

echo '1. '
echo 'Account tradeaccount runs the contract which lets users buy and sell accounts'
echo 'cleos get account tradeaccount'
cleos get account tradeaccount

echo 'Account daniel is selling is second account account4sale'
echo 'cleos get account daniel'
cleos get account daniel

echo 'cleos get account account4sale'
cleos get account account4sale

echo 'sarah is going to buy account4sale from daniel'
echo 'cleos get account sarah'
cleos get account sarah

echo '-------------------------------------------------------------------'

echo "seller needs to add tradeaccount@eosio.code permission to their "
echo "active authority.  This lets the smart contract act on behalf of"
echo " the account4sale account.  This smart contract needs this "
echo "permission because will switch the keys of the account4sale to "
echo "the keys of the buyer."

# TODO: Do this with the system contract?
echo "add tradeaccount@eosio.code permission to account4sale@active"
cleos set account permission account4sale active \
'{"threshold": 1,"keys": [{"key": "EOS78RuuHNgtmDv9jwAzhxZ9LmC6F295snyQ9eUDQ5YtVHJ1udE6p","weight": 1}],"accounts": [{"permission":{"actor":"tradeaccount","permission":"eosio.code"},"weight":1}]}' \
owner -p account4sale

# echo 'cleos push action tradeaccount giveauth [""] -p account4sale'
# cleos push action tradeaccount giveauth '["account4sale","EOS8BCgapgYA2L4LJfCzekzeSr3rzgSTUXRXwNi8bNRoz31D14en9"]' -p account4sale@active

# echo 'cleos get account account4sale'
# cleos get account account4sale


# echo "=== Need this for 'giveauth' action ==="
# echo "=== give daniel to eosio.code permission ==="
# cleos set account permission daniel owner \
# '{"threshold": 1,"keys": [{"key": "EOS6kYgMTCh1iqpq9XGNQbEi8Q6k5GujefN9DSs55dcjVyFAq7B6b","weight": 1}],"accounts": [{"permission":{"actor":"tradeaccount","permission":"eosio.code"},"weight":1}]}' \
#  -p daniel@owner


echo '2. '
echo 'daniel will sell their account called account4sale for 100.0000 EOS'

echo 'before: '
echo 'cleos get table eosio.token tradeaccount accounts'
cleos get table eosio.token tradeaccount accounts
echo 'cleos get table eosio.token daniel accounts'
cleos get table eosio.token daniel accounts
echo 'cleos get table tradeaccount tradeaccount sellorders'
cleos get table tradeaccount tradeaccount sellorders

echo 'cleos push action tradeaccount sellaccount ["daniel","account4sale","100.0000 EOS"] -p account4sale'
cleos push action tradeaccount sellaccount '["daniel","account4sale","100.0000 EOS"]' -p account4sale
sleep 1

echo 'after: '
echo 'cleos get table eosio.token tradeaccount accounts'
cleos get table eosio.token tradeaccount accounts

echo 'cleos get table eosio.token daniel accounts'
cleos get table eosio.token daniel accounts

echo 'cleos get table tradeaccount tradeaccount sellorders'
cleos get table tradeaccount tradeaccount sellorders

echo 'NOTE: no EOS has been transferred yet!'

echo '3. '
echo 'sarah will buy the account from the tradaccount contract'

echo "=== add tradeaccount@eosio.code permission to sarah@active ==="
cleos set account permission sarah active \
'{"threshold": 1,"keys": [{"key": "EOS5yd9aufDv7MqMquGcQdD6Bfmv6umqSuh9ru3kheDBqbi6vtJ58","weight": 1}],"accounts": [{"permission":{"actor":"tradeaccount","permission":"eosio.code"},"weight":1}]}' \
owner -p sarah

echo 'cleos get account sarah'
cleos get account sarah

echo 'cleos push action tradeaccount buyaccount ["sarah","account4sale","EOS5yd9aufDv7MqMquGcQdD6Bfmv6umqSuh9ru3kheDBqbi6vtJ58"] -p sarah'
cleos push action tradeaccount buyaccount '["sarah","account4sale","EOS5yd9aufDv7MqMquGcQdD6Bfmv6umqSuh9ru3kheDBqbi6vtJ58"]' -p sarah
sleep 1
echo 'cleos get table tradeaccount tradeaccount sellorders'
cleos get table tradeaccount tradeaccount sellorders

echo 'cleos get account account4sale'
cleos get account account4sale
