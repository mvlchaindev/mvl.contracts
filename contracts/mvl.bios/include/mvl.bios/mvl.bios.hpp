#pragma once

#include <eosio/action.hpp>
#include <eosio/crypto.hpp>
#include <eosio/eosio.hpp>
#include <eosio/privileged.hpp>
#include <eosio/producer_schedule.hpp>

#include <mvllib/chain_types.hpp>

using namespace eosio;

namespace mvl {
  using eosio::permission_level;
  using eosio::public_key;
  using eosio::ignore;

  class [[eosio::contract("mvl.bios")]] bios : public contract {
    public:
      using contract::contract;

      [[eosio::action]]
      void newaccount( name             creator,
                       name             name,
                       ignore<authority> owner,
                       ignore<authority> active){}


      [[eosio::action]]
      void updateauth(  ignore<name>  account,
                        ignore<name>  permission,
                        ignore<name>  parent,
                        ignore<authority> auth ) {}

      [[eosio::action]]
      void deleteauth( ignore<name>  account,
                       ignore<name>  permission ) {}

      [[eosio::action]]
      void linkauth(  ignore<name>    account,
                      ignore<name>    code,
                      ignore<name>    type,
                      ignore<name>    requirement  ) {}

      [[eosio::action]]
      void unlinkauth( ignore<name>  account,
                       ignore<name>  code,
                       ignore<name>  type ) {}


      [[eosio::action]]
      void canceldelay( ignore<permission_level> canceling_auth, ignore<capi_checksum256> trx_id ) {}

      [[eosio::action]]
      void onerror( ignore<uint128_t> sender_id, ignore<std::vector<char>> sent_trx ) {}

      [[eosio::action]]
      void setcode( name account, uint8_t vmtype, uint8_t vmversion, const std::vector<char>& code ) {}

      [[eosio::action]]
      void setpriv( name account, uint8_t is_priv ) {
        require_auth( _self );
        set_privileged( account, is_priv );
      }

      [[eosio::action]]
      void setalimits( name account, int64_t ram_bytes, int64_t net_weight, int64_t cpu_weight ) {
        require_auth( _self );
        set_resource_limits( account, ram_bytes, net_weight, cpu_weight );
      }

      [[eosio::action]]
      void setglimits( uint64_t ram, uint64_t net, uint64_t cpu ) {
        (void)ram; (void)net; (void)cpu;
        require_auth( _self );
      }

      [[eosio::action]]
      void setprods( std::vector<eosio::producer_key> schedule ) {
        (void)schedule; // schedule argument just forces the deserialization of the action data into vector<producer_key> (necessary check)
        require_auth( _self );
        set_proposed_producers(schedule);
      }

      [[eosio::action]]
      void setparams( const eosio::blockchain_parameters& params ) {
        require_auth( _self );
        set_blockchain_parameters( params );
      }

      [[eosio::action]]
      void reqauth( name from ) {
        require_auth( from );
      }

      struct [[eosio::table]] abi_hash {
        name              owner;
        capi_checksum256  hash;
        uint64_t primary_key()const { return owner.value; }

        EOSLIB_SERIALIZE( abi_hash, (owner)(hash) )
      };

      using abi_hash_table = eosio::multi_index< "abihash"_n, abi_hash >;

      [[eosio::action]]
      void setabi( name account, const std::vector<char>& abi ) {
        abi_hash_table table(_self, _self.value);
        auto itr = table.find( account.value );
        if( itr == table.end() ) {
          table.emplace( account, [&]( auto& row ) {
            row.owner = account;
            auto hash = sha256( const_cast<char*>(abi.data()), abi.size() ).extract_as_byte_array();
            std::copy(hash.begin(), hash.end(), row.hash.begin());
          });
        } else {
          table.modify( itr, same_payer, [&]( auto& row ) {
            auto hash = sha256( const_cast<char*>(abi.data()), abi.size() ).extract_as_byte_array();
            std::copy(hash.begin(), hash.end(), row.hash.begin());
          });
        }
      }

      using newaccount_action = action_wrapper<"newaccount"_n, &bios::newaccount>;
      using updateauth_action = action_wrapper<"updateauth"_n, &bios::updateauth>;
      using deleteauth_action = action_wrapper<"deleteauth"_n, &bios::deleteauth>;
      using linkauth_action = action_wrapper<"linkauth"_n, &bios::linkauth>;
      using unlinkauth_action = action_wrapper<"unlinkauth"_n, &bios::unlinkauth>;
      using canceldelay_action = action_wrapper<"canceldelay"_n, &bios::canceldelay>;
      using setcode_action = action_wrapper<"setcode"_n, &bios::setcode>;
      using setpriv_action = action_wrapper<"setpriv"_n, &bios::setpriv>;
      using setalimits_action = action_wrapper<"setalimits"_n, &bios::setalimits>;
      using setglimits_action = action_wrapper<"setglimits"_n, &bios::setglimits>;
      using setprods_action = action_wrapper<"setprods"_n, &bios::setprods>;
      using setparams_action = action_wrapper<"setparams"_n, &bios::setparams>;
      using reqauth_action = action_wrapper<"reqauth"_n, &bios::reqauth>;
      using setabi_action = action_wrapper<"setabi"_n, &bios::setabi>;
  };
} /// namespace mvl

