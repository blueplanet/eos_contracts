#include <eosiolib/eosio.hpp>

using std::string;
using namespace eosio;

class [[eosio::contract]] addressbook_history : public eosio::contract {
  public:
    addressbook_history(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds){}

    [[eosio::action]]
    void create(name user, uint64_t addressbook_id, string history_type, string history_content) {
      require_auth(name{"eosbookaddrs"});

      history_index histories(_code, _code.value);

      histories.emplace(_code, [&](auto &row) {
        row.id = histories.available_primary_key();
        row.addressbook_id = addressbook_id;
        row.user = user.to_string();
        row.history_type = history_type;
        row.history_content = history_content;
      });
    }

    [[eosio::action]]
    void destroy(uint64_t id) {
      require_auth(name{"eosbookaddrs"});

      history_index histories(_code, _code.value);
      auto iterator = histories.find(id);
      eosio_assert(iterator != histories.end(), "Record does not exist");

      histories.erase(iterator);
    }

  private:
    struct [[eosio::table]] history
    {
      uint64_t id;
      uint64_t addressbook_id;
      string user;
      string history_type;
      string history_content;

      auto primary_key() const { return id; }
    };

    typedef eosio::multi_index<"histories"_n, history> history_index;
};

EOSIO_DISPATCH(addressbook_history, (create)(destroy))
