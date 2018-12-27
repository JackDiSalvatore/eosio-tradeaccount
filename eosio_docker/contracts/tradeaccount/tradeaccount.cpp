#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/public_key.hpp>

#include "abieos_numeric.hpp"

// #include "../eosio.contracts/eosio.token/include/eosio.token/eosio.token.hpp"
// #include "../eosio.contracts/eosio.token/src/eosio.token.cpp"

#define EOS_SYMBOL symbol("EOS", 4)
#define RAMCORE_SYMBOL symbol("RAMCORE", 4)
#define RAM_SYMBOL symbol("RAM", 0)


using namespace eosio;


CONTRACT tradeaccount : public eosio::contract {
  private:

    /****************************************************************************
     *                                D A T A  T Y P E S
     ***************************************************************************/

    struct permission_level_weight {
       eosio::permission_level  permission;
       uint16_t          weight;
       // explicit serialization macro is not necessary, used here only to improve compilation time
       // EOSLIB_SERIALIZE( permission_level_weight, (permission)(weight) )
    };

    struct wait_weight {
       uint32_t           wait_sec;
       uint16_t           weight;
       // explicit serialization macro is not necessary, used here only to improve compilation time
       // EOSLIB_SERIALIZE( wait_weight, (wait_sec)(weight) )
    };

   struct key_weight {
      eosio::public_key  key;
      uint16_t           weight;
      // explicit serialization macro is not necessary, used here only to improve compilation time
      // EOSLIB_SERIALIZE( key_weight, (key)(weight) )
   };

    struct authority {
       uint32_t                              threshold = 0;
       std::vector<key_weight>               keys;
       std::vector<permission_level_weight>  accounts;
       std::vector<wait_weight>              waits;
       // explicit serialization macro is not necessary, used here only to improve compilation time
       // EOSLIB_SERIALIZE( authority, (threshold)(keys)(accounts)(waits) )
    };

    /****************************************************************************
     *                                T A B L E S
     ***************************************************************************/

    TABLE sell_order {
      name       account_for_sale;
      name       seller;
      asset      price_ask{0, EOS_SYMBOL};
      time_point time_stamp;

      uint64_t primary_key() const { return account_for_sale.value; }
    };

    typedef eosio::multi_index< "sellorders"_n, sell_order > sell_orders;
    sell_orders _sell_orders;

    /****************************************************************************
     *                            F U N C T I O N S
     ***************************************************************************/

    time_point current_time_point() {
      const static time_point ct{ microseconds{ static_cast<int64_t>( current_time() ) } };
      return ct;
    }

    void create_sell_order( name  seller,
                            name  account_for_sale,
                            asset price_ask ) {
      /*sell_orders _sell_orders( _self, seller.value );*/
      eosio_assert( _sell_orders.find( account_for_sale.value ) == _sell_orders.end(),
                    "you are already selling this account" );

      /* contract account pays for ram */
      auto order = _sell_orders.emplace( _self, [&]( auto& s ) {
        s.account_for_sale = account_for_sale;
        s.seller           = seller;
        s.price_ask        = price_ask;
        s.time_stamp       = current_time_point();
      });
    }

  public:
    using contract::contract;

    // constructor
    tradeaccount( name receiver, name code, datastream<const char*> ds ):
                  contract( receiver, code, ds ),
                  _sell_orders( receiver, receiver.value ) {}

    /****************************************************************************
     *                              A C T I O N S
     ***************************************************************************/

    ACTION hello( void ) {
      print("*** hello ***\n");
      action(
         permission_level{ "sarah"_n, "active"_n },
         "eosio.token"_n,
         "transfer"_n,
         std::make_tuple( "sarah"_n, "eosio.token"_n, asset(10000, EOS_SYMBOL), std::string("payment from buyer") )
      ).send();
    }

    ACTION regaccount( ) {

    }

    ACTION sellaccount( name seller, name account_for_sale, asset price_ask ) {
      require_auth( account_for_sale );

      eosio_assert( is_account( account_for_sale ),
                    "account_for_sale does not exist");
      eosio_assert( seller != account_for_sale,
                    "cannot sell your own account" );

      eosio_assert( price_ask.is_valid(), "invalid price_ask" );
      eosio_assert( price_ask.amount > 0, "quantity must be positive" );
      eosio_assert( price_ask.symbol == EOS_SYMBOL,
                    "symbol precision mismatch" );

      create_sell_order( seller, account_for_sale, price_ask );
    }

    ACTION buyaccount( name buyer,
                       name account_to_buy,
                       std::string new_public_key ) {
      require_auth( buyer );

      eosio_assert( is_account( account_to_buy ),
                    "account_for_sale does not exist");
      eosio_assert( buyer != account_to_buy,
                    "cannot buy your own account" );

      // find account for sale / seller / price
      auto order = _sell_orders.find( account_to_buy.value );

      eosio_assert( order != _sell_orders.end(),
                    "could not find account for sale" );

      // buyer sends money (price) to seller
      print( order->seller, " is selling '", order->account_for_sale,
             "' to ", buyer, " for ", order->price_ask, "\n" );

      action(
         permission_level{ buyer, "active"_n },
         "eosio.token"_n,
         "transfer"_n,
         std::make_tuple( buyer,
                          order->seller,
                          order->price_ask,
                          std::string("buying account") )
      ).send();

      // if seller receives money
      // giveauth( order->account_for_sale, new_public_key );
      action(
         permission_level{ order->account_for_sale, "active"_n },
         get_self(),
         "giveauth"_n,
         std::make_tuple( order->account_for_sale, new_public_key )
      ).send();

      _sell_orders.erase( order );
    }

    ACTION giveauth( const name user, const std::string new_owner_pubkey ) {
      // permission
      // parent
      // auth
      print("*** giveauth ***\n");
      std::array<char, 33> owner_pubkey_char;

      const std::string owner_key_str = new_owner_pubkey.substr(0, 53);
      print("new key: ", owner_key_str, "\n");

      // create new public key
      const abieos::public_key owner_pubkey =
            abieos::string_to_public_key( owner_key_str );

      std::copy( owner_pubkey.data.begin(),
                 owner_pubkey.data.end(),
                 owner_pubkey_char.begin() );

      // create new authority
      const auto owner_auth = authority{ 1,
        {
          {/*key_weight*/
            {(uint8_t)abieos::key_type::k1, owner_pubkey_char}, 1
          }
        },
        {/*accounts*/},
        {/*waits*/}
      };

      // TODO: Change this from changing the "active" permission, to
      //       changing the "owner" permission!
      action(
         permission_level{ user, "active"_n },
         "eosio"_n,
         "updateauth"_n,
         std::make_tuple( user, "active"_n, "owner"_n, owner_auth)
      ).send();

    }

};

// specify the contract name, and export a public action: update
EOSIO_DISPATCH( tradeaccount, (hello)(regaccount)(sellaccount)(buyaccount)(giveauth) )

/*extern "C" {
  void apply( uint64_t receiver, uint64_t code, uint64_t action ) {
    // tradeaccount _accountmaker(receiver);
    //auto self = receiver;

    if( code==receiver && action== name("hello").value ) {
      execute_action( name(receiver), name(code), &tradeaccount::hello );
    }
    else if(code==receiver && action== name("deposit").value) {
      execute_action(name(receiver), name(code), &tradeaccount::deposit );
    }
    else if(code==receiver && action== name("giveauth").value) {
      execute_action(name(receiver), name(code), &tradeaccount::giveauth );
    }

  }
};
*/