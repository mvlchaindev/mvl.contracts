#include <mvl.system/mvl.system.hpp>
#include "delegate_bandwidth.cpp"
#include "exchange_state.cpp"
#include "native.cpp"

namespace mvl {
  system_contract::system_contract( name s, name code, datastream<const char*> ds )
  : contract(s, code, ds),
  _rammarket(_self, _self.value),
  _global(_self, _self.value) {
    _gstate = _global.exists() ? _global.get() : get_default_parameters();
  }

  system_contract::~system_contract() {
    _global.set(_gstate, _self);
  }

  mvl_global_state system_contract::get_default_parameters() {
    mvl_global_state gs;
    get_blockchain_parameters(gs);
    return gs;
  }

  symbol system_contract::core_symbol()const {
    const static auto sym = get_core_symbol( _rammarket );
    return sym;
  }

  void system_contract::setram( uint64_t max_ram_size ) {
    require_auth( _self );

    check( _gstate.max_ram_size < max_ram_size, "ram may only be increased" );
    check( max_ram_size < 1024ll*1024*1024*1024*1024, "ram size is unrealistic" );
    check( max_ram_size >  _gstate.total_ram_bytes_reserved, "attempt to set max below reserved" );

    auto delta = int64_t(max_ram_size) - int64_t(_gstate.max_ram_size);
    auto itr = _rammarket.find(ramcore_symbol.raw());

    _rammarket.modify( itr, same_payer, [&]( auto& m ) {
        m.base.balance.amount += delta;
    });

    _gstate.max_ram_size = max_ram_size;
  }

  void system_contract::update_ram_supply() {
    auto cbt = current_block_time();

    if (cbt <= _gstate.last_ram_increase) {
      return;
    }

    auto itr = _rammarket.find(ramcore_symbol.raw());
    auto new_ram = (cbt.slot - _gstate.last_ram_increase.slot)*_gstate.new_ram_per_block;
    _gstate.max_ram_size += new_ram;

    _rammarket.modify( itr, same_payer, [&]( auto& m ) {
        m.base.balance.amount += new_ram;
    });
    _gstate.last_ram_increase = cbt;
  }

  void system_contract::setramrate( uint16_t bytes_per_block ) {
    require_auth( _self );
    update_ram_supply();
    _gstate.new_ram_per_block = bytes_per_block;
  }

  void system_contract::setparams( const mvl::blockchain_parameters& params ) {
    require_auth( _self );
    (mvl::blockchain_parameters&)(_gstate) = params;
    check( 3 <= _gstate.max_authority_depth, "max_authority_depth should be at least 3" );
    set_blockchain_parameters( params );
  }

  void system_contract::setpriv( name account, uint8_t ispriv ) {
    require_auth( _self );
    eosio::set_privileged( account, ispriv );
  }


  void system_contract::setalimits( name account, int64_t ram, int64_t net, int64_t cpu ) {
    require_auth( _self );
    user_resources_table userres( _self, account.value );
    auto ritr = userres.find( account.value );
    check( ritr == userres.end(), "only supports unlimited accounts" );
    eosio::set_resource_limits( account, ram, net, cpu );
  }

  void system_contract::init( unsigned_int version, symbol core ) {
    require_auth( _self );
    check( version.value == 0, "unsupported version for init action" );

    auto itr = _rammarket.find(ramcore_symbol.raw());
    check( itr == _rammarket.end(), "system contract has already been initialized" );

    auto system_token_supply = token::get_supply(_self, core.code());
    check( system_token_supply.symbol == core, "specified core symbol does not exist (percision mismatch)" );
    check(system_token_supply.amount >= 0, "system token supply must be greater than 0");

    _rammarket.emplace( _self, [&]( auto& m ) {
      m.supply.amount = 100'000'000'000'000ll;
      m.supply.symbol = ramcore_symbol;
      m.base.balance.amount = int64_t(_gstate.free_ram());
      m.base.balance.symbol = ram_symbol;
      m.quote.balance.amount = system_token_supply.amount / 1000;
      m.quote.balance.symbol = core;
    });
  }

  void system_contract::genaccount(name creator, name name, authority owner, authority active) {
    require_auth(creator);
    //action({{_self, active_permission}}, user_account, "setnick"_n, std::make_tuple(name, nickname)).send();
    newaccount_action(_self, {{creator, active_permission},{_self, active_permission}}).send(creator, name, owner, active);
  }
}

