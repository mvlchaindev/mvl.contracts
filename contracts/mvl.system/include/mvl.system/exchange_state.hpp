#pragma once

#include <eosio/asset.hpp>

namespace mvl {
  using eosio::asset;
  using eosio::symbol;

  typedef double real_type;

  /**
   * Uses Bancor math to create a 50/50 relay between two asset types. The state of the
   * bancor exchange is entirely contained within this struct. There are no external
   * side effects associated with using this API.
   */

  struct [[eosio::table, eosio::contract("mvl.system")]] exchange_state {
    asset    supply;
    struct connector {
      asset balance;
      double weight = .5;
      EOSLIB_SERIALIZE( connector, (balance)(weight) )
    };

    connector base;
    connector quote;

    uint64_t primary_key()const { return supply.symbol.raw(); }

    asset convert_to_exchange( connector& c, asset in );
    asset convert_from_exchange( connector& c, asset in );
    asset convert( asset from, const symbol& to );

    EOSLIB_SERIALIZE( exchange_state, (supply)(base)(quote) )
  };

  typedef eosio::multi_index< "rammarket"_n, exchange_state > rammarket;
}
