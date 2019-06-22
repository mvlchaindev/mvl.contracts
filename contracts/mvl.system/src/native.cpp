#include <eosio/crypto.hpp>
#include <mvl.system/mvl.system.hpp>

namespace mvl {
  void system_contract::onblock(ignore<block_header> header) {
    require_auth(_self);
    block_timestamp timestamp;
    _ds >> timestamp;

    _gstate.last_block_num = timestamp;
  }

  void system_contract::newaccount(name creator, name newact, ignore<authority> owner, ignore<authority> active) {
    require_auth(_self);

    if (creator != _self) {
      // check dot in name
      uint64_t tmp = newact.value >> 4;
      bool has_dot = false;
      for( uint32_t i = 0; i < 12; ++i ) {
        has_dot |= !(tmp & 0x1f);
        tmp >>= 5;
      }
      check(newact.length() >= 6, "account name length should be greaterr than 6");
      check(!has_dot, "user account name cannot contain dot");

      user_resources_table userres(_self, newact.value);

      userres.emplace(newact, [&](auto& res) {
        res.owner = newact;
        res.net_weight = asset(0, system_contract::get_core_symbol());
        res.cpu_weight = asset(0, system_contract::get_core_symbol());
      });

      eosio::set_resource_limits(newact, 0, 0, 0);
    }
  }

  void system_contract::setabi(name account, const std::vector<char>& abi) {
    eosio::multi_index<"abihash"_n, abi_hash> table(_self, _self.value);
    auto itr = table.find(account.value);
    if (itr == table.end()) {
      table.emplace(account, [&](auto& row) {
        row.owner = account;
        auto hash = sha256(const_cast<char*>(abi.data()), abi.size()).extract_as_byte_array();
        std::copy(hash.begin(), hash.end(), row.hash.begin());
      });
    } else {
      table.modify(itr, same_payer, [&](auto& row) {
        auto hash = sha256(const_cast<char*>(abi.data()), abi.size()).extract_as_byte_array();
        std::copy(hash.begin(), hash.end(), row.hash.begin());
      });
    }
  }

}
