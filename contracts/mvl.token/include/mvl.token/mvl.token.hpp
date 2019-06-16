#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>

using namespace eosio;

namespace mvl {
  using std::string;

  constexpr name active_permission {"active"_n};

  class [[eosio::contract("mvl.token")]] token : public contract {
    public:
      using contract::contract;

      [[eosio::action]]
      void create(name issuer, asset maximum_supply);

      [[eosio::action]]
      void mint(asset maximum_supply);

      [[eosio::action]]
      void burn(asset quantity, string memo);

      [[eosio::action]]
      void issue(name to, asset quantity, string memo);

      [[eosio::action]]
      void transfer(name from, name to, asset quantity, string memo);

      [[eosio::action]]
      void open(name owner, const symbol& symbol, name payer);

      [[eosio::action]]
      void close(name owner, const symbol& symbol);

      static asset get_supply( name token_contract_account, symbol_code sym_code ) {
        stats statstable( token_contract_account, sym_code.raw() );
        const auto& st = statstable.get( sym_code.raw() );
        return st.supply;
      }

      static asset get_balance( name token_contract_account, name owner, symbol_code sym_code ) {
        accounts accountstable( token_contract_account, owner.value );
        const auto& ac = accountstable.get( sym_code.raw() );
        return ac.balance;
      }

      struct [[eosio::table("accounts"), eosio::contract("mvl.token")]] account {
        asset balance;
        uint64_t primary_key()const { return balance.symbol.code().raw(); }
      };

      struct [[eosio::table("stat"), eosio::contract("mvl.token")]] currency_stats {
        asset   supply;
        asset   max_supply;
        name    issuer;

        uint64_t primary_key()const { return supply.symbol.code().raw(); }
      };

      typedef multi_index<"accounts"_n, account> accounts;
      typedef multi_index<"stat"_n, currency_stats> stats;

      void sub_balance( name owner, asset value );
      void add_balance( name owner, asset value, name ram_payer );
  };
}
