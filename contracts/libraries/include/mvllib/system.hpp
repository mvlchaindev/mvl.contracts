#pragma once
#include <eosio/symbol.hpp>

namespace mvl {
  using eosio::name;
  using eosio::symbol;

  constexpr name system_account = "mvl"_n;
  constexpr name token_account = "mvl.token"_n;

  constexpr name owner_permission = "owner"_n;
  constexpr name active_permission = "active"_n;
  constexpr name code_permission = "mvl.code"_n;

  constexpr symbol core_symbol = symbol("MVL", 4);
}
