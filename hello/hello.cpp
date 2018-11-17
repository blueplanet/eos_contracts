#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] addressbook : public eosio::contract {
  public:
    void create(name user, string name, string address, string tel) {
      require_auth(user);

      address_index addresses(_self, _self);

      addresses.emplace(user, [&]( auto& row ) {
        row.id = addresses.available_primary_key();
        row.name = name;
        row.address = address;
        row.tel = tel;
      });
    }



  private:
    struct person {
      uint64_t id;
      string name;
      string address;
      string tel;

      auto primary_key() const { return id; }
    };

    typedef eosio::multi_index<"people"_n, person> address_index;
};
